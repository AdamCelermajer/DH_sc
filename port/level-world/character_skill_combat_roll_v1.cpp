#include "character_skill_combat_roll_v1.hpp"

namespace dh2::character_skill_combat_roll_v1 {
namespace {
bool failed(int status, const char* phase, std::string& error) {
    if (!status) return false;
    if (error.empty()) error = std::string("Character::_SkillCombatRoll ") +
                               phase + " provider failed";
    return true;
}

Status attack_leg(const Request& request, const Services& services,
                  std::uint32_t flags, std::int32_t skill_element,
                  Result& result, std::string& error) {
    if (!services.skill_attack) {
        error = "Character::_SkillCombatRoll requires F_SkillAttack";
        return Status::invalid_argument;
    }
    data::CombatResult combat{};
    ++result.provider_calls;
    if (failed(services.skill_attack(services.context, request.character,
                                     result.target, flags, skill_element,
                                     &combat, error), "F_SkillAttack", error))
        return Status::provider_failed;

    if (!services.apply_result) {
        error = "Character::_SkillCombatRoll requires F_ApplyResult";
        return Status::invalid_argument;
    }
    // Source passes false for both dual-wield legs and the non-dual path.
    ++result.provider_calls;
    if (failed(services.apply_result(services.context, request.character,
                                     result.target, 0,
                                     &combat, error), "F_ApplyResult", error))
        return Status::provider_failed;
    auto& applied = result.attacks[result.attack_count++];
    applied.flags = flags;
    applied.skill_element = skill_element;
    applied.amount = combat.amount;
    result.disposition = Disposition::attacks;
    return Status::complete;
}
} // namespace

Status execute(const Request* request, const Services* services, Result* output,
               std::string& error) {
    if (!request || !services || !output || !request->character) {
        error = "Character::_SkillCombatRoll requires Character and skill providers";
        return Status::invalid_argument;
    }
    *output = {};
    output->character = request->character;

    // Source checks for at least two arguments, a skill index in the current
    // Character skill list, and an accepted object/userdata target first.
    if (request->argument_count < 2 ||
        request->skill_index >= request->skill_list_size ||
        (request->target_argument_type != userdata_type &&
         request->target_argument_type != object_type) ||
        request->target_userdata == 0) {
        error.clear();
        return Status::complete;
    }
    if (!services->resolve_character) {
        error = "Character::_SkillCombatRoll requires ObjectHandle-to-Character conversion";
        return Status::invalid_argument;
    }
    std::uintptr_t target = 0;
    ++output->provider_calls;
    if (failed(services->resolve_character(services->context,
                                           request->target_userdata,
                                           &target, error),
               "ObjectHandle conversion", error))
        return Status::provider_failed;
    output->target = target;
    if (!target) {
        if (!services->read_value_tag) {
            error = "Character::_SkillCombatRoll requires unresolved Value::getValue";
            return Status::invalid_argument;
        }
        std::uint32_t tag = 0;
        ++output->provider_calls;
        if (failed(services->read_value_tag(services->context,
                                             request->target_userdata,
                                             &tag, error),
                   "unresolved Value::getValue", error))
            return Status::provider_failed;
        if (tag == false_value_tag) {
            if (!services->consume_false_value) {
                error = "Character::_SkillCombatRoll requires byte8 Value::getValue side effect";
                return Status::invalid_argument;
            }
            ++output->provider_calls;
            if (failed(services->consume_false_value(services->context,
                                                      request->target_userdata,
                                                      error),
                       "byte8 Value::getValue", error))
                return Status::provider_failed;
            output->disposition = Disposition::boolean_false;
        }
        error.clear();
        return Status::complete;
    }

    if (!services->skill_metadata) {
        error = "Character::_SkillCombatRoll requires Character::GetCharSkill";
        return Status::invalid_argument;
    }
    std::uint32_t type = 0;
    std::uint32_t flags = 0;
    std::int32_t skill_element = 0;
    ++output->provider_calls;
    if (failed(services->skill_metadata(services->context, request->character,
                                        request->skill_index, &type, &flags,
                                        &skill_element, error),
               "GetCharSkill", error))
        return Status::provider_failed;
    if (type != skill_type_active && type != skill_type_passive) {
        error.clear();
        return Status::complete;
    }

    if ((flags & dual_wield_flag) != 0) {
        if (!services->has_main_hand_weapon) {
            error = "Character::_SkillCombatRoll requires HasMainHandWeapon";
            return Status::invalid_argument;
        }
        bool main_hand = false;
        ++output->provider_calls;
        if (failed(services->has_main_hand_weapon(services->context,
                                                  request->character,
                                                  &main_hand, error),
                   "HasMainHandWeapon", error))
            return Status::provider_failed;
        if (main_hand) {
            const auto status = attack_leg(*request, *services, flags,
                                           skill_element, *output, error);
            if (status != Status::complete) return status;
        }

        if (!services->has_off_hand_weapon) {
            error = "Character::_SkillCombatRoll requires HasOffHandWeapon";
            return Status::invalid_argument;
        }
        bool off_hand = false;
        ++output->provider_calls;
        if (failed(services->has_off_hand_weapon(services->context,
                                                 request->character,
                                                 &off_hand, error),
                   "HasOffHandWeapon", error))
            return Status::provider_failed;
        if (off_hand) {
            const auto status = attack_leg(*request, *services,
                                           flags | offhand_attack_flag,
                                           skill_element, *output, error);
            if (status != Status::complete) return status;
        }
    } else {
        const auto status = attack_leg(*request, *services, flags,
                                       skill_element, *output, error);
        if (status != Status::complete) return status;
    }
    error.clear();
    return Status::complete;
}
} // namespace dh2::character_skill_combat_roll_v1
