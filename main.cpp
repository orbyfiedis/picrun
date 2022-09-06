/*
 * By orbyfied (2022)
 * License can be found at https://github.com/orbyfiedistrash/picrun
 */

#include "defines.h"

#include <stdio.h>

int main(int argv, char** argc) {
    // check arguments
    if (argv < 2) {
        pr_printheader();
        printf("Missing arguments.\n");
        printf("Usage: picrun <file>\n");
        printf("\n");
        return -1;
    }

    // return
    return 0;
}
