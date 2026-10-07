#include "player_manager_combat_runtime_v2.hpp"
namespace dh2::player {
PlayerManagerCombatRuntimeV2::PlayerManagerCombatRuntimeV2(PlayerManagerCombatServicesV2 s)
 :services_(s),manager_({this,service}){}
bool PlayerManagerCombatRuntimeV2::service(void* p,const PlayerManagerRequestV1& q,
 PlayerManagerResponseV1& r,std::string& e){
 auto& self=*static_cast<PlayerManagerCombatRuntimeV2*>(p);
 ++self.service_depth_v59_;
 struct Scope {std::uint32_t& depth;~Scope(){--depth;}} scope{self.service_depth_v59_};
 if(self.development_scalar_projection_&&q.operation==PlayerManagerOperationV1::construct_player_info){
  if(!q.player){e="Required source PlayerInfo scalar receiver";return false;}
  // Reset373bdc: local1, Character0, IDs/ordinals-1, unknown680/684=0.
  // Only the retained scalar projection is initialized. No CNetPlayerInfo or
  // embedded network-property constructor completion is claimed here.
  *q.player=PlayerInfoFieldsV1{};return true;
 }
 if(!self.services_.invoke){e="Required actual PlayerInfo constructor / PlayerManager platform service";return false;}
 return self.services_.invoke(self.services_.context,q,r,e);
}
bool PlayerManagerCombatRuntimeV2::rebind_source_services_v59(const PlayerManagerCombatServicesV2& expected,
 const PlayerManagerCombatServicesV2& replacement,std::string& e){
 if(source_transport_active_v59()||!manager_.source_initialized_v59()||development_scalar_projection_){
  e="Required idle actual constructed PlayerManager before native provider adoption";return false;
 }
 if(services_.context!=expected.context||services_.invoke!=expected.invoke||!replacement.context||!replacement.invoke){
  e="PlayerManager native provider owner changed before adoption";return false;
 }
 services_=replacement;e.clear();return true;
}
bool PlayerManagerCombatRuntimeV2::initialize(){return manager_.initialize(error_);}
bool PlayerManagerCombatRuntimeV2::initialize_development_scalar_projection(){
 development_scalar_projection_=true;return manager_.initialize(error_);
}
bool PlayerManagerCombatRuntimeV2::adopt_created_character(const DevelopmentPlayerLaunchV2& q){
 return adopt_created_character(q.internal,q.controller,q.local_index,q.local,q.character,q.same_base_id);
}
bool PlayerManagerCombatRuntimeV2::adopt_created_character(std::int32_t id,
 std::int32_t controller,std::int32_t index,bool local,std::uintptr_t character,
 const std::int16_t* base){
 error_.clear();if(id==-1||!character){error_="Required actual launch ID and same created Character";return false;}
 if(!manager_.add_player(id,controller,index,local,error_))return false;
 PlayerInfoFieldsV1* record{};
 if(!manager_.get_by_internal(id,false,record,error_))return false;
 if(!record||record->internal670!=id){error_="Required source AddPlayer map publication";return false;}
 if(record->character660&&record->character660!=character){error_="PlayerInfo already owns a different Character publication";return false;}
 // The source publication prefix precedes InitializePlayerSavegame and all
 // subsequent controller/save effects. Adoption claims only this field store.
 record->character660=character;record->character_base_id13c8=base;return true;
}
int PlayerManagerCombatRuntimeV2::hit(const character::HitRequest32& q,std::uintptr_t* out){
 using namespace character;using namespace character::skills;
 if(q.service!=hit_main_player&&q.service!=hit_local_player_v6)return 0;
 error_.clear();if(!out){error_="Required Hit query output";return -1;}
 PlayerInfoFieldsV1* record{};
 if(q.service==hit_main_player){
  if(!manager_.get_local_player(0,true,record,error_))return -1;
  *out=record->character660;
 }else{
  if(!q.subject){*out=0;return 1;}
  if(!manager_.get_by_character(q.subject,false,record,error_))return -1;
  *out=record->local66c!=0;
 }
 return 1;
}
int PlayerManagerCombatRuntimeV2::application(const character::skills::SkillApplyRequestV6& q,
 character::skills::SkillApplyResponseV6* out){
 using namespace character::skills;
 if(q.service!=skill_apply_player_lookup_v6)return 0;
 error_.clear();if(!out){error_="Required Apply player lookup output";return -1;}
 PlayerInfoFieldsV1* record{};
 if(!manager_.get_by_character(q.subject,false,record,error_))return -1;
 out->identity=reinterpret_cast<std::uintptr_t>(record);return 1;
}
}
