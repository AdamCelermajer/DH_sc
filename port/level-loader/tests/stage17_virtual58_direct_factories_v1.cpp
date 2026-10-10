#include "canonical_receiver_transport_v1.hpp"
#include "canonical_colbox_v71.hpp"
#include "canonical_floor_v72.hpp"
#include <canonical_destructible_container_v16.hpp>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>
using namespace dh2::world;
namespace {
unsigned checks{};
void check(bool value){++checks;if(!value){std::fprintf(stderr,"FAIL %u\n",checks);std::exit(1);}}
void verify_retained_gameobject_final(dh2::loader::CanonicalReceiverTransportV1& transport,
 const CanonicalClassReceiverV1& produced,unsigned& calls,std::string& error){
 check(transport.admit_constructed_source_v91(produced,error));
 const CanonicalClassReceiverV1* retained{};check(transport.receiver(produced.object,retained,error)&&retained);
 bool game_object{};check(retained->is_game_object&&retained->is_game_object(game_object,error)&&game_object);
 check(static_cast<bool>(retained->source_init_final_v95));check(retained->source_init_final_v95(error));check(calls==1);
 check(static_cast<bool>(retained->source_is_updatable_v95));bool updatable=true;check(retained->source_is_updatable_v95(updatable,error)&&!updatable);
 check(static_cast<bool>(retained->source_loading_fields_v95));CanonicalObjectLoadingFieldsV95 fields;check(retained->source_loading_fields_v95(fields,error)&&fields.gameobject_base);
}
}
int main(){
 std::string error;CanonicalPropertyMapV1 properties({});dh2::loader::CanonicalReceiverTransportV1 transport(properties,{});
 {
  dh2::actor::RuntimeState runtime{};auto owner=std::make_shared<int>(1);unsigned calls{};
  GameObjectInitializationServicesV1 init;init.owner=owner;init.check_spawn_probability=[&](std::int32_t& roll,std::string&){++calls;roll=100;return true;};
  FloorServicesV72 services;services.owner=owner;
  auto actual=std::make_shared<CanonicalFloorV72>(owner,runtime,init,services);
  auto receiver=CanonicalFloorV72::factory_receiver(actual,owner);verify_retained_gameobject_final(transport,receiver,calls,error);
 }
 {
  dh2::actor::RuntimeState runtime{};auto owner=std::make_shared<int>(2);unsigned calls{};
  GameObjectInitializationServicesV1 init;init.owner=owner;init.check_spawn_probability=[&](std::int32_t& roll,std::string&){++calls;roll=100;return true;};
  ColBoxServicesV71 services;services.owner=owner;
  auto actual=std::make_shared<CanonicalColBoxV71>(owner,runtime,init,services);
  auto receiver=CanonicalColBoxV71::factory_receiver(actual,owner);verify_retained_gameobject_final(transport,receiver,calls,error);
 }
 {
  dh2::actor::RuntimeState runtime{};auto owner=std::make_shared<int>(3);unsigned calls{};
  DestructibleContainerServicesV16 services;services.common.spawn_roll_and_probability=[&](std::int32_t& roll,std::int32_t& probability,std::string&){++calls;roll=100;probability=100;return true;};
  auto actual=std::make_shared<CanonicalDestructibleContainerV16>(owner,runtime,GameObjectInitializationServicesV1{},services);
  auto receiver=CanonicalDestructibleContainerV16::factory_receiver(actual,owner);verify_retained_gameobject_final(transport,receiver,calls,error);
 }
 std::printf("Stage17 Floor/ColBox/DestructibleContainer retained factory→transport→actual InitFinal PASS checks=%u\n",checks);
}
