// Track B batch e1-glitch-scene-collada-gui-glitch_collada_CMeshSceneNode-0.
#include "glitch-scene-collada-gui/collada_scene_accessors.hpp"

namespace glitch::collada {

// 0x646730 _ZN6glitch7collada14CMeshSceneNode6renderEPv
// PARTIAL. The original, in order (Hex-Rays and assembly at 0x646730 agree):
//   1. Loads the mesh record at +0x134 and the video driver at scene manager (+0x110) +0x14.
//      Returns if either is null.
//   2. Calls the mesh record's vtable slot at byte +0x44 with (mesh, driver, &absolute transform at +0x24).
//   3. If the context argument a2 is non-zero, uses index = a2 - 1 and calls the mesh vtable slot at +0x14
//      with (&buffer, mesh, index). Returns if the returned buffer is null.
//   4. Reads the mesh word at +0x14, bit (index & 31). If the bit is set, the post-draw flag is 0; otherwise
//      slot +0x38 (1, driver, index) is called and its result masked with 4 gives the post-draw flag.
//   5. Slot +0x18 gives the material, slot +0x1C gives the vertex attribute map, both ref-counted.
//      The attribute map reference is dropped, then driver setMaterial(driver, &material, &map) and
//      driver drawMeshBuffer(driver, &buffer) run, the buffer reference is dropped.
//   6. If the post-draw flag is set, slot +0x3C (driver, index) runs.
//   7. Drops the material and buffer references.
// Missing, so this body is a no-op and the function stays partial:
//   - The vtable of the object at +0x134. The header types it as SNamedMeshData, which has no vptr, but
//     render dispatches through it. Layout request: SNamedMeshData (or the owner's record type) needs a
//     vptr at +0x00 and the mesh interface, with slots +0x14, +0x18, +0x1C, +0x38, +0x3C, +0x44 named.
//   - The scene manager at +0x110 and its driver word at +0x14. CMeshSceneNode has no ISceneNode base
//     or scene manager member in this category yet, so the driver cannot be reached.
//   - The IVideoDriver interface (setMaterial, drawMeshBuffer) and the ref-counted buffer, material and
//     attribute-map types as declared in this category.
// The original return value is the leftover r0 value and is not reproduced (void here).
void CMeshSceneNode::render(void* context) {
    (void)context;
}

}  // namespace glitch::collada
