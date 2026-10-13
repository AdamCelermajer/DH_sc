// CSMove bodies (batch g1-game-character-CSMove-401d7e-0): the move state. Focus selects the walk or run animation,
// blur stops the actor, init registers the state's events. Declarations come from dh2/CSMove.hpp. Each PARTIAL comment
// names what is missing, and the ledger rows in game-character/ledger.d/g1-game-character-CSMove-401d7e-0.csv repeat it.
//
// Fields of other classes that are not reachable from this unit are read or written through the stub accessors in the
// anonymous namespace below. Each stub is one line, so the layout request that makes the field public replaces one
// accessor and nothing else.

#include "dh2/CSMove.hpp"

#include <cmath>
#include <cstdint>
#include <cstring>

#include "dh2/Character.hpp"
#include "dh2/CharAnimator.hpp"
#include "dh2/CharProperties.hpp"
#include "dh2/CharStateMachine.hpp"
#include "dh2/DebugSwitches.hpp"
#include "dh2/GameObject.hpp"
#include "dh2/PhysicalObject.hpp"
#include "dh2/global/charai_ai.hpp"
#include "pydata-accessors/w7_pydata12.hpp"

namespace {

// Move types stored in CharStateMachine +0x40 (the original's Character +0x53c).
constexpr int kMoveNone = 0;
constexpr int kMoveWalk = 1;
constexpr int kMoveRun = 2;

// CharStateMachine +0x24 is written as one word with this value by focus (the original's Character +0x520): 0x23c1.
constexpr std::uint32_t kFocusStateFlags = 9153u;

// Stance bits tested against the AnimStancedAnim constant: 0x20 for the run anim, 0x10 for the walk anim.
constexpr int kRunStanceBit = 0x20;
constexpr int kWalkStanceBit = 0x10;

// Event id raised when the character has no heading (or stops at its destination without a target).
constexpr int kEventNoHeading = 63;

// Reinterprets the float bits that CharProperties::PROPS_GetWalkSpeed returns as int (the original's LODWORD).
float FloatFromBits(std::int32_t bits) {
    float value = 0.0f;
    std::memcpy(&value, &bits, sizeof value);
    return value;
}

// Runs the DebugSwitches probe that focus and blur both perform. DebugSwitches::GetInstance (0x330740) is
// load(&s_inst) followed by return &s_inst, so GetInstance()->GetSwitch(name) matches the original's
// load(&s_inst) then GetSwitch(&s_inst, name). The bool is discarded, as in the original.
void ProbeTracingSwitch() {
    DebugSwitches::GetInstance()->GetSwitch("isTracingCharState");
}

// (x*x + y*y) + z*z of the heading, in the original's float evaluation order.
float HeadingLengthSquared(const Character* character) {
    const float x = character->m_heading.X;
    const float y = character->m_heading.Y;
    const float z = character->m_heading.Z;
    return ((x * x) + (y * y)) + (z * z);
}

// PARTIAL. CharStateMachine +0x40 (move type), +0x30 (walk speed as float bits) and +0x24 (m_stateFlags) are private
// (unk_40, unk_30 and m_stateFlags in CharStateMachine.hpp). Until they are public, reads return 0 and writes are
// dropped. Layout requests filed for all three.
int ReadMoveType(const Character*) { return kMoveNone; }
void WriteMoveType(Character*, int) {}
std::int32_t ReadWalkSpeedBits(const Character*) { return 0; }
void WriteWalkSpeedBits(Character*, std::int32_t) {}
void WriteStateFlags(Character*, std::uint32_t) {}

// PARTIAL. CharAI +0x40 (m_target) is private in CharAI.hpp, and OnUpdate only tests it for null. Until it is public,
// this reports no target, so OnUpdate always takes the no-target branch. Layout request filed.
bool AiHasTarget(const Character*) { return false; }

// PARTIAL. Structs::DesignSettings (record 0 of Arrays::DesignSettingsTable) floats at +0x5c (walk threshold) and
// +0x60 (run threshold) are not named, so these return 0 until the record is declared. Layout request filed.
float WalkThreshold() { return 0.0f; }
float RunThreshold() { return 0.0f; }

// PARTIAL. Structs::CharAnim fields of the character's row in Arrays::CharAnimTable: int32 +0x70 (run anim id, stance
// bit 0x20) and int32 +0x94 (walk anim id, stance bit 0x10) are not named. The row index itself comes from
// Character::GetCharAnimTableId, which is partial. These return 0 until the record is declared.
int RunAnimField(const Character*) { return 0; }
int WalkAnimField(const Character*) { return 0; }

// The AnimStancedAnim/SL__LIST_IPHONE constant from the application's PyDataConstants, masked with `bit`.
int StanceBit(int bit) {
    return ApplicationGetPyDataConstants()->getConstant("AnimStancedAnim", "SL__LIST_IPHONE") & bit;
}

// The ANIM_Set argument: the CharAnim field, plus the character's stance when the stance bit is set. This is the
// original's (AnimStance + v10), where AnimStance is 0 or Character::GetAnimStance().
int StancedAnim(const Character* character, int field, int stanceBit) {
    int anim = field;
    if (StanceBit(stanceBit) != 0) {
        anim += character->GetAnimStance();
    }
    return anim;
}

// Walk-type tail shared by UpdateType's walk paths: sets the walk move type and speed, then plays the walk anim.
void ApplyWalk(Character* character) {
    WriteMoveType(character, kMoveWalk);
    const std::int32_t speedBits = character->CharProperties::PROPS_GetWalkSpeed();
    WriteWalkSpeedBits(character, speedBits);
    const int anim = StancedAnim(character, WalkAnimField(character), kWalkStanceBit);
    character->CharAnimator::ANIM_Set(anim);
    character->CharAnimator::ANIM_SetSpeed(FloatFromBits(speedBits));
}

// Run-type tail: sets the run move type and speed, then plays the run anim.
void ApplyRun(Character* character) {
    WriteMoveType(character, kMoveRun);
    const std::int32_t speedBits = character->CharProperties::PROPS_GetWalkSpeed();
    WriteWalkSpeedBits(character, speedBits);
    const int anim = StancedAnim(character, RunAnimField(character), kRunStanceBit);
    character->CharAnimator::ANIM_Set(anim);
    character->CharAnimator::ANIM_SetSpeed(FloatFromBits(speedBits));
}

}  // namespace

// 0x3c0008 CSMove::~CSMove()
// Empty: the Hex-Rays body is a bare return (4-byte bx lr). No base destructor call and no member cleanup appear.
CSMove::~CSMove() {}

// 0x3c09c4 CSMove::~CSMove() (deleting destructor, vtable slot 0x04)
// Not written: the compiler emits the deleting destructor from the virtual destructor above, and C++ delete covers it.
// The original sets the vptr to off_9663A8 and calls CustomFree(this). See the ledger row.

// 0x3c000c CSMove::OnEvent(int,Character*,CharStateMachine*,int,void*)
// Empty body (4-byte bx lr). The arguments are not read.
void CSMove::OnEvent(int, Character*, CharStateMachine*, int, void*) {}

// 0x3c0f18 CSMove::UpdateType(Character*,CharStateMachine*)
// PARTIAL. The control flow is reconstructed from the pseudocode. These inputs are stubs: the move type and walk speed
// fields (CharStateMachine +0x40 and +0x30), the walk and run thresholds (DesignSettings +0x5c and +0x60) and the run and
// walk anim ids (CharAnim +0x70 and +0x94). The original's int result is not kept (see the header). The machine
// argument is unused in the original.
void CSMove::UpdateType(Character* character, CharStateMachine*) {
    if (character->IsPlayer()) {
        const int moveType = ReadMoveType(character);
        const float lengthSquared = HeadingLengthSquared(character);

        if (moveType != kMoveRun) {
            // Run when the run threshold squared is below the heading length squared.
            const float runThreshold = RunThreshold();
            if (runThreshold * runThreshold < lengthSquared) {
                ApplyRun(character);
                return;
            }
            if (moveType == kMoveNone) {
                ApplyWalk(character);
                return;
            }
            if (moveType == kMoveWalk) {
                return;
            }
        }

        // Walk when the walk threshold squared is above the heading length squared (the original's LABEL_6).
        const float walkThreshold = WalkThreshold();
        if (walkThreshold * walkThreshold > lengthSquared) {
            ApplyWalk(character);
        }
        return;
    }

    // Non-player with no move type yet: walk without the threshold test.
    if (ReadMoveType(character) == kMoveNone) {
        ApplyWalk(character);
    }
}

// 0x3c1184 CSMove::OnUpdate(int,Character*,CharStateMachine*)
// PARTIAL. The target test (CharAI +0x40) and the walk speed field (CharStateMachine +0x30) are stubs. The original
// returns int (RaiseEvent's result, or a float comparison). This void slot drops that result, and its use by
// CharStateMachine is unverified.
void CSMove::OnUpdate(int, Character* character, CharStateMachine* machine) {
    if (!character->m_hasHeading) {
        character->RaiseEvent(kEventNoHeading, nullptr);
        return;
    }
    if (!AiHasTarget(character)) {
        // Without a target, a non-player that is at its destination and not remotely updated stops (event 63).
        if (!character->IsPlayer()) {
            if (character->GameObject::IsAtDestination() && !character->IsRemotelyUpdated()) {
                character->RaiseEvent(kEventNoHeading, nullptr);
                return;
            }
        }
    }

    UpdateType(character, machine);
    const std::int32_t speedBits = character->CharProperties::PROPS_GetWalkSpeed();
    const float speed = FloatFromBits(speedBits);
    const float previous = FloatFromBits(ReadWalkSpeedBits(character));
    if (!(std::fabs(speed - previous) < 0.0001f)) {
        WriteWalkSpeedBits(character, speedBits);
        character->CharAnimator::ANIM_SetSpeed(speed);
    }
}

// 0x3c3aa4 CSMove::OnBlur(int,Character*,CharStateMachine*,int)
void CSMove::OnBlur(int, Character* character, CharStateMachine*, int) {
    ProbeTracingSwitch();
    character->GameObject::Stop();
    if (character->m_physicalObject != nullptr) {
        character->m_physicalObject->pin();
    }
}

// 0x3c3bf8 CSMove::OnFocus(int,Character*,CharStateMachine*,int,int,void*)
// PARTIAL. The original writes CharStateMachine +0x24 (state flags, 0x23c1) and +0x40 (move type, 0) before UpdateType.
// Both writes are dropped until the fields are public. The trailing arguments are not read.
void CSMove::OnFocus(int, Character* character, CharStateMachine* machine, int, int, void*) {
    ProbeTracingSwitch();
    WriteStateFlags(character, kFocusStateFlags);
    WriteMoveType(character, kMoveNone);
    UpdateType(character, machine);
    if (character->m_physicalObject != nullptr) {
        character->m_physicalObject->unpin();
    }
}

// 0x3c80ac CSMove::OnInit(int,Character*,CharStateMachine*)
// The registrations go to the CharStateMachine base of the character (the original's (char*)a3 + 1276), not to the
// machine argument, which the original never reads. Each row is (state, event, next state, handler), in the
// original's call order. A null handler is the original's zero pointer-to-member.
void CSMove::OnInit(int stateId, Character* character, CharStateMachine*) {
    CharStateMachine* machine = character;
    machine->SM_RegisterEvent(stateId, kEventNoHeading, 3, nullptr);
    machine->SM_RegisterEvent(stateId, 50008, 12, nullptr);
    machine->SM_RegisterEvent(stateId, 50010, 11, &Character::CSM_Injured);
    machine->SM_RegisterEvent(stateId, 50011, 10, &Character::CSM_Interrupted);
    machine->SM_RegisterEvent(stateId, 50012, 9, &Character::CSM_Interrupted);
    machine->SM_RegisterEvent(stateId, 50013, 8, &Character::CSM_Interrupted);
    machine->SM_RegisterEvent(stateId, 50006, 7, nullptr);
    machine->SM_RegisterEvent(stateId, 50005, 6, nullptr);
    machine->SM_RegisterEvent(stateId, 50004, 5, &Character::CSM_Attack);
    machine->SM_RegisterEvent(stateId, 50003, 13, nullptr);
    machine->SM_RegisterEvent(stateId, 50007, 15, nullptr);
}
