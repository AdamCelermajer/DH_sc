#include "canonical_door_v27.hpp"
#include <algorithm>
#include <utility>
#include <cstring>
#include <limits>
#include "game_object_spawn_probability_v1.hpp"
namespace dh2::world {
CanonicalDoorV27::CanonicalDoorV27(std::shared_ptr<void> world,actor::RuntimeState& runtime,
 GameObjectInitializationServicesV1 initialization,DoorServicesV27 services):
 base_(reinterpret_cast<std::uintptr_t>(this),2,std::move(world),runtime),
 initialization_services_(std::move(initialization)),initialization_(base_,initialization_services_),services_(std::move(services)){
 base_.lifecycle().static84=1;*base_.byte(0x28)=1;*base_.byte(0xf8)=3;
 *base_.pointer(0x100)=reinterpret_cast<std::uintptr_t>(&network_[0]);
 *base_.pointer(0x104)=reinterpret_cast<std::uintptr_t>(&network_[1]);
}
CanonicalPropertyActorV1 CanonicalDoorV27::properties()noexcept{
 auto result=canonical_family_fields_v15(*this);
 result.fields.write_vector3=[](void* p,std::uint32_t o,const auto& v,std::string& e){return static_cast<CanonicalDoorV27*>(p)->write_vector3(o,v,e);};return result;
}
bool CanonicalDoorV27::read_bool(std::uint32_t o,std::uint8_t& v,std::string& e){
 if(o==0x3a4){v=opened3a4_;return true;}if(o==0x3a5){v=collision3a5_;return true;}
 auto f=base_.properties().fields;return f.read_bool(f.context,o,v,e);
}
bool CanonicalDoorV27::write_bool(std::uint32_t o,std::uint8_t v,std::string& e){
 if(o==0x3a4){opened3a4_=v;return true;}if(o==0x3a5){collision3a5_=v;return true;}
 auto f=base_.properties().fields;return f.write_bool(f.context,o,v,e);
}
bool CanonicalDoorV27::write_int(std::uint32_t o,std::int32_t v,std::string& e){auto f=base_.properties().fields;return f.write_int(f.context,o,v,e);}
bool CanonicalDoorV27::write_string(std::uint32_t o,const std::string& v,std::string& e){
 if(o==0x388){data388_=v;return true;}auto f=base_.properties().fields;return f.write_string(f.context,o,v,e);
}
bool CanonicalDoorV27::write_vector3(std::uint32_t o,const std::array<float,3>& v,std::string& e){
 if(o==0x374){dimensions374_=v;return true;}auto f=base_.properties().fields;return f.write_vector3(f.context,o,v,e);
}
namespace {
bool required_door_v77(const char* name,std::string& e){e=std::string("Required actual Door startup ")+name;return false;}
bool borrow_door_table_v77(const DoorStartupServicesV77& services,DoorTableBorrowV77& out,std::string& e){
 if(!services.owner||!services.borrow_tables)return required_door_v77("genuine Arrays::Doors/GameObjectDict borrow",e);
 DoorTableBorrowV77 same;if(!services.borrow_tables(same,e))return false;
 if(!same.receiver||!same.names||!same.rows||same.names->size()!=same.rows->size()||same.names->size()>std::size_t(std::numeric_limits<std::int32_t>::max()))
  return required_door_v77("SAME actual Door declaration names/rows",e);
 out=std::move(same);return true;
}
const DoorDeclarationRowV77* door_row_v77(const DoorTableBorrowV77& table,std::int32_t id,std::string& e){
 if(id<0||std::size_t(id)>=table.rows->size()){required_door_v77("actual source3a0 declaration row",e);return nullptr;}
 return &(*table.rows)[id];
}
}
bool CanonicalDoorV27::startup_spawn_gate_v77(bool& eligible,std::string& e){
 eligible=false;std::int32_t roll{};
 if(!initialization_services_.owner||!initialization_services_.check_spawn_probability)return required_door_v77("CheckSpawnProbability38bd64",e);
 if(!initialization_services_.check_spawn_probability(roll,e))return false;
 const auto* probability=base_.integer(0x274);if(!probability)return required_door_v77("SAME freshly read probability274",e);
 eligible=roll<*probability;return true;
}
bool CanonicalDoorV27::init_post(std::string& e){
 e.clear();bool eligible{};if(!startup_spawn_gate_v77(eligible,e))return false;if(!eligible)return true;
 const auto& leaves=services_.startup;DoorTableBorrowV77 table;if(!borrow_door_table_v77(leaves,table,e))return false;
 std::int32_t found=-1;
 for(std::size_t i=0;i<table.names->size();++i)if(!std::strcmp(data388_.c_str(),(*table.names)[i].c_str())){found=static_cast<std::int32_t>(i);break;}
 table_id3a0_=found; // Source stores miss -1 too, before Zone/visual work.
 if(found!=-1){const auto* row=door_row_v77(table,found,e);if(!row)return false;
  const auto visual_id=row->visual14;
  if(visual_id!=-1){
   if(!table.objects)return required_door_v77("SAME GameObjectDict owner",e);
   const std::string* model{};if(!table.objects->file(visual_id,model,e))return false;
   if(!model)return required_door_v77("actual GameObjectDict visual14 CString",e);
   auto* slot=base_.string(0x290);if(!slot)return required_door_v77("SAME model290",e);*slot=*model;
  }
 }
 if(!zone_init_post_v76(base_,initialization_,initialization_services_,dimensions374_,physical380_,trigger381_,colzone384_,colzone_lease_,leaves.zone,e))return false;
 bool meets{};if(!game_object_meet_condition_v1(meets,e))return false;
 if(!meets){if(!leaves.owner||!leaves.source_delete)return required_door_v77("selected virtual40 Delete",e);return leaves.source_delete(*this,e);}
 const auto* visual_slot=base_.pointer(0x2d8);if(!visual_slot)return required_door_v77("SAME visual2d8",e);
 const auto visual=*visual_slot;
 if(visual){
  if(!leaves.owner||!leaves.register_animation_callbacks)return required_door_v77("actual controller38 Door callbacks",e);
  if(!leaves.register_animation_callbacks(*this,visual,e))return false;
  // Original uses the captured same visual after callback, not a new lookup.
  if(!leaves.apply_mesh_box)return required_door_v77("actual captured Visual ApplyMeshBox470a54",e);
  if(!leaves.apply_mesh_box(*this,visual,e))return false;
 }
 if(collision3a5_){if(!leaves.owner||!leaves.create_door_physical)return required_door_v77("actual Door physical constructor/assignment",e);if(!leaves.create_door_physical(*this,e))return false;}
 if(!table_id3a0_)return required_door_v77("produced source3a0",e);
 if(*table_id3a0_==-1)return true;
 if(!leaves.owner||!leaves.borrow_sound_manager)return required_door_v77("actual nullable VoxSoundManager snapshot",e);
 StartupSoundManagerBorrowV87 sound;if(!leaves.borrow_sound_manager(sound,e))return false;
 if(!sound.valid_nullable())return required_door_v77("actual captured SoundManager owner/method",e);
 if(!sound)return true;
 DoorTableBorrowV77 first;if(!borrow_door_table_v77(leaves,first,e))return false;
 const auto* row=door_row_v77(first,*table_id3a0_,e);if(!row)return false;
 if(!sound.load_sound(row->sound10,e))return false; // source10 first
 // Original reloads3a0 and table base after the first sound callback.
 DoorTableBorrowV77 second;if(!borrow_door_table_v77(leaves,second,e))return false;
 if(!table_id3a0_)return required_door_v77("retained source3a0 after LoadSound",e);
 row=door_row_v77(second,*table_id3a0_,e);if(!row)return false;
 return sound.load_sound(row->sound_c,e); // sourcec second, even -1 UID
}
bool CanonicalDoorV27::startup_state_v77(bool opened,std::string& e){return source_transition_v91(opened,false,e);}
bool CanonicalDoorV27::source_transition_v91(bool opened,bool skip_animation,std::string& e){
 if(destroyed_){e="Door transition cannot use a native destruction prefix";return false;}
 const auto& leaves=services_.startup;const auto* visual_slot=base_.pointer(0x2d8);const auto* physical_slot=base_.pointer(0x2dc);
 if(!visual_slot||!physical_slot)return required_door_v77("SAME native visual/physical slots",e);
 const auto visual=*visual_slot;state3a8_=opened?1:0;const auto physical=*physical_slot;
 if(visual&&!skip_animation){if(!leaves.owner||!leaves.play_idle_animation)return required_door_v77("actual IdleOpened/IdleClosed controller",e);
  if(!leaves.play_idle_animation(*this,visual,opened?"IdleOpened":"IdleClosed",true,false,false,e))return false;
 }
 if(physical){if(!leaves.owner||!leaves.physical_filter)return required_door_v77("captured physical enable/disable filter",e);
  if(!leaves.physical_filter(*this,physical,!opened,e))return false;
 }
 if(!leaves.owner||!leaves.flag_floor_dead_end)return required_door_v77("PFWorld FlagFloorAsDeadEnd5252ec",e);
 if(!leaves.flag_floor_dead_end(*this,!opened,e))return false;
 auto* disabled=base_.byte(0x373);if(!disabled)return required_door_v77("SAME disabled373",e);*disabled=opened?1:0;return true;
}
bool CanonicalDoorV27::init_final(std::string& e){
 e.clear();bool eligible{};if(!startup_spawn_gate_v77(eligible,e))return false;if(!eligible)return true;
 bool base_eligible{};if(!initialization_.init_final(base_eligible,e))return false;
 bool meets{};if(!game_object_meet_condition_v1(meets,e))return false;if(!meets)return true;
 // Original ignores GameObject eligibility and tails Closed(false)/Opened(false).
 return startup_state_v77(opened3a4_!=0,e);
}
bool CanonicalDoorV27::set_position(const std::array<float,3>& p,bool destination,std::string& e){
 if(!initialization_services_.set_position){e="Required SAME Door SetPosition";return false;}return initialization_services_.set_position(p.data(),destination,e);
}
bool CanonicalDoorV27::destroy(std::string& e){
 if(destroyed_){e="Door destruction cannot replay";return false;}
 if(!services_.whole_destroy){e="Required actual Door/NetStruct/Zone/GameObject destruction";return false;}
 destroyed_=true;return services_.whole_destroy(*this,e);
}
CanonicalClassReceiverV1 CanonicalDoorV27::factory_receiver(std::shared_ptr<CanonicalDoorV27> owner,std::shared_ptr<const void> xml){
 auto result=canonical_class_receiver_v1(owner);result.source_lease=std::move(xml);
 result.init_post=[owner](std::string& e){return owner->init_post(e);};
 result.is_game_object=[](bool& value,std::string&){value=true;return true;};
 result.position=[owner](std::array<float,3>& value,std::string&){std::copy_n(owner->base().vector3(0x160),3,value.begin());return true;};
 result.set_position=[owner](const auto& value,bool destination,std::string& e){return owner->set_position(value,destination,e);};return result;
}
}
