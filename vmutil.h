/*
 * By orbyfied (2022) 
 * License can be found at https://github.com/orbyfiedistrash/picrun
 */

#ifndef PICRUN_VMUTIL_H
#define PICRUN_VMUTIL_H

#include "util.h"
#include "opcodes.h"

// buffer write util:
// string
void bw_str(int_col* buf, int* ptr, const char* str);

// buffer write util:
// push string
void bw_push_str(int_col* buf, int* ptr, const char* str);

// buffer write util:
// print string
void bw_print_str(int_col* buf, int* ptr, const char* str);

// buffer write util:
// print new line
void bw_print_newln(int_col* buf, int* ptr);

#endif //PICRUN_VMUTIL_H
