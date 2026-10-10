#include "../app/src/main/cpp/native_quest_owner.hpp"
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace d=dh2::data;
namespace n=dh2::native::quests;
namespace t=d::quest_table_bindings_v1;

std::shared_ptr<const std::vector<std::uint8_t>> read(const std::string& path){
 std::ifstream f(path,std::ios::binary|std::ios::ate);
 if(!f)throw std::runtime_error("open "+path);
 auto bytes=std::make_shared<std::vector<std::uint8_t>>(std::size_t(f.tellg()));
 f.seekg(0);f.read(reinterpret_cast<char*>(bytes->data()),std::streamsize(bytes->size()));
 if(!f)throw std::runtime_error("read "+path);
 return bytes;
}
struct Fixture {std::int32_t level_id=0,selector=-1,count=0;std::vector<std::int32_t> scripts;};
bool population(void* raw,std::uintptr_t expected_owner,std::uintptr_t expected_level,
 std::int32_t property,n::ClearEnemiesPopulationSnapshotV1& out,std::string& error){
 auto& f=*static_cast<Fixture*>(raw);f.selector=property;
 out.event_owner_identity=expected_owner;out.level_identity=expected_level;
 out.level_id=f.level_id;out.loaded_match_count=f.count;error.clear();return true;
}
bool start_script(void* raw,std::int32_t id,std::string& error){
 static_cast<Fixture*>(raw)->scripts.push_back(id);error.clear();return true;
}
int main(int argc,char** argv){
 try{
  if(argc!=2)throw std::runtime_error("expected original data/ directory");
  const std::string root=argv[1];
  auto packed=read(root+"/v2quests_pyarray.bin");
  auto names=read(root+"/v2quests_pyarraynames.bin");
  auto pycst=read(root+"/v2quests_pycst.bin");
  dh2_quest_table table_data{};
  if(dh2_quests_open(&table_data,packed->data(),std::uint32_t(packed->size())))
   throw std::runtime_error("quest table parse");
  t::Input input;input.table=table_data;input.packed_owner=packed;
  input.names=names->data();input.names_size=names->size();input.names_owner=names;
  t::Owner table;std::string error;
  if(!table.load(input,error))throw std::runtime_error("quest table binding: "+error);
  const auto view=table.borrow();
  if(view.count()!=64)throw std::runtime_error("unexpected selected quest table size");
  n::Constants constants;
  if(dh2_pycst_open(&constants.view,pycst->data(),std::uint32_t(pycst->size())))
   throw std::runtime_error("constants parse");
  constants.owner=pycst;
  auto save=std::make_shared<d::PlayerSavegameV1>();
  save->set_character(UINT64_C(0x574553540023));
  n::Owner quests(save,view,constants);
  if(!quests.initialize(0,error))throw std::runtime_error("QEST init: "+error);

  // This is v2quests table row 23 (FireTemp_FindHotty). It is unrelated to
  // generated Crypt level-plan row 23; keep the two source indices distinct.
  constexpr std::uint32_t selected_quest_row=23;
  const auto* source=view.row(selected_quest_row);
  const auto* list=source?view.list(*source,1):nullptr;
  if(!list||!list->definition||list->definition->count!=1)
   throw std::runtime_error("selected v2quests row 23 ObjectiveList shape changed");
  dh2_quest_objective objective{};
  if(!view.objective(*list,0,&objective,error)||objective.common[0]!=0||
     objective.args[0]<0||objective.args[2]<=0)
   throw std::runtime_error("selected v2quests row 23 is not the pinned KillXEnemies source objective");
  auto* quest_ref=save->source_quest_log_b8().quests[0][selected_quest_row];
  auto* quest=quests.resolve(quest_ref);
  if(!quest_ref||!quest)throw std::runtime_error("v2quests row-23 QEST ref is not canonical");

  dh2_pycst_result event_type{};constexpr char group[]="v2QuestObjectiveType",key[]="KillXEnemies";
  if(dh2_pycst_get(&constants.view,group,sizeof(group)-1,key,sizeof(key)-1,&event_type)||!event_type.found)
   throw std::runtime_error("KillXEnemies event type missing");
  Fixture fixture;fixture.level_id=objective.args[1]<0?23:objective.args[1];
  fixture.selector=objective.args[0];fixture.count=objective.args[2]+1;
  n::KillEnemiesPopulationServicesV1 level{&fixture,population,UINT64_C(0x620023)};
  n::CombatObjectivePopulationServicesV1 objective_population{};
  objective_population.kill_enemies=level;
  quest->state_0=6;
  if(!quests.compile_combat_objective_list_v1(quest_ref->identity,objective_population,
       &fixture,start_script,error))throw std::runtime_error("KillX ObjectiveList Compile: "+error);
  if(fixture.selector!=objective.args[0])
   throw std::runtime_error("Compile queried a different source TestCharPropId");

  for(std::int32_t progress=1;progress<=objective.args[2];++progress){
   dh2::character_kill_quest_tail_v1::Event event{};
   event.kind=dh2::character_kill_quest_tail_v1::Kind::kill_enemies;
   event.objective_type=event_type.value;event.character_word_25=save->character();
   event.source_word_24=static_cast<std::int16_t>(objective.args[0]);event.source_subject=-1;
   if(!quests.raise_character_kill_event(event,error))
    throw std::runtime_error("selected quest kill event: "+error);
   if(event.source_subject!=progress||event.flag0!=1||event.flag1!=0)
    throw std::runtime_error("Crypt KillX progress did not follow source event order");
  }
  const std::vector<std::int32_t> expected=objective.common[2]>=0?
      std::vector<std::int32_t>{objective.common[2]}:std::vector<std::int32_t>{};
  if(fixture.scripts!=expected)throw std::runtime_error("Objective completion script differs from source data");
  if(!quests.flush_current_level_detaches(error))throw std::runtime_error("receiver detach: "+error);
  // Row 1 is an authored, unsupported objective type. Compile must stop at
  // that exact dispatch; a subsequent state-6 Register pass must see it as
  // uncompiled and leave it dormant instead of inventing a receiver.
  const auto* other_list=view.list(*view.row(1),1);
  dh2_quest_objective other_definition{};
  if(!other_list||!other_list->definition||other_list->definition->count!=1||
     !view.objective(*other_list,0,&other_definition,error)||other_definition.common[0]==0)
   throw std::runtime_error("pinned unsupported source objective row changed");
  auto* other_ref=save->source_quest_log_b8().quests[0][1];
  auto* other_quest=quests.resolve(other_ref);
  if(!other_ref||!other_quest)throw std::runtime_error("unsupported QEST row is not canonical");
  if(quests.compile_kill_x_enemies_objective_list_v1(other_ref->identity,level,
       &fixture,start_script,error)||error.find("no source provider") == std::string::npos)
   throw std::runtime_error("unsupported Objective dispatch was accepted by KillX adapter");
  const auto old_state=other_quest->state_0;other_quest->state_0=6;
  if(!quests.transition_combat_objective_registration(other_ref->identity,old_state,6,
       &fixture,start_script,error))
   throw std::runtime_error("unsupported Objective was falsely marked compiled: "+error);
  other_quest->state_0=old_state;
  std::cout<<"PASS quest_row=23 objective_type=0 required="<<objective.args[2]
           <<" event_selector="<<objective.args[0]<<" unsupported_row=1 fail_closed\n";
  return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<"\n";return 1;}
}
