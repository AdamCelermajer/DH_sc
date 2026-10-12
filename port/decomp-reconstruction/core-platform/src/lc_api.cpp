#include "core-platform/lc_api.hpp"

#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

#include "core-platform/time_util.hpp"

namespace {
// Eight file slots and the two debug logs, as in the original's static data.
constexpr int kFileSlots = 8;
std::FILE* l_file_listLC[kFileSlots] = {};
std::FILE* fp_recvLC = nullptr;
std::FILE* fp_sendLC = nullptr;

// Shared by LC_API_STRCHRFIND and FUN_008ac2cc. Scans from the start of s and
// counts occurrences of c until the count reaches n or the string ends. Returns
// the number of characters consumed. The loop examines s[0] first, as the listing does.
std::size_t ScanForNth(const char* s, int c, int n, int& count) {
    std::size_t index = 0;
    count = 0;
    if (s[0] == '\0' || n == 0 || n < 0) {
        return 0;
    }
    char prev = '\0';
    char next = s[0];
    do {
        prev = next;
        ++index;
        next = s[index];
        if (c == static_cast<int>(prev)) {
            ++count;
        }
    } while (next != '\0' && count < n);
    return index;
}
}  // namespace

// 0x89c2f4 FUN_008ac2f4 (bare return in the binary)
void FUN_008ac2f4() {}

// 0x89c2f8 LC_API_STRCHR (bare return in the binary)
void LC_API_STRCHR() {}

// 0x89c2fc LC_DEBUG_INIT (bare return in the binary)
void LC_DEBUG_INIT() {}

// 0x89c300 LC_DEBUG_FINALIZE (bare return in the binary)
void LC_DEBUG_FINALIZE() {}

// 0x89c304 LC_DEBUG_OUT (bare return in the binary)
void LC_DEBUG_OUT() {}

// 0x89c310 LC_API_FTOA (bare return in the binary)
void LC_API_FTOA() {}

// 0x89c99c LC_API_GetNationality (bare return in the binary)
void LC_API_GetNationality(char*) {}

// 0x89c29c LC_API_STRCHRFIND(char*, int, int)
char* LC_API_STRCHRFIND(char* s, int c, int n) {
    int count = 0;
    const std::size_t consumed = ScanForNth(s, c, n, count);
    char* result = s + consumed;
    if (count != n) {
        return nullptr;
    }
    return result;
}

// 0x89c2cc FUN_008ac2cc
char* FUN_008ac2cc(char* s, int c, int n) {
    int count = 0;
    const std::size_t consumed = ScanForNth(s, c, n, count);
    if (count != n) {
        return nullptr;
    }
    return s + consumed;
}

// 0x89c314 LC_API_ITOA(int, char*, int)
// The listing writes a zero result as "0", writes '-' for negatives, and then
// stores one digit from the table "0123...XYZ" before the tail call to
// __aeabi_idiv. No terminator follows the digit.
void LC_API_ITOA(int value, char* out, int base) {
    static const char kDigits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    if (value == 0) {
        out[0] = '0';
        out[1] = '\0';
        return;
    }
    if (value < 0) {
        *out++ = '-';
        value = -value;
    }
    *out = kDigits[value % base];
}

// 0x89c88c LC_API_PARSE_DATA
int LC_API_PARSE_DATA(const char* in, char* out, int skip, char separator) {
    int first = -1;
    int written = 0;
    int i = 0;
    *out = '\0';
    for (char c = in[0];; c = in[++i]) {
        if (c == '\0') {
            return first;
        }
        if (c == separator) {
            skip -= 1;
        } else if (skip == 0) {
            out[written] = c;
            if (first == -1) {
                first = i;
            }
            out[written + 1] = '\0';
            written += 1;
        } else if (skip < 0) {
            return first;
        }
    }
}

// 0x89c9a0 getValueLC
int getValueLC(const char* in, char* out, int skip, char separator) {
    int written = 0;
    int i = 0;
    *out = '\0';
    for (char c = in[0];; c = in[++i]) {
        if (c == '\0') {
            return 0;
        }
        if (c == separator) {
            skip -= 1;
        } else if (skip == 0) {
            out[written] = c;
            out[written + 1] = '\0';
            written += 1;
        } else if (skip < 0) {
            return 0;
        }
    }
}

// 0x89ca14 _Z14LC_API_TOLOWERPKcPc
void LC_API_TOLOWER(const char* in, char* out) {
    if (in == nullptr || out == nullptr) {
        return;
    }
    int i = 0;
    unsigned char c = static_cast<unsigned char>(in[0]);
    if (c != 0) {
        do {
            out[i] = (static_cast<unsigned>(c) - 'A' < 26u) ? static_cast<char>(c + ' ') : static_cast<char>(c);
            ++i;
            c = static_cast<unsigned char>(in[i]);
        } while (c != 0);
    }
    out[i] = '\0';
}

// 0x89cb6c _Z26LC_DEBUG_OPEN_RECVDATA_LOGv
void LC_DEBUG_OPEN_RECVDATA_LOG() { fp_recvLC = std::fopen("log_recv.txt", "wt"); }

// 0x89caf8 _Z27LC_DEBUG_CLOSE_RECVDATA_LOGv
void LC_DEBUG_CLOSE_RECVDATA_LOG() { std::fclose(fp_recvLC); }

// 0x89ca7c _Z26LC_DEBUG_SAVE_RECVDATA_LOGPKci
void LC_DEBUG_SAVE_RECVDATA_LOG(const char* data, int size) {
    std::fwrite(data, 1, static_cast<std::size_t>(size), fp_recvLC);
}

// 0x89cbac _Z26LC_DEBUG_OPEN_SENDDATA_LOGv
void LC_DEBUG_OPEN_SENDDATA_LOG() { fp_sendLC = std::fopen("log_send.txt", "wt"); }

// 0x89cb18 _Z27LC_DEBUG_CLOSE_SENDDATA_LOGv
void LC_DEBUG_CLOSE_SENDDATA_LOG() { std::fclose(fp_sendLC); }

// 0x89caa4 _Z26LC_DEBUG_SAVE_SENDDATA_LOGPKci
void LC_DEBUG_SAVE_SENDDATA_LOG(const char* data, int size) {
    std::fwrite(data, 1, static_cast<std::size_t>(size), fp_sendLC);
}

// 0x89cbec LC_API_FILE_OPEN(char*, char*)
int LC_API_FILE_OPEN(const char* name, const char* mode) {
    int slot = 0;
    while (l_file_listLC[slot] != nullptr) {
        ++slot;
        if (slot == kFileSlots) {
            return 0;
        }
    }
    if (slot == kFileSlots) {
        return 0;
    }
    std::FILE* f = std::fopen(name, mode);
    if (f == nullptr) {
        return 0;
    }
    l_file_listLC[slot] = f;
    return slot + 1;
}

// 0x89cb38 LC_API_FILE_CLOSE(int)
void LC_API_FILE_CLOSE(int handle) {
    std::fclose(l_file_listLC[handle - 1]);
    l_file_listLC[handle - 1] = nullptr;
}

// 0x89cacc LC_API_FILE_WRITE(void*, size_t, size_t, int)
void LC_API_FILE_WRITE(const void* data, std::size_t size, std::size_t count, int handle) {
    std::fwrite(data, size, count, l_file_listLC[handle - 1]);
}

// 0x89ce98 LC_API_FILE_READ(void*, size_t, size_t, int)
void LC_API_FILE_READ(void* data, std::size_t size, std::size_t count, int handle) {
    const std::size_t got = std::fread(data, size, count, l_file_listLC[handle - 1]);
    static_cast<void>(got);  // the original ignores the count
}

// 0x89ce28 LC_API_FILE_GET_LENGTH(int)
// Seeks to the start, reads the offset, seeks to the end, reads the offset,
// rewinds, and returns end - start.
int LC_API_FILE_GET_LENGTH(int handle) {
    std::FILE* f = l_file_listLC[handle - 1];
    std::fseek(f, 0, SEEK_SET);
    const long start = std::ftell(f);
    std::fseek(f, 0, SEEK_END);
    const long end = std::ftell(f);
    std::fseek(f, 0, SEEK_SET);
    return static_cast<int>(end - start);
}

// 0x89cc98 LC_API_STRLEN
std::size_t LC_API_STRLEN(const char* s) { return std::strlen(s); }

// 0x89cec4 LC_API_ATOF: strtod with no end pointer.
double LC_API_ATOF(const char* s) { return std::strtod(s, nullptr); }

// 0x89cecc LC_API_ATOI
int LC_API_ATOI(const char* s) { return std::atoi(s); }

// 0x89ced0 LC_API_STRSTR
char* LC_API_STRSTR(const char* haystack, const char* needle) {
    return const_cast<char*>(std::strstr(haystack, needle));
}

// 0x89ced4 LC_API_STRCAT
char* LC_API_STRCAT(char* dest, const char* src) { return std::strcat(dest, src); }

// 0x89ced8 LC_API_STRNCPY
char* LC_API_STRNCPY(char* dest, const char* src, std::size_t n) { return std::strncpy(dest, src, n); }

// 0x89cedc LC_API_STRCPY
char* LC_API_STRCPY(char* dest, const char* src) { return std::strcpy(dest, src); }

// 0x89cee0 LC_API_STRCASECMP. The C library's strcasecmp is POSIX-only, so the
// ASCII comparison is written out here.
int LC_API_STRCASECMP(const char* a, const char* b) {
    for (;; ++a, ++b) {
        const int ca = std::tolower(static_cast<unsigned char>(*a));
        const int cb = std::tolower(static_cast<unsigned char>(*b));
        if (ca != cb) {
            return ca - cb;
        }
        if (ca == '\0') {
            return 0;
        }
    }
}

// 0x89cee4 LC_API_STRNCMP
int LC_API_STRNCMP(const char* a, const char* b, std::size_t n) { return std::strncmp(a, b, n); }

// 0x89cee8 LC_API_STRCMP
int LC_API_STRCMP(const char* a, const char* b) { return std::strcmp(a, b); }

// 0x89ceec LC_API_MEMCPY
void* LC_API_MEMCPY(void* dest, const void* src, std::size_t n) { return std::memcpy(dest, src, n); }

// 0x89cf00 LC_API_MEMSET
void* LC_API_MEMSET(void* dest, int value, std::size_t n) { return std::memset(dest, value, n); }

// 0x89cd00 LC_API_STRTRIM: the listing only measures the string.
void LC_API_STRTRIM(char* s) { LC_API_STRLEN(s); }

// 0x89d364 LC_API_MALLOC
void* LC_API_MALLOC(std::size_t n) { return std::malloc(n); }

// 0x89d360 LC_API_FREE
void LC_API_FREE(void* p) { std::free(p); }

// 0x89d3a8 LC_API_GET_TIME: the listing branches straight to OS_GetTime.
int LC_API_GET_TIME() { return OS_GetTime(); }

// 0x89cc4c LC_API_IP_INT2STR
char* LC_API_IP_INT2STR(unsigned int address, char* out) {
    std::sprintf(out, "%d.%d.%d.%d", address & 0xFF, (address & 0xFF00) >> 8, (address & 0xFF0000) >> 16,
                 address >> 24);
    return out;
}

// 0x89cc9c LC_API_GET_RANDOM. Partial, see the header. Like the original, it
// reseeds from the clock on every call before drawing one value.
long LC_API_GET_RANDOM(const char* seed) {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    if (seed != nullptr) {
        LC_API_STRLEN(seed);
    }
    return static_cast<long>(std::rand());
}
