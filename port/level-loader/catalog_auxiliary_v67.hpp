#pragma once
#include "production_noncharacter_catalog_v67.hpp"
#include "noncharacter_save_connection_v89.hpp"
#include "canonical_auxiliary_families_v16.hpp"
#include <game_object_set_position_v2.hpp>
#include <map>
#include <vector>
#include <type_traits>
namespace dh2::loader {
struct AuxiliaryDeliveryV67 {
 std::shared_ptr<void> actual_world;
 std::shared_ptr<CanonicalLevelContextV1> level;
 std::shared_ptr<world::CanonicalObjectManagerV1> objects;
 world::CanonicalPropertyMapV1* properties{};
};
template<class Record>using AuxiliaryReceiverV67=typename decltype(std::declval<Record>().owner)::element_type;
// Lends existing genuine engine leaves for THIS already constructed receiver.
// It never allocates, returns or replaces a canonical class/factory receiver.
// Returned callbacks live only for the actual leaf delivery, not on the actor.
template<class Record,class Services>using AuxiliaryServicesLenderV67=std::function<bool(
 const AuxiliaryDeliveryV67&,AuxiliaryReceiverV67<Record>&,Services&,std::string&)>;
// Actual resource prefix retained by the existing admitted class journal.
// backing precedes receiver so native destruction cannot outlive its world.
struct AuxiliaryResourcePrefixV82 {
 std::shared_ptr<void> backing,receiver;
 std::uintptr_t identity{};const std::uintptr_t* source_owner8{};bool assigned{},released{};
 std::function<bool(std::string&)> release_source;
};
struct AuxiliaryReleaseRecordV67 {
 std::vector<std::shared_ptr<AuxiliaryResourcePrefixV82>> resources_v82; // BEFORE native construct/assign
 std::shared_ptr<void> record; // SAME runtime/declaration/unique canonical C1
 const char* class_name{};
 std::function<bool()> has_constructed_owner;
 std::function<CanonicalConstructorStateV89()> constructor_state;
 std::function<std::uintptr_t()> identity; //0 before that real C1 completes
 std::function<bool(std::string&)> destroy_source;
 std::function<bool(std::string&)> retire_storage_after_source_release_v91;
 std::function<bool(level::LevelSaveObjectBorrowV2&,std::string&)> borrow_save_header;
 // Typed SAME-record OBJS projection; no caller type-erasure cast or lookup.
 std::function<bool(NonCharacterRestoreServicesV89,std::shared_ptr<NonCharacterSaveConnectionV89>&,std::string&)> make_save_connection;
};
struct CatalogAuxiliaryServicesV67 {
 // Independent process/source authority and immutable real Arrays snapshots.
 // No callback, array lease or returned service may retain the containing World,
 // Level or manager. Scope is weak; delivery pins exist only during the call.
 std::shared_ptr<void> owner;
 std::shared_ptr<const std::vector<std::string>> difficulty_names,sound_names;
 std::function<bool(const AuxiliaryDeliveryV67&,std::string&)> validate_current;
 std::function<bool(const AuxiliaryDeliveryV67&,world::CanonicalGameObjectBaseOwnerV1&,
  world::GameObjectInitializationServicesV1&,std::string&)> lend_initialization;
 std::function<bool(const AuxiliaryDeliveryV67&,world::CanonicalGameObjectBaseOwnerV1&,
  world::GameObjectSetPositionServicesV2&,std::string&)> lend_position;
 AuxiliaryServicesLenderV67<CanonicalDummyRecordV16,world::DummyContinuationServicesV14> dummy;
 AuxiliaryServicesLenderV67<CanonicalSpawnPointRecordV16,world::SpawnPointServicesV15> spawn_point;
 AuxiliaryServicesLenderV67<CanonicalSpawnSpotRecordV108,world::SpawnSpotServicesV108> spawn_spot;
 AuxiliaryServicesLenderV67<CanonicalDecorRecordV16,world::DecorServicesV15> decor;
 AuxiliaryServicesLenderV67<CanonicalTriggerZoneRecordV22,world::TriggerZoneServicesV22> trigger_zone;
 AuxiliaryServicesLenderV67<CanonicalAnimatedDecorRecordV23,world::AnimatedDecorServicesV23> animated_decor;
 AuxiliaryServicesLenderV67<CanonicalCheckpointRecordV26,world::CheckpointZoneServicesV26> checkpoint;
 AuxiliaryServicesLenderV67<CanonicalDoorRecordV27,world::DoorServicesV27> door;
 AuxiliaryServicesLenderV67<CanonicalTriggerObjectRecordV28,world::TriggerObjectServicesV28> trigger_object;
 AuxiliaryServicesLenderV67<CanonicalExitZoneRecordV29,world::ExitZoneServicesV29> exit_zone;
 AuxiliaryServicesLenderV67<CanonicalQuestMoveRecordV31,world::QuestMoveZoneServicesV31> quest_move;
 AuxiliaryServicesLenderV67<CanonicalSoundEmitterRecordV32,world::SoundEmitterServicesV32> sound;
 // Admit before C1, retain every reached prefix, then run destroy_source only
 // at the real source destruction point. Absence of C1 (identity0) is a separate
 // journal prefix; it is not a fabricated successful D0.
 std::function<bool(const AuxiliaryDeliveryV67&,const std::shared_ptr<AuxiliaryReleaseRecordV67>&,
  const world::CanonicalSourceObjectRequestV1&,std::string&)> retain_release_record;
 // Confirms real native destruction, manager unpublication, receiver transport
 // erase and exhausted native/draw aliases. Only then may unique C1 storage drop.
 std::function<bool(const AuxiliaryDeliveryV67&,std::uintptr_t,std::string&)> release_completed;
};
struct AuxiliaryStateV67;
class CatalogAuxiliaryV67 final {
 std::shared_ptr<AuxiliaryStateV67> state_;
 std::unique_ptr<CanonicalAuxiliaryFamiliesV16> families_;
 bool busy_{},failed_{};
 CatalogAuxiliaryV67()=default;
public:
 static bool create(ScopeV67,std::shared_ptr<const CatalogAuxiliaryServicesV67>,
  std::shared_ptr<CatalogAuxiliaryV67>&,PartV67&,std::string&);
 bool construct(const world::CanonicalFactoryEntryV1&,const world::CanonicalSourceObjectRequestV1&,
  world::CanonicalClassReceiverV1&,std::string&);
 // Scoped alias of the already constructed/admitted SpawnPoint; no XML
 // rescan, copied source fields or second receiver. Placement uses this SAME C1.
 bool borrow_spawn_point(std::uintptr_t,std::shared_ptr<world::CanonicalSpawnPointV15>&,std::string&);
 const CanonicalAuxiliaryFamiliesV16* source_families_v105()const noexcept{return families_.get();}
 bool erase_after_source_release(std::uintptr_t,std::string&);
};
bool make_auxiliary_catalog_part_v67(ScopeV67,CatalogAuxiliaryServicesV67,PartV67&,std::string&);
}
