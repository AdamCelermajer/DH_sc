#include "glitch-core-io/core/strings.hpp"

#include <cmath>
#include <cstdio>

namespace glitch::core {

// The original formats into a temporary from the process buffer heap (0x44 bytes
// for the wide form, 0x11 for the narrow), formats with a 16 character limit, copies
// the text into the result and releases the temporary. The temporary is not
// observable, so it is a local array here.

// 0x325f7c _ZN6glitch4core11int2stringwEi
std::wstring int2stringw(int value) {
    wchar_t text[0x11];
    std::swprintf(text, 0x10, L"%d", value);
    return std::wstring(text);
}

// 0x326070 _ZN6glitch4core11int2stringcEi
std::string int2stringc(int value) {
    char text[0x11];
    std::snprintf(text, 0x10, "%d", value);
    return std::string(text);
}

namespace {

// Decimal scale factors, indexed by the number of decimal digits. These are the
// first 16 entries of Irrlicht's fast_atof_table (the glitch table is 16 floats in
// .rodata; its values are not read from the binary). Longer fractions use pow.
constexpr float kFastAtofTable[16] = {
    0.f,
    0.1f,
    0.01f,
    0.001f,
    0.0001f,
    0.00001f,
    0.000001f,
    0.0000001f,
    0.00000001f,
    0.000000001f,
    0.0000000001f,
    0.00000000001f,
    0.000000000001f,
    0.0000000000001f,
    0.00000000000001f,
    0.000000000000001f,
};

bool isDigit(char ch) { return ch >= '0' && ch <= '9'; }

}  // namespace

// 0x322bc8 _ZN6glitch4core14fast_atof_moveEPKcRf
const char* fast_atof_move(const char* text, float& result) {
    const char* c = text;
    const bool negative = *c == '-';
    if (negative) ++c;

    // Integer digits, accumulated with wrapping 32-bit arithmetic.
    unsigned whole = 0;
    if (isDigit(*c)) {
        do {
            whole = whole * 10u + static_cast<unsigned>(*c - '0');
            ++c;
        } while (isDigit(*c));
    }
    float value = static_cast<float>(whole);

    if (*c == '.') {
        const char* decimalStart = c + 1;
        const char* end = decimalStart;
        unsigned decimals = 0;
        while (isDigit(*end)) {
            decimals = decimals * 10u + static_cast<unsigned>(*end - '0');
            ++end;
        }
        const auto count = static_cast<std::size_t>(end - decimalStart);
        const float scale = count < 16 ? kFastAtofTable[count] : std::pow(10.f, -static_cast<float>(count));
        value = value + static_cast<float>(decimals) * scale;
        c = end;

        if (*c == 'e') {
            ++c;
            const bool exponentNegative = *c == '-';
            if (exponentNegative) ++c;
            unsigned exponent = 0;
            while (isDigit(*c)) {
                exponent = exponent * 10u + static_cast<unsigned>(*c - '0');
                ++c;
            }
            const float signedExponent = exponentNegative ? -static_cast<float>(exponent) : static_cast<float>(exponent);
            value = value * std::pow(10.f, signedExponent);
        }
    }

    result = negative ? -value : value;
    return c;
}

}  // namespace glitch::core
