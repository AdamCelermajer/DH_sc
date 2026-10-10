#pragma once
#include "inventory_menu.hpp"
#include "../equipment/equipment_menu.hpp"
#include <array>
namespace dh::foundation::inventory {
// slot: a rail icon (SideList/btn_TypeN) selected InvSlotId N. previous/next: the rail arrows btn_left/btn_right, which
// step InvSlotId one slot with wrap (ClassChangeUp/ClassChangeDown), not the list row.
enum class DetailAction { none,select,previous,next,equip,unequip,drop,transmute,auto_equip,slot };
struct DetailHit {DetailAction action{};std::string path;std::vector<HudGeometryVertex> triangles;};
struct DetailRowArt {int relative_index{};character_menu::MenuArt unselected,selected;std::vector<HudGeometryVertex> hit;};
struct DetailTextVariant {std::vector<HudGeometryBatch> batches;std::vector<character_menu::MenuTextField> fields;std::vector<character_menu::MenuSolidBatch> solids;};
struct DetailTextStates {DetailTextVariant transmute_idle,transmute_disabled;};
struct DetailArt {character_menu::MenuArt panel;std::vector<DetailRowArt> rows;std::vector<DetailHit> actions;DetailTextStates text_states;};
const DetailArt& original_inventory_details();
// B045: true when (x,y) is on the visible body of a list row (the row art box), not only its border sliver.
bool details_row_hit(const DetailRowArt& row,float x,float y);
// Details slot rail (menu_InventorySheetDetails/SideList/btn_TypeN, N = InvSlotId 0..9, top to bottom). Each icon's hit
// box is the bounding box of its authored SideList batches. Returns the icon index at (x,y), or -1.
struct DetailRailBox{float x0,y0,x1,y1;};
bool details_rail_box(const DetailArt& art,unsigned slot,DetailRailBox& output);
int details_rail_slot_at(const DetailArt& art,float x,float y);
// The native character preview is an SWF display callback, not a panel-wide
// overlay. These generated records retain its exact authored pane and sibling
// insertion point so the renderer can interleave the existing preview owner.
// Matrix/bounds use the movie's 480x320 authored pixel coordinates.
struct SourceCharacterPaneV1 {
    std::string path;
    unsigned root_depth{};
    unsigned child_depth{};
    std::string after_role;
    std::string before_role;
    std::array<float,6> matrix{}; // local pane symbol to movie coordinates
    std::array<float,4> bounds{}; // left, top, right, bottom
};
const std::vector<SourceCharacterPaneV1>& original_inventory_character_panes_v1();
struct DetailBindings {
    std::function<bool(const InventoryItem&,const dh2::data::Item&,std::string&,std::string&)> item_name;
    std::function<bool(const std::string&,std::string&,std::string&)> symbol;
    // Actual generated ItemInstance descriptors; sourcefield is ItemInfo1..5,
    // ItemReq, EquipedItemInfo1..5 or EquipedItemReq. Missing means unavailable;
    // the panel keeps those fields empty instead of inferring powered stats.
    std::function<bool(const std::string&,const InventoryItem&,std::string&,std::string&)> item_details;
    // Source ItemTransmuteValueString. If unavailable, the source value field
    // stays empty; the label and button name are independently localized.
    std::function<bool(const InventoryItem&,std::string&,std::string&)> transmute_value;
};
// Borrows equipment's sole slot/instance selection; owns only panel visibility.
class DetailsPresenter {
    const CharacterState& owner_;const dh2::data::ItemTable& table_;
    equipment_menu::Presenter& selection_;bool open_=false;
    bool candidates(std::vector<equipment_menu::OwnedSelection>&,std::string&)const;
public:
    DetailsPresenter(const CharacterState& owner,const dh2::data::ItemTable& table,equipment_menu::Presenter& selection):owner_(owner),table_(table),selection_(selection){}
    void close() noexcept{open_=false;}
    bool is_open()const noexcept{return open_;}
    bool open(unsigned source_slot,std::string& error);
    std::size_t selected_index()const;
    bool reselect_near(std::size_t index,std::string& error);
    bool frame(const DetailBindings&,character_menu::Frame&,std::string& error)const;
    // Sourcecoords already inverse-transformed by PC/touch transport.
    // Selection and arrows act here; mutation commands returned to real owner.
    bool release(float source_x,float source_y,DetailAction&,std::string& error);
};
}
