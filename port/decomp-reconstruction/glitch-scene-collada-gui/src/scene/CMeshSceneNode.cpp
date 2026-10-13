// Bodies of glitch::scene::IMeshSceneNode and glitch::scene::CMeshSceneNode (canonical classes: scene/CMeshSceneNode.hpp). This
// file replaces src/tb_scene-collada-gui-glitch_scene_CMeshSceneNode-0.cpp and the CMeshSceneNode bodies of src/scene_nodes.cpp
// and recon/slice_05.cpp (stand-in free functions).
#include "glitch-scene-collada-gui/scene/CMeshSceneNode.hpp"

#include "glitch-scene-collada-gui/scene/CSceneManager.hpp"

namespace glitch::scene {

// 0x35abc4 _ZN6glitch5scene14IMeshSceneNodeD1Ev (skipped; D0 0x35b3dc likewise)
IMeshSceneNode::~IMeshSceneNode() = default;

// 0x596cc8 _ZN6glitch5scene14IMeshSceneNode30setNodeHierarchyAsShadowCasterEb
// Sets or clears the shadow-caster bit (0x800) of the flags, then runs the ISceneNode version, which forwards to the children.
void IMeshSceneNode::setNodeHierarchyAsShadowCaster(bool enabled) {
    if (enabled) {
        flags |= ESNF_SHADOW_CASTER;
    } else {
        flags &= ~static_cast<std::uint32_t>(ESNF_SHADOW_CASTER);
    }
    ISceneNode::setNodeHierarchyAsShadowCaster(enabled);
}

// 0x585118 _ZN6glitch5scene14CMeshSceneNodeC2ERKN5boost13intrusive_ptrINS0_5IMeshEEEiRKNS_4core8vector3dIfEERKNS5_10quaternionES9_
// 0x58529c (the second listing of the same constructor)
// IMeshSceneNode(id, position, rotation, scale), the mesh stored (a use is added), then ISceneNode::setAutomaticCulling(2).
CMeshSceneNode::CMeshSceneNode(const boost::intrusive_ptr<IMesh>& meshValue, int nodeId, const core::vector3d<float>& nodePosition,
                               const core::quaternion& nodeRotation, const core::vector3d<float>& nodeScale)
    : IMeshSceneNode(nodeId, nodePosition, nodeRotation, nodeScale), mesh(meshValue) {
    setAutomaticCulling(2);
}

// 0x584c10 _ZN6glitch5scene14CMeshSceneNodeD1Ev (skipped; D2 0x584c94 and D0 0x584cb0 likewise)
CMeshSceneNode::~CMeshSceneNode() = default;

// 0x584b90 _ZNK6glitch5scene14CMeshSceneNode19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// PARTIAL: the ISceneNode attributes only (the mesh file name attribute is not reconstructed).
void CMeshSceneNode::serializeAttributes(io::IAttributes* attributes, io::SAttributeReadWriteOptions* options) const {
    ISceneNode::serializeAttributes(attributes, options);
}

// 0x584ed0 _ZN6glitch5scene14CMeshSceneNode21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// PARTIAL: the ISceneNode attributes only (the mesh is loaded by name in the original).
void CMeshSceneNode::deserializeAttributes(io::IAttributes* attributes, io::SAttributeReadWriteOptions* options) {
    ISceneNode::deserializeAttributes(attributes, options);
}

// 0x584d2c _ZN6glitch5scene14CMeshSceneNode19onRegisterSceneNodeEv
// With a mesh: clears the render counter, then walks the mesh buffers and counts how many materials have the transparent
// technique bit (0x10000 of the technique word) and how many do not. Registers the node with the scene manager (vptr+0x24,
// registerNodeForRendering, no material, sort order 0x7FFFFFFF) for the solid pass 4 when any solid material exists, for the
// transparent pass 8 when any transparent one does, and for the shadow pass 7 when the shadow-caster bit is set. Always
// returns 1. PARTIAL: the technique word comes from CMaterial::getTechnique and the renderer's technique table, which are
// glitch-video's, so every material is counted as solid here.
int CMeshSceneNode::onRegisterSceneNode() {
    if (mesh && sceneManager != nullptr) {
        renderCount = 0;
        const boost::intrusive_ptr<video::CMaterial> noMaterial;
        if (mesh->getMeshBufferCount() != 0) {
            sceneManager->registerNodeForRendering(this, noMaterial, nullptr, 4, nullptr, 0x7FFFFFFF);
        }
        if ((flags & ESNF_SHADOW_CASTER) != 0) {
            sceneManager->registerNodeForRendering(this, noMaterial, nullptr, 7, nullptr, 0x7FFFFFFF);
        }
    }
    return 1;
}

// 0x585370 _ZN6glitch5scene14CMeshSceneNode6renderEPv
// PARTIAL. The original: returns when the mesh or the scene manager's driver is null; increments the render counter; sets the
// driver's world transform to the absolute matrix (driver vtable +108); then for every mesh buffer whose material passes the
// technique test, sets the material and draws the buffer. The driver is glitch-video's and is not called, so only the counter
// step is done (the original skips it when the driver is null).
void CMeshSceneNode::render(void* /*renderContext*/) {
    if (!mesh) {
        return;
    }
    ++renderCount;
}

// 0x584ac0 _ZNK6glitch5scene14CMeshSceneNode14getBoundingBoxEv (skipped: a thunk to the mesh's getBoundingBox)
const core::aabbox3d<float>& CMeshSceneNode::getBoundingBox() const {
    return mesh->getBoundingBox();
}

// 0x584adc _ZNK6glitch5scene14CMeshSceneNode11getMaterialEj
// The mesh's material (vptr+0x18 on the mesh) when the index is below the buffer count, else a null handle.
boost::intrusive_ptr<video::CMaterial> CMeshSceneNode::getMaterial(std::uint32_t index) const {
    if (mesh && index < mesh->getMeshBufferCount()) {
        return mesh->getMaterial(index);
    }
    return boost::intrusive_ptr<video::CMaterial>();
}

// 0x584b44 _ZNK6glitch5scene14CMeshSceneNode16getMaterialCountEv
std::uint32_t CMeshSceneNode::getMaterialCount() const {
    return mesh ? mesh->getMeshBufferCount() : 0;
}

// 0x584a98 _ZNK6glitch5scene14CMeshSceneNode7getTypeEv
E_SCENE_NODE_TYPE CMeshSceneNode::getType() const {
    return ESNT_MESH;
}

// 0x58521c _ZN6glitch5scene14CMeshSceneNode5cloneEv
// A new node (operator new(0x140)) over a clone of the mesh (mesh vptr+0x0c) with the base members copied (cloneMembers).
ISceneNode* CMeshSceneNode::clone() {
    auto* copy = new CMeshSceneNode(mesh->clone(), id, position, rotation, scale);
    copy->cloneMembers(this);
    return copy;
}

// 0x5850ec _ZN6glitch5scene14CMeshSceneNode7setMeshERKN5boost13intrusive_ptrINS0_5IMeshEEE
// Replaces the mesh (the new one gains a use, the old one loses its).
void CMeshSceneNode::setMesh(const boost::intrusive_ptr<IMesh>& newMesh) {
    mesh = newMesh;
}

// 0x584aa4 _ZNK6glitch5scene14CMeshSceneNode7getMeshEv
boost::intrusive_ptr<IMesh> CMeshSceneNode::getMesh() const {
    return mesh;
}

}  // namespace glitch::scene
