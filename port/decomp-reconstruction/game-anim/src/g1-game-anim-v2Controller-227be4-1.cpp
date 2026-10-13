// Batch g1-game-anim-v2Controller-227be4-1: the second half of the v2Controller Cmd_* forwarders
// (Cmd_Zoom .. Cmd_Attack). Declarations come from dh2/v2Controller.hpp and dh2/v2Controllable.hpp.
//
// Every gated command first tests the block state: it runs when unk_009 (+0x09) is set, or when neither
// the global s_blocked nor m_blocked (+0x08) is set. A closed gate returns without forwarding.
// The Cmd_* that forward to the controllable call the Ctrl_* slot of the same name (see the comments).

#include "dh2/v2Controllable.hpp"
#include "dh2/v2Controller.hpp"
#include "net-session-online/c_online.hpp"

namespace {

// Block test shared by the gated Cmd_* functions (original: unk_009 || (!s_blocked && !m_blocked)).
bool CommandsEnabled(const v2Controller& controller) {
    return controller.unk_009 || (!v2Controller::s_blocked && !controller.m_blocked);
}

}  // namespace

// 0x405744 _ZN12v2Controller8Cmd_ZoomEf
// Not gated. Forwards to Ctrl_Zoom (vtable +0x60 of v2Controllable).
void v2Controller::Cmd_Zoom(float amount) {
    m_controllable->Ctrl_Zoom(amount);
}

// 0x405760 _ZN12v2Controller13Cmd_ResetZoomEv
// Not gated. Forwards to Ctrl_ResetZoom (vtable +0x64).
void v2Controller::Cmd_ResetZoom() {
    m_controllable->Ctrl_ResetZoom();
}

// 0x40577c _ZN12v2Controller19Cmd_TranslateMapCamEff
// Not gated. Forwards both offsets to Ctrl_TranslateMapCam (vtable +0x68).
void v2Controller::Cmd_TranslateMapCam(float dx, float dy) {
    m_controllable->Ctrl_TranslateMapCam(dx, dy);
}

// 0x405798 _ZN12v2Controller14Cmd_DropObjectEv
// Gated. The original calls GetOnline() and discards its result, then calls Ctrl_DropObject (vtable +0x54).
// GetOnline() is a thunk (0x7fd794 is a branch to COnline::GetInstance at 0x7fd744), so the call is written
// as the thunk's target.
void v2Controller::Cmd_DropObject() {
    if (!CommandsEnabled(*this)) {
        return;
    }
    COnline::GetInstance();
    m_controllable->Ctrl_DropObject();
}

// 0x4057fc _ZN12v2Controller10Cmd_UseOOIEP10GameObject
// PARTIAL. The gate and the final forward to Ctrl_UseOOI (vtable +0x50) are rebuilt. Not rebuilt: the online
// branch that runs before the forward. It needs (1) the online-game flag, COnline +0x05 (isOnlineGame_), which
// c_online.hpp keeps private with no getter (Application::IsOnlineGame, 0x320668, is a stub); (2) the per-controller
// flag at +0x0a, which is in the header; (3) the target: the argument, or else Character::m_objectOfInterest
// (+0x14a4, a GameObject*, read as the default target by the asm); (4) the merchant test on the target
// (GameObject::IsCharacter, vtable +0x24, then Character::IsMerchant) and the GetInteractionType (vtable +0x90)
// comparisons, whose meaning is not recovered; (5) the CMsgControllerAction payload (action 5) at +0x50/+0x52/+0x54,
// which c_msg_fixed.hpp keeps opaque, and CMessaging::Get()->SendMsg.
// Layout requests are listed in the ledger notes.
void v2Controller::Cmd_UseOOI(GameObject* target) {
    if (!CommandsEnabled(*this)) {
        return;
    }
    // MISSING: online branch described above.
    m_controllable->Ctrl_UseOOI(target);
}

// 0x405954 _ZN12v2Controller12Cmd_EndSkillEj
// PARTIAL. The gate and the final forward to Ctrl_EndSkill (vtable +0x40) are rebuilt. Not rebuilt: the online
// branch that runs before the forward. When the online-game flag (COnline +0x05, no getter, see Cmd_UseOOI) and
// the per-controller flag at +0x0a are set, and m_character is non-null, the original sends a
// CMsgControllerAction with action code 2, the skill id, and the low byte of Character unk_108 (+0x108),
// through CMessaging::Get()->SendMsg. The payload fields are not declared in c_msg_fixed.hpp (layout request).
void v2Controller::Cmd_EndSkill(unsigned int skill) {
    if (!CommandsEnabled(*this)) {
        return;
    }
    // MISSING: online branch described above.
    m_controllable->Ctrl_EndSkill(skill);
}

// 0x405a20 _ZN12v2Controller14Cmd_BeginSkillEj
// PARTIAL. The gate and the final forward to Ctrl_BeginSkill (vtable +0x3c) are rebuilt. Not rebuilt: the online
// branch. When the online-game flag (COnline +0x05, no getter) and the per-controller flag at +0x0a are set and
// m_character is non-null, the original asks CharAI::AI_IsSkillUsable(skill) on the character's CharAI block;
// if that is true it sends a CMsgControllerAction with action code 1, the skill id, and the low byte of
// Character unk_108, through CMessaging::Get()->SendMsg. The payload fields are not declared (layout request).
void v2Controller::Cmd_BeginSkill(unsigned int skill) {
    if (!CommandsEnabled(*this)) {
        return;
    }
    // MISSING: online branch described above.
    m_controllable->Ctrl_BeginSkill(skill);
}

// 0x405b04 _ZN12v2Controller10Cmd_AttackEP10GameObject
// PARTIAL. The gate and the final forward to Ctrl_Attack (vtable +0x38) are rebuilt. Not rebuilt: the online
// branch, which is the whole of the non-forward work. It needs the online-game flag (COnline +0x05, no getter).
// Inside the branch: the character's current and previous targets (CharAI m_target at +0x40 and m_lastTarget at
// +0x44 of the CharAI block, Character +0x408 and +0x40c), CharAI::AI_DoMeleeAttack, the attacking flag
// (CharAI m_attacking, Character +0x440), CharStateMachine::SM_IsAttacking, AI_SetTarget and AI_SyncLastTarget,
// and a CMsgControllerAction with action code 0 carrying the target's unk_108 low 16 bits. The message payload
// fields are not declared (layout request). Every path of the original ends in the forward below.
void v2Controller::Cmd_Attack(GameObject* target) {
    if (!CommandsEnabled(*this)) {
        return;
    }
    // MISSING: online branch described above.
    m_controllable->Ctrl_Attack(target);
}
