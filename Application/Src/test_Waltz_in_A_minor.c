
#include <application.h>
#include <demo_song.h>


void load_debug_song(void) {
    
    // temp array that have all info

    // long press --> hold to next instance
    // left hand structure: white (space) flat white white white flat white
    static const ChordEvent_t debug_data[1024] = {
        
        // ================== PAGE 1 ==================

        // ================== Line 1 ==================
        { .positions = {L_C1, R_G3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} },

        { .positions = {L_C1, R_A3}, .pressed = {0, 0, 0, 0, 0, 0, 1, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_C1, R_B3}, .pressed = {0, 0, 0, 0, 0, 0, 1, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_E2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_E2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} },
        
        { .positions = {L_D2, R_D2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_D2, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_F2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1}, .duration_ms = QUARTER, .long_pressed = {0, 1} },
        { .positions = {L_A2, R_F2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1}, .duration_ms = QUARTER, .long_pressed = {0, 0} },
        
        { .positions = {L_G1, R_D2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_G1, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_G2, R_F2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_G2, R_A2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_G2, R_G2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_G2, R_F2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },

        { .positions = {L_C2, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_C2, R_E2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_G1, R_E2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1}, .duration_ms = QUARTER, .long_pressed = {0, 1} },
        { .positions = {L_G1, R_E2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1}, .duration_ms = QUARTER, .long_pressed = {0, 0} },
    
        { .positions = {L_A1, R_C2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_A1, R_D2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_E2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_E2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} },
    
        
        // ================== Line 2 ==================
        { .positions = {L_D2, R_F2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_D2, R_G2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_A2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 1, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 1} },
        { .positions = {L_A2, R_A2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 1, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} },
    
        { .positions = {L_G1, R_D2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_G1, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_G2, R_F2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_G2, R_A2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_G2, R_G2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_G2, R_D2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        
        { .positions = {L_C2, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} },
        { .positions = {L_G2, R_E2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} },
        { .positions = {L_A1, R_G3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} },
        
        { .positions = {L_A1, R_A3}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_A1, R_B3}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_C2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = QUARTER, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_C2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = QUARTER, .long_pressed = {0, 0} },

        { .positions = {L_D2, R_D2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_D2, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_F2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1}, .duration_ms = QUARTER, .long_pressed = {0, 1} },
        { .positions = {L_A2, R_F2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1}, .duration_ms = QUARTER, .long_pressed = {0, 0} },
        
        { .positions = {L_G1, R_D2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_G1, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_G2, R_F2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_G2, R_A2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_G2, R_G2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_G2, R_F2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
    
        // ================== Line 3 ==================
        { .positions = {L_C2, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_C2, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_G2, R_E2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1}, .duration_ms = QUARTER, .long_pressed = {0, 1} },
        { .positions = {L_G2, R_E2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1}, .duration_ms = QUARTER, .long_pressed = {0, 0} },

        { .positions = {L_A1, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_A1, R_F2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_G2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_G2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} },

        { .positions = {L_D2, R_A2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_D2, R_B2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_C1}, .pressed = {1, 0, 0, 1, 0, 0, 1, 1, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 1} },
        { .positions = {L_A2, R_C1}, .pressed = {1, 0, 0, 1, 0, 0, 1, 1, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} },
    
        { .positions = {L_G1, R_G2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = QUARTER, .long_pressed = {0, 0} },
        { .positions = {L_G2, R_G2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_G2, R_G2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_G2, R_D1}, .pressed = {1, 0, 1, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_G2, R_A2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },

        { .positions = {L_C2, R_G2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 1} },
        { .positions = {L_G2, R_G2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 1, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 1} },
        { .positions = {L_G2, R_G2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 1, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} },

        { .positions = {L_E2, R_G2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_G2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_F2, R_A2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 1, 0}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_F2, R_A2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_G2, R_B2}, .pressed = {1, 0, 0, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_G2, R_C1}, .pressed = {1, 0, 0, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
    
        // ================== Line 4 ==================
        { .positions = {L_A1, R_B2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_A1, R_A2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_G2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_B2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_A2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1}, .duration_ms = QUARTER, .long_pressed = {0, 0} },

        { .positions = {L_E2, R_A2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_A2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_G2}, .pressed = {1, 0, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_A2}, .pressed = {1, 0, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_G2}, .pressed = {1, 0, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} },
        
        { .positions = {L_A1, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_A1, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_C2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_D2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_C2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 1, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} },
        
        { .positions = {L_E2, R_G3}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_A3}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_B3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_G3}, .pressed = {1, 0, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = SIXTEENTH, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_A3}, .pressed = {1, 0, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = SIXTEENTH, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_B3}, .pressed = {1, 0, 0, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = SIXTEENTH, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_G2}, .pressed = {1, 0, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = SIXTEENTH, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_A2}, .pressed = {1, 0, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = SIXTEENTH, .long_pressed = {0, 0} },
        { .positions = {L_G2, R_B2}, .pressed = {1, 0, 0, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = SIXTEENTH, .long_pressed = {1, 1} },
        { .positions = {L_G2, R_B2}, .pressed = {1, 0, 0, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = SIXTEENTH, .long_pressed = {1, 1} },
        { .positions = {L_G2, R_B2}, .pressed = {1, 0, 0, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = SIXTEENTH, .long_pressed = {1, 0} },
        { .positions = {L_G2, R_C1}, .pressed = {1, 0, 0, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = SIXTEENTH, .long_pressed = {0, 0} },
        
        { .positions = {L_A1, R_B2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_A1, R_A2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_G2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_B2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_A2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1}, .duration_ms = QUARTER, .long_pressed = {0, 0} },

        // ================== Line 5 ==================
        { .positions = {L_E2, R_A2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_A2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_G2}, .pressed = {1, 0, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_E1}, .pressed = {1, 0, 0, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_E1}, .pressed = {1, 0, 0, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = QUARTER, .long_pressed = {0, 0} },

        { .positions = {L_A1, R_C2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 1} },
        { .positions = {L_E2, R_C2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 1, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_C2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} },
    
        { .positions = {L_A1, R_C2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_A1, R_D2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_E2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_E2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} },

        { .positions = {L_D2, R_F2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_D2, R_G2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_A2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 1, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 1} },
        { .positions = {L_A2, R_A2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 1, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} },

        { .positions = {L_G1, R_D2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },      
        { .positions = {L_G1, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },        
        { .positions = {L_G2, R_F2}, .pressed = {1, 0, 1, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },        
        { .positions = {L_G2, R_A2}, .pressed = {1, 0, 1, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },  
        { .positions = {L_G2, R_G2}, .pressed = {1, 0, 1, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },        
        { .positions = {L_G2, R_F2}, .pressed = {1, 0, 1, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {0, 0} }, 

        { .positions = {L_C2, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },        
        { .positions = {L_C2, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH, .long_pressed = {0, 0} }, 
        { .positions = {L_G2, R_E2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1}, .duration_ms = QUARTER, .long_pressed = {0, 1} },    
        { .positions = {L_G2, R_E2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1}, .duration_ms = QUARTER, .long_pressed = {0, 0} },  
    
        // ================== PAGE 2 ==================
        // ================== Line 1 ==================
        { .positions = {L_A1, R_C2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_A1, R_D2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_E2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_E2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} },
    
        { .positions = {L_D2, R_F2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_D2, R_G2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_A2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 1, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 1} },
        { .positions = {L_A2, R_A2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 1, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} },
    
        { .positions = {L_B2, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_B2, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_F2, R_G2}, .pressed = {0, 0, 1, 1, 0, 1, 0, 0, 1, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} }, // could be different
        { .positions = {L_F2, R_G2}, .pressed = {0, 0, 1, 1, 0, 1, 0, 0, 1, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} }, // could be different
    
        { .positions = {L_E2, R_A2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_A2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_B2}, .pressed = {1, 0, 0, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = QUARTER, .long_pressed = {0, 1} },
        { .positions = {L_E2, R_B2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = QUARTER, .long_pressed = {0, 0} }, // quarter rest

        { .positions = {L_E2, R_D1}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_D1}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_D1}, .pressed = {1, 0, 1, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_E1}, .pressed = {1, 0, 1, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {0, 0} }, // cannot play this note (out of reach)
        { .positions = {L_E2, R_E1}, .pressed = {1, 0, 1, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_D1}, .pressed = {1, 0, 1, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },

        { .positions = {L_A1, R_D1}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_A1, R_D1}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_C1}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_A2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 1, 0}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_A2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_G2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 1, 0}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
    
        // ================== Line 2 ==================
        { .positions = {L_E2, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_D2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_C2}, .pressed = {1, 0, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_G2}, .pressed = {1, 0, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_G2}, .pressed = {1, 0, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} },

        { .positions = {L_A1, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_A1, R_D2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_C2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_G2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 1, 0}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_G2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 1, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} },

        { .positions = {L_E2, R_D1}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_D1}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_D1}, .pressed = {1, 0, 1, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_E1}, .pressed = {1, 0, 1, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {0, 0} }, // cannot play this note
        { .positions = {L_E2, R_E1}, .pressed = {1, 0, 1, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_D1}, .pressed = {1, 0, 1, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },

        { .positions = {L_A1, R_D1}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_A1, R_D1}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_C1}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_A2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 1, 0}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_A2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_G2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 1, 0}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },

        { .positions = {L_E2, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_D2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_F2, R_A3}, .pressed = {0, 0, 0, 1, 0, 0, 1, 0, 1, 0}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_F2, R_F2}, .pressed = {0, 0, 0, 1, 0, 0, 1, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_F2, R_E2}, .pressed = {0, 0, 0, 1, 0, 0, 1, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_F2, R_A3}, .pressed = {0, 0, 0, 1, 0, 0, 1, 0, 1, 0}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
    
        // ================== Line 3 ==================
        { .positions = {L_A1, R_A3}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = QUARTER, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_C2}, .pressed = {1, 0, 0, 0, 0, 0, 1, 0, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} },
        { .positions = {L_A1, R_G3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} },

        { .positions = {L_A1, R_A3}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_A1, R_B3}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_E2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_E2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} },

        { .positions = {L_D2, R_F2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_D2, R_G2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_A2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 1, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 1} },
        { .positions = {L_A2, R_A2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 1, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} },

        { .positions = {L_G1, R_D2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_G1, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_G2, R_F2}, .pressed = {1, 0, 1, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_G2, R_A2}, .pressed = {1, 0, 1, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_G2, R_G2}, .pressed = {1, 0, 1, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_G2, R_F2}, .pressed = {1, 0, 1, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        
        { .positions = {L_C2, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_C2, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_G2, R_E2}, .pressed = {1, 0, 0, 1, 0, 1, 0, 0, 0, 1}, .duration_ms = QUARTER, .long_pressed = {0, 1} },
        { .positions = {L_G2, R_E2}, .pressed = {1, 0, 0, 1, 0, 1, 0, 0, 0, 1}, .duration_ms = QUARTER, .long_pressed = {0, 0} },

        { .positions = {L_A1, R_C2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_A1, R_D2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_E2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_E2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} },

        // ================== Line 4 ==================
        { .positions = {L_D2, R_F2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_D2, R_G2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_A2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 1, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 1} },
        { .positions = {L_A2, R_A2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 1, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} },

        { .positions = {L_G1, R_D2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_G1, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_F2}, .pressed = {0, 0, 1, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_A2}, .pressed = {0, 0, 1, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_G2, R_G2}, .pressed = {1, 0, 0, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {1, 0} }, // could be changed
        { .positions = {L_G2, R_D2}, .pressed = {1, 0, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },

        { .positions = {L_C2, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} },
        { .positions = {L_G2, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 1, 0, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} },
        { .positions = {L_A1, R_G3}, .pressed = {0, 0, 0, 0, 0, 0, 0,  0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} },

        { .positions = {L_A1, R_C2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_A1, R_D2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_E2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_E2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} },

        { .positions = {L_D2, R_F2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_D2, R_G2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_A2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 1, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 1} },
        { .positions = {L_A2, R_A2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 1, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} },

        { .positions = {L_E2, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_E2}, .pressed = {1, 0, 1, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_E2}, .pressed = {1, 0, 1, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_B2}, .pressed = {1, 0, 1, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_F2}, .pressed = {1, 0, 1, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },

        // ================== Line 5 ==================
        { .positions = {L_A1, R_D2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 1} },
        { .positions = {L_E2, R_D2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 1, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_D2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} },

        { .positions = {L_C2, R_B2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_C2, R_B2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_A2}, .pressed = {1, 0, 0, 1, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_G2}, .pressed = {1, 0, 0, 1, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_D2, R_F2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_D2, R_E2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },

        { .positions = {L_D2, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_D2, R_D2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_G2, R_D2}, .pressed = {0, 0, 1, 0, 0, 0, 1, 0, 1, 0}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_G2, R_D2}, .pressed = {0, 0, 1, 0, 0, 0, 1, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_E2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_F2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        
        { .positions = {L_E2, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = QUARTER, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 1, 0, 1, 0}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 1, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_F2}, .pressed = {1, 0, 0, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_E2, R_A3}, .pressed = {1, 0, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },

        { .positions = {L_A1, R_A3}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = QUARTER, .long_pressed = {0, 1} },
        { .positions = {L_E2, R_A3}, .pressed = {1, 0, 0, 0, 0, 0, 1, 0, 0, 1}, .duration_ms = QUARTER, .long_pressed = {0, 0} },
        { .positions = {L_E2, R_A3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} },
        
        // HOMING 
        { .positions = {L_C1, R_F1}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 2000, .long_pressed = {0, 0} },
        
    }; 

    for (int i = 0; i < 1024; i++) {
        chord_events[i] = debug_data[i];
    }
}
