#pragma once

#include "faery_menu.hpp"
#include "../character_menu/source_composition.hpp"
#include "../../character_state.hpp"

namespace dh::foundation::faery_menu {

enum class SourceFaeryKnowledgeV1 : std::uint8_t { unknown, locked, known };

inline SourceFaeryKnowledgeV1 source_faery_knowledge_v1(bool source_state_known,
                                                        std::uint8_t source_state,
                                                        bool debug_fact_known = false,
                                                        bool debug_unlock_all = false) noexcept {
    if (!source_state_known) return SourceFaeryKnowledgeV1::unknown;
    // NativeHUDGetIsFaeryUnlocked compares the saved source byte to exactly 1.
    if (source_state == 1 || (debug_fact_known && debug_unlock_all))
        return SourceFaeryKnowledgeV1::known;
    return debug_fact_known ? SourceFaeryKnowledgeV1::locked
                            : SourceFaeryKnowledgeV1::unknown;
}

struct CharacterStateFaerySlotV1 {
    std::int32_t menu_slot{-1};
    std::int32_t table_record_id{-1};
    std::string table_name;
    std::uint32_t description_symbol{};
    std::uint32_t elemental_id{};
    std::uint32_t model_file_id{};
    std::uint32_t name_symbol{};
    std::int32_t spell_type{-1};
    std::int32_t source_type{-1};
    std::string spell_script;
    SourceFaeryKnowledgeV1 knowledge{SourceFaeryKnowledgeV1::unknown};
    std::uint8_t raw_saved_state{};
    std::uint16_t saved_level{};
    ButtonVisual visual{ButtonVisual::locked};
};

struct CharacterStateFaeryPageV1 {
    std::int32_t list_id{-1};
    std::string list_name;
    std::int32_t difficulty{-1};
    std::int32_t selected_slot{-1};
    std::array<CharacterStateFaerySlotV1, 5> slots{};
    std::vector<HudGeometryBatch> art;
    std::array<std::vector<HudGeometryBatch>, 5> button_art;
    std::array<std::vector<SolidArt>, 5> button_solids;
    HudGeometry selected_faery_art;
    std::vector<PresentedText> text;
};

struct CharacterStateFaeryBindingsV1 {
    std::shared_ptr<void> owner;
    CharacterState* character{};
    dh2::data::FaeryTables::Borrow tables;
    std::int32_t difficulty{-1};
    std::function<bool(const CharacterState*, std::string&)> validate_same_character;
    // Original NativeHUDGetIsFaeryUnlocked applies Debug UnlockAllFaeries
    // before comparing the saved byte to 1. Without this genuine source fact,
    // raw non-1 rows remain Unknown rather than being mislabeled Locked.
    std::function<bool(bool&, std::string&)> debug_unlock_all_faeries;
    // Resolve the exact source symbol using the root's existing localization
    // owner. The provider never invents fallback labels or values.
    std::function<bool(const std::string&, std::string&, std::string&)> localize;
    // Production hosts provide the existing same-owner NativeHUDSetActiveFaery /
    // CharacterMenuFaeryActions callback. When absent, the page helper only
    // commits the validated single CharacterState current-slot cell (fixture use).
    std::function<bool(std::uint32_t, std::string&)> activate_slot;
};

bool present_character_state_faery_v1(const CharacterStateFaeryBindingsV1&,
                                      CharacterStateFaeryPageV1&,
                                      std::string& error);
bool select_character_state_faery_v1(CharacterStateFaeryBindingsV1&,
                                      std::uint32_t menu_slot,
                                      std::string& error);
bool bind_character_state_faery_provider_v1(
    CharacterStateFaeryBindingsV1,
    dh::foundation::character_menu::SourcePageProviderV1&,
    std::string& error);

} // namespace dh::foundation::faery_menu
