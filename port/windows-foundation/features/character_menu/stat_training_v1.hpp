#pragma once

#include "source_composition.hpp"
#include "../../character_state.hpp"
#include "../../combat_session.hpp"
#include "../../original_combat_properties.hpp"

#include <array>
#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <string>

namespace dh::foundation::character_menu {

// Shared stat/skill allocation of one Character menu visit. Original SWF
// AddedStatsThisTurn/useSkillPoint gate only the first pre-spend save; repeated
// spends are original behavior. Either kind prompts on exit; No reloads PROP
// and SKIL (ranks and slots), then recalculates against the current gear.
struct StatTrainingVisitV1 {
    std::array<std::uint32_t, 4> staged{}; // per stat: 0..3 = Strength, Dexterity, Endurance, Energy
    std::uint32_t staged_skills = 0;
    std::optional<CharacterState> baseline;
    dh2::data::PropertySheet baseline_saved{};
    const CombatSession* session_owner = nullptr;
    void open_visit() noexcept { staged = {}; staged_skills = 0; baseline.reset(); session_owner = nullptr; }
    std::uint32_t staged_total() const noexcept { return staged[0] + staged[1] + staged[2] + staged[3] + staged_skills; }
    bool has_staged() const noexcept { return staged_total() != 0; }
};

struct StatTrainingCommitV1 {
    std::uint32_t stat = 0;            // 0..3 = Strength, Dexterity, Endurance, Energy
    std::uint32_t previous_points = 0; // property 148 integer points before the spend
    std::uint32_t remaining_points = 0;
    std::int32_t previous_value = 0;   // property 149+stat integer value before/after
    std::int32_t current_value = 0;
    // Derived vitals before and after the class recalculation (integer display values).
    std::int32_t previous_max_health = 0, current_max_health = 0;
    std::int32_t previous_max_resource = 0, current_max_resource = 0;
};

// Persists the staged CharacterState before the live Session is published.
// A false return leaves both the Session and the canonical CharacterState unchanged.
using StatTrainingPersistV1 = std::function<bool(const CharacterState&, std::string&)>;

// Call after native admission and before the first mutation of either kind.
// Saves/snapshots the common authoritative baseline once. A failed prefix
// leaves no snapshot or staged allocation; later spends never overwrite it.
bool begin_allocation_visit_v1(const CharacterState& same_state, CombatSession& same_session,
    StatTrainingVisitV1& visit, const StatTrainingPersistV1& persist, std::string& error);

// Same-Session Stat_Points spend. Refuses (no mutation) when property 148 is
// zero, when the CharacterState points disagree with the live sheet, or when
// the stat index is outside 0..3. On success it debits property 148, adds one
// point to saved property 149+stat, resets base from the authored CharacterTable
// row (keeping the current base level, as the source profile projection does),
// runs the full class recalculation on the live actor, publishes the new sheet
// to the combat world, and mirrors points/stats/vitals into CharacterState.
// When persist is non-empty the staged CharacterState is saved before publishing;
// an empty persist stages the spend in memory (persisted on commit).
bool train_stat_in_session_v1(CharacterState& same_state, CombatSession& same_session,
    const OriginalPropertyDatabase& source_properties, std::uint32_t stat,
    const StatTrainingPersistV1& persist, StatTrainingCommitV1& output,
    std::string& error);

// Inverse of one spend (original NativeStatsRemoveAssign / Character::ResetStatsChange
// semantics): credits property 148, removes one point from property 149+stat,
// recalculates the same way. Refuses when that stat has no point above zero.
bool refund_stat_in_session_v1(CharacterState& same_state, CombatSession& same_session,
    const OriginalPropertyDatabase& source_properties, std::uint32_t stat,
    const StatTrainingPersistV1& persist, StatTrainingCommitV1& output,
    std::string& error);

// Confirm (Yes): persists the staged CharacterState once and clears the batch.
// A failed save leaves the batch staged.
bool commit_stat_visit_v1(CharacterState& same_state, StatTrainingVisitV1& visit,
    const StatTrainingPersistV1& persist, std::string& error);

// Cancel (No): restores the common PROP/SKIL baseline against current gear,
// persists once and clears the batch. A failed save retains both staged kinds
// and the live Session exactly. Legacy direct stat-only batches without a
// baseline retain the inverse-stat helper behavior (not used by the menu).
bool cancel_stat_visit_v1(CharacterState& same_state, CombatSession& same_session,
    const OriginalPropertyDatabase& source_properties, StatTrainingVisitV1& visit,
    const StatTrainingPersistV1& persist, std::string& error);

// Authored btn_train_* hit regions in 480x320 menu space. Returns the stat
// index 0..3 or -1 when the point is on no stat button.
int stats_training_button_at_v1(float authored_x, float authored_y) noexcept;

// Registers the live Stats-tab +/- route on the menu composition. session is
// queried at each click so a replaced Session is never retained.
bool register_stat_training_v1(SourceCompositionV1& composition,
    std::shared_ptr<void> owner, CharacterState& character,
    std::function<CombatSession*()> session, const OriginalPropertyDatabase& source_properties,
    StatTrainingPersistV1 persist, std::shared_ptr<StatTrainingVisitV1> visit,
    std::string& error);

} // namespace dh::foundation::character_menu
