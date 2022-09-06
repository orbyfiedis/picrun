/*
 * By orbyfied (2022) 
 * License can be found at https://github.com/orbyfiedistrash/picrun
 */

#include "vm.h"

/*
 * PicFile
 */

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

unsigned long long PicFile::get_id() const {
    return _id;
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
    // TODO: check bounds
    if (idx < 0) {
        // return relative to top
        return _data[_top - idx];
    } else {
        // return relative to bottom
        return _data[idx];
    }
}

void VmDataStack::resize(unsigned int len) {
    // TODO
}

/*
 * PicVm
 */

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

/**
 * Starts running the VM while it should be active.
 */
int PicVm::run() {
    // the return code that should be passed
    int exitCode = 0;

    // set flags
    bool shouldExit = false;
    this->active    = true;

    // utilities
    auto exitwc = [&] (int code) {
        exitCode   = code;
        shouldExit = true;
    };

    /*
     * Main Loop
     */
    while (!shouldExit && this->active) {
        // advance a position
        // and get pixel data
        advance_position();
        int_col pix = get_current_pixel();

        // match instruction
        switch (pix.i) {

            // op - panic
            case OP_PANIC:
                exitwc(VM_EXIT_PANIC);
                break;

            // op - exit
            case OP_EXIT:
                exitwc(VM_EXIT_OK);
                break;

        }
    }

    // return
    return exitCode;
}
