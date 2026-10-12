// Slice 20 reconstruction tests: hash fold and the empty render-data bodies.
#include "glitch-scene-collada-gui/recon/slice_20.hpp"

#include <cstdint>

int main() {
    using glitch::collada::recon_20::IParticleContext_hashString;

    // Empty string hashes to zero.
    if (IParticleContext_hashString(nullptr, "") != 0u) return 1;

    // One char 'a' (97): 97 - 0x61C88647 mod 2^32.
    if (IParticleContext_hashString(nullptr, "a") != 2654435866u) return 1;

    // Chars are sign-extended: 0xFF becomes -1 before the subtraction.
    if (IParticleContext_hashString(nullptr, "\xff") != 0x9E3779B8u) return 1;

    // The fold is sequential: hashing "ab" equals folding 'b' onto the hash of "a".
    const std::uint32_t ha = IParticleContext_hashString(nullptr, "a");
    const std::uint32_t expected = ha ^ (98u - 0x61C88647u + (ha << 6) + (ha >> 2));
    if (IParticleContext_hashString(nullptr, "ab") != expected) return 1;

    // Empty-body pseudocode functions must be callable with no side effects.
    glitch::collada::recon_20::RDB_NullColor_initPRenderData();
    glitch::collada::recon_20::RDB_BillboardColor_initPRenderData();

    return 0;
}
