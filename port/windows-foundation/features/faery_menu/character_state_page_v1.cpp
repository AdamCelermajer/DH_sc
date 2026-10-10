#include "character_state_page_v1.hpp"

#include "native_binding_v1.hpp"

namespace dh::foundation::faery_menu {
namespace {
constexpr std::array<std::uint32_t, 5> source_element_by_slot{{2, 3, 1, 4, 0}};

bool source_list(const CharacterStateFaeryBindingsV1& bindings,
                 const std::vector<std::int32_t>*& ids,
                 std::array<CharacterStateFaerySlotV1, 5>& slots,
                 std::string& error) {
    ids = nullptr;
    if (!bindings.character || !bindings.tables) {
        error = "CharacterState Faery page requires owned source state and actual FaeryTables";
        return false;
    }
    const auto list_id = bindings.character->source_faery_list_id;
    if (list_id < -1) {
        error = "CharacterState source FaeryList ID is outside known/unknown range";
        return false;
    }
    if (list_id < 0 || std::size_t(list_id) >= bindings.tables.list_names().size() ||
        std::size_t(list_id) >= bindings.tables.lists().size()) {
        // An older CharacterState can still open the page, but list order and
        // source labels are unknown and therefore no rows are projected.
        error.clear();
        return true;
    }
    const auto& list = bindings.tables.lists()[std::size_t(list_id)];
    const auto& rows = bindings.tables.faeries();
    if (list.size() != slots.size()) {
        error = "Source CharacterState FaeryList is not the five-slot authored Faery page";
        return false;
    }
    for (std::size_t slot = 0; slot < slots.size(); ++slot) {
        const auto record_id = list[slot];
        if (record_id < 0 || std::size_t(record_id) >= rows.size() ||
            std::size_t(record_id) >= bindings.tables.faery_names().size()) {
            error = "Source CharacterState FaeryList references an invalid Faery row";
            return false;
        }
        const auto& row = rows[std::size_t(record_id)];
        // The original SWF button sprite embeds element-specific art at each
        // source slot. Validate the actual table order against those authored
        // icon facts instead of silently showing a mismatched icon.
        if (row.scalar.words[2] != source_element_by_slot[slot] ||
            row.scalar.words[8] != slot) {
            error = "Actual FaeryList order/Elemental/Type differs from exported source button icons";
            return false;
        }
        auto& projected = slots[slot];
        projected.menu_slot = static_cast<std::int32_t>(slot);
        projected.table_record_id = record_id;
        projected.table_name = bindings.tables.faery_names()[std::size_t(record_id)];
        projected.description_symbol = row.scalar.words[1];
        projected.elemental_id = row.scalar.words[2];
        projected.model_file_id = row.scalar.words[3];
        projected.name_symbol = row.scalar.words[4];
        projected.spell_type = static_cast<std::int32_t>(row.scalar.words[7]);
        projected.source_type = static_cast<std::int32_t>(row.scalar.words[8]);
        projected.spell_script = row.script;
    }
    ids = &list;
    error.clear();
    return true;
}

bool append_page_to_menu(const CharacterStateFaeryPageV1& page,
                         dh::foundation::character_menu::Frame& output,
                         std::string& error) {
    auto next = output;
    const auto page_start = next.art.batches.size();
    next.art.batches.insert(next.art.batches.end(), page.art.begin(), page.art.end());
    const auto button_at = original_faery_buttons_insert_at();
    std::size_t button_count = 0;
    for (const auto& button : page.button_art) {
        if (button.empty()) continue; // Unknown state has no honest SWF button frame.
        const auto at = next.art.batches.begin() +
            static_cast<std::ptrdiff_t>(page_start + button_at + button_count);
        next.art.batches.insert(at, button.begin(), button.end());
        button_count += button.size();
    }
    for (const auto& slot_solids : page.button_solids) {
        for (const auto& solid : slot_solids) {
            dh::foundation::character_menu::MenuSolidBatch batch;
            batch.geometry = solid.geometry;
            batch.rgba = solid.rgba;
            batch.after_bitmap_role = solid.after_bitmap_role;
            next.solids.push_back(std::move(batch));
        }
    }
    if (!page.selected_faery_art.batches.empty()) {
        const auto image_at = page_start + original_faery_image_insert_at() + button_count;
        next.art.batches.insert(next.art.batches.begin() +
            static_cast<std::ptrdiff_t>(image_at),
            page.selected_faery_art.batches.begin(), page.selected_faery_art.batches.end());
    }
    for (const auto& item : page.text) {
        const auto& f = item.field;
        dh::foundation::character_menu::MenuTextField field{
            f.path, f.character_id, f.font_id, f.source_height, f.bounds, f.rgba,
            f.align, f.matrix, f.local_bounds, f.margins, f.leading};
        next.text.push_back({std::move(field), item.value});
    }
    output = std::move(next);
    error.clear();
    return true;
}
}

bool present_character_state_faery_v1(const CharacterStateFaeryBindingsV1& bindings,
                                      CharacterStateFaeryPageV1& output,
                                      std::string& error) {
    if (!bindings.owner || !bindings.character || !bindings.tables ||
        !bindings.validate_same_character ||
        bindings.difficulty < 0 || bindings.difficulty >= 3 || !bindings.localize) {
        error = "CharacterState Faery page requires same-owner Character, source tables, difficulty and localization";
        return false;
    }
    if (!bindings.validate_same_character(bindings.character, error)) {
        if (error.empty()) error = "Faery page CharacterState is not the selected owner's state";
        return false;
    }
    CharacterStateFaeryPageV1 next;
    next.list_id = bindings.character->source_faery_list_id;
    next.difficulty = bindings.difficulty;
    if (next.list_id >= 0 && std::size_t(next.list_id) < bindings.tables.list_names().size())
        next.list_name = bindings.tables.list_names()[std::size_t(next.list_id)];
    next.art = original_faery_art().batches;

    const std::vector<std::int32_t>* list{};
    if (!source_list(bindings, list, next.slots, error)) return false;
    const bool known = bindings.character->source_faery_state_known;
    if (known && !list) {
        error = "Known CharacterState Faery state has no valid source FaeryList";
        return false;
    }
    if (known) {
        const auto& difficulty = bindings.character->faery_by_difficulty[
            std::size_t(bindings.difficulty)];
        if (difficulty.current_faery < 0 || difficulty.current_faery >= 5) {
            error = "Known source current Faery is outside menu slots 0..4";
            return false;
        }
        next.selected_slot = difficulty.current_faery;
        next.selected_faery_art.batches = original_faery_image(
            static_cast<unsigned>(next.selected_slot));
        bool debug_known = bool(bindings.debug_unlock_all_faeries);
        bool debug_unlock_all = false;
        if (debug_known && !bindings.debug_unlock_all_faeries(debug_unlock_all, error)) {
            if (error.empty()) error = "Same-owner source UnlockAllFaeries Debug query failed";
            return false;
        }
        for (std::size_t slot = 0; slot < next.slots.size(); ++slot) {
            auto& row = next.slots[slot];
            const auto& saved = difficulty.faeries[slot];
            row.raw_saved_state = saved.state;
            row.saved_level = saved.level;
            row.knowledge = source_faery_knowledge_v1(true, saved.state,
                                                      debug_known, debug_unlock_all);
            // ActionScript computes all locked button frames first, then
            // applies Focused to the actual saved selection, even if locked.
            row.visual = static_cast<std::int32_t>(slot) == next.selected_slot
                ? ButtonVisual::focused
                : row.knowledge == SourceFaeryKnowledgeV1::known
                    ? ButtonVisual::idle : ButtonVisual::locked;
            if (row.knowledge == SourceFaeryKnowledgeV1::unknown &&
                static_cast<std::int32_t>(slot) != next.selected_slot) continue;
            next.button_art[slot] = original_faery_button(
                static_cast<unsigned>(slot), row.visual);
            next.button_solids[slot] = original_faery_button_solids(
                static_cast<unsigned>(slot), row.visual);
        }
    } else {
        // Keep actual list/name/icon facts, but never interpret constructor
        // zeroes or legacy placeholders as locked/unlocked/current values.
        next.selected_slot = -1;
        for (auto& slot : next.slots) slot.knowledge = SourceFaeryKnowledgeV1::unknown;
    }

    for (const auto& field : original_faery_art().text_fields) {
        std::string value;
        if (field.path.find("menu_title/") != std::string::npos) {
            std::string symbol;
            if (!source_text_symbol_v1(field.path, -1, 0, symbol, error) ||
                !bindings.localize(symbol, value, error)) return false;
        } else if (next.selected_slot >= 0) {
            std::string symbol;
            const auto level = known ? next.slots[std::size_t(next.selected_slot)].saved_level : 0;
            if (!source_text_symbol_v1(field.path, next.selected_slot, level,
                                       symbol, error) ||
                !bindings.localize(symbol, value, error)) return false;
        }
        next.text.push_back({field, std::move(value)});
    }
    output = std::move(next);
    error.clear();
    return true;
}

bool select_character_state_faery_v1(CharacterStateFaeryBindingsV1& bindings,
                                      std::uint32_t menu_slot,
                                      std::string& error) {
    if (!bindings.owner || !bindings.character || !bindings.tables ||
        !bindings.validate_same_character || bindings.difficulty < 0 ||
        bindings.difficulty >= 3 || menu_slot >= 5) {
        error = "CharacterState Faery selection requires source slot 0..4 and difficulty 0..2";
        return false;
    }
    if (!bindings.validate_same_character(bindings.character, error)) {
        if (error.empty()) error = "Faery selection CharacterState is not the selected owner's state";
        return false;
    }
    if (!bindings.character->source_faery_state_known) {
        error = "CharacterState has no known source faery selection/progress to mutate";
        return false;
    }
    std::array<CharacterStateFaerySlotV1, 5> slots{};
    const std::vector<std::int32_t>* list{};
    if (!source_list(bindings, list, slots, error) || !list) {
        if (error.empty()) error = "CharacterState has no valid original FaeryList for selection";
        return false;
    }
    const auto index = std::size_t(bindings.difficulty);
    const auto current = bindings.character->faery_by_difficulty[index].current_faery;
    if (current < 0 || current >= 5) {
        error = "Known source current Faery is outside menu slots 0..4";
        return false;
    }
    // All source assertions are checked before dispatch. The original SWF
    // dispatches locked-looking buttons too, so do not reject based on state.
    // Production binds the same-owner source action to retain UpdateAllSkills,
    // visual and Level placement continuation; the direct cell commit is only
    // the focused projection fixture path when no action owner is supplied.
    if (bindings.activate_slot) {
        if (!bindings.activate_slot(menu_slot, error)) {
            if (error.empty()) error = "Same-owner source SetActiveFaery action failed";
            return false;
        }
        error.clear();
        return true;
    }
    auto& current_slot = bindings.character->faery_by_difficulty[index].current_faery;
    const auto before = current_slot;
    current_slot = static_cast<std::int32_t>(menu_slot);
    if (current_slot != static_cast<std::int32_t>(menu_slot)) {
        current_slot = before;
        error = "Transactional source Faery selection did not commit";
        return false;
    }
    error.clear();
    return true;
}

bool bind_character_state_faery_provider_v1(
    CharacterStateFaeryBindingsV1 bindings,
    dh::foundation::character_menu::SourcePageProviderV1& output,
    std::string& error) {
    if (!bindings.owner || !bindings.character || !bindings.tables ||
        !bindings.validate_same_character || !bindings.localize) {
        error = "CharacterState Faery provider requires same owner/state, actual table and localization";
        return false;
    }
    const auto owner = bindings.owner;
    auto ready = [bindings](std::string& e) {
        CharacterStateFaeryPageV1 discarded;
        return present_character_state_faery_v1(bindings, discarded, e);
    };
    auto append = [bindings](dh::foundation::character_menu::Frame& frame,
                             std::string& e) {
        CharacterStateFaeryPageV1 page;
        if (!present_character_state_faery_v1(bindings, page, e)) return false;
        return append_page_to_menu(page, frame, e);
    };
    auto release = [bindings = std::move(bindings)](float x, float y,
                                                   std::string& e) mutable {
        const auto slot = slot_at(x, y);
        if (slot < 0) {
            e.clear(); // Original menu consumes non-button releases without mutation.
            return true;
        }
        return select_character_state_faery_v1(
            bindings, static_cast<std::uint32_t>(slot), e);
    };
    output = {owner, std::move(ready), std::move(append), std::move(release)};
    error.clear();
    return true;
}

} // namespace dh::foundation::faery_menu

