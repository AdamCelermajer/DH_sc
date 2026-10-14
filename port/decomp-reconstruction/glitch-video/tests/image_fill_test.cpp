// Checks for glitch::video::CImage::fill (0x5ff9b4): the per-format colour packing and the word loop that
// writes the data block. The packing is the inverse of getPixel (0x5ff808, same file) and is named by the
// PFDTable rows: 5 is R5G6B5, 8 is A1R5G5B5, and 12/13/14 are the 32-bit rows whose masks give the byte order.
//
// NOT REGISTERED with add_test, and deliberately so. CImage::fill and initData are reconstructed, and the
// packing and the loop bounds were confirmed against the binary (the loop is `m_dataSize >> 5` passes of eight
// words plus `(m_dataSize >> 2) & 7` words - see the STR R2,[R3,#0x1c] block at 0x5ffa38 and the UBFX at
// 0x5ffa68). What is NOT settled is the rest of CImage's construction path: the 5- and 6-argument
// constructors (0x6021f8, 0x602670, 0x6027e8) are still `partial`/`skipped`, and the port's 5-argument
// constructor carries a deliberate host deviation in the own-clear case (see image.cpp). Until that is settled
// this file must not report a pass: a green check built on an unsettled constructor would be a false green.
//
// The facts already established, to be turned into checks once the constructor is settled:
//   - format 12 writes byte order  a, r, g, b   (observed)
//   - format 13 writes byte order  b, a, r, g   (observed)
//   - format 14 writes byte order  c, b, a, r   (observed)
//   - formats 5 and 8 pack one halfword and replicate it into both halves of the word
//   - a format outside those five writes nothing
// Where a, b, c are components 0, 1, 2 of SColor and r is component 3.
#include "glitch-video/image.hpp"

#include <cstdio>

int main() {
    std::printf("image_fill: not run (CImage's construction path is not settled; see the file comment)\n");
    return 0;
}
