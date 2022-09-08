/*
 * By orbyfied (2022) 
 * License can be found at https://github.com/orbyfiedistrash/picrun
 */

#include "vm.h"

#include <cmath>

#define m_min(a, b) (a < b ? a : b)
#define m_max(a, b) (a > b ? a : b)

/*
 * PicFile
 */

PicFile* load_pic_file(unsigned int w, unsigned int h, int_col* pixels) {
    return new PicFile(w, h, pixels);
}

PicFile::PicFile(unsigned int w, unsigned int h, int_col *pixels) {
    _w = w;
    _h = h;
    _s = w * h;
    _pixels = pixels;
}

inline unsigned int PicFile::flatten(unsigned int x, unsigned int y) const {
    return y * _w + x;
}

unsigned int PicFile::get_width() const {
    return _w;
}

unsigned int PicFile::get_height() const {
    return _h;
}

unsigned int PicFile::get_size_1d() const {
    return _s;
}

int_col* PicFile::get_pixels(){
    return _pixels;
}


int_col PicFile::get_pixel_value(unsigned int i) {
    return _pixels[i];
}

int_col PicFile::get_pixel_value(unsigned int x, unsigned int y){
    return get_pixel_value(flatten(x, y));
}

/*
 * VmDataStack
 */

long long* VmDataStack::get_data() {
    return _data;
}

unsigned int VmDataStack::get_top_pointer() {
    return _top;
}

unsigned int VmDataStack::get_allocated() {
    return _alloc;
}

void VmDataStack::push(long long data) {
    // grow if necessary
    if (_top += 1 >= _alloc)
        resize((int)(_alloc * 1.5));

    // set element
    _data[_top] = data;
}

inline void VmDataStack::push_ptr(void* ptr) {
    push((long long)ptr);
}

inline void VmDataStack::push_str(char* str) {
    push_ptr((void*)str);
}

inline void VmDataStack::push_long(long long l) {
    push(l);
}

inline void VmDataStack::push_int(int i) {
    push(*(long long*)&i);
}

inline void VmDataStack::push_float(float f) {
    push_int(*(int*)&f);
}

inline void VmDataStack::push_double(double d) {
    push_long(*(double*)&d);
}

long long VmDataStack::pop() {
    // get data
    long long data = _data[_top];

    // reduce top
    _top--;

    // return data
    return data;
}

long long VmDataStack::peek() {
    return _data[_top];
}

long long VmDataStack::at(int idx) {
    if (idx < 0) {
        // return relative to top
        return _data[_top - idx];
    } else {
        // return relative to bottom
        return _data[idx];
    }
}

void VmDataStack::resize(unsigned int len) {
    // allocate new data
    auto* new_data = new long long[len];

    // copy old data into new buffer
    if (_data != nullptr) {
        memcpy(_data, new_data, m_min(_top, len) * sizeof(long long));
    }

    // replace old data
    this->_alloc = len;
    _data = new_data;
}

/*
 * PicVm
 */

PicVm::PicVm() {
    // allocate and set data stack
    _data_stack = new VmDataStack();
    _data_stack->resize(1000);

    // allocate function map
    _functions = std::unordered_map<unsigned int, PicFunction*>();
}

std::unordered_map<unsigned int, PicFunction*> PicVm::get_functions() {
    return this->_functions;
}

PicFunction* PicVm::get_function(unsigned int id) {
    return this->_functions[id];
}

PicFunction* PicVm::get_function(std::string name) {
    return get_function(strhash(name));
}

PicVm* PicVm::set_file(PicFile* f) {
    _file = f;
    return this;
}

PicVm* PicVm::set_direction(unsigned int dir) {
    _dir = dir;
    return this;
}

PicVm* PicVm::set_x(unsigned int x) {
    _x = x;
    return this;
}

PicVm* PicVm::set_y(unsigned int y) {
    _y = y;
    return this;
}

PicFile* PicVm::get_file() {
    return _file;
}

unsigned int PicVm::get_direction() const {
    return _dir;
}

unsigned int PicVm::get_x() const {
    return _x;
}

unsigned int PicVm::get_y() const {
    return _y;
}


void PicVm::advance_position() {
    // calculate direction
    switch (_dir) {
        case 0: // right
            _x++;
            break;
        case 1: // down
            _y--;
            break;
        case 2: // left
            _x--;
            break;
        case 3: // up
            _y++;
            break;

    }
}

int_col PicVm::get_current_pixel() {
    return _file->get_pixel_value(_x, _y);
}

VmDataStack* PicVm::get_data_stack() {
    return _data_stack;
}

static char* O_collect_string(PicVm* vm) {
    vm->advance_position();
    // collect length
    std::vector<char>* chars = new std::vector<char>();
    int l = 0;
    while (true) {
        bool sbreak = false;

        int_col pix = vm->get_current_pixel();
        unsigned int pn = pix.i;
        for (int i = 0; i < 4; i++) {
            char c = *(char*)(&pn);
            if (c == 0) {
                sbreak = true;
                break;
            }
            chars->push_back(c);
            pn >>= 8;
        }

        if (sbreak)
            break;

        vm->advance_position();
    }

    chars->push_back('\0');

    return chars->data();
}

static void O_rpush_string(PicVm* vm) {
    vm->_data_stack->push_str(O_collect_string(vm));
}

static void O_print(PicVm* vm) {
    // pop string pointer off stack
    char* str = (char*)vm->_data_stack->pop();
    printf(str);
    printf("\n");
}

/**
 * Starts running the VM while it should be active.
 */
int PicVm::run(char** out_err = nullptr) {
    // the return code that should be passed
    int exitCode = 0;

    // utilities
    bool shouldExit = false;

    auto exitwc = [&] (int code) {
        exitCode   = code;
        shouldExit = true;
    };

    auto exitws = [&] (int code, const char* str) {
        exitCode   = code;
        shouldExit = true;
        *out_err   = *(char**)&str;
    };

    // check setup before starting
    if (_file == nullptr) {
        exitws(VM_EXIT_ERR_BOOT, "no file has been opened");
        return exitCode;
    }

    if (this->active) {
        exitws(VM_EXIT_ERR_BOOT, "the vm is already running");
        return exitCode;
    }

    // set flag
    this->active = true;

    /*
     * Main Loop
     */
    while (!shouldExit && this->active) {
        // get pixel data
        int_col pix = get_current_pixel();

        printf("current | x: %d, y: %d, d: %d\n", _x, _y, _dir);
        printf("current | l: %d, r: %d, g: %d, b: %d\n", pix.i, pix.rgb.r, pix.rgb.g, pix.rgb.b);

        // match instruction
        switch (pix.i) {

            // op - panic
            case OP_PANIC:
                exitws(VM_EXIT_PANIC, "panic");
                break;

            // op - exit
            case OP_EXIT:
                exitwc(VM_EXIT_OK);
                break;

            // op - pop string and print
            case OP_PRINT:
                O_print(this);
                break;

            // op - push string compact
            case OP_PUSH_STRING:
                O_rpush_string(this);
                break;

            // unknown op: error
            default:
                exitws(VM_EXIT_UNKNOWN_OPCODE,
                       string_format("unknown opcode {} (rgb: {},{},{})\n",
                                     pix.i, pix.rgb.r, pix.rgb.g, pix.rgb.b));

        }

        // advance position
        advance_position();
    }

    // return
    return exitCode;
}
