// pixel_format conversion: the table-driven packed path, checked against PFDTable rows of the
// original binary (pixel_format_table.cpp). Expected values are derived from the listing:
//   converter ctor 0x5ee958 (replicate), 0x5ee790 (LUT), 0x5ee5e8 (alpha LUT),
//   per-pixel expressions in convertPackedImpl<uint,uint> 0x5eeef4 (replicate and plain terms),
//   LUT bytes at 0x8e3694 / 0x8e3698 / 0x8e36a0.
//
// Rows used (PFDTable index): 0 L8 and 2 A8 (type 0), 5 RGB565, 6 ARGB4444, 8 A1R5G5B5 (type 1),
// 12 ARGB8888 (type 2, field17 == 4), 14 RGBA8888 (type 2), 17 a compressed block row.
#include <cstdint>
#include <cstdio>

#include "glitch-video/pixel_format.hpp"
#include "glitch-video/pixel_format_convert.hpp"

using namespace glitch::video;
namespace pf = glitch::video::pixel_format;

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

static const int kL8 = 0;
static const int kRGB565 = 5;
static const int kARGB4444 = 6;
static const int kA1R5G5B5 = 8;
static const int kARGB8888 = 12;
static const int kCompressed = 17;

int main() {
    const auto& t = pf::detail::PFDTable;

    // Table facts the checks rely on (row bytes: widths +0x18..+0x1b, shifts +0x1c..+0x1f).
    CHECK(pf::getPackedType(kRGB565) == 1);
    CHECK(pf::getPackedType(kARGB4444) == 1);
    CHECK(pf::getPackedType(kA1R5G5B5) == 1);
    CHECK(pf::getPackedType(kL8) == 0);
    CHECK(pf::getPackedType(kARGB8888) == 2);  // field17 == 4
    CHECK(pf::getPackedType(kL8) == 0 && pf::getPackedType(2) == 0);
    CHECK(t[kRGB565].unknown18[4 + 0] == 11 && t[kRGB565].unknown18[4 + 1] == 5);
    CHECK(t[kARGB4444].unknown18[4 + 0] == 8 && t[kARGB4444].unknown18[4 + 3] == 12);
    CHECK(t[kA1R5G5B5].unknown18[3] == 1 && t[kA1R5G5B5].key[3] == 0x8000u);

    // Check 1: L8 -> A8 (uchar instance, key 0). Both formats have packed type 0. The destination
    // has the alpha flag and the source does not, so the fill (0xff) is the whole result.
    {
        uint8_t src[2] = {0x80, 0x12};
        uint8_t dst[2] = {0, 0};
        const int r = pf::convert(kL8, src, 2, 2, dst, 2, 2, 1, false);
        CHECK(r == 1);
        CHECK(dst[0] == 0xff);
        CHECK(dst[1] == 0xff);
    }

    // Check 2: RGB565 -> ARGB4444 (ushort instance, key 5). Reductions R 5->4, G 6->4, B 5->4,
    // and alpha filled with 0xf. Expected: top bits of each field.
    {
        uint16_t src[2] = {0xffff, 0x8000};
        uint16_t dst[2] = {0, 0};
        CHECK(pf::convert(kRGB565, src, 4, kARGB4444, dst, 4, 2, 1, false) == 1);
        CHECK(dst[0] == 0xffff);
        CHECK(dst[1] == 0xf800);
    }

    // Check 3: ARGB4444 -> RGB565 (key 5). G 4->6 and R 4->5 are upscales of up to 2x (line 681
    // path): (v << (db - sb)) | (v >> (2 sb - db)) per colour, and the alpha fill is absent.
    //   0x0f8f: R4=F, G4=8, B4=F -> R5=1F, G6=22 (0x22), B5=1F  -> 0xfc5f
    //   0x1234: R4=2, G4=3, B4=4 -> R5=04, G6=0c, B5=08         -> 0x2188
    {
        uint16_t src[2] = {0x0f8f, 0x1234};
        uint16_t dst[2] = {0, 0};
        CHECK(pf::convert(kARGB4444, src, 4, kRGB565, dst, 4, 2, 1, false) == 1);
        CHECK(dst[0] == 0xfc5f);
        CHECK(dst[1] == 0x2188);
    }

    // Check 4: A1R5G5B5 -> ARGB4444 (alpha-LUT path, SPackedConverter<0,2>): alpha 1 -> 0xf through the
    // 1-bit table (0x0000, 0xffff) shifted by 16 - 4; colours reduced by plain shifts.
    {
        uint16_t src[2] = {0xfc00, 0x7fff};
        uint16_t dst[2] = {0, 0};
        CHECK(pf::convert(kA1R5G5B5, src, 4, kARGB4444, dst, 4, 2, 1, false) == 1);
        CHECK(dst[0] == 0xff00);
        CHECK(dst[1] == 0x0fff);
    }

    // Flip: rows are written in reverse order (dst row r takes src row height - 1 - r).
    {
        uint16_t src[2] = {0xffff, 0x8000};
        uint16_t dst[2] = {0, 0};
        CHECK(pf::convert(kRGB565, src, 2, kARGB4444, dst, 2, 1, 2, true) == 1);
        CHECK(dst[0] == 0xf800);
        CHECK(dst[1] == 0xffff);
    }

    // In place with equal pitches (ARGB4444 -> RGB565 is 2 bytes per pixel on both sides).
    {
        uint16_t buf[2] = {0x0f8f, 0x1234};
        CHECK(pf::convert(kARGB4444, buf, 4, kRGB565, buf, 4, 2, 1, false) == 1);
        CHECK(buf[0] == 0xfc5f);
        CHECK(buf[1] == 0x2188);
    }

    // Coverage: branches the port does not model return 0 and leave the destination untouched.
    {
        uint32_t src[1] = {0x11223344u};
        uint32_t dst[1] = {0xdeadbeefu};
        // Same packed type, no alpha flag change: the same-type arm of 0x5f95ac (not modelled).
        CHECK(pf::convert(14, src, 4, 12, dst, 4, 1, 1, false) == 0);
        CHECK(dst[0] == 0xdeadbeefu);
        // Compressed destination: "Compressed destination format not supported".
        CHECK(pf::convert(kARGB8888, src, 4, kCompressed, dst, 4, 1, 1, false) == 0);
        CHECK(dst[0] == 0xdeadbeefu);
        // Same format is the copy() branch (not modelled).
        CHECK(pf::convert(kARGB8888, src, 4, kARGB8888, dst, 4, 1, 1, false) == 0);
    }

    if (failures == 0) {
        std::printf("pixel_format_convert_test: all checks passed\n");
        return 0;
    }
    std::printf("pixel_format_convert_test: %d failure(s)\n", failures);
    return 1;
}
