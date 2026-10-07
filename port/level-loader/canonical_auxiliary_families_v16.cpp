#include "canonical_auxiliary_families_v16.hpp"
#include "canonical_loading_receiver_v95.hpp"
#include <cstring>
#include <utility>
#include <type_traits>

namespace dh2::loader {
namespace {
bool same_lease(const std::shared_ptr<void>& a,const std::shared_ptr<void>& b){
 return a&&b&&!a.owner_before(b)&&!b.owner_before(a);
}
template<class Receiver,class Services>
bool construct_family(const char* name,const std::shared_ptr<void>& world,
 const CanonicalAuxiliaryProviderV16<CanonicalAuxiliaryRecordV16<Receiver>,Services>& provider,
 const world::CanonicalSourceObjectRequestV1& source,
 std::vector<std::shared_ptr<CanonicalAuxiliaryRecordV16<Receiver>>>& retained,
 world::CanonicalClassReceiverV1& output,std::string& error){
 if(!world||!source.source_lease||!provider){error=std::string("Required actual loader family providers: ")+name;return false;}
 auto record=std::make_shared<CanonicalAuxiliaryRecordV16<Receiver>>();record->declaration=source.source_lease;
 world::GameObjectInitializationServicesV1 separate_initialization;
 Services services;services.owner=world;
 if constexpr(std::is_same_v<Receiver,world::CanonicalExitZoneV29>)services.trigger.owner=world;
 auto& initialization=[&]() -> world::GameObjectInitializationServicesV1& {
  if constexpr(std::is_same_v<Receiver,world::CanonicalTriggerZoneV22>)return services.initialization;
  else if constexpr(std::is_same_v<Receiver,world::CanonicalExitZoneV29>)return services.trigger.initialization;
  else return separate_initialization;
 }();initialization.owner=world;
 if(!provider(source,record,initialization,services,error))return false;
 if(!same_lease(initialization.owner,world)||!same_lease(services.owner,world)){
  error=std::string("Loader family replaced SAME World lease: ")+name;return false;
 }
 if constexpr(std::is_same_v<Receiver,world::CanonicalExitZoneV29>)if(!same_lease(services.trigger.owner,world)){error="Exit family replaced SAME inherited TriggerZone lease";return false;}
 if(record->owner){error=std::string("Loader family provider replayed source constructor: ")+name;return false;}
 record->constructor_state=CanonicalConstructorStateV89::constructing;
 try{
 if constexpr(std::is_same_v<Receiver,world::CanonicalTriggerZoneV22>||std::is_same_v<Receiver,world::CanonicalExitZoneV29>)record->owner=std::make_unique<Receiver>(world,record->runtime,std::move(services));
 else record->owner=std::make_unique<Receiver>(world,record->runtime,std::move(initialization),std::move(services));
 }catch(...){record->constructor_state=CanonicalConstructorStateV89::failed;throw;}
 record->constructor_state=CanonicalConstructorStateV89::completed;
 auto receiver=std::shared_ptr<Receiver>(record,record->owner.get());
 auto candidate=Receiver::factory_receiver(receiver,source.source_lease);
 constexpr bool updatable=!(std::is_same_v<Receiver,world::CanonicalDummyOwnerV14>||std::is_same_v<Receiver,world::CanonicalSpawnPointV15>||std::is_same_v<Receiver,world::CanonicalSpawnSpotV108>||std::is_same_v<Receiver,world::CanonicalDecorV15>||std::is_same_v<Receiver,world::CanonicalAnimatedDecorV23>||std::is_same_v<Receiver,world::CanonicalDestructibleContainerV16>);
 world::bind_gameobject_loading_v95(receiver,updatable,candidate);
 retained.push_back(std::move(record));output=std::move(candidate);return true;
}
}
CanonicalAuxiliaryFamiliesV16::CanonicalAuxiliaryFamiliesV16(CanonicalAuxiliaryInputsV16 inputs):inputs_(std::move(inputs)){}
bool CanonicalAuxiliaryFamiliesV16::construct(const world::CanonicalFactoryEntryV1& entry,
 const world::CanonicalSourceObjectRequestV1& source,world::CanonicalClassReceiverV1& output,std::string& error){
 if(!entry.name){error="Required actual catalog class name";return false;}
 if(!std::strcmp(entry.name,"TriggerTrap")&&entry.original_address==0x340e7c)
   return construct_family<world::CanonicalTriggerTrapV37>(entry.name,inputs_.world,inputs_.trigger_trap,source,traps_,output,error);
  if(!std::strcmp(entry.name,"SoundEmitter")&&entry.original_address==0x340ccc)
  return construct_family<world::CanonicalSoundEmitterV32>(entry.name,inputs_.world,inputs_.sound,source,sounds_,output,error);
 if(!std::strcmp(entry.name,"QuestMoveInZone")&&entry.original_address==0x340f50)
  return construct_family<world::CanonicalQuestMoveZoneV31>(entry.name,inputs_.world,inputs_.quest_move,source,quest_moves_,output,error);
 if(!std::strcmp(entry.name,"TriggerZoneExitLevel")&&entry.original_address==0x340ea0)
  return construct_family<world::CanonicalExitZoneV29>(entry.name,inputs_.world,inputs_.exit_zone,source,exits_,output,error);
 if(!std::strcmp(entry.name,"TriggerObject")&&entry.original_address==0x340f0c)
  return construct_family<world::CanonicalTriggerObjectV28>(entry.name,inputs_.world,inputs_.trigger_object,source,trigger_objects_,output,error);
 if(!std::strcmp(entry.name,"Door")&&entry.original_address==0x340824)
  return construct_family<world::CanonicalDoorV27>(entry.name,inputs_.world,inputs_.door,source,doors_,output,error);
 if(!std::strcmp(entry.name,"CheckpointZone")&&entry.original_address==0x340f2c)
  return construct_family<world::CanonicalCheckpointZoneV26>(entry.name,inputs_.world,inputs_.checkpoint,source,checkpoints_,output,error);
 if(!std::strcmp(entry.name,"AnimatedDecor")&&entry.original_address==0x342600)
  return construct_family<world::CanonicalAnimatedDecorV23>(entry.name,inputs_.world,inputs_.animated_decor,source,animated_,output,error);
 if(!std::strcmp(entry.name,"TriggerZone")&&entry.original_address==0x340ec4)
  return construct_family<world::CanonicalTriggerZoneV22>(entry.name,inputs_.world,inputs_.trigger_zone,source,triggers_,output,error);
 if(!std::strcmp(entry.name,"Dummy")&&entry.original_address==0x3410a4)
  return construct_family<world::CanonicalDummyOwnerV14>(entry.name,inputs_.world,inputs_.dummy,source,dummies_,output,error);
 if(!std::strcmp(entry.name,"SpawnSpot")&&entry.original_address==0x340dec)
  return construct_family<world::CanonicalSpawnSpotV108>(entry.name,inputs_.world,inputs_.spawn_spot,source,spawn_spots_,output,error);
 if(!std::strcmp(entry.name,"SpawnPoint")&&entry.original_address==0x340e10)
  return construct_family<world::CanonicalSpawnPointV15>(entry.name,inputs_.world,inputs_.spawn_point,source,spawn_points_,output,error);
 if(!std::strcmp(entry.name,"Decor")&&entry.original_address==0x3410fc)
  return construct_family<world::CanonicalDecorV15>(entry.name,inputs_.world,inputs_.decor,source,decor_,output,error);
 if(!std::strcmp(entry.name,"DestructibleContainer")&&entry.original_address==0x340d5c)
  return construct_family<world::CanonicalDestructibleContainerV16>(entry.name,inputs_.world,inputs_.destructible,source,destructibles_,output,error);
 error=std::string("Required actual catalog constructor: ")+entry.name;return false;
}
}
