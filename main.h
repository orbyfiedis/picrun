/*
 * By orbyfied (2022) 
 * License can be found at https://github.com/orbyfiedistrash/picrun
 */

#ifndef PICRUN_MAIN_H
#define PICRUN_MAIN_H

#include "defines.h"

// the main arg struct
struct MainArgs {

    // file to run
    const char* entry_file = nullptr;

    // work directory
    const char* work_dir = nullptr;

};

// main args container
extern MainArgs* pr_main_args;

#endif //PICRUN_MAIN_H
