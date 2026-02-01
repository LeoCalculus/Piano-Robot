#ifndef __NOTES_H
#define __NOTES_H

#include "main.h"
#include <stdint.h>

const char CMD2NOTES[24][4] = {
    "C1 ",
    "C1#",
    "D1 ",
    "D1#",
    "E1 ",
    "F1 ",
    "F1#",
    "G1 ",
    "G1#",
    "A1 ",
    "A1#",
    "B1 ",
    "C2 ",
    "C2#",
    "D2 ",
    "D2#",
    "E2 ",
    "F2 ",
    "F2#",
    "G2 ",
    "G2#",
    "A2 ",
    "A2#",
    "B2 ",
};


const char LYRICS[][17] = {
    // 1234567890123456
      "test line 1     ",
      "test line 2     ",
      "test line 3     ",
      "test line 4     ",
      "test line 5     ",
      "test line 6     ",
    // keep 3 lines of empty
      "                ",
      "                ",
      "                ",
};

#endif
