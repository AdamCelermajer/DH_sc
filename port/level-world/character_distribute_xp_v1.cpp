#include "character_distribute_xp_v1.hpp"

#include <array>
#include <cmath>
#include <cstring>
#include <limits>
#include <utility>

namespace dh2::character_distribute_xp_v1 {
namespace {
constexpr std::uint32_t kMaxLocalPlayers = 4;

std::int32_t signed_word(std::uint32_t value) {
    std::int32_t result;
    std::memcpy(&result, &value, sizeof(result));
    return result;
}

std::int32_t arithmetic_shift8(std::int32_t value) {
    const auto bits = static_cast<std::uint32_t>(value);
    return signed_word((bits >> 8) | ((bits & 0x80000000u) ? 0xff000000u : 0u));
}

std::int32_t wrap_add(std::int32_t a, std::int32_t b) {
    return signed_word(static_cast<std::uint32_t>(a) + static_cast<std::uint32_t>(b));
}

std::int32_t wrap_mul(std::int32_t a, std::int32_t b) {
    return signed_word(static_cast<std::uint32_t>(a) * static_cast<std::uint32_t>(b));
}

std::int32_t wrap_sub(std::int32_t a, std::int32_t b) {
    return signed_word(static_cast<std::uint32_t>(a) - static_cast<std::uint32_t>(b));
}

bool property(data::PropertyView* view, std::int32_t id, std::int32_t& value) {
    return view && !dh2_property_resolve(view, id, &value);
}

Status failure(Status status, Result& result, Result* output,
               std::string& error, const char* fallback) {
    result.status = status;
    if (error.empty()) error = fallback;
    *output = result;
    return status;
}

Status call_design(const Services& services, std::uint32_t offset,
                   Result& result, float& value, std::string& error) {
    result.last_operation = Operation::design_setting;
    if (!services.design_setting) return Status::service_unavailable;
    ++result.service_calls;
    try {
        if (services.design_setting(services.context, offset, &value, error))
            return Status::service_failed;
    } catch (...) {
        if (error.empty()) error = "DesignSettings provider threw";
        return Status::service_failed;
    }
    if (!std::isfinite(value)) return Status::invalid_source_fact;
    return Status::complete;
}

Status call_player(const Services& services, std::uint32_t index,
                   Result& result, CharacterView& character,
                   std::string& error) {
    result.last_operation = Operation::player_by_friendly_ordinal;
    if (!services.player_by_friendly_ordinal) return Status::service_unavailable;
    ++result.service_calls;
    character = {};
    try {
        if (services.player_by_friendly_ordinal(services.context, index, 1,
                                                &character, error))
            return Status::service_failed;
    } catch (...) {
        if (error.empty()) error = "PlayerManager GetPlayer provider threw";
        return Status::service_failed;
    }
    if (!character.identity) {
        character = {};
        return Status::complete;
    }
    return Status::complete;
}

bool valid_character_properties(const CharacterView& character, bool save) {
    if (!character.identity || !character.properties ||
        dh2_property_validate(character.properties))
        return false;
    return !save || (character.savegame &&
                     character.savegame->character() == character.identity);
}

std::int32_t cast_int(float value, bool& valid) {
    if (!std::isfinite(value) || static_cast<double>(value) < -2147483648.0 ||
        static_cast<double>(value) >= 2147483648.0) {
        valid = false;
        return 0;
    }
    return static_cast<std::int32_t>(value);
}

} // namespace

std::int32_t current_level_difficulty_from_owner(
    void* context,std::int32_t* difficulty,std::string& error) {
    if(!context||!difficulty) {
        error="Current-Level difficulty owner binding is incomplete";
        return 1;
    }
    const auto& owner=*static_cast<const CurrentLevelDifficultyOwner*>(context);
    if(!owner.fields||!owner.ready||!*owner.ready) {
        error="Application current-Level owner is not ready";
        return 1;
    }
    *difficulty=owner.fields->difficulty_118;
    error.clear();
    return 0;
}

Status distribute(State* state, const Services* services, Result* output,
                  std::string& error) {
    if (!state || !services || !output || !state->killed ||
        !state->killed->identity || !state->killed->properties ||
        dh2_property_validate(state->killed->properties) ||
        !std::isfinite(state->killed->x) || !std::isfinite(state->killed->y))
        return Status::invalid_argument;
    if (state->killer && (!state->killer->identity ||
        !std::isfinite(state->killer->x) || !std::isfinite(state->killer->y)))
        return Status::invalid_source_fact;

    Result result{};
    error.clear();
    const auto finish = [&](Status status) {
        result.status = status;
        *output = result;
        return status;
    };
    const auto fail = [&](Status status, const char* fallback) {
        return failure(status, result, output, error, fallback);
    };

    // The original first reads killed property 35 as fixed-point XP, then the
    // shared DesignSettings radius before querying PlayerManager.
    std::int32_t raw_base_xp = 0;
    if (!property(state->killed->properties, 35, raw_base_xp))
        return fail(Status::invalid_source_fact,
                    "Killed Character property 35 is unavailable");
    result.victim_base_xp = arithmetic_shift8(raw_base_xp);
    if (result.victim_base_xp < 0) result.victim_base_xp = 0;

    float radius = 0.0f;
    auto status = call_design(*services, 160, result, radius, error);
    if (status == Status::service_unavailable)
        return fail(status, "DesignSettings XP radius at members+160 is unavailable");
    if (status == Status::service_failed)
        return fail(status, "DesignSettings XP radius provider failed");
    if (status != Status::complete)
        return fail(status, "DesignSettings XP radius is not a finite source value");

    result.last_operation = Operation::player_count;
    if (!services->player_count)
        return fail(Status::service_unavailable,
                    "PlayerManager GetNumPlayers provider is unavailable");
    ++result.service_calls;
    std::int32_t count = 0;
    try {
        if (services->player_count(services->context, &count, error))
            return fail(Status::service_failed,
                        "PlayerManager GetNumPlayers provider failed");
    } catch (...) {
        return fail(Status::service_failed,
                    "PlayerManager GetNumPlayers provider threw");
    }
    if (count < 0) return fail(Status::invalid_source_fact,
                               "PlayerManager returned a negative roster count");
    if (count > static_cast<std::int32_t>(kMaxLocalPlayers))
        return fail(Status::invalid_source_fact,
                    "Source DistributeXP stack array supports at most four players");
    result.player_slots = static_cast<std::uint32_t>(count);
    if (count == 0) return finish(Status::complete);

    struct Slot {
        bool had_character{};
        float scaled_xp{};
    };
    std::array<Slot, kMaxLocalPlayers> slots{};
    std::uint32_t eligible = 0;
    const CharacterView* origin = state->killer ? state->killer : state->killed;

    for (std::uint32_t i = 0; i < result.player_slots; ++i) {
        CharacterView recipient{};
        status = call_player(*services, i, result, recipient, error);
        if (status == Status::service_unavailable)
            return fail(status, "PlayerManager GetPlayer(i,true) provider is unavailable");
        if (status == Status::service_failed)
            return fail(status, "PlayerManager GetPlayer(i,true) provider failed");
        if (status != Status::complete)
            return fail(status, "PlayerManager returned an invalid Character projection");
        if (!recipient.identity) continue;
        if (!valid_character_properties(recipient, false) ||
            !std::isfinite(recipient.x) || !std::isfinite(recipient.y))
            return fail(Status::invalid_source_fact,
                        "Player Character property/position projection is unavailable");
        slots[i].had_character = true;
        ++result.player_characters;

        std::int32_t victim_level_raw = 0, player_level_raw = 0;
        if (!property(state->killed->properties, 19, victim_level_raw) ||
            !property(recipient.properties, 19, player_level_raw))
            return fail(Status::invalid_source_fact,
                        "Character::GetLevel property 19 is unavailable");
        std::int32_t delta = wrap_sub(arithmetic_shift8(victim_level_raw),
                                      arithmetic_shift8(player_level_raw));

        // Match GetLevelScaledXP's live reads: max positive difference, then
        // positive/negative slopes, then floor and ceiling percentages.
        float max_delta_value = 0.0f, positive_slope_value = 0.0f;
        float negative_slope_value = 0.0f, percent_floor = 0.0f;
        float percent_ceiling = 0.0f;
        const std::array<std::pair<std::uint32_t, float*>, 5> scaling_reads{{
            {156, &max_delta_value}, {140, &positive_slope_value},
            {144, &negative_slope_value}, {152, &percent_floor},
            {148, &percent_ceiling},
        }};
        for (const auto& read : scaling_reads) {
            status = call_design(*services, read.first, result, *read.second, error);
            if (status == Status::service_unavailable)
                return fail(status, "DesignSettings level-scaling field is unavailable");
            if (status == Status::service_failed)
                return fail(status, "DesignSettings level-scaling provider failed");
            if (status != Status::complete)
                return fail(status, "DesignSettings level-scaling value is invalid");
        }
        bool cast_valid = true;
        const auto max_delta = cast_int(max_delta_value, cast_valid);
        const auto positive_slope = cast_int(positive_slope_value, cast_valid);
        const auto negative_slope = cast_int(negative_slope_value, cast_valid);
        if (!cast_valid)
            return fail(Status::invalid_source_fact,
                        "DesignSettings level-scaling float cannot convert to source int");
        if (delta >= max_delta) delta = max_delta;
        float percent = 100.0f;
        if (delta > 0)
            percent = static_cast<float>(wrap_mul(positive_slope, delta)) + 100.0f;
        else if (delta < 0)
            percent = static_cast<float>(wrap_mul(negative_slope, delta)) + 100.0f;
        if (percent_floor >= percent) percent = percent_floor;
        if (percent_ceiling <= percent) percent = percent_ceiling;
        slots[i].scaled_xp = (percent / 100.0f) *
                             static_cast<float>(result.victim_base_xp);
        if (!std::isfinite(slots[i].scaled_xp))
            return fail(Status::invalid_source_fact,
                        "GetLevelScaledXP produced a non-finite result");
        // The source excludes negative XP before it reads positions or counts
        // the recipient toward the cooperative percentage penalty.
        if (slots[i].scaled_xp < 0.0f) continue;

        const float dx = recipient.x - origin->x;
        const float dy = recipient.y - origin->y;
        const float distance = std::sqrt((dx * dx) + (dy * dy));
        if (!std::isfinite(distance))
            return fail(Status::invalid_source_fact,
                        "DistributeXP player distance is not finite");
        if (radius >= distance || recipient.identity == (state->killer ? state->killer->identity : 0))
            ++eligible;
        else
            slots[i].scaled_xp = 0.0f;
    }

    result.range_eligible = eligible;
    if (!eligible) return finish(Status::complete);

    float extra_player_penalty = 0.0f;
    status = call_design(*services, 164, result, extra_player_penalty, error);
    if (status == Status::service_unavailable)
        return fail(status, "DesignSettings extra-player XP penalty at members+164 is unavailable");
    if (status == Status::service_failed)
        return fail(status, "DesignSettings extra-player XP penalty provider failed");
    if (status != Status::complete)
        return fail(Status::invalid_source_fact,
                    "DesignSettings extra-player XP penalty is not finite");
    const float reduction = static_cast<float>(eligible - 1) * extra_player_penalty;
    const float share_percent = 100.0f - reduction;
    if (!std::isfinite(share_percent))
        return fail(Status::invalid_source_fact,
                    "DistributeXP cooperative share percentage is not finite");
    result.xp_share_percent = share_percent;

    for (std::uint32_t i = 0; i < result.player_slots; ++i) {
        CharacterView recipient{};
        status = call_player(*services, i, result, recipient, error);
        if (status == Status::service_unavailable)
            return fail(status, "PlayerManager second GetPlayer(i,true) provider is unavailable");
        if (status == Status::service_failed)
            return fail(status, "PlayerManager second GetPlayer(i,true) provider failed");
        if (status != Status::complete)
            return fail(Status::invalid_source_fact,
                        "PlayerManager returned an invalid second-pass Character projection");
        if (!recipient.identity) {
            if (slots[i].had_character)
                continue;
            continue;
        }
        if (!slots[i].had_character)
            return fail(Status::invalid_source_fact,
                        "Roster gained a Character for a slot whose source XP value was uninitialized");
        const float share = (share_percent * slots[i].scaled_xp) / 100.0f;
        if (!std::isfinite(share))
            return fail(Status::invalid_source_fact,
                        "DistributeXP fixed-point share is not finite");
        if (share < 0.0f) continue;
        if (!valid_character_properties(recipient, true))
            return fail(Status::invalid_source_fact,
                        "XP recipient is missing its canonical Save/PropertyView owner");
        const float rounded = share + 1.0f;
        if (rounded < 0.0f || static_cast<double>(rounded) >= 8388608.0)
            return fail(Status::invalid_source_fact,
                        "DistributeXP fixed-point share exceeds safe source integer range");
        const auto amount_fixed = static_cast<std::int32_t>(rounded) << 8;
        result.last_xp_amount_fixed = amount_fixed;
        result.last_operation = Operation::give_xp;
        if (!services->give_xp)
            return fail(Status::service_unavailable,
                        "Character::_GiveXP provider is unavailable; bind the current canonical XP Runtime");
        ++result.service_calls;
        ++result.give_xp_calls;
        std::uint32_t source_return = 0;
        try {
            if (services->give_xp(services->context, &recipient, amount_fixed, 1,
                                  &source_return, error))
                return fail(Status::service_failed,
                            "Character::_GiveXP provider failed after its reached prefix");
        } catch (...) {
            return fail(Status::service_failed,
                        "Character::_GiveXP provider threw after its reached prefix");
        }
        if (!source_return) continue;
        ++result.xp_awarded;

        result.last_operation = Operation::is_local_player;
        if (!services->is_local_player)
            return fail(Status::service_unavailable,
                        "PlayerManager IsLocalPlayer provider is unavailable for XP text");
        ++result.service_calls;
        ++result.local_player_checks;
        std::uint32_t is_local = 0;
        try {
            if (services->is_local_player(services->context, recipient.identity,
                                          &is_local, error))
                return fail(Status::service_failed,
                            "PlayerManager IsLocalPlayer provider failed after the XP award");
        } catch (...) {
            return fail(Status::service_failed,
                        "PlayerManager IsLocalPlayer provider threw after the XP award");
        }
        if (!is_local) continue;

        result.last_operation = Operation::current_level_difficulty;
        if (!services->current_level_difficulty)
            return fail(Status::service_unavailable,
                        "Application current-Level difficulty provider is unavailable for XP text");
        ++result.service_calls;
        std::int32_t level_difficulty = 0;
        try {
            if (services->current_level_difficulty(services->context,
                                                   &level_difficulty, error))
                return fail(Status::service_failed,
                            "Current-Level difficulty provider failed after the XP award");
        } catch (...) {
            return fail(Status::service_failed,
                        "Current-Level difficulty provider threw after the XP award");
        }
        std::int32_t text_amount_fixed = amount_fixed;
        if (recipient.savegame->unlocked_difficulty() < level_difficulty)
            text_amount_fixed = 256;
        std::int32_t xp_bonus = 0;
        if (!property(recipient.properties, 200, xp_bonus))
            return fail(Status::invalid_source_fact,
                        "Local recipient XP bonus property 200 is unavailable after award");
        const auto factor = wrap_add(xp_bonus, 25600) / 100;
        const auto modified_fixed = arithmetic_shift8(wrap_mul(factor, text_amount_fixed));
        const auto displayed = arithmetic_shift8(modified_fixed);
        result.last_displayed_xp = displayed;
        result.last_operation = Operation::scrolling_xp_text;
        if (!services->scrolling_xp_text)
            return fail(Status::service_unavailable,
                        "F_ApplyScrollingCombatTextXP provider is unavailable after the XP award");
        ++result.service_calls;
        ++result.scrolling_text_calls;
        try {
            if (services->scrolling_xp_text(services->context,
                                            state->killed->identity,
                                            displayed, error))
                return fail(Status::service_failed,
                            "Scrolling XP text provider failed after the XP award");
        } catch (...) {
            return fail(Status::service_failed,
                        "Scrolling XP text provider threw after the XP award");
        }
    }
    return finish(Status::complete);
}

} // namespace dh2::character_distribute_xp_v1
