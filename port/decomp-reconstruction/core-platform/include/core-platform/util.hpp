// Small free helpers from the original string and number utilities.
#pragma once

#include <string>

// 0x317a04 strrstr: last occurrence of needle in haystack.
const char* strrstr(const char* haystack, const char* needle);

// 0x69f54c util_number_is_power_2
bool util_number_is_power_2(unsigned value);

// Lowercases the ASCII capitals in data[from..to] inclusive. A `to` of -1 or
// one past the end means the last character. Shared by the ToLowerCase overloads.
void LowerCaseRange(char* data, int size, int from, int to);

// 0x34e024 _Z11ToLowerCaseRSsii
void ToLowerCase(std::string& s, int from, int to);

// 0x34e090 ToLowerCase over the glitch string type (SAllocator-backed). Partial:
// the glitch string stand-in is not declared, so this template is only used
// through the data()/size() interface.
template <typename StringT>
void ToLowerCase(StringT& s, int from, int to) {
    LowerCaseRange(s.data(), static_cast<int>(s.size()), from, to);
}
