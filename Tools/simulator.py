#!/usr/bin/env python3
"""
Piano Simulator  —  C2 to F6  (32 white + 22 black = 54 keys)

Song file format (space-separated per line):
  lpos  rpos  s0..s6  s7..s9  dur  keep_l  keep_r
  (cm)  (cm)  left 7  right 3  (ms)

Hand layouts (center-to-center from anchor = F0):
  Left  (0-6): W B W W W B W  @ 0, 3.4, 4.5, 6.6, 8.9, 9.9, 11.0 cm
  Right (7-9): W B W          @ 0, 3.4, 4.6 cm  (anchor = F0, rightmost = F2)

Initial calibration (lpos=0 / rpos=0 mechanical zero):
  Left  F0 → C2 centre  (1.15 cm from piano left)
  Right F2 → F6 centre  (72.45 cm); rpos in file = key left-edge (n×2.3), +1.15 applied on load

Physics: underdamped 2nd-order, max speed 1 m/s
  Flow: set target → animate → enter deadband → stop → press solenoid → wait → next
"""
import threading
import tkinter as tk
from tkinter import filedialog
from dataclasses import dataclass
from typing import List, Optional

# ── Piano / layout constants ──────────────────────────────────────────────────
PIANO_CM  = 73.6      # 32 × 2.3 cm
HALF_WW   = 2.3 / 2  # 1.15 cm — song file stores key left-edges (n×2.3); add to get centre
PAD      = 14
RULER_H  = 42
HAND_H   = 52

# Home display positions (cm, absolute piano coords for F0 anchor)
LPOS_HOME = 1.15       # left  F0 → C2 centre
RPOS_HOME = 72.15      # right: rpos − F2_offset(0.3) = 72.45 = F6 centre

# ── Physics constants ─────────────────────────────────────────────────────────
# Stability requirement for semi-implicit Euler:
#   det(A) = 1 − KD·dt  must be in (−1, +1)  →  KD < 2/dt = 125
#   With KD·dt < 1 (det > 0), eigenvalues are complex → smooth underdamped spiral.
#   With KD·dt > 1 (det < 0), eigenvalues are real & opposite-sign → flip-flop divergence.
#   ωₙ = √KP,  ζ = KD/(2·ωₙ).  Schur-Cohn: |tr| < 1+det  →  KP < (2−KD·dt)/dt²
KP       = 1000.0  # spring  ωₙ ≈ 31.6 rad/s   (KP_max ≈ 12375 for KD=26)
KD       = 26.0    # damping ζ  ≈ 0.41           KD·dt = 0.416 < 1  →  det = +0.584
VMAX     = 100.0   # max speed, cm/s  (= 1 m/s)
DEADBAND = 0.30    # cm  — position settled threshold
VSETTLE  = 3.0     # cm/s — velocity settled threshold
ANIM_DT  = 16      # ms  per physics frame  (~60 fps)

_SEMIS = ['C','C#','D','D#','E','F','F#','G','G#','A','A#','B']

def note_midi(name: str) -> int:
    return (int(name[-1]) + 1) * 12 + _SEMIS.index(name[:-1])

def note_freq(name: str) -> int:
    return round(440.0 * 2.0 ** ((note_midi(name) - 69) / 12.0))

# ── Hand finger definitions ───────────────────────────────────────────────────
# (type, centre-offset-cm from F0 anchor)
LEFT_FINGERS: List = [
    ('W', 0.0),   # F0 → C2 at home
    ('B', 3.6),   # F1 → D#2 approx
    ('W', 4.8),   # F2 → E2
    ('W', 6.9),   # F3 → F2-key
    ('W', 9.0),   # F4 → G2
    ('B', 10.3),   # F5 → G#2 approx
    ('W', 11.4),  # F6 → A2
]
RIGHT_FINGERS: List = [
    ('W', 4.8),   # F0 → D6 at home  (rpos − 4.8 cm)
    ('B', 3.2),   # F1 → D#6 at home (rpos − 3.5 cm)
    ('W', 0.0),   # F2 anchor → F6 at home  (rpos = F2 position)
]
FWIDTH = {'W': 0.7, 'B': 1.0}   # finger width, cm

# ── Song data ─────────────────────────────────────────────────────────────────
@dataclass
class Step:
    lpos:      float
    rpos:      float
    solenoids: List[int]
    duration:  int        # ms to hold solenoid
    keep_l:    int
    keep_r:    int

def parse_song(path: str) -> List[Step]:
    steps = []
    with open(path) as fh:
        for line in fh:
            v = line.strip().split()
            if len(v) < 15:
                continue
            steps.append(Step(
                lpos=float(v[0]) + HALF_WW, rpos=float(v[1]) + HALF_WW,
                solenoids=[int(x) for x in v[2:12]],
                duration=int(v[12]),
                keep_l=int(v[13]), keep_r=int(v[14]),
            ))
    return steps

# ── Piano key layout ──────────────────────────────────────────────────────────
def build_keys(WW, WH, BW, BH, SP2, SP3):
    whites, blacks = [], []

    def add_octave(num, x0):
        for i, p in enumerate(['C','D','E','F','G','A','B']):
            whites.append(dict(name=f"{p}{num}", x=x0+i*WW, w=WW, h=WH))
        o2 = x0 + (3*WW - 2*SP2) / 2
        for i, p in enumerate(['C#','D#']):
            blacks.append(dict(name=f"{p}{num}",
                               x=o2+i*SP2+(SP2-BW)/2, w=BW, h=BH))
        o3 = x0 + 3*WW + (4*WW - 3*SP3) / 2
        for i, p in enumerate(['F#','G#','A#']):
            blacks.append(dict(name=f"{p}{num}",
                               x=o3+i*SP3+(SP3-BW)/2, w=BW, h=BH))

    for o in range(4):
        add_octave(2+o, o*7*WW)
    x6 = 4*7*WW
    for i, p in enumerate(['C','D','E','F']):
        whites.append(dict(name=f"{p}6", x=x6+i*WW, w=WW, h=WH))
    o2 = x6 + (3*WW - 2*SP2) / 2
    for i, p in enumerate(['C#','D#']):
        blacks.append(dict(name=f"{p}6",
                           x=o2+i*SP2+(SP2-BW)/2, w=BW, h=BH))
    return whites, blacks

# ── Sound ─────────────────────────────────────────────────────────────────────
def _detect_backend() -> str:
    try:
        __import__('numpy'); __import__('pygame')
        return 'pygame'
    except ImportError:
        pass
    try:
        __import__('numpy')
        return 'wave'
    except ImportError:
        return 'silent'

_BACKEND     = _detect_backend()
_sound_cache: dict = {}   # note name → pygame.Sound  or  raw WAV bytes
_audio_ready = False

def _synth(freq: float):
    import numpy as np
    SR  = 44100
    DUR = 2.0
    t   = np.linspace(0, DUR, int(SR * DUR), endpoint=False)
    # Partials: (harmonic, amplitude, decay_rate)
    partials = [(1, 1.00, 2.2), (2, 0.45, 3.5), (3, 0.22, 5.0),
                (4, 0.10, 7.0), (5, 0.05, 9.5), (6, 0.02, 13.0)]
    sig = sum(amp * np.exp(-dec * t) * np.sin(2 * np.pi * freq * h * t)
              for h, amp, dec in partials)
    atk = int(SR * 0.004)           # 4 ms soft attack
    sig[:atk] *= np.linspace(0, 1, atk)
    sig = sig / (np.max(np.abs(sig)) + 1e-9) * 0.80
    return sig

def audio_init(note_names: list, on_ready, root_widget):
    """Synthesise and cache all notes in a background thread."""
    global _audio_ready
    if _BACKEND == 'silent':
        _audio_ready = True
        root_widget.after(0, on_ready)
        return

    def _worker():
        global _audio_ready
        import numpy as np
        if _BACKEND == 'pygame':
            import pygame
            pygame.mixer.init(44100, -16, 2, 1024)
            pygame.mixer.set_num_channels(32)
            for name in note_names:
                freq   = 440.0 * 2.0 ** ((note_midi(name) - 69) / 12.0)
                mono   = _synth(freq)
                stereo = np.column_stack([mono, mono])
                arr    = (stereo * 32767).astype(np.int16)
                _sound_cache[name] = pygame.sndarray.make_sound(arr)
        else:  # wave + winsound
            import io, wave
            for name in note_names:
                freq = 440.0 * 2.0 ** ((note_midi(name) - 69) / 12.0)
                arr  = (_synth(freq) * 32767).astype(__import__('numpy').int16)
                buf  = io.BytesIO()
                with wave.open(buf, 'wb') as wf:
                    wf.setnchannels(1)
                    wf.setsampwidth(2)
                    wf.setframerate(44100)
                    wf.writeframes(arr.tobytes())
                _sound_cache[name] = buf.getvalue()
        _audio_ready = True
        root_widget.after(0, on_ready)

    threading.Thread(target=_worker, daemon=True).start()

def audio_play(name: str, ms: int = 700):
    if not _audio_ready or name not in _sound_cache:
        return
    if _BACKEND == 'pygame':
        import pygame
        _sound_cache[name].play(maxtime=ms)
    elif _BACKEND == 'wave':
        import winsound
        wav = _sound_cache[name]
        threading.Thread(
            target=winsound.PlaySound,
            args=(wav, winsound.SND_MEMORY | winsound.SND_ASYNC),
            daemon=True).start()

# ── Keyboard map ──────────────────────────────────────────────────────────────
KB = {
    'z':'C3',  's':'C#3', 'x':'D3',  'd':'D#3', 'c':'E3',
    'v':'F3',  'g':'F#3', 'b':'G3',  'h':'G#3', 'n':'A3',
    'j':'A#3', 'm':'B3',
    'q':'C4',  '2':'C#4', 'w':'D4',  '3':'D#4', 'e':'E4',
    'r':'F4',  '5':'F#4', 't':'G4',  '6':'G#4', 'y':'A4',
    '7':'A#4', 'u':'B4',
}
KB_REV = {v: k for k, v in KB.items()}

# ── App ───────────────────────────────────────────────────────────────────────
class PianoApp:
    def __init__(self, root: tk.Tk):
        root.title("Piano Simulator  —  C2 to F6  (54 keys)")
        root.resizable(False, False)
        root.configure(bg='#0f172a')

        scr_w    = root.winfo_screenwidth()
        PX       = (scr_w - 2*PAD - 40) / PIANO_CM
        self._PX = PX

        WW  = round(2.3*PX);  WH = round(5.5*PX)
        BW  = round(1.35*PX); BH = round(3.3*PX)
        SP2 = 2.8*PX;         SP3 = 2.6*PX
        cw  = 32*WW + 2*PAD

        # ── Labels ────────────────────────────────────────────────────────────
        self.info = tk.StringVar(
            value="Click a key  ·  z–m (C3–B3)  ·  q–u (C4–B4)")
        tk.Label(root, textvariable=self.info, font=('Courier', 10),
                 bg='#0f172a', fg='#94a3b8').pack(pady=(10, 1))

        self.warn = tk.StringVar(value="")
        self._warn_lbl = tk.Label(root, textvariable=self.warn,
                                  font=('Courier', 9), bg='#0f172a', fg='#ef4444')
        self._warn_lbl.pack(pady=(0, 2))

        # ── Canvases ──────────────────────────────────────────────────────────
        self.hcv = tk.Canvas(root, width=cw, height=HAND_H,
                             bg='#0f172a', highlightthickness=0)
        self.hcv.pack(padx=PAD, pady=0)

        self.cv = tk.Canvas(root, width=cw, height=WH+2,
                            bg='#0f172a', highlightthickness=0)
        self.cv.pack(padx=PAD, pady=0)

        self.ruler = tk.Canvas(root, width=cw, height=RULER_H,
                               bg='#0f172a', highlightthickness=0)
        self.ruler.pack(padx=PAD, pady=0)

        # ── Controls ──────────────────────────────────────────────────────────
        ctrl = tk.Frame(root, bg='#0f172a')
        ctrl.pack(pady=(6, 12))
        bcfg = dict(font=('Courier', 10), bg='#1e293b', fg='#94a3b8',
                    activebackground='#334155', activeforeground='#e2e8f0',
                    relief='flat', padx=12, pady=4, cursor='hand2')
        tk.Button(ctrl, text='Load File', command=self._load_file, **bcfg).pack(side='left', padx=4)
        tk.Button(ctrl, text='▶ Play',    command=self._play,      **bcfg).pack(side='left', padx=4)
        tk.Button(ctrl, text='■ Stop',    command=self._stop,      **bcfg).pack(side='left', padx=4)
        self._song_label = tk.StringVar(value="No file loaded")
        tk.Label(ctrl, textvariable=self._song_label, font=('Courier', 9),
                 bg='#0f172a', fg='#475569').pack(side='left', padx=8)

        # ── State ─────────────────────────────────────────────────────────────
        whites, blacks = build_keys(WW, WH, BW, BH, SP2, SP3)
        self._all      = whites + blacks
        self._ids      = {}
        self._active   = set()
        self._sol_keys = set()
        self._song: List[Step] = []

        # Physics state
        self._lpos = LPOS_HOME
        self._rpos = RPOS_HOME
        self._lvel = 0.0
        self._rvel = 0.0
        self._ltgt: Optional[float] = None
        self._rtgt: Optional[float] = None

        # Playback jobs
        self._anim_job  = None    # animation frame
        self._hold_job  = None    # solenoid hold timer
        self._pending_i = -1      # step index waiting to be activated
        self._keep_anim_sol: List[int] = [0]*10   # solenoids kept during move

        self._draw_keys(whites, blacks, WW)
        self._draw_ruler(PX, WW)
        self._render_hands(LPOS_HOME, RPOS_HOME, [0]*10)

        # Start audio caching in background
        if _BACKEND != 'silent':
            self.info.set("Loading piano sounds…")
            def _on_audio_ready():
                self.info.set("Click a key  ·  z–m (C3–B3)  ·  q–u (C4–B4)")
            audio_init([k['name'] for k in self._all], _on_audio_ready, root)

        self.cv.bind('<Button-1>',        self._on_click)
        self.cv.bind('<ButtonRelease-1>', lambda _: self._on_mouse_release())
        root.bind('<KeyPress>',   self._on_key_down)
        root.bind('<KeyRelease>', self._on_key_up)

    # ── Piano drawing ─────────────────────────────────────────────────────────
    def _draw_keys(self, whites, blacks, WW):
        ox  = PAD
        fsz = max(6, round(WW * 0.17))
        for k in whites:
            x   = ox + k['x']
            iid = self.cv.create_rectangle(
                x, 0, x+k['w']-1, k['h'],
                fill='#f8fafc', outline='#334155', width=1)
            self._ids[k['name']] = iid
            if k['name'][:-1] == 'C':
                self.cv.create_text(x+k['w']//2, k['h']-9,
                                    text=k['name'], fill='#94a3b8',
                                    font=('Courier', fsz))
            kb = KB_REV.get(k['name'])
            if kb:
                self.cv.create_text(x+k['w']//2, k['h']-9-fsz-4,
                                    text=f'[{kb}]', fill='#64748b',
                                    font=('Courier', max(6, fsz-1)))
        for k in blacks:
            x   = ox + k['x']
            iid = self.cv.create_rectangle(
                x, 0, x+k['w'], k['h'],
                fill='#1e293b', outline='#000000', width=1)
            self._ids[k['name']] = iid

    # ── Ruler ─────────────────────────────────────────────────────────────────
    def _draw_ruler(self, PX, WW):
        ox = PAD; piano_w = 32*WW; y0 = 6
        self.ruler.create_line(ox, y0, ox+piano_w, y0, fill='#475569', width=1)
        for edge in [ox, ox+piano_w]:
            self.ruler.create_line(edge, y0-6, edge, y0+6, fill='#64748b', width=1)
        cm = 0.0
        while cm <= PIANO_CM + 0.01:
            x    = ox + cm*PX
            is10 = round(cm*10) % 100 == 0
            is5  = round(cm*10) %  50 == 0
            tl   = 12 if is10 else (7 if is5 else 3)
            col  = '#94a3b8' if is10 else ('#64748b' if is5 else '#334155')
            self.ruler.create_line(x, y0, x, y0+tl, fill=col, width=1)
            if is10:
                self.ruler.create_text(x, y0+22, text=f"{round(cm)} cm",
                                       fill='#94a3b8', font=('Courier', 8), anchor='n')
            cm += 1.0
        self.ruler.create_text(ox+piano_w/2, y0-10,
                               text=f"← {PIANO_CM:.1f} cm →",
                               fill='#475569', font=('Courier', 8))

    # ── Hand rendering ────────────────────────────────────────────────────────
    def _render_hands(self, lpos: float, rpos: float,
                      solenoids: List[int], warnings: Optional[List[str]] = None):
        """
        Blue  = left hand (anchor lpos = F0 centre).
        Green = right hand (anchor rpos = F0 centre; F2 is rightmost).
        Active solenoid → finger drops & turns red/orange.
        Out-of-scope finger → red outline.
        """
        self.hcv.delete('all')
        PX = self._PX
        ox = PAD
        fh = HAND_H - 14
        fy = 4
        warn_set = set(warnings) if warnings else set()

        def draw_finger(anchor, ftype, offset, active,
                        c_idle, c_on, label, warn_label):
            fw   = FWIDTH[ftype] * PX
            cx   = ox + (anchor + offset) * PX
            x0   = cx - fw / 2
            x1   = cx + fw / 2
            dy   = 9 if active else 0
            fill = c_on if active else c_idle
            oc   = '#ff0000' if warn_label in warn_set else ('#991b1b' if active else '#1e3a5f')
            self.hcv.create_rectangle(x0, fy+dy, x1, fy+fh,
                                      fill=fill, outline=oc,
                                      width=2 if warn_label in warn_set else 1)
            self.hcv.create_text((x0+x1)/2, fy+dy+fh//2,
                                 text=label, fill='#e2e8f0',
                                 font=('Courier', max(6, round(fw*0.5))))

        for i, (ft, off) in enumerate(LEFT_FINGERS):
            draw_finger(lpos, ft, off, bool(solenoids[i]),
                        '#2563eb', '#ef4444', str(i), f'L{i}')

        for i, (ft, off) in enumerate(RIGHT_FINGERS):
            draw_finger(rpos, ft, -off, bool(solenoids[7+i]),
                        '#059669', '#f59e0b', str(i), f'R{i}')

        # Show position readout on hand canvas
        PX = self._PX
        ox = PAD
        lx = ox + lpos * PX
        rx = ox + rpos * PX            # rpos = F2 (rightmost) position
        self.hcv.create_text(lx, 1, text=f"L:{lpos:.1f}", fill='#60a5fa',
                             font=('Courier', 7), anchor='n')
        self.hcv.create_text(rx, 1, text=f"R:{rpos:.1f}", fill='#34d399',
                             font=('Courier', 7), anchor='n')

    # ── Hit testing ───────────────────────────────────────────────────────────
    def _key_at_px(self, mx: int, my: int) -> Optional[str]:
        ox = PAD
        for k in reversed(self._all):
            kx = ox + k['x']
            if kx <= mx <= kx+k['w'] and 0 <= my <= k['h']:
                return k['name']
        return None

    def _key_at_cm(self, cx: float) -> Optional[str]:
        """Key whose bounding box contains cx (cm from piano left). Blacks first."""
        px = PAD + cx * self._PX
        for k in reversed(self._all):
            kx = PAD + k['x']
            if kx <= px <= kx + k['w']:
                return k['name']
        return None

    # ── Scope checking ────────────────────────────────────────────────────────
    def _scope_warnings(self, s: Step) -> List[str]:
        """Return list of warning tags for any active finger outside piano range."""
        issues: List[str] = []
        for idx, (_, off) in enumerate(LEFT_FINGERS):
            if not s.solenoids[idx]:
                continue
            cx = s.lpos + off
            hw = FWIDTH[LEFT_FINGERS[idx][0]] / 2
            if cx - hw < 0 or cx + hw > PIANO_CM:
                issues.append(f'L{idx}')
            elif self._key_at_cm(cx) is None:
                issues.append(f'L{idx}')
        for idx, (_, off) in enumerate(RIGHT_FINGERS):
            if not s.solenoids[7+idx]:
                continue
            cx = s.rpos - off
            hw = FWIDTH[RIGHT_FINGERS[idx][0]] / 2
            if cx - hw < 0 or cx + hw > PIANO_CM:
                issues.append(f'R{idx}')
            elif self._key_at_cm(cx) is None:
                issues.append(f'R{idx}')
        return issues

    # ── Key-type mismatch checking ────────────────────────────────────────────
    def _type_warnings(self, s: Step) -> List[str]:
        """Return tags where a white finger lands on a black key or vice-versa."""
        issues: List[str] = []
        for idx, (ft, off) in enumerate(LEFT_FINGERS):
            if not s.solenoids[idx]:
                continue
            key = self._key_at_cm(s.lpos + off)
            if key is None:
                continue
            key_is_black = '#' in key
            if ft == 'W' and key_is_black:
                issues.append(f'L{idx}')
            elif ft == 'B' and not key_is_black:
                issues.append(f'L{idx}')
        for idx, (ft, off) in enumerate(RIGHT_FINGERS):
            if not s.solenoids[7+idx]:
                continue
            key = self._key_at_cm(s.rpos - off)
            if key is None:
                continue
            key_is_black = '#' in key
            if ft == 'W' and key_is_black:
                issues.append(f'R{idx}')
            elif ft == 'B' and not key_is_black:
                issues.append(f'R{idx}')
        return issues

    # ── Overlap checking ─────────────────────────────────────────────────────
    def _overlap_warnings(self, s: Step) -> List[str]:
        """Return descriptions of any two active fingers whose physical extents overlap."""
        # Collect (label, centre_cm, half_width_cm) for every active finger
        active = []
        for idx, (ft, off) in enumerate(LEFT_FINGERS):
            if s.solenoids[idx]:
                active.append((f'L{idx}', s.lpos + off, FWIDTH[ft] / 2))
        for idx, (ft, off) in enumerate(RIGHT_FINGERS):
            if s.solenoids[7+idx]:
                active.append((f'R{idx}', s.rpos - off, FWIDTH[ft] / 2))

        issues: List[str] = []
        for i in range(len(active)):
            for j in range(i + 1, len(active)):
                la, ca, ha = active[i]
                lb, cb, hb = active[j]
                # Intervals [ca-ha, ca+ha] and [cb-hb, cb+hb] overlap when:
                if max(ca - ha, cb - hb) < min(ca + ha, cb + hb):
                    mid = (ca + cb) / 2
                    issues.append(f'{la}↔{lb} @ {mid:.1f}cm')
        return issues

    # ── Manual press / release ────────────────────────────────────────────────
    def _press(self, name: str):
        if not name or name in self._active:
            return
        self._active.add(name)
        is_b = '#' in name
        self.cv.itemconfig(self._ids[name],
                           fill='#ef4444' if is_b else '#f59e0b')
        f = note_freq(name)
        self.info.set(f"♪  {name}   {f} Hz   (midi {note_midi(name)})")
        audio_play(name, 700)

    def _release(self, name: str):
        if name not in self._active:
            return
        self._active.discard(name)
        is_b = '#' in name
        self.cv.itemconfig(self._ids[name],
                           fill='#1e293b' if is_b else '#f8fafc')

    # ── Solenoid press / release ──────────────────────────────────────────────
    def _sol_press(self, name: str, silent: bool = False):
        if not name:
            return
        fresh = name not in self._sol_keys
        self._sol_keys.add(name)
        is_b = '#' in name
        self.cv.itemconfig(self._ids[name],
                           fill='#ef4444' if is_b else '#f59e0b')
        if fresh and not silent:
            audio_play(name, 700)

    def _sol_release_all(self, keep: Optional[set] = None):
        keep = keep or set()
        for n in list(self._sol_keys):
            if n in keep:
                continue
            self._sol_keys.discard(n)
            if n not in self._active:
                is_b = '#' in n
                self.cv.itemconfig(self._ids[n],
                                   fill='#1e293b' if is_b else '#f8fafc')
        self._sol_keys = self._sol_keys & keep

    # ── Events ────────────────────────────────────────────────────────────────
    def _on_click(self, e):
        self._press(self._key_at_px(e.x, e.y))

    def _on_mouse_release(self):
        for n in list(self._active):
            self._release(n)

    def _on_key_down(self, e):
        self._press(KB.get(e.char) or KB.get(e.keysym.lower()))

    def _on_key_up(self, e):
        n = KB.get(e.char) or KB.get(e.keysym.lower())
        if n:
            self._release(n)

    # ── Song validation ───────────────────────────────────────────────────────
    def _scan_song_issues(self):
        """Scan all steps on load; report scope / type / overlap problems."""
        scope_steps:   List[int] = []
        type_steps:    List[int] = []
        overlap_steps: List[int] = []
        for i, s in enumerate(self._song):
            if self._scope_warnings(s):
                scope_steps.append(i + 1)
            if self._type_warnings(s):
                type_steps.append(i + 1)
            if self._overlap_warnings(s):
                overlap_steps.append(i + 1)

        parts: List[str] = []
        if scope_steps:
            parts.append(f"⚠ SCOPE steps: {scope_steps}")
        if type_steps:
            parts.append(f"⚠ KEY TYPE steps: {type_steps}")
        if overlap_steps:
            parts.append(f"⚠ OVERLAP steps: {overlap_steps}")
        if parts:
            self.warn.set("  ".join(parts))
        else:
            self.warn.set("✓ No scope / type / overlap issues found")

    # ── Song playback ─────────────────────────────────────────────────────────
    def _load_file(self):
        path = filedialog.askopenfilename(
            title='Load song file',
            filetypes=[('Text files', '*.txt'), ('All files', '*.*')])
        if not path:
            return
        try:
            self._song = parse_song(path)
            name = path.replace('\\', '/').split('/')[-1]
            self._song_label.set(f"{name}  ({len(self._song)} steps)")
            self.info.set(f"Loaded: {name}  — press ▶ Play")
            self._scan_song_issues()
        except Exception as ex:
            self.info.set(f"Error loading file: {ex}")

    def _play(self):
        if not self._song:
            self.info.set("No file loaded — use Load File first")
            return
        self._stop()
        self._step(0)

    def _stop(self):
        if self._anim_job:
            self.cv.after_cancel(self._anim_job)
            self._anim_job = None
        if self._hold_job:
            self.cv.after_cancel(self._hold_job)
            self._hold_job = None
        self._ltgt = None
        self._rtgt = None
        self._lvel = 0.0
        self._rvel = 0.0
        self._sol_release_all()
        self.warn.set("")
        self._render_hands(LPOS_HOME, RPOS_HOME, [0]*10)
        self._lpos = LPOS_HOME
        self._rpos = RPOS_HOME

    # ── Step sequencer ────────────────────────────────────────────────────────
    def _step(self, i: int):
        if i >= len(self._song):
            self.info.set("Playback complete.")
            self.warn.set("")
            self._sol_release_all()
            self._render_hands(LPOS_HOME, RPOS_HOME, [0]*10)
            self._hold_job = None
            return

        s    = self._song[i]
        prev = self._song[i-1] if i > 0 else None

        # Determine which keys to keep lit during movement (keep flags)
        keep_set: set = set()
        keep_sol = [0]*10
        if prev:
            if prev.keep_l:
                for idx, (_, off) in enumerate(LEFT_FINGERS):
                    if prev.solenoids[idx]:
                        k = self._key_at_cm(prev.lpos + off)
                        if k:
                            keep_set.add(k)
                        keep_sol[idx] = 1
            if prev.keep_r:
                for idx, (_, off) in enumerate(RIGHT_FINGERS):
                    if prev.solenoids[7+idx]:
                        k = self._key_at_cm(prev.rpos - off)
                        if k:
                            keep_set.add(k)
                        keep_sol[7+idx] = 1

        self._sol_release_all(keep=keep_set)
        self._keep_anim_sol = keep_sol
        self._pending_i = i

        # Set physics targets and start animation
        self._ltgt = s.lpos
        self._rtgt = s.rpos
        self._anim_job = self.cv.after(ANIM_DT, self._animate)

        n_total = len(self._song)
        self.info.set(f"Moving → step {i+1}/{n_total}  "
                      f"L:{s.lpos:.1f} cm  R:{s.rpos:.1f} cm")

    def _animate(self):
        """Physics update — runs at ANIM_DT ms intervals."""
        self._anim_job = None
        if self._ltgt is None:
            return

        dt = ANIM_DT / 1000.0

        el = self._ltgt - self._lpos
        al = KP * el - KD * self._lvel
        self._lvel = max(-VMAX, min(VMAX, self._lvel + al * dt))
        self._lpos += self._lvel * dt

        er = self._rtgt - self._rpos
        ar = KP * er - KD * self._rvel
        self._rvel = max(-VMAX, min(VMAX, self._rvel + ar * dt))
        self._rpos += self._rvel * dt

        self._render_hands(self._lpos, self._rpos, self._keep_anim_sol)

        settled = (abs(el) < DEADBAND and abs(er) < DEADBAND
                   and abs(self._lvel) < VSETTLE and abs(self._rvel) < VSETTLE)

        if settled:
            # Snap exactly to target
            self._lpos = self._ltgt
            self._rpos = self._rtgt
            self._lvel = 0.0
            self._rvel = 0.0
            self._ltgt = None
            self._rtgt = None
            self._on_settled()
        else:
            self._anim_job = self.cv.after(ANIM_DT, self._animate)

    def _on_settled(self):
        """Hand reached target — press solenoids and start hold timer."""
        i = self._pending_i
        if i < 0 or i >= len(self._song):
            return

        s = self._song[i]
        warnings  = self._scope_warnings(s)
        type_warn = self._type_warnings(s)
        overlaps  = self._overlap_warnings(s)

        # Press keys under active solenoid fingers
        for idx, (ft, off) in enumerate(LEFT_FINGERS):
            if s.solenoids[idx]:
                k = self._key_at_cm(s.lpos + off)
                if k:
                    mismatch = (ft == 'W') == ('#' in k)
                    self._sol_press(k, silent=mismatch)

        for idx, (ft, off) in enumerate(RIGHT_FINGERS):
            if s.solenoids[7+idx]:
                k = self._key_at_cm(s.rpos - off)
                if k:
                    mismatch = (ft == 'W') == ('#' in k)
                    self._sol_press(k, silent=mismatch)

        all_warn_tags = list(dict.fromkeys(warnings + type_warn))   # unique, order-preserved
        self._render_hands(s.lpos, s.rpos, s.solenoids,
                           warnings=all_warn_tags if all_warn_tags else None)

        n_total = len(self._song)
        flags = ("  [keep L]" if s.keep_l else "") + ("  [keep R]" if s.keep_r else "")
        self.info.set(
            f"Step {i+1}/{n_total}  "
            f"L:{s.lpos:.1f}  R:{s.rpos:.1f}  "
            f"sol={''.join(str(x) for x in s.solenoids)}  "
            f"{s.duration} ms{flags}"
        )

        parts: List[str] = []
        if warnings:
            parts.append("⚠ SCOPE: " + "  ".join(
                f"{w} @ {s.lpos + LEFT_FINGERS[int(w[1])][1]:.1f}cm"
                if w[0] == 'L'
                else f"{w} @ {s.rpos - RIGHT_FINGERS[int(w[1])][1]:.1f}cm"
                for w in warnings))
        if type_warn:
            parts.append("⚠ KEY TYPE: " + "  ".join(type_warn))
        if overlaps:
            parts.append("⚠ OVERLAP: " + "  ".join(overlaps))
        self.warn.set("   ".join(parts))

        self._hold_job = self.cv.after(s.duration, lambda: self._step(i+1))


if __name__ == '__main__':
    root = tk.Tk()
    PianoApp(root)
    root.mainloop()
