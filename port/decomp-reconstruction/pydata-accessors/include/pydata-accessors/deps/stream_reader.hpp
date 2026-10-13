#pragma once
// IStreamBase and StreamReader are defined once, in the game headers (dh2/IStreamBase.hpp and dh2/StreamReader.hpp);
// this header includes them so the pydata includes keep working. The readers decode through the virtual read slot
// (little-endian values, as in the original).
// CustomFree and MemoryHintState are core-platform's (core-platform/memory.hpp). The original's allocation
// operator new[](size, MemoryHintState) is declared in dh2/global/runtime_support.hpp and defined in game-app.
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>

#include "core-platform/memory.hpp"
#include "dh2/global/runtime_support.hpp"
#include "dh2/IStreamBase.hpp"
#include "dh2/StreamReader.hpp"

// Hint the pydata code passes to operator new[] for its string buffers, element tables and word arrays. The original
// passes 1 (in R1 at the operator new[] call).
inline constexpr MemoryHintState kPydataBufferHint = static_cast<MemoryHintState>(1);
