#pragma once
#include "actor_state.hpp"
#include "original_actor_properties.hpp"
#include "../game-data/combat_result.hpp"
#include "../game-data/item_gear_properties_v5.hpp"
#include <map>

namespace dh::foundation {
struct OriginalEquippedItem {
    dh2::data::ItemRecord164 record{};
    bool left_hand = false;
    std::vector<dh2::data::GearPowerProperty12V5> powers;
};
struct OriginalCombatFacts {
    // Supplied by original equipment queries, not inferred from actor identity.
    std::int32_t main_damage_class = -1, off_damage_class = -1;
    bool two_hander = false, dual_wield = false, shield = false;
    std::int32_t original_state = -1;
    std::uint32_t combo_hits = 0;
};
struct OriginalCombatProperties {
    dh2::data::PropertyState sheets;
    OriginalCombatFacts facts;
};
// Exact equipment query projection: category word37, main twohand word26=-4,
// offhand shield type word22=6; preserves caller's state/combo fields.
bool original_combat_equipment_facts(const dh2::data::ItemRecord164* main,
    const dh2::data::ItemRecord164* off, OriginalCombatFacts&, std::string& error);
// Original raw row + actual equipment -> class/stat/gear resolved 224-sheet.
// Powers must be the instance's generated powers, not its power-list ID.
// Options.level_raw carries campaign/spawn level; absent retains authored row.
bool build_original_combat_properties(const OriginalPropertyDatabase&,
    const std::string& character_row, const OriginalActorPropertyOptions&,
    const std::vector<OriginalEquippedItem>&, const OriginalCombatFacts&,
    OriginalCombatProperties&, std::string& error);

struct OriginalMeleeSource { bool off_hand = false, alternate = false; };
struct OriginalMeleeResolution {
    dh2::data::CombatResult original;
    float damage = 0.0f;
};

// Host's CombatWorld::resolve_damage delegates here. No player/NPC branch.
// RNG is supplied/owned by host; use the original chosen stream/seed.
class OriginalMeleeDamageProvider {
public:
    bool bind_actor(ActorId, OriginalCombatProperties, std::string& error);
    void remove_actor(ActorId);
    bool bind_source(std::string id, OriginalMeleeSource, std::string& error);
    // Successful miss/dodge is damage0 with original outcome bits preserved.
    // Failure preserves result and RNG. Does not mutate actor health or apply
    // DOT/leech/status/alternate buff removal; those are lifecycle consumers.
    bool resolve(const std::string& source_id, ActorId attacker, ActorId victim,
                 dh2::data::CombatRandom&, OriginalMeleeResolution&,
                 std::string& error) const;
    // Shared result calculation for source skill/spell callers. The caller
    // supplies its authored mask/category/element/amount; bound sheets, facts,
    // RNG and full outcome payload remain the same as ordinary attacks. An
    // optional source-produced scratch sheet affects this calculation only;
    // the bound actor's equipment facts and permanent properties are retained.
    bool resolve_result(ActorId attacker, ActorId victim, std::uint32_t mask,
                        std::int32_t category, std::int32_t element,
                        std::int32_t direct_amount, dh2::data::CombatRandom&,
                        OriginalMeleeResolution&, std::string& error,
                        const dh2::data::PropertySheet* attacker_formula_sheet = nullptr) const;
    bool update_facts(ActorId, const OriginalCombatFacts&, std::string& error);
    const OriginalCombatProperties* properties(ActorId) const noexcept;
private:
    std::map<ActorId, OriginalCombatProperties> actors_;
    std::map<std::string, OriginalMeleeSource> sources_;
};
} // namespace dh::foundation
