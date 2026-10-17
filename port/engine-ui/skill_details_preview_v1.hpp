#pragma once

namespace dh2::ui {

enum class SkillDetailCurrentTextV1 : unsigned char {
    unlock_level,
    needs_skill_points,
    current_rank,
};

enum class SkillDetailNextTextV1 : unsigned char {
    empty,
    maximum_level,
    training_blocked,
    next_rank,
};

struct SkillDetailPreviewV1 {
    bool unlocked{};
    SkillDetailCurrentTextV1 current{};
    SkillDetailNextTextV1 next{};
};

// NativeGetSkillDetails chooses localized preview text before it formats rank
// DisplayProps. These states mirror its character-level, zero-rank, cap, and
// CanIncrementSkill branches; this helper owns no Save or skill data.
inline SkillDetailPreviewV1 skill_detail_preview_v1(
        int character_level, int required_level, int current_rank,
        int skill_cap, bool can_increment) noexcept {
    if (character_level < required_level)
        return {false, SkillDetailCurrentTextV1::unlock_level,
                SkillDetailNextTextV1::empty};
    const auto current = current_rank <= 0
        ? SkillDetailCurrentTextV1::needs_skill_points
        : SkillDetailCurrentTextV1::current_rank;
    if (skill_cap <= current_rank)
        return {true, current, SkillDetailNextTextV1::maximum_level};
    if (!can_increment)
        return {true, current, SkillDetailNextTextV1::training_blocked};
    return {true, current, SkillDetailNextTextV1::next_rank};
}

} // namespace dh2::ui
