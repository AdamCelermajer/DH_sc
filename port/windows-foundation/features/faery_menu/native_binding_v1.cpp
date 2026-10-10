#include "native_binding_v1.hpp"

#include <utility>

namespace dh::foundation::faery_menu {
namespace {
struct NativeState {
    NativeOwnerInputV1 input;
    std::uintptr_t character{};

    bool validate(std::string& error) const {
        const auto& record = input.record;
        if (!input.selected_character_owner || !record || !record->actor ||
            !record->actor->object || record->failed || !record->save || !record->load ||
            !record->profile_bootstrap || !record->player_script_owner_v62 ||
            !record->faery_association_v68) {
            error = "Faery page requires one retained canonical Player/profile owner graph";
            return false;
        }
        const auto identity = record->actor->object->identity;
        const auto profile = record->profile_bootstrap->profile();
        const auto* save_slot = record->save_fields
            ? record->save_fields->save_slot14e8() : nullptr;
        if (!identity || identity != character || record->save->character() != identity ||
            record->player_script_owner_v62->native_savegame() != record->save.get() ||
            record->profile_bootstrap->save() != record->save ||
            record->profile_bootstrap->load_owner() != record->load ||
            !profile || !profile->ready() ||
            record->load->profile().identity != profile->receiver().identity ||
            !save_slot || *save_slot != reinterpret_cast<std::uintptr_t>(record->save.get())) {
            error = "Faery page Save/Profile/Character aliases are not the same source owners";
            return false;
        }
        if (!record->actor->source_ai_pointers_v105() ||
            record->faery_association_v68->character != identity ||
            record->actor->source_ai_pointers_v105()->auxiliary58 !=
                record->faery_association_v68->faery420) {
            error = "Faery association does not match the same native CharAI+58/Character+420 field";
            return false;
        }
        if (!input.actions || !input.queries || !input.query_graph ||
            !input.faery_actions || !input.query_graph->owner ||
            input.query_graph->owner.get() != input.selected_character_owner.get() ||
            !input.actions->bindings().owner ||
            input.actions->bindings().owner.get() != input.selected_character_owner.get() ||
            input.query_graph->actions != input.actions ||
            input.query_graph->faery_actions != input.faery_actions ||
            input.faery_actions->bindings().actions != input.actions ||
            input.actions->bindings().save != record->save.get() ||
            !input.actions->bindings().equipment ||
            !input.actions->bindings().equipment->inventory() ||
            input.actions->bindings().equipment->inventory()->character() != identity ||
            input.actions->bindings().skills.identity() != record->player_script_owner_v62.get() ||
            !input.query_graph->text || !input.query_graph->difficulty) {
            error = "Faery page requires same-owner CharacterMenu Queries/Actions/Text graph";
            return false;
        }
        if (!input.faery_tables || !record->services.faeries_v70 ||
            &input.faery_tables.lists() != &record->services.faeries_v70.lists() ||
            &input.faery_tables.faeries() != &record->services.faeries_v70.faeries() ||
            input.faery_tables.lists().empty() ||
            input.faery_tables.lists().front() != std::vector<std::int32_t>{2, 4, 5, 6, 3}) {
            error = "Faery page requires the same source FaeryTables borrow and first five-list IDs [2,4,5,6,3]";
            return false;
        }
        if (!input.actions->validate_graph(true, error)) return false;
        error.clear();
        return true;
    }

    bool query_bool(const char* name, std::uint32_t slot, bool& value,
                    std::string& error) const {
        if (!validate(error)) return false;
        dh2::ui::CharacterMenuCallV1 call;
        call.arguments = {dh2::ui::CharacterMenuValueV1::numeric(slot),
                          dh2::ui::CharacterMenuValueV1::numeric(0)};
        if (!input.queries->dispatch(name, call, error)) return false;
        if (call.result.kind != 1) {
            error = "Source faery query returned no Boolean result for selected Player";
            return false;
        }
        value = call.result.boolean;
        error.clear();
        return true;
    }
};
}

bool bind_native_owners_v1(const NativeOwnerInputV1& input, Bindings& output,
                           std::string& error) {
    auto state = std::make_shared<NativeState>();
    state->input = input;
    if (!input.record || !input.record->actor || !input.record->actor->object) {
        error = "Faery page requires a retained canonical Player Character record";
        return false;
    }
    state->character = input.record->actor->object->identity;
    if (!state->validate(error)) return false;

    Bindings result;
    result.owner = input.selected_character_owner;
    result.character = state->character;
    result.save = input.record->save.get();
    result.tables = input.faery_tables;
    result.validate_same_owner = [state](const dh2::data::PlayerSavegameV1& save,
                                         std::uintptr_t character, std::string& e) {
        if (!state->validate(e)) return false;
        if (&save != state->input.record->save.get() || character != state->character) {
            e = "Faery page received a foreign Save or Character identity";
            return false;
        }
        e.clear();
        return true;
    };
    result.current_difficulty = [state](std::int32_t& value, std::string& e) {
        if (!state->validate(e)) return false;
        return state->input.query_graph->difficulty(value, e);
    };
    result.unlocked = [state](std::uintptr_t character, std::uint32_t slot,
                              bool& value, std::string& e) {
        if (character != state->character || slot >= 5) {
            e = "Faery unlock query requires the same Character and source slot 0..4";
            return false;
        }
        return state->query_bool("NativeHUDGetIsFaeryUnlocked", slot, value, e);
    };
    result.text = [state](const std::string& field, std::int32_t slot,
                          std::string& value, std::string& e) {
        if (!state->validate(e)) return false;
        std::int32_t difficulty{};
        if (!state->input.query_graph->difficulty(difficulty, e)) return false;
        if (difficulty < 0 || difficulty >= 3) {
            e = "Source faery difficulty outside Save storage";
            return false;
        }
        std::int32_t saved_level = 0;
        if (field != "menu_FaerySheet/menu_title/txt_title") {
            if (slot < 0 || slot >= 5) {
                e = "Faery text field requires the same selected Save slot";
                return false;
            }
            saved_level = state->input.record->save->faery_level(
                static_cast<std::uint32_t>(slot), static_cast<std::uint32_t>(difficulty));
        }
        std::string symbol;
        if (!source_text_symbol_v1(field, slot, saved_level, symbol, e)) return false;
        dh2::ui::LocalizationResult localized;
        if (!state->input.query_graph->text->native_string(
                symbol, state->input.query_graph->text_environment.localization,
                localized, e)) return false;
        // HudTextV1 preserves the original StringManager miss result as the
        // literal "notfound". Keep that source result rather than adding a
        // page-specific fallback or fabricated translation.
        value = std::move(localized.text);
        e.clear();
        return true;
    };
    result.select = [state](std::uint32_t slot, std::string& e) {
        if (slot >= 5 || !state->validate(e)) {
            if (e.empty()) e = "Faery selection requires a source slot 0..4";
            return false;
        }
        dh2::ui::CharacterMenuCallV1 call;
        call.arguments = {dh2::ui::CharacterMenuValueV1::numeric(slot),
                          dh2::ui::CharacterMenuValueV1::numeric(0)};
        return state->input.queries->dispatch("NativeHUDSetActiveFaery", call, e);
    };

    Frame probe;
    if (!present(result, probe, error)) return false;
    output = std::move(result);
    error.clear();
    return true;
}

bool bind_source_page_provider_v1(Bindings bindings,
                                  dh::foundation::character_menu::SourcePageProviderV1& output,
                                  std::string& error) {
    if (!bindings.owner || !bindings.save || !bindings.character || !bindings.tables) {
        error = "Faery source page registration requires the same selected-character owner";
        return false;
    }
    auto ready = [bindings](std::string& e) {
        Frame discarded;
        return present(bindings, discarded, e);
    };
    auto append = content_callback(bindings);
    auto release = [bindings](float x, float y, std::string& e) {
        const auto slot = slot_at(x, y);
        if (slot < 0) {
            e = "Pointer is outside all five source Faery hit contours";
            return false;
        }
        Frame discarded;
        return activate_slot(bindings, static_cast<unsigned>(slot), discarded, e);
    };
    output = {bindings.owner, std::move(ready),
              [append = std::move(append)](dh::foundation::character_menu::Frame& frame,
                                             std::string& e) mutable {
                  return append(dh::foundation::character_menu::Tab::faery, frame, e);
              }, std::move(release)};
    error.clear();
    return true;
}

} // namespace dh::foundation::faery_menu
