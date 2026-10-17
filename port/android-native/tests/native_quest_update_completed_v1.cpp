#include "../app/src/main/cpp/native_quest_owner.hpp"

#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

namespace d=dh2::data;
namespace n=dh2::native::quests;

namespace {
void check(bool value,const char* message){if(!value)throw std::runtime_error(message);}
std::shared_ptr<const std::vector<std::uint8_t>> read(const std::string& path){
 std::ifstream input(path,std::ios::binary|std::ios::ate);
 if(!input)throw std::runtime_error("missing test input: "+path);
 auto bytes=std::make_shared<std::vector<std::uint8_t>>(
     static_cast<std::size_t>(input.tellg()));
 input.seekg(0);input.read(reinterpret_cast<char*>(bytes->data()),
                          static_cast<std::streamsize>(bytes->size()));
 if(!input)throw std::runtime_error("short test input: "+path);
 return bytes;
}
struct Fixture {
 d::quest_reward_execution_v1::CharacterRef character{};
 std::int32_t gold_receipt=0,xp_receipt=0,time=0x34567812;
 std::uint32_t character_queries=0,gold_calls=0,xp_calls=0,time_queries=0,script_calls=0,save_calls=0;
 std::string script_name;std::int32_t script_argument=0;bool script_flag=false;
 std::uintptr_t expected_character=0;std::int32_t* quest_state=nullptr;std::int32_t* quest_time=nullptr;
 bool save_after_transition=false,script_after_transition=false;
 std::vector<std::int32_t> post_transition_order;
};
d::quest_reward_execution_v1::CharacterRef* resolve_character(void* raw,
 std::uintptr_t identity){
 auto& fixture=*static_cast<Fixture*>(raw);++fixture.character_queries;
 return fixture.character.identity==identity?&fixture.character:nullptr;
}
std::int32_t add_gold(void* raw,d::quest_reward_execution_v1::Record&,
 d::quest_reward_execution_v1::CharacterRef&,std::int32_t){
 ++static_cast<Fixture*>(raw)->gold_calls;return 0;
}
std::int32_t give_xp(void* raw,d::quest_reward_execution_v1::Record&,
 d::quest_reward_execution_v1::CharacterRef& character,std::int32_t amount,
 std::uint8_t source_flag,bool* granted){
 auto& fixture=*static_cast<Fixture*>(raw);++fixture.xp_calls;
 if(character.identity!=fixture.character.identity||source_flag!=1||!granted)return 1;
 fixture.xp_receipt=amount;*granted=true;return 0;
}
bool application_time(void* raw,std::int32_t& value,std::string& error){
 auto& fixture=*static_cast<Fixture*>(raw);++fixture.time_queries;
 value=fixture.time;error.clear();return true;
}
bool start_script(void* raw,const std::string& name,std::int32_t argument,
 bool flag,std::string& error){
 auto& fixture=*static_cast<Fixture*>(raw);++fixture.script_calls;
 fixture.post_transition_order.push_back(7);
 fixture.script_name=name;fixture.script_argument=argument;fixture.script_flag=flag;
 fixture.script_after_transition=fixture.quest_state&&fixture.quest_time&&
     *fixture.quest_state==13&&*fixture.quest_time==fixture.time;
 error.clear();return true;
}
bool save_character(void* raw,std::uintptr_t identity,std::string& error){
 auto& fixture=*static_cast<Fixture*>(raw);++fixture.save_calls;
 fixture.post_transition_order.push_back(7);
 fixture.save_after_transition=identity==fixture.expected_character&&fixture.quest_state&&
     fixture.quest_time&&*fixture.quest_state==13&&*fixture.quest_time==fixture.time;
 error.clear();return true;
}
}

int main(int argc,char** argv){
 try{
  check(argc==2,"expected the extracted game data directory");
  const std::string cache=argv[1];
  auto quests=read(cache+"/v2quests_pyarray.bin");
  auto names=read(cache+"/v2quests_pyarraynames.bin");
  auto constants=read(cache+"/v2quests_pycst.bin");
  d::quest_table_bindings_v1::Input input{};
  check(dh2_quests_open(&input.table,quests->data(),
        static_cast<std::uint32_t>(quests->size()))==0,"open Quest table");
  input.packed_owner=quests;input.names=names->data();input.names_size=names->size();
  input.names_owner=names;
  d::quest_table_bindings_v1::Owner table;std::string error;
  check(table.load(input,error),"load Quest table");
  d::quest_table_bindings_v1::View view=table.borrow();
  n::Constants constant_view{};
  check(dh2_pycst_open(&constant_view.view,constants->data(),
        static_cast<std::uint32_t>(constants->size()))==0,"open constants");
  constant_view.owner=constants;

  auto save=std::make_shared<d::PlayerSavegameV1>();
  constexpr std::uintptr_t character=0x12345678000000a1ull;
  save->set_character(character);
  n::Owner owner(save,view,constant_view);
  check(owner.initialize(0,error),"initialize canonical offline QEST");

  std::uint32_t row=UINT32_MAX,difficulty=UINT32_MAX,reward_type=UINT32_MAX;
  int best_script_coverage=-1;
  for(std::uint32_t dindex=0;dindex<3;++dindex){
   for(std::uint32_t index=0;index<view.count();++index){
    const auto* pydata=view.row(index);const auto* rewards=view.list(*pydata,2+dindex);
    if(!rewards||!rewards->definition||rewards->definition->count!=1)continue;
    dh2_quest_span span{};d::quest_table_bindings_v1::Span bytes{};
    if(!view.list_record(*rewards,0,&span,error)||!view.bytes(span,&bytes,error)||bytes.size!=12)continue;
    const auto* p=bytes.data;
    const auto type=std::uint32_t(p[0])|(std::uint32_t(p[1])<<8)|
                    (std::uint32_t(p[2])<<16)|(std::uint32_t(p[3])<<24);
    if(type<=1){
     const auto* source=view.record(*pydata);
    const int coverage=source->scripts[7].size?1:0;
     if(coverage>best_script_coverage){
      row=index;difficulty=dindex;reward_type=type;best_script_coverage=coverage;
     }
     if(coverage==3)break;
    }
   }
   if(best_script_coverage==3)break;
  }
  check(row!=UINT32_MAX,"cache has no one-entry Gold/XP RewardList");
  auto& log=save->source_quest_log_b8();auto* ref=log.quests[difficulty][row];
  auto* quest=owner.resolve(ref);check(quest,"selected Quest resolves in canonical owner");
  check(quest->fields.character_60==character&&log.character_5c==character,
        "Quest and log share the retained Save Character");
  bool has_state=false;
  check(owner.has_state_in_log(0,difficulty,12,has_state,error)&&
        has_state==(quest->state_0==12),
        "selected log state query did not read the canonical Quest record");
  const auto state_before_query=quest->state_0;quest->state_0=12;
  check(owner.has_state_in_log(0,difficulty,12,has_state,error)&&has_state,
        "selected log state query missed the canonical state-12 Quest");
  quest->state_0=state_before_query;
  const auto prior_state=quest->state_0,prior_time=quest->word_4;
  const auto prior_current=log.word_2c[difficulty];
  Fixture fixture;fixture.character={character,nullptr,&fixture.gold_receipt,
                                     &fixture.xp_receipt};
  fixture.expected_character=character;fixture.quest_state=&quest->state_0;
  fixture.quest_time=&quest->word_4;
  const auto* source_definition=view.record(*quest->py_data_68);
  check(source_definition,"selected Quest has source definition row");
  const bool slot7_present=source_definition->scripts[7].size!=0;
  const bool slot7_script_present=source_definition->scripts[7].size!=0;
  const bool save_expected=slot7_present&&!quest->byte_64;
  const n::QuestUpdateCompletedServicesV1 services{
      &fixture,resolve_character,add_gold,give_xp,application_time,save_character,start_script};
  n::QuestUpdateCompletedResultV1 result{};

  quest->state_0=6;
  check(owner.update_completed(ref->identity,0,difficulty,services,result,error),
        "state guard call");
  check(result.outcome==d::quest_update_completed_v1::Outcome::skipped_not_completed&&
        quest->state_0==6&&fixture.character_queries==0&&fixture.time_queries==0,
        "non-state-12 Quest reached providers or mutated");

  quest->state_0=12;log.word_2c[difficulty]=static_cast<std::int32_t>(row);
  const bool completed=owner.update_completed(ref->identity,0,difficulty,
                                               services,result,error);
  if(completed){
   check(result.outcome==d::quest_update_completed_v1::Outcome::transitioned&&
         result.rewards_attempted==1&&quest->state_0==13&&
         quest->word_4==fixture.time&&fixture.time_queries==1&&
         fixture.save_calls==std::uint32_t(save_expected)&&
         fixture.script_calls==std::uint32_t(slot7_script_present)&&
         fixture.post_transition_order.size()==std::size_t(save_expected)+std::size_t(slot7_script_present)&&
         fixture.post_transition_order==([&]{
           std::vector<std::int32_t> expected;
           if(save_expected)expected.push_back(7);
           if(slot7_script_present)expected.push_back(7);
           return expected;
         })()&&
         (!fixture.save_calls||fixture.save_after_transition)&&
         (!fixture.script_calls||(fixture.script_after_transition&&
          fixture.script_argument==-1&&fixture.script_flag&&!fixture.script_name.empty())),
         "canonical Reward::Give success did not transition through Owner");
  }else{
   check(result.status==d::quest_update_completed_v1::Status::reward_rejected&&
         result.rewards_attempted==1&&quest->state_0==12&&
         quest->word_4==prior_time&&fixture.time_queries==0,
         "canonical Reward::Give failure did not preserve state/timestamp");
  }
  check((reward_type==0?fixture.xp_calls==0:fixture.gold_calls==0)&&
        fixture.character_queries<=1,
        "Reward owner dispatched the wrong effect/provider");

  const auto saved_character=log.character_5c;quest->state_0=12;
  log.character_5c=saved_character+1;
  const auto prior_time_queries=fixture.time_queries;
  check(!owner.update_completed(ref->identity,0,difficulty,services,result,error)&&
        quest->state_0==12&&fixture.time_queries==prior_time_queries,
        "Owner accepted a selected QEST log bound to another Character");
  log.character_5c=saved_character;quest->state_0=prior_state;
  quest->word_4=prior_time;log.word_2c[difficulty]=prior_current;
  check(owner.close(error),"close canonical QEST owner");
  std::cout<<"PASS native_quest_update_completed_v1 7 checks\n";
  return 0;
 }catch(const std::exception& error){
  std::cerr<<"FAIL native_quest_update_completed_v1: "<<error.what()<<'\n';
  return 1;
 }
}
