#pragma once

#include "character_quest_page_v1.hpp"
#include "../character_menu/character_menu.hpp"

namespace dh::foundation {

// Rendering remains in the retained original SWF. This typed frame is the
// feature-owned data/action boundary for that authored movie.
struct RuntimeQuestMenuArtV1 {
    const char* movie{};
    const char* page{};
    const char* sha256{};
    std::uint32_t sprite{};
    std::array<float, 6> placement{};
    std::uint32_t stage_width_twips{}, stage_height_twips{};
    std::uint32_t row_step_swf_pixels{};
    const char* assigned_clip{};
    const char* completed_clip{};
    const char* row_template_clip{};
    const char* row_title_field{};
    const char* current_marker_clip{};
    const char* selected_row_frame{};
    const char* unselected_row_frame{};
    const char* objective_detail_field{};
    const char* secondary_detail_field{};
    const char* activate_clip{};
    const char* activate_hit_target{};
};

// The source Completed clip follows the Assigned list: its COMPLETED header and its rows move down one authored
// row step for each Assigned row beyond the first (reference video: four Assigned rows put the header near
// y 195 of 320). Returns the authored Y shift for the given number of Assigned rows.
float completed_list_shift_v1(std::size_t assigned_rows);

enum class RuntimeQuestMenuAvailabilityV1 { unknown, ready };
enum class RuntimeQuestMenuHitV1 { quest_row_release, activate_release };

struct RuntimeQuestMenuRowV1 {
    CharacterQuestIdV1 id;
    std::optional<std::string> title;
    bool is_current{};
    std::uint32_t authored_row_index{};
    std::uint32_t y_offset_swf_pixels{};
};

struct RuntimeQuestMenuFrameV1 {
    RuntimeQuestMenuArtV1 art;
    RuntimeQuestMenuAvailabilityV1 availability{RuntimeQuestMenuAvailabilityV1::unknown};
    CharacterQuestProgressV1::Origin progress_origin{CharacterQuestProgressV1::Origin::unknown};
    CharacterQuestCategoryV1 category{CharacterQuestCategoryV1::assigned};
    std::uint32_t collection{};
    std::int32_t difficulty{};
    std::vector<RuntimeQuestMenuRowV1> rows;            // Assigned list (AllQuests/content/Assigned)
    std::vector<RuntimeQuestMenuRowV1> completed_rows;  // Completed list (AllQuests/content/Completed); no activate
    std::optional<CharacterQuestPageSelectionV1> selection;
};

// Binds the source-authored Quest SWF presentation to one CharacterState and
// the same CharacterQuestProgressV1 owner. It never creates a second quest
// collection or substitutes page art/text. Hit actions correspond to the
// SWF's row onRelease and btn_Activate handlers.
class RuntimeQuestMenuV1 {
    CharacterState* character_{};
    CharacterQuestProgressV1* progress_{};
    std::shared_ptr<const dh2::data::QuestTablesPersistenceV51> tables_;
    CharacterQuestLogPolicyV1 policy_{};
    SourceCharacterQuestPageV1 page_;
    RuntimeQuestMenuFrameV1 frame_;
    bool current_page_valid_{};
public:
    static constexpr RuntimeQuestMenuArtV1 source_art{
        "dqcharmenu_droid.swf", "menu_QuestLogSheetNEW",
        "43227075f407626b52eca4c345ac6f568023fcb39c269588201026c0fc6760e0",
        599, {1.f,0.f,0.f,1.f,170.f,2126.f}, 9600,6400,50,
        "AllQuests/content/Assigned", "AllQuests/content/Completed",
        "AllQuests/content/Assigned/btnQuests", "btnQuests/TextBox/QuestName/text",
        "btnQuests/LightOn", "Selected", "Unselected",
        "QuestDesc/Description/text", "QuestDetailsText/text",
        "btn_Activate", "btn_Activate/hitzone"};

    RuntimeQuestMenuV1(CharacterState&, CharacterQuestProgressV1&,
        std::shared_ptr<const dh2::data::QuestTablesPersistenceV51>,
        CharacterQuestLogPolicyV1, CharacterQuestTextV1 = {});

    // Unknown saved progress is represented explicitly as unavailable. It is
    // never rendered as an empty/fresh quest list.
    bool refresh(std::uint32_t collection, std::int32_t difficulty,
        CharacterQuestCategoryV1, std::string& error);
    bool route_hit(RuntimeQuestMenuHitV1, const CharacterQuestIdV1&,
        std::string& error);
    bool load_progress_from_character(std::string& error);
    bool persist_progress_to_character(std::string& error);
    bool initialize_fresh_progress(std::string& error);
    bool record_accepted_source_state(const CharacterQuestIdV1&,
        std::int32_t source_state, std::string& error);
    const RuntimeQuestMenuFrameV1& frame() const noexcept { return frame_; }
    const CharacterState* character_owner() const noexcept { return character_; }
};

using RuntimeQuestMenuSymbolTextV1 = std::function<bool(
    const std::string& source_symbol, std::string& value, std::string& error)>;
using RuntimeQuestMenuActivePageV1 = std::function<bool()>;

// Installs the Quest page projection into CharacterMenu's existing
// authored-content callback. Root supplies the current source-page gate and
// resolves authored labels through its retained StringManager; this binding
// emits source sprite599 art, masks, text fields, and rows directly into Frame.
// Row/category/Activate actions use RuntimeQuestMenuV1 and the same progress
// owner.
class RuntimeQuestCharacterMenuBindingV1 :
    public std::enable_shared_from_this<RuntimeQuestCharacterMenuBindingV1> {
    std::shared_ptr<RuntimeQuestMenuV1> menu_;
    std::shared_ptr<CharacterState> character_;
    std::shared_ptr<void> source_owner_;
    RuntimeQuestMenuActivePageV1 active_page_;
    RuntimeQuestMenuSymbolTextV1 source_symbol_text_;
    bool content_installed_{};
    bool append_source_page(const character_menu::Bindings&,
        character_menu::Frame&, std::string&) const;
public:
    RuntimeQuestCharacterMenuBindingV1(
        std::shared_ptr<RuntimeQuestMenuV1>,
        std::shared_ptr<CharacterState>, std::shared_ptr<void> same_source_owner,
        RuntimeQuestMenuActivePageV1, RuntimeQuestMenuSymbolTextV1);

    bool show(std::uint32_t collection, std::int32_t difficulty,
        CharacterQuestCategoryV1, std::string& error);
    bool load_progress_from_character(std::string& error);
    bool initialize_fresh_progress(std::string& error);
    bool record_accepted_source_state(const CharacterQuestIdV1&,
        std::int32_t source_state, std::string& error);
    bool route_hit(RuntimeQuestMenuHitV1, const CharacterQuestIdV1&,
        std::string& error);
    const RuntimeQuestMenuFrameV1* current_frame() const noexcept;
    bool source_menu_page_ready(std::string& error) const;
    bool append_source_menu_page(character_menu::Frame&, std::string& error) const;
    bool retains_source_owner(const CharacterState*, const std::shared_ptr<void>&) const noexcept;
    bool install_content(character_menu::Bindings&, std::string& error);
};

} // namespace dh::foundation
