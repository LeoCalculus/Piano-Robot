#ifndef DEBUG_SONG_H
#define DEBUG_SONG_H

#include <application.h>

/* Octave 1 */
#define DO_1    0.0f
#define RE_1    -21.0f
#define MI_1    -42.0f
#define FA_1    -63.0f
#define SO_1    -84.0f
#define LA_1    -105.0f
#define SI_1    -126.0f

/* Octave 2 */
#define DO_2    -147.0f
#define RE_2    -168.0f
#define MI_2    -189.0f
#define FA_2    -210.0f
#define SO_2    -231.0f
#define LA_2    -252.0f
#define SI_2    -273.0f

/* Octave 3 */
#define DO_3    -294.0f
#define RE_3    -315.0f
#define MI_3    -336.0f
#define FA_3    -357.0f
#define SO_3    -378.0f
#define LA_3    -399.0f
#define SI_3    -420.0f

// Function to load a hardcoded song into chord_events for testing
void load_debug_song(void);

#endif