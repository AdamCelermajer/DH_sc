// Batch g1-game-app-GSViewer-081209-0 (GSViewer, 9 entries: the C2 and C1 constructors, the D2 and D1 destructors,
// the D0 deleting destructor, Dtor, Update, Draw and Ctor).
//
// Scene manager. The Hex-Rays word dword_99F73C is Application's m_device (the IDevice* at Application +0x10): the asm
// loads it from the singleton's s_inst base with LDR R3,[R3,#(dword_99F73C - 0x99F72C)]. The scene manager pointer is
// the word at IDevice +0x1c (Hex-Rays dword_99F73C + 28; the asm reads it with LDR R4,[R3,#0x1C]). glitch::IDevice does
// not declare that word, and no game header declares the scene manager object, so every call made through it is not
// written here (layout requests in the notes of the four partial rows).
//
// Scene manager vtable calls. Hex-Rays prints these offsets in decimal (+ 60 is 0x3c); the notes below use hex. The
// listing of tools/decomp_vtable.py for CSceneManager has its address point at +0x0c, so a listing offset is the
// Hex-Rays offset plus 0x0c (the vbase offset is read at vptr-12, as the drop() calls show, and scene_nodes.hpp documents
// the same rule). The calls in this batch:
//   Draw   Hex +0x3c  = listing +0x48  drawAll(ISceneNode*), called on the manager with (0)
//   Update Hex +0x60  = listing +0x6c  update(float, bool), called on the manager with (float dt, false)
//   Dtor   Hex +0x68  = listing +0x74  clear(), called on the manager with no arguments
//   Ctor   Hex +0x5c  = listing +0x68  ISceneNode::addChild(ISceneNode*), called on the scene root node (the word at
//                                      manager +0x04) once for each of the camera, the target and the light
//   Ctor   Hex +0x104 = listing +0x110 setTarget(vector3d const&), called on the camera (confirmed by the
//                                      CCameraSceneNode listing; setTarget is declared at that slot in scene_nodes.hpp)
#include "dh2/GSViewer.hpp"

#include "glitch-scene-collada-gui/scene_nodes.hpp"

// 0x387158 GSViewer::GSViewer()  (C2, complete-object constructor)
// 0x38718c GSViewer::GSViewer()  (C1, same body)
// Asm and Hex-Rays: the node words at +0x0c, +0x04 and +0x08 are cleared. The vptr store at +0x00 is done by the
// compiler, so the initialiser list only names the three words.
GSViewer::GSViewer() : m_camera(nullptr), m_target(nullptr), m_light(nullptr) {}

// 0x3871c0 GSViewer::~GSViewer()  (D2, complete-object destructor)
// 0x3871c4 GSViewer::~GSViewer()  (D1, base-object destructor, same body)
// Both are a single BX LR (checked with decomp_asm.py). The node words are released in Dtor, not here.
GSViewer::~GSViewer() {}

// 0x3871c8 GSViewer::Dtor(StateMachine const*)
// PARTIAL. Original: for the words at +0x04, +0x08 and +0x0c, in that order, a non-null word is released through the
// IReferenceCounted base of its node (drop() on the subobject, adjusted by the vtable's offset-to-top), then cleared.
// After the three releases the original returns the result of the scene manager's Hex +0x68 slot (listing +0x74,
// clear()), called with the manager only.
// This body clears the three words in the same order but does not release them. scene_nodes.hpp does not model the
// IReferenceCounted base of the three node classes (CCameraSceneNode says so in its layout comment; CEmptySceneNode and
// CLightSceneNode map only their own members), so drop() has no typed call here. The clear() call is not written
// because the manager is not typed (see the file header and the layout requests). The released nodes are therefore
// leaked until both are written.
void GSViewer::Dtor(const StateMachine* machine) {
    (void)machine;
    m_camera = nullptr;
    m_target = nullptr;
    m_light = nullptr;
}

// 0x387270 GSViewer::Update(StateMachine*, double)
// PARTIAL. Original: converts dt to float (__aeabi_d2f) and returns the scene manager's Hex +0x60 slot (listing +0x6c,
// update(float, bool)), called with (manager, float bits of dt, 0), where the 0 is the bool false. The call is not
// written: scene_nodes.hpp declares no update on CSceneManager (update(int) belongs to ITimeController), and the
// manager is not typed (layout request). The frame update therefore does nothing here.
void GSViewer::Update(StateMachine* machine, double dt) {
    (void)machine;
    (void)dt;
}

// 0x3872bc GSViewer::Draw(StateMachine const*)
// PARTIAL. Original: returns the scene manager's Hex +0x3c slot (listing +0x48, drawAll(ISceneNode*)), called with
// (manager, 0), that is a draw of a null node. scene_nodes.hpp declares drawAll(ISceneNode*) without the virtual, and
// the manager is not typed (layout request), so the draw call is not written and Draw does nothing.
void GSViewer::Draw(const StateMachine* machine) {
    (void)machine;
}

// 0x3872f8 GSViewer::Ctor(StateMachine const*)
// PARTIAL. Written: the camera node. The Hex-Rays constants are float bit patterns: -1007026176 is -500.0f for the
// position's y, and 1120403456 is 100.0f for the target's z. The camera gets id -1, position (0, -500, 0), target
// (0, 0, 100) and farToInfinity false. Scene_nodes.hpp declares this constructor (its base is not reconstructed).
// The original uses operator new(908) with a memory hint; the host allocation is a plain new.
// Not written, in the original order:
//   - The scene manager is read from IDevice +0x1c first (layout request). Each step below goes through it, or through
//     its +0x04 word, the scene root node.
//   - The camera is added to the scene with ISceneNode::addChild (listing +0x68, Hex +0x5c, called on the root node),
//     then released (drop, not typed: see Dtor). scene_node.hpp declares addChild non-virtually, so the vtable call is
//     not written.
//   - The empty target node (CEmptySceneNode, 336 bytes, constructor with id -1) and the light node (CLightSceneNode,
//     348 bytes, bool constructor with argument 1). scene_nodes.hpp declares neither constructor. Both are also added
//     to the root node and released in the same way.
//   - The camera's target is set to the target node's absolute position through the camera's vtable slot Hex +0x104,
//     which is listing +0x110, setTarget(vector3d const&). That slot is confirmed by the CCameraSceneNode listing and is
//     declared at that position in scene_nodes.hpp. Only the manager and target types are missing.
//   - The camera is made the active camera with CSceneManager::setActiveCamera, which scene_nodes.hpp does not declare
//     (it has the activeCamera member at +0xe4 only). The ambient light is set to white (1, 1, 1, 1) with
//     CSceneManager::setAmbientLight, which is declared (SColorf is defined in glitch-shared's glitch_core.hpp).
//   - The light's intrusive pointer (CLightSceneNode +0x134) gets a uint16 2 at CLight +0x58. CLight is only
//     forward-declared in scene_nodes.hpp, so the write is not made.
//   - ASSERT(m_camera && m_target && m_light), file GSViewer.cpp line 65. Not evaluated here, because the target and
//     light are not created yet. At gAssertLevel 2 the original writes to address 0 (abort in the tree's convention).
void GSViewer::Ctor(const StateMachine* machine) {
    (void)machine;
    const glitch::core::vector3d<float> cameraPosition{0.0f, -500.0f, 0.0f};
    const glitch::core::vector3d<float> cameraTarget{0.0f, 0.0f, 100.0f};
    m_camera = new glitch::scene::CCameraSceneNode(-1, cameraPosition, cameraTarget, false);
}

// 0x3875a4 GSViewer::~GSViewer()  (D0, scalar deleting destructor)
// Not written: compiler-generated from the virtual destructor. Asm: BL D1 (0x3871c4), then CustomFree(this).
// C++ delete covers it.
