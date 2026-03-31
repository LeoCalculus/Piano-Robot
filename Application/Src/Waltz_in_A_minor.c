
#include <application.h>
#include <Waltz_in_A_minor.h>


void load_debug_song(void) {
    
    // temp array that have all info

    // long press --> hold to next instance
    // left hand structure: white (space) flat white white white flat white
    static const ChordEvent_t debug_data[128] = {

        // ================== Line 1 ==================
        { .positions = {L_C1, R_G3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = QUARTER, .long_pressed = {0, 0} },

        { .positions = {L_C1, R_A3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_B3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_C2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = QUARTER, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_C2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = QUARTER, .long_pressed = {0, 0} },
        
        { .positions = {L_D1, R_D2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_D1, R_E2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_A2, R_F2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1}, .duration_ms = QUARTER, .long_pressed = {0, 1} },
        { .positions = {L_A2, R_F2}, .pressed = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1}, .duration_ms = QUARTER, .long_pressed = {0, 0} },
        
        { .positions = {L_G1, R_B3}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_G1, R_C2}, .pressed = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
        { .positions = {L_G2, R_D2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {1, 0} },
        { .positions = {L_G2, R_A2}, .pressed = {1, 0, 1, 0, 1, 0, 0, 0, 0, 1}, .duration_ms = EIGHTH, .long_pressed = {0, 0} },
    };

    for (int i = 0; i < 128; i++) {
        chord_events[i] = debug_data[i];
    }
}