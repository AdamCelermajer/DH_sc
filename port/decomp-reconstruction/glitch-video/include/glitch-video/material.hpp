// glitch::video::CMaterial: material instance. The hash, dirty-flag and renderer
// links used by the comparison operators are laid out here; the heap block and
// parameter blocks are not modelled yet.
#pragma once

#include <cstdint>

#include "glitch-video/deps/glitch_video_types.hpp"

namespace glitch::video {

namespace detail::material {
// Placeholder for the material render-state block. Layout not reconstructed.
struct SRenderState {};
}  // namespace detail::material

class CMaterial {
public:
    // 0x35364c: hash code of the current technique. A dirty bit forces a refresh first.
    uint32_t getHashCode() const;
    // 0x3537b0: same technique and equal parameters.
    bool operator==(const CMaterial& other) const;
    // 0x3537e4: orders by hash code, then by the technique's parameter count, then by compare().
    bool operator<(const CMaterial& other) const;
    // 0x5aa4a8: clears one parameter dirty bit. The original returns a pointer that the decompiler
    // could not tie to a value, so this port returns void.
    void unsetParametersDirty(uint8_t parameterIndex);

    // 0x5c5d7c: the render-state block size for a renderer. The original returns 0.
    uint32_t getRenderStateBlockSize(const CMaterialRenderer&);
    // 0x5c5d84: no body in the original.
    void setRenderStateInternal(uint8_t, uint8_t, const detail::material::SRenderState&);

    // Helpers used by the functions above, reconstructed elsewhere (not defined in this slice).
    // 0x5c5d34: the technique index selected for this material. Partial, see the .cpp.
    int32_t getTechnique() const;
    // updateHashCode (0x5c5fd8) and updateParametersHashCode (0x5c5d88): refresh a cached hash.
    void updateHashCode(uint8_t technique) const;
    void updateParametersHashCode(uint8_t parameterIndex) const;
    // equals (0x353684) and compare (0x5ca3fc): parameter comparisons for one technique pair.
    bool equals(uint8_t technique, const CMaterial& other, uint8_t otherTechnique) const;
    bool compare(uint8_t technique, uint8_t parameterCount, const CMaterial& other, uint8_t otherTechnique) const;
    // Parameter count of a technique: the byte at +4 of the 12-byte technique entry, which the
    // renderer keeps at renderer+0x18.
    uint8_t getTechniqueParameterCount(int32_t technique) const;

private:
    uint32_t m_unknown00 = 0;                 // +0x00, not identified
    CMaterialRenderer* m_renderer = nullptr;  // +0x04, renderer whose technique table is read
    uint8_t unk_08 = 0;                       // +0x08, byte; the fallback result of getTechnique
    uint32_t m_dirtyParameters = 0;           // +0x0c, one bit per parameter that changed
    mutable uint32_t m_dirtyHashes = 0;       // +0x10, one bit per technique whose hash is stale
    uint8_t unk_14 = 0;                       // +0x14, byte; index into the owner's pointer table in getTechnique
    mutable uint32_t* m_hashCodes = nullptr;  // +0x18, one hash word per technique
};

}  // namespace glitch::video
