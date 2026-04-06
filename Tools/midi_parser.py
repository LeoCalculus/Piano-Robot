#!/usr/bin/env python3
"""
MIDI -> Piano-Robot Song File Parser
====================================

Fetches a MIDI file (from bitmidi.com or any URL / local path), parses it, and
emits a song file consumable by the real robot firmware (see sd.c:sd_parse_array
and command.c:traversal).

Output format (one line per chord event, space separated, 15 tokens):
    lpos rpos s0 s1 s2 s3 s4 s5 s6 s7 s8 s9 dur keep_l keep_r
           ^ left hand anchor (mm)      ^ right hand anchor (mm)
           s0..s6 = left fingers, s7..s9 = right fingers
           dur    = ms to hold chord
           keep_l / keep_r = long-press flags (unused here, always 0)

Geometry
--------
Piano: C2..F6 (32 white + 22 black = 54 keys) at 21 mm white-key spacing.
Hand finger layouts are scaled from Tools/piano_simulator.py from the
simulator's 23 mm reference to the real 21 mm case.

Hand collision
--------------
Fingers physically span a fixed region around each anchor:
    Left  extents: lpos + 0 .. lpos + 105 mm  (F0..F6)
    Right extents: rpos - 42 .. rpos + 0 mm   (F0..F2, anchor = F2 rightmost)
We require the gap between the rightmost left finger and leftmost right
finger to be >= 21 mm:
    rpos - 42 - (lpos + 105) >= 21   =>   rpos - lpos >= 168 mm

Simplification
--------------
We emit monophonic-per-hand events: at most one left-hand note and one
right-hand note per chord. Split is at MIDI 60 (C4) by default. This keeps
the scheduler simple and avoids figuring out whether arbitrary chord voicings
match the fixed finger geometry.

Usage
-----
    python midi_parser.py <query-or-url> [-o sd-card-file/<name>.txt]

Examples:
    python midi_parser.py "fur elise"
    python midi_parser.py https://bitmidi.com/uploads/12345.mid
    python midi_parser.py ./local.mid
"""
from __future__ import annotations

import argparse
import os
import re
import sys
import urllib.error
import urllib.parse
import urllib.request
from dataclasses import dataclass, field
from typing import List, Optional, Tuple

# ── Geometry (matches Tools/piano_simulator.py: 21 mm pitch, mm units) ───────
# All positions below are in mm in the simulator's absolute piano frame
# (0 = left edge of the leftmost white key, PIANO_MM = right edge of C6-block).
PIANO_FIRST_MIDI = 36   # C2
PIANO_LAST_MIDI  = 89   # F6

PIANO_MM  = 672.0            # 32 × 21 mm
WHITE_MM  = 21.0
HALF_WW   = 10.5             # = WHITE_MM / 2  (simulator parse_song offset)
RPOS_HOME = 661.5            # simulator's right anchor at F6 centre = 31.5 × 21
# Simulator file format convention (parse_song in piano_simulator.py:72-85):
#     lpos_actual_mm = file_value + HALF_WW
#     rpos_actual_mm = RPOS_HOME - file_value

# Finger layout — COPIED VERBATIM from piano_simulator.py:46-59 so our
# placement math can't drift from what the simulator validates.
LEFT_FINGERS = [
    ('W',   0.0),  # F0  W
    ('B',  33.8),  # F1  B  (~D# relative to F0-on-C)
    ('W',  42.0),  # F2  W
    ('W',  63.0),  # F3  W
    ('W',  84.0),  # F4  W
    ('B',  94.5),  # F5  B  (~G#)
    ('W', 105.0),  # F6  W
]
RIGHT_FINGERS = [
    ('W', 42.0),   # F0 (solenoid 7)  W  — 42 mm LEFT of anchor
    ('B', 11.0),   # F1 (solenoid 8)  B  — 11 mm LEFT of anchor
    ('W',  0.0),   # F2 (solenoid 9)  W  — anchor (rightmost)
]
FWIDTH = {'W': 7.0, 'B': 10.0}

# Key-centre lookup in the simulator's frame.
# Whites at centres HALF_WW, HALF_WW+21, ... (simulator build_keys uses left-
# edge x = i*WW, so centre = i*WW + HALF_WW).
# Blacks are computed with SP2 = 25.6, SP3 = 23.7 per piano_simulator.py:183
# (same formula as build_keys, evaluated symbolically in mm).
_SP2, _SP3 = 25.6, 23.7
_BW_MM     = 12.3
def _octave_black_centres(x0: float) -> dict:
    o2 = x0 + (3*WHITE_MM - 2*_SP2) / 2.0
    cs = o2 + 0*_SP2 + (_SP2 - _BW_MM)/2 + _BW_MM/2     # C#
    ds = o2 + 1*_SP2 + (_SP2 - _BW_MM)/2 + _BW_MM/2     # D#
    o3 = x0 + 3*WHITE_MM + (4*WHITE_MM - 3*_SP3) / 2.0
    fs = o3 + 0*_SP3 + (_SP3 - _BW_MM)/2 + _BW_MM/2     # F#
    gs = o3 + 1*_SP3 + (_SP3 - _BW_MM)/2 + _BW_MM/2     # G#
    as_ = o3 + 2*_SP3 + (_SP3 - _BW_MM)/2 + _BW_MM/2    # A#
    return {1: cs, 3: ds, 6: fs, 8: gs, 10: as_}

_WHITE_INDEX_IN_OCTAVE = {0:0, 2:1, 4:2, 5:3, 7:4, 9:5, 11:6}

def midi_to_mm(midi: int) -> Tuple[float, bool]:
    """Return (piano_mm_absolute_centre, is_black) matching the simulator's key layout."""
    if midi < PIANO_FIRST_MIDI or midi > PIANO_LAST_MIDI:
        raise ValueError(f"MIDI {midi} out of piano range C2..F6")
    octave = (midi // 12) - 1
    semi   = midi % 12
    x0     = (octave - 2) * 7 * WHITE_MM          # left edge of this octave block
    if semi in _WHITE_INDEX_IN_OCTAVE:
        wi = _WHITE_INDEX_IN_OCTAVE[semi]
        return x0 + wi * WHITE_MM + HALF_WW, False
    return _octave_black_centres(x0)[semi], True

def place_left_options(midi: int) -> List[Tuple[float, int]]:
    """Return all valid (lpos_actual_mm, solenoid_index) for each finger that
    matches the note's colour (white or black)."""
    note_mm, is_black = midi_to_mm(midi)
    colour = 'B' if is_black else 'W'
    opts = []
    for i, (fc, off) in enumerate(LEFT_FINGERS):
        if fc == colour:
            lpos = note_mm - off
            # anchor must keep all fingers on piano
            if lpos >= 0 and lpos + LEFT_FINGERS[-1][1] <= PIANO_MM:
                opts.append((lpos, i))
    return opts

def place_right_options(midi: int) -> List[Tuple[float, int]]:
    """Return all valid (rpos_actual_mm, solenoid_index) for each finger that
    matches the note's colour."""
    note_mm, is_black = midi_to_mm(midi)
    colour = 'B' if is_black else 'W'
    opts = []
    for i, (fc, off) in enumerate(RIGHT_FINGERS):
        if fc == colour:
            rpos = note_mm + off
            if rpos - RIGHT_FINGERS[0][1] >= 0 and rpos <= PIANO_MM:
                opts.append((rpos, 7 + i))
    return opts

# ── Collision rule (mirrors piano_simulator._get_warnings) ───────────────────
# Simulator flags collision when  lpos + LEFT_FINGERS[-1][1] >= rpos - RIGHT_FINGERS[0][1].
# LEFT_FINGERS[-1][1] = 105, RIGHT_FINGERS[0][1] = 42, so the rightmost left
# finger and leftmost right finger touch at rpos - lpos = 147 mm. The hand
# bodies carry extra mechanical parts beyond the finger centres, so the
# physical margin is ~2 white keys (42 mm).
COLLIDE_MIN_GAP_MM = 2 * WHITE_MM     # 42 mm  (~2 white keys)
MIN_RPOS_MINUS_LPOS = (LEFT_FINGERS[-1][1] + RIGHT_FINGERS[0][1]) + COLLIDE_MIN_GAP_MM  # 189

# ── Long-move limit ──────────────────────────────────────────────────────────
# A single commanded motion of more than half the piano can overrun the
# controller's setpoint and crash the system. Any hand transition larger
# than this gets a midpoint waypoint inserted (fingers up, half-way position).
LONG_MOVE_MM = PIANO_MM / 2.0         # 336 mm

# ── MIDI decoding ────────────────────────────────────────────────────────────
@dataclass
class MidiNote:
    start_s: float
    dur_s:   float
    pitch:   int
    track:   int = 0

def load_mido():
    try:
        import mido  # type: ignore
        return mido
    except ImportError:
        sys.stderr.write(
            "ERROR: the 'mido' package is required.\n"
            "       install with:  pip install mido\n")
        sys.exit(2)

def parse_midi_file(path: str) -> List[MidiNote]:
    mido = load_mido()
    mid = mido.MidiFile(path)
    notes: List[MidiNote] = []
    # Use mid.tracks with accumulated ticks per track, then tick->seconds via tempo map.
    tempo = 500000  # default 120 bpm
    tpb = mid.ticks_per_beat or 480
    # Build tempo map from merged absolute-tick track
    merged = []
    for ti, track in enumerate(mid.tracks):
        abs_tick = 0
        for msg in track:
            abs_tick += msg.time
            merged.append((abs_tick, ti, msg))
    merged.sort(key=lambda r: (r[0], 0 if r[2].type == 'set_tempo' else 1))

    # Convert tick to seconds while walking in order (respecting tempo changes)
    active: dict = {}  # (track, pitch) -> (start_s, start_tick)
    cur_tempo = tempo
    last_tick = 0
    cur_seconds = 0.0
    for abs_tick, ti, msg in merged:
        # advance time from last_tick to abs_tick using cur_tempo
        delta_tick = abs_tick - last_tick
        if delta_tick > 0:
            cur_seconds += (delta_tick / tpb) * (cur_tempo / 1_000_000.0)
            last_tick = abs_tick
        if msg.type == 'set_tempo':
            cur_tempo = msg.tempo
        elif msg.type == 'note_on' and msg.velocity > 0:
            active[(ti, msg.note)] = cur_seconds
        elif msg.type == 'note_off' or (msg.type == 'note_on' and msg.velocity == 0):
            key = (ti, msg.note)
            if key in active:
                start = active.pop(key)
                dur = max(cur_seconds - start, 0.02)
                notes.append(MidiNote(start_s=start, dur_s=dur, pitch=msg.note, track=ti))
    notes.sort(key=lambda n: n.start_s)
    return notes

# ── Splitting + event construction ───────────────────────────────────────────
SPLIT_MIDI = 60   # C4 -> right

@dataclass
class Event:
    t_s:     float            # absolute start time
    dur_ms:  int
    lpos:    float = 138.0
    rpos:    float = 306.0
    sols:    List[int] = field(default_factory=lambda: [0]*10)

PREV_LPOS = 1000.0  # "far" sentinel for when left is idle
PREV_RPOS = 1000.0

def _try_left(pitch: int, last_lpos: float, last_rpos: float):
    """Try to play `pitch` with the left hand. Picks the finger that minimises
    total hand motion (anchor move + any shove of the right hand).
    Returns (lpos, sol, new_rpos) or None."""
    try:
        opts = place_left_options(pitch)
    except ValueError:
        return None
    if not opts:
        return None
    best = None
    best_cost = float('inf')
    for lpos, sol in opts:
        new_rpos = last_rpos
        if new_rpos - lpos < MIN_RPOS_MINUS_LPOS:
            new_rpos = lpos + MIN_RPOS_MINUS_LPOS
            if new_rpos > PIANO_MM:
                continue
        cost = abs(lpos - last_lpos) + abs(new_rpos - last_rpos)
        if cost < best_cost:
            best_cost = cost
            best = (lpos, sol, new_rpos)
    return best

def _try_right(pitch: int, last_lpos: float, last_rpos: float):
    """Mirror of _try_left for right hand. Returns (rpos, sol, new_lpos) or None."""
    try:
        opts = place_right_options(pitch)
    except ValueError:
        return None
    if not opts:
        return None
    best = None
    best_cost = float('inf')
    for rpos, sol in opts:
        new_lpos = last_lpos
        if rpos - new_lpos < MIN_RPOS_MINUS_LPOS:
            new_lpos = rpos - MIN_RPOS_MINUS_LPOS
            if new_lpos < 0:
                continue
        cost = abs(rpos - last_rpos) + abs(new_lpos - last_lpos)
        if cost < best_cost:
            best_cost = cost
            best = (rpos, sol, new_lpos)
    return best

def build_events(notes: List[MidiNote],
                 split: Optional[int] = None) -> Tuple[List[Event], List[str]]:
    """Group simultaneous notes into chord events and balance them across both
    hands. For each note, prefer whichever hand is physically closer to the
    target (minimising per-step motion) while honouring the 189 mm collision
    margin. `split` (if given) forces a static pitch split instead.
    Positions in returned Events are in the simulator's absolute mm frame."""
    warnings: List[str] = []

    GROUP_EPS = 0.008
    notes = sorted(notes, key=lambda n: n.start_s)
    groups: List[List[MidiNote]] = []
    for n in notes:
        if groups and abs(n.start_s - groups[-1][0].start_s) <= GROUP_EPS:
            groups[-1].append(n)
        else:
            groups.append([n])

    events: List[Event] = []
    last_lpos = HALF_WW            # F0 over C2 centre
    last_rpos = RPOS_HOME          # F2 over F6 centre

    for g in groups:
        pitches = sorted({n.pitch for n in g})
        ev = Event(t_s=g[0].start_s, dur_ms=max(60, int(round(g[0].dur_s*1000))))
        ev.lpos, ev.rpos = last_lpos, last_rpos
        if len(pitches) >= 2:
            # Chord: lowest pitch → left, highest → right (natural hand-shape).
            # Drop middle notes (monophonic per hand).
            low, high = pitches[0], pitches[-1]
            l = _try_left(low, last_lpos, last_rpos)
            if l is None:
                warnings.append(f"t={ev.t_s:6.2f}s: cannot place low={low} on left")
            else:
                ev.lpos, sol, last_rpos = l[0], l[1], l[2]
                ev.sols[sol] = 1
                last_lpos = ev.lpos
                ev.rpos = last_rpos
            # Right placed after left has (possibly) moved
            r = _try_right(high, last_lpos, last_rpos)
            if r is not None:
                ev.rpos, sol, last_lpos = r[0], r[1], r[2]
                ev.sols[sol] = 1
                last_rpos = ev.rpos
                ev.lpos = last_lpos
            dropped = len(pitches) - (1 if l else 0) - (1 if r else 0)
            if dropped > 0:
                warnings.append(f"t={ev.t_s:6.2f}s: dropped {dropped} chord note(s)")

        elif len(pitches) == 1:
            pitch = pitches[0]
            if split is not None:
                prefer_left = pitch < split
            else:
                try:
                    note_mm, _ = midi_to_mm(pitch)
                except ValueError:
                    return [], [f"Song unplayable: MIDI {pitch} out of piano range"]
                midpoint = (last_lpos + last_rpos) / 2.0
                prefer_left = note_mm < midpoint

            l = _try_left (pitch, last_lpos, last_rpos)
            r = _try_right(pitch, last_lpos, last_rpos)

            def apply_left(res):
                nonlocal last_lpos, last_rpos
                ev.lpos, sol, last_rpos = res[0], res[1], res[2]
                ev.sols[sol] = 1
                last_lpos = ev.lpos
                ev.rpos = last_rpos

            def apply_right(res):
                nonlocal last_lpos, last_rpos
                ev.rpos, sol, last_lpos = res[0], res[1], res[2]
                ev.sols[sol] = 1
                last_rpos = ev.rpos
                ev.lpos = last_lpos

            if l is not None and r is not None:
                # Total hand-motion cost (includes any forced shove on the other hand)
                l_cost = abs(l[0] - last_lpos) + abs(l[2] - last_rpos)
                r_cost = abs(r[0] - last_rpos) + abs(r[2] - last_lpos)
                if prefer_left:
                    pick_l = l_cost <= r_cost * 1.5
                else:
                    pick_l = l_cost * 1.5 < r_cost
                apply_left(l) if pick_l else apply_right(r)
            elif l is not None:
                apply_left(l)
            elif r is not None:
                apply_right(r)
            else:
                warnings.append(f"t={ev.t_s:6.2f}s: dropped note (neither hand can reach MIDI {pitch})")

        events.append(ev)

    return events, warnings

def insert_long_move_transits(events: List[Event]) -> Tuple[List[Event], int]:
    """Insert a midpoint waypoint before any event whose lpos or rpos delta
    from the previous event exceeds LONG_MOVE_MM. Waypoint has all solenoids
    off; it consumes the first half of the time gap before the target event.
    Returns (new_events, number_of_transits_inserted)."""
    if not events:
        return events, 0
    out: List[Event] = [events[0]]
    inserted = 0
    last_l = events[0].lpos
    last_r = events[0].rpos
    last_t = events[0].t_s
    for ev in events[1:]:
        dl = abs(ev.lpos - last_l)
        dr = abs(ev.rpos - last_r)
        if dl > LONG_MOVE_MM or dr > LONG_MOVE_MM:
            # Waypoint needs enough dwell for the motor to reach the midpoint
            # AND settle before the next commanded target. 150 ms minimum.
            TRANSIT_DWELL_S = 0.15
            mid_t = (last_t + ev.t_s) / 2.0
            if ev.t_s - mid_t < TRANSIT_DWELL_S:
                mid_t = max(last_t, ev.t_s - TRANSIT_DWELL_S)
            mid = Event(
                t_s=mid_t,
                dur_ms=150,
                lpos=(last_l + ev.lpos) / 2.0,
                rpos=(last_r + ev.rpos) / 2.0,
                sols=[0]*10,
            )
            out.append(mid)
            inserted += 1
        out.append(ev)
        last_l, last_r, last_t = ev.lpos, ev.rpos, ev.t_s
    return out, inserted

def emit_lines(events: List[Event]) -> List[str]:
    """Write simulator-format lines (mm units, 15 tokens).

    File layout per piano_simulator.parse_song:
        lpos_file = lpos_actual_mm - HALF_WW
        rpos_file = RPOS_HOME      - rpos_actual_mm
    """
    lines: List[str] = []
    for i, ev in enumerate(events):
        if i + 1 < len(events):
            gap_ms = int(round((events[i+1].t_s - ev.t_s) * 1000))
        else:
            gap_ms = ev.dur_ms
        dur = max(40, min(ev.dur_ms, gap_ms))

        lv = ev.lpos - HALF_WW
        rv = RPOS_HOME - ev.rpos
        sols_str = ' '.join(str(x) for x in ev.sols)
        lines.append(f"{lv:.1f} {rv:.1f} {sols_str} {dur} 0 0")
    return lines

# ── Bitmidi fetch ────────────────────────────────────────────────────────────
BITMIDI_BASE = "https://bitmidi.com"
USER_AGENT = "Mozilla/5.0 (MidiParser/1.0)"

def http_get(url: str, accept: str = "*/*") -> bytes:
    req = urllib.request.Request(url, headers={
        "User-Agent": USER_AGENT,
        "Accept": accept,
    })
    with urllib.request.urlopen(req, timeout=20) as r:
        return r.read()

def bitmidi_search(query: str) -> List[str]:
    """Scrape the bitmidi search HTML and return a list of absolute song-page URLs."""
    url = f"{BITMIDI_BASE}/search?q={urllib.parse.quote(query)}"
    html = http_get(url, "text/html").decode("utf-8", "ignore")
    # Song links look like  href="/fur-elise-mid"  or  /song-name-mid-1
    paths = re.findall(r'href="(/[a-zA-Z0-9][^"]*-mid(?:-\d+)?)"', html)
    # De-duplicate, preserve order
    seen = set(); out = []
    for p in paths:
        if p in seen: continue
        seen.add(p); out.append(BITMIDI_BASE + p)
    return out

def bitmidi_mid_from_page(page_url: str) -> str:
    """Return the absolute .mid URL referenced by a bitmidi song page."""
    html = http_get(page_url, "text/html").decode("utf-8", "ignore")
    # Pages embed  "/uploads/NNNNN.mid"  in their JSON/props
    m = re.search(r'"(/uploads/[^"]+\.mid)"', html)
    if not m:
        m = re.search(r'https?://[^\s"\']+\.mid', html)
        if not m:
            raise RuntimeError(f"No .mid link found on {page_url}")
        return m.group(0)
    return BITMIDI_BASE + m.group(1)

def bitmidi_download(query_or_url: str) -> Tuple[str, bytes]:
    """Return (suggested_slug, midi_bytes)."""
    if query_or_url.startswith("http://") or query_or_url.startswith("https://"):
        url = query_or_url
        parsed = urllib.parse.urlparse(url)
        slug = re.sub(r"[^a-z0-9]+", "_", parsed.path.strip("/").lower()).strip("_") or "song"
        if not url.lower().endswith(".mid"):
            print(f"scraping song page  {url}")
            url = bitmidi_mid_from_page(url)
        print(f"downloading  {url}")
        data = http_get(url)
        if data[:4] != b"MThd":
            raise RuntimeError("downloaded bytes are not a MIDI file (no MThd header)")
        return slug, data

    # Plain search query -> pick first bitmidi hit
    hits = bitmidi_search(query_or_url)
    if not hits:
        raise RuntimeError(f"No bitmidi search results for '{query_or_url}'")
    print(f"found {len(hits)} bitmidi hits; using  {hits[0]}")
    page = hits[0]
    slug = re.sub(r"[^a-z0-9]+", "_",
                  urllib.parse.urlparse(page).path.strip("/").lower()).strip("_") or "song"
    mid_url = bitmidi_mid_from_page(page)
    print(f"downloading  {mid_url}")
    data = http_get(mid_url)
    if data[:4] != b"MThd":
        raise RuntimeError("downloaded bytes are not a MIDI file (no MThd header)")
    return slug, data

# ── Main ─────────────────────────────────────────────────────────────────────
def main():
    ap = argparse.ArgumentParser(description="MIDI -> piano-robot song-file parser")
    ap.add_argument("query", help="bitmidi search query, a URL, or a local .mid path")
    ap.add_argument("-o", "--output", help="output .txt path (default: sd-card-file/<slug>.txt)")
    ap.add_argument("--split", type=int, default=SPLIT_MIDI,
                    help=f"MIDI note below which notes go to LEFT hand (default {SPLIT_MIDI} = C4)")
    ap.add_argument("--dry-run", action="store_true", help="parse and report but do not write file")
    args = ap.parse_args()

    # 1. Obtain a .mid file (local path, URL, or bitmidi search)
    if os.path.isfile(args.query):
        path = args.query
        slug = os.path.splitext(os.path.basename(path))[0].lower()
        slug = re.sub(r"[^a-z0-9]+", "_", slug).strip("_") or "song"
    else:
        try:
            slug, mid_bytes = bitmidi_download(args.query)
        except Exception as e:
            print(f"ERROR fetching midi: {e}", file=sys.stderr); sys.exit(1)
        cache_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), "_midi_cache")
        os.makedirs(cache_dir, exist_ok=True)
        path = os.path.join(cache_dir, f"{slug}.mid")
        with open(path, "wb") as f: f.write(mid_bytes)
        print(f"cached -> {path}")

    # 2. Parse notes
    notes = parse_midi_file(path)
    if not notes:
        print("ERROR: no notes found in MIDI file", file=sys.stderr); sys.exit(1)

    # 3. Range check BEFORE building events (stop early if song doesn't fit)
    lo = min(n.pitch for n in notes); hi = max(n.pitch for n in notes)
    if lo < PIANO_FIRST_MIDI or hi > PIANO_LAST_MIDI:
        out_of_range = [n for n in notes if n.pitch < PIANO_FIRST_MIDI or n.pitch > PIANO_LAST_MIDI]
        frac = len(out_of_range) / len(notes)
        print(f"Song range: MIDI {lo}..{hi}  (piano is {PIANO_FIRST_MIDI}..{PIANO_LAST_MIDI})")
        print(f"  {len(out_of_range)}/{len(notes)} notes ({frac*100:.1f}%) out of range")
        if frac > 0.05:
            print("SONG NOT FIT FOR THIS PIANO (C2..F6). Aborting.", file=sys.stderr)
            sys.exit(3)
        print("  (within 5% tolerance -- clamping out-of-range notes to nearest valid note)")
        for n in notes:
            if n.pitch < PIANO_FIRST_MIDI: n.pitch += 12 * ((PIANO_FIRST_MIDI - n.pitch) // 12 + 1)
            if n.pitch > PIANO_LAST_MIDI:  n.pitch -= 12 * ((n.pitch - PIANO_LAST_MIDI) // 12 + 1)

    # 4. Build events (single 21 mm / mm geometry matching piano_simulator.py)
    events, warnings = build_events(notes, split=args.split)
    for w in warnings[:20]: print("  WARN", w)
    if len(warnings) > 20: print(f"  ... and {len(warnings)-20} more warnings")
    if not events:
        print("ERROR: no playable events produced", file=sys.stderr); sys.exit(1)

    # 4b. Split any >half-piano moves with midpoint waypoints
    events, transits = insert_long_move_transits(events)
    if transits:
        print(f"Inserted {transits} long-move midpoint waypoint(s) "
              f"(>{LONG_MOVE_MM:.0f} mm)")

    lines = emit_lines(events)
    print(f"Parsed {len(notes)} notes  ->  {len(events)} chord events")

    if args.dry_run:
        for ln in lines[:10]: print("  ", ln)
        return

    # 5. Write into sd-card-file/
    if args.output:
        out_path = args.output
    else:
        sd_dir = os.path.normpath(os.path.join(
            os.path.dirname(os.path.abspath(__file__)), "..", "sd-card-file"))
        os.makedirs(sd_dir, exist_ok=True)
        out_path = os.path.join(sd_dir, f"{slug}.txt")
    with open(out_path, "w", encoding="ascii") as f:
        f.write("\n".join(lines) + "\n")
    print(f"wrote {out_path}")

if __name__ == "__main__":
    main()
