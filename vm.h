/*
 * By orbyfied (2022) 
 * License can be found at https://github.com/orbyfiedistrash/picrun
 */

#ifndef PICRUN_VM_H
#define PICRUN_VM_H

#include "util.h"
#include <vector>
#include <unordered_map>
#include <atomic>

// include opcodes from file
#include "opcodes.h"

class PicFunction {
private:
    // the id of this function
    unsigned long long _id;

public:

};

class PicFile {
private:
    // the dimensions
    unsigned int _w;
    unsigned int _h;
    unsigned int _s;

    // the pixel data (program data)
    int_col* _pixels;

public:

    PicFile(unsigned int w, unsigned int h, int_col* pixels);

    [[nodiscard]] inline unsigned int flatten(unsigned int x, unsigned int y) const;

    [[nodiscard]] unsigned int get_width() const;
    [[nodiscard]] unsigned int get_height() const;
    [[nodiscard]] unsigned int get_size_1d() const;

    int_col* get_pixels();

    int_col get_pixel_value(unsigned int i);
    int_col get_pixel_value(unsigned int x, unsigned int y);

};

PicFile* load_pic_file(unsigned int w, unsigned int h, int_col* pixels);

/*
 * The VM is primarily stack based for control flow,
 * arithmetic and logic, and return values/parameters,
 * using a simple byte aligned heap implementation
 * for long term variable storage.
 *
 * When a pixel is executed by the VM, that pixels col_rgb
 * is matched to an instruction, after which that instruction
 * can use the VM features to optionally read more parameters
 * and do its thing.
 */

// vm data stack
class VmDataStack {
private:
    // the data stored in the stack
    long long* _data = nullptr;
    // the index of the top of the stack
    unsigned int _top = 0;
    // the size of the currently allocated block
    unsigned int _alloc = 0;

public:

    long long* get_data();
    unsigned int get_top_pointer();
    unsigned int get_allocated();

    void push(long long data);
    inline void push_ptr(void* ptr);
    inline void push_str(char* str);
    inline void push_long(long long l);
    inline void push_int(int i);
    inline void push_float(float f);
    inline void push_double(double d);

    long long pop();
    long long peek();
    // index format -> negative is from top of stack, positive is from bottom
    long long at(int idx);

    void resize(unsigned int len);

};

// vm exit codes
constexpr int VM_EXIT_OK             = 0;
constexpr int VM_EXIT_ERR_BOOT       = -1;
constexpr int VM_EXIT_PANIC          = -999;
constexpr int VM_EXIT_UNKNOWN_OPCODE = -100;

// vm class
class PicVm {
private:
    // functions
    std::unordered_map<unsigned int, PicFunction*> _functions;

    // the pointer information
    // (file, x, y, direction)
    PicFile* _file;
    unsigned int _x   = 0;
    unsigned int _y   = 0;
    unsigned int _dir = 0;

public:

    PicVm();

    // the data stack
    VmDataStack* _data_stack;

    // if it should be active
    std::atomic_bool active = false;

    std::unordered_map<unsigned int, PicFunction*> get_functions();
    PicFunction* get_function(unsigned int id);
    PicFunction* get_function(std::string name);

    VmDataStack* get_data_stack();

    PicFile* get_file();
    unsigned int get_direction() const;
    unsigned int get_x() const;
    unsigned int get_y() const;

    PicVm* set_file(PicFile* f);
    PicVm* set_direction(unsigned int dir);
    PicVm* set_x(unsigned int x);
    PicVm* set_y(unsigned int y);

    void advance_position();
    int_col get_current_pixel();

    int run(char** out_err);

};

#endif //PICRUN_VM_H
