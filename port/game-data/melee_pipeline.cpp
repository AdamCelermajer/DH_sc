#include "melee_pipeline.hpp"
#include <cstddef>
#include <cstdint>

namespace dh2::data::melee_pipeline {
namespace {
struct Span { std::uintptr_t begin, end; };
bool span(const void* pointer, std::size_t size, std::size_t alignment, Span& out) {
    const auto begin = reinterpret_cast<std::uintptr_t>(pointer);
    if (!pointer || begin % alignment || begin > UINTPTR_MAX - size) return false;
    out = {begin, begin + size};
    return true;
}
bool overlaps(Span a, Span b) { return a.begin < b.end && b.begin < a.end; }
bool valid_state(const CombatActorState* state) {
    return state && state->dead <= 1 && state->low_health_armed <= 1 &&
           state->combo_hits <= 65535 && state->push_death <= 1;
}
bool valid_owner(const Owner* owner) {
    return owner && owner->properties && owner->state &&
           owner->main_damage_class >= -1 && owner->main_damage_class < 141 &&
           owner->off_damage_class >= -1 && owner->off_damage_class < 141 &&
           owner->two_hander <= 1 && owner->dual_wield <= 1 && owner->shield <= 1 &&
           valid_state(owner->state) && !dh2_property_validate(owner->properties);
}
}

Status monster_to_player(const Request* request, Exchange* output) {
    Span request_span{}, output_span{}, attacker_span{}, player_span{}, random_span{};
    if (!span(request, sizeof(*request), alignof(Request), request_span) ||
        !span(output, sizeof(*output), alignof(Exchange), output_span) ||
        overlaps(request_span, output_span)) return Status::invalid_argument;

    const auto args = *request;
    if (args.offhand > 1 || args.player_idle > 1 ||
        !args.attacker || !args.player || args.attacker == args.player ||
        !args.random || !span(args.attacker, sizeof(Owner), alignof(Owner), attacker_span) ||
        !span(args.player, sizeof(Owner), alignof(Owner), player_span) ||
        !span(args.random, sizeof(CombatRandom), alignof(CombatRandom), random_span) ||
        overlaps(attacker_span, player_span) || overlaps(request_span, attacker_span) ||
        overlaps(request_span, player_span) || overlaps(request_span, random_span) ||
        overlaps(output_span, attacker_span) || overlaps(output_span, player_span) ||
        overlaps(output_span, random_span) || overlaps(attacker_span, random_span) ||
        overlaps(player_span, random_span) || !valid_owner(args.attacker) ||
        !valid_owner(args.player) || args.attacker->properties == args.player->properties ||
        args.attacker->state == args.player->state ||
        args.attacker->properties->resolved == args.player->properties->resolved)
        return Status::invalid_argument;

    Span attacker_properties_span{}, player_properties_span{}, attacker_state_span{}, player_state_span{};
    if (!span(args.attacker->properties, sizeof(PropertyView), alignof(PropertyView), attacker_properties_span) ||
        !span(args.player->properties, sizeof(PropertyView), alignof(PropertyView), player_properties_span) ||
        !span(args.attacker->state, sizeof(CombatActorState), alignof(CombatActorState), attacker_state_span) ||
        !span(args.player->state, sizeof(CombatActorState), alignof(CombatActorState), player_state_span))
        return Status::invalid_argument;
    const Span nested[] = {attacker_properties_span, player_properties_span, attacker_state_span, player_state_span};
    const Span outer[] = {request_span, output_span, attacker_span, player_span, random_span};
    for (unsigned i = 0; i < 4; ++i) {
        for (const auto& parent : outer) if (overlaps(nested[i], parent)) return Status::invalid_argument;
        for (unsigned j = 0; j < i; ++j) if (overlaps(nested[i], nested[j])) return Status::invalid_argument;
    }
    Span attacker_sheet{}, player_sheet{};
    if (!span(args.attacker->properties->resolved, 224 * sizeof(std::int32_t), alignof(std::int32_t), attacker_sheet) ||
        !span(args.player->properties->resolved, 224 * sizeof(std::int32_t), alignof(std::int32_t), player_sheet) ||
        overlaps(attacker_sheet, player_sheet)) return Status::invalid_argument;
    for (const auto& parent : outer) {
        if (overlaps(attacker_sheet, parent) || overlaps(player_sheet, parent)) return Status::invalid_argument;
    }
    for (const auto& child : nested) {
        if (overlaps(attacker_sheet, child) || overlaps(player_sheet, child)) return Status::invalid_argument;
    }

    CombatantView attacker{args.attacker->properties->resolved,
        args.attacker->main_damage_class, args.attacker->off_damage_class,
        args.attacker->two_hander, args.attacker->dual_wield, args.attacker->shield,
        args.attacker->character_state, args.attacker->state->combo_hits};
    CombatantView player{args.player->properties->resolved,
        args.player->main_damage_class, args.player->off_damage_class,
        args.player->two_hander, args.player->dual_wield, args.player->shield,
        args.player->character_state, args.player->state->combo_hits};

    Exchange candidate{};
    if (dh2_combat_melee(&candidate.result, &attacker, &player, args.random,
                         args.offhand, 0)) return Status::calculation_failed;
    const MonsterApplicationRequest apply{
        &candidate.result, args.attacker->properties, args.player->properties,
        args.attacker->state, args.player->state,
        args.before_hit, args.before_hit_context,
        args.before_dead, args.before_dead_context};
    if (dh2_combat_apply_monster_to_player(&candidate.application, &apply, args.player_idle))
        return Status::application_failed;
    *output = candidate;
    return Status::complete;
}
} // namespace dh2::data::melee_pipeline
