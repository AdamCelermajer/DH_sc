// Track B batch e1-glitch-scene-collada-gui-glitch_scene_CMeshSceneNode-0.
#include "glitch-scene-collada-gui/scene_nodes.hpp"

namespace glitch::scene {

// 0x585118 _ZN6glitch5scene14CMeshSceneNodeC2ERKN5boost13intrusive_ptrINS0_5IMeshEEEiRKNS_4core8vector3dIfEERKNS5_10quaternionES9_
// PARTIAL. The original does four things in order:
//   1. Writes its primary vtable at +0x00, the secondary vtable at +0x138 and a vtable word through
//      the virtual-base offset at vptr-12. The class layout here has no vptr or virtual base, so none of
//      these stores is modelled.
//   2. Calls the ISceneNode base constructor with the id, position, rotation and scale. ISceneNode has no
//      constructor in this category yet, so the four arguments are not forwarded. The base layout is
//      unknown.
//   3. Stores the mesh at +0x130 with a reference count bump on the mesh (+0x04). The category's
//      intrusive_ptr stand-in does not count, so only the pointer copy is done here.
//   4. Calls ISceneNode::setAutomaticCulling(this, 2). CMeshSceneNode does not derive from ISceneNode in
//      scene_nodes.hpp, so that call is not applied.
// Reconstructed: +0x130 mesh, +0x134 render counter = 0 (in-class initializer), +0x13c = 1 (in-class initializer).
CMeshSceneNode::CMeshSceneNode(const boost::intrusive_ptr<IMesh>& meshValue, int id,
                               const core::vector3d<float>& position, const core::quaternion& rotation,
                               const core::vector3d<float>& scale)
    : mesh(meshValue) {
    (void)id;
    (void)position;
    (void)rotation;
    (void)scale;
}

// 0x585370 _ZN6glitch5scene14CMeshSceneNode6renderEPv
// PARTIAL. The original, in order:
//   1. Reads the scene manager at this+0x110 (ISceneNode base, not in the layout) and the video driver at
//      scene manager +0x14 (CSceneManager has no driver member here). Returns if the mesh (+0x130) or the
//      driver is null.
//   2. Increments the render counter at +0x134.
//   3. Calls the driver vtable slot at byte 108 with (driver, 1, &absoluteTransform), where the matrix is
//      at +0x24. The slot is not named in the category, so the call is omitted.
//   4. Loops over the mesh buffers through the mesh vtable: count at byte +16, buffer(i) at +20 (a
//      ref-counted buffer, null ends the iteration), material(i) at +24 and vertex attribute map(i) at +28.
//      The vtable slots are not named in the category, so the loop is omitted.
//   5. For each buffer whose material technique passes the flag test (scene manager byte +0x28a, technique
//      word +4 bit 16), calls driver setMaterial(material, attribute map) and drawMeshBuffer(buffer).
//      IVideoDriver has no declarations in this category, so these calls are omitted.
// The original's return value is the leftover r0 value, not a result, and is not reproduced (void here).
// Missing: the scene-manager and driver layout, the mesh vtable slots, the material technique table and
// the driver interface. The only deviation kept here: the counter is also incremented when the driver is
// null, which the original skips. The steps above are the functions to add once those are mapped.
void CMeshSceneNode::render(void* /*context*/) {
    if (!mesh) {
        return;
    }
    ++renderCount;
}

}  // namespace glitch::scene
