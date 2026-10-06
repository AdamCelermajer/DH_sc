#include "../canonical_level_context_v1.hpp"
#include <cassert>
#include <cstring>
#include <iostream>
using namespace dh2::loader;
struct ApplicationFixture {
 dh2::data::LevelTables levels;
 std::uint32_t debug{},module_id=99;
};
int main(){
 unsigned checks=0;
 for(bool match:{false,true})for(bool fail_script:{false,true}){
  auto app=std::make_shared<ApplicationFixture>();
  dh2::data::LevelRecord row;row.file="SWAMP";row.scalar.words[4]=3;row.scalar.words[5]=2;app->levels.levels.push_back(row);
  LevelSourceRequestV1 source;source.identity="swamp01";source.definition="fixture/swamp.cfg";source.seed=7;
  std::string error;std::shared_ptr<CanonicalLevelContextV1> level;
  assert(CanonicalLevelContextV1::create(source,app,level,error));++checks;
  std::weak_ptr<CanonicalLevelContextV1> weak=level;
  auto fields=level->constructor_borrow_v3();auto config=level->config_fields();auto module=level->module_load_fields();
  assert(fields.identity==level->identity()&&fields.config38==config.config38&&fields.music11c==config.music11c&&fields.module_offset160==module.module_offset160&&fields.module_id18c==module.object_module_id18c);++checks;
  *fields.config38=99;*fields.gate150=99;*fields.music11c=99;*fields.module_id18c=99;
  std::weak_ptr<int> script_weak,save_weak;std::shared_ptr<void> allocated;
  unsigned calls=0;LevelConstructorServicesV3 services;
  services.application=app;services.debug_level_load_count=&app->debug;services.module_id_global=&app->module_id;services.levels=&app->levels;
  services.construct_script=[&](auto identity,bool deferred,auto& script,auto& e){
   auto retained=weak.lock();assert(retained&&identity==retained->identity()&&!deferred);
   assert(retained->constructor_fields_v3().events->identity()==identity&&*retained->config_fields().config38==0);checks+=2;++calls;
   if(fail_script){e="explicit script fixture failure";return false;}
   auto owner=std::make_shared<int>(1);script_weak=owner;script=owner;return true;
  };
  services.script_load=[&](auto& script,const char* name,auto&){assert(script&&(!std::strcmp(name,"level/combat_formulas")||!std::strcmp(name,"level/death_scripts")));++calls;return true;};
  services.script_assign_path=[](auto& script,const char* name,std::size_t length,auto&){assert(script&&std::string(name,length)=="data/scripts/");return true;};
  services.online_byte5=[](auto& value,auto&){value=0;return true;};
  services.allocate_save=[&](auto& storage,auto&){auto save=std::make_shared<int>(2);save_weak=save;storage=save;allocated=save;++calls;return true;};
  services.construct_save=[&](const auto& storage,const auto& request,auto&){auto retained=weak.lock();assert(retained&&storage==allocated&&request.level_identity==retained->identity()&&request.seed==7&&request.row==0&&request.difficulty==3&&request.mode==0&&!request.source_flag);++checks;++calls;return true;};
  const bool result=level->construct_source_v3({match?"worlds/swamp01.dwld":"missing",0,7,1,0,1,0,-1,0},std::move(services),error);
  assert(result==!fail_script);++checks;
  const auto* owner=level->constructor_owner_v3();assert(owner);++checks;
  auto before=calls;assert(!level->construct_source_v3({"retry",0,7,1,0,1,0,-1,0},{},error)&&calls==before);++checks;
  if(!fail_script){assert(*fields.gate150==0&&*fields.config38==0&&*fields.music11c==-1&&*fields.module_id18c==-1&&app->debug==1&&app->module_id==0);++checks;assert(bool(level->constructor_fields_v3().save_ec)==match);++checks;}
  std::shared_ptr<CanonicalLevelContextV1> global=level;CanonicalCurrentLevelBorrowV1 current;
  assert(borrow_current_canonical_level_v1({app,&global},current,error)&&current.identity()==level->identity());++checks;
  level->source_word150()=123;assert(current.kill_level()->loot_gate150==123);++checks;
  global.reset();level.reset();assert(!weak.expired());++checks;
  fields.owner.reset();config.level_owner.reset();module.level_owner.reset();assert(!weak.expired());++checks;
  current={};allocated.reset();assert(weak.expired()&&script_weak.expired()&&save_weak.expired());++checks;
 }
 std::cout<<"PASS same Level constructor lifetime "<<checks<<'\n';
}
