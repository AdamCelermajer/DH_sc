#include "canonical_trigger_zone_v22.hpp"
#include "zone_ex_contacts_v90.hpp"
#include <algorithm>
namespace dh2::world {
namespace {template<class F,class... A>bool required(const F& f,const char* n,std::string& e,A&&... a){if(!f){e=std::string("Required actual TriggerZone ")+n;return false;}return f(std::forward<A>(a)...,e);}}
CanonicalTriggerZoneV22::CanonicalTriggerZoneV22(std::shared_ptr<void> p,actor::RuntimeState& r,TriggerZoneServicesV22 s,std::uint32_t source_go_id):base_(reinterpret_cast<std::uintptr_t>(this),source_go_id,std::move(p),r),services_(std::move(s)),initialization_(base_,services_.initialization){
 base_.lifecycle().static84=1;*base_.byte(0x28)=1;*base_.byte(0xf8)=4;*base_.pointer(0x100)=reinterpret_cast<std::uintptr_t>(&network_[0]);*base_.pointer(0x104)=reinterpret_cast<std::uintptr_t>(&network_[1]);
 // ZoneC2 zeroes all three dimensions; actual map defaults then produce200.
}
bool CanonicalTriggerZoneV22::missing(const char* n,std::string& e)const{e=std::string("Required actual TriggerZone ")+n;return false;}
CanonicalPropertyActorV1 CanonicalTriggerZoneV22::properties()noexcept{auto a=canonical_family_fields_v15(*this);a.fields.write_vector3=[](void* p,std::uint32_t o,const auto& v,std::string& e){return static_cast<CanonicalTriggerZoneV22*>(p)->write_vector3(o,v,e);};return a;}
bool CanonicalTriggerZoneV22::read_bool(std::uint32_t o,std::uint8_t& v,std::string& e){if(o==0x3b0){v=reset3b0_;return true;}auto f=base_.properties().fields;return f.read_bool(f.context,o,v,e);}
bool CanonicalTriggerZoneV22::write_bool(std::uint32_t o,std::uint8_t v,std::string& e){if(o==0x3b0){reset3b0_=v;return true;}auto f=base_.properties().fields;return f.write_bool(f.context,o,v,e);}
bool CanonicalTriggerZoneV22::write_int(std::uint32_t o,std::int32_t v,std::string& e){if(o==0x3a8){count3a8_=v;return true;}if(o==0x3ac){delay3ac_=v;return true;}if(o>=0x718&&o<=0x720&&!(o&3)){const auto i=(o-0x718)/4;classification_[i]=v;classification_written_[i]=true;return true;}auto f=base_.properties().fields;return f.write_int(f.context,o,v,e);}
bool CanonicalTriggerZoneV22::write_string(std::uint32_t o,const std::string& v,std::string& e){const std::uint32_t offsets[]{0x724,0x740,0x75c,0x778,0x794,0x7bc};for(unsigned i=0;i<6;++i)if(o==offsets[i]){names_[i]=v;return true;}auto f=base_.properties().fields;return f.write_string(f.context,o,v,e);}
bool CanonicalTriggerZoneV22::write_vector3(std::uint32_t o,const std::array<float,3>& v,std::string& e){if(o==0x374){dimensions_=v;dimensions_written_=true;return true;}auto f=base_.properties().fields;return f.write_vector3(f.context,o,v,e);}
std::int32_t* CanonicalTriggerZoneV22::source_integer(std::uint32_t o)noexcept{switch(o){case 0x3a0:return &characters3a0_;case 0x3a4:return &players3a4_;case 0x3a8:return &count3a8_;case 0x3ac:return &delay3ac_;case 0x3b4:return &activated3b4_;case 0x3b8:return &timer3b8_;case 0x3c0:return &touching3c0_;default:break;}if(o>=0x718&&o<=0x720&&!(o&3)){auto i=(o-0x718)/4;return classification_written_[i]?&classification_[i]:nullptr;}const std::uint32_t offsets[]{0x73c,0x758,0x774,0x790,0x7ac};for(unsigned i=0;i<5;++i)if(o==offsets[i])return &resolved_[i];return nullptr;}
std::uint8_t* CanonicalTriggerZoneV22::source_byte(std::uint32_t o)noexcept{switch(o){case 0x3b0:return &reset3b0_;case 0x3bc:return &local_only3bc_;case 0x7b4:return &active7b4_;case 0x7b5:return &one_player7b5_;default:return base_.byte(o);}}
std::string* CanonicalTriggerZoneV22::source_string(std::uint32_t o)noexcept{
 const std::uint32_t offsets[]{0x724,0x740,0x75c,0x778,0x794,0x7bc};
 for(unsigned i=0;i<6;++i)if(o==offsets[i])return &names_[i];
 return base_.string(o);
}
bool CanonicalTriggerZoneV22::source_set_position_v29(const std::array<float,3>& p,bool destination,std::string& e){
 return required(services_.initialization.set_position,"source SetPosition",e,p.data(),destination);
}
bool CanonicalTriggerZoneV22::zone_init_post(std::string& e){std::int32_t roll{},prob{};if(!required(services_.spawn_roll_probability,"CheckSpawnProbability38bd64",e,roll,prob))return false;if(roll>=prob)return true;
 bool eligible{};if(!initialization_.init_post(eligible,e))return false;if(!dimensions_written_)return missing("source dimensions defaults374",e);
 const auto* scale=base_.vector3(0x120);if(!scale)return missing("SAME source scale120",e);std::array<float,6> box{};for(unsigned i=0;i<3;++i){dimensions_[i]=dimensions_[i]*scale[i];const float half=dimensions_[i]*0.5f;box[i]=-half;box[i+3]=half;}
 if(!required(services_.set_bounding_box,"source virtual9c SetBoundingBox(false)",e,box,false))return false;
 if(physical380_&&!required(services_.create_zone_physical,"actual source PODecorItem/Zone sensor assignment",e,trigger381_))return false;
 if(colzone384_||!*base_.pointer(0x2d8))return true;
 return required(services_.bind_visual_collision_zone,"whole actual _colzone mesh/selector continuation",e,"_colzone",colzone384_,colzone_lease_);
}
bool CanonicalTriggerZoneV22::init_post(std::string& e){if(!zone_init_post(e))return false;
 for(unsigned i=0;i<4;++i){resolved_[i]=-1;if(!names_[i].empty()&&!required(services_.script_id,"ScriptManager GetIDFromName(false)",e,names_[i].c_str(),false,resolved_[i]))return false;}
 resolved_[4]=-1;if(!names_[4].empty()&&!required(services_.effect_id,"actual FxNames ordered lookup",e,names_[4].c_str(),resolved_[4]))return false;
 timer3b8_=delay3ac_;
 if(!names_[5].empty()){std::uintptr_t object{};std::uint32_t type{};if(!required(services_.find_named_object,"GetObjectByName(room=-1,create=false)/Handle/GetObject",e,names_[5].c_str(),-1,false,object,type))return false;if(object)door7b8_=type==2?object:0;}
 local_only3bc_=0;if(resolved_[2]!=-1||resolved_[3]!=-1||resolved_[0]==-1||resolved_[0]<0)return true;
 return required(services_.script_flags,"actual ScriptManager12-byte row+4",e,resolved_[0],local_only3bc_);
}
bool CanonicalTriggerZoneV22::init_final(std::string& e){bool eligible{};return initialization_.init_final(eligible,e);}
void CanonicalTriggerZoneV22::activate()noexcept{activated3b4_=static_cast<std::int32_t>(static_cast<std::uint32_t>(activated3b4_)+1);timer3b8_=delay3ac_;}
bool CanonicalTriggerZoneV22::can_activate()noexcept{return (count3a8_<0||count3a8_>activated3b4_)&&timer3b8_<=0&&base_.lifecycle().enabled8a;}
bool CanonicalTriggerZoneV22::start_once(std::int32_t id,std::string& e){if(id==-1)return true;bool running{};if(!required(services_.script_running,"ScriptManager IsRunning455bec",e,id,running))return false;return running||required(services_.start_script,"StartScript4605c0(room64)",e,id,base_.room64());}
bool CanonicalTriggerZoneV22::show_marker(std::string& e){if(resolved_[4]==-1)return true;if(marker_.identity)return missing("source ShowMarker assertion positive duplicate",e);
 if(!required(services_.create_marker,"FxManager CreateEffect495430(false)",e,resolved_[4],false,marker_))return false;if(!marker_.identity)return true;if(!marker_.lease)return missing("actual marker lease",e);
 return required(services_.marker_owner,"actual FX owner28 assignment",e,marker_,base_.identity())&&required(services_.marker_restart,"FX492aa0(true)",e,marker_,true)&&required(services_.marker_visible,"FX492ef0(true)",e,marker_,true)&&required(services_.marker_loop,"FXGetVisual49267c/visual44/controller40(true)",e,marker_,true);
}
bool CanonicalTriggerZoneV22::hide_marker(std::string& e){if(!marker_.identity)return true;
 if(!required(services_.marker_owner,"actual FX owner28 clear",e,marker_,std::uintptr_t{})||!required(services_.marker_restart,"FX492aa0(true)",e,marker_,true)||!required(services_.marker_visible,"FX492ef0(false)",e,marker_,false)||!required(services_.marker_release,"FxManager494978 SAME pointer release",e,marker_))return false;marker_={};return true;
}
bool CanonicalTriggerZoneV22::number_touching(std::int32_t& n,std::string& e){n=0;std::int32_t count{};if(!required(services_.player_count,"PlayerManager count6c4",e,count))return false;for(std::int32_t i=0;i<count;++i){std::uintptr_t c{};if(!required(services_.player_character,"GetPlayer(index,true)->660",e,i,true,c))return false;if(c){bool hit{};if(!required(services_.touching,"GameObject IsTouching38b518 SAME absolute AABB",e,c,hit))return false;if(hit)++n;}if(!required(services_.player_count,"PlayerManager count6c4 reread",e,count))return false;}return true;}
bool CanonicalTriggerZoneV22::trigger_update(std::string& e){
 // Source ZoneEx.update prunes existing contact entries only. Empty ctor tree
 // is a genuine no-op; positive entries require actual collision predicates.
 for(auto i=contacts_.begin();i!=contacts_.end();){bool inside{};if(!required(services_.zone_inside,"Zone IsInside3970c8",e,*i,inside))return false;if(inside){++i;continue;}bool character{},player{};if(!required(services_.is_character,"actual virtual24",e,*i,character))return false;if(character){--characters3a0_;if(!required(services_.is_player,"actual virtual28",e,*i,player))return false;if(player)--players3a4_;}i=contacts_.erase(i);}
 if(!update_timer(e))return false;
 if(!required(services_.require_online_update,"GameObject RequireOnlineUpdate38b8b8",e))return false;auto* idle=base_.integer(0x370);if(!idle)return missing("source idle sound signedshort370",e);return *idle<0||required(services_.play_idle_sound,"GameObject PlaySound38ae2c",e);
}
bool CanonicalTriggerZoneV22::update_timer(std::string& e){if(timer3b8_<=0)return true;std::int32_t dt{};if(!required(services_.app_delta_ms,"Application31f66c actual dt",e,dt))return false;timer3b8_=static_cast<std::int32_t>(static_cast<std::uint32_t>(timer3b8_)-static_cast<std::uint32_t>(dt));return true;}
bool CanonicalTriggerZoneV22::update(std::string& e){TriggerLocalPlayerV22 local;if(!required(services_.local_player,"GetLocalPlayer(0,true)",e,0,true,local))return false;if(local.character&&local.dead1480)return true;
 if(door7b8_){std::int32_t state{};if(!required(services_.door_state,"actual Door state3a8",e,door7b8_,state))return false;if(state==1||state==3)return true;}
 if(!can_activate())return true; // Qualified GameObject MeetCondition38ab60=true.
 bool online{},remote{};if(!required(services_.online5,"actual Online byte5",e,online))return false;if(online){if(!required(services_.virtual54,"actual source virtual54",e,remote))return false;if(remote)return true;}
 if(!trigger_update(e))return false;std::int32_t players{};if(!required(services_.player_count,"PlayerManager count6c4",e,players)||!required(services_.online5,"actual Online byte5 reread",e,online))return false;
 std::int32_t touching=touching3c0_;if(online){if(!required(services_.virtual54,"actual source virtual54 reread",e,remote))return false;}if(!online||!remote){if(!number_touching(touching,e))return false;touching3c0_=touching;}
 const auto on=resolved_[2]!=-1?resolved_[2]:resolved_[0];const auto off=resolved_[3]!=-1?resolved_[3]:resolved_[1];bool entered=resolved_[2]!=-1?touching==players:touching>0;bool exited=touching<=0;
 if(!required(services_.local_player,"GetLocalPlayer(0,true) reread",e,0,true,local))return false;
 if(entered&&local_only3bc_){if(!local.character)entered=false;else{bool hit{};if(!required(services_.touching,"source local character IsTouching",e,local.character,hit))return false;if(!hit)entered=false;}}
 // Source repeats IsTouching when local-only; do not collapse the callback.
 if(local_only3bc_){if(!local.character)exited=false;else{bool hit{};if(!required(services_.touching,"source repeated local character IsTouching",e,local.character,hit))return false;exited=!hit;}}
 if(entered&&!active7b4_){active7b4_=1;if(!start_once(on,e))return false;timer3b8_=delay3ac_;if(!hide_marker(e))return false;if(off==-1)activate();}
 if(active7b4_&&exited){active7b4_=0;if(off!=-1){if(!start_once(off,e))return false;activate();}}
 if(players<=1||local_only3bc_||resolved_[2]==-1||resolved_[0]==-1)return true;
 const bool any=touching>0;if(!one_player7b5_){if(!any)return true;if(!start_once(resolved_[0],e))return false;one_player7b5_=1;return show_marker(e);}if(any)return true;one_player7b5_=0;return start_once(resolved_[1],e)&&hide_marker(e);
}
bool CanonicalTriggerZoneV22::collision_begin(std::uintptr_t object,std::string& e){return zone_ex_contact_v90(contacts_,characters3a0_,players3a4_,object,true,services_.zone_inside,services_.is_character,services_.is_player,e);}
bool CanonicalTriggerZoneV22::collision_end(std::uintptr_t object,std::string& e){return zone_ex_contact_v90(contacts_,characters3a0_,players3a4_,object,false,services_.zone_inside,services_.is_character,services_.is_player,e);}
bool CanonicalTriggerZoneV22::destroy(std::string& e){if(destroyed_)return missing("destruction replay refused",e);destroyed_=true;if(!hide_marker(e))return false;for(auto i=names_.rbegin();i!=names_.rend();++i)i->clear();for(auto index:{1u,0u})if(!required(services_.destroy_network,"actual NetStructTrigger destructor",e,network_[index]))return false;colzone_lease_.reset();return required(services_.destroy_base,"actual Zone/GameObject destructor",e,base_);}
CanonicalClassReceiverV1 CanonicalTriggerZoneV22::factory_receiver(std::shared_ptr<CanonicalTriggerZoneV22> o,std::shared_ptr<const void> xml){auto a=canonical_class_receiver_v1(o);a.source_lease=std::move(xml);a.init_post=[o](auto& e){return o->init_post(e);};a.is_game_object=[](bool& v,auto&){v=true;return true;};a.position=[o](auto& p,auto&){std::copy_n(o->base().vector3(0x160),3,p.begin());return true;};a.set_position=[o](const auto& p,bool b,auto& e){return required(o->services_.initialization.set_position,"source SetPosition",e,p.data(),b);};return a;}
}
