// Batch g1-game-world-ShadowSkinnedMeshSceneNode-844318-0: the eleven functions of ShadowSkinnedMeshSceneNode (the two
// bounding-box forwards, UpdateShadow, onRegisterSceneNode, renderMesh, render, the complete, base-object and deleting
// destructors, and the complete and base-object constructors). Declarations come from dh2/ShadowSkinnedMeshSceneNode.hpp.
//
// No body is defined in this file. The class cannot be defined yet: its base, BaseMeshSceneNode<glitch::collada::CSkinnedMeshSceneNode>,
// needs a complete glitch::collada::CSkinnedMeshSceneNode, and the tree has none. The same blocker holds for the
// CSkinnedMeshSceneNode functions of BaseMeshSceneNode and for SkinnedMeshSceneNode. The owner header therefore fails
// check_tu. This file includes nothing and defines nothing, because game-world/src/*.cpp is compiled into dh2_game
// (CMakeLists.txt, GLOB_RECURSE) and a definition here would break that build.
//
// The notes under each function record what the original does, in the engine names the tree already declares, so the
// bodies can be written in one pass once the base is complete:
//   - "scene manager" is the object at ISceneNode +0x110 (scene/ISceneNode.hpp), reached from the node as
//     this+0x110. Its +0x14 is the video driver (scene/CSceneManager.hpp). Its vptr+36 is registerNodeForRendering.
//   - "device" is the global at 0x99F73C. Its +0x1c is the scene manager (see PFRoom.hpp).
//   - "mesh" is the object at +0x134 (CMeshSceneNode's mesh). Its vtable offsets below are from its own vptr; their
//     names are not in the tree.
//   - vptr offsets in these notes are code offsets (decomp_vtable offset minus 0x0c).

// 0x35dc98 _ZNK26ShadowSkinnedMeshSceneNode14getBoundingBoxEv
// partial, no body. Original: returns the bounding box of the node at +0x1a0, through that node's vptr+48
// (ISceneNode::getBoundingBox, slot 12). Waits on the complete base.

// 0x35dcb4 _ZNK26ShadowSkinnedMeshSceneNode25getTransformedBoundingBoxEv
// partial, no body. Original: the same forward to the node at +0x1a0, through its vptr+52 (ISceneNode::getTransformedBoundingBox,
// slot 13). Waits on the complete base.

// 0x35f780 _ZN26ShadowSkinnedMeshSceneNode12UpdateShadowEv
// partial, no body. Original, in order:
//   1. Plane: normal (0, 0, 1) and d = -0.0, transformed by an identity matrix (no change).
//   2. S = glitch::core::buildShadowMatrix(light, plane), with light = the scene manager's vector3d at +0x400.
//   3. P = absolute position of the node at +0x1a0 (ISceneNode::getAbsolutePosition).
//   4. M = T(P.x + 10, P.y - 10, P.z + 10) * S * I * T(-P), with CMatrix4 operator* applied in that order.
//   5. this->setAbsoluteTransformation(M), a virtual call through vptr+60 (ISceneNode slot 15, takes CMatrix4&).
// Missing: the complete base, for the virtual call and ISceneNode's members. The scene manager's +0x400 is not declared.

// 0x35ffb8 _ZN26ShadowSkinnedMeshSceneNode19onRegisterSceneNodeEv
// partial, no body. Original:
//   strength = scene manager +0x418 (float), read first.
//   If the mesh at +0x134 is set: DebugSwitches::load(), then GetSwitch on an empty name. Only when that switch is true
//   and strength > 0: driver = scene manager +0x14. If driver is non-null, for each buffer index i below the mesh's count
//   (mesh vptr+16): buffer = mesh vptr+20 (i). If buffer is non-null, flags = mesh vptr+56 (0, driver, i), then
//     flags 4 or 16: scene manager vptr+36 = registerNodeForRendering(this, m_shadowMaterial, (void*)(i+1), pass 6,
//                    nullptr, INT_MAX);
//     flags 5:       mesh vptr+36 (mesh).
//   Returns 1 in every case.
// Missing: the complete base; names of mesh vptr+16, +20, +36 and +56 (IMesh.hpp declares onPrepareBufferForRendering and
// getMeshBuffer for other classes, but the slots of this mesh are not bound).

// 0x3607a0 _ZN26ShadowSkinnedMeshSceneNode10renderMeshEPv
// partial, no body. Original: index = context - 1. The render argument is a 1-based mesh buffer index carried as void*.
// The body runs only when the mesh at +0x134, the driver (scene manager +0x14) and context are all non-null. Then:
//   buffer = mesh vptr+20 (index). If buffer is non-null:
//     step = 0 when the mesh's flags word (+0x14) has bit index set; otherwise mesh vptr+56 (1, driver, index) & 4.
//     driver vptr+108 (slot 27) with (world state 1, this+0x24): the node's absolute transform is set on the driver.
//     driver setMaterial(m_shadowMaterial, unk_19c).
//     driver +0xe8 is cleared.
//     driver drawMeshBuffer(buffer).
//     If step is set: mesh vptr+60 (driver, index).
//     The map, material and buffer references are released.
// The original's return value is never used (the decompile types it as IReferenceCounted*); the header returns void.
// Missing: the complete base (+0x24 is ISceneNode's absolute transform), the driver's slot-27 name (not verified),
// driver +0xe8 (unnamed), and the names of the mesh's slots.

// 0x360948 _ZN26ShadowSkinnedMeshSceneNode6renderEPv
// partial, no body. Original: a DebugSwitches lookup on an empty name must be true, and the scene manager +0x418 strength
// must be >= 0.0001. Then:
//   UpdateShadow();
//   mesh vptr+32 (0, &m_shadowMaterial, &unk_19c): the mesh draws with the shadow material and the shadow map;
//   renderMesh(context);
//   mesh vptr+32 (0, begin of unk_180, begin of unk_18c): the mesh's own materials are restored.
// The strength gate is >= 0.0001 here and > 0 in onRegisterSceneNode. Waits on the complete base.

// 0x360b7c _ZN26ShadowSkinnedMeshSceneNodeD1Ev
// partial, no body. Original: stores the complete object's vptrs, releases the vertex map (+0x19c) and the shadow
// material (+0x198), destroys unk_18c and then unk_180 element by element, and runs the base destructors. The decompile
// calls glitch::collada::CMeshSceneNode::~CMeshSceneNode by name; that call is not checked against the base chain.
// The member releases run in the order 0x19c, 0x198, 0x18c, 0x180, the reverse of the header's declaration order. Once the
// base is complete, the compiler's member destruction gives the same sequence and the body can stay empty.

// 0x360c60 _ZN26ShadowSkinnedMeshSceneNodeD0Ev
// skipped: compiler-generated deleting destructor. It is the complete destructor (0x360b7c) followed by CustomFree(this).
// A delete expression does the same.

// 0x360c7c _ZN26ShadowSkinnedMeshSceneNodeD2Ev
// partial, no body of its own. This is the base-object destructor that takes the VTT. The compiler emits it from the
// complete destructor (0x360b7c). The original also writes the base vptrs from the VTT entries and calls
// CMeshSceneNode's destructor by name. Neither is reproduced or checked.

// 0x361b54 _ZN26ShadowSkinnedMeshSceneNodeC1EN5boost13intrusive_ptrIN6glitch7collada5IMeshEEEPKNS2_5scene10ISceneNodeE
// partial, no body. Original, in order:
//   1. The virtual base IReferenceCounted: vptr at +0x1a4, count 1 at +0x1a8.
//   2. The base constructor BaseMeshSceneNode<CSkinnedMeshSceneNode>(mesh) through the VTT.
//   3. The node at +0x1a0; +0x180 to +0x19c zeroed; the primary vptr.
//   4. Effect: CColladaDatabase on "gl_shadowprojection.bdae" (DefaultFactory), then constructEffect(database,
//      device +0x10, "GL_ShadowProjection-fx"). n = mesh vptr+16 (the material count).
//   5. Material "SH_SHADOW_SKINNED" from the effect, stored at +0x198 (its flag byte at +8 cleared). Vertex map from
//      material + 4, stored at +0x19c.
//   6. unk_180 is resized to n, element i = mesh vptr+24 (i). unk_18c is resized to n, element i = mesh vptr+28 (i).
//   7. The effect and the database are released.
// Missing: the complete base; device +0x10 (unnamed); the names of mesh vptr+24 and +28; the meaning of material + 4.

// 0x361f24 _ZN26ShadowSkinnedMeshSceneNodeC2EN5boost13intrusive_ptrIN6glitch7collada5IMeshEEEPKNS2_5scene10ISceneNodeE
// partial, no body of its own. This is the base-object constructor. It takes the VTT and the node (a4). It runs the steps of
// 0x361b54 but not the virtual base IReferenceCounted: it does not write +0x1a4 or +0x1a8. It calls the BaseMeshSceneNode
// constructor with a2+1 as the VTT, writes the vptrs from the VTT entries, then does steps 3 to 7 above.
