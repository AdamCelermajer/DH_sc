#pragma once
#include "canonical_level_context_v1.hpp"
#include "../level-world/application_player_manager_bootstrap_v59.hpp"
#include "../level-world/canonical_object_manager_v1.hpp"
#include <climits>
namespace dh2::loader {
struct SpawnPointPlacementBorrowV68 {
 std::shared_ptr<void> receiver;std::uintptr_t identity{};
 std::function<bool(std::uint8_t&,std::string&)> source_enabled8a;
 std::function<bool(std::int32_t&,std::string&)> source_entrypoint374;
 std::function<bool(std::uintptr_t,std::string&)> place_object;
};
struct LevelPlayerPlacementServicesV68 {
 std::shared_ptr<void> provider;
 std::function<bool(std::uintptr_t,SpawnPointPlacementBorrowV68&,std::string&)> spawn_point;
 std::function<bool(std::uintptr_t,std::string&)> set_initial_position160;
 std::function<bool(std::uintptr_t,std::int32_t,std::int32_t,std::string&)> save_entrypoint;
};
// Whole Level._LoadPlayer3f0018: it places characters already produced by
// earlier PM.Update. It never calls AddCharacter or changes source count6c4.
inline bool source_place_level_players_v68(CanonicalLevelContextV1& level,
 player::ApplicationPlayerManagerBootstrapV59& pm,world::CanonicalObjectManagerV1& objects,
 const LevelPlayerPlacementServicesV68& services,std::string& error){
 auto* manager=pm.manager();const auto* count=pm.count_field();
 if(!manager||!count||!manager->source_frame_fields_v68()){
  error="Required completed SAME PM C1 for Level._LoadPlayer";return false;
 }
 std::int32_t index=0;
 while(index<*count){
  player::PlayerInfoFieldsV1* info{};
  if(!pm.get_local_player(index,true,info,error))return false;
  if(!info){error="Original local PlayerInfo NULL dereference rejected";return false;}
  const auto character=info->character660;
  if(character){
   std::int32_t key{};const world::CanonicalObjectBorrowV1* object{};
   bool more=objects.source_ordered_begin_v38(key,object),found=false;
   SpawnPointPlacementBorrowV68 selected;
   while(more){
    if(object&&object->identity){
     if(!object->shared_handle){error="Required actual ObjectBase.GetHandle";return false;}
     auto handle=*object->shared_handle;const world::CanonicalObjectBorrowV1* resolved{};
     if(!objects.resolve_handle_v4(handle,false,resolved,{},error))return false;
     if(resolved){
      if(!resolved->type_f4){error="Required actual resolved ObjectBase typef4";return false;}
      if(*resolved->type_f4==13){
       if(!services.provider||!services.spawn_point||!services.spawn_point(resolved->identity,selected,error)){
        if(error.empty())error="Required actual retained SpawnPoint13 placement receiver";return false;
       }
       if(!selected.receiver||selected.identity!=resolved->identity||!selected.source_enabled8a||!selected.source_entrypoint374){
        error="Foreign/incomplete actual SpawnPoint source fields";return false;
       }
       std::uint8_t enabled{};if(!selected.source_enabled8a(enabled,error))return false;
       if(enabled){std::int32_t entry{};if(!selected.source_entrypoint374(entry,error))return false;
        if(entry==level.constructor_fields_v3().level110){found=true;break;}
       }
      }
     }
    }
    more=objects.source_ordered_next_v38(key,key,object);
   }
   if(found&&!manager->source_frame_fields_v68()->byte6d0){
    if(!selected.place_object||!selected.place_object(character,error))return false;
    if(!services.set_initial_position160||!services.set_initial_position160(character,error))return false;
    if(!services.save_entrypoint||!services.save_entrypoint(character,level.constructor_fields_v3().level110,-1,error))return false;
   }
  }
  // Original rereads6c4 after placement callbacks; no snapshot loop bound.
  if(index==INT32_MAX){error="Native player placement index overflow";return false;}
  ++index;
 }
 error.clear();return true;
}
}
