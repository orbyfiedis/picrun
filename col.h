/*
 * By orbyfied (2022) 
 * License can be found at https://github.com/orbyfiedistrash/picrun
 */

#ifndef PICRUN_COL_H
#define PICRUN_COL_H

#include <cstdint>

typedef char byte;

// col_rgb
struct col_rgb {
    byte r = 0;
    byte g = 0;
    byte b = 0;
    byte a = 255;
};

union int_col {
    explicit int_col(unsigned int i1) {
        i = i1;
    }

    explicit int_col(byte r1, byte g1, byte b1, byte a1) {
        rgb.r = r1;
        rgb.g = g1;
        rgb.b = b1;
        rgb.a = a1;
    }

    // the col_rgb value
    col_rgb rgb;

    // the integer value
    unsigned int i { };
};

#endif //PICRUN_COL_H
