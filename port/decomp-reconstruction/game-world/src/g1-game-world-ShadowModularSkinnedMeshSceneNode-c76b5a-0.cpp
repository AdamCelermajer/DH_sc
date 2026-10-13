// Batch g1-game-world-ShadowModularSkinnedMeshSceneNode-c76b5a-0: the twelve ShadowModularSkinnedMeshSceneNode functions
// (two bounding-box forwarders, UpdateShadow, onRegisterSceneNode, renderMesh, render, regenerateShadowMaterial, the two
// constructors and the three destructors). Declarations come from dh2/ShadowModularSkinnedMeshSceneNode.hpp.
//
// Reconstructed: getBoundingBox, getTransformedBoundingBox.
// Partial: UpdateShadow, onRegisterSceneNode, renderMesh, render, regenerateShadowMaterial, the complete constructor and
// the complete destructor. Each comment names what is missing. The VTT-taking base-object forms (C2 0x361ab0 and D2
// 0x360aa8) have no body of their own: the compiler emits them from the complete forms. Skipped: the deleting destructor
// (D0 0x3602cc), which is the complete destructor followed by CustomFree.
//
// Engine declarations that these bodies need are not in this tree (layout requests in the ledger notes):
//   - glitch::IDevice has no member at +0x10 (video driver) or +0x1c (scene manager, the SceneManager of dh2/SceneManager.hpp);
//   - SceneManager's light set (m_lightSetManager, a byte array) has no typed words at +0x400 (light position) and +0x418 (the shadow float);
//   - glitch::collada::CModularSkinnedMeshSceneNode has no member at +0x110 (the scene manager, ISceneNode's +0x110), is not
//     a scene::ISceneNode, and its mesh at +0x134 (CModularSkinnedMesh stand-in) declares none of the IMesh virtuals;
//   - the mesh and module virtuals (getMeshBuffer, getMaterial, onPrepareBufferForRendering, ...) are not declared.

#include "dh2/ShadowModularSkinnedMeshSceneNode.hpp"

#include "glitch-scene-collada-gui/scene/ISceneNode.hpp"

// The base's constructor and destructor are explicit specializations defined in
// g1-game-world-BaseMeshSceneNode-fb9ec3-0.cpp. They are declared here so this file refers to them as specializations.
template <>
BaseMeshSceneNode<glitch::collada::CModularSkinnedMeshSceneNode>::BaseMeshSceneNode(
        const boost::intrusive_ptr<glitch::collada::IMesh>& mesh);
template <>
BaseMeshSceneNode<glitch::collada::CModularSkinnedMeshSceneNode>::~BaseMeshSceneNode();

// 0x35dcd0 _ZNK33ShadowModularSkinnedMeshSceneNode14getBoundingBoxEv
// Reconstructed. Forwards to node1 (+0x1c8): the original calls the virtual at vptr+0x30 (ISceneNode::getBoundingBox)
// on it and returns the reference.
const glitch::core::aabbox3d<float>& ShadowModularSkinnedMeshSceneNode::getBoundingBox() const {
    return unk_1c8->getBoundingBox();
}

// 0x35dcec _ZNK33ShadowModularSkinnedMeshSceneNode25getTransformedBoundingBoxEv
// Reconstructed. The original is the same forward as 0x35dcd0: it calls vptr+0x30 on node1 (getBoundingBox), not
// vptr+0x34 (getTransformedBoundingBox), and returns that result. Reproduced as found.
const glitch::core::aabbox3d<float>& ShadowModularSkinnedMeshSceneNode::getTransformedBoundingBox() const {
    return unk_1c8->getBoundingBox();
}

// 0x35f348 _ZN33ShadowModularSkinnedMeshSceneNode12UpdateShadowEv
// partial, empty body. The original builds the shadow projection and hands it to this node:
//   light  = scene manager +0x400 (the float[3] at LightSetManager +0x16c)
//   plane  = (0, 0, 1, -0.0), transformed by an identity CMatrix4 (transformPlane, bound in the engine)
//   shadow = buildShadowMatrix(light, plane)
//   pos    = node2->getAbsolutePosition() (node2 at +0x1cc; bound: ISceneNode::getAbsolutePosition is declared)
//   result = T(pos + (10, -10, 10)) * shadow * I * T(-pos)
//   this->setAbsoluteTransformation(result)  (ISceneNode vptr+0x3c, virtual, not bound)
// Missing: the scene manager pointer (glitch::IDevice +0x1c, read from Singleton<Application>::s_inst.m_device, whose
// member at +0x10 is dword_99F73C); the light position and the shadow float in SceneManager's m_lightSetManager (a byte
// array); and setAbsoluteTransformation on this (the base is not an ISceneNode in this tree).
void ShadowModularSkinnedMeshSceneNode::UpdateShadow() {}

// 0x35f530 _ZN33ShadowModularSkinnedMeshSceneNode19onRegisterSceneNodeEv
// partial. Calls UpdateShadow and returns 1, which the original does on every path. Not reconstructed: when the mesh at
// +0x134 is set, DebugSwitches::GetSwitch("RENDERING_EnableShadowProjection") is true, the scene manager float at +0x418 is
// > 0, and the driver (scene manager +0x14) is non-null, it walks the mesh buffers i. For each i whose getMeshBuffer(i)
// (mesh vptr+0x14) is non-null, it takes unk_1b0[unk_180[i]] with one extra use and calls mesh vptr+0x38
// (onPrepareBufferForRendering, step 0, driver, index i). A result of 4 or 16 calls the scene manager's vptr+0x24 with
// (this, &material, i+1, 6, 0, 0x7fffffff). A result of 5 calls mesh vptr+0x24 with no arguments. Missing: the scene manager
// and driver (see the file header), the mesh virtuals, the scene manager's vptr+0x24 (not declared on SceneManager), and
// the material handle type (the stand-in has no intrusive_ptr<CMaterial> return on the mesh).
int ShadowModularSkinnedMeshSceneNode::onRegisterSceneNode() {
    UpdateShadow();
    return 1;
}

// 0x3602e8 _ZN33ShadowModularSkinnedMeshSceneNode10renderMeshEPv
// partial, empty body. The original takes a 1-based buffer index a2 (the index used is a2-1) and returns a value that
// render ignores. When the mesh, the driver (scene manager +0x14) and a2 are all set, it takes getMeshBuffer(index) (mesh
// vptr+0x14) and then: tests bit (index & 31) of the mesh word at +0x14 and, when it is clear, calls mesh vptr+0x38
// (onPrepareBufferForRendering, step 1, driver, index) and keeps its & 4; calls the driver's setTransform (vptr+0x6c, state 1,
// the node's absolute matrix at +0x24); sets the material unk_1b0[unk_180[index]] and vertex map unk_1bc[unk_180[index]] on
// the driver (IVideoDriver::setMaterial); clears the driver word at +0xe8; draws the buffer (IVideoDriver::drawMeshBuffer);
// calls mesh vptr+0x3c (releaseProcessBuffer, driver, index) when the flag was set; and drops its handles. Missing: the mesh
// virtuals and the driver (see the file header).
void ShadowModularSkinnedMeshSceneNode::renderMesh(void* context) {
    (void)context;
}

// 0x3604a8 _ZN33ShadowModularSkinnedMeshSceneNode6renderEPv
// partial, empty body. The original runs only when DebugSwitches::GetSwitch("RENDERING_EnableShadowProjection") is set
// and the scene manager float at +0x418 is >= 0.0001. It then calls UpdateShadow and:
//   - for each mesh buffer i, mesh vptr+0x20 (setMaterial) with (i, unk_1b0[unk_180[i]], unk_1bc[unk_180[i]]);
//   - for each current module j (currentModules at mesh +0x24), module vptr+0x20 with (0, unk_1b0[unk_18c[j]], unk_1bc[unk_18c[j]]);
//   - renderMesh(context), called once;
//   - for each mesh buffer i, mesh vptr+0x20 with (i, unk_198[unk_180[i]], unk_1a4[unk_180[i]]);
//   - for each module j, module vptr+0x20 with (0, unk_198[unk_18c[j]], unk_1a4[unk_18c[j]]);
//   - and it destroys two local vectors that are always empty.
// Missing: the gate's scene manager (see the file header), the mesh and module virtuals, and the module list type.
void ShadowModularSkinnedMeshSceneNode::render(void* context) {
    (void)context;
}

// 0x3601fc _ZN33ShadowModularSkinnedMeshSceneNodeD1Ev
// partial. The compiler destroys the six vectors in reverse declaration order (+0x1bc, +0x1b0, +0x1a4, +0x198, +0x18c,
// +0x180), the same order as the original, and then the base destructor. Not reproduced: the primary vptr store and the
// IRC virtual-base vptr store at +0x1d0 (the virtual base is pad_1d0), which have no host counterpart. The original
// releases each CMaterial and CMaterialVertexAttributeMap handle in those vectors. The material vectors do that through
// intrusive_ptr. The vertex-map vectors (+0x1a4, +0x1bc) are raw pointers in the header, so their counts are not released.
ShadowModularSkinnedMeshSceneNode::~ShadowModularSkinnedMeshSceneNode() {}

// 0x3602cc _ZN33ShadowModularSkinnedMeshSceneNodeD0Ev
// skipped: compiler-generated deleting destructor. It is the complete destructor (0x3601fc) followed by CustomFree(this).
// A delete expression does the same.

// 0x360aa8 _ZN33ShadowModularSkinnedMeshSceneNodeD2Ev
// partial, no body of its own. This is the base-object destructor that takes the VTT (a2). The compiler emits it from the
// complete destructor above. Its VTT-indexed stores and the call to BaseMeshSceneNode's base-object destructor with a2+1
// are not checked on the host.

// 0x361298 _ZN33ShadowModularSkinnedMeshSceneNode24regenerateShadowMaterialEv
// partial, empty body. The original does this:
//   1. opens gl_shadowprojection.bdae (CColladaDatabase with CColladaDatabase::DefaultFactory) and constructs the effect
//      "GL_ShadowProjection-fx" on the driver at IDevice +0x10 with root 0, keeping its CMaterialRenderer;
//   2. releases the entries of unk_198 and unk_1a4, then resizes unk_198, unk_1a4 and unk_180 to the mesh buffer count
//      (mesh vptr+0x10);
//   3. for each buffer i, finds the entry k of unk_198 that holds the mesh's getMaterial(i) (vptr+0x18). When the entry is
//      empty it stores that material in it, stores getMaterialVertexAttributeMap(i) (vptr+0x1c) in unk_1a4[i], and sets
//      unk_180[i] = k;
//   4. resizes unk_1b0 and unk_1bc to the size of unk_198, and unk_18c to the module count (currentModules at mesh +0x24).
//      For each module it records in unk_18c the entry of unk_198 whose material equals the module's getMaterial(0) (module
//      vptr+0x18);
//   5. for each entry of unk_198 it allocates a material (CMaterial::allocate with the renderer, name
//      "SH_SHADOW_SKINNED_MODULAR", count 0), sets its m_technique (+0x08) to 3, and, when unk_1a4 has the entry, allocates
//      a CMaterialVertexAttributeMap from it (CMaterialVertexAttributeMap::allocate, argument material+4). The map is kept
//      as a raw pointer in unk_1bc (see the header).
// Missing: the driver at IDevice +0x10 (layout request); the mesh virtuals (getMeshBufferCount vptr+0x10, getMaterial vptr+0x18,
// getMaterialVertexAttributeMap vptr+0x1c) and the module virtuals, since the stand-in CModularSkinnedMesh and ISkinnedMesh
// declare none; the CColladaDatabase::constructEffect(driver, name, root) signature, which returns a raw pointer here where
// the original writes an intrusive_ptr through its first argument; and the meaning of the material+4 argument of
// CMaterialVertexAttributeMap::allocate, which is not identified.
void ShadowModularSkinnedMeshSceneNode::regenerateShadowMaterial() {}

// 0x3619c8 _ZN33ShadowModularSkinnedMeshSceneNodeC1EN5boost13intrusive_ptrIN6glitch7collada5IMeshEEEPKNS2_5scene10ISceneNodeES9_
// partial. Sets node1 at +0x1c8 and node2 at +0x1cc, zeroes the six vectors (the compiler does this for the members), calls
// the BaseMeshSceneNode constructor with the mesh, and then calls regenerateShadowMaterial. Not reproduced: the IRC
// virtual-base vptr (off_95B720 at +0x1d0) and its count (1 at +0x1d4), which have no host counterpart (pad_1d0). The
// base constructor is partial (it drops the mesh, see g1-game-world-BaseMeshSceneNode-fb9ec3-0.cpp), and
// regenerateShadowMaterial is partial, so the object does no shadow work yet.
ShadowModularSkinnedMeshSceneNode::ShadowModularSkinnedMeshSceneNode(
        boost::intrusive_ptr<glitch::collada::IMesh> mesh,
        const glitch::scene::ISceneNode* node1,
        const glitch::scene::ISceneNode* node2)
    : BaseMeshSceneNode<glitch::collada::CModularSkinnedMeshSceneNode>(mesh),
      unk_1c8(node1),
      unk_1cc(node2) {
    regenerateShadowMaterial();
}

// 0x361ab0 _ZN33ShadowModularSkinnedMeshSceneNodeC2EN5boost13intrusive_ptrIN6glitch7collada5IMeshEEEPKNS2_5scene10ISceneNodeES9_
// partial, no body of its own. This is the base-object constructor that takes the VTT (a2). The compiler emits it from the
// complete constructor above. Its VTT-indexed vptr stores (the IRC virtual base) and the call to BaseMeshSceneNode's
// base-object constructor with a2+1 are not checked on the host.
