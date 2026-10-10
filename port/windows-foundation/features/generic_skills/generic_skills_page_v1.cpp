#include "generic_skills_page_v1.hpp"

#include <algorithm>
#include <cstring>
#include <limits>
#include <stdexcept>

namespace dh::foundation::generic_skills {
namespace {
bool fail(std::string& error, const char* message) {
    error = message;
    return false;
}

bool fail(std::string& error, const std::string& message) {
    error = message;
    return false;
}

int signed_word(std::uint32_t raw) noexcept {
    std::int32_t value{};
    std::memcpy(&value, &raw, sizeof(value));
    return value;
}

int skill_tree_column(const dh2::data::CharacterTable& characters) {
    const auto at = std::find(characters.fields.begin(), characters.fields.end(), "SkillTree");
    return at == characters.fields.end() ? -1 : static_cast<int>(at - characters.fields.begin());
}

bool row_matches_list(const CharacterState& state, const std::vector<std::int32_t>& list,
                      const dh2::data::SkillTables::Borrow& tables) {
    if (list.size() != state.skills.size()) return false;
    const auto& names = tables.skill_names();
    for (std::size_t i = 0; i < list.size(); ++i) {
        const auto id = list[i];
        if (id < 0 || static_cast<std::size_t>(id) >= names.size() ||
            state.skills[i].id != names[static_cast<std::size_t>(id)]) return false;
    }
    return true;
}

std::optional<unsigned> find_saved_row(const CharacterState& state, const std::string& id) {
    std::optional<unsigned> found;
    for (std::size_t i = 0; i < state.skills.size(); ++i) {
        if (state.skills[i].id != id) continue;
        if (found) return std::nullopt;
        found = static_cast<unsigned>(i);
    }
    return found;
}
} // namespace

PageV1::PageV1(CharacterState& state, const dh2::data::CharacterTable& characters,
               dh2::data::SkillTables::Borrow tables, ServicesV1 services)
    : state_(&state), characters_(&characters), tables_(std::move(tables)),
      services_(std::move(services)) {}

bool PageV1::resolve(ViewV1& out, std::string& error) const {
    error.clear();
    if (!state_ || !characters_) return fail(error, "Generic skills page has no CharacterState owner");
    if (!tables_) return fail(error, "Generic skills page requires the original SkillTables borrow");
    const auto class_at = std::find(characters_->names.begin(), characters_->names.end(), state_->class_id);
    if (class_at == characters_->names.end())
        return fail(error, "CharacterState.class_id is not an original CharacterTable row token");
    const auto class_row = static_cast<int>(class_at - characters_->names.begin());
    const auto column = skill_tree_column(*characters_);
    if (column < 0) return fail(error, "Original CharacterTable has no SkillTree field");
    if (static_cast<std::size_t>(class_row) >= characters_->rows.size() ||
        static_cast<std::size_t>(column) >= characters_->rows[static_cast<std::size_t>(class_row)].size())
        return fail(error, "Original CharacterTable class row has no SkillTree value");

    const int authored_list = characters_->rows[static_cast<std::size_t>(class_row)][static_cast<std::size_t>(column)];
    if (authored_list < 0 || static_cast<std::size_t>(authored_list) >= tables_.lists().size())
        return fail(error, "Original CharacterTable.SkillTree references an absent SkillList");

    if (state_->source_skill_slots_known && state_->skills.empty())
        return fail(error, "Source-known skill slots have no saved CharacterState skill rows");

    int active_list = authored_list;
    bool from_saved_rows = false;
    if (!state_->skills.empty()) {
        for (std::size_t i = 0; i < tables_.lists().size(); ++i) {
            if (!row_matches_list(*state_, tables_.lists()[i], tables_)) continue;
            active_list = static_cast<int>(i);
            from_saved_rows = true;
            // Prefer the class metadata ID when the saved rows have identical
            // contents; otherwise retain the exact matching source list row.
            if (active_list == authored_list) break;
        }
        if (state_->source_skill_slots_known && !from_saved_rows)
            return fail(error, "Source-known saved skill rows do not match any original SkillList order");
    }

    const auto& list = tables_.lists()[static_cast<std::size_t>(active_list)];
    const auto& skills = tables_.skills();
    const auto& names = tables_.skill_names();
    ViewV1 next;
    next.character_row = class_row;
    next.authored_skill_list_id = authored_list;
    next.active_skill_list_id = active_list;
    next.active_list_from_saved_rows = from_saved_rows && active_list != authored_list;
    next.skill_points_known = state_->source_points_known;
    if (next.skill_points_known) next.skill_points = state_->source_skill_points;
    next.slots_source_known = state_->source_skill_slots_known;

    for (std::size_t position = 0; position < list.size(); ++position) {
        const auto id = list[position];
        if (id < 0 || static_cast<std::size_t>(id) >= skills.size() ||
            static_cast<std::size_t>(id) >= names.size())
            return fail(error, "Original SkillList references an absent SkillTable row");
        const auto& record = skills[static_cast<std::size_t>(id)];
        RowV1 row;
        row.position = static_cast<int>(position);
        row.table_id = id;
        row.source_name = names[static_cast<std::size_t>(id)];
        row.source_icon = record.icon;
        row.required_level = signed_word(record.scalar.words[8]);

        if (from_saved_rows) {
            row.saved_skill_row = static_cast<std::uint32_t>(position);
            row.saved_rank = state_->skills[position].rank;
        } else if (const auto saved = find_saved_row(*state_, row.source_name)) {
            row.saved_skill_row = *saved;
            row.saved_rank = state_->skills[*saved].rank;
        }
        if (services_.localized_text &&
            !services_.localized_text(*state_, id, row.localized_name, row.description, error)) return false;
        next.rows.push_back(std::move(row));
    }

    for (const auto& binding : state_->skill_slots) {
        SlotV1 slot;
        slot.equipment_set = binding.equipment_set;
        slot.slot = binding.slot;
        slot.saved_skill_row = binding.saved_skill_row;
        if (binding.saved_skill_row < state_->skills.size()) {
            const auto& saved = state_->skills[binding.saved_skill_row];
            const auto at = std::find_if(next.rows.begin(), next.rows.end(), [&](const RowV1& row) {
                return row.saved_skill_row && *row.saved_skill_row == binding.saved_skill_row &&
                       row.source_name == saved.id;
            });
            if (at != next.rows.end()) slot.class_skill_position = at->position;
        }
        next.slots.push_back(std::move(slot));
    }
    out = std::move(next);
    return true;
}

bool PageV1::view(ViewV1& out, std::string& error) const { return resolve(out, error); }

bool PageV1::select(int position, std::string& error) {
    ViewV1 current;
    if (!resolve(current, error)) return false;
    if (position < 0 || static_cast<std::size_t>(position) >= current.rows.size())
        return fail(error, "Skill selection is outside the active authored SkillList");
    selected_position_ = position;
    selected_list_ = current.active_skill_list_id;
    return true;
}

bool PageV1::assign(unsigned equipment_set, unsigned source_slot, std::string& error) {
    ViewV1 current;
    if (!resolve(current, error)) return false;
    if (!selected_position_ || selected_list_ != current.active_skill_list_id ||
        *selected_position_ < 0 || static_cast<std::size_t>(*selected_position_) >= current.rows.size())
        return fail(error, "No current authored skill row is selected");
    if (equipment_set >= 2 || source_slot >= 3)
        return fail(error, "Saved skill assignment is outside the original two-set, three-slot bounds");
    const auto& row = current.rows[static_cast<std::size_t>(*selected_position_)];
    if (!row.saved_skill_row || *row.saved_skill_row >= state_->skills.size())
        return fail(error, "Selected skill has no unambiguous saved CharacterState row");
    if (!row.saved_rank || *row.saved_rank == 0)
        return fail(error, "Character::IsSkillEquippable requires a saved rank above zero");
    if (row.table_id < 0 || static_cast<std::size_t>(row.table_id) >= tables_.skills().size())
        return fail(error, "Selected skill has no original SkillTable record");
    if (tables_.skills()[static_cast<std::size_t>(row.table_id)].scalar.words[18] == 0xffffffffu)
        return fail(error, "Original SkillTable marks this skill non-assignable");
    if (!services_.can_assign)
        return fail(error, "Generic skill assignment requires the source availability provider");
    bool available = false;
    if (!services_.can_assign(*state_, row.table_id, row.position, available, error)) {
        if (error.empty()) error = "Source skill availability provider failed";
        return false;
    }
    if (!available) return fail(error, "Source skill availability rejected assignment");

    auto found = state_->skill_slots.end();
    for (auto it = state_->skill_slots.begin(); it != state_->skill_slots.end(); ++it) {
        if (it->equipment_set != equipment_set || it->slot != source_slot) continue;
        if (found != state_->skill_slots.end())
            return fail(error, "CharacterState has duplicate bindings for this skill slot");
        found = it;
    }
    if (found != state_->skill_slots.end()) found->saved_skill_row = *row.saved_skill_row;
    else state_->skill_slots.push_back({equipment_set, source_slot, *row.saved_skill_row});
    return true;
}

bool PageV1::train(std::string& error) {
    ViewV1 current;
    if (!resolve(current, error)) return false;
    if (!selected_position_ || selected_list_ != current.active_skill_list_id ||
        *selected_position_ < 0 || static_cast<std::size_t>(*selected_position_) >= current.rows.size())
        return fail(error, "No current authored skill row is selected");
    if (!services_.train) return fail(error, "Generic skill training requires the original progression/action provider");
    const auto& row = current.rows[static_cast<std::size_t>(*selected_position_)];
    const CharacterState before = *state_;
    try {
        if (!services_.train(*state_, current.active_skill_list_id, row.position, row.table_id, error)) {
            *state_ = before;
            if (error.empty()) error = "Original progression/action provider rejected skill training";
            return false;
        }
    } catch (const std::exception& ex) {
        *state_ = before;
        return fail(error, std::string("Generic skill training provider threw: ") + ex.what());
    } catch (...) {
        *state_ = before;
        return fail(error, "Generic skill training provider threw an unknown exception");
    }
    return true;
}

} // namespace dh::foundation::generic_skills
