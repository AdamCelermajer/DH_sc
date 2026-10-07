#include "canonical_destructible_container_v16.hpp"
#include <algorithm>
#include <cstring>
namespace dh2::world {
namespace {template<class F,class... A>bool required(const F& f,const char* name,std::string& e,A&&... a){if(!f){e=std::string("Required actual DestructibleContainer ")+name;return false;}return f(std::forward<A>(a)...,e);}}
CanonicalDestructibleContainerV16::CanonicalDestructibleContainerV16(std::shared_ptr<void> p,actor::RuntimeState& r,GameObjectInitializationServicesV1 i,DestructibleContainerServicesV16 s):base_(reinterpret_cast<std::uintptr_t>(this),1,std::move(p),r),init_services_(std::move(i)),initialization_(base_,init_services_),services_(std::move(s)){
 *base_.byte(0x28)=1;base_.lifecycle().static84=0;*base_.byte(0xf8)=2;*base_.pointer(0x100)=reinterpret_cast<std::uintptr_t>(&network_[0]);*base_.pointer(0x104)=reinterpret_cast<std::uintptr_t>(&network_[1]);
}
bool CanonicalDestructibleContainerV16::missing(const char* n,std::string& e)const{e=std::string("Required actual DestructibleContainer ")+n;return false;}
bool CanonicalDestructibleContainerV16::read_bool(std::uint32_t o,std::uint8_t& v,std::string& e){if(o==0x390){v=death_reset390_;return true;}auto a=base_.properties().fields;return a.read_bool(a.context,o,v,e);}
bool CanonicalDestructibleContainerV16::write_bool(std::uint32_t o,std::uint8_t v,std::string& e){if(o==0x390){death_reset390_=v!=0;return true;}auto a=base_.properties().fields;return a.write_bool(a.context,o,v,e);}
bool CanonicalDestructibleContainerV16::write_int(std::uint32_t o,std::int32_t v,std::string& e){if(o==0x374){data374_=v;data_produced_=true;return true;}auto a=base_.properties().fields;return a.write_int(a.context,o,v,e);}
bool CanonicalDestructibleContainerV16::write_string(std::uint32_t o,const std::string& v,std::string& e){if(o==0x378){data_desc378_=v;return true;}auto a=base_.properties().fields;return a.write_string(a.context,o,v,e);}
const DestructibleContainerRowV16* CanonicalDestructibleContainerV16::current_row()const{return services_.table?services_.table->row(data_id()):nullptr;}
bool CanonicalDestructibleContainerV16::set_state(std::int32_t v,std::string& e){if(!*base_.pointer(0x2d8))return missing("source SetState requires actual visual/root",e);if(!required(services_.common.scene_flags,"Container SetState39f3cc",e,v==3?0x400u:0u,v==3?0u:0x400u))return false;state394_=v;return true;}
bool CanonicalDestructibleContainerV16::container_init_post(std::string& e){
 std::int32_t roll{},prob{};if(!required(services_.common.spawn_roll_and_probability,"CheckSpawnProbability",e,roll,prob))return false;if(roll>=prob)return true;
 if(!services_.table)return missing("SAME Arrays DestructibleContainers",e);data374_=data_id();data_produced_=true;auto* row=services_.table->row(data374_);
 if(row&&row->visual()!=-1&&!required(services_.common.visual_asset,"actual Visuals asset",e,row->visual()))return false;
 bool eligible{};if(!initialization_.init_post(eligible,e))return false;
 // Qualified GameObject::MeetCondition38ab60 is the recovered literal true.
 if(*base_.pointer(0x2d8)){
  if(!required(services_.bind_callbacks,"Container timeline callbacks",e,false))return false;bool played{};
  if(!required(services_.common.play_animation,"prespawn",e,"prespawn",played))return false;
  if(played){if(!set_state(0,e))return false;}else{
   if(!required(services_.common.play_animation,"spawn",e,"spawn",played))return false;
   if(played){if(!set_state(1,e))return false;}else{if(!required(services_.common.play_animation,"idle",e,"idle",played))return false;if(played&&!set_state(2,e))return false;}}
  if(!required(services_.visual_sync,"VisualObject Sync470a54",e))return false;
 }
 bool sound{};if(!required(services_.common.has_sound_manager,"actual sound manager presence",e,sound))return false;
 if(sound){row=current_row();if(!required(services_.common.load_sound,"LoadSound3699fc",e,row?row->sound():-1))return false;}
 row=data_produced_?services_.table->row(data374_):nullptr;
 return required(services_.common.load_object_script,"LoadScript38ef60",e,row?row->script30.c_str():nullptr,"data/scripts/objects/");
}
bool CanonicalDestructibleContainerV16::init_post(std::string& e){if(!container_init_post(e))return false;if(!*base_.pointer(0x2d8))return true;if(!required(services_.bind_callbacks,"derived timeline callbacks",e,true))return false;std::uint32_t count{};if(!required(services_.animation_count,"timeline count slot10",e,count))return false;if(count>3)stages6f0_=remaining6f4_=count-3;return true;}
bool CanonicalDestructibleContainerV16::init_final(std::string& e){std::int32_t roll{},prob{};if(!required(services_.common.spawn_roll_and_probability,"CheckSpawnProbability",e,roll,prob))return false;if(roll>=prob)return true;bool eligible{};if(!initialization_.init_final(eligible,e))return false;if(std::uint32_t(state394_)-3u>1u&&!required(services_.common.source_on_interact,"GameObject Update",e))return false;return required(services_.common.create_attach_po_decor,"actual PODecor/SetPhysical",e);}
bool CanonicalDestructibleContainerV16::do_open(std::string& e){if(!services_.table)return missing("SAME Arrays DestructibleContainers",e);auto* row=current_row();if(!required(services_.common.drop_loot_table,"actual ItemObject DropLootTable",e,row?row->loot():-1,opener398_,-1,false))return false;bool script{};if(!required(services_.common.has_script,"same object LuaScript presence",e,script))return false;return !script||required(services_.common.script_call,"OnOpen",e,"OnOpen",opener398_,nullptr);}
bool CanonicalDestructibleContainerV16::container_interact(std::uintptr_t actor,std::string& e){if(std::uint32_t(state394_)-3u<=1u)return true;opener398_=actor;if(!set_state(4,e))return false;if(!services_.table)return missing("SAME Arrays DestructibleContainers",e);auto* row=current_row();if((!row||!row->keep_physics20)&&!required(services_.common.detach_physical,"SetPhysical(NULL,false)",e))return false;
 if(*base_.pointer(0x2d8)){bool played{};if(!set_state(3,e)||!required(services_.common.play_animation,"activate",e,"activate",played))return false;}else if(!do_open(e))return false;
 if(!required(services_.common.emit_complete_source_sound_v40,"complete source container sound emission",e))return false;return required(services_.common.source_on_interact,"GameObject Update",e);
}
bool CanonicalDestructibleContainerV16::destruction_quest(std::uintptr_t actor,std::string& e){if(!services_.table)return missing("SAME Arrays DestructibleContainers",e);std::int32_t id{};if(!required(services_.constant,"GetPyCst",e,"v2QuestObjectiveType","DestroyGameObject",id))return false;DestructibleQuestEventV16 event{id,actor,base_.room64(),0,0,-1,data_id()};return required(services_.raise_quest,"SAME Level EventManager RaiseAsync synchronous339090",e,event);}
bool CanonicalDestructibleContainerV16::interact(std::uintptr_t actor,std::string& e){
 if(remaining6f4_){--remaining6f4_;if(*base_.pointer(0x2d8)&&!required(services_.play_index,"timeline staged Play index",e,stages6f0_-remaining6f4_,false))return false;if(!services_.table)return missing("SAME Arrays DestructibleContainers",e);return required(services_.common.emit_complete_source_sound_v40,"complete source staged container sound emission",e);}
 if(!destruction_quest(actor,e)||!container_interact(actor,e))return false;
 bool character{};if(!required(services_.as_character,"actual Handle AsChar",e,actor,character))return false;if(!character)return true;
 if(!required(services_.increment_stat,"actual CharStat increment",e,actor,217,1))return false;std::int32_t count{};if(!required(services_.get_stat,"actual CharStat query",e,actor,217,count))return false;if(count<=199)return true;
 bool local{};if(!required(services_.is_local_player,"actual PlayerManager local predicate",e,actor,local))return false;if(!local)return true;std::int32_t id{};if(!required(services_.trophy_id,"actual TrophyNames lookup",e,"destroy_200_breakables",id))return false;return required(services_.unlock_trophy,"actual TrophyManager unlock",e,id);
}
bool CanonicalDestructibleContainerV16::base_animation_event(const char* name,std::string& e){if(!std::strcmp(name,"opened"))return do_open(e);bool script{};if(!required(services_.common.has_script,"same object LuaScript presence",e,script))return false;return !script||required(services_.common.script_call,"OnAnimEvent",e,"OnAnimEvent",std::uintptr_t{},name);}
bool CanonicalDestructibleContainerV16::animation_event(const char* name,std::string& e){if(!name)return missing("authored event name",e);if(!std::strcmp(name,"fx"))return true; // whole DoEffects3a0d68 bx lr
 if(!std::strcmp(name,"opened")&&!destruction_quest(0,e))return false;return base_animation_event(name,e);}
bool CanonicalDestructibleContainerV16::animation_finished(bool active,std::string& e){bool played{};if(state394_==1)return set_state(2,e)&&required(services_.common.play_animation,"idle",e,"idle",played);if(state394_==3)return set_state(4,e)&&required(services_.common.play_animation,"idleactive",e,"idleactive",played);return active||required(services_.common.scene_flags,"inactive scene flag",e,0x200u,0u);}
bool CanonicalDestructibleContainerV16::spawn(std::string& e){if(!*base_.pointer(0x2d8)||state394_!=0)return true;bool played{};return set_state(1,e)&&required(services_.common.play_animation,"spawn",e,"spawn",played);}
bool CanonicalDestructibleContainerV16::destroy(std::string& e){if(destroyed_)return missing("destruction replay refused",e);if(!services_.destroy_base)return missing("Container destructor actual base release",e);destroyed_=true;
 for(auto index:{1u,0u}){auto& n=network_[index];if(*n.raw(0x11c)){if(!required(services_.destroy_network,"actual network node destruction370fd0",e,n,index?0x548u:0x3a0u))return false;*n.raw(0x11c)=0;*n.raw(0x110)=0;const auto header=reinterpret_cast<std::uintptr_t>(n.byte(0x10c));*n.pointer(0x114)=header;*n.pointer(0x118)=header;}}
 data_desc378_.clear();return services_.destroy_base(base_,e);}
bool CanonicalDestructibleContainerV16::set_position(const std::array<float,3>& p,bool update,std::string& e){return required(init_services_.set_position,"SAME SetPosition",e,p.data(),update);}
CanonicalClassReceiverV1 CanonicalDestructibleContainerV16::factory_receiver(std::shared_ptr<CanonicalDestructibleContainerV16> o,std::shared_ptr<const void> xml){auto a=canonical_class_receiver_v1(o);a.source_lease=std::move(xml);a.init_post=[o](std::string& e){return o->init_post(e);};a.is_game_object=[](bool& b,std::string&){b=true;return true;};a.position=[o](std::array<float,3>& p,std::string&){std::copy_n(o->base().vector3(0x160),3,p.begin());return true;};a.set_position=[o](const auto& p,bool b,std::string& e){return o->set_position(p,b,e);};return a;}
}
