#pragma once
#include <canonical_decor_v15.hpp>
#include <canonical_gameobject_graph_v68.hpp>
#include <module_pf_room_v3.hpp>
namespace model_renderer {
// Services only: no new visual, room registry, floor world or base receiver.
// Self is the existing V69 campaign bundle, with weak current-record methods.
template<class Self>void bind_retained_decor_floor_v77(std::weak_ptr<Self> weak,
 dh2::world::DecorServicesV15& services){
 if(!services.load_room)services.load_room=[weak](std::uintptr_t root,std::int32_t source_room,
  const std::string& name,std::uintptr_t& identity,std::uint32_t*& flags,std::string& e){
  identity=0;flags=nullptr;auto self=weak.lock();
  if(!self||!self->current(e)||!root||!self->graph){if(e.empty())e="Required SAME current Decor root/graph";return false;}
  auto rooms=self->rooms.lock();auto floors=self->floors.lock();auto map=self->map.lock();
  if(!rooms||!floors||!map||rooms->world().get()!=floors.get()||rooms->map_owner_v69().get()!=map.get()||
   rooms->world().owner_before(floors)||floors.owner_before(rooms->world())||
   rooms->map_owner_v69().owner_before(map)||map.owner_before(rooms->map_owner_v69())){
   e="Required SAME current PF rooms/floor world/map";return false;
  }
  std::vector<std::shared_ptr<dh2::world::RetainedGameObjectVisualV1>> visuals;
  if(!self->graph->capture_visuals(visuals,e))return false;
  std::shared_ptr<dh2::world::RetainedGameObjectVisualV1> visual;
  for(const auto& candidate:visuals)if(candidate&&candidate->root_identity()==root){
   if(visual&&visual.get()!=candidate.get()){e="Ambiguous actual Decor visual root";return false;}visual=candidate;
  }
  if(!visual||!visual->ready()){e="Required actual admitted Decor VisualObject/root";return false;}
  std::shared_ptr<dh2::world::ModulePFRoomV3> room;
  // UINT_MAX from signed room64=-1 is the original constructor input.
  if(!rooms->load(*visual,static_cast<std::uint32_t>(source_room),name,room,e))return false;
  if(!self->current(e))return false;
  // Genuine nullable result only after the real source search/room algorithm.
  if(room){identity=reinterpret_cast<std::uintptr_t>(room.get());flags=&room->flags24;}
  e.clear();return true;
 };
 if(!services.extend_bounds)services.extend_bounds=[weak](std::uintptr_t identity,const float* bounds,std::string& e){
  auto self=weak.lock();if(!self||!self->current(e)||!identity||!bounds){if(e.empty())e="Required SAME Decor room/bounds";return false;}
  auto rooms=self->rooms.lock();if(!rooms){e="Released actual PF room owner";return false;}
  for(const auto& room:rooms->rooms())if(room&&reinterpret_cast<std::uintptr_t>(room.get())==identity)
   return rooms->extend_source_bounds_v77(room,bounds,e)&&self->current(e);
  e="Required SAME published PFRoom from Decor LoadRoom";return false;
 };
}
}
