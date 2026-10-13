// Batch g1-game-anim-AnimatedFX-6c1bfb-0: the AnimatedFX constructors and destructors, speed and loop
// control, scale, the material assert, Load, SyncIrrData and SetAnimFX.
// Declarations and member names come from dh2/AnimatedFX.hpp. Each PARTIAL function says in its comment
// what is not rebuilt; the same gaps are listed in the ledger notes and the layout requests.

#include <cstdio>
#include <cstdlib>
#include <string>

#include "core-platform/deps/game_types.hpp"
#include "dh2/AnimController.hpp"
#include "dh2/AnimatedFX.hpp"
#include "dh2/VisualObject.hpp"

// Global assert level (libDungeonHunter2.so gAssertLevel at 0x99f914). core-platform/memory.hpp declares it too,
// but that header redefines glitch::core types that glitch-shared's glitch_core.hpp also defines, so it is not included here.
extern int gAssertLevel;

// 0x4922e0 _ZN10AnimatedFXC2Ei
// 0x492374 _ZN10AnimatedFXC1Ei
// Both constructor addresses have the same body (base-object and complete-object variants), so one
// definition covers both. m_endOfLoop is cleared here; the original leaves it unset, and no code reads it
// before SetAnimFX stores a callback.
AnimatedFX::AnimatedFX(int id) {
    unk_08 = id;                    // +0x08 constructor argument
    unk_1c = -1;                    // +0x1c
    m_speed = 1.0f;                 // +0x20
    unk_28 = 0;                     // +0x28 owner word (int in the header, see the layout requests)
    m_visual = nullptr;             // +0x2c
    m_looping = false;              // +0x18
    unk_24 = true;                  // +0x24 visibility flag
    m_name = nullptr;               // +0x0c
    m_animName = nullptr;           // +0x10
    m_loopCount = 0;                // +0x14
    unk_30 = false;                 // +0x30
    unk_31 = false;                 // +0x31
    unk_32 = false;                 // +0x32
    unk_34 = 0;                     // +0x34 (0.0f bit pattern)
    unk_38 = 0;                     // +0x38
    unk_3c = 0;                     // +0x3c
    unk_40 = 0;                     // +0x40
    unk_44 = 0;                     // +0x44
    unk_48 = 0;                     // +0x48
    unk_4c = false;                 // +0x4c
    m_setData = nullptr;            // +0x50
    m_endOfLoop = nullptr;          // +0x04
}

// 0x492408 _ZN10AnimatedFXD2Ev
// 0x49245c _ZN10AnimatedFXD1Ev
// Both destructor addresses have the same body. The vtable slot 1 variant 0x492950 is the compiler's
// deleting destructor and is not written here (see the ledger row).
AnimatedFX::~AnimatedFX() {
    if (m_visual != nullptr) {      // +0x2c
        delete m_visual;            // VisualObject's virtual deleting destructor (vtable slot +4 in the original)
        m_visual = nullptr;
    }
}

// 0x4924b0 _ZN10AnimatedFX8SetSpeedEf
// The call passes r2 = 0 (asm: MOV R2, #0 before the call through slot +0x28), so the AnimController
// SetScale index is 0. Hex-Rays shows only the controller and the speed.
void AnimatedFX::SetSpeed(float speed) {
    m_speed = speed;                                            // +0x20
    if (m_visual != nullptr) {                                  // +0x2c
        m_visual->m_animController->SetScale(speed, 0);         // AnimController slot +0x28
    }
}

// 0x4924e0 _ZNK10AnimatedFX12HasCompletedEv
// PARTIAL. Not rebuilt. The original reads the global at 0x99F73C (asm: Singleton<Application>::s_inst
// + 0x10), calls its object's slot +0x1c with (0x70656164, m_visual->m_node), and when a node comes back it
// returns (the node's vtable slot +0xf8 result <= 0). Returns true when no node is found. Neither the
// global's object nor the node interface is declared in the tree. Placeholder: returns false.
bool AnimatedFX::HasCompleted() const {
    return false;
}

// 0x49267c _ZN10AnimatedFX11GetAnimatorEv
glitch::collada::ISceneNodeAnimator* AnimatedFX::GetAnimator() {
    if (m_visual != nullptr) {
        return m_visual->m_animController->GetAnim(0);
    }
    return nullptr;
}

// 0x492550 _ZN10AnimatedFX17GetAnimControllerEv
AnimController* AnimatedFX::GetAnimController() {
    if (m_visual != nullptr) {
        return m_visual->m_animController;
    }
    return nullptr;
}

// 0x492560 _ZN10AnimatedFX11SetEndPointERK7Point3DIfE
// PARTIAL. The original, when the owner word +0x28 is set, takes the distance from the owner's target
// position to the end point, and writes it into the node's animator sub-object (node word +0x178) and the
// node's x position (-0.5 times 0.3 of the distance; y and z from the node's position getter). Not rebuilt:
// the owner word is int in the header (layout request GameObject* at +0x28), and the node sub-object and
// position interface are not declared in the tree.
void AnimatedFX::SetEndPoint(const Point3D<float>& position) {
    (void)position;
}

// 0x492694 _ZN10AnimatedFX10SetLoopingEb
// PARTIAL. When the FX has a visual and an animator, the original calls the animator's slot +0x44 (returns the
// glitch::scene::ITimelineController) and then the controller's slot +0x40 with the flag. Neither interface is
// declared in the tree, so that call is not written. The flag store is written.
void AnimatedFX::SetLooping(bool looping) {
    if (m_visual != nullptr && GetAnimator() != nullptr) {
        // NOT RECONSTRUCTED: ITimelineController slot +0x40 (looping), see above.
    }
    m_looping = looping;                                        // +0x18
}

// 0x4926e4 _ZN10AnimatedFX6SetEndEv
// PARTIAL. The original passes the controller's end word (+0x14) to the controller's slot +0x0c. The
// controller comes from the animator (slot +0x44). Interfaces not declared in the tree; the call is not
// written.
void AnimatedFX::SetEnd() {
    if (m_visual != nullptr && GetAnimator() != nullptr) {
        // NOT RECONSTRUCTED: ITimelineController slot +0x0c with the controller's +0x14 word.
    }
}

// 0x492744 _ZN10AnimatedFX8SetStartEv
// PARTIAL. The same call as SetEnd with the controller's +0x10 word (the start). Not written, for the same
// reason.
void AnimatedFX::SetStart() {
    if (m_visual != nullptr && GetAnimator() != nullptr) {
        // NOT RECONSTRUCTED: ITimelineController slot +0x0c with the controller's +0x10 word.
    }
}

// 0x4927a4 _ZN10AnimatedFX14_HandleLoopEndEv
// PARTIAL. Written: the gate (speed > 0, or a callback set), the loop countdown, the loop-end branch that
// clears looping and sets the first byte of the set data, and the end-of-loop callback. Not rebuilt: the
// timeline part that follows the callback in the original (animator slot +0x44, the controller's +0x14 and
// +0x04 words, SetEnd when they differ, then the animator's slot +0x10 with (m_visual->m_node, the end word)). Those
// interfaces are not declared in the tree.
void AnimatedFX::_HandleLoopEnd() {
    if (!(m_speed <= 0.0f) || m_endOfLoop != nullptr) {         // speed > 0, or a callback is set
        const int loops = m_loopCount;                          // +0x14
        if (loops >= 0) {
            if (loops != 0) {
                m_loopCount = loops - 1;
                SetStart();
                return;
            }
            SetLooping(false);                                  // +0x18
            if (m_setData != nullptr) {                         // +0x50
                *static_cast<unsigned char*>(m_setData) = 1;    // first byte of the set data
            }
            if (m_endOfLoop != nullptr) {
                m_endOfLoop(this, m_setData);
                m_endOfLoop = nullptr;
                // NOT RECONSTRUCTED: the timeline part after the callback, see the comment above.
            }
        }
    }
}

// 0x4928a4 _ZN10AnimatedFX7_CBLoopEPN6glitch5scene19ITimelineControllerEPv
// The controller argument is unused. The user pointer is the FX (Load registers this).
void AnimatedFX::_CBLoop(glitch::scene::ITimelineController* /*controller*/, void* user) {
    static_cast<AnimatedFX*>(user)->_HandleLoopEnd();
}

// 0x4928ac _ZN10AnimatedFX8SetScaleERK7Point3DIfE
void AnimatedFX::SetScale(const Point3D<float>& scale) {
    if (m_visual != nullptr) {                                  // +0x2c
        unk_32 = false;                                         // +0x32
        m_visual->SetScaling(scale);
    }
}

// 0x4928c8 _ZN10AnimatedFX13ForceMaterialEi
// The original's assert path: level 2 writes through a null pointer to crash (here std::abort, as in
// GCSingleton::GetInstance); level 1 prints the assert message to stderr. Level 0 does nothing. The
// material argument is not used by any path.
void AnimatedFX::ForceMaterial(int /*material*/) {
    if (gAssertLevel == 2) {
        std::abort();
    } else if (gAssertLevel == 1) {
        std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", "false",
                     "..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\VisualFX\\AnimatedFX.cpp", 284);
    }
}

// 0x49296c _ZN10AnimatedFX4LoadEPKcS1_fib
// PARTIAL. Written: the speed, name, loop count and animation name stores, the VisualObject creation
// (owner null, model = name, variant = animation name), the SetCallbacks on the controller with _CBLoop
// and this, and SetScale(speed, 0). Not rebuilt: the self-illuminated branch (selfIlluminated true). It calls
// SetNodeToSelfIlluminated(ISceneNode*) with the visual's root node, a RootSceneNode*; RootSceneNode has only
// a forward declaration in the tree, so the conversion cannot be written.
void AnimatedFX::Load(const char* name, const char* animName, float speed, int loopCount, bool selfIlluminated) {
    m_speed = speed;                                            // +0x20
    m_name = name;                                              // +0x0c
    m_loopCount = loopCount;                                    // +0x14
    m_animName = animName;                                      // +0x10
    m_visual = new VisualObject(nullptr, std::string(name), std::string(animName));
    if (m_visual != nullptr) {
        m_visual->m_animController->SetCallbacks(&AnimatedFX::_CBLoop, this, nullptr, nullptr);
        m_visual->m_animController->SetScale(m_speed, 0);
        if (selfIlluminated) {
            // NOT RECONSTRUCTED: SetNodeToSelfIlluminated(root node). See the note above.
        }
    }
}

// 0x492aa0 _ZN10AnimatedFX11SyncIrrDataEb
// PARTIAL. Only the early return without a visual is written. The rest of the original depends on the owner
// word +0x28, which the header declares as int (layout request: GameObject* at +0x28). Not rebuilt:
//   - the owner's target position added to the offset words +0x34..+0x3c;
//   - rotation: from the visual's node (node slot +0x38 gives the absolute transform, read by
//     glitch::core::CMatrix4::getRotationDegrees, scaled by 0.017453 = bits 0x3C8EFA35) or else from the
//     owner's rotation (GameObject +0x16c); written to +0x40..+0x48 and passed to VisualObject::SetRotation;
//   - scale: the owner's scale (+0x120) or GetFXScale(), passed to VisualObject::SetScaling, gated by +0x32;
//   - the floor normal: PFWorld::GetFloorHeightAt(const Point3D<float>&, float*, Point3D<float>*, PFRoom**,
//     PFFloor**, bool) through Singleton<PFWorld>::s_inst (dh2/PFWorld.hpp is absent). The normal is only
//     kept on the stack, so only this query call is at stake;
//   - the setData walk when push and +0x31 are set: VisualFXManager::AnimFXSetData is not declared
//     (+0x2c next pointer, +0x04 count; a count above zero on the last entry clears the rotation flag).
// The position and the explicit rotation (+0x4c) are therefore not written here.
void AnimatedFX::SyncIrrData(bool push) {
    if (m_visual == nullptr) {
        return;
    }
    // NOT RECONSTRUCTED beyond this point, see the comment above. The push flag is not used yet.
    (void)push;
}

// 0x492e8c _ZN10AnimatedFX9SetAnimFXEbbbfiPN15VisualFXManager13AnimFXSetDataEPFvPS_S2_E
// Stores the three rotation/scale flags (+0x30, +0x31, +0x32) with a SyncIrrData sync after each, sets the
// speed, then the set data (+0x50), the loop count (+0x14) and the end-of-loop callback (+0x04). The
// flags are named after what SyncIrrData does with them (see the header comments).
void AnimatedFX::SetAnimFX(bool rotateWithOwner, bool rotateWithOwnerOnPush, bool scaleWithOwner, float speed,
                           int loopCount, void* setData, EndOfLoopCallback endOfLoop) {
    unk_30 = rotateWithOwner;               // +0x30
    SyncIrrData(false);
    unk_31 = rotateWithOwnerOnPush;         // +0x31
    SyncIrrData(true);
    unk_32 = scaleWithOwner;                // +0x32
    SyncIrrData(false);
    SetSpeed(speed);
    m_setData = setData;                    // +0x50
    m_loopCount = loopCount;                // +0x14
    m_endOfLoop = endOfLoop;                // +0x04
}
