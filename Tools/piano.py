"""
Virtual Piano - A playable piano with realistic sound synthesis
Supports mouse clicks, computer keyboard input, and MIDI file playback
Standard piano tuning (A4 = 440Hz)
Features: MIDI parsing, hand simulation (max 10 simultaneous keys)
"""

import numpy as np
import tkinter as tk
from tkinter import ttk, filedialog, messagebox
import threading
import time
import json
from dataclasses import dataclass
from typing import List, Optional, Tuple
from collections import defaultdict

# Audio settings
SAMPLE_RATE = 44100
DURATION = 1.5  # Note duration in seconds
MAX_FINGERS = 10  # Maximum simultaneous notes (simulating two hands)


@dataclass
class MidiNote:
    """Represents a single MIDI note event"""
    note_number: int  # MIDI note number (0-127)
    note_name: str    # Human readable name like "C4", "F#5"
    start_time: float # Start time in seconds
    duration: float   # Duration in seconds
    velocity: int     # Velocity (0-127)
    channel: int      # MIDI channel
    
    def __repr__(self):
        return f"{self.note_name}(t={self.start_time:.2f}s, dur={self.duration:.2f}s, vel={self.velocity})"


@dataclass 
class MidiTrack:
    """Represents a parsed MIDI track"""
    name: str
    notes: List[MidiNote]
    
    
@dataclass
class ParsedMidi:
    """Complete parsed MIDI file"""
    filename: str
    tempo: float  # BPM
    duration: float  # Total duration in seconds
    tracks: List[MidiTrack]
    all_notes: List[MidiNote]  # All notes sorted by start time
    

class MidiParser:
    """Parses MIDI files into playable note sequences"""
    
    NOTE_NAMES = ['C', 'C#', 'D', 'D#', 'E', 'F', 'F#', 'G', 'G#', 'A', 'A#', 'B']
    
    def __init__(self):
        self.mido = None
        try:
            import mido
            self.mido = mido
        except ImportError:
            print("Note: Install 'mido' for MIDI support: pip install mido")
    
    @staticmethod
    def midi_note_to_name(note_number: int) -> str:
        """Convert MIDI note number to note name (e.g., 60 -> 'C4')"""
        octave = (note_number // 12) - 1
        note_index = note_number % 12
        note_name = MidiParser.NOTE_NAMES[note_index]
        return f"{note_name}{octave}"
    
    @staticmethod
    def name_to_midi_note(name: str) -> int:
        """Convert note name to MIDI number (e.g., 'C4' -> 60)"""
        # Parse note name
        if '#' in name:
            note = name[:2]
            octave = int(name[2:])
        else:
            note = name[0]
            octave = int(name[1:])
        
        note_index = MidiParser.NOTE_NAMES.index(note)
        return (octave + 1) * 12 + note_index
    
    def parse_file(self, filepath: str) -> Optional[ParsedMidi]:
        """Parse a MIDI file and return structured data"""
        if not self.mido:
            raise ImportError("mido library not installed. Run: pip install mido")
        
        try:
            mid = self.mido.MidiFile(filepath)
        except Exception as e:
            raise ValueError(f"Failed to open MIDI file: {e}")
        
        # Get tempo (default 120 BPM)
        tempo = 500000  # microseconds per beat (default)
        for track in mid.tracks:
            for msg in track:
                if msg.type == 'set_tempo':
                    tempo = msg.tempo
                    break
        
        bpm = 60000000 / tempo
        ticks_per_beat = mid.ticks_per_beat
        
        def ticks_to_seconds(ticks):
            return (ticks / ticks_per_beat) * (tempo / 1000000)
        
        tracks = []
        all_notes = []
        
        for track_idx, track in enumerate(mid.tracks):
            track_name = f"Track {track_idx}"
            for msg in track:
                if msg.type == 'track_name':
                    track_name = msg.name
                    break
            
            notes = []
            active_notes = {}  # note_number -> (start_time, velocity, channel)
            current_time = 0  # in ticks
            
            for msg in track:
                current_time += msg.time
                current_time_sec = ticks_to_seconds(current_time)
                
                if msg.type == 'note_on' and msg.velocity > 0:
                    # Note started
                    active_notes[msg.note] = (current_time_sec, msg.velocity, msg.channel)
                    
                elif msg.type == 'note_off' or (msg.type == 'note_on' and msg.velocity == 0):
                    # Note ended
                    if msg.note in active_notes:
                        start_time, velocity, channel = active_notes.pop(msg.note)
                        duration = current_time_sec - start_time
                        
                        note = MidiNote(
                            note_number=msg.note,
                            note_name=self.midi_note_to_name(msg.note),
                            start_time=start_time,
                            duration=max(0.1, duration),  # Minimum duration
                            velocity=velocity,
                            channel=channel
                        )
                        notes.append(note)
                        all_notes.append(note)
            
            if notes:
                tracks.append(MidiTrack(name=track_name, notes=notes))
        
        # Sort all notes by start time
        all_notes.sort(key=lambda n: (n.start_time, -n.note_number))
        
        # Calculate total duration
        total_duration = max((n.start_time + n.duration for n in all_notes), default=0)
        
        return ParsedMidi(
            filename=filepath.split('/')[-1].split('\\')[-1],
            tempo=bpm,
            duration=total_duration,
            tracks=tracks,
            all_notes=all_notes
        )
    
    def export_to_text(self, parsed: ParsedMidi) -> str:
        """Export parsed MIDI to readable text format"""
        lines = []
        lines.append(f"# MIDI File: {parsed.filename}")
        lines.append(f"# Tempo: {parsed.tempo:.1f} BPM")
        lines.append(f"# Duration: {parsed.duration:.2f} seconds")
        lines.append(f"# Total Notes: {len(parsed.all_notes)}")
        lines.append(f"# Tracks: {len(parsed.tracks)}")
        lines.append("")
        lines.append("# Format: TIME_SEC | NOTE | DURATION | VELOCITY")
        lines.append("# " + "=" * 50)
        lines.append("")
        
        for note in parsed.all_notes:
            lines.append(f"{note.start_time:8.3f} | {note.note_name:4s} | {note.duration:.3f}s | vel={note.velocity:3d}")
        
        return "\n".join(lines)
    
    def export_to_json(self, parsed: ParsedMidi) -> str:
        """Export parsed MIDI to JSON format"""
        data = {
            "filename": parsed.filename,
            "tempo_bpm": parsed.tempo,
            "duration_seconds": parsed.duration,
            "total_notes": len(parsed.all_notes),
            "notes": [
                {
                    "note": n.note_name,
                    "midi_number": n.note_number,
                    "start": round(n.start_time, 3),
                    "duration": round(n.duration, 3),
                    "velocity": n.velocity
                }
                for n in parsed.all_notes
            ]
        }
        return json.dumps(data, indent=2)


class HandSimulator:
    """Simulates playing with two hands (max 10 fingers)"""
    
    def __init__(self, max_fingers=MAX_FINGERS):
        self.max_fingers = max_fingers
        self.active_notes = []  # List of (note_name, end_time)
        
    def get_playable_notes(self, notes_at_time: List[MidiNote], current_time: float) -> List[MidiNote]:
        """
        Given notes that should play at current_time, return only those
        that can be played with available fingers.
        Prioritizes: melody (highest notes) and bass (lowest notes)
        """
        # Remove expired notes
        self.active_notes = [(n, t) for n, t in self.active_notes if t > current_time]
        
        available_fingers = self.max_fingers - len(self.active_notes)
        
        if available_fingers <= 0:
            return []
        
        if len(notes_at_time) <= available_fingers:
            # Can play all notes
            for note in notes_at_time:
                self.active_notes.append((note.note_name, current_time + note.duration))
            return notes_at_time
        
        # Need to prioritize - keep highest and lowest notes (melody + bass)
        sorted_notes = sorted(notes_at_time, key=lambda n: n.note_number)
        
        # Split available fingers between bass and melody
        bass_fingers = available_fingers // 3
        melody_fingers = available_fingers - bass_fingers
        
        selected = []
        # Add bass notes (lowest)
        selected.extend(sorted_notes[:bass_fingers])
        # Add melody notes (highest)
        selected.extend(sorted_notes[-melody_fingers:])
        
        # Remove duplicates while preserving order
        seen = set()
        unique_selected = []
        for note in selected:
            if note.note_name not in seen:
                seen.add(note.note_name)
                unique_selected.append(note)
                self.active_notes.append((note.note_name, current_time + note.duration))
        
        return unique_selected[:available_fingers]
    
    def reset(self):
        """Reset hand state"""
        self.active_notes = []


class PianoSynth:
    """Synthesizes piano-like sounds using pre-cached tones and a real-time mixer"""

    NOTE_NAMES = ['C', 'C#', 'D', 'D#', 'E', 'F', 'F#', 'G', 'G#', 'A', 'A#', 'B']

    def __init__(self, sample_rate=SAMPLE_RATE):
        self.sample_rate = sample_rate
        self.running = True
        self.tone_cache = {}  # Pre-generated tones for instant playback
        self.active_sounds = []  # Currently playing sounds: [(samples, position, velocity)]
        self.sound_lock = threading.Lock()
        self.stream = None

        # Try to import sounddevice
        try:
            import sounddevice as sd
            self.sd = sd
            self.audio_available = True
            self._init_audio_stream()
            self._precache_tones()
        except ImportError:
            self.audio_available = False
            print("Note: Install 'sounddevice' for audio: pip install sounddevice")
        except Exception as e:
            self.audio_available = False
            print(f"Audio init error: {e}")

    def _init_audio_stream(self):
        """Initialize a continuous audio output stream with mixer callback"""
        try:
            self.stream = self.sd.OutputStream(
                samplerate=self.sample_rate,
                channels=1,
                dtype='float32',
                callback=self._audio_callback,
                blocksize=512,  # Small buffer for low latency
                latency='low'
            )
            self.stream.start()
        except Exception as e:
            print(f"Stream init error: {e}")
            self.audio_available = False

    def _audio_callback(self, outdata, frames, time_info, status):
        """Real-time audio mixer callback - mixes all active sounds"""
        output = np.zeros(frames, dtype=np.float32)

        with self.sound_lock:
            still_active = []
            for samples, pos, vel in self.active_sounds:
                end_pos = pos + frames
                if pos < len(samples):
                    # Get the chunk to play
                    chunk_end = min(end_pos, len(samples))
                    chunk = samples[pos:chunk_end] * vel
                    chunk_len = len(chunk)
                    output[:chunk_len] += chunk

                    # Keep if not finished
                    if chunk_end < len(samples):
                        still_active.append((samples, chunk_end, vel))

            self.active_sounds = still_active

        # Soft limit to prevent clipping
        max_val = np.max(np.abs(output))
        if max_val > 0.95:
            output = output * 0.95 / max_val

        outdata[:, 0] = output

    def _precache_tones(self):
        """Pre-generate all piano tones for instant playback"""
        print("Caching piano tones...", end=" ", flush=True)
        for octave in range(2, 7):  # C2 to B6
            for note in self.NOTE_NAMES:
                note_name = f"{note}{octave}"
                freq = self.note_to_frequency(note_name)
                # Cache at standard velocity - will scale at playback
                self.tone_cache[note_name] = self._generate_tone(freq)
        print("Done!")

    def _generate_tone(self, frequency, duration=DURATION):
        """Generate a piano-like tone (called once per note during caching)"""
        t = np.linspace(0, duration, int(self.sample_rate * duration), False)

        tone = np.zeros_like(t)

        # Harmonic series (piano-like)
        harmonics = [1, 2, 3, 4, 5, 6, 7, 8]
        harmonic_amps = [1.0, 0.5, 0.35, 0.25, 0.15, 0.1, 0.08, 0.05]

        # Use deterministic "detuning" based on frequency for consistency
        np.random.seed(int(frequency) % 1000)
        for h, amp in zip(harmonics, harmonic_amps):
            detune = 1 + (np.random.random() - 0.5) * 0.001
            tone += amp * np.sin(2 * np.pi * frequency * h * detune * t)
        np.random.seed()  # Reset seed

        # ADSR Envelope
        attack_time = 0.008  # Faster attack for responsiveness
        decay_time = 0.12
        sustain_level = 0.35

        envelope = np.ones_like(t)
        attack_samples = int(attack_time * self.sample_rate)
        decay_samples = int(decay_time * self.sample_rate)

        if attack_samples > 0:
            envelope[:attack_samples] = np.linspace(0, 1, attack_samples)

        decay_end = attack_samples + decay_samples
        if decay_samples > 0 and decay_end < len(envelope):
            envelope[attack_samples:decay_end] = np.linspace(1, sustain_level, decay_samples)

        if decay_end < len(envelope):
            release_len = len(envelope) - decay_end
            envelope[decay_end:] = sustain_level * np.exp(-3 * np.linspace(0, 1, release_len))

        tone = tone * envelope * 0.3
        tone = np.tanh(tone)

        return tone.astype(np.float32)

    def note_to_frequency(self, note_name):
        """Convert note name to frequency"""
        if '#' in note_name:
            note = note_name[:2]
            octave = int(note_name[2:])
        else:
            note = note_name[0]
            octave = int(note_name[1:])

        note_index = self.NOTE_NAMES.index(note)
        semitones_from_a4 = (octave - 4) * 12 + (note_index - 9)
        frequency = 440 * (2 ** (semitones_from_a4 / 12))
        return frequency

    def play_note(self, frequency, velocity=0.7, duration=None, note_name=None):
        """Play a note instantly using cached tone"""
        if not self.audio_available:
            return

        # Find cached tone by note name or frequency
        if note_name and note_name in self.tone_cache:
            samples = self.tone_cache[note_name]
        else:
            # Find closest cached note by frequency
            closest_note = None
            min_diff = float('inf')
            for name, cached_samples in self.tone_cache.items():
                cached_freq = self.note_to_frequency(name)
                diff = abs(cached_freq - frequency)
                if diff < min_diff:
                    min_diff = diff
                    closest_note = name

            if closest_note and min_diff < 1:  # Within 1 Hz
                samples = self.tone_cache[closest_note]
            else:
                # Generate on the fly as fallback (shouldn't happen often)
                samples = self._generate_tone(frequency)

        # Add to active sounds
        with self.sound_lock:
            self.active_sounds.append((samples, 0, velocity))

    def play_chord(self, frequencies, velocities=None, duration=None, note_names=None):
        """Play multiple notes simultaneously"""
        if not self.audio_available or not frequencies:
            return

        if velocities is None:
            velocities = [0.7] * len(frequencies)

        if note_names is None:
            note_names = [None] * len(frequencies)

        # Add all notes to mixer
        for freq, vel, name in zip(frequencies, velocities, note_names):
            self.play_note(freq, vel, duration, name)

    def stop(self):
        """Stop the audio stream"""
        self.running = False
        if self.stream:
            try:
                self.stream.stop()
                self.stream.close()
            except:
                pass


class VirtualPiano:
    """Main piano application with GUI"""
    
    # Standard piano frequencies (A4 = 440Hz)
    NOTE_NAMES = ['C', 'C#', 'D', 'D#', 'E', 'F', 'F#', 'G', 'G#', 'A', 'A#', 'B']
    
    def __init__(self):
        self.root = tk.Tk()
        self.root.title("🎹 Virtual Piano with MIDI Support")
        self.root.configure(bg='#1a1a2e')
        self.root.resizable(True, False)
        
        self.synth = PianoSynth()
        self.midi_parser = MidiParser()
        self.hand_simulator = HandSimulator(MAX_FINGERS)
        
        self.keys = {}  # Store key widgets
        self.key_bindings = {}  # Keyboard to note mapping
        self.active_keys = set()  # Currently pressed keys
        
        # MIDI playback state
        self.parsed_midi = None
        self.is_playing = False
        self.playback_thread = None
        self.playback_paused = False
        self.playback_position = 0.0
        
        # Piano range: C3 to C6 (37 keys like many electric keyboards)
        self.start_octave = 3
        self.num_octaves = 3
        
        self.setup_ui()
        self.setup_keyboard_bindings()
        
    def note_to_frequency(self, note_name, octave):
        """Convert note name and octave to frequency"""
        note_index = self.NOTE_NAMES.index(note_name)
        semitones_from_a4 = (octave - 4) * 12 + (note_index - 9)
        frequency = 440 * (2 ** (semitones_from_a4 / 12))
        return frequency
    
    def setup_ui(self):
        """Create the piano GUI"""
        # Main container
        main_frame = tk.Frame(self.root, bg='#1a1a2e')
        main_frame.pack(fill='both', expand=True, padx=10, pady=10)
        
        # Title
        title_frame = tk.Frame(main_frame, bg='#1a1a2e')
        title_frame.pack(pady=(0, 10))
        
        title = tk.Label(
            title_frame, 
            text="🎹 Virtual Piano", 
            font=('Helvetica', 24, 'bold'),
            fg='#eee',
            bg='#1a1a2e'
        )
        title.pack()
        
        subtitle = tk.Label(
            title_frame,
            text="Play with mouse/keyboard or load a MIDI file",
            font=('Helvetica', 10),
            fg='#888',
            bg='#1a1a2e'
        )
        subtitle.pack()
        
        # MIDI Controls Frame
        midi_frame = tk.LabelFrame(main_frame, text="MIDI Controls", 
                                   bg='#1a1a2e', fg='#aaa', font=('Helvetica', 10))
        midi_frame.pack(fill='x', pady=10)
        
        # Button row
        btn_frame = tk.Frame(midi_frame, bg='#1a1a2e')
        btn_frame.pack(fill='x', padx=10, pady=5)
        
        self.load_btn = tk.Button(
            btn_frame, text="📂 Load MIDI", command=self.load_midi_file,
            bg='#3a3a5a', fg='white', font=('Helvetica', 10),
            relief='flat', padx=15, pady=5
        )
        self.load_btn.pack(side='left', padx=5)
        
        self.play_btn = tk.Button(
            btn_frame, text="▶ Play", command=self.toggle_playback,
            bg='#2d5a2d', fg='white', font=('Helvetica', 10),
            relief='flat', padx=15, pady=5, state='disabled'
        )
        self.play_btn.pack(side='left', padx=5)
        
        self.stop_btn = tk.Button(
            btn_frame, text="⏹ Stop", command=self.stop_playback,
            bg='#5a2d2d', fg='white', font=('Helvetica', 10),
            relief='flat', padx=15, pady=5, state='disabled'
        )
        self.stop_btn.pack(side='left', padx=5)
        
        self.export_btn = tk.Button(
            btn_frame, text="💾 Export Notes", command=self.export_midi,
            bg='#3a3a5a', fg='white', font=('Helvetica', 10),
            relief='flat', padx=15, pady=5, state='disabled'
        )
        self.export_btn.pack(side='left', padx=5)
        
        # Hand simulation toggle
        self.hand_sim_var = tk.BooleanVar(value=True)
        hand_sim_cb = tk.Checkbutton(
            btn_frame, text=f"👐 Hand Simulation (max {MAX_FINGERS} keys)",
            variable=self.hand_sim_var, bg='#1a1a2e', fg='#aaa',
            selectcolor='#2a2a4a', font=('Helvetica', 9)
        )
        hand_sim_cb.pack(side='left', padx=15)
        
        # MIDI info label
        self.midi_info = tk.Label(
            midi_frame, text="No MIDI loaded",
            font=('Helvetica', 9), fg='#666', bg='#1a1a2e'
        )
        self.midi_info.pack(pady=5)
        
        # Progress bar
        self.progress_var = tk.DoubleVar(value=0)
        self.progress = ttk.Progressbar(
            midi_frame, variable=self.progress_var, maximum=100, length=400
        )
        self.progress.pack(pady=5, padx=10, fill='x')
        
        # Current note display
        self.current_notes_label = tk.Label(
            midi_frame, text="Current: ---",
            font=('Courier', 10), fg='#4a9eff', bg='#1a1a2e'
        )
        self.current_notes_label.pack(pady=5)
        
        # Piano frame
        piano_frame = tk.Frame(main_frame, bg='#2d2d44', padx=10, pady=10)
        piano_frame.pack(padx=10, pady=10)
        
        # Key dimensions
        white_width = 45
        white_height = 180
        black_width = 28
        black_height = 110
        
        # Canvas for piano keys
        num_white_keys = self.num_octaves * 7 + 1
        canvas_width = num_white_keys * white_width + 20
        canvas_height = white_height + 20
        
        self.canvas = tk.Canvas(
            piano_frame, 
            width=canvas_width, 
            height=canvas_height,
            bg='#2d2d44',
            highlightthickness=0
        )
        self.canvas.pack()
        
        # Draw white keys first
        all_notes = []
        
        for octave in range(self.start_octave, self.start_octave + self.num_octaves + 1):
            for note in self.NOTE_NAMES:
                if octave == self.start_octave + self.num_octaves and note != 'C':
                    break
                all_notes.append((note, octave))
        
        # Separate white and black keys
        white_notes = [(n, o) for n, o in all_notes if '#' not in n]
        black_notes = [(n, o) for n, o in all_notes if '#' in n]
        
        # Draw white keys
        for i, (note, octave) in enumerate(white_notes):
            x1 = 10 + i * white_width
            y1 = 10
            x2 = x1 + white_width - 2
            y2 = y1 + white_height
            
            freq = self.note_to_frequency(note, octave)
            key_id = self.canvas.create_rectangle(
                x1, y1, x2, y2,
                fill='#fefefe',
                outline='#333',
                width=1,
                tags=f'white_{note}{octave}'
            )
            
            label = self.canvas.create_text(
                (x1 + x2) / 2, y2 - 15,
                text=f'{note}{octave}',
                font=('Helvetica', 8),
                fill='#666',
                tags=f'label_{note}{octave}'
            )
            
            self.keys[f'{note}{octave}'] = {
                'id': key_id,
                'label': label,
                'freq': freq,
                'is_black': False,
                'coords': (x1, y1, x2, y2)
            }
            
            self.canvas.tag_bind(key_id, '<Button-1>', lambda e, f=freq, n=f'{note}{octave}': self.on_key_press(f, n))
            self.canvas.tag_bind(key_id, '<ButtonRelease-1>', lambda e, n=f'{note}{octave}': self.on_key_release(n))
        
        # Draw black keys
        for octave in range(self.start_octave, self.start_octave + self.num_octaves):
            for note in self.NOTE_NAMES:
                if '#' not in note:
                    continue
                
                base_note = note[0]
                white_index = sum(1 for n, o in white_notes if o < octave or (o == octave and self.NOTE_NAMES.index(n.replace('#', '')) < self.NOTE_NAMES.index(base_note)))
                
                x_center = 10 + (white_index + 1) * white_width - 1
                x1 = x_center - black_width // 2
                y1 = 10
                x2 = x1 + black_width
                y2 = y1 + black_height
                
                freq = self.note_to_frequency(note, octave)
                key_id = self.canvas.create_rectangle(
                    x1, y1, x2, y2,
                    fill='#1a1a1a',
                    outline='#000',
                    width=1,
                    tags=f'black_{note}{octave}'
                )
                
                self.keys[f'{note}{octave}'] = {
                    'id': key_id,
                    'freq': freq,
                    'is_black': True,
                    'coords': (x1, y1, x2, y2)
                }
                
                self.canvas.tag_bind(key_id, '<Button-1>', lambda e, f=freq, n=f'{note}{octave}': self.on_key_press(f, n))
                self.canvas.tag_bind(key_id, '<ButtonRelease-1>', lambda e, n=f'{note}{octave}': self.on_key_release(n))
        
        # Keyboard mapping display
        mapping_frame = tk.Frame(main_frame, bg='#1a1a2e')
        mapping_frame.pack(pady=10)
        
        mapping_label = tk.Label(
            mapping_frame,
            text="Keyboard Mapping:",
            font=('Helvetica', 10, 'bold'),
            fg='#aaa',
            bg='#1a1a2e'
        )
        mapping_label.pack()
        
        mapping_text = tk.Label(
            mapping_frame,
            text="White: A S D F G H J K L ; ' (C4-C5)  |  Black: W E T Y U O P (sharps)\n"
                 "Lower: Z X C V B N M , . / (C3-E4)   |  Upper: 1-0 (C5-E6)",
            font=('Courier', 9),
            fg='#888',
            bg='#1a1a2e',
            justify='left'
        )
        mapping_text.pack()
        
        # Status bar
        self.status_label = tk.Label(
            main_frame,
            text="Ready | Hold Shift for louder notes | Press Esc to quit",
            font=('Helvetica', 9),
            fg='#666',
            bg='#1a1a2e'
        )
        self.status_label.pack(pady=(0, 5))
        
    def setup_keyboard_bindings(self):
        """Setup computer keyboard to piano key mappings"""
        # Lower row: C3 to E4
        lower_keys = ['z', 'x', 'c', 'v', 'b', 'n', 'm', 'comma', 'period', 'slash']
        lower_notes = [('C', 3), ('D', 3), ('E', 3), ('F', 3), ('G', 3), 
                       ('A', 3), ('B', 3), ('C', 4), ('D', 4), ('E', 4)]
        
        # Middle row: C4 to C5 (white keys)
        middle_keys = ['a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'semicolon', 'apostrophe']
        middle_notes = [('C', 4), ('D', 4), ('E', 4), ('F', 4), ('G', 4),
                        ('A', 4), ('B', 4), ('C', 5), ('D', 5), ('E', 5), ('F', 5)]
        
        # Top row: black keys
        top_keys = ['w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', 'bracketleft']
        top_notes = [('C#', 4), ('D#', 4), ('F#', 4), ('G#', 4), ('A#', 4),
                     ('C#', 5), ('D#', 5), ('F#', 5), ('G#', 5), ('A#', 5)]
        
        # Number row: C5 to E6
        number_keys = ['1', '2', '3', '4', '5', '6', '7', '8', '9', '0']
        number_notes = [('C', 5), ('D', 5), ('E', 5), ('F', 5), ('G', 5),
                        ('A', 5), ('B', 5), ('C', 6), ('D', 6), ('E', 6)]
        
        all_mappings = [(lower_keys, lower_notes), (middle_keys, middle_notes),
                        (top_keys, top_notes), (number_keys, number_notes)]
        
        for keys, notes in all_mappings:
            for key, (note, octave) in zip(keys, notes):
                note_key = f'{note}{octave}'
                if note_key in self.keys:
                    self.key_bindings[key] = note_key
        
        self.root.bind('<KeyPress>', self.on_keyboard_press)
        self.root.bind('<KeyRelease>', self.on_keyboard_release)
        self.root.bind('<Escape>', lambda e: self.root.quit())
        self.root.bind('<space>', lambda e: self.toggle_playback())
        
    def on_key_press(self, frequency, note_name):
        """Handle mouse click on piano key"""
        if note_name not in self.active_keys:
            self.active_keys.add(note_name)
            self.highlight_key(note_name, True)
            self.synth.play_note(frequency, note_name=note_name)
    
    def on_key_release(self, note_name):
        """Handle mouse release on piano key"""
        if note_name in self.active_keys:
            self.active_keys.discard(note_name)
            self.highlight_key(note_name, False)
    
    def on_keyboard_press(self, event):
        """Handle computer keyboard press"""
        key = event.keysym.lower()

        if event.char in [',', '.', '/', ';', "'", '[']:
            key = {',': 'comma', '.': 'period', '/': 'slash',
                   ';': 'semicolon', "'": 'apostrophe', '[': 'bracketleft'}.get(event.char, key)

        if key in self.key_bindings:
            note_name = self.key_bindings[key]
            if note_name not in self.active_keys:
                self.active_keys.add(note_name)
                freq = self.keys[note_name]['freq']
                velocity = 0.9 if event.state & 0x1 else 0.7
                self.highlight_key(note_name, True)
                self.synth.play_note(freq, velocity, note_name=note_name)
    
    def on_keyboard_release(self, event):
        """Handle computer keyboard release"""
        key = event.keysym.lower()
        
        if event.char in [',', '.', '/', ';', "'", '[']:
            key = {',': 'comma', '.': 'period', '/': 'slash',
                   ';': 'semicolon', "'": 'apostrophe', '[': 'bracketleft'}.get(event.char, key)
        
        if key in self.key_bindings:
            note_name = self.key_bindings[key]
            self.active_keys.discard(note_name)
            self.highlight_key(note_name, False)
    
    def highlight_key(self, note_name, pressed):
        """Visually highlight/unhighlight a piano key"""
        if note_name not in self.keys:
            return
            
        key_info = self.keys[note_name]
        key_id = key_info['id']
        is_black = key_info['is_black']
        
        if pressed:
            color = '#4a9eff' if is_black else '#87CEEB'
        else:
            color = '#1a1a1a' if is_black else '#fefefe'
        
        self.canvas.itemconfig(key_id, fill=color)
    
    def load_midi_file(self):
        """Open file dialog to load MIDI file"""
        filepath = filedialog.askopenfilename(
            title="Select MIDI File",
            filetypes=[("MIDI Files", "*.mid *.midi"), ("All Files", "*.*")]
        )
        
        if not filepath:
            return
        
        try:
            self.parsed_midi = self.midi_parser.parse_file(filepath)
            
            # Update UI
            self.midi_info.config(
                text=f"📄 {self.parsed_midi.filename} | "
                     f"⏱ {self.parsed_midi.duration:.1f}s | "
                     f"🎵 {len(self.parsed_midi.all_notes)} notes | "
                     f"🎼 {self.parsed_midi.tempo:.0f} BPM"
            )
            
            self.play_btn.config(state='normal')
            self.export_btn.config(state='normal')
            self.progress_var.set(0)
            self.status_label.config(text=f"Loaded: {self.parsed_midi.filename}")
            
        except ImportError as e:
            messagebox.showerror("Missing Module", 
                "MIDI support requires the 'mido' library.\n\n"
                "Install it with: pip install mido")
        except Exception as e:
            messagebox.showerror("Error", f"Failed to load MIDI file:\n{e}")
    
    def toggle_playback(self):
        """Start or pause MIDI playback"""
        if not self.parsed_midi:
            return
        
        if self.is_playing:
            self.playback_paused = not self.playback_paused
            self.play_btn.config(text="▶ Resume" if self.playback_paused else "⏸ Pause")
        else:
            self.start_playback()
    
    def start_playback(self):
        """Start playing the loaded MIDI file"""
        if not self.parsed_midi:
            return
        
        self.is_playing = True
        self.playback_paused = False
        self.playback_position = 0.0
        self.hand_simulator.reset()
        
        self.play_btn.config(text="⏸ Pause")
        self.stop_btn.config(state='normal')
        self.load_btn.config(state='disabled')
        
        self.playback_thread = threading.Thread(target=self._playback_loop, daemon=True)
        self.playback_thread.start()
    
    def stop_playback(self):
        """Stop MIDI playback"""
        self.is_playing = False
        self.playback_paused = False
        
        self.play_btn.config(text="▶ Play", state='normal' if self.parsed_midi else 'disabled')
        self.stop_btn.config(state='disabled')
        self.load_btn.config(state='normal')
        
        # Clear all highlighted keys
        for note_name in list(self.active_keys):
            self.highlight_key(note_name, False)
        self.active_keys.clear()
        
        self.current_notes_label.config(text="Current: ---")
        self.progress_var.set(0)
    
    def _playback_loop(self):
        """Background thread for MIDI playback"""
        if not self.parsed_midi:
            return
        
        notes = self.parsed_midi.all_notes
        total_duration = self.parsed_midi.duration
        
        # Group notes by approximate start time (10ms resolution)
        note_groups = defaultdict(list)
        for note in notes:
            time_bucket = round(note.start_time, 2)
            note_groups[time_bucket].append(note)
        
        sorted_times = sorted(note_groups.keys())
        
        start_time = time.time()
        time_index = 0
        
        while self.is_playing and time_index < len(sorted_times):
            if self.playback_paused:
                pause_start = time.time()
                while self.playback_paused and self.is_playing:
                    time.sleep(0.01)
                start_time += time.time() - pause_start
                continue
            
            current_time = time.time() - start_time
            target_time = sorted_times[time_index]
            
            # Wait until it's time to play these notes
            if current_time < target_time:
                time.sleep(0.005)
                continue
            
            # Get notes at this time
            notes_to_play = note_groups[target_time]
            
            # Apply hand simulation if enabled
            if self.hand_sim_var.get():
                notes_to_play = self.hand_simulator.get_playable_notes(notes_to_play, target_time)
            
            if notes_to_play:
                # Play the notes
                frequencies = []
                velocities = []
                note_names_playing = []
                
                for note in notes_to_play:
                    freq = self.synth.note_to_frequency(note.note_name)
                    vel = note.velocity / 127.0
                    frequencies.append(freq)
                    velocities.append(vel)
                    note_names_playing.append(note.note_name)
                
                # Play chord with note names for fast cache lookup
                avg_duration = sum(n.duration for n in notes_to_play) / len(notes_to_play)
                self.synth.play_chord(frequencies, velocities, duration=min(avg_duration, 2.0), note_names=note_names_playing)
                
                # Update UI (thread-safe)
                def update_ui():
                    # Clear previous highlights
                    for n in list(self.active_keys):
                        if n not in note_names_playing:
                            self.highlight_key(n, False)
                            self.active_keys.discard(n)
                    
                    # Highlight current notes
                    for n in note_names_playing:
                        if n in self.keys:
                            self.highlight_key(n, True)
                            self.active_keys.add(n)
                    
                    # Update labels
                    display_notes = note_names_playing[:10]
                    self.current_notes_label.config(
                        text=f"Current: {' '.join(display_notes)}" + 
                             (f" (+{len(note_names_playing)-10})" if len(note_names_playing) > 10 else "")
                    )
                    
                    progress = (current_time / total_duration) * 100 if total_duration > 0 else 0
                    self.progress_var.set(min(progress, 100))
                
                self.root.after(0, update_ui)
            
            time_index += 1
        
        # Playback finished
        self.root.after(0, self.stop_playback)
    
    def export_midi(self):
        """Export parsed MIDI notes to file"""
        if not self.parsed_midi:
            return
        
        filepath = filedialog.asksaveasfilename(
            title="Export MIDI Notes",
            defaultextension=".txt",
            filetypes=[("Text File", "*.txt"), ("JSON File", "*.json"), ("All Files", "*.*")]
        )
        
        if not filepath:
            return
        
        try:
            if filepath.endswith('.json'):
                content = self.midi_parser.export_to_json(self.parsed_midi)
            else:
                content = self.midi_parser.export_to_text(self.parsed_midi)
            
            with open(filepath, 'w') as f:
                f.write(content)
            
            self.status_label.config(text=f"Exported to: {filepath.split('/')[-1]}")
            messagebox.showinfo("Export Complete", f"Notes exported to:\n{filepath}")
            
        except Exception as e:
            messagebox.showerror("Export Error", f"Failed to export:\n{e}")
    
    def run(self):
        """Start the piano application"""
        if not self.synth.audio_available:
            messagebox.showwarning(
                "Audio Not Available",
                "The 'sounddevice' module is not installed.\n\n"
                "To enable sound, install it with:\n"
                "pip install sounddevice\n\n"
                "The piano will still display but won't make sound."
            )

        if not self.midi_parser.mido:
            messagebox.showinfo(
                "MIDI Support",
                "For MIDI file support, install the 'mido' library:\n\n"
                "pip install mido"
            )

        # Cleanup on window close
        def on_close():
            self.is_playing = False
            self.synth.stop()
            self.root.destroy()

        self.root.protocol("WM_DELETE_WINDOW", on_close)
        self.root.mainloop()


def main():
    """Main entry point"""
    print("=" * 60)
    print("🎹 Virtual Piano with MIDI Support")
    print("=" * 60)
    print("\nFeatures:")
    print("  • Play with mouse or keyboard")
    print("  • Load and play MIDI files")
    print("  • Hand simulation (max 10 simultaneous keys)")
    print("  • Export MIDI notes to text/JSON")
    print("\nRequired packages:")
    print("  pip install numpy sounddevice mido")
    print("-" * 60)
    
    try:
        piano = VirtualPiano()
        piano.run()
    except Exception as e:
        print(f"Error: {e}")
        import traceback
        traceback.print_exc()


if __name__ == "__main__":
    main()