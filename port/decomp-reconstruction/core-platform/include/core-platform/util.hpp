// Small free helpers from the original string and number utilities.
#pragma once

// 0x317a04 strrstr: last occurrence of needle in haystack.
const char* strrstr(const char* haystack, const char* needle);

// 0x69f54c util_number_is_power_2
bool util_number_is_power_2(unsigned value);
