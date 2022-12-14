/*
 * By orbyfied (2022) 
 * License can be found at https://github.com/orbyfiedistrash/picrun
 */

#ifndef PICRUN_OPCODES_H
#define PICRUN_OPCODES_H

// utilities
#define print_char_compound(c, l) for (int i = 0; i < l; i++) { char* ptr = (char*)(c + i); printf("comp(%d): %d ['%c' i: %d]\n", l, i, *ptr, *ptr); }
/*
 * The program data is stored in raw col_rgb form, using the
 * <int_col> union. This makes sure we can access the col_rgb
 * and number without any bit magic.
 */

#include "col.h"

// opcode type
typedef int_col      opu;
typedef unsigned int opc;

inline opu t(opc opc) {
    return opu(opc);
}

inline opu t_pack_chars(const char* str) {
    opu* o = new opu(0);
    for (int i = 0; i < 4; i++) {
        char c = str[i];
        *((char*)(o + i)) = c;
        if (c == 0)
            break;
    }

    print_char_compound(o, 4);
    printf("p.i: %d, 0x%x\n", o->i, o->i);

    return *o;
}

/*
 * Opcode definitions.
 */

constexpr opc OP_EXIT  = 0x00000000;
constexpr opc OP_PANIC = 0x00010101;

constexpr opc OP_PTOSTR = 0x01010101;
constexpr opc OP_PRINT  = 0x01020000;
constexpr opc OP_INLN   = 0x01030100;

constexpr opc OP_PUSH_STRING = 0x01030100;
constexpr opc OP_PUSH_INT    = 0x01030200;
constexpr opc OP_PUSH_LONG   = 0x01030300;

constexpr opc OP_A_ADD = 0x02010101;
constexpr opc OP_A_SUB = 0x02010201;
constexpr opc OP_A_MUL = 0x02010301;
constexpr opc OP_A_DIV = 0x02010401;

#endif //PICRUN_OPCODES_H
