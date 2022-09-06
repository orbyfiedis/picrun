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

bool str_starts_with(std::string_view what, std::string_view query) {
    // get lengths
    int lq = query.length();
    int lw = what.length();

    // pre check length
    if (lw < lq)
        return false;

    // check each character
    return memcmp(what.data(), query.data(), lq) == 0;
}

bool str_ends_with(std::string_view what, std::string_view query) {
    // get lengths
    int lq = query.length();
    int lw = what.length();

    // pre check length
    if (lw < lq)
        return false;

    // check characters
    return memcmp(what.data() + (lw - lq), query.data(), lq) == 0;
}

std::string str_sub(std::string_view str, int start = 0, int end = 0) {
    int sl = str.length();
    if (end == 0)
        end = sl;
    if (end < 0)
        end = sl - end;
    if (start < 0 || start >= sl || end < 0 || end > sl)
        return "<error>";

    if (start > end) {
        int sEnd = end;
        end = start;
        start = sEnd;
    }

    unsigned long long len = end - start;

    char* target = new char[len];
    strncpy(target, str.data() + start, len);

    return { target, len };
}

constexpr int PRIME_CONST = 31;

constexpr int strhash(std::string_view str) {
    int hashCode = 0;
    for (int i = 0; i < str.length(); i++) {
        hashCode += str[i] * i * PRIME_CONST;
    }
    return hashCode;
}

std::string fix_path_slash(std::string str) {
    if (str_ends_with(str, "/"))
        return str_sub(str, 0, -1);
    return str;
}

#endif //PICRUN_UTIL_H
