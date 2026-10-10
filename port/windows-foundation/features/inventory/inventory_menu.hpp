#pragma once
#include "inventory_feature.hpp"
#include "../character_menu/character_menu.hpp"
#include <functional>
namespace dh::foundation::inventory {
struct SlotArt {
    unsigned source_slot{};std::string path;
    std::vector<HudGeometryBatch> icons;
    std::vector<character_menu::MenuTextField> fields;
    std::vector<HudGeometryVertex> hit_contour;
    std::array<std::vector<HudGeometryBatch>,5> color_fills;
};
const std::vector<SlotArt>& original_inventory_slots();
struct MenuBindings {
    // Actual StringManager/name builder, never Item.name (model URI).
    std::function<bool(const Row&,std::string&,std::string&)> item_name;
    std::function<bool(const std::string&,std::string&,std::string&)> symbol;
    // Genuine ItemColor from actual ItemInstance; absent preserves source neutral fill.
    std::function<bool(const Row&,unsigned&,std::string&)> item_color;
    // Actual GAMEPLAYMENUS_POTIONS + recovered integer varargs formatter.
    std::function<bool(std::uint32_t,std::string&,std::string&)> potions;
    std::vector<std::string> equipment_slots{"slot0","slot1","slot2","slot3","slot4","slot5","slot6","slot7","slot8"};
};
class MenuPresenter {
    CharacterState& owner_;Presenter items_;int requested_slot_{-1};
    bool apply(const MenuBindings&,character_menu::Frame&,bool potions_only,std::string&);
public:
    MenuPresenter(CharacterState& owner,const dh2::data::ItemTable& table):owner_(owner),items_(owner,table){}
    // Uses authored480x320 coordinates; root's PC/touch viewport performs inverse transform.
    int hit_slot(float source_x,float source_y) const noexcept;
    bool release_slot(float source_x,float source_y,std::string& error);
    int requested_slot() const noexcept{return requested_slot_;}
    // Replaces wrong default torsoicons, binds actual equipped names/empty localized labels.
    // Does not own stats/skills or create original ItemInstance effects.
    bool content(const MenuBindings&,character_menu::Frame&,std::string& error);
    // Compose with equipment_menu::Presenter::frame (its sole selection owner).
    // Only updates original potion slot9; never overrides equipment slot0..8.
    bool potions_content(const MenuBindings&,character_menu::Frame&,std::string& error);
    Presenter& items() noexcept{return items_;}
};
}
