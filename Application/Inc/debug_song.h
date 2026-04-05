#ifndef DEBUG_SONG_H
#define DEBUG_SONG_H

#include <application.h>

// Jamie stuff:
// Tempo 
#define spacing 21.0f 

// Left hand targets 
#define L_C1 ((spacing * 0.0))
#define L_D1 ((spacing * 1.0))
#define L_E1 ((spacing * 2.0))
#define L_F1 ((spacing * 3.0))
#define L_G1 ((spacing * 4.0))
#define L_A1 ((spacing * 5.0))
#define L_B1 ((spacing * 6.0))

#define L_C2 ((spacing * 7.0))
#define L_D2 ((spacing * 8.0))
#define L_E2 ((spacing * 9.0))
#define L_F2 ((spacing * 10.0))
#define L_G2 ((spacing * 11.0))
#define L_A2 ((spacing * 12.0))
#define L_B2 ((spacing * 13.0))

#define L_C3 ((spacing * 14.0))
#define L_D3 ((spacing * 15.0))
#define L_E3 ((spacing * 16.0))
#define L_F3 ((spacing * 17.0))
#define L_G3 ((spacing * 18.0))
#define L_A3 ((spacing * 19.0))
#define L_B3 ((spacing * 20.0))

// Right hand targets
#define R_F1 ((spacing * 0.0))
#define R_E1 ((spacing * 1.0))
#define R_D1 ((spacing * 2.0))
#define R_C1 ((spacing * 3.0))

#define R_B2 ((spacing * 4.0))
#define R_A2 ((spacing * 5.0))
#define R_G2 ((spacing * 6.0))
#define R_F2 ((spacing * 7.0))
#define R_E2 ((spacing * 8.0))
#define R_D2 ((spacing * 9.0))
#define R_C2 ((spacing * 10.0))

#define R_B3 ((spacing * 11.0))
#define R_A3 ((spacing * 12.0))
#define R_G3 ((spacing * 13.0))
#define R_F3 ((spacing * 14.0))
#define R_E3 ((spacing * 15.0))
#define R_D3 ((spacing * 16.0))
#define R_C3 ((spacing * 17.0))

#define R_B4 ((spacing * 18.0))
#define R_A4 ((spacing * 19.0))
#define R_G4 ((spacing * 20.0))
#define R_F4 ((spacing * 21.0))
#define R_E4 ((spacing * 22.0))
#define R_D4 ((spacing * 23.0))
#define R_C4 ((spacing * 24.0))

// Tempo for Waltz in A minor
#define bpm_wa 80.0f
#define HALF_WA ((60000.0f / bpm_wa * 2.0f))
#define QUARTER_WA ((60000.0f / bpm_wa))
#define EIGHTH_WA ((QUARTER_WA / 2.0f) )
#define SIXTEENTH_WA ((EIGHTH_WA / 2.0f) )

// Tempo for Swan Lake
#define bpm_sl 70.0f
#define HALF_SL ((60000.0f / bpm_sl * 2.0f))
#define QUARTER_SL ((60000.0f / bpm_sl))
#define EIGHTH_SL ((QUARTER_SL / 2.0f) )
#define SIXTEENTH_SL ((EIGHTH_SL / 2.0f) )

// Tempo for The Ballad of Gilligan's Isle 
#define bpm_bi 100.0f
#define HALF_BI ((60000.0f / bpm_bi * 2.0f))
#define QUARTER_BI ((60000.0f / bpm_bi))
#define EIGHTH_BI ((QUARTER_BI / 2.0f) )
#define SIXTEENTH_BI ((EIGHTH_BI / 2.0f) )

// Ji's definition:
#define CHORD_1 3 * 21.0f 
#define CHORD_2 4 * 21.0f 
#define CHORD_3 3 * 21.0f
#define CHORD_4 3 * 21.0f

#define DO_3    15 * 21.0f
#define REb_3   16 * 21.0f 
#define SIb_2   18 * 21.0f
#define LAb_2   19 * 21.0f
#define MIb_2   22 * 21.0f
#define FA_2    19 * 21.0f
#define SO_2    18 * 21.0f
#define FA_3    12 * 21.0f
#define LA_3    11 * 21.0f
#define MIb_3   15 * 21.0f

#define MIb_4   8 *  21.0f
#define LA_4    4 *  21.0f
#define FA_4    5 *  21.0f
#define SIb_3   11 * 21.0f
#define DO_4    8 *  21.0f
#define SO_3    11 * 21.0f
#define LAb_3   12 * 21.0f
#define REb_4   9 *  21.0f 
#define SO_4    4 *  21.0f

// Function to load a hardcoded song into chord_events for testing
void load_debug_song(void); // Jamie: n
void load_debug_song2(void); // Ji : v 
void load_debug_song3(void); // stucco song : b

#endif