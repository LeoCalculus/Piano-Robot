import re
import os

# ==============================================================================
# 1. CONFIGURATION (Change these whenever you need to)
# ==============================================================================
BPM = 100.0
SPACING = 23.0

INPUT_FILE = "input.txt"
OUTPUT_FILE = "waltz_in_a_minor.txt"

# ==============================================================================
# 2. MACRO DEFINITIONS (Matches your C header file)
# ==============================================================================
LEFT_MACROS = {
    'L_C1': 0, 'L_D1': 1, 'L_E1': 2, 'L_F1': 3, 'L_G1': 4, 'L_A1': 5, 'L_B1': 6,
    'L_C2': 7, 'L_D2': 8, 'L_E2': 9, 'L_F2': 10, 'L_G2': 11, 'L_A2': 12, 'L_B2': 13,
    'L_C3': 14, 'L_D3': 15, 'L_E3': 16, 'L_F3': 17, 'L_G3': 18, 'L_A3': 19, 'L_B3': 20,
}

RIGHT_MACROS = {
    'R_F1': 0, 'R_E1': 1, 'R_D1': 2, 'R_C1': 3,
    'R_B2': 4, 'R_A2': 5, 'R_G2': 6, 'R_F2': 7, 'R_E2': 8, 'R_D2': 9, 'R_C2': 10,
    'R_B3': 11, 'R_A3': 12, 'R_G3': 13, 'R_F3': 14, 'R_E3': 15, 'R_D3': 16, 'R_C3': 17,
    'R_B4': 18, 'R_A4': 19, 'R_G4': 20, 'R_F4': 21, 'R_E4': 22, 'R_D4': 23, 'R_C4': 24,
}

# ==============================================================================
# 3. HELPER FUNCTIONS
# ==============================================================================
def get_duration_ms(dur_str, bpm):
    """Converts a tempo macro into milliseconds."""
    quarter = 60000.0 / bpm
    dur_str = dur_str.strip()
    
    if dur_str == 'QUARTER': return int(quarter)
    if dur_str == 'EIGHTH': return int(quarter / 2.0)
    if dur_str == 'HALF': return int(quarter * 2.0)
    if dur_str == 'SIXTEENTH': return int(quarter / 4.0)
    
    # If it's already a number, just return it
    try:
        return int(dur_str)
    except ValueError:
        return int(quarter) # Fallback

def main():
    if not os.path.exists(INPUT_FILE):
        print(f"Error: I couldn't find '{INPUT_FILE}'. Make sure it is in the same folder.")
        return

    # Regex to pull out the 4 groups of data we care about from the C struct
    pattern = re.compile(
        r'\.positions\s*=\s*\{\s*([A-Za-z0-9_]*)\s*,\s*([A-Za-z0-9_]*)\s*\}'
        r'.*?\.pressed\s*=\s*\{([^}]+)\}'
        r'.*?\.duration_ms\s*=\s*([A-Za-z0-9_]+)'
        r'.*?\.long_pressed\s*=\s*\{([^}]+)\}'
    )

    # These remember the last position in case you leave a hand blank like {L_C2, }
    last_lpos = 0.0
    last_rpos = 0.0
    multiplier = SPACING / 10.0
    
    lines_processed = 0

    with open(INPUT_FILE, 'r') as infile, open(OUTPUT_FILE, 'w') as outfile:
        for line in infile:
            line = line.strip()
            
            # Skip empty lines or lines that start with a comment //
            if not line or line.startswith('//'):
                continue

            match = pattern.search(line)
            if not match:
                continue

            left_macro, right_macro, pressed_str, dur_str, keep_str = match.groups()

            # Calculate Left Position (or keep previous if empty)
            if left_macro and left_macro in LEFT_MACROS:
                last_lpos = round(LEFT_MACROS[left_macro] * multiplier, 1)

            # Calculate Right Position (or keep previous if empty)
            if right_macro and right_macro in RIGHT_MACROS:
                last_rpos = round(RIGHT_MACROS[right_macro] * multiplier, 1)

            # Format the 10 solenoids (remove commas, separate by spaces)
            solenoids = " ".join([s.strip() for s in pressed_str.split(',')])
            
            # Format the keep flags (remove commas, separate by spaces)
            keeps = " ".join([s.strip() for s in keep_str.split(',')])

            # Calculate the duration based on BPM
            dur_ms = get_duration_ms(dur_str, BPM)

            # Build the final clean string and write it to the file
            out_str = f"{last_lpos:.1f} {last_rpos:.1f} {solenoids} {dur_ms} {keeps}\n"
            outfile.write(out_str)
            lines_processed += 1

    print(f"Done! Successfully converted {lines_processed} steps.")
    print(f"Output saved to: {OUTPUT_FILE}")
    print(f"Settings Used: BPM = {BPM}, Spacing = {SPACING}")

if __name__ == "__main__":
    main()