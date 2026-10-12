// slim UTF-8 helpers and the XmlDocument stubs. Code points are held in uint32_t
// because the original wchar_t is 32 bits (Android); MinGW wchar_t is 16 bits.
#pragma once

#include <cstdint>

namespace slim {
// 0x844190 slim::utf8toutf16(char const*, unsigned, wchar_t*, unsigned)
int utf8toutf16(const char* src, unsigned srcLen, std::uint32_t* dst, unsigned dstMax);
// 0x844270 slim::utf16toutf8(wchar_t const*, unsigned, char*, unsigned)
void utf16toutf8(const std::uint32_t* src, unsigned count, char* dst, unsigned dstMax);
// 0x844368 slim::detectUtf8(char const*, unsigned)
bool detectUtf8(const char* src, unsigned len);

class XmlDocument {
public:
    bool loadFromStream();        // 0x844180: unsupported in the original, returns false
    bool save(const char* path) const;  // 0x844188: unsupported in the original, returns true
};
}  // namespace slim
