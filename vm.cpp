/*
 * By orbyfied (2022) 
 * License can be found at https://github.com/orbyfiedistrash/picrun
 */

#include "vm.h"

#include <cmath>
#include <iostream>

#define m_min(a, b) (a < b ? a : b)
#define m_max(a, b) (a > b ? a : b)

/*
 * VmStackVal
 */

VmStackVal pcast_val(VmStackVal val, unsigned char type) {
    // switch current type
    switch (val.type) {
        case ST_INT:
        case ST_LONG:
            switch (type) {
                case ST_INT:    return { ST_INT, tl_s((int)val.value) };
                case ST_LONG:   return { ST_LONG, (long long)val.value };
                case ST_FLOAT:  return { ST_FLOAT, tl_s((float) val.value) };
                case ST_DOUBLE: return { ST_DOUBLE, tl_s((double) val.value) };
                case ST_SHORT:  return { ST_SHORT, tl_s((short)val.value) };
                default: return NIL_VALUE;
            }

        default: return NIL_VALUE;
    }
}

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

VmStackVal* VmDataStack::get_data() {
    return _data;
}

int VmDataStack::get_top_pointer() {
    return _top;
}

unsigned int VmDataStack::get_allocated() {
    return _alloc;
}

void VmDataStack::push(VmStackVal data) {
    // grow if necessary
    _top++;
    if (_top >= _alloc)
        resize((int)(_alloc * 1.5));

    // set element
    _data[_top] = data;
}

inline void VmDataStack::push_extern_ptr(void* ptr) {
    push({ ST_EXTERN, (long long)ptr });
}

inline void VmDataStack::push_str(std::string* str) {
    push({ ST_STRING, tl_c(str) });
}

inline void VmDataStack::push_long(long long l) {
    push({ ST_LONG, l });
}

inline void VmDataStack::push_int(int i) {
    push({ ST_INT, tl_p(i) });
}

inline void VmDataStack::push_float(float f) {
    push({ ST_FLOAT, tl_p(f) });
}

inline void VmDataStack::push_double(double d) {
    push({ ST_DOUBLE, tl_p(d) });
}

VmStackVal VmDataStack::pop() {
    // check if we have data
    if (_top == -1)
        return NIL_VALUE;

    // get data
    VmStackVal data = _data[_top];

    // reduce top
    _top--;

    // return data
    return data;
}

VmStackVal VmDataStack::peek() {
    return _data[_top];
}

VmStackVal VmDataStack::at(int idx) {
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
    auto* new_data = new VmStackVal[len];

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

static std::string* O_collect_string(PicVm* vm) {
    vm->advance_position();
    // collect length
    std::vector<char>* chars = new std::vector<char>();
    int l = 0;
    while (true) {
        bool sbreak = false;

        int_col pix = vm->get_current_pixel();
        col_rgb rgb = pix.rgb;
        for (int i = 0; i < 4; i++) {
            char c = (char)(pix.i << i * 8);
            printf("i: %d, c: '%c' (%d), p.i: %d, 0x%x\n", i, c, c, pix.i, pix.i);
            if (c == 0) {
                sbreak = true;
                break;
            }
            chars->push_back(c);
        }

        if (sbreak)
            break;

        vm->advance_position();
    }

    chars->push_back('\0');

    return new std::string(chars->data());
}

static void O_rpush_string(PicVm* vm) {
    vm->_data_stack->push_str(O_collect_string(vm));
}

static void O_ptostr(PicVm* vm) {
    // pop value off stack
    VmStackVal val = vm->_data_stack->pop();
    // switch type
    std::string* string;
    switch (val.type) {
        case ST_INT:
            string = new std::string(std::to_string((int)val.value));

        case ST_LONG:
            string = new std::string(std::to_string((long long)val.value));
            break;

        default:
            string = nullptr;
    }

    // push value to stack
    vm->_data_stack->push_str(string);
}

static void O_print(PicVm* vm) {
    // pop string pointer off stack
    std::string* str = (std::string*)vm->_data_stack->pop().value;
    if (str == nullptr)
        std::cout << "null";
    std::cout << *str;
}

static void O_inln(PicVm* vm) {

}

static void O_a_add(PicVm* vm) {
    VmDataStack* stack = vm->_data_stack;

    // pop two values
    VmStackVal a = stack->pop();
    VmStackVal b = stack->pop();

    if (is_nil(a) || is_nil(b))
        return;

    // switch type operation
    switch (a.type) {
        case ST_INT:
            stack->push({ ST_INT, (long long) ((int)a.value + pcast_val(b, a.type).value) });
            break;
        case ST_LONG:
            stack->push({ ST_LONG, a.value + pcast_val(b, a.type).value });
            break;
    }
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

#ifdef _VMDEBUG
        printf("current | x: %d, y: %d, d: %d\n", _x, _y, _dir);
        printf("current | l: %d, r: %d, g: %d, b: %d\n", pix.i, pix.rgb.r, pix.rgb.g, pix.rgb.b);
#endif

//        printf("opcode: %d\n", pix.i);

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

            // op - push int
            case OP_PUSH_INT:
                advance_position();
                _data_stack->push({ ST_INT, get_current_pixel().i });
                break;

            // op - to string primitive]
            case OP_PTOSTR:
                O_ptostr(this);
                break;

            // op - arithmetic any-type add
            case OP_A_ADD:
                O_a_add(this);
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
