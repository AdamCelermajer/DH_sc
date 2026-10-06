#pragma once
#include "canonical_dummy_owner_v14.hpp"
#include "canonical_spawn_point_v15.hpp"
#include "canonical_decor_v15.hpp"
#include "canonical_destructible_container_v16.hpp"
#include "canonical_trigger_zone_v22.hpp"
#include "canonical_animated_decor_v23.hpp"
#include "canonical_checkpoint_zone_v26.hpp"
#include "canonical_door_v27.hpp"
#include "canonical_trigger_object_v28.hpp"
#include "canonical_exit_zone_v29.hpp"
#include "canonical_quest_move_zone_v31.hpp"
#include "canonical_sound_emitter_v32.hpp"
#include <canonical_trigger_trap_v37.hpp>

namespace dh2::loader {
// Runtime precedes its receiver: the source receiver borrows that same runtime
// for its entire lifetime. The aliasing factory lease retains both and XML.
template<class Receiver> struct CanonicalAuxiliaryRecordV16 {
 actor::RuntimeState runtime;
 std::unique_ptr<Receiver> owner;
 std::shared_ptr<const void> declaration;
};
using CanonicalDummyRecordV16=CanonicalAuxiliaryRecordV16<world::CanonicalDummyOwnerV14>;
using CanonicalSpawnPointRecordV16=CanonicalAuxiliaryRecordV16<world::CanonicalSpawnPointV15>;
using CanonicalDecorRecordV16=CanonicalAuxiliaryRecordV16<world::CanonicalDecorV15>;
using CanonicalDestructibleRecordV16=CanonicalAuxiliaryRecordV16<world::CanonicalDestructibleContainerV16>;
using CanonicalTriggerZoneRecordV22=CanonicalAuxiliaryRecordV16<world::CanonicalTriggerZoneV22>;
using CanonicalAnimatedDecorRecordV23=CanonicalAuxiliaryRecordV16<world::CanonicalAnimatedDecorV23>;
using CanonicalCheckpointRecordV26=CanonicalAuxiliaryRecordV16<world::CanonicalCheckpointZoneV26>;
using CanonicalDoorRecordV27=CanonicalAuxiliaryRecordV16<world::CanonicalDoorV27>;
using CanonicalTriggerObjectRecordV28=CanonicalAuxiliaryRecordV16<world::CanonicalTriggerObjectV28>;
using CanonicalExitZoneRecordV29=CanonicalAuxiliaryRecordV16<world::CanonicalExitZoneV29>;
using CanonicalQuestMoveRecordV31=CanonicalAuxiliaryRecordV16<world::CanonicalQuestMoveZoneV31>;
using CanonicalSoundEmitterRecordV32=CanonicalAuxiliaryRecordV16<world::CanonicalSoundEmitterV32>;
using CanonicalTriggerTrapRecordV37=CanonicalAuxiliaryRecordV16<world::CanonicalTriggerTrapV37>;
template<class Record,class Services> using CanonicalAuxiliaryProviderV16=std::function<bool(
 const world::CanonicalSourceObjectRequestV1&,const std::shared_ptr<Record>&,
 world::GameObjectInitializationServicesV1&,Services&,std::string&)>;
struct CanonicalAuxiliaryInputsV16 {
 std::shared_ptr<void> world;
 CanonicalAuxiliaryProviderV16<CanonicalDummyRecordV16,world::DummyContinuationServicesV14> dummy;
 CanonicalAuxiliaryProviderV16<CanonicalSpawnPointRecordV16,world::SpawnPointServicesV15> spawn_point;
 CanonicalAuxiliaryProviderV16<CanonicalDecorRecordV16,world::DecorServicesV15> decor;
 CanonicalAuxiliaryProviderV16<CanonicalDestructibleRecordV16,world::DestructibleContainerServicesV16> destructible;
 CanonicalAuxiliaryProviderV16<CanonicalTriggerZoneRecordV22,world::TriggerZoneServicesV22> trigger_zone;
 CanonicalAuxiliaryProviderV16<CanonicalAnimatedDecorRecordV23,world::AnimatedDecorServicesV23> animated_decor;
 CanonicalAuxiliaryProviderV16<CanonicalCheckpointRecordV26,world::CheckpointZoneServicesV26> checkpoint;
 CanonicalAuxiliaryProviderV16<CanonicalDoorRecordV27,world::DoorServicesV27> door;
 CanonicalAuxiliaryProviderV16<CanonicalTriggerObjectRecordV28,world::TriggerObjectServicesV28> trigger_object;
 CanonicalAuxiliaryProviderV16<CanonicalExitZoneRecordV29,world::ExitZoneServicesV29> exit_zone;
 CanonicalAuxiliaryProviderV16<CanonicalQuestMoveRecordV31,world::QuestMoveZoneServicesV31> quest_move;
 CanonicalAuxiliaryProviderV16<CanonicalSoundEmitterRecordV32,world::SoundEmitterServicesV32> sound;
 CanonicalAuxiliaryProviderV16<CanonicalTriggerTrapRecordV37,world::TriggerTrapServicesV37> trigger_trap;
};
class CanonicalAuxiliaryFamiliesV16 {
 CanonicalAuxiliaryInputsV16 inputs_;
 std::vector<std::shared_ptr<CanonicalDummyRecordV16>> dummies_;
 std::vector<std::shared_ptr<CanonicalSpawnPointRecordV16>> spawn_points_;
 std::vector<std::shared_ptr<CanonicalDecorRecordV16>> decor_;
 std::vector<std::shared_ptr<CanonicalDestructibleRecordV16>> destructibles_;
 std::vector<std::shared_ptr<CanonicalTriggerZoneRecordV22>> triggers_;
 std::vector<std::shared_ptr<CanonicalAnimatedDecorRecordV23>> animated_;
 std::vector<std::shared_ptr<CanonicalCheckpointRecordV26>> checkpoints_;
 std::vector<std::shared_ptr<CanonicalDoorRecordV27>> doors_;
 std::vector<std::shared_ptr<CanonicalTriggerObjectRecordV28>> trigger_objects_;
 std::vector<std::shared_ptr<CanonicalExitZoneRecordV29>> exits_;
 std::vector<std::shared_ptr<CanonicalQuestMoveRecordV31>> quest_moves_;
 std::vector<std::shared_ptr<CanonicalSoundEmitterRecordV32>> sounds_;
 std::vector<std::shared_ptr<CanonicalTriggerTrapRecordV37>> traps_;
public:
 explicit CanonicalAuxiliaryFamiliesV16(CanonicalAuxiliaryInputsV16);
 // Connect to the existing class dispatch's remaining callback. Unsupported
 // catalog entries fail explicitly; this never filters an authored declaration.
 bool construct(const world::CanonicalFactoryEntryV1&,const world::CanonicalSourceObjectRequestV1&,
                world::CanonicalClassReceiverV1&,std::string&);
 const auto& dummies()const noexcept{return dummies_;}
 const auto& spawn_points()const noexcept{return spawn_points_;}
 const auto& decor()const noexcept{return decor_;}
 const auto& destructibles()const noexcept{return destructibles_;}
 const auto& triggers()const noexcept{return triggers_;}
 const auto& animated_decor()const noexcept{return animated_;}
 const auto& checkpoints()const noexcept{return checkpoints_;}
 const auto& doors()const noexcept{return doors_;}
 const auto& trigger_objects()const noexcept{return trigger_objects_;}
 const auto& exit_zones()const noexcept{return exits_;}
 const auto& quest_move_zones()const noexcept{return quest_moves_;}
 const auto& sound_emitters()const noexcept{return sounds_;}
 const auto& trigger_traps()const noexcept{return traps_;}
};
}
