#include "../character_player_skills_v6.hpp"
#define CharacterPlayerSkillsV3 CharacterPlayerSkillsV6
#define main existing_player_fixture_main
#include "character_player_skills_v3.cpp"
#undef main
#include "../character_skill_native_v5.hpp"
#include "../gameobject_lua_representation.hpp"
#include "../character_target_providers.hpp"
#include "../../script-runtime/script_return_observer_v3.h"
namespace {
struct NativeDebug {
 DebugSwitches* debug;const DebugFileServices24* files;
 std::map<std::uintptr_t,std::string> strings;std::map<std::string,std::uint32_t> application_flags;
 std::uintptr_t serial=1;unsigned queries=0;
 bool fail_stats=false;
 static int mana(void* p,const sk::SkillManaRequestV5* q,sk::SkillManaResponseV5* r){
  auto& self=*static_cast<NativeDebug*>(p);
  switch(q->service){
  case sk::mana_application_v5:r->word=0;return 0; // Explicit application byte5 fixture.
  case sk::mana_player_v5:return -1; // Bypass never reached in this fixture.
  case sk::mana_debug_contains_v5:r->word=self.application_flags.find(q->name)!=self.application_flags.end();return 0;
  case sk::mana_debug_load_v5:return dh2_character_debug_load(self.debug,self.files)==1?0:-1;
  case sk::mana_debug_construct_v5:r->identity=++self.serial;self.strings[r->identity]=q->name;return 0;
  case sk::mana_debug_get_v5:{auto it=self.strings.find(q->subject);if(it==self.strings.end())return -1;++self.queries;
   if(self.fail_stats&&it->second=="isTracingChar_Stats")return -1;
   return dh2_character_debug_get(&r->word,self.debug,it->second.c_str(),self.files)==1?0:-1;}
  case sk::mana_debug_destroy_v5:return self.strings.erase(q->subject)==1?0:-1;
  default:return -1;
  }
 }
 static int target(void* p,TargetState48*,const TargetRequest24* q,std::uint32_t* out){auto& self=*static_cast<NativeDebug*>(p);
  if(q->service==target_debug_load)return dh2_character_debug_load(self.debug,self.files)==1?0:-1;
  if(q->service==target_debug_query)return dh2_character_debug_get(out,self.debug,q->text,self.files)==1?0:-1;
  return -1; // ClearTarget(null) reaches only the actual debug providers.
 }
};
struct EmptyWorld {
 target_search::Object48 owner{};
 target_search::Room16 sentinel{};target_search::Registry8 registry{};
 target_providers::Character32 character{};CombatProperties896 cached{};
 std::vector<std::int32_t> types;target_providers::Types16 ai_types{};
 static int search(void* p,const target_search::Request24* q,target_search::Response16* out){
  auto& self=*static_cast<EmptyWorld*>(p);
  if(q->service==target_search::is_character){std::int32_t answer;
   if(target_providers::dh2_character_target_query(&answer,target_providers::is_character,&self.character,nullptr,&self.ai_types,nullptr))return -1;
   out->word=answer;return 0;}
  if(q->service==target_search::melee_radius){out->number=0;return 0;} // Named zero-radius geometry input.
  return -1; // No fabricated enemy/death/interaction success.
 }
 static int resolve(void*,std::uintptr_t,target_search::Object48**){return -1;}
 static int type(void*,std::uintptr_t,const char**){return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;}
 static int methods(void*,std::uintptr_t,const dh2_script_object_method**,std::uint32_t*){return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;}
 static int method(void*,const dh2_script_callback_scope*,std::uintptr_t,std::uint32_t,const dh2_script_value*,std::uint32_t,dh2_script_value*,std::uint32_t,std::uint32_t*,char*,std::size_t){return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;}
};
}
int main(int argc,char** argv){try{
 check(argc==6,"Usage: v5 game-design assets cache original-cache.zip loot-pydata");
 Inputs raw(argv[1]);const std::string assets=argv[2],cache=argv[3],loot=argv[5];
 CharacterGameDesign design;std::string error;check(design.initialize(raw.input,error),error);auto d=design.borrow();
 data::SkillTables skills;data::FaeryTables faeries;data::LootTablesV2 loots;
 auto load=[&](auto& owner,const std::string& base){auto a=file(base+"_pyarray.bin"),b=file(base+"_pyarraynames.bin"),c=file(base+"_pystructnames.bin");check(owner.load(bytes(a),bytes(b),bytes(c),error),error);};
 load(skills,assets+"/data/skills");load(faeries,cache+"/data/pydata/faeries");load(loots,loot+"/loot_table");
 auto sb=skills.borrow();auto source=script_assets(argv[4],skills.borrow());auto all=source.borrow();
 Debug debug_files;DebugFileServices24 files{&debug_files,Debug::open,Debug::close};
 auto* debug=dh2_character_debug_create();std::unique_ptr<DebugSwitches,decltype(&dh2_character_debug_destroy)> debug_owner(debug,dh2_character_debug_destroy);
 auto* modules=dh2_fx_debug_modules_create(debug,&files);std::unique_ptr<fx::DebugModules,decltype(&dh2_fx_debug_modules_destroy)> module_owner(modules,dh2_fx_debug_modules_destroy);
 fx::PreloadServices16 debug_services{modules,dh2_fx_debug_preload_service};Vitals vitals{&debug_services};
 const char* classes[]={"KnightPlayerBase","MagePlayerBase","RoguePlayerBase"};
 const char* scripts[]={"prince_warrior_bashdown","prince_mage_coldray","prince_rogue_jump_kick"};
 unsigned callbacks=0,required_prefix=0,spent=0;
 for(unsigned which=0;which<3;++which){
  Unit unit;const auto id=std::uintptr_t(0xabcdef0500000001ull+which);
  auto it=std::find(d.characters()->names.begin(),d.characters()->names.end(),classes[which]);check(it!=d.characters()->names.end());
  unit.fsm_state.current=3;unit.fsm={&unit.fsm_state,id,1,0};
  unit.event_owner={id,id+1,reinterpret_cast<std::uintptr_t>(&unit.fsm_state),id+2,0,0,0,0};unit.ai_keys[0x90/4]=0x3d0ca0;unit.ais_keys[0x90/4]=0x3dcc80;
  unit.events={id+3,&unit.event_owner,unit.ai_keys.data(),0,unit.ais_keys.data(),0,0,0,0,0,0};
  CharacterScriptSessionInputV3 input;input.identity=id;input.name="Player";input.source_is_character=1;
  input.properties=std::make_shared<data::PropertyState>();input.combat=std::make_shared<data::CombatActorState>();input.temporary=std::make_shared<data::PropertySheet>();
  data::reset_properties(*d.rules(),*input.properties,&d.characters()->rows[it-d.characters()->names.begin()]);check(data::recalc_properties_with_class(*d.classes(),*d.rules(),*input.properties,error),error);
  unit.saved=std::make_shared<data::PlayerSavegameV1>();unit.saved->set_character(id);const auto list=input.properties->resolved[28]>=0&&std::size_t(input.properties->resolved[28])<sb.lists().size()?std::size_t(input.properties->resolved[28]):3;check(list<sb.lists().size());check(unit.saved->initialize_skills(sb.lists()[list],error),error);unit.saved->initialize_faeries();input.savegame=unit.saved;
  unit.scripts=all;input.common=all.common();input.cached_file_context=&unit;input.cached_file=Unit::cached;input.state_machine=&unit.fsm;
  data::LootRandom8V2 random{1,0};data::FreshInventoryOwnedV4 inventory(id,loots.borrow(),random,-1,input.properties);
  NativeDebug native_debug{debug,&files};sk::SkillManaServicesV5 mana_services{&native_debug,NativeDebug::mana};
  TargetOwner16 target_owner{id,0,0,0};TargetState48 target{id+4,&target_owner,0,0,0,0,0,0,0,0};
  TargetBindings48 target_bindings{&target,{&native_debug,NativeDebug::target},nullptr,{0,0}};input.target=&target_bindings;
  EmptyWorld world;world.owner.identity=id;world.owner.visible=1;world.sentinel.next=&world.sentinel;world.registry.rooms=&world.sentinel;
  std::memcpy(world.cached.words,input.properties->resolved.data(),896);world.character={id,&world.cached,"Player",0,0,0,1,1};
  for(const auto& row:d.ai()->rows)world.types.push_back(row.type);world.ai_types={world.types.data(),static_cast<std::uint32_t>(world.types.size()),0};
  dh2_script_object_services objects{&world,EmptyWorld::type,EmptyWorld::methods,EmptyWorld::method};input.objects=&objects;
  sk::SkillNativeWorldV5 live{&world.owner,&world.registry,{&world,EmptyWorld::search},&world,nullptr,EmptyWorld::resolve,&objects};
  sk::CharacterSkillNativeBindingsV5 native(inventory,unit.fsm,mana_services,live);
  input.gameplay_context=&native;input.gameplay_binding=sk::CharacterSkillNativeBindingsV5::binding;
  sk::PlayerSkillInitServicesV3 init{&vitals,Vitals::invoke};init.gameplay.ai=&unit.events;unit.skill_owner={id,0,0};init.gameplay.skill_owner=&unit.skill_owner;init.gameplay.difficulty_context=&unit;init.gameplay.difficulty=Unit::selected;
  unit.player=sk::CharacterPlayerSkillsV3::create(design.borrow(),input,skills.borrow(),faeries.borrow(),debug_services,unit.fsm,init,error);check(bool(unit.player),error);
  check(!native.attach(unit.player->session()),native.error());check(unit.player->initialize(1)==1,unit.player->error());check(!native.install_object_binding(),native.error());
  auto& player=*unit.player;
  // Source CancelSneaking's fixed class146 native delete uses THIS existing
  // BuffOwner, same property groups and same V3 TimerStore, not another graph.
  check(player.native_buffs()!=nullptr,"V6 native buff borrow");
  check(player.session().classes().rows.size()>146,"Source fixed buff class range");
  const auto initial_buffs=player.buff_count(),initial_groups=player.session().property_view().group_count;
  BuffResult24 added{};check(dh2_character_buff_add(&added,player.native_buffs(),146,100000,1,256,-1,"native_v6_source146")==1,"Same-owner native buff add");
  check(added.instance&&player.buff_count()==initial_buffs+1,"Owned native Buff instance");
  BuffSnapshot48 snapshot{};bool found_buff=false;
  for(unsigned n=0;n<player.buff_count();++n){check(player.buff_snapshot(&snapshot,n)==1,"Actual native Buff snapshot");if(snapshot.instance==added.instance){found_buff=true;break;}}
  check(found_buff,"Source class146 native instance");
  const auto timer_id=snapshot.timer_id;check(timer_id>=0&&player.session().timers().slots[timer_id].active,"Same V3 Buff timer active");
  check(snapshot.sheet&&player.session().property_view().group_count,"Same property group publication");
  BuffResult24 removed{};check(player.native_delete_buff(&removed,146)==1,"Source same-owner PROPS_DelBuff");
  check(player.buff_count()==initial_buffs&&!player.session().timers().slots[timer_id].active,"Same timer native removal");
  check(player.session().property_view().group_count==initial_groups,"Same property group removal");
  std::uint8_t source_byte415=0;
  check(player.native_cancel_sneaking(&source_byte415)==0&&source_byte415==1,"Same owner source CancelSneaking byte write");
  const auto& slots=player.state().skills;
  auto found=std::find_if(slots.items,slots.items+slots.count,[&](auto* value){return value&&!std::strcmp(value->script,scripts[which]);});check(found!=slots.items+slots.count);
  const auto index=unsigned(found-slots.items);check(unit.saved->set_skill_level(index,1,error),error);check(player.update()==1,player.error());
  std::uint32_t answer=123;check(!dh2_property_set(&player.session().property_view(),41,100000));
  check(!player.callback(index,sk::skill_check_usable_v3,&answer)&&answer==1,player.error());++callbacks;
  const auto before=input.properties->resolved[41];const auto pre=player.callback(index,sk::skill_pre_v3,&answer);++callbacks;
  check(pre==0&&answer==1,player.error());
  check(input.properties->resolved[41]<before,"Same-owner actual UseMana effect");++spent;
  check(!player.callback(index,sk::skill_use_v3,&answer)&&answer==1,player.error());++callbacks;
  player.skill_ai().current=int(index);
  check(!player.native_skill_animation_event(&answer),player.error());++callbacks;
  target.candidate=target.target=target.last_target=id+100;
  check(!player.callback(index,sk::skill_post_v3,&answer),player.error());++callbacks;
  check(!target.candidate&&!target.target&&!target.last_target,"Actual scoped native ClearTarget source effects");
  check(!dh2_property_set(&player.session().property_view(),41,0));
  check(!player.callback(index,sk::skill_check_usable_v3,&answer)&&answer==0,"Actual source insufficient mana Check");++callbacks;
  check(native.targets().owner==&world.owner&&player.session().properties()==inventory.properties(),"Same world/property owner");
  check(native_debug.strings.empty(),"Source debug tokens released");
  // Reached final Debug failure occurs AFTER native mana subtraction. Invoke
  // the actual installed wrapper, never substitute an authored skill alias.
  check(!dh2_property_set(&player.session().property_view(),41,100000));native_debug.fail_stats=true;
  ScriptSessionView active{};check(player.session().view(active));dh2_script_value cost{};cost.type=DH2_SCRIPT_NUMBER;cost.number=256;First observed;
  check(dh2_script_vm_call_indexed_source_v3(active.vm,"UseMana",&cost,1,0,First::observe,&observed)==DH2_SCRIPT_REQUIRED_FAILURE_STATUS,"Required source wrapper diagnostic");
  check(input.properties->resolved[41]==100000-256,"Mana prefix retained on reached final Debug failure");++required_prefix;
  player.session().close();
 }
 std::cout<<"{\"validation\":\"PASS\",\"actual_source_scripts\":3,\"same_v3_callbacks\":"<<callbacks<<",\"same_owner_mana_spends\":"<<spent<<",\"required_debug_failure_prefix\":"<<required_prefix<<",\"explicit_empty_world_fixture\":true,\"no_alias_fixture\":true,\"full_campaign\":false,\"checks\":"<<checks<<"}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}

#undef CharacterPlayerSkillsV3
