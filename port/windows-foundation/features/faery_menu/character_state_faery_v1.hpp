#pragma once

// P14 FAERY: host glue between the live CharacterState and the Faery character-menu
// page / script effects. Logic lives here so main.cpp only holds small hunks.

#include "character_state_page_v1.hpp"

#include <cstdint>
#include <functional>
#include <memory>
#include <string>

namespace dh::foundation::faery_menu {

// T4: the ONE named owner of the difficulty used for Faery reads/writes (page, HUD key-4,
// script effects, cast arm). Preview 14 starts Normal only (creation admits difficulty 0),
// and CharacterState has no current-difficulty field yet. When the schema stream adds one,
// change this accessor and nothing else.
constexpr std::int32_t active_faery_difficulty_v1() noexcept { return 0; }

// Script_SetFaeryState (campaign command 27): slot @8, state @12. Writes only the state byte
// of one row. The first-unlock tutorial cinematic (slot != 0, difficulty 0) is NOT routed here.
bool apply_source_set_faery_state_v1(CharacterState& character, std::int32_t difficulty,
                                     std::uint32_t slot, std::uint32_t state, std::string& error);

// Script_IncFaeryLevel (campaign command 28): slot @8. Level += 1 on one row (WORD).
// The faery_charged trophy and UpdateAllSkills are NOT routed here (quests/skills streams).
bool apply_source_inc_faery_level_v1(CharacterState& character, std::int32_t difficulty,
                                     std::uint32_t slot, std::string& error);

// ChangeFaery equivalent: writes current_faery of one difficulty only. Callers must have
// passed the unlock gate (the page provider does).
bool commit_source_faery_selection_v1(CharacterState& character, std::int32_t difficulty,
                                      std::uint32_t slot, std::string& error);

// Creation-equivalent Faery rows: every difficulty starts at current 0 with all five rows zero.
// runtime_creation_persistence_v1 uses this for new characters; legacy slots reuse it below.
void initialize_source_faery_rows_v1(CharacterState& character);

// Legacy slots (source_faery_state_known == false) have no Faery rows. Load/first use gives them
// the creation zeros so the authored Swamp_Intro SetFaeryState can unlock Celest. Known rows are
// never touched. Returns true when the rows were initialized.
bool ensure_source_faery_rows_v1(CharacterState& character);

// Spells implemented so far: Celest (slot 0) and Hotty (slot 1). Rocky, Wetty and Windy are shown
// and selectable on the page, but key 4 has no cast path for them yet.
constexpr bool faery_slot_has_spell_v1(std::int32_t slot) noexcept { return slot == 0 || slot == 1; }
std::string faery_no_spell_message_v1(std::int32_t slot);

struct CharacterStateFaeryPageHostV1 {
    std::shared_ptr<CharacterState> owner;            // the selected character (same owner as the composition)
    dh2::data::FaeryTables::Borrow tables;              // actual source FaeryTables
    std::function<bool(const std::string&, std::string&, std::string&)> localize; // symbol, value, error
    std::function<bool(std::string&)> persist;          // saves the committed selection; failure rolls back
    std::function<void()> refresh_hud;                  // re-composes the key-4 circle after a change
};

// Action owner for an accepted page click: commits current_faery, persists through host.persist
// (rolled back on failure) and refreshes the HUD key-4 circle through host.refresh_hud.
bool commit_character_state_faery_selection_v1(const CharacterStateFaeryPageHostV1& host,
                                               std::int32_t difficulty, std::uint32_t slot,
                                               std::string& error);

// Registers Tab::faery on the character-menu composition with the CharacterState provider.
bool register_character_state_faery_page_v1(dh::foundation::character_menu::SourceCompositionV1& composition,
                                            CharacterStateFaeryPageHostV1 host,
                                            std::string& error);

} // namespace dh::foundation::faery_menu
