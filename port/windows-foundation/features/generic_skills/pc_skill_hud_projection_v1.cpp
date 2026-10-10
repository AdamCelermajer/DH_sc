#include "pc_skill_hud_projection_v1.hpp"

#include <algorithm>

namespace dh::foundation::generic_skills {
namespace {
bool fail(std::string& error, const char* reason) {
    error = reason;
    return false;
}

constexpr std::array<std::uint32_t, 3> source_slot_by_physical_position{{2, 0, 1}};

bool active_cast_phase(RuntimeSkillCastPhaseV1 phase) {
    return phase == RuntimeSkillCastPhaseV1::prepared_pending_use ||
           phase == RuntimeSkillCastPhaseV1::use_applied;
}
}

bool project_pc_skill_hud_v1(
    CharacterState& state, ActorId same_source_actor,
    const dh2::data::CharacterTable& characters,
    dh2::data::SkillTables::Borrow skills, std::uint32_t equipment_set,
    const std::array<PcSkillHudSourceStatusV1, 3>& source_status_by_slot,
    const RuntimeSkillCastReceiptV1* current_cast,
    PcSkillHudFrameV1& output, std::string& error) {
    error.clear();
    if (state.id.empty() || same_source_actor == invalid_actor_id || !skills || equipment_set >= 2)
        return fail(error, "PC skill HUD requires same CharacterState, actor, SkillTables and saved set");
    if (!state.source_skill_slots_known)
        return fail(error, "PC skill HUD requires the actual saved source skill-slot owner");

    for (std::uint32_t slot = 0; slot < source_status_by_slot.size(); ++slot) {
        if (source_status_by_slot[slot].source_slot != slot)
            return fail(error, "PC skill HUD runtime status is not indexed by logical source slot");
    }

    ViewV1 source_view;
    PageV1 page(state, characters, skills);
    if (!page.view(source_view, error)) return false;
    if (!source_view.slots_source_known)
        return fail(error, "PC skill HUD source slot view is not authoritative");

    PcSkillHudFrameV1 next;
    next.character_state_id = state.id;
    next.source_actor = same_source_actor;
    next.equipment_set = equipment_set;
    for (std::uint32_t physical = 0; physical < next.left_middle_right.size(); ++physical) {
        auto& cell = next.left_middle_right[physical];
        const std::uint32_t source_slot = source_slot_by_physical_position[physical];
        cell.physical_position = physical;
        cell.pc_key_number = physical + 1;
        cell.source_slot = source_slot;
        cell.key_label = std::to_string(cell.pc_key_number);
        cell.source_cooldown_frame = source_status_by_slot[source_slot].cooldown_frame;
        cell.source_usable = source_status_by_slot[source_slot].usable;

        const SlotV1* assignment = nullptr;
        for (const auto& slot : source_view.slots) {
            if (slot.equipment_set != equipment_set || slot.slot != source_slot) continue;
            if (assignment)
                return fail(error, "PC skill HUD found duplicate saved assignments for one source slot");
            assignment = &slot;
        }
        if (!assignment) continue;
        if (!assignment->saved_skill_row || !assignment->class_skill_position)
            return fail(error, "PC skill HUD saved assignment has no matching original class-list row");
        const auto row = std::find_if(source_view.rows.begin(), source_view.rows.end(),
            [&](const RowV1& candidate) {
                return candidate.saved_skill_row == assignment->saved_skill_row &&
                       candidate.position == *assignment->class_skill_position;
            });
        if (row == source_view.rows.end())
            return fail(error, "PC skill HUD assignment no longer matches its source SkillList row");
        cell.assigned = true;
        cell.saved_skill_row = row->saved_skill_row;
        cell.saved_rank = row->saved_rank;
        cell.class_skill_position = row->position;
        cell.skill_table_id = row->table_id;
        cell.source_skill_name = row->source_name;
        cell.source_icon_key = row->source_icon;
    }

    if (current_cast) {
        if (current_cast->actor != same_source_actor)
            return fail(error, "PC skill HUD cast receipt belongs to another source actor");
        if (active_cast_phase(current_cast->phase) &&
            !current_cast->native_hud_spell &&
            current_cast->equipment_set == equipment_set &&
            current_cast->source_slot < 3) {
            auto& cell = next.left_middle_right[
                current_cast->source_slot == 2 ? 0u :
                current_cast->source_slot == 0 ? 1u : 2u];
            if (cell.assigned && cell.saved_skill_row == current_cast->saved_skill_row) {
                if (!current_cast->generation)
                    return fail(error, "Active PC skill HUD cast receipt has no source generation");
                cell.cast_in_progress = true;
                cell.cast_generation = current_cast->generation;
            }
        }
    }

    output = std::move(next);
    error.clear();
    return true;
}

bool pc_skill_hud_key_for_hit_v1(const PcSkillHudFrameV1& frame,
                                 std::uint32_t physical_position,
                                 std::uint32_t& pc_key_number,
                                 std::string& error) {
    error.clear();
    if (physical_position >= frame.left_middle_right.size())
        return fail(error, "PC skill HUD hit is outside the three authored skill cells");
    const auto& cell = frame.left_middle_right[physical_position];
    if (cell.physical_position != physical_position ||
        cell.pc_key_number != physical_position + 1 ||
        cell.source_slot != source_slot_by_physical_position[physical_position] ||
        cell.key_label != std::to_string(physical_position + 1))
        return fail(error, "PC skill HUD frame has stale or inconsistent cell/key identity");
    pc_key_number = cell.pc_key_number;
    error.clear();
    return true;
}

} // namespace dh::foundation::generic_skills
