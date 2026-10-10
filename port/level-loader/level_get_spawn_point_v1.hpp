#pragma once
#include <canonical_spawn_point_v15.hpp>
#include <canonical_object_manager_v1.hpp>
namespace dh2::loader {
// Whole Level::GetSpawnPoint3ef614 over the same published ordered registry.
// R0 is an accumulator: rejected type13 receivers survive exhaustion and null
// map entries, while an unresolved handle or another type clears it.
template<class MakeHandle,class SpawnPoint,class Current>
bool source_get_spawn_point_v1(const std::shared_ptr<world::CanonicalObjectManagerV1>& objects,
 const std::int32_t& entrypoint,MakeHandle make_handle,SpawnPoint spawn_point,
 Current current,std::shared_ptr<world::CanonicalSpawnPointV15>& out,std::string& e){
 out.reset();
 if(!objects){e="Required SAME GetSpawnPoint ObjectManager";return false;}
 std::int32_t key{};const world::CanonicalObjectBorrowV1* actor{};
 bool found=objects->source_ordered_begin_v38(key,actor);
 while(found){
  if(actor){
   target_providers::Handle16 handle{};
   if(!make_handle||!make_handle(actor,handle,e)||!current(e))return false;
   const world::CanonicalObjectBorrowV1* resolved{};
   if(!objects->resolve_handle_v4(handle,false,resolved,{},e))return false;
   if(!resolved)out.reset();
   else{
    if(!resolved->type_f4){e="Required actual spawn lookup typeF4";return false;}
    if(*resolved->type_f4!=13)out.reset();
    else{
     std::shared_ptr<world::CanonicalSpawnPointV15> actual;
     if(!spawn_point||!spawn_point(*resolved,actual,e)||!current(e))return false;
     if(!actual||actual->base().identity()!=resolved->identity||
        actual.owner_before(resolved->lease)||resolved->lease.owner_before(actual)){
      e="Required SAME mapped SpawnPoint receiver/lease";return false;
     }
     const auto* visible=actual->base().byte(0x8a);
     if(!visible){e="Required actual SpawnPoint byte8a";return false;}
     out=std::move(actual); // Original3ef6c0 retains even a rejected SpawnPoint.
     if(*visible&&out->entrypoint()==entrypoint)return true;
    }
   }
  }
  found=objects->source_ordered_next_v38(key,key,actor);
 }
 return true;
}
}
