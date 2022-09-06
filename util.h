/*
 * By orbyfied (2022) 
 * License can be found at https://github.com/orbyfiedistrash/picrun
 */

#ifndef PICRUN_UTIL_H
#define PICRUN_UTIL_H

#include <cmath>
#include <string>
#include <string.h>
#include <cstdio>

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

#endif //PICRUN_UTIL_H
