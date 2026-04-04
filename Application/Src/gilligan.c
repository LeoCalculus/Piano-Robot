
#include <application.h>
#include <Waltz_in_A_minor.h>


void load_debug_song(void) {
    
    // temp array that have all info

    // long press --> hold to next instance
    // left hand structure: white (space) flat white white white flat white
    static const ChordEvent_t debug_data[1024] = {
        // ================== PAGE 1 ==================

        // ================== Line 1 ==================
        { .positions = {L_A1, }, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} },

        { .positions = {L_A1, }, .pressed = {0, 0, 0, 1, 0, 0, 0, 0, 0, 0}, .duration_ms = QUARTER, .long_pressed = {1, 0} },
        { .positions = {L_A1, }, .pressed = {0, 0, 0, 1, 0, 0, 0, 0, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} },
        { .positions = {L_D2, }, .pressed = {0, 0, 1, 0, 1, 0, 0, 0, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} },
        { .positions = {L_A1, }, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_A1, }, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },

        { .positions = {L_A1, }, .pressed = {0, 0, 1, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = QUARTER, .long_pressed = {1, 0} },
        { .positions = {L_A1, }, .pressed = {0, 0, 1, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} },
        { .positions = {L_B1, }, .pressed = {0, 0, 0, 1, 0, 0, 1, 0, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} },
        { .positions = {L_B1, }, .pressed = {0, 0, 1, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = QUARTER, .long_pressed = {1, 0} },
        { .positions = {L_B1, }, .pressed = {0, 0, 1, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_B1, }, .pressed = {0, 0, 1, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_C2, }, .pressed = {0, 0, 0, 1, 0, 0, 1, 0, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} },
        { .positions = {L_C2, }, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} },

        // ================== Line 2 ==================
        { .positions = {L_C2, }, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = HALF, .long_pressed = {0, 0} },
        { .positions = {L_C2, }, .pressed = {0, 0, 1, 0, 1, 0, 0, 0, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} },
        { .positions = {L_C2, }, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} },
        
        { .positions = {L_C2, }, .pressed = {0, 0, 1, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = QUARTER, .long_pressed = {1, 0} },
        { .positions = {L_C2, }, .pressed = {0, 0, 1, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} },
        { .positions = {L_C2, }, .pressed = {0, 0, 0, 1, 0, 0, 1, 0, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} },

        // HOMING 
        { .positions = {L_C1, R_F1}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 2000, .long_pressed = {0, 0} },
        
    }; 

    for (int i = 0; i < 1024; i++) {
        chord_events[i] = debug_data[i];
    }
}
