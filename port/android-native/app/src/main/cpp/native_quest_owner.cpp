#include "native_quest_owner.hpp"
#include "native_quest_cursor.hpp"
#include "quest_instance_v1.hpp"
#include "quest_condition_factory_v1.hpp"
#include "quest_condition_eval_v1.hpp"
#include "quest_objective_factory_v1.hpp"
#include "quest_reward_factory_v1.hpp"
#include "quest_reward_execution_v1.hpp"
#include "quest_compile_v1.hpp"
#include "quest_objective_payload_v1.hpp"
#include "quest_stream_read_v1.hpp"
#include "fresh_inventory_owned_v4.hpp"
#include "quest_gather_loot_receiver_v1.hpp"
#include "../../../../../quest-kill/quest.h"
#include "../../../../../level-world/open_game_object_objective_v1.hpp"
#include <algorithm>
#include <android/log.h>
#include <map>
#include <cstring>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

namespace dh2::native::quests {
namespace scalar=data::quest_runtime_fields_v1;
namespace logs=data::quest_savegame_v1;
namespace instance=data::quest_instance_v1;
namespace c=data::quest_condition_list_v1;
namespace cf=data::quest_condition_factory_v1;
namespace o=data::quest_objective_list_v1;
namespace of=data::quest_objective_factory_v1;
namespace r=data::quest_reward_list_v1;
namespace rf=data::quest_reward_factory_v1;
namespace re=data::quest_reward_execution_v1;
namespace ce=data::quest_condition_eval_v1;
namespace qc=data::quest_compile_v1;
namespace og=level_world::open_game_object_objective_v1;
namespace {
template<class T>std::int32_t retire(std::map<std::uintptr_t,std::unique_ptr<T>>& arena,T* record,std::uintptr_t identity){
 const auto found=arena.find(identity);
 if(found==arena.end()||found->second.get()!=record)return 1;
 arena.erase(found);return 0;
}
template<class Record>std::int32_t allocate(std::map<std::uintptr_t,std::unique_ptr<Record>>& arena,std::uint32_t bytes,std::uint32_t tag,Record** out){
 if(!out||!bytes||tag)return 1;
 auto record=std::make_unique<Record>(1);const auto identity=reinterpret_cast<std::uintptr_t>(record.get());
 if constexpr(std::is_same_v<Record,of::Record>)record->ref.action.identity=identity;
 else record->ref.identity=identity;
 *out=record.get();return arena.emplace(identity,std::move(record)).second?0:1;
}
template<class Array>std::int32_t allocate_array(std::map<std::uintptr_t,std::unique_ptr<Array>>& arena,std::uint32_t bytes,std::uint32_t tag,Array** out){
 if(!out||tag||bytes%4)return 1;
 auto array=std::make_unique<Array>();array->identity=reinterpret_cast<std::uintptr_t>(array.get());
 array->slots.resize(bytes/4,nullptr);*out=array.get();const auto identity=array->identity;
 return arena.emplace(identity,std::move(array)).second?0:1;
}
bool objective_definition(const of::Record& objective,dh2_quest_objective& value,std::string& error){
 const auto& definition=objective.fields.py_data_c;
 if(definition.event){
  if(definition.list||definition.stub||
     definition.index>=definition.event->triggers.size()){
   error="Invalid retained GameEvent Objective trigger reference";return false;
  }
  const auto& trigger=definition.event->triggers[definition.index];
  value.common[0]=trigger.type;value.common[1]=trigger.op1;value.common[2]=trigger.op2;
  value.args[0]=trigger.value1;value.args[1]=trigger.value2;value.args[2]=trigger.value3;
  value.strings[0]={};value.strings[1]={};error.clear();return true;
 }
 if(definition.stub){
  if(definition.list||!definition.view||!definition.stub->row||
     definition.view.resolve_stub(definition.stub->row->identity+definition.stub->offset)!=definition.stub||
     !definition.stub->definition){error="Invalid retained Objective pydata StubRef";return false;}
  value=*definition.stub->definition;error.clear();return true;
 }
 if(!definition.list||definition.list->kind!=1||
    !definition.view.objective(*definition.list,definition.index,&value,error)){
  if(error.empty())error="Objective has no valid retained quest-table definition";
  return false;
 }
 return true;
}
bool objective_string(const of::Record& objective,const dh2_quest_objective& definition,
                      std::size_t index,std::string& value,std::string& error){
 if(index>=2){error="Objective string index is outside the source stub";return false;}
 const auto& pydata=objective.fields.py_data_c;
 if(pydata.event){
  if(index>=2||pydata.index>=pydata.event->triggers.size()){
   error="Objective string lookup has no retained GameEvent trigger";return false;
  }
  value=index?pydata.event->triggers[pydata.index].text2:
             pydata.event->triggers[pydata.index].text1;
  error.clear();return true;
 }
 if(!pydata.view){error="Objective string requires its retained immutable pydata View";return false;}
 data::quest_table_bindings_v1::Span span{};
 if(!pydata.view.bytes(definition.strings[index],&span,error))return false;
 if(span.size&&!span.data){error="Objective string span has no retained bytes";return false;}
 if(span.size)value.assign(reinterpret_cast<const char*>(span.data),span.size);
 else value.clear();
 error.clear();return true;
}
}
struct Owner::Impl {
 struct Slot {std::unique_ptr<instance::Instance> value;bool constructed=false;};
 std::shared_ptr<data::PlayerSavegameV1> save;
 data::quest_table_bindings_v1::View definitions;
 Constants constants;
 Receipt result;
 std::map<std::uintptr_t,std::unique_ptr<Slot>> slots;
 std::map<std::uintptr_t,std::unique_ptr<cf::Record>> conditions;
 std::map<std::uintptr_t,std::unique_ptr<of::Record>> objectives;
 std::map<std::uintptr_t,std::unique_ptr<rf::Record>> rewards;
 std::map<std::uintptr_t,std::unique_ptr<qc::Runtime>> compile_runtimes;
 std::map<std::uintptr_t,std::unique_ptr<data::quest_gather_loot_receiver_v1::Binding>> gather_loot;
 struct KillXEnemiesBinding {
  of::Record* objective=nullptr;
  of::Dispatch expected_dispatch=of::Dispatch::kill_enemies;
  std::uintptr_t level_identity=0,game_event_manager_identity=0;
  std::int32_t event_type=0,match_id=-1,script_id=-1;
  void* script_context=nullptr;
  KillObjectiveStartScript start_script=nullptr;
  bool attached=false,detach_pending=false,target_marked=false,game_event_trigger=false;
  static std::int32_t receive(void*,Owner::LevelEventRuntime&,
                              Owner::LevelEvent&,std::string&);
 };
 struct TalkToNpcBinding {
  Owner* owner=nullptr;of::Record* objective=nullptr;
  std::uintptr_t quest_identity=0;
  std::uintptr_t level_identity=0,game_event_manager_identity=0;
  std::uint32_t ordinal=0;
  std::int32_t event_type=0,target_props_id=-1,target_count=0,script_id=-1;
  void* script_context=nullptr;KillObjectiveStartScript start_script=nullptr;
  void* marker_context=nullptr;TalkObjectiveMarkerV1 marker=nullptr;
  bool attached=false,detach_pending=false,target_marked=false,game_event_trigger=false;
  static std::int32_t receive(void*,Owner::LevelEventRuntime&,
                              Owner::LevelEvent&,std::string&);
 };
 struct MoveInZoneBinding {
  Owner* owner=nullptr;of::Record* objective=nullptr;
  std::uintptr_t quest_identity=0,target_identity=0;
  std::uintptr_t level_identity=0,game_event_manager_identity=0;
  std::uint32_t ordinal=0;
  std::int32_t event_type=0,target_props_id=-1,script_id=-1;
  void* script_context=nullptr;KillObjectiveStartScript start_script=nullptr;
  MoveInZoneCharacterServicesV1 character{};
  bool attached=false,detach_pending=false,game_event_trigger=false;
  static std::int32_t receive(void*,Owner::LevelEventRuntime&,
                              Owner::LevelEvent&,std::string&);
 };
 struct OpenGameObjectBinding {
  Owner* owner=nullptr;of::Record* objective=nullptr;
  std::uintptr_t quest_identity=0;
  std::uint32_t ordinal=0;
  void* script_context=nullptr;KillObjectiveStartScript start_script=nullptr;
  og::Binding objective_binding{};
  static bool complete(void*,og::Objective&,std::string&);
 };
 std::map<std::uintptr_t,std::unique_ptr<KillXEnemiesBinding>> kill_x_enemies;
 std::map<std::uintptr_t,std::unique_ptr<TalkToNpcBinding>> talk_to_npc;
 std::map<std::uintptr_t,std::unique_ptr<MoveInZoneBinding>> move_in_zone;
 std::map<std::uintptr_t,std::unique_ptr<OpenGameObjectBinding>> open_game_object;
 std::map<std::uintptr_t,std::unique_ptr<c::Array>> ca;
 std::map<std::uintptr_t,std::unique_ptr<o::Array>> oa;
 std::map<std::uintptr_t,std::unique_ptr<r::Array>> ra;
 cf::Runtime condition_factory;
 of::Runtime objective_factory;
 rf::Runtime reward_factory;
 std::unique_ptr<logs::Runtime> log_runtime[2];
 std::unique_ptr<qc::LogRuntime> compile_log_runtime[2];
 const QuestCompileServicesV1* active_compile_services=nullptr;
 level_world::current_level_quest_event_v1::Runtime current_level_events;
 Cursor* active_cursor=nullptr;
 bool busy=false,closed=false;
 Impl(std::shared_ptr<data::PlayerSavegameV1> saved,data::quest_table_bindings_v1::View tables,Constants input):
  save(std::move(saved)),definitions(std::move(tables)),constants(std::move(input)),
  condition_factory({this,
   [](void* raw,std::uint32_t bytes,std::uint32_t tag,cf::Record** out){return allocate(static_cast<Impl*>(raw)->conditions,bytes,tag,out);},
   [](void* raw,cf::Record* record){return retire(static_cast<Impl*>(raw)->conditions,record,record->ref.identity);}}),
  objective_factory({this,
   [](void* raw,std::uint32_t bytes,std::uint32_t tag,of::Record** out){return allocate(static_cast<Impl*>(raw)->objectives,bytes,tag,out);},
   [](void* raw,of::Record&,const char* group,const char* name,std::int32_t* out){return static_cast<Impl*>(raw)->constant(group,name,out);},
   [](void* raw,of::Record* record){return retire(static_cast<Impl*>(raw)->objectives,record,record->ref.action.identity);}}),
  reward_factory({this,
   [](void* raw,std::uint32_t bytes,std::uint32_t tag,rf::Record** out){return allocate(static_cast<Impl*>(raw)->rewards,bytes,tag,out);},
   [](void* raw,rf::Record&,const char* group,const char* name,std::int32_t* out){return static_cast<Impl*>(raw)->constant(group,name,out);},
   [](void* raw,rf::Record* record){return retire(static_cast<Impl*>(raw)->rewards,record,record->ref.identity);}}) {
  dh2_pycst_view checked{};
  if(!save||!definitions||!constants.owner||dh2_pycst_open(&checked,constants.view.bytes,constants.view.size))
   throw std::invalid_argument("Actual Save and retained Quest definition/constants owners required");
  for(unsigned log=0;log<2;++log){
   auto& store=log?save->source_quest_log_118():save->source_quest_log_b8();
   for(const auto& vector:store.quests)if(!vector.empty())throw std::invalid_argument("Quest Save logs already have a factory owner");
   log_runtime[log]=std::make_unique<logs::Runtime>(store,log_services());
   compile_log_runtime[log]=std::make_unique<qc::LogRuntime>(store,compile_log_services());
  }
 }
 std::int32_t constant(const char* group,const char* name,std::int32_t* out){
  if(!group||!name||!out)return 1;
  dh2_pycst_result value{};++result.constant_queries;
  if(dh2_pycst_get(&constants.view,group,std::uint32_t(std::strlen(group)),name,std::uint32_t(std::strlen(name)),&value))return 1;
  *out=value.found?value.value:0;return 0;
 }
 instance::Instance* resolve(const logs::QuestRef* ref){
  if(!ref)return nullptr;
  const auto found=slots.find(ref->identity);
  if(found==slots.end()||!found->second->value||&found->second->value->record().ref!=ref)return nullptr;
  return found->second->value.get();
 }
 instance::Instance* resolve_quest_identity(std::uintptr_t identity){
  const auto found=slots.find(identity);
  if(found==slots.end()||!found->second->value||found->second->value->record().ref.identity!=identity)return nullptr;
  return found->second->value.get();
 }
 of::Record* resolve_compile_objective(qc::ActionRef* ref){
  if(!ref||!ref->identity)return nullptr;
  const auto found=objectives.find(ref->identity);
  return found==objectives.end()||&found->second->ref.action!=ref?nullptr:found->second.get();
 }
 rf::Record* resolve_compile_reward(r::RewardRef* ref){
  if(!ref||!ref->identity)return nullptr;
  const auto found=rewards.find(ref->identity);
  return found==rewards.end()||&found->second->ref!=ref?nullptr:found->second.get();
 }
 qc::Runtime* resolve_compile_quest(logs::QuestRef* ref){
  auto* value=resolve(ref);if(!value)return nullptr;
  const auto identity=value->record().ref.identity;
  auto found=compile_runtimes.find(identity);
  if(found!=compile_runtimes.end()&&&found->second->instance()==value)return found->second.get();
  qc::Services services{this,
   [](void* raw,qc::ActionRef* action)->qc::Objective*{
    if(!action)return nullptr;
    return static_cast<Impl*>(raw)->resolve_compile_objective(action);
   },
   [](void* raw,r::RewardRef* reward)->qc::Reward*{
    return static_cast<Impl*>(raw)->resolve_compile_reward(reward);
   },
   [](void* raw,qc::Objective& objective,const qc::MemberCall& call)->std::int32_t{
    auto& owner=*static_cast<Impl*>(raw);
    const auto* provider=owner.active_compile_services;
    if(!provider||!provider->objective_member)return 1;
    try{return provider->objective_member(provider->context,objective,call);}catch(...){return 1;}
   }};
  auto runtime=std::make_unique<qc::Runtime>(*value,definitions,services);
  auto* result=runtime.get();
  if(found==compile_runtimes.end())compile_runtimes.emplace(identity,std::move(runtime));
  else found->second=std::move(runtime);
  return result;
 }
 qc::LogServices compile_log_services(){return {this,
  [](void* raw,std::uintptr_t character,std::int32_t* out)->std::int32_t{
   auto& owner=*static_cast<Impl*>(raw);const auto* provider=owner.active_compile_services;
   if(!provider||!provider->difficulty)return 1;
   try{return provider->difficulty(provider->context,character,out);}catch(...){return 1;}
  },
  [](void* raw,logs::QuestRef* ref)->qc::Runtime*{
   return static_cast<Impl*>(raw)->resolve_compile_quest(ref);
  }};}
 // A Quest's accept (+0x18) and end (+0x1c) Objectives are ActionRefs outside its
 // ObjectiveList. The bindings below address every Objective as (Quest, ordinal);
 // these two sentinels name the actions so the one Talk/MoveInZone/Automatic
 // implementation serves list children and quest actions alike.
 static constexpr std::uint32_t accept_action_ordinal=0xFFFFFFF0u,
                                end_action_ordinal=0xFFFFFFF1u;
 of::Record* objective_at(std::uintptr_t quest_identity,std::uint32_t ordinal){
  auto* quest=resolve_quest_identity(quest_identity);if(!quest)return nullptr;
  if(ordinal==accept_action_ordinal)return resolve_compile_objective(quest->record().action_18);
  if(ordinal==end_action_ordinal)return resolve_compile_objective(quest->record().action_1c);
  auto& list=quest->objectives();
  if(list.count_0<0||ordinal>=std::uint32_t(list.count_0)||!list.children_4||
     !list.children_4->identity||std::uint64_t(std::uint32_t(list.count_0))>list.children_4->slots.size())return nullptr;
  return resolve_objective(list.children_4->slots[ordinal]);
 }
 of::Record* resolve_objective(const o::ObjectiveRef* ref){
  if(!ref||!ref->action.identity)return nullptr;
  const auto found=objectives.find(ref->action.identity);
  if(found==objectives.end()||&found->second->ref!=ref)return nullptr;
  return found->second.get();
 }
 std::int32_t delete_objective(std::uintptr_t identity,const o::ObjectiveRef* expected=nullptr){
  const auto found=objectives.find(identity);
  if(found==objectives.end()||(expected&&&found->second->ref!=expected))return 1;
  of::Result r;return objective_factory.destroy(*found->second,true,&r)==of::Status::complete?0:1;
 }
 static std::int32_t read_stream(void* raw,data::player_saved_quests_v1::StreamRef& stream,
                                void* destination,std::uint64_t requested,std::uint64_t* returned){
  auto& owner=*static_cast<Impl*>(raw);
  return owner.active_cursor&&owner.active_cursor->read(stream,destination,requested,returned)?0:1;
 }
 std::int32_t load_objective(std::uintptr_t identity,data::player_saved_quests_v1::StreamRef* stream,
                            const o::ObjectiveRef* expected=nullptr){
  const auto found=objectives.find(identity);
  if(!active_cursor||!stream||stream!=&active_cursor->stream()||found==objectives.end()||
     (expected&&&found->second->ref!=expected))return 1;
  namespace payload=data::quest_objective_payload_v1;
  payload::Runtime reader(*found->second,{this,read_stream,nullptr,nullptr});
  // On full delivery both transient residues are overwritten. A short read
  // stops at the unbound source assertion policy before publishing scratch.
  payload::ReaderScratch scratch(0,0);payload::Result answer;
  if(reader.load(*stream,scratch,&answer)!=payload::Status::complete)return 1;
  ++result.objective_payloads;return 0;
 }
 instance::Services child_services(){
  instance::Services s;
  s.conditions={this,
   [](void* raw,c::List&,std::uint32_t bytes,std::uint32_t tag,c::Array** out){return allocate_array(static_cast<Impl*>(raw)->ca,bytes,tag,out);},
   [](void* raw,c::List&,std::int32_t kind,c::ConditionRef** out)->std::int32_t{cf::Result r;auto& owner=*static_cast<Impl*>(raw);if(owner.condition_factory.create(kind,&r)!=cf::Status::complete)return 1;*out=&r.record->ref;return 0;},
   [](void* raw,c::List&,c::ConditionRef* ref)->std::int32_t{auto& owner=*static_cast<Impl*>(raw);const auto found=owner.conditions.find(ref->identity);if(found==owner.conditions.end()||&found->second->ref!=ref)return 1;cf::Result r;return owner.condition_factory.destroy(*found->second,true,&r)==cf::Status::complete?0:1;},
   [](void* raw,c::List&,c::Array* array){return retire(static_cast<Impl*>(raw)->ca,array,array->identity);},nullptr};
  s.objectives={this,
   [](void* raw,o::List&,std::uint32_t bytes,std::uint32_t tag,o::Array** out){return allocate_array(static_cast<Impl*>(raw)->oa,bytes,tag,out);},
   [](void* raw,o::List&,const o::Definition&,std::int32_t kind,o::ObjectiveRef** out)->std::int32_t{of::Result r;auto& owner=*static_cast<Impl*>(raw);if(owner.objective_factory.create(kind,&r)!=of::Status::complete)return 1;*out=&r.record->ref;return 0;},
   [](void* raw,o::List&,const o::StreamCall& call)->std::int32_t{
    if(!call.objective||!call.virtual_call||call.function!=0x28||call.encoded_adjustment!=1||
       call.adjusted_target!=call.objective->action.identity)return 1;
    return static_cast<Impl*>(raw)->load_objective(call.adjusted_target,call.stream,call.objective);
   },
   [](void* raw,o::List&,o::ObjectiveRef* ref){return static_cast<Impl*>(raw)->delete_objective(ref->action.identity,ref);},
   [](void* raw,o::List&,o::Array* array){return retire(static_cast<Impl*>(raw)->oa,array,array->identity);}};
  s.rewards={this,
   [](void*,r::List& list)->std::int32_t{list.text_8.reserve(15);return 0;},
   [](void*,r::List& list)->std::int32_t{list.text_8.clear();return 0;},
   [](void* raw,r::List&,std::uint32_t bytes,std::uint32_t tag,r::Array** out){return allocate_array(static_cast<Impl*>(raw)->ra,bytes,tag,out);},
   [](void* raw,r::List&,std::int32_t kind,r::RewardRef** out)->std::int32_t{rf::Result r;auto& owner=*static_cast<Impl*>(raw);if(owner.reward_factory.create(kind,&r)!=rf::Status::complete)return 1;*out=&r.record->ref;return 0;},
   [](void* raw,r::List&,r::RewardRef* ref)->std::int32_t{auto& owner=*static_cast<Impl*>(raw);const auto found=owner.rewards.find(ref->identity);if(found==owner.rewards.end()||&found->second->ref!=ref)return 1;rf::Result r;return owner.reward_factory.destroy(*found->second,true,&r)==rf::Status::complete?0:1;},
   [](void* raw,r::List&,r::Array* array){return retire(static_cast<Impl*>(raw)->ra,array,array->identity);},
   [](void*,r::List& list)->std::int32_t{std::string().swap(list.text_8);return 0;}};
  s.leaves={this,[](void* raw,const scalar::Request& request,scalar::Response*)->std::int32_t{
   auto& owner=*static_cast<Impl*>(raw);
   if(request.operation==scalar::Operation::action_virtual){
    if(request.offset==4)return owner.delete_objective(request.target);
    if(request.offset==0x28)return owner.load_objective(request.target,request.stream);
    return 1;
   }
   if(request.operation==scalar::Operation::read_stream_word){
    if(!owner.active_cursor||request.stream!=&owner.active_cursor->stream()||!request.quest||
       request.destination!=&request.quest->state_0||!owner.resolve(&request.quest->ref))return 1;
    namespace reader=data::quest_stream_read_v1;
    reader::Runtime runtime(*request.stream,{raw,read_stream,nullptr,nullptr});reader::Result answer;
    return runtime.read_quest_signed(&request.quest->state_0,&answer)==reader::Status::complete?0:1;
   }
   return 1;
  }};
  return s;
 }
 logs::Services log_services(){return {this,
  [](void* raw,std::uint32_t* out)->std::int32_t{*out=static_cast<Impl*>(raw)->definitions.count();return 0;},
  [](void* raw,std::uintptr_t* out)->std::int32_t{*out=static_cast<Impl*>(raw)->definitions.rows_identity();return 0;},
  [](void* raw,std::uint32_t bytes,std::uint32_t tag,std::uintptr_t* out)->std::int32_t{
   if(bytes!=0x6c||tag)return 1;
   auto& owner=*static_cast<Impl*>(raw);auto slot=std::make_unique<Slot>();*out=reinterpret_cast<std::uintptr_t>(slot.get());
   return owner.slots.emplace(*out,std::move(slot)).second?0:1;
  },
  [](void* raw,std::uintptr_t identity,std::int32_t difficulty,logs::QuestRef** out)->std::int32_t{
   auto& owner=*static_cast<Impl*>(raw);const auto found=owner.slots.find(identity);if(found==owner.slots.end()||found->second->value)return 1;
   auto& value=found->second->value;value=std::make_unique<instance::Instance>(identity,owner.definitions,owner.child_services());
   scalar::Result r;if(value->construct(difficulty,&r)!=scalar::Status::complete)return 1;
   found->second->constructed=true;*out=&value->record().ref;return 0;
  },
  [](void* raw,logs::QuestRef* ref)->std::int32_t{auto* value=static_cast<Impl*>(raw)->resolve(ref);scalar::Result r;return value&&value->owner_children(&r)==scalar::Status::complete?0:1;},
  [](void* raw,std::uint32_t row,std::uintptr_t* out)->std::int32_t{const auto* name=static_cast<Impl*>(raw)->definitions.definition_name(row);if(!name)return 1;*out=reinterpret_cast<std::uintptr_t>(name);return 0;},
  [](void* raw,logs::QuestRef* ref,std::uintptr_t row)->std::int32_t{auto* value=static_cast<Impl*>(raw)->resolve(ref);scalar::Result r;return value&&value->assign_pydata(row,&r)==scalar::Status::complete?0:1;},
  [](void* raw,logs::QuestRef* ref)->std::int32_t{auto* value=static_cast<Impl*>(raw)->resolve(ref);scalar::Result r;return value&&value->reinit(&r)==scalar::Status::complete?0:1;},
  [](void* raw,logs::QuestRef* ref)->std::int32_t{auto* value=static_cast<Impl*>(raw)->resolve(ref);scalar::Result r;return value&&value->destroy(&r)==scalar::Status::complete?0:1;},
  [](void* raw,std::uintptr_t identity)->std::int32_t{auto& owner=*static_cast<Impl*>(raw);const auto found=owner.slots.find(identity);if(found==owner.slots.end())return 1;owner.slots.erase(found);return 0;}};}
 bool run(unsigned log,bool destruction,std::string& error){
  logs::Result r;const auto status=destruction?log_runtime[log]->destroy(&r):log_runtime[log]->init_quests(&r);
  result.published[log]+=r.published;result.reinitialized[log]+=r.reinitialized;result.destroyed[log]+=r.destroyed;
  if(status!=logs::Status::complete){error="Native Quest source provider failed at operation "+std::to_string(unsigned(r.last_operation))+" difficulty "+std::to_string(r.difficulty)+" row "+std::to_string(r.row);return false;}
  return true;
 }
};
std::int32_t Owner::Impl::KillXEnemiesBinding::receive(
 void* raw,Owner::LevelEventRuntime& runtime,Owner::LevelEvent& event,std::string& error){
 if(!raw){error="KillXEnemies receiver context is null";return 1;}
 auto& binding=*static_cast<KillXEnemiesBinding*>(raw);
 if(!binding.objective||!binding.objective->compiled_8||
    binding.objective->dispatch_0!=binding.expected_dispatch){
  error="Kill/Clear receiver lost its canonical compiled Objective";return 1;
 }
 if(event.objective_type!=binding.event_type||event.network_id!=binding.match_id)return 0;
 dh2_kill_objective objective{binding.match_id,
  static_cast<std::int32_t>(binding.objective->quantity_20),
  static_cast<std::int32_t>(binding.objective->derived_2c),
  binding.objective->done_14};
 dh2_kill_progress_event progress{event.network_id,event.subject_id,event.flag0,event.flag1};
 dh2_kill_progress_result result{};
 if(dh2_quest_kill_event(&objective,&progress,&result)!=0){
  error="Kill/Clear source progress rejected invalid Objective/Event fields";return 1;
 }
 if(!result.matched){error.clear();return 0;}
 binding.objective->quantity_20=static_cast<std::uint32_t>(objective.current);
 binding.objective->done_14=static_cast<std::uint8_t>(objective.completed);
 event.flag0=static_cast<std::uint8_t>(progress.outbound);
 event.flag1=static_cast<std::uint8_t>(progress.synchronized);
 event.subject_id=progress.quantity;
 if(result.newly_completed&&binding.script_id>=0&&
    !binding.start_script(binding.script_context,binding.script_id,error)){
  if(error.empty())error="Kill/Clear completion script provider failed";
  return 1;
 }
 if(result.completion_requested&&!binding.detach_pending){
  bool scheduled=false;
  if(!binding.objective->ref.action.identity){
   error="Kill/Clear Objective vtable+28 cannot resolve its receiver identity";return 1;
  }
  const auto status=runtime.delayed_detach(binding.event_type,
       binding.objective->ref.action.identity,scheduled,error);
  if(status!=Owner::LevelEventStatus::complete||!scheduled){
   if(error.empty())error="Kill/Clear Objective::Unregister delayed detach failed";
   return 1;
  }
  binding.detach_pending=true;
 }
 error.clear();return 0;
}
std::int32_t Owner::Impl::TalkToNpcBinding::receive(
 void* raw,Owner::LevelEventRuntime&,Owner::LevelEvent& event,
 std::string& error){
 if(!raw){error="TalkToNPC receiver context is null";return 1;}
 auto& binding=*static_cast<TalkToNpcBinding*>(raw);
 if(!binding.owner||!binding.objective||!binding.objective->compiled_8||
    binding.objective->dispatch_0!=of::Dispatch::talk_to_npc||
    binding.objective->derived_24.index()!=0||
    std::get<std::int32_t>(binding.objective->derived_24)!=binding.target_props_id){
  error="TalkToNPC receiver lost its canonical compiled Objective/target";return 1;
 }
 if(event.objective_type!=binding.event_type||event.network_id!=binding.target_props_id)return 0;
 auto& objective=*binding.objective;
 if(event.flag1){
  if(static_cast<std::int32_t>(objective.quantity_20)>=event.subject_id)return 0;
  objective.quantity_20=std::uint32_t(event.subject_id);
 }else{
  ++objective.quantity_20;
  event.flag0=1;event.subject_id=static_cast<std::int32_t>(objective.quantity_20);
 }
 if(static_cast<std::int32_t>(objective.quantity_20)<binding.target_count||objective.done_14){
  error.clear();return 0;
 }
 if(binding.game_event_trigger){
  // GameEvent Objectives have no Quest parent. SetIsCompleted still owns the
  // Objective byte and optional source script; GameEvent::Update observes the
  // completed list and runs the row script afterward.
  objective.done_14=1;
  if(binding.script_id>=0&&(!binding.start_script||
      !binding.start_script(binding.script_context,binding.script_id,error))){
   if(error.empty())error="GameEvent TalkToNPC completion needs its source script provider";
   return 1;
  }
  if(!binding.detach_pending){
   bool scheduled=false;
   const auto status=binding.owner->impl_->current_level_events.delayed_detach(
       binding.event_type,objective.ref.action.identity,scheduled,error);
   if(status!=Owner::LevelEventStatus::complete||!scheduled){
    if(error.empty())error="GameEvent TalkToNPC delayed detach failed";
    return 1;
   }
   binding.detach_pending=true;
  }
  if(binding.target_marked){
   if(!binding.marker||!binding.marker(binding.marker_context,
       objective.ref.action.identity,&objective.derived_28,
       binding.target_props_id,
       TalkObjectiveMarkerOperationV1::unregister_target,0,0,error)){
    if(error.empty())error="GameEvent TalkToNPC target marker clear failed";
    return 1;
   }
   binding.target_marked=false;
  }
  error.clear();return 0;
 }
 if(!binding.owner->complete_objective(binding.quest_identity,binding.ordinal,
       binding.script_context,binding.start_script,error))return 1;
 // Source Objective_TalkToNPC::Unregister is delayed detach followed by
 // clearing Character+0x2fa for the matching SafeGetCharPropsId.
 if(!binding.owner->unregister_talk_to_npc_objective(binding.quest_identity,
      binding.ordinal,error))return 1;
 error.clear();return 0;
}
bool Owner::Impl::OpenGameObjectBinding::complete(
 void* raw,og::Objective& objective,std::string& error){
 if(!raw){error="OpenGameObject completion binding is null";return false;}
 auto& binding=*static_cast<OpenGameObjectBinding*>(raw);
 if(!binding.owner||binding.objective!=&objective||
    !binding.objective->ref.action.identity){
  error="OpenGameObject completion lost its canonical Save Objective";return false;
 }
 return binding.owner->complete_objective(binding.quest_identity,binding.ordinal,
       binding.script_context,binding.start_script,error);
}
std::int32_t Owner::Impl::MoveInZoneBinding::receive(
 void* raw,Owner::LevelEventRuntime&,Owner::LevelEvent& event,std::string& error){
 if(!raw){error="MoveInZone receiver context is null";return 1;}
 auto& binding=*static_cast<MoveInZoneBinding*>(raw);
 if(!binding.owner||!binding.objective||!binding.objective->compiled_8||
    binding.objective->dispatch_0!=of::Dispatch::move_in_zone||
    binding.objective->derived_28!=binding.target_identity){
  error="MoveInZone receiver lost its canonical compiled Objective/target";return 1;
 }
 if(event.objective_type!=binding.event_type||
    event.object_identity!=binding.target_identity)return 0;
 // IDA 0x47bcd4: null Character is accepted only for the source sync form
 // (flag1=1). Otherwise the target filter runs; -1 means IsPlayer, with the
 // source SafeGetCharPropsId==-1 fallback for non-player Characters.
 if(event.character||!event.flag1){
  if(!event.character){error="MoveInZone local event requires its canonical Character; only a null sync event bypasses filtering";return 1;}
  if(binding.target_props_id!=-1){
   std::int32_t props_id=-1;
   if(!binding.character.safe_get_char_props_id||
      !binding.character.safe_get_char_props_id(binding.character.context,
         event.character,props_id,error)){
    if(error.empty())error="MoveInZone requires the canonical Character::SafeGetCharPropsId provider";
    return 1;
   }
   if(props_id!=binding.target_props_id)return 0;
  }else{
   bool is_player=false;
   if(!binding.character.is_player||
      !binding.character.is_player(binding.character.context,event.character,
                                   is_player,error)){
    if(error.empty())error="MoveInZone target=-1 requires the canonical Character::IsPlayer provider";
    return 1;
   }
   if(!is_player){
    std::int32_t props_id=-1;
    if(!binding.character.safe_get_char_props_id||
       !binding.character.safe_get_char_props_id(binding.character.context,
          event.character,props_id,error)){
     if(error.empty())error="MoveInZone non-player fallback requires Character::SafeGetCharPropsId";
     return 1;
    }
    if(props_id!=-1)return 0;
   }
  }
 }
 auto& objective=*binding.objective;
 if(binding.game_event_trigger){
  if(!objective.done_14){
   objective.done_14=1;
   if(binding.script_id>=0&&(!binding.start_script||
       !binding.start_script(binding.script_context,binding.script_id,error))){
    if(error.empty())error="GameEvent MoveInZone completion needs its source script provider";
    return 1;
   }
  }
  if(!binding.detach_pending){
   bool scheduled=false;
   const auto status=binding.owner->impl_->current_level_events.delayed_detach(
       binding.event_type,objective.ref.action.identity,scheduled,error);
   if(status!=Owner::LevelEventStatus::complete||!scheduled){
    if(error.empty())error="GameEvent MoveInZone delayed detach failed";
    return 1;
   }
   binding.detach_pending=true;
  }
  if(!event.flag1)event.flag0=1;
  error.clear();return 0;
 }
 if(!objective.done_14&&
    !binding.owner->complete_objective(binding.quest_identity,binding.ordinal,
       binding.script_context,binding.start_script,error))return 1;
 if(!binding.detach_pending&&
    !binding.owner->unregister_move_in_zone_objective(binding.quest_identity,
       binding.ordinal,error))return 1;
 if(!event.flag1)event.flag0=1;
 error.clear();return 0;
}
Owner::Owner(std::shared_ptr<data::PlayerSavegameV1> save,data::quest_table_bindings_v1::View definitions,Constants constants):
 impl_(std::make_unique<Impl>(std::move(save),std::move(definitions),std::move(constants))) {}
Owner::~Owner(){std::string error;if(!close(error))std::terminate();}
bool Owner::objective_list_services(data::quest_objective_list_v1::Services& out,
                                    std::string& error)const{
 if(!impl_||impl_->busy||impl_->closed){error="Native Quest ObjectiveList factory is unavailable";return false;}
 out=impl_->child_services().objectives;
 if(!out.allocate||!out.factory||!out.delete_virtual4||!out.deallocate){
  out={};error="Native Quest ObjectiveList factory bundle is incomplete";return false;
 }
 error.clear();return true;
}
bool Owner::resolve_objective_record(
 const data::quest_objective_list_v1::ObjectiveRef& ref,
 data::quest_objective_factory_v1::Record*& out,std::string& error)const{
 out=nullptr;
 if(!impl_||impl_->closed||!ref.action.identity){
  error="Native Quest Objective factory is unavailable for this reference";return false;
 }
 out=impl_->resolve_objective(&ref);
 if(!out){error="ObjectiveRef is not retained by this Native Quest Objective factory";return false;}
 error.clear();return true;
}
bool Owner::game_event_objective_list_predicate_v1(
 const data::game_event_manager_owner_v1::Row& row,bool evaluate_done,
 bool& out,std::string& error)const{
 out=false;
 if(!impl_||impl_->closed||!row.identity||!row.definition||
    row.objectives.count_0<0||
    (row.objectives.count_0>0&&(!row.objectives.children_4||
     std::uint64_t(static_cast<std::uint32_t>(row.objectives.count_0))>
       row.objectives.children_4->slots.size()))){
  error="GameEvent ObjectiveList predicate requires retained valid child storage";return false;
 }
 const auto count=static_cast<std::uint32_t>(row.objectives.count_0);
 for(std::uint32_t index=0;index<count;++index){
  const auto* ref=row.objectives.children_4->slots[index];
  data::quest_objective_factory_v1::Record* record=nullptr;
  if(!ref||!resolve_objective_record(*ref,record,error)||
     record->fields.py_data_c.event!=row.definition||
     record->fields.py_data_c.index>=row.definition->triggers.size()){
   if(error.empty())error="GameEvent ObjectiveList predicate lost its same-row canonical factory child";
   return false;
  }
  if(evaluate_done?!record->done_14:!record->compiled_8){
   out=false;error.clear();return true;
  }
 }
 out=true;error.clear();return true;
}
bool Owner::initialize(std::uint32_t log,std::string& error){
 if(log>1||impl_->busy||impl_->closed){error="Native Quest owner/log unavailable";return false;}
 impl_->busy=true;struct Guard{Impl& owner;~Guard(){owner.busy=false;}}guard{*impl_};error.clear();
 return impl_->run(log,false,error);
}
bool Owner::compile_quests(std::uint32_t log,bool force,
 const QuestCompileServicesV1& services,qc::Result& out,std::string& error){
 if(log>1||impl_->closed||!impl_->save||!services.difficulty||
    !services.objective_member){
  error="CompileQuests requires an open canonical Save log and explicit difficulty/Objective providers";
  return false;
 }
 const auto* previous=impl_->active_compile_services;
 impl_->active_compile_services=&services;
 struct Restore{Impl& owner;const QuestCompileServicesV1* previous;
  ~Restore(){owner.active_compile_services=previous;}} restore{*impl_,previous};
 qc::Result result{};
 const auto status=impl_->compile_log_runtime[log]->compile_quests(force,&result);
 out=result;
 if(status!=qc::Status::complete){
  error="Source CompileQuests adapter stopped at operation "+
    std::to_string(unsigned(result.last_operation))+" status "+
    std::to_string(unsigned(status))+" after "+
    std::to_string(result.visited)+" Quest(s)";
  return false;
 }
 error.clear();return true;
}
bool compose_objective_description_v1(const std::vector<std::int32_t>& ids,
 const QuestTextServicesV1& text,std::string& out,std::string& error){
 std::string value;
 for(const auto id:ids){
  if(id<=0)continue;
  if(!text.string_id){error="Quest objective description requires the existing localization service";return false;}
  std::string resolved;
  if(!text.string_id(text.context,id,resolved,error)){
   if(error.empty())error="Quest objective description localization failed";
   return false;
  }
  if(resolved.empty())continue;
  if(!value.empty())value.push_back('\n');
  value+=resolved;
 }
 out=std::move(value);error.clear();return true;
}
bool Owner::read_quest(std::uint32_t log,std::uint32_t difficulty,
 std::uint32_t ordinal,QuestReadV1& out,std::string& error,
 const QuestTextServicesV1* text) const{
 if(log>1||difficulty>2||impl_->busy||impl_->closed||!impl_->save){
  error="Native Quest read requires an open source Save log and difficulty";return false;
 }
 const auto& store=log?impl_->save->source_quest_log_118():impl_->save->source_quest_log_b8();
 const auto& quests=store.quests[difficulty];
 if(ordinal>=quests.size()||!quests[ordinal]){
  error="Native Quest read ordinal is outside the published source Quest vector";return false;
 }
 auto* instance=impl_->resolve(quests[ordinal]);
 if(!instance||instance->record().ref.identity!=quests[ordinal]->identity||
    instance->record().fields.row_8<0||!instance->record().py_data_68||
    impl_->definitions.resolve(instance->record().py_data_68->identity)!=instance->record().py_data_68){
  error="Native Quest read cannot resolve the same Save-published Quest and pydata row";return false;
 }
 const auto* definition=impl_->definitions.record(*instance->record().py_data_68);
 if(!definition){error="Native Quest read pydata row has no validated immutable definition";return false;}
 QuestReadV1 value;
 value.id=instance->record().fields.row_8;
 value.state=instance->record().state_0;
 value.priority=definition->priority;
 for(std::size_t i=0;i<value.text_ids.size();++i)value.text_ids[i]=definition->ids[i];
 if(text){
  const auto& list=instance->objectives();
  std::vector<std::int32_t> objective_text_ids;
  if(list.count_0>0){
   if(!list.children_4||!list.children_4->identity||
      std::uint64_t(std::uint32_t(list.count_0))>list.children_4->slots.size()){
    error="Quest objective description cannot resolve its canonical ObjectiveList";return false;
   }
   objective_text_ids.reserve(std::uint32_t(list.count_0));
   for(std::int32_t index=0;index<list.count_0;++index){
    const auto* objective=impl_->resolve_objective(list.children_4->slots[std::size_t(index)]);
    if(!objective){error="Quest objective description found a foreign canonical Objective";return false;}
    const auto& retained=objective->fields.py_data_c;
    if(!retained.list||retained.stub||retained.list->kind!=1){
     error="Quest objective description requires retained Objective definitions of kind 1";return false;
    }
    dh2_quest_objective source{};
    if(!objective_definition(*objective,source,error))return false;
    objective_text_ids.push_back(source.common[1]);
   }
  }
  if(!compose_objective_description_v1(objective_text_ids,*text,
                                      value.objective_description,error))return false;
 }
 out=value;error.clear();return true;
}
bool Owner::lookup_quest_state(std::int32_t quest_id,std::int32_t difficulty,
 bool online,std::int32_t* state,QuestLookupStatusV1& status) const noexcept{
 status=QuestLookupStatusV1::unavailable;
 if(!state||difficulty<0||difficulty>2||impl_->busy||impl_->closed||!impl_->save)
  return false;
 if(quest_id<0||std::uint32_t(quest_id)>=impl_->definitions.count()){
  status=QuestLookupStatusV1::missing;return false;
 }
 const auto& log=online?impl_->save->source_quest_log_118():
                        impl_->save->source_quest_log_b8();
 const auto& quests=log.quests[std::uint32_t(difficulty)];
 if(std::uint32_t(quest_id)>=quests.size()){
  status=QuestLookupStatusV1::compile_required;return false;
 }
 auto* instance=impl_->resolve(quests[std::uint32_t(quest_id)]);
 if(!instance||!instance->record().py_data_68||
    instance->record().fields.row_8!=quest_id||
    impl_->definitions.resolve(instance->record().py_data_68->identity)!=
      instance->record().py_data_68){
  status=QuestLookupStatusV1::source_fault;return false;
 }
 *state=instance->record().state_0;status=QuestLookupStatusV1::found;return true;
}
bool Owner::update_active(std::uintptr_t quest_identity,std::uint32_t log,
 std::uint32_t difficulty,
 const QuestUpdateActiveServicesV1& services,QuestUpdateActiveResultV1& out,
 std::string& error){
 if(log>1||difficulty>2||impl_->busy||impl_->closed||!impl_->save){
  error="Quest::UpdateActive requires the open canonical Save/QEST owner";return false;
 }
 auto* quest=impl_->resolve_quest_identity(quest_identity);
 if(!quest||!quest->record().py_data_68||
    impl_->definitions.resolve(quest->record().py_data_68->identity)!=
      quest->record().py_data_68){
  error="Quest::UpdateActive requires a Quest published in this Save and definition generation";return false;
 }
 const auto& selected_log=log?impl_->save->source_quest_log_118():
                              impl_->save->source_quest_log_b8();
 const auto& selected_quests=selected_log.quests[difficulty];
 if(std::none_of(selected_quests.begin(),selected_quests.end(),[&](const auto* ref){
     return ref&&ref->identity==quest_identity&&ref==&quest->record().ref;
    })){
  error="Quest::UpdateActive Quest does not belong to the supplied Save log/difficulty";return false;
 }
 QuestUpdateActiveResultV1 value;value.quest_identity=quest_identity;
 value.prior_state=quest->record().state_0;
 if(value.prior_state!=6){value.outcome=QuestUpdateActiveOutcomeV1::inactive;out=std::move(value);error.clear();return true;}

 // ObjectiveList::Eval is a direct byte scan in source order (ELF 0x47a444):
 // each canonical Objective+0x14 done byte must be nonzero. Empty lists pass.
 // Do not call a guessed virtual IsComplete or allocate a parallel done map.
 auto& list=quest->objectives();
 if(list.count_0<0||(list.count_0>0&&(!list.children_4||!list.children_4->identity||
    std::uint64_t(std::uint32_t(list.count_0))>list.children_4->slots.size()))){
  error="Quest::UpdateActive found invalid canonical ObjectiveList backing";return false;
 }
 for(std::uint32_t ordinal=0;ordinal<std::uint32_t(list.count_0);++ordinal){
  const auto* objective=impl_->resolve_objective(list.children_4->slots[ordinal]);
  if(!objective){error="Quest::UpdateActive could not resolve a canonical ObjectiveList child";return false;}
  if(!objective->done_14){value.outcome=QuestUpdateActiveOutcomeV1::objectives_pending;out=std::move(value);error.clear();return true;}
 }

 const auto* definition=impl_->definitions.record(*quest->record().py_data_68);
 if(!definition){error="Quest::UpdateActive could not read its retained Quest row";return false;}
 auto script_suffix=[&](std::uint32_t slot,std::string& suffix)->bool{
  if(slot>=14){error="Quest::UpdateActive script slot is outside the source row";return false;}
  const auto& span=definition->scripts[slot];
  if(!span.size){suffix.clear();return true;}
  data::quest_table_bindings_v1::Span bytes{};
  if(!impl_->definitions.bytes(span,&bytes,error)||!bytes.data||!bytes.size){
   if(error.empty())error="Quest::UpdateActive source script span is invalid";
   return false;
  }
  std::string source(reinterpret_cast<const char*>(bytes.data),bytes.size);
  const auto nul=source.find('\0');if(nul!=std::string::npos)source.resize(nul);
  const auto dot=source.find('.');
  if(dot==std::string::npos||dot+1==source.size()){
   error="Quest::UpdateActive source script reference has no resolvable manager name";
   return false;
  }
  suffix=source.substr(dot+1);
  return true;
 };
 if(!script_suffix(0,value.active_script)||!script_suffix(5,value.post_active_script))return false;
 if(!value.active_script.empty()){
  if(!services.script_is_running){error="Quest::UpdateActive requires the current Level's ScriptManager running-state provider";return false;}
  bool running=false;
  if(!services.script_is_running(services.context,value.active_script,running,error)){
   if(error.empty())error="Quest::UpdateActive ScriptManager query failed";
   return false;
  }
  if(running){value.outcome=QuestUpdateActiveOutcomeV1::completion_script_running;out=std::move(value);error.clear();return true;}
 }
 if(!services.application_time_valid){error="Quest::SetState requires the current Application time snapshot";return false;}
 std::int32_t post_active_state=0;
 if(impl_->constant("v2QuestState","PostActive",&post_active_state)||
    post_active_state<0||post_active_state>13){
  error="Quest::UpdateActive could not resolve the source v2QuestState.PostActive constant";return false;
 }
 // Source SetState stores state and timestamp first, unregisters state-6
 // objectives, then its PostActive switch clears the current Quest and starts
 // script slot 5. The target state is nonvolatile (IsVolatileState(7)==false).
 auto& mutable_record=quest->record();
 mutable_record.state_0=post_active_state;
 mutable_record.word_4=services.application_time;
 if(!unregister_objectives(quest_identity,error))return false;
 auto& current_log=log?impl_->save->source_quest_log_118():
                        impl_->save->source_quest_log_b8();
 current_log.word_2c[difficulty]=-1;
 if(!value.post_active_script.empty()){
  if(!services.start_script){error="Quest::ExecScript(PostActive slot 5) requires the current ScriptManager start provider";return false;}
  if(!services.start_script(services.context,value.post_active_script,-1,true,error)){
   if(error.empty())error="Quest::ExecScript(PostActive slot 5) failed";
   return false;
  }
 }
 value.new_state=post_active_state;value.outcome=QuestUpdateActiveOutcomeV1::state_transitioned;
 out=std::move(value);error.clear();return true;
}
bool Owner::unregister_objectives(std::uintptr_t quest_identity,std::string& error){
 auto* quest=impl_->resolve_quest_identity(quest_identity);
 if(!quest){error="ObjectiveList::Unregister requires a Quest published in this Save";return false;}
 auto& unregister_list=quest->objectives();
 if(unregister_list.count_0<0||(unregister_list.count_0>0&&(!unregister_list.children_4||
    std::uint64_t(std::uint32_t(unregister_list.count_0))>unregister_list.children_4->slots.size()))){
  error="ObjectiveList::Unregister found invalid canonical ObjectiveList backing";return false;
 }
 for(std::uint32_t ordinal=0;ordinal<std::uint32_t(unregister_list.count_0);++ordinal){
  auto* objective=impl_->resolve_objective(unregister_list.children_4->slots[ordinal]);
  if(!objective){error="Quest::SetState could not resolve an Objective during Unregister";return false;}
  if(!objective->compiled_8)continue;
  const auto identity=objective->ref.action.identity;
  bool ok=false;
  switch(objective->dispatch_0){
   case of::Dispatch::gather_loot:
    ok=unregister_gather_loot_objective(identity,error);break;
   case of::Dispatch::talk_to_npc:
    ok=unregister_talk_to_npc_objective(quest_identity,ordinal,error);break;
   case of::Dispatch::move_in_zone:
    ok=unregister_move_in_zone_objective(quest_identity,ordinal,error);break;
   case of::Dispatch::open_game_object:
    ok=unregister_open_game_object_objective(quest_identity,ordinal,error);break;
   case of::Dispatch::kill_enemies:
    ok=unregister_kill_x_enemies_objective(quest_identity,ordinal,error);break;
   case of::Dispatch::clear_enemies:
    ok=unregister_clear_enemies_objective(quest_identity,ordinal,error);break;
   case of::Dispatch::kill_enemy_template:
    ok=unregister_kill_enemy_template_objective(quest_identity,ordinal,error);break;
   case of::Dispatch::clear_enemy_template:
    ok=unregister_clear_enemy_template_objective(quest_identity,ordinal,error);break;
   default:
    error="Quest::SetState ObjectiveList::Unregister reached a compiled Objective without a source owner";return false;
  }
  if(!ok)return false;
 }
 error.clear();return true;
}
bool Owner::update_active_log(std::uint32_t log,std::uint32_t difficulty,
 const QuestUpdateActiveServicesV1& services,
 std::vector<QuestUpdateActiveResultV1>& out,std::string& error){
 if(log>1||difficulty>2||impl_->busy||impl_->closed||!impl_->save){
  error="QuestSavegame::UpdateQuests requires its selected open Save log/difficulty";return false;
 }
 const auto& store=log?impl_->save->source_quest_log_118():
                       impl_->save->source_quest_log_b8();
 const auto& quests=store.quests[difficulty];
 std::vector<QuestUpdateActiveResultV1> values;values.reserve(quests.size());
 for(const auto* ref:quests){
  if(!ref){error="QuestSavegame::UpdateQuests found a null canonical Quest entry";return false;}
  QuestUpdateActiveResultV1 value{};
  if(!update_active(ref->identity,log,difficulty,services,value,error))return false;
  values.push_back(std::move(value));
 }
 out=std::move(values);error.clear();return true;
}
bool Owner::update_lifecycle(std::uint32_t log,std::uint32_t difficulty,
 const QuestLifecycleServicesV1& services,
 std::vector<QuestLifecycleResultV1>& out,std::string& error){
 out.clear();
 if(log>1||difficulty>2||impl_->busy||impl_->closed||!impl_->save){
  error="Quest lifecycle requires the open canonical Save/QEST owner";return false;
 }
 if(!services.current_level_index||!services.script_exists||
    !services.save_for_state_transition||!services.announce_new_quest||
    !services.announce_quest_completed||!services.reward_text||
    !services.scripts.script_is_running||!services.scripts.start_script||
    !services.scripts.application_time_valid||!services.objectives.difficulty||
    !services.objectives.objective_member){
  error="Quest lifecycle requires the Level's script, save, message and Objective providers";
  return false;
 }
 auto& store=log?impl_->save->source_quest_log_118():impl_->save->source_quest_log_b8();
 const auto& quests=store.quests[difficulty];
 // Registration and marker virtuals reach the Level through the same
 // Objective member provider CompileQuests uses.
 const auto* previous_services=impl_->active_compile_services;
 impl_->active_compile_services=&services.objectives;
 struct RestoreServices{Impl& impl;const QuestCompileServicesV1* previous;
  ~RestoreServices(){impl.active_compile_services=previous;}} restore{*impl_,previous_services};
 std::int32_t primary_priority=0;
 if(impl_->constant("v2QuestPriority","Primary",&primary_priority)){
  error="Quest lifecycle could not resolve v2QuestPriority.Primary";return false;
 }

 // ConditionList::Eval (0x478704): every child's virtual Eval must hold; an
 // empty list is true. Condition kinds 0..3 evaluate through the recovered
 // condition kernel against this Save's quest vector and the current Level.
 struct EvalContext {
  Impl* impl=nullptr;const QuestLifecycleServicesV1* services=nullptr;
  const logs::QuestSavegame* store=nullptr;std::uint32_t difficulty=0;
  std::uintptr_t character_660=0;
  ce::PlayerRef player{};ce::QuestStateRef quest{};ce::LevelRef level{};
  std::int32_t level_index=-1;std::string error;
 } context{};
 context.impl=impl_.get();context.services=&services;
 context.store=&store;context.difficulty=difficulty;
 const ce::Services eval_services{&context,
  [](void*,std::uintptr_t* application)->std::int32_t{*application=1;return 0;},
  [](void*,std::uintptr_t,std::uintptr_t* manager)->std::int32_t{*manager=2;return 0;},
  [](void* raw,std::uintptr_t,std::int32_t ordinal,std::uint32_t require_character,
     ce::PlayerRef** player)->std::int32_t{
   auto& self=*static_cast<EvalContext*>(raw);
   const auto character=self.impl->save->character();
   if(ordinal!=0||(require_character&&!character))return 1;
   self.character_660=character;self.player={1,&self.character_660};
   *player=&self.player;return 0;
  },
  [](void* raw,std::uintptr_t character,std::int32_t quest_id,std::int32_t quest_difficulty,
     ce::QuestStateRef** quest)->std::int32_t{
   auto& self=*static_cast<EvalContext*>(raw);
   // Character::SG_GetQuestByID(id,-1) selects the current difficulty.
   if(character!=self.impl->save->character()||
      (quest_difficulty!=-1&&quest_difficulty!=std::int32_t(self.difficulty)))return 1;
   *quest=nullptr;
   const auto& vector=self.store->quests[self.difficulty];
   if(quest_id<0||std::size_t(quest_id)>=vector.size())return 0;
   auto* instance=self.impl->resolve(vector[std::size_t(quest_id)]);
   if(!instance)return 1;
   self.quest={instance->record().ref.identity,&instance->record().state_0};
   *quest=&self.quest;return 0;
  },
  [](void* raw,std::uintptr_t,ce::LevelRef** level)->std::int32_t{
   auto& self=*static_cast<EvalContext*>(raw);
   if(!self.services->current_level_index(self.services->context,self.level_index,
        self.error))return 1;
   self.level={1,&self.level_index};*level=&self.level;return 0;
  }};
 const auto condition_record=[&](const c::ConditionRef* ref)->cf::Record*{
  if(!ref||!ref->identity)return nullptr;
  const auto found=impl_->conditions.find(ref->identity);
  return found==impl_->conditions.end()||&found->second->ref!=ref?nullptr:found->second.get();
 };
 const auto conditions_supported=[&](instance::Instance& quest)->bool{
  auto& list=quest.conditions();
  if(list.count_0<=0)return true;
  if(!list.children_4||std::uint64_t(std::uint32_t(list.count_0))>list.children_4->slots.size())
   return false;
  for(std::int32_t index=0;index<list.count_0;++index){
   const auto* record=condition_record(list.children_4->slots[std::size_t(index)]);
   if(!record)return false;
   switch(record->dispatch_0){
   case cf::Dispatch::quest_in_state:case cf::Dispatch::quest_state_lower:
   case cf::Dispatch::quest_state_higher:case cf::Dispatch::player_in_level:break;
   default:return false;
   }
  }
  return true;
 };
 const auto eval_conditions=[&](instance::Instance& quest,bool& value)->bool{
  value=true;
  auto& list=quest.conditions();
  for(std::int32_t index=0;index<list.count_0;++index){
   auto* record=condition_record(list.children_4->slots[std::size_t(index)]);
   if(!record){error="ConditionList::Eval lost its canonical Condition";return false;}
   ce::Runtime runtime(*record,eval_services);ce::Result evaluated;
   context.error.clear();
   if(runtime.evaluate(&evaluated)!=ce::Status::complete){
    error="Condition::Eval failed at operation "+std::to_string(unsigned(evaluated.last_operation))+
          (context.error.empty()?"":" | "+context.error);
    return false;
   }
   if(!evaluated.value){value=false;return true;}
  }
  return true;
 };
 // Quest::GetScriptIDFromID (0x47fa80): v2QuestScripts[slot] is "Module.Name";
 // the ScriptManager name is the text after the first '.', and a string
 // without one (or an empty one) is no script.
 const auto slot_script=[&](instance::Instance& quest,std::uint32_t slot,
                            std::string& name)->bool{
  name.clear();
  const auto* definition=impl_->definitions.record(*quest.record().py_data_68);
  if(!definition||slot>=14){error="Quest script slot is outside the source row";return false;}
  const auto& span=definition->scripts[slot];
  if(!span.size)return true;
  data::quest_table_bindings_v1::Span bytes{};
  if(!impl_->definitions.bytes(span,&bytes,error)||!bytes.data||!bytes.size){
   if(error.empty())error="Quest source script span is invalid";
   return false;
  }
  std::string source(reinterpret_cast<const char*>(bytes.data),bytes.size);
  const auto nul=source.find('\0');if(nul!=std::string::npos)source.resize(nul);
  const auto dot=source.find('.');
  if(dot!=std::string::npos&&dot+1!=source.size())name=source.substr(dot+1);
  return true;
 };
 const auto script_present=[&](instance::Instance& quest,std::uint32_t slot,bool& present)->bool{
  std::string name;present=false;
  if(!slot_script(quest,slot,name))return false;
  if(name.empty())return true;
  return services.script_exists(services.context,name,present,error);
 };
 // Quest::TestIsScriptRunning (0x47fde4).
 const auto script_running=[&](instance::Instance& quest,std::uint32_t slot,bool& running)->bool{
  std::string name;running=false;
  if(!slot_script(quest,slot,name))return false;
  if(name.empty())return true;
  if(!services.scripts.script_is_running(services.scripts.context,name,running,error)){
   if(error.empty())error="Quest::TestIsScriptRunning ScriptManager query failed";
   return false;
  }
  return true;
 };
 // Quest::ExecScript (0x47ff20): ScriptManager::StartScript(id,-1,true).
 const auto exec_script=[&](instance::Instance& quest,std::uint32_t slot)->bool{
  std::string name;
  if(!slot_script(quest,slot,name))return false;
  if(name.empty())return true;
  if(!services.scripts.start_script(services.scripts.context,name,-1,true,error)){
   if(error.empty())error="Quest::ExecScript could not start its ScriptManager script";
   return false;
  }
  return true;
 };

 // The accept (+0x18) / end (+0x1c) Objective virtuals SetState and the
 // Compile-time registration reach: InstallObjectiveMarker +0x10 (priority,
 // state), RemoveObjectiveMarker +0x14, Register +0x18, Unregister +0x1c. They
 // run on the canonical Objective factory Record through the same Level-backed
 // Objective member provider CompileQuests uses.
 const auto action_virtual=[&](instance::Instance& quest,bool end_action,
     std::uint32_t function,std::int32_t priority,std::int32_t state)->bool{
  auto& record=quest.record();
  auto* action=impl_->resolve_compile_objective(end_action?record.action_1c:record.action_18);
  if(!action){
   error=end_action?"Quest::SetState lost its end Objective":"Quest::SetState lost its accept Objective";
   return false;
  }
  const qc::MemberCall call{function,0,true,priority,state};
  if(services.objectives.objective_member(services.objectives.context,*action,call)!=0){
   if(error.empty())error=std::string("Quest ")+(end_action?"end":"accept")+
       " Objective virtual +"+std::to_string(function)+" failed";
   return false;
  }
  return true;
 };
 // RewardList::Give (0x482920) through Reward_Gold/XP::Give; Quest::GiveRewards
 // then clears Quest+0x5c whatever the list returned.
 const auto give_rewards=[&](instance::Instance& quest)->bool{
  auto& record=quest.record();
  auto& list=quest.rewards();
  const auto character=impl_->save->character();
  if(!character||store.character_5c!=character||record.fields.character_60!=character){
   error="Quest::GiveRewards requires the current Character bound to this Save and QEST log";
   return false;
  }
  if(list.count_0<0||(list.count_0>0&&(!list.children_4||!list.children_4->identity||
     std::uint64_t(std::uint32_t(list.count_0))>list.children_4->slots.size()))){
   error="Quest::GiveRewards found invalid canonical RewardList backing";return false;
  }
  if(list.count_0>0&&(!services.rewards.resolve_character||!services.rewards.give_xp)){
   error="Quest::GiveRewards requires Character resolution and Character::_GiveXP providers";
   return false;
  }
  impl_->busy=true;
  struct Guard{Impl& impl;~Guard(){impl.busy=false;}}guard{*impl_};
  const re::Services reward_services{const_cast<QuestRewardServicesV1*>(&services.rewards),
   [](void* raw,std::uintptr_t requested)->re::CharacterRef*{
    auto& rewards=*static_cast<QuestRewardServicesV1*>(raw);
    auto* resolved=rewards.resolve_character(rewards.context,requested);
    return resolved&&resolved->identity==requested?resolved:nullptr;
   },
   [](void* raw,re::Record& reward,re::CharacterRef& resolved,std::int32_t amount)->std::int32_t{
    auto& rewards=*static_cast<QuestRewardServicesV1*>(raw);
    return rewards.add_gold?rewards.add_gold(rewards.context,reward,resolved,amount):1;
   },
   [](void* raw,re::Record& reward,re::CharacterRef& resolved,std::int32_t fixed_point,
      std::uint8_t source_flag,bool* granted)->std::int32_t{
    auto& rewards=*static_cast<QuestRewardServicesV1*>(raw);
    return rewards.give_xp(rewards.context,reward,resolved,fixed_point,source_flag,granted);
   }};
  for(std::int32_t index=0;index<list.count_0;++index){
   auto* reward=impl_->resolve_compile_reward(list.children_4->slots[std::size_t(index)]);
   if(!reward||reward->fields.character_10!=character){
    error="Quest::GiveRewards reached a foreign Reward or Character owner";return false;
   }
   re::Runtime runtime(*reward,reward_services);re::Result result{};
   re::Status status=re::Status::source_fault;
   switch(reward->dispatch_0){
    case rf::Dispatch::gold:status=runtime.give_gold(&result);break;
    case rf::Dispatch::xp:status=runtime.give_xp(&result);break;
    default:
     error="Quest::GiveRewards reached a Reward kind without a recovered Give implementation";
     return false;
   }
   if(status!=re::Status::complete){
    error="Recovered Reward::Give failed at operation "+
          std::to_string(static_cast<unsigned>(result.last_operation));
    return false;
   }
   if(result.value==0)break; // RewardList::Give stops at the first zero result.
  }
  record.byte_5c=0;
  return true;
 };
 // Reward::GetParam1 (0x482874): the amount, the second word of the reward stub.
 const auto reward_param1=[&](rf::Record& reward,std::int32_t& value)->bool{
  const auto& definition=reward.fields.py_data_c;
  dh2_quest_span span{};data::quest_table_bindings_v1::Span bytes{};
  if(!definition.list||definition.list->kind<2||definition.list->kind>4||
     !definition.view.list_record(*definition.list,definition.index,&span,error)||
     !definition.view.bytes(span,&bytes,error)||bytes.size!=12||!bytes.data){
   if(error.empty())error="Reward::GetParam1 could not read its retained reward row";
   return false;
  }
  std::uint32_t raw=std::uint32_t(bytes.data[4])|(std::uint32_t(bytes.data[5])<<8)|
                    (std::uint32_t(bytes.data[6])<<16)|(std::uint32_t(bytes.data[7])<<24);
  std::memcpy(&value,&raw,sizeof(value));
  return true;
 };
 // RewardList::CreateRewardString (0x4835a0): the list's own string gains one
 // line per compiled Gold/XP reward; nothing produced assigns the "none" text.
 const auto create_reward_string=[&](instance::Instance& quest)->bool{
  auto& list=quest.rewards();
  if(list.count_0<=0)return true;
  if(!list.children_4||!list.children_4->identity||
     std::uint64_t(std::uint32_t(list.count_0))>list.children_4->slots.size()){
   error="RewardList::CreateRewardString found invalid canonical RewardList backing";return false;
  }
  if(!services.reward_text){error="RewardList::CreateRewardString requires the StringManager provider";return false;}
  bool first=true;
  for(std::int32_t index=0;index<list.count_0;++index){
   auto* reward=impl_->resolve_compile_reward(list.children_4->slots[std::size_t(index)]);
   if(!reward){error="RewardList::CreateRewardString lost a canonical Reward";return false;}
   if(!reward->compiled_8)continue;
   std::string line;
   const auto type=reward->fields.type_4;
   if(type==0||type==1){
    std::int32_t amount=0;
    if(!reward_param1(*reward,amount))return false;
    if(!services.reward_text(services.context,type==0?QuestRewardTextKindV1::gold:
                             QuestRewardTextKindV1::xp,amount,line,error)){
     if(error.empty())error="RewardList::CreateRewardString text provider failed";
     return false;
    }
    if(!first)list.text_8.append("\n");
   }
   list.text_8.append(line);
   first=false;
  }
  if(first){
   std::string none;
   if(!services.reward_text(services.context,QuestRewardTextKindV1::none,0,none,error)){
    if(error.empty())error="RewardList::CreateRewardString none-text provider failed";
    return false;
   }
   list.text_8=none;
  }
  return true;
 };

 // Quest::SetState (0x480c78): store state and time, leave the old state
 // (Active: ObjectiveList::Unregister; Completed/Available: the end/accept
 // Objective's Unregister), save across a volatile state, then enter `next`.
 const auto set_state=[&](instance::Instance& quest,std::int32_t next,
                          QuestLifecycleResultV1& trace)->bool{
  auto& record=quest.record();
  if(next<0||next>13)return true;
  const auto old=record.state_0;
  record.state_0=next;
  if(old!=next)record.word_4=services.scripts.application_time;
  else if(!record.byte_64)return true;
  else record.word_4=services.scripts.application_time;
  trace.path.push_back(next);
  if(old==6){
   if(!unregister_objectives(record.ref.identity,error))return false;
  }else if(old==9){
   if(!action_virtual(quest,true,0x1c,0,0))return false;
  }else if(old==3){
   if(!action_virtual(quest,false,0x1c,0,0))return false;
  }
  if(scalar::is_volatile_state(next)&&!record.byte_64){
   // Quest::HandleSaveInStateTransitonIfNeeded (0x47fb74): save when the entered
   // state starts a script that exists.
   static constexpr std::int32_t save_slots[14][2]{
     {-1,-1},{-1,-1},{11,1},{-1,-1},{6,-1},{10,0},{-1,-1},
     {5,-1},{13,3},{-1,-1},{8,-1},{12,2},{-1,-1},{7,-1}};
   bool save=false;
   for(const auto slot:save_slots[next]){
    if(slot<0)continue;
    bool present=false;
    if(!script_present(quest,std::uint32_t(slot),present))return false;
    save=save||present;
   }
   if(save&&!services.save_for_state_transition(services.context,error))return false;
  }
  const auto* definition=impl_->definitions.record(*record.py_data_68);
  if(!definition){error="Quest::SetState lost its retained Quest row";return false;}
  const auto priority=definition->priority;
  switch(record.state_0){
  case 0:return true;
  case 1:return exec_script(quest,9);
  case 2:
   if(!action_virtual(quest,false,0x10,priority,2))return false;
   return exec_script(quest,11);
  case 3:
   if(!action_virtual(quest,false,0x18,0,0))return false;
   return exec_script(quest,old==6?4:1);
  case 4:
   if(!action_virtual(quest,false,0x14,0,0))return false;
   return exec_script(quest,6);
  case 5:{
   auto* runtime=impl_->resolve_compile_quest(&record.ref);
   if(!runtime){error="Quest::SetState(PreActive) lost its canonical compile runtime";return false;}
   qc::Result installed;
   if(runtime->install_markers(priority,5,&installed)!=qc::Status::complete){
    if(error.empty())error="ObjectiveList::InstallObjectiveMarkers stopped at operation "+
        std::to_string(unsigned(installed.last_operation));
    return false;
   }
   return exec_script(quest,10);
  }
  case 6:{
   if(!exec_script(quest,0))return false;
   auto* runtime=impl_->resolve_compile_quest(&record.ref);
   if(!runtime){error="Quest::SetState(Active) lost its canonical compile runtime";return false;}
   qc::Result registered;
   if(runtime->register_objectives(&registered)!=qc::Status::complete){
    if(error.empty())error="ObjectiveList::Register stopped at operation "+
        std::to_string(unsigned(registered.last_operation));
    return false;
   }
   if(!services.announce_new_quest(services.context,definition->ids[0],error))return false;
   // Character::SG_SetCurrentQuest / SG_SetCurrentPrimaryQuest (difficulty -1).
   store.word_2c[difficulty]=record.fields.row_8;
   if(definition->priority==primary_priority)store.word_38[difficulty]=record.fields.row_8;
   return true;
  }
  case 7:
   store.word_2c[difficulty]=-1; // Character::SG_SetCurrentQuest(-1,-1)
   return exec_script(quest,5);
  case 8:
   if(!action_virtual(quest,true,0x10,priority,8))return false;
   return exec_script(quest,13);
  case 9:{
   if(!action_virtual(quest,true,0x18,0,0))return false;
   auto* runtime=impl_->resolve_compile_quest(&record.ref);
   if(!runtime){error="Quest::SetState(Completed) lost its canonical compile runtime";return false;}
   qc::Result removed;
   if(runtime->loop_objectives(0x14,1,&removed)!=qc::Status::complete){
    if(error.empty())error="ObjectiveList::RemoveObjectiveMarkers stopped at operation "+
        std::to_string(unsigned(removed.last_operation));
    return false;
   }
   if(!exec_script(quest,3))return false;
   // Character::SG_SetCurrentAct keeps the highest act reached.
   if(record.word_c>store.word_44[difficulty])store.word_44[difficulty]=record.word_c;
   return true;
  }
  case 10:
   if(!action_virtual(quest,true,0x14,0,0))return false;
   return exec_script(quest,8);
  case 11:return exec_script(quest,12);
  case 12:
   if(!exec_script(quest,2)||!create_reward_string(quest))return false;
   return services.announce_quest_completed(services.context,definition->ids[0],
                                            quest.rewards().text_8,error);
  case 13:return exec_script(quest,7);
  default:return true;
  }
 };

 for(const auto* ref:quests){
  if(!ref){error="Quest lifecycle found a null canonical Quest entry";return false;}
  auto* quest=impl_->resolve(ref);
  if(!quest||!quest->record().py_data_68||
     impl_->definitions.resolve(quest->record().py_data_68->identity)!=
       quest->record().py_data_68){
   error="Quest lifecycle could not resolve a published Quest";return false;
  }
  auto& record=quest->record();
  QuestLifecycleResultV1 result;result.quest_identity=record.ref.identity;
  result.quest_id=record.fields.row_8;
  result.prior_state=result.new_state=record.state_0;
  if(record.state_0<0||record.state_0>13)continue;
  const auto* accept=impl_->resolve_compile_objective(record.action_18);
  const auto* end=impl_->resolve_compile_objective(record.action_1c);
  const auto supported_action=[](const of::Record* action){
   return action&&(action->dispatch_0==of::Dispatch::base||
                   action->dispatch_0==of::Dispatch::automatic||
                   action->dispatch_0==of::Dispatch::event_receiver||
                   action->dispatch_0==of::Dispatch::talk_to_npc||
                   action->dispatch_0==of::Dispatch::move_in_zone);
  };
  if(!supported_action(accept)||!supported_action(end)){
   result.outcome=QuestLifecycleOutcomeV1::unsupported;
   result.reason="accept or end Objective kind has no InstallObjectiveMarker/Register owner";
   out.push_back(std::move(result));continue;
  }
  if(!conditions_supported(*quest)){
   result.outcome=QuestLifecycleOutcomeV1::unsupported;
   result.reason="ConditionList holds a Condition kind the kernel does not evaluate";
   out.push_back(std::move(result));continue;
  }
  // Quest::Update (0x481818): a Quest loaded in a volatile state re-enters it.
  if(record.byte_64){
   if(!set_state(*quest,record.state_0,result))return false;
   record.byte_64=0;
  }
  bool running=false,holds=false;
  // Each state test re-reads Quest+0 (see the dispatch in Quest::Update), so a
  // Quest cascades through every following state its scripts allow in one pass.
  if(record.state_0==0){ // UpdateLocked 0x4817b4
   if(!eval_conditions(*quest,holds))return false;
   if(holds&&!set_state(*quest,1,result))return false;
  }
  if(record.state_0==1){ // UpdatePostLocked 0x4813e4
   if(!script_running(*quest,9,running))return false;
   if(!running&&!set_state(*quest,2,result))return false;
  }
  if(record.state_0==2){ // UpdatePreAvailable 0x481380
   if(!script_running(*quest,11,running))return false;
   if(!running&&!set_state(*quest,3,result))return false;
  }
  if(record.state_0==3){ // UpdateAvailable 0x481700
   if(!eval_conditions(*quest,holds))return false;
   if(!holds){if(!set_state(*quest,0,result))return false;}
   else{
    if(!script_running(*quest,1,running))return false;
    // The accept Objective's done byte (+0x14) is set by its completion event.
    if(accept->done_14&&!running&&!set_state(*quest,4,result))return false;
   }
  }
  if(record.state_0==4){ // UpdatePostAvailable 0x48131c
   if(!script_running(*quest,6,running))return false;
   if(!running&&!set_state(*quest,5,result))return false;
  }
  if(record.state_0==5){ // UpdatePreActive 0x4815d4 (offline)
   if(!script_running(*quest,10,running))return false;
   if(!running&&!set_state(*quest,6,result))return false;
  }
  if(record.state_0==6){ // UpdateActive 0x481688: ObjectiveList::Eval, then slot 0
   // ObjectiveList::Eval (0x47a444) is a byte scan: every child's done byte set.
   auto& objectives=quest->objectives();
   if(objectives.count_0<0||(objectives.count_0>0&&(!objectives.children_4||
      std::uint64_t(std::uint32_t(objectives.count_0))>objectives.children_4->slots.size()))){
    error="Quest::UpdateActive found invalid canonical ObjectiveList backing";return false;
   }
   bool all_done=true;
   for(std::int32_t index=0;index<objectives.count_0&&all_done;++index){
    const auto* child=impl_->resolve_objective(objectives.children_4->slots[std::size_t(index)]);
    if(!child){error="Quest::UpdateActive could not resolve a canonical ObjectiveList child";return false;}
    all_done=child->done_14!=0;
   }
   if(all_done){
    if(!script_running(*quest,0,running))return false;
    if(!running&&!set_state(*quest,7,result))return false;
   }
  }
  if(record.state_0==7){ // UpdatePostActive 0x4812b8 tests slot 8, not 5 (source quirk).
   if(!script_running(*quest,8,running))return false;
   if(!running&&!set_state(*quest,8,result))return false;
  }
  if(record.state_0==8){ // UpdatePreCompleted 0x481254
   if(!script_running(*quest,13,running))return false;
   if(!running&&!set_state(*quest,9,result))return false;
  }
  if(record.state_0==9){ // UpdateCompleted 0x4814d8 (offline: no room-attribute sync)
   if(end->done_14){
    if(!script_running(*quest,3,running))return false;
    if(!running){
     if(!give_rewards(*quest)||!set_state(*quest,10,result))return false;
    }
   }
  }
  if(record.state_0==10){ // UpdatePostCompleted 0x4811f0
   if(!script_running(*quest,8,running))return false;
   if(!running&&!set_state(*quest,11,result))return false;
  }
  if(record.state_0==11){ // UpdatePreClosed 0x481184
   if(!script_running(*quest,12,running))return false;
   if(!running){
    record.byte_5d=1;
    if(!set_state(*quest,12,result))return false;
   }
  }
  if(record.state_0==12){ // UpdateClosed 0x481448
   if(!script_running(*quest,2,running))return false;
   if(!running){
    if(!set_state(*quest,13,result))return false;
    record.byte_5d=1;
    // Character::CheckForAllQuestsTrophy (0x3a3fe4): every Quest past Closed.
    bool all_past_closed=!quests.empty();
    for(const auto* other:quests){
     const auto* other_quest=impl_->resolve(other);
     if(!other_quest){error="Quest trophy scan found a foreign canonical Quest";return false;}
     if(other_quest->record().state_0<=12){all_past_closed=false;break;}
    }
    if(all_past_closed&&services.all_quests_trophy&&
       !services.all_quests_trophy(services.context,error)){
     if(error.empty())error="Character::CheckForAllQuestsTrophy could not unlock its trophy";
     return false;
    }
   }
  }
  result.new_state=record.state_0;
  if(result.new_state!=result.prior_state||!result.path.empty()){
   result.outcome=QuestLifecycleOutcomeV1::advanced;
   out.push_back(std::move(result));
  }
 }
 error.clear();return true;
}
bool Owner::state_summary(std::uint32_t log,std::uint32_t difficulty,std::string& out,
 std::string& error) const{
 out.clear();
 if(log>1||difficulty>2||impl_->busy||impl_->closed||!impl_->save){
  error="Quest state summary requires the open canonical Save/QEST owner";return false;
 }
 const auto& store=log?impl_->save->source_quest_log_118():impl_->save->source_quest_log_b8();
 for(const auto* ref:store.quests[difficulty]){
  auto* quest=impl_->resolve(ref);
  if(!quest){error="Quest state summary found a foreign canonical Quest";return false;}
  if(quest->record().state_0==0)continue;
  const char* name=impl_->definitions.definition_name(std::uint32_t(quest->record().fields.row_8));
  out+=(out.empty()?"":" ")+std::string(name?name:"?")+"="+std::to_string(quest->record().state_0);
 }
 error.clear();return true;
}
bool Owner::has_state_in_log(std::uint32_t log,std::uint32_t difficulty,
 std::int32_t state,bool& found,std::string& error) const{
 found=false;
 if(log>1||difficulty>2||impl_->busy||impl_->closed||!impl_->save){
  error="Quest state query requires the open canonical Save/QEST owner";return false;
 }
 const auto& store=log?impl_->save->source_quest_log_118():
                       impl_->save->source_quest_log_b8();
 if(!impl_->save->character()||store.character_5c!=impl_->save->character()){
  error="Quest state query rejected a Save/QEST Character identity mismatch";return false;
 }
 for(const auto* ref:store.quests[difficulty]){
  auto* quest=impl_->resolve(ref);
  if(!quest){error="Quest state query found a foreign canonical Quest";return false;}
  if(quest->record().state_0==state){found=true;break;}
 }
 error.clear();return true;
}
bool Owner::complete_objective(std::uintptr_t quest_identity,
 std::uint32_t objective_ordinal,void* script_context,
 KillObjectiveStartScript start_script,std::string& error){
 if(impl_->busy||impl_->closed){error="Native Quest owner unavailable for Objective::SetIsCompleted";return false;}
 auto* objective=impl_->objective_at(quest_identity,objective_ordinal);
 if(!objective){error="Objective::SetIsCompleted requires a canonical ObjectiveList child";return false;}
 if(objective->done_14){error.clear();return true;}
 objective->done_14=1;
 dh2_quest_objective definition{};
 if(!objective_definition(*objective,definition,error))return false;
 const auto script_id=definition.common[2];
 if(script_id<0){error.clear();return true;}
 if(!start_script){error="Objective::SetIsCompleted requires the current ScriptManager start provider";return false;}
 if(!start_script(script_context,script_id,error)){
  if(error.empty())error="Objective::SetIsCompleted script activation failed";
  return false;
 }
 error.clear();return true;
}
bool quest_state_lookup_v1(void* raw,std::int32_t quest_id,
 std::int32_t* state) noexcept{
 if(!raw)return false;
 auto& context=*static_cast<QuestLookupContextV1*>(raw);
 context.status=QuestLookupStatusV1::unavailable;
 if(!context.owner||!context.current_difficulty||!context.online)return false;
 return context.owner->lookup_quest_state(quest_id,*context.current_difficulty,
      *context.online!=0,state,context.status);
}
bool Owner::load_quests(Cursor& cursor,std::string& error){
 if(impl_->busy||impl_->closed){error="Native Quest owner unavailable during payload load";return false;}
 impl_->busy=true;impl_->active_cursor=&cursor;
 struct Guard{Impl& owner;~Guard(){owner.active_cursor=nullptr;owner.busy=false;}}guard{*impl_};error.clear();
 namespace saved=data::player_saved_quests_v1;
 saved::Services services{impl_.get(),[](void* raw,const saved::Request& request,saved::Reply& reply,std::string& failure)->std::int32_t{
  auto& owner=*static_cast<Impl*>(raw);auto* stream=request.stream;
  if(!owner.active_cursor||stream!=&owner.active_cursor->stream())return 1;
  using Op=saved::Operation;
  if(request.operation==Op::tell)return owner.active_cursor->tell(*stream,&reply.position)?0:1;
  if(request.operation==Op::seek)return owner.active_cursor->seek(*stream,request.offset)?0:1;
  if(request.operation==Op::read_unsigned||request.operation==Op::read_signed){
   namespace reader=data::quest_stream_read_v1;
   reader::Runtime runtime(*stream,{raw,Impl::read_stream,nullptr,nullptr});reader::Result answer;
   const auto status=request.operation==Op::read_unsigned?
    runtime.read_unsigned(static_cast<std::uint32_t*>(request.destination),&answer):
    runtime.read_signed(static_cast<std::int32_t*>(request.destination),&answer);
   if(status==reader::Status::complete)return 0;
   failure="Native QEST typed reader failed at operation "+std::to_string(unsigned(answer.last_operation));return 1;
  }
  if(request.operation==Op::quest_data){
   auto* value=owner.resolve(request.quest);scalar::Result answer;
   if(!value||value->load_quest_data(*stream,request.flag,&answer)!=scalar::Status::complete){
    failure="Native QEST Quest payload provider failed";return 1;
   }
   ++owner.result.quest_payloads;return 0;
  }
  failure="Native QEST reached unbound source assertion/logger";return 1;
 }};
 saved::Runtime runtime({impl_->save.get(),&impl_->save->source_quest_log_b8(),
  &impl_->save->source_quest_log_118(),&cursor.stream(),services});saved::Result answer;
 return runtime.load(&answer,error)==saved::Status::complete;
}
bool Owner::save_quests(
 const data::player_save_section_writers_v1::WriteServicesV1& stream,
 std::string& error){
 if(impl_->busy||impl_->closed){error="Native Quest owner unavailable during payload save";return false;}
 if(!stream.write){error="Native QEST output stream unavailable";return false;}
 impl_->busy=true;struct Guard{Impl& owner;~Guard(){owner.busy=false;}}guard{*impl_};
 error.clear();
 // PlayerSavegame::__SaveQuests selects the live Save's embedded log from
 // save_mode_ (+0x178): odd modes use +0xb8, even modes use +0x118.
 auto& store=(impl_->save->source_save_mode()&1)?
  impl_->save->source_quest_log_b8():impl_->save->source_quest_log_118();
 const auto write_word=[&](std::uint32_t value){
  const std::uint8_t bytes[]{std::uint8_t(value),std::uint8_t(value>>8),
   std::uint8_t(value>>16),std::uint8_t(value>>24)};
  if(stream.write(stream.context,{bytes,sizeof(bytes)},error))return true;
  if(error.empty())error="Native QEST source word write failed";
  return false;
 };
 const auto save_objective=[&](const data::quest_runtime_fields_v1::ActionRef* action){
  if(!action||!action->identity){error="Native QEST ActionRef is unavailable";return false;}
  const auto found=impl_->objectives.find(action->identity);
  if(found==impl_->objectives.end()||&found->second->ref.action!=action){
   error="Native QEST ActionRef is outside the same-Save Objective factory";return false;
  }
  of::Result result{};
  const auto status=impl_->objective_factory.save_data(*found->second,stream,&result,error);
  if(status==of::Status::complete)return true;
  if(error.empty())error="Native QEST Objective virtual save provider failed";
  return false;
 };
 for(std::uint32_t difficulty=0;difficulty<3;++difficulty){
  const auto& quests=store.quests[difficulty];
  if(quests.size()>std::uint32_t(INT32_MAX)){
   error="Native QEST quest vector exceeds source signed ordinal range";return false;
  }
  if(!write_word(std::uint32_t(quests.size())))return false;
  for(std::uint32_t ordinal=0;ordinal<quests.size();++ordinal){
   auto* ref=quests[ordinal];auto* quest=impl_->resolve(ref);
   if(!quest||&quest->record().ref!=ref||quest->record().difficulty_10!=std::int32_t(difficulty)){
    error="Native QEST vector contains a Quest outside its same-Save factory";return false;
   }
   if(!write_word(ordinal)||!write_word(std::uint32_t(quest->record().state_0))||
      !save_objective(quest->record().action_18)||
      !save_objective(quest->record().action_1c))return false;
   auto& list=quest->objectives();
   if(list.count_0<0||
      (list.count_0&&(!list.children_4||
       std::uint32_t(list.count_0)>list.children_4->slots.size()))){
    error="Native QEST ObjectiveList storage is outside its same-Save bounds";return false;
   }
   for(std::int32_t i=0;i<list.count_0;++i){
    const auto* ref=list.children_4->slots[std::size_t(i)];
    if(!ref||!ref->action.identity){error="Native QEST ObjectiveList contains a missing Objective";return false;}
    const auto found=impl_->objectives.find(ref->action.identity);
    if(found==impl_->objectives.end()||&found->second->ref!=ref){
     error="Native QEST ObjectiveList entry is outside the same-Save factory";return false;
    }
    of::Result result{};
    if(impl_->objective_factory.save_data(*found->second,stream,&result,error)!=of::Status::complete){
     if(error.empty())error="Native QEST ObjectiveList virtual save provider failed";
     return false;
    }
   }
  }
  if(!write_word(std::uint32_t(store.word_2c[difficulty]))||
     !write_word(std::uint32_t(store.word_38[difficulty]))||
     !write_word(std::uint32_t(store.word_44[difficulty])))return false;
 }
 error.clear();return true;
}
bool Owner::attach_current_level_receiver(std::int32_t event_type,
 std::uintptr_t receiver,std::int32_t priority,void* context,
 LevelEventReceiver invoke,std::string& error){
 if(impl_->busy||impl_->closed){error="Native Quest owner unavailable for current-Level receiver attach";return false;}
 bool attached=false;
 const auto status=impl_->current_level_events.attach(event_type,receiver,priority,
  context,invoke,attached,error);
 (void)attached; // Source Attach's result does not alter Objective::isRegistered.
 return status==LevelEventStatus::complete;
}
bool Owner::detach_current_level_receiver(std::int32_t event_type,
 std::uintptr_t receiver,std::string& error){
 if(impl_->busy||impl_->closed){error="Native Quest owner unavailable for current-Level receiver detach";return false;}
 bool detached=false;
 return impl_->current_level_events.detach(event_type,receiver,detached,error)==
  LevelEventStatus::complete;
}
bool Owner::delay_detach_current_level_receiver(std::int32_t event_type,
 std::uintptr_t receiver,std::string& error){
 if(impl_->busy||impl_->closed){error="Native Quest owner unavailable for current-Level delayed detach";return false;}
 bool scheduled=false;
 return impl_->current_level_events.delayed_detach(event_type,receiver,scheduled,error)==
  LevelEventStatus::complete;
}
bool Owner::raise_current_level_event(LevelEvent& event,std::string& error){
 if(impl_->busy||impl_->closed){error="Native Quest owner unavailable for current-Level event delivery";return false;}
 LevelEventResult result{};
 return impl_->current_level_events.raise(event,result,error)==
  LevelEventStatus::complete;
}
bool Owner::raise_character_kill_event(character_kill_quest_tail_v1::Event& source,
 std::string& error){
 LevelEvent event{};event.objective_type=source.objective_type;
 event.character=source.character_word_25;
 event.network_id=source.source_word_24;
 event.subject_id=source.source_subject;
 event.flag0=source.flag0;event.flag1=source.flag1;
 if(!raise_current_level_event(event,error))return false;
 source.source_subject=event.subject_id;source.flag0=event.flag0;source.flag1=event.flag1;
 error.clear();return true;
}
bool Owner::flush_current_level_detaches(std::string& error){
 if(impl_->busy||impl_->closed){error="Native Quest owner unavailable for current-Level detach flush";return false;}
 LevelEventResult result{};
 if(impl_->current_level_events.flush_delayed_detaches(result,error)!=LevelEventStatus::complete)return false;
 for(auto at=impl_->kill_x_enemies.begin();at!=impl_->kill_x_enemies.end();){
  if(at->second->detach_pending)at=impl_->kill_x_enemies.erase(at);else ++at;
 }
 for(auto at=impl_->gather_loot.begin();at!=impl_->gather_loot.end();){
  const auto& binding=*at->second;
  if(binding.detach_pending&&!binding.inventory_registered)at=impl_->gather_loot.erase(at);
  else ++at;
 }
 for(auto at=impl_->move_in_zone.begin();at!=impl_->move_in_zone.end();){
  if(at->second->detach_pending)at=impl_->move_in_zone.erase(at);else ++at;
 }
 for(auto at=impl_->open_game_object.begin();at!=impl_->open_game_object.end();){
  if(at->second->objective_binding.detach_pending)at=impl_->open_game_object.erase(at);else ++at;
 }
 return true;
}
bool Owner::register_gather_loot_objective(std::uintptr_t identity,
 data::FreshInventoryOwnedV4& inventory,void* script_context,
 GatherLootStartScript start_script,std::string& error){
 if(impl_->busy||impl_->closed){error="Native Quest owner unavailable for GatherLoot registration";return false;}
 const auto found=impl_->objectives.find(identity);
 if(!identity||found==impl_->objectives.end()||found->second->ref.action.identity!=identity){
  error="GatherLoot Register requires an Objective owned by this native Quest owner";return false;
 }
 auto& objective=*found->second;
 if(objective.compiled_8!=1||objective.dispatch_0!=of::Dispatch::gather_loot){
  error="GatherLoot Register requires the source Objective_GatherLoot::Compile owner to set compiled_8";return false;
 }
 if(objective.fields.character_10!=inventory.character()){
  error="GatherLoot Register requires the canonical inventory of the Objective Character";return false;
 }
 if(const auto prior=impl_->gather_loot.find(identity);prior!=impl_->gather_loot.end()){
  auto& binding=*prior->second;
  if(binding.quantity_context!=&inventory||binding.script_context!=script_context||
     binding.start_script!=start_script||binding.detach_pending){
   error="GatherLoot Objective is already registered with different caller-owned services";return false;
  }
  if(!binding.inventory_registered){
   if(!inventory.register_quest_gathering_item_id(binding.item_id,error))return false;
   binding.inventory_registered=true;
  }
  error.clear();return true;
 }
 dh2_quest_objective definition{};
 if(!objective_definition(objective,definition,error))return false;
 if(definition.common[0]!=objective.fields.type_4){error="GatherLoot Objective type projection differs from its retained pydata selector";return false;}
 // IDA Objective_GatherLoot reads source pydata+36/+40 as item ID/goal;
 // serialized definitions include the vtable word, so these are args[1]/[2].
 const auto item_id=definition.args[1],target=definition.args[2];
 const auto script_id=definition.common[2]; // Objective::SetIsCompleted reads pydata+12.
 if(item_id<0||target<=0){error="GatherLoot item ID/target quantity is outside the recovered source domain";return false;}
 if(script_id>=0&&!start_script){error="GatherLoot completion requires the source ScriptManager::StartScript provider";return false;}
 dh2_pycst_result constant{};
 if(dh2_pycst_get(&impl_->constants.view,"v2QuestObjectiveType",20,"GatherLoot",10,&constant)||!constant.found){
  error="Actual v2QuestObjectiveType.GatherLoot constant unavailable";return false;
 }
 using Gather=data::quest_gather_loot_receiver_v1::Binding;
 auto binding=std::make_unique<Gather>();
 binding->objective=&objective;binding->quantity_context=&inventory;
 binding->item_quantity=[](void* raw,std::int32_t id,bool& found,std::int32_t& quantity,std::string& failure){
  return static_cast<data::FreshInventoryOwnedV4*>(raw)->quest_gathering_item_quantity(id,found,quantity,failure);
 };
 binding->event_type=constant.value;
 binding->item_id=item_id;binding->target_quantity=target;binding->script_id=script_id;
 binding->script_context=script_context;binding->start_script=start_script;
 auto* context=binding.get();
 try{if(!impl_->gather_loot.emplace(identity,std::move(binding)).second){
   error="GatherLoot objective registration raced an existing binding";return false;
  }}catch(const std::exception&){
  error="GatherLoot receiver registration metadata allocation failed";return false;
 }
 bool attached=false;
 const auto status=impl_->current_level_events.attach(constant.value,identity,0,context,
  Gather::receive,attached,error);
 if(status!=LevelEventStatus::complete||!attached){
  impl_->gather_loot.erase(identity);
  if(error.empty())error="GatherLoot EventManager receiver was already attached outside this Owner";
  return false;
 }
 context->attached=true;
 // Source Objective_GatherLoot::Register calls the base EventReceiver first,
 // then adds its item ID to Character::ItemInventory+0x30.
 if(!inventory.register_quest_gathering_item_id(item_id,error))return false;
 context->inventory_registered=true;
 error.clear();return true;
}
bool Owner::compile_kill_x_enemies_objective(std::uintptr_t quest_identity,
 std::uint32_t objective_ordinal,
 std::int32_t current_level_id,std::int32_t loaded_match_count,
 void* script_context,KillObjectiveStartScript start_script,std::string& error){
 if(impl_->busy||impl_->closed){error="Native Quest owner unavailable for KillXEnemies Compile";return false;}
 auto* objective=impl_->objective_at(quest_identity,objective_ordinal);
 if(!objective){error="KillXEnemies Compile requires an Objective in this Owner's canonical Quest list";return false;}
 if(objective->dispatch_0!=of::Dispatch::kill_enemies||current_level_id<0||loaded_match_count<0){
  error="KillXEnemies Compile requires type 0, a live Level ID, and a nonnegative loaded-enemy count";return false;
 }
 dh2_quest_objective definition{};
 if(!objective_definition(*objective,definition,error))return false;
 if(definition.common[0]!=objective->fields.type_4){
  error="KillXEnemies Compile found a changed Objective type selector";return false;
 }
 objective->derived_24=objective->fields.py_data_c;
 const auto required_kills=definition.args[2];
 objective->derived_2c=static_cast<std::uint32_t>(required_kills);
 const auto level_filter=definition.args[1];
 if(level_filter!=-1&&level_filter!=current_level_id){
  objective->compiled_8=0;error.clear();return true;
 }
 // The source population query is only an availability gate for KillXEnemies;
 // its target is the independent row field at definition +0x28.
 if(loaded_match_count==0||required_kills<=0){
  objective->compiled_8=0;error.clear();return true;
 }
 objective->compiled_8=1;
 if(required_kills<=static_cast<std::int32_t>(objective->quantity_20)&&!objective->done_14){
  objective->done_14=1;
  if(definition.common[2]>=0&&
     (!start_script||!start_script(script_context,definition.common[2],error))){
   if(error.empty())error="KillXEnemies Compile completion needs the source script-base provider";
   return false;
  }
 }
 error.clear();return true;
}
bool Owner::compile_kill_x_enemies_objective_from_level(
 std::uintptr_t quest_identity,std::uint32_t objective_ordinal,
 const KillEnemiesPopulationServicesV1& population,
 void* script_context,KillObjectiveStartScript start_script,std::string& error){
 if(impl_->busy||impl_->closed){error="Native Quest owner unavailable for live KillXEnemies Compile";return false;}
 if(!population.read_current_level_population||!population.expected_level_identity){
  error="Live KillXEnemies Compile requires the current-Level population provider and identity";return false;
 }
 auto* objective=impl_->objective_at(quest_identity,objective_ordinal);
 if(!objective||objective->dispatch_0!=of::Dispatch::kill_enemies){
  error="Live KillXEnemies Compile requires its canonical KillXEnemies Objective";return false;
 }
 dh2_quest_objective definition{};
 if(!objective_definition(*objective,definition,error))return false;
 if(definition.common[0]!=objective->fields.type_4||definition.args[0]<-32768||
    definition.args[0]>32767){
  error="Live KillXEnemies Compile found an invalid source property selector";return false;
 }
 const auto expected_owner=reinterpret_cast<std::uintptr_t>(this);
 ClearEnemiesPopulationSnapshotV1 snapshot{};
 try{
  if(!population.read_current_level_population(population.context,expected_owner,
       population.expected_level_identity,
       definition.args[0],snapshot,error)){
   if(error.empty())error="Current-Level KillXEnemies population query failed";
   return false;
  }
 }catch(const std::exception& ex){error=ex.what();return false;}
 catch(...){error="Current-Level KillXEnemies population provider threw";return false;}
 if(snapshot.event_owner_identity!=expected_owner||
    snapshot.level_identity!=population.expected_level_identity||snapshot.level_id<0||
    snapshot.loaded_match_count<0){
  error="Live KillXEnemies Compile rejected a stale Level owner or invalid population";return false;
 }
 return compile_kill_x_enemies_objective(quest_identity,objective_ordinal,
       snapshot.level_id,snapshot.loaded_match_count,script_context,start_script,error);
}
bool Owner::compile_kill_x_enemies_objective_list_v1(
 std::uintptr_t quest_identity,const KillEnemiesPopulationServicesV1& population,
 void* script_context,KillObjectiveStartScript start_script,std::string& error){
 CombatObjectivePopulationServicesV1 services{};
 services.kill_enemies=population;
 return compile_combat_objective_list_v1(quest_identity,services,script_context,
                                         start_script,error);
}
bool Owner::compile_combat_objective_list_v1(
 std::uintptr_t quest_identity,const CombatObjectivePopulationServicesV1& population,
 void* script_context,KillObjectiveStartScript start_script,std::string& error){
 if(impl_->busy||impl_->closed){error="Native Quest owner unavailable for ObjectiveList::Compile";return false;}
 auto* quest=impl_->resolve_quest_identity(quest_identity);
 if(!quest){
  error="ObjectiveList::Compile requires this canonical Quest";return false;
 }
 auto& list=quest->objectives();
 if(list.count_0<0||(list.count_0>0&&(!list.children_4||
    std::uint64_t(list.count_0)>list.children_4->slots.size()))){
  error="ObjectiveList::Compile saw invalid canonical list storage";return false;
 }
 struct Context {
  Owner* owner=nullptr;instance::Instance* quest=nullptr;
  std::uintptr_t quest_identity=0;
  const CombatObjectivePopulationServicesV1* population=nullptr;
  void* script_context=nullptr;KillObjectiveStartScript start_script=nullptr;
  std::string* error=nullptr;
 } context{this,quest,quest_identity,&population,script_context,start_script,&error};
 const qc::Services services{&context,
  [](void* raw,scalar::ActionRef* action)->of::Record*{
   auto& c=*static_cast<Context*>(raw);if(!action)return nullptr;
   auto& children=c.quest->objectives();if(children.count_0<0||!children.children_4)return nullptr;
   for(std::uint32_t ordinal=0;ordinal<std::uint32_t(children.count_0)&&
       ordinal<children.children_4->slots.size();++ordinal){
    auto* ref=children.children_4->slots[ordinal];
    if(ref&&&ref->action==action)return c.owner->impl_->resolve_objective(ref);
   }
   return nullptr;
  },nullptr,
  [](void* raw,of::Record& objective,const qc::MemberCall& call)->std::int32_t{
   auto& c=*static_cast<Context*>(raw);auto& children=c.quest->objectives();
   std::uint32_t ordinal=UINT32_MAX;
   if(children.count_0<0||!children.children_4)return 1;
   for(std::uint32_t index=0;index<std::uint32_t(children.count_0)&&
       index<children.children_4->slots.size();++index){
    auto* ref=children.children_4->slots[index];
    if(ref&&ref->action.identity==objective.ref.action.identity&&
       c.owner->impl_->resolve_objective(ref)==&objective){ordinal=index;break;}
   }
   if(ordinal==UINT32_MAX){*c.error="Objective Compile callback lost the canonical Quest child";return 1;}
   bool ok=false;
   if(call.is_virtual&&call.adjustment==0&&call.function==8){
    switch(objective.dispatch_0){
    case of::Dispatch::kill_enemies:
     ok=c.owner->compile_kill_x_enemies_objective_from_level(
       c.quest_identity,ordinal,c.population->kill_enemies,c.script_context,
       c.start_script,*c.error);break;
    case of::Dispatch::clear_enemies:
     ok=c.owner->compile_clear_enemies_objective_from_level(
       c.quest_identity,ordinal,c.population->clear_enemies,c.script_context,
       c.start_script,*c.error);break;
    case of::Dispatch::kill_enemy_template:
     ok=c.owner->compile_kill_enemy_template_objective_from_level(
       c.quest_identity,ordinal,c.population->kill_enemy_template,c.script_context,
       c.start_script,*c.error);break;
    case of::Dispatch::clear_enemy_template:
     ok=c.owner->compile_clear_enemy_template_objective_from_level(
       c.quest_identity,ordinal,c.population->clear_enemy_template,c.script_context,
       c.start_script,*c.error);break;
    default:
     *c.error="ObjectiveList::Compile has no source provider for this Objective dispatch";return 1;
    }
   }else if(call.is_virtual&&call.adjustment==0&&call.function==0x18){
    if(!objective.compiled_8)return 0;
    switch(objective.dispatch_0){
    case of::Dispatch::kill_enemies:
     ok=c.owner->register_kill_x_enemies_objective(
       c.quest_identity,ordinal,c.script_context,c.start_script,*c.error);break;
    case of::Dispatch::clear_enemies:
     ok=c.owner->register_clear_enemies_objective(
       c.quest_identity,ordinal,c.script_context,c.start_script,*c.error);break;
    case of::Dispatch::kill_enemy_template:
     ok=c.owner->register_kill_enemy_template_objective(
       c.quest_identity,ordinal,c.script_context,c.start_script,*c.error);break;
    case of::Dispatch::clear_enemy_template:
     ok=c.owner->register_clear_enemy_template_objective(
       c.quest_identity,ordinal,c.script_context,c.start_script,*c.error);break;
    default:
     *c.error="ObjectiveList::Register has no source provider for this Objective dispatch";return 1;
    }
   }else{
    *c.error="ObjectiveList reached an unsupported virtual member call";return 1;
   }
   return ok?0:1;
  }};
 qc::Runtime runtime(*quest,impl_->definitions,services);qc::Result result{};
 auto status=runtime.invalidate_objectives(&result);
 if(status==qc::Status::complete)status=runtime.compile_objectives(&result);
 if(status==qc::Status::complete&&quest->record().state_0==6)
  status=runtime.register_objectives(&result);
 if(status!=qc::Status::complete){
  if(error.empty())error="KillXEnemies ObjectiveList source adapter failed at operation "+
      std::to_string(static_cast<unsigned>(result.last_operation));
  return false;
 }
 error.clear();return true;
}
bool Owner::dispatch_objective_compile_member_v1(
 std::uintptr_t quest_identity,qc::Objective& objective,
 const qc::MemberCall& call,const ObjectiveCompileDispatchServicesV1& services,
 std::string& error){
 if(impl_->busy||impl_->closed){error="Native Quest owner unavailable for Objective member dispatch";return false;}
 auto* quest=impl_->resolve_quest_identity(quest_identity);
 if(!quest){error="Objective member dispatch requires this Owner's canonical Quest";return false;}
 auto& list=quest->objectives();
 if(list.count_0<0||(list.count_0>0&&(!list.children_4||
    std::uint64_t(std::uint32_t(list.count_0))>list.children_4->slots.size()))){
  error="Objective member dispatch saw invalid canonical ObjectiveList storage";return false;
 }
 std::uint32_t ordinal=UINT32_MAX;
 // The accept/end ActionRefs are not ObjectiveList children; they keep their
 // own sentinel ordinals so every binding below serves them unchanged.
 if(&objective==impl_->resolve_compile_objective(quest->record().action_18))
  ordinal=Impl::accept_action_ordinal;
 else if(&objective==impl_->resolve_compile_objective(quest->record().action_1c))
  ordinal=Impl::end_action_ordinal;
 else for(std::uint32_t i=0;i<std::uint32_t(list.count_0);++i){
  auto* candidate=impl_->resolve_objective(list.children_4->slots[i]);
  if(candidate==&objective){ordinal=i;break;}
 }
 if(ordinal==UINT32_MAX){error="Objective member dispatch lost the canonical Quest child identity";return false;}
 const auto& ref=objective.ref;
 if(!ref.action.identity||ref.fields!=&objective.fields||
    ref.action.character_10!=&objective.fields.character_10){
  error="Objective member dispatch found a changed factory projection";return false;
 }
 const auto clear_error=[&](){error.clear();return true;};
 if(call.is_virtual&&call.adjustment==0&&call.function==8){
  switch(objective.dispatch_0){
  case of::Dispatch::base:case of::Dispatch::event_receiver:
   return clear_error(); // Source Objective base Compile is empty.
  case of::Dispatch::automatic:{
   dh2_quest_objective definition{};
   if(!objective_definition(objective,definition,error))return false;
   if(definition.common[0]!=objective.fields.type_4){
    error="Automatic Compile found a changed retained source selector";return false;
   }
   // IDA 0x47bd80 stores compiled_8 before Objective::SetIsCompleted.
   objective.compiled_8=1;
   return complete_objective(quest_identity,ordinal,services.script_context,
                            services.start_script,error);
  }
  case of::Dispatch::kill_enemies:
   return compile_kill_x_enemies_objective_from_level(quest_identity,ordinal,
       services.population.kill_enemies,services.script_context,
       services.start_script,error);
  case of::Dispatch::clear_enemies:
   return compile_clear_enemies_objective_from_level(quest_identity,ordinal,
       services.population.clear_enemies,services.script_context,
       services.start_script,error);
  case of::Dispatch::kill_enemy_template:
   return compile_kill_enemy_template_objective_from_level(quest_identity,ordinal,
       services.population.kill_enemy_template,services.script_context,
       services.start_script,error);
  case of::Dispatch::clear_enemy_template:
   return compile_clear_enemy_template_objective_from_level(quest_identity,ordinal,
       services.population.clear_enemy_template,services.script_context,
       services.start_script,error);
  case of::Dispatch::gather_loot:{
   if(!services.inventory||services.current_level_id<0||
      services.inventory->character()!=objective.fields.character_10){
    error="GatherLoot Compile requires the same Character's current V4 inventory and Level ID";return false;
   }
   dh2_quest_objective definition{};
   if(!objective_definition(objective,definition,error))return false;
   if(definition.common[0]!=objective.fields.type_4){error="GatherLoot Compile found a changed source selector";return false;}
   const auto level_filter=definition.args[0],item_id=definition.args[1],target=definition.args[2];
   if(item_id<=0||target<=0||std::size_t(item_id)>=services.inventory->table().rows.size()){
    error="GatherLoot Compile reached invalid source item/target fields";return false;
   }
   if(level_filter!=-1&&level_filter!=services.current_level_id)return clear_error();
   bool found=false;std::int32_t quantity=0;
   if(!services.inventory->quest_gathering_item_quantity(item_id,found,quantity,error))return false;
   return data::quest_gather_loot_receiver_v1::compile(objective,true,item_id,target,
       definition.common[2],found,quantity,services.script_context,
       services.start_script,error);
  }
  case of::Dispatch::talk_to_npc:{
   dh2_quest_objective definition{};
   if(!objective_definition(objective,definition,error))return false;
   if(definition.common[0]!=objective.fields.type_4){error="TalkToNPC Compile found a changed source selector";return false;}
   // Source v2QuestTalkToNPC::Compile reads pydata+32 (args[0]) as target
   // SafeGetCharPropsId and pydata+40 (args[2]) as required interaction count.
   const auto target=definition.args[0],required=definition.args[2];
   objective.derived_24=target;
   if(target!=-1){
    objective.compiled_8=1;
    if(static_cast<std::int32_t>(objective.quantity_20)>=required&&!objective.done_14){
     if(!complete_objective(quest_identity,ordinal,services.script_context,
                            services.start_script,error))return false;
     if(impl_->talk_to_npc.find(objective.ref.action.identity)!=impl_->talk_to_npc.end()){
      if(!unregister_talk_to_npc_objective(quest_identity,ordinal,error))return false;
     }else if(definition.common[0]==5){
      if(!services.talk_marker){error="TalkToNPC Compile completion needs the current-Level target marker provider";return false;}
      if(!services.talk_marker(services.talk_marker_context,
          objective.ref.action.identity,&objective.derived_28,target,
          TalkObjectiveMarkerOperationV1::unregister_target,0,0,error))return false;
     }
    }
   }
   return clear_error();
  }
  case of::Dispatch::move_in_zone:{
   dh2_quest_objective definition{};std::string target_name;
   if(!objective_definition(objective,definition,error)||
      !objective_string(objective,definition,1,target_name,error))return false;
   if(definition.common[0]!=objective.fields.type_4){error="MoveInZone Compile found a changed source selector";return false;}
   if(!services.object_manager){error="MoveInZone Compile requires the current Level's canonical ObjectManager";return false;}
   // Source MoveInZone::Compile calls GetObjectByName(name,-1,false). The
   // source MeetCondition(Object) body is a literal true at 0x38ab60.
   auto* target=services.object_manager->find_by_name(target_name,-1);
   objective.derived_24=std::int32_t(-1);objective.derived_28=0;
   if(!target){objective.compiled_8=0;return clear_error();}
   if(!target->identity){error="MoveInZone ObjectManager returned a name row without its canonical object identity";return false;}
   bool loaded=true;
   const auto target_props_id=definition.args[1];
   if(target_props_id!=-1){
    const auto& population=services.population.kill_enemies;
    if(!population.read_current_level_population||!population.expected_level_identity){
     error="MoveInZone Compile requires current-Level TestCharPropId population services";return false;
    }
    ClearEnemiesPopulationSnapshotV1 snapshot{};
    if(!population.read_current_level_population(population.context,
        reinterpret_cast<std::uintptr_t>(this),population.expected_level_identity,
        target_props_id,snapshot,error)){
     if(error.empty())error="MoveInZone current-Level TestCharPropId query failed";
     return false;
    }
    if(snapshot.event_owner_identity!=reinterpret_cast<std::uintptr_t>(this)||
       snapshot.level_identity!=population.expected_level_identity||snapshot.level_id<0||
       snapshot.loaded_match_count<0){
     error="MoveInZone population query returned a stale Level or invalid count";return false;
    }
    loaded=snapshot.loaded_match_count>0;
   }
   if(!loaded){objective.compiled_8=0;return clear_error();}
   objective.derived_24=target->source_handle;objective.derived_28=target->identity;
   objective.compiled_8=1;
   if(definition.common[2]>=0&&!services.start_script){
    error="MoveInZone Compile requires its source completion-script provider";return false;
   }
   const auto identity=objective.ref.action.identity;
   auto existing=impl_->move_in_zone.find(identity);
   if(existing!=impl_->move_in_zone.end()){
    auto& binding=*existing->second;
    if(binding.objective!=&objective||binding.quest_identity!=quest_identity||
       binding.ordinal!=ordinal||binding.target_identity!=target->identity||
       binding.target_props_id!=target_props_id||binding.script_context!=services.script_context||
       binding.start_script!=services.start_script||
       binding.character.context!=services.move_in_zone_character.context||
       binding.character.safe_get_char_props_id!=services.move_in_zone_character.safe_get_char_props_id||
       binding.character.is_player!=services.move_in_zone_character.is_player||
       binding.detach_pending){error="MoveInZone receiver already has different live services";return false;}
    return clear_error();
   }
   std::int32_t event_type=-1;
   if(impl_->constant("v2QuestObjectiveType","MoveInZone",&event_type)||event_type<0){
    error="Actual v2QuestObjectiveType.MoveInZone constant unavailable";return false;
   }
   auto binding=std::make_unique<Impl::MoveInZoneBinding>();
   binding->owner=this;binding->objective=&objective;binding->quest_identity=quest_identity;
   binding->ordinal=ordinal;binding->event_type=event_type;
   binding->target_identity=target->identity;binding->target_props_id=target_props_id;
   binding->script_id=definition.common[2];binding->script_context=services.script_context;
   binding->start_script=services.start_script;binding->character=services.move_in_zone_character;
   try{if(!impl_->move_in_zone.emplace(identity,std::move(binding)).second){
    error="MoveInZone receiver metadata allocation raced another binding";return false;
   }}catch(const std::exception&){error="MoveInZone receiver metadata allocation failed";return false;}
   return clear_error();
  }
  case of::Dispatch::open_game_object:{
   dh2_quest_objective definition{};
   if(!objective_definition(objective,definition,error))return false;
   if(definition.common[0]!=objective.fields.type_4){error="OpenGameObject Compile found a changed source selector";return false;}
   if(definition.common[2]>=0&&!services.start_script){error="OpenGameObject Compile requires its source completion-script provider";return false;}
   std::int32_t event_type=-1;
   if(impl_->constant("v2QuestObjectiveType","OpenGameObject",&event_type)||event_type<0){
    error="Actual v2QuestObjectiveType.OpenGameObject constant unavailable";return false;
   }
   const auto identity=objective.ref.action.identity;
   auto existing=impl_->open_game_object.find(identity);
   Impl::OpenGameObjectBinding* binding=nullptr;
   if(existing==impl_->open_game_object.end()){
    auto owned=std::make_unique<Impl::OpenGameObjectBinding>();
    owned->owner=this;owned->objective=&objective;owned->quest_identity=quest_identity;
    owned->ordinal=ordinal;owned->script_context=services.script_context;
    owned->start_script=services.start_script;binding=owned.get();
    try{if(!impl_->open_game_object.emplace(identity,std::move(owned)).second){
     error="OpenGameObject receiver metadata allocation raced another binding";return false;
    }}catch(const std::exception&){error="OpenGameObject receiver metadata allocation failed";return false;}
   }else{
    binding=existing->second.get();
    if(binding->objective!=&objective||binding->quest_identity!=quest_identity||
       binding->ordinal!=ordinal||binding->script_context!=services.script_context||
       binding->start_script!=services.start_script||binding->objective_binding.detach_pending){
     error="OpenGameObject receiver already has different live services";return false;
    }
   }
   og::Services open_services{};open_services.context=binding;
   open_services.complete=Impl::OpenGameObjectBinding::complete;
   open_services.events=&impl_->current_level_events;
   if(!og::compile(objective,definition,event_type,open_services,
                   binding->objective_binding,error))return false;
   error.clear();return true;
  }
  default:
   error="Objective Compile dispatch has no source kernel for this canonical kind";return false;
  }
 }
 if(call.is_virtual&&call.adjustment==0&&call.function==0x18){
  if(!objective.compiled_8)return clear_error();
  switch(objective.dispatch_0){
  case of::Dispatch::kill_enemies:
   return register_kill_x_enemies_objective(quest_identity,ordinal,
       services.script_context,services.start_script,error);
  case of::Dispatch::clear_enemies:
   return register_clear_enemies_objective(quest_identity,ordinal,
       services.script_context,services.start_script,error);
  case of::Dispatch::kill_enemy_template:
   return register_kill_enemy_template_objective(quest_identity,ordinal,
       services.script_context,services.start_script,error);
  case of::Dispatch::clear_enemy_template:
   return register_clear_enemy_template_objective(quest_identity,ordinal,
       services.script_context,services.start_script,error);
  case of::Dispatch::gather_loot:
   if(!services.inventory){error="GatherLoot Register requires the current Character V4 inventory";return false;}
   return register_gather_loot_objective(objective.ref.action.identity,
       *services.inventory,services.script_context,services.start_script,error);
  case of::Dispatch::talk_to_npc:{
   dh2_quest_objective definition{};
   if(!objective_definition(objective,definition,error))return false;
   if(definition.common[0]!=objective.fields.type_4||objective.derived_24.index()!=0){
    error="TalkToNPC Register requires its compiled source target projection";return false;
   }
   const bool marks_target=definition.common[0]==5;
   if(marks_target&&!services.talk_marker){error="TalkToNPC Register requires the current-Level CharacterList marker provider";return false;}
   const auto identity=objective.ref.action.identity;
   const auto existing=impl_->talk_to_npc.find(identity);
   if(existing!=impl_->talk_to_npc.end()){
    const auto& binding=*existing->second;
    if(binding.objective!=&objective||binding.quest_identity!=quest_identity||
       binding.ordinal!=ordinal||binding.target_props_id!=std::get<std::int32_t>(objective.derived_24)||
       binding.marker_context!=services.talk_marker_context||binding.marker!=services.talk_marker||
       binding.script_context!=services.script_context||binding.start_script!=services.start_script||
       binding.detach_pending){error="TalkToNPC receiver already has different live services";return false;}
    error.clear();return true;
   }
   dh2_pycst_result event_constant{};
   if(dh2_pycst_get(&impl_->constants.view,"v2QuestObjectiveType",20,
       "TalkToNPC",9,&event_constant)||!event_constant.found){
    error="Actual v2QuestObjectiveType.TalkToNPC constant unavailable";return false;
   }
   if(definition.common[2]>=0&&!services.start_script){error="TalkToNPC Register requires its source completion-script provider";return false;}
   auto binding=std::make_unique<Impl::TalkToNpcBinding>();
   binding->owner=this;binding->objective=&objective;binding->quest_identity=quest_identity;
   binding->ordinal=ordinal;binding->event_type=event_constant.value;
   binding->target_props_id=std::get<std::int32_t>(objective.derived_24);
   binding->target_count=definition.args[2];binding->script_id=definition.common[2];
   binding->script_context=services.script_context;binding->start_script=services.start_script;
   binding->marker_context=services.talk_marker_context;binding->marker=services.talk_marker;
   auto* context=binding.get();
   try{if(!impl_->talk_to_npc.emplace(identity,std::move(binding)).second){
    error="TalkToNPC receiver registration raced another binding";return false;
   }}catch(const std::exception&){error="TalkToNPC receiver metadata allocation failed";return false;}
   bool attached=false;
   const auto status=impl_->current_level_events.attach(event_constant.value,identity,0,
       context,Impl::TalkToNpcBinding::receive,attached,error);
   if(status!=LevelEventStatus::complete||!attached){
    impl_->talk_to_npc.erase(identity);
    if(error.empty())error="TalkToNPC receiver was attached outside this Owner";
    return false;
   }
   context->attached=true;
   // Objective_TalkToNPC::Register attaches the base receiver before setting
   // matching Character+0x2fa. Keep that source prefix if the marker provider fails.
   if(marks_target){
    if(!services.talk_marker(services.talk_marker_context,
        objective.ref.action.identity,&objective.derived_28,context->target_props_id,
        TalkObjectiveMarkerOperationV1::register_target,0,0,error))return false;
    context->target_marked=true;
   }
   error.clear();return true;
  }
  case of::Dispatch::move_in_zone:{
   const auto identity=objective.ref.action.identity;
   const auto found=impl_->move_in_zone.find(identity);
   if(found==impl_->move_in_zone.end()){error="MoveInZone Register requires its source Compile binding";return false;}
   auto& binding=*found->second;
   if(binding.objective!=&objective||binding.quest_identity!=quest_identity||
      binding.ordinal!=ordinal||binding.target_identity!=objective.derived_28||
      objective.derived_24.index()!=0||
      std::get<std::int32_t>(objective.derived_24)<0||binding.detach_pending){
    error="MoveInZone Register requires its unchanged compiled target binding";return false;
   }
   if(binding.script_id>=0&&!binding.start_script){error="MoveInZone Register requires its completion-script provider";return false;}
   if(binding.attached){error.clear();return true;}
   auto* context=&binding;bool attached=false;
   const auto status=impl_->current_level_events.attach(binding.event_type,identity,0,
       context,Impl::MoveInZoneBinding::receive,attached,error);
   if(status!=LevelEventStatus::complete||!attached){
    if(error.empty())error="MoveInZone EventManager::Attach did not add its receiver";
    return false;
   }
   binding.attached=true;error.clear();return true;
  }
  case of::Dispatch::open_game_object:{
   const auto found=impl_->open_game_object.find(objective.ref.action.identity);
   if(found==impl_->open_game_object.end()){
    error="OpenGameObject Register requires its source Compile binding";return false;
   }
   return og::register_objective(objective,found->second->objective_binding,error);
  }
  case of::Dispatch::base:case of::Dispatch::event_receiver:
   return clear_error(); // no registration override on the base Objective.
  case of::Dispatch::automatic:
   return clear_error(); // Objective_Automatic::Register (0x47a1dc) is a bare return.
  default:
   error="Objective Register dispatch has no source owner for this canonical kind";return false;
  }
 }
 if(call.is_virtual&&call.adjustment==0&&call.function==0x10){
  if(objective.dispatch_0==of::Dispatch::open_game_object){
   if(!objective.compiled_8)return clear_error();
   const auto found=impl_->open_game_object.find(objective.ref.action.identity);
   if(found==impl_->open_game_object.end()){
    error="OpenGameObject InstallObjectiveMarker requires its source Compile binding";return false;
   }
   return og::marker(objective,og::MarkerOperation::install,error);
  }
  if(objective.dispatch_0!=of::Dispatch::talk_to_npc)return clear_error();
  if(!objective.compiled_8)return clear_error();
  if(!services.talk_marker){error="TalkToNPC InstallObjectiveMarker requires the current-Level CharacterList/VisualFX provider";return false;}
  dh2_quest_objective definition{};
  if(!objective_definition(objective,definition,error))return false;
  if(definition.common[0]!=5)return clear_error();
  const auto target=std::get_if<std::int32_t>(&objective.derived_24);
  if(!target){error="TalkToNPC marker target projection is unavailable";return false;}
  return services.talk_marker(services.talk_marker_context,
       objective.ref.action.identity,&objective.derived_28,*target,
       TalkObjectiveMarkerOperationV1::install,call.marker_priority,
       call.quest_state,error);
 }
 if(call.is_virtual&&call.adjustment==0&&call.function==0x14){
  if(objective.dispatch_0==of::Dispatch::open_game_object){
   const auto found=impl_->open_game_object.find(objective.ref.action.identity);
   if(found==impl_->open_game_object.end())return clear_error();
   return og::marker(objective,og::MarkerOperation::remove,error);
  }
  if(objective.dispatch_0==of::Dispatch::talk_to_npc){
   if(!services.talk_marker){error="TalkToNPC RemoveObjectiveMarker requires the current-Level CharacterList/VisualFX provider";return false;}
   dh2_quest_objective definition{};
   if(!objective_definition(objective,definition,error))return false;
   if(definition.common[0]!=5)return clear_error();
   const auto target=std::get_if<std::int32_t>(&objective.derived_24);
   if(!target){error="TalkToNPC marker target projection is unavailable";return false;}
   return services.talk_marker(services.talk_marker_context,
       objective.ref.action.identity,&objective.derived_28,*target,
       TalkObjectiveMarkerOperationV1::remove,call.marker_priority,
       call.quest_state,error);
  }
  return clear_error();
 }
 if(call.is_virtual&&call.adjustment==0&&call.function==0x1c){
  switch(objective.dispatch_0){
  case of::Dispatch::base:case of::Dispatch::event_receiver:
   return clear_error();
  case of::Dispatch::automatic:
   return clear_error(); // Objective_Automatic::Unregister (0x47a1e0) is a bare return.
  case of::Dispatch::kill_enemies:
   return unregister_kill_x_enemies_objective(quest_identity,ordinal,error);
  case of::Dispatch::clear_enemies:
   return unregister_clear_enemies_objective(quest_identity,ordinal,error);
  case of::Dispatch::kill_enemy_template:
   return unregister_kill_enemy_template_objective(quest_identity,ordinal,error);
  case of::Dispatch::clear_enemy_template:
   return unregister_clear_enemy_template_objective(quest_identity,ordinal,error);
  case of::Dispatch::gather_loot:
   return unregister_gather_loot_objective(objective.ref.action.identity,error);
  case of::Dispatch::talk_to_npc:
   return unregister_talk_to_npc_objective(quest_identity,ordinal,error);
  case of::Dispatch::move_in_zone:
   return unregister_move_in_zone_objective(quest_identity,ordinal,error);
  case of::Dispatch::open_game_object:
   return unregister_open_game_object_objective(quest_identity,ordinal,error);
  default:
   error="Objective Unregister dispatch has no source owner for this canonical kind";return false;
  }
 }
 error="Objective Compile adapter reached an unsupported virtual/member call";return false;
}
bool Owner::dispatch_game_event_objective_member_v1(
 std::uintptr_t level_identity,std::uintptr_t manager_identity,
 data::game_event_manager_owner_v1::Row& row,const o::MemberCall& call,
 const ObjectiveCompileDispatchServicesV1& services,std::string& error){
 if(!impl_||impl_->closed||impl_->busy||!impl_->save||!level_identity||!manager_identity||
    !row.identity||!row.definition||!call.objective){error="GameEvent Objective dispatch requires a live Save/Level/manager/row/Objective";return false;}
 auto* objective=impl_->resolve_objective(call.objective);
 if(!objective||objective->ref.fields!=&objective->fields||
    objective->ref.action.character_10!=&objective->fields.character_10||
    objective->fields.py_data_c.event!=row.definition||
    objective->fields.py_data_c.index>=row.definition->triggers.size()){
  error="GameEvent Objective is not the trigger record retained by this row and Save's factory";return false;
 }
 dh2_quest_objective definition{};
 if(!objective_definition(*objective,definition,error))return false;
 const auto& trigger=row.definition->triggers[objective->fields.py_data_c.index];
 if(trigger.type!=objective->fields.type_4||trigger.type!=definition.common[0]){error="GameEvent Objective trigger selector changed";return false;}
 const auto type=trigger.type;
 const bool reset=call.operation==o::MemberOperation::reset_data&&call.virtual_call&&call.function==0x30&&call.encoded_adjustment==1;
 const bool invalidate=call.operation==o::MemberOperation::invalidate_compile&&!call.virtual_call&&call.function==0x479f70&&call.encoded_adjustment==0;
 const bool compile=call.operation==o::MemberOperation::compile&&call.virtual_call&&call.function==8&&call.encoded_adjustment==1;
 const bool register_call=call.operation==o::MemberOperation::register_objectives&&call.virtual_call&&call.function==0x18&&call.encoded_adjustment==1;
 if(reset){objective->done_14=0;if(objective->dispatch_0==of::Dispatch::kill_enemies)objective->quantity_20=0;error.clear();return true;}
 if(invalidate){objective->done_14=0;objective->compiled_8=0;error.clear();return true;}
 if(compile){
  if(type==0){
   if(objective->dispatch_0!=of::Dispatch::kill_enemies){error="GameEvent kind 0 has an unexpected Objective factory kind";return false;}
   const auto& population=services.population.kill_enemies;
   if(!population.read_current_level_population||population.expected_level_identity!=level_identity){error="GameEvent KillXEnemies requires same-Level population services";return false;}
   ClearEnemiesPopulationSnapshotV1 snapshot{};
   const auto expected_event_owner=reinterpret_cast<std::uintptr_t>(this);
   if(!population.read_current_level_population(population.context,expected_event_owner,level_identity,definition.args[0],snapshot,error))return false;
   if(snapshot.event_owner_identity!=expected_event_owner||snapshot.level_identity!=level_identity||snapshot.level_id<0||snapshot.loaded_match_count<0){error="GameEvent KillXEnemies population rejected stale Level/Quest-owner identity";return false;}
   objective->derived_24=objective->fields.py_data_c;objective->derived_2c=static_cast<std::uint32_t>(definition.args[2]);
   objective->compiled_8=(definition.args[1]!=-1&&definition.args[1]!=snapshot.level_id)||snapshot.loaded_match_count==0||definition.args[2]<=0?0:1;
   if(objective->compiled_8&&definition.args[2]<=static_cast<std::int32_t>(objective->quantity_20)&&!objective->done_14){
    objective->done_14=1;if(definition.common[2]>=0&&(!services.start_script||!services.start_script(services.script_context,definition.common[2],error))){if(error.empty())error="GameEvent KillXEnemies requires its ScriptManager provider";return false;}
   }
   if(definition.common[2]>=0&&!services.start_script){error="GameEvent KillXEnemies requires its ScriptManager provider";return false;}
   error.clear();return true;
  }
  if(type==4){
   if(objective->dispatch_0!=of::Dispatch::move_in_zone||!services.object_manager){error="GameEvent MoveInZone requires its native Objective kind and current-Level ObjectManager";return false;}
   std::string name;if(!objective_string(*objective,definition,1,name,error))return false;
   auto* target=services.object_manager->find_by_name(name,-1);
   objective->derived_24=std::int32_t(-1);objective->derived_28=0;objective->compiled_8=0;
   if(!target){error.clear();return true;}if(!target->identity){error="GameEvent MoveInZone target has no canonical object identity";return false;}
   if(definition.args[1]!=-1){const auto& population=services.population.kill_enemies;if(!population.read_current_level_population||population.expected_level_identity!=level_identity){error="GameEvent MoveInZone requires same-Level population services";return false;}ClearEnemiesPopulationSnapshotV1 snapshot{};const auto expected_event_owner=reinterpret_cast<std::uintptr_t>(this);if(!population.read_current_level_population(population.context,expected_event_owner,level_identity,definition.args[1],snapshot,error))return false;if(snapshot.event_owner_identity!=expected_event_owner||snapshot.level_identity!=level_identity||snapshot.level_id<0||snapshot.loaded_match_count<0){error="GameEvent MoveInZone rejected stale Level/Quest-owner population data";return false;}if(snapshot.loaded_match_count==0){error.clear();return true;}}
   objective->derived_24=target->source_handle;objective->derived_28=target->identity;objective->compiled_8=1;
   if(definition.common[2]>=0&&!services.start_script){error="GameEvent MoveInZone requires its ScriptManager provider";return false;}
   const auto identity=objective->ref.action.identity;auto prior=impl_->move_in_zone.find(identity);
   if(prior==impl_->move_in_zone.end()){
    std::int32_t event_type=-1;if(impl_->constant("v2QuestObjectiveType","MoveInZone",&event_type)||event_type<0){error="Actual MoveInZone event type constant unavailable";return false;}
    auto binding=std::make_unique<Impl::MoveInZoneBinding>();binding->owner=this;binding->objective=objective;binding->level_identity=level_identity;binding->game_event_manager_identity=manager_identity;binding->event_type=event_type;binding->target_identity=target->identity;binding->target_props_id=definition.args[1];binding->script_id=definition.common[2];binding->script_context=services.script_context;binding->start_script=services.start_script;binding->character=services.move_in_zone_character;binding->game_event_trigger=true;
    if(!impl_->move_in_zone.emplace(identity,std::move(binding)).second){error="GameEvent MoveInZone receiver metadata collision";return false;}
   }else if(prior->second->objective!=objective||prior->second->level_identity!=level_identity||prior->second->game_event_manager_identity!=manager_identity||prior->second->target_identity!=target->identity||prior->second->target_props_id!=definition.args[1]||prior->second->script_context!=services.script_context||prior->second->start_script!=services.start_script){error="GameEvent MoveInZone receiver services changed";return false;}
   error.clear();return true;
  }
  if(type==5){
   if(objective->dispatch_0!=of::Dispatch::talk_to_npc){error="GameEvent kind 5 has an unexpected Objective factory kind";return false;}
   objective->derived_24=definition.args[0];objective->compiled_8=definition.args[0]!=-1;
   if(objective->compiled_8&&static_cast<std::int32_t>(objective->quantity_20)>=definition.args[2]&&!objective->done_14){objective->done_14=1;if(definition.common[2]>=0&&(!services.start_script||!services.start_script(services.script_context,definition.common[2],error))){if(error.empty())error="GameEvent TalkToNPC requires its ScriptManager provider";return false;}}
   if(definition.common[2]>=0&&!services.start_script){error="GameEvent TalkToNPC requires its ScriptManager provider";return false;}
   error.clear();return true;
  }
 }
 if(register_call){
  if(!objective->compiled_8){error.clear();return true;}
  const auto identity=objective->ref.action.identity;
  if(type==0){
   if(objective->dispatch_0!=of::Dispatch::kill_enemies){error="GameEvent KillXEnemies Register found unexpected factory kind";return false;}
   std::int32_t event_type=-1;if(impl_->constant("v2QuestObjectiveType","KillXEnemies",&event_type)||event_type<0){error="Actual KillXEnemies event type constant unavailable";return false;}
   auto prior=impl_->kill_x_enemies.find(identity);
   if(prior==impl_->kill_x_enemies.end()){
    auto binding=std::make_unique<Impl::KillXEnemiesBinding>();binding->objective=objective;binding->level_identity=level_identity;binding->game_event_manager_identity=manager_identity;binding->event_type=event_type;binding->match_id=definition.args[0];binding->script_id=definition.common[2];binding->script_context=services.script_context;binding->start_script=services.start_script;binding->game_event_trigger=true;auto* context=binding.get();
    if(!impl_->kill_x_enemies.emplace(identity,std::move(binding)).second){error="GameEvent KillXEnemies receiver metadata collision";return false;}
    bool attached=false;auto status=impl_->current_level_events.attach(event_type,identity,0,context,Impl::KillXEnemiesBinding::receive,attached,error);if(status!=LevelEventStatus::complete||!attached){impl_->kill_x_enemies.erase(identity);if(error.empty())error="GameEvent KillXEnemies EventManager attach failed";return false;}context->attached=true;
   }else if(prior->second->objective!=objective||prior->second->level_identity!=level_identity||prior->second->game_event_manager_identity!=manager_identity||prior->second->event_type!=event_type||prior->second->match_id!=definition.args[0]||prior->second->script_context!=services.script_context||prior->second->start_script!=services.start_script){error="GameEvent KillXEnemies receiver services changed";return false;}
   error.clear();return true;
  }
  if(type==4){
   auto found=impl_->move_in_zone.find(identity);if(found==impl_->move_in_zone.end()){error="GameEvent MoveInZone Register requires its compiled target binding";return false;}auto& binding=*found->second;
   if(!binding.game_event_trigger||binding.objective!=objective||binding.level_identity!=level_identity||binding.game_event_manager_identity!=manager_identity||binding.target_identity!=objective->derived_28){error="GameEvent MoveInZone Register found stale target identity";return false;}
   if(!binding.attached){bool attached=false;auto status=impl_->current_level_events.attach(binding.event_type,identity,0,&binding,Impl::MoveInZoneBinding::receive,attached,error);if(status!=LevelEventStatus::complete||!attached){if(error.empty())error="GameEvent MoveInZone EventManager attach failed";return false;}binding.attached=true;}error.clear();return true;
  }
  if(type==5){
   if(objective->dispatch_0!=of::Dispatch::talk_to_npc||objective->derived_24.index()!=0){error="GameEvent TalkToNPC Register found invalid target";return false;}
   auto prior=impl_->talk_to_npc.find(identity);
   if(prior==impl_->talk_to_npc.end()){
    std::int32_t event_type=-1;if(impl_->constant("v2QuestObjectiveType","TalkToNPC",&event_type)||event_type<0){error="Actual TalkToNPC event type constant unavailable";return false;}
    if(definition.common[0]==5&&!services.talk_marker){error="GameEvent TalkToNPC marker requires current-Level marker services";return false;}
    auto binding=std::make_unique<Impl::TalkToNpcBinding>();binding->owner=this;binding->objective=objective;binding->level_identity=level_identity;binding->game_event_manager_identity=manager_identity;binding->event_type=event_type;binding->target_props_id=std::get<std::int32_t>(objective->derived_24);binding->target_count=definition.args[2];binding->script_id=definition.common[2];binding->script_context=services.script_context;binding->start_script=services.start_script;binding->marker_context=services.talk_marker_context;binding->marker=services.talk_marker;binding->game_event_trigger=true;auto* context=binding.get();
    if(!impl_->talk_to_npc.emplace(identity,std::move(binding)).second){error="GameEvent TalkToNPC receiver metadata collision";return false;}
    bool attached=false;auto status=impl_->current_level_events.attach(event_type,identity,0,context,Impl::TalkToNpcBinding::receive,attached,error);if(status!=LevelEventStatus::complete||!attached){impl_->talk_to_npc.erase(identity);if(error.empty())error="GameEvent TalkToNPC EventManager attach failed";return false;}context->attached=true;
    if(definition.common[0]==5){if(!services.talk_marker(services.talk_marker_context,identity,&objective->derived_28,context->target_props_id,TalkObjectiveMarkerOperationV1::register_target,0,0,error))return false;context->target_marked=true;}
   }else if(prior->second->objective!=objective||prior->second->level_identity!=level_identity||prior->second->game_event_manager_identity!=manager_identity||prior->second->target_props_id!=std::get<std::int32_t>(objective->derived_24)||prior->second->script_context!=services.script_context||prior->second->start_script!=services.start_script){error="GameEvent TalkToNPC receiver services changed";return false;}
   error.clear();return true;
  }
  error="GameEvent Register reached an unsupported trigger kind";return false;
 }
 error="GameEvent Objective adapter received an unsupported source member pointer";return false;
}

bool Owner::dispatch_objective_compile_member_v1(
 qc::Objective& objective,const qc::MemberCall& call,
 const ObjectiveCompileDispatchServicesV1& services,std::string& error){
 if(impl_->closed||!impl_->save){error="Objective Compile callback has no live canonical Save";return false;}
 // CompileQuests visits each Quest's accept/end ActionRefs before iterating
 // ObjectiveList children. Those refs borrow canonical Objective factory
 // Records too, but are deliberately not members of the child list; the base
 // Objective virtuals reached here are source no-ops. Recognize only records
 // retained by this factory and actually referenced by a published Quest.
 const auto objective_factory_record=impl_->objectives.find(
     objective.ref.action.identity);
 if(objective_factory_record!=impl_->objectives.end()&&
    objective_factory_record->second.get()==&objective&&
    objective.ref.fields==&objective.fields&&
    objective.ref.action.character_10==&objective.fields.character_10){
  // Quest actions (accept +0x18 / end +0x1c) are canonical Objective factory
  // Records outside the ObjectiveList. They run through the same Compile /
  // Register / marker / Unregister leaves as list children, addressed by the
  // owning Quest and the action sentinel ordinal.
  std::uintptr_t action_quest=0;
  for(const auto& entry:impl_->slots){
   const auto& slot=*entry.second;
   if(!slot.value||slot.value->record().ref.identity!=entry.first)continue;
   const auto& quest=slot.value->record();
   if(quest.action_18==&objective.ref.action||
      quest.action_1c==&objective.ref.action){action_quest=entry.first;break;}
  }
  if(action_quest)
   return dispatch_objective_compile_member_v1(action_quest,objective,call,services,error);
 }
 std::uintptr_t parent=0;unsigned matches=0;
 for(const auto& entry:impl_->slots){
  const auto& slot=*entry.second;
  if(!slot.value||slot.value->record().ref.identity!=entry.first)continue;
  const auto& list=slot.value->objectives();
  if(list.count_0<0||(list.count_0>0&&(!list.children_4||
     std::uint64_t(std::uint32_t(list.count_0))>list.children_4->slots.size()))){
   error="Objective Compile callback encountered an invalid canonical ObjectiveList";return false;
  }
  for(std::uint32_t index=0;index<std::uint32_t(list.count_0);++index){
   if(impl_->resolve_objective(list.children_4->slots[index])!=&objective)continue;
   parent=entry.first;++matches;
  }
 }
 if(matches!=1){
  error=matches?"Objective Compile callback found an aliased parent identity":
                "Objective Compile callback cannot find this canonical Objective in its Save";
  return false;
 }
 return dispatch_objective_compile_member_v1(parent,objective,call,services,error);
}
bool Owner::register_kill_x_enemies_objective(std::uintptr_t quest_identity,
 std::uint32_t objective_ordinal,
 void* script_context,KillObjectiveStartScript start_script,std::string& error){
 if(impl_->busy||impl_->closed){error="Native Quest owner unavailable for KillXEnemies Register";return false;}
 auto* resolved=impl_->objective_at(quest_identity,objective_ordinal);
 if(!resolved){error="KillXEnemies Register requires an Objective in this Owner's canonical Quest list";return false;}
 auto& objective=*resolved;const auto identity=objective.ref.action.identity;
 if(objective.dispatch_0!=of::Dispatch::kill_enemies||!objective.compiled_8){
  error="KillXEnemies Register requires the source Compile projection";return false;
 }
 dh2_quest_objective definition{};
 if(!objective_definition(objective,definition,error))return false;
 if(definition.common[0]!=objective.fields.type_4){error="KillXEnemies Register found a changed Objective selector";return false;}
 const auto script_id=definition.common[2];
 if(script_id>=0&&!start_script){error="KillXEnemies Register requires its source completion-script provider";return false;}
 std::int32_t event_type=0;
 if(impl_->constant("v2QuestObjectiveType","KillXEnemies",&event_type)){
  error="Actual v2QuestObjectiveType.KillXEnemies constant unavailable";return false;
 }
 if(const auto prior=impl_->kill_x_enemies.find(identity);prior!=impl_->kill_x_enemies.end()){
  const auto& binding=*prior->second;
  if(binding.objective!=&objective||binding.event_type!=event_type||
     binding.match_id!=definition.args[0]||binding.script_id!=script_id||
     binding.script_context!=script_context||binding.start_script!=start_script||
     binding.detach_pending){error="KillXEnemies Objective is already registered with different services";return false;}
  error.clear();return true;
 }
 auto binding=std::make_unique<Impl::KillXEnemiesBinding>();
 binding->objective=&objective;binding->event_type=event_type;
 binding->match_id=definition.args[0];binding->script_id=script_id;
 binding->script_context=script_context;binding->start_script=start_script;
 auto* context=binding.get();
 try{if(!impl_->kill_x_enemies.emplace(identity,std::move(binding)).second){
  error="KillXEnemies receiver registration raced an existing binding";return false;
 }}catch(const std::exception&){error="KillXEnemies receiver metadata allocation failed";return false;}
 bool attached=false;
 const auto status=impl_->current_level_events.attach(event_type,identity,0,context,
  Impl::KillXEnemiesBinding::receive,attached,error);
 if(status!=LevelEventStatus::complete||!attached){
  impl_->kill_x_enemies.erase(identity);
  if(error.empty())error="KillXEnemies receiver was already attached outside this Owner";
  return false;
 }
 context->attached=true;error.clear();return true;
}
bool Owner::unregister_kill_x_enemies_objective(std::uintptr_t quest_identity,
 std::uint32_t objective_ordinal,
 std::string& error){
 if(impl_->busy||impl_->closed){error="Native Quest owner unavailable for KillXEnemies Unregister";return false;}
 auto* objective=impl_->objective_at(quest_identity,objective_ordinal);
 if(!objective){error="KillXEnemies Unregister requires an Objective in this Owner's canonical Quest list";return false;}
 const auto identity=objective->ref.action.identity;
 const auto found=impl_->kill_x_enemies.find(identity);
 if(found==impl_->kill_x_enemies.end()){error="KillXEnemies Objective is not registered by this Owner";return false;}
 auto& binding=*found->second;
 if(!binding.detach_pending){bool scheduled=false;
  if(impl_->current_level_events.delayed_detach(binding.event_type,identity,scheduled,error)!=LevelEventStatus::complete)return false;
  if(!scheduled){error="KillXEnemies receiver disappeared before Objective::Unregister";return false;}
  binding.detach_pending=true;
 }
 error.clear();return true;
}
bool Owner::compile_clear_enemies_objective(std::uintptr_t quest_identity,
 std::uint32_t objective_ordinal,
 std::int32_t current_level_id,std::int32_t loaded_match_count,
 void* script_context,KillObjectiveStartScript start_script,std::string& error){
 if(impl_->busy||impl_->closed){error="Native Quest owner unavailable for ClearEnemies Compile";return false;}
 auto* objective=impl_->objective_at(quest_identity,objective_ordinal);
 if(!objective){error="ClearEnemies Compile requires an Objective in this Owner's canonical Quest list";return false;}
 if(objective->dispatch_0!=of::Dispatch::clear_enemies||current_level_id<0||loaded_match_count<0){
  error="ClearEnemies Compile requires type 1, a live Level ID, and a nonnegative loaded-enemy count";return false;
 }
 dh2_quest_objective definition{};
 if(!objective_definition(*objective,definition,error))return false;
 if(definition.common[0]!=objective->fields.type_4){error="ClearEnemies Compile found a changed Objective type selector";return false;}
 objective->derived_24=objective->fields.py_data_c;
 const auto level_filter=definition.args[1];
 if(level_filter!=-1&&level_filter!=current_level_id){error.clear();return true;}
 if(loaded_match_count==0){error.clear();return true;}
 objective->derived_2c=static_cast<std::uint32_t>(loaded_match_count);
 objective->compiled_8=1;
 if(loaded_match_count<=static_cast<std::int32_t>(objective->quantity_20)&&!objective->done_14){
  objective->done_14=1;
  if(definition.common[2]>=0&&
     (!start_script||!start_script(script_context,definition.common[2],error))){
   if(error.empty())error="ClearEnemies Compile completion needs the source script-base provider";
   return false;
  }
 }
 error.clear();return true;
}
bool Owner::compile_clear_enemies_objective_from_level(
 std::uintptr_t quest_identity,std::uint32_t objective_ordinal,
 const ClearEnemiesPopulationServicesV1& population,
 void* script_context,KillObjectiveStartScript start_script,std::string& error){
 if(impl_->busy||impl_->closed){error="Native Quest owner unavailable for live ClearEnemies Compile";return false;}
 if(!population.read_current_level_population||!population.expected_level_identity){
  error="Live ClearEnemies Compile requires the current-Level population provider and identity";return false;
 }
 auto* objective=impl_->objective_at(quest_identity,objective_ordinal);
 if(!objective||objective->dispatch_0!=of::Dispatch::clear_enemies){
  error="Live ClearEnemies Compile requires its canonical ClearEnemies Objective";return false;
 }
 dh2_quest_objective definition{};
 if(!objective_definition(*objective,definition,error))return false;
 if(definition.common[0]!=objective->fields.type_4||definition.args[0]<-32768||
    definition.args[0]>32767){
  error="Live ClearEnemies Compile found an invalid source property selector";return false;
 }
 const auto expected_owner=reinterpret_cast<std::uintptr_t>(this);
 ClearEnemiesPopulationSnapshotV1 snapshot{};
 try{
  if(!population.read_current_level_population(population.context,expected_owner,
       population.expected_level_identity,
       definition.args[0],snapshot,error)){
   if(error.empty())error="Current-Level ClearEnemies population query failed";
   return false;
  }
 }catch(const std::exception& ex){error=ex.what();return false;}
 catch(...){error="Current-Level ClearEnemies population provider threw";return false;}
 if(snapshot.event_owner_identity!=expected_owner||
    snapshot.level_identity!=population.expected_level_identity||snapshot.level_id<0||
    snapshot.loaded_match_count<0){
  error="Live ClearEnemies Compile rejected a stale Level owner or invalid population";return false;
 }
 return compile_clear_enemies_objective(quest_identity,objective_ordinal,
       snapshot.level_id,snapshot.loaded_match_count,script_context,start_script,error);
}
bool Owner::register_clear_enemies_objective(std::uintptr_t quest_identity,
 std::uint32_t objective_ordinal,
 void* script_context,KillObjectiveStartScript start_script,std::string& error){
 if(impl_->busy||impl_->closed){error="Native Quest owner unavailable for ClearEnemies Register";return false;}
 auto* resolved=impl_->objective_at(quest_identity,objective_ordinal);
 if(!resolved){error="ClearEnemies Register requires an Objective in this Owner's canonical Quest list";return false;}
 auto& objective=*resolved;const auto identity=objective.ref.action.identity;
 if(objective.dispatch_0!=of::Dispatch::clear_enemies||!objective.compiled_8){
  error="ClearEnemies Register requires the source Compile projection";return false;
 }
 dh2_quest_objective definition{};
 if(!objective_definition(objective,definition,error))return false;
 if(definition.common[0]!=objective.fields.type_4){error="ClearEnemies Register found a changed Objective selector";return false;}
 const auto script_id=definition.common[2];
 if(script_id>=0&&!start_script){error="ClearEnemies Register requires its source completion-script provider";return false;}
 std::int32_t event_type=0;
 if(impl_->constant("v2QuestObjectiveType","ClearEnemies",&event_type)){
  error="Actual v2QuestObjectiveType.ClearEnemies constant unavailable";return false;
 }
 if(const auto prior=impl_->kill_x_enemies.find(identity);prior!=impl_->kill_x_enemies.end()){
  const auto& binding=*prior->second;
  if(binding.objective!=&objective||binding.expected_dispatch!=of::Dispatch::clear_enemies||
     binding.event_type!=event_type||binding.match_id!=definition.args[0]||
     binding.script_id!=script_id||binding.script_context!=script_context||
     binding.start_script!=start_script||binding.detach_pending){
   error="ClearEnemies Objective is already registered with different services";return false;
  }
  error.clear();return true;
 }
 auto binding=std::make_unique<Impl::KillXEnemiesBinding>();
 binding->objective=&objective;binding->expected_dispatch=of::Dispatch::clear_enemies;
 binding->event_type=event_type;binding->match_id=definition.args[0];
 binding->script_id=script_id;binding->script_context=script_context;binding->start_script=start_script;
 auto* context=binding.get();
 try{if(!impl_->kill_x_enemies.emplace(identity,std::move(binding)).second){
  error="ClearEnemies receiver registration raced an existing binding";return false;
 }}catch(const std::exception&){error="ClearEnemies receiver metadata allocation failed";return false;}
 bool attached=false;
 const auto status=impl_->current_level_events.attach(event_type,identity,0,context,
  Impl::KillXEnemiesBinding::receive,attached,error);
 if(status!=LevelEventStatus::complete||!attached){
  impl_->kill_x_enemies.erase(identity);
  if(error.empty())error="ClearEnemies receiver was already attached outside this Owner";
  return false;
 }
 context->attached=true;error.clear();return true;
}
bool Owner::unregister_clear_enemies_objective(std::uintptr_t quest_identity,
 std::uint32_t objective_ordinal,std::string& error){
 if(impl_->busy||impl_->closed){error="Native Quest owner unavailable for ClearEnemies Unregister";return false;}
 auto* objective=impl_->objective_at(quest_identity,objective_ordinal);
 if(!objective||objective->dispatch_0!=of::Dispatch::clear_enemies){
  error="ClearEnemies Unregister requires its Objective in this Owner's canonical Quest list";return false;
 }
 const auto identity=objective->ref.action.identity;
 const auto found=impl_->kill_x_enemies.find(identity);
 if(found==impl_->kill_x_enemies.end()||found->second->expected_dispatch!=of::Dispatch::clear_enemies){
  error="ClearEnemies Objective is not registered by this Owner";return false;
 }
 auto& binding=*found->second;
 if(!binding.detach_pending){bool scheduled=false;
  if(impl_->current_level_events.delayed_detach(binding.event_type,identity,scheduled,error)!=LevelEventStatus::complete)return false;
  if(!scheduled){error="ClearEnemies receiver disappeared before Objective::Unregister";return false;}
  binding.detach_pending=true;
 }
 error.clear();return true;
}
bool Owner::compile_kill_enemy_template_objective_from_level(
 std::uintptr_t quest_identity,std::uint32_t objective_ordinal,
 const KillEnemyTemplatePopulationServicesV1& population,
 void* script_context,KillObjectiveStartScript start_script,std::string& error){
 if(impl_->busy||impl_->closed){error="Native Quest owner unavailable for live KillEnemyTemplate Compile";return false;}
 if(!population.read_current_level_population||!population.expected_level_identity){
  error="Live KillEnemyTemplate Compile requires the current-Level population provider and identity";return false;
 }
 auto* objective=impl_->objective_at(quest_identity,objective_ordinal);
 if(!objective||objective->dispatch_0!=of::Dispatch::kill_enemy_template){
  error="Live KillEnemyTemplate Compile requires its canonical Objective";return false;
 }
 dh2_quest_objective definition{};
 if(!objective_definition(*objective,definition,error))return false;
 const auto template_id=definition.args[0];
 const auto level_filter=definition.args[1];
 const auto required=definition.args[2];
 if(definition.common[0]!=objective->fields.type_4||template_id<-32768||
    template_id>32767||required<=0){
  error="Live KillEnemyTemplate Compile found an invalid source template selector or target";return false;
 }
 const auto expected_owner=reinterpret_cast<std::uintptr_t>(this);
 ClearEnemiesPopulationSnapshotV1 snapshot{};
 try{
  if(!population.read_current_level_population(population.context,expected_owner,
       population.expected_level_identity,template_id,snapshot,error)){
   if(error.empty())error="Current-Level KillEnemyTemplate population query failed";
   return false;
  }
 }catch(const std::exception& ex){error=ex.what();return false;}
 catch(...){error="Current-Level KillEnemyTemplate population provider threw";return false;}
 if(snapshot.event_owner_identity!=expected_owner||
    snapshot.level_identity!=population.expected_level_identity||snapshot.level_id<0||
    snapshot.loaded_match_count<0){
  error="Live KillEnemyTemplate Compile rejected a stale Level owner or invalid population";return false;
 }
 if(impl_->busy||impl_->closed){error="Native Quest owner changed during KillEnemyTemplate population query";return false;}
 objective=impl_->objective_at(quest_identity,objective_ordinal);
 if(!objective||objective->dispatch_0!=of::Dispatch::kill_enemy_template){
  error="KillEnemyTemplate Objective changed during population query";return false;
 }
 objective->derived_24=objective->fields.py_data_c;
 objective->derived_2c=static_cast<std::uint32_t>(required);
 if((level_filter!=-1&&level_filter!=snapshot.level_id)||snapshot.loaded_match_count<=0){
  objective->compiled_8=0;error.clear();return true;
 }
 objective->compiled_8=1;
 if(required<=static_cast<std::int32_t>(objective->quantity_20)&&!objective->done_14){
  objective->done_14=1;
  if(definition.common[2]>=0&&(!start_script||!start_script(script_context,definition.common[2],error))){
   if(error.empty())error="KillEnemyTemplate Compile completion needs the source script-base provider";
   return false;
  }
 }
 error.clear();return true;
}
bool Owner::register_kill_enemy_template_objective(std::uintptr_t quest_identity,
 std::uint32_t objective_ordinal,void* script_context,
 KillObjectiveStartScript start_script,std::string& error){
 if(impl_->busy||impl_->closed){error="Native Quest owner unavailable for KillEnemyTemplate Register";return false;}
 auto* resolved=impl_->objective_at(quest_identity,objective_ordinal);
 if(!resolved){error="KillEnemyTemplate Register requires a canonical Objective";return false;}
 auto& objective=*resolved;const auto identity=objective.ref.action.identity;
 if(objective.dispatch_0!=of::Dispatch::kill_enemy_template||!objective.compiled_8){
  error="KillEnemyTemplate Register requires its source Compile projection";return false;
 }
 dh2_quest_objective definition{};
 if(!objective_definition(objective,definition,error))return false;
 if(definition.common[0]!=objective.fields.type_4){error="KillEnemyTemplate Register found a changed Objective selector";return false;}
 const auto script_id=definition.common[2];
 if(script_id>=0&&!start_script){error="KillEnemyTemplate Register requires its completion-script provider";return false;}
 std::int32_t event_type=0;
 if(impl_->constant("v2QuestObjectiveType","KillEnemyTemplate",&event_type)){
  error="Actual v2QuestObjectiveType.KillEnemyTemplate constant unavailable";return false;
 }
 if(const auto prior=impl_->kill_x_enemies.find(identity);prior!=impl_->kill_x_enemies.end()){
  const auto& binding=*prior->second;
  if(binding.objective!=&objective||binding.expected_dispatch!=of::Dispatch::kill_enemy_template||
     binding.event_type!=event_type||binding.match_id!=definition.args[0]||
     binding.script_id!=script_id||binding.script_context!=script_context||
     binding.start_script!=start_script||binding.detach_pending){
   error="KillEnemyTemplate Objective is already registered with different services";return false;
  }
  error.clear();return true;
 }
 auto binding=std::make_unique<Impl::KillXEnemiesBinding>();
 binding->objective=&objective;binding->expected_dispatch=of::Dispatch::kill_enemy_template;
 binding->event_type=event_type;binding->match_id=definition.args[0];
 binding->script_id=script_id;binding->script_context=script_context;binding->start_script=start_script;
 auto* context=binding.get();
 try{if(!impl_->kill_x_enemies.emplace(identity,std::move(binding)).second){
  error="KillEnemyTemplate receiver registration raced an existing binding";return false;
 }}catch(const std::exception&){error="KillEnemyTemplate receiver metadata allocation failed";return false;}
 bool attached=false;
 const auto status=impl_->current_level_events.attach(event_type,identity,0,context,
  Impl::KillXEnemiesBinding::receive,attached,error);
 if(status!=LevelEventStatus::complete||!attached){
  impl_->kill_x_enemies.erase(identity);
  if(error.empty())error="KillEnemyTemplate receiver was already attached outside this Owner";
  return false;
 }
 context->attached=true;error.clear();return true;
}
bool Owner::unregister_kill_enemy_template_objective(std::uintptr_t quest_identity,
 std::uint32_t objective_ordinal,std::string& error){
 if(impl_->busy||impl_->closed){error="Native Quest owner unavailable for KillEnemyTemplate Unregister";return false;}
 auto* objective=impl_->objective_at(quest_identity,objective_ordinal);
 if(!objective||objective->dispatch_0!=of::Dispatch::kill_enemy_template){
  error="KillEnemyTemplate Unregister requires its Objective in this Owner's canonical Quest list";return false;
 }
 const auto identity=objective->ref.action.identity;
 const auto found=impl_->kill_x_enemies.find(identity);
 if(found==impl_->kill_x_enemies.end()||found->second->expected_dispatch!=of::Dispatch::kill_enemy_template){
  error="KillEnemyTemplate Objective is not registered by this Owner";return false;
 }
 auto& binding=*found->second;
 if(!binding.detach_pending){bool scheduled=false;
  if(impl_->current_level_events.delayed_detach(binding.event_type,identity,scheduled,error)!=LevelEventStatus::complete)return false;
  if(!scheduled){error="KillEnemyTemplate receiver disappeared before Objective::Unregister";return false;}
  binding.detach_pending=true;
 }
 error.clear();return true;
}
bool Owner::compile_clear_enemy_template_objective_from_level(
 std::uintptr_t quest_identity,std::uint32_t objective_ordinal,
 const ClearEnemyTemplatePopulationServicesV1& population,
 void* script_context,KillObjectiveStartScript start_script,std::string& error){
 if(impl_->busy||impl_->closed){error="Native Quest owner unavailable for live ClearEnemyTemplate Compile";return false;}
 if(!population.read_current_level_population||!population.expected_level_identity){
  error="Live ClearEnemyTemplate Compile requires the current-Level population provider and identity";return false;
 }
 auto* objective=impl_->objective_at(quest_identity,objective_ordinal);
 if(!objective||objective->dispatch_0!=of::Dispatch::clear_enemy_template){
  error="Live ClearEnemyTemplate Compile requires its canonical Objective";return false;
 }
 dh2_quest_objective definition{};
 if(!objective_definition(*objective,definition,error))return false;
 if(definition.common[0]!=objective->fields.type_4||definition.args[0]<-32768||
    definition.args[0]>32767){
  error="Live ClearEnemyTemplate Compile found an invalid source template selector";return false;
 }
 const auto expected_owner=reinterpret_cast<std::uintptr_t>(this);
 ClearEnemiesPopulationSnapshotV1 snapshot{};
 try{
  if(!population.read_current_level_population(population.context,expected_owner,
       population.expected_level_identity,
       definition.args[0],snapshot,error)){
   if(error.empty())error="Current-Level ClearEnemyTemplate population query failed";
   return false;
  }
 }catch(const std::exception& ex){error=ex.what();return false;}
 catch(...){error="Current-Level ClearEnemyTemplate population provider threw";return false;}
 if(snapshot.event_owner_identity!=expected_owner||
    snapshot.level_identity!=population.expected_level_identity||snapshot.level_id<0||
    snapshot.loaded_match_count<0){
  error="Live ClearEnemyTemplate Compile rejected a stale Level owner or invalid population";return false;
 }
 if(impl_->busy||impl_->closed){error="Native Quest owner changed during ClearEnemyTemplate population query";return false;}
 objective=impl_->objective_at(quest_identity,objective_ordinal);
 if(!objective||objective->dispatch_0!=of::Dispatch::clear_enemy_template){
  error="ClearEnemyTemplate Objective changed during population query";return false;
 }
 objective->derived_24=objective->fields.py_data_c;
 if(definition.args[1]!=-1&&definition.args[1]!=snapshot.level_id){error.clear();return true;}
 if(snapshot.loaded_match_count==0){error.clear();return true;}
 objective->derived_2c=static_cast<std::uint32_t>(snapshot.loaded_match_count);
 objective->compiled_8=1;
 if(snapshot.loaded_match_count<=static_cast<std::int32_t>(objective->quantity_20)&&!objective->done_14){
  objective->done_14=1;
  if(definition.common[2]>=0&&(!start_script||!start_script(script_context,definition.common[2],error))){
   if(error.empty())error="ClearEnemyTemplate Compile completion needs the source script-base provider";
   return false;
  }
 }
 error.clear();return true;
}
bool Owner::register_clear_enemy_template_objective(std::uintptr_t quest_identity,
 std::uint32_t objective_ordinal,void* script_context,
 KillObjectiveStartScript start_script,std::string& error){
 if(impl_->busy||impl_->closed){error="Native Quest owner unavailable for ClearEnemyTemplate Register";return false;}
 auto* resolved=impl_->objective_at(quest_identity,objective_ordinal);
 if(!resolved){error="ClearEnemyTemplate Register requires a canonical Objective";return false;}
 auto& objective=*resolved;const auto identity=objective.ref.action.identity;
 if(objective.dispatch_0!=of::Dispatch::clear_enemy_template||!objective.compiled_8){
  error="ClearEnemyTemplate Register requires its source Compile projection";return false;
 }
 dh2_quest_objective definition{};
 if(!objective_definition(objective,definition,error))return false;
 if(definition.common[0]!=objective.fields.type_4){error="ClearEnemyTemplate Register found a changed Objective selector";return false;}
 const auto script_id=definition.common[2];
 if(script_id>=0&&!start_script){error="ClearEnemyTemplate Register requires its completion-script provider";return false;}
 std::int32_t event_type=0;
 if(impl_->constant("v2QuestObjectiveType","ClearEnemyTemplate",&event_type)){
  error="Actual v2QuestObjectiveType.ClearEnemyTemplate constant unavailable";return false;
 }
 if(const auto prior=impl_->kill_x_enemies.find(identity);prior!=impl_->kill_x_enemies.end()){
  const auto& binding=*prior->second;
  if(binding.objective!=&objective||binding.expected_dispatch!=of::Dispatch::clear_enemy_template||
     binding.event_type!=event_type||binding.match_id!=definition.args[0]||
     binding.script_id!=script_id||binding.script_context!=script_context||
     binding.start_script!=start_script||binding.detach_pending){
   error="ClearEnemyTemplate Objective is already registered with different services";return false;
  }
  error.clear();return true;
 }
 auto binding=std::make_unique<Impl::KillXEnemiesBinding>();
 binding->objective=&objective;binding->expected_dispatch=of::Dispatch::clear_enemy_template;
 binding->event_type=event_type;binding->match_id=definition.args[0];
 binding->script_id=script_id;binding->script_context=script_context;binding->start_script=start_script;
 auto* context=binding.get();
 try{if(!impl_->kill_x_enemies.emplace(identity,std::move(binding)).second){
  error="ClearEnemyTemplate receiver registration raced an existing binding";return false;
 }}catch(const std::exception&){error="ClearEnemyTemplate receiver metadata allocation failed";return false;}
 bool attached=false;
 const auto status=impl_->current_level_events.attach(event_type,identity,0,context,
  Impl::KillXEnemiesBinding::receive,attached,error);
 if(status!=LevelEventStatus::complete||!attached){
  impl_->kill_x_enemies.erase(identity);
  if(error.empty())error="ClearEnemyTemplate receiver was already attached outside this Owner";
  return false;
 }
 context->attached=true;error.clear();return true;
}
bool Owner::unregister_clear_enemy_template_objective(std::uintptr_t quest_identity,
 std::uint32_t objective_ordinal,std::string& error){
 if(impl_->busy||impl_->closed){error="Native Quest owner unavailable for ClearEnemyTemplate Unregister";return false;}
 auto* objective=impl_->objective_at(quest_identity,objective_ordinal);
 if(!objective||objective->dispatch_0!=of::Dispatch::clear_enemy_template){
  error="ClearEnemyTemplate Unregister requires its canonical Objective";return false;
 }
 const auto identity=objective->ref.action.identity;
 const auto found=impl_->kill_x_enemies.find(identity);
 if(found==impl_->kill_x_enemies.end()||found->second->expected_dispatch!=of::Dispatch::clear_enemy_template){
  error="ClearEnemyTemplate Objective is not registered by this Owner";return false;
 }
 auto& binding=*found->second;
 if(!binding.detach_pending){bool scheduled=false;
  if(impl_->current_level_events.delayed_detach(binding.event_type,identity,scheduled,error)!=LevelEventStatus::complete)return false;
  if(!scheduled){error="ClearEnemyTemplate receiver disappeared before Objective::Unregister";return false;}
  binding.detach_pending=true;
 }
 error.clear();return true;
}
bool Owner::unregister_gather_loot_objective(std::uintptr_t identity,std::string& error){
 if(impl_->busy||impl_->closed){error="Native Quest owner unavailable for GatherLoot unregister";return false;}
 const auto found=impl_->gather_loot.find(identity);
 if(found==impl_->gather_loot.end()){error="GatherLoot Objective is not registered by this Owner";return false;}
 auto& binding=*found->second;
 if(!binding.detach_pending){bool scheduled=false;
  if(impl_->current_level_events.delayed_detach(binding.event_type,identity,scheduled,error)!=LevelEventStatus::complete)return false;
  if(!scheduled){error="GatherLoot EventManager receiver disappeared before Objective::Unregister";return false;}
  binding.detach_pending=true;
 }
 if(binding.inventory_registered){
  if(!static_cast<data::FreshInventoryOwnedV4*>(binding.quantity_context)->unregister_quest_gathering_item_id(binding.item_id,error))return false;
  binding.inventory_registered=false;
 }
 error.clear();return true;
}
bool Owner::unregister_talk_to_npc_objective(std::uintptr_t quest_identity,
 std::uint32_t objective_ordinal,std::string& error){
 if(impl_->busy||impl_->closed){error="Native Quest owner unavailable for TalkToNPC Unregister";return false;}
 auto* objective=impl_->objective_at(quest_identity,objective_ordinal);
 if(!objective||objective->dispatch_0!=of::Dispatch::talk_to_npc){
  error="TalkToNPC Unregister requires its canonical Quest Objective";return false;
 }
 const auto found=impl_->talk_to_npc.find(objective->ref.action.identity);
 // Objective_EventReceiver::Unregister (0x47adbc) is a DelayedDetach that finds
 // nothing for a receiver this Level never attached; that is not a fault.
 if(found==impl_->talk_to_npc.end()){error.clear();return true;}
 auto& binding=*found->second;
 if(binding.objective!=objective||binding.quest_identity!=quest_identity||
    binding.ordinal!=objective_ordinal){error="TalkToNPC receiver identity/projection changed";return false;}
 if(!binding.detach_pending){
  bool scheduled=false;
  const auto status=impl_->current_level_events.delayed_detach(binding.event_type,
       objective->ref.action.identity,scheduled,error);
  if(status!=LevelEventStatus::complete||!scheduled){
   if(error.empty())error="TalkToNPC EventManager delayed detach failed";
   return false;
  }
  binding.detach_pending=true;
 }
 if(binding.target_marked){
  if(!binding.marker){error="TalkToNPC Unregister lost its borrowed Character marker provider";return false;}
  if(!binding.marker(binding.marker_context,binding.objective->ref.action.identity,
       &binding.objective->derived_28,binding.target_props_id,
       TalkObjectiveMarkerOperationV1::unregister_target,0,0,error)){
   if(error.empty())error="TalkToNPC target Character marker clear failed";
   return false;
  }
  binding.target_marked=false;
 }
 error.clear();return true;
}
bool Owner::unregister_move_in_zone_objective(std::uintptr_t quest_identity,
 std::uint32_t objective_ordinal,std::string& error){
 if(impl_->busy||impl_->closed){error="Native Quest owner unavailable for MoveInZone Unregister";return false;}
 auto* objective=impl_->objective_at(quest_identity,objective_ordinal);
 if(!objective||objective->dispatch_0!=of::Dispatch::move_in_zone){
  error="MoveInZone Unregister requires its canonical Quest Objective";return false;
 }
 const auto found=impl_->move_in_zone.find(objective->ref.action.identity);
 if(found==impl_->move_in_zone.end()){error.clear();return true;} // never attached: DelayedDetach finds nothing
 auto& binding=*found->second;
 if(binding.objective!=objective||binding.quest_identity!=quest_identity||
    binding.ordinal!=objective_ordinal){error="MoveInZone receiver identity/projection changed";return false;}
 if(binding.detach_pending){error.clear();return true;}
 bool scheduled=false;
 if(impl_->current_level_events.delayed_detach(binding.event_type,
      objective->ref.action.identity,scheduled,error)!=LevelEventStatus::complete)return false;
 if(binding.attached&&!scheduled){error="MoveInZone EventManager receiver disappeared before Unregister";return false;}
 binding.attached=false;binding.detach_pending=scheduled;
 error.clear();return true;
}
bool Owner::unregister_open_game_object_objective(std::uintptr_t quest_identity,
 std::uint32_t objective_ordinal,std::string& error){
 if(impl_->busy||impl_->closed){error="Native Quest owner unavailable for OpenGameObject Unregister";return false;}
 auto* objective=impl_->objective_at(quest_identity,objective_ordinal);
 if(!objective||objective->dispatch_0!=of::Dispatch::open_game_object){
  error="OpenGameObject Unregister requires its canonical Quest Objective";return false;
 }
 const auto found=impl_->open_game_object.find(objective->ref.action.identity);
 if(found==impl_->open_game_object.end()){error="OpenGameObject Objective has no source Compile binding";return false;}
 auto& binding=*found->second;
 if(binding.objective!=objective||binding.quest_identity!=quest_identity||
    binding.ordinal!=objective_ordinal){error="OpenGameObject receiver identity/projection changed";return false;}
 return og::unregister_objective(*objective,binding.objective_binding,error);
}
bool Owner::compile_gather_loot_objectives(std::uintptr_t quest_identity,
 data::FreshInventoryOwnedV4& inventory,std::int32_t current_level_id,
 void* script_context,GatherLootStartScript start_script,std::string& error){
 if(impl_->busy||impl_->closed){error="Native Quest owner unavailable for Objective_GatherLoot::Compile";return false;}
 auto* quest=impl_->resolve_quest_identity(quest_identity);
 if(!quest||inventory.character()!=quest->record().fields.character_60||current_level_id<0){
  error="GatherLoot Compile requires this owner's Quest, its Character inventory, and a live source Level ID";return false;
 }
 auto& list=quest->objectives();
 if(list.count_0<0||(list.count_0>0&&(!list.children_4||std::uint64_t(list.count_0)>list.children_4->slots.size()))){
  error="GatherLoot Compile saw an invalid canonical ObjectiveList backing";return false;
 }
 impl_->busy=true;struct Guard{Impl& owner;~Guard(){owner.busy=false;}}guard{*impl_};
 for(std::uint32_t index=0;index<std::uint32_t(list.count_0);++index){
  auto* objective=impl_->resolve_objective(list.children_4->slots[index]);
  if(!objective){error="GatherLoot Compile could not resolve the canonical Objective factory Record";return false;}
  if(objective->dispatch_0!=of::Dispatch::gather_loot){
   error="Quest Compile reached an Objective kind whose source Compile provider is not wired";return false;
  }
  dh2_quest_objective definition{};
  if(!objective_definition(*objective,definition,error))return false;
  if(definition.common[0]!=objective->fields.type_4){error="GatherLoot Compile found a changed Objective type selector";return false;}
  const auto level_filter=definition.args[0],item_id=definition.args[1],target=definition.args[2];
  if(item_id<=0||target<=0||std::size_t(item_id)>=inventory.table().rows.size()){
   error="Objective_GatherLoot::Compile reached invalid source Loot/target fields";return false;
  }
  if(level_filter!=-1&&level_filter!=current_level_id)continue;
  bool found_item=false;std::int32_t quantity=0;
  if(!inventory.quest_gathering_item_quantity(item_id,found_item,quantity,error))return false;
  if(!data::quest_gather_loot_receiver_v1::compile(*objective,true,item_id,target,
      definition.common[2],found_item,quantity,script_context,start_script,error))return false;
 }
 error.clear();return true;
}
bool Owner::transition_gather_loot_registration(std::uintptr_t quest_identity,
 std::int32_t prior_state,std::int32_t stored_state,
 data::FreshInventoryOwnedV4& inventory,void* script_context,
 GatherLootStartScript start_script,std::string& error){
 if(impl_->busy||impl_->closed){error="Native Quest owner unavailable for Quest::SetState objective-registration step";return false;}
 auto* quest=impl_->resolve_quest_identity(quest_identity);
 if(!quest||quest->record().state_0!=stored_state||stored_state<0||stored_state>13||
    inventory.character()!=quest->record().fields.character_60){
  error="Quest::SetState objective step requires the already-stored canonical Quest state and Character inventory";return false;
 }
 if(prior_state==stored_state&&!quest->record().byte_64){error.clear();return true;}
 if(prior_state==6&&stored_state==6){
  error="Quest::SetState volatile state-6 self-transition needs source delayed-detach/refcount reattach semantics";return false;
 }
 auto& list=quest->objectives();
 if(list.count_0<0||(list.count_0>0&&(!list.children_4||std::uint64_t(list.count_0)>list.children_4->slots.size()))){
  error="Quest::SetState saw an invalid canonical ObjectiveList backing";return false;
 }
 // Quest::SetState stores the new state first. Leaving state 6 unregisters
 // before the switch; entering 6 registers after ExecScript (the caller must
 // run that source script before invoking this substep).
 if(prior_state==6&&stored_state!=6){
  for(std::uint32_t index=0;index<std::uint32_t(list.count_0);++index){
   auto* objective=impl_->resolve_objective(list.children_4->slots[index]);
   if(!objective){error="Quest::SetState could not resolve an Objective during Unregister";return false;}
   if(objective->dispatch_0!=of::Dispatch::gather_loot){
    if(objective->compiled_8){error="ObjectiveList::Unregister requires an unwired non-GatherLoot Objective provider";return false;}
    continue;
   }
   if(impl_->gather_loot.count(objective->ref.action.identity)&&
      !unregister_gather_loot_objective(objective->ref.action.identity,error))return false;
  }
 }
 if(stored_state==6){
  for(std::uint32_t index=0;index<std::uint32_t(list.count_0);++index){
   auto* objective=impl_->resolve_objective(list.children_4->slots[index]);
   if(!objective){error="Quest::SetState could not resolve an Objective during Register";return false;}
   if(!objective->compiled_8)continue; // Objective_EventReceiver::Register is a source no-op when uncompiled.
   if(objective->dispatch_0!=of::Dispatch::gather_loot){
    error="ObjectiveList::Register requires an unwired non-GatherLoot Objective provider";return false;
   }
   if(!register_gather_loot_objective(objective->ref.action.identity,inventory,
                                      script_context,start_script,error))return false;
  }
 }
 error.clear();return true;
}
bool Owner::transition_combat_objective_registration(
 std::uintptr_t quest_identity,std::int32_t prior_state,
 std::int32_t stored_state,void* script_context,
 KillObjectiveStartScript start_script,std::string& error){
 if(impl_->busy||impl_->closed){error="Native Quest owner unavailable for combat ObjectiveList transition";return false;}
 auto* quest=impl_->resolve_quest_identity(quest_identity);
 if(!quest||quest->record().state_0!=stored_state||stored_state<0||stored_state>13){
  error="Combat ObjectiveList transition requires the already-stored canonical Quest state";return false;
 }
 if(prior_state==stored_state&&!quest->record().byte_64){error.clear();return true;}
 if(prior_state==6&&stored_state==6){
  error="ObjectiveList volatile state-6 self-transition needs source delayed-detach/refcount reattach semantics";return false;
 }
 auto& list=quest->objectives();
 if(list.count_0<0||(list.count_0>0&&(!list.children_4||
    std::uint64_t(std::uint32_t(list.count_0))>list.children_4->slots.size()))){
  error="Combat ObjectiveList transition found invalid canonical list backing";return false;
 }
 for(std::uint32_t ordinal=0;ordinal<std::uint32_t(list.count_0);++ordinal){
  auto* objective=impl_->resolve_objective(list.children_4->slots[ordinal]);
  if(!objective){error="Combat ObjectiveList transition could not resolve its canonical Objective";return false;}
  const auto identity=objective->ref.action.identity;
  const bool supported=objective->dispatch_0==of::Dispatch::kill_enemies||
      objective->dispatch_0==of::Dispatch::clear_enemies||
      objective->dispatch_0==of::Dispatch::kill_enemy_template||
      objective->dispatch_0==of::Dispatch::clear_enemy_template;
  if(!supported){
   if(objective->compiled_8){
    error="ObjectiveList Register/Unregister reached an unbound compiled Objective kind";return false;
   }
   continue;
  }
  const auto binding=impl_->kill_x_enemies.find(identity);
  if(prior_state==6&&stored_state!=6){
   if(binding==impl_->kill_x_enemies.end())continue;
   if(binding->second->expected_dispatch!=objective->dispatch_0){
    error="Combat Objective receiver dispatch differs from its canonical Objective";return false;
   }
   bool ok=false;
   switch(objective->dispatch_0){
    case of::Dispatch::kill_enemies:
     ok=unregister_kill_x_enemies_objective(quest_identity,ordinal,error);break;
    case of::Dispatch::clear_enemies:
     ok=unregister_clear_enemies_objective(quest_identity,ordinal,error);break;
    case of::Dispatch::kill_enemy_template:
     ok=unregister_kill_enemy_template_objective(quest_identity,ordinal,error);break;
    case of::Dispatch::clear_enemy_template:
     ok=unregister_clear_enemy_template_objective(quest_identity,ordinal,error);break;
    default:break;
   }
   if(!ok)return false;
  }else if(stored_state==6&&objective->compiled_8){
   bool ok=false;
   switch(objective->dispatch_0){
    case of::Dispatch::kill_enemies:
     ok=register_kill_x_enemies_objective(quest_identity,ordinal,
                                            script_context,start_script,error);break;
    case of::Dispatch::clear_enemies:
     ok=register_clear_enemies_objective(quest_identity,ordinal,
                                          script_context,start_script,error);break;
    case of::Dispatch::kill_enemy_template:
     ok=register_kill_enemy_template_objective(quest_identity,ordinal,
                                                 script_context,start_script,error);break;
    case of::Dispatch::clear_enemy_template:
     ok=register_clear_enemy_template_objective(quest_identity,ordinal,
                                                 script_context,start_script,error);break;
    default:break;
   }
   if(!ok)return false;
  }
 }
 error.clear();return true;
}
bool Owner::close(std::string& error){
 if(impl_->closed){error.clear();return true;}
 if(impl_->busy){error="Native Quest owner is executing";return false;}
 impl_->busy=true;struct Guard{Impl& owner;~Guard(){owner.busy=false;}}guard{*impl_};error.clear();
 while(!impl_->kill_x_enemies.empty()){
  const auto found=impl_->kill_x_enemies.begin();const auto identity=found->first;
  if(found->second->attached){bool detached=false;
   if(impl_->current_level_events.detach(found->second->event_type,identity,detached,error)!=LevelEventStatus::complete)return false;
   if(!detached){error="KillXEnemies receiver disappeared during Owner close";return false;}
   found->second->attached=false;
  }
  impl_->kill_x_enemies.erase(found);
 }
 while(!impl_->gather_loot.empty()){
  const auto found=impl_->gather_loot.begin();const auto identity=found->first;
  // Close is the Objective teardown boundary. Retire each receiver and its
  // inventory list-30 reference before the Objective factory destroys it.
  auto& binding=*found->second;
  if(binding.attached){bool detached=false;
   if(impl_->current_level_events.detach(binding.event_type,identity,detached,error)!=LevelEventStatus::complete)return false;
   if(!detached){error="GatherLoot EventManager receiver disappeared during Owner close";return false;}
   binding.attached=false;
  }
  if(binding.inventory_registered){
   if(!static_cast<data::FreshInventoryOwnedV4*>(binding.quantity_context)->unregister_quest_gathering_item_id(binding.item_id,error))return false;
   binding.inventory_registered=false;
  }
  impl_->gather_loot.erase(found);
 }
 while(!impl_->talk_to_npc.empty()){
  const auto found=impl_->talk_to_npc.begin();const auto identity=found->first;
  auto& binding=*found->second;
  if(binding.attached){bool detached=false;
   if(impl_->current_level_events.detach(binding.event_type,identity,detached,error)!=LevelEventStatus::complete)return false;
   if(!detached){error="TalkToNPC EventManager receiver disappeared during Owner close";return false;}
   binding.attached=false;
  }
  if(binding.target_marked){
   if(!binding.marker||!binding.marker(binding.marker_context,
       binding.objective->ref.action.identity,&binding.objective->derived_28,
       binding.target_props_id,
       TalkObjectiveMarkerOperationV1::unregister_target,0,0,error)){
    if(error.empty())error="TalkToNPC target marker clear failed during Owner close";
    return false;
   }
   binding.target_marked=false;
  }
  impl_->talk_to_npc.erase(found);
 }
 while(!impl_->move_in_zone.empty()){
  const auto found=impl_->move_in_zone.begin();const auto identity=found->first;
  auto& binding=*found->second;
  if(binding.attached){bool detached=false;
   if(impl_->current_level_events.detach(binding.event_type,identity,detached,error)!=LevelEventStatus::complete)return false;
   if(!detached){error="MoveInZone EventManager receiver disappeared during Owner close";return false;}
   binding.attached=false;
  }
  impl_->move_in_zone.erase(found);
 }
 while(!impl_->open_game_object.empty()){
  const auto found=impl_->open_game_object.begin();const auto identity=found->first;
  auto& binding=*found->second;
  if(binding.objective_binding.registered){bool detached=false;
   if(impl_->current_level_events.detach(binding.objective_binding.event_type,
       identity,detached,error)!=LevelEventStatus::complete)return false;
   if(!detached){error="OpenGameObject EventManager receiver disappeared during Owner close";return false;}
   binding.objective_binding.registered=false;
  }
  impl_->open_game_object.erase(found);
 }
 // These thin source-runtime adapters borrow the same Instances. Retire them
 // before the canonical Quest factory records are destroyed below.
 impl_->compile_runtimes.clear();
 if(!impl_->run(0,true,error)||!impl_->run(1,true,error))return false;
 // InitQuests publishes only after all row providers return. A failed row can
 // therefore own constructed children outside the Save vectors. Close their
 // genuine D1 bodies while all factory/array leases still belong to this context.
 for(auto at=impl_->slots.begin();at!=impl_->slots.end();){
  auto& slot=*at->second;
  if(slot.value&&slot.constructed){
   scalar::Result result;
   if(slot.value->destroy(&result)!=scalar::Status::complete){
    error="Native unpublished Quest cleanup failed at operation "+std::to_string(unsigned(result.last_operation));return false;
   }
   ++impl_->result.unpublished_destroyed;
  }
  at=impl_->slots.erase(at);
 }
 impl_->current_level_events.clear();
 impl_->closed=true;return true;
}
bool Owner::owns_save(const data::PlayerSavegameV1* save) const noexcept{return impl_->save.get()==save;}
const Receipt& Owner::receipt() const noexcept{return impl_->result;}
scalar::Record* Owner::resolve(const logs::QuestRef* ref) noexcept{auto* value=impl_->resolve(ref);return value?&value->record():nullptr;}
}
