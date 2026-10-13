// CSDespawn and CSInjured bodies (batch g1-game-character-pack2-fa21c9-0): the despawn and injured character states.
// Focus, blur and init hooks are written from the Hex-Rays pseudocode and checked against the assembly for the two blur
// hooks. The update and event hooks are empty, and the base destructors are empty. The deleting destructors are
// compiler-generated and are not written (skipped). Declarations come from dh2/CSDespawn.hpp and dh2/CSInjured.hpp; the
// Character layout comes from dh2/Character.hpp.
//
// PARTIAL functions say at the point of the gap what is missing. The ledger rows in
// game-character/ledger.d/g1-game-character-pack2-fa21c9-0.csv repeat it. There are two gaps, both outside this unit:
//   - COnline +0x05 is isOnlineGame_ (private in net-session-online/c_online.hpp, with only a setter). OnlineGameFlag
//     reads false until the field has a getter, so the online branches of CSDespawn::OnBlur run as offline (the same
//     stand-in as CSIdle). Layout request filed.
//   - The local player's object of interest is cleared through PlayerManager::GetLocalPlayer(0, true)->unk_660 (a
//     Character* at PlayerInfo +0x660). dh2/PlayerManager.hpp cannot be included with dh2/Character.hpp, because
//     net-transport's i_stream_base.hpp and pydata-accessors' stream_reader.hpp both define IStreamBase. The stub
//     ClearLocalPlayerObjectOfInterest stands in until that include conflict is resolved.

#include "dh2/CSDespawn.hpp"
#include "dh2/CSInjured.hpp"

#include <map>

#include "dh2/Character.hpp"
#include "dh2/CharAnimator.hpp"
#include "dh2/CharStateMachine.hpp"
#include "dh2/DebugSwitches.hpp"
#include "dh2/ObjectBase.hpp"
#include "dh2/global/net_singletons.hpp"
#include "dh2/v2Controller.hpp"

namespace {

// Runs the tracing-switch probe that the focus and blur hooks perform. The original calls DebugSwitches::load on the
// singleton, builds the switch name as a std::string and reads the switch. The bool is discarded, as in the original.
void ProbeSwitch(const char* name) {
    DebugSwitches::s_inst.load();
    (void)DebugSwitches::s_inst.GetSwitch(name);
}

// PARTIAL. COnline::isOnlineGame_ (+0x05) is private with no getter. Reads false until it is public, so the online
// branches of CSDespawn::OnBlur run as if the game were offline.
bool OnlineGameFlag(const COnline*) { return false; }

// PARTIAL. The original's local-player block in CSDespawn::OnBlur reads PlayerInfo +0x660 (the local player's Character)
// and, when its m_objectOfInterest (Character +0x14a4) is this character, clears that field. The stub does nothing until
// dh2/PlayerManager.hpp can be included with dh2/Character.hpp.
void ClearLocalPlayerObjectOfInterest(Character*) {}

}  // namespace

// ---------------------------------------------------------------------------------------------------------------------
// CSDespawn
// ---------------------------------------------------------------------------------------------------------------------

// 0x3bfff4 CSDespawn::~CSDespawn()
// Empty body (4-byte bx lr). No member cleanup appears, and the base destructor runs implicitly.
CSDespawn::~CSDespawn() {}

// 0x3bfff8 CSDespawn::OnUpdate(int,Character*,CharStateMachine*)
// Empty body (4-byte bx lr). The arguments are not read.
void CSDespawn::OnUpdate(int, Character*, CharStateMachine*) {}

// 0x3bfffc CSDespawn::OnEvent(int,Character*,CharStateMachine*,int,void*)
// Empty body (4-byte bx lr). The arguments are not read.
void CSDespawn::OnEvent(int, Character*, CharStateMachine*, int, void*) {}

// 0x3c0a2c CSDespawn::~CSDespawn() (deleting destructor, vtable slot 0x04)
// skipped: compiler-generated. It stores off_9663A8 into the vptr and calls CustomFree(this). delete on a CSDespawn
// produces it, so no body is written here.

// 0x3c32fc CSDespawn::OnFocus(int,Character*,CharStateMachine*,int,int,void*)
// Traces two switches, then sets the despawn flags and animation. The original reads the dead animation (Character +0x534,
// CharStateMachine +0x38) before it writes the state flags and the pending animation (+0x520 and +0x524). The machine
// argument is not read: the writes go to the CharStateMachine base of the character, as in the original's (char*)a3 + 1276.
void CSDespawn::OnFocus(int, Character* character, CharStateMachine*, int, int, void*) {
    CharStateMachine* machine = character;
    ProbeSwitch("isTracingCharState");
    ProbeSwitch("isTracingCSDespawn");
    const int deadAnim = machine->m_deadAnim;      // +0x534 (CSM +0x38)
    machine->m_stateFlags = 512u;                  // +0x520 (CSM +0x24)
    machine->m_pendingAnim = deadAnim;             // +0x524 (CSM +0x28)
    machine->SM_SetAnim(-1);
    character->CancelSneaking();
}

// 0x3c3794 CSDespawn::OnBlur(int,Character*,CharStateMachine*,int)
// PARTIAL. Two gaps (see the file comment): the online flag reads false (OnlineGameFlag), and the local player's
// object-of-interest clear is a stub (ClearLocalPlayerObjectOfInterest). The rest is written in the original's order.
// The assembly (0x3c3794) confirms the block condition, the Delete path and the unconditional tail calls.
void CSDespawn::OnBlur(int, Character* character, CharStateMachine*, int) {
    CharStateMachine* machine = character;
    ProbeSwitch("isTracingCharState");
    character->m_controller->m_blocked = false;     // v2Controller +0x08, reached through Character +0x378
    machine->m_limbusFlag = true;                   // +0x530 (CSM +0x34)

    if (!character->CanRespawn() && (!OnlineGameFlag(GetOnline()) || !character->IsPlayer())) {
        ClearLocalPlayerObjectOfInterest(character);  // PARTIAL
        if (character->m_isNetSpawned != 0 || character->IsSummoned()) {
            static_cast<ObjectBase*>(character)->Delete();
        }
    }

    // The original clears the byte at +0x118 (ObjectBase::unk_118) when the online flag is set. The flag is the stand-in
    // above, so this write is not made in an online game.
    if (OnlineGameFlag(GetOnline())) {
        character->unk_118 = 0;
    }
    machine->m_stateFlags = 0;                      // +0x520 (CSM +0x24)

    CharAnimator* animator = character;             // the original's (char*)a3 + 1180
    animator->DecAnimSetUsers();
    animator->RequestAnimPurge();

    // The iterator argument is &Character::s_concurrentAI, which is the map's header node, that is, end(). The flag is false.
    character->UnLoadScriptProcess(Character::s_concurrentAI.end(), false);
}

// 0x3c7dfc CSDespawn::OnInit(int,Character*,CharStateMachine*)
// Registers two events on the character's CharStateMachine base (the original's (char*)a3 + 1276), with null handlers.
// The state's return value is the second call's return, which is void in the header, so it is dropped.
void CSDespawn::OnInit(int stateId, Character* character, CharStateMachine*) {
    CharStateMachine* machine = character;
    machine->SM_RegisterEvent(stateId, 64, 0, nullptr);
    machine->SM_RegisterEvent(stateId, 34, 0, nullptr);
}

// ---------------------------------------------------------------------------------------------------------------------
// CSInjured
// ---------------------------------------------------------------------------------------------------------------------

// 0x3c003c CSInjured::~CSInjured()
// Empty body (4-byte bx lr). No member cleanup appears, and the base destructor runs implicitly.
CSInjured::~CSInjured() {}

// 0x3c0040 CSInjured::OnUpdate(int,Character*,CharStateMachine*)
// Empty body (4-byte bx lr). The arguments are not read.
void CSInjured::OnUpdate(int, Character*, CharStateMachine*) {}

// 0x3c0044 CSInjured::OnEvent(int,Character*,CharStateMachine*,int,void*)
// Empty body (4-byte bx lr). The arguments are not read.
void CSInjured::OnEvent(int, Character*, CharStateMachine*, int, void*) {}

// 0x3c0858 CSInjured::~CSInjured() (deleting destructor, vtable slot 0x04)
// skipped: compiler-generated. It stores off_9663A8 into the vptr and calls CustomFree(this). delete on a CSInjured
// produces it, so no body is written here.

// 0x3c33e8 CSInjured::OnFocus(int,Character*,CharStateMachine*,int,int,void*)
// Traces two switches, then sets the injured flags and animation. The machine argument is not read (see CSDespawn::OnFocus).
void CSInjured::OnFocus(int, Character* character, CharStateMachine*, int, int, void*) {
    CharStateMachine* machine = character;
    ProbeSwitch("isTracingCharState");
    ProbeSwitch("isTracingCSInjured");
    machine->m_stateFlags = 11073u;                 // +0x520 (CSM +0x24), 0x2b41
    machine->SM_SetAnim(-1);
    character->CancelSneaking();
}

// 0x3c4ba4 CSInjured::OnBlur(int,Character*,CharStateMachine*,int)
// Traces one switch, then orders the controller to look at the character's target. The controller is Character +0x378
// (v2Controllable::m_controller) and the target is Character +0x408, which is CharAI::m_target (CharAI +0x40). The original
// has no null check on either (the assembly at 0x3c4ba4 loads both and calls Cmd_LookAt unconditionally). The return
// value of Cmd_LookAt is void, so the declared void OnBlur keeps the original's behaviour.
void CSInjured::OnBlur(int, Character* character, CharStateMachine*, int) {
    ProbeSwitch("isTracingCharState");
    character->m_controller->Cmd_LookAt(character->m_target);
}

// 0x3c8850 CSInjured::OnInit(int,Character*,CharStateMachine*)
// Registers five events on the character's CharStateMachine base, in the original's order. All handlers are null.
void CSInjured::OnInit(int stateId, Character* character, CharStateMachine*) {
    CharStateMachine* machine = character;
    machine->SM_RegisterEvent(stateId, 34, 3, nullptr);
    machine->SM_RegisterEvent(stateId, 50008, 12, nullptr);
    machine->SM_RegisterEvent(stateId, 50010, 11, nullptr);
    machine->SM_RegisterEvent(stateId, 50011, 10, nullptr);
    machine->SM_RegisterEvent(stateId, 50005, 6, nullptr);
}
