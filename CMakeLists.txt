cmake_minimum_required(VERSION 3.22)
project(picrun)

set(CMAKE_CXX_STANDARD 20)

add_executable(picrun main.cpp bmp.cpp bmp.h defines.h vm.cpp vm.h main.h util.h opcodes.h col.h util.cpp)