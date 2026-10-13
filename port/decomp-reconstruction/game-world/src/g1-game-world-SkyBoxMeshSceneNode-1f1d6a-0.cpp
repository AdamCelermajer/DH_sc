// Batch g1-game-world-SkyBoxMeshSceneNode-1f1d6a-0: the eight SkyBoxMeshSceneNode functions (getType, render,
// onRegisterSceneNode, the complete and base-object constructors and the three destructors). Declarations come from
// dh2/SkyBoxMeshSceneNode.hpp.
//
// Reconstructed: getType (0x36233c) and the complete destructor (0x362718), which drops the node reference.
// Partial: render (0x362398), onRegisterSceneNode (0x3625d0) and the complete constructor (0x362810). Each comment names
// what is missing. The base-object forms (C2 0x36299c, D2 0x3627a8) have no body of their own: the compiler emits them
// from the complete forms. Skipped: the deleting destructor (D0 0x36278c), which is the complete destructor followed by
// CustomFree.
//
// The base is the accessor stub glitch::collada::CMeshSceneNode. It has no ISceneNode base, no constructor with the
// binary's arguments, and its member at +0x134 is typed SNamedMeshData* while the original calls IMesh slots on it.
// That is why the mesh, the scene manager and the absolute transform are not bound here. See the ledger notes.

#include "dh2/SkyBoxMeshSceneNode.hpp"

#include "glitch-core-io/core/reference_counted.hpp"
#include "glitch-scene-collada-gui/scene/CMeshSceneNode.hpp"

// 0x36233c _ZNK19SkyBoxMeshSceneNode7getTypeEv
int SkyBoxMeshSceneNode::getType() const {
    return 1601792883;
}

// 0x362398 _ZN19SkyBoxMeshSceneNode6renderEPv
// partial. The render gate (+0x138, m_renderEnabled) is bound: the original does nothing unless it is set. Not bound:
//   1. scene = this+0x110 (ISceneNode::sceneManager), driver = scene+0x14, camera = scene+0xe4 (the active camera).
//      The stub base has no ISceneNode, so none of these are declared.
//   2. With the mesh at +0x134, the driver and the camera set, the absolute transform (+0x24) gets the camera's
//      absolute position in its translation words (+0x54..+0x5c), the diagonal words (+0x24, +0x38, +0x4c) get twice
//      the camera far value (vptr+0x11c here, see the ledger notes), and the flag byte at +0x64 is cleared.
//   3. mesh->setTransform(driver, absolute transform) (IMesh vptr+0x44).
//   4. For a non-null context c, index = c-1: getMeshBuffer(index) (vptr+0x14). When it is non-null, and bit index of
//      the mesh's unk_14 word is clear, onPrepareBufferForRendering(1, driver, index) (vptr+0x38). Then getMaterial
//      (vptr+0x18), getMaterialVertexAttributeMap (vptr+0x1c), driver setMaterial, driver vtable index 67 with 0, the
//      driver's drawMeshBuffer, and releaseProcessBuffer (vptr+0x3c) when the onPrepare result has bit 4 set.
void SkyBoxMeshSceneNode::render(void* context) {
    (void)context;
    if (!m_renderEnabled) {
        return;
    }
}

// 0x3625d0 _ZN19SkyBoxMeshSceneNode19onRegisterSceneNodeEv
// partial. The original always returns 1. Not bound: when the mesh at +0x134 and the driver (scene+0x14, scene = +0x110)
// are both set, it loops over the mesh buffers (getMeshBufferCount, vptr+0x10). For each non-null getMeshBuffer(i)
// (vptr+0x14) it releases that reference, takes getMaterial(i) (vptr+0x18) and calls onPrepareBufferForRendering(0,
// driver, i) (vptr+0x38). Result 4 or 16 registers the node with the scene manager (vptr+0x24, seven arguments: the
// manager, this, the material, i+1, 2, 0, 0x7fffffff). Result 5 calls the mesh's vptr+0x24 with no arguments (the
// scene::IMesh slot named getBoundingBox; the result is discarded in the pseudocode).
int SkyBoxMeshSceneNode::onRegisterSceneNode() {
    return 1;
}

// 0x362718 _ZN19SkyBoxMeshSceneNodeD1Ev
// Drops the reference that the constructor took on the node at +0x13c, through the node's IReferenceCounted base, and
// clears the pointer. The vptr and virtual-base stores, and the call to the stub base's destructor, are emitted by the
// compiler.
SkyBoxMeshSceneNode::~SkyBoxMeshSceneNode() {
    if (unk_13c != nullptr) {
        static_cast<glitch::IReferenceCounted*>(unk_13c)->drop();
        unk_13c = nullptr;
    }
}

// 0x36278c _ZN19SkyBoxMeshSceneNodeD0Ev
// skipped: compiler-generated deleting destructor. It is the complete destructor (0x362718) followed by CustomFree(this).
// A delete expression does the same.

// 0x3627a8 _ZN19SkyBoxMeshSceneNodeD2Ev
// partial, no body of its own: the VTT-taking base-object destructor. The compiler emits it from the complete destructor
// (0x362718). Its VTT-indexed vptr and virtual-base stores are not checked on the host.

// 0x362810 _ZN19SkyBoxMeshSceneNodeC1ERKN5boost13intrusive_ptrIN6glitch7collada5IMeshEEEPNS2_5scene14IMeshSceneNodeE
// partial. Bound: the render gate (+0x138) is set to 1, the node is stored at +0x13c and its reference count is raised
// by one. Not bound: (a) the base constructor call CMeshSceneNode(mesh, node 0, id -1, position 0, identity rotation,
// scale 1). The stub base has no such constructor, so the mesh is dropped here. (b) setAutomaticCulling(0), which needs
// the ISceneNode base. (c) The mesh step: getMaterial(0) on the mesh at +0x134, then the renderer flag step. The step
// clears bit 0x100000 in the word at pass+0x04, and sets the byte at pass+0x30 to 1 when that bit was set. The pass is
// the first technique's passes pointer, reached through the renderer's technique table at +0x18.
SkyBoxMeshSceneNode::SkyBoxMeshSceneNode(const boost::intrusive_ptr<glitch::collada::IMesh>& mesh,
                                         glitch::scene::IMeshSceneNode* node)
    : glitch::collada::CMeshSceneNode() {
    (void)mesh;
    m_renderEnabled = true;
    unk_13c = node;
    if (unk_13c != nullptr) {
        ++static_cast<glitch::IReferenceCounted*>(unk_13c)->referenceCounter;
    }
}

// 0x36299c _ZN19SkyBoxMeshSceneNodeC2ERKN5boost13intrusive_ptrIN6glitch7collada5IMeshEEEPNS2_5scene14IMeshSceneNodeE
// partial, no body of its own: the VTT-taking base-object constructor. The compiler emits it from the complete constructor
// (0x362810). Its VTT-indexed vptr and virtual-base stores are not checked on the host.
