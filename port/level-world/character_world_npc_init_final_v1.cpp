#include "character_world_npc_init_final_v1.hpp"
namespace dh2::character {
bool character_npc_spawn_handle_player_v1(skills::CharacterWorldRuntimeV1& world,std::uintptr_t identity,bool& player,std::string& error){
 target_providers::Handle16 handle{};std::uintptr_t object=0;
 if(world.get_handle(identity,&handle)||world.resolve(&handle,&object)){error="required source spawn handle lookup failed";return false;}
 if(!object){player=false;return true;}
 return character_npc_actor_player_v1(world,object,player,error);
}
bool character_npc_actor_player_v1(skills::CharacterWorldRuntimeV1& world,std::uintptr_t object,bool& player,std::string& error){
 auto services=world.targets().search_services();target_search::Request24 request{target_search::is_player,0,object,0};target_search::Response16 result{};
 if(services.invoke(services.context,&request,&result)){error="required source spawn IsPlayer failed";return false;}
 player=result.word!=0;return true;
}
std::int32_t character_generate_spawn_probability_v1(WorldNpcSpawnGlobalsV1& g,bool network) noexcept {
 auto& seed=network?g.network_seed:g.local_seed;auto& count=network?g.network_count:g.local_count;
 const std::uint32_t next=seed*59051u+177149u;
 seed=next%14348907u;++count;return std::int32_t(seed%99u);
}
std::uint32_t character_light_set_id_v1(const std::string& name) noexcept {
 static constexpr const char* names[]={"PlayerLight","SceneLight","CameraLight","MonsterLight"};
 for(std::uint32_t i=0;i<4;++i)if(name==names[i])return i;return 0;
}
bool character_npc_external_init_final_v1(CharacterScriptSession& session,std::uintptr_t active,std::string& error){
 error.clear();
 ScriptSessionView view{};
 if(!active||!session.owner().active(view)||view.identity!=active){error="required same published AISExternal unavailable";return false;}
 if(view.kind!=script_monster&&view.kind!=script_external){error="required source AIS OnInitFinal class dispatch unavailable";return false;}
 std::uint32_t lua_error=0;
 const int result=session.owner().call_discard(active,"OnInitFinal",nullptr,0,lua_error);
 if(result){error=session.owner().error();if(error.empty())error="source AISExternal OnInitFinal call failed";return false;}
 // Original ScriptBase call discards the ordinary Lua status; the real call
 // already preserves its diagnostic. Native missing providers never succeed.
 return true;
}
bool character_check_spawn_probability_v1(WorldNpcSpawnGlobalsV1& globals,std::int32_t& cached,
 const std::int32_t& threshold,std::uint8_t& deleted,CharacterWorldNpcObjectV1& object,
 const WorldNpcSpawnServicesV1& s,std::int32_t& result,std::string& error){
 error.clear();bool player;
 if(!s.handle_is_player||!s.handle_is_player(s.context,player,error)){if(error.empty())error="required source spawn GetHandle/IsPlayer unavailable";return false;}
 if(player){cached=-2;result=cached;return true;}
 if(cached!=-1){result=cached;return true;}
 bool network;
 if(!s.network_enabled||!s.network_enabled(s.context,network,error)){if(error.empty())error="required source NetworkManager mode unavailable";return false;}
 bool use_network=false;
 if(network){
  std::int32_t index;
  if(!s.network_index||!s.network_index(s.context,index,error)){if(error.empty())error="required source network object index unavailable";return false;}
  if(index!=-1){if(!s.network_generator||!s.network_generator(s.context,use_network,error)){if(error.empty())error="required source network generator selector unavailable";return false;}}
 }
 cached=character_generate_spawn_probability_v1(globals,use_network);
 if(cached<threshold){cached=-2;result=cached;return true;}
 if(!object.set_visible(false)){error=object.error();return false;}
 if(!s.object_delete||!s.object_delete(s.context,error)){if(error.empty())error="required source ObjectBase::Delete unavailable";return false;}
 deleted=0;
 if(!s.mark_for_deletion||!s.mark_for_deletion(s.context,error)){if(error.empty())error="required source ObjectManager::MarkForDeletion unavailable";return false;}
 result=cached;return true;
}
CharacterWorldNpcInitFinalV1::CharacterWorldNpcInitFinalV1(WorldNpcObjectFieldsV1& fields,
 CharacterWorldNpcObjectV1& object,std::uint8_t& initialized,std::int32_t& probability,
 std::string& name,std::uintptr_t& node,WorldNpcInitFinalServicesV1 services)
 :fields_(fields),object_(object),initialized_(initialized),probability_(probability),pf_name_(name),target_node_(node),services_(services){}
bool CharacterWorldNpcInitFinalV1::call(bool (*fn)(void*,std::string&),const char* missing){
 if(!fn){error_=missing;return false;}if(fn(services_.context,error_))return true;
 if(error_.empty())error_=missing;return false;
}
bool CharacterWorldNpcInitFinalV1::player(bool& value){
 if(!services_.is_player){error_="required source IsPlayer unavailable";return false;}
 return services_.is_player(services_.context,value,error_);
}
bool CharacterWorldNpcInitFinalV1::game_final(const char* name,const std::string& light,const float* pos,const float* aabb){
 std::int32_t spawn;
 if(!services_.check_spawn||!services_.check_spawn(services_.context,spawn,error_)){if(error_.empty())error_="required source CheckSpawnProbability unavailable";return false;}
 if(spawn>=probability_||fields_.disabled81)return true;
 if(fields_.non_zonable2ed&&!object_.set_visible(true)){error_=object_.error();return false;}
 if(!object_.init_pf_object(pos,aabb)){error_=object_.error();return false;}
 if(!name){error_="required actual Object name unavailable";return false;}pf_name_=name;
 if(!services_.has_visual){error_="required actual VisualObject presence unavailable";return false;}
 if(services_.has_visual(services_.context)){
  if(!call(services_.sync_visual,"required source VisualObject::Sync unavailable"))return false;
  if(!services_.assign_light){error_="required same VisualObject light field unavailable";return false;}
  if(!services_.assign_light(services_.context,character_light_set_id_v1(light),error_))return false;
  // Source reloads visual and its scene pointer after synchronous light lookup.
  if(services_.has_visual(services_.context)){
   if(!services_.has_scene){error_="required actual VisualObject scene presence unavailable";return false;}
   if(!services_.has_scene(services_.context))return object_.update_pf()||(error_=object_.error(),false);
   if(!services_.find_target_node){error_="required actual scene target_node lookup unavailable";return false;}
   if(!services_.find_target_node(services_.context,"target_node",target_node_,error_))return false;
  }
 }
 if(!object_.update_pf()){error_=object_.error();return false;}return true;
}
bool CharacterWorldNpcInitFinalV1::initialize(const char* name,const std::string& light,const float* pos,const float* aabb){
 error_.clear();if(initialized_)return true;initialized_=1;
 std::int32_t spawn;
 if(!services_.check_spawn||!services_.check_spawn(services_.context,spawn,error_)){if(error_.empty())error_="required source CheckSpawnProbability unavailable";return false;}
 if(spawn>=probability_)return true;
 if(!game_final(name,light,pos,aabb))return false;
 if(!services_.char_type){error_="required same Character AI type unavailable";return false;}
 std::int32_t type;if(!services_.char_type(services_.context,type,error_))return false;
 bool follower=type==3;
 if(!follower){if(!services_.char_type(services_.context,type,error_))return false;follower=type==2;}
 if(follower&&!call(services_.follower_placement,"required source follower/faerie placement unavailable"))return false;
 if(!services_.has_visual){error_="required actual VisualObject presence unavailable";return false;}
 if(services_.has_visual(services_.context)){
  bool isplayer;if(!player(isplayer))return false;
  if(!services_.assign_light){error_="required same VisualObject light field unavailable";return false;}
  if(!services_.assign_light(services_.context,character_light_set_id_v1(isplayer?"PlayerLight":"MonsterLight"),error_))return false;
 }
 if(!call(services_.ai_init_final,"required source CharAI::OnInitFinal unavailable"))return false;
 bool isplayer;if(!player(isplayer))return false;
 if(isplayer&&!call(services_.local_player_tail,"required source local PlayerManager/skill/recalc/save tail unavailable"))return false;
 return true;
}
}
