// glitch::core string helpers used by the io attribute code. The original returns
// glitch strings by value (sret); here they are std::string and std::wstring.
#pragma once

#include <string>

namespace glitch::core {

// 0x325f7c _ZN6glitch4core11int2stringwEi: decimal text of value, as a wide string.
std::wstring int2stringw(int value);

// 0x326070 _ZN6glitch4core11int2stringcEi: decimal text of value, as a narrow string.
std::string int2stringc(int value);

// 0x322bc8 _ZN6glitch4core14fast_atof_moveEPKcRf: parses an optional '-', the integer
// digits, and an optional '.' with decimal digits. An exponent ('e', optional '-') is
// only read after a decimal point, as in the original. Returns the first character
// not consumed; result is written with the sign applied.
const char* fast_atof_move(const char* text, float& result);

}  // namespace glitch::core
