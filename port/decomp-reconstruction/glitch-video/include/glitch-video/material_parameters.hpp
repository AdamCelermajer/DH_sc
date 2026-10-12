// glitch::video::detail::IMaterialParameters<Owner, Header>: the typed setters
// and getters of a parameter block. The three original instances are
// IMaterialParameters<CGlobalMaterialParameterManager, SEmptyBase>,
// IMaterialParameters<CMaterial, ISharedMemoryBlockHeader<CMaterial>> and
// IMaterialParameters<CMaterialRenderer, ISharedMemoryBlockHeader<CMaterialRenderer>>.
// The value conversions are the same in all three. Only CMaterial tracks dirty
// words (+0x0c and +0x10); the other two owners store without marking.
#pragma once

#include <cstdint>

#include "glitch-video/deps/glitch_core.hpp"

namespace glitch::video {

class CGlobalMaterialParameterManager;
class CMaterial;
class CMaterialRenderer;

namespace detail {

template <typename Owner, bool kTrackDirty>
class IMaterialParameters {
public:
    // setDirty: CMaterial sets both dirty words to 0xffffffff; the others do nothing.
    void setDirty();

    // float -> int, and int -> float, with the dirty mark on change when tracked.
    void setParameterAt(int32_t* dst, float value);
    void setParameterAt(float* dst, int32_t value);
    // Colour and vector copies.
    void setParameterAt(core::vector4d<float>* dst, const SColorf& value);
    void setParameterAt(SColorf* dst, const core::vector4d<float>& value);

    void getParameterAt(const int32_t* src, float& value) const;
    void getParameterAt(const float* src, int32_t& value) const;
    void getParameterAt(const SColorf* src, core::vector4d<float>& value) const;
    void getParameterAt(const core::vector4d<float>* src, SColorf& value) const;

    // Test helpers, not in the original.
    uint32_t dirtyWord0() const { return m_dirty0; }
    uint32_t dirtyWord1() const { return m_dirty1; }

private:
    uint32_t m_dirty0 = 0;  // +0x0c, set to 0xffffffff by setDirty (tracking owners)
    uint32_t m_dirty1 = 0;  // +0x10
};

}  // namespace detail

}  // namespace glitch::video
