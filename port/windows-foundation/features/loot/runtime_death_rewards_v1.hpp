#pragma once

#include "../../character_state.hpp"
#include "../../combat_system.hpp"
#include "../../../game-data/design_settings.hpp"
#include "../../../game-data/loot_entry_selection_v8.hpp"
#include "../../../game-data/loot_power_resources_v7.hpp"
#include "../../../game-data/loot_tables_v2.hpp"
#include <optional>
#include "../../original_actor_properties.hpp"
#include <functional>
#include <map>
#include <memory>
#include <string>

namespace dh::foundation { class CombatSession; }
namespace dh::foundation { class PlayableActorWorld; }

namespace dh::foundation::loot {

struct RuntimeDeathActorV1 {
    // A caller advances this token only when the same ActorId is bound to a
    // new source character lifetime (including an actual source revive).
    std::uint64_t binding_lifecycle{};
    std::shared_ptr<dh::foundation::CharacterState> character;
};

struct RuntimeWorldItemRecordV1 {
    dh::foundation::ActorId source_actor{dh::foundation::invalid_actor_id};
    dh::foundation::ActorId killer_actor{dh::foundation::invalid_actor_id};
    std::int32_t loot_table{-1};
    std::int16_t item_id{-1};
    std::uint8_t quantity{};
    const dh2::data::Item* authored_item{};
    const dh2::data::LootEntry32V2* authored_entry{};
    // Source AddLoot's explicit value-bonus argument, applied once at item
    // construction using the same Application loot RNG. Present only for
    // successfully valued type13 GoldStack rows.
    std::optional<std::int32_t> resolved_gold_value;
};

enum class RuntimeDeathRewardStateV1 { none, attempted, completed, failed };
// The native Level+0x150 word gates both Character::Kill loot and
// Character::AddExperience. The current gameplay owner supplies its current
// source value; the ordinary source Level constructor initializes it to zero.
struct RuntimeDeathRewardAdmissionV1 {
    bool rewards_suppressed{};
};
enum class RuntimeDeathRewardDiagnosticV1 {
    none,
    killer_properties_unavailable_for_gold_value
};
enum class RuntimeGoldValueBonusSourceV1 {
    not_used,
    absent_killer_zero_baseline,
    same_world_killer_property195,
    unavailable
};
struct RuntimeDeathRewardOutcomeV1 {
    dh::foundation::ActorId victim{dh::foundation::invalid_actor_id};
    RuntimeDeathRewardStateV1 state{RuntimeDeathRewardStateV1::none};
    std::size_t selected_items{};
    std::size_t spawned_items{};
    std::size_t unsupported_gold_items{};
    std::uint32_t xp_recipients{};
    bool rewards_suppressed{};
    RuntimeDeathRewardDiagnosticV1 diagnostic{RuntimeDeathRewardDiagnosticV1::none};
    RuntimeGoldValueBonusSourceV1 gold_bonus_source{RuntimeGoldValueBonusSourceV1::not_used};
};

struct RuntimeDeathRewardServicesV1 {
    RuntimeDeathRewardAdmissionV1 admission{};
    // Existing immutable source authorities and the Application gameplay RNG.
    dh2::data::LootTablesV2::Borrow loot_tables;
    dh2::data::LootPowerResourcesV7::Borrow loot_powers;
    dh2::data::LootEntryServicesV8 loot_entry;
    dh2::data::LootRandom8V2* gameplay_rng{};
    const dh2::data::DesignSettingsProjection176* xp_design{};
    const OriginalPropertyDatabase* properties{};
    // Source-level inputs read by the current game context. `max_level` must
    // be the real CharacterDesign cap for `current_difficulty`; no default is
    // provided. `one_kill_level_up` is the actual debug policy value.
    std::int32_t current_difficulty{-1};
    std::int32_t unlocked_difficulty{-1};
    std::int32_t max_level{};
    bool one_kill_level_up{};
    // Binds an ActorId in the same session to its one canonical shared state.
    // NPCs may return a null state; player XP recipients may not.
    void* context{};
    // Optional source Debug owner query used at the original XP-award branch
    // (after max-level/player admission and before the XP property mutation).
    bool (*query_one_kill_level_up)(void*, bool&, std::string&){};
    bool (*resolve_character)(void*, dh::foundation::ActorId,
                              RuntimeDeathActorV1&, std::string&){};
    // Presentation sidecar fired once after a real level gain (source
    // Character::LevelUp: FX set 135, MENU_LEVEL_UP status). Called with the
    // reached level; a failure is returned to the award (see caller).
    bool (*on_level_up)(void*, dh::foundation::ActorId, std::int32_t,
                        std::string&){};
    // Generic world-item creation/publication is an explicit receiver; this
    // router retains no second item pool. It receives an actual original Loot
    // table selection with borrowed Item/Loot row identities.
    bool (*spawn_world_item)(void*, const RuntimeWorldItemRecordV1&,
                            const dh::foundation::ActorState&,
                            const dh::foundation::ActorState*,
                            std::string&){};
};

// Consumes one CombatSession event batch. It accepts only applied target_died
// events, selects original loot entries with the caller RNG, awards source XP
// using the existing pure progression formula kernels and mutates the same
// CharacterState/property/world authorities. Prefix failures are terminal for
// the actor lifetime, so a retry cannot duplicate loot or XP.
class RuntimeDeathRewardsV1 {
public:
    // P16 QUESTS: a quest XP reward through the same award path as kill XP (level-up included).
    bool award_experience(dh::foundation::PlayableActorWorld& world, dh::foundation::ActorId player,
                          float xp, const RuntimeDeathRewardServicesV1& services, std::string& error);
    bool consume(dh::foundation::CombatSession&,
                 const RuntimeDeathRewardServicesV1&,
                 std::vector<RuntimeDeathRewardOutcomeV1>&,
                 std::string& error);
    // Shared core for headless source-event verification. Production should
    // call consume(session), which forwards exactly session.events().
    bool consume_events(dh::foundation::CombatSession&,
                        const std::vector<dh::foundation::DamageEvent>&,
                        const RuntimeDeathRewardServicesV1&,
                        std::vector<RuntimeDeathRewardOutcomeV1>&,
                        std::string& error);
    // The session entry point is a strict wrapper over this shared world-owned
    // path; it is exposed so focused tests can exercise real actor/property
    // owners without fabricating a CombatSession animation backend.
    bool consume_events(dh::foundation::PlayableActorWorld&,
                        const std::vector<dh::foundation::DamageEvent>&,
                        const RuntimeDeathRewardServicesV1&,
                        std::vector<RuntimeDeathRewardOutcomeV1>&,
                        std::string& error);
    void clear() noexcept { receipts_.clear(); }
private:
    struct Receipt {
        std::uint64_t lifecycle{};
        const dh::foundation::CharacterState* state_identity{};
        std::shared_ptr<dh::foundation::CharacterState> state_lease;
        RuntimeDeathRewardStateV1 state{RuntimeDeathRewardStateV1::none};
        bool rewards_suppressed{};
    };
    std::map<dh::foundation::ActorId, Receipt> receipts_;
};

} // namespace dh::foundation::loot
