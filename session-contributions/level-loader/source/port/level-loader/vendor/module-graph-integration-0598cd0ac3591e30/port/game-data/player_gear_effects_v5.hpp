#pragma once
#include "fresh_inventory_owned_v4.hpp"
#include "item_power_tables_v5.hpp"
namespace dh2::data {
enum class GearSkinOperationV5:std::uint32_t {category_id=0x470e5c,module_id=0x474568,debug_load=0x337888,debug_query=0x337a88,set_modular=0x470e18,set_weapon=0x473cd8};
struct GearSkinRequestV5 {GearSkinOperationV5 operation;std::uintptr_t visual;const char* name;std::int32_t category,module,slot,mode;};
struct GearSkinServicesV5 {void* context{};bool(*invoke)(void*,FreshInventoryOwnedV4&,const GearSkinRequestV5&,std::int32_t& result,std::string& error){};};
// One live inventory/property graph. These are source effects, not detached
// equipment mirrors. PropertyView and class rows must outlive this adapter;
// View must project the SAME V4 shared PropertyState, including live buffs.
class PlayerGearEffectsV5 {
 FreshInventoryOwnedV4* inventory_;PropertyView* properties_;const ClassRow* classes_;std::uint32_t class_count_;ItemPowerTablesV5::Borrow powers_;
 bool valid(std::string&)const;
public:
 PlayerGearEffectsV5(FreshInventoryOwnedV4&,PropertyView&,const ClassRow*,std::uint32_t,ItemPowerTablesV5::Borrow);
 bool update_properties(std::string&);
 bool validate_hp_mp(std::string&);
 // visual is a live source VisualObject field projection. Re-read before
 // every original visual call. Null genuinely skips Skin. Required factories
 // and Debug services are delivered; no invented successful clone/effect.
 bool update_skin(const std::uintptr_t* visual,const GearSkinServicesV5&,std::string&);
 const FreshInventoryOwnedV4& inventory()const noexcept{return *inventory_;}
};
const char* gear_modular_category_v5(std::uint32_t slot) noexcept;
}
