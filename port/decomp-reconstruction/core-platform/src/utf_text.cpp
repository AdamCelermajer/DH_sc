#include "core-platform/utf_text.hpp"

#include <cstring>
#include <new>

// 0x82a788 / 0x89c42c
// Standard UTF-8 encoding of each unit. The listing computes the same bits with
// pairs of bitwise complements; the output is the plain encoding.
int ConvertUnicodeToUTF8(char* out, const std::uint16_t* in, int count) {
    int written = 0;
    unsigned char* p = reinterpret_cast<unsigned char*>(out);
    for (int i = 0; i < count; ++i) {
        const unsigned c = in[i];
        if (c < 0x80) {
            *p++ = static_cast<unsigned char>(c);
            written += 1;
        } else if (c < 0x800) {
            *p++ = static_cast<unsigned char>(0xC0 | (c >> 6));
            *p++ = static_cast<unsigned char>(0x80 | (c & 0x3F));
            written += 2;
        } else {
            *p++ = static_cast<unsigned char>(0xE0 | (c >> 12));
            *p++ = static_cast<unsigned char>(0x80 | ((c >> 6) & 0x3F));
            *p++ = static_cast<unsigned char>(0x80 | (c & 0x3F));
            written += 3;
        }
    }
    *p = 0;
    return written;
}

// 0x82a868 / 0x89c514
// The original reads `count` input bytes (not NUL-terminated) and writes the
// terminator after the last unit. Output and input use the same index order.
int ConvertUTF8ToUnicode(std::uint16_t* out, const char* in, int count) {
    if (count < 1) {
        out[0] = 0;
        return 0;
    }
    const unsigned char* src = reinterpret_cast<const unsigned char*>(in);
    int written = 0;
    int i = 0;
    while (i < count) {
        const unsigned char b0 = src[i];
        if ((static_cast<int>(static_cast<signed char>(b0)) & 0xE0) == 0xE0) {
            // Three-byte form (also takes lead bytes 0xF0 and up, as the listing does).
            out[written++] = static_cast<std::uint16_t>((b0 << 12) | ((src[i + 1] & 0x3F) << 6) |
                                                       (src[i + 2] & 0x3F));
            i += 3;
        } else if ((b0 & 0xC0) == 0xC0) {
            out[written++] = static_cast<std::uint16_t>(((b0 & 0x1F) << 6) | (src[i + 1] & 0x3F));
            i += 2;
        } else {
            out[written++] = static_cast<std::uint16_t>(static_cast<int>(static_cast<signed char>(b0)));
            i += 1;
        }
    }
    out[written] = 0;
    return written;
}

// 0x82b6cc / 0x89d264
void TranslateUtf82Unicode(std::uint16_t** out, const char* in, int count, int allocate) {
    const std::size_t units = static_cast<std::size_t>(count) + 1;
    if (allocate != 0) {
        *out = new std::uint16_t[units];
    }
    std::memset(*out, 0, units * sizeof(std::uint16_t));
    ConvertUTF8ToUnicode(*out, in, count);
}

// 0x82b728 / 0x89d2c0
void TranslateUnicode2Utf8(char** out, const std::uint16_t* in, int count, int allocate) {
    const std::size_t bytes = static_cast<std::size_t>(count) * 3 + 1;
    if (allocate != 0) {
        *out = new char[bytes];
    }
    std::memset(*out, 0, bytes);
    ConvertUnicodeToUTF8(*out, in, count);
}
