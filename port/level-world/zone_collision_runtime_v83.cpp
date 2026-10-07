#include "zone_collision_runtime_v83.hpp"
#include <cmath>
#include <cstring>
#include <exception>
#include <utility>
namespace dh2::world {namespace {
template<class F,class... A>bool call(const F& f,const char* name,std::string& e,A&&... a){
 if(!f){e=std::string("Required actual Zone ")+name;return false;}return f(std::forward<A>(a)...,e);
}
bool asserted(const ZoneCollisionServicesV83& s,const char* file,int line,const char* expression,std::string& e){return call(s.assertion,"assertion owner",e,file,line,expression);}
bool point_equal(const float* a,const float* b){
 // Point3D<float>::operator==312b6c uses abs(delta)<float bits38d1b717.
 std::uint32_t bits=0x38d1b717;float epsilon;std::memcpy(&epsilon,&bits,4);
 return std::fabs(a[0]-b[0])<epsilon&&std::fabs(a[1]-b[1])<epsilon&&std::fabs(a[2]-b[2])<epsilon;
}
}
bool zone_collision_notification_v83(CanonicalGameObjectBaseOwnerV1&,std::uintptr_t,std::string& e){e.clear();return true;}
bool zone_is_inside_v83(CanonicalGameObjectBaseOwnerV1& base,std::uintptr_t colzone,
 std::uintptr_t id,const ZoneCollisionServicesV83& s,bool& inside,std::string& e){
 if(!id){if(!asserted(s,"..\\..\\sources\\Game\\Objects\\Zone.cpp",0x98,"obj != 0",e))return false;e="Original Zone.IsInside NULL peer dereference";return false;}
 ZonePeerBorrowV83 peer;if(!call(s.peer,"SAME peer2dc/position160",e,id,peer))return false;
 if(!peer.receiver||peer.identity!=id||!peer.physical2dc){e="Required SAME Zone peer physical2dc";return false;}
 if(!*peer.physical2dc){inside=false;e.clear();return true;}
 if(colzone)return call(s.mesh_inside,"actual colzone selector/ray branch397110",e,base,colzone,peer,inside);
 if(!peer.position160){e="Required Zone peer source position160";return false;}
 // Original computes normalized XY delta and GetRadius46e750 then does not
 // use that expanded vector in the six final comparisons397450..39755c.
 float radius{};if(!call(peer.physical_radius,"PhysicalObject.GetRadius46e750",e,radius))return false;
 (void)radius;const auto* bounds=base.absolute_aabb12c();const auto* p=peer.position160;
 inside=bounds[0]<=p[0]&&p[0]<=bounds[3]&&bounds[1]<=p[1]&&p[1]<=bounds[4]&&bounds[2]<=p[2]&&p[2]<=bounds[5];
 e.clear();return true;
}
bool zone_is_touching_v83(CanonicalGameObjectBaseOwnerV1& base,std::uintptr_t id,
 const ZoneCollisionServicesV83& s,bool& touching,std::string& e){
 ZonePeerBorrowV83 peer;if(!call(s.peer,"actual GameObject.IsTouching38b518 peer",e,id,peer))return false;
 if(!peer.receiver||peer.identity!=id||!peer.absolute12c){e="Required SAME IsTouching absolute12c";return false;}
 const auto* a=base.absolute_aabb12c();const auto* b=peer.absolute12c;
 // Actual PLT30e9ac=__aeabi_fcmple and30e4b4=__aeabi_fcmpge.
 // Preserve boundary equality and normal IEEE unordered comparison results.
 touching=a[0]<=b[3]&&a[3]>=b[0]&&a[1]<=b[4]&&a[4]>=b[1]&&a[2]<=b[5]&&a[5]>=b[2];e.clear();return true;
}
bool level_checkpoint_save_v83(const ZoneLevelBorrowV83& level,const float* position,bool respawn_at_checkpoint,
 const ZoneCollisionServicesV83& s,std::string& e){
 std::uintptr_t id{};if(!call(s.local_player,"CheckpointSave GetLocalPlayer(0,true)",e,0,true,id))return false;
 if(!level.receiver||!level.identity||!level.save_ec||!level.phase130){e="Required actual Level checkpoint receiver/ec/130";return false;}
 if(!*level.save_ec||static_cast<std::int32_t>(*level.phase130)<=33||!id){e.clear();return true;}
 ZonePlayerBorrowV83 player;if(!call(s.player,"SAME Character checkpoint receiver",e,id,player))return false;
 bool dead{};if(!player.receiver||player.identity!=id||!call(player.dead,"Character virtual34",e,dead))return false;
 if(dead){e.clear();return true;}
 if(!position||!player.checkpoint1468||!player.save_position1474||!player.position160){e="Required actual Character checkpoint1468/savePosition1474/160";return false;}
 // Source stores occur before SG_SaveCheckpoint failure and are not undone.
 for(unsigned i=0;i<3;++i)player.checkpoint1468[i]=position[i];
 const float* saved=respawn_at_checkpoint?position:player.position160;
 for(unsigned i=0;i<3;++i)player.save_position1474[i]=saved[i];
 if(!call(s.save_player_checkpoint,"Character.SG_SaveCheckpoint3bc494",e,id))return false;
 if(!level.seed114||!level.difficulty40||!level.row3c){e="Required SAME Level checkpoint114/40/3c";return false;}
 return call(s.save_level_checkpoint,"LevelSavegame.SaveCheckPoint463130",e,*level.save_ec,*level.seed114,*level.difficulty40,*level.row3c);
}
bool checkpoint_collision_begin_v83(CanonicalCheckpointZoneV26& zone,std::uintptr_t object,
 const ZoneCollisionServicesV83& s,std::string& e){
 if(!object){if(!asserted(s,"..\\..\\sources\\Game\\Objects\\CheckpointZone.cpp",0x26,"obj != 0",e))return false;e="Original Checkpoint NULL peer dereference";return false;}
 bool character{};if(!call(s.is_character,"Checkpoint virtual24",e,object,character))return false;
 if(!character){e.clear();return true;}
 std::uintptr_t id{};if(!call(s.handle_character,"GetHandle33dd2c/Character conversion33ff54",e,object,id))return false;
 std::uintptr_t local{};if(!call(s.local_player,"Checkpoint GetLocalPlayer(0,true)",e,0,true,local))return false;
 ZoneLevelBorrowV83 level;if(!call(s.current_level,"Application.GetCurrentLevel31f594",e,level))return false;
 if(!level.identity){if(!asserted(s,"..\\..\\sources\\Game\\Objects\\CheckpointZone.cpp",0x2e,"level != 0",e))return false;e="Original Checkpoint NULL Level dereference";return false;}
 bool player{};if(!call(s.is_player,"Checkpoint Character virtual28",e,id,player))return false;
 if(player&&id==local){ZonePlayerBorrowV83 actor;if(!call(s.player,"Checkpoint SAME local1468",e,id,actor))return false;
  const auto* p=zone.base().vector3(0x160);
  if(!actor.receiver||!actor.checkpoint1468||!p){e="Required actual Checkpoint/Character position fields";return false;}
  if(!point_equal(p,actor.checkpoint1468)&&!level_checkpoint_save_v83(level,p,false,s,e))return false;
 }
 // Source membership test and insertion follow the save branch. Repeated
 // entry can revisit save if the checkpoint differs; the set never clears on
 // collision end because Checkpoint inherits Zone's literal void end method.
 auto& seen=zone.source_checkpoint388();if(seen.find(id)==seen.end())seen.insert(id);
 e.clear();return true;
}
bool quest_move_collision_begin_v83(CanonicalQuestMoveZoneV31& zone,std::uintptr_t object,
 const ZoneCollisionServicesV83& s,std::string& e){
 if(!object){if(!asserted(s,"..\\..\\sources\\Game\\Objects\\QuestMoveInZone.cpp",0x26,"obj != 0",e))return false;e="Original QuestMove NULL peer dereference";return false;}
 bool character{};if(!call(s.is_character,"QuestMove virtual24",e,object,character))return false;
 if(!character){e.clear();return true;}
 ZoneLevelBorrowV83 level;if(!call(s.current_level,"QuestMove currentLevel",e,level))return false;
 if(!level.identity){e.clear();return true;}
 if(!level.receiver||!level.events||level.events->identity()!=level.identity){e="Required SAME Level inherited EventManager";return false;}
 // Exact QE_MoveInZone stack stores396198..3961c8. room64 and network−1
 // are retained alongside the native pointer-width actor/subject projection.
 struct MoveEvent {std::int32_t type{};std::uintptr_t actor{};std::int32_t room{};std::uint8_t pending{},from_network{};std::int32_t network{-1};std::uintptr_t zone{};} payload;
 if(!call(s.constant,"actual MoveInZone event constant",e,"v2QuestObjectiveType","MoveInZone",payload.type))return false;
 payload.actor=object;payload.room=zone.base().room64();payload.zone=zone.base().identity();
 loader::GameEventQuestBorrowV75 fields;fields.receiver=level.receiver;fields.character8=payload.actor;fields.object18=payload.zone;
 fields.pending_network10=&payload.pending;fields.from_network11=&payload.from_network;fields.quantity14=&payload.network;
 fields.character8_cell=&payload.actor;fields.object18_cell=&payload.zone;
 loader::ScopedGameQuestEventV75 envelope(reinterpret_cast<std::uintptr_t>(&payload),&payload.type,std::move(fields));
 return level.events->raise_async(envelope.borrow(),e);
}
bool exit_zone_update_v83(CanonicalExitZoneV29& zone,const ExitZoneRuntimeServicesV83& s,std::string& e){
 e.clear();
 auto current=[&](std::string& error){return !s.current_v114||s.current_v114(zone,error);};
 auto reached=[&](const auto& fn,const char* name,auto&&... args){
  if(!current(e))return false;if(!fn){e=std::string("Required actual Exit ")+name;return false;}
  std::string local;const bool done=fn(std::forward<decltype(args)>(args)...,local);
  if(!done){e=local.empty()?std::string("Required actual Exit ")+name:local;return false;}
  return current(e);
 };
 try{
  if(!current(e))return false;
  auto* idle=zone.base().integer(0x370);if(!idle){e="Required SAME Exit signed-short370 cell";return false;}
  const auto raw=static_cast<std::uint16_t>(*idle);std::int16_t ordinal;std::memcpy(&ordinal,&raw,2);
  if(ordinal>=0&&!reached(s.update_idle_sound_v114,"GameObject.UpdateIdleSound38ae2c",zone))return false;
  bool online{},remote{};if(!reached(s.online,"GetOnline.byte5",online))return false;
  if(online&&!reached(s.virtual54,"source virtual54",remote))return false;
  auto* touching=zone.source_integer(0x3c0);if(!touching){e="Required SAME Exit touching3c0";return false;}
  if(!online||!remote){
   std::int32_t count{};if(!zone.source_number_touching_v83(count,e)||!current(e))return false;
   *touching=count;
  }
  ZoneLevelBorrowV83 level;if(!reached(s.collision.current_level,"GetCurrentLevel",level))return false;
  if(level.identity){if(!level.receiver||!level.transitions144){e="Required SAME Exit Level byte144";return false;}
   if(!*level.transitions144){e.clear();return true;}}
  std::uintptr_t local{};if(!reached(s.collision.local_player,"GetLocalPlayer(0,true)",0,true,local))return false;
  auto* displayed=zone.source_byte(0x819);if(!displayed){e="Required SAME Exit displayed819";return false;}
  auto show=[&](bool visible){
   auto* name=zone.source_string(0x7dc);if(!name){e="Required actual Exit LevelName7dc";return false;}
   std::string title;if(!reached(s.level_display_name,"LevelList lookup/GetString row24",name->c_str(),title))return false;
   //Source rereads destination buffer and entry AFTER localization.
   name=zone.source_string(0x7dc);auto* entry=zone.source_integer(0x7f4);
   if(!name||!entry){e="Required actual reached Exit destination/entry fields";return false;}
   if(!reached(s.display_fasttravel,"HUD.DisplayFastTravel(active,title,LevelName,EntryPoint)",visible,title.c_str(),name->c_str(),*entry))return false;
   *displayed=visible?1:0;return true;
  };
  if(local){
   if(*touching){
    auto* unlocked=zone.source_byte(0x818);if(!unlocked){e="Required SAME Exit unlocked818";return false;}
    if(!*unlocked){
     auto* fasttravel=zone.source_string(0x800);if(!fasttravel){e="Required actual FastTravelName800";return false;}
     if(!reached(s.unlock_fasttravel,"Character.SG_SetFastTravelIdUnlocked3bba84",local,fasttravel->c_str(),true,-1))return false;
     *unlocked=1;
    }
   }
   bool hit{};if(!zone_is_touching_v83(zone.base(),local,s.collision,hit,e)||!current(e))return false;
   if(hit){bool hosting{};if(!reached(s.local_hosting,"PlayerManager.IsLocalPlayerHosting36f074",hosting))return false;
    if(hosting){
     //Qualified GameObject.MeetCondition38ab60 is literal TRUE.
     //No Trigger.CanActivate/Update, new evaluator or automatic transition.
     if(!reached(s.require_online_update,"GameObject.RequireOnlineUpdate38b8b8"))return false;
     auto* level_id=zone.source_integer(0x7d4);if(!level_id){e="Required SAME Exit LevelListID7d4";return false;}
     if(*level_id==-1||*displayed){e.clear();return true;}
     return show(true);
    }
   }
  }
  if(*displayed)return show(false);e.clear();return true;
 }catch(const std::exception& ex){e=ex.what();return false;}catch(...){e="Exit native leaf threw; reached prefix retained";return false;}
}

}
