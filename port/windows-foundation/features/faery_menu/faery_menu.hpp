#pragma once
#include "../../hud_geometry.hpp"
#include "../character_menu/character_menu.hpp"
#include "../../../game-data/faery_tables.hpp"
#include "../../../game-data/player_savegame_v1.hpp"
#include <array>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace dh::foundation::faery_menu {
struct TextField {
    std::string path;std::uint32_t character_id{},font_id{};float source_height{};
    std::array<float,4> bounds{};std::array<std::uint8_t,4> rgba{};unsigned align{};
    std::array<float,6> matrix{};std::array<float,4> local_bounds{};
    std::array<float,3> margins{};float leading{};
};
struct SourceArt {
    std::vector<HudGeometryBatch> batches;
    std::vector<TextField> text_fields;
    std::array<std::vector<HudGeometryVertex>,5> slots; // authored FAERY_1..FAERY_5 contours
};
enum class ButtonVisual : unsigned { idle,focused,locked };
struct SolidArt { HudGeometryBatch geometry;std::array<float,4> rgba{};std::string after_bitmap_role; };
struct PresentedText {TextField field;std::string value;};
struct Frame {
    std::vector<HudGeometryBatch> art;
    std::vector<PresentedText> text;
    HudGeometry faery_visual;
    std::array<std::vector<HudGeometryBatch>,5> button_visuals;
    std::array<std::vector<SolidArt>,5> button_solid_visuals;
    std::array<std::int32_t,5> faery_ids{}; // Save/ChangeFaery slot ids 0..4
    std::array<std::int32_t,5> table_record_ids{}; // actual first FaeryList ids
    std::array<bool,5> unlocked{};
    std::array<std::int32_t,5> levels{};
    std::int32_t selected_id{-1},difficulty{-1};
};
struct Bindings {
    std::shared_ptr<void> owner;
    std::uintptr_t character{};
    const dh2::data::PlayerSavegameV1* save{};
    dh2::data::FaeryTables::Borrow tables;
    std::function<bool(const dh2::data::PlayerSavegameV1&,std::uintptr_t,std::string&)> validate_same_owner;
    std::function<bool(std::int32_t&,std::string&)> current_difficulty;
    // Must route through the actual CharacterMenu faery-unlock query. This
    // carries Debug UnlockAllFaeries and the saved row state semantics.
    std::function<bool(std::uintptr_t,std::uint32_t,bool&,std::string&)> unlocked;
    // Resolves the actual original AS field from the same query/text owners.
    // Paths are the exact SWF paths in TextField::path.
    std::function<bool(const std::string&,std::int32_t,std::string&,std::string&)> text;
    // Must call the existing CharacterMenuFaeryActions/source ChangeFaery path.
    std::function<bool(std::uint32_t,std::string&)> select;
};
const SourceArt& original_faery_art();
const std::vector<HudGeometryBatch>& original_faery_image(unsigned slot);
const std::vector<HudGeometryBatch>& original_faery_button(unsigned slot,ButtonVisual);
const std::vector<SolidArt>& original_faery_button_solids(unsigned slot,ButtonVisual);
std::size_t original_faery_buttons_insert_at() noexcept;
std::size_t original_faery_image_insert_at() noexcept;
// Input uses the authored 480x320 menu coordinates. Returns -1 outside the
// five original button hit contours, otherwise the source Save slot 0..4.
int slot_at(float authored_x,float authored_y) noexcept;
const std::vector<HudGeometryVertex>& original_faery_tab_hit();
bool present(const Bindings&,Frame&,std::string& error);
bool activate_slot(const Bindings&,unsigned slot,Frame& refreshed,std::string& error);
std::function<bool(dh::foundation::character_menu::Tab,
    dh::foundation::character_menu::Frame&,std::string&)> content_callback(Bindings);
}
