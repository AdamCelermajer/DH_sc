#include "player_add_after_save_v29.hpp"
#include <cstring>
namespace dh2::player {
bool PlayerAddAfterSaveV29::call(PlayerAddRequestV5 q,PlayerAddResponseV5& r,std::string& e){
 ++result_.calls;r={};using O=PlayerAddOperationV5;
 if(!record_){e="Required same AddCharacter PlayerInfo receiver";return false;}
 if(q.operation==O::is_local){
  if(q.subject!=reinterpret_cast<std::uintptr_t>(record_)){e="AddCharacter local query receiver differs";return false;}
  bool local{};if(!network_.is_local(*record_,local,e))return false;r.value=local;return true;
 }
 if(q.operation==O::set_slot)return save_.set_slot(q.argument,e);
 if(q.operation==O::set_class){save_.receiver().set_player_class(q.argument);return true;}
 if(q.operation==O::skill_count){const auto count=save_.receiver().skills().size();if(count>std::size_t(INT32_MAX)){e="Source skill count outside signed native domain";return false;}r.value=std::int32_t(count);return true;}
 if(q.operation==O::get_slots)return arrays_.get_slots(*record_,q.buffer,q.size,r.value,e);
 if(q.operation==O::get_levels)return arrays_.get_levels(*record_,q.buffer,q.size,r.value,e);
 if(q.operation==O::set_skill_slot)return save_.receiver().set_skill_in_slot(q.argument,std::uint32_t(q.secondary),skill_update_,e);
 if(q.operation==O::set_skill_level)return save_.receiver().set_skill_level(std::uint32_t(q.argument),q.secondary,e);
 if(!services_.provider_lease||!services_.invoke||!services_.invoke(q,r,e)){
  if(e.empty())e="Required source AddCharacter continuation service "+std::to_string(std::uint32_t(q.operation));return false;
 }return true;
}
bool PlayerAddAfterSaveV29::execute(PlayerInfoFieldsV1& p,std::int32_t* count,std::int32_t input,std::string& e){
 e.clear();if(attempted_||busy_||failed_||!p.character660||p.character660!=save_.receiver().character()||
  (p.character_base_id13c8&&p.character_base_id13c8!=save_.property_id_field())){
  e="Required first SAME Character/constructed Save/count continuation";return false;
 }
 if(arrays_.receiver()!=&p||!network_.borrow(p)){e="Required same PlayerInfo constructor skill arrays/network owner";return false;}
 attempted_=busy_=true;source_internal_input_=input;record_=&p;
 struct Guard{bool& b;~Guard(){b=false;}}guard{busy_};
 result_={};result_.development_continuation=true;result_.phase=PlayerAddPhaseV5::save;
 // Native pointer transport to existing metadata only; no cached base-id or
 // original scalar is inferred, and no count publication occurs here.
 p.character_base_id13c8=save_.property_id_field();
 const bool ok=continuation(p,count,e);if(!ok)failed_=true;return ok;
}
bool PlayerAddAfterSaveV29::continuation(PlayerInfoFieldsV1& p,std::int32_t* count,std::string& e){
 using O=PlayerAddOperationV5;const auto id=p.character660;PlayerAddCharacterBorrowV5 actor;
 if(!count||count!=manager_.character_count_field()||!services_.character||!services_.character(id,actor,e)||actor.identity!=id||!actor.receiver||!actor.same_facet){if(e.empty())e="Required SAME retained canonical Player/AddCharacter/count graph";return false;}
 world::CanonicalExistingActorV3 canonical;
 if(!actor.same_facet->borrow(actor.receiver,canonical,e)||canonical.actor.identity!=id||!canonical.actor.shared_handle||canonical.actor.shared_handle->cached!=id){if(e.empty())e="Required same published canonical Player facet/Handle";return false;}
 PlayerAddResponseV5 r;auto invoke=[&](O op,std::uintptr_t subject=0,std::int32_t a=0,std::int32_t b=0,void* buffer=nullptr,std::size_t size=0){return call({op,subject?subject:id,a,b,nullptr,buffer,size},r,e);};
 result_.phase=PlayerAddPhaseV5::save;
 if(!invoke(O::is_local,reinterpret_cast<std::uintptr_t>(&p)))return false;
 if(r.value){if(!invoke(O::set_slot,0,p.save_slot664))return false;}
 else {PlayerAddNetworkBorrowV5 network;if(!services_.network_fields||!services_.network_fields(p,network,e)||!network.player_class380){if(e.empty())e="Required same PlayerInfo class380";return false;}if(!invoke(O::set_class,0,*network.player_class380))return false;}
 if(!actor.controller1f88||!actor.internal1f8c){e="Required source player controller/internal ID store fields";return false;}
 *actor.controller1f88=p.controller_local674;*actor.internal1f8c=p.internal670;result_.phase=PlayerAddPhaseV5::ids;
 if(!invoke(O::online))return false;
 if(r.value){
  if(!invoke(O::is_host,reinterpret_cast<std::uintptr_t>(&p)))return false;
  if(!r.value){if(!invoke(O::get_host))return false;const auto host=r.identity;
   if(host){PlayerAddCharacterBorrowV5 h;if(!services_.character(host,h,e)||h.identity!=host||!h.receiver||!h.position160||!h.rotation16c||!h.initial_position1450||!h.initial_rotation145c||!h.room_zone2f4){if(e.empty())e="Required actual hosting Character placement fields";return false;}
    result_.phase=PlayerAddPhaseV5::placement;
    if(!invoke(O::set_position,0,1,0,const_cast<float*>(h.position160),12)||!invoke(O::set_rotation,0,0,0,const_cast<float*>(h.rotation16c),12)||!invoke(O::set_initial_position,0,0,0,const_cast<float*>(h.initial_position1450),12))return false;
    if(!actor.initial_rotation145c){e="Required same initial rotation145c destination";return false;}
    std::memcpy(actor.initial_rotation145c,h.initial_rotation145c,12);
    bool placed=false;if(*h.room_zone2f4){if(!invoke(O::room_add,*h.room_zone2f4,0,0,reinterpret_cast<void*>(id)))return false;placed=r.value!=0;}
    if(!placed){if(!invoke(O::no_room_add,0,0,0,reinterpret_cast<void*>(id)))return false;if(!actor.zoned2ef){e="Required same source zoned2ef field";return false;}*actor.zoned2ef=1;if(!invoke(O::zone_entered))return false;}
   }
  }
 }
 result_.phase=PlayerAddPhaseV5::initialized;if(!invoke(O::init_all)||!invoke(O::is_local,reinterpret_cast<std::uintptr_t>(&p)))return false;
 if(r.value){if(!invoke(O::is_active,reinterpret_cast<std::uintptr_t>(&p)))return false;if(r.value&&!invoke(O::init_camera))return false;}
 if(!invoke(O::set_idle,0,0))return false;
 std::array<std::int8_t,3> slots; if(!invoke(O::get_slots,reinterpret_cast<std::uintptr_t>(&p),3,0,slots.data(),slots.size()))return false;
 result_.phase=PlayerAddPhaseV5::slots;
 for(unsigned i=0;i<3;++i){if(slots[i]<-1)slots[i]=-1;if(!invoke(O::set_skill_slot,0,int(i),slots[i]))return false;}
 if(!invoke(O::skill_count))return false;
 if(r.value>30&&!invoke(O::assert_skill_count,0,r.value))return false;
 std::array<std::int8_t,30> levels;if(!invoke(O::get_levels,reinterpret_cast<std::uintptr_t>(&p),30,0,levels.data(),levels.size()))return false;
 result_.phase=PlayerAddPhaseV5::levels;
 for(unsigned i=0;i<30;++i){if(!invoke(O::skill_count))return false;if(r.value>int(i)&&levels[i]>=0&&!invoke(O::set_skill_level,0,int(i),levels[i]))return false;}
 if(!invoke(O::is_local,reinterpret_cast<std::uintptr_t>(&p)))return false;
 const bool local=r.value!=0;std::int32_t visible=1;
 if(!local){PlayerAddNetworkBorrowV5 network;if(!services_.network_fields||!services_.network_fields(p,network,e)||!network.visible4e5){if(e.empty())e="Required source network visible4e5";return false;}visible=*network.visible4e5;}
 result_.phase=PlayerAddPhaseV5::visible;if(!invoke(O::set_visible,0,visible)||!invoke(O::current_level))return false;
 result_.phase=PlayerAddPhaseV5::quicksave;if(r.identity&&!invoke(O::quick_save,r.identity,0))return false;
 result_.phase=PlayerAddPhaseV5::count;*count=std::int32_t(std::uint32_t(*count)+1u);result_.count_written=true;
 if(!invoke(O::online))return false;
 if(r.value){PlayerInfoFieldsV1* current{};if(!manager_.get_by_internal(source_internal_input_,true,current,e)||!current)return false;if(current->character660!=id){if(!invoke(O::remove_character,0,0,0,reinterpret_cast<void*>(id)))return false;result_.phase=PlayerAddPhaseV5::complete;result_.completed=true;return true;}}
 result_.phase=PlayerAddPhaseV5::controller;if(!invoke(O::attach_controller,0,source_internal_input_))return false;
 if(!invoke(O::is_local,reinterpret_cast<std::uintptr_t>(&p)))return false;
 if(r.value){result_.phase=PlayerAddPhaseV5::light;if(!invoke(O::attach_light,0,source_internal_input_))return false;}
 result_.phase=PlayerAddPhaseV5::complete;result_.completed=true;return true;
}
}
