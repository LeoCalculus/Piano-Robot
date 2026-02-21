#include "ELEC391_533.h"

/* 第一八度 (Octave 1) */
#define DO_1    0.0f
#define RE_1    -21.0f
#define MI_1    -42.0f
#define FA_1    -63.0f
#define SO_1    -84.0f
#define LA_1    -105.0f
#define SI_1    -126.0f

/* 第二八度 (Octave 2) */
#define DO_2    -147.0f
#define RE_2    -168.0f
#define MI_2    -189.0f
#define FA_2    -210.0f
#define SO_2    -231.0f
#define LA_2    -252.0f
#define SI_2    -273.0f

/* 第三八度 (Octave 3) */
#define DO_3    -294.0f
#define RE_3    -315.0f
#define MI_3    -336.0f
#define FA_3    -357.0f
#define SO_3    -378.0f
#define LA_3    -399.0f
#define SI_3    -420.0f


// ChordEvent_t SONG[SONG_EVENT] = {
//     {{DO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 500, 150},
//     {{MI_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 500, 150},
//     {{SO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 500, 150},
//     {{SO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 500, 150},
//     {{LA_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 500, 150},
//     {{SO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 500, 150},
//     {{MI_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 500, 150},
//     {{DO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 500, 150},
//     {{SO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
//     {{SO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
//     {{SO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
//     {{MI_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 500, 150},
//     {{DO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 500, 150},
//     {{SO_1, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
//     {{SO_1, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
//     {{SO_1, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
//     {{SO_1, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
//     {{SO_1, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
//     {{SO_1, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
//     {{DO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 500, 150},
//     {{0.0f, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 0, 150},
// };

ChordEvent_t SONG[SONG_EVENT] = {
    {{SO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 125, 150},
    {{SO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 125, 150},
    {{SO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 750, 150},

    {{SO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 125, 150},
    {{SO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 125, 150},
    {{SO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 750, 150},

    {{SO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 125, 150},
    {{SO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 125, 150},
    {{SO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
    {{FA_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
    {{MI_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
    {{RE_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
    {{DO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},

    // 五星红旗
    {{SO_1, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 375, 150},
    {{SO_1, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 125, 150},
    {{DO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 500, 150},
    {{SO_1, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 500, 150},
    {{MI_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 500, 150},
    {{DO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 500, 150},
    {{SO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 750, 150},
    {{LA_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
    {{SO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 500, 150},

    {{SO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 375, 150},
    {{SO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 125, 150},
    {{DO_3, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 500, 150},
    {{DO_3, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 500, 150},
    {{LA_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 375, 150},
    {{SO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 125, 150},
    {{FA_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
    {{LA_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
    {{SO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 1500, 150},

    {{SO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 375, 150},
    {{SO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 125, 150},
    {{LA_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 500, 150},
    {{LA_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 500, 150},
    {{RE_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 500, 150},
    {{RE_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 375, 150},
    {{RE_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 125, 150},
    {{SO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 750, 150},
    {{FA_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
    {{MI_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 500, 150},

    {{SO_1, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 375, 150},
    {{SO_1, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 125, 150},
    {{SO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 500, 150},
    {{SO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
    {{LA_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
    {{SO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
    {{FA_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
    {{MI_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
    {{RE_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
    {{DO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 1500, 150},

    {{SO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 375, 150},
    {{SO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 125, 150},
    {{DO_3, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 500, 150},
    {{DO_3, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 500, 150},
    {{LA_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 500, 150},
    {{LA_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 375, 150},
    {{SO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 125, 150},
    {{FA_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 750, 150},
    {{SO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
    {{LA_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 500, 150},

    {{RE_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 375, 150},
    {{RE_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 125, 150},
    {{SO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 500, 150},
    {{SO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
    {{LA_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
    {{SO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
    {{FA_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
    {{MI_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
    {{RE_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
    {{DO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 2000, 150},

    {{DO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 375, 150},
    {{SO_1, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 125, 150},
    {{MI_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 500, 150},
    {{MI_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 1000, 150},

    {{MI_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 375, 150},
    {{DO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 125, 150},
    {{LA_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 500, 150},
    {{LA_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 1000, 150},

    {{LA_1, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 750, 150},
    {{LA_1, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
    {{RE_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 500, 150},
    {{RE_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 375, 150},
    {{MI_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 125, 150},
    {{RE_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
    {{DO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
    {{SI_1, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
    {{LA_1, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
    {{SO_1, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 2000, 150},

    {{DO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 500, 150},
    {{SO_1, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 500, 150},
    {{LA_1, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
    {{LA_1, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 500, 150},
    {{SO_1, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
    {{DO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 500, 150},
    {{RE_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 500, 150},
    {{MI_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 1000, 150},

    {{RE_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 500, 150},
    {{LA_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
    {{LA_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
    {{SO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
    {{SO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 500, 150},
    {{MI_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
    {{RE_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 500, 150},
    {{LA_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 500, 150},
    {{SO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 500, 150},

    {{DO_3, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
    {{DO_3, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 375, 150},
    {{DO_3, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 125, 150},
    {{DO_3, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
    {{SO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
    {{LA_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 750, 150},
    {{DO_3, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
    {{LA_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 375, 150},
    {{SO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 125, 150},
    {{FA_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
    {{LA_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
    {{SO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 500, 150},

    {{DO_3, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 375, 150},
    {{DO_3, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 125, 150},
    {{DO_3, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
    {{DO_3, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
    {{SO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 500, 150},
    {{SO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
    {{LA_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
    {{SO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
    {{FA_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
    {{MI_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
    {{RE_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 250, 150},
    {{DO_2, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 1000, 150},

    {{0.0f, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 0, 150},
    {{-1.0f, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 0, 150},
};

