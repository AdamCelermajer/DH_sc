// Batch e1-glitch-video-glitch_video_CMaterialRenderer-0 (owner glitch::video::CMaterialRenderer).
// Layout used here (material_vertex_attribute_map.hpp stand-in, material_renderer_manager_layout.hpp):
//   +0x10 u8 technique count, +0x18 technique table (12-byte entries, stride 12 in the original),
//   entry +0x08 pass array (52-byte elements), element +0x00..+0x1f render state, +0x30 u8 dirty flag.

#include <cstddef>
#include <cstdint>
#include <cstring>

#include "glitch-video/material_vertex_attribute_map.hpp"
#include "glitch-video/tb/renderpass_srenderstate.hpp"

namespace glitch::video {

// The copied block is the first 32 bytes of SRenderState. The layout check keeps the memcpy honest.
static_assert(sizeof(detail::renderpass::SRenderState) == 32, "SRenderState is the 32-byte state block");

namespace {
constexpr std::size_t kPassStateBytes = 32;  // memcmp and copy length (0x20 in the original)
constexpr std::size_t kPassDirtyFlag = 48;   // +0x30 of the pass element, set to 1 after a change
}  // namespace

// 0x5d3034 _ZN6glitch5video17CMaterialRenderer14setRenderStateEhhRKNS0_6detail10renderpass12SRenderStateE
// Returns the bit pattern of the state's word at +0x10 when the pass changed, and 0 when it did not.
// The original's return value is that word (its result is overwritten after the copy); kept as is.
int32_t CMaterialRenderer::setRenderState(uint8_t technique, uint8_t pass,
                                          const detail::renderpass::SRenderState& state) {
    // table[technique].blocks is the pass array of that technique (the original reads +0x18, *12 + 8).
    // The stand-in declares the array const; the original owns it and writes into it, so it is cast here.
    const STechniqueEntry& entry = m_techniques18[technique];
    SRendererParameterBlock& element =
        const_cast<SRendererParameterBlock&>(entry.blocks[pass]);  // element = blocks + 52 * pass

    if (std::memcmp(element.bytes, &state, kPassStateBytes) != 0) {
        std::memcpy(element.bytes, &state, kPassStateBytes);  // eight words, in order
        element.bytes[kPassDirtyFlag] = 1;                     // *((_BYTE *)v5 + 48) = 1

        int32_t word10 = 0;  // the original reads the word at argument +0x10 as an int
        std::memcpy(&word10, &state.m_float10, sizeof(word10));
        return word10;
    }
    return 0;
}

// getTechniqueID(char const*) const (0x5d4714) is not written in this batch. The original obtains the
// shared-string key with SSharedStringHeapEntry::SData::get(name, false), compares the table entries
// (+0x00 word, the key) against that key, restores the key's refcount and releases it when it reaches
// zero. The port declares no int* refcounted SData get/release, and the stand-in entry's +0x00 word is
// not identified. Not-found result, verified in the listing: MOV R4, #0xFF (the value 255).

}  // namespace glitch::video
