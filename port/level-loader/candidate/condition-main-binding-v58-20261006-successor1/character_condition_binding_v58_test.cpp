#include <retained_character_actor_v1.hpp>
#include <character_condition_binding_v58.hpp>
#include "canonical_condition_receivers_v55.hpp"
#include "canonical_point3d_globals_v1.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh2;
static void check(bool ok,const std::string& error){if(!ok)throw std::runtime_error(error);}
static std::vector<std::uint8_t> read(const char* file){std::ifstream f(file,std::ios::binary|std::ios::ate);check(bool(f),"original condition fixture unavailable");std::vector<std::uint8_t> out(f.tellg());f.seekg(0);f.read(reinterpret_cast<char*>(out.data()),out.size());check(bool(f),"short original condition fixture");return out;}
int main(int argc,char** argv){
 check(argc==3,"expected original condition data and names");std::string error;
 auto world_pin=std::make_shared<unsigned>(0);
 auto adopted=std::make_shared<character::RetainedCharacterActorV1>(0x580001,world_pin,"Character",character::RetainedCharacterConstructionV7::existing_actor_adoption);
 check(!adopted->source_bool_field(0x8a),"adoption fabricated enabled8a");
 character::CharacterEnableConditionFieldsV58 unchanged;unchanged.identity=0xfeed;
 check(!adopted->enable_condition_fields_v58(adopted,unchanged,error)&&unchanged.identity==0xfeed&&!adopted->source_bool_field(0x8a),"adoption borrow fabricated or overwrote fields");
 auto actor=std::make_shared<character::RetainedCharacterActorV1>(0x580002,world_pin,"Character",character::RetainedCharacterConstructionV7::fresh_canonical);
 check(actor->source_bool_field(0x8a)&&*actor->source_bool_field(0x8a)==1,"fresh C2 enabled8a producer missing");
 check(!actor->enable_condition_fields_v58(actor,unchanged,error)&&unchanged.identity==0xfeed,"missing actual f1 producer accepted");
 world::CanonicalPropertySourceServicesV1 source;source.position_rotation_default=&world::canonical_vec3_origin_v1();source.load_template_assertion=[](void*,std::string&){return true;};
 world::CanonicalPropertyMapV1 map(source);auto property=actor->properties();check(map.init_properties(property,error)&&map.load_defaults(property,error),error);
 auto object=std::make_shared<character::ScriptCharacterObject>(actor->shared_handle().cached,"bounded-v58-actor",std::make_shared<data::PropertyState>(),std::make_shared<data::CombatActorState>(),std::array<float,3>{});actor->object=object;
 character::CharacterParentInitFieldsV48 parent;check(actor->parent_init_fields_v48(actor,parent,error),error);
 character::CharacterEnableConditionFieldsV58 fields;check(actor->enable_condition_fields_v58(actor,fields,error),error);
 check(fields.receiver_lease.get()==actor.get()&&fields.identity==object->identity&&fields.enabled8a==actor->source_bool_field(0x8a)&&fields.disable_f1==actor->source_bool_field(0xf1),"same actor/PropertyMap bool alias failed");
 check(fields.minimum_difficulty_ec==parent.min_difficulty_ec&&fields.condition_a8==parent.conditions[0].compiled&&fields.tested_ac==parent.conditions[0].tested,"parent/enable scalar alias failed");
 auto enable=fields.borrow();check(enable.enabled8a==fields.enabled8a&&enable.condition_a8==fields.condition_a8&&enable.tested_ac==fields.tested_ac,"V2 conversion copied cells");
 *fields.enabled8a=0;check(*actor->source_bool_field(0x8a)==0,"borrow mutation did not reach same actual field");*fields.enabled8a=1;
 *parent.min_difficulty_ec=2;check(*fields.minimum_difficulty_ec==2,"difficulty alias copied value");
 check(!actor->enable_condition_fields_v58(world_pin,unchanged,error)&&unchanged.identity==0xfeed,"wrong lease accepted");
 auto rows=read(argv[1]),names=read(argv[2]);std::shared_ptr<const world::ConditionTablesV55> tables;check(world::ConditionTablesV55::load(rows.data(),rows.size(),names.data(),names.size(),tables,error),error);
 auto classes=std::make_shared<world::CanonicalConditionFactoryV55>(world_pin);auto lists=std::make_shared<world::ConditionListOwnerV55>(tables,classes->factories());auto concrete=lists->services();
 check(map.set_property(property,"activate_cond","RENE_FOLLOW",error),error);
 world::GameObjectInitializationServicesV1 services;services.owner=world_pin;
 check(loader::bind_character_conditions_v58(parent,lists,services,error),error);
 check(lists->retained_list_allocations()==0&&!*fields.condition_a8,"binding compiled early");
 auto malformed=parent;malformed.conditions[0].compiled=nullptr;auto old_callback=services.condition_init;
 check(!loader::bind_character_conditions_v58(malformed,lists,services,error)&&bool(services.condition_init),"invalid binding replaced callback");
 std::weak_ptr<character::RetainedCharacterActorV1> actor_weak=actor;std::weak_ptr<world::ConditionListOwnerV55> list_weak=lists;
 parent={};fields={};actor.reset();object.reset();lists.reset();classes.reset();tables.reset();
 check(!actor_weak.expired()&&!list_weak.expired(),"callback did not pin same owners");
 check(services.condition_init(0x8c,error),error);auto pinned_actor=actor_weak.lock();auto pinned_lists=list_weak.lock();character::CharacterEnableConditionFieldsV58 after;check(pinned_actor->enable_condition_fields_v58(pinned_actor,after,error),error);
 check(*after.condition_a8&&*after.tested_ac==0&&pinned_lists->fields(*after.condition_a8),"actual C1/Assign not retained in same cells");
 character::CharacterParentInitFieldsV48 after_parent;check(pinned_actor->parent_init_fields_v48(pinned_actor,after_parent,error),error);auto backing=world::character_gameobject_fields_v51(after_parent);
 check(world::condition_data_clear_borrow_v51(backing,0x8c,concrete,error)&&!*after.condition_a8&&pinned_lists->retained_list_allocations()==0,"same cell Clear failed");
 // This is a bounded callback/borrow component test. It does not invoke
 // Character ordered InitPost, Condition Eval, enable events or save/quests.
 std::cout<<"PASS fresh8a=1 adoption_unchanged=1 same_actor_alias=1 f1_PropertyMap=1 same_min_difficulty_condition_tested=1 no_bind_compile=1 callback_owner_pins=1 actual_C1_Assign_Clear=1 Main_Eval_called=0 ordered_InitPost_called=0\n";
}