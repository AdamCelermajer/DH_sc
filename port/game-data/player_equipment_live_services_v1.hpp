#pragma once
#include "player_gear_effects_v5.hpp"
#include <stdexcept>

namespace dh2::data {
// Character::INV_DoesMeetRequirements 0x3a4930. Cached fields are source
// fixed-point values; this predicate has no class-requirement/unlock grant.
struct EquipmentRequirements32V1 {
 std::uint32_t online{},remote{},present{};
 std::int32_t cached[5]{};
};
static_assert(sizeof(EquipmentRequirements32V1)==32);
int equipment_requirements_v1(std::int32_t*,const EquipmentRequirements32V1*,
                             const ItemRecord164*) noexcept;
enum class EquipmentWorldQueryV1:std::uint32_t {
 online=0x7fd794,online_player_record=0x36eea8,current_player=0x31f594,
 current_difficulty=1,player_count=0x4043a8,remotely_updated=0x33dd10
};
struct EquipmentWorldServicesV1 {
 void* context{};
 bool(*invoke)(void*,EquipmentWorldQueryV1,std::uintptr_t character,
               std::uintptr_t& identity,std::int32_t& scalar,std::string&){};
};
// V4's void storage observation cannot return a required-service rejection.
// A failed binding/retirement delivery therefore unwinds BEFORE V4 deletes
// the actual Item. Direct V4 callers using services() must catch this type;
// adapter mutation methods translate it to false/error, retaining the prefix.
class EquipmentLifecycleFailureV1:public std::runtime_error {
public:
 explicit EquipmentLifecycleFailureV1(const std::string& error):std::runtime_error(error){}
};
// The actual native owner supplies and keeps these live. validate_binding must
// verify renderer/Scene/resource attachment and callback-context lifetimes.
// A genuine source-null VisualObject is allowed; a detached renderer is not.
// required.observe_storage MUST retire ItemPresentationV5 entries (and other
// item projections) synchronously before V4's destroy_item erases the Item.
// It retires projections of the actual source Item, never a replacement store
// or policy decision. It must throw if required retirement cannot complete;
// the adapter converts that delivery failure to EquipmentLifecycleFailureV1.
struct EquipmentLiveHooksV1 {
 void* context{};
 bool(*validate_binding)(void*,FreshInventoryOwnedV4&,PropertyView&,std::string&){};
 EquipmentWorldServicesV1 world{};
 const std::uintptr_t* visual{};
 GearSkinServicesV5 skin{};
 OwnedInventoryServicesV4 required{};
};
// Borrows the ONE native inventory and exact live PropertyView (including buff
// groups). Owns no inventory/save/property/RNG/visual/VM/timer/frame state.
// The class/power tables and hooks must outlive this adapter. Rebind/detach is
// the caller's lifetime operation; hooks are borrowed, never snapshot-copied.
class PlayerEquipmentLiveServicesV1 {
 FreshInventoryOwnedV4* inventory_;
 PropertyView* properties_;
 EquipmentLiveHooksV1* hooks_;
 PlayerGearEffectsV5 gear_;
 bool running_{};
 unsigned callback_depth_{};
 bool graph(std::string&)const;
 bool binding(std::string&);
 bool begin(std::string&);
 bool query(EquipmentWorldQueryV1,std::uintptr_t&,std::int32_t&,std::string&);
 bool skin(std::string&);
 bool prune(std::string&,unsigned=0);
 bool refresh_impl(bool,std::string&);
 static bool effect(void*,FreshInventoryOwnedV4&,const OwnedInventoryRequestV4&,
                    OwnedInventoryResponseV4&,std::string&);
 static void observe(void*,FreshInventoryOwnedV4&,const OwnedInventoryRequestV4&);
public:
 PlayerEquipmentLiveServicesV1(FreshInventoryOwnedV4&,PropertyView&,
  const ClassRow*,std::uint32_t,ItemPowerTablesV5::Borrow,EquipmentLiveHooksV1&);
 PlayerEquipmentLiveServicesV1(const PlayerEquipmentLiveServicesV1&)=delete;
 PlayerEquipmentLiveServicesV1& operator=(const PlayerEquipmentLiveServicesV1&)=delete;
 // Caller may use this descriptor on the SAME V4 owner. Every direct delivery
 // validates the binding. Catch EquipmentLifecycleFailureV1 around direct V4
 // mutations: its void storage observation may reject before deletion with a
 // retained quantity/slot/insert prefix and caller-owned incoming Item.
 // Text/debug/current
 // world/fullness/notifications/add-power continuations must be real providers
 // when reached. V4 retains all original partial prefixes and mutation guards.
 OwnedInventoryServicesV4 services() noexcept;
 bool meets_requirements(const ItemInstanceV1*,bool&,std::string&);
 // Character::CheckItems requirement-pruning phase only. This deliberately
 // does not substitute the broader properties/skin/vitals refresh path.
 bool check_item_requirements(std::string&);
 // Properties -> optional recursive requirement pruning -> Skin -> HP/MP.
 // Failure keeps the reached source prefix; no rollback or inferred producer.
 bool refresh(bool check_requirements,std::string&);
 // Character::Skin after a menu transmute. Runs the same bound V5 visual
 // provider and source Item identities without refreshing gear/vitals again.
 bool skin_only(std::string&);
 bool equip(std::uint32_t slot,std::uint32_t index,std::string&);
 bool unequip(std::uint32_t slot,std::string&);
 bool swap(std::string&);
 bool auto_equip(std::uint32_t index,std::int32_t& result,std::string&);
 // DisplayRightHud/FillActionIcon, actual draw/resource lifecycle and native
 // inventory serialization/binding remain mandatory caller integrations.
 FreshInventoryOwnedV4& inventory()const noexcept{return *inventory_;}
 PropertyView& property_view()const noexcept{return *properties_;}
};
}
