// Track B batch e1-glitch-scene-collada-gui-glitch_collada_detail_CColladaHardwareQuatSkinTe-0.
//
// Partial batch (one function). The bounding-box walk is reconstructed from the pseudocode,
// but its inputs are read from SSkin (+0x8c, +0x90) and SSkinCache (+0x00, +0x10, +0x14),
// and neither type has a layout in any header yet. The method therefore cannot gather them.
#include <cstddef>
#include <cstdint>
#include <limits>

#include "glitch-scene-collada-gui/collada_misc.hpp"
#include "glitch-shared/glitch_core.hpp"

namespace glitch::collada::detail {

namespace {

using Box = glitch::core::aabbox3d<float>;
using Matrix = glitch::core::CMatrix4<float>;

// Initial value of the result, as the original writes it: MinEdge = +FLT_MAX (0x7f7fffff) on
// every axis and MaxEdge = -FLT_MAX (0xff7fffff). The pseudocode prints the -FLT_MAX compare
// value in the translation path as -3.4028e38; the assembly loads the 0xff7fffff word itself.
Box emptyBox() {
    Box b;
    b.MinEdge.X = b.MinEdge.Y = b.MinEdge.Z = std::numeric_limits<float>::max();
    b.MaxEdge.X = b.MaxEdge.Y = b.MaxEdge.Z = -std::numeric_limits<float>::max();
    return b;
}

// Path taken when SSkin +0x8c is nonzero. Each skin box (SSkin +0x90 array, 0x18 bytes per box
// in the original, i.e. one aabbox3d) is copied, moved by the matrix at the same index (SSkinCache
// +0x10 array of matrix pointers), and added to the result. Each transformed box is added as
// addInternalBox does: its MaxEdge corner first, then its MinEdge corner. The count is the
// matrix-array length truncated to 8 bits (the original's UBFX #2, #8).
[[maybe_unused]] void expandTransformedSkinBoxes(Box& result, const Box* skinBoxes,
                                                 const Matrix* const* matrices,
                                                 std::uint8_t count) {
    for (std::uint8_t i = 0; i < count; ++i) {
        Box local = skinBoxes[i];
        matrices[i]->transformBox(local);
        result.addInternalBox(local);
    }
}

// Path taken when SSkin +0x8c is zero. Each bone matrix contributes its translation as a single
// point: elements 12, 13 and 14 of the row-major matrix (x, y, z). Points are added with the
// strict per-axis compares of addInternalPoint.
[[maybe_unused]] void expandBoneTranslations(Box& result, const Matrix* const* matrices,
                                             std::uint8_t count) {
    for (std::uint8_t i = 0; i < count; ++i) {
        const float* m = matrices[i]->M;
        glitch::core::vector3d<float> p;
        p.X = m[12];
        p.Y = m[13];
        p.Z = m[14];
        result.addInternalPoint(p);
    }
}

}  // namespace

// 0x66e384 _ZN6glitch7collada6detail33CColladaHardwareQuatSkinTechnique18computeBoundingBoxEv
//
// Partial. The original returns the box through the sret pointer (ARM R0), which Hex-Rays shows
// as "this"; the technique is the second argument (R1) and is the receiver. Its steps:
//   1. preparePtrCache()                                  (0x66df88; the member is not reconstructed,
//                                                          so it is not called here)
//   2. result = emptyBox()
//   3. SSkin +0x8c != 0: expandTransformedSkinBoxes over the SSkin +0x90 boxes and the SSkinCache
//      +0x10 matrices; else expandBoneTranslations over the SSkinCache +0x10 matrices
//   4. clear bit 0x8 in the word at offset 0 of the SSkinCache object (the original's **(a2+0x10))
// Steps 1, 3 (the inputs) and 4 need the SSkin and SSkinCache layouts, which no header declares
// (see the layout requests). Until they land, this returns the empty box. The two helpers above
// hold the exact control flow of step 3 and are wired in once the inputs can be read.
glitch::core::aabbox3d<float> CColladaHardwareQuatSkinTechnique::computeBoundingBox() {
    return emptyBox();
}

}  // namespace glitch::collada::detail
