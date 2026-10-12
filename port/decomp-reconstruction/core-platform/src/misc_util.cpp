#include "core-platform/misc_util.hpp"

#include <cstdint>
#include <cstdio>
#include <cstring>

#if defined(_WIN32)
#include <winsock2.h>
#else
#include <arpa/inet.h>
#include <netdb.h>
#endif

#include "core-platform/memory.hpp"

// 0x840000 _Z8in_cksumPti
// Internet checksum: 16-bit words in host byte order, a trailing odd byte added
// unshifted, two end-around folds, then the one's complement.
unsigned short in_cksum(const unsigned char* data, int length) {
    std::uint32_t sum = 0;
    int remaining = length;
    if (length >= 2) {
        const int words = (length - 2) / 2 + 1;
        for (int i = 0; i < words; ++i) {
            std::uint16_t w;
            std::memcpy(&w, data + 2 * i, sizeof w);
            sum += w;
        }
        remaining = (length - 2) - (words - 1) * 2;
        data += 2 * words;
    }
    if (remaining == 1) {
        sum += *data;
    }
    const std::uint32_t folded = (sum & 0xFFFFu) + (sum >> 16);
    return static_cast<unsigned short>(~(folded + (folded >> 16)) & 0xFFFFu);
}

// 0x3ef1b8 _Z12Color255To01R7Point3DIfE
// Partial: the listing divides only the first component by 255.0f and returns
// that value; the other components are not used.
float Color255To01(Point3D<float>& color) {
    return color.X / 255.0f;
}

// 0x827970 _Z9TransToIpRPc
// Partial: resolves the host name and writes its first IPv4 address as text into
// a new 128-byte buffer that replaces *host. The listing copies the address into
// a local array; the output buffer is taken to be the intended destination.
void TransToIp(char** host) {
    hostent* entry = gethostbyname(*host);
    if (entry == nullptr) {
        return;
    }
    char* buffer = static_cast<char*>(CustomAlloc(128));
    *host = buffer;
    std::memset(buffer, 0, 128);
    in_addr address;
    std::memcpy(&address, entry->h_addr_list[0], sizeof address);
    std::snprintf(buffer, 128, "%s", inet_ntoa(address));
}
