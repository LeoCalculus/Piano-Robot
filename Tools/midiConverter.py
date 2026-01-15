import mido
import sys
import os

# Solfège names for each semitone (0-11)
SOLFEGE = ['Do', 'Do#', 'Re', 'Re#', 'Mi', 'Fa', 'Fa#', 'Sol', 'Sol#', 'La', 'La#', 'Si']

def midi_note_to_solfege(note_num):
    """Convert MIDI note number to solfège name with octave."""
    octave = (note_num // 12) - 1
    note_idx = note_num % 12
    return f"{SOLFEGE[note_idx]}{octave}"

def midi_note_to_freq(note_num):
    """Convert MIDI note number to frequency in Hz."""
    return 440.0 * (2 ** ((note_num - 69) / 12))

def convert_midi_to_c_array(midi_path):
    """Convert MIDI file to C header file."""
    mid = mido.MidiFile(midi_path)

    notes = []
    ticks_per_beat = mid.ticks_per_beat
    tempo = 500000  # Default tempo (120 BPM)

    for track in mid.tracks:
        current_time = 0
        for msg in track:
            current_time += msg.time

            if msg.type == 'set_tempo':
                tempo = msg.tempo
            elif msg.type == 'note_on' and msg.velocity > 0:
                # Calculate duration by looking for note_off
                duration_ticks = 0
                for future_msg in track[track.index(msg)+1:]:
                    duration_ticks += future_msg.time
                    if (future_msg.type == 'note_off' and future_msg.note == msg.note) or \
                       (future_msg.type == 'note_on' and future_msg.note == msg.note and future_msg.velocity == 0):
                        break

                # Convert ticks to milliseconds
                duration_ms = int(mido.tick2second(duration_ticks, ticks_per_beat, tempo) * 1000)

                solfege = midi_note_to_solfege(msg.note)
                freq = int(midi_note_to_freq(msg.note))

                notes.append({
                    'note': msg.note,
                    'solfege': solfege,
                    'freq': freq,
                    'duration_ms': duration_ms if duration_ms > 0 else 200
                })

    # Generate output file path (same folder as midi file, .h extension)
    midi_dir = os.path.dirname(os.path.abspath(midi_path))
    midi_name = os.path.splitext(os.path.basename(midi_path))[0]
    output_path = os.path.join(midi_dir, f"{midi_name}.h")

    lines = []
    lines.append(f"// Converted from: {os.path.basename(midi_path)}")
    lines.append(f"// Total notes: {len(notes)}")
    lines.append("")

    # Define note frequencies
    lines.append("// Note definitions (frequency in Hz)")
    unique_notes = sorted(set((n['solfege'], n['freq']) for n in notes), key=lambda x: x[1])
    for solfege, freq in unique_notes:
        c_name = solfege.replace('#', 's')
        lines.append(f"#define NOTE_{c_name.upper()} {freq}")
    lines.append("#define NOTE_REST 0")
    lines.append("")

    # Generate melody array
    lines.append("#include <stdint.h>")
    lines.append("typedef struct { uint16_t freq; uint16_t duration_ms; } Note;")
    lines.append("")
    lines.append("const Note melody[] = {")
    for i, n in enumerate(notes):
        c_name = n['solfege'].replace('#', 's').upper()
        comma = "," if i < len(notes) - 1 else ""
        lines.append(f"    {{NOTE_{c_name}, {n['duration_ms']}}}{comma}  // {n['solfege']}")
    lines.append("};")
    lines.append(f"const uint16_t melody_len = {len(notes)};")

    # Write to file
    with open(output_path, 'w') as f:
        f.write('\n'.join(lines))

    print(f"Generated: {output_path}")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python midiConverter.py <midi_file>")
        print("Example: python midiConverter.py test1.mid")
        sys.exit(1)

    midi_file = sys.argv[1]
    if not os.path.exists(midi_file):
        print(f"Error: File '{midi_file}' not found")
        sys.exit(1)

    convert_midi_to_c_array(midi_file)