#include "canonical_trigger_object_v28.hpp"
#include "../game-data/player_savegame_v1.hpp"
#include <algorithm>
#include <utility>
#include <cstring>
#include <limits>
#include "game_object_spawn_probability_v1.hpp"
namespace dh2::world {
CanonicalTriggerObjectV28::CanonicalTriggerObjectV28(std::shared_ptr<void> world,actor::RuntimeState& runtime,
 GameObjectInitializationServicesV1 initialization,TriggerObjectServicesV28 services):
 base_(reinterpret_cast<std::uintptr_t>(this),20,std::move(world),runtime),
 initialization_services_(std::move(initialization)),initialization_(base_,initialization_services_),services_(std::move(services)){
 base_.lifecycle().static84=0;base_.lifecycle().updating85=1;
 *base_.byte(0x28)=1;*base_.byte(0xf8)=4;
 *base_.pointer(0x100)=reinterpret_cast<std::uintptr_t>(&network_[0]);
 *base_.pointer(0x104)=reinterpret_cast<std::uintptr_t>(&network_[1]);
}
CanonicalPropertyActorV1 CanonicalTriggerObjectV28::properties()noexcept{
 auto result=canonical_family_fields_v15(*this);
 result.fields.write_vector3=[](void* p,std::uint32_t o,const auto& v,std::string& e){return static_cast<CanonicalTriggerObjectV28*>(p)->write_vector3(o,v,e);};return result;
}
bool CanonicalTriggerObjectV28::read_bool(std::uint32_t o,std::uint8_t& v,std::string& e){if(o==0x3b0){v=reset3b0_;return true;}auto f=base_.properties().fields;return f.read_bool(f.context,o,v,e);}
bool CanonicalTriggerObjectV28::write_bool(std::uint32_t o,std::uint8_t v,std::string& e){if(o==0x3b0){reset3b0_=v;return true;}auto f=base_.properties().fields;return f.write_bool(f.context,o,v,e);}
bool CanonicalTriggerObjectV28::write_int(std::uint32_t o,std::int32_t v,std::string& e){
 if(o==0x3a8){count3a8_=v;return true;}if(o==0x3ac){delay3ac_=v;return true;}
 auto f=base_.properties().fields;return f.write_int(f.context,o,v,e);
}
bool CanonicalTriggerObjectV28::write_string(std::uint32_t o,const std::string& v,std::string& e){
 const std::uint32_t offsets[]{0x718,0x734,0x750,0x76c};for(unsigned i=0;i<4;++i)if(o==offsets[i]){names_[i]=v;return true;}
 auto f=base_.properties().fields;return f.write_string(f.context,o,v,e);
}
bool CanonicalTriggerObjectV28::write_vector3(std::uint32_t o,const std::array<float,3>& v,std::string& e){if(o==0x374){dimensions374_=v;return true;}auto f=base_.properties().fields;return f.write_vector3(f.context,o,v,e);}
std::int32_t* CanonicalTriggerObjectV28::source_integer(std::uint32_t o)noexcept{
 switch(o){case 0x3a0:return &characters3a0_;case 0x3a4:return &players3a4_;case 0x3a8:return &count3a8_;case 0x3ac:return &delay3ac_;case 0x3b4:return &activated3b4_;case 0x3b8:return &timer3b8_;case 0x3c0:return &touching3c0_;case 0x730:return &data_id730_;case 0x74c:return &script_id74c_;case 0x768:return script_id768_?&*script_id768_:nullptr;default:return base_.integer(o);}
}
std::uint8_t* CanonicalTriggerObjectV28::source_byte(std::uint32_t o)noexcept{switch(o){case 0x3b0:return &reset3b0_;case 0x3bc:return &local_only3bc_;case 0x784:return &byte784_;default:return base_.byte(o);}}
std::string* CanonicalTriggerObjectV28::source_string(std::uint32_t o)noexcept{const std::uint32_t offsets[]{0x718,0x734,0x750,0x76c};for(unsigned i=0;i<4;++i)if(o==offsets[i])return &names_[i];return base_.string(o);}
namespace {
bool required_trigger_startup_v78(const char* name,std::string& e){e=std::string("Required actual TriggerObject startup ")+name;return false;}
bool borrow_trigger_table_v78(const TriggerObjectStartupServicesV78& services,TriggerObjectTableBorrowV78& out,std::string& e){
 if(!services.owner||!services.borrow_tables)return required_trigger_startup_v78("Arrays::TriggerObjects producer",e);
 TriggerObjectTableBorrowV78 same;if(!services.borrow_tables(same,e))return false;
 if(!same.receiver||!same.names||!same.rows||same.names->size()!=same.rows->size()||same.names->size()>std::size_t(std::numeric_limits<std::int32_t>::max()))
  return required_trigger_startup_v78("SAME actual declaration names/rows",e);
 out=std::move(same);return true;
}
const TriggerObjectDeclarationRowV78* trigger_row_v78(const TriggerObjectTableBorrowV78& table,std::int32_t id,std::string& e){
 if(id<0||std::size_t(id)>=table.rows->size()){required_trigger_startup_v78("actual source730 declaration row",e);return nullptr;}return &(*table.rows)[id];
}
}
bool CanonicalTriggerObjectV28::init_post(std::string& e){
 if(destruction_started_){e="TriggerObject startup cannot reuse a native destruction prefix";return false;}
 e.clear();std::int32_t roll{};
 if(!initialization_services_.owner||!initialization_services_.check_spawn_probability)return required_trigger_startup_v78("CheckSpawnProbability38bd64",e);
 if(!initialization_services_.check_spawn_probability(roll,e))return false;
 const auto* probability=base_.integer(0x274);if(!probability)return required_trigger_startup_v78("fresh SAME probability274",e);
 if(roll>=*probability)return true;
 const auto& leaves=services_.startup;TriggerObjectTableBorrowV78 table;if(!borrow_trigger_table_v78(leaves,table,e))return false;
 std::int32_t found=-1;for(std::size_t i=0;i<table.names->size();++i)if(!std::strcmp(names_[0].c_str(),(*table.names)[i].c_str())){found=static_cast<std::int32_t>(i);break;}
 data_id730_=found; // miss -1 too, BEFORE script-name resolution/Zone body
 if(found!=-1){const auto* row=trigger_row_v78(table,found,e);if(!row)return false;
  if(row->visual14!=-1){if(!table.objects)return required_trigger_startup_v78("SAME GameObjectDict",e);const std::string* model{};
   if(!table.objects->file(row->visual14,model,e))return false;if(!model)return required_trigger_startup_v78("actual visual14 model CString",e);
   auto* target=base_.string(0x290);if(!target)return required_trigger_startup_v78("SAME model290",e);*target=*model;
  }
 }
 if(!leaves.owner||!leaves.borrow_script_manager)return required_trigger_startup_v78("SAME loaded ScriptManager names",e);
 std::shared_ptr<loader::ScriptManagerOwnerV52> scripts;if(!leaves.borrow_script_manager(scripts,e))return false;
 if(!scripts)return required_trigger_startup_v78("actual ScriptManager receiver",e);
 // Original always calls for both names, including empty CStrings. One source
 // manager receiver spans both reads; false excludes genuine common names.
 script_id74c_=scripts->id_from_name(names_[1].c_str(),false);
 script_id768_=scripts->id_from_name(names_[2].c_str(),false);
 // Source Trigger::InitPost398874 is exactly B Zone::InitPost39771c.
 if(!zone_init_post_v76(base_,initialization_,initialization_services_,dimensions374_,physical380_,trigger381_,colzone384_,colzone_lease_,leaves.zone,e))return false;
 if(names_[3].empty()||!std::strcmp(names_[3].c_str(),"Invalid"))byte784_=1;
 else{
  if(!leaves.owner||!leaves.borrow_conditions)return required_trigger_startup_v78("SAME Main Arrays::Conditions arena",e);
  ConditionDataInitServicesV3 conditions;if(!leaves.borrow_conditions(conditions,e))return false;
  if(!conditions.owner||!conditions.conditions)return required_trigger_startup_v78("genuine ConditionList declarations",e);
  for(const auto& row:*conditions.conditions)if(!std::strcmp(names_[3].c_str(),row.name.c_str())){
   if(!conditions.construct_condition)return required_trigger_startup_v78("actual ConditionList C1",e);
   std::uintptr_t actual{};if(!conditions.construct_condition(actual,e))return false;
   if(!actual)return required_trigger_startup_v78("positive native ConditionList allocation",e);
   condition788_=actual;condition_lease_=conditions.owner; // source publishes BEFORE Assign
   // Capture this arena's actual destructor now, while the exact V3 owner is
   // proven. Teardown must not borrow a later World/runtime/table to free788.
   destroy_condition788_=conditions.destroy_condition;
   if(!conditions.initialize_condition)return required_trigger_startup_v78("actual AssignPyData478914",e);
   if(!conditions.initialize_condition(actual,row.argument8,row.argument4,e))return false;break;
  }
  // Exact name miss retains prior788/784, with no fabricated alias or Eval.
 }
 bool meets{};if(!game_object_meet_condition_v1(meets,e))return false;
 if(data_id730_==-1||!meets){if(!leaves.owner||!leaves.source_delete)return required_trigger_startup_v78("selected virtual40 Delete(false)",e);return leaves.source_delete(*this,false,e);}
 const auto* visual_slot=base_.pointer(0x2d8);if(!visual_slot)return required_trigger_startup_v78("SAME visual2d8",e);
 const auto visual=*visual_slot;if(visual){if(!leaves.owner||!leaves.play_idle_animation)return required_trigger_startup_v78("actual controller idle/idlelocked",e);
  if(!leaves.play_idle_animation(*this,visual,byte784_?"idle":"idlelocked",true,false,false,e))return false;
 }
 if(!leaves.owner||!leaves.create_physical)return required_trigger_startup_v78("genuine resource constructor/assignment",e);
 if(!leaves.create_physical(*this,e))return false;
 if(!leaves.borrow_sound_manager)return required_trigger_startup_v78("actual nullable VoxSoundManager snapshot",e);
 StartupSoundManagerBorrowV87 sound;if(!leaves.borrow_sound_manager(sound,e))return false;
 if(!sound.valid_nullable())return required_trigger_startup_v78("actual captured SoundManager owner/method",e);
 if(sound){TriggerObjectTableBorrowV78 current;if(!borrow_trigger_table_v78(leaves,current,e))return false;
  const auto* row=trigger_row_v78(current,data_id730_,e);if(!row)return false;
  if(!sound.load_sound(row->sound10,e))return false;
 }
 // Source reborrows730/table after sound; even absent sound manager reaches
 // LoadExternalScript(rowCStringc,"data/scripts/objects/"). No scheduling.
 TriggerObjectTableBorrowV78 current;if(!borrow_trigger_table_v78(leaves,current,e))return false;
 const auto* row=trigger_row_v78(current,data_id730_,e);if(!row)return false;
 if(!leaves.load_external_script)return required_trigger_startup_v78("actual object LoadExternalScript38ef60",e);
 return leaves.load_external_script(*this,row->external_script_c.c_str(),"data/scripts/objects/",e);
}
bool CanonicalTriggerObjectV28::init_final(std::string& e){
 if(destruction_started_){e="TriggerObject finalization cannot reuse a native destruction prefix";return false;}
 // Genuine inherited GameObject38cd48; no TriggerObject final override or
 // second outer roll gate exists in this original class inheritance.
 bool eligible{};return initialization_.init_final(eligible,e);
}
bool CanonicalTriggerObjectV28::source_can_activate_v91(bool& out,std::string& e){
 if(destruction_started_){e="Trigger CanActivate cannot reuse native destruction prefix";return false;}
 // Whole original3987ac: count/activated, timer, enabled8a in source order.
 if(count3a8_>=0&&count3a8_<=activated3b4_){out=false;e.clear();return true;}
 if(timer3b8_>0){out=false;e.clear();return true;}
 const auto* enabled=base_.byte(0x8a);if(!enabled){e="Required SAME Trigger source8a";return false;}
 out=*enabled!=0;e.clear();return true;
}
bool CanonicalTriggerObjectV28::source_test_interactive_condition_v91(const TriggerObjectRestoreServicesV91& s,std::string& e){
 if(destruction_started_||!s.owner||!s.local_player){e="Required SAME live TriggerObject/player native authority";return false;}
 TriggerObjectLocalPlayerV91 player;
 if(!s.local_player(0,true,player,e))return false;
 if(!player.receiver||!player.identity||!player.character660){e="Original GetLocalPlayer NULL before660 dereference";return false;}
 const auto id=*player.character660;
 if(id){TriggerObjectSavedCharacterV91 character;if(!s.character_save||!s.character_save(id,character,e))return false;
  if(!character.receiver||character.identity!=id||!character.save14e8){e="Required SAME actual local Character14e8";return false;}
  if(!*character.save14e8){byte784_=0;e.clear();return true;}
  if(!character.save||*character.save14e8!=reinterpret_cast<std::uintptr_t>(character.save.get())){e="Required positive SAME genuine Save14e8 owner";return false;}
  if(!character.save->source_quest_sync_ready14_v3()){byte784_=0;e.clear();return true;}
 }
 // Character NULL reaches Eval. Compiled NULL skips Eval and takes success.
 const auto condition=condition788_;
 if(condition){
  if(!condition_lease_||!s.condition_owner||condition_lease_.owner_before(s.condition_owner)||s.condition_owner.owner_before(condition_lease_)||!s.evaluate_condition){e="Required SAME captured condition788 arena/Eval";return false;}
  bool result{};if(!s.evaluate_condition(condition,result,e))return false;
  if(destruction_started_){e="Native TriggerObject retired during condition evaluation";return false;}
  if(!result){byte784_=0;e.clear();return true;} // retains373 exactly
 }
 byte784_=1; // original399434 BEFORE captured visual/controller call
 const auto* slot=base_.pointer(0x2d8);if(!slot){e="Required actual TriggerObject visual2d8 slot";return false;}
 const auto visual=*slot;
 if(visual){const auto& leaves=services_.startup;
  if(!leaves.owner||!leaves.play_idle_animation){e="Required SAME native TriggerObject Animator38 play";return false;}
  if(!leaves.play_idle_animation(*this,visual,"idle",false,0,0,e))return false;
 }
 if(destruction_started_){e="Native TriggerObject retired during idle animation";return false;}
 auto* disabled=base_.byte(0x373);if(!disabled){e="Required SAME TriggerObject373";return false;}*disabled=0;e.clear();return true;
}
bool CanonicalTriggerObjectV28::update(std::string& e){if(destruction_started_){e="TriggerObject update requires an undestroyed source receiver";return false;}if(!services_.whole_update){e="Required actual TriggerObject Update3994a8";return false;}return services_.whole_update(*this,e);}
bool CanonicalTriggerObjectV28::interact(std::uintptr_t object,std::string& e){if(destruction_started_){e="TriggerObject interaction requires an undestroyed source receiver";return false;}if(!services_.whole_interact){e="Required actual TriggerObject Interact399af0";return false;}return services_.whole_interact(*this,object,e);}
bool CanonicalTriggerObjectV28::set_position(const std::array<float,3>& value,bool destination,std::string& e){if(!initialization_services_.set_position){e="Required SAME TriggerObject SetPosition";return false;}return initialization_services_.set_position(value.data(),destination,e);}
bool CanonicalTriggerObjectV28::destroy(std::string& e){
 e.clear();if(destroyed_){e="TriggerObject completed destruction cannot replay";return false;}
 if(destroying_){e="TriggerObject native destruction cannot reenter";return false;}
 struct Scope {bool& running;~Scope(){running=false;}} scope{destroying_};destroying_=true;destruction_started_=true;
 if(condition788_){
  if(!condition_lease_||!destroy_condition788_){e="Required captured SAME native condition arena for custom788 destruction";return false;}
  // Actual Main runtime destroy is original ConditionListD1 +allocation drop.
  // It can retain a partial Assign/D1 prefix and returnfalse. Never clear the
  // owning slot/pin until that exact native receiver genuinely retires.
  if(!destroy_condition788_(condition788_,e))return false;
  condition788_=0; //399988, only AFTER478eac/CustomFree310440 succeed
 }
 destroy_condition788_={};condition_lease_.reset();
 //399994/99c/99a8/99b4: owned CString allocations release in reverse native
 // declaration order. Keep each host field valid/empty for failed-D1 transport
 // retention; no post-release source lookup or default refill is allowed.
 for(auto i=names_.rbegin();i!=names_.rend();++i)std::string{}.swap(*i);
 if(!services_.owner||!services_.destroy_trigger_base){e="Required actual inherited TriggerD1/network/Zone/GameObjectD2 continuation";return false;}
 // Main's inherited body/journal owns exact native partial cleanup resumption.
 // The completed custom D0 prefix stays cached by its genuinely NULL788.
 if(!services_.destroy_trigger_base(*this,e))return false;
 destroyed_=true;return true;
}
CanonicalClassReceiverV1 CanonicalTriggerObjectV28::factory_receiver(std::shared_ptr<CanonicalTriggerObjectV28> owner,std::shared_ptr<const void> xml){
 auto result=canonical_class_receiver_v1(owner);result.source_lease=std::move(xml);
 result.init_post=[owner](std::string& e){return owner->init_post(e);};
 result.is_game_object=[](bool& value,std::string&){value=true;return true;};
 result.position=[owner](std::array<float,3>& value,std::string&){std::copy_n(owner->base().vector3(0x160),3,value.begin());return true;};
 result.set_position=[owner](const auto& value,bool destination,std::string& e){return owner->set_position(value,destination,e);};return result;
}
}
