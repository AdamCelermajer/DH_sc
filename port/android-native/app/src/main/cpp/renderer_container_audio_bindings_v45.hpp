#pragma once
#include "port/engine-audio/integration-v44/root_audio_loader_v44.hpp"
#include "canonical_openable_container_v1.hpp"
#include "canonical_destructible_container_v16.hpp"
namespace dh2::android_audio {
// Loader passes the SAME table used to construct its actual canonical receiver.
// Receiver is weak to avoid service->receiver cycles; table remains leased.
inline std::function<bool(std::string&)> bind_openable_raw_precache_v45(
 const std::shared_ptr<dh2::world::CanonicalOpenableContainerV1>&actual,
 std::shared_ptr<const dh2::world::OpenableContainerTableV1>table){
 return bind_root_container_precache_v44([weak=std::weak_ptr<dh2::world::CanonicalOpenableContainerV1>(actual),table=std::move(table)](int&uid,std::string&error){
  auto object=weak.lock();if(!object||!table){error="Required SAME canonical Openable receiver/Arrays table";return false;}
  dh2::world::OpenableContainerRowV1 row;std::int32_t id=-1;
  if(!table->resolve(object->fields().data_desc,id,row,error))return false;
  uid=row.sound;error.clear();return true;
 });
}
inline std::function<bool(std::string&)> bind_destructible_raw_precache_v45(
 const std::shared_ptr<dh2::world::CanonicalDestructibleContainerV16>&actual,
 std::shared_ptr<const dh2::world::DestructibleContainerTableV16>table){
 return bind_root_container_precache_v44([weak=std::weak_ptr<dh2::world::CanonicalDestructibleContainerV16>(actual),table=std::move(table)](int&uid,std::string&error){
  auto object=weak.lock();if(!object||!table){error="Required SAME canonical Container receiver/Arrays table";return false;}
  std::int32_t id=-1;if(!object->get_data_id(id,error))return false;
  const auto*row=table->row(id);uid=row?row->sound():-1;error.clear();return true;
 });
}
// Build services before C1, populate the weak slot with its actual result
// before InitPost. No actor allocation or replacement occurs in this binder.
inline std::function<bool(std::string&)> bind_openable_raw_precache_slot_v45(
 std::shared_ptr<std::weak_ptr<dh2::world::CanonicalOpenableContainerV1>>slot,
 std::shared_ptr<const dh2::world::OpenableContainerTableV1>table){
 return bind_root_container_precache_v44([slot=std::move(slot),table=std::move(table)](int&uid,std::string&error){
  auto object=slot?slot->lock():nullptr;if(!object||!table){error="Required completed SAME Openable C1/table slot";return false;}
  dh2::world::OpenableContainerRowV1 row;std::int32_t id=-1;
  if(!table->resolve(object->fields().data_desc,id,row,error))return false;
  uid=row.sound;error.clear();return true;
 });
}
inline std::function<bool(std::string&)> bind_destructible_raw_precache_slot_v45(
 std::shared_ptr<std::weak_ptr<dh2::world::CanonicalDestructibleContainerV16>>slot,
 std::shared_ptr<const dh2::world::DestructibleContainerTableV16>table){
 return bind_root_container_precache_v44([slot=std::move(slot),table=std::move(table)](int&uid,std::string&error){
  auto object=slot?slot->lock():nullptr;if(!object||!table){error="Required completed SAME Container C1/table slot";return false;}
  std::int32_t id=-1;if(!object->get_data_id(id,error))return false;
  const auto*row=table->row(id);uid=row?row->sound():-1;error.clear();return true;
 });
}
}
