// Track B batch e1-glitch-scene-collada-gui-glitch_collada_detail_CColladaHardwareTextureSki-0:
// glitch::collada::detail::CColladaHardwareTextureSkinTechnique.
//
// Partial. Only the bounding-box arithmetic of computeBoundingBox is reconstructed, as a free helper
// below. The member wrapper is not defined, because the fields it reads are not declared in the owner
// class (collada_misc.hpp maps the technique only up to +0x14 and pads the rest):
//   - +0x0c (skin, SSkin&) and +0x10 (skinCache, SSkinCache&) are not members of the owner class.
//   - SSkin: the count word at +0x8c and the 24-byte box-table pointer at +0x90 are not named.
//   - SSkinCache: the flags word at +0x00 (bit 0x8 is cleared by the original) and the matrix-pointer
//     vector (begin at +0x10, end at +0x14) are not named. SSkinCache is only a stand-in in recon/.
//   - preparePtrCache() (0x66ee94) is partial; it is called first by the original.
// Layout requests are recorded in the batch ledger row.
//
// Return convention: Hex-Rays prints the function as returning a pointer to its first argument, which
// it treats as `this`. The first argument is the aabbox3d<float> sret slot (the box is written there and
// returned), and the second is the technique (a2 in the listing). The real return type is therefore
// glitch::core::aabbox3d<float>, returned by value.

#include <cstddef>
#include <cstdint>
#include <limits>

#include "glitch-shared/glitch_core.hpp"

namespace glitch::collada::detail {

namespace {

using Box = glitch::core::aabbox3d<float>;
using Matrix = glitch::core::CMatrix4<float>;

// Grows `box` to contain one point. Each axis is compared on its own, against the current edge.
void expandByPoint(Box& box, float x, float y, float z) noexcept {
    if (x > box.MaxEdge.X) box.MaxEdge.X = x;
    if (y > box.MaxEdge.Y) box.MaxEdge.Y = y;
    if (z > box.MaxEdge.Z) box.MaxEdge.Z = z;
    if (x < box.MinEdge.X) box.MinEdge.X = x;
    if (y < box.MinEdge.Y) box.MinEdge.Y = y;
    if (z < box.MinEdge.Z) box.MinEdge.Z = z;
}

// 0x66ef44 glitch::collada::detail::CColladaHardwareTextureSkinTechnique::computeBoundingBox()
// Arithmetic core of the original. Inputs, all read by the partial member wrapper:
//   matrices     the skin cache's matrix pointers (skinCache +0x10 begin).
//   matrixCount  (end - begin) / 4, cast to 8 bits: the original's `(unsigned __int8)` truncation is kept.
//   boxTable     the skin's 24-byte box entries (min corner, then max corner) at skin +0x90. Pass null
//                when the original's skin +0x8c word is zero (branch 2).
// Starts from an empty box (min = +FLT_MAX, max = -FLT_MAX), as the original's stores do.
// With boxTable: each entry is copied, transformed by its matrix, and both corners are added (the max
// corner first, as in the original). Without it: each matrix contributes its translation (M[12..14]).
// The original's side effect on the skin cache (clearing bit 0x8 of its flags word at +0x00) is not
// done here; it belongs to the wrapper.
[[maybe_unused]] Box skinBoundingBox(const Matrix* const* matrices, std::uint8_t matrixCount,
                                     const Box* boxTable) noexcept {
    Box box;
    box.MinEdge.X = box.MinEdge.Y = box.MinEdge.Z = std::numeric_limits<float>::max();
    box.MaxEdge.X = box.MaxEdge.Y = box.MaxEdge.Z = -std::numeric_limits<float>::max();

    for (std::size_t i = 0; i < matrixCount; ++i) {
        if (boxTable != nullptr) {
            Box local = boxTable[i];
            matrices[i]->transformBox(local);
            expandByPoint(box, local.MaxEdge.X, local.MaxEdge.Y, local.MaxEdge.Z);
            expandByPoint(box, local.MinEdge.X, local.MinEdge.Y, local.MinEdge.Z);
        } else {
            const Matrix& m = *matrices[i];
            expandByPoint(box, m.M[12], m.M[13], m.M[14]);
        }
    }
    return box;
}

}  // namespace

}  // namespace glitch::collada::detail
