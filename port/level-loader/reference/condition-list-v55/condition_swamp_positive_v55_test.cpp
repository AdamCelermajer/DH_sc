#include "condition_list_owner_v55.hpp"
#include "canonical_condition_receivers_v55.hpp"
#include "condition_data_kernel_v51.hpp"
#include "game_object_initialization_fields_v51.hpp"
#include "canonical_character_family_v4.hpp"
#include "application_spawn_random_owner_v4.hpp"
#include "canonical_point3d_globals_v1.hpp"
#include <zip_asset_pack_v1.hpp>
#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <stdexcept>
using namespace dh2;using namespace dh2::world;
static void require(bool ok,const std::string& e){if(!ok)throw std::runtime_error(e);}
#include "character_factory_setup_v55.inc"
struct Placement {std::map<std::string,std::string> attrs;};
struct ControlledCondition {std::uintptr_t stub{};}; // TEST constructor fixture only, never evaluation
static std::vector<std::map<std::string,std::string>> source_conditions_v55(){return {
{{"name","_prim_NPC_PriestGood"},{"gametype","Character"},{"activate_cond","RENE_FOLLOW"}},
{{"name","_prim_NPC"},{"gametype","Character"}},
{{"name","_prim_NPC01_001"},{"gametype","Character"}},
{{"name","_prim_Faery"},{"gametype","Character"}},
{{"name","_prim_ActorTroll"},{"gametype","Character"},{"activate_cond","GameStartOnly"}},
{{"name","_prim_ActorPrisoner"},{"gametype","Character"},{"activate_cond","GameStartOnly"}},
{{"name","_prim_MothTemplate_13"},{"gametype","Character"},{"activate_cond","Invalid"},{"deactivate_cond","Invalid"}},
{{"name","_prim_Monster_3_05_313_356_14_33_13_179_13_38"},{"gametype","Character"}},
{{"name","_prim_Monster_LizManIntro1"},{"gametype","Character"}},
{{"name","_prim_Monster_LizManIntro2"},{"gametype","Character"}},
{{"name","_prim_Monster_FakeMoth_01"},{"gametype","Character"}},
{{"name","_prim_Monster_FakeMoth_02"},{"gametype","Character"}},
{{"name","_prim_LizTemplate_01"},{"gametype","Character"}},
{{"name","_prim_LizTemplate_02"},{"gametype","Character"}},
{{"name","_prim_LizTemplate_03"},{"gametype","Character"}},
{{"name","_prim_MothTemplate_01"},{"gametype","Character"}},
{{"name","_prim_MothTemplate_02"},{"gametype","Character"}},
{{"name","_prim_Monster_54_chest2"},{"gametype","Character"}},
{{"name","_prim_Monster_53_03_001"},{"gametype","Character"}},
{{"name","_prim_Monster_54_chest1"},{"gametype","Character"}},
{{"name","_prim_MothTemplate_05"},{"gametype","Character"}},
{{"name","_prim_LizTemplate_05"},{"gametype","Character"}},
{{"name","_prim_LizTemplate_06"},{"gametype","Character"}},
{{"name","_prim_Monster_04_324"},{"gametype","Character"}},
{{"name","_prim_Monster_05_325"},{"gametype","Character"}},
{{"name","_prim_Monster_08_329"},{"gametype","Character"}},
{{"name","_prim_LizTemplate_07"},{"gametype","Character"}},
{{"name","_prim_LizTemplate_08"},{"gametype","Character"}},
{{"name","_prim_LizTemplate_09"},{"gametype","Character"}},
{{"name","_prim_NPC_04"},{"gametype","Character"}},
{{"name","_prim_NPC_05"},{"gametype","Character"},{"deactivate_cond","IsBefore_Swamp_KillWitch"}},
{{"name","_prim_NPC_LizBlood"},{"gametype","Character"}},
{{"name","_prim_NPC_LizBloodPOST"},{"gametype","Character"},{"activate_cond","IsAfter_Swamp_LizBlood"}},
{{"name","_prim_Monster_3_276"},{"gametype","Character"}},
{{"name","_prim_Monster_3"},{"gametype","Character"}},
{{"name","_prim_LizTemplate_10"},{"gametype","Character"}},
{{"name","_prim_LizTemplate_11"},{"gametype","Character"}},
{{"name","_prim_LizTemplate_13"},{"gametype","Character"}},
{{"name","_prim_LizTemplate_14"},{"gametype","Character"}},
{{"name","_prim_LizTemplate_21"},{"gametype","Character"}},
{{"name","_prim_LizTemplate_22"},{"gametype","Character"}},
{{"name","_prim_Monster_LizardFan01"},{"gametype","Character"},{"activate_cond","IsBefore_Swamp_Escape"}},
{{"name","_prim_Monster_MisterSnack"},{"gametype","Character"},{"activate_cond","IsBefore_Swamp_Escape"}},
{{"name","_prim_Monster_LizardFan03"},{"gametype","Character"},{"activate_cond","IsBefore_Swamp_Escape"}},
{{"name","_prim_Boss_SwampKing_01"},{"gametype","Character"},{"activate_cond","IsBefore_Swamp_Escape"}},
{{"name","_prim_NPC_Eddie"},{"gametype","Character"},{"activate_cond","EDWARD_FOLLOW"}},
{{"name","_prim_NPC_Mothgiver"},{"gametype","Character"}},
{{"name","_prim_MothTemplate_06"},{"gametype","Character"}},
{{"name","_prim_MothTemplate_08"},{"gametype","Character"}},
{{"name","_prim_MothTemplate_10"},{"gametype","Character"}}
};}
int main(int argc,char** argv){require(argc==3,"V55 requires original ZIP/design");auto archive=pack(argv[1]);std::string error;auto raw=[&](const char* name){std::vector<std::uint8_t> b;bool found{};require(archive.read(std::string("data/pydata/")+name,found,b,error)&&found,error);return b;};auto data=raw("v2conditions_pyarray.bin"),names=raw("v2conditions_pyarraynames.bin");std::shared_ptr<const ConditionTablesV55> table;require(ConditionTablesV55::load(data.data(),data.size(),names.data(),names.size(),table,error),error);require(table->rows().size()==118,"actual Conditions118 dimension");std::size_t stubs=0;for(const auto& row:table->storage()){stubs+=row.stubs.size();for(const auto& s:row.stubs)require(s.source_vtable==0x96a318,"source Stub vtable provenance");}require(stubs==154,"actual Conditions154 stubs");
 auto prior=table;require(!ConditionTablesV55::load(data.data(),data.size()-1,names.data(),names.size(),table,error)&&table==prior,"truncated table committed");auto bad=data;bad[0]=117;require(!ConditionTablesV55::load(bad.data(),bad.size(),names.data(),names.size(),table,error)&&table==prior,"dimension mismatch committed");bad=data;bad.push_back(0);require(!ConditionTablesV55::load(bad.data(),bad.size(),names.data(),names.size(),table,error)&&table==prior,"data suffix accepted");bad=data;std::fill(bad.begin()+4,bad.begin()+8,255);require(!ConditionTablesV55::load(bad.data(),bad.size(),names.data(),names.size(),table,error)&&table==prior,"malformed count accepted");
 std::vector<int> created;std::size_t deletes=0;ActualConditionFactoriesV55 fixture;fixture.owner=std::make_shared<int>(1);
 for(int op=0;op<7;++op)fixture.create[op]=[&,op](ActualConditionBorrowV55& out,std::string& e){auto actual=std::make_shared<ControlledCondition>();out={reinterpret_cast<std::uintptr_t>(actual.get()),actual,&actual->stub};created.push_back(op);e.clear();return true;};
 fixture.deleting_destructor=[&](const ActualConditionBorrowV55& actual,std::string& e){require(actual.receiver_alias.get()==reinterpret_cast<const void*>(actual.receiver)&&actual.live_stub4&&*actual.live_stub4,"controlled constructor/stub pointer mismatch");++deletes;e.clear();return true;};
 auto control=std::make_shared<ConditionListOwnerV55>(table,fixture);auto services=control->services();
 for(const auto& row:table->rows()){std::uintptr_t id{};require(services.construct_condition(id,error),error);require(control->fields(id)->count==0&&!control->fields(id)->instances4&&!control->fields(id)->stubs8,"source C1 zero cells");auto before=created.size();require(services.initialize_condition(id,row.argument8,row.argument4,error),error);auto* f=control->fields(id);require(f&&f->count==static_cast<std::int32_t>(row.argument4)&&reinterpret_cast<std::uintptr_t>(f->stubs8)==row.argument8,"Assign source count/stub aliases");for(int i=0;i<f->count;++i){auto* actual=reinterpret_cast<ControlledCondition*>(f->instances4[i]);require(actual->stub==reinterpret_cast<std::uintptr_t>(f->stubs8+i)&&created[before+i]==f->stubs8[i].op,"source factory order/actual stub4 store");}require(services.destroy_condition(id,error)&&!control->fields(id),error);}
 require(created.size()==154&&deletes==154&&control->retained_list_allocations()==0,"constructor fixture cleanup order");
 const auto positive=*std::find_if(table->rows().begin(),table->rows().end(),[](const auto& row){return row.argument4>0;});std::uintptr_t id{};require(services.construct_condition(id,error)&&services.initialize_condition(id,positive.argument8,positive.argument4,error),error);auto* old=control->fields(id)->instances4;auto oldDeletes=deletes;require(services.initialize_condition(id,positive.argument8,positive.argument4,error)&&control->fields(id)->instances4!=old&&deletes==oldDeletes,"repeated Assign incorrectly cleared old source array");old=control->fields(id)->instances4;require(services.initialize_condition(id,0,0,error)&&control->fields(id)->instances4==old&&!control->fields(id)->stubs8,"zero count didn't preserve array");require(services.initialize_condition(id,123,0xffffffffu,error)&&control->fields(id)->count==-1&&control->fields(id)->instances4==old&&reinterpret_cast<std::uintptr_t>(control->fields(id)->stubs8)==123,"signed negative Assign differs");require(services.destroy_condition(id,error)&&deletes==oldDeletes,"negative D1 incorrectly destroyed skipped instances");require(!services.destroy_condition(123,error),"foreign receiver accepted");
 // Actual selected canonical factory actors, with FOUR cache-authored input
 // properties only. No fabricated CCondition providers are supplied here.
 auto game=std::make_shared<CharacterTables>(archive,argv[2]);auto app=std::make_shared<ApplicationSpawnRandomOwnerV4>();CanonicalCharacterFamilyServicesV4 fs;fs.world=game;fs.design=&game->design;fs.models=&game->models;fs.loot_tables=&game->loot;fs.loot_random=&app->channel(0);CanonicalCharacterFamilyFactoryV4 factory(fs);CanonicalPropertyMapV1 property_map({nullptr,&canonical_vec3_origin_v1(),nullptr});auto actual_classes=std::make_shared<CanonicalConditionFactoryV55>(game);auto required=std::make_shared<ConditionListOwnerV55>(table,actual_classes->factories());auto required_services=required->services();std::size_t conditions=0,sentinels=0,stops=0,authActivate=0,authDeactivate=0;
 for(const auto& attrs:source_conditions_v55()){auto p=std::make_shared<Placement>();p->attrs=attrs;authActivate+=attrs.count("activate_cond");authDeactivate+=attrs.count("deactivate_cond");CanonicalSourceObjectRequestV1 request;request.source_lease=p;request.source_context=p.get();request.element=1;request.attribute=[](void* context,std::uint32_t,const char* key)->const char*{auto& a=static_cast<Placement*>(context)->attrs;auto i=a.find(key);return i==a.end()?nullptr:i->second.c_str();};CanonicalClassReceiverV1 receiver;require(factory.construct({"Character",0x340800},request,receiver,error),error);auto record=factory.find(receiver.object.identity);require(record&&record->actor,"actual Character record required");auto actor=std::shared_ptr<character::RetainedCharacterActorV1>(record,record->actor.get());auto props=actor->properties();require(property_map.init_properties(props,error)&&property_map.load_defaults(props,error)&&property_map.load_overrides(props,request,error),error);character::CharacterParentInitFieldsV48 borrowed;require(actor->parent_init_fields_v48(actor,borrowed,error),error);auto fields=character_gameobject_fields_v51(borrowed);
  for(const auto& c:borrowed.conditions){require(c.name==actor->source_string(c.source_offset+4)&&!*c.compiled&&!*c.tested,"same actor ConditionData C1/name alias");if(c.name->empty()||*c.name=="Invalid"){require(condition_data_init_borrow_v51(fields,c.source_offset,required_services,error)&&!*c.compiled&&!*c.tested,"empty/Invalid fabricated allocation");++sentinels;continue;}
   ++conditions;require(condition_data_init_borrow_v51(fields,c.source_offset,required_services,error),error);require(*c.compiled&&!(*c.tested),"same actor compiled C1/Assign transport");const auto* f=required->fields(*c.compiled);require(f&&f->count>0&&f->instances4&&f->stubs8,"actual decoded list backing missing");
   for(int i=0;i<f->count;++i){CanonicalConditionFieldsV55 actual;require(actual_classes->borrow(f->instances4[i],actual,error),error);require(*actual.stub4==reinterpret_cast<std::uintptr_t>(f->stubs8+i)&&actual.comparator8&&*actual.comparator8==f->stubs8[i].op,"same canonical constructor/stub/comparator backing");bool value=true;require(!actual_classes->evaluate(actual.identity,value,error)&&value&&error.find("Required main original Condition Eval ")==0,"main Eval fabricated success");}
   require(condition_data_clear_borrow_v51(fields,c.source_offset,required_services,error)&&!*c.compiled&&!*c.tested,"actual D1/D0/CustomFree source same-actor Clear");++stops;
  }
 }
 require(authActivate==10&&authDeactivate==2&&conditions==10&&sentinels==90&&stops==10&&required->retained_list_allocations()==0,"actual SWAMP50/100cells/12authored/10meaningful mismatch");require(app->channel(0).calls==0,"condition transport consumed application RNG");
 std::cout<<"PASS original_cache_rows118_stubs154; ctor_assign_D1_controlled_factory_fixtures118; malformed4; repeatedAssign/zero/negative/foreign; actual_selected_Character50_conditions100_authored12_Invalid2_CANONICAL_C1_Assign_Clear10; same_actor_compiled_lifetime_and_clear; main_Eval_required_no_eval_no_RNG_no_whole_InitPost_claim\n";
}
