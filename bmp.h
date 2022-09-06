/*
 * By orbyfied (2022)
 * License can be found at https://github.com/orbyfiedistrash/picrun
 */

#ifndef PICRUN_BMP_H
#define PICRUN_BMP_H

#include <vector>
#include <fstream>

#include <cstdint>

// col_rgb format
#include "col.h"

// typedefs for compatibility
typedef int      LONG;
typedef uint16_t WORD;
typedef uint32_t DWORD;

// from:
// https://docs.microsoft.com/en-gb/windows/win32/api/wingdi/ns-wingdi-bitmapfileheader
// https://docs.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapinfoheader

typedef struct tagBITMAPFILEHEADER {

    WORD  bfType;
    DWORD bfSize;
    WORD  bfReserved1;
    WORD  bfReserved2;
    DWORD bfOffBits;

} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {

    DWORD biHeaderSize;
    LONG  biWidth;
    LONG  biHeight;
    WORD  biPlanes;
    WORD  biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG  biXPixPerMeter;
    LONG  biYPixPerMeter;
    DWORD biClrPalUsed;
    DWORD biClrPalImportant;

} BITMAPINFOHEADER;

#define SIZE_BFH 14
#define SIZE_BFI 40

/**
 * Reads a BMP file into its headers
 * and a vector of pixel data.
 */
int_col* read_bmp_file(const char* file_name,
        BITMAPFILEHEADER* out_fh,
        BITMAPINFOHEADER* out_ih,
        unsigned long long* out_data_len = nullptr,
        int* out_code = nullptr);

#endif //PICRUN_BMP_H
