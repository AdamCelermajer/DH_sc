#pragma once

#include "../../combat_session.hpp"

#include <array>
#include <cstdint>
#include <map>
#include <optional>
#include <string>
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

// The object registry and typed source owner are borrowed synchronously. Unknown
// reached predicates reject; no Character sheets or IDs are fabricated.
struct SkillObjectTargetFactsV1 {
    std::optional<bool> visible, zonable, zoned, in_zone, interactive;
    std::optional<int> interaction_type;
    std::optional<float> interaction_radius;
};
struct SkillObjectTargetArmV1 {
    std::vector<ObjectId> authored_order;
    std::vector<ObjectId> registry_order; // actual mixed actor/object enrollment order, when supplied
    std::function<bool(const WorldObject&, ActorId, SkillObjectTargetFactsV1&, std::string&)> facts;
    std::function<bool(CombatSession&, ObjectId, ActorId, std::string&)> interact;
};

enum class SkillActorTargetQueryStatusV1 : unsigned char {
    unavailable,
    selected_character,
    selected_object,
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
    std::vector<ObjectId> ordered_targets; // exact mixed source heap pop order
    std::vector<ObjectId> ordered_object_targets;
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
// represented by this legacy Character-only query; if there are no eligible character rows,
// the caller must establish that no AttackableOnly object can be selected
// before treating the source Lua local as nil.
bool query_bashdown_character_targets_v1(
    const CombatSession&, ActorId caster,
    const std::vector<ActorId>& authored_actor_order,
    const std::vector<SkillTargetSourceFactsV1>& source_facts,
    std::optional<bool> non_character_attackable_objects_absent,
    SkillActorTargetQueryV1&, std::string& error);

// v21.8 mage: TargetListSearch(range, angle, <GameObject>) / TargetListSearch(range, angle, x, y, z): the Lua skills that
// search around another object or point (ChainLightning around the struck victim, the StoneEscape statue, the Cyclone
// tornado). Only the search origin changes: position, look vector and the searcher's own AI_GetMeleeRadius. The
// Enemy/AttackableOnly filters stay relative to the casting actor (inference: the Lua sets them once for the caster).
struct SkillTargetOriginV1 {
    std::array<float, 3> position{};
    std::array<float, 3> look{0.0f, -1.0f, 0.0f};
    float radius = 0.0f;
    ActorId exclude = invalid_actor_id;  // the searcher when it is itself an actor
};

// General source TargetListSearch; optional typed arm includes real world objects. `sort` follows the
// original heap values (0 NoSort, 1 ClosestFirst, 2 FrontalFirst); cone is in
// radians and a value >= pi preserves the full authored radius. It uses the
// same active population order and same source property/AI facts as BashDown.
bool query_source_character_targets_v1(
    const CombatSession&, ActorId caster,
    const std::vector<ActorId>& authored_actor_order,
    const std::vector<SkillTargetSourceFactsV1>& source_facts,
    std::optional<bool> non_character_attackable_objects_absent,
    float range, float cone_radians, SkillTargetSortV1 sort,
    SkillActorTargetQueryV1&, std::string& error,
    const SkillObjectTargetArmV1* objects = nullptr,
    const SkillTargetOriginV1* search_origin = nullptr);

// Source GameObject::LookAt(Point) leaf on this same session actor. This does
// not write ActorState::target_id: BashDown keeps its selected target in a Lua
// local. The native command's controller admission gates are outside the
// generic Session API and remain an integration uncertainty.
bool look_at_bashdown_target_v1(CombatSession&, SkillActorTargetQueryV1&,
                                std::string& error);

// ---- v21.8 ES-API: actor-side TargetList arms used by the authored skill Lua. ----

// Lua AITargetList_CharacterFilter.Player = 128 (atlas ai_pycst.bin, proven). Character::_SetTargetListCharacterFilter
// 0x390690 stores the raw number; TargetListSearch 0x38fcf0 selects the PlayerList source when bit 0x80 is set and
// the default population otherwise. The numeric value of "Enemy" is not in the atlas (INFERENCE: it is not 0x80).
inline constexpr std::int32_t kSkillCharacterFilterPlayerV1 = 128;

// TargetListBackup(name) (0x390bd4) stores the current ordered list; TargetListSearch(range, angle, name) (0x38fcf0)
// searches only that stored list. A missing or non-string name uses "default". A missing name reads as an empty list
// (the native std::map operator[] inserts an empty backup).
class SkillTargetListBackupsV1 {
public:
    static constexpr const char* kDefaultName = "default";
    void backup(const std::string& name, std::vector<ActorId> ordered_targets);
    std::vector<ActorId> get(const std::string& name) const;
private:
    std::map<std::string, std::vector<ActorId>> lists_;
};

// TargetListSearch(R, A, dx, dy, dz, true) relative origin (0x38fcf0 five-argument branch): the origin is the caster
// moved along its look vector by `forward`. Lateral and vertical offsets are not modelled (no authored row uses them;
// the DeathClaw row passes 0, 0). Returns the caster position unchanged when forward is 0.
std::array<float, 3> relative_forward_search_origin_v1(const std::array<float, 3>& caster_position,
                                                       const std::array<float, 3>& look, float forward);

// Source TargetListSearch(range, cone) with SetTargetListCharacterFilter(Player): the living allied characters
// (IsPlayer) other than the caster, inside range and cone, in the requested sort order. `origin` overrides the search
// centre (radius stays the caster's). INFERENCE: the Player population is not gated by the Enemy visibility/zone facts,
// which apply only to the Enemy/AttackableOnly population.
bool query_player_character_targets_v1(
    const PlayableActorWorld& world, ActorId caster,
    const std::vector<ActorId>& authored_actor_order,
    float range, float cone_radians, SkillTargetSortV1 sort,
    const std::optional<std::array<float, 3>>& origin,
    SkillActorTargetQueryV1& output, std::string& error);

// Source TargetListSearch(range, cone, "<name>"): candidates are the stored backup list in its stored order; only
// range, cone and sort apply (the backup was already filtered by the search that produced it).
bool query_backup_character_targets_v1(
    const PlayableActorWorld& world, ActorId caster,
    const std::vector<ActorId>& backup_order,
    float range, float cone_radians, SkillTargetSortV1 sort,
    const std::optional<std::array<float, 3>>& origin,
    SkillActorTargetQueryV1& output, std::string& error);

// Presentation the authored Lua performs during a skill (PlaySound / PlayFX). The coordinator records them in
// authored order on the cast receipt; the host dispatches them (sound by label, FX by AnimatedEffectTable set name).
struct SkillPresentationCueV1 {
    enum class Kind : std::uint8_t { sound, self_fx, target_fx };
    Kind kind = Kind::sound;
    std::string name;                      // sound label, or AnimatedEffectTable set name
    ActorId actor = invalid_actor_id;      // FX owner (caster for self_fx, victim for target_fx); invalid for sound
};

} // namespace dh::foundation::generic_skills
