#include "net-session-online/xp_api.hpp"

#include <cstdio>
#include <cstring>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    char buf[64];
    std::strcpy(buf, "hello world");
    CHECK(XP_API_STRLEN(buf) == 11);
    CHECK(XP_API_STRCMP(buf, (char*)"hello world") == 0);
    CHECK(XP_API_STRNCMP(buf, (char*)"help", 3) == 0);
    CHECK(XP_API_STRCASECMP((char*)"ABC", (char*)"abc") == 0);
    CHECK(XP_API_STRSTR(buf, (char*)"wor") == buf + 6);
    CHECK(XP_API_STRCHR(buf, 'o', 1) == buf + 4);   // mode 1: first occurrence
    CHECK(XP_API_STRCHR(buf, 'o', 0) == buf + 7);   // any other mode: last occurrence

    char dst[32];
    XP_API_STRCPY(dst, (char*)"abc");
    CHECK(std::strcmp(dst, "abc") == 0);
    XP_API_STRCAT(dst, (char*)"def");
    CHECK(std::strcmp(dst, "abcdef") == 0);
    CHECK(XP_API_STRNCPY(dst, (char*)"xy", 2) == dst);
    CHECK(dst[0] == 'x' && dst[1] == 'y');

    CHECK(XP_API_MEMSET(dst, 'z', 3) == dst && dst[2] == 'z');
    char copy[8];
    CHECK(XP_API_MEMCPY(copy, (char*)"qrs", 4) == copy);
    CHECK(std::memcmp(copy, "qrs", 3) == 0);

    CHECK(XP_API_ATOI((char*)"-42") == -42);
    CHECK(XP_API_ATOF((char*)"2.5") == 2.5);

    // Leading spaces are removed in place. Trailing and inner spaces are kept.
    char spaced[] = "   ab c ";
    CHECK(XP_API_STRTRIM(spaced) == spaced);
    CHECK(std::strcmp(spaced, "ab c ") == 0);
    char none[] = "   ";
    XP_API_STRTRIM(none);
    CHECK(none[0] == '\0');
    char empty[] = "";
    XP_API_STRTRIM(empty);
    CHECK(empty[0] == '\0');

    char* made = XP_API_STRNEW((char*)"copy me");
    CHECK(std::strcmp(made, "copy me") == 0);
    delete[] made;

    void* block = XP_API_MALLOC(16);
    CHECK(block != nullptr);
    XP_API_FREE(block);
    XP_API_FREE(nullptr);

    XP_DEBUG_INIT();
    XP_DEBUG_FINALIZE();
    XP_API_GetNationality(buf);

    std::printf("%s\n", failures ? "xp_api: FAILED" : "xp_api: ok");
    return failures != 0;
}
