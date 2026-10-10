#include "../app/src/main/cpp/native_quest_owner.hpp"
#include "../app/src/main/cpp/native_level_sg_update_v1.hpp"
#include "../app/src/main/cpp/native_level_quest_active_v1.hpp"
#include "../../game-data/condition_data_v1.hpp"
#include "../app/src/main/cpp/native_current_level_character_population_v1.hpp"
#include <fstream>
#include <cstdio>
#include <cstring>
#include <set>
#include <stdexcept>
#include <vector>
namespace d=dh2::data;
namespace n=dh2::native::quests;
namespace level_sg=dh2::native::level_sg_update_v1;
namespace level_quest=dh2::native::level_quest_active_v1;
namespace t=d::quest_table_bindings_v1;
namespace k=dh2::character_kill_quest_tail_v1;
namespace population=dh2::native::current_level_character_population_v1;
namespace character_list=dh2::character::aggro::object_manager_list;
namespace aggro=dh2::character::aggro_search;
namespace manager=dh2::object_manager_runtime_owner_v1;
unsigned checks=0;
void check(bool value){if(!value)throw std::runtime_error("Native Quest owner check "+std::to_string(checks+1));++checks;}
void check(bool value,const char* message){if(!value)throw std::runtime_error(message);++checks;}
void check(bool value,const std::string& message){if(!value)throw std::runtime_error(message);++checks;}
auto read(const std::string& path){
 std::ifstream f(path,std::ios::binary|std::ios::ate);check(bool(f));
 auto bytes=std::make_shared<std::vector<std::uint8_t>>(std::size_t(f.tellg()));f.seekg(0);
 f.read(reinterpret_cast<char*>(bytes->data()),std::streamsize(bytes->size()));check(bool(f));
 return std::shared_ptr<const std::vector<std::uint8_t>>(bytes);
}
struct TextFixture {std::vector<std::int32_t> requested;std::int32_t empty_id=-1;};
struct ScriptFixture {std::vector<std::int32_t> started;std::vector<std::string> order;};
struct QuestCompileProbe {std::uintptr_t character=0;unsigned difficulty_calls=0,member_calls=0;};
std::int32_t quest_compile_difficulty(void* raw,std::uintptr_t character,std::int32_t* out){
 auto& probe=*static_cast<QuestCompileProbe*>(raw);++probe.difficulty_calls;
 if(character!=probe.character||!out)return 1;
 *out=0;return 0;
}
std::int32_t quest_compile_member(void* raw,d::quest_compile_v1::Objective&,
 const d::quest_compile_v1::MemberCall&){
 ++static_cast<QuestCompileProbe*>(raw)->member_calls;return 1;
}
struct LevelSGUpdateFixture {
 bool in_game_view=false,checkpoint=true,script_flag=true;
 std::uintptr_t level=0,character=0;
 std::int32_t script=-1,argument=0;
 n::Owner* qest_owner=nullptr;
 d::PlayerSavegameV1* save=nullptr;
 const level_quest::SourceLevel* source_level=nullptr;
 level_quest::ScriptServices quest_scripts{};
 std::vector<n::QuestUpdateActiveResultV1>* quest_updates=nullptr;
 std::int32_t difficulty=0,application_time=0;
 bool online=false,update_active_quests=false;
 unsigned quest_update_calls=0;
 std::vector<std::string> order;
};
bool level_sg_in_game_view(void* raw,bool& value,std::string& error){
 auto& f=*static_cast<LevelSGUpdateFixture*>(raw);f.order.push_back("game-view");
 value=f.in_game_view;error.clear();return true;
}
bool level_sg_start_script(void* raw,std::uintptr_t level,std::int32_t script,
 std::int32_t argument,bool flag,std::string& error){
 auto& f=*static_cast<LevelSGUpdateFixture*>(raw);f.order.push_back("start-script");
 f.level=level;f.script=script;f.argument=argument;f.script_flag=flag;
 error.clear();return true;
}
bool level_sg_character_update(void* raw,std::uintptr_t level,std::uintptr_t character,
 bool checkpoint,std::string& error){
 auto& f=*static_cast<LevelSGUpdateFixture*>(raw);f.order.push_back("character-sg-update");
 f.level=level;f.character=character;f.checkpoint=checkpoint;
 if(f.qest_owner&&(!f.save||!f.qest_owner->owns_save(f.save)||
                   character!=f.save->character())){
  error="Level frame Character/Save/QEST owner identities differ";return false;
 }
 if(f.update_active_quests){
  ++f.quest_update_calls;f.order.push_back("quest-active-update");
  if(!f.qest_owner||!f.save||!f.source_level||!f.quest_updates||
     !level_quest::update_active_log(*f.source_level,level,character,*f.save,
       *f.qest_owner,f.online,f.difficulty,f.application_time,f.quest_scripts,
       *f.quest_updates,error))return false;
 }
 error.clear();return true;
}
bool level_sg_execute_scripts(void* raw,std::uintptr_t level,std::string& error){
 auto& f=*static_cast<LevelSGUpdateFixture*>(raw);f.order.push_back("execute-scripts");
 f.level=level;error.clear();return true;
}
struct UpdateActiveFixture {
 d::quest_runtime_fields_v1::Record* canonical=nullptr;
 std::uintptr_t identity=0;
 bool running=false;
 std::string queried_script,post_active_script;
 std::int32_t start_argument=0;
 bool start_flag=false;
 std::vector<std::string> order;
};
bool update_active_script_running(void* raw,const std::string& script,
 bool& running,std::string& error){
 auto& fixture=*static_cast<UpdateActiveFixture*>(raw);
 fixture.queried_script=script;fixture.order.push_back("script-running");
 running=fixture.running;error.clear();return true;
}
bool update_active_start_script(void* raw,const std::string& script,
 std::int32_t argument,bool flag,std::string& error){
 auto& fixture=*static_cast<UpdateActiveFixture*>(raw);
 fixture.order.push_back("start-script");fixture.post_active_script=script;
 fixture.start_argument=argument;fixture.start_flag=flag;error.clear();return true;
}
bool start_objective_script(void* context,std::int32_t script,std::string& error){
 auto& fixture=*static_cast<ScriptFixture*>(context);fixture.started.push_back(script);
 fixture.order.push_back("script");error.clear();return true;
}
bool localized_text(void* context,std::int32_t id,std::string& out,std::string& error){
 auto& fixture=*static_cast<TextFixture*>(context);fixture.requested.push_back(id);
 out=id==fixture.empty_id?std::string{}:"text"+std::to_string(id);error.clear();return true;
}
struct ClearPopulationFixture {std::int32_t level_id=7,count=2,selector=-1;std::uintptr_t level_identity=0;bool stale_owner=false,stale_level=false;};
bool read_clear_population(void* context,std::uintptr_t expected_owner,
 std::uintptr_t expected_level,std::int32_t selector,
 n::ClearEnemiesPopulationSnapshotV1& snapshot,std::string& error){
 auto& fixture=*static_cast<ClearPopulationFixture*>(context);fixture.selector=selector;
 snapshot.event_owner_identity=fixture.stale_owner?expected_owner+1:expected_owner;
 snapshot.level_identity=fixture.stale_level?expected_level+1:expected_level;
 snapshot.level_id=fixture.level_id;snapshot.loaded_match_count=fixture.count;
 error.clear();return true;
}
struct CanonicalRosterFixture {
 character_list::Owner list{};
 character_list::Node nodes[4]{};
 aggro::GameObject objects[3]{};
 aggro::Character characters[3]{};
 manager::Owner object_manager;
 std::size_t next_node=0;
 std::uintptr_t event_owner=0x6100,level_identity=0x6200;
 std::int32_t properties[3]{1,1,2};
 std::int16_t property_cache[3]{1,1,2};
 bool stale_character=false;
 static character_list::Node* allocate(void* raw){
  auto& self=*static_cast<CanonicalRosterFixture*>(raw);
  return self.next_node<4?&self.nodes[self.next_node++]:nullptr;
 }
 static bool safe_property_id(void* raw,std::uintptr_t owner,
  std::uintptr_t level,std::uintptr_t character,std::int32_t* property,
  std::string& error){
  auto& self=*static_cast<CanonicalRosterFixture*>(raw);
  if(!property||owner!=self.event_owner||level!=self.level_identity||
     (self.stale_character&&character==self.characters[2].identity)){
   error="stale test Character owner";return false;
  }
  for(std::size_t i=0;i<3;++i)if(self.characters[i].identity==character){
   *property=self.properties[i];error.clear();return true;
  }
  error="unknown test Character";return false;
 }
 static bool safe_get_bindings(void* raw,std::uintptr_t owner,
  std::uintptr_t level,std::uintptr_t character,
  population::SafeGetBindings& bindings,std::string& error){
  auto& self=*static_cast<CanonicalRosterFixture*>(raw);
  if(owner!=self.event_owner||level!=self.level_identity||
     (self.stale_character&&character==self.characters[2].identity)){
   error="stale test Character owner";return false;
  }
  for(std::size_t i=0;i<3;++i)if(self.characters[i].identity==character){
   return population::bind_existing_character_property_cache(character,
       &self.property_cache[i],bindings,error);
  }
  error="unknown test Character";return false;
 }
 CanonicalRosterFixture(){
  check(character_list::initialize(&list)==character_list::Status::ok);
  for(std::size_t i=0;i<3;++i){
   const auto identity=std::uintptr_t(0x7000+i);
   objects[i].identity=identity;
   characters[i].identity=identity;characters[i].object=&objects[i];
   manager::GameObject registered{};registered.identity=identity;
   manager::GameObject* stored=nullptr;
   check(object_manager.add_object(std::int32_t(20+i),registered,&stored)==manager::Status::ok&&stored);
   bool appended=false;
   check(character_list::append_after_add(&list,{false,&characters[i]},allocate,
         this,&appended)==character_list::Status::ok&&appended);
  }
 }
};
void current_level_character_population_cases(){
 using Cache=dh2::character_oid_cache_v1::Owner;
 CanonicalRosterFixture fixture;Cache cache;
 check(cache.begin_level(fixture.level_identity,32)==dh2::character_oid_cache_v1::Status::complete);
 const auto cache_generation=cache.generation();
 dh2::character_oid_cache_v1::Result cached{};
 check(cache.add_char_oid(1,7,&cached)==dh2::character_oid_cache_v1::Status::complete);
 check(cache.add_char_oid(9,4,&cached)==dh2::character_oid_cache_v1::Status::complete);
 population::Bindings bindings{&fixture,&fixture.list,&fixture.object_manager,&cache,
   fixture.event_owner,fixture.level_identity,7,3,
   fixture.list.mutation_generation,32,cache_generation,true,
   nullptr,&CanonicalRosterFixture::safe_get_bindings};
 population::Provider provider(bindings);auto services=provider.services();
 n::ClearEnemiesPopulationSnapshotV1 snapshot{};std::string error;
 check(services.read_current_level_population(services.context,fixture.event_owner,
   fixture.level_identity,1,snapshot,error));
 // This source projection deliberately labels one listed Character as the
 // Player and another as dead: HasEnemyOfTypeLoaded applies neither filter.
 check(snapshot.loaded_match_count==2&&snapshot.level_id==7&&
   snapshot.event_owner_identity==fixture.event_owner&&snapshot.level_identity==fixture.level_identity);
 check(services.read_current_level_population(services.context,fixture.event_owner,
   fixture.level_identity,2,snapshot,error)&&snapshot.loaded_match_count==1,
   "loaded Character count did not take precedence over the larger OID cache value");
 check(services.read_current_level_population(services.context,fixture.event_owner,
   fixture.level_identity,9,snapshot,error)&&snapshot.loaded_match_count==4,
   "source HasCharOIDInCache fallback was not used when no loaded Character matched");
 bindings.roster_complete=false;population::Provider incomplete(bindings);
 auto incomplete_services=incomplete.services();snapshot.loaded_match_count=99;
 check(!incomplete_services.read_current_level_population(incomplete_services.context,
   fixture.event_owner,fixture.level_identity,1,snapshot,error)&&snapshot.loaded_match_count==-1,
   "partial CharacterList advanced the quest population query");
 bindings.roster_complete=true;bindings.expected_character_count=4;
 population::Provider short_roster(bindings);auto short_services=short_roster.services();
 check(!short_services.read_current_level_population(short_services.context,
   fixture.event_owner,fixture.level_identity,1,snapshot,error),
   "CharacterList count mismatch was accepted as complete");
 bindings.expected_character_count=3;fixture.stale_character=true;
 population::Provider stale_character(bindings);auto stale_services=stale_character.services();
 check(!stale_services.read_current_level_population(stale_services.context,
   fixture.event_owner,fixture.level_identity,1,snapshot,error),
   "stale Character property owner was counted");
 fixture.stale_character=false;
 population::Provider stale_level(bindings);auto stale_level_services=stale_level.services();
 check(!stale_level_services.read_current_level_population(stale_level_services.context,
   fixture.event_owner,fixture.level_identity+1,1,snapshot,error),
   "stale Level identity was accepted");
 bindings.oid_cache_generation=cache_generation+1;
 population::Provider stale_cache_generation(bindings);auto stale_cache_services=stale_cache_generation.services();
 check(!stale_cache_services.read_current_level_population(stale_cache_services.context,
   fixture.event_owner,fixture.level_identity,1,snapshot,error),
   "stale Character OID cache generation was accepted");
 bindings.oid_cache_generation=cache_generation;
 bindings.character_table_size=31;
 population::Provider mismatched_table_size(bindings);auto table_size_services=mismatched_table_size.services();
 check(!table_size_services.read_current_level_population(table_size_services.context,
   fixture.event_owner,fixture.level_identity,1,snapshot,error),
   "CharacterTable size mismatch with the OID cache was accepted");
 bindings.character_table_size=32;
 population::Provider out_of_range_selector(bindings);auto out_of_range_services=out_of_range_selector.services();
 check(!out_of_range_services.read_current_level_population(out_of_range_services.context,
   fixture.event_owner,fixture.level_identity,32,snapshot,error),
   "out-of-range CharacterTable selector was accepted");
 check(!services.read_current_level_population(services.context,fixture.event_owner+1,
   fixture.level_identity,1,snapshot,error),
   "stale event owner identity was accepted");
 bool duplicate_appended=false;
 check(character_list::append_after_add(&fixture.list,{false,&fixture.characters[0]},
       CanonicalRosterFixture::allocate,&fixture,&duplicate_appended)==
       character_list::Status::ok&&duplicate_appended,
       "duplicate Character fixture was not constructed");
 bindings.expected_character_count=4;
 bindings.roster_generation=fixture.list.mutation_generation;
 population::Provider duplicate_character(bindings);
 auto duplicate_services=duplicate_character.services();
 check(!duplicate_services.read_current_level_population(duplicate_services.context,
       fixture.event_owner,fixture.level_identity,1,snapshot,error)&&
       error.find("duplicate Character identity")!=std::string::npos,
       "duplicate CharacterList identity was counted as additional QEST population");
}
struct ActiveLevelPopulationFixture {
 dh2::source_level_owner_v1::Owner level{};
 dh2::native::character_list::Owner characters{};
 aggro::GameObject object{};
 aggro::Character character{};
 std::int16_t property_cache=7;
 std::uintptr_t event_owner=0x8100;
 static bool safe_bindings(void* raw,std::uintptr_t event_owner,
  std::uintptr_t level_identity,std::uintptr_t character_identity,
  population::SafeGetBindings& bindings,std::string& error){
  auto& self=*static_cast<ActiveLevelPopulationFixture*>(raw);
  if(event_owner!=self.event_owner||level_identity!=self.level.snapshot().source_level||
     character_identity!=self.character.identity){
   error="active Level SafeGet identity mismatch";return false;
  }
  return population::bind_existing_character_property_cache(
      character_identity,&self.property_cache,bindings,error);
 }
 bool initialize(std::string& error){
  static int projection_token=0;
  if(!level.begin_load()||!level.request_level(41,0,0)||
     !level.begin_source_load()||!level.publish_projection(&projection_token)){
   error="active Level population fixture lifecycle failed";return false;
  }
  const auto identity=level.snapshot().source_level;
  character.identity=0x8200;object.identity=character.identity;
  character.object=&object;
  manager::GameObject row{};row.identity=character.identity;
  manager::GameObject* stored=nullptr;
  if(level.object_manager().add_object(1,row,&stored)!=manager::Status::ok||!stored){
   error="active Level population ObjectManager add failed";return false;
  }
  bool appended=false;
  if(characters.enroll_after_add(&character,false,&appended)!=
       character_list::Status::ok||!appended){
   error="active Level population CharacterList add failed";return false;
  }
  cache.begin_level(identity,32);
  return true;
 }
 dh2::character_oid_cache_v1::Owner cache{};
};
void active_level_population_binding_cases(){
 ActiveLevelPopulationFixture fixture;std::string error;
 check(fixture.initialize(error),error);
 population::Bindings bindings{};
 check(population::make_active_level_bindings(fixture.level,fixture.characters,
   fixture.cache,fixture.event_owner,41,32,true,&fixture,
   nullptr,&ActiveLevelPopulationFixture::safe_bindings,bindings,error),error);
 check(bindings.object_manager==&fixture.level.object_manager()&&
       bindings.characters==&fixture.characters.source()&&
       bindings.expected_character_count==1&&bindings.roster_complete&&
       bindings.level_identity==fixture.level.snapshot().source_level);
 population::Provider provider(bindings);auto services=provider.services();
 n::ClearEnemiesPopulationSnapshotV1 snapshot{};
 check(services.read_current_level_population(services.context,fixture.event_owner,
   bindings.level_identity,7,snapshot,error)&&snapshot.loaded_match_count==1,
   "active source Level/ObjectManager/CharacterList binding did not query its canonical cached Character");
}
void objective_description_cases(){
 TextFixture fixture;const n::QuestTextServicesV1 text{&fixture,localized_text};
 std::string output,error;
 check(n::compose_objective_description_v1({41,0,-5,42},text,output,error));
 check(output=="text41\ntext42"&&fixture.requested==std::vector<std::int32_t>{41,42});
 fixture.requested.clear();
 check(n::compose_objective_description_v1({0,-1,-8},text,output,error));
 check(output.empty()&&fixture.requested.empty());
 fixture.requested.clear();fixture.empty_id=5;
 check(n::compose_objective_description_v1({8,5,3},text,output,error));
 check(output=="text8\ntext3"&&fixture.requested==std::vector<std::int32_t>({8,5,3}));
}
void kill_objective_progression(n::Owner& owner,const t::View& definitions,
 const n::Constants& constants,const std::shared_ptr<d::PlayerSavegameV1>& save){
 std::string error;const auto& quests=save->source_quest_log_b8().quests[0];
 dh2_pycst_result event_constant{};constexpr char group[]="v2QuestObjectiveType",key[]="KillXEnemies";
 check(dh2_pycst_get(&constants.view,group,sizeof(group)-1,key,sizeof(key)-1,&event_constant)==0&&event_constant.found,"ClearEnemies event constant missing");
 std::uint32_t quest_ordinal=UINT32_MAX,objective_ordinal=UINT32_MAX;
 dh2_quest_objective source_definition{};
 for(std::uint32_t quest=0;quest<definitions.count()&&quest<quests.size()&&quest_ordinal==UINT32_MAX;++quest){
  const auto* list=definitions.list(*definitions.row(quest),1);
  const auto* quest_definition=definitions.record(*definitions.row(quest));
  // Choose a real one-objective Quest with a source PostActive script so this
  // fixture can join the kill receipt to the next canonical Quest update.
  if(!list||!list->definition||list->definition->count!=1||!quest_definition||
     !quest_definition->scripts[5].size)continue;
  for(std::uint32_t objective=0;objective<list->definition->count;++objective){
   dh2_quest_objective candidate{};check(definitions.objective(*list,objective,&candidate,error));
   if(candidate.common[0]==0&&candidate.args[0]>=0&&candidate.args[0]<INT16_MAX&&
      (candidate.args[1]==-1||candidate.args[1]>=0)&&
      candidate.args[2]>0&&candidate.args[2]<=5){
    quest_ordinal=quest;objective_ordinal=objective;source_definition=candidate;break;
   }
  }
 }
 check(quest_ordinal!=UINT32_MAX&&objective_ordinal!=UINT32_MAX);
 const auto* quest_ref=quests[quest_ordinal];check(quest_ref&&owner.resolve(quest_ref));
 const auto current_level=source_definition.args[1]<0?7:source_definition.args[1];
 ScriptFixture scripts;
 n::KillEnemiesPopulationServicesV1 absent_population{};
 check(!owner.compile_kill_x_enemies_objective_from_level(
       quest_ref->identity,objective_ordinal,absent_population,&scripts,
       start_objective_script,error)&&
       error.find("current-Level population provider")!=std::string::npos,
       "live KillXEnemies Compile did not fail closed without its Level population provider");
 ClearPopulationFixture population_fixture;population_fixture.level_id=current_level;
 population_fixture.count=source_definition.args[2]+7;
 population_fixture.level_identity=0x1000;
 population_fixture.stale_owner=true;
 const n::KillEnemiesPopulationServicesV1 stale_population{
   &population_fixture,read_clear_population,population_fixture.level_identity};
 check(!owner.compile_kill_x_enemies_objective_from_level(
       quest_ref->identity,objective_ordinal,stale_population,&scripts,
       start_objective_script,error)&&error.find("stale Level owner")!=std::string::npos,
       "live KillXEnemies Compile accepted population from another event owner");
 population_fixture.stale_owner=false;
 const n::KillEnemiesPopulationServicesV1 selected_population{
   &population_fixture,read_clear_population,population_fixture.level_identity};
 check(owner.compile_kill_x_enemies_objective_from_level(quest_ref->identity,
       objective_ordinal,selected_population,&scripts,start_objective_script,error));
 check(population_fixture.selector==source_definition.args[0],
       "KillXEnemies population provider received a different source property selector");
 check(population_fixture.count!=source_definition.args[2],
       "KillXEnemies source test did not separate loaded population from row target");
 auto* quest_record=owner.resolve(quest_ref);check(quest_record);
 const auto before_activation=quest_record->state_0;
 const auto before_current=save->source_quest_log_b8().word_2c[0];
 const auto before_timestamp=quest_record->word_4;
 quest_record->state_0=6;
 save->source_quest_log_b8().word_2c[0]=static_cast<std::int32_t>(quest_ordinal);
 check(owner.transition_combat_objective_registration(quest_ref->identity,
       before_activation,6,&scripts,start_objective_script,error),"state-6 registration: "+error);
 auto deliver=[&](std::int32_t match_id){
  dh2::character_kill_quest_tail_v1::Event event{};
  event.kind=dh2::character_kill_quest_tail_v1::Kind::kill_enemies;
  event.objective_type=event_constant.value;event.character_word_25=save->character();
  event.source_word_24=static_cast<std::int16_t>(match_id);event.source_subject=-1;
  check(owner.raise_character_kill_event(event,error));return event;
 };
 const auto mismatch=deliver(source_definition.args[0]+1);check(mismatch.source_subject==-1&&scripts.started.empty());
 // Quest::SetState leaving state 6 unregisters the same Objective receiver;
 // after the source delayed-detach boundary, later Crypt kills cannot update it.
 quest_record->state_0=7;
 check(owner.transition_combat_objective_registration(quest_ref->identity,6,7,
       &scripts,start_objective_script,error));
 check(owner.flush_current_level_detaches(error));
 const auto inactive=deliver(source_definition.args[0]);
 check(inactive.source_subject==-1&&scripts.started.empty());
 quest_record->state_0=6;
 check(owner.transition_combat_objective_registration(quest_ref->identity,7,6,
       &scripts,start_objective_script,error));
 const auto required=source_definition.args[2];
 for(std::int32_t progress=1;progress<required;++progress){
  const auto event=deliver(source_definition.args[0]);
  check(event.source_subject==progress&&event.flag0==1&&event.flag1==0&&scripts.started.empty(),
        "KillXEnemies completed before its independent row target");
 }
 const auto completed=deliver(source_definition.args[0]);
 const std::vector<std::int32_t> expected_scripts=source_definition.common[2]>=0?
  std::vector<std::int32_t>{source_definition.common[2]}:std::vector<std::int32_t>{};
 check(completed.source_subject==required&&completed.flag0==1&&completed.flag1==0&&scripts.started==expected_scripts&&
       scripts.order==(source_definition.common[2]>=0?std::vector<std::string>{"script"}:std::vector<std::string>{}));
 check(save->source_quest_log_b8().word_2c[0]==static_cast<std::int32_t>(quest_ordinal),
       "KillXEnemies objective completion cleared the active current-Quest marker before Quest::Update");
 // The original Level frame reaches QuestSavegame::UpdateQuests after the
 // kill receiver has set Objective+0x14. Exercise that next source-owned step
 // against this same Save/log/Quest and prove it clears the marker and starts
 // the Quest's slot-5 PostActive script through the existing ScriptManager.
 UpdateActiveFixture update_fixture;update_fixture.canonical=quest_record;
 update_fixture.identity=quest_ref->identity;
 const n::QuestUpdateActiveServicesV1 update_services{
   &update_fixture,update_active_script_running,0x76543210,true,
   update_active_start_script};
 std::vector<n::QuestUpdateActiveResultV1> updates;
 check(owner.update_active_log(0,0,update_services,updates,error),error);
 check(updates.size()==quests.size()&&
       updates[quest_ordinal].outcome==n::QuestUpdateActiveOutcomeV1::state_transitioned&&
       updates[quest_ordinal].prior_state==6&&updates[quest_ordinal].new_state==7&&
       quest_record->state_0==7&&
       save->source_quest_log_b8().word_2c[0]==-1,
       "canonical QuestSavegame::UpdateQuests did not consume the completed Kill Objective/current marker");
 check(!updates[quest_ordinal].post_active_script.empty()&&
       update_fixture.post_active_script==updates[quest_ordinal].post_active_script&&
       update_fixture.start_argument==-1&&update_fixture.start_flag&&
       update_fixture.order==(updates[quest_ordinal].active_script.empty()?
         std::vector<std::string>{"start-script"}:
         std::vector<std::string>{"script-running","start-script"})&&
       quest_record->word_4==0x76543210,
       "completed Kill Objective did not enter the source PostActive script/timestamp continuation");
 check(owner.flush_current_level_detaches(error));
 const auto after_completion=deliver(source_definition.args[0]);
 check(after_completion.source_subject==-1&&scripts.started==expected_scripts&&
       scripts.order==(source_definition.common[2]>=0?std::vector<std::string>{"script"}:std::vector<std::string>{}));
 quest_record->state_0=before_activation;quest_record->word_4=before_timestamp;
 save->source_quest_log_b8().word_2c[0]=before_current;
}
void clear_objective_progression(const t::View& definitions,
 const n::Constants& constants,const t::Input& original){
 std::string error;
 dh2_pycst_result event_constant{};constexpr char group[]="v2QuestObjectiveType",key[]="ClearEnemies";
 check(dh2_pycst_get(&constants.view,group,sizeof(group)-1,key,sizeof(key)-1,&event_constant)==0&&event_constant.found);
 std::uint32_t quest_ordinal=UINT32_MAX,objective_ordinal=UINT32_MAX;
 dh2_quest_objective source_definition{};
 dh2_quest_span selected_span{};t::Span selected_bytes{};bool found=false;
 for(std::uint32_t quest=0;quest<definitions.count()&&!found;++quest){
  const auto* list=definitions.list(*definitions.row(quest),1);
  if(!list||!list->definition)continue;
  for(std::uint32_t objective=0;objective<list->definition->count;++objective){
   dh2_quest_objective candidate{};std::string read_error;
   check(definitions.objective(*list,objective,&candidate,read_error),"read selected pydata objective");
   if(candidate.common[0]==0&&candidate.args[0]>=0&&candidate.args[0]<INT16_MAX&&
      (candidate.args[1]==-1||candidate.args[1]>=0)){
    quest_ordinal=quest;objective_ordinal=objective;source_definition=candidate;
    check(definitions.list_record(*list,objective,&selected_span,error),"resolve selected pydata bytes");
    check(definitions.bytes(selected_span,&selected_bytes,error),"read selected pydata bytes");
    found=true;break;
   }
  }
 }
 check(found&&selected_span.size>=12&&selected_bytes.data[0]==0,
       "selected cache lacks a mutable KillX source objective for ClearEnemies compatibility coverage");
 // The shipped campaign cache contains no selector-1 rows. For this focused
 // compatibility regression, clone one real selected record and change only
 // its type word to the recovered ClearEnemies selector. All other arguments,
 // factories, constants, Save and event paths remain selected implementations.
 auto projected_bytes=std::make_shared<std::vector<std::uint8_t>>(
   original.table.bytes,original.table.bytes+original.table.size);
 for(unsigned byte=0;byte<4;++byte)(*projected_bytes)[selected_span.offset+byte]=byte?0:1;
 t::Input projected=original;
 check(dh2_quests_open(&projected.table,projected_bytes->data(),
       std::uint32_t(projected_bytes->size()))==0,"open ClearEnemies selector projection");
 projected.packed_owner=projected_bytes;
 t::Owner projected_table;check(projected_table.load(projected,error),"load ClearEnemies selector projection");
 const auto clear_definitions=projected_table.borrow();
 auto clear_save=std::make_shared<d::PlayerSavegameV1>();
 clear_save->set_character(UINT64_C(0x12345678000000c3));
 n::Owner clear_owner(clear_save,clear_definitions,constants);
 check(clear_owner.initialize(0,error),"initialize selected ClearEnemies quest owner");
 const auto* quest_ref=clear_save->source_quest_log_b8().quests[0][quest_ordinal];
 check(quest_ref&&clear_owner.resolve(quest_ref),"ClearEnemies canonical Quest missing");
 const auto current_level=source_definition.args[1]<0?7:source_definition.args[1];
 ScriptFixture scripts;
 n::ClearEnemiesPopulationServicesV1 absent_population{};
 check(!clear_owner.compile_clear_enemies_objective_from_level(
       quest_ref->identity,objective_ordinal,absent_population,&scripts,
       start_objective_script,error)&&
       error.find("current-Level population provider")!=std::string::npos,
       "live ClearEnemies Compile did not fail closed without its Level population provider");
 check(!clear_owner.register_clear_enemies_objective(
       quest_ref->identity,objective_ordinal,&scripts,start_objective_script,error),
       "failed live ClearEnemies Compile mutated/registered the canonical Objective");
 ClearPopulationFixture population_fixture;population_fixture.level_id=current_level;
 population_fixture.level_identity=0x2000;
 population_fixture.stale_owner=true;
 const n::ClearEnemiesPopulationServicesV1 stale_population{
   &population_fixture,read_clear_population,population_fixture.level_identity};
 check(!clear_owner.compile_clear_enemies_objective_from_level(
       quest_ref->identity,objective_ordinal,stale_population,&scripts,
       start_objective_script,error)&&error.find("stale Level owner")!=std::string::npos,
       "live ClearEnemies Compile accepted population from another event owner");
 check(!clear_owner.register_clear_enemies_objective(
       quest_ref->identity,objective_ordinal,&scripts,start_objective_script,error),
       "stale ClearEnemies population query mutated/registered the canonical Objective");
 population_fixture.stale_owner=false;
 population_fixture.stale_level=true;
 const n::ClearEnemiesPopulationServicesV1 stale_level_population{
   &population_fixture,read_clear_population,population_fixture.level_identity};
 check(!clear_owner.compile_clear_enemies_objective_from_level(
       quest_ref->identity,objective_ordinal,stale_level_population,&scripts,
       start_objective_script,error)&&error.find("stale Level owner")!=std::string::npos,
       "live ClearEnemies Compile accepted a stale source-Level identity");
 population_fixture.stale_level=false;
 const n::ClearEnemiesPopulationServicesV1 selected_population{
   &population_fixture,read_clear_population,population_fixture.level_identity};
 check(clear_owner.compile_clear_enemies_objective_from_level(
       quest_ref->identity,objective_ordinal,selected_population,&scripts,
       start_objective_script,error),"compile ClearEnemies projection: "+error);
 check(population_fixture.selector==source_definition.args[0],
       "ClearEnemies population provider received a different source property selector");
 auto* clear_quest=clear_owner.resolve(quest_ref);check(clear_quest);
 const auto clear_prior_state=clear_quest->state_0;clear_quest->state_0=6;
 check(clear_owner.transition_combat_objective_registration(quest_ref->identity,
       clear_prior_state,6,&scripts,start_objective_script,error),
       "state-6 ClearEnemies registration: "+error);
 auto deliver=[&](std::int32_t match_id){
  k::Event event{};event.kind=k::Kind::clear_enemies;event.objective_type=event_constant.value;
  event.character_word_25=clear_save->character();event.source_word_24=static_cast<std::int16_t>(match_id);
  event.source_subject=-1;check(clear_owner.raise_character_kill_event(event,error));return event;
 };
 clear_quest->state_0=7;
 check(clear_owner.transition_combat_objective_registration(quest_ref->identity,6,7,
       &scripts,start_objective_script,error),"state-6 ClearEnemies unregistration: "+error);
 check(clear_owner.flush_current_level_detaches(error),"flush ClearEnemies unregister");
 check(deliver(source_definition.args[0]).source_subject==-1,"unregistered ClearEnemies receiver still ran");
 clear_quest->state_0=6;
 check(clear_owner.transition_combat_objective_registration(quest_ref->identity,7,6,
       &scripts,start_objective_script,error),"re-register ClearEnemies at state 6: "+error);
 const auto mismatch=deliver(source_definition.args[0]+1);
 check(mismatch.source_subject==-1&&scripts.started.empty());
 const auto first=deliver(source_definition.args[0]);
 check(first.source_subject==1&&first.flag0==1&&first.flag1==0&&scripts.started.empty());
 const auto second=deliver(source_definition.args[0]);
 const std::vector<std::int32_t> expected_scripts=source_definition.common[2]>=0?
  std::vector<std::int32_t>{source_definition.common[2]}:std::vector<std::int32_t>{};
 check(second.source_subject==2&&second.flag0==1&&second.flag1==0&&scripts.started==expected_scripts);
 check(clear_owner.flush_current_level_detaches(error));
 const auto third=deliver(source_definition.args[0]);
 check(third.source_subject==-1&&scripts.started==expected_scripts);
 check(clear_owner.close(error),"close ClearEnemies projection owner");

 // ClearEnemyTemplate has the adjacent recovered factory/event route, but the
 // selected cache also has no authored selector-11 row. A synthetic selector
 // checks only that production compilation refuses to guess without a live
 // TestCharTemplate population provider; it is not gameplay parity evidence.
 auto template_bytes=std::make_shared<std::vector<std::uint8_t>>(
   original.table.bytes,original.table.bytes+original.table.size);
 for(unsigned byte=0;byte<4;++byte)(*template_bytes)[selected_span.offset+byte]=byte?0:11;
 t::Input template_input=original;
 check(dh2_quests_open(&template_input.table,template_bytes->data(),
       std::uint32_t(template_bytes->size()))==0,"open ClearEnemyTemplate selector projection");
 template_input.packed_owner=template_bytes;
 t::Owner template_table;check(template_table.load(template_input,error),"load ClearEnemyTemplate selector projection");
 auto template_save=std::make_shared<d::PlayerSavegameV1>();
 template_save->set_character(UINT64_C(0x12345678000000c4));
 n::Owner template_owner(template_save,template_table.borrow(),constants);
 check(template_owner.initialize(0,error),"initialize ClearEnemyTemplate selector owner");
 const auto* template_quest=template_save->source_quest_log_b8().quests[0][quest_ordinal];
 check(template_quest&&template_owner.resolve(template_quest),"ClearEnemyTemplate canonical Quest missing");
 n::ClearEnemyTemplatePopulationServicesV1 absent_template_population{};
 check(!template_owner.compile_clear_enemy_template_objective_from_level(
       template_quest->identity,objective_ordinal,absent_template_population,
       &scripts,start_objective_script,error)&&
       error.find("current-Level population provider")!=std::string::npos,
       "live ClearEnemyTemplate Compile did not fail closed without its Level population provider");
 check(!template_owner.register_clear_enemy_template_objective(
       template_quest->identity,objective_ordinal,&scripts,start_objective_script,error),
       "failed live ClearEnemyTemplate Compile mutated/registered the canonical Objective");
 check(template_owner.close(error),"close ClearEnemyTemplate selector owner");
}
void fresh_row_failure_and_retry(const std::shared_ptr<d::PlayerSavegameV1>& save,
 const t::Input& original,const t::View& original_view,const n::Constants& constants){
 auto* const canonical_log0=&save->source_quest_log_b8();
 auto* const canonical_log1=&save->source_quest_log_118();
 // This real row has three objectives. Reject its last selector so the
 // unpublished Quest owns two actual Objective factory allocations already.
 unsigned bad_row=original_view.count();
 for(unsigned row=1;row<original_view.count();++row)
  if(original_view.record(*original_view.row(row))->lists[1].count>1){bad_row=row;break;}
 check(bad_row==47);
 const auto* row=original_view.row(bad_row);const auto* list=original_view.list(*row,1);
 check(list&&list->definition->count==3);const auto bad_index=list->definition->count-1;
 dh2_quest_span target{};std::string error;
 check(original_view.list_record(*list,bad_index,&target,error)&&target.size>=12);
 t::Span original_bytes;check(original_view.bytes(target,&original_bytes,error));
 check(original_bytes.data[0]!=13);
 auto mutated=std::make_shared<std::vector<std::uint8_t>>(original.table.bytes,original.table.bytes+original.table.size);
 const std::weak_ptr<const void> retained_generation=mutated;
 for(unsigned byte=0;byte<4;++byte)(*mutated)[target.offset+byte]=byte?0:13;
 t::Input failed=original;
 check(dh2_quests_open(&failed.table,mutated->data(),std::uint32_t(mutated->size()))==0);failed.packed_owner=mutated;
 t::Owner generations;check(generations.load(failed,error));auto failed_view=generations.borrow();
 dh2_quest_objective rejected{};
 check(failed_view.objective(*failed_view.list(*failed_view.row(bad_row),1),bad_index,&rejected,error)&&rejected.common[0]==13);
 auto failed_owner=std::make_unique<n::Owner>(save,failed_view,constants);
 check(!failed_owner->initialize(0,error));
 check(error.find("difficulty 0 row "+std::to_string(bad_row))!=std::string::npos);
 check(failed_owner->receipt().published[0]==bad_row&&failed_owner->receipt().published[1]==0);
 check(failed_owner->receipt().reinitialized[0]==bad_row&&failed_owner->receipt().destroyed[0]==0);
 check(&save->source_quest_log_b8()==canonical_log0&&&save->source_quest_log_118()==canonical_log1);
 check(canonical_log0->quests[0].size()==64&&canonical_log0->quests[1].empty()&&canonical_log0->quests[2].empty());
 for(unsigned index=0;index<64;++index){
  auto* ref=canonical_log0->quests[0][index];
  if(index>=bad_row){check(ref==nullptr);continue;}
  auto* record=failed_owner->resolve(ref);
  check(ref&&record&&&record->ref==ref&&ref->fields==&record->fields);
  check(record->py_data_68==failed_view.row(index)&&record->fields.row_8==int(index));
  check(record->fields.character_60==save->character());
 }
 for(const auto& vector:canonical_log1->quests)check(vector.empty());
 // A reload publishes a valid generation while the failed owner still needs
 // its old real definitions/names to destroy constructed unpublished children.
 check(generations.load(original,error));const auto retry_view=generations.borrow();
 check(retry_view.row(0)!=failed_view.row(0));
 auto* first=failed_owner->resolve(canonical_log0->quests[0][0]);const auto* old_row=first->py_data_68;
 failed_view={};failed.packed_owner.reset();mutated.reset();
 check(!retained_generation.expired()&&first->py_data_68==old_row);
 check(failed_owner->close(error));
 check(failed_owner->receipt().destroyed[0]==bad_row&&failed_owner->receipt().destroyed[1]==0);
 check(failed_owner->receipt().unpublished_destroyed==1);
 for(auto* log:{canonical_log0,canonical_log1})for(const auto& vector:log->quests)check(vector.empty());
 check(failed_owner->close(error)&&failed_owner->receipt().unpublished_destroyed==1);
 check(!failed_owner->initialize(0,error));check(!retained_generation.expired());
 failed_owner.reset();check(retained_generation.expired());
 // Retry on the same sole Save logs, using the valid retained generation and
 // exactly the same real selected factories; no alternate gameplay providers.
 {n::Owner retry(save,retry_view,constants);check(retry.initialize(0,error)&&retry.initialize(1,error));
  check(retry.receipt().published[0]==192&&retry.receipt().published[1]==192);
  auto* record=retry.resolve(canonical_log0->quests[0][bad_row]);
  check(record&&record->py_data_68==retry_view.row(bad_row)&&record->fields.character_60==save->character());
  check(retry.close(error)&&retry.receipt().destroyed[0]==192&&retry.receipt().destroyed[1]==192&&retry.receipt().unpublished_destroyed==0);}
 for(auto* log:{canonical_log0,canonical_log1})for(const auto& vector:log->quests)check(vector.empty());
}
int main(int argc,char** argv){try{
 // Level::Update(bool) source dispatch: an active gameplay frame with a local
 // Character invokes SG_Update(false). Level+0x148 only gates the optional
 // pre-SG StartScript branch; its sentinel/non-sentinel paths both converge.
 {
  LevelSGUpdateFixture fixture;level_sg::ResultV1 result{};std::string error;
  const level_sg::ServicesV1 services{&fixture,level_sg_in_game_view,
    level_sg_start_script,level_sg_character_update,level_sg_execute_scripts};
  {
   dh2::source_level_owner_v1::Snapshot source{};
   source.phase=dh2::source_level_owner_v1::Phase::active;
   source.source_level=UINT64_C(0x3101);source.player_character=UINT64_C(0x3102);
   source.pending_script_id=-1;source.pending_script_id_known=true;
   level_sg::FrameV1 projected{};
   check(level_sg::frame_from_source_level_v1(source,true,projected,error)&&
         projected.level_identity==source.source_level&&
         projected.local_character_identity==source.player_character&&
         projected.pending_script_id_known&&projected.pending_script_id==-1,
         "SourceLevelOwner constructor value did not project with provenance");
   level_sg::ResultV1 projected_result{};
   check(level_sg::dispatch_v1(projected,services,projected_result,error)&&
         fixture.order==std::vector<std::string>{"character-sg-update","execute-scripts"},
         "projected SourceLevelOwner did not preserve SG_Update/ScriptManager order");
   fixture.order.clear();source.pending_script_id_known=false;
   check(level_sg::frame_from_source_level_v1(source,true,projected,error)&&
         !projected.pending_script_id_known&&
         !level_sg::dispatch_v1(projected,services,projected_result,error)&&
         error.find("source-owned Level+0x148")!=std::string::npos&&fixture.order.empty(),
         "unknown borrowed Level field was converted into the source constructor sentinel");
  }
  fixture.order.clear();
  level_sg::FrameV1 frame{};
  check(level_sg::dispatch_v1(frame,services,result,error)&&
        result.outcome==level_sg::OutcomeV1::inactive_level_frame&&fixture.order.empty(),
        "inactive Level frame reached SG_Update");
  frame.level_identity=UINT64_C(0x1001);frame.active_gameplay_update=true;
  check(!level_sg::dispatch_v1(frame,services,result,error)&&
        error.find("source-owned Level+0x148")!=std::string::npos&&fixture.order.empty(),
        "active Level frame accepted an unknown +0x148 value");
  frame={};frame.level_identity=UINT64_C(0x1001);frame.active_gameplay_update=true;
  frame.pending_script_id=-1;frame.pending_script_id_known=true;
  check(level_sg::dispatch_v1(frame,services,result,error)&&
        result.outcome==level_sg::OutcomeV1::no_local_character&&
        fixture.order==std::vector<std::string>{"execute-scripts"}&&
        result.script_manager_executed,
        "missing local Character suppressed the Level ScriptManager tail");
  fixture.order.clear();
  frame.local_character_identity=UINT64_C(0x2002);
  check(level_sg::dispatch_v1(frame,services,result,error)&&
        result.outcome==level_sg::OutcomeV1::character_updated&&
        fixture.order==std::vector<std::string>{"character-sg-update","execute-scripts"}&&
        !fixture.checkpoint&&fixture.level==frame.level_identity&&
        fixture.character==frame.local_character_identity&&result.script_manager_executed,
        "sentinel branch SG_Update/ScriptManager order diverged");
  fixture.order.clear();frame.pending_script_id=73;fixture.in_game_view=false;
  check(level_sg::dispatch_v1(frame,services,result,error)&&
        fixture.order==std::vector<std::string>{"game-view","character-sg-update","execute-scripts"}&&
        !result.pending_script_started,
        "out-of-game pending-script branch or ExecuteAllScripts ordering changed");
  fixture.order.clear();fixture.in_game_view=true;
  check(level_sg::dispatch_v1(frame,services,result,error)&&
        fixture.order==std::vector<std::string>{"game-view","start-script","character-sg-update","execute-scripts"}&&
        result.pending_script_started&&fixture.script==73&&fixture.argument==-1&&
        !fixture.script_flag&&!fixture.checkpoint,
        "non-sentinel Level script/SG_Update source ordering or arguments changed");
  fixture.order.clear();frame.local_character_identity=0;
  check(level_sg::dispatch_v1(frame,services,result,error)&&
        result.outcome==level_sg::OutcomeV1::no_local_character&&
        result.pending_script_started&&
        fixture.order==std::vector<std::string>{"game-view","start-script","execute-scripts"},
        "Level pending script was incorrectly suppressed by a missing local Character");
 }
 objective_description_cases();
 current_level_character_population_cases();
 active_level_population_binding_cases();
 check(argc==2);const std::string cache=argv[1];const auto packed=read(cache+"/v2quests_pyarray.bin"),names=read(cache+"/v2quests_pyarraynames.bin"),constants=read(cache+"/v2quests_pycst.bin");
 t::Input input;check(dh2_quests_open(&input.table,packed->data(),std::uint32_t(packed->size()))==0);input.packed_owner=packed;input.names=names->data();input.names_size=names->size();input.names_owner=names;
 t::Owner table;std::string error;check(table.load(input,error));const auto view=table.borrow();check(view.count()==64);
 const auto condition_root=cache+"/../original-cache/data/pydata/";
 const auto condition_packed=read(condition_root+"v2conditions_pyarray.bin"),
  condition_names=read(condition_root+"v2conditions_pyarraynames.bin"),
  condition_schema=read(condition_root+"v2conditions_pystructnames.bin"),
  condition_constants=read(condition_root+"v2conditions_pycst.bin");
 dh2::data::condition_data_v1::Table condition_table;
 check(dh2::data::condition_data_v1::load(
  {condition_packed->data(),std::uint32_t(condition_packed->size())},
  {condition_names->data(),std::uint32_t(condition_names->size())},
  {condition_schema->data(),std::uint32_t(condition_schema->size())},
  {condition_constants->data(),std::uint32_t(condition_constants->size())},
  condition_table,error));
 n::Constants constants_input;check(dh2_pycst_open(&constants_input.view,constants->data(),std::uint32_t(constants->size()))==0);constants_input.owner=constants;
 auto save=std::make_shared<d::PlayerSavegameV1>();save->set_character(UINT64_C(0x12345678000000a1));
 auto owner=std::make_unique<n::Owner>(save,view,constants_input);
 check(owner->initialize(0,error));check(owner->initialize(1,error));
 check(owner->receipt().published[0]==192&&owner->receipt().published[1]==192);
 // The retained gameplay Character, Save, and QEST owner can supply one
 // coherent SG_Update provider context. The Level Character callback must
 // call its same-owner Quest update once before Level executes the scripts.
 {
  LevelSGUpdateFixture fixture;fixture.qest_owner=owner.get();fixture.save=save.get();
  fixture.update_active_quests=true;fixture.application_time=0x76543210;
  level_quest::SourceLevel source_level{};
  source_level.phase=dh2::source_level_owner_v1::Phase::active;
  source_level.source_level_state=38;source_level.source_level=UINT64_C(0x1001);
  source_level.player_character=save->character();source_level.quest_owner=owner.get();
  source_level.canonical_player_savegame=reinterpret_cast<std::uintptr_t>(save.get());
  fixture.source_level=&source_level;
  std::vector<n::QuestUpdateActiveResultV1> quest_updates;
  fixture.quest_updates=&quest_updates;
  UpdateActiveFixture update_fixture;
  fixture.quest_scripts={&update_fixture,update_active_script_running,
                         update_active_start_script};
  const level_sg::ServicesV1 services{&fixture,level_sg_in_game_view,
    level_sg_start_script,level_sg_character_update,level_sg_execute_scripts};
  level_sg::FrameV1 frame{};frame.level_identity=UINT64_C(0x1001);
  frame.active_gameplay_update=true;frame.pending_script_id=-1;
  frame.pending_script_id_known=true;frame.local_character_identity=save->character();
  level_sg::ResultV1 result{};
  check(level_sg::dispatch_v1(frame,services,result,error),error);
  check(result.outcome==level_sg::OutcomeV1::character_updated&&
        fixture.character==save->character()&&fixture.level==frame.level_identity&&
        owner->owns_save(save.get())&&!fixture.checkpoint,
        "Level SG_Update adapter Character/Save/QEST owner contract diverged");
  check(fixture.quest_update_calls==1,
        "Level Character SG_Update must invoke canonical QEST UpdateActive once");
  check(quest_updates.size()==64&&
        quest_updates.size()==save->source_quest_log_b8().quests[0].size(),
        "Level Character SG_Update did not update the selected canonical QEST log");
  check(fixture.order==std::vector<std::string>{"character-sg-update",
          "quest-active-update","execute-scripts"},
        "canonical QEST UpdateActive did not run before Level ExecuteAllScripts");
  check(source_level.quest_owner==owner.get()&&
        source_level.canonical_player_savegame==reinterpret_cast<std::uintptr_t>(save.get()),
        "Level SourceLevelOwner did not retain its same-Save QEST identity");
 }
 // Quest::UpdateActive evaluates the exact same Save-owned ObjectiveList,
 // asks the current Level's one ScriptManager about source slot 0, then
 // requests the canonical SetState(7) owner. It never writes a second state.
 {
  std::uint32_t active_row=UINT32_MAX;
  for(std::uint32_t row=0;row<view.count();++row){
   const auto* objectives=view.list(*view.row(row),1);
   const auto* definition=view.record(*view.row(row));
   t::Span script_bytes{};std::string ignored;
   if(objectives&&objectives->definition&&definition&&definition->scripts[0].size&&
      view.bytes(definition->scripts[0],&script_bytes,ignored)&&script_bytes.data&&
      std::string(reinterpret_cast<const char*>(script_bytes.data),script_bytes.size).find('.')!=std::string::npos){
    if(objectives->definition->count>0&&active_row==UINT32_MAX)active_row=row;
   }
  }
  check(active_row!=UINT32_MAX,"cache lacks scripted Quest with a nonempty ObjectiveList");
  const n::QuestUpdateActiveServicesV1 services{nullptr,
    update_active_script_running,0x12345678,true,update_active_start_script};
  auto* active_ref=save->source_quest_log_b8().quests[0][active_row];
  auto* active=owner->resolve(active_ref);check(active);
  const auto prior_state=active->state_0;const auto prior_time=active->word_4;
  const auto prior_current=save->source_quest_log_b8().word_2c[0];
  active->state_0=6;save->source_quest_log_b8().word_2c[0]=active_row;
  UpdateActiveFixture fixture;fixture.canonical=active;fixture.identity=active_ref->identity;
  auto active_services=services;active_services.context=&fixture;
  auto& objective_list=view.list(*view.row(active_row),1)->definition;
  n::QuestUpdateActiveResultV1 result{};
  check(owner->update_active(active_ref->identity,0,0,active_services,result,error),error);
  check(result.outcome==n::QuestUpdateActiveOutcomeV1::objectives_pending&&
        fixture.order.empty()&&active->state_0==6,
        "Quest::UpdateActive queried scripts before ObjectiveList::Eval succeeded");
  ScriptFixture objective_start;
  for(std::uint32_t ordinal=0;ordinal<objective_list->count;++ordinal)
   check(owner->complete_objective(active_ref->identity,ordinal,&objective_start,
                                   start_objective_script,error),error);
  const auto completion_scripts=objective_start.started.size();
  for(std::uint32_t ordinal=0;ordinal<objective_list->count;++ordinal)
   check(owner->complete_objective(active_ref->identity,ordinal,&objective_start,
                                   start_objective_script,error),error);
  check(objective_start.started.size()==completion_scripts&&
        completion_scripts<=objective_list->count,
        "Objective::SetIsCompleted was not idempotent or started duplicate scripts");
  fixture.running=true;
  check(owner->update_active(active_ref->identity,0,0,active_services,result,error),error);
  check(result.outcome==n::QuestUpdateActiveOutcomeV1::completion_script_running&&
        fixture.order==std::vector<std::string>{"script-running"}&&active->state_0==6,
        "Quest::UpdateActive did not honor ScriptManager::IsScriptRunning slot 0");
  fixture.running=false;fixture.order.clear();
  check(owner->update_active(active_ref->identity,0,0,active_services,result,error),error);
  check(result.outcome==n::QuestUpdateActiveOutcomeV1::state_transitioned&&
        result.prior_state==6&&result.new_state==7&&active->state_0==7&&
        fixture.order==(result.post_active_script.empty()?std::vector<std::string>{"script-running"}:
                       std::vector<std::string>{"script-running","start-script"})&&
        fixture.queried_script==result.active_script&&
        fixture.post_active_script==result.post_active_script,
        std::string("Quest::UpdateActive did not dispatch source SetState(7) after its script guard: outcome=")+
        std::to_string(static_cast<int>(result.outcome))+" prior="+std::to_string(result.prior_state)+
        " new="+std::to_string(result.new_state)+" state="+std::to_string(active->state_0)+
        " activeScript='"+result.active_script+"' postScript='"+result.post_active_script+
        "' queried='"+fixture.queried_script+"' callbackPost='"+fixture.post_active_script+
        "' orders="+std::to_string(fixture.order.size()));
  check(active->word_4==0x12345678&&save->source_quest_log_b8().word_2c[0]==-1&&
        (result.post_active_script.empty()||
         (fixture.start_argument==-1&&fixture.start_flag)),
        "Quest::SetState(PostActive) did not store timestamp/clear current quest/start source slot 5");
  active->state_0=prior_state;active->word_4=prior_time;
  save->source_quest_log_b8().word_2c[0]=prior_current;
 }
 // Row 47 has three retained kind-1 objective definitions. The Owner must
 // project their actual canonical child order through the injected existing
 // localization service without creating another ObjectiveList.
 const auto* objective_list=view.list(*view.row(47),1);check(objective_list&&objective_list->definition->count==3);
 std::vector<std::int32_t> objective_ids;objective_ids.reserve(3);
 for(std::uint32_t index=0;index<objective_list->definition->count;++index){
  dh2_quest_objective source{};check(view.objective(*objective_list,index,&source,error));
  objective_ids.push_back(source.common[1]);
 }
 TextFixture objective_text;const n::QuestTextServicesV1 objective_services{&objective_text,localized_text};
 std::string expected_description;check(n::compose_objective_description_v1(
      objective_ids,objective_services,expected_description,error));
 objective_text.requested.clear();n::QuestReadV1 described;
 check(owner->read_quest(0,0,47,described,error,&objective_services));
 check(described.objective_description==expected_description);
 std::vector<std::int32_t> expected_requests;
 for(const auto id:objective_ids)if(id>0)expected_requests.push_back(id);
 check(objective_text.requested==expected_requests);
 std::set<std::uintptr_t> identities;std::uint32_t instances=0;
 for(auto* log:{&save->source_quest_log_b8(),&save->source_quest_log_118()}){
  check(log->character_5c==save->character());
 for(unsigned difficulty=0;difficulty<3;++difficulty){
   check(log->quests[difficulty].size()==64&&log->byte_28[difficulty]==0);
   for(unsigned row=0;row<64;++row){
    auto* ref=log->quests[difficulty][row];check(ref&&ref->identity&&identities.insert(ref->identity).second);
    auto* record=owner->resolve(ref);check(record&&&record->ref==ref&&ref->fields==&record->fields);
    const auto* definition=view.record(*view.row(row));
    check(record->fields.character_60==save->character());check(record->fields.row_8==int(row));
    check(record->fields.definition_name_14==reinterpret_cast<std::uintptr_t>(view.definition_name(row)));
    check(record->py_data_68==view.row(row)&&record->state_0==definition->state&&record->word_c==definition->act);
    check(record->difficulty_10==int(difficulty)&&record->action_18&&record->action_1c);
    check(*record->action_18->character_10==save->character()&&*record->action_1c->character_10==save->character());
    check(record->byte_64==0);++instances;
   }
  }
 }
 dh2_pycst_result debug_priority{};
 constexpr char quest_priority_group[]="v2QuestPriority",debug_name[]="Debug";
 check(dh2_pycst_get(&constants_input.view,quest_priority_group,
       sizeof(quest_priority_group)-1,debug_name,sizeof(debug_name)-1,&debug_priority)==0&&
       debug_priority.found);
 // Quest Log query data borrows the actual published vectors and preserves
 // source ordinals, state, priority and all four authored string IDs.
 for(std::uint32_t difficulty=0;difficulty<3;++difficulty){
  const auto& source=save->source_quest_log_b8().quests[difficulty];
  check(source.size()==64);
  for(std::uint32_t ordinal=0;ordinal<source.size();++ordinal){
   n::QuestReadV1 actual;check(owner->read_quest(0,difficulty,ordinal,actual,error));
   auto* record=owner->resolve(source[ordinal]);const auto* row=view.record(*view.row(ordinal));
   check(record&&actual.id==int(ordinal)&&actual.state==record->state_0&&
         actual.priority==row->priority);
   for(std::size_t text=0;text<actual.text_ids.size();++text)
    check(actual.text_ids[text]==row->ids[text]);
   const bool open=actual.priority!=debug_priority.value&&actual.state>=6&&actual.state<=12;
   const bool closed=actual.priority!=debug_priority.value&&actual.state>12;
   check(!(open&&closed));
   if(actual.priority==debug_priority.value)check(!open&&!closed);
   else if(actual.state>=6&&actual.state<=12)check(open&&!closed);
   else if(actual.state>12)check(!open&&closed);
   else check(!open&&!closed);
  }
 }
 // SG_GetQuestByID routes to the online/offline embedded log and uses the
 // live process difficulty. Exercise the callback shape consumed by conditions.
 auto* q27_offline=owner->resolve(save->source_quest_log_b8().quests[0][27]);
 auto* q27_online=owner->resolve(save->source_quest_log_118().quests[0][27]);
 auto* q8_hard=owner->resolve(save->source_quest_log_b8().quests[1][8]);
 check(q27_offline&&q27_online&&q8_hard);
 const auto old_offline=q27_offline->state_0,old_online=q27_online->state_0,
            old_hard=q8_hard->state_0;
 q27_offline->state_0=31;q27_online->state_0=42;q8_hard->state_0=55;
 std::int32_t live_difficulty=0,quest_state=-999;std::uint8_t online=0;
 n::QuestLookupContextV1 lookup{owner.get(),&live_difficulty,&online};
 check(n::quest_state_lookup_v1(&lookup,27,&quest_state)&&quest_state==31&&
       lookup.status==n::QuestLookupStatusV1::found);
 online=1;check(n::quest_state_lookup_v1(&lookup,27,&quest_state)&&quest_state==42);
 online=0;live_difficulty=1;
 check(n::quest_state_lookup_v1(&lookup,8,&quest_state)&&quest_state==55);
 quest_state=-777;
 check(!n::quest_state_lookup_v1(&lookup,64,&quest_state)&&quest_state==-777&&
       lookup.status==n::QuestLookupStatusV1::missing);
 const auto* rene=dh2::data::condition_data_v1::find(condition_table,"RENE_FOLLOW");
 const auto* survivors=dh2::data::condition_data_v1::find(condition_table,
      "IsBefore_Gothicus2Survivors");
 check(rene&&survivors);
 q8_hard->state_0=12;live_difficulty=1;
 dh2::data::condition_data_v1::EvalResult condition_result{};
 check(dh2::data::condition_data_v1::evaluate(*rene,false,
      n::quest_state_lookup_v1,&lookup,&condition_result)==
      dh2::data::condition_data_v1::Status::complete&&condition_result.value);
 q27_offline->state_0=11;live_difficulty=0;
 check(dh2::data::condition_data_v1::evaluate(*survivors,false,
      n::quest_state_lookup_v1,&lookup,&condition_result)==
      dh2::data::condition_data_v1::Status::complete&&condition_result.value);
 online=1;
 check(dh2::data::condition_data_v1::evaluate(*survivors,false,
      n::quest_state_lookup_v1,&lookup,&condition_result)==
      dh2::data::condition_data_v1::Status::complete&&!condition_result.value);
 online=0;
 live_difficulty=0;q27_offline->state_0=old_offline;
 q27_online->state_0=old_online;q8_hard->state_0=old_hard;
 kill_objective_progression(*owner,view,constants_input,save);
 clear_objective_progression(view,constants_input,input);
 n::QuestReadV1 untouched;untouched.id=777;
 check(!owner->read_quest(0,0,64,untouched,error)&&untouched.id==777);
 check(!owner->read_quest(2,0,0,untouched,error)&&untouched.id==777);
 check(instances==384&&owner->receipt().constant_queries==1612);
 const auto constant_queries=owner->receipt().constant_queries;
 bool rejected=false;try{n::Owner other(save,view,constants_input);}catch(const std::invalid_argument&){rejected=true;}check(rejected);
 check(!owner->initialize(2,error));check(owner->receipt().published[0]==192&&owner->receipt().published[1]==192);
 // No native Compile/marker/action cleanup provider is substituted. ReInit of
 // state2 reaches its real cleanup boundary, fails and retains the same Quest.
 auto* first=owner->resolve(save->source_quest_log_b8().quests[0][0]);const auto* action=first->action_18;
 first->state_0=2;check(!owner->initialize(0,error));check(first->state_0==2&&first->action_18==action);
 first->state_0=view.record(*view.row(0))->state;
 QuestCompileProbe compile_probe{save->character()};
 n::QuestCompileServicesV1 compile_services{&compile_probe,
   quest_compile_difficulty,quest_compile_member};
 d::quest_compile_v1::Result compile_result{};
 auto& canonical_compile_log=save->source_quest_log_b8();
 check(canonical_compile_log.byte_28[0]==0);
 check(!owner->compile_quests(0,false,compile_services,compile_result,error));
 check(compile_probe.difficulty_calls>0&&canonical_compile_log.byte_28[0]==1);
 const auto compile_calls=compile_probe.member_calls;
 check(compile_calls>0);
 check(owner->compile_quests(0,false,compile_services,compile_result,error));
 check(compile_probe.member_calls==compile_calls);
 check(!owner->compile_quests(0,true,compile_services,compile_result,error));
 check(compile_probe.member_calls>compile_calls&&canonical_compile_log.byte_28[0]==1);
 canonical_compile_log.byte_28[0]=0;
 check(owner->close(error));check(owner->receipt().destroyed[0]==192&&owner->receipt().destroyed[1]==192);
 for(auto* log:{&save->source_quest_log_b8(),&save->source_quest_log_118()})for(const auto& vector:log->quests)check(vector.empty());
 check(owner->close(error));check(!owner->initialize(0,error));
 owner.reset();
 // A valid source row absent from the Save vector would make the original
 // getter call CompileQuests(false); this adapter reports the boundary and
 // leaves the vector/state untouched instead of compiling implicitly.
 {n::Owner pending(save,view,constants_input);std::int32_t untouched_state=8765;
  n::QuestLookupContextV1 lookup{&pending,&live_difficulty,&online};
  check(!n::quest_state_lookup_v1(&lookup,27,&untouched_state)&&
        untouched_state==8765&&lookup.status==n::QuestLookupStatusV1::compile_required);
 }
 // RAII invokes the same complete source destructor closure while the Save
 // and its sole factory/provider context still exist.
 {n::Owner restart(save,view,constants_input);check(restart.initialize(0,error));}
 for(const auto& vector:save->source_quest_log_b8().quests)check(vector.empty());
 fresh_row_failure_and_retry(save,input,view,constants_input);
 n::Constants absent=constants_input;absent.owner.reset();rejected=false;
 try{n::Owner invalid(save,view,absent);}catch(const std::invalid_argument&){rejected=true;}check(rejected);
 std::printf("{\"validation\":\"PASS\",\"checks\":%u,\"actual_quest_instances\":%u,\"constant_queries\":%u,\"same_save_logs\":true,\"genuine_selected_factories\":true,\"kill_objective_progression\":true,\"clear_objective_progression\":true,\"explicit_destructor_and_raii\":true,\"fresh_row_failure\":{\"row\":47,\"objective_index\":2,\"published_prefix\":47,\"unpublished_destroyed\":1,\"valid_retry_instances\":384,\"retained_generation_lifetime\":true},\"live_gameplay\":false}\n",checks,instances,constant_queries);return 0;
 }catch(const std::exception& e){std::fprintf(stderr,"%s\n",e.what());return 1;}}
