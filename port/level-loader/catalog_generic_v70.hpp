#pragma once
#include "production_noncharacter_catalog_v67.hpp"
#include <canonical_auxiliary_families_v16.hpp>
#include <canonical_room_zone_factory_v3.hpp>
#include <game_object_set_position_v2.hpp>
namespace dh2::loader {
struct GenericDeliveryV70 {
 std::shared_ptr<void> actual_world;
 std::shared_ptr<CanonicalLevelContextV1> level;
 std::shared_ptr<world::CanonicalObjectManagerV1> objects;
 world::CanonicalPropertyMapV1* properties{};
};
struct GenericReleaseRecordV70 {
 std::shared_ptr<void> record;
 std::shared_ptr<const void> source_lease;
 const char* class_name{};
 std::function<std::uintptr_t()> identity;
 // Called only at authentic native destruction, before unpublication/erasure.
 // C1-incomplete prefixes (identity0) stay explicit journal entries.
 std::function<bool(std::string&)> destroy_source;
};
struct CatalogGenericServicesV70 {
 // Independent authority; no callback or Arrays lease may capture the
 // containing World, Level or manager strongly. Delivery pins are transient.
 std::shared_ptr<void> owner;
 std::shared_ptr<const std::vector<std::string>> difficulty_names,sound_names;
 std::function<bool(const GenericDeliveryV70&,std::string&)> validate_current;
 std::function<bool(const GenericDeliveryV70&,world::CanonicalGameObjectBaseOwnerV1&,
  world::GameObjectInitializationServicesV1&,std::string&)> lend_initialization;
 std::function<bool(const GenericDeliveryV70&,world::CanonicalGameObjectBaseOwnerV1&,
  world::GameObjectSetPositionServicesV2&,std::string&)> lend_position;
 // Existing V37 methods operate on this already-produced actual TriggerTrap.
 // This callback lends method services, never a whole constructor/receiver.
 std::function<bool(const GenericDeliveryV70&,world::CanonicalTriggerTrapV37&,
  world::TriggerTrapServicesV37&,std::string&)> lend_trigger_trap;
 // V3 owns RoomZone C1, InitPost, InitBounds and inherited InitFinal. Its
 // genuine Zone/base D0 continuation is still an explicit engine binding.
 std::function<bool(const GenericDeliveryV70&,world::CanonicalRoomZoneV3&,std::string&)> destroy_room_zone;
 std::function<bool(const GenericDeliveryV70&,const std::shared_ptr<GenericReleaseRecordV70>&,
  const world::CanonicalSourceObjectRequestV1&,std::string&)> retain_release_record;
 // Confirm real native D0, manager/Room unpublication and exhausted aliases
 // BEFORE dropping the unique C1 or erasing the existing factory record.
 std::function<bool(const GenericDeliveryV70&,std::uintptr_t,std::string&)> release_completed;
};
struct GenericStateV70;
class CatalogGenericV70 final {
 std::shared_ptr<GenericStateV70> state_;
 std::unique_ptr<world::CanonicalRoomZoneFactoryV3> rooms_;
 std::unique_ptr<CanonicalAuxiliaryFamiliesV16> traps_;
 CatalogGenericV70()=default;
public:
 static bool create(ScopeV67,std::shared_ptr<const CatalogGenericServicesV70>,
  std::shared_ptr<CatalogGenericV70>&,PartV67&,std::string&);
 bool construct(const world::CanonicalFactoryEntryV1&,const world::CanonicalSourceObjectRequestV1&,
  world::CanonicalClassReceiverV1&,std::string&);
 bool erase_after_source_release(std::uintptr_t,std::string&);
 std::shared_ptr<world::CanonicalRoomZoneRecordV3> source_room_v105(std::uintptr_t id)const{return rooms_?rooms_->record_v91(id):nullptr;}
 const CanonicalAuxiliaryFamiliesV16* source_traps_v105()const noexcept{return traps_.get();}
};
bool make_generic_catalog_part_v70(ScopeV67,CatalogGenericServicesV70,PartV67&,std::string&);
}
