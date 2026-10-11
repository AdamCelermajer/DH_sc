#include "character_spell_combat_roll_v1.hpp"

namespace dh2::character_spell_combat_roll_v1 {
namespace {
bool fail(int status, const char* phase, std::string& error) {
    if (!status) return false;
    if (error.empty()) error = std::string("Character::_SpellCombatRoll ") + phase + " provider failed";
    return true;
}
}

Status execute(const Request* q, const Services* services, Result* out, std::string& error) {
    if (!q || !services || !out || !q->character || !services->current_faery) {
        error = "Character::_SpellCombatRoll requires its Character and current-Faery provider";
        return Status::invalid_argument;
    }
    *out = {};
    out->character = q->character;

    std::int32_t slot = -1;
    ++out->provider_calls;
    if (fail(services->current_faery(services->context, q->character, &slot, error),
             "SG_GetCurrentFaerieId", error)) return Status::provider_failed;
    out->faery_slot = slot;

    // The source has already read SG_GetCurrentFaerieId before inspecting Arguments[0].
    if (!q->argument_count || (q->argument_type != userdata_type && q->argument_type != object_type)) {
        error.clear();
        return Status::complete;
    }
    if (!q->userdata) {
        error.clear();
        return Status::complete;
    }
    if (!services->resolve_character) {
        error = "Character::_SpellCombatRoll requires ObjectHandle-to-Character conversion";
        return Status::invalid_argument;
    }

    std::uintptr_t target = 0;
    ++out->provider_calls;
    if (fail(services->resolve_character(services->context, q->userdata, &target, error),
             "ObjectHandle conversion", error)) return Status::provider_failed;
    out->target = target;
    if (!target) {
        if (!services->read_value_tag) {
            error = "Character::_SpellCombatRoll requires unresolved userdata Value::getValue";
            return Status::invalid_argument;
        }
        std::uint32_t value_tag = 0;
        ++out->provider_calls;
        if (fail(services->read_value_tag(services->context, q->userdata, &value_tag, error),
                 "unresolved Value::getValue", error)) return Status::provider_failed;
        if (value_tag == false_value_tag) {
            if (!services->consume_false_value) {
                error = "Character::_SpellCombatRoll requires byte8 Value::getValue side effect";
                return Status::invalid_argument;
            }
            ++out->provider_calls;
            if (fail(services->consume_false_value(services->context, q->userdata, error),
                     "byte8 Value::getValue", error)) return Status::provider_failed;
            out->disposition = Disposition::boolean_false;
        }
        error.clear();
        return Status::complete;
    }

    if (!services->faery_element) {
        error = "Character::_SpellCombatRoll requires GetCharFaery";
        return Status::invalid_argument;
    }
    std::int32_t element = 0;
    ++out->provider_calls;
    if (fail(services->faery_element(services->context, q->character, slot, &element, error),
             "GetCharFaery", error)) return Status::provider_failed;
    out->element = element;

    if (!services->spell_attack) {
        error = "Character::_SpellCombatRoll requires F_SpellAttack";
        return Status::invalid_argument;
    }
    data::CombatResult result{};
    ++out->provider_calls;
    if (fail(services->spell_attack(services->context, q->character, target, element,
                                    spell_attack_mask, -1, 0, &result, error),
             "F_SpellAttack", error)) return Status::provider_failed;

    if (!services->apply_result) {
        error = "Character::_SpellCombatRoll requires F_ApplyResult";
        return Status::invalid_argument;
    }
    ++out->provider_calls;
    if (fail(services->apply_result(services->context, q->character, target, 0, &result, error),
             "F_ApplyResult", error)) return Status::provider_failed;
    out->amount = result.amount;
    out->disposition = Disposition::integer;
    error.clear();
    return Status::complete;
}
} // namespace dh2::character_spell_combat_roll_v1
