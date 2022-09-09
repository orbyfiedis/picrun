/*
 * By orbyfied (2022) 
 * License can be found at https://github.com/orbyfiedistrash/picrun
 */

#include "vmutil.h"

void bw_str(int_col* buf, int* ptr, const char* str) {
    char  c = -1;
    int   i = 0;
    char* b = (char*) calloc(4, sizeof(char));
    while (c != 0) {
        c = str[i];
        int m = i % 4;
        b[m] = c;
        if (m == 3 || c == 0) {
            // write packed chars
            buf_push(buf, ptr, t_pack_chars(b));
            // reset char buffer
            b = (char*) calloc(4, sizeof(char));
        }
        i++;
    }
    if (i % 4 != 0)
        buf_push(buf, ptr, t(0));
}

void bw_push_str(int_col* buf, int* ptr, const char* str) {
    buf_push(buf, ptr, t(OP_PUSH_STRING));
    bw_str(buf, ptr, str);
}

void bw_print_str(int_col* buf, int* ptr, const char* str) {
    bw_push_str(buf, ptr, str);
    buf_push(buf, ptr, t(OP_PRINT));
}

void bw_print_newln(int_col* buf, int* ptr) {
    buf_push(buf, ptr, t(OP_PUSH_STRING));
    buf_push(buf, ptr, t_pack_chars("\n"));
    buf_push(buf, ptr, t(OP_PRINT));
}