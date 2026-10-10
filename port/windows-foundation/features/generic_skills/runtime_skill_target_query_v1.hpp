#pragma once

#include "../../combat_session.hpp"

#include <optional>
#include <vector>

namespace dh::foundation::generic_skills {

// BashDown's Search walks source actors in the order supplied by the active
// population. ActorState does not carry these GameObject query facts, so the
// caller must pass actual same-actor values when they are available. Unknown
// reached values fail; they are never treated as visible/interactable.
struct SkillTargetSourceFactsV1 {
    ActorId actor = invalid_actor_id;
    std::optional<bool> visible;
    std::optional<bool> zonable;
    std::optional<bool> zoned;
    std::optional<bool> in_zone;
    std::optional<bool> interactive;
};

enum class SkillActorTargetQueryStatusV1 : unsigned char {
    unavailable,
    selected_character,
    no_actor_target,
    non_character_target_unknown
};

enum class SkillTargetSortV1 : std::uint8_t {
    source_order = 0,
    closest_first = 1,
    frontal_first = 2
};

struct SkillActorTargetQueryV1 {
    SkillActorTargetQueryStatusV1 status = SkillActorTargetQueryStatusV1::unavailable;
    ActorId caster = invalid_actor_id;
    ActorId selected = invalid_actor_id;
    std::vector<ActorId> ordered_character_targets;
    float selected_distance = 0.0f;
    float selected_angle = 0.0f;
    float look_at_heading = 0.0f;
    bool look_at_heading_known = false;
    bool source_character_order_preserved = false;
};

// Replays the character subset of BashDown's Enemy/AttackableOnly,
// FrontalFirst, RANGE=160 TargetListSearch over the same generic Session.
// `authored_actor_order` is the active actor order from the caller's source
// population, not an ActorId sort. Non-character destructible objects are not
// represented by PlayableActorWorld; if there are no eligible character rows,
// the caller must establish that no AttackableOnly object can be selected
// before treating the source Lua local as nil.
bool query_bashdown_character_targets_v1(
    const CombatSession&, ActorId caster,
    const std::vector<ActorId>& authored_actor_order,
    const std::vector<SkillTargetSourceFactsV1>& source_facts,
    std::optional<bool> non_character_attackable_objects_absent,
    SkillActorTargetQueryV1&, std::string& error);

// General source TargetListSearch Character subset. `sort` follows the
// original heap values (0 NoSort, 1 ClosestFirst, 2 FrontalFirst); cone is in
// radians and a value >= pi preserves the full authored radius. It uses the
// same active population order and same source property/AI facts as BashDown.
bool query_source_character_targets_v1(
    const CombatSession&, ActorId caster,
    const std::vector<ActorId>& authored_actor_order,
    const std::vector<SkillTargetSourceFactsV1>& source_facts,
    std::optional<bool> non_character_attackable_objects_absent,
    float range, float cone_radians, SkillTargetSortV1 sort,
    SkillActorTargetQueryV1&, std::string& error);

// Source GameObject::LookAt(Point) leaf on this same session actor. This does
// not write ActorState::target_id: BashDown keeps its selected target in a Lua
// local. The native command's controller admission gates are outside the
// generic Session API and remain an integration uncertainty.
bool look_at_bashdown_target_v1(CombatSession&, SkillActorTargetQueryV1&,
                                std::string& error);

} // namespace dh::foundation::generic_skills
