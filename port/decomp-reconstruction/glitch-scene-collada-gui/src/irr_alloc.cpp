#include "glitch-scene-collada-gui/irr_alloc.hpp"

#include <cstdlib>

extern "C" {

// 0x5341a0 irr_malloc
// partial: GlitchAlloc(size, hint 0) is replaced by std::malloc.
void* irr_malloc(std::size_t size) {
    return std::malloc(size);
}

// 0x534180 irr_free
// partial: GlitchFree is replaced by std::free.
void irr_free(void* block) {
    std::free(block);
}

}  // extern "C"
