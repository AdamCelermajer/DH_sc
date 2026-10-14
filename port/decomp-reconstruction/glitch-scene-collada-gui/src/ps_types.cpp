#include "glitch-scene-collada-gui/ps_types.hpp"

#include <cstdlib>

namespace glitch::ps {

// The PD shape bodies are in src/ps/PDomain.cpp.

// 0x64d290 _ZN6glitch2ps6PS_NEWEj
void* PS_NEW(std::uint32_t size) {
    return std::malloc(size);
}

// 0x64d778 _ZN6glitch2ps7PS_FREEEPv
void PS_FREE(void* p) {
    std::free(p);
}

}  // namespace glitch::ps
