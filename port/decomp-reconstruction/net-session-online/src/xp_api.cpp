#include "net-session-online/xp_api.hpp"

#include <cstdlib>
#include <cstring>
#include <new>

#ifdef _WIN32
#include <string.h>
#else
#include <strings.h>
#endif

// 0x82a664 XP_DEBUG_INIT (bx lr)
void XP_DEBUG_INIT() {}

// 0x82a668 XP_DEBUG_FINALIZE (bx lr)
void XP_DEBUG_FINALIZE() {}

// 0x82b334 XP_API_STRSTR
char* XP_API_STRSTR(char* haystack, char* needle) { return std::strstr(haystack, needle); }

// 0x82b338 XP_API_STRCAT
char* XP_API_STRCAT(char* dest, char* src) { return std::strcat(dest, src); }

// 0x82b33c XP_API_STRNCPY
char* XP_API_STRNCPY(char* dest, char* src, std::size_t n) { return std::strncpy(dest, src, n); }

// 0x82b340 XP_API_STRCPY
char* XP_API_STRCPY(char* dest, char* src) { return std::strcpy(dest, src); }

// 0x82b344 XP_API_STRCASECMP
int XP_API_STRCASECMP(char* s1, char* s2) {
#ifdef _WIN32
    return _stricmp(s1, s2);
#else
    return ::strcasecmp(s1, s2);
#endif
}

// 0x82b348 XP_API_STRNCMP
int XP_API_STRNCMP(char* s1, char* s2, std::size_t n) { return std::strncmp(s1, s2, n); }

// 0x82b34c XP_API_STRCMP
int XP_API_STRCMP(char* s1, char* s2) { return std::strcmp(s1, s2); }

// 0x82b364 XP_API_MEMSET
void* XP_API_MEMSET(void* s, int c, std::size_t n) { return std::memset(s, c, n); }

// 0x82b320 XP_API_ATOI
int XP_API_ATOI(char* text) { return std::atoi(text); }

// 0x82b318 XP_API_ATOF
// The ARM sets r1 = 0 and tail-calls strtod, so the double result comes back unchanged.
double XP_API_ATOF(char* text) { return std::strtod(text, nullptr); }

// 0x82afac XP_API_STRLEN
std::size_t XP_API_STRLEN(char* s) { return std::strlen(s); }

// 0x82b350 XP_API_MEMCPY
// The ARM keeps dest in r4 across the memcpy call and returns it.
void* XP_API_MEMCPY(void* dest, void* src, std::size_t n) {
    std::memcpy(dest, src, n);
    return dest;
}

// 0x82b9f0 XP_API_MALLOC
void* XP_API_MALLOC(std::size_t size) { return std::malloc(size); }

// 0x82b9e4 XP_API_FREE
void XP_API_FREE(void* p) {
    if (p == nullptr) {
        return;
    }
    std::free(p);
}

// 0x82b9a0 XP_API_STRNEW
// The ARM allocates len + 1 bytes with operator new[] (_Znaj), zeroes them and copies
// len bytes from text.
char* XP_API_STRNEW(char* text) {
    const std::size_t len = XP_API_STRLEN(text);
    char* copy = new char[len + 1];
    std::memset(copy, 0, len + 1);
    std::memcpy(copy, text, len);
    return copy;
}

// 0x82b324 XP_API_STRCHR
char* XP_API_STRCHR(char* s, int c, int mode) {
    if (mode != 1) {
        return std::strrchr(s, c);
    }
    return std::strchr(s, c);
}

// 0x82b014 XP_API_STRTRIM
// The ARM keeps a flag that is set while the output is still at the start of the
// string. Spaces seen before the first other character are dropped, and everything
// after that is copied as it is. The terminator is written at the new length.
char* XP_API_STRTRIM(char* s) {
    const std::size_t len = XP_API_STRLEN(s);
    std::size_t out = 0;
    bool leading = true;
    for (std::size_t i = 0; i < len; ++i) {
        const char c = s[i];
        if (c == ' ' && leading) {
            continue;
        }
        leading = false;
        s[out++] = c;
    }
    s[out] = '\0';
    return s;
}

// 0x82acdc _Z21XP_API_GetNationalityPc (bx lr)
void XP_API_GetNationality(char* /*out*/) {}
