/*
 * By orbyfied (2022) 
 * License can be found at https://github.com/orbyfiedistrash/picrun
 */

#ifndef PICRUN_COL_H
#define PICRUN_COL_H

#include <cstdint>

typedef uint8_t byte;

// col_rgb
struct col_rgb {
    byte r = 0;
    byte g = 0;
    byte b = 0;
    byte a = 255;
};

union int_col {
    // the col_rgb value
    col_rgb rgb;

    // the integer value
    unsigned int i { };
};

#endif //PICRUN_COL_H
