#include "character_state_faery_v1.hpp"

namespace dh::foundation::faery_menu {

void initialize_source_faery_rows_v1(CharacterState& character) {
    for (auto& difficulty : character.faery_by_difficulty) {
        difficulty.current_faery = 0;
        for (auto& faery : difficulty.faeries) faery = {};
    }
    character.source_faery_state_known = true;
}

bool ensure_source_faery_rows_v1(CharacterState& character) {
    if (character.source_faery_state_known) return false;
    initialize_source_faery_rows_v1(character);
    return true;
}

std::string faery_no_spell_message_v1(std::int32_t slot) {
    return "no spell implemented for Faery slot " + std::to_string(slot) + " (key 4 does nothing)";
}
namespace {
bool row_address_ok(const CharacterState& character, std::int32_t difficulty,
                    std::uint32_t slot, std::string& error) {
    if (difficulty < 0 || difficulty >= 3 || slot >= 5) {
        error = "Source Faery row outside difficulty 0..2 / slot 0..4";
        return false;
    }
    if (!character.source_faery_state_known) {
        error = "CharacterState has no known source Faery rows";
        return false;
    }
    return true;
}
}

bool apply_source_set_faery_state_v1(CharacterState& character, std::int32_t difficulty,
                                     std::uint32_t slot, std::uint32_t state, std::string& error) {
    if (!row_address_ok(character, difficulty, slot, error)) return false;
    if (state > 255) {
        error = "Source SetFaeryState byte outside 0..255";
        return false;
    }
    character.faery_by_difficulty[std::size_t(difficulty)].faeries[slot].state =
        static_cast<std::uint8_t>(state);
    error.clear();
    return true;
}

bool apply_source_inc_faery_level_v1(CharacterState& character, std::int32_t difficulty,
                                     std::uint32_t slot, std::string& error) {
    if (!row_address_ok(character, difficulty, slot, error)) return false;
    auto& row = character.faery_by_difficulty[std::size_t(difficulty)].faeries[slot];
    if (row.level == 0xFFFFu) {
        error = "Source IncFaeryLevel would overflow the WORD level";
        return false;
    }
    ++row.level;
    error.clear();
    return true;
}

bool commit_source_faery_selection_v1(CharacterState& character, std::int32_t difficulty,
                                      std::uint32_t slot, std::string& error) {
    if (!row_address_ok(character, difficulty, slot, error)) return false;
    character.faery_by_difficulty[std::size_t(difficulty)].current_faery =
        static_cast<std::int32_t>(slot);
    error.clear();
    return true;
}

bool commit_character_state_faery_selection_v1(const CharacterStateFaeryPageHostV1& host,
                                               std::int32_t difficulty, std::uint32_t slot,
                                               std::string& error) {
    if (!host.owner) {
        error = "Faery selection requires the owner CharacterState";
        return false;
    }
    auto& character = *host.owner;
    const auto previous = character.faery_by_difficulty[std::size_t(difficulty)].current_faery;
    if (!commit_source_faery_selection_v1(character, difficulty, slot, error)) return false;
    if (host.persist && !host.persist(error)) {
        character.faery_by_difficulty[std::size_t(difficulty)].current_faery = previous;
        if (error.empty()) error = "Faery selection save failed; selection rolled back";
        return false;
    }
    if (host.refresh_hud) host.refresh_hud();
    error.clear();
    return true;
}

bool register_character_state_faery_page_v1(dh::foundation::character_menu::SourceCompositionV1& composition,
                                            CharacterStateFaeryPageHostV1 host,
                                            std::string& error) {
    if (!host.owner || !host.tables || !host.localize) {
        error = "Faery page host requires the owner, actual FaeryTables and localization";
        return false;
    }
    CharacterStateFaeryBindingsV1 bindings;
    bindings.owner = host.owner;
    bindings.character = host.owner.get();
    bindings.tables = host.tables;
    bindings.difficulty = active_faery_difficulty_v1();
    const CharacterState* owner = host.owner.get();
    bindings.validate_same_character = [owner](const CharacterState* actual, std::string& e) {
        if (actual != owner) {
            e = "Faery page CharacterState is not the selected owner's state";
            return false;
        }
        e.clear();
        return true;
    };
    // The port has no Debug UnlockAllFaeries switch; the source default is off.
    bindings.debug_unlock_all_faeries = [](bool& unlocked, std::string&) {
        unlocked = false;
        return true;
    };
    bindings.localize = host.localize;
    const auto difficulty = bindings.difficulty;
    bindings.activate_slot = [host, difficulty](std::uint32_t slot, std::string& e) {
        return commit_character_state_faery_selection_v1(host, difficulty, slot, e);
    };
    dh::foundation::character_menu::SourcePageProviderV1 provider;
    if (!bind_character_state_faery_provider_v1(bindings, provider, error)) return false;
    return composition.register_page(dh::foundation::character_menu::Tab::faery,
                                     std::move(provider), error);
}

} // namespace dh::foundation::faery_menu
