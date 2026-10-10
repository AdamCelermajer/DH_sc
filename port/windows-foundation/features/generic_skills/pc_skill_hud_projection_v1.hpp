#pragma once

#include "generic_skills_page_v1.hpp"
#include "runtime_skill_cast_coordinator_v1.hpp"

#include <array>
#include <optional>

namespace dh::foundation::generic_skills {

// Runtime status is borrowed from the existing authored HUD/source producer.
// Unknown cooldown or usability values stay unknown; this projection does not
// derive, start, or clear source timers.
struct PcSkillHudSourceStatusV1 {
    std::uint32_t source_slot = 0;
    std::optional<std::int32_t> cooldown_frame;
    std::optional<bool> usable;
};

struct PcSkillHudCellV1 {
    // Physical left/middle/right cell index and the PC label printed on it.
    std::uint32_t physical_position = 0;
    std::uint32_t pc_key_number = 1;
    // Logical CharacterState/NativeHUDSkill identity; never reordered in Save.
    std::uint32_t source_slot = 2;
    std::string key_label;
    bool assigned = false;
    std::optional<std::uint32_t> saved_skill_row;
    std::optional<std::uint32_t> saved_rank;
    std::optional<int> class_skill_position;
    std::optional<int> skill_table_id;
    std::optional<std::string> source_skill_name;
    std::optional<std::string> source_icon_key;
    std::optional<std::int32_t> source_cooldown_frame;
    std::optional<bool> source_usable;
    bool cast_in_progress = false;
    std::uint64_t cast_generation = 0;
};

struct PcSkillHudFrameV1 {
    std::string character_state_id;
    ActorId source_actor = invalid_actor_id;
    std::uint32_t equipment_set = 0;
    // Physical left, middle, right. Each cell carries its original logical
    // slot and exact SkillTable identity for the PC renderer.
    std::array<PcSkillHudCellV1, 3> left_middle_right{};
};

// Builds a PC-facing frame from the same saved CharacterState and original
// SkillTables used by the Skills page. `source_status_by_slot` is logical
// [0,1,2]; the output is physical [2,0,1]. An optional live cast receipt is
// consumed only when it proves the same actor/set/source row.
bool project_pc_skill_hud_v1(
    CharacterState&, ActorId same_source_actor,
    const dh2::data::CharacterTable&, dh2::data::SkillTables::Borrow,
    std::uint32_t equipment_set,
    const std::array<PcSkillHudSourceStatusV1, 3>& source_status_by_slot,
    const RuntimeSkillCastReceiptV1* current_cast,
    PcSkillHudFrameV1&, std::string& error);

// Called only after the existing authored HUD shape traversal confirms a
// physical left/middle/right skill-cell hit. Returns the PC key number to emit
// into SemanticInput; the existing pre-cast mapper then converts that key to
// its logical source slot exactly once. Empty cells still return their key so
// the normal source assignment gate remains authoritative.
bool pc_skill_hud_key_for_hit_v1(const PcSkillHudFrameV1&,
                                 std::uint32_t physical_position,
                                 std::uint32_t& pc_key_number,
                                 std::string& error);

} // namespace dh::foundation::generic_skills
