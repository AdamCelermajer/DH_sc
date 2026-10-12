// Track B batch e1-glitch-scene-collada-gui-glitch_collada_detail_CColladaHardwareMatrixSkin-0.
// Owner: glitch::collada::detail::CColladaHardwareMatrixSkinTechnique (declared in
// glitch-scene-collada-gui/collada_misc.hpp).

#include <limits>

#include "glitch-scene-collada-gui/collada_misc.hpp"
#include "glitch-shared/glitch_core.hpp"

namespace glitch::collada::detail {

// 0x66c5f4 _ZN6glitch7collada6detail35CColladaHardwareMatrixSkinTechnique18computeBoundingBoxEv
// partial: the original is `aabbox3d<float> computeBoundingBox()`. Its return value goes
// through the hidden result pointer (Hex-Rays `this`, the first argument), and the technique
// is the second argument (Hex-Rays `a2`). Reconstructed: the box reset to the inverted extremes
// (min = +FLT_MAX, max = -FLT_MAX, as stored by the original) and the return.
// Not reconstructed, so the returned box is still the empty reset box:
//   - preparePtrCache(a2) (0x66c544) at entry. The member has no definition in any batch yet
//     (recon/slice_23 holds a free-function stub for another owner), so it is not called here.
//   - the box loop when skin+0x8c is non-zero: boxes of six floats at a 24-byte stride from
//     skin+0x90, each transformed by the matrix pointed to by skinCache+0x10 + 4*i, then merged
//     into the box at both corners (min corner, then max corner). Needs the SSkin layout
//     (count +0x8c, box array +0x90) and the SSkinCache layout (matrix pointer vector begin
//     +0x10 and end +0x14); neither is mapped.
//   - the point loop when skin+0x8c is zero: the translation (M[12], M[13], M[14]) of each cached
//     matrix is merged into the box as one point. Same missing layouts as above.
//     The pseudocode truncates the element count to 8 bits: (unsigned __int8)((end - begin) >> 2).
//   - the final clear of bit 0x08 in the first word of the SSkinCache object (`**cache &= ~8`).
glitch::core::aabbox3d<float> CColladaHardwareMatrixSkinTechnique::computeBoundingBox() {
    constexpr float kMax = std::numeric_limits<float>::max();  // 0x7f7fffff

    glitch::core::aabbox3d<float> box;
    box.MinEdge.X = kMax;   // +0x00 (returned at offset 0 of the result)
    box.MinEdge.Y = kMax;   // +0x04
    box.MinEdge.Z = kMax;   // +0x08
    box.MaxEdge.X = -kMax;  // +0x0c (0xff7fffff)
    box.MaxEdge.Y = -kMax;  // +0x10
    box.MaxEdge.Z = -kMax;  // +0x14
    return box;
}

}  // namespace glitch::collada::detail
