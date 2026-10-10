#pragma once
#include <canonical_gameobject_graph_v68.hpp>
#include <production_noncharacter_catalog_v67.hpp>
#include <cstring>
namespace model_renderer {
// Exact auxiliary factory classes already admitted by the original V67
// catalog. This is constructor transport; no level-content filtering.
inline bool auxiliary_base_class_v78(const char* name)noexcept{
 if(!name)return false;
 constexpr const char* classes[]{"Dummy","SpawnPoint","Decor","AnimatedDecor","TriggerZone",
  "CheckpointZone","Door","TriggerObject","TriggerZoneExitLevel",
  "QuestMoveInZone","SoundEmitter","DestructibleContainer"};
 for(const auto* candidate:classes)if(!std::strcmp(name,candidate))return true;
 return false;
}
template<class Self>void bind_campaign_base_admission_v78(std::weak_ptr<Self> weak,
 dh2::loader::ClassFactoryV67& factory){
 if(!factory)return;auto source=std::move(factory);
 factory=[weak,source=std::move(source)](const auto& entry,const auto& request,auto& receiver,std::string& e){
  if(!source(entry,request,receiver,e))return false; // retain genuine failed C1 prefix
  if(!auxiliary_base_class_v78(entry.name))return true;
  auto self=weak.lock();std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};
  if(!self||!self->graph||!self->borrow(receiver.object.identity,pin,base,e))return false;
  if(!receiver.object.lease||pin.owner_before(receiver.object.lease)||receiver.object.lease.owner_before(pin)){
   e="Required SAME constructed auxiliary receiver lease";return false;
  }
  const auto id=base->identity();
  dh2::world::CanonicalBaseBorrowV68 actual=[weak,id](auto& pin,auto*& base,std::string& e){
   auto self=weak.lock();return self&&self->borrow(id,pin,base,e);
  };
  return self->graph->observe_base_v78(id,std::move(actual),e);
 };
}
}
