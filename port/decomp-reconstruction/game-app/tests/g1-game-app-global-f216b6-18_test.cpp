// Test for batch g1-game-app-global-f216b6-18. Checks the fields the reconstructed factories set, and that GetPosition
// returns zero. The factories covered are the script commands (Script_SetCameraClip, Script_PlayCamera, Script_SetCamera,
// Script_CONSOLE, Script_ExitCutSceneMode, Script_EnterCutSceneMode, Script_ExecScript, Script_PlayActorAnim), the
// conditions (Condition_IsEventInState, Condition_IsPlayerAtLevel, Condition_IsLevelInState) and the objectives
// (Objective_GatherLoot, ObjectiveTemplate_KillCharacter over Objective_SavedQty, Objective_MoveInZone). The partial
// factories (the quest-state conditions, Condition_IsPlayerInLevel) and Script_CharacterState_Callback are not covered.
// Exit status 0 means every check passed.
//
// The batch source is linked into this test (check_tu.sh --test), so every vtable, implicit constructor and destructor
// it references must be defined here. The test-only stubs below stand in for the methods that other categories define.
// Each class's key function (its first out-of-line virtual) is one of them, so this test emits that class's vtable, and
// every out-of-line virtual the vtable names is defined too. The stubs do nothing and return zero or false. The objects
// are leaked, not deleted.
#include <cstddef>
#include <cstdio>
#include <string>

#include "dh2/global/GetNewScriptCmdImplInstance.hpp"
#include "dh2/global/condition_objective_factory.hpp"
#include "dh2/global/get_position.hpp"

#include "dh2/Condition.hpp"
#include "dh2/Condition_IsEventInState.hpp"
#include "dh2/Condition_IsLevelInState.hpp"
#include "dh2/Condition_IsPlayerAtLevel.hpp"
#include "dh2/ObjectHandle.hpp"
#include "dh2/Objective.hpp"
#include "dh2/ObjectiveTemplate_KillCharacter.hpp"
#include "dh2/Objective_EventReceiver.hpp"
#include "dh2/Objective_GatherLoot.hpp"
#include "dh2/Objective_MoveInZone.hpp"
#include "dh2/Objective_Qty.hpp"
#include "dh2/Objective_SavedQty.hpp"
#include "dh2/Script_CONSOLE.hpp"
#include "dh2/Script_EnterCutSceneMode.hpp"
#include "dh2/Script_ExecScript.hpp"
#include "dh2/Script_ExitCutSceneMode.hpp"
#include "dh2/Script_PlayActorAnim.hpp"
#include "dh2/Script_PlayCamera.hpp"
#include "dh2/Script_SetCamera.hpp"
#include "dh2/Script_SetCameraClip.hpp"

// ---- Test-only link stubs -------------------------------------------------------------------------------------------

ScriptCmdImpl::~ScriptCmdImpl() {}
void ScriptCmdImpl::Init() {}
void ScriptCmdImpl::Update() {}

void Script_SetCameraClip::Execute(bool, int) {}
bool Script_SetCameraClip::IsBlocking() const { return false; }
void Script_PlayCamera::Execute(bool, int) {}
bool Script_PlayCamera::IsBlocking() const { return false; }
void Script_SetCamera::Execute(bool, int) {}
bool Script_SetCamera::IsBlocking() const { return false; }
void Script_CONSOLE::Execute(bool, int) {}
bool Script_CONSOLE::IsBlocking() const { return false; }
void Script_ExitCutSceneMode::Execute(bool, int) {}
bool Script_ExitCutSceneMode::IsBlocking() const { return false; }
void Script_EnterCutSceneMode::Execute(bool, int) {}
bool Script_EnterCutSceneMode::IsBlocking() const { return false; }
void Script_ExecScript::Execute(bool, int) {}
bool Script_ExecScript::IsBlocking() const { return false; }
void Script_PlayActorAnim::Init() {}
void Script_PlayActorAnim::Execute(bool, int) {}
bool Script_PlayActorAnim::IsBlocking() const { return false; }

ObjectHandle::ObjectHandle() {}

Condition::Condition() {}
Condition::~Condition() {}
void Condition::Compile() {}
std::size_t Condition::DBG_TraceDetailedConditionInformation(__sFILE*) { return 0; }

Condition_IsEventInState::~Condition_IsEventInState() {}
bool Condition_IsEventInState::Eval() { return false; }
std::size_t Condition_IsEventInState::DBG_TraceDetailedConditionInformation(__sFILE*) { return 0; }
Condition_IsPlayerAtLevel::~Condition_IsPlayerAtLevel() {}
bool Condition_IsPlayerAtLevel::Eval() { return false; }
std::size_t Condition_IsPlayerAtLevel::DBG_TraceDetailedConditionInformation(__sFILE*) { return 0; }
Condition_IsLevelInState::~Condition_IsLevelInState() {}
bool Condition_IsLevelInState::Eval() { return false; }
std::size_t Condition_IsLevelInState::DBG_TraceDetailedConditionInformation(__sFILE*) { return 0; }

Objective::Objective() {}
Objective::~Objective() {}
void Objective::Compile() {}
int Objective::DBG_TraceDetailedObjectiveInformation(__sFILE*) const { return 0; }
void Objective::InstallObjectiveMarker(int, int) {}
void Objective::RemoveObjectiveMarker() {}
std::string Objective::GetDesc() const { return std::string(); }
void Objective::_loadData(IStreamBase*) {}
void Objective::_saveData(IStreamBase*) {}
void Objective::_resetData() {}

Objective_EventReceiver::~Objective_EventReceiver() {}
int Objective_EventReceiver::DBG_TraceDetailedObjectiveInformation(__sFILE*) const { return 0; }
void Objective_EventReceiver::Register() {}
void Objective_EventReceiver::Unregister() {}
bool Objective_EventReceiver::onEvent(const IEvent*, const EventManager*) { return false; }

Objective_Qty::~Objective_Qty() {}

Objective_SavedQty::~Objective_SavedQty() {}
void Objective_SavedQty::_loadData(IStreamBase*) {}
void Objective_SavedQty::_saveData(IStreamBase*) {}
void Objective_SavedQty::_resetData() {}

Objective_GatherLoot::~Objective_GatherLoot() {}
void Objective_GatherLoot::Compile() {}
int Objective_GatherLoot::DBG_TraceDetailedObjectiveInformation(__sFILE*) const { return 0; }
void Objective_GatherLoot::Register() {}
void Objective_GatherLoot::Unregister() {}
void Objective_GatherLoot::GetPositions(Vector3DFList&) const {}
bool Objective_GatherLoot::handleEvent(const IEvent*, const EventManager*) { return false; }

Objective_MoveInZone::~Objective_MoveInZone() {}
void Objective_MoveInZone::Compile() {}
int Objective_MoveInZone::DBG_TraceDetailedObjectiveInformation(__sFILE*) const { return 0; }
void Objective_MoveInZone::GetPositions(Vector3DFList&) const {}
bool Objective_MoveInZone::handleEvent(const IEvent*, const EventManager*) { return false; }

// ObjectiveTemplate_KillCharacter is a template, so its stubs are templated and then explicitly instantiated for the one
// instantiation the batch uses.
template <typename TStruct, typename TEvent, typename TTest, typename TBase>
ObjectiveTemplate_KillCharacter<TStruct, TEvent, TTest, TBase>::~ObjectiveTemplate_KillCharacter() {}
template <typename TStruct, typename TEvent, typename TTest, typename TBase>
void ObjectiveTemplate_KillCharacter<TStruct, TEvent, TTest, TBase>::Compile() {}
template <typename TStruct, typename TEvent, typename TTest, typename TBase>
int ObjectiveTemplate_KillCharacter<TStruct, TEvent, TTest, TBase>::DBG_TraceDetailedObjectiveInformation(
    __sFILE*) const {
    return 0;
}
template <typename TStruct, typename TEvent, typename TTest, typename TBase>
void ObjectiveTemplate_KillCharacter<TStruct, TEvent, TTest, TBase>::GetPositions(Vector3DFList&) const {}
template <typename TStruct, typename TEvent, typename TTest, typename TBase>
bool ObjectiveTemplate_KillCharacter<TStruct, TEvent, TTest, TBase>::handleEvent(const IEvent*, const EventManager*) {
    return false;
}
template class ObjectiveTemplate_KillCharacter<Structs::v2QuestClearEnemyTemplate, QE_ClearEnemyTemplate, TestCharTemplate,
                                               Objective_SavedQty>;

namespace {

using KillCharacterQty = ObjectiveTemplate_KillCharacter<Structs::v2QuestClearEnemyTemplate, QE_ClearEnemyTemplate,
                                                         TestCharTemplate, Objective_SavedQty>;

int g_failures = 0;

void Check(bool ok, const char* name, const char* what) {
    if (!ok) {
        std::printf("FAIL: %s: %s\n", name, what);
        ++g_failures;
    }
}

// Script command factories: the command is a new instance of its own class with unk_04 cleared, m_cmdId = -1 and
// m_args null. Returns the command as its own class, or nullptr.
template <class T>
T* CheckScriptCmd(const char* name) {
    T* cmd = dynamic_cast<T*>(GetNewScriptCmdImplInstance<T>());
    Check(cmd != nullptr, name, "returns a command of its own class");
    if (cmd == nullptr) {
        return nullptr;
    }
    Check(!cmd->unk_04, name, "unk_04 is cleared");
    Check(cmd->m_cmdId == -1, name, "m_cmdId is -1");
    Check(cmd->m_args == nullptr, name, "m_args is null");
    return cmd;
}

void CheckScriptFactories() {
    CheckScriptCmd<Script_SetCameraClip>("Script_SetCameraClip");
    CheckScriptCmd<Script_PlayCamera>("Script_PlayCamera");
    CheckScriptCmd<Script_SetCamera>("Script_SetCamera");
    CheckScriptCmd<Script_CONSOLE>("Script_CONSOLE");
    CheckScriptCmd<Script_ExitCutSceneMode>("Script_ExitCutSceneMode");
    CheckScriptCmd<Script_EnterCutSceneMode>("Script_EnterCutSceneMode");
    CheckScriptCmd<Script_PlayActorAnim>("Script_PlayActorAnim");

    Script_ExecScript* exec = CheckScriptCmd<Script_ExecScript>("Script_ExecScript");
    if (exec != nullptr) {
        Check(exec->unk_10 == 0, "Script_ExecScript", "unk_10 is zero");
    }
}

// Condition factories: the condition is a new instance of its own class with m_stub null.
template <class T>
void CheckCondition(const char* name) {
    T* cond = dynamic_cast<T*>(GetConditionImplInstance<T>());
    Check(cond != nullptr, name, "returns a condition of its own class");
    if (cond != nullptr) {
        Check(cond->m_stub == nullptr, name, "m_stub is zero");
    }
}

void CheckConditionFactories() {
    CheckCondition<Condition_IsEventInState>("Condition_IsEventInState");
    CheckCondition<Condition_IsPlayerAtLevel>("Condition_IsPlayerAtLevel");
    CheckCondition<Condition_IsLevelInState>("Condition_IsLevelInState");
}

void CheckObjectiveFactories() {
    Objective_GatherLoot* gather = dynamic_cast<Objective_GatherLoot*>(GetObjectiveImplInstance<Objective_GatherLoot>());
    Check(gather != nullptr, "Objective_GatherLoot", "returns an objective of its own class");
    if (gather != nullptr) {
        Check(gather->m_currentQty == 0, "Objective_GatherLoot", "m_currentQty is zero");
    }

    KillCharacterQty* kill = dynamic_cast<KillCharacterQty*>(GetObjectiveImplInstance<KillCharacterQty>());
    Check(kill != nullptr, "ObjectiveTemplate_KillCharacter", "returns an objective of its own class");
    if (kill != nullptr) {
        Check(kill->m_currentQty == 0, "ObjectiveTemplate_KillCharacter", "m_currentQty is zero");
        Check(kill->m_questRow == nullptr, "ObjectiveTemplate_KillCharacter", "m_questRow is null");
        Check(kill->m_requiredQty == 0, "ObjectiveTemplate_KillCharacter", "m_requiredQty is zero");
    }

    Objective_MoveInZone* move = dynamic_cast<Objective_MoveInZone*>(GetObjectiveImplInstance<Objective_MoveInZone>());
    Check(move != nullptr, "Objective_MoveInZone", "returns an objective of its own class");
    if (move != nullptr) {
        Check(move->m_object == nullptr, "Objective_MoveInZone", "m_object is null");
    }
}

void CheckGetPosition() {
    Vector3D p = GetPosition();
    Check(p.X == 0.0f, "GetPosition", "X is zero");
    Check(p.Y == 0.0f, "GetPosition", "Y is zero");
    Check(p.Z == 0.0f, "GetPosition", "Z is zero");
}

}  // namespace

int main() {
    CheckScriptFactories();
    CheckConditionFactories();
    CheckObjectiveFactories();
    CheckGetPosition();

    if (g_failures == 0) {
        std::printf("g1-game-app-global-f216b6-18: all checks passed\n");
    }
    return g_failures == 0 ? 0 : 1;
}
