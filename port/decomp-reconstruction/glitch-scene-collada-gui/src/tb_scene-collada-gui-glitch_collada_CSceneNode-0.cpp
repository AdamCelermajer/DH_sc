// Track B batch e1-glitch-scene-collada-gui-glitch_collada_CSceneNode-0.
#include "glitch-scene-collada-gui/collada_scene_accessors.hpp"

namespace glitch::collada {

// 0x65cd80 _ZNK6glitch7collada10CSceneNode18getUserPropertyStrEv
// PARTIAL. The original, from Hex-Rays (the assembly agrees):
//   1. Reads the property block at +0x154. If it is null, returns 0.
//   2. Reads the word at +0x48 of the block. If it is 0, returns 0.
//   3. Otherwise returns the first word of the object that the +0x48 word points to (a uid holder,
//      the same shape as SUidHolder).
// Steps 1 and 2 are done. Step 3 is missing: the +0x48 word is a 32-bit pointer in the original and
// is mapped as int32 `value` for getUserProperty, so it cannot be followed as a host pointer. Layout
// request: the +0x48 word needs a host-width typed view, like the +0x160 union in CLightSceneNode.
const char* CSceneNode::getUserPropertyStr() const {
    if (userProperties == nullptr || userProperties->value == 0) {
        return nullptr;
    }
    // PARTIAL: step 3 (follow the pointer at +0x48) is not reconstructed.
    return nullptr;
}

// 0x65cda8 _ZN6glitch7collada10CSceneNode18computeBoundingBoxEv
// PARTIAL. The original walks the children of the node. ISceneNode::getChildren returns this+0xF0
// (Hex-Rays: `(char *)this + 240`), and the walk starts at the sentinel at +0xF4. Each child is an
// intrusive list node at object+4, so the object is node-4. For each child it does:
//   - virtual slot +0xBC (188) on the child returns its type tag. 'mead' (0x6d656164), 'sead'
//     (0x73656164) and 'Mead' (0x4d656164) are mesh-like children. 'nead' (0x6e656164) is a node child,
//     and for it slot +0xF4 (244) is called first.
//   - virtual slot +0x30 (48) returns a pointer to the child box (6 floats, copied out).
//   - virtual slot +0x40 (64) returns a pointer to the child matrix; transformBoxEx(matrix, box) applies it.
// The first child copies its (untransformed) box into this node's box at +0x130, then transforms that
// box in place with its matrix. Later children are transformed into a local copy and merged with
// addInternalBox into the box at +0x130.
// Missing, so this body is a no-op and the box at +0x130 is left unchanged:
//   - No vptr in this category yet (see glitch::scene::ISceneNode in scene_node.hpp, which has no vptr).
//     None of the slots above can be called without one.
//   - The child container. The stand-in ISceneNode::children is a std::vector at +0xF0, but the
//     original is an intrusive list with a sentinel at +0xF4. Layout request: replace the stand-in with
//     the intrusive list (object+4 node, next word first).
// The original return value is the leftover `this` and is not reproduced (void here).
void CSceneNode::computeBoundingBox() {
    // PARTIAL: see the notes above. No child is visited.
}

// 0x65cf8c _ZNK6glitch7collada10CSceneNode18computeBoundingBoxERNS_4core8aabbox3dIfEE
// PARTIAL. The same child walk as computeBoundingBox() above, but it fills `box` and recurses:
//   - a local box is initialised to min (-1, -1, -1) and max (1, 1, 1) (0xBF800000 and 0x3F800000).
//   - mesh-like children ('mead', 'sead', 'Mead') give their box through slot +0x34 (52), merged into
//     `box` (the first one copies it in and sets the found flag).
//   - node children ('nead') call this function on the child with the local box (the same slot +0xBC type
//     check comes first). When the recursive call returns non-zero the local box is merged or copied in.
//   - the return value is 1 when at least one child contributed, otherwise 0.
// Missing, for the same two reasons as computeBoundingBox() above (no vptr for the slots, and the child
// list layout). The body returns 0 and leaves `box` unchanged, so callers must treat the result as
// unknown until those are in place.
int CSceneNode::computeBoundingBox(core::aabbox3d<float>& box) const {
    (void)box;
    // PARTIAL: child walk not reconstructed. Returns 0 (no child contributed).
    return 0;
}

}  // namespace glitch::collada
