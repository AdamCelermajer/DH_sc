#include "canonical_object_factory_v1.hpp"
#include <cstring>
namespace dh2::world {
const std::array<CanonicalFactoryEntryV1,33>& canonical_factories_v1()noexcept{
 static const std::array<CanonicalFactoryEntryV1,33> entries{{
 {"Decor",0x3410fc},{"AnimatedDecor",0x342600},{"Dummy",0x3410a4},{"Floor",0x34104c},{"LightPoint",0x34115c},{"ColBox",0x340fe0},{"Billboard",0x340fbc},{"Module",0x340f98},{"Block",0x340f98},{"Character",0x340800},{"Player",0x340800},{"RoomZone",0x340f74},{"QuestMoveInZone",0x340f50},{"CheckpointZone",0x340f2c},{"TriggerObject",0x340f0c},{"TriggerPlate",0x340ee8},{"TriggerZone",0x340ec4},{"TriggerZoneExitLevel",0x340ea0},{"TriggerTrap",0x340e7c},{"TimerTrap",0x340e58},{"ProjectileTrap",0x340e34},{"Door",0x340824},{"SpawnPoint",0x340e10},{"SpawnSpot",0x340dec},{"LiftableObject",0x340dc8},{"OpenableContainer",0x340da4},{"SlotContainer",0x340d80},{"DestructibleContainer",0x340d5c},{"Item",0x340d38},{"Projectile",0x340d14},{"LaserTypeProjectile",0x340cf0},{"SoundEmitter",0x340ccc},{"LevelConfig",0x340ca8}}};return entries;
}
bool CanonicalObjectFactoryAttemptV1::execute(CanonicalObjectManagerV1& manager,const CanonicalClassServicesV1& s,std::string& e){
 if(attempted_){e="source factory attempt already delivered; release failed candidate explicitly";return false;}attempted_=true;
 auto fail=[&](const char* service){if(e.empty())e=std::string("required source factory provider: ")+service;prefix_=stage_;stage_=CanonicalFactoryStageV1::failed;return false;};
 if(!request_.source_lease||!request_.attribute)return fail("retained XML borrow");
 const char* type=request_.attribute(request_.source_context,request_.element,"gametype");const char* name=request_.attribute(request_.source_context,request_.element,"name");
 if(!type||!name||(request_.type_filter&&std::strcmp(request_.type_filter,type))){stage_=CanonicalFactoryStageV1::source_skip;return true;}
 const CanonicalFactoryEntryV1* entry{};for(auto& v:canonical_factories_v1())if(!std::strcmp(v.name,type)){entry=&v;break;}
 if(!entry){if(!s.unknown_type_debug||!s.unknown_type_debug(s.context,type,e))return fail("unknown-type Debug load/query");stage_=CanonicalFactoryStageV1::source_skip;return true;}
 CanonicalObjectBorrowV1 actor;if(!s.construct||!s.construct(s.context,*entry,request_,actor,e))return fail("registered constructor");
 if(!actor.identity||!actor.lease)return fail("actual constructor object");stage_=CanonicalFactoryStageV1::constructed;
 std::string special;auto room=request_.runtime_module_id;if(!std::strcmp(name,"_prim_PlayerLight")){special=std::string(name)+"_S";name=special.c_str();room=-1;}
 if(!manager.add(std::move(actor),name,type,room,true,handle_,e))return fail("ObjectManager Add");stage_=CanonicalFactoryStageV1::registered;
 const auto* actual=manager.object(handle_.key);if(!actual)return fail("GetObject(true)");
 if(!s.init_properties||!s.init_properties(s.context,*actual,e))return fail("InitProperties");stage_=CanonicalFactoryStageV1::properties;
 if(auto* value=request_.attribute(request_.source_context,request_.element,"template")){if(!s.set_template||!s.set_template(s.context,*actual,value,e))return fail("SetTemplate");stage_=CanonicalFactoryStageV1::template_set;}
 if(!s.load_defaults||!s.load_defaults(s.context,*actual,e))return fail("LoadDefaultProperties");stage_=CanonicalFactoryStageV1::defaults;
 if(!s.load_overrides||!s.load_overrides(s.context,*actual,request_,e))return fail("LoadOverridesFromXML");stage_=CanonicalFactoryStageV1::overrides;
 if(!std::strcmp(type,"LevelConfig")){if(!s.init_post||!s.init_post(s.context,*actual,e))return fail("LevelConfig early InitPost");stage_=CanonicalFactoryStageV1::early_init_post;}
 bool game{};if(!s.is_game_object||!s.is_game_object(s.context,*actual,game,e))return fail("IsGameObject virtual20");
 if(game){std::array<float,3> position;if(!s.position||!s.position(s.context,*actual,position,e))return fail("actual GameObject position160");
  // Original performs y, z, x additions then one source SetPosition(...,true).
  position[1]+=request_.module_offset[1];position[2]+=request_.module_offset[2];position[0]+=request_.module_offset[0];
  if(!s.set_position||!s.set_position(s.context,*actual,position,true,e))return fail("SetPosition393db4");stage_=CanonicalFactoryStageV1::offset;
 }
 stage_=CanonicalFactoryStageV1::complete;prefix_=stage_;return true;
}
}
