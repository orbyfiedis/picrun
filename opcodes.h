/*
 * By orbyfied (2022) 
 * License can be found at https://github.com/orbyfiedistrash/picrun
 */

#ifndef PICRUN_OPCODES_H
#define PICRUN_OPCODES_H

// opcode type
typedef unsigned long long opc;

/*
 * Opcode definitions.
 */

constexpr opc OP_PANIC = 0;
constexpr opc OP_EXIT  = 1;

#endif //PICRUN_OPCODES_H
