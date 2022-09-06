/*
 * By orbyfied (2022) 
 * License can be found at https://github.com/orbyfiedistrash/picrun
 */

#ifndef PICRUN_OPCODES_H
#define PICRUN_OPCODES_H

/*
 * The program data is stored in raw col_rgb form, using the
 * <int_col> union. This makes sure we can access the col_rgb
 * and number without any bit magic.
 */

#include "col.h"

// opcode type
typedef int_col      opu;
typedef unsigned int opc;


/*
 * Opcode definitions.
 */

constexpr opc OP_PANIC = 0x010101fe;
constexpr opc OP_EXIT  = 0x010201fe;

#endif //PICRUN_OPCODES_H
