/*
 * By orbyfied (2022) 
 * License can be found at https://github.com/orbyfiedistrash/picrun
 */

#ifndef PICRUN_UTIL_H
#define PICRUN_UTIL_H

#include <cmath>
#include <string>
#include <cstring>
#include <cstdio>
#include <memory>
#include <stdexcept>

bool str_starts_with(std::string_view what, std::string_view query);
bool str_ends_with(std::string_view what, std::string_view query);
std::string str_sub(std::string_view str, int start = 0, int end = 0);

constexpr int PRIME_CONST = 31;

constexpr int strhash(std::string_view str) {
    int hashCode = 0;
    for (int i = 0; i < str.length(); i++) {
        hashCode += str[i] * i * PRIME_CONST;
    }

    return hashCode;
}

std::string fix_path_slash(std::string str);

// from: https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
template<typename ... Args>
char* string_format( const std::string& format, Args ... args ) {
    int size_s = std::snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    if( size_s <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
    auto size = static_cast<size_t>( size_s );
    std::unique_ptr<char[]> buf( new char[ size ] );
    std::snprintf( buf.get(), size, format.c_str(), args ... );
    return buf.get();
}

template <typename T>
static void buf_push(T* buf, int* ptr, T elem) {
    *ptr = *ptr + 1;
    buf[*ptr] = elem;
}

#endif //PICRUN_UTIL_H
