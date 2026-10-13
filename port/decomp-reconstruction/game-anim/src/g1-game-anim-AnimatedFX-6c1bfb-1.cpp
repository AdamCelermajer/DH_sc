// Batch g1-game-anim-AnimatedFX-6c1bfb-1: AnimatedFX::SetVisible, SetRotation and Update.
// Declarations and member names come from dh2/AnimatedFX.hpp. The comments on each function say
// which parts of the original are not rebuilt yet (see the layout requests in the ledger notes).

#include <cstring>
#include <string>

#include "core-platform/deps/game_types.hpp"
#include "dh2/AnimatedFX.hpp"
#include "dh2/DebugSwitches.hpp"
#include "dh2/VisualObject.hpp"

namespace {

// Switch read by the trace check in Update (string literal in the original).
const char kTraceAnimFxSwitch[] = "isTracingAnim_FX";

// Copies the 32-bit pattern of a float into an int member. The header types the rotation words
// (+0x40, +0x44, +0x48) as int; they hold Point3D<float> data. This keeps the exact bits the
// original copies until the layout request is applied.
void CopyFloatBits(float src, int& dst) {
    std::memcpy(&dst, &src, sizeof(float));
}

}  // namespace

// 0x492ef0 _ZN10AnimatedFX10SetVisibleEb
// PARTIAL. Missing: after the VisualObject call the original also stores the new visible flag in the
// byte at +0x200 of the VisualObject's root node (VisualObject +0x08, RootSceneNode). RootSceneNode
// has no complete declaration in the game headers beyond its forward declaration, so that store is not
// written here (layout request).
void AnimatedFX::SetVisible(bool visible) {
    if (unk_24 != visible) {                // +0x24 visibility flag
        VisualObject* visual = m_visual;    // +0x2c
        unk_24 = visible;
        if (visual != nullptr) {
            visual->SetVisible(visible);
            // MISSING: root node byte +0x200 = visible (see the note above).
        }
        SyncIrrData(false);
    }
}

// 0x492f3c _ZN10AnimatedFX11SetRotationERK7Point3DIfE
// Stores the three rotation words at +0x40, +0x44 and +0x48, sets the flag at +0x4c, then calls
// SyncIrrData(false). The words are copied bit for bit (see CopyFloatBits). Once the layout request
// makes +0x40 a Point3D<float>, this becomes a plain assignment.
void AnimatedFX::SetRotation(const Point3D<float>& rotation) {
    CopyFloatBits(rotation.X, unk_40);      // +0x40
    CopyFloatBits(rotation.Y, unk_44);      // +0x44
    unk_4c = true;                          // +0x4c
    CopyFloatBits(rotation.Z, unk_48);      // +0x48
    SyncIrrData(false);
}

// 0x492f68 _ZN10AnimatedFX6UpdateEv
// PARTIAL. Not rebuilt, in the original's order:
//   1. The owner at +0x28 is cleared when owner->IsDead() is true, or when the owner's byte at
//      ObjectBase +0x81 (m_markedForDeletion) is set. Needs +0x28 typed GameObject* (layout request).
//   2. Lifetime countdown at +0x1c (ms, -1 = none): while it is >= 0, GetDt() is subtracted, and when
//      the result is <= 0 the loop count at +0x14 is set to 0. Needs Application::GetDt() on
//      Singleton<Application>::s_inst; no game header declares that accessor.
//   3. When the owner is still set and its ObjectBase +0x84 byte (m_static) is zero, SyncIrrData(false)
//      runs. Depends on item 1.
// Rebuilt below: the trace-switch check (only while visible), SetSpeed(m_speed), and the
// completion check that hides a finished, non-looping FX.
void AnimatedFX::Update() {
    if (unk_24) {                           // +0x24 visible
        // +0x50 is m_setData. When it is set, the trace switch is read one extra time first.
        if (m_setData != nullptr) {
            (void)DebugSwitches::GetInstance()->GetSwitch(kTraceAnimFxSwitch);
        }
        // GetInstance() loads the switch file, as the original's load call did, before the read.
        (void)DebugSwitches::GetInstance()->GetSwitch(kTraceAnimFxSwitch);
    }

    SetSpeed(m_speed);

    const bool looping = m_looping;         // +0x18
    if (!looping && unk_24) {
        if (HasCompleted()) {
            SetVisible(looping);            // looping is false here, so this hides the finished FX
        }
    }
}
