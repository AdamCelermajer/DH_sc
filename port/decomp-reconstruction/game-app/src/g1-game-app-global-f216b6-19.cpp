// Batch g1-game-app-global-f216b6-19 (<global>, part 20 of 22): the quest objective factories
// GetObjectiveImplInstance<T>() (10), the quest reward factories GetRewardImplInstance<T>() (5), and the
// scene-node helpers SetWireFrame, SetLodBiasOnAllTextures, SetNodeToSelfIlluminated, SetNodeVisibleRecur and
// IsNodeMesh.
// Declarations: dh2/global/GetObjectiveImplInstance.hpp, dh2/global/GetRewardImplInstance.hpp and
// dh2/global/SceneNodeHelpers.hpp.

#include "dh2/global/GetObjectiveImplInstance.hpp"
#include "dh2/global/GetRewardImplInstance.hpp"
#include "dh2/global/SceneNodeHelpers.hpp"

#include "dh2/Objective_Automatic.hpp"
#include "dh2/Objective_SavedQty.hpp"
#include "dh2/Objective_TalkToNPC.hpp"
#include "dh2/ObjectiveTemplate_InteractWith.hpp"
#include "dh2/ObjectiveTemplate_KillCharacter.hpp"
#include "dh2/ObjectiveTemplate_KillEnemies.hpp"

#include "dh2/Reward_CharacterProps.hpp"
#include "dh2/Reward_ConsumeLoot.hpp"
#include "dh2/Reward_Gold.hpp"
#include "dh2/Reward_Loot.hpp"
#include "dh2/Reward_XP.hpp"

// ---------------------------------------------------------------------------------------------------------------
// Objective factories. Each allocates one objective of its class and returns it as an Objective*. new T() zeroes the
// whole object before the constructors run, so every word the original zeroes is zero here too. That includes
// m_registered (+0x1c) and the other words the original factory never writes; those were left to the allocator and
// are zero here. The explicit stores below are the non-zero ones, m_objectId = -1.
// ---------------------------------------------------------------------------------------------------------------

namespace {

// Shared body of the five ObjectiveTemplate_InteractWith instances. The original builds the object through
// Objective::Objective and then stores m_objectId (+0x24) = -1, which means no object has been matched yet.
template <typename TStruct, typename TEvent>
Objective* NewInteractWithObjective() {
    auto* objective = new ObjectiveTemplate_InteractWith<TStruct, TEvent>();
    objective->m_objectId = -1;
    return objective;
}

}  // namespace

// 0x47bf70 Objective* GetObjectiveImplInstance<Objective_Automatic>()
// Original: operator new(0x18), zeroes the 0x18 bytes, Objective::Objective, then the Objective_Automatic vptr.
template <>
Objective* GetObjectiveImplInstance<Objective_Automatic>() {
    return new Objective_Automatic();
}

// 0x47cec4 Objective* GetObjectiveImplInstance<Objective_TalkToNPC>()
// Original: operator new(0x2c), Objective::Objective, m_objectId (+0x24) = -1, m_currentQty (+0x20) = 0,
// m_objectiveMarker (+0x28) = null, the vptrs. The zero stores come from new; m_objectId is the explicit store.
template <>
Objective* GetObjectiveImplInstance<Objective_TalkToNPC>() {
    auto* objective = new Objective_TalkToNPC();
    objective->m_objectId = -1;
    return objective;
}

// 0x47cf20 Objective* GetObjectiveImplInstance<ObjectiveTemplate_InteractWith<Structs::v2QuestDestroyGameObject, QE_DestroyGameObject>>()
// Original: operator new(0x28), Objective::Objective, m_currentQty (+0x20) = 0, m_objectId (+0x24) = -1, the vptrs.
template <>
Objective* GetObjectiveImplInstance<ObjectiveTemplate_InteractWith<Structs::v2QuestDestroyGameObject, QE_DestroyGameObject>>() {
    return NewInteractWithObjective<Structs::v2QuestDestroyGameObject, QE_DestroyGameObject>();
}

// 0x47cf78 Objective* GetObjectiveImplInstance<ObjectiveTemplate_InteractWith<Structs::v2QuestTriggerPlate, QE_TriggerPlate>>()
// Original: the same stores as the DestroyGameObject instance (operator new(0x28), m_objectId = -1).
template <>
Objective* GetObjectiveImplInstance<ObjectiveTemplate_InteractWith<Structs::v2QuestTriggerPlate, QE_TriggerPlate>>() {
    return NewInteractWithObjective<Structs::v2QuestTriggerPlate, QE_TriggerPlate>();
}

// 0x47cfd0 Objective* GetObjectiveImplInstance<ObjectiveTemplate_KillCharacter<Structs::v2QuestClearEnemies, QE_ClearEnemies, TestCharPropId, Objective_SavedQty>>()
// Original: operator new(0x30), Objective::Objective, m_currentQty (+0x20), m_questRow (+0x24), pad (+0x28),
// m_requiredQty (+0x2c) all = 0, the vptrs. Every stored value is zero, so new is the whole factory.
template <>
Objective* GetObjectiveImplInstance<ObjectiveTemplate_KillCharacter<Structs::v2QuestClearEnemies, QE_ClearEnemies, TestCharPropId, Objective_SavedQty>>() {
    return new ObjectiveTemplate_KillCharacter<Structs::v2QuestClearEnemies, QE_ClearEnemies, TestCharPropId, Objective_SavedQty>();
}

// 0x47d02c Objective* GetObjectiveImplInstance<ObjectiveTemplate_KillEnemies<Structs::v2QuestKillEnemies, QE_KillEnemies, TestCharPropId, Objective_SavedQty>>()
// Original: the same zero stores as the ClearEnemies instance (operator new(0x30)).
template <>
Objective* GetObjectiveImplInstance<ObjectiveTemplate_KillEnemies<Structs::v2QuestKillEnemies, QE_KillEnemies, TestCharPropId, Objective_SavedQty>>() {
    return new ObjectiveTemplate_KillEnemies<Structs::v2QuestKillEnemies, QE_KillEnemies, TestCharPropId, Objective_SavedQty>();
}

// 0x47d088 Objective* GetObjectiveImplInstance<ObjectiveTemplate_KillEnemies<Structs::v2QuestKillEnemyTemplate, QE_KillEnemyTemplate, TestCharTemplate, Objective_SavedQty>>()
// Original: the same zero stores as the KillEnemies instance, with TestCharTemplate.
template <>
Objective* GetObjectiveImplInstance<ObjectiveTemplate_KillEnemies<Structs::v2QuestKillEnemyTemplate, QE_KillEnemyTemplate, TestCharTemplate, Objective_SavedQty>>() {
    return new ObjectiveTemplate_KillEnemies<Structs::v2QuestKillEnemyTemplate, QE_KillEnemyTemplate, TestCharTemplate, Objective_SavedQty>();
}

// 0x47d0e4 Objective* GetObjectiveImplInstance<ObjectiveTemplate_InteractWith<Structs::v2QuestPickedUpLiftable, QE_PickedUpLiftable>>()
// Original: the same stores as the DestroyGameObject instance (operator new(0x28), m_objectId = -1).
template <>
Objective* GetObjectiveImplInstance<ObjectiveTemplate_InteractWith<Structs::v2QuestPickedUpLiftable, QE_PickedUpLiftable>>() {
    return NewInteractWithObjective<Structs::v2QuestPickedUpLiftable, QE_PickedUpLiftable>();
}

// 0x47d13c Objective* GetObjectiveImplInstance<ObjectiveTemplate_InteractWith<Structs::v2QuestTriggerOn, QE_TriggerOn>>()
// Original: the same stores as the DestroyGameObject instance (operator new(0x28), m_objectId = -1).
template <>
Objective* GetObjectiveImplInstance<ObjectiveTemplate_InteractWith<Structs::v2QuestTriggerOn, QE_TriggerOn>>() {
    return NewInteractWithObjective<Structs::v2QuestTriggerOn, QE_TriggerOn>();
}

// 0x47d194 Objective* GetObjectiveImplInstance<ObjectiveTemplate_InteractWith<Structs::v2QuestOpenGameObject, QE_OpenGameObject>>()
// Original: the same stores as the DestroyGameObject instance (operator new(0x28), m_objectId = -1).
template <>
Objective* GetObjectiveImplInstance<ObjectiveTemplate_InteractWith<Structs::v2QuestOpenGameObject, QE_OpenGameObject>>() {
    return NewInteractWithObjective<Structs::v2QuestOpenGameObject, QE_OpenGameObject>();
}

// ---------------------------------------------------------------------------------------------------------------
// Reward factories. Each original zeroes every word of its object, calls Reward::Reward, then writes the derived
// vptr. new T() does the same: value-initialisation zeroes the object, then the implicit constructor runs Reward().
// ---------------------------------------------------------------------------------------------------------------

// 0x482d80 Reward* GetRewardImplInstance<Reward_ConsumeLoot>()
// Original allocates 0x14 bytes, one word short of the Reward base (+0x00..+0x17, see dh2/Reward.hpp); this factory
// zeroes +0x00..+0x10 and does not touch +0x14. Layout request: confirm the size. Host sizeof is used here.
template <>
Reward* GetRewardImplInstance<Reward_ConsumeLoot>() {
    return new Reward_ConsumeLoot();
}

// 0x482dd8 Reward* GetRewardImplInstance<Reward_Loot>()
template <>
Reward* GetRewardImplInstance<Reward_Loot>() {
    return new Reward_Loot();
}

// 0x482e34 Reward* GetRewardImplInstance<Reward_CharacterProps>()
// Original: operator new(0x1c), zeroes the 0x1c bytes (the Character pointer at +0x18 included), calls Reward::Reward,
// then writes the Reward_CharacterProps vptr. Same as new Reward_CharacterProps() (see dh2/Reward_CharacterProps.hpp).
template <>
Reward* GetRewardImplInstance<Reward_CharacterProps>() {
    return new Reward_CharacterProps();
}

// 0x482e94 Reward* GetRewardImplInstance<Reward_XP>()
template <>
Reward* GetRewardImplInstance<Reward_XP>() {
    return new Reward_XP();
}

// 0x482ef0 Reward* GetRewardImplInstance<Reward_Gold>()
template <>
Reward* GetRewardImplInstance<Reward_Gold>() {
    return new Reward_Gold();
}

// ---------------------------------------------------------------------------------------------------------------
// Scene-node helpers (glitch::scene::ISceneNode).
// ---------------------------------------------------------------------------------------------------------------

// 0x50a4c0 SetWireFrame(glitch::scene::ISceneNode *,bool)
// Empty in the original (a single return). Reconstructed as an empty body.
void SetWireFrame(glitch::scene::ISceneNode* node, bool wireframe) {
    (void)node;
    (void)wireframe;
}

// 0x50e394 SetLodBiasOnAllTextures(float)
// Empty in the original (a single return). Reconstructed as an empty body.
void SetLodBiasOnAllTextures(float bias) {
    (void)bias;
}

// 0x50e398 SetNodeToSelfIlluminated(glitch::scene::ISceneNode *)
// PARTIAL: stub returns 0. Original: calls the node's virtual at byte offset 0x88 with the node as the only argument
// (asm: R0 = node, LDR PC,[R3,#0x88]). That slot is ISceneNode::getMaterialCount() const (tools/decomp_vtable.py
// prints it at +0x94, 0x0c high for this class, see the layout request). The result stays in the return
// register: it is the return value when the child list is empty, and each child call overwrites it. Then the child list
// whose sentinel is at node+0xf4 is walked, and each child (child = element - 4) is passed to this function again.
// Missing: the virtual needs a vptr and a getMaterialCount() declared virtual in glitch-scene-collada-gui/scene_node.hpp
// (the header has neither), and the child-list container at +0xf4 is not declared in ISceneNode. Layout requests for both.
int SetNodeToSelfIlluminated(glitch::scene::ISceneNode* node) {
    (void)node;
    return 0;
}

// 0x50e41c SetNodeVisibleRecur(glitch::scene::ISceneNode *,bool)
// PARTIAL: a null node returns 0, as the original does. For a non-null node the original calls the node's virtual at
// byte offset 0x48 with (node, visible): the bool stays live in R1 across the call (asm: MOV R6,R1, then LDR PC,[R3,#0x48]
// with R1 never reloaded). That slot is ISceneNode::setVisible(bool) (tools/decomp_vtable.py prints it at +0x54,
// 0x0c high for this class). Then it recurses on each child of the sentinel-at-+0xf4 list with the same bool.
// The stub skips both steps. Missing: the child-list container at +0xf4 is not declared in ISceneNode, and the header
// glitch-scene-collada-gui/scene_node.hpp declares no setVisible (it has no vptr), so the call cannot be written from the
// current headers. Layout requests for the slot and the child list. The original's int result (the last R0 value) is not
// reproduced.
int SetNodeVisibleRecur(glitch::scene::ISceneNode* node, bool visible) {
    if (node == nullptr) {
        return 0;
    }
    (void)visible;
    return 0;
}

// 0x50e514 IsNodeMesh(glitch::scene::ISceneNode *)
// PARTIAL: stub returns false. Original: calls the node's virtual at byte offset 0xbc (tools/decomp_vtable.py prints
// it at +0xc8, 0x0c high for this class) and returns true when it returns 0x6D656164 ('mead').
// Otherwise it calls the same virtual again and returns true when that result is 0x73656164 ('sead'). That slot is
// ISceneNode::getType() const. The engine base getType() (0x597298, scene_node.hpp) returns 0x6e6b6e75, so the
// override is what the comparison needs. Partial: scene_node.hpp declares getType() non-virtual because the class has
// no vptr, so a plain node->getType() call would bind to the base implementation, not the node's own override. Layout
// request for the vptr and the virtual slot.
bool IsNodeMesh(glitch::scene::ISceneNode* node) {
    (void)node;
    return false;
}
