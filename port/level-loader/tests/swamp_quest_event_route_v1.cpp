#include "../game_event_runtime_v75.hpp"
#include "../../game-data/quest_persistence_v51.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

using namespace dh2;
namespace {
unsigned checks;
void check(bool value,const std::string& why){++checks;if(!value)throw std::runtime_error(why);}
std::vector<std::uint8_t> read(const char* path){std::ifstream f(path,std::ios::binary);if(!f)throw std::runtime_error(std::string("cannot read ")+path);return {std::istreambuf_iterator<char>(f),{}};}
std::int32_t qindex(const data::QuestTablesPersistenceV51& q,const char* name){
 for(std::size_t i=0;i<q.rows().size();++i)if(q.rows()[i].name==name)return static_cast<std::int32_t>(i);
 throw std::runtime_error(std::string("missing authored quest ")+name);
}
void check_quest_data(const data::QuestTablesPersistenceV51& quests){
 const auto& escape=quests.rows().at(qindex(quests,"Swamp_Escape"));
 check(escape.objectives.size()==1&&escape.objectives[0].type==0&&escape.objectives[0].oid1==358&&escape.objectives[0].value==1,"Swamp Escape authored objective row");
 check(escape.rewards[0].size()==1&&escape.rewards[0][0].type==1&&escape.rewards[0][0].parameter1==100,"Swamp Escape normal XP reward row");
 check(escape.scripts[1]=="SwampScripts.Swamp_Intro"&&escape.scripts[6]=="SwampScripts.Flushit"&&escape.scripts[13]=="SwampScripts.Swamp_Serpent_Defeated","Swamp Escape authored script selectors");
 const auto& liz=quests.rows().at(qindex(quests,"Swamp_KillFiveLizman"));
 check(liz.accept.type==5&&liz.accept.oid1==366&&liz.accept.oid2==41,"Five Lizman authored accept TalkToNPC");
 check(liz.objectives.size()==1&&liz.objectives[0].type==10&&liz.objectives[0].oid1==93&&liz.objectives[0].value==5,"Five Lizman authored kill objective");
 check(liz.rewards[0].size()==2&&liz.rewards[0][0].type==1&&liz.rewards[0][0].parameter1==20&&liz.rewards[0][1].type==0&&liz.rewards[0][1].parameter1==200,"Five Lizman normal XP and gold reward rows");
 check(liz.scripts[6]=="SwampScripts.LizBlood_Activate","Five Lizman authored follower activation selector");
 const auto& witch=quests.rows().at(qindex(quests,"Swamp_KillWitch"));
 check(witch.accept.type==4&&witch.accept.str2=="_prim_WitchQuestStart","Witch authored start-zone objective");
 check(witch.objectives.size()==1&&witch.objectives[0].type==0&&witch.objectives[0].oid1==435&&witch.objectives[0].value==1,"Witch authored kill objective");
 check(witch.rewards[0].size()==2&&witch.rewards[0][0].type==1&&witch.rewards[0][0].parameter1==50&&witch.rewards[0][1].type==0&&witch.rewards[0][1].parameter1==50,"Witch normal XP and gold reward rows");
 check(witch.scripts[7]=="SwampCaveWitchScripts.Open_portal_witch"&&witch.scripts[10]=="SwampScripts.Swamp_Witch_Activate"&&witch.scripts[13]=="SwampScripts.Return_Cinematic","Witch portal, activation, and return cinematic selectors");
 const auto& moths=quests.rows().at(qindex(quests,"Swamp_Moths"));
 check(moths.prerequisites.size()==1&&moths.prerequisites[0].type==2&&moths.prerequisites[0].parameter1==qindex(quests,"Swamp_Escape")&&moths.prerequisites[0].parameter2==3,"Moths authored Escape prerequisite");
 check(moths.objectives.size()==1&&moths.objectives[0].type==10&&moths.objectives[0].oid1==94&&moths.objectives[0].value==8,"Moths authored kill objective");
}
struct Fixture {
 std::shared_ptr<loader::GameEventTablesV50> source=std::make_shared<loader::GameEventTablesV50>();
 std::shared_ptr<loader::GameEventManagerV50> manager=std::make_shared<loader::GameEventManagerV50>();
 std::shared_ptr<int> provider=std::make_shared<int>(0),level=std::make_shared<int>(0),payload=std::make_shared<int>(0);
 events::EventManagerOwnerV12 dispatcher{0x5357414d50};
 std::int32_t level_row{41};unsigned starts{};std::vector<std::string> scripts;
 std::shared_ptr<loader::GameEventRuntimeV75> runtime;
 void init(const std::vector<std::uint8_t>& records,const std::vector<std::uint8_t>& names){
  std::string e;check(source->initialize(records.data(),records.size(),names.data(),names.size(),e),e);
  auto all=source->borrow();check(*all.size==66,"Actual authored v2Events row count");
  // Keep only the two first authored Swamp post-objective routes while copying
  // their decoded source rows without rewriting a field.
  struct Selection {std::uint32_t size{2};std::vector<loader::GameEventRowV50> rows;std::vector<std::string> names;std::vector<const char*> name_ptrs;const loader::GameEventRowV50* members{};const char* const* name_array{};};
  auto selected=std::make_shared<Selection>();
  for(auto id:{62,63}){selected->rows.push_back((*all.members)[id]);selected->names.emplace_back((*all.names)[id]);}
  for(auto& n:selected->names)selected->name_ptrs.push_back(n.c_str());selected->members=selected->rows.data();selected->name_array=selected->name_ptrs.data();
  loader::GameEventTablesBorrowV50 borrow{selected,&selected->size,&selected->members,&selected->name_array,2,2};
  auto status=loader::GameEventLoadStatusV50::pending;unsigned steps{};
  while(status==loader::GameEventLoadStatusV50::pending&&steps++<100)status=manager->load_step(borrow);
  check(status==loader::GameEventLoadStatusV50::complete,"Selected actual event source rows construct through GameEventManager.Load");
  auto* liz=manager->by_id(0);auto* moth=manager->by_id(1);
  check(liz&&liz->fields().name8&&std::string(liz->fields().name8)=="Kill5Lizman_Post"&&liz->fields().data1c->script_c=="SwampScripts.Swamp_Camp_PrisonerB_b","Authored five-Lizman event identity and completion script");
  check(moth&&moth->fields().name8&&std::string(moth->fields().name8)=="KillMoths_Post"&&moth->fields().data1c->script_c=="SwampScripts.Swamp_Camp_PrisonerC_b","Authored moth event identity and completion script");
  check(liz->objectives().size()==1&&liz->objectives()[0]->fields().data_c->field20==444&&liz->objectives()[0]->fields().data_c->field24==41&&liz->objectives()[0]->fields().data_c->field28==1,"Actual Lizman event objective payload");
  check(moth->objectives().size()==1&&moth->objectives()[0]->fields().data_c->field20==445&&moth->objectives()[0]->fields().data_c->field24==41&&moth->objectives()[0]->fields().data_c->field28==1,"Actual moth event objective payload");
  loader::GameEventRuntimeServicesV75 s;s.provider=provider;
  s.current_level=[this](auto& pin,auto*& ev,auto*& row,std::string& error){pin=level;ev=&dispatcher;row=&level_row;error.clear();return true;};
  s.constant=[](const char* group,const char* name,std::int32_t& value,std::string& error){if(std::string(group)!="v2EventState"){error="wrong event state constants";return false;}value=std::string(name)=="Inactive"?0:std::string(name)=="Active"?1:2;error.clear();return true;};
  s.common_script_count=[](auto& n,std::string& error){n=0;error.clear();return true;};
  s.script_id=[](const char* name,std::int32_t& id,std::string& error){id=std::string(name)=="SwampScripts.Swamp_Camp_PrisonerB_b"?62:std::string(name)=="SwampScripts.Swamp_Camp_PrisonerC_b"?63:-1;error.clear();return true;};
  s.start_script=[this](std::int32_t id,std::int32_t module,bool received,std::string& error){check((id==62||id==63)&&module==-1&&!received,"Actual event completion script arguments");scripts.push_back(id==62?"SwampScripts.Swamp_Camp_PrisonerB_b":"SwampScripts.Swamp_Camp_PrisonerC_b");error.clear();return true;};
  s.enemies_loaded=[](bool,std::int32_t,std::int32_t& n,std::string& error){n=1;error.clear();return true;};
  s.talk_flag=[](std::int32_t,std::uint8_t,std::string& error){error.clear();return true;};
  s.project_event=[](const events::EventBorrowV12& event,loader::GameEventQuestBorrowV75& out,std::string& error){return loader::project_scoped_game_quest_event_v75(event,out,error);};
  s.send_network=[](const auto&,std::string& error){error.clear();return true;};
  runtime=std::make_shared<loader::GameEventRuntimeV75>(manager,std::move(s));
  check(runtime->reinit(e),e.empty()?"Actual Swamp event Compile/Register succeeds":e);
  check(runtime->update(e),e.empty()?"Actual Swamp event transition to Active succeeds":e);
 }
 void raise(std::int32_t id){
  std::int32_t type=5;std::uint8_t pending{};const std::uint8_t network{};std::int32_t quantity{};const std::uintptr_t character{};const std::int32_t event_id=id;
  loader::GameEventQuestBorrowV75 fields;fields.receiver=payload;fields.pending_network10=&pending;fields.from_network11=&network;fields.quantity14=&quantity;fields.character8_cell=&character;fields.id18_cell=&event_id;
  loader::ScopedGameQuestEventV75 event{reinterpret_cast<std::uintptr_t>(&event_id),&type,std::move(fields)};std::string e;
  check(dispatcher.raise(event.borrow(),e),e.empty()?"Typed actual QuestEvent delivery succeeds":e);
  check(pending==1&&quantity==1,"Matching authored post-event objective reaches one-of-one");
  check(dispatcher.update(0.0,e),e.empty()?"Source dispatcher applies delayed observer detach":e);
  check(runtime->update(e),e.empty()?"Completed Swamp objective transitions event to Completed":e);
 }
};
}
int main(int argc,char** argv){try{
 if(argc!=5)throw std::runtime_error("usage: test event_records event_names quest_records quest_names");
 const auto event_records=read(argv[1]),event_names=read(argv[2]),quest_records=read(argv[3]),quest_names=read(argv[4]);
 data::QuestTablesPersistenceV51 quest_table;std::string e;
 check(quest_table.decode({quest_records.data(),quest_records.size()},{quest_names.data(),quest_names.size()},e),e);
 check_quest_data(quest_table);
 Fixture f;f.init(event_records,event_names);
 f.raise(444);check(f.scripts.size()==1&&f.scripts[0]=="SwampScripts.Swamp_Camp_PrisonerB_b","Five-Lizman authored follower script fires only after matching event completion");
 check(f.manager->by_id(1)->fields().state0==1,"Moth post-event remains active after Lizman post-event");
 f.raise(445);check(f.scripts.size()==2&&f.scripts[1]=="SwampScripts.Swamp_Camp_PrisonerC_b","Moth authored follower script fires only after matching event completion");
 check(f.manager->by_id(0)->fields().state0==2&&f.manager->by_id(1)->fields().state0==2,"Both authored post-events reach Completed");
 std::cout<<"PASS authored Swamp quest/event route: table decode, objective delivery, rewards/selectors, event completion and follower scripts checks="<<checks<<" (host source proof only)\n";return 0;
}catch(const std::exception& ex){std::cerr<<ex.what()<<'\n';return 1;}}
