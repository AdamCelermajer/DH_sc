#include "character_combat_status_v1.hpp"
#include "character_coordinator.hpp"

#include "../game-data/properties.hpp"

namespace dh2::character_combat_status_v1 {
namespace {
using character_apply_result::Operation;
using character_apply_result::Request;
using character_player_buffs_v1::Result;
using BuffStatus = character_player_buffs_v1::Status;

bool lookup(int (*fn)(void*, const char*, std::int32_t*), void* context,
            const char* name, std::int32_t& out) {
    return fn && fn(context, name, &out) == 0;
}

Status add_dot(const Request& request, const Bindings& bindings,
               std::string& error) {
    const auto element = static_cast<std::int32_t>(request.word2);
    if (element < -1 || element > 4) {
        error = "PROPS_AddDot received an element outside -1..4";
        return Status::unsupported_operation;
    }
    const char* names[] = {"dot_normal", "dot_fire", "dot_water",
                           "dot_lightning", "dot_earth", "dot_air"};
    std::int32_t class_base = -1, effect_base = -1;
    if (!lookup(bindings.catalog.class_id, bindings.catalog.context,
                "AUTO_DOT_01_FIRE", class_base) ||
        !lookup(bindings.catalog.effect_id, bindings.catalog.context,
                "AUTO_DOT_01_FIRE", effect_base)) {
        error = "PROPS_AddDot could not query the loaded class/effect tables";
        return Status::unavailable;
    }
    const auto class_id = class_base + element;
    const auto effect_id = effect_base + element;
    Result result{};
    const auto created = bindings.buffs->add(
        class_id, request.word0, 1, request.word1, effect_id,
        names[element + 1], &result);
    if (created != BuffStatus::complete) {
        error = "PROPS_AddDot failed while adding the source buff";
        return Status::provider_failed;
    }
    if (!result.instance) return Status::complete;

    std::int32_t* sheet = nullptr;
    const auto property = 127 + element;
    if (!bindings.buffs->owned_sheet(result.instance, &sheet) || !sheet) {
        error = "PROPS_AddDot lost its newly added source buff sheet";
        return Status::provider_failed;
    }
    sheet[property] = static_cast<std::int32_t>(request.word1);
    if (!bindings.native.recalculate_property ||
        bindings.native.recalculate_property(bindings.native.context,
            bindings.character, bindings.properties, property) != 0) {
        error = "PROPS_AddDot reached an unavailable RecalcProperty provider";
        return Status::provider_failed;
    }
    return Status::complete;
}

Status add_slow(const Request& request, const Bindings& bindings,
                std::string& error) {
    if (!bindings.native.is_boss) {
        error = "PROPS_DebuffSlow reached an unavailable Character::IsBoss provider";
        return Status::unavailable;
    }
    std::uint32_t boss = 0;
    if (bindings.native.is_boss(bindings.native.context, bindings.character,
                                 &boss) != 0 || boss > 1) {
        error = "Character::IsBoss provider failed or returned an invalid value";
        return Status::provider_failed;
    }
    if (boss || request.word0 == 0) return Status::complete;

    std::int32_t class_id = -1, effect_id = -1;
    if (!lookup(bindings.catalog.class_id, bindings.catalog.context,
                "Debuff_Slow", class_id) || class_id < 0 ||
        !lookup(bindings.catalog.effect_id, bindings.catalog.context,
                "AUTO_DEBUFF_SLOW", effect_id)) {
        error = "PROPS_DebuffSlow could not query the loaded class/effect tables";
        return Status::unavailable;
    }
    Result result{};
    const auto created = bindings.buffs->add(class_id, request.word0, 1, 0,
                                             effect_id, "debuff_slow", &result);
    if (created != BuffStatus::complete) {
        error = "PROPS_DebuffSlow failed while adding the source buff";
        return Status::provider_failed;
    }
    if (!result.instance) return Status::complete;
    if (bindings.buffs->apply(class_id, result.instance, &result) !=
        BuffStatus::complete) {
        error = "PROPS_DebuffSlow failed while applying the source class";
        return Status::provider_failed;
    }
    return Status::complete;
}

bool callback_ok(int (*callback)(void*, std::uintptr_t, std::uint32_t*),
                 void* context, std::uintptr_t character,
                 std::uint32_t& value) {
    return callback && callback(context, character, &value) == 0 && value <= 1;
}

Status route_state(const Request& request, const Bindings& bindings,
                   std::int32_t next, std::int32_t event, bool direct,
                   std::string& error) {
    if (!bindings.coordinator || !bindings.coordinator->bound() ||
        bindings.coordinator->owner() != bindings.character) {
        error = "combat status has no same-Character Coordinator";
        return Status::unavailable;
    }
    const int routed = direct
        ? bindings.coordinator->transition(next, event, request.peer)
        : bindings.coordinator->event(static_cast<std::uint32_t>(event), request.peer);
    if (routed < 0) {
        error = "CharStateMachine status routing failed on the canonical Coordinator";
        return Status::provider_failed;
    }
    return Status::complete;
}

Status read_table(const Bindings& bindings, std::uint32_t& valid_table,
                  std::string& error) {
    if (!callback_ok(bindings.native.animation_table_valid,
                     bindings.native.context, bindings.character, valid_table)) {
        error = "CharAnimTable validity provider is unavailable or invalid";
        return Status::unavailable;
    }
    return Status::complete;
}

Status choose_animation(const Bindings& bindings, std::int32_t row_offset,
                        std::uint32_t stance_mask, std::int32_t& animation,
                        std::string& error) {
    if (!bindings.native.select_animation ||
        bindings.native.select_animation(bindings.native.context,
            bindings.character, row_offset, stance_mask, &animation) != 0) {
        error = "CharAnimTable row or source stance selection failed";
        return Status::provider_failed;
    }
    return Status::complete;
}

Status set_machine_animation(const Bindings& bindings, std::int32_t animation,
                             std::string& error) {
    if (!bindings.native.set_machine_animation ||
        bindings.native.set_machine_animation(bindings.native.context,
            bindings.character, animation) != 0) {
        error = "CharStateMachine current-animation provider is unavailable";
        return Status::provider_failed;
    }
    return Status::complete;
}

Status get_machine_flags(const Bindings& bindings, std::uint32_t& flags,
                         std::string& error) {
    if (!bindings.native.read_machine_flags ||
        bindings.native.read_machine_flags(bindings.native.context,
            bindings.character, &flags) != 0) {
        error = "CharStateMachine flags provider is unavailable";
        return Status::unavailable;
    }
    bindings.coordinator->set_source_machine_flags_44_528(flags);
    return Status::complete;
}

Status put_machine_flags(const Bindings& bindings, std::uint32_t flags,
                         std::string& error) {
    if (!bindings.native.write_machine_flags ||
        bindings.native.write_machine_flags(bindings.native.context,
            bindings.character, flags) != 0) {
        error = "CharStateMachine flags writer is unavailable";
        return Status::provider_failed;
    }
    bindings.coordinator->set_source_machine_flags_44_528(flags);
    return Status::complete;
}

Status set_character_flags(const Bindings& bindings, std::uint32_t mask,
                           std::string& error) {
    if (!bindings.native.set_character_flags ||
        bindings.native.set_character_flags(bindings.native.context,
            bindings.character, mask) != 0) {
        error = "Character combat flags writer is unavailable";
        return Status::provider_failed;
    }
    bindings.coordinator->set_source_machine_flags_44_528(
        bindings.coordinator->source_machine_flags_44_528() | mask);
    return Status::complete;
}

Status run_reaction(const Request& request, const Bindings& bindings,
                    std::int32_t row_offset, std::uint32_t stance_mask,
                    bool skip_missing_sequence, bool direct,
                    std::string& error) {
    if (!bindings.native.reaction_value) {
        error = "Character+0x14fc reaction-gate provider is unavailable";
        return Status::unavailable;
    }
    float value = 0;
    if (bindings.native.reaction_value(bindings.native.context,
            bindings.character, &value) != 0) {
        error = "Character reaction-gate read failed";
        return Status::provider_failed;
    }
    if (value > 0.0f) return Status::complete;
    if (!bindings.native.prepare_reaction ||
        bindings.native.prepare_reaction(bindings.native.context,
            bindings.character) != 0) {
        error = "Character reaction prelude (IsPlayer vcall+0x28 and +0x14fc write) failed";
        return Status::provider_failed;
    }
    std::uint32_t table_valid = 0;
    auto status = read_table(bindings, table_valid, error);
    if (status != Status::complete || !table_valid) return status;
    std::int32_t animation = -1;
    status = choose_animation(bindings, row_offset, stance_mask, animation, error);
    if (status != Status::complete) return status;
    if (skip_missing_sequence && animation == -1) return Status::complete;
    status = set_machine_animation(bindings, animation, error);
    if (status != Status::complete) return status;
    return route_state(request, bindings, 11, 50010, direct, error);
}

Status run_push(const Request& request, const Bindings& bindings,
                std::string& error) {
    if (!bindings.native.is_boss) {
        error = "SM_SetKnockBackState reached an unavailable IsBoss provider";
        return Status::unavailable;
    }
    std::uint32_t boss = 0;
    if (bindings.native.is_boss(bindings.native.context, bindings.character,
                                 &boss) != 0 || boss > 1) {
        error = "SM_SetKnockBackState IsBoss provider failed";
        return Status::provider_failed;
    }
    if (boss) return Status::complete;
    std::uint32_t table_valid = 0;
    auto status = read_table(bindings, table_valid, error);
    if (status != Status::complete || !table_valid) return status;
    const bool forward = request.word0 != 0;
    std::int32_t animation = -1;
    status = choose_animation(bindings, forward ? 36 : 72,
                              forward ? 0x800u : 0x400u,
                              animation, error);
    if (status != Status::complete) return status;
    status = set_machine_animation(bindings, animation, error);
    if (status != Status::complete) return status;
    std::uint32_t machine_flags = 0;
    status = get_machine_flags(bindings, machine_flags, error);
    if (status != Status::complete) return status;
    machine_flags = forward ? 0x18u : machine_flags & ~0x18u;
    status = put_machine_flags(bindings, machine_flags, error);
    if (status != Status::complete) return status;
    return route_state(request, bindings, 10, 50011, request.word2 != 0, error);
}

Status run_timed_status(const Request& request, const Bindings& bindings,
                        bool scared, std::string& error) {
    if (!bindings.native.is_boss) {
        error = "timed CharStateMachine status reached an unavailable IsBoss provider";
        return Status::unavailable;
    }
    std::uint32_t boss = 0;
    if (bindings.native.is_boss(bindings.native.context, bindings.character,
                                 &boss) != 0 || boss > 1) {
        error = "timed CharStateMachine status IsBoss provider failed";
        return Status::provider_failed;
    }
    if (boss) return Status::complete;
    std::uint32_t table_valid = 0;
    auto status = read_table(bindings, table_valid, error);
    if (status != Status::complete || !table_valid) return status;
    std::uint32_t machine_flags = 0;
    status = get_machine_flags(bindings, machine_flags, error);
    if (status != Status::complete) return status;
    const std::uint32_t bit = scared ? 4u : 2u;
    if (!(machine_flags & bit)) {
        if (!bindings.coordinator || !bindings.coordinator->bound() ||
            bindings.coordinator->owner() != bindings.character) {
            error = "timed status has no same-Character timer Coordinator";
            return Status::unavailable;
        }
        const auto timer = bindings.coordinator->start_timer(
            request.word0, 0, scared ? 44 : 43, 0);
        if (timer < 0) {
            error = "CharTimers::TMR_Start failed on the canonical Coordinator";
            return Status::provider_failed;
        }
        machine_flags |= bit;
        status = put_machine_flags(bindings, machine_flags, error);
        if (status != Status::complete) return status;
    }
    std::int32_t animation = -1;
    status = choose_animation(bindings, scared ? 124 : 140,
                              scared ? 0x100u : 0x200u,
                              animation, error);
    if (status != Status::complete) return status;
    status = set_machine_animation(bindings, animation, error);
    if (status != Status::complete) return status;
    status = route_state(request, bindings, scared ? 8 : 9,
                         scared ? 50013 : 50012, request.word2 != 0, error);
    if (status != Status::complete) return status;
    if (request.word1) return set_character_flags(bindings,
        scared ? 0x400u : 0x800u, error);
    return Status::complete;
}
} // namespace

Status execute(const Request* request, const Bindings* bindings,
               character_apply_result::Reply* reply, std::string& error) {
    if (!request || !bindings || !reply || !bindings->character ||
        request->subject != bindings->character || !bindings->properties ||
        !bindings->buffs || bindings->buffs->character_identity() != bindings->character ||
        bindings->buffs->character_identity() == 0 ||
        ::dh2_property_validate(bindings->properties)) {
        error = "combat status binding does not identify the canonical Character";
        return Status::invalid_argument;
    }
    switch (request->operation) {
    case Operation::apply_dot:
        return add_dot(*request, *bindings, error);
    case Operation::slow:
        return add_slow(*request, *bindings, error);
    case Operation::dodge:
        return run_reaction(*request, *bindings, 32, 0x4000u, true,
                            request->word0 != 0, error);
    case Operation::block:
        return run_reaction(*request, *bindings, 12, 0x2000u, true,
                            request->word0 != 0, error);
    case Operation::hurt:
        return run_reaction(*request, *bindings, 60, 0x1000u, false,
                            request->word0 != 0, error);
    case Operation::push:
        return run_push(*request, *bindings, error);
    case Operation::stun:
        return run_timed_status(*request, *bindings, false, error);
    case Operation::fear:
        return run_timed_status(*request, *bindings, true, error);
    default:
        error = "operation is outside the Character combat status closure";
        return Status::unsupported_operation;
    }
}

} // namespace dh2::character_combat_status_v1
