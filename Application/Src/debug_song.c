#include <debug_song.h>


// #define CHORD_1 3 * 21.0f 
// #define CHORD_2 4 * 21.0f 
// #define CHORD_3 3 * 21.0f
// #define CHORD_4 3 * 21.0f

// #define DO_3    15 * 21.0f
// #define REb_3   16 * 21.0f 
// #define SIb_2   18 * 21.0f
// #define LAb_2   19 * 21.0f
// #define MIb_2   22 * 21.0f
// #define FA_2    19 * 21.0f
// #define SO_2    18 * 21.0f
// #define FA_3    12 * 21.0f
// #define LA_3    11 * 21.0f
// #define MIb_3   15 * 21.0f

// #define MIb_4   8 *  21.0f
// #define LA_4    4 *  21.0f
// #define FA_4    5 *  21.0f
// #define SIb_3   11 * 21.0f
// #define DO_4    8 *  21.0f
// #define SO_3    11 * 21.0f
// #define LAb_3   12 * 21.0f
// #define REb_4   9 *  21.0f 
// #define SO_4    4 *  21.0f

// Jamie song
void load_debug_song(){    
    // temp array that have all info
    // long press --> hold to next instance
    // left hand structure: white (space) flat white white white flat white
    static const ChordEvent_t debug_data[1024] = {
        
        // ================== PAGE 1 ==================

        // ================== Line 1 ==================
        { .positions = {L_C1, R_G3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },

        { .positions = {L_C2, R_A3}, .pressed = {0, 0, 0, 0, 0, 0, 1, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_C2, R_B3}, .pressed = {0, 0, 0, 0, 0, 0, 1, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_E2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_E2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },
        
        { .positions = {L_D2, R_D2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_D2, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_F2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1}, .duration_ms = QUARTER_WA, .long_pressed = {0, 1} },
        { .positions = {L_A2, R_F2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },
        
        { .positions = {L_C2, R_D2}, .pressed = {0, 0, 0, 0, 0, 0, 1, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_C2, R_E2}, .pressed = {0, 0, 0, 0, 0, 0, 1, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_G2, R_F2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_G2, R_A2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_G2, R_G2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_G2, R_F2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },

        { .positions = {L_C2, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_C2, R_E2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_G1, R_E2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1}, .duration_ms = QUARTER_WA, .long_pressed = {0, 1} },
        { .positions = {L_G1, R_E2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },
    
        { .positions = {L_C2, R_C2}, .pressed = {0, 0, 0, 0, 0, 0, 1, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_C2, R_D2}, .pressed = {0, 0, 0, 0, 0, 0, 1, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_E2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_E2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },
    
        
        // ================== Line 2 ==================
        { .positions = {L_D2, R_F2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_D2, R_G2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_A2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 1, 0, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 1} },
        { .positions = {L_A2, R_A2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 1, 0, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },
    
        { .positions = {L_C2, R_D2}, .pressed = {0, 0, 0, 0, 0, 0, 1, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_C2, R_E2}, .pressed = {0, 0, 0, 0, 0, 0, 1, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_G2, R_F2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_G2, R_A2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_G2, R_G2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_G2, R_D2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        
        { .positions = {L_C2, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },
        { .positions = {L_G2, R_E2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 0, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },
        { .positions = {L_C2, R_G3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },
        
        { .positions = {L_C2, R_A3}, .pressed = {0, 0, 0, 0, 0, 0, 1, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_C2, R_B3}, .pressed = {0, 0, 0, 0, 0, 0, 1, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_C2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_C2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },

        { .positions = {L_D2, R_D2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_D2, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_F2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1}, .duration_ms = QUARTER_WA, .long_pressed = {0, 1} },
        { .positions = {L_A2, R_F2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },
        
        { .positions = {L_C2, R_D2}, .pressed = {0, 0, 0, 0, 0, 0, 1, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_C2, R_E2}, .pressed = {0, 0, 0, 0, 0, 0, 1, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_G2, R_F2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_G2, R_A2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_G2, R_G2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_G2, R_F2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
    
        // ================== Line 3 ==================
        { .positions = {L_C2, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_C2, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_G2, R_E2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1}, .duration_ms = QUARTER_WA, .long_pressed = {0, 1} },
        { .positions = {L_G2, R_E2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },

        { .positions = {L_A2, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_A2, R_F2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_G2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_G2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },

        { .positions = {L_D2, R_A2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_D2, R_B2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_C1}, .pressed = {1, 0, 0, 1, 0, 0, 1, 1, 0, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 1} },
        { .positions = {L_A2, R_C1}, .pressed = {1, 0, 0, 1, 0, 0, 1, 1, 0, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },
    
        { .positions = {L_A2, R_G2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },
        { .positions = {L_G2, R_G2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_G2, R_G2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_G2, R_D1}, .pressed = {1, 0, 1, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_G2, R_A2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },

        { .positions = {L_C2, R_G2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 1} },
        { .positions = {L_G2, R_G2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 1, 0, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 1} },
        { .positions = {L_G2, R_G2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 1, 0, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },

        { .positions = {L_E2, R_G2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_G2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_F2, R_A2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 1, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_F2, R_A2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_G2, R_B2}, .pressed = {1, 0, 0, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_G2, R_C1}, .pressed = {1, 0, 0, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
    
        // ================== Line 4 ==================
        { .positions = {L_A2, R_B2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_A2, R_A2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_G2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_B2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_A2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },

        { .positions = {L_E2, R_A2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_A2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_G2}, .pressed = {1, 0, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_A2}, .pressed = {1, 0, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_G2}, .pressed = {1, 0, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },
        
        { .positions = {L_A2, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_A2, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_C2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_D2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_C2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 1, 0, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },
        
        { .positions = {L_E2, R_G3}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_A3}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_B3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_G3}, .pressed = {1, 0, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = SIXTEENTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_A3}, .pressed = {1, 0, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = SIXTEENTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_B3}, .pressed = {1, 0, 0, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = SIXTEENTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_G2}, .pressed = {1, 0, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = SIXTEENTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_A2}, .pressed = {1, 0, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = SIXTEENTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_G2, R_B2}, .pressed = {1, 0, 0, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = SIXTEENTH_WA, .long_pressed = {1, 1} },
        { .positions = {L_G2, R_B2}, .pressed = {1, 0, 0, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = SIXTEENTH_WA, .long_pressed = {1, 1} },
        { .positions = {L_G2, R_B2}, .pressed = {1, 0, 0, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = SIXTEENTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_G2, R_C1}, .pressed = {1, 0, 0, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = SIXTEENTH_WA, .long_pressed = {0, 0} },
        
        { .positions = {L_A2, R_B2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_A2, R_A2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_G2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_B2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_A2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },

        // ================== Line 5 ==================
        { .positions = {L_E2, R_A2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_A2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_G2}, .pressed = {1, 0, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_E1}, .pressed = {1, 0, 0, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_E1}, .pressed = {1, 0, 0, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },

        { .positions = {L_E2, R_C2}, .pressed = {0, 0, 0, 1, 0, 0, 0, 1, 0, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 1} },
        { .positions = {L_E2, R_C2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 1, 0, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_C2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 0, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },
    
        { .positions = {L_E2, R_C2}, .pressed = {0, 0, 0, 1, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_D2}, .pressed = {0, 0, 0, 1, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_E2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_E2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },

        { .positions = {L_D2, R_F2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_D2, R_G2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_A2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 1, 0, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 1} },
        { .positions = {L_A2, R_A2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 1, 0, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },

        { .positions = {L_C2, R_D2}, .pressed = {0, 0, 0, 0, 0, 0, 1, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },      
        { .positions = {L_C2, R_E2}, .pressed = {0, 0, 0, 0, 0, 0, 1, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },        
        { .positions = {L_G2, R_F2}, .pressed = {1, 0, 1, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },        
        { .positions = {L_G2, R_A2}, .pressed = {1, 0, 1, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },  
        { .positions = {L_G2, R_G2}, .pressed = {1, 0, 1, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },        
        { .positions = {L_G2, R_F2}, .pressed = {1, 0, 1, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} }, 

        { .positions = {L_C2, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },        
        { .positions = {L_C2, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} }, 
        { .positions = {L_G2, R_E2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1}, .duration_ms = QUARTER_WA, .long_pressed = {0, 1} },    
        { .positions = {L_G2, R_E2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },  
    
        // ================== PAGE 2 ==================
        // ================== Line 1 ==================
        { .positions = {L_C2, R_C2}, .pressed = {0, 0, 0, 0, 0, 0, 1, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_C2, R_D2}, .pressed = {0, 0, 0, 0, 0, 0, 1, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_E2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_E2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },
    
        { .positions = {L_D2, R_F2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_D2, R_G2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_A2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 1, 0, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 1} },
        { .positions = {L_A2, R_A2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 1, 0, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },
    
        { .positions = {L_B2, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_B2, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_F2, R_G2}, .pressed = {0, 0, 1, 1, 0, 1, 0, 0, 1, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} }, // could be different
        { .positions = {L_F2, R_G2}, .pressed = {0, 0, 1, 1, 0, 1, 0, 0, 1, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} }, // could be different
    
        { .positions = {L_E2, R_A2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_A2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_B2}, .pressed = {1, 0, 0, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = QUARTER_WA, .long_pressed = {0, 1} },
        { .positions = {L_E2, R_B2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} }, // QUARTER_WA rest

        { .positions = {L_E2, R_D1}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_D1}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_D1}, .pressed = {1, 0, 1, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_E1}, .pressed = {1, 0, 1, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} }, // cannot play this note (out of reach)
        { .positions = {L_E2, R_E1}, .pressed = {1, 0, 1, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_D1}, .pressed = {1, 0, 1, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },

        { .positions = {L_E2, R_D1}, .pressed = {0, 0, 0, 1, 0, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_D1}, .pressed = {0, 0, 0, 1, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_C1}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_A2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 1, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_A2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_G2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 1, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
    
        // ================== Line 2 ==================
        { .positions = {L_E2, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_D2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_C2}, .pressed = {1, 0, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_G2}, .pressed = {1, 0, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_G2}, .pressed = {1, 0, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },

        { .positions = {L_C2, R_E2}, .pressed = {0, 0, 0, 0, 0, 0, 1, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_C2, R_D2}, .pressed = {0, 0, 0, 0, 0, 0, 1, 0, 1, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_C2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_G2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 1, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_G2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 1, 0, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },

        { .positions = {L_E2, R_D1}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_D1}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_D1}, .pressed = {1, 0, 1, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_E1}, .pressed = {1, 0, 1, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} }, // cannot play this note
        { .positions = {L_E2, R_E1}, .pressed = {1, 0, 1, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_D1}, .pressed = {1, 0, 1, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },

        { .positions = {L_A2, R_D1}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_A2, R_D1}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_C1}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_A2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 1, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_A2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_G2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 1, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },

        { .positions = {L_E2, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_D2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_F2, R_A3}, .pressed = {0, 0, 0, 1, 0, 0, 1, 0, 1, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_F2, R_F2}, .pressed = {0, 0, 0, 1, 0, 0, 1, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_F2, R_E2}, .pressed = {0, 0, 0, 1, 0, 0, 1, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_F2, R_A3}, .pressed = {0, 0, 0, 1, 0, 0, 1, 0, 1, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
    
        // ================== Line 3 ==================
        { .positions = {L_C2, R_A3}, .pressed = {0, 0, 0, 0, 0, 0, 1, 0, 0, 1}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_C2}, .pressed = {1, 0, 0, 0, 0, 0, 1, 0, 0, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },
        { .positions = {L_C2, R_G3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },

        { .positions = {L_C2, R_A3}, .pressed = {0, 0, 0, 0, 0, 0, 1, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_C2, R_B3}, .pressed = {0, 0, 0, 0, 0, 0, 1, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_E2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_E2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },

        { .positions = {L_D2, R_F2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_D2, R_G2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_A2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 1, 0, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 1} },
        { .positions = {L_A2, R_A2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 1, 0, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },

        { .positions = {L_C2, R_D2}, .pressed = {0, 0, 0, 0, 0, 0, 1, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_C2, R_E2}, .pressed = {0, 0, 0, 0, 0, 0, 1, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_G2, R_F2}, .pressed = {1, 0, 1, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_G2, R_A2}, .pressed = {1, 0, 1, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_G2, R_G2}, .pressed = {1, 0, 1, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_G2, R_F2}, .pressed = {1, 0, 1, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        
        { .positions = {L_C2, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_C2, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_G2, R_E2}, .pressed = {1, 0, 0, 1, 0, 1, 0, 0, 0, 1}, .duration_ms = QUARTER_WA, .long_pressed = {0, 1} },
        { .positions = {L_G2, R_E2}, .pressed = {1, 0, 0, 1, 0, 1, 0, 0, 0, 1}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },

        { .positions = {L_A2, R_C2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_A2, R_D2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_E2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_E2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },

        // ================== Line 4 ==================
        { .positions = {L_D2, R_F2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_D2, R_G2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_A2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 1, 0, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 1} },
        { .positions = {L_A2, R_A2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 1, 0, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },

        { .positions = {L_C2, R_D2}, .pressed = {0, 0, 0, 0, 0, 0, 1, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_C2, R_E2}, .pressed = {0, 0, 0, 0, 0, 0, 1, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_F2}, .pressed = {0, 0, 1, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_A2}, .pressed = {0, 0, 1, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_G2, R_G2}, .pressed = {1, 0, 0, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_G2, R_D2}, .pressed = {1, 0, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },

        { .positions = {L_C2, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },
        { .positions = {L_G2, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 1, 0, 0, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },
        { .positions = {L_C2, R_G3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },

        { .positions = {L_C2, R_C2}, .pressed = {0, 0, 0, 0, 0, 0, 1, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_C2, R_D2}, .pressed = {0, 0, 0, 0, 0, 0, 1, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_E2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_E2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },

        { .positions = {L_D2, R_F2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_D2, R_G2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_A2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 1, 0, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 1} },
        { .positions = {L_A2, R_A2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 1, 0, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },

        { .positions = {L_E2, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_E2}, .pressed = {1, 0, 1, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_E2}, .pressed = {1, 0, 1, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_B2}, .pressed = {1, 0, 1, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_F2}, .pressed = {1, 0, 1, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },

        // ================== Line 5 ==================
        { .positions = {L_C2, R_D2}, .pressed = {0, 0, 0, 0, 0, 0, 1, 1, 0, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 1} },
        { .positions = {L_E2, R_D2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 1, 0, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_D2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 0, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },

        { .positions = {L_C2, R_B2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_C2, R_B2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_A2}, .pressed = {1, 0, 0, 1, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_G2}, .pressed = {1, 0, 0, 1, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_D2, R_F2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_D2, R_E2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },

        { .positions = {L_D2, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_D2, R_D2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_G2, R_D2}, .pressed = {0, 0, 1, 0, 0, 0, 1, 0, 1, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_G2, R_D2}, .pressed = {0, 0, 1, 0, 0, 0, 1, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_E2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_F2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        
        { .positions = {L_E2, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 1, 0, 1, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 1, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_F2}, .pressed = {1, 0, 0, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_WA, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_A3}, .pressed = {1, 0, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH_WA, .long_pressed = {0, 0} },

        { .positions = {L_C2, R_A3}, .pressed = {0, 0, 0, 0, 0, 0, 1, 0, 0, 1}, .duration_ms = QUARTER_WA, .long_pressed = {0, 1} },
        { .positions = {L_E2, R_A3}, .pressed = {1, 0, 0, 0, 0, 0, 1, 0, 0, 1}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_A3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = QUARTER_WA, .long_pressed = {0, 0} },
        
        // HOMING 
        { .positions = {L_C1, R_F1}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 500, .long_pressed = {0, 0} },
        { .positions = {1000.0, 1000.0}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 1000, .long_pressed = {0, 0} }
        
        
    }; 

    for (int i = 0; i < 1024; i++) {
        chord_events[i] = debug_data[i];
    }
}

// Ji's song
void load_debug_song2() {
        // temp array that have all info
    static const ChordEvent_t debug_data[1024] = {

        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 0
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 1
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 2
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 3
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 4
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 5
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 6

        { .positions = {CHORD_2, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 7
        { .positions = {CHORD_2, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 8
        { .positions = {CHORD_2, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 9
        { .positions = {CHORD_2, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 10
        { .positions = {CHORD_2, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 11
        { .positions = {CHORD_2, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 12
        { .positions = {CHORD_2, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 13
        { .positions = {CHORD_2, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 14
        { .positions = {CHORD_3, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 15

        { .positions = {CHORD_3, DO_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 16
        { .positions = {CHORD_3, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 17
        { .positions = {CHORD_3, DO_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 18
        { .positions = {CHORD_3, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 19
        { .positions = {CHORD_3, DO_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 20
        { .positions = {CHORD_3, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 21
        { .positions = {CHORD_3, DO_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 22

        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 23
        { .positions = {CHORD_4, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 24
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 25
        { .positions = {CHORD_4, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 26
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 27
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 28
        { .positions = {CHORD_4, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 29
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 30
        { .positions = {CHORD_4, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 31

        // ================== 6 (160-191) ==================
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 0
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 1
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 2
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 3
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 4
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 5
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 6

        { .positions = {CHORD_2, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 7
        { .positions = {CHORD_2, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 8
        { .positions = {CHORD_2, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 9
        { .positions = {CHORD_2, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 10
        { .positions = {CHORD_2, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 11
        { .positions = {CHORD_2, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 12
        { .positions = {CHORD_2, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 13
        { .positions = {CHORD_2, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 14
        { .positions = {CHORD_3, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 15

        { .positions = {CHORD_3, DO_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 16
        { .positions = {CHORD_3, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 17
        { .positions = {CHORD_3, DO_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 18
        { .positions = {CHORD_3, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 19
        { .positions = {CHORD_3, DO_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 20
        { .positions = {CHORD_3, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 21
        { .positions = {CHORD_3, DO_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 22

        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 23
        { .positions = {CHORD_4, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 24

        // start
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 25
        { .positions = {CHORD_4, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 26
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 27
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 28
        { .positions = {CHORD_4, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 29
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 30
        { .positions = {CHORD_4, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 31

        // ================== 7 (192-223) ==================
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 0
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 1
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 2
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 3
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 4
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 5
        { .positions = {CHORD_1, REb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 6

        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 7
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 8
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 9
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 10
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 11
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 12
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 13
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 14
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 15

        { .positions = {CHORD_3, SIb_2}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 16
        { .positions = {CHORD_3, SIb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 17
        { .positions = {CHORD_3, SIb_2}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 18
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 19
        { .positions = {CHORD_3, SIb_2}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 20
        { .positions = {CHORD_3, SIb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 21
        { .positions = {CHORD_3, DO_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 22

        { .positions = {CHORD_4, LAb_2}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 23
        { .positions = {CHORD_4, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 24
        { .positions = {CHORD_4, SO_2}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 25
        { .positions = {CHORD_4, FA_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 26
        { .positions = {CHORD_4, FA_2}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 27
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 28
        { .positions = {CHORD_4, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 29
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 30
        { .positions = {CHORD_4, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 31

        // ================== 7 (224-255) ==================
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 0
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 1
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 2
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 3
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 4
        { .positions = {CHORD_1, REb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 5
        { .positions = {CHORD_1, REb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 6

        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 7
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 8
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 9
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 10
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 11
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 12
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 13
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 14
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 15

        { .positions = {CHORD_3, DO_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 16
        { .positions = {CHORD_3, SIb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 17
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 18
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 19
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 20
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 21
        { .positions = {CHORD_3, DO_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 22

        // line 28
        { .positions = {CHORD_4, SIb_2}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 23
        { .positions = {CHORD_4, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 24
        { .positions = {CHORD_4, LAb_2}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 25
        { .positions = {CHORD_4, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 26
        { .positions = {CHORD_4, FA_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 27
        { .positions = {CHORD_4, FA_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 28
        { .positions = {CHORD_4, FA_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 29
        { .positions = {CHORD_4, FA_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 30
        { .positions = {CHORD_4, FA_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 31

        // ================== 8 (256-287) ==================
        { .positions = {CHORD_1, FA_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 0
        { .positions = {CHORD_1, FA_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 1
        { .positions = {CHORD_1, FA_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 2
        { .positions = {CHORD_1, FA_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 3
        { .positions = {CHORD_1, FA_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 4
        { .positions = {CHORD_1, FA_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 5
        { .positions = {CHORD_1, REb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 6

        { .positions = {CHORD_2, MIb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 7
        { .positions = {CHORD_2, MIb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 8
        { .positions = {CHORD_2, MIb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 9
        { .positions = {CHORD_2, MIb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 10
        { .positions = {CHORD_2, MIb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 11
        { .positions = {CHORD_2, MIb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 12
        { .positions = {CHORD_2, MIb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 13
        { .positions = {CHORD_2, MIb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 14
        { .positions = {CHORD_3, MIb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 15

        { .positions = {CHORD_3, MIb_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 16
        { .positions = {CHORD_3, MIb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 17
        { .positions = {CHORD_3, MIb_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 18
        { .positions = {CHORD_3, MIb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 19
        { .positions = {CHORD_3, MIb_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 20
        { .positions = {CHORD_3, MIb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 21
        { .positions = {CHORD_3, REb_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 22

        { .positions = {CHORD_4, REb_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 23
        { .positions = {CHORD_4, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 24
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 25
        { .positions = {CHORD_4, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 26
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 27
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 28
        { .positions = {CHORD_4, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 29
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 30
        { .positions = {CHORD_4, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 31

        // line 36

        // ================== 9 (288-319) ==================
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 0
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 1
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 2
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 3
        { .positions = {CHORD_1, REb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 4
        { .positions = {CHORD_1, REb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 5
        { .positions = {CHORD_1, REb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 6

        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 7
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 8
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 9
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 10
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 11
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 12
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 13
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 14
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 15

        { .positions = {CHORD_3, DO_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 16
        { .positions = {CHORD_3, SIb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 17
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 18
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 19
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 20
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 21
        { .positions = {CHORD_3, DO_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 22

        { .positions = {CHORD_4, SIb_2}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 23
        { .positions = {CHORD_4, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 24
        { .positions = {CHORD_4, LAb_2}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 25
        { .positions = {CHORD_4, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 26
        { .positions = {CHORD_4, LAb_2}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 27
        { .positions = {CHORD_4, MIb_4}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 28
        { .positions = {CHORD_4, MIb_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 29
        { .positions = {CHORD_4, MIb_4}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 30
        { .positions = {CHORD_4, MIb_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 31

        // line 40
        // ================== 10 (320-351) ==================
        { .positions = {CHORD_1, FA_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 0
        { .positions = {CHORD_1, FA_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 1
        { .positions = {CHORD_1, FA_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 2
        { .positions = {CHORD_1, FA_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 3
        { .positions = {CHORD_1, FA_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 4
        { .positions = {CHORD_1, MIb_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 5
        { .positions = {CHORD_1, FA_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 6

        { .positions = {CHORD_2, MIb_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 7
        { .positions = {CHORD_2, MIb_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 8
        { .positions = {CHORD_2, MIb_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 9
        { .positions = {CHORD_2, SIb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 10
        { .positions = {CHORD_2, DO_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 11
        { .positions = {CHORD_2, DO_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 12
        { .positions = {CHORD_2, REb_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 13
        { .positions = {CHORD_2, REb_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 14
        { .positions = {CHORD_3, REb_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 15

        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 16
        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 17
        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 18
        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 19
        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 20
        { .positions = {CHORD_3, DO_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 21
        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 22

        { .positions = {CHORD_4, DO_4}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 23
        { .positions = {CHORD_4, DO_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 24
        { .positions = {CHORD_4, DO_4}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 25
        { .positions = {CHORD_4, FA_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 26
        { .positions = {CHORD_4, SO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 27
        { .positions = {CHORD_4, SO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 28
        { .positions = {CHORD_4, LAb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 29
        { .positions = {CHORD_4, LAb_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 30
        { .positions = {CHORD_4, LAb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 31

        // line 44
        // ================== 11 (352-383) ==================
        { .positions = {CHORD_1, FA_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 0
        { .positions = {CHORD_1, FA_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 1
        { .positions = {CHORD_1, FA_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 2
        { .positions = {CHORD_1, FA_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 3
        { .positions = {CHORD_1, FA_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 4
        { .positions = {CHORD_1, MIb_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 5
        { .positions = {CHORD_1, FA_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 6

        { .positions = {CHORD_2, MIb_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 7
        { .positions = {CHORD_2, MIb_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 8
        { .positions = {CHORD_2, MIb_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 9
        { .positions = {CHORD_2, SIb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 10
        { .positions = {CHORD_2, DO_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 11
        { .positions = {CHORD_2, DO_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 12
        { .positions = {CHORD_2, REb_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 13
        { .positions = {CHORD_2, REb_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 14
        { .positions = {CHORD_3, DO_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 15

        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 16
        { .positions = {CHORD_3, DO_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 17
        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 18
        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 19
        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 20
        { .positions = {CHORD_3, DO_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 21
        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 22

        { .positions = {CHORD_4, DO_4}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 23
        { .positions = {CHORD_4, DO_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 24
        { .positions = {CHORD_4, FA_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 25
        { .positions = {CHORD_4, FA_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 26
        { .positions = {CHORD_4, SO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 27
        { .positions = {CHORD_4, SO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 28
        { .positions = {CHORD_4, LAb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 29
        { .positions = {CHORD_4, LAb_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 30
        { .positions = {CHORD_4, LAb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 31

        // line 48
        // ================== 12 (384-415) ==================
        { .positions = {CHORD_1, FA_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 0
        { .positions = {CHORD_1, FA_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 1
        { .positions = {CHORD_1, FA_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 2
        { .positions = {CHORD_1, FA_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 3
        { .positions = {CHORD_1, FA_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 4
        { .positions = {CHORD_1, MIb_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 5
        { .positions = {CHORD_1, FA_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 6

        { .positions = {CHORD_2, SO_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 7
        { .positions = {CHORD_2, SO_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 8
        { .positions = {CHORD_2, SO_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 9
        { .positions = {CHORD_2, SO_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 10
        { .positions = {CHORD_2, SO_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 11
        { .positions = {CHORD_2, SO_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 12
        { .positions = {CHORD_2, SO_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 13
        { .positions = {CHORD_2, SO_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 14
        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 15

        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 16
        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 17
        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 18
        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 19
        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 20
        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 21
        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 22

        { .positions = {CHORD_4, DO_4}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 23
        { .positions = {CHORD_4, REb_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 24
        { .positions = {CHORD_4, DO_4}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 25
        { .positions = {CHORD_4, DO_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 26
        { .positions = {CHORD_4, DO_4}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 27
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 28
        { .positions = {CHORD_4, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 29
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 30
        { .positions = {CHORD_4, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 31

        // line 52
        // ================== 13 (416-447) ==================
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 0
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 1
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 2
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 3
        { .positions = {CHORD_1, REb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 4
        { .positions = {CHORD_1, REb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 5
        { .positions = {CHORD_1, REb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 6

        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 7
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 8
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 9
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 10
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 11
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 12
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 13
        { .positions = {CHORD_3, SO_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 14
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 15

        { .positions = {CHORD_3, SO_2}, .pressed = {0, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 16
        { .positions = {CHORD_3, SO_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 17
        { .positions = {CHORD_3, SO_2}, .pressed = {0, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 18
        { .positions = {CHORD_3, SO_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 19
        { .positions = {CHORD_3, DO_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 20
        { .positions = {CHORD_3, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 21
        { .positions = {CHORD_3, SO_2}, .pressed = {0, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 22

        { .positions = {CHORD_4, LAb_2}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 23
        { .positions = {CHORD_4, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 24
        { .positions = {CHORD_4, LAb_2}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 25
        { .positions = {CHORD_4, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 26
        { .positions = {CHORD_4, LAb_2}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 27
        { .positions = {CHORD_4, FA_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 28
        { .positions = {CHORD_4, MIb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 29
        { .positions = {CHORD_4, FA_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 30
        { .positions = {CHORD_4, LA_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 31

        // ================== 5 (128-159) ==================
        { .positions = {CHORD_1, REb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 0
        { .positions = {CHORD_1, REb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 1
        { .positions = {CHORD_1, REb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 2
        { .positions = {CHORD_1, REb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 3
        { .positions = {CHORD_1, REb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 4
        { .positions = {CHORD_1, REb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 5
        { .positions = {CHORD_1, REb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 6

        { .positions = {CHORD_2, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 7
        { .positions = {CHORD_2, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 8
        { .positions = {CHORD_2, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 9
        { .positions = {CHORD_2, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 10
        { .positions = {CHORD_2, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 11
        { .positions = {CHORD_2, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 12
        { .positions = {CHORD_2, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 13
        { .positions = {CHORD_2, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 14
        { .positions = {CHORD_3, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 15

        { .positions = {CHORD_3, REb_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 16
        { .positions = {CHORD_3, REb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 17
        { .positions = {CHORD_3, REb_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 18
        { .positions = {CHORD_3, REb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 19
        { .positions = {CHORD_3, REb_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 20
        { .positions = {CHORD_3, REb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 21
        { .positions = {CHORD_3, REb_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 22

        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 23
        { .positions = {CHORD_4, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 24
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 25
        { .positions = {CHORD_4, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 26
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 27
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 28
        { .positions = {CHORD_4, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 29
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 30
        { .positions = {CHORD_4, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 31

        // ================== 6 (160-191) ==================
        { .positions = {CHORD_1, REb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 0
        { .positions = {CHORD_1, REb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 1
        { .positions = {CHORD_1, REb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 2
        { .positions = {CHORD_1, REb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 3
        { .positions = {CHORD_1, REb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 4
        { .positions = {CHORD_1, REb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 5
        { .positions = {CHORD_1, REb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 6

        { .positions = {CHORD_2, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 7
        { .positions = {CHORD_2, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 8
        { .positions = {CHORD_2, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 9
        { .positions = {CHORD_2, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 10
        { .positions = {CHORD_2, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 11
        { .positions = {CHORD_2, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 12
        { .positions = {CHORD_2, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 13
        { .positions = {CHORD_2, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 14
        { .positions = {CHORD_3, LAb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 15

        { .positions = {CHORD_3, MIb_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 16
        { .positions = {CHORD_3, SIb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 17
        { .positions = {CHORD_3, SIb_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 18
        { .positions = {CHORD_3, SIb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 19
        { .positions = {CHORD_3, SIb_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 20
        { .positions = {CHORD_3, SIb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 21
        { .positions = {CHORD_3, SIb_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 22

        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 23
        { .positions = {CHORD_4, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 24
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 25
        { .positions = {CHORD_4, MIb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 26
        { .positions = {CHORD_4, LAb_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 27
        { .positions = {CHORD_4, LAb_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 28
        { .positions = {CHORD_4, LAb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 29
        { .positions = {CHORD_4, SO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 30
        { .positions = {CHORD_4, FA_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 31
        
        { .positions = {CHORD_1, FA_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 0
        { .positions = {CHORD_1, FA_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 1
        { .positions = {CHORD_1, FA_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 2
        { .positions = {CHORD_1, FA_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 3
        { .positions = {CHORD_1, FA_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 4
        { .positions = {CHORD_1, FA_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 5
        { .positions = {CHORD_1, FA_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 6

        { .positions = {CHORD_2, MIb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 7
        { .positions = {CHORD_2, MIb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 8
        { .positions = {CHORD_2, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 9
        { .positions = {CHORD_2, MIb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 10
        { .positions = {CHORD_2, LAb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 11
        { .positions = {CHORD_2, LAb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 12
        { .positions = {CHORD_2, LAb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 13
        { .positions = {CHORD_2, SO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 14
        
        { .positions = {CHORD_3, MIb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 15
        { .positions = {CHORD_3, MIb_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 16
        { .positions = {CHORD_3, MIb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 17
        { .positions = {CHORD_3, MIb_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 18
        { .positions = {CHORD_3, MIb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 19
        { .positions = {CHORD_3, MIb_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 20
        { .positions = {CHORD_3, MIb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 21
        { .positions = {CHORD_3, MIb_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 22

        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 23
        { .positions = {CHORD_4, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 24
        { .positions = {CHORD_4, MIb_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 25
        { .positions = {CHORD_4, LAb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 26
        { .positions = {CHORD_4, SO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 27
        { .positions = {CHORD_4, MIb_4}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 28
        { .positions = {CHORD_4, MIb_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 29
        { .positions = {CHORD_4, MIb_4}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 30
        { .positions = {CHORD_4, MIb_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 31

        // line 40
        // ================== 10 (320-351) ==================
        { .positions = {CHORD_1, FA_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 0
        { .positions = {CHORD_1, FA_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 1
        { .positions = {CHORD_1, FA_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 2
        { .positions = {CHORD_1, FA_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 3
        { .positions = {CHORD_1, FA_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 4
        { .positions = {CHORD_1, MIb_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 5
        { .positions = {CHORD_1, FA_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 6

        { .positions = {CHORD_2, MIb_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 7
        { .positions = {CHORD_2, MIb_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 8
        { .positions = {CHORD_2, MIb_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 9
        { .positions = {CHORD_2, SIb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 10
        { .positions = {CHORD_2, DO_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 11
        { .positions = {CHORD_2, DO_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 12
        { .positions = {CHORD_2, REb_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 13
        { .positions = {CHORD_2, REb_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 14
        { .positions = {CHORD_3, REb_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 15

        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 16
        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 17
        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 18
        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 19
        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 20
        { .positions = {CHORD_3, DO_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 21
        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 22

        { .positions = {CHORD_4, DO_4}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 23
        { .positions = {CHORD_4, DO_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 24
        { .positions = {CHORD_4, DO_4}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 25
        { .positions = {CHORD_4, FA_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 26
        { .positions = {CHORD_4, SO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 27
        { .positions = {CHORD_4, SO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 28
        { .positions = {CHORD_4, LAb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 29
        { .positions = {CHORD_4, LAb_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 30
        { .positions = {CHORD_4, LAb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 31

        // line 44
        // ================== 11 (352-383) ==================
        { .positions = {CHORD_1, FA_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 0
        { .positions = {CHORD_1, FA_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 1
        { .positions = {CHORD_1, FA_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 2
        { .positions = {CHORD_1, FA_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 3
        { .positions = {CHORD_1, FA_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 4
        { .positions = {CHORD_1, MIb_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 5
        { .positions = {CHORD_1, FA_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 6

        { .positions = {CHORD_2, MIb_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 7
        { .positions = {CHORD_2, MIb_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 8
        { .positions = {CHORD_2, MIb_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 9
        { .positions = {CHORD_2, SIb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 10
        { .positions = {CHORD_2, DO_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 11
        { .positions = {CHORD_2, DO_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 12
        { .positions = {CHORD_2, REb_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 13
        { .positions = {CHORD_2, REb_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 14
        { .positions = {CHORD_3, DO_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 15

        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 16
        { .positions = {CHORD_3, DO_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 17
        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 18
        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 19
        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 20
        { .positions = {CHORD_3, DO_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 21
        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 22

        { .positions = {CHORD_4, DO_4}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 23
        { .positions = {CHORD_4, DO_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 24
        { .positions = {CHORD_4, FA_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 25
        { .positions = {CHORD_4, FA_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 26
        { .positions = {CHORD_4, SO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 27
        { .positions = {CHORD_4, SO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 28
        { .positions = {CHORD_4, LAb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 29
        { .positions = {CHORD_4, LAb_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 30
        { .positions = {CHORD_4, LAb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 31

        // line 48
        // ================== 12 (384-415) ==================
        { .positions = {CHORD_1, FA_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 0
        { .positions = {CHORD_1, FA_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 1
        { .positions = {CHORD_1, FA_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 2
        { .positions = {CHORD_1, FA_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 3
        { .positions = {CHORD_1, FA_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 4
        { .positions = {CHORD_1, MIb_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 5
        { .positions = {CHORD_1, FA_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 6

        { .positions = {CHORD_2, SO_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 7
        { .positions = {CHORD_2, SO_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 8
        { .positions = {CHORD_2, SO_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 9
        { .positions = {CHORD_2, SO_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 10
        { .positions = {CHORD_2, SO_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 11
        { .positions = {CHORD_2, SO_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 12
        { .positions = {CHORD_2, SO_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 13
        { .positions = {CHORD_2, SO_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 14
        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 15

        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 16
        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 17
        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 18
        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 19
        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 20
        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 21
        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 22

        { .positions = {CHORD_4, DO_4}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 23
        { .positions = {CHORD_4, REb_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 24
        { .positions = {CHORD_4, DO_4}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 25
        { .positions = {CHORD_4, DO_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 26
        { .positions = {CHORD_4, DO_4}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 27
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 28
        { .positions = {CHORD_4, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 29
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 30
        { .positions = {CHORD_4, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 31

        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 0
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 1
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 2
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 3
        { .positions = {CHORD_1, REb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 4
        { .positions = {CHORD_1, REb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 5
        { .positions = {CHORD_1, REb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 6

        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 7
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 8
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 9
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 10
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 11
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 12
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 13
        { .positions = {CHORD_3, SO_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 14
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 15

        { .positions = {CHORD_3, SO_2}, .pressed = {0, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 16
        { .positions = {CHORD_3, SO_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 17
        { .positions = {CHORD_3, SO_2}, .pressed = {0, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 18
        { .positions = {CHORD_3, SO_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 19
        { .positions = {CHORD_3, DO_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 20
        { .positions = {CHORD_3, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 21
        { .positions = {CHORD_3, SO_2}, .pressed = {0, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 22

        { .positions = {CHORD_4, LAb_2}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 23
        { .positions = {CHORD_4, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 24
        { .positions = {CHORD_4, LAb_2}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 25
        { .positions = {CHORD_4, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 26
        { .positions = {CHORD_4, LAb_2}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 27
        { .positions = {CHORD_4, LAb_2}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 28
        { .positions = {CHORD_4, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 29
        { .positions = {CHORD_4, LAb_2}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 30
        { .positions = {CHORD_4, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 31

        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 0
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 1
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 2
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 3
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 4
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 5
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 6

        { .positions = {CHORD_2, REb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 7
        { .positions = {CHORD_2, REb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 8
        { .positions = {CHORD_2, REb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 9
        { .positions = {CHORD_2, REb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 10
        { .positions = {CHORD_2, REb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 11
        { .positions = {CHORD_2, REb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 12
        { .positions = {CHORD_2, REb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 13
        { .positions = {CHORD_2, REb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 14
        { .positions = {CHORD_2, REb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 15

        { .positions = {CHORD_3, DO_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 16
        { .positions = {CHORD_3, DO_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 17
        { .positions = {CHORD_3, DO_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 18
        { .positions = {CHORD_3, DO_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 19
        { .positions = {CHORD_3, DO_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 20
        { .positions = {CHORD_3, DO_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 21
        { .positions = {CHORD_3, DO_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 22

        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 23
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 24
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 25
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 26
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 27
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 28
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 29
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 30
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 31

        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 0
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 1
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 2
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 3
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 4
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 5
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 6

        { .positions = {CHORD_2, REb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 7
        { .positions = {CHORD_2, REb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 8
        { .positions = {CHORD_2, REb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 9
        { .positions = {CHORD_2, REb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 10
        { .positions = {CHORD_2, REb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 11
        { .positions = {CHORD_2, REb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 12
        { .positions = {CHORD_2, REb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 13
        { .positions = {CHORD_2, REb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 14
        { .positions = {CHORD_2, REb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 15

        { .positions = {CHORD_3, DO_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 16
        { .positions = {CHORD_3, DO_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 17
        { .positions = {CHORD_3, DO_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 18
        { .positions = {CHORD_3, DO_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 19
        { .positions = {CHORD_3, DO_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 20
        { .positions = {CHORD_3, DO_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 21
        { .positions = {CHORD_3, DO_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 22

        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 23
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 24
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 25
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 26
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 27
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 28
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 29
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {1, 1} }, // 30
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 31

        { .positions = {0.0f, 0.0f}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // home
        { .positions = {1000.0f, 1000.0f}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }  // end flag
    };

    for (int i = 0; i < 1024; i++) {
        chord_events[i] = debug_data[i];
    }
}

// stucco song:
void load_debug_song3(void) {
    
    // temp array that have all info

    // long press --> hold to next instance
    // left hand structure: white (space) flat white white white flat white
    static const ChordEvent_t debug_data[1024] = {

        { .positions = {L_B1, R_G2}, .pressed = {1, 0, 1, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = HALF_SL, .long_pressed = {1, 1} },
        { .positions = {L_B1, R_G2}, .pressed = {1, 0, 1, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = QUARTER_SL, .long_pressed = {0, 0} },


        { .positions = {L_E2, R_D2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_D2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_D2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_SL, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_G2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },

        { .positions = {L_B1, R_G2}, .pressed = {1, 0, 1, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {1, 1} },
        { .positions = {L_B1, R_G2}, .pressed = {1, 0, 1, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {1, 1} },
        { .positions = {L_B1, R_G2}, .pressed = {1, 0, 1, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {1, 0} },
        { .positions = {L_B1, R_F2}, .pressed = {1, 0, 1, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },

        { .positions = {L_B1, R_G2}, .pressed = {1, 0, 1, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {1, 1} },
        { .positions = {L_B1, R_G2}, .pressed = {1, 0, 1, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {1, 1} },
        { .positions = {L_B1, R_G2}, .pressed = {1, 0, 1, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {1, 0} },
        { .positions = {L_B1, R_F2}, .pressed = {1, 0, 1, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },

        { .positions = {L_B1, R_G2}, .pressed = {1, 0, 1, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {1, 1} },
        { .positions = {L_B1, R_G2}, .pressed = {1, 0, 1, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {1, 1} },
        { .positions = {L_B1, R_G2}, .pressed = {1, 0, 1, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },


        { .positions = {L_G2, R_D2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },
        { .positions = {L_G2, R_D2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 1, 0, 1}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },
        { .positions = {L_G2, R_D2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },
        { .positions = {L_G2, R_B3}, .pressed = {1, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },
        { .positions = {L_B1, R_D2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },

        { .positions = {L_B1, R_D2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = HALF_SL, .long_pressed = {1, 1} },
        { .positions = {L_B1, R_D2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {1, 0} },

        { .positions = {L_B1, R_E2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },
        { .positions = {L_B1, R_D2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_SL, .long_pressed = {1, 0} },  
        { .positions = {L_B1, R_D2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },


        { .positions = {L_B1, R_G2}, .pressed = {1, 0, 1, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = HALF_SL, .long_pressed = {0, 0} },


        { .positions = {L_E2, R_D2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_D2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_D2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_SL, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_G2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },

        { .positions = {L_B1, R_G2}, .pressed = {1, 0, 1, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {1, 1} },
        { .positions = {L_B1, R_G2}, .pressed = {1, 0, 1, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {1, 1} },
        { .positions = {L_B1, R_G2}, .pressed = {1, 0, 1, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {1, 0} },
        { .positions = {L_B1, R_F2}, .pressed = {1, 0, 1, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },

        { .positions = {L_B1, R_G2}, .pressed = {1, 0, 1, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {1, 1} },
        { .positions = {L_B1, R_G2}, .pressed = {1, 0, 1, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {1, 1} },
        { .positions = {L_B1, R_G2}, .pressed = {1, 0, 1, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {1, 0} },
        { .positions = {L_B1, R_F2}, .pressed = {1, 0, 1, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },

        { .positions = {L_B1, R_G2}, .pressed = {1, 0, 1, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {1, 1} },
        { .positions = {L_B1, R_G2}, .pressed = {1, 0, 1, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {1, 1} },
        { .positions = {L_B1, R_G2}, .pressed = {1, 0, 1, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },

        { .positions = {L_G2, R_D2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },
        { .positions = {L_G2, R_D2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 1, 0, 1}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },
        { .positions = {L_G2, R_D2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },
        { .positions = {L_G2, R_B3}, .pressed = {1, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },
        { .positions = {L_B1, R_D2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },

        { .positions = {L_B1, R_D2}, .pressed = {1, 0, 1, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = HALF_SL, .long_pressed = {1, 1} },

        { .positions = {L_B1, R_D2}, .pressed = {1, 0, 1, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = HALF_SL, .long_pressed = {0, 0} },

        { .positions = {L_A2, R_D2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = QUARTER_SL, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_D2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = QUARTER_SL, .long_pressed = {0, 0} },
        { .positions = {L_G2, R_D2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = QUARTER_SL, .long_pressed = {0, 0} },
        { .positions = {L_G2, R_E2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1}, .duration_ms = QUARTER_SL, .long_pressed = {1, 0} },

        { .positions = {L_G2, R_G2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 1, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },
        { .positions = {L_F2, R_G2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },

        { .positions = {L_F2, R_A2}, .pressed = {1, 0, 1, 0, 0, 1, 0, 0, 0, 1}, .duration_ms = EIGHTH_SL, .long_pressed = {1, 1} },
        { .positions = {L_F2, R_A2}, .pressed = {1, 0, 1, 0, 0, 1, 0, 0, 0, 1}, .duration_ms = EIGHTH_SL, .long_pressed = {1, 1} },
        { .positions = {L_F2, R_A2}, .pressed = {1, 0, 1, 0, 0, 1, 0, 0, 0, 1}, .duration_ms = EIGHTH_SL, .long_pressed = {1, 0} },

        { .positions = {L_F2, R_G2}, .pressed = {1, 0, 1, 0, 0, 1, 0, 0, 0, 1}, .duration_ms = EIGHTH_SL, .long_pressed = {1, 0} },

        { .positions = {L_F2, R_G2}, .pressed = {1, 0, 1, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = QUARTER_SL, .long_pressed = {0, 0} },

        { .positions = {L_G2, R_G2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },
        { .positions = {L_F2, R_A2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },

        { .positions = {L_E2, R_B2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = QUARTER_SL, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_B2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },

        { .positions = {L_D2, R_A2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },

        { .positions = {L_D2, R_G2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1}, .duration_ms = QUARTER_SL, .long_pressed = {1, 0} },

        { .positions = {L_D2, R_A2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },
        { .positions = {L_C2, R_B2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },

        { .positions = {L_C2, R_D1}, .pressed = {1, 0, 0, 1, 0, 1, 0, 0, 1, 0}, .duration_ms = QUARTER_SL, .long_pressed = {1, 1} },
        { .positions = {L_C2, R_D1}, .pressed = {1, 0, 0, 1, 0, 1, 0, 0, 1, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },

        { .positions = {L_F2, R_D1}, .pressed = {1, 0, 0, 1, 0, 1, 0, 1, 0, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {1, 0} },
        { .positions = {L_F2, R_G2}, .pressed = {1, 0, 0, 1, 0, 1, 0, 0, 1, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {1, 0} },
        { .positions = {L_F2, R_D2}, .pressed = {1, 0, 0, 1, 0, 1, 0, 0, 0, 1}, .duration_ms = EIGHTH_SL, .long_pressed = {1, 0} },
        { .positions = {L_F2, R_D2}, .pressed = {1, 0, 0, 1, 0, 1, 0, 0, 1, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },
        { .positions = {L_B2, R_D2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },

        { .positions = {L_B2, R_D2}, .pressed = {0, 1, 0, 1, 0, 0, 0, 0, 1, 0}, .duration_ms = QUARTER_SL, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_D2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = QUARTER_SL, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_E2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = QUARTER_SL, .long_pressed = {0, 0} },





        { .positions = {L_G2, R_G2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 1, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },
        { .positions = {L_F2, R_G2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },

        { .positions = {L_F2, R_A2}, .pressed = {1, 0, 1, 0, 0, 1, 0, 0, 0, 1}, .duration_ms = EIGHTH_SL, .long_pressed = {1, 1} },
        { .positions = {L_F2, R_A2}, .pressed = {1, 0, 1, 0, 0, 1, 0, 0, 0, 1}, .duration_ms = EIGHTH_SL, .long_pressed = {1, 1} },
        { .positions = {L_F2, R_A2}, .pressed = {1, 0, 1, 0, 0, 1, 0, 0, 0, 1}, .duration_ms = EIGHTH_SL, .long_pressed = {1, 0} },

        { .positions = {L_F2, R_G2}, .pressed = {1, 0, 1, 0, 0, 1, 0, 0, 0, 1}, .duration_ms = EIGHTH_SL, .long_pressed = {1, 0} },

        { .positions = {L_F2, R_G2}, .pressed = {1, 0, 1, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = QUARTER_SL, .long_pressed = {0, 0} },

        { .positions = {L_G2, R_G2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },
        { .positions = {L_F2, R_A2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },

        { .positions = {L_E2, R_B2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = QUARTER_SL, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_B2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },

        { .positions = {L_D2, R_A2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },

        { .positions = {L_D2, R_G2}, .pressed = {1, 0, 1, 1, 0, 0, 1, 0, 0, 1}, .duration_ms = QUARTER_SL, .long_pressed = {1, 0} },

        { .positions = {L_D2, R_A2}, .pressed = {1, 0, 1, 1, 0, 0, 1, 0, 0, 1}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },
        { .positions = {L_C2, R_B2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },

        { .positions = {L_C2, R_C1}, .pressed = {1, 0, 1, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = QUARTER_SL, .long_pressed = {1, 1} },
        { .positions = {L_C2, R_C1}, .pressed = {1, 0, 1, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },

        { .positions = {L_C2, R_G2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },
        { .positions = {L_C2, R_G2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = QUARTER_SL, .long_pressed = {1, 0} },


        { .positions = {L_C2, R_G2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },
        { .positions = {L_B1, R_C1}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },

        { .positions = {L_B1, R_D1}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = QUARTER_SL, .long_pressed = {1, 0} },
        { .positions = {L_B1, R_D1}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },

        { .positions = {L_B1, R_A2}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },

        { .positions = {L_F2, R_D1}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = QUARTER_SL, .long_pressed = {0, 0} },



        { .positions = {L_E3, R_G2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = SIXTEENTH_SL, .long_pressed = {0, 0} },

        { .positions = {L_E3, R_G2}, .pressed = {0, 1, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = SIXTEENTH_SL, .long_pressed = {0, 0} },

        { .positions = {L_E3, R_G2}, .pressed = {0, 0, 0, 0, 1, 0, 0, 0, 0, 0}, .duration_ms = SIXTEENTH_SL, .long_pressed = {0, 0} },

        { .positions = {L_E3, R_G2}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 1} },

        { .positions = {L_B1, R_G2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 1} },
        { .positions = {L_E2, R_G2}, .pressed = {0, 1, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 1} },
        { .positions = {L_B2, R_G2}, .pressed = {0, 0, 1, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {1, 0} },

        { .positions = {L_B2, R_D2}, .pressed = {0, 0, 1, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 1} },
        { .positions = {L_B2, R_D2}, .pressed = {0, 0, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 1} },
        { .positions = {L_B2, R_D2}, .pressed = {0, 0, 1, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },




        { .positions = {L_D2, R_D2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },
        { .positions = {L_D2, R_D2}, .pressed = {0, 0, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },

        { .positions = {L_D2, R_D2}, .pressed = {0, 0, 0, 1, 0, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },
        { .positions = {L_D2, R_D2}, .pressed = {0, 0, 0, 0, 0, 0, 1, 0, 0, 1}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },

        { .positions = {L_D2, R_D2}, .pressed = {0, 0, 0, 1, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_SL, .long_pressed = {1, 0} },
        { .positions = {L_D2, R_G2}, .pressed = {0, 0, 0, 1, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },

        { .positions = {L_B2, R_G2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 1} },
        { .positions = {L_B2, R_G2}, .pressed = {0, 0, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 1} },
        { .positions = {L_B2, R_G2}, .pressed = {0, 0, 1, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 1} },
        { .positions = {L_B2, R_G2}, .pressed = {0, 0, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 1} },

        { .positions = {L_B2, R_G2}, .pressed = {0, 0, 1, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {1, 0} },
        { .positions = {L_B2, R_F2}, .pressed = {0, 0, 1, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {1, 0} },


        { .positions = {L_B2, R_G2}, .pressed = {0, 0, 1, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 1} },
        { .positions = {L_B2, R_G2}, .pressed = {0, 0, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 1} },
        { .positions = {L_B2, R_G2}, .pressed = {0, 0, 1, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {1, 1} },

        { .positions = {L_B2, R_G2}, .pressed = {0, 0, 1, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {1, 1} },
        { .positions = {L_B2, R_G2}, .pressed = {0, 0, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },

        { .positions = {L_B2, R_F2}, .pressed = {0, 0, 1, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },



        { .positions = {L_B2, R_G2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 1} },
        { .positions = {L_B2, R_G2}, .pressed = {0, 0, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 1} },
        { .positions = {L_B2, R_G2}, .pressed = {0, 0, 1, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {1, 0} },

        { .positions = {L_B2, R_D2}, .pressed = {0, 0, 1, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 1} },
        { .positions = {L_B2, R_D2}, .pressed = {0, 0, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },
        { .positions = {L_B2, R_D2}, .pressed = {0, 0, 1, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },




        { .positions = {L_B2, R_D2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 1} },
        { .positions = {L_B2, R_D2}, .pressed = {0, 0, 0, 0, 0, 0, 1, 0, 0, 1}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },

        { .positions = {L_B2, R_D2}, .pressed = {0, 0, 0, 1, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },

        { .positions = {L_G2, R_B3}, .pressed = {0, 1, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {1, 0} },
        { .positions = {L_G2, R_B3}, .pressed = {0, 0, 1, 0, 0, 0, 1, 1, 0, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },

        { .positions = {L_B2, R_D2}, .pressed = {0, 0, 1, 0, 0, 0, 1, 0, 0, 1}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },


        { .positions = {L_B2, R_D2}, .pressed = {1, 0, 1, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 1} },
        { .positions = {L_E2, R_D2}, .pressed = {0, 1, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 1} },
        { .positions = {L_F2, R_D2}, .pressed = {0, 0, 0, 1, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 1} },
        { .positions = {L_F2, R_D2}, .pressed = {0, 0, 0, 0, 0, 0, 1, 1, 0, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 1} },
        { .positions = {L_F2, R_D2}, .pressed = {0, 0, 0, 1, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 1} },
        { .positions = {L_F2, R_D2}, .pressed = {0, 0, 0, 0, 0, 0, 1, 1, 0, 0}, .duration_ms = EIGHTH_SL, .long_pressed = {0, 0} },

        { .positions = {L_G2, R_B3}, .pressed = {1, 0, 1, 0, 0, 0, 1, 1, 0, 1}, .duration_ms = HALF_SL, .long_pressed = {1, 1} },
        { .positions = {L_G2, R_B3}, .pressed = {1, 0, 1, 0, 0, 0, 1, 1, 0, 1}, .duration_ms = HALF_SL, .long_pressed = {1, 1} },
        { .positions = {L_G2, R_B3}, .pressed = {1, 0, 1, 0, 0, 0, 1, 1, 0, 1}, .duration_ms = HALF_SL, .long_pressed = {0, 0} },


        { .positions = {L_B1, R_D3}, .pressed = {1, 0, 0, 1, 0, 1, 0, 1, 0, 1}, .duration_ms = HALF_SL, .long_pressed = {1, 1} },
        { .positions = {L_B1, R_D3}, .pressed = {1, 0, 0, 1, 0, 1, 0, 1, 0, 1}, .duration_ms = HALF_SL, .long_pressed = {1, 1} },

        { .positions = {L_B1, R_D3}, .pressed = {1, 0, 0, 1, 0, 1, 0, 1, 0, 1}, .duration_ms = HALF_SL, .long_pressed = {0, 0} },


        // HOMING 
        { .positions = {L_C1, R_F1}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 500, .long_pressed = {0, 0} },
        { .positions = {1000.0, 1000.0}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 1000, .long_pressed = {0, 0} }

    }; 

    for (int i = 0; i < 1024; i++) {
        chord_events[i] = debug_data[i];
    }
}
