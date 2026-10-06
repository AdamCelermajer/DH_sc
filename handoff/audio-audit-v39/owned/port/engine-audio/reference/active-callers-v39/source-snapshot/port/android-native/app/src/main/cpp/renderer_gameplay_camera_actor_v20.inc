// Include after PlayerSkillsRuntime and PlayerCanonicalMetadataLiveV4.
// Requires gameplay_camera_load_v19.hpp, gameplay_camera_layout_v14.hpp and
// gameplay_camera_anchor_v6.hpp in the renderer header region.
// This is actor transport for the existing development Crypt Level. It does
// not claim completed canonical GSLevel/PlayerManager.AddCharacter startup.
struct RendererGameplayCameraActorServicesV20 {
 std::shared_ptr<WorldScriptContext> world;
 // Positive AnchorBase+c and destructor paths must use actual receivers.
 std::function<bool(std::uintptr_t,const float*&,std::string&)> anchor_position;
 std::function<bool(std::uintptr_t,std::string&)> delete_anchor;
 // Remaining real actor types are required, not classified by pointer value.
 std::function<bool(std::uintptr_t,dh2::camera::PointV2&,std::string&)> other_position;
 std::function<bool(std::uintptr_t,dh2::camera::PointV2&,std::string&)> other_anchor;
 std::function<bool(std::uintptr_t,bool&,std::string&)> other_disabled81;
 std::function<bool(std::uintptr_t,dh2::camera::CameraPartyActorV14&,std::string&)> other_party_actor;
 // Existing Level presence must be produced by the actual development load
 // owner. The generic canonical Level/GS startup is a separate integration.
 std::function<bool(bool&,std::string&)> development_level_present;
};
static bool gameplay_camera_player_runtime_v20(const std::shared_ptr<WorldScriptContext>& world,
 PlayerSkillsRuntime*& out,std::string& error){
 out=player_skills_runtime.get();
 if(!world||!out||out->world!=world||!out->player_manager||!world->player_object||
    !world->player_canonical||!world->player_canonical->bound||
    !world->player_canonical->position_fields_v7().constructed||
    world->player_canonical->handle.cached!=world->player_object->identity){
  error="Required same published player camera/metadata/PlayerManager graph";return false;
 }
 return true;
}
static bool gameplay_camera_anchor_borrow_v20(const RendererGameplayCameraActorServicesV20& s,
 dh2::camera::CameraAnchorBorrowV6& out,std::string& error){
 PlayerSkillsRuntime* runtime{};if(!gameplay_camera_player_runtime_v20(s.world,runtime,error))return false;
 auto metadata=s.world->player_canonical;
 out={metadata,s.world->player_object->identity,&metadata->position_fields_v7().attached2e0,
      prince_runtime.subobjects.position,s.anchor_position,s.delete_anchor};return true;
}
static bool bind_gameplay_camera_actors_v20(const RendererGameplayCameraActorServicesV20& s,
 dh2::camera::CameraLoadServicesV19& load,dh2::camera::CameraLayoutServicesV14& layout,
 std::string& error){
 PlayerSkillsRuntime* runtime{};if(!gameplay_camera_player_runtime_v20(s.world,runtime,error))return false;
 // Preserve real scene/root/dt/offset/view/skybox services supplied by root.
 if(!load.runtime.actor_services.target.center_offset||!load.runtime.actor_services.handle_centering||
    !s.development_level_present){error="Required genuine camera center-offset/centering/Level producer";return false;}
 auto position=[s](std::uintptr_t id,dh2::camera::PointV2& out,std::string& e){
  PlayerSkillsRuntime* t{};if(!gameplay_camera_player_runtime_v20(s.world,t,e))return false;
  if(id==t->world->player_object->identity){std::copy_n(prince_runtime.subobjects.position,3,out.begin());return true;}
  if(s.other_position)return s.other_position(id,out,e);e="Required actual remaining camera actor position";return false;
 };
 load.runtime.actor_services.actor_position=position;
 load.runtime.actor_services.target.actor_anchor=[s](std::uintptr_t id,dh2::camera::PointV2& out,std::string& e){
  dh2::camera::CameraAnchorBorrowV6 b;if(!gameplay_camera_anchor_borrow_v20(s,b,e))return false;
  if(id==b.identity)return dh2::camera::source_camera_anchor_v6(b,out,e);
  if(s.other_anchor)return s.other_anchor(id,out,e);e="Required actual remaining camera actor anchor";return false;
 };
 load.runtime.actor_services.actor_disabled81=[s](std::uintptr_t id,bool& out,std::string& e){
  PlayerSkillsRuntime* t{};if(!gameplay_camera_player_runtime_v20(s.world,t,e))return false;
  if(id==t->world->player_object->identity){out=s.world->player_canonical->disabled81!=0;return true;}
  if(s.other_disabled81)return s.other_disabled81(id,out,e);e="Required actual remaining camera actor disabled81";return false;
 };
 load.local_character0=[s](std::uintptr_t& out,std::string& e){
  PlayerSkillsRuntime* t{};if(!gameplay_camera_player_runtime_v20(s.world,t,e))return false;
  dh2::player::PlayerInfoFieldsV1* record{};
  if(!t->player_manager->manager().get_local_player(0,true,record,e)||!record){if(e.empty())e="Required actual local PlayerInfo0";return false;}
  out=record->character660;return true;
 };
 load.overview.local_player0=[s,position](bool& present,dh2::camera::PointV2& out,std::string& e){
  PlayerSkillsRuntime* t{};if(!gameplay_camera_player_runtime_v20(s.world,t,e))return false;
  dh2::player::PlayerInfoFieldsV1* record{};
  if(!t->player_manager->manager().get_local_player(0,true,record,e)||!record){if(e.empty())e="Required actual overview PlayerInfo0";return false;}
  present=record->character660!=0;return !present||position(record->character660,out,e);
 };
 layout.world=s.world;
 layout.source_online5=[s](std::uint8_t& out,std::string& e){
  PlayerSkillsRuntime* t{};if(!gameplay_camera_player_runtime_v20(s.world,t,e))return false;
  std::uintptr_t identity{};std::int32_t value{};
  if(!EquipmentPlatform::query(equipment_platform.get(),dh2::player::EquipmentWorldQueryV1::online,0,identity,value,e))return false;
  if(value<0||value>255){e="Required source online5 byte projection";return false;}out=std::uint8_t(value);return true;
 };
 layout.source_party_count6c4=[s](std::int32_t& out,std::string& e){
  PlayerSkillsRuntime* t{};if(!gameplay_camera_player_runtime_v20(s.world,t,e))return false;
  out=*t->player_manager->manager().character_count_field();return true;
 };
 layout.num_local_players=[s](std::int32_t& out,std::string& e){
  PlayerSkillsRuntime* t{};return gameplay_camera_player_runtime_v20(s.world,t,e)&&t->player_manager->manager().num_local_players(false,out,e);
 };
 layout.has_current_level=s.development_level_present;
 layout.get_player=[s,position](unsigned index,dh2::camera::CameraPartyActorV14& out,std::string& e){
  PlayerSkillsRuntime* t{};if(!gameplay_camera_player_runtime_v20(s.world,t,e))return false;
  dh2::player::PlayerInfoFieldsV1* record{};if(!t->player_manager->manager().get_player(int(index),true,record,e)||!record){if(e.empty())e="Required actual layout PlayerInfo";return false;}
  out={};out.character=record->character660;if(!out.character)return true;
  if(out.character==t->world->player_object->identity){
   if(!t->world->player_object->life){e="Required same camera player life";return false;}
   out.dead=t->world->player_object->life->dead!=0;return position(out.character,out.position160,e);
  }
  if(s.other_party_actor)return s.other_party_actor(out.character,out,e);e="Required actual remaining party Character projection";return false;
 };
 return true;
}
