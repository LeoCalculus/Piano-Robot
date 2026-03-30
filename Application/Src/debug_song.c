#include <debug_song.h>


void load_debug_song(void){
    // DO
    chord_events[0] = (ChordEvent_t){
        .positions   = {DO_1, DO_2},
        .pressed     = {true, false, false, false, false, false, false, false, false, false},
        .duration_ms = 500
    };
    // RE
    chord_events[1] = (ChordEvent_t){
        .positions   = {RE_1, RE_2},
        .pressed     = {false, true, false, false, false, false, false, false, false, false},
        .duration_ms = 500
    };
    // MI
    chord_events[2] = (ChordEvent_t){
        .positions   = {MI_1, MI_2},
        .pressed     = {false, false, true, false, false, false, false, false, false, false},
        .duration_ms = 500
    };
    // FA
    chord_events[3] = (ChordEvent_t){
        .positions   = {FA_1, FA_2},
        .pressed     = {false, false, false, true, false, false, false, false, false, false},
        .duration_ms = 500
    };
    // SO
    chord_events[4] = (ChordEvent_t){
        .positions   = {SO_1, SO_2},
        .pressed     = {false, false, false, false, true, false, false, false, false, false},
        .duration_ms = 500
    };
}