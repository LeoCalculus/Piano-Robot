"""
MIDI to STM32 Linked List Converter
Converts MIDI files to C code for piano playing robot

Structure:
- Each linked list node represents a "chord event" (notes played simultaneously)
- Each node contains: positions array, durations array, note count
- Positions use {NOTE}_POS macros for easy hardware mapping

Usage:
    python midi_to_stm32.py input.mid [output_name]
    python midi_to_stm32.py --from-txt viva.txt [output_name]
"""

import sys
import re
from collections import defaultdict
from dataclasses import dataclass
from typing import List, Dict, Tuple, Optional


# Maximum simultaneous notes (fingers/actuators)
MAX_FINGERS = 10


@dataclass
class MidiNote:
    """Represents a single MIDI note event"""
    note_number: int
    note_name: str
    start_time: float
    duration: float
    velocity: int
    channel: int = 0

    def __repr__(self):
        return f"{self.note_name}(t={self.start_time:.3f}s, dur={self.duration:.3f}s)"


@dataclass
class ChordEvent:
    """Represents notes played at the same time"""
    start_time: float
    notes: List[MidiNote]
    
    @property
    def duration_ms(self) -> int:
        """Get average duration in milliseconds"""
        if not self.notes:
            return 0
        avg_dur = sum(n.duration for n in self.notes) / len(self.notes)
        return int(avg_dur * 1000)
    
    @property
    def delay_to_next_ms(self) -> int:
        """Placeholder - will be calculated when building sequence"""
        return 0


class MidiParser:
    """Parses MIDI files into note sequences"""
    
    NOTE_NAMES = ['C', 'C#', 'D', 'D#', 'E', 'F', 'F#', 'G', 'G#', 'A', 'A#', 'B']
    
    def __init__(self):
        self.mido = None
        try:
            import mido
            self.mido = mido
        except ImportError:
            pass
    
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
        match = re.match(r'^([A-G]#?)(-?\d+)$', name.strip())
        if not match:
            raise ValueError(f"Invalid note name: {name}")
        note = match.group(1)
        octave = int(match.group(2))
        note_index = MidiParser.NOTE_NAMES.index(note)
        return (octave + 1) * 12 + note_index
    
    def parse_midi_file(self, filepath: str) -> List[MidiNote]:
        """Parse a MIDI file and return list of notes"""
        if not self.mido:
            raise ImportError("mido library not installed. Run: pip install mido")
        
        mid = self.mido.MidiFile(filepath)
        
        # Get tempo
        tempo = 500000  # default microseconds per beat
        for track in mid.tracks:
            for msg in track:
                if msg.type == 'set_tempo':
                    tempo = msg.tempo
                    break
        
        ticks_per_beat = mid.ticks_per_beat
        
        def ticks_to_seconds(ticks):
            return (ticks / ticks_per_beat) * (tempo / 1000000)
        
        all_notes = []
        
        for track in mid.tracks:
            active_notes = {}
            current_time = 0
            
            for msg in track:
                current_time += msg.time
                current_time_sec = ticks_to_seconds(current_time)
                
                if msg.type == 'note_on' and msg.velocity > 0:
                    active_notes[msg.note] = (current_time_sec, msg.velocity, msg.channel)
                    
                elif msg.type == 'note_off' or (msg.type == 'note_on' and msg.velocity == 0):
                    if msg.note in active_notes:
                        start_time, velocity, channel = active_notes.pop(msg.note)
                        duration = current_time_sec - start_time
                        
                        note = MidiNote(
                            note_number=msg.note,
                            note_name=self.midi_note_to_name(msg.note),
                            start_time=start_time,
                            duration=max(0.05, duration),
                            velocity=velocity,
                            channel=channel
                        )
                        all_notes.append(note)
        
        all_notes.sort(key=lambda n: (n.start_time, -n.note_number))
        return all_notes
    
    def parse_txt_file(self, filepath: str) -> List[MidiNote]:
        """Parse exported TXT file (from piano.py export)"""
        notes = []
        
        with open(filepath, 'r') as f:
            for line in f:
                line = line.strip()
                # Skip comments and empty lines
                if not line or line.startswith('#'):
                    continue
                
                # Parse format: "   0.000 | C#5  | 0.236s | vel= 80"
                match = re.match(
                    r'^\s*([\d.]+)\s*\|\s*([A-G]#?\d+)\s*\|\s*([\d.]+)s?\s*\|\s*vel\s*=\s*(\d+)',
                    line
                )
                if match:
                    start_time = float(match.group(1))
                    note_name = match.group(2).strip()
                    duration = float(match.group(3))
                    velocity = int(match.group(4))
                    
                    note = MidiNote(
                        note_number=self.name_to_midi_note(note_name),
                        note_name=note_name,
                        start_time=start_time,
                        duration=duration,
                        velocity=velocity
                    )
                    notes.append(note)
        
        notes.sort(key=lambda n: (n.start_time, -n.note_number))
        return notes


def group_notes_to_chords(notes: List[MidiNote], time_tolerance: float = 0.02) -> List[ChordEvent]:
    """
    Group notes that start at approximately the same time into chord events.
    
    Args:
        notes: List of notes sorted by start time
        time_tolerance: Maximum time difference (seconds) to consider notes as simultaneous
    
    Returns:
        List of ChordEvent objects
    """
    if not notes:
        return []
    
    chords = []
    current_chord_notes = [notes[0]]
    current_chord_time = notes[0].start_time
    
    for note in notes[1:]:
        if abs(note.start_time - current_chord_time) <= time_tolerance:
            # Same chord
            current_chord_notes.append(note)
        else:
            # New chord - save current and start new
            chords.append(ChordEvent(
                start_time=current_chord_time,
                notes=current_chord_notes[:MAX_FINGERS]  # Limit to max fingers
            ))
            current_chord_notes = [note]
            current_chord_time = note.start_time
    
    # Don't forget the last chord
    if current_chord_notes:
        chords.append(ChordEvent(
            start_time=current_chord_time,
            notes=current_chord_notes[:MAX_FINGERS]
        ))
    
    return chords


def note_name_to_pos_macro(note_name: str) -> str:
    """
    Convert note name to position macro name.
    E.g., "C#4" -> "CS4_POS", "G4" -> "G4_POS"
    """
    # Replace # with S (sharp)
    macro_name = note_name.replace('#', 'S')
    return f"{macro_name}_POS"


def note_name_to_enum(note_name: str) -> str:
    """
    Convert note name to enum name for note identification.
    E.g., "C#4" -> "NOTE_CS4"
    """
    macro_name = note_name.replace('#', 'S')
    return f"NOTE_{macro_name}"


def generate_prototype_txt(chords: List[ChordEvent], output_path: str):
    """
    Generate a human-readable prototype showing the linked list structure.
    """
    lines = []
    lines.append("=" * 70)
    lines.append("PIANO ROBOT LINKED LIST PROTOTYPE")
    lines.append("=" * 70)
    lines.append(f"Total chord events: {len(chords)}")
    lines.append(f"Max simultaneous notes: {MAX_FINGERS}")
    lines.append("")
    lines.append("Format: [INDEX] TIME -> NOTES (DURATION) -> DELAY_TO_NEXT")
    lines.append("-" * 70)
    lines.append("")
    
    for i, chord in enumerate(chords):
        # Calculate delay to next chord
        if i < len(chords) - 1:
            delay_ms = int((chords[i+1].start_time - chord.start_time) * 1000)
        else:
            delay_ms = 0
        
        note_names = [n.note_name for n in chord.notes]
        pos_macros = [note_name_to_pos_macro(n) for n in note_names]
        
        lines.append(f"[{i:4d}] t={chord.start_time:8.3f}s")
        lines.append(f"       Notes: {', '.join(note_names)}")
        lines.append(f"       Positions: {', '.join(pos_macros)}")
        lines.append(f"       Duration: {chord.duration_ms}ms")
        lines.append(f"       Delay to next: {delay_ms}ms")
        lines.append("")
    
    with open(output_path, 'w') as f:
        f.write('\n'.join(lines))
    
    print(f"Generated prototype: {output_path}")


def generate_c_header(chords: List[ChordEvent], output_path: str, name: str = "song"):
    """
    Generate C header file with:
    - Position macro definitions (placeholders)
    - Note enum
    - Linked list structure definition
    """
    # Collect all unique notes used
    all_notes = set()
    for chord in chords:
        for note in chord.notes:
            all_notes.add(note.note_name)
    
    # Sort notes by MIDI number for logical ordering
    sorted_notes = sorted(all_notes, key=lambda n: MidiParser.name_to_midi_note(n))
    
    lines = []
    lines.append("/**")
    lines.append(f" * @file {name}_data.h")
    lines.append(" * @brief Piano robot song data - Auto-generated from MIDI")
    lines.append(" * ")
    lines.append(f" * Total chord events: {len(chords)}")
    lines.append(f" * Unique notes: {len(sorted_notes)}")
    lines.append(" */")
    lines.append("")
    lines.append("#ifndef " + name.upper() + "_DATA_H")
    lines.append("#define " + name.upper() + "_DATA_H")
    lines.append("")
    lines.append("#include <stdint.h>")
    lines.append("")
    lines.append("/* ============================================================")
    lines.append(" * POSITION MACROS - DEFINE THESE BASED ON YOUR HARDWARE!")
    lines.append(" * Each macro should be the physical position/step count for")
    lines.append(" * the robot actuator to press that piano key.")
    lines.append(" * ============================================================ */")
    lines.append("")
    
    # Generate position macros
    for note in sorted_notes:
        macro = note_name_to_pos_macro(note)
        midi_num = MidiParser.name_to_midi_note(note)
        # Default value based on MIDI number (can be adjusted)
        lines.append(f"#ifndef {macro}")
        lines.append(f"#define {macro:<12} ({midi_num - 36}U)  /* {note} - MIDI {midi_num} */")
        lines.append(f"#endif")
        lines.append("")
    
    lines.append("")
    lines.append("/* ============================================================")
    lines.append(" * DATA STRUCTURES")
    lines.append(" * ============================================================ */")
    lines.append("")
    lines.append(f"#define MAX_CHORD_NOTES    {MAX_FINGERS}")
    lines.append(f"#define {name.upper()}_TOTAL_EVENTS  {len(chords)}")
    lines.append("")
    lines.append("/**")
    lines.append(" * @brief Represents a chord event (notes played simultaneously)")
    lines.append(" */")
    lines.append("typedef struct ChordEvent {")
    lines.append("    uint8_t  num_notes;                    /* Number of notes in this chord */")
    lines.append("    uint8_t  positions[MAX_CHORD_NOTES];   /* Key positions to press */")
    lines.append("    uint16_t duration_ms;                  /* How long to hold the keys */")
    lines.append("    uint16_t delay_to_next_ms;             /* Delay before next chord */")
    lines.append("} ChordEvent_t;")
    lines.append("")
    lines.append("/**")
    lines.append(" * @brief Linked list node for chord sequence")
    lines.append(" */")
    lines.append("typedef struct ChordNode {")
    lines.append("    ChordEvent_t event;")
    lines.append("    struct ChordNode* next;")
    lines.append("} ChordNode_t;")
    lines.append("")
    lines.append("/* ============================================================")
    lines.append(" * FUNCTION DECLARATIONS")
    lines.append(" * ============================================================ */")
    lines.append("")
    lines.append(f"extern const ChordEvent_t {name}_data[{name.upper()}_TOTAL_EVENTS];")
    lines.append("")
    lines.append(f"ChordNode_t* {name}_create_linked_list(void);")
    lines.append(f"void {name}_free_linked_list(ChordNode_t* head);")
    lines.append(f"uint32_t {name}_get_total_duration_ms(void);")
    lines.append("")
    lines.append("#endif /* " + name.upper() + "_DATA_H */")
    
    with open(output_path, 'w') as f:
        f.write('\n'.join(lines))
    
    print(f"Generated header: {output_path}")


def generate_c_source(chords: List[ChordEvent], output_path: str, name: str = "song"):
    """
    Generate C source file with the chord data array and helper functions.
    """
    lines = []
    lines.append("/**")
    lines.append(f" * @file {name}_data.c")
    lines.append(" * @brief Piano robot song data - Auto-generated from MIDI")
    lines.append(" */")
    lines.append("")
    lines.append(f'#include "{name}_data.h"')
    lines.append("#include <stdlib.h>")
    lines.append("")
    lines.append("/* ============================================================")
    lines.append(" * CHORD DATA ARRAY")
    lines.append(" * Each entry: {num_notes, {positions...}, duration_ms, delay_to_next_ms}")
    lines.append(" * ============================================================ */")
    lines.append("")
    lines.append(f"const ChordEvent_t {name}_data[{name.upper()}_TOTAL_EVENTS] = {{")
    
    total_duration = 0
    
    for i, chord in enumerate(chords):
        # Calculate delay to next
        if i < len(chords) - 1:
            delay_ms = int((chords[i+1].start_time - chord.start_time) * 1000)
        else:
            delay_ms = 0
        
        total_duration += delay_ms
        
        num_notes = len(chord.notes)
        pos_macros = [note_name_to_pos_macro(n.note_name) for n in chord.notes]
        
        # Pad positions array to MAX_FINGERS
        while len(pos_macros) < MAX_FINGERS:
            pos_macros.append("0")
        
        positions_str = ", ".join(pos_macros)
        
        # Format comment with note names
        note_names = ", ".join(n.note_name for n in chord.notes)
        
        lines.append(f"    /* [{i:4d}] t={chord.start_time:.3f}s: {note_names} */")
        lines.append(f"    {{{num_notes}, {{{positions_str}}}, {chord.duration_ms}, {delay_ms}}},")
        lines.append("")
    
    lines.append("};")
    lines.append("")
    lines.append("/* ============================================================")
    lines.append(" * HELPER FUNCTIONS")
    lines.append(" * ============================================================ */")
    lines.append("")
    lines.append("/**")
    lines.append(" * @brief Create a linked list from the chord data array")
    lines.append(" * @return Pointer to head of linked list, or NULL on failure")
    lines.append(" */")
    lines.append(f"ChordNode_t* {name}_create_linked_list(void) {{")
    lines.append(f"    if ({name.upper()}_TOTAL_EVENTS == 0) return NULL;")
    lines.append("")
    lines.append("    ChordNode_t* head = (ChordNode_t*)malloc(sizeof(ChordNode_t));")
    lines.append("    if (!head) return NULL;")
    lines.append("")
    lines.append(f"    head->event = {name}_data[0];")
    lines.append("    head->next = NULL;")
    lines.append("")
    lines.append("    ChordNode_t* current = head;")
    lines.append(f"    for (uint32_t i = 1; i < {name.upper()}_TOTAL_EVENTS; i++) {{")
    lines.append("        ChordNode_t* node = (ChordNode_t*)malloc(sizeof(ChordNode_t));")
    lines.append("        if (!node) {")
    lines.append(f"            {name}_free_linked_list(head);")
    lines.append("            return NULL;")
    lines.append("        }")
    lines.append(f"        node->event = {name}_data[i];")
    lines.append("        node->next = NULL;")
    lines.append("        current->next = node;")
    lines.append("        current = node;")
    lines.append("    }")
    lines.append("")
    lines.append("    return head;")
    lines.append("}")
    lines.append("")
    lines.append("/**")
    lines.append(" * @brief Free a linked list created by create_linked_list")
    lines.append(" * @param head Pointer to head of linked list")
    lines.append(" */")
    lines.append(f"void {name}_free_linked_list(ChordNode_t* head) {{")
    lines.append("    while (head) {")
    lines.append("        ChordNode_t* next = head->next;")
    lines.append("        free(head);")
    lines.append("        head = next;")
    lines.append("    }")
    lines.append("}")
    lines.append("")
    lines.append("/**")
    lines.append(" * @brief Get total playback duration in milliseconds")
    lines.append(" * @return Total duration in ms")
    lines.append(" */")
    lines.append(f"uint32_t {name}_get_total_duration_ms(void) {{")
    lines.append("    uint32_t total = 0;")
    lines.append(f"    for (uint32_t i = 0; i < {name.upper()}_TOTAL_EVENTS; i++) {{")
    lines.append(f"        total += {name}_data[i].delay_to_next_ms;")
    lines.append("    }")
    lines.append(f"    if ({name.upper()}_TOTAL_EVENTS > 0) {{")
    lines.append(f"        total += {name}_data[{name.upper()}_TOTAL_EVENTS - 1].duration_ms;")
    lines.append("    }")
    lines.append("    return total;")
    lines.append("}")
    lines.append("")
    
    with open(output_path, 'w') as f:
        f.write('\n'.join(lines))
    
    print(f"Generated source: {output_path}")


def generate_static_linked_list(chords: List[ChordEvent], output_path: str, name: str = "song"):
    """
    Generate a C file with statically allocated linked list (no malloc needed).
    Better for embedded systems with limited heap.
    """
    lines = []
    lines.append("/**")
    lines.append(f" * @file {name}_static.c")
    lines.append(" * @brief Piano robot song data - Static linked list (no malloc)")
    lines.append(" * @note Include this file OR the dynamic version, not both!")
    lines.append(" */")
    lines.append("")
    lines.append(f'#include "{name}_data.h"')
    lines.append("")
    lines.append("/* ============================================================")
    lines.append(" * STATIC LINKED LIST - NO MALLOC REQUIRED")
    lines.append(" * ============================================================ */")
    lines.append("")
    lines.append(f"static ChordNode_t {name}_nodes[{name.upper()}_TOTAL_EVENTS];")
    lines.append(f"static uint8_t {name}_initialized = 0;")
    lines.append("")
    lines.append("/**")
    lines.append(" * @brief Get pointer to static linked list (initializes on first call)")
    lines.append(" * @return Pointer to head of linked list")
    lines.append(" */")
    lines.append(f"ChordNode_t* {name}_get_static_list(void) {{")
    lines.append(f"    if (!{name}_initialized) {{")
    lines.append(f"        for (uint32_t i = 0; i < {name.upper()}_TOTAL_EVENTS; i++) {{")
    lines.append(f"            {name}_nodes[i].event = {name}_data[i];")
    lines.append(f"            {name}_nodes[i].next = (i < {name.upper()}_TOTAL_EVENTS - 1) ? &{name}_nodes[i + 1] : NULL;")
    lines.append("        }")
    lines.append(f"        {name}_initialized = 1;")
    lines.append("    }")
    lines.append(f"    return &{name}_nodes[0];")
    lines.append("}")
    lines.append("")
    lines.append("/**")
    lines.append(" * @brief Reset static list to start (for replay)")
    lines.append(" */")
    lines.append(f"void {name}_reset_static_list(void) {{")
    lines.append(f"    {name}_initialized = 0;")
    lines.append("}")
    lines.append("")
    
    with open(output_path, 'w') as f:
        f.write('\n'.join(lines))
    
    print(f"Generated static linked list: {output_path}")


def print_summary(chords: List[ChordEvent], notes: List[MidiNote]):
    """Print summary of parsed data"""
    print("\n" + "=" * 60)
    print("CONVERSION SUMMARY")
    print("=" * 60)
    print(f"Total notes parsed: {len(notes)}")
    print(f"Total chord events: {len(chords)}")
    
    if chords:
        max_chord_size = max(len(c.notes) for c in chords)
        avg_chord_size = sum(len(c.notes) for c in chords) / len(chords)
        total_duration = chords[-1].start_time + chords[-1].duration_ms / 1000
        
        print(f"Max simultaneous notes: {max_chord_size}")
        print(f"Avg notes per chord: {avg_chord_size:.1f}")
        print(f"Total duration: {total_duration:.2f}s")
    
    # Count unique notes
    all_notes = set()
    for chord in chords:
        for note in chord.notes:
            all_notes.add(note.note_name)
    
    sorted_notes = sorted(all_notes, key=lambda n: MidiParser.name_to_midi_note(n))
    print(f"Unique notes used: {len(sorted_notes)}")
    print(f"Note range: {sorted_notes[0] if sorted_notes else 'N/A'} to {sorted_notes[-1] if sorted_notes else 'N/A'}")
    print("=" * 60 + "\n")


def main():
    """Main entry point"""
    if len(sys.argv) < 2:
        print(__doc__)
        print("\nExamples:")
        print("  python midi_to_stm32.py input.mid")
        print("  python midi_to_stm32.py input.mid mysong")
        print("  python midi_to_stm32.py --from-txt viva.txt viva")
        return 1
    
    # Parse arguments
    from_txt = False
    args = sys.argv[1:]
    
    if args[0] == '--from-txt':
        from_txt = True
        args = args[1:]
    
    if not args:
        print("Error: No input file specified")
        return 1
    
    input_file = args[0]
    
    # Determine output name
    if len(args) > 1:
        output_name = args[1]
    else:
        # Extract name from input file
        import os
        base = os.path.basename(input_file)
        output_name = os.path.splitext(base)[0]
        # Clean up name for C identifier
        output_name = re.sub(r'[^a-zA-Z0-9_]', '_', output_name)
        output_name = re.sub(r'^[0-9]', '_', output_name)  # Can't start with number
        output_name = output_name.lower()
    
    print(f"\n[MIDI to STM32 Converter]")
    print(f"Input: {input_file}")
    print(f"Output name: {output_name}")
    
    # Parse input
    parser = MidiParser()
    
    try:
        if from_txt:
            print("Parsing TXT file...")
            notes = parser.parse_txt_file(input_file)
        else:
            print("Parsing MIDI file...")
            notes = parser.parse_midi_file(input_file)
    except ImportError as e:
        print(f"Error: {e}")
        return 1
    except Exception as e:
        print(f"Error parsing file: {e}")
        return 1
    
    if not notes:
        print("Error: No notes found in file")
        return 1
    
    # Group notes into chords
    print("Grouping notes into chords...")
    chords = group_notes_to_chords(notes)
    
    # Print summary
    print_summary(chords, notes)
    
    # Generate output files
    import os
    output_dir = os.path.dirname(input_file) or '.'
    
    prototype_path = os.path.join(output_dir, f"{output_name}_prototype.txt")
    header_path = os.path.join(output_dir, f"{output_name}_data.h")
    source_path = os.path.join(output_dir, f"{output_name}_data.c")
    static_path = os.path.join(output_dir, f"{output_name}_static.c")
    
    print("\nGenerating output files...")
    generate_prototype_txt(chords, prototype_path)
    generate_c_header(chords, header_path, output_name)
    generate_c_source(chords, source_path, output_name)
    generate_static_linked_list(chords, static_path, output_name)
    
    print("\n[OK] Conversion complete!")
    print(f"\nGenerated files:")
    print(f"  - {prototype_path} - Human-readable prototype")
    print(f"  - {header_path} - C header with structs and macros")
    print(f"  - {source_path} - C source with data array")
    print(f"  - {static_path} - Static linked list (no malloc)")
    print(f"\n[!] IMPORTANT: Edit {header_path} to set correct {'{NOTE}_POS'} values")
    print("    for your robot's physical key positions!")
    
    return 0


if __name__ == "__main__":
    sys.exit(main())
