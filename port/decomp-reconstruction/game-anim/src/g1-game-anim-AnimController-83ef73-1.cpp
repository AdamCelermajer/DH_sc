// AnimController constructor, rebuilt from libDungeonHunter2.so (Hex-Rays and the 0x474e44 disassembly).
// Status: partial. Rebuilt: the m_node store, the null-node assert, and the do-nothing callbacks used when
// trackEvents is clear. Missing (waits on dh2/RootSceneNode.hpp, not written yet): the reference-count step and
// the trackEvents call. RootSceneNode is only forward-declared in the game headers.
#include "dh2/AnimController.hpp"

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

    // partial: the reference-count step is not rebuilt. The original adds 1 to the counter of the base subobject
    // of *m_node: the subobject offset is read from the node's vtable at vptr-12 (524 in RootSceneNode's vtable),
    // and the counter is at +4 inside it.

    if (!trackEvents) {
        SetCallbacksOnAll(&AnimController::_CBAnim_DoNothing, this, &AnimController::_CBEvent_DoNothing, this);
    }
    // partial: when trackEvents is set, the original calls the node's virtual at vptr+0x74 with the node as this
    // (vtables-001.json: glitch::scene::ISceneNode::removeAnimators(), not verified). Not rebuilt until
    // RootSceneNode is declared.
}
