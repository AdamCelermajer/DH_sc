#pragma once
#include "equipment_adapter.hpp"
#include "../character_menu/character_menu.hpp"
namespace dh::foundation::equipment_menu {
struct SlotArt {
    unsigned source_slot=0;std::string button_path,category;
    character_menu::MenuArt art;
    std::vector<HudGeometryVertex> hit_triangles;
};
const std::vector<SlotArt>& original_slot_art();
struct OwnedSelection {
    std::string instance_id,definition_id,icon_name;
    std::int32_t name_text_oid=-1,type=-1,slotting=-1;
    bool equipped=false,requirements_met=false,applicable_to_selected_slot=false;
    std::array<std::int32_t,5> required{},actual{};
};
struct Options {
    std::vector<std::string> slots{"slot0","slot1","slot2","slot3","slot4","slot5","slot6","slot7","slot8"};
    bool online_requirements_bypass=false;
    // Source player CharacterTable row (e.g. KnightPlayerBase) for the IsEquippableBy class gate; empty disables it.
    std::string player_class_id;
    // Native ItemName provider may apply generated power names/grammar. Bare
    // original instances can use source numeric text OID17 through menu corpus.
    std::function<bool(const InventoryItem&,const dh2::data::Item&,std::string&,std::string&)> item_name;
    std::function<bool(std::string&,std::string&)> empty_name;
};
// Owns only selected source slot/instance. Reads authoritative source sheets;
// frame() and view() never recalculate class, gear or vitals.
class Presenter {
    const CharacterState& owner_;const dh2::data::ItemTable& items_;
    const dh2::data::PropertyState& properties_;EquipmentAdapter& mutations_;
    Options options_;std::string selected_;unsigned slot_=10;
public:
    Presenter(const CharacterState&,const dh2::data::ItemTable&,
              const dh2::data::PropertyState&,EquipmentAdapter&,Options = {});
    bool select_instance(const std::string&,std::string&);
    bool select_slot(unsigned,std::string&);
    unsigned hit_test(float authored_x,float authored_y)const noexcept;
    unsigned selected_slot()const noexcept{return slot_;}
    const std::string& selected_instance()const noexcept{return selected_;}
    bool view(std::vector<OwnedSelection>&,std::string&)const;
    bool view_for_selected_slot(std::vector<OwnedSelection>&,std::string&)const;
    bool frame(character_menu::Frame&,std::string&)const;
    bool equip_selected(std::string&);
    bool unequip_selected_slot(std::string&);
};
}
