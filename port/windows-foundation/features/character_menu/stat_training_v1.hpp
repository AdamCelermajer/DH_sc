#pragma once

#include "source_composition.hpp"
#include "../../character_state.hpp"
#include "../../combat_session.hpp"
#include "../../original_combat_properties.hpp"

#include <cstdint>
#include <functional>
#include <memory>
#include <string>

namespace dh::foundation::character_menu {

// Original SWF AddedStatsThisTurn gate (btn_train_* onRelease, IDA
// 0x149de..0x14a4e): one stat point per Character menu visit. onPush resets it
// when the menu opens; a successful spend sets it.
struct StatTrainingVisitV1 {
    bool added_this_turn = false;
    void open_visit() noexcept { added_this_turn = false; }
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

// Same-Session Stat_Points spend. Refuses (no mutation) when property 148 is
// zero, when the CharacterState points disagree with the live sheet, or when
// the stat index is outside 0..3. On success it debits property 148, adds one
// point to saved property 149+stat, resets base from the authored CharacterTable
// row (keeping the current base level, as the source profile projection does),
// runs the full class recalculation on the live actor, persists, publishes the
// new sheet to the combat world, and mirrors points/stats/vitals into
// CharacterState.
bool train_stat_in_session_v1(CharacterState& same_state, CombatSession& same_session,
    const OriginalPropertyDatabase& source_properties, std::uint32_t stat,
    const StatTrainingPersistV1& persist, StatTrainingCommitV1& output,
    std::string& error);

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
