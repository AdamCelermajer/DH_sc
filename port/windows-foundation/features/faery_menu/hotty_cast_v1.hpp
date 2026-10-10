#pragma once

#include "../../actor_state.hpp"
#include "../../character_state.hpp"
#include "../../playable_actor_world.hpp"
#include "../../../game-data/faery_tables.hpp"
#include "../../../game-data/class_tables.hpp"
#include "../../../game-data/properties.hpp"

#include <map>
#include <memory>
#include <optional>

namespace dh::foundation { class CombatSession; }
namespace dh::foundation::faery_menu {

// Runtime cooldown bookkeeping belongs to the same CombatSession lifetime.
// Call advance once, after each successful session.update, with that update's
// dt. A duplicate/skipped serial is rejected rather than advancing twice.
struct HottyCooldownClockV1 {
    std::uint64_t session_update_serial{};
    std::uint64_t elapsed_ms{};
    std::map<ActorId, std::uint64_t> spell_ready_at_ms;
    // Production/session overloads pin one host lifetime. Serial-only test
    // helpers intentionally have no lease and cannot be used for live casting.
    std::weak_ptr<const void> binding_lease;
    bool has_binding_lease{};
};

struct HottySourceActorFactsV1 {
    ActorId actor{invalid_actor_id};
    std::optional<bool> visible;
    std::optional<bool> zonable;
    std::optional<bool> zoned;
    std::optional<bool> in_zone;
    std::optional<bool> interactive;
};

struct HottyAuthoredActorOrderV1 {
    // The generic Session host supplies the complete active ActorPopulation
    // enrollment order. This is the Preview10 NoSort policy and makes no claim
    // about native intrusive World-registry pointer order.
    bool complete{};
    std::vector<ActorId> actor_ids;
};

enum class HottyTargetScopeV1 : std::uint8_t {
    unknown,
    current_character_population,
    source_object_targets_included
};

// A no-sort target result in the current generic Character population. It is
// complete only for this declared host scope; it does not claim that the
// original global source registry contains no GameObjects.
struct HottySourceTargetListV1 {
    bool query_complete{};
    bool authored_world_order_preserved{};
    HottyTargetScopeV1 scope{HottyTargetScopeV1::unknown};
    std::vector<ActorId> character_targets_in_source_order;
};

bool advance_hotty_cooldown_clock_v1(const CombatSession&, double update_dt_seconds,
                                     HottyCooldownClockV1&, std::string& error);
bool advance_hotty_cooldown_clock_v1(std::uint64_t session_update_serial,
                                     double update_dt_seconds,
                                     HottyCooldownClockV1&, std::string& error);

struct HottySourcePolicyV1 {
    bool complete{};
    // These are actual current gameplay-policy facts. All must be known even
    // when a particular branch would short-circuit before reading some facts.
    bool source_online{};
    bool has_controller{};
    bool saved_god_mana{};
    bool debug_god_mana{};
    bool character_god_mana{};
};

enum class HottyPrepareStatusV1 : std::uint8_t {
    rejected,
    prepared_pending_spell_combat,
    prepared_no_targets_pending_spell_combat
};

struct HottyPreparedCastV1 {
    HottyPrepareStatusV1 status{HottyPrepareStatusV1::rejected};
    ActorId caster{invalid_actor_id};
    std::int32_t difficulty{-1};
    std::int32_t faery_slot{-1};
    std::int32_t faery_record_id{-1};
    std::int32_t source_spell_type{-1};
    std::int32_t skill_level{};
    std::int32_t fixed_mana_cost{};
    std::int32_t range{};
    std::vector<ActorId> character_targets;
    std::uint64_t session_update_serial{};
    std::uint64_t cooldown_ready_at_ms{};
    std::int32_t spell_class_id{-1};
    // Prep-time projection for auditing the source row. Hotty applies this
    // class again at each OnSkill target after the OnPre UseMana prefix.
    dh2::data::PropertySheet spell_properties{};
    bool mana_debited{};
    bool original_target_order_known{};
};

// Pure execution of the literal Hotty Lua ToFixed/MulFixed expressions. The
// source's ToFixed(0.2) truncates to zero before <<8, so this is always the
// authored fixed 10/20 base for ranks 0/1.
bool hotty_mana_cost_v1(std::uint16_t saved_faery_level,
                        std::int32_t source_player_level_fixed,
                        std::int32_t& skill_level, std::int32_t& fixed_cost,
                        std::string& error);

// Legacy lower-level candidate helper. It has no source visibility/zone facts
// and iterates the generic map's ActorId order, so it is not source NoSort
// evidence and must not be used for production cast dispatch.
bool source_hotty_character_targets_v1(const PlayableActorWorld&, ActorId caster,
                                       std::int32_t range,
                                       std::vector<ActorId>& targets,
                                       std::string& error);

// Replays the current generic Session's Character-only subset of source
// TargetListSearch at 600/800 units. Iteration preserves active ActorPopulation
// enrollment order (NoSort); source facts are required at each reached gate.
bool query_hotty_character_targets_v1(
    const PlayableActorWorld&, ActorId caster,
    const HottyAuthoredActorOrderV1&,
    const std::vector<HottySourceActorFactsV1>&, std::int32_t range,
    HottySourceTargetListV1&, std::string& error);

// Runs the recovered Hotty OnSkillUpdate/Check/Pre prefix on one same-session
// player and CharacterState. Production dispatch must pass a complete exact
// current Character query from query_hotty_character_targets_v1; omitted
// exact_source_targets retains a test-only ActorId-map fallback and does not
// claim source order. GameObject rows remain outside this Character-only host
// scope. This returns a pending-combat prefix, never cast success.
bool prepare_hotty_spell_v1(
    CombatSession&, CharacterState&, std::int32_t difficulty,
    const dh2::data::FaeryTables::Borrow&, const dh2::data::ClassTables&,
    const dh2::data::PropertyRules&, const HottySourcePolicyV1&,
    HottyCooldownClockV1&, HottyPreparedCastV1&, std::string& error,
    const HottySourceTargetListV1* exact_source_targets = nullptr);

// Lower-level same-world entry used by focused tests and by a host adapter
// that has already resolved session.player_id()/session.world(). Production
// callers should prefer the CombatSession overload above.
bool prepare_hotty_spell_v1(
    PlayableActorWorld&, ActorId caster, std::uint64_t session_update_serial,
    CharacterState&, std::int32_t difficulty,
    const dh2::data::FaeryTables::Borrow&, const dh2::data::ClassTables&,
    const dh2::data::PropertyRules&, const HottySourcePolicyV1&,
    HottyCooldownClockV1&, HottyPreparedCastV1&, std::string& error,
    const HottySourceTargetListV1* exact_source_targets = nullptr);

} // namespace dh::foundation::faery_menu
