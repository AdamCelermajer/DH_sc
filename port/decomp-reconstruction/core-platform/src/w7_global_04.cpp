#include "core-platform/w7_global_04.hpp"

#include <algorithm>
#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstring>

#if defined(_WIN32)
#include <winsock2.h>
#else
#include <arpa/inet.h>
#include <netdb.h>
#endif

#include "core-platform/debug_out.hpp"
#include "core-platform/lc_api.hpp"
#include "core-platform/misc_util.hpp"

namespace {

// Copies a C string onto the heap with operator new[]: strlen + 1 bytes, zero-filled, then the
// text. This is the body of LC_API_STRNEW (0x89d31c) and XP_API_STRNEW (0x82b9a0), so the
// matching cleanup can use operator delete[].
char* DupCString(const char* text) {
    const std::size_t length = std::strlen(text);
    char* copy = new char[length + 1];
    std::memcpy(copy, text, length + 1);
    return copy;
}

// Calls slot `byte_offset / 4` of the object's vtable. The offsets are the
// original 32-bit ARM byte offsets, so the slot index is the offset divided by four.
template <typename Fn>
Fn VirtualSlot(const void* object, std::size_t byte_offset) {
    void* const* vtable = *static_cast<void* const* const*>(object);
    return reinterpret_cast<Fn>(vtable[byte_offset / 4]);
}

// Maps a 16-bit value to a PCM sample: scale by 32768 and clamp to the short range.
std::int16_t ToPcm16(float sample) {
    const int value = static_cast<int>(sample * 32768.0f);
    if (value < -32768) {
        return -32768;
    }
    if (value > 32767) {
        return 32767;
    }
    return static_cast<std::int16_t>(value);
}

// Reads one UTF-16 unit as two bytes, low byte first. The original reads whole
// units when the source is aligned and byte pairs when it is not; both give the
// same unit on a little-endian target.
std::uint16_t LoadUnit(const unsigned char* at) {
    return static_cast<std::uint16_t>(at[0] | (at[1] << 8));
}

int Utf8Length(std::uint16_t unit) {
    if (unit <= 0x7F) {
        return 1;
    }
    if (unit <= 0x7FF) {
        return 2;
    }
    return 3;
}

// Shared body of ConvertUnicodeToUTF8_Old and ConvertUnicodeToUTF8_OldLC. The
// two originals differ only in how the masks are spelled, not in the result.
int ConvertUnicodeToUTF8Impl(char* dst, const unsigned short* src) {
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(src);
    if (dst == nullptr) {
        int needed = 1;  // terminating NUL
        for (std::size_t i = 0;; i += 2) {
            const std::uint16_t unit = LoadUnit(bytes + i);
            if (unit == 0) {
                break;
            }
            needed += Utf8Length(unit);
        }
        return needed;
    }

    char* out = dst;
    for (std::size_t i = 0;; i += 2) {
        const std::uint16_t unit = LoadUnit(bytes + i);
        if (unit == 0) {
            break;
        }
        if (unit <= 0x7F) {
            *out++ = static_cast<char>(unit);
        } else if (unit <= 0x7FF) {
            *out++ = static_cast<char>(0xC0 | ((unit >> 6) & 0x1F));
            *out++ = static_cast<char>(0x80 | (unit & 0x3F));
        } else {
            *out++ = static_cast<char>(0xE0 | (unit >> 12));
            *out++ = static_cast<char>(0x80 | ((unit >> 6) & 0x3F));
            *out++ = static_cast<char>(0x80 | (unit & 0x3F));
        }
    }
    *out = '\0';
    return static_cast<int>(out - dst);
}

// The .rodata table channel_position (IDA listing, 48 bytes). Six masks per
// source channel count n, at index 6 * n + channel.
constexpr std::uint8_t kChannelPosition[48] = {
    0, 0, 0, 0, 0, 0,   // n = 0
    7, 0, 0, 0, 0, 0,   // n = 1
    3, 5, 0, 0, 0, 0,   // n = 2
    3, 7, 5, 0, 0, 0,   // n = 3
    3, 5, 3, 5, 0, 0,   // n = 4
    3, 7, 5, 3, 5, 0,   // n = 5
    3, 7, 5, 3, 5, 7,   // n = 6
    0, 0, 0, 0, 0, 0,
};

// The function-local table convert_samples_short::channel_selector, read as
// little-endian 32-bit ints: row = output channel count, column = output channel.
constexpr std::uint32_t kChannelSelector[3][2] = {
    {0, 0},
    {1, 0},
    {2, 4},
};

}  // namespace

// 0x8279d8 _Z24ParseDedicatedServerInfoR21_tDedicatedServerInfoP10DataPacket
// Partial: the vtable slots (byte offsets 64, 36, 20) and the field offsets (24, 28, 30, 44)
// are the original's, read by offset. _tDedicatedServerInfo is not declared in the tree (only a
// forward declaration in game/include/dh2/global/net_singletons.hpp), so its layout is unknown.
// DataPacket is declared in net-transport with addBlob and getBlob (+0x4c, +0x50), not the slots
// used here, so those calls go through the vtable.
bool ParseDedicatedServerInfo(_tDedicatedServerInfo& info, DataPacket* packet) {
    unsigned char* raw = reinterpret_cast<unsigned char*>(&info);

    char* host = nullptr;
    std::uint16_t first_port = 0;
    VirtualSlot<void (*)(DataPacket*, char**, std::uint16_t*)>(packet, 64)(packet, &host, &first_port);
    TransToIp(&host);
    if (host == nullptr) {
        return false;
    }

    const std::uint32_t address = inet_addr(host);
    std::memcpy(raw + 24, &address, sizeof address);

    std::uint16_t port_a = 0;
    std::uint16_t port_b = 0;
    VirtualSlot<void (*)(DataPacket*, std::uint16_t*)>(packet, 36)(packet, &port_a);
    VirtualSlot<void (*)(DataPacket*, std::uint16_t*)>(packet, 36)(packet, &port_b);
    std::memcpy(raw + 28, &port_a, sizeof port_a);
    std::memcpy(raw + 30, &port_b, sizeof port_b);

    VirtualSlot<void (*)(DataPacket*, void*)>(packet, 20)(packet, raw + 44);

    in_addr stored{};
    std::memcpy(&stored, raw + 24, sizeof stored);
    (void)inet_ntoa(stored);
    (void)inet_ntoa(stored);
    return true;
}

// 0x828b64 _Z8_DBG_OUTPKcz
// Partial: the original formats with vsprintf into a stack buffer of about 1 KB, prints the
// text, and returns the result of __android_log_print(4, "WIFI", "%s", text). Here the
// text is formatted with a 1024-byte bound, printed, and sent to appDebugLog with the same
// tag. The return value is the text length, not the log-call result.
int _DBG_OUT(const char* format, ...) {
    char buffer[1024];
    va_list args;
    va_start(args, format);
    std::vsnprintf(buffer, sizeof buffer, format, args);
    va_end(args);
    std::printf("%s", buffer);
    appDebugLog("WIFI", buffer);
    return static_cast<int>(std::strlen(buffer));
}

// 0x82aa10 _Z24ConvertUnicodeToUTF8_OldPcPKt
int ConvertUnicodeToUTF8_Old(char* dst, const unsigned short* src) {
    return ConvertUnicodeToUTF8Impl(dst, src);
}

// 0x89c6ac _Z26ConvertUnicodeToUTF8_OldLCPcPKt
int ConvertUnicodeToUTF8_OldLC(char* dst, const unsigned short* src) {
    return ConvertUnicodeToUTF8Impl(dst, src);
}

// The process-wide host caches. The binary has one l_host / l_count pair, reached through the GOT
// pointers l_host_ptr and l_count_ptr, and one l_hostLC / l_countLC pair, reached through
// l_hostLC_ptr and l_countLC_ptr. Each is defined once here with the host hostent type; an entry
// is the hostent the thread functions below fill in. Declared in w7_global_04.hpp.
hostent* l_host[4] = {};
int l_count = 0;
hostent* l_hostLC[4] = {};
int l_countLC = 0;

// 0x841ba8 _Z26threadGetHostByNameProcessPv
// Partial: the request is read at the original 32-bit layout (busy-flag pointer at byte offset 0,
// host name at byte offset 4). No request struct is declared in the tree, so this raw-offset read
// is not host-correct yet. An added entry goes into the shared l_host / l_count cache as a host
// hostent. When an entry is added, the listing returns the XP_DEBUG_OUT result instead of the
// gethostbyname result. XP_DEBUG_OUT (0x82b784) is a 1991-byte formatter that is not
// reconstructed, so this returns the gethostbyname result on every path. The trace goes to
// _DBG_OUT.
hostent* threadGetHostByNameProcess(void* request) {
    unsigned char* raw = static_cast<unsigned char*>(request);
    int** busy = reinterpret_cast<int**>(raw);
    const char* host = reinterpret_cast<const char*>(raw + 4);

    **busy = 1;
    hostent* found = gethostbyname(host);
    if (found != nullptr) {
        if (l_count <= 3) {
            hostent* entry = new hostent{};
            l_host[l_count] = entry;
            entry->h_length = found->h_length;
            entry->h_name = DupCString(host);
            entry->h_addr_list = new char*[1]{};
            // As in the original, the first address pointer is copied as a C string.
            entry->h_addr_list[0] = DupCString(found->h_addr_list[0]);
            ++l_count;
            _DBG_OUT("GetHostByName() - Add %s \n", host);
        }
        **busy = 0;
    }
    return found;
}

// 0x89f8a0 _Z28threadGetHostByNameProcessLCPv
// Partial: the request is read at the 32-bit layout as in threadGetHostByNameProcess (busy-flag
// pointer at byte offset 0, host name at byte offset 4), which is not host-correct until a request
// struct is declared. An added entry goes into the shared l_hostLC / l_countLC cache that the
// binary's GetHostIP, GetHostByName and Startup read. LC_DEBUG_OUT (0x89c304) is a bare-return
// stub in the binary, so its call is kept with no arguments; the stub ignores them. LC_API_STRNEW
// (0x89d31c) is DupCString, as above. net-transport/src/lc_android_socket.cpp keeps its own
// copy of this cache and of this function, so its readers do not see these entries yet.
void threadGetHostByNameProcessLC(void* request) {
    unsigned char* raw = static_cast<unsigned char*>(request);
    int** busy = reinterpret_cast<int**>(raw);
    const char* host = reinterpret_cast<const char*>(raw + 4);

    **busy = 1;
    hostent* found = gethostbyname(host);
    if (found != nullptr) {
        if (l_countLC <= 3) {
            hostent* entry = new hostent{};
            l_hostLC[l_countLC] = entry;
            entry->h_length = found->h_length;
            entry->h_name = DupCString(host);
            entry->h_addr_list = new char*[1]{};
            entry->h_addr_list[0] = DupCString(found->h_addr_list[0]);
            ++l_countLC;
            LC_DEBUG_OUT();
        }
        **busy = 0;
    }
}

// 0x843b2c _Z11trimHTMLstrSs
// Note: the original erases (close + 1) characters starting at the '<' rather
// than the length of the tag. That is reproduced as found.
std::string trimHTMLstr(std::string text) {
    static const char* const kEntities[5][2] = {
        {"&lt;", "<"},
        {"&gt;", ">"},
        {"&amp;", "&"},
        {"&quot;", "\""},
        {"&apos;", "'"},
    };
    for (const auto& pair : kEntities) {
        const std::size_t length = std::strlen(pair[0]);
        std::size_t at = 0;
        while ((at = text.find(pair[0], 0, length)) != std::string::npos) {
            text.replace(at, length, pair[1]);
        }
    }

    std::size_t open = 0;
    while ((open = text.find('<', open)) != std::string::npos) {
        std::size_t close = text.find('>', open);
        if (close == std::string::npos) {
            close = text.size();
        }
        text.erase(open, close + 1);
    }
    return text;
}

// 0x876e8c _ZL21convert_samples_shortiPPsiiPPfii
// Converts float planes to 16-bit PCM. A mapping path downmixes to mono or
// stereo with channel_position; otherwise the common channels are copied, the
// rest are silenced, and the copy ignores in_offset (as in the original).
namespace {
[[maybe_unused]] void convert_samples_short(int out_channels, std::int16_t** out, int out_offset, int in_channels,
                           float** in, int in_offset, int sample_count) {
    const bool mapping = out_channels <= 2 && out_channels != in_channels && in_channels <= 6;
    if (mapping) {
        if (out_channels <= 0) {
            return;
        }
        for (int out_ch = 0; out_ch < out_channels; ++out_ch) {
            const std::uint32_t selector = kChannelSelector[out_channels][out_ch];
            std::int16_t* dst = out[out_ch] + out_offset;
            if (sample_count <= 0) {
                continue;
            }
            int block = 32;
            int done = 0;
            int src = in_offset;
            do {
                float acc[32] = {};
                if (sample_count < block + done) {
                    block = sample_count - done;
                }
                if (in_channels > 0) {
                    for (int ch = 0; ch < in_channels; ++ch) {
                        if ((kChannelPosition[6 * in_channels + ch] & selector) != 0 && block > 0) {
                            for (int i = 0; i != block; ++i) {
                                acc[i] += in[ch][src + i];
                            }
                        }
                    }
                }
                if (block > 0) {
                    for (int j = 0; j != block; ++j) {
                        dst[j] = ToPcm16(acc[j]);
                    }
                }
                src += 32;
                done += 32;
                dst += 32;
            } while (sample_count > done);
        }
        return;
    }

    const int copied = std::min(in_channels, out_channels);
    for (int ch = 0; ch < copied; ++ch) {
        std::int16_t* dst = out[ch] + out_offset;
        for (int k = 0; k < sample_count; ++k) {
            dst[k] = ToPcm16(in[ch][k]);
        }
    }
    for (int ch = std::max(copied, 0); ch < out_channels; ++ch) {
        std::memset(out[ch] + out_offset, 0, static_cast<std::size_t>(sample_count) * sizeof(std::int16_t));
    }
}

// 0x8771bc _ZL34convert_channels_short_interleavediPsiPPfii
// Converts float planes to interleaved 16-bit PCM. Stereo outputs from six or
// fewer channels use channel_position: a bit 2 channel feeds left, bit 4 feeds
// right, and mask 6 feeds both. Otherwise the common channels are copied and the
// rest are silenced.
[[maybe_unused]] void convert_channels_short_interleaved(int out_channels, std::int16_t* out, int in_channels,
                                        float** in, int in_offset, int sample_count) {
    const bool mapping = out_channels <= 2 && out_channels != in_channels && in_channels <= 6;
    if (mapping) {
        if (out_channels <= 0 || sample_count <= 0) {
            return;
        }
        for (int done = 0; done < sample_count; done += 16) {
            int block = 16;
            if (sample_count < done + block) {
                block = sample_count - done;
            }
            float acc[32] = {};
            const int src = in_offset + done;
            if (in_channels > 0 && block > 0) {
                for (int ch = 0; ch < in_channels; ++ch) {
                    const int mask = kChannelPosition[6 * in_channels + ch] & 6;
                    if (mask == 6) {
                        for (int k = 0; k < block; ++k) {
                            acc[2 * k] += in[ch][src + k];
                            acc[2 * k + 1] += in[ch][src + k];
                        }
                    } else if (mask == 2) {
                        for (int k = 0; k < block; ++k) {
                            acc[2 * k] += in[ch][src + k];
                        }
                    } else if (mask == 4) {
                        for (int k = 0; k < block; ++k) {
                            acc[2 * k + 1] += in[ch][src + k];
                        }
                    }
                }
            }
            if (block > 0) {
                std::int16_t* dst = out + 2 * done;
                for (int k = 0; k < 2 * block; ++k) {
                    dst[k] = ToPcm16(acc[k]);
                }
            }
        }
        return;
    }

    const int copied = std::min(in_channels, out_channels);
    for (int frame = 0; frame < sample_count; ++frame) {
        std::int16_t* row = out + frame * out_channels;
        const int src = in_offset + frame;
        for (int ch = 0; ch < copied; ++ch) {
            row[ch] = ToPcm16(in[ch][src]);
        }
        for (int ch = std::max(copied, 0); ch < out_channels; ++ch) {
            row[ch] = 0;
        }
    }
}
}  // namespace
