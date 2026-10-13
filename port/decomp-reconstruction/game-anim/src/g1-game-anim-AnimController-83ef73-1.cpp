// AnimController constructor, rebuilt from libDungeonHunter2.so (Hex-Rays and the 0x474e44 disassembly).
// Status: partial. Rebuilt: the m_node store, the null-node assert, the reference-count step on the node's second
// subobject, and the do-nothing callbacks used when trackEvents is clear. Missing: the trackEvents call (the node's
// virtual at vptr+0x74, see below), which needs an ISceneNode base on RootSceneNode.
#include "dh2/AnimController.hpp"
#include "dh2/RootSceneNode.hpp"

#include <cstdio>
#include <cstdlib>

// Engine assert level, defined in core-platform/src/memory.cpp. Declared here rather than through
// core-platform/memory.hpp, which redefines glitch::core types that the game headers already provide.
extern int gAssertLevel;

// 0x474e44 _ZN14AnimControllerC2EP13RootSceneNodeb
// The complete-object constructor at 0x474d30 (C1) has the same body; it is defined once, here.
AnimController::AnimController(RootSceneNode* node, bool trackEvents) : m_node(node) {
    // +0x00 vptr: the original stores AnimController's vtable first (the compiler does this here).
    // +0x04 m_node: stored before the null check, as in the original.
    if (m_node == nullptr) {
        if (gAssertLevel == 2) {
            // The original writes to address 0 here to force a crash.
            std::abort();
        } else if (gAssertLevel == 1) {
            std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", "m_node",
                         "..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\ObjectsSub\\Visual\\Animations"
                         "\\AnimController.cpp",
                         28);
        }
        // The original then reads the node's vtable through the null pointer and faults at every assert level.
        std::abort();
    }

    // Reference count: the original adds 1 to the counter of the second subobject of *m_node. Its offset is read from
    // the node's vtable at vptr-12 (524 = 0x20c in RootSceneNode's vtable), and the counter is at +4 inside it, which
    // is RootSceneNode::m_secondaryRefCount (+0x210, dh2/RootSceneNode.hpp).
    ++m_node->m_secondaryRefCount;

    if (!trackEvents) {
        SetCallbacksOnAll(&AnimController::_CBAnim_DoNothing, this, &AnimController::_CBEvent_DoNothing, this);
    }
    // partial: when trackEvents is set, the original calls the node's virtual at vptr+0x74 with the node as this.
    // Hex-Rays gives the call as +116 (0x74). Per the RootSceneNode listing of decomp_vtable.py (+0x80, address point
    // +0x0c, as getPosition's +0xac / vptr+0xa0 shows) that slot is glitch::scene::ISceneNode::removeAnimators().
    // Not written: RootSceneNode has no ISceneNode base in glitch-scene-collada-gui/root_scene_node.hpp, so the call
    // has no declared target. Not verified in IDA.
}
