/*
 * By orbyfied (2022)
 * License can be found at https://github.com/orbyfiedistrash/picrun
 */

#include "bmp.h"

int_col* read_bmp_file(const char* file_name,
                       BITMAPFILEHEADER* out_fh,
                       BITMAPINFOHEADER* out_ih,
                       unsigned long long* out_data_len,
                       int* out_code) {

    // open file input stream
    std::ifstream file = std::ifstream();
    file.open(file_name, std::ios::in);
    if (file.is_open()) {
        // get file size
        file.seekg(0, std::ios::end);
        std::streampos length = file.tellg();
        file.seekg(0, std::ios::beg);

        printf("fspos0: %d\n", (long long)file.tellg());

        char* buf_full = new char[length];

        // read file
        file.read(buf_full, length);

        unsigned long off = 0;

        // read headers
        *out_fh = *(tagBITMAPFILEHEADER*) (buf_full + off /* TODO find better fix */ - 2);
        off += SIZE_BFH;
        *out_ih = *(tagBITMAPINFOHEADER*) (buf_full + off);
        off += SIZE_BFI;

        if (out_ih->biBitCount != 32) {
            // invalid bit width
            if (out_code) *out_code = -2;
            return (int_col*) "cannot parse file with non-32 bit pixel width---";
        }

        // read pixel data
        int_col* buf = (int_col*) buf_full + out_fh->bfOffBits;
        if (out_data_len)
            *out_data_len = (((long long) length) - off) / 4;
        printf("fsize: %d, dbufs: %d, biW: %d, biH: %d, biBitCount: %d\n",
               (long long)length, ((long long)length) - off,
               out_ih->biWidth, out_ih->biHeight, out_ih->biBitCount);

        // close stream
        file.close();

        // return successfully
        if (out_code) *out_code = 0;
        return buf;
    }

    // it didnt return successfully
    if (out_code) *out_code = -1;
    return nullptr;
}