// Batch g1-game-app-global-f216b6-18 (<global>, part 19 of 22): the script command factories for the camera,
// cutscene, console, exec-script and actor-animation commands, the progression Condition factories, the quest
// Objective factories, Script_CharacterState_Callback and GetPosition.
// Declarations: dh2/global/GetNewScriptCmdImplInstance.hpp, dh2/global/condition_objective_factory.hpp,
// dh2/global/script_character_callback.hpp, dh2/global/get_position.hpp.

#include "dh2/global/GetNewScriptCmdImplInstance.hpp"
#include "dh2/global/condition_objective_factory.hpp"
#include "dh2/global/get_position.hpp"
#include "dh2/global/script_character_callback.hpp"

#include "dh2/Condition_IsEventInState.hpp"
#include "dh2/Condition_IsLevelInState.hpp"
#include "dh2/Condition_IsPlayerAtLevel.hpp"
#include "dh2/Script_CONSOLE.hpp"
#include "dh2/Script_EnterCutSceneMode.hpp"
#include "dh2/Script_ExecScript.hpp"
#include "dh2/Script_ExitCutSceneMode.hpp"
#include "dh2/Script_PlayActorAnim.hpp"
#include "dh2/Script_PlayCamera.hpp"
#include "dh2/Script_SetCamera.hpp"
#include "dh2/Script_SetCameraClip.hpp"

#include "dh2/Objective_GatherLoot.hpp"
#include "dh2/Objective_MoveInZone.hpp"
#include "dh2/Objective_SavedQty.hpp"
#include "dh2/ObjectiveTemplate_KillCharacter.hpp"

namespace {

// Shared allocation of the plain script commands. The original does operator new(size), stores 0 in unk_04 (+0x04) and
// m_args (+0x0c), stores the vptr, and stores -1 in m_cmdId (+0x08). Value-initialization of the new-expression
// zeroes the object before the implicit constructor runs, so the vptr store and the member defaults match.
template <class T>
T* NewScriptCmd() {
    auto* cmd = new T();
    cmd->unk_04 = false;
    cmd->m_cmdId = -1;
    cmd->m_args = nullptr;
    return cmd;
}

}  // namespace

// 0x458b90 ScriptCmdImpl* GetNewScriptCmdImplInstance<Script_SetCameraClip>()
template <>
ScriptCmdImpl* GetNewScriptCmdImplInstance<Script_SetCameraClip>() {
    return NewScriptCmd<Script_SetCameraClip>();
}

// 0x458bd8 ScriptCmdImpl* GetNewScriptCmdImplInstance<Script_PlayCamera>()
template <>
ScriptCmdImpl* GetNewScriptCmdImplInstance<Script_PlayCamera>() {
    return NewScriptCmd<Script_PlayCamera>();
}

// 0x458c20 ScriptCmdImpl* GetNewScriptCmdImplInstance<Script_SetCamera>()
template <>
ScriptCmdImpl* GetNewScriptCmdImplInstance<Script_SetCamera>() {
    return NewScriptCmd<Script_SetCamera>();
}

// 0x458c68 ScriptCmdImpl* GetNewScriptCmdImplInstance<Script_CONSOLE>()
template <>
ScriptCmdImpl* GetNewScriptCmdImplInstance<Script_CONSOLE>() {
    return NewScriptCmd<Script_CONSOLE>();
}

// 0x458cb0 ScriptCmdImpl* GetNewScriptCmdImplInstance<Script_ExitCutSceneMode>()
template <>
ScriptCmdImpl* GetNewScriptCmdImplInstance<Script_ExitCutSceneMode>() {
    return NewScriptCmd<Script_ExitCutSceneMode>();
}

// 0x458cf8 ScriptCmdImpl* GetNewScriptCmdImplInstance<Script_EnterCutSceneMode>()
template <>
ScriptCmdImpl* GetNewScriptCmdImplInstance<Script_EnterCutSceneMode>() {
    return NewScriptCmd<Script_EnterCutSceneMode>();
}

// 0x458d40 ScriptCmdImpl* GetNewScriptCmdImplInstance<Script_ExecScript>()
// The original allocates 0x14 bytes, the ScriptCmdImpl base plus unk_10 (+0x10), and stores 0 in unk_10.
template <>
ScriptCmdImpl* GetNewScriptCmdImplInstance<Script_ExecScript>() {
    Script_ExecScript* cmd = NewScriptCmd<Script_ExecScript>();
    cmd->unk_10 = 0;
    return cmd;
}

// 0x4594a8 void Script_CharacterState_Callback(char const*, int, void (*)(ObjectBase*))
// PARTIAL: empty body. The original does one of two things. When strcasecmp(name, "All") is non-zero, it resolves the
// name through ObjectManager::GetObjectByName (0x34aca0) on the global manager dword_99F764, reads the ObjectHandle's
// object with ObjectHandle::GetObject(false) (0x33fdc0), and calls the callback once with it. When the name is "All",
// it walks the manager's object list (sentinel at manager +0x2c, node payload at node +0x08) and calls the callback for
// each node. Waits on dh2/ObjectManager.hpp, which is not written yet, so neither the manager's fields nor its methods
// can be named.
void Script_CharacterState_Callback(const char* /*name*/, int /*arg*/, void (* /*callback*/)(ObjectBase*)) {}

// 0x4595c8 ScriptCmdImpl* GetNewScriptCmdImplInstance<Script_PlayActorAnim>()
// The original allocates 0x1c bytes, zeroes the fields, stores the vptr and then calls ObjectHandle::ObjectHandle on the
// m_actor member at +0x10. The new-expression value-initializes the object, and the implicit constructor default-constructs
// m_actor through ObjectHandle(), so the same handle constructor runs.
template <>
ScriptCmdImpl* GetNewScriptCmdImplInstance<Script_PlayActorAnim>() {
    return NewScriptCmd<Script_PlayActorAnim>();
}

// 0x47899c Condition* GetConditionImplInstance<Condition_IsEventInState>()
// The original allocates 8 bytes, zeroes m_stub (+0x04), runs Condition::Condition and stores the concrete vptr.
template <>
Condition* GetConditionImplInstance<Condition_IsEventInState>() {
    return new Condition_IsEventInState();
}

// 0x4789e8 Condition* GetConditionImplInstance<Condition_IsPlayerAtLevel>()
template <>
Condition* GetConditionImplInstance<Condition_IsPlayerAtLevel>() {
    return new Condition_IsPlayerAtLevel();
}

// 0x478a34 Condition* GetConditionImplInstance<Condition_IsLevelInState>()
template <>
Condition* GetConditionImplInstance<Condition_IsLevelInState>() {
    return new Condition_IsLevelInState();
}

// 0x478a80 Condition* GetConditionImplInstance<Condition_IsPlayerInLevel>()
// PARTIAL: stub returns nullptr. The original allocates 8 bytes, zeroes m_stub (+0x04), runs Condition::Condition and stores the
// vptr. Waits on dh2/Condition_IsPlayerInLevel.hpp, which does not compile: its DBG_TraceDetailedConditionInformation is declared
// int, but it overrides Condition::DBG_TraceDetailedConditionInformation, which returns std::size_t. The header is not owned by
// this batch. Fix the return type there, then restore the body as new Condition_IsPlayerInLevel().
template <>
Condition* GetConditionImplInstance<Condition_IsPlayerInLevel>() {
    return nullptr;
}

// 0x478acc Condition* GetConditionImplInstance<Condition_IsQuestStateHigher>()
// PARTIAL: stub returns nullptr. The original allocates 12 bytes, runs Condition::Condition and sets m_comparator (+0x08) to
// COMPARE_HIGHER (2). Waits on dh2/Condition_IsQuestInStateGeneric.hpp, which does not compile: its
// DBG_TraceDetailedConditionInformation is declared int against the std::size_t base (see GetConditionImplInstance<Condition_IsPlayerInLevel>).
template <>
Condition* GetConditionImplInstance<Condition_IsQuestStateHigher>() {
    return nullptr;
}

// 0x478b14 Condition* GetConditionImplInstance<Condition_IsQuestStateLower>()
// PARTIAL: stub returns nullptr. The original allocates 12 bytes, runs Condition::Condition and sets m_comparator (+0x08) to
// COMPARE_LOWER (1). Waits on dh2/Condition_IsQuestInStateGeneric.hpp (same DBG return-type conflict as Condition_IsQuestStateHigher).
template <>
Condition* GetConditionImplInstance<Condition_IsQuestStateLower>() {
    return nullptr;
}

// 0x478b5c Condition* GetConditionImplInstance<Condition_IsQuestInState>()
// PARTIAL: stub returns nullptr. The original allocates 12 bytes, runs Condition::Condition and sets m_comparator (+0x08) to
// COMPARE_EQUAL (0). Waits on dh2/Condition_IsQuestInStateGeneric.hpp (same DBG return-type conflict as Condition_IsQuestStateHigher).
template <>
Condition* GetConditionImplInstance<Condition_IsQuestInState>() {
    return nullptr;
}

// 0x47a3ac GetPosition()
// PARTIAL: the body is exact. The asm (MOV R2,#0; STR R2 to [R0,#8], [R0] and [R0,#4]; BX LR) zeroes the three 32-bit words
// behind the return slot. The return type is not evidenced: the symbol _Z11GetPositionv carries no return type, and the only
// reference is a data pointer at 0x33858 (no direct caller in the listing). Vector3D from core-platform/w7_global_00.hpp
// (three floats) is used for the 12-byte value, unconfirmed.
Vector3D GetPosition() {
    return Vector3D{0.0f, 0.0f, 0.0f};
}

// 0x47be74 Objective* GetObjectiveImplInstance<Objective_GatherLoot>()
// The original allocates 0x24 bytes, runs Objective::Objective, stores 0 in m_currentQty (+0x20), and stores the primary vptr
// (asm: class vtable + 8) and the IEventReceiver vptr at +0x18 (asm: class vtable + 0x4C). The new-expression runs the same
// Objective() constructor through the implicit chain and sets both vptrs. m_registered (+0x1c) is zeroed by value-initialization;
// the original does not write it.
template <>
Objective* GetObjectiveImplInstance<Objective_GatherLoot>() {
    auto* obj = new Objective_GatherLoot();
    obj->m_currentQty = 0;
    return obj;
}

// 0x47bec4 Objective* GetObjectiveImplInstance<ObjectiveTemplate_KillCharacter<Structs::v2QuestClearEnemyTemplate, QE_ClearEnemyTemplate, TestCharTemplate, Objective_SavedQty>>()
// The original allocates 0x30 bytes, runs Objective::Objective and stores 0 in m_currentQty (+0x20), m_questRow (+0x24),
// pad_28 (+0x28) and m_requiredQty (+0x2c), then stores the primary vptr (Hex-Rays off_969350) and the IEventReceiver vptr
// at +0x18 (off_969394). The new-expression builds the object through the implicit chain (Objective_SavedQty, Objective_Qty,
// Objective_EventReceiver, Objective), which runs Objective::Objective and sets both vptrs. Value-initialization zeroes the
// members the original zeroes (and m_registered at +0x1c, which the original does not write).
template <>
Objective* GetObjectiveImplInstance<ObjectiveTemplate_KillCharacter<Structs::v2QuestClearEnemyTemplate, QE_ClearEnemyTemplate, TestCharTemplate, Objective_SavedQty>>() {
    auto* obj = new ObjectiveTemplate_KillCharacter<Structs::v2QuestClearEnemyTemplate, QE_ClearEnemyTemplate, TestCharTemplate, Objective_SavedQty>();
    obj->m_currentQty = 0;
    obj->m_questRow = nullptr;
    obj->m_requiredQty = 0;
    return obj;
}

// 0x47bf20 Objective* GetObjectiveImplInstance<Objective_MoveInZone>()
// The original allocates 0x24 bytes, runs Objective::Objective, stores 0 at +0x20 (the m_object GameObject* of
// Objective_MoveInZone, null until Compile), and stores the primary vptr (Hex-Rays off_969700) and the IEventReceiver vptr at
// +0x18 (off_969744). Objective_MoveInZone derives from Objective_EventReceiver, not Objective_Qty, so there is no m_currentQty.
// The new-expression sets the same vptrs through the implicit chain, and value-initialization zeroes m_object.
template <>
Objective* GetObjectiveImplInstance<Objective_MoveInZone>() {
    auto* obj = new Objective_MoveInZone();
    obj->m_object = nullptr;
    return obj;
}
