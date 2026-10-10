#include "native_quest_runtime_v76.hpp"
#include <exception>
#include <limits>
namespace dh2::world {
namespace {
loader::GameEventObjectiveRowV50 row(const data::QuestObjectiveDefinitionV51& q){
 return {q.type,q.description,q.on_complete,q.oid1,q.oid2,q.value,q.str1,q.str2};
}
void produce_missing_ctor(data::QuestObjectivePersistenceV51& q){
 if(q.runtime_ctor_produced_v76)return;
 q.source_type4=q.definition->type;
 //These are missing derived C1 fields, not the already restored quantity20.
 if(q.source_type4==0||q.source_type4==1||q.source_type4==10||q.source_type4==11)q.words24_2c={0u,0u,0u};
 else if(q.source_type4!=4&&q.source_type4!=6&&q.source_type4!=12)q.words24_2c[0]=UINT32_MAX;
 if(q.source_type4==5)q.marker28=0;
 q.runtime_ctor_produced_v76=true;
}
}
struct NativeQuestRuntimeV76::Record {
 struct Reward {
  const data::QuestRewardDefinitionV51* data_c{};
  const data::QuestRewardDefinitionV51* compiled14{};
  std::int32_t type4{};std::uint8_t enabled8{};
  std::uintptr_t owner10{};
 };
 data::QuestPersistenceStateV51* quest{};
 loader::GameEventObjectiveRowV50 accept_row,end_row;
 std::vector<loader::GameEventObjectiveRowV50> objective_rows;
 std::vector<NativeConditionStubV69> prerequisite_rows;
 std::vector<Reward> rewards;
 std::string reward_description8;
 bool attempted{},ready{};
 loader::ObjectiveBorrowV75 borrow(const std::shared_ptr<character::CharacterMenuQuestsV51>& owner,
  data::QuestObjectivePersistenceV51& q){
  const loader::GameEventObjectiveRowV50* data{};
  if(&q==&quest->accept)data=&accept_row;
  else if(&q==&quest->end)data=&end_row;
  else for(std::size_t i=0;i<quest->objectives.size();++i)if(&q==&quest->objectives[i]){data=&objective_rows[i];break;}
  return {std::shared_ptr<void>(owner,&q),reinterpret_cast<std::uintptr_t>(&q),q.source_type4,q.compiled8,data,
   q.character_owner,q.completed14,q.receiver1c,{{loader::ObjectiveWordBorrowV75(&q.quantity20),
    loader::ObjectiveWordBorrowV75(&q.words24_2c[0]),loader::ObjectiveWordBorrowV75(&q.words24_2c[1]),
    loader::ObjectiveWordBorrowV75(&q.words24_2c[2])}}};
 }
};
NativeQuestRuntimeV76::NativeQuestRuntimeV76(std::weak_ptr<character::CharacterMenuQuestsV51> q,
 NativeQuestRuntimeServicesV76 s):quests_(std::move(q)),services_(std::move(s)){}
bool quest_reward_sequence_v108(std::uintptr_t owner,
 const std::vector<data::QuestRewardDefinitionV51>& rewards,
 const std::function<bool(std::uintptr_t,const data::QuestRewardDefinitionV51&,bool&,std::string&)>& give,
 std::string& e){
 if(!give){e="Required original RewardList::Give callback";return false;}
 for(const auto& reward:rewards){
  bool source_result{};
  if(!give(owner,reward,source_result,e))return false;
  if(!source_result)break; //RewardList::Give returns at the first virtual false.
 }
 e.clear();return true;
}
bool NativeQuestRuntimeV76::fail(std::string& e,const char* fallback){
 if(!failed_){failed_=true;failure_=e.empty()?fallback:e;}e=failure_;return false;
}
bool NativeQuestRuntimeV76::prepare(std::uintptr_t identity,std::shared_ptr<Record>& out,std::string& e){
 auto owner=quests_.lock();auto* quest=owner?owner->resolve_v70(identity):nullptr;
 if(!owner||!quest||!quest->definition||quest->difficulty<0||quest->difficulty>=3||
    !owner->save()||owner->save()->character()!=quest->character_owner){
  e="Required SAME actual initialized Quest/Save/immutable row";return fail(e,"Quest owner unavailable");
 }
 auto found=records_.find(identity);
 if(found==records_.end())found=records_.emplace(identity,std::make_shared<Record>()).first;
 out=found->second;auto& r=*out;
 if(r.ready){if(r.quest!=quest){e="Quest identity changed under its retained native runtime";return fail(e,"Quest lifetime mismatch");}return true;}
 if(r.attempted){e="Quest runtime retains failed native child construction prefix";return fail(e,"Quest preparation failed");}
 r.attempted=true;r.quest=quest;
 if(!services_.provider||!services_.objectives||!services_.conditions){e="Required SAME shared Objective and Condition runtime owners";return fail(e,"Quest services unavailable");}
 const auto& definition=*quest->definition;
 if(quest->objectives.size()!=definition.objectives.size()){e="Actual Quest objective receiver/row count differs";return fail(e,"Quest rows unavailable");}
 r.accept_row=row(definition.accept);r.end_row=row(definition.end);
 r.objective_rows.reserve(definition.objectives.size());
 for(const auto& o:definition.objectives)r.objective_rows.push_back(row(o));
 auto ctor=[&](data::QuestObjectivePersistenceV51& o){
  if(!o.definition||o.definition->type<0||o.definition->type>=13){e="Quest Objective factory outside actual captured thirteen entries";return false;}
  produce_missing_ctor(o);return true;
 };
 if(!ctor(quest->accept)||!ctor(quest->end))return fail(e,"Quest Objective C1 failed");
 for(auto& objective:quest->objectives)if(!ctor(objective))return fail(e,"Quest Objective C1 failed");
 for(const auto& reward:definition.rewards[std::size_t(quest->difficulty)]){
  if(reward.type<0||reward.type>=5){e="Reward factory outside captured five entries";return fail(e,"Reward factory failed");}
  r.rewards.push_back({&reward,nullptr,reward.type,0,quest->character_owner});
 }
 if(definition.prerequisites.size()>std::size_t(std::numeric_limits<std::int32_t>::max())){
  e="Quest inline conditions exceed original signed count";return fail(e,"Quest condition count failed");
 }
 for(const auto& condition:definition.prerequisites)r.prerequisite_rows.push_back({condition.type,condition.parameter1,condition.parameter2});
 if(quest->prerequisites20){e="Quest ConditionList already published by another native child owner";return fail(e,"Quest child identity mismatch");}
 if(!services_.conditions->construct(quest->prerequisites20,e))return fail(e,"Quest ConditionList C1 failed");
 if(!services_.conditions->assign_authored_py_data_v76(quest->prerequisites20,out,r.prerequisite_rows.data(),
       static_cast<std::int32_t>(r.prerequisite_rows.size()),e))return fail(e,"Quest inline AssignPyData failed");
 r.ready=true;return true;
}
bool NativeQuestRuntimeV76::marker(Record& r,data::QuestObjectivePersistenceV51& objective,
 std::int32_t state,std::string& e){
 if(objective.source_type4!=5)return true; //actual selected Objective base479f80 BX LR
 if(!objective.compiled8||!objective.definition||objective.definition->type!=5)return true;
 if(!services_.install_talk_marker){e="Required actual Objective_TalkToNPC.InstallObjectiveMarker47d8e8";return false;}
 return services_.install_talk_marker(objective,r.quest->definition->priority,state,e);
}
bool NativeQuestRuntimeV76::compile(std::uintptr_t identity,std::string& e){
 if(failed_){e=failure_;return false;}if(busy_){e="Quest.Compile reentered its actual native owner";return fail(e,"Quest reentrancy");}
 struct Scope{bool& active;~Scope(){active=false;}} scope{busy_};busy_=true;
 try{
  std::shared_ptr<Record> record;if(!prepare(identity,record,e))return false;
  auto owner=quests_.lock();if(!owner){e="Actual Quest owner expired";return fail(e,"Quest lifetime");}
  auto& r=*record;auto& q=*r.quest;auto& runtime=*services_.objectives;
  auto invalidate=[&](auto& objective){return runtime.invalidate_objective(r.borrow(owner,objective),e);};
  auto compile=[&](auto& objective){return runtime.compile_objective(r.borrow(owner,objective),e);};
  auto attach=[&](auto& objective){return runtime.register_objective(r.borrow(owner,objective),true,e);};
  //48018c..1b0: complete the WHOLE invalidation prefix before any Compile.
  if(!invalidate(q.accept))return fail(e,"Quest accept invalidation");
  for(auto& o:q.objectives)if(!invalidate(o))return fail(e,"Quest list invalidation");
  if(!invalidate(q.end))return fail(e,"Quest end invalidation");
  r.reward_description8.clear();for(auto& reward:r.rewards)reward.enabled8=0;
  if(!compile(q.accept))return fail(e,"Quest accept Compile");
  for(auto& o:q.objectives)if(!compile(o))return fail(e,"Quest ObjectiveList Compile");
  if(!compile(q.end))return fail(e,"Quest end Compile");
  if(q.rewards_enabled5c)for(auto& reward:r.rewards){
   if(reward.type4<=3){reward.enabled8=1;reward.compiled14=reward.data_c;}
   else if(reward.data_c->parameter2>=0)reward.enabled8=1; //4828d0 ConsumeLoot
  }
  const auto state=q.state;
  if(state==6){
   for(auto& o:q.objectives)if(!attach(o))return fail(e,"Quest ObjectiveList Register");
   for(auto& o:q.objectives)if(!marker(r,o,q.state,e))return fail(e,"Quest list markers");
  }else if(state==9){if(!attach(q.end)||!marker(r,q.end,q.state,e))return fail(e,"Quest end Register/marker");}
  else if(state==3){if(!attach(q.accept)||!marker(r,q.accept,q.state,e))return fail(e,"Quest accept Register/marker");}
  e.clear();return true;
 }catch(const std::exception& ex){e=ex.what();return fail(e,"Quest runtime provider exception");}
}
bool NativeQuestRuntimeV76::prerequisites(std::uintptr_t identity,bool& truth,std::string& e){
 if(failed_){e=failure_;return false;}
 std::shared_ptr<Record> record;if(!prepare(identity,record,e))return false;
 return services_.conditions->evaluate(record->quest->prerequisites20,truth,e);
}
bool NativeQuestRuntimeV76::destroy_quest_v108(std::uintptr_t identity,std::string& e){
 if(busy_){e="Actual Quest D1 during source gameplay callback";return false;}
 auto owner=quests_.lock();auto* quest=owner?owner->resolve_v70(identity):nullptr;
 if(!quest){e="Required SAME positive Quest allocation at D1";return false;}
 auto found=records_.find(identity);
 //No runtime child was enrolled before Compile: original selected Objective
 //D1 is BXLR; destroying the persistence allocation later frees its storage.
 if(found==records_.end()){e.clear();return true;}
 auto record=found->second;if(record->quest!=quest||!services_.objectives){e="Required SAME Quest/Objective D1 native owner";return false;}
 auto destroy=[&](auto& q){return services_.objectives->destroy_objective_aliases_v108(record->borrow(owner,q),e);};
 //Actual4809a4: accept18 D0, end1c D0, rewardlist38 D1, objective2c D1,
 //then embedded ConditionList20 D1. No Unregister/marker side effects.
 if(!destroy(quest->accept)||!destroy(quest->end))return false;
 record->rewards.clear();for(auto& q:quest->objectives)if(!destroy(q))return false;
 quest->objectives.clear();record->objective_rows.clear();
 if(quest->prerequisites20&&!services_.conditions->destroy(quest->prerequisites20,e))return false;
 quest->prerequisites20=0;records_.erase(found);e.clear();return true;
}

bool NativeQuestRuntimeV76::bind_frame_v108(NativeQuestFrameServicesV108 s,std::string& e){
 if(busy_||frame_v108_.provider||!s.provider||!s.frame_gate||!s.constant||!s.application_time70||!s.script_id||!s.script_running||!s.start_script){e="Required once-bound actual Quest frame services";return false;}frame_v108_=std::move(s);e.clear();return true;
}
bool NativeQuestRuntimeV76::script_v108(Record& r,std::int32_t selector,bool execute,bool& running,std::string& e){
 running=false;if(selector<0||selector>=14){e="Original Quest script selector assertion outside0..13";return false;}
 std::int32_t id;if(!frame_v108_.script_id||!frame_v108_.script_id(r.quest->definition->scripts[std::size_t(selector)].c_str(),id,e))return false;
 if(id<0){e.clear();return true;}
 if(execute)return frame_v108_.start_script&&frame_v108_.start_script(id,-1,true,e);
 return frame_v108_.script_running&&frame_v108_.script_running(id,running,e);
}
bool NativeQuestRuntimeV76::set_state_v108(Record& r,const std::shared_ptr<character::CharacterMenuQuestsV51>& owner,std::int32_t next,std::string& e){
 auto& q=*r.quest;if(std::uint32_t(next)>13){e.clear();return true;} //original unsigned earlyreturn
 const auto previous=q.state;q.state=next;
 if(previous==next&&!q.volatile64){e.clear();return true;}
 if(!frame_v108_.application_time70(q.state_date4,e))return false;
 auto& engine=*services_.objectives;auto registration=[&](auto& o,bool adding){return engine.register_objective(r.borrow(owner,o),adding,e);};
 auto list=[&](bool adding){for(auto& o:q.objectives)if(!registration(o,adding))return false;return true;};
 if(previous==6){if(!list(false))return false;}else if(previous==9){if(!registration(q.end,false))return false;}else if(previous==3){if(!registration(q.accept,false))return false;}
 //Captured IsVolatileState47f70c. Saved64 bypasses save-on-transition until
 //the authentic first Update restores state-specific effects then clears64.
 constexpr std::uint8_t vol[]{1,0,1,1,0,1,1,0,1,1};
 if(next>=2&&next<=11&&vol[next-2]&&!q.volatile64){
  int first=-1,second=-1;
  switch(next){case 2:first=11;second=1;break;case 4:first=6;break;case 5:first=10;second=0;break;case 7:first=5;break;case 8:first=13;second=3;break;case 10:first=8;break;case 11:first=12;second=2;break;default:break;}
  bool needs=false;for(int selector:{first,second})if(selector>=0){std::int32_t id;if(!frame_v108_.script_id(q.definition->scripts[std::size_t(selector)].c_str(),id,e))return false;if(id>=0)needs=true;}
  if(needs&&(!frame_v108_.transition_save||!frame_v108_.transition_save(e)))return false;
 }
 auto remove=[&](auto& o){if(o.source_type4!=5)return true;if(!services_.remove_talk_marker){e="Required selected Objective.RemoveObjectiveMarker";return false;}return services_.remove_talk_marker(o,e);};
 bool ignored{};auto exec=[&](int selector){return script_v108(r,selector,true,ignored,e);};
 switch(next){
 case 1:return exec(9);
 case 2:return marker(r,q.accept,2,e)&&exec(11);
 case 3:return registration(q.accept,true)&&exec(previous==6?4:1);
 case 4:return remove(q.accept)&&exec(6);
 case 5:for(auto& o:q.objectives)if(!marker(r,o,5,e))return false;return exec(10);
 case 6:{
  if(!exec(0)||!list(true)||!frame_v108_.new_dialog||!frame_v108_.new_dialog(q,e)||!frame_v108_.current_quest||!frame_v108_.current_quest(q.index,-1,e))return false;
  std::int32_t primary;if(!frame_v108_.constant("v2QuestPriority","Primary",primary,e))return false;
  return q.definition->priority!=primary||(frame_v108_.current_primary&&frame_v108_.current_primary(q.index,-1,e));
 }
 case 7:return frame_v108_.current_quest&&frame_v108_.current_quest(-1,-1,e)&&exec(5);
 case 8:return marker(r,q.end,8,e)&&exec(13);
 case 9:for(auto& o:q.objectives)if(!remove(o))return false;return exec(3)&&frame_v108_.current_act&&frame_v108_.current_act(q.definition->act,-1,e);
 case 10:return registration(q.end,true)&&exec(8);
 case 11:return exec(12);
 case 12:{if(!exec(2))return false;std::vector<data::QuestRewardDefinitionV51> rewards;for(const auto& reward:r.rewards)if(reward.enabled8)rewards.push_back(*reward.data_c);
  return frame_v108_.completed_dialog&&frame_v108_.completed_dialog(q,rewards,e);}
 case 13:return exec(7);
 default:e.clear();return true;
 }
}
bool NativeQuestRuntimeV76::update_quest_v108(std::uintptr_t identity,std::string& e){
 if(failed_){e=failure_;return false;}if(busy_||!frame_v108_.provider){e="Required actual nonreentrant Quest.Update";return fail(e,"Quest frame provider");}
 bool permitted;if(!frame_v108_.frame_gate(permitted,e))return fail(e,"Quest PM/Save/network gate");if(!permitted){e.clear();return true;}
 std::shared_ptr<Record> held;if(!prepare(identity,held,e))return false;auto owner=quests_.lock();if(!owner)return fail(e,"Quest owner retired");
 busy_=true;struct Guard{bool& value;~Guard(){value=false;}} guard{busy_};auto& r=*held;auto& q=*r.quest;
 if(q.volatile64){if(!set_state_v108(r,owner,q.state,e))return fail(e,"Quest saved-state reinit");q.volatile64=0;}
 const char* keys[]={"Locked","PostLocked","PreAvailable","Available","PostAvailable","PreActive","Active","PostActive","PreCompleted","Completed","PostCompleted","PreClosed","Closed","Closed"};
 const int scripts[]={-1,9,11,-1,6,10,-1,8,13,-1,8,12,2,-1};
 const char* successors[]={"PostLocked","PreAvailable","Available","PostAvailable","PreActive","Active","PostActive","PreCompleted","Completed","PostCompleted","PreClosed","Closed","PostClosed",nullptr};
 for(unsigned branch=0;branch<14;++branch){std::int32_t state;if(!frame_v108_.constant("v2QuestState",keys[branch],state,e))return fail(e,"Quest live state constant");if(q.state!=state)continue;
  if(branch==13)continue; //UpdatePostClosed47f700 literal BXLR
  bool proceed=true,running{};
  if(scripts[branch]>=0){if(!script_v108(r,scripts[branch],false,running,e))return fail(e,"Quest script wait");proceed=!running;}
  if(branch==0||branch==3){bool valid;if(!services_.conditions->evaluate(q.prerequisites20,valid,e))return fail(e,"Quest prerequisite evaluation");
   if(branch==0)proceed=valid;else if(!valid){std::int32_t locked;if(!frame_v108_.constant("v2QuestState","Locked",locked,e)||!set_state_v108(r,owner,locked,e))return fail(e,"Quest return Locked");continue;}
   else {proceed=q.accept.completed14!=0;if(proceed){if(!script_v108(r,1,false,running,e))return fail(e,"Quest accept script wait");proceed=!running;}}
  }
  if(branch==6){for(const auto& o:q.objectives)if(!o.completed14){proceed=false;break;}if(proceed){if(!script_v108(r,0,false,running,e))return fail(e,"Quest active script wait");proceed=!running;}}
  if(branch==9){
   proceed=q.end.completed14!=0;
   if(proceed){
    if(!script_v108(r,3,false,running,e))return fail(e,"Quest completion script wait");
    proceed=!running;
    if(proceed){
      std::vector<data::QuestRewardDefinitionV51> rewards;
      for(const auto& reward:r.rewards)if(reward.enabled8)rewards.push_back(*reward.data_c);
      if(!quest_reward_sequence_v108(q.character_owner,rewards,frame_v108_.give_reward,e))return fail(e,"Quest Reward.Give");
     q.rewards_enabled5c=0;
     bool online;
     if(!frame_v108_.online||!frame_v108_.online(online,e))return fail(e,"Quest online gate");
     if(online&&(!frame_v108_.online_act||!frame_v108_.online_act(e)))return fail(e,"Quest online current act");
    }
   }
  }
  if(!proceed)continue;
  if(branch==11)q.completed5d=1; //source store before SetState in PreClosed
  std::int32_t next;if(!frame_v108_.constant("v2QuestState",successors[branch],next,e)||!set_state_v108(r,owner,next,e))return fail(e,"Quest state transition");
  if(branch==5){bool online;if(!frame_v108_.online||!frame_v108_.online(online,e))return fail(e,"Quest active online gate");if(online&&(!frame_v108_.online_activation||!frame_v108_.online_activation(e)))return fail(e,"Quest online current quest");}
  if(branch==12){q.completed5d=1;if(q.character_owner){bool local;if(!frame_v108_.is_local||!frame_v108_.is_local(q.character_owner,local,e))return fail(e,"Quest source local selector");if(local&&(!frame_v108_.all_quests_trophy||!frame_v108_.all_quests_trophy(q.character_owner,e)))return fail(e,"Quest all-quests trophy");}}
 }
 e.clear();return true;
}

}
