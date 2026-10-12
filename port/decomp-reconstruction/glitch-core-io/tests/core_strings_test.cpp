// int2stringc / int2stringw: decimal text of an int, including the extremes.
#include "glitch-core-io/core/strings.hpp"

#include <climits>
#include <cstdio>
#include <string>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    using glitch::core::int2stringc;
    using glitch::core::int2stringw;

    CHECK(int2stringc(0) == "0");
    CHECK(int2stringc(-42) == "-42");
    CHECK(int2stringc(123456) == "123456");
    CHECK(int2stringc(INT_MIN) == "-2147483648");
    CHECK(int2stringc(INT_MAX) == "2147483647");

    CHECK(int2stringw(0) == L"0");
    CHECK(int2stringw(-7) == L"-7");
    CHECK(int2stringw(INT_MIN) == L"-2147483648");

    // fast_atof_move: sign, integer and decimal digits; exponent only after a '.'.
    float v = 0.f;
    const char* end = glitch::core::fast_atof_move("3.25 rest", v);
    CHECK(v == 3.25f && std::string(end) == " rest");
    end = glitch::core::fast_atof_move("-0.5", v);
    CHECK(v == -0.5f && *end == '\0');
    end = glitch::core::fast_atof_move("12", v);
    CHECK(v == 12.f && *end == '\0');
    end = glitch::core::fast_atof_move("1.5e2x", v);
    CHECK(v == 150.f && std::string(end) == "x");
    end = glitch::core::fast_atof_move("2.5e-1", v);
    CHECK(v > 0.2499f && v < 0.2501f && *end == '\0');
    end = glitch::core::fast_atof_move("1e3", v);
    CHECK(v == 1.f && std::string(end) == "e3");
    end = glitch::core::fast_atof_move("abc", v);
    CHECK(v == 0.f && std::string(end) == "abc");

    // stringc2stringw / stringw2stringc: element-wise conversion; null gives "".
    CHECK(glitch::core::stringc2stringw("ab1") == L"ab1");
    CHECK(glitch::core::stringc2stringw(nullptr).empty());
    CHECK(glitch::core::stringw2stringc(L"Hi!") == "Hi!");
    CHECK(glitch::core::stringw2stringc(nullptr).empty());

    std::printf("%s\n", failures ? "core_strings: FAILED" : "core_strings: ok");
    return failures != 0;
}
