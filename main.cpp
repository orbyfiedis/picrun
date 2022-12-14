/*
 * By orbyfied (2022)
 * License can be found at https://github.com/orbyfiedistrash/picrun
 */

#include "main.h"

#include "vm.h"
#include "vmutil.h"
#include "bmp.h"

#include <cstdio>
#include <vector>
#include <bitset>

void print_ma_usage() {
    pr_printheader();
    printf("Missing arguments.\n");
    printf("Usage: picrun <file> [--workdir <dir>]\n");
    printf("\n");
}

MainArgs* pr_main_args;
PicVm* pr_vm;

// main entry point
// it sucks
int main(int argc, char** argv) {
    // get binary path
    char* bin_path = argv[0];

    // check arguments
    if (argc < 2) {

        //
        // bitmap reading test
        //

        const char* fn = "../data.bmp";
        BITMAPFILEHEADER bfh;
        BITMAPINFOHEADER bih;
        int bcode;
        unsigned long long bdatlen;
        int_col* buf = read_bmp_file(fn, &bfh, &bih, &bdatlen, &bcode);
        if (bcode != 0) {
            char* err_text = (char *) buf;
            printf("error %d while reading %s: %s\n", bcode, fn, err_text);
        } else {
            printf("bfsize: %d, bftype: %d, bfpixoff: %d\n", bfh.bfSize, bfh.bfType, bfh.bfOffBits);
            printf("data | bdatlen: %d\n", bdatlen);
//            for (int i = 0; i < bdatlen; i++) {
//                int_col col = buf[i];
//                col_rgb rgb = col.rgb;
//                printf("- i: %d, rgb: %d,%d,%d,%d, intf: %d\n", i, rgb.r, rgb.g, rgb.b, rgb.a, col.i);
//            }
        }

        printf("\n\n");

        //
        // vm run test
        //

        int bp = -1;
        buf = (int_col*) calloc(100, sizeof(int_col));

//        buf_push(buf, &bp, t(OP_PUSH_INT)); // push 69696969
//        buf_push(buf, &bp, t(69696969));
//        buf_push(buf, &bp, t(OP_PUSH_INT)); // push 42020202
//        buf_push(buf, &bp, t(42020202));
//        buf_push(buf, &bp, t(OP_A_ADD)); // arithmetic add -1 & -2
//        buf_push(buf, &bp, t(OP_PTOSTR)); // primitive to string
//        buf_push(buf, &bp, t(OP_PRINT)); // print result
//        buf_push(buf, &bp, t(OP_PUSH_STRING)); // print \n
//        buf_push(buf, &bp, t_pack_chars("\n"));
//        buf_push(buf, &bp, t(OP_PRINT));

        bw_print_str(buf, &bp, "HELLO MOTHERFUCKER!");
        print_char_compound(buf, 16);

        // create file
        PicFile* file = load_pic_file(10, 10, buf);

        PicVm* vm = new PicVm();
        pr_vm = vm;
        vm->set_file(file);

        // run vm
        char* err;
        int code = vm->run(&err);

        // check result
        printf("vm exited with code %d", code);
        if (code != 0) {
            if (err != nullptr) {
                printf(": %s\n", err);
            }
        } else {
            printf("\n");
        }
        printf("vm data stack top: %d\n", vm->_data_stack->get_top_pointer());

        // print usage
        print_ma_usage();
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

    // set missing properties
    if (positioned_args.empty()) {
        print_ma_usage();
        return -1;
    }

    pr_main_args->entry_file = positioned_args[0].c_str();
    if (pr_main_args->work_dir == nullptr)
        pr_main_args->work_dir = ".";

    printf("%s\n%s", pr_main_args->entry_file, pr_main_args->work_dir);

    // return
    return 0;
}
