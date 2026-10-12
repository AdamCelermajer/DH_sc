#include "game_object_initialization_owner_v1.hpp"
#include "game_object_initialization_fields_v51.hpp"
#include "game_object_initialization_kernel_v51.hpp"
#include "condition_data_kernel_v51.hpp"
#include "source_device_pipeline_v51.hpp"
#include "character_set_position_v7.hpp"
#include "game_object_spawn_probability_v1.hpp"
#include "canonical_point3d_globals_v1.hpp"
#include <cassert>
#include <iostream>
using namespace dh2;using namespace dh2::world;
struct Fixture {
 std::shared_ptr<int> lease=std::make_shared<int>(1);actor::RuntimeState runtime{};CanonicalGameObjectBaseOwnerV1 base{42,7,lease,runtime};CanonicalPropertyMapV1 properties{{nullptr,&canonical_vec3_origin_v1(),nullptr}};std::string error;std::array<std::uint8_t,3> device_flags{{1,0,0}};unsigned loads{},syncs{};
 Fixture(){base.class_name20()="GameObject";auto p=base.properties();assert(properties.init_properties(p,error)&&properties.load_defaults(p,error));assert(properties.set_property(p,"scale","0,-0.00005,-3",error)&&properties.set_property(p,"rotation","90,180,-90",error)&&properties.set_property(p,"position","10,20,30",error));*base.integer(0x270)=-2;*base.pointer(0x2dc)=123;}
 GameObjectInitializationServicesV1 services(){GameObjectInitializationServicesV1 s;s.owner=lease;
  s.condition_init=[this](auto offset,auto& e){auto fields=canonical_gameobject_fields_v51(base,lease);return condition_data_init_borrow_v51(fields,offset,ConditionDataInitServicesV3{},e);};
  s.check_spawn_probability=[this](auto& roll,auto& e){GameObjectSpawnProbabilityBorrowV1 b;b.cached_roll270=base.integer(0x270);b.probability274=base.integer(0x274);b.handle_as_player_character=[this](bool& v,std::string& error){auto actual=base.canonical(lease);std::uintptr_t character{};if(!actual.as_character(actual.context,character,error))return false;v=character!=0;return true;};std::int32_t probability;return game_object_check_spawn_probability_v1(b,roll,probability,e);};
  s.set_position=[this](const float* p,bool destination,std::string& e){character::CharacterPositionBorrowV7 b;b.receiver=lease;b.identity=base.identity();b.position160=base.vector3(0x160);b.relative144=base.relative_aabb144();b.absolute12c=base.absolute_aabb12c();b.destination1a8=base.vector3(0x1a8);b.physical2dc=base.pointer(0x2dc);b.attached2e0=base.pointer(0x2e0);b.visual2d8=base.pointer(0x2d8);character::CharacterPositionResultV7 result;return character::character_set_position_v7(result,b,p,destination,{},e);};
  s.device_high_performance=[this](bool& high,std::string& e){SourceDevicePipelineBorrowV51 d;d.actual_owner=lease;for(unsigned i=0;i<3;++i)d.disable_bytes[i]=&device_flags[i];return source_device_high_performance_v51(d,high,e);};
  s.load_visual=[this](auto& e){++loads;e="Required actual main LoadVisual owner";return false;};s.visual_sync=[this](auto,auto& e){++syncs;e="Required actual main Visual Sync owner";return false;};return s;
 }
};
int main(){unsigned completed{},required{};
 {Fixture f;*f.base.integer(0x270)=42;*f.base.integer(0x274)=0;GameObjectInitializationOwnerV1 wrapper(f.base,f.services());bool eligible=true;assert(wrapper.init_post(eligible,f.error)&&!eligible&&*f.base.pointer(0x2dc)==123&&f.loads==0);++completed;}
 for(auto sound:std::vector<std::string>{"","idle","absent"}){Fixture f;*f.base.byte(0x10c)=1;*f.base.byte(0x15c)=1;*f.base.byte(0x28)=1;auto p=f.base.properties();assert(f.properties.set_property(p,"idle_sound",sound.c_str(),f.error));std::vector<std::string> names{"none","idle","other"};auto s=f.services();s.sound_names=&names;GameObjectInitializationOwnerV1 wrapper(f.base,s);bool eligible=false;assert(wrapper.init_post(eligible,f.error)&&eligible&&*f.base.pointer(0x2dc)==0&&*f.base.pointer(0x2d8)==0&&*f.base.byte(0x28)==0&&f.loads==0&&f.syncs==0);assert(f.base.vector3(0x120)[0]==1&&f.base.vector3(0x120)[1]==1&&f.base.vector3(0x120)[2]==-3&&f.base.relative_aabb144()[2]==300&&f.base.absolute_aabb12c()[2]==330&&f.base.vector3(0x1a8)[2]==30);assert(*f.base.integer(0x370)==(sound=="idle"?1:-1));++completed;}
 {Fixture f;GameObjectInitializationOwnerV1 wrapper(f.base,f.services());bool eligible;assert(!wrapper.init_post(eligible,f.error)&&eligible&&f.error=="Required actual main LoadVisual owner"&&f.loads==1&&f.syncs==0&&*f.base.pointer(0x2dc)==0);++required;}
 {Fixture f;f.device_flags={0,0,0};*f.base.byte(0x10c)=1;GameObjectInitializationOwnerV1 wrapper(f.base,f.services());bool eligible;assert(!wrapper.init_post(eligible,f.error)&&f.error.find("renderer programmable-pipeline mask")!=std::string::npos&&f.loads==0&&*f.base.pointer(0x2dc)==0);++required;}
 {Fixture f;auto fields=canonical_gameobject_fields_v51(f.base,f.lease);std::string error;SourceDevicePipelineBorrowV51 d;d.actual_owner=f.lease;d.disable_bytes={&f.device_flags[0],nullptr,nullptr};bool value=true;assert(source_device_high_performance_v51(d,value,error)&&!value);f.device_flags[0]=0;value=true;assert(!source_device_high_performance_v51(d,value,error)&&value);}
 {struct Halfword {std::int16_t sound{-1};std::uint16_t upper{0x6677};} words;character::CharacterParentInitFieldsV48 actual;actual.idle_sound370=&words.sound;auto fields=character_gameobject_fields_v51(actual);std::string error;assert(fields.store_idle_sound16(65535,error)&&words.sound==-1&&words.upper==0x6677);assert(fields.store_idle_sound16(32768,error)&&words.sound==-32768&&words.upper==0x6677);}
 std::cout<<"PASS shared_generic_control_original_cases="<<completed<<" required_actual_producer_stops="<<required<<" genuine_ConditionData_empty_Invalid=1 actual_cached_spawn=1 whole_SetPosition_kernel=1 source_device_branch=1 Character_halfword_upper_guard=1 no_successful_visual_callbacks=1 controlled_inputs_not_firstSWAMP=1\n";
}
