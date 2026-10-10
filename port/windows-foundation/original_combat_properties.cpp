#include "original_combat_properties.hpp"
#include <algorithm>
#include <cstring>

namespace dh::foundation {
namespace {
bool facts_valid(const OriginalCombatFacts& facts) {
    return facts.main_damage_class >= -1 && facts.main_damage_class < 141 &&
           facts.off_damage_class >= -1 && facts.off_damage_class < 141 && facts.combo_hits <= 65535;
}
dh2::data::CombatantView view(const OriginalCombatProperties& properties) {
    const auto& f = properties.facts;
    return {properties.sheets.resolved.data(), f.main_damage_class, f.off_damage_class,
            unsigned(f.two_hander), unsigned(f.dual_wield), unsigned(f.shield),
            f.original_state, f.combo_hits};
}
}
bool original_combat_equipment_facts(const dh2::data::ItemRecord164* main,
    const dh2::data::ItemRecord164* off, OriginalCombatFacts& output, std::string& error) {
    auto next = output;
    next.main_damage_class = main ? main->words[37] : -1;
    next.off_damage_class = off ? off->words[37] : -1;
    next.two_hander = main && main->words[26] == -4;
    next.shield = off && off->words[22] == 6;
    next.dual_wield = off && !next.shield;
    if (!facts_valid(next)) { error = "Original equipment combat category outside bounds"; return false; }
    output = next; error.clear(); return true;
}
bool build_original_combat_properties(const OriginalPropertyDatabase& database,
    const std::string& row_name, const OriginalActorPropertyOptions& options,
    const std::vector<OriginalEquippedItem>& equipment, const OriginalCombatFacts& facts,
    OriginalCombatProperties& output, std::string& error) {
    if (!facts_valid(facts) || equipment.size() > 4096) {
        error = "Original combat facts/equipment outside bounds"; return false;
    }
    dh2::data::PropertyRules rules;
    if (!dh2::data::load_property_rules(database.characters,rules,error)) return false;
    const auto row = std::find(database.characters.names.begin(),database.characters.names.end(),row_name);
    if (row == database.characters.names.end()) { error = "Original combat character row absent"; return false; }
    OriginalCombatProperties next; next.facts = facts;
    dh2::data::reset_properties(rules,next.sheets,
        &database.characters.rows[static_cast<std::size_t>(row-database.characters.names.begin())]);
    if (options.level_raw) {
        if (*options.level_raw < 0) { error = "Original combat level raw invalid"; return false; }
        next.sheets.base[19] = *options.level_raw;
    }
    for (const auto& item : equipment) {
        if (item.powers.size() > 65536) { error = "Original item power count outside bounds"; return false; }
        const dh2::data::GearPowerView16V5 powers{item.powers.data(),std::uint32_t(item.powers.size()),0};
        if (dh2_gear_stats_v5(next.sheets.gear.data(),rules.defaults.data(),&item.record,unsigned(item.left_hand)) ||
            dh2_gear_power_v5(next.sheets.gear.data(),rules.defaults.data(),&powers,unsigned(item.left_hand))) {
            error = "Original gear stat/power projection failed"; return false;
        }
    }
    // Apply class exactly once to the raw base; uncached formula reads see gear.
    if (!dh2::data::recalc_properties_with_class(database.classes,rules,next.sheets,error)) return false;
    if (options.refill_vitals) {
        auto owner = dh2::data::property_view(rules,next.sheets);
        for (const unsigned current : {36u,41u}) {
            const unsigned maximum = current == 36 ? 38 : 43;
            const std::uint32_t bits = std::uint32_t(next.sheets.resolved[maximum])-
                                       std::uint32_t(next.sheets.resolved[current]);
            std::int32_t delta; std::memcpy(&delta,&bits,sizeof(delta));
            if (delta > 0 && dh2_property_add(&owner,current,delta)) {
                error = "Original combat vital refill failed"; return false;
            }
        }
    }
    output = std::move(next); error.clear(); return true;
}
bool OriginalMeleeDamageProvider::bind_actor(ActorId id, OriginalCombatProperties properties,
                                          std::string& error) {
    if (id == invalid_actor_id || !facts_valid(properties.facts)) {
        error = "Original combat actor binding invalid"; return false;
    }
    actors_.insert_or_assign(id,std::move(properties)); error.clear(); return true;
}
void OriginalMeleeDamageProvider::remove_actor(ActorId id) { actors_.erase(id); }
bool OriginalMeleeDamageProvider::bind_source(std::string id, OriginalMeleeSource source,
                                            std::string& error) {
    if (id.empty() || id.size() > 1024) { error = "Original melee source ID invalid"; return false; }
    sources_.insert_or_assign(std::move(id),source); error.clear(); return true;
}
const OriginalCombatProperties* OriginalMeleeDamageProvider::properties(ActorId id) const noexcept {
    const auto found = actors_.find(id); return found == actors_.end() ? nullptr : &found->second;
}
bool OriginalMeleeDamageProvider::update_facts(ActorId id, const OriginalCombatFacts& facts,
                                             std::string& error) {
    const auto found = actors_.find(id);
    if (found == actors_.end() || !facts_valid(facts)) { error = "Original combat facts update invalid"; return false; }
    found->second.facts = facts; error.clear(); return true;
}
bool OriginalMeleeDamageProvider::resolve(const std::string& source_id, ActorId attacker,
    ActorId victim, dh2::data::CombatRandom& random, OriginalMeleeResolution& output,
    std::string& error) const {
    const auto source = sources_.find(source_id);
    const auto* a = properties(attacker); const auto* d = properties(victim);
    if (source == sources_.end() || !a || !d || attacker == victim) {
        error = "Original melee source/actor binding unavailable"; return false;
    }
    const auto av = view(*a), dv = view(*d); auto rng = random;
    OriginalMeleeResolution next;
    if (dh2_combat_melee(&next.original,&av,&dv,&rng,unsigned(source->second.off_hand),
                        unsigned(source->second.alternate))) {
        error = "Original melee result calculation failed"; return false;
    }
    if ((next.original.outcomes & 3u) == 0) {
        if (next.original.amount < 0) { error = "Original melee result has no damage"; return false; }
        next.damage = original_signed256(next.original.amount);
    }
    output = next; random = rng; error.clear(); return true;
}
bool OriginalMeleeDamageProvider::resolve_result(ActorId attacker, ActorId victim,
    std::uint32_t mask, std::int32_t category, std::int32_t element,
    std::int32_t direct_amount, dh2::data::CombatRandom& random,
    OriginalMeleeResolution& output, std::string& error,
    const dh2::data::PropertySheet* attacker_formula_sheet) const {
    const auto* a=properties(attacker);const auto* d=properties(victim);
    if(!a||!d||attacker==victim){error="Original result actor binding unavailable";return false;}
    auto av=view(*a);const auto dv=view(*d);auto rng=random;
    if(attacker_formula_sheet)av.properties=attacker_formula_sheet->data();
    const dh2::data::CombatResultRequest request{&av,&dv,&rng,mask,category,element,direct_amount};
    OriginalMeleeResolution next;
    if(dh2_combat_result(&next.original,&request)){
        error="Original result calculation failed";return false;
    }
    if((next.original.outcomes&3u)==0){
        if(next.original.amount<0){error="Original result has no damage";return false;}
        next.damage=original_signed256(next.original.amount);
    }
    output=next;random=rng;error.clear();return true;
}
} // namespace dh::foundation
