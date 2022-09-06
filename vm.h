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

typedef char byte;

/*
 * The program data is stored in raw color form, using the
 * <long_col> union. This makes sure we can access the color
 * and number without any bit magic.
 */

// color
struct color {
    byte r;
    byte g;
    byte b;
    byte a;
};

union long_col {
    // the color value
    color col;

    // the integer value
    unsigned long long i;
};

class PicFunction {
private:
    // the id of this function
    unsigned long long _id;

public:

};

class PicFile {
private:
    // the loaded id of this file
    unsigned long long _id;

    // the dimensions
    unsigned int _w;
    unsigned int _h;
    unsigned int _s;

    // the pixel data (program data)
    long_col* _pixels;

public:

    [[nodiscard]] inline unsigned int flatten(unsigned int x, unsigned int y) const;

    [[nodiscard]] unsigned long long get_id() const;

    [[nodiscard]] unsigned int get_width() const;
    [[nodiscard]] unsigned int get_height() const;
    [[nodiscard]] unsigned int get_size_1d() const;

    long_col* get_pixels();

    long_col get_pixel_value(unsigned int i);
    long_col get_pixel_value(unsigned int x, unsigned int y);

};

/*
 * The VM is primarily stack based for control flow,
 * arithmetic and logic, and return values/parameters,
 * using a simple byte aligned heap implementation
 * for long term variable storage.
 *
 * When a pixel is executed by the VM, that pixels color
 * is matched to an instruction, after which that instruction
 * can use the VM features to optionally read more parameters
 * and do its thing.
 */

// vm data stack
class VmDataStack {
private:
    // the data stored in the stack
    long long* _data;
    // the index of the top of the stack
    unsigned int _top;
    // the size of the currently allocated block
    unsigned int _alloc;

public:

    VmDataStack() {
        // resize to 1000 elements
        resize(1000);
    }

    long long* get_data();
    unsigned int get_top_pointer();
    unsigned int get_allocated();

    void push(long long data);
    long long pop();
    long long peek();
    // index format -> negative is from top of stack, positive is from bottom
    long long at(int idx);

    void resize(unsigned int len);

};

// vm exit codes
constexpr int VM_EXIT_OK = 0;
constexpr int VM_EXIT_PANIC = -999;
constexpr int VM_EXIT_UNKNOWN_INST = -100;

// vm class
class PicVm {
private:
    // functions
    std::unordered_map<unsigned int, PicFunction*> _functions;

    // the pointer information
    // (file, x, y, direction)
    PicFile* _file;
    unsigned int _x;
    unsigned int _y;
    unsigned int _dir;

    // the data stack
    VmDataStack* _data_stack;

public:

    // if it should be active
    std::atomic_bool active = false;

    std::unordered_map<unsigned int, PicFunction*> get_functions();
    PicFunction* get_function(unsigned int id);
    PicFunction* get_function(std::string name);

    PicFile* get_file();
    unsigned int get_direction() const;
    unsigned int get_x() const;
    unsigned int get_y() const;

    PicVm* set_file(PicFile* f);
    PicVm* set_direction(unsigned int dir);
    PicVm* set_x(unsigned int x);
    PicVm* set_y(unsigned int y);

    void advance_position();
    long_col get_current_pixel();

    int run();

};

#endif //PICRUN_VM_H
