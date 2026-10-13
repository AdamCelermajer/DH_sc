#include "character_anim_set_owner_v1.hpp"

#include "character_faery_visual_facade_v1.hpp"

#include <algorithm>
#include <array>
#include <limits>
#include <utility>

namespace dh2::character_anim_set_owner_v1 {
namespace {
constexpr std::int32_t kFaerySkillListFallback = 3;
constexpr std::uint32_t kSourceStanceMask = 210;

struct StancedMember {
    std::int32_t raw_member;
    std::uint32_t flags;
};

// CharAnim fields 16/17 and 33/34 are native vector count/data pairs. The
// portable table flattens each pair to one vector at schema fields 15 and 31.
const std::vector<std::int32_t>* raw_member(
    const data::CharacterAnimations& row, std::int32_t member) noexcept {
    if (member < 1 || member > 39) return nullptr;
    std::size_t field = 0;
    if (member <= 15) field = static_cast<std::size_t>(member - 1);
    else if (member <= 17) field = 15;
    else if (member <= 32) field = static_cast<std::size_t>(member - 2);
    else if (member <= 34) field = 31;
    else field = static_cast<std::size_t>(member - 3);
    return &row.fields[field];
}

std::int32_t scalar_member(const data::CharacterAnimations& row,
                           std::int32_t member) noexcept {
    const auto* values = raw_member(row, member);
    return values && !values->empty() ? values->front() : -1;
}

class Compiler {
public:
    Compiler(const data::AnimationTables& animations, const Effects& effects,
             AnimSetState& output, std::string& error)
        : animations_(animations), effects_(effects), output_(output), error_(error) {}

    bool add_template(std::int32_t source_sequence) {
        if (source_sequence < 0 ||
            static_cast<std::size_t>(source_sequence) >= animations_.sequences.size())
            return true;
        const auto& sequence = animations_.sequences[static_cast<std::size_t>(source_sequence)];
        if (sequence.steps.size() != 1) {
            error_ = "Source _AddTemplateAnimTable expected exactly one animation step";
            return false;
        }
        const auto& step = sequence.steps.front();
        if (step.redir != 0) return true;
        Registration registration;
        registration.kind = RegistrationKind::template_animation;
        registration.source_sequence_id = source_sequence;
        registration.animation_table_sequence_id = source_sequence;
        registration.animation_dictionary_id = step.anim;
        return append(registration);
    }

    bool add_table(std::int32_t source_sequence, std::int32_t stance,
                   std::uint32_t flags, std::uint32_t stance_mask,
                   std::int32_t root_sequence, std::size_t depth) {
        if (source_sequence < 0) return true;
        if (depth > animations_.sequences.size()) {
            error_ = "Source AnimTable redirect recursion exceeded table bounds";
            return false;
        }
        const auto offset = static_cast<std::int64_t>(source_sequence) + stance;
        if (offset < 0 || static_cast<std::uint64_t>(offset) >= animations_.sequences.size())
            return true;
        if (stance != 0 && (stance_mask & flags) != flags) return true;

        const auto sequence_id = static_cast<std::int32_t>(offset);
        const auto& sequence = animations_.sequences[static_cast<std::size_t>(sequence_id)];
        for (const auto& step : sequence.steps) {
            if (step.redir != 0) {
                if (step.redir != 1) {
                    error_ = "Source animation redirect flag is unsupported";
                    return false;
                }
                if (!add_table(step.anim, 0, 0, 0, root_sequence, depth + 1))
                    return false;
                continue;
            }

            Registration registration;
            registration.kind = RegistrationKind::animation;
            registration.source_sequence_id = root_sequence;
            registration.animation_table_sequence_id = sequence_id;
            registration.animation_dictionary_id = step.anim;
            registration.stance = stance;
            registration.flags = flags;
            registration.stance_mask = stance_mask;
            if (!append(registration)) return false;

            // CharAnimator::_AddAnimTable performs AddAnim, sound loading, then
            // FX registration for each non-redirect step, in this order.
            if (step.sound >= 0 && effects_.load_sound &&
                !effects_.load_sound(effects_.context, step.sound, error_)) {
                fail_effect("source animation sound load failed");
                return false;
            }
            if (step.fx >= 0 && effects_.register_fx &&
                !effects_.register_fx(effects_.context, step.fx, error_)) {
                fail_effect("source animation FX registration failed");
                return false;
            }
        }
        return true;
    }

    bool add_array_member(const data::CharacterAnimations& row,
                         std::int32_t raw_slot, std::int32_t stance,
                         std::uint32_t flags, std::uint32_t stance_mask) {
        const auto* values = raw_member(row, raw_slot);
        if (!values) return true;
        for (const auto sequence : *values) {
            if (!add_table(sequence, stance, flags, stance_mask, sequence, 0))
                return false;
        }
        return true;
    }

    bool effect_failed() const noexcept { return effect_failed_; }

private:
    bool append(const Registration& registration) {
        output_.registrations.push_back(registration);
        if (registration.animation_dictionary_id >= 0 &&
            std::find(output_.animation_dictionary_ids.begin(),
                      output_.animation_dictionary_ids.end(),
                      registration.animation_dictionary_id) ==
                output_.animation_dictionary_ids.end()) {
            output_.animation_dictionary_ids.push_back(
                registration.animation_dictionary_id);
        }
        return true;
    }

    void fail_effect(const char* fallback) {
        effect_failed_ = true;
        if (error_.empty()) error_ = fallback;
    }

    const data::AnimationTables& animations_;
    const Effects& effects_;
    AnimSetState& output_;
    std::string& error_;
    bool effect_failed_{};
};

bool add_member(Compiler& compiler, const data::CharacterAnimations& row,
                std::int32_t raw_slot, std::int32_t stance,
                std::uint32_t flags, std::uint32_t mask) {
    const auto sequence = scalar_member(row, raw_slot);
    return compiler.add_table(sequence, stance, flags, mask, sequence, 0);
}

// Character::SetAnimationSet (IDA 0x3c9f4c) for one Character whose stance count
// is `stance_count` (COUNT_IPHONE for a Player, otherwise 1). The skill loop is
// inside the stance loop exactly as in the source; its 0x200000 flag only passes
// the stance mask at stance 0.
Result compile_state(AnimSetState& state, Identity expected_character,
                     const data::AnimationTables& animation_tables,
                     const data::SkillTables& skill_tables,
                     std::int32_t animation_table_id,
                     std::int32_t source_skill_list_id,
                     std::int32_t stance_count, std::uint32_t stance_mask,
                     const Effects& effects, std::string& error) {
    if (animation_table_id < 0 ||
        static_cast<std::size_t>(animation_table_id) >= animation_tables.characters.size()) {
        error = "Faery AnimSet CharAnimTable row is outside retained source tables";
        return Result::invalid_animation_table;
    }
    if (stance_count < 1) {
        error = "Character AnimSet stance count is not positive";
        return Result::malformed_source;
    }

    auto skill_list_id = source_skill_list_id;
    if (skill_list_id < 0 ||
        static_cast<std::size_t>(skill_list_id) >= skill_tables.skill_lists.size())
        skill_list_id = kFaerySkillListFallback;
    if (skill_list_id < 0 ||
        static_cast<std::size_t>(skill_list_id) >= skill_tables.skill_lists.size()) {
        error = "Faery AnimSet fallback SkillList row 3 is unavailable";
        return Result::invalid_skill_list;
    }
    const auto set_id_wide = (static_cast<std::int64_t>(animation_table_id) << 8) |
        static_cast<std::uint32_t>(skill_list_id);
    if (set_id_wide < 0 || set_id_wide > std::numeric_limits<std::int32_t>::max()) {
        error = "Faery AnimSet source identifier overflows signed source ID";
        return Result::invalid_animation_table;
    }

    if (state.phase == Phase::ready) {
        if (state.character == expected_character &&
            state.anim_set_id == static_cast<std::int32_t>(set_id_wide))
            return Result::already_registered;
        error = "CharacterVisual already owns a different source AnimSet";
        return Result::identity_mismatch;
    }
    if (state.phase != Phase::empty) {
        error = "CharacterVisual AnimSet registration is already in progress or failed";
        return Result::identity_mismatch;
    }

    state.character = expected_character;
    state.anim_set_id = static_cast<std::int32_t>(set_id_wide);
    state.animation_table_id = animation_table_id;
    state.skill_list_id = skill_list_id;
    state.phase = Phase::registering;

    Compiler compiler(animation_tables, effects, state, error);
    const auto& row = animation_tables.characters[static_cast<std::size_t>(animation_table_id)];

    // Character::SetAnimationSet first registers the template, then these four
    // un-stanced source members, all at stance/flags zero.
    if (!compiler.add_template(scalar_member(row, 36)) ||
        !add_member(compiler, row, 22, 0, 0, 0) ||
        !add_member(compiler, row, 25, 0, 0, 0) ||
        !add_member(compiler, row, 32, 0, 0, 0) ||
        !add_member(compiler, row, 23, 0, 0, 0)) {
        state.phase = Phase::failed;
        return compiler.effect_failed() ? Result::effect_failed : Result::malformed_source;
    }

    // Keep the complete source table order (non-player Characters use one stance).
    constexpr std::array<StancedMember, 28> stanced{{
        {10, 0x000002}, {37, 0x000010}, {28, 0x000020}, {1, 0x000040},
        {2, 0x000080}, {31, 0x000100}, {35, 0x000200}, {18, 0x000400},
        {9, 0x000800}, {15, 0x001000}, {3, 0x002000}, {8, 0x004000},
        {7, 0x008000}, {5, 0x010000}, {4, 0x020000}, {6, 0x040000},
        {26, 0x100000}, {27, 0x080000}, {38, 0}, {29, 0}, {12, 0x000004},
        {14, 0x000008}, {11, 0x000008}, {13, 0}, {39, 0}, {30, 0},
        {20, 0x1000000}, {21, 0x1000000},
    }};
    for (std::int32_t stance = 0; stance < stance_count; ++stance) {
        for (const auto& entry : stanced) {
            if (!add_member(compiler, row, entry.raw_member, stance,
                            entry.flags, stance_mask)) {
                state.phase = Phase::failed;
                return compiler.effect_failed() ? Result::effect_failed : Result::malformed_source;
            }
        }
        if (!compiler.add_array_member(row, 17, stance, 0x800000, stance_mask) ||
            !compiler.add_array_member(row, 34, stance, 0x400000, stance_mask)) {
            state.phase = Phase::failed;
            return compiler.effect_failed() ? Result::effect_failed : Result::malformed_source;
        }
        for (const auto skill_id : skill_tables.skill_lists[
                 static_cast<std::size_t>(skill_list_id)].members) {
            if (skill_id < 0 || static_cast<std::size_t>(skill_id) >= skill_tables.skills.size()) {
                error = "Faery SkillList references a Skill row outside retained source tables";
                state.phase = Phase::failed;
                return Result::invalid_skill_list;
            }
            const auto sequence = skill_tables.skills[static_cast<std::size_t>(skill_id)].anim;
            if (!compiler.add_table(sequence, stance, 0x200000, stance_mask, sequence, 0)) {
                state.phase = Phase::failed;
                return compiler.effect_failed() ? Result::effect_failed : Result::malformed_source;
            }
        }
    }

    state.phase = Phase::ready;
    return Result::registered;
}
} // namespace

std::vector<std::int32_t> AnimSetState::animation_dictionary_ids_for_sequence(
    std::int32_t source_sequence_id) const {
    std::vector<std::int32_t> result;
    for (const auto& registration : registrations) {
        if (registration.kind != RegistrationKind::animation ||
            registration.source_sequence_id != source_sequence_id ||
            registration.animation_dictionary_id < 0)
            continue;
        if (std::find(result.begin(), result.end(),
                      registration.animation_dictionary_id) == result.end())
            result.push_back(registration.animation_dictionary_id);
    }
    return result;
}

Result register_faery_anim_set(
    character_faery_visual_facade_v1::CharacterVisual& visual,
    Identity expected_character, const data::AnimationTables& animation_tables,
    const data::SkillTables& skill_tables, std::int32_t animation_table_id,
    std::int32_t source_skill_list_id, const Effects& effects,
    std::string& error) {
    error.clear();
    if (!expected_character) {
        error = "Faery AnimSet requires a live Character identity";
        return Result::invalid_identity;
    }
    if (visual.character != expected_character) {
        error = "Faery AnimSet CharacterVisual identity does not match factory Record";
        return Result::identity_mismatch;
    }
    // Non-player Characters use one stance (source Character::IsPlayer branch).
    return compile_state(visual.anim_set, expected_character, animation_tables,
                         skill_tables, animation_table_id, source_skill_list_id,
                         1, kSourceStanceMask, effects, error);
}

Result compile_character_anim_set(
    AnimSetState& state, Identity expected_character,
    const data::AnimationTables& animation_tables,
    const data::SkillTables& skill_tables, std::int32_t animation_table_id,
    std::int32_t source_skill_list_id, std::int32_t stance_count,
    std::uint32_t stance_mask, const Effects& effects, std::string& error) {
    error.clear();
    if (!expected_character) {
        error = "Character AnimSet requires a live Character identity";
        return Result::invalid_identity;
    }
    return compile_state(state, expected_character, animation_tables, skill_tables,
                         animation_table_id, source_skill_list_id, stance_count,
                         stance_mask, effects, error);
}

Result add_animation_dictionary(AnimSetState& state, Identity expected_character,
                                std::int32_t animation_dictionary_id, bool& added,
                                std::string& error) {
    error.clear();
    added = false;
    if (!expected_character) {
        error = "AnimDict registration requires a live Character identity";
        return Result::invalid_identity;
    }
    if (state.character != expected_character || state.phase != Phase::ready) {
        error = "AnimDict registration requires the ready AnimSet of the same Character";
        return Result::identity_mismatch;
    }
    if (animation_dictionary_id < 0) return Result::already_registered;
    // The source set is keyed by AnimDict id: an AnimDict an authored AddAnim
    // already put in the set is found and nothing is loaded again. The
    // `added_dictionary` row still records that the script path named it, so
    // the sequence-keyed projection accepts it for any scripted AnimTable row.
    const bool in_set = std::find(state.animation_dictionary_ids.begin(),
                                  state.animation_dictionary_ids.end(),
                                  animation_dictionary_id) !=
        state.animation_dictionary_ids.end();
    const bool recorded = std::any_of(state.registrations.begin(),
        state.registrations.end(), [&](const Registration& row) {
            return row.kind == RegistrationKind::added_dictionary &&
                row.animation_dictionary_id == animation_dictionary_id;
        });
    if (!recorded) {
        Registration registration;
        registration.kind = RegistrationKind::added_dictionary;
        registration.animation_dictionary_id = animation_dictionary_id;
        state.registrations.push_back(registration);
    }
    if (in_set) return Result::already_registered;
    state.animation_dictionary_ids.push_back(animation_dictionary_id);
    added = true;
    return Result::registered;
}

bool retire(character_faery_visual_facade_v1::CharacterVisual& visual,
            Identity expected_character) noexcept {
    if (!expected_character || visual.character != expected_character ||
        visual.anim_set.character != expected_character)
        return false;
    visual.anim_set = {};
    return true;
}

} // namespace dh2::character_anim_set_owner_v1
