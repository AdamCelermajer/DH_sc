#include "core-platform/lc_api.hpp"

#include <cstdio>
#include <cstring>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    // Counted search: pointer past the n-th match.
    char text[] = "a,b,c";
    CHECK(LC_API_STRCHRFIND(text, ',', 1) == text + 2);
    CHECK(LC_API_STRCHRFIND(text, ',', 2) == text + 4);
    CHECK(LC_API_STRCHRFIND(text, ',', 3) == nullptr);
    CHECK(LC_API_STRCHRFIND(text, ',', 0) == text);
    CHECK(FUN_008ac2cc(text, ',', 1) == text + 2);

    // Integer to text: zero, negative, and the single-digit output of the listing.
    char num[8];
    LC_API_ITOA(0, num, 10);
    CHECK(std::strcmp(num, "0") == 0);
    LC_API_ITOA(-7, num, 10);
    CHECK(num[0] == '-' && num[1] == '7');

    // Parse: skip one separator, copy the rest.
    char out[32];
    const int first = LC_API_PARSE_DATA("key,value,more", out, 1, ',');
    CHECK(std::strcmp(out, "value") == 0);
    CHECK(first == 4);
    CHECK(LC_API_PARSE_DATA("abc", out, 0, ',') == 0 && std::strcmp(out, "abc") == 0);
    CHECK(getValueLC("x,y", out, 1, ',') == 0 && std::strcmp(out, "y") == 0);

    char low[16];
    LC_API_TOLOWER("MiXed 123", low);
    CHECK(std::strcmp(low, "mixed 123") == 0);

    CHECK(LC_API_STRLEN("hello") == 5);
    CHECK(LC_API_ATOI("42") == 42);
    CHECK(LC_API_ATOF("2.5") == 2.5);
    CHECK(LC_API_STRCMP("a", "b") < 0);
    CHECK(LC_API_STRNCMP("abc", "abd", 2) == 0);
    CHECK(LC_API_STRCASECMP("HeLLo", "hello") == 0);
    CHECK(LC_API_STRCASECMP("a", "B") < 0);

    char buf[32] = "abc";
    LC_API_STRCPY(buf, "xy");
    CHECK(std::strcmp(buf, "xy") == 0);
    LC_API_STRCAT(buf, "z");
    CHECK(std::strcmp(buf, "xyz") == 0);
    CHECK(LC_API_STRSTR(buf, "yz") == buf + 1);

    char ip[32];
    LC_API_IP_INT2STR(0x0100007Fu, ip);  // 127.0.0.1 stored little endian
    CHECK(std::strcmp(ip, "127.0.0.1") == 0);

    // File slots: open, write, measure, read back, close.
    const char* path = "lc_api_test.tmp";
    int h = LC_API_FILE_OPEN(path, "wb");
    CHECK(h == 1);
    LC_API_FILE_WRITE("0123456789", 1, 10, h);
    LC_API_FILE_CLOSE(h);
    h = LC_API_FILE_OPEN(path, "rb");
    CHECK(h == 1);
    CHECK(LC_API_FILE_GET_LENGTH(h) == 10);
    char back[11] = {};
    LC_API_FILE_READ(back, 1, 10, h);
    CHECK(std::strcmp(back, "0123456789") == 0);
    LC_API_FILE_CLOSE(h);
    std::remove(path);
    CHECK(LC_API_FILE_OPEN("/nonexistent/dir/x", "rb") == 0);

    void* m = LC_API_MALLOC(8);
    CHECK(m != nullptr);
    LC_API_MEMSET(m, 0x5A, 8);
    LC_API_FREE(m);

    char dst[4];
    LC_API_MEMCPY(dst, "abc", 4);
    CHECK(std::strcmp(dst, "abc") == 0);
    LC_API_GET_TIME();
    LC_API_GET_RANDOM(nullptr);

    std::printf("%s\n", failures ? "lc_api: FAILED" : "lc_api: ok");
    return failures != 0;
}
