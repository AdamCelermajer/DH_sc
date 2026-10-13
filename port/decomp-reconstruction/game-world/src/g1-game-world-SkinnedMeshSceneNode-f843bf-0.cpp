// Batch g1-game-world-SkinnedMeshSceneNode-f843bf-0: the eight SkinnedMeshSceneNode functions (two methods,
// two constructors and three destructors). Declarations come from dh2/SkinnedMeshSceneNode.hpp.
//
// No body is written in this file. Each function reads or writes the skinned node's base part, and the base
// glitch::collada::CSkinnedMeshSceneNode is not complete in this tree. BaseMeshSceneNode<CSkinnedMeshSceneNode>
// therefore cannot be instantiated, so dh2/SkinnedMeshSceneNode.hpp does not compile, and a member function
// cannot be defined. The offsets the bodies need (+0x110 and +0x134) lie inside that base and have no declared
// member. Each record below gives the original behaviour and what is missing. This file has no #include, so it
// compiles on its own, the same way ColladaFactory.cpp keeps its createSkinNode stub free of the header.
//
// Waits on: a complete glitch::collada::CSkinnedMeshSceneNode (layout requests in the ledger notes).

// 0x35a914 SkinnedMeshSceneNode::onRegisterSceneNode()
// partial, not rebuilt. Original: calls the virtual at vtable +0x88 of the object at +0x110, passing that object and
// this+0x180 (the address of the subobject whose vtable holds prepareSkinForRendering), discards the result, then
// returns glitch::collada::CMeshSceneNode::onRegisterSceneNode(this). Missing: the complete base, the type of the
// object at +0x110 (layout request), and a declaration of CMeshSceneNode::onRegisterSceneNode (the collada stand-in
// in collada_scene_accessors.hpp has none).

// 0x35a974 SkinnedMeshSceneNode::updateAbsolutePosition(bool)
// partial, not rebuilt. Original: calls the vtable +0x34 slot of the mesh at +0x134 with argument 1 (IMesh::onAnimate,
// declared in collada/IMesh.hpp), then returns glitch::scene::ISceneNode::updateAbsolutePosition(this, includeChildren).
// Missing: the complete base and the +0x134 member (layout request). The ISceneNode call is not bound to this class.

// 0x35ad8c SkinnedMeshSceneNode::prepareSkinForRendering()
// partial, not rebuilt. Virtual of the +0x180 subobject; the thunk at vtable +0x114 passes the full object. Original:
// does nothing when the mesh at +0x134 is null. Reads driver = the word at +0x14 of the object at +0x110, with no null
// check on that object, and does nothing when driver is null. Otherwise n = mesh->getMeshBufferCount() (IMesh slot
// +0x10). For i in [0, n): buf = mesh->getMeshBuffer(i) (+0x14, returns an intrusive_ptr); when buf is non-null it is
// released, mesh->getMaterial(i) (+0x18) is held, mesh->onPrepareBufferForRendering(0, driver, i) (+0x38) is called,
// and the material is released. The leftover return register is not read by any caller, so the host form is void.
// Missing: the complete base and the +0x110 and +0x134 members (layout requests). The four IMesh slots are declared
// (scene/IMesh.hpp and collada/IMesh.hpp), so only those two members are unbound.

// 0x35b0a0 SkinnedMeshSceneNode::SkinnedMeshSceneNode(boost::intrusive_ptr<glitch::collada::IMesh> const&) (C1, complete)
// partial, not rebuilt. Original, through the VTT: before the base call it stores 1 at +0x188 and the
// glitch::IReferenceCounted address point at +0x184, plus the intermediate vptrs. It then calls the
// BaseMeshSceneNode<CSkinnedMeshSceneNode> base-object constructor with the mesh. Afterwards it stores the complete
// object's vptrs (primary, +0x180 and +0x184). Missing: the complete base. BaseMeshSceneNode<CSkinnedMeshSceneNode>
// cannot be instantiated, and its constructor 0x35affc is partial for the same reason. The vptr stores are emitted
// by the compiler from the hierarchy and are not written here.

// 0x35b128 SkinnedMeshSceneNode::SkinnedMeshSceneNode(boost::intrusive_ptr<glitch::collada::IMesh> const&) (C2, VTT form)
// partial, no body of its own. The compiler emits this base-object constructor from the complete form 0x35b0a0.
// Original: calls the base-object constructor of BaseMeshSceneNode<CSkinnedMeshSceneNode> with VTT+4 and the mesh,
// then stores the vptrs and virtual-base offsets taken from the VTT entries. Not checked on the host; waits on the
// complete base.

// 0x35b860 SkinnedMeshSceneNode::~SkinnedMeshSceneNode() (D1, complete)
// partial, empty body. Original: stores the vptrs of the primary, +0x180 and +0x184 parts, stores the virtual-base
// offsets through the vtable, then calls glitch::collada::CMeshSceneNode::~CMeshSceneNode(this) directly. The
// binary's base chain therefore reaches CMeshSceneNode without a CSkinnedMeshSceneNode destructor. The compiler
// emits the host chain from the declared hierarchy, so the body is empty and the stores are not checked.
// Missing: the complete base.

// 0x35b8f4 SkinnedMeshSceneNode::~SkinnedMeshSceneNode() (D0, deleting)
// skipped: compiler-generated deleting destructor. It is the complete destructor 0x35b860 followed by CustomFree(this).
// A delete expression does the same.

// 0x35b910 SkinnedMeshSceneNode::~SkinnedMeshSceneNode() (D2, VTT form)
// partial, no body of its own. The compiler emits the VTT-taking base-object destructor from 0x35b860. Original:
// stores the primary, +0x180 and +0x184 vptrs and the virtual-base offsets from VTT entries 0, 1 and 2 (the primary
// and two virtual-base forms), then calls glitch::collada::CMeshSceneNode::~CMeshSceneNode(this). Not checked on the
// host; waits on the complete base.
