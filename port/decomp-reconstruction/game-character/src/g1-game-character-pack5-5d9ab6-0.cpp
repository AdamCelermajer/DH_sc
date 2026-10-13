// Batch g1-game-character-pack5-5d9ab6-0: the CSLimbus and CSPreSpawn character states. Empty update
// and destructor bodies, the state registration in OnInit, the focus, blur and event hooks, and the
// deleting destructors (compiler-generated, skipped). Declarations come from dh2/CSLimbus.hpp and
// dh2/CSPreSpawn.hpp; the Character layout comes from dh2/Character.hpp.
//
// Partial functions say at the point of the gap what is missing. The gaps are all private members of
// other classes, so they need layout requests (see the batch report):
//   - Character +0x520 is CharStateMachine::m_stateFlags (CSM +0x24, private).
//   - Character +0x530 is CharStateMachine::m_limbusFlag (CSM +0x34, private).
//   - Character +0x3fc and +0x400 are CharAI::m_group and CharAI::m_groupRole (private).
//   - GroupInfo +0x24 is written 0 or 2 by the limbus logic. CharAI.hpp names it m_allMembersDead, which
//     does not explain those values, so the write is not made.
//   - COnline +0x05 is isOnlineGame_ (private, no getter).
//   - The Arrays::CharAnimTable row fields at +0x64 and +0x80 have no named host layout.

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

#include "core-platform/memory.hpp"  // gAssertLevel
#include "dh2/CSLimbus.hpp"
#include "dh2/CSPreSpawn.hpp"
#include "dh2/Character.hpp"
#include "dh2/DebugSwitches.hpp"
#include "dh2/v2Controller.hpp"

namespace {

// The original ASSERT(expr) macro, as in the CharAI batch: level 2 stops the process, level 1 prints.
void AssertFailed(const char* expr, const char* file, int line) {
    if (gAssertLevel == 2) {
        std::abort();
    } else if (gAssertLevel == 1) {
        std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", expr, file, line);
    }
}

// Both states start with this call (0x330740 DebugSwitches::GetInstance() loads the switches and returns
// the singleton). The original reads the "isTracingCharState" switch and discards the result.
void ReadTraceSwitch() {
    (void)DebugSwitches::GetInstance()->GetSwitch(std::string("isTracingCharState"));
}

const char* const kCSPreSpawnFile =
    "..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\Objects\\Characters\\StateMachine\\/../States/CSPreSpawn.h";

}  // namespace

// 0x3bffe0 _ZN8CSLimbusD1Ev
// Complete-object destructor. The body is empty (asm: BX LR).
CSLimbus::~CSLimbus() {}

// 0x3bffe4 CSLimbus::OnUpdate(int, Character*, CharStateMachine*)
// Empty body (asm: BX LR).
void CSLimbus::OnUpdate(int, Character*, CharStateMachine*) {}

// 0x3bffe8 CSLimbus::OnEvent(int, Character*, CharStateMachine*, int, void*)
// Empty body (asm: BX LR).
void CSLimbus::OnEvent(int, Character*, CharStateMachine*, int, void*) {}

// 0x3c0a94 _ZN8CSLimbusD0Ev
// skipped: compiler-generated deleting destructor. It stores the CharState vtable into the object and
// then calls CustomFree(this). delete on a CSLimbus produces it, so no C++ body is written.

// 0x3c2be4 CSLimbus::OnBlur(int, Character*, CharStateMachine*, int)
// partial. Written: the trace switch, the controller block flag (Character +0x378 is the v2Controller
// pointer, m_blocked at +0x08 is cleared), SetVisible(true) through vtable slot +0x40, the initial
// position and rotation (Character +0x1450 and +0x145c), and Revive(nullptr, false).
// Missing: the group block. When m_groupRole (CharAI +0x38, private) is 3, the original scans m_group
// (CharAI +0x34, private) members other than this one: if any is in limbus, the group value is 2, else
// 0 (0 when the group is empty). That value goes to GroupInfo +0x24 (see the file comment). Omitted.
void CSLimbus::OnBlur(int /*stateId*/, Character* character, CharStateMachine* /*machine*/, int /*nextState*/) {
    ReadTraceSwitch();
    character->m_controller->m_blocked = false;
    character->SetVisible(true);
    character->GameObject::SetPosition(character->m_initialPosition, true);
    character->GameObject::SetRotation(character->m_initialRotation);
    character->Character::Revive(nullptr, false);
    // partial: group block omitted (see above).
}

// 0x3c2e58 CSLimbus::OnFocus(int, Character*, CharStateMachine*, int, int, void*)
// partial. Written: the trace switch, SetVisible(false) through vtable slot +0x40 (asm: the argument is
// 0), and the final CharAI::AI_ClearAllAggro.
// Missing 1: Character +0x520 (CharStateMachine::m_stateFlags, private) is set to 0 before SetVisible.
// Missing 2: the respawn timer branch. The original takes it when the limbus flag (Character +0x530,
// CharStateMachine::m_limbusFlag, private) is set, GetRespawnDelay() > 0, and either COnline +0x05
// (isOnlineGame_, private, no getter) is clear or dword_99F76C->IsLocalPlayerHosting(). The branch then
// calls CharTimers::TMR_Start(delay, 0, 47, nullptr) on the CharTimers subobject (Character +0x3b4).
void CSLimbus::OnFocus(int /*stateId*/, Character* character, CharStateMachine* /*machine*/, int /*fromState*/,
                       int /*toState*/, void* /*data*/) {
    ReadTraceSwitch();
    // partial: *(Character +0x520) = 0 omitted (see above).
    character->SetVisible(false);
    // partial: respawn timer branch omitted (see above).
    character->CharAI::AI_ClearAllAggro();
}

// 0x3c7cc0 CSLimbus::OnInit(int, Character*, CharStateMachine*)
// Registers event 47 for this state with the Character CSM_Spawn handler and next state 1. The first
// argument is the state id that the state machine passes in.
void CSLimbus::OnInit(int stateId, Character* character, CharStateMachine* /*machine*/) {
    character->CharStateMachine::SM_RegisterEvent(stateId, 47, 1, &Character::CSM_Spawn);
}

// 0x3c006c _ZN10CSPreSpawnD1Ev
// Complete-object destructor. The body is empty (asm: BX LR).
CSPreSpawn::~CSPreSpawn() {}

// 0x3c0070 CSPreSpawn::OnUpdate(int, Character*, CharStateMachine*)
// Empty body (asm: BX LR).
void CSPreSpawn::OnUpdate(int, Character*, CharStateMachine*) {}

// 0x3c0720 _ZN10CSPreSpawnD0Ev
// skipped: compiler-generated deleting destructor. It stores the CharState vtable into the object and
// then calls CustomFree(this). delete on a CSPreSpawn produces it, so no C++ body is written.

// 0x3c2810 CSPreSpawn::OnEvent(int, Character*, CharStateMachine*, int, void*)
// partial. Event 9 (spawn): calls Character::CSM_Spawn(9, data, stateId, nextState) and, when it returns
// true with the next state still 1, calls SetSpawnState(delayed = true, flag = false). When the next state
// changed, the original asserts (level 2 stops, level 1 prints) with the source location of CSPreSpawn.h
// line 114.
// Event 40 ("is_interactive"): when the string matches, the original sets Character +0x520 |= 0x2000 and
// calls InitPhysicalObject().
// Missing 1: in the spawn case, when m_groupRole (CharAI +0x38, private) is 3, the group value (GroupInfo
// +0x24) is set to 0. Omitted (see the file comment).
// Missing 2: the Character +0x520 flag OR for event 40 (CharStateMachine::m_stateFlags, private). Omitted.
// The original return value (a pointer or the fprintf result) is discarded: the header declares void.
void CSPreSpawn::OnEvent(int stateId, Character* character, CharStateMachine* /*machine*/, int event, void* data) {
    if (event == 9) {
        int nextState = 1;
        const bool spawned = character->Character::CSM_Spawn(9, data, stateId, nextState);
        if (spawned) {
            if (nextState == 1) {
                character->CharStateMachine::SM_SetSpawnState(true, false);
                // partial: group write omitted (see above).
            } else {
                AssertFailed(
                    "!\"CSM_Spawn() changed the next state... needs to decide what to do with that case\"",
                    kCSPreSpawnFile, 114);
            }
        }
    } else if (event == 40) {
        if (std::strcmp(static_cast<const char*>(data), "is_interactive") == 0) {
            // partial: Character +0x520 |= 0x2000 omitted (see above).
            character->InitPhysicalObject();
        }
    }
}

// 0x3c67d4 CSPreSpawn::OnBlur(int, Character*, CharStateMachine*, int)
// Trace switch, SetVisible(true) through vtable slot +0x40 (asm: argument 1), Revive(nullptr, false), and
// EnableCollisions.
void CSPreSpawn::OnBlur(int /*stateId*/, Character* character, CharStateMachine* /*machine*/, int /*nextState*/) {
    ReadTraceSwitch();
    character->SetVisible(true);
    character->Character::Revive(nullptr, false);
    character->GameObject::EnableCollisions();
}

// 0x3c688c CSPreSpawn::OnFocus(int, Character*, CharStateMachine*, int, int, void*)
// partial. Written: the trace switch, SetPhysicalObject(nullptr, false), SetVisible(false) when
// m_aiStateVisible (Character +0x13e4) is 0 (asm: the byte is the argument, so the value is 0), and
// DisableCollisions.
// Missing 1: Character +0x520 = 0x1300 (CharStateMachine::m_stateFlags, private), written before the
// animation choice. Omitted.
// Missing 2: the animation choice. The row is Arrays::CharAnimTable::members + 160 * GetCharAnimTableId().
// When its field at +0x64 is -1, the original calls ANIM_Set(row +0x80 plus GetAnimStance() when
// PyDataConstants "AnimStancedAnim"/"SL__LIST_IPHONE" has bit 0 set, else plus 0) and ANIM_SetSpeed(0.0f).
// Otherwise it calls ANIM_Set(row +0x64). The row fields have no host struct layout (the pydata CharAnim
// record is a class with a vptr and placeholder names), so the choice is omitted.
void CSPreSpawn::OnFocus(int /*stateId*/, Character* character, CharStateMachine* /*machine*/, int /*fromState*/,
                         int /*toState*/, void* /*data*/) {
    ReadTraceSwitch();
    // partial: Character +0x520 = 0x1300 and the animation choice omitted (see above).
    character->GameObject::SetPhysicalObject(nullptr, false);
    if (!character->m_aiStateVisible) {
        character->SetVisible(false);
    }
    character->GameObject::DisableCollisions();
}

// 0x3c8d24 CSPreSpawn::OnInit(int, Character*, CharStateMachine*)
// Registers event 45 for this state with next state 1 and no handler (null pointer-to-member).
void CSPreSpawn::OnInit(int stateId, Character* character, CharStateMachine* /*machine*/) {
    character->CharStateMachine::SM_RegisterEvent(stateId, 45, 1, nullptr);
}
