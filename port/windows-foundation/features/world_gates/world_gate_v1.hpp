#pragma once
// Preview 17 CONDITIONS: gated world declarations (Door, AnimatedDecor, TriggerObject, QuestMoveInZone, ...)
// as persistent world objects. Each gate is a neutral WorldObject with its authored stable ID, the same
// binding the containers use, so the GameSave object roster carries its state. No schema change.
//
// Component "dh2.world-gate.v1" (1 byte, 0/1) = the condition value the owner LAST APPLIED. The owner
// (a door/decor runtime, not wired yet) is called only when that value changes:
//   - a live flip (ConditionWatchV1) applies the new value and persists it;
//   - a bind after load reconciles the persisted value with the re-derived condition. Equal values
//     replay nothing; a different value is applied once.
// The owner interprets the value by the gate key: unlock_cond true = unlocked, activate_cond true =
// active, deactivate_cond true = inactive.
#include "../../playable_actor_world.hpp"
#include "../conditions/condition_watch_v1.hpp"

#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <vector>

namespace dh::foundation::world_gates {

inline constexpr const char* world_gate_component_v1 = "dh2.world-gate.v1";

struct WorldGateV1 {
    std::uint64_t stable_id{};  // authored identity (ActorDefinition::stableId)
    std::string gametype;       // Door, AnimatedDecor, TriggerObject, QuestMoveInZone, ...
    std::string name;           // authored declaration name
    std::string key;            // activate_cond | deactivate_cond | unlock_cond
    std::string condition;      // authored condition name
    bool applied{};             // value the owner last applied (persisted)
};

using WorldGateOwnerV1 = std::function<void(const WorldGateV1& gate, bool value)>;

// Binds every gate (idempotent per stable ID: an existing object keeps its persisted component).
// `current` answers the authored condition's value now (false + no value = condition unavailable).
bool bind_world_gates_v1(PlayableActorWorld& world, std::vector<WorldGateV1>& gates,
                         const std::function<bool(const std::string& condition, bool& value)>& current,
                         const WorldGateOwnerV1& owner, std::vector<std::string>& notices,
                         std::string& error);

// Applies live condition flips to the gates that read them, then persists each applied value.
bool apply_world_gate_flips_v1(PlayableActorWorld& world, std::vector<WorldGateV1>& gates,
                               const std::vector<conditions::ConditionFlipV1>& flips,
                               const WorldGateOwnerV1& owner, std::string& error);

// ObjectBase::TestEnableCondition and TestDisableCondition are separate,
// ordered source calls. TestEnable sets the object to the condition result
// (an absent activate list is true: IsTrue=1, so it enables);
// TestDisable only disables when its condition is false. Unknown values fail
// without changing the retained object's current enable state.
bool source_test_enable_condition_v1(bool has_condition, std::optional<bool> value,
                                     bool& enabled, std::string& error);
bool source_test_disable_condition_v1(bool has_condition, std::optional<bool> value,
                                      bool& enabled, std::string& error);

// ObjectManager::Update loop (0x52835 region) gate + TestEnableCondition(true) + TestDisableCondition(true).
// ConditionData::IsTrue (0x33e5f8) is true when its "tested" latch is set or its list is empty, so:
//  - an empty activate list enables the object (SetEnable(true)); it is not a no-op;
//  - a true enable test latches (SetAsTested) and stays true for the rest of the load;
//  - a false deactivate test disables and latches, so later flips do not re-enable the object.
// is_demo_only (ObjectBase byte 241) forces the enable test false. Latches are not persisted
// (ObjectBase::Deserialize 0x33e0f8 resets them); only the enable byte is. min_difficulty is not modeled.
struct SourceConditionLatchV1 { bool enable_tested{}; bool disable_tested{}; };
bool source_condition_lifecycle_step_v1(bool has_enable, std::optional<bool> enable_value,
                                        bool has_disable, std::optional<bool> disable_value,
                                        bool demo_only, SourceConditionLatchV1& latch,
                                        bool& enabled, std::string& error);

} // namespace dh::foundation::world_gates
