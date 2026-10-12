#include "core-platform/util.hpp"

#include <cstring>

// 0x317a04 _Z7strrstrPKcS0_
// Partial: the original tail-calls into an unrecovered routine for non-empty
// needles. This implements the name's meaning (last occurrence) and is not
// verified against the binary.
const char* strrstr(const char* haystack, const char* needle) {
    if (*needle == '\0') {
        return haystack;
    }
    const char* found = nullptr;
    const char* p = haystack;
    while ((p = std::strstr(p, needle)) != nullptr) {
        found = p;
        ++p;
    }
    return found;
}

// 0x69f54c _Z22util_number_is_power_2j
bool util_number_is_power_2(unsigned value) {
    if (value == 0) {
        return false;
    }
    return ((value - 1) | value) == ((value - 1) ^ value);
}
