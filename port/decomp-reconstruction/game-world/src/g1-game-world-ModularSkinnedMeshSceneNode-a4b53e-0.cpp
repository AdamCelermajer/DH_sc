// Batch g1-game-world-ModularSkinnedMeshSceneNode-a4b53e-0: the eight ModularSkinnedMeshSceneNode functions
// (two methods, the complete and base-object constructors, and the three destructors). Declarations come from
// dh2/ModularSkinnedMeshSceneNode.hpp.
//
// Partial: onRegisterSceneNode, updateAbsolutePosition, prepareSkinForRendering, the complete constructor (C1) and the
// complete destructor (D1). Each comment names what is missing. The VTT-taking base-object forms (C2 0x35af88 and
// D2 0x35babc) have no body of their own: the compiler emits them from the complete forms. Skipped: the deleting
// destructor (D0 0x35baa0), which is the complete destructor followed by CustomFree.
//
// Engine declarations that these bodies need are not in this tree (layout and declaration requests in the ledger notes):
//   - glitch::collada::CModularSkinnedMesh (the mesh at +0x134) declares none of onAnimate, getMeshBuffer or
//     onPrepareBufferForRendering. The vtable (tools/decomp_vtable.py) gives them at +0x34, +0x14 and +0x38;
//   - glitch::collada::CMeshSceneNode has no onRegisterSceneNode declaration in collada_scene_accessors.hpp;
//   - CModularSkinnedMeshSceneNode declares no member at +0x110 (the skin owner used by 0x35a8e8 and 0x35acb4).

#include "dh2/ModularSkinnedMeshSceneNode.hpp"

#include "glitch-scene-collada-gui/collada/IMesh.hpp"
#include "glitch-scene-collada-gui/collada_scene_accessors.hpp"
#include "glitch-scene-collada-gui/modular_skinned_mesh.hpp"

// The base's constructor and destructor are explicit specializations defined in
// g1-game-world-BaseMeshSceneNode-fb9ec3-0.cpp. They are declared here so this file refers to them as specializations.
template <>
BaseMeshSceneNode<glitch::collada::CModularSkinnedMeshSceneNode>::BaseMeshSceneNode(
        const boost::intrusive_ptr<glitch::collada::IMesh>& mesh);
template <>
BaseMeshSceneNode<glitch::collada::CModularSkinnedMeshSceneNode>::~BaseMeshSceneNode();

// 0x35a8e8 _ZN27ModularSkinnedMeshSceneNode19onRegisterSceneNodeEv
// partial: the original calls the virtual at vtable offset +0x88 of the object at this+0x110, with that object and
// this+0x180 as arguments, and then tail-calls glitch::collada::CMeshSceneNode::onRegisterSceneNode(this), returning
// its result. Neither call is bound: the object at +0x110 has no declared type in this tree, and CMeshSceneNode's
// onRegisterSceneNode is not declared in collada_scene_accessors.hpp. The body returns 0 as a stub.
int ModularSkinnedMeshSceneNode::onRegisterSceneNode() {
    return 0;
}

// 0x35a940 _ZN27ModularSkinnedMeshSceneNode22updateAbsolutePositionEb
// partial: the original calls glitch::collada::CModularSkinnedMesh::onAnimate(1) on the mesh at +0x134 (vtable
// +0x34), then tail-calls glitch::scene::ISceneNode::updateAbsolutePosition(this, includeChildren). onAnimate is not
// declared on CModularSkinnedMesh, and the node has no ISceneNode base in this tree, so neither call is bound. The body
// is empty.
void ModularSkinnedMeshSceneNode::updateAbsolutePosition(bool includeChildren) {
    (void)includeChildren;
}

// 0x35acb4 _ZN27ModularSkinnedMeshSceneNode23prepareSkinForRenderingEv
// partial. The original does nothing unless the mesh at +0x134 is set and the word at (this+0x110)+0x14 (the driver,
// v3) is non-null. It then loops i over getMeshBufferCount() (vtable +0x10). For each i whose getMeshBuffer(i)
// (vtable +0x14, returns an intrusive_ptr by value) is non-null, the temporary is released, getMaterial(i) (vtable +0x18,
// intrusive_ptr result) is held, and onPrepareBufferForRendering(0, driver, i) (vtable +0x38) is called. The body is
// empty. Missing: the type of the object at +0x110, the getMeshBuffer and onPrepareBufferForRendering declarations
// on CModularSkinnedMesh, and getMaterial's intrusive_ptr return (the stand-in returns SCountedRef*&).
void ModularSkinnedMeshSceneNode::prepareSkinForRendering() {}

// 0x35af00 _ZN27ModularSkinnedMeshSceneNodeC1ERKN5boost13intrusive_ptrIN6glitch7collada5IMeshEEE
// partial. The original stores 1 at +0x188 and the vptr of a glitch::IReferenceCounted virtual base at +0x184 before
// it calls the BaseMeshSceneNode base-object constructor (through the VTT). It then stores the complete object's vptrs.
// The host base constructor is partial (it drops the mesh, see g1-game-world-BaseMeshSceneNode-fb9ec3-0.cpp,
// 0x35ae5c), and the vptr stores are emitted by the compiler from the class hierarchy. The +0x188 store is the only
// store the body reproduces: unk_188 is set after the base constructor, which gives the same final value.
ModularSkinnedMeshSceneNode::ModularSkinnedMeshSceneNode(const boost::intrusive_ptr<glitch::collada::IMesh>& mesh)
    : BaseMeshSceneNode<glitch::collada::CModularSkinnedMeshSceneNode>(mesh) {
    unk_188 = 1;
}

// 0x35af88 _ZN27ModularSkinnedMeshSceneNodeC2ERKN5boost13intrusive_ptrIN6glitch7collada5IMeshEEE
// partial, no body of its own. This is the base-object form that takes the VTT (a2) and the mesh. The compiler emits it
// from the constructor above. Its VTT-indexed vptr stores (VTT[19] and VTT[20], at the virtual-base offsets of the
// vtable) are not checked on the host.

// 0x35ba4c _ZN27ModularSkinnedMeshSceneNodeD1Ev
// partial. The original stores the complete object's vptrs (primary, +0x180 and +0x184) and calls the BaseMeshSceneNode
// base-object destructor with the VTT. The host destructor has no user code, so the compiler emits the base chain. The
// stores of the +0x180 and +0x184 vptrs are not reproduced: those virtual bases are not declared on the host class.
ModularSkinnedMeshSceneNode::~ModularSkinnedMeshSceneNode() {}

// 0x35baa0 _ZN27ModularSkinnedMeshSceneNodeD0Ev
// skipped: compiler-generated deleting destructor. It is the complete destructor (0x35ba4c) followed by CustomFree(this).
// A delete expression does the same.

// 0x35babc _ZN27ModularSkinnedMeshSceneNodeD2Ev
// partial, no body of its own. This is the base-object destructor that takes the VTT (a2). The compiler emits it from the
// destructor above. Its VTT-indexed stores and the call to BaseMeshSceneNode's base-object destructor with a2+1 are not
// checked on the host.
