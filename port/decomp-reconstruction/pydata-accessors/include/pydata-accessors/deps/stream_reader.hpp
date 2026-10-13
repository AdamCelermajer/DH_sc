#pragma once
// IStreamBase and StreamReader are defined once, in the game headers (dh2/IStreamBase.hpp and dh2/StreamReader.hpp);
// this header includes them so the pydata includes keep working. The readers decode through the virtual read slot
// (little-endian values, as in the original).
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>

#include "dh2/IStreamBase.hpp"
#include "dh2/StreamReader.hpp"

// Original MemoryHintState(size, hint): allocation (new[] in the original). Some pseudocode calls
// pass only the size; the hint then defaults to 0 here (the original's value is not in the listing).
inline void* MemoryHintState(std::size_t size, int = 0) {
    return std::malloc(size ? size : 1);
}
// Original CustomFree(p).
inline void CustomFree(void* p) { std::free(p); }
