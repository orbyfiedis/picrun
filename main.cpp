/*
 * By orbyfied (2022)
 * License can be found at https://github.com/orbyfiedistrash/picrun
 */

#include "main.h"

#include <cstdio>
#include <iostream>
#include <vector>

struct MainArgs* pr_main_args;

// main entry point
// it sucks
int main(int argc, char** argv) {
    // get binary path
    char* bin_path = argv[0];

    printf("%s\n",bin_path );

    // check arguments
    if (argc < 2) {
        pr_printheader();
        printf("Missing arguments.\n");
        printf("Usage: picrun <file> [--workdir <dir>]\n");
        printf("\n");
        return -1;
    }

    // parse args
    pr_main_args = { };

    // positioned arguments
    std::vector<std::string> positioned_args;
    // argument index
    int argi = 0;
    // next argument macro
    auto next_arg = [&] () {
        return std::string(argv[argi += 1]);
    };
    // for every argument after arg 0
    while (argi < argc) {
        // construct string
        std::string arg = next_arg();

        // check if it is a named argument start
        if (str_starts_with(arg, "--")) {
            // get name
            std::string name = str_sub(arg, 2);

            switch (strhash(name)) {
                // set working directory
                case strhash("workdir"):
                    std::string dir = next_arg();
                    dir = fix_path_slash(dir);
                    pr_main_args->work_dir = dir.c_str();
                    break;
            }
        } else {
            // append to positioned
            positioned_args.push_back(arg);
        }
    }

    // set missing directories
    pr_main_args->entry_file = positioned_args[0].c_str();
    if (pr_main_args->work_dir == nullptr)
        pr_main_args->work_dir = ".";

    printf("%s\n%s", pr_main_args->entry_file, pr_main_args->work_dir);

    // return
    return 0;
}
