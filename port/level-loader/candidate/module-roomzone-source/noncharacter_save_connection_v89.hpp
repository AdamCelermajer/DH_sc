#pragma once
#include <object_save_restore_v3.hpp>
#include <level_savegame_objects_v2.hpp>
#include <canonical_trigger_zone_v22.hpp>
#include <canonical_trigger_object_v28.hpp>
#include <canonical_door_v27.hpp>
#include <canonical_openable_container_v1.hpp>
#include <canonical_destructible_container_v16.hpp>
#include <canonical_light_point_v53.hpp>
#include <canonical_constructor_prefix_v89.hpp>
#include <canonical_level_config_module_v1.hpp>
#include <functional>
#include <memory>
#include <cstring>
#include <exception>
#include <type_traits>
#include <canonical_gameobject_graph_v68.hpp>
namespace dh2::loader {
enum class NonCharacterSaveKindV89 {object_base,gameobject,trigger,door,trigger_object,container,module};
struct NonCharacterSaveFieldsV89 {
 NonCharacterSaveKindV89 kind{NonCharacterSaveKindV89::gameobject};world::ObjectSaveRestoreBorrowV3 base;
 std::int32_t *timer3b8{},*activated3b4{},*door_state3a8{};
 std::uint8_t *reset3b0{},*interactive784{},*module_visited3fc{};
 target_providers::Handle16* module_zone400{};
 std::function<bool(std::int32_t&,std::string&)> container_state394;
 std::function<bool(bool&,std::string&)> death_reset390;
};
struct NonCharacterLevelFlagsV89 {std::shared_ptr<void> owner;std::uint8_t *bytef3{},*bytef4{};};
struct NonCharacterRestoreServicesV89 {
 std::shared_ptr<void> owner; // independent authority; no strong actor/World/Level
 world::ObjectSaveRestoreServicesV3 gameobject; // existing assertion/visual kernel
 std::shared_ptr<void> gameobject_context_owner; // pins actual callback context only
 std::function<bool(std::uintptr_t,std::string&)> validate_current;
 std::function<bool(target_providers::Handle16&,std::uint8_t&,std::string&)> module_room_set_visited;
 // Actual App.GetCurrentLevel, borrowed at reached source points.
 std::function<bool(NonCharacterLevelFlagsV89&,std::string&)> level_flags;
 // Genuine same-Door Opened(false)/Closed(false), not startup approximations.
 std::function<bool(bool,std::string&)> door_opened,door_closed;
 // Same Container::SetState39f3cc and actual derived KeepPhysics virtualdc.
 std::function<bool(std::uint8_t,std::string&)> container_set_state;
 std::function<bool(bool&,std::string&)> container_keep_physics;
 std::function<bool(std::string&)> set_physical_null_false;
 // Same TriggerObject::TestInteractiveCond3993bc / Trigger::CanActivate3987ac.
 std::function<bool(std::string&)> test_interactive_condition;
 std::function<bool(bool&,std::string&)> can_activate;
 // Actual visual2d8->Animator38 virtual20. Integers preserve exact source
 // argument positions; nullable result controls ONLY reached fallback branch.
 std::function<bool(std::uintptr_t,const char*,unsigned,unsigned,unsigned,bool&,std::string&)> play_animation;
};
inline bool gameobject_save_fields_v89(world::CanonicalGameObjectBaseOwnerV1& actual,NonCharacterSaveFieldsV89& out,std::string& e){out={};out.kind=NonCharacterSaveKindV89::gameobject;out.base={actual.identity(),&actual.lifecycle().visible80,actual.byte(0x8a),actual.byte(0xac),actual.byte(0xd0),actual.integer(0x270),actual.pointer(0x2d8),&actual.lifecycle().visible_written};e.clear();return out.base.identity!=0;}
template<class Trigger>bool trigger_save_fields_v89(Trigger& actual,NonCharacterSaveFieldsV89& out,std::string& e){
 if(!gameobject_save_fields_v89(actual.base(),out,e))return false;out.kind=NonCharacterSaveKindV89::trigger;out.timer3b8=actual.source_integer(0x3b8);out.activated3b4=actual.source_integer(0x3b4);out.reset3b0=actual.source_byte(0x3b0);
 if(!out.timer3b8||!out.activated3b4||!out.reset3b0){e="Required SAME Trigger timer/activation/reset cells";return false;}return true;
}
inline bool door_save_fields_v89(world::CanonicalDoorV27& actual,NonCharacterSaveFieldsV89& out,std::string& e){if(!gameobject_save_fields_v89(actual.base(),out,e))return false;out.kind=NonCharacterSaveKindV89::door;out.door_state3a8=&actual.source_state3a8();return true;}
inline bool trigger_object_save_fields_v89(world::CanonicalTriggerObjectV28& actual,NonCharacterSaveFieldsV89& out,std::string& e){if(!trigger_save_fields_v89(actual,out,e))return false;out.kind=NonCharacterSaveKindV89::trigger_object;out.interactive784=actual.source_byte(0x784);if(!out.interactive784){e="Required SAME TriggerObject interactive784 cell";return false;}return true;}
inline bool container_save_fields_v89(world::CanonicalOpenableContainerV1& actual,NonCharacterSaveFieldsV89& out,std::string& e){
 if(!gameobject_save_fields_v89(actual.base(),out,e))return false;out.kind=NonCharacterSaveKindV89::container;
 out.container_state394=[&actual](auto& value,std::string& e){value=actual.fields().state394;e.clear();return true;};out.death_reset390=[&actual](bool& value,std::string& e){value=actual.fields().death_reset;e.clear();return true;};return true;
}
inline bool container_save_fields_v89(world::CanonicalDestructibleContainerV16& actual,NonCharacterSaveFieldsV89& out,std::string& e){
 if(!gameobject_save_fields_v89(actual.base(),out,e))return false;out.kind=NonCharacterSaveKindV89::container;
 out.container_state394=[&actual](auto& value,std::string& e){value=actual.state();e.clear();return true;};out.death_reset390=[&actual](bool& value,std::string& e){std::uint8_t raw{};if(!actual.read_bool(0x390,raw,e))return false;value=raw!=0;return true;};return true;
}
inline bool module_save_fields_v91(world::CanonicalModuleV1& actual,NonCharacterSaveFieldsV89& out,std::string& e){if(!gameobject_save_fields_v89(actual.base(),out,e))return false;out.kind=NonCharacterSaveKindV89::module;out.module_visited3fc=&actual.source_visited3fc_v91();out.module_zone400=&actual.source_zone400_v91();return true;}
inline bool light_save_fields_v89(world::CanonicalLightPointV53& actual,NonCharacterSaveFieldsV89& out,std::string& e){
 out={};out.kind=NonCharacterSaveKindV89::object_base;out.base.identity=actual.identity();out.base.visible80=actual.source_save_byte_v89(0x80,e);out.base.enabled8a=actual.source_save_byte_v89(0x8a,e);out.base.tested_ac=actual.source_save_byte_v89(0xac,e);out.base.tested_d0=actual.source_save_byte_v89(0xd0,e);
 // Source byte80 is genuinely unproduced until real visibility property/write;
 // never manufacture C1/condition flags to make a checkpoint payload succeed.
 e.clear();return out.base.identity!=0; // kernels require only cells actually reached
}
inline bool noncharacter_serialize_v89(level::SavegameStreamV2& s,const NonCharacterSaveFieldsV89& f,std::string& e){
 if(f.kind==NonCharacterSaveKindV89::object_base)return world::object_base_serialize_v3(s,f.base,e);
 if(!world::gameobject_serialize_v3(s,f.base,e))return false;
 if(f.kind==NonCharacterSaveKindV89::trigger||f.kind==NonCharacterSaveKindV89::trigger_object){if(!f.timer3b8||!f.activated3b4){e="Required actual Trigger serialization fields";return false;}return s.write_u32(static_cast<std::uint32_t>(*f.timer3b8),e)&&s.write_u32(static_cast<std::uint32_t>(*f.activated3b4),e);}
 if(f.kind==NonCharacterSaveKindV89::door){if(!f.door_state3a8){e="Required actual Door state3a8";return false;}return s.write_u32(static_cast<std::uint32_t>(*f.door_state3a8),e);}
 if(f.kind==NonCharacterSaveKindV89::module){if(!f.module_visited3fc){e="Required SAME Module3fc serialization cell";return false;}return s.write({f.module_visited3fc,1},e);}
 if(f.kind==NonCharacterSaveKindV89::container){std::int32_t state{};if(!f.container_state394||!f.container_state394(state,e))return false;const auto byte=static_cast<std::uint8_t>(state);return s.write({&byte,1},e);}return true;
}
inline bool noncharacter_deserialize_v89(level::SavegameStreamV2& s,const NonCharacterSaveFieldsV89& f,const NonCharacterRestoreServicesV89& leaves,std::string& e){
 auto flags=[&](NonCharacterLevelFlagsV89& level){if(!leaves.level_flags||!leaves.level_flags(level,e)||!level.owner||!level.bytef3||!level.bytef4){if(e.empty())e="Required actual current Levelf3/f4 source flags";return false;}return true;};
 auto read_int=[&](std::int32_t* actual){if(!actual){e="Required actual deserialize integer cell";return false;}std::uint32_t value;if(!s.read_u32(value,e))return false;std::memcpy(actual,&value,4);return true;};
 auto play=[&](std::uintptr_t visual,const char* name,unsigned arg2,bool& result){if(!visual||!leaves.play_animation){e="Required SAME positive visual/Animator38 native play leaf";return false;}return leaves.play_animation(visual,name,arg2,0,0,result,e);};
 if(f.kind==NonCharacterSaveKindV89::object_base)return world::object_base_deserialize_v3(s,f.base,e);
 if(f.kind==NonCharacterSaveKindV89::container){
  // Original Container reads current Level BEFORE reset390 and before base.
  NonCharacterLevelFlagsV89 level;if(!flags(level))return false;bool reset{};if(!f.death_reset390||!f.death_reset390(reset,e))return false;
  if(!reset&&(*level.bytef3||*level.bytef4))return true; // actual no-read branch
 }
 if(!world::gameobject_deserialize_v3(s,f.base,leaves.gameobject,e))return false;
 if(f.kind==NonCharacterSaveKindV89::module){if(!f.module_visited3fc||!f.module_zone400){e="Required SAME Module3fc/400 restore cells";return false;}if(!s.read(f.module_visited3fc,1,e))return false;if(!leaves.module_room_set_visited){e="Required genuine Module400.GetObject(false)/RoomZone.SetVisited";return false;}return leaves.module_room_set_visited(*f.module_zone400,*f.module_visited3fc,e);}

 if(f.kind==NonCharacterSaveKindV89::trigger||f.kind==NonCharacterSaveKindV89::trigger_object){
  // Fresh App current Level AFTER GameObject base/SyncVisibility callback.
  NonCharacterLevelFlagsV89 level;if(!flags(level))return false;if(!f.reset3b0){e="Required actual Trigger reset3b0";return false;}
  if(!(*level.bytef3&&*f.reset3b0)){if(!read_int(f.timer3b8)||!read_int(f.activated3b4))return false;}
  if(f.kind==NonCharacterSaveKindV89::trigger)return true;
  if(!leaves.test_interactive_condition||!leaves.test_interactive_condition(e)){if(e.empty())e="Required actual TriggerObject TestInteractiveCond";return false;}
  if(!f.base.visual2d8){e="Required SAME TriggerObject visual2d8 cell";return false;}if(!*f.base.visual2d8)return true;
  bool can{};if(!leaves.can_activate||!leaves.can_activate(can,e)){if(e.empty())e="Required genuine Trigger CanActivate";return false;}bool result{};
  if(can){if(!f.interactive784){e="Required SAME TriggerObject interactive784";return false;}return play(*f.base.visual2d8,*f.interactive784?"idle":"idlelocked",1,result);}
  if(!play(*f.base.visual2d8,"idleactive",1,result))return false;if(result)return true;return play(*f.base.visual2d8,"activate",0,result);
 }
 if(f.kind==NonCharacterSaveKindV89::door){if(!read_int(f.door_state3a8))return false;const bool opened=*f.door_state3a8==1||*f.door_state3a8==3;const auto& leaf=opened?leaves.door_opened:leaves.door_closed;if(!leaf){e="Required actual Door Opened(false)/Closed(false) restore body";return false;}return leaf(false,e);}
 if(f.kind==NonCharacterSaveKindV89::container){
  std::uint8_t state{};if(!s.read(&state,1,e))return false;if(!f.base.visual2d8){e="Required SAME Container visual2d8 slot";return false;}if(!*f.base.visual2d8)return true;
  if(!leaves.container_set_state||!leaves.container_set_state(state,e)){if(e.empty())e="Required actual Container SetState source body";return false;}
  std::int32_t current{};if(!f.container_state394||!f.container_state394(current,e))return false;const auto cached_visual=*f.base.visual2d8;
  if(current==2){if(!cached_visual)return true;bool result{};return play(cached_visual,"idle",0,result);}
  if(current!=4)return true;bool keep{};if(!leaves.container_keep_physics||!leaves.container_keep_physics(keep,e)){if(e.empty())e="Required actual derived Container KeepPhysics virtualdc";return false;}
  if(!keep&&(!leaves.set_physical_null_false||!leaves.set_physical_null_false(e))){if(e.empty())e="Required actual SetPhysicalObject(NULL,false)";return false;}
  // Original cached visual is loaded BEFORE KeepPhysics; preserve that read.
  if(!cached_visual)return true;bool result{};return play(cached_visual,"idleactive",0,result);
 }return true;
}
class NonCharacterSaveConnectionV89 final {
 std::function<bool(std::shared_ptr<void>&,NonCharacterSaveFieldsV89&,std::string&)> borrow_;
 std::function<bool(std::shared_ptr<void>&,std::uintptr_t&,std::string&)> identity_borrow_;
 NonCharacterRestoreServicesV89 leaves_;std::uintptr_t identity_{};bool busy_{},failed_{};
 void* query_context_{};bool(*is_character_)(void*,bool&,std::string&){};bool(*is_player_)(void*,bool&,std::string&){};
 bool operation(level::SavegameStreamV2& stream,bool load,std::string& e){
  if(busy_||failed_){e="Sameclass non-Character save/restore cannot replay failed mutation prefix";return false;}busy_=true;struct Guard{bool& busy;~Guard(){busy=false;}}guard{busy_};bool result=false;
  try{std::shared_ptr<void> pin;NonCharacterSaveFieldsV89 f;if(!borrow_||!borrow_(pin,f,e)||!pin||!identity_||f.base.identity!=identity_||!leaves_.validate_current||!leaves_.validate_current(identity_,e)){if(e.empty())e="Required SAME live typed non-Character save fields";failed_=true;return false;}
   result=load?noncharacter_deserialize_v89(stream,f,leaves_,e):noncharacter_serialize_v89(stream,f,e);if(result)result=leaves_.validate_current(identity_,e);
  }catch(const std::exception& ex){e=ex.what();}catch(...){e="Actual non-Character restore leaf threw; source mutation prefix retained";}if(!result)failed_=true;return result;
 }
public:
 NonCharacterSaveConnectionV89(std::function<bool(std::shared_ptr<void>&,NonCharacterSaveFieldsV89&,std::string&)> borrow,
  std::function<bool(std::shared_ptr<void>&,std::uintptr_t&,std::string&)> identity_borrow,NonCharacterRestoreServicesV89 leaves):borrow_(std::move(borrow)),identity_borrow_(std::move(identity_borrow)),leaves_(std::move(leaves)){}
 bool bind_methods(level::LevelSaveObjectBorrowV2& actual,std::string& e){
  if(!actual.identity||!actual.is_character||!actual.is_player||!leaves_.owner||!leaves_.validate_current){e="Required actual OBJS receiver projection and genuine scoped save authority";return false;}const auto id=reinterpret_cast<std::uintptr_t>(actual.identity);
  if(identity_&&identity_!=id){e="Save connection cannot bind another source receiver";return false;}if(actual.context==this)return true;std::shared_ptr<void> pin;std::uintptr_t actual_id{};if(!identity_borrow_||!identity_borrow_(pin,actual_id,e)||!pin||actual_id!=id||!leaves_.validate_current(id,e))return false;
  identity_=id;query_context_=actual.context;is_character_=actual.is_character;is_player_=actual.is_player;actual.context=this;
  actual.is_character=[](void* p,bool& v,std::string& e){auto& self=*static_cast<NonCharacterSaveConnectionV89*>(p);std::shared_ptr<void> pin;std::uintptr_t id{};if(self.failed_||!self.identity_borrow_||!self.identity_borrow_(pin,id,e)||!pin||id!=self.identity_||!self.leaves_.validate_current(id,e))return false;return self.is_character_(self.query_context_,v,e);};
  actual.is_player=[](void* p,bool& v,std::string& e){auto& self=*static_cast<NonCharacterSaveConnectionV89*>(p);std::shared_ptr<void> pin;std::uintptr_t id{};if(self.failed_||!self.identity_borrow_||!self.identity_borrow_(pin,id,e)||!pin||id!=self.identity_||!self.leaves_.validate_current(id,e))return false;return self.is_player_(self.query_context_,v,e);};
  actual.save=[](void* p,level::SavegameStreamV2& stream,std::string& e){return static_cast<NonCharacterSaveConnectionV89*>(p)->operation(stream,false,e);};actual.load=[](void* p,level::SavegameStreamV2& stream,std::string& e){return static_cast<NonCharacterSaveConnectionV89*>(p)->operation(stream,true,e);};e.clear();return true;
 }
 bool failed()const noexcept{return failed_;}
};
// Field projection is a typed SAME-receiver accessor above (or Main's exact
// generic base borrower), not serialization/gameplay callback. Record/runtime
// are pinned only during an operation; no strong containing World/Level cycle.
template<class Record,class Projection>
bool make_noncharacter_record_save_v89(const std::shared_ptr<Record>& actual,Projection project,NonCharacterRestoreServicesV89 leaves,std::shared_ptr<NonCharacterSaveConnectionV89>& out,std::string& e){
 if(out||!actual||!leaves.owner||!leaves.validate_current||(!leaves.owner.owner_before(actual)&&!actual.owner_before(leaves.owner))){e="Required independent non-Character save authority and SAME actual record";return false;}std::weak_ptr<Record> weak=actual;
 auto identity=[weak](std::shared_ptr<void>& pin,std::uintptr_t& id,std::string& e){auto same=weak.lock();if(!same||same->constructor_state!=CanonicalConstructorStateV89::completed||!same->owner){e="Required SAME completed actual C1 record for OBJS identity";return false;}pin=std::shared_ptr<void>(same,same->owner.get());using Receiver=typename decltype(same->owner)::element_type;if constexpr(std::is_same_v<Receiver,world::CanonicalLightPointV53>)id=same->owner->identity();else id=same->owner->base().identity();return id!=0;};
 out=std::make_shared<NonCharacterSaveConnectionV89>([weak,project](std::shared_ptr<void>& pin,NonCharacterSaveFieldsV89& fields,std::string& e){auto same=weak.lock();if(!same||same->constructor_state!=CanonicalConstructorStateV89::completed||!same->owner){e="Required SAME completed actual C1 record for OBJS projection";return false;}pin=std::shared_ptr<void>(same,same->owner.get());return project(*same->owner,fields,e);},std::move(identity),std::move(leaves));e.clear();return true;
}
template<class Receiver,class Projection>
bool make_noncharacter_receiver_save_v89(const std::shared_ptr<Receiver>& actual,Projection project,NonCharacterRestoreServicesV89 leaves,std::shared_ptr<NonCharacterSaveConnectionV89>& out,std::string& e){
 if(out||!actual||!leaves.owner||!leaves.validate_current||(!leaves.owner.owner_before(actual)&&!actual.owner_before(leaves.owner))){e="Required independent save authority/SAME actual post-C1 receiver";return false;}std::weak_ptr<Receiver> weak=actual;
 auto identity=[weak](std::shared_ptr<void>& pin,std::uintptr_t& id,std::string& e){auto same=weak.lock();if(!same){e="Required SAME live receiver save identity";return false;}pin=same;if constexpr(std::is_same_v<Receiver,world::CanonicalLightPointV53>)id=same->identity();else id=same->base().identity();return id!=0;};
 out=std::make_shared<NonCharacterSaveConnectionV89>([weak,project](std::shared_ptr<void>& pin,NonCharacterSaveFieldsV89& f,std::string& e){auto same=weak.lock();if(!same){e="Required SAME live non-Character receiver projection";return false;}pin=same;return project(*same,f,e);},std::move(identity),std::move(leaves));e.clear();return true;
}
// Existing SAME generic/scenery graph base borrower for RoomZone/ColBox/Floor
// and other proven GameObject inherited serializers; no new generic receiver.
inline bool make_noncharacter_base_save_v89(world::CanonicalBaseBorrowV68 borrow,NonCharacterRestoreServicesV89 leaves,std::shared_ptr<NonCharacterSaveConnectionV89>& out,std::string& e){
 if(out||!borrow||!leaves.owner||!leaves.validate_current){e="Required existing actual base borrower and genuine save authority";return false;}
 auto identity=[borrow](std::shared_ptr<void>& pin,std::uintptr_t& id,std::string& e){world::CanonicalGameObjectBaseOwnerV1* base{};if(!borrow(pin,base,e)||!pin||!base)return false;id=base->identity();return id!=0;};
 out=std::make_shared<NonCharacterSaveConnectionV89>([borrow](std::shared_ptr<void>& pin,NonCharacterSaveFieldsV89& f,std::string& e){world::CanonicalGameObjectBaseOwnerV1* base{};if(!borrow(pin,base,e)||!pin||!base)return false;return gameobject_save_fields_v89(*base,f,e);},std::move(identity),std::move(leaves));e.clear();return true;
}
}
