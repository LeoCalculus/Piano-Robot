#!/usr/bin/env python3
"""
Piano Simulator — C2 to F6 (32 white + 22 black = 54 keys)
Units: Millimeters (mm) | Key Pitch: 21 mm
UBC ELEC 391 Project Support - Custom Step Jump Added
"""
import threading
import tkinter as tk
from tkinter import filedialog
from dataclasses import dataclass
from typing import List, Optional

# ── 1. Hand finger definitions (Offsets in mm from F0 anchor) ────────────────
LEFT_FINGERS: List = [
    ('W', 0.0),    # F0 -> C2 (Anchor)
    ('B', 33.8),   # F1 -> D#2
    ('W', 42.0),   # F2 -> E2
    ('W', 63.0),   # F3 -> F2
    ('W', 84.0),   # F4 -> G2
    ('B', 94.5),   # F5 -> G#2
    ('W', 105.0),  # F6 -> A2
]
RIGHT_FINGERS: List = [
    ('W', 42.0),   # F0 -> D6
    ('B', 11.0),   # F1 -> D#6
    ('W', 0.0),    # F2 anchor -> F6 centre
]
FWIDTH = {'W': 7.0, 'B': 10.0}

# ── 2. Piano / layout constants (Units in mm) ────────────────────────────────
PIANO_MM  = 672.0
HALF_WW   = 21.0 / 2
PAD       = 14
RULER_H   = 42
HAND_H    = 52
LPOS_HOME = 10.5
RPOS_HOME = 661.5

# ── 3. Physics constants ─────────────────────────────────────────────────────
KP, KD = 1000.0, 26.0
VMAX, DEADBAND, VSETTLE = 1000.0, 3.0, 30.0
ANIM_DT = 16

_SEMIS = ['C','C#','D','D#','E','F','F#','G','G#','A','A#','B']

def note_midi(name: str) -> int:
    return (int(name[-1]) + 1) * 12 + _SEMIS.index(name[:-1])

def note_freq(name: str) -> int:
    return round(440.0 * 2.0 ** ((note_midi(name) - 69) / 12.0))

@dataclass
class Step:
    lpos:      float
    rpos:      float
    solenoids: List[int]
    duration:  int
    keep_l:    int
    keep_r:    int

def parse_song(path: str) -> List[Step]:
    steps = []
    with open(path) as fh:
        for line in fh:
            v = line.strip().split()
            if len(v) < 15: continue
            steps.append(Step(
                lpos=float(v[0]) + HALF_WW, 
                rpos=RPOS_HOME - float(v[1]), 
                solenoids=[int(x) for x in v[2:12]],
                duration=int(v[12]),
                keep_l=int(v[13]), keep_r=int(v[14]),
            ))
    return steps

def build_keys(WW, WH, BW, BH, SP2, SP3):
    whites, blacks = [], []
    def add_octave(num, x0):
        for i, p in enumerate(['C','D','E','F','G','A','B']):
            whites.append(dict(name=f"{p}{num}", x=x0+i*WW, w=WW, h=WH))
        o2 = x0 + (3*WW - 2*SP2) / 2
        for i, p in enumerate(['C#','D#']):
            blacks.append(dict(name=f"{p}{num}", x=o2+i*SP2+(SP2-BW)/2, w=BW, h=BH))
        o3 = x0 + 3*WW + (4*WW - 3*SP3) / 2
        for i, p in enumerate(['F#','G#','A#']):
            blacks.append(dict(name=f"{p}{num}", x=o3+i*SP3+(SP3-BW)/2, w=BW, h=BH))
    for o in range(4): add_octave(2+o, o*7*WW)
    x6 = 4*7*WW
    for i, p in enumerate(['C','D','E','F']):
        whites.append(dict(name=f"{p}6", x=x6+i*WW, w=WW, h=WH))
    o2 = x6 + (3*WW - 2*SP2) / 2
    for i, p in enumerate(['C#','D#']):
        blacks.append(dict(name=f"{p}6", x=o2+i*SP2+(SP2-BW)/2, w=BW, h=BH))
    return whites, blacks

def _detect_backend() -> str:
    try:
        __import__('numpy'); __import__('pygame')
        return 'pygame'
    except ImportError: return 'silent'

_BACKEND = _detect_backend()
_sound_cache: dict = {}
_audio_ready = False

def _synth(freq: float):
    import numpy as np
    SR, DUR = 44100, 2.0
    t = np.linspace(0, DUR, int(SR * DUR), endpoint=False)
    partials = [(1, 1.0, 2.2), (2, 0.45, 3.5), (3, 0.22, 5.0)]
    sig = sum(amp * np.exp(-dec * t) * np.sin(2 * np.pi * freq * h * t) for h, amp, dec in partials)
    atk = int(SR * 0.004)
    sig[:atk] *= np.linspace(0, 1, atk)
    return sig / (np.max(np.abs(sig)) + 1e-9) * 0.80

def audio_init(note_names: list, on_ready, root_widget):
    global _audio_ready
    if _BACKEND == 'silent':
        _audio_ready = True
        root_widget.after(0, on_ready); return
    def _worker():
        global _audio_ready
        import numpy as np
        import pygame
        pygame.mixer.init(44100, -16, 2, 1024)
        for name in note_names:
            freq = 440.0 * 2.0 ** ((note_midi(name) - 69) / 12.0)
            mono = _synth(freq)
            arr = (np.column_stack([mono, mono]) * 32767).astype(np.int16)
            _sound_cache[name] = pygame.sndarray.make_sound(arr)
        _audio_ready = True
        root_widget.after(0, on_ready)
    threading.Thread(target=_worker, daemon=True).start()

def audio_play(name: str, ms: int = 700):
    if not _audio_ready or name not in _sound_cache: return
    if _BACKEND == 'pygame':
        import pygame
        _sound_cache[name].play(maxtime=ms)

KB = {
    'z':'C3', 's':'C#3', 'x':'D3', 'd':'D#3', 'c':'E3', 'v':'F3', 'g':'F#3', 
    'b':'G3', 'h':'G#3', 'n':'A3', 'j':'A#3', 'm':'B3',
    'q':'C4', '2':'C#4', 'w':'D4', '3':'D#4', 'e':'E4', 'r':'F4', '5':'F#4', 
    't':'G4', '6':'G#4', 'y':'A4', '7':'A#4', 'u':'B4',
}
KB_REV = {v: k for k, v in KB.items()}

# ── Main App Class ────────────────────────────────────────────────────────
class PianoApp:
    def __init__(self, root: tk.Tk):
        root.title("Piano Simulator (Step Jump Enabled) — UBC ELEC 391")
        root.resizable(False, False)
        root.configure(bg='#0f172a')

        scr_w = root.winfo_screenwidth()
        PX = (scr_w - 2*PAD - 40) / PIANO_MM
        self._PX = PX

        WW = round(21.0*PX);  WH = round(5.5*21*PX)
        BW = round(12.3*PX);  BH = round(3.3*21*PX)
        SP2, SP3 = 25.6*PX, 23.7*PX
        cw = 32*WW + 2*PAD

        self.info = tk.StringVar(value="Click a key · z–m (C3–B3) · q–u (C4–B4)")
        tk.Label(root, textvariable=self.info, font=('Courier', 10), bg='#0f172a', fg='#94a3b8').pack(pady=(10, 1))

        self.warn = tk.StringVar(value="")
        tk.Label(root, textvariable=self.warn, font=('Courier', 9), bg='#0f172a', fg='#ef4444').pack(pady=(0, 2))

        self.hcv = tk.Canvas(root, width=cw, height=HAND_H, bg='#0f172a', highlightthickness=0)
        self.hcv.pack(padx=PAD)
        self.cv = tk.Canvas(root, width=cw, height=WH+2, bg='#0f172a', highlightthickness=0)
        self.cv.pack(padx=PAD)
        self.ruler = tk.Canvas(root, width=cw, height=RULER_H, bg='#0f172a', highlightthickness=0)
        self.ruler.pack(padx=PAD)

        # ── Controls ──────────────────────────────────────────────────────────
        ctrl = tk.Frame(root, bg='#0f172a')
        ctrl.pack(pady=(6, 12))
        bcfg = dict(font=('Courier', 10), bg='#1e293b', fg='#94a3b8', relief='flat', padx=10, pady=4, cursor='hand2')
        
        tk.Button(ctrl, text='Load File', command=self._load_file, **bcfg).pack(side='left', padx=4)
        tk.Button(ctrl, text='▶ Play', command=self._play, **bcfg).pack(side='left', padx=4)
        
        self._paused = False
        self._pause_text = tk.StringVar(value="Pause")
        tk.Button(ctrl, textvariable=self._pause_text, command=self._toggle_pause, **bcfg).pack(side='left', padx=4)
        
        tk.Button(ctrl, text='■ Stop', command=self._stop, **bcfg).pack(side='left', padx=4)

        # ── 新增：跳转 Entry ──────────────────────────────────────────────────
        tk.Label(ctrl, text="Step:", font=('Courier', 10), bg='#0f172a', fg='#64748b').pack(side='left', padx=(10, 0))
        self._step_input = tk.Entry(ctrl, width=5, font=('Courier', 10), bg='#1e293b', fg='#e2e8f0', insertbackground='white', borderwidth=0)
        self._step_input.pack(side='left', padx=4, ipady=3)
        tk.Button(ctrl, text='Jump', command=self._jump_to_step, **bcfg).pack(side='left', padx=4)
        
        self._song_label = tk.StringVar(value="No file loaded")
        tk.Label(ctrl, textvariable=self._song_label, font=('Courier', 9), bg='#0f172a', fg='#475569').pack(side='left', padx=8)

        # State
        whites, blacks = build_keys(WW, WH, BW, BH, SP2, SP3)
        self._all, self._ids = whites + blacks, {}
        self._active, self._sol_keys = set(), set()
        self._song, self._lpos, self._rpos = [], LPOS_HOME, RPOS_HOME
        self._lvel, self._rvel, self._ltgt, self._rtgt = 0.0, 0.0, None, None
        self._anim_job, self._hold_job, self._pending_i = None, None, -1
        self._keep_anim_sol = [0]*10

        self._draw_keys(whites, blacks, WW)
        self._draw_ruler(PX, WW)
        self._render_hands(LPOS_HOME, RPOS_HOME, [0]*10)

        audio_init([k['name'] for k in self._all], lambda: None, root)
        self.cv.bind('<Button-1>', self._on_click)
        self.cv.bind('<ButtonRelease-1>', lambda _: self._on_mouse_release())
        root.bind('<KeyPress>', self._on_key_down)
        root.bind('<KeyRelease>', self._on_key_up)

    def _draw_keys(self, whites, blacks, WW):
        ox, fsz = PAD, max(6, round(WW * 0.17))
        for k in whites:
            x = ox + k['x']
            iid = self.cv.create_rectangle(x, 0, x+k['w']-1, k['h'], fill='#f8fafc', outline='#334155')
            self._ids[k['name']] = iid
            if k['name'][:-1] == 'C':
                self.cv.create_text(x+k['w']//2, k['h']-9, text=k['name'], fill='#94a3b8', font=('Courier', fsz))
        for k in blacks:
            x = ox + k['x']
            self._ids[k['name']] = self.cv.create_rectangle(x, 0, x+k['w'], k['h'], fill='#1e293b', outline='#000000')

    def _draw_ruler(self, PX, WW):
        ox, piano_w, y0 = PAD, 32*WW, 6
        self.ruler.create_line(ox, y0, ox+piano_w, y0, fill='#475569')
        for mm in range(0, int(PIANO_MM) + 1, 10):
            x = ox + mm * PX
            is100 = mm % 100 == 0
            tl, col = (12, '#94a3b8') if is100 else (6, '#334155')
            self.ruler.create_line(x, y0, x, y0+tl, fill=col)
            if is100:
                self.ruler.create_text(x, y0+22, text=f"{mm}mm", fill='#94a3b8', font=('Courier', 8), anchor='n')
        self.ruler.create_text(ox+piano_w/2, y0-10, text=f"← {PIANO_MM:.0f} mm →", fill='#475569', font=('Courier', 8))

    def _render_hands(self, lpos, rpos, solenoids, warnings=None):
        self.hcv.delete('all')
        PX, ox, fh, fy = self._PX, PAD, HAND_H-14, 4
        warn_set = set(warnings) if warnings else set()
        def draw_finger(anchor, ftype, offset, active, c_idle, c_on, label, warn_label):
            fw = FWIDTH[ftype] * PX
            cx = ox + (anchor + offset) * PX
            x0, x1 = cx - fw/2, cx + fw/2
            dy, fill = (9, c_on) if active else (0, c_idle)
            oc = '#ff0000' if warn_label in warn_set else ('#991b1b' if active else '#1e3a5f')
            self.hcv.create_rectangle(x0, fy+dy, x1, fy+fh, fill=fill, outline=oc, width=2 if warn_label in warn_set else 1)
            self.hcv.create_text(cx, fy+dy+fh//2, text=label, fill='#e2e8f0', font=('Courier', max(6, round(fw*0.5))))
        for i, (ft, off) in enumerate(LEFT_FINGERS):
            draw_finger(lpos, ft, off, bool(solenoids[i]), '#2563eb', '#ef4444', str(i), f'L{i}')
        for i, (ft, off) in enumerate(RIGHT_FINGERS):
            draw_finger(rpos, ft, -off, bool(solenoids[7+i]), '#059669', '#f59e0b', str(i), f'R{i}')
        self.hcv.create_text(ox+lpos*PX, 1, text=f"L:{lpos:.0f}", fill='#60a5fa', font=('Courier', 7), anchor='n')
        self.hcv.create_text(ox+rpos*PX, 1, text=f"R:{rpos:.0f}", fill='#34d399', font=('Courier', 7), anchor='n')

    def _key_at_cm(self, cx_mm: float) -> Optional[str]:
        px = PAD + cx_mm * self._PX
        for k in reversed(self._all):
            kx = PAD + k['x']
            if kx <= px <= kx + k['w']: return k['name']
        return None

    def _scope_warnings(self, s: Step) -> List[str]:
        issues = []
        for i, (_, off) in enumerate(LEFT_FINGERS):
            if s.solenoids[i] and (s.lpos+off < 0 or s.lpos+off > PIANO_MM): issues.append(f'L{i}')
        for i, (_, off) in enumerate(RIGHT_FINGERS):
            if s.solenoids[7+i] and (s.rpos-off < 0 or s.rpos-off > PIANO_MM): issues.append(f'R{i}')
        return issues

    def _type_warnings(self, s: Step) -> List[str]:
        issues = []
        for i, (ft, off) in enumerate(LEFT_FINGERS):
            if s.solenoids[i]:
                k = self._key_at_cm(s.lpos+off)
                if k and (('#' in k) != (ft == 'B')): issues.append(f'L{i}')
        for i, (ft, off) in enumerate(RIGHT_FINGERS):
            if s.solenoids[7+i]:
                k = self._key_at_cm(s.rpos-off)
                if k and (('#' in k) != (ft == 'B')): issues.append(f'R{i}')
        return issues

    def _press(self, name: str):
        if not name or name in self._active: return
        self._active.add(name)
        self.cv.itemconfig(self._ids[name], fill='#ef4444' if '#' in name else '#f59e0b')
        audio_play(name)

    def _release(self, name: str):
        if name not in self._active: return
        self._active.discard(name)
        self.cv.itemconfig(self._ids[name], fill='#1e293b' if '#' in name else '#f8fafc')

    def _sol_press(self, name: str):
        if name in self._sol_keys: return 
        self._sol_keys.add(name)
        self.cv.itemconfig(self._ids[name], fill='#ef4444' if '#' in name else '#f59e0b')
        audio_play(name)

    def _sol_release_all(self, keep=None):
        keep = keep or set()
        for n in list(self._sol_keys):
            if n in keep: continue
            self._sol_keys.discard(n)
            if n not in self._active:
                self.cv.itemconfig(self._ids[n], fill='#1e293b' if '#' in n else '#f8fafc')
        self._sol_keys &= keep

    def _on_click(self, e):
        for k in reversed(self._all):
            kx = PAD + k['x']
            if kx <= e.x <= kx+k['w'] and 0 <= e.y <= k['h']:
                self._press(k['name']); break

    def _on_mouse_release(self):
        for n in list(self._active): self._release(n)

    def _on_key_down(self, e): self._press(KB.get(e.char) or KB.get(e.keysym.lower()))
    def _on_key_up(self, e):
        n = KB.get(e.char) or KB.get(e.keysym.lower()); 
        if n: self._release(n)

    def _load_file(self):
        path = filedialog.askopenfilename(filetypes=[('Text', '*.txt'), ('All', '*.*')])
        if path:
            self._song = parse_song(path)
            self._song_label.set(f"{path.split('/')[-1]} ({len(self._song)} steps)")

    # ── 跳转逻辑 ──
    def _jump_to_step(self):
        if not self._song:
            self.warn.set("⚠ Load a file first")
            return
        try:
            target_idx = int(self._step_input.get()) - 1
            if 0 <= target_idx < len(self._song):
                self._stop() # 先停止当前所有动作
                self.warn.set("")
                self._step(target_idx) # 从目标位置开始
            else:
                self.warn.set(f"⚠ Invalid index (1-{len(self._song)})")
        except ValueError:
            self.warn.set("⚠ Enter a valid number")

    def _play(self):
        if self._song:
            self._stop()
            self._step(0)

    def _toggle_pause(self):
        if not self._song or self._pending_i < 0: return
        self._paused = not self._paused
        if self._paused:
            self._pause_text.set("Resume")
            if self._anim_job: self.cv.after_cancel(self._anim_job)
            if self._hold_job: self.cv.after_cancel(self._hold_job)
        else:
            self._pause_text.set("Pause")
            if self._ltgt is not None:
                self._animate()
            else:
                self._step(self._pending_i + 1)

    def _stop(self):
        self._paused = False
        self._pause_text.set("Pause")
        if self._anim_job: self.cv.after_cancel(self._anim_job)
        if self._hold_job: self.cv.after_cancel(self._hold_job)
        self._sol_release_all(); self._render_hands(LPOS_HOME, RPOS_HOME, [0]*10)
        self._lpos, self._rpos, self._ltgt = LPOS_HOME, RPOS_HOME, None
        self._pending_i = -1

    def _step(self, i):
        if i >= len(self._song): 
            self.info.set("Playback Done.")
            self._stop()
            return
        
        self._pending_i = i
        if self._paused: return

        s, prev = self._song[i], self._song[i-1] if i > 0 else None
        keep_set, keep_sol = set(), [0] * 10 

        if prev:
            if prev.keep_l == 1:
                for idx in range(7):
                    if prev.solenoids[idx]:
                        k = self._key_at_cm(prev.lpos + LEFT_FINGERS[idx][1])
                        if k: keep_set.add(k); keep_sol[idx] = 1
            if prev.keep_r == 1:
                for idx in range(3):
                    if prev.solenoids[7+idx]:
                        k = self._key_at_cm(prev.rpos - RIGHT_FINGERS[idx][1])
                        if k: keep_set.add(k); keep_sol[7+idx] = 1

        self._sol_release_all(keep=keep_set)
        self._keep_anim_sol = keep_sol
        self._ltgt, self._rtgt = s.lpos, s.rpos
        self._animate()

    def _animate(self):
        if self._ltgt is None or self._paused: return
        dt = ANIM_DT/1000.0
        el, er = self._ltgt - self._lpos, self._rtgt - self._rpos
        self._lvel = max(-VMAX, min(VMAX, self._lvel + (KP*el - KD*self._lvel)*dt))
        self._rvel = max(-VMAX, min(VMAX, self._rvel + (KP*er - KD*self._rvel)*dt))
        self._lpos += self._lvel*dt; self._rpos += self._rvel*dt
        self._render_hands(self._lpos, self._rpos, self._keep_anim_sol)
        if abs(el)<DEADBAND and abs(er)<DEADBAND and abs(self._lvel)<VSETTLE:
            self._lpos, self._rpos, self._ltgt = self._ltgt, self._rtgt, None
            self._on_settled()
        else: self._anim_job = self.cv.after(ANIM_DT, self._animate)

    def _on_settled(self):
        if self._paused: return
        s = self._song[self._pending_i]
        for i, (_, off) in enumerate(LEFT_FINGERS):
            if s.solenoids[i]:
                k = self._key_at_cm(s.lpos+off)
                if k: self._sol_press(k)
        for i, (_, off) in enumerate(RIGHT_FINGERS):
            if s.solenoids[7+i]:
                k = self._key_at_cm(s.rpos-off)
                if k: self._sol_press(k)
        
        tw = self._type_warnings(s)
        self._render_hands(s.lpos, s.rpos, s.solenoids, warnings=self._scope_warnings(s)+tw)
        self.info.set(f"Step {self._pending_i+1}/{len(self._song)} | L:{s.lpos:.0f} R:{s.rpos:.0f}")
        self._hold_job = self.cv.after(s.duration, lambda: self._step(self._pending_i+1))

if __name__ == '__main__':
    root = tk.Tk()
    PianoApp(root)
    root.mainloop()