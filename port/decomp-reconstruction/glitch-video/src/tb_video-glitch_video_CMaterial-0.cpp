// Batch e1-glitch-video-glitch_video_CMaterial-0: glitch::video::CMaterial::getTechnique.
#include "glitch-video/material.hpp"

namespace glitch::video {

// 0x5c5d34 _ZNK6glitch5video9CMaterial12getTechniqueEv
// Partial. Hex-Rays control flow, with the parts that cannot be named yet marked:
//   v1 = m_renderer (+0x04); v4 = renderer +0x04 (owner object); v5 = renderer +0x0c (uint16);
//   v2 = this +0x14 (byte, unk_14); result = this +0x08 (byte, unk_08).
//   If owner +0x108 (v6) is non-null: v8 = entry[v5] (owner +0x104 ->+0x18 table, 8-byte entries,
//   +0x04 pointer) -> +0x1c; if v8 != -1 return byte at (v6[v2] + result + v8).
//   Otherwise return result.
// Missing: the owner object's type and fields (+0x104, +0x108 and the table entry at +0x1c), the
// renderer's +0x04 and +0x0c fields (CMaterialRenderer declares only +0x00, +0x10, +0x18, +0x1c),
// and CMaterial's +0x14 byte is declared but its role is not confirmed. Only the path where the
// owner table is absent (v6 == 0) is reproduced, and the fallback result is returned as-is.
// Do not extend this without the owner layout: the other path depends on it.
int32_t CMaterial::getTechnique() const {
    return unk_08;
}

}  // namespace glitch::video
