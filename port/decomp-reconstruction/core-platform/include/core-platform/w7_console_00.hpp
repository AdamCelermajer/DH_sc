// Console helpers used by w7_console_00.cpp. Console itself is defined in w7_console_01.hpp.
// The Console object layout is only partly known. Offsets are taken from the
// listings at each use site, so no data members are declared here.
#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "core-platform/w7_console_01.hpp"  // the one Console definition (w7_console_01)


// View of the std::vector<std::string> stored at byte offset `off` in a Console.
// The offset comes from the listing (ARM 32-bit layout); no member is declared for it.
inline std::vector<std::string>& ConsoleVectorAt(Console* self, std::size_t off) {
    return *reinterpret_cast<std::vector<std::string>*>(reinterpret_cast<char*>(self) + off);
}
