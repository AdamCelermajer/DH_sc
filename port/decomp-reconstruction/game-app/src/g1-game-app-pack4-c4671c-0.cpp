// Batch g1-game-app-pack4-c4671c-0 (ScriptCmdImpl and the Script_* commands of the AI, timer and trigger group,
// 20 ELF addresses, 20 bodies). Bodies follow the Hex-Rays pseudocode of libDungeonHunter2.so, checked against the
// assembly (decomp_asm.py) for the empty bodies, the IsBlocking returns, the trace probe and the destructor.
//
// What is reconstructed and what is not:
//   - ScriptCmdImpl::Init and Update, the empty Execute bodies of Script_AIChangeScript, Script_AIDoSkill,
//     Script_AIEnableTimer, Script_AIResetTimer and Script_AISetHP, and every IsBlocking in this batch (all return
//     false) are complete.
//   - Script_AISetInt::Execute, Script_ActivateTriggerPlate::Execute and Script_AutoEquip::Execute are PARTIAL. Each
//     starts with the isTracingScriptCmd trace probe, which is reproduced. The rest reads the command's argument record
//     (ScriptCmdImpl::m_args, +0x0c), which has no class or layout in the tree, so the object lookup and the effect on
//     the object are not written. See the layout requests.
//   - ~ScriptCmdImpl is PARTIAL for the same reason: the argument record is not released.
//
// The D2 and D1 destructors (0x4559f8, 0x455a68) have identical bodies, so each pair is one C++ definition.

#include <string>

#include "dh2/DebugSwitches.hpp"
#include "dh2/ScriptCmdImpl.hpp"
#include "dh2/Script_AIChangeScript.hpp"
#include "dh2/Script_AIDoSkill.hpp"
#include "dh2/Script_AIEnableTimer.hpp"
#include "dh2/Script_AIResetTimer.hpp"
#include "dh2/Script_AISetHP.hpp"
#include "dh2/Script_AISetInt.hpp"
#include "dh2/Script_ActivateTriggerPlate.hpp"
#include "dh2/Script_AutoEquip.hpp"

namespace {

// The trace probe that the original runs first in Script_AISetInt::Execute, Script_ActivateTriggerPlate::Execute and
// Script_AutoEquip::Execute (the asm of 0x45f2fc, 0x45d1d8 and 0x45db88 is the same sequence): DebugSwitches::load(),
// then GetSwitch("isTracingScriptCmd") with the result discarded. GetInstance() calls load() before it returns the
// singleton, which is the same sequence (see game-app/src/g1-game-app-DebugSwitches-d73440-0.cpp). DebugSwitches::s_inst
// is private in dh2/DebugSwitches.hpp, so GetInstance() supplies the object.
void TraceScriptCmdSwitch() {
    (void)DebugSwitches::GetInstance()->GetSwitch(std::string("isTracingScriptCmd"));
}

}  // namespace

// 0x455628 ScriptCmdImpl::Init()
// Empty body (asm: BX LR). Slot +0x00 of the ScriptCmdImpl vtable.
void ScriptCmdImpl::Init() {}

// 0x45562c ScriptCmdImpl::Update()
// Empty body (asm: BX LR). Slot +0x04 of the ScriptCmdImpl vtable.
void ScriptCmdImpl::Update() {}

// 0x4559f8 ScriptCmdImpl::~ScriptCmdImpl() (D2 base-object destructor) and 0x455a68 (D1 complete-object destructor)
// PARTIAL. The original sets the vptr to ScriptCmdImpl, then, when m_args (+0x0c) is set, calls its vtable slot +0x08
// with the record as this, re-reads m_args, calls its vtable slot +0x04 (the deleting destructor) when it is still set,
// and stores null in m_args. The record's class is not in the tree, so neither virtual call can be written and the
// record is not released here. Layout request: ScriptCmdImpl +0x0c m_args.
ScriptCmdImpl::~ScriptCmdImpl() {}

// 0x4557cc Script_AIChangeScript::Execute(bool, int)
// Empty body (asm: BX LR). The original does nothing.
void Script_AIChangeScript::Execute(bool /*skipping*/, int /*module*/) {}

// 0x4557d0 Script_AIChangeScript::IsBlocking() const
// Returns false (asm: MOV R0,#0; BX LR).
bool Script_AIChangeScript::IsBlocking() const { return false; }

// 0x4557b4 Script_AIDoSkill::Execute(bool, int)
// Empty body (asm: BX LR). The original does nothing.
void Script_AIDoSkill::Execute(bool /*skipping*/, int /*module*/) {}

// 0x4557b8 Script_AIDoSkill::IsBlocking() const
// Returns false.
bool Script_AIDoSkill::IsBlocking() const { return false; }

// 0x45579c Script_AIEnableTimer::Execute(bool, int)
// Empty body (asm: BX LR). The original does nothing.
void Script_AIEnableTimer::Execute(bool /*skipping*/, int /*module*/) {}

// 0x4557a0 Script_AIEnableTimer::IsBlocking() const
// Returns false.
bool Script_AIEnableTimer::IsBlocking() const { return false; }

// 0x4557a8 Script_AIResetTimer::Execute(bool, int)
// Empty body (asm: BX LR). The original does nothing.
void Script_AIResetTimer::Execute(bool /*skipping*/, int /*module*/) {}

// 0x4557ac Script_AIResetTimer::IsBlocking() const
// Returns false.
bool Script_AIResetTimer::IsBlocking() const { return false; }

// 0x4557c0 Script_AISetHP::Execute(bool, int)
// Empty body (asm: BX LR). The original does nothing, so the HP is not set by this command.
void Script_AISetHP::Execute(bool /*skipping*/, int /*module*/) {}

// 0x4557c4 Script_AISetHP::IsBlocking() const
// Returns false.
bool Script_AISetHP::IsBlocking() const { return false; }

// 0x4557d8 Script_AISetInt::IsBlocking() const
// Returns false.
bool Script_AISetInt::IsBlocking() const { return false; }

// 0x45f2fc Script_AISetInt::Execute(bool, int)
// PARTIAL. Reproduced: the isTracingScriptCmd trace probe. Missing: the rest of the body. The original reads the object
// name from the argument record (m_args +0x14, const char*), looks it up with ObjectManager::GetObjectByName(name, module,
// false, nullptr) on the manager at Application +0x38 (ApplicationGetInstance().m_objectManager), and calls
// ObjectHandle::GetObject(false). When the object's IsCharacter() (ObjectBase vtable slot +0x24) is true, it reads the
// LuaScript pointer at object +0x3e4 and, when that is set, calls LuaScript::SetInt(script, name, value) with the record's
// +0x0c (const char*, the Lua variable name) and +0x18 (int). Hex-Rays types the +0x3e4 pointer as LuaScript*, but
// CharAI declares that slot (CharAI +0x1c, Character +0x3e4) as CharAIScript*, private, and CharAIScript has no header.
// Waits on the argument record type (layout request) and on the script pointer's type (layout request).
void Script_AISetInt::Execute(bool /*skipping*/, int /*module*/) {
    TraceScriptCmdSwitch();
}

// 0x4558cc Script_ActivateTriggerPlate::IsBlocking() const
// Returns false.
bool Script_ActivateTriggerPlate::IsBlocking() const { return false; }

// 0x45d1d8 Script_ActivateTriggerPlate::Execute(bool, int)
// PARTIAL. Reproduced: the isTracingScriptCmd trace probe. Missing: the rest of the body. The original looks up the object
// named by the argument record (m_args +0x0c, const char*) with GetObjectByName(name, module, false, nullptr) on the manager
// at Application +0x38, and calls ObjectHandle::GetObject(false). When the object's m_goId (ObjectBase +0xf4) is 18, it
// stores the object in m_triggerPlate (+0x10, declared) and sets a byte at object +0x779 to 1. Otherwise m_triggerPlate
// becomes null. The GO_IDS 18 object is a TriggerPlate (GetNewInstance<TriggerPlate> at 0x340ee8, global/object_factory.hpp),
// and +0x779 is past ObjectBase's 0x120 bytes, so the flag belongs to TriggerPlate. dh2/TriggerPlate.hpp is not in the tree.
// Waits on the argument record type (layout request) and on dh2/TriggerPlate.hpp (flag +0x779).
void Script_ActivateTriggerPlate::Execute(bool /*skipping*/, int /*module*/) {
    TraceScriptCmdSwitch();
}

// 0x4557f0 Script_AutoEquip::IsBlocking() const
// Returns false.
bool Script_AutoEquip::IsBlocking() const { return false; }

// 0x45db88 Script_AutoEquip::Execute(bool, int)
// PARTIAL. Reproduced: the isTracingScriptCmd trace probe. Missing: the rest of the body. The original looks up the object
// named by the argument record (m_args +0x0c, const char*) with GetObjectByName(name, module, false, nullptr) on the
// manager at Application +0x38, and calls ObjectHandle::GetObject(false). When that object is set, it converts the handle
// with ObjectHandle::operator Character*() and, when the result is non-null, calls Character::EquipAllSlotsAuto() through
// vtable slot +0x138 (dh2/Character.hpp, declared at +0x138). Waits on the argument record type (layout request).
void Script_AutoEquip::Execute(bool /*skipping*/, int /*module*/) {
    TraceScriptCmdSwitch();
}
