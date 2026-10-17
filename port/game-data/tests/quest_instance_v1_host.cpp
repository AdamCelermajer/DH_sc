#include "../quest_instance_v1.hpp"
#include "../quest_condition_factory_v1.hpp"
#include <cstdio>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <vector>
namespace d=dh2::data;
namespace q=d::quest_instance_v1;
namespace scalar=d::quest_runtime_fields_v1;
namespace c=d::quest_condition_list_v1;
namespace cf=d::quest_condition_factory_v1;
namespace o=d::quest_objective_list_v1;
namespace r=d::quest_reward_list_v1;
namespace tables=d::quest_table_bindings_v1;
unsigned checks=0;
void check(bool value){if(!value)throw std::runtime_error("Quest composition check "+std::to_string(checks+1));++checks;}
template<class T> void erase(std::vector<std::unique_ptr<T>>& owner,T* pointer){
 for(auto i=owner.begin();i!=owner.end();++i)if(i->get()==pointer){owner.erase(i);return;}
 throw std::runtime_error("Real fixture allocation missing");
}
struct Objective {
 o::ObjectiveFields fields;o::ObjectiveRef ref{{reinterpret_cast<std::uintptr_t>(this),&fields.character_10},&fields};
};
struct Reward {r::RewardFields fields;r::RewardRef ref{reinterpret_cast<std::uintptr_t>(this),&fields};};
// Source condition factories are selected production bodies. Objective/reward
// factory and virtual gameplay leaves are explicit observing fixtures here.
// This verifies composition/ownership, not those missing native implementations.
struct Arena {
 std::vector<std::unique_ptr<cf::Record>> conditions;
 std::vector<std::unique_ptr<Objective>> objectives;
 std::vector<std::unique_ptr<Reward>> rewards;
 std::vector<std::unique_ptr<c::Array>> ca;
 std::vector<std::unique_ptr<o::Array>> oa;
 std::vector<std::unique_ptr<r::Array>> ra;
 bool fail_objective=false,fail_action_delete=false;
 unsigned objective_calls=0,action_loads=0,objective_loads=0;
 std::vector<unsigned> deletes;
 d::player_saved_quests_v1::StreamRef stream{reinterpret_cast<std::uintptr_t>(this)};
 cf::Runtime condition_factory{{this,
  [](void* raw,std::uint32_t bytes,std::uint32_t tag,cf::Record** out)->std::int32_t {
   auto& a=*static_cast<Arena*>(raw);if(tag||!(bytes==8||bytes==12))return 1;
   auto record=std::make_unique<cf::Record>(1);record->ref.identity=reinterpret_cast<std::uintptr_t>(record.get());
   *out=record.get();a.conditions.push_back(std::move(record));return 0;
  },[](void* raw,cf::Record* record)->std::int32_t {erase(static_cast<Arena*>(raw)->conditions,record);return 0;}}};
 template<class Array>static std::int32_t allocate(std::vector<std::unique_ptr<Array>>& arena,std::uint32_t bytes,std::uint32_t tag,Array** out){
  if(tag||bytes%4)return 1;
  auto array=std::make_unique<Array>();array->identity=reinterpret_cast<std::uintptr_t>(array.get());
  array->slots.resize(bytes/4,nullptr);*out=array.get();arena.push_back(std::move(array));return 0;
 }
 Objective* objective(o::ObjectiveRef* ref){for(auto& record:objectives)if(&record->ref==ref)return record.get();throw std::runtime_error("Actual fixture objective missing");}
 Objective* action(std::uintptr_t identity){for(auto& record:objectives)if(record->ref.action.identity==identity)return record.get();throw std::runtime_error("Actual fixture action missing");}
 q::Services services(){
  q::Services s;
  s.conditions={this,
   [](void* raw,c::List&,std::uint32_t bytes,std::uint32_t tag,c::Array** out){return allocate(static_cast<Arena*>(raw)->ca,bytes,tag,out);},
   [](void* raw,c::List&,std::int32_t type,c::ConditionRef** out)->std::int32_t{auto& a=*static_cast<Arena*>(raw);cf::Result result;if(a.condition_factory.create(type,&result)!=cf::Status::complete)return 1;*out=&result.record->ref;return 0;},
   [](void* raw,c::List&,c::ConditionRef* ref)->std::int32_t{auto& a=*static_cast<Arena*>(raw);for(auto& record:a.conditions)if(&record->ref==ref){cf::Result result;a.deletes.push_back(3);return a.condition_factory.destroy(*record,true,&result)==cf::Status::complete?0:1;}return 1;},
   [](void* raw,c::List&,c::Array* array)->std::int32_t{erase(static_cast<Arena*>(raw)->ca,array);return 0;},nullptr};
  s.objectives={this,
   [](void* raw,o::List&,std::uint32_t bytes,std::uint32_t tag,o::Array** out){return allocate(static_cast<Arena*>(raw)->oa,bytes,tag,out);},
   [](void* raw,o::List&,const o::Definition&,std::int32_t,o::ObjectiveRef** out)->std::int32_t{auto& a=*static_cast<Arena*>(raw);++a.objective_calls;if(a.fail_objective)return 1;auto record=std::make_unique<Objective>();*out=&record->ref;a.objectives.push_back(std::move(record));return 0;},
   [](void* raw,o::List&,const o::StreamCall& call)->std::int32_t{auto& a=*static_cast<Arena*>(raw);if(call.stream!=&a.stream||call.function!=0x28||!call.virtual_call||call.encoded_adjustment!=1||call.adjusted_target!=call.objective->action.identity)return 1;++a.objective_loads;return 0;},
   [](void* raw,o::List&,o::ObjectiveRef* ref)->std::int32_t{auto& a=*static_cast<Arena*>(raw);a.deletes.push_back(2);erase(a.objectives,a.objective(ref));return 0;},
   [](void* raw,o::List&,o::Array* array)->std::int32_t{erase(static_cast<Arena*>(raw)->oa,array);return 0;}};
  s.rewards={this,
   [](void*,r::List& list)->std::int32_t{list.text_8.reserve(15);return 0;},
   [](void*,r::List& list)->std::int32_t{list.text_8.clear();return 0;},
   [](void* raw,r::List&,std::uint32_t bytes,std::uint32_t tag,r::Array** out){return allocate(static_cast<Arena*>(raw)->ra,bytes,tag,out);},
   [](void* raw,r::List&,std::int32_t,r::RewardRef** out)->std::int32_t{auto& a=*static_cast<Arena*>(raw);auto record=std::make_unique<Reward>();*out=&record->ref;a.rewards.push_back(std::move(record));return 0;},
   [](void* raw,r::List&,r::RewardRef* ref)->std::int32_t{auto& a=*static_cast<Arena*>(raw);for(auto& record:a.rewards)if(&record->ref==ref){a.deletes.push_back(1);erase(a.rewards,record.get());return 0;}return 1;},
   [](void* raw,r::List&,r::Array* array)->std::int32_t{erase(static_cast<Arena*>(raw)->ra,array);return 0;},
   [](void*,r::List& list)->std::int32_t{std::string().swap(list.text_8);return 0;}};
  s.leaves={this,[](void* raw,const scalar::Request& request,scalar::Response*)->std::int32_t{
   auto& a=*static_cast<Arena*>(raw);
   if(request.operation==scalar::Operation::read_stream_word){if(request.stream!=&a.stream||request.destination!=&request.quest->state_0)return 1;*static_cast<std::int32_t*>(request.destination)=3;return 0;}
   if(request.operation!=scalar::Operation::action_virtual)return 1;
   if(request.offset==0x28){if(request.stream!=&a.stream)return 1;(void)a.action(request.target);++a.action_loads;return 0;}
   if(request.offset!=4||a.fail_action_delete)return 1;
   a.deletes.push_back(0);erase(a.objectives,a.action(request.target));return 0;
  }};
  return s;
 }
 bool empty() const{return conditions.empty()&&objectives.empty()&&rewards.empty()&&ca.empty()&&oa.empty()&&ra.empty();}
};
std::shared_ptr<const std::vector<std::uint8_t>> bytes(const std::string& path){
 std::ifstream file(path,std::ios::binary|std::ios::ate);if(!file)throw std::runtime_error("Original cache input missing");
 auto data=std::make_shared<std::vector<std::uint8_t>>(std::size_t(file.tellg()));file.seekg(0);file.read(reinterpret_cast<char*>(data->data()),std::streamsize(data->size()));check(bool(file));return data;
}
tables::View definitions(const std::string& cache){
 const auto packed=bytes(cache+"/v2quests_pyarray.bin"),names=bytes(cache+"/v2quests_pyarraynames.bin");
 tables::Input input;check(dh2_quests_open(&input.table,packed->data(),std::uint32_t(packed->size()))==0);
 input.packed_owner=packed;input.names=names->data();input.names_size=names->size();input.names_owner=names;
 tables::Owner owner;std::string error;check(owner.load(input,error));return owner.borrow();
}
int main(int argc,char** argv){try{
 check(argc==2);const auto view=definitions(argv[1]);check(view.count()==64);unsigned instances=0;
 for(std::uint32_t row=0;row<view.count();++row)for(int difficulty=0;difficulty<3;++difficulty){
  Arena arena;q::Instance instance(reinterpret_cast<std::uintptr_t>(&arena),view,arena.services());scalar::Result result;
  auto& record=instance.record();record.fields.character_60=UINT64_C(0x12345678000000a1);
  check(instance.construct(difficulty,&result)==scalar::Status::complete);check(record.fields.character_60==UINT64_C(0x12345678000000a1));
  check(instance.owner_children(&result)==scalar::Status::complete);
  check(instance.assign_pydata(view.row(row)->identity,&result)==scalar::Status::complete);
  const auto* definition=view.record(*view.row(row));check(record.py_data_68==view.row(row)&&record.word_c==definition->act);
  check(instance.conditions().count_0==std::int32_t(definition->lists[0].count));
  check(instance.objectives().count_0==std::int32_t(definition->lists[1].count));
  check(instance.rewards().count_0==std::int32_t(definition->lists[2+difficulty].count));
  for(auto& child:arena.conditions)check(child->fields.py_data_4.view.record(*child->fields.py_data_4.list->row)==definition);
  for(auto& child:arena.objectives)check(child->fields.character_10==record.fields.character_60);
  for(auto& child:arena.rewards)check(child->fields.character_10==record.fields.character_60);
  check(instance.reinit(&result)==scalar::Status::complete&&record.state_0==definition->state);
  check(instance.load_quest_data(arena.stream,255,&result)==scalar::Status::complete&&record.state_0==3&&record.byte_64==0);
  check(arena.action_loads==2&&arena.objective_loads==definition->lists[1].count);
  if(instance.conditions().children_4)check(instance.construct(0,reinterpret_cast<scalar::Result*>(instance.conditions().children_4))==scalar::Status::invalid_argument);
  check(instance.destroy(&result)==scalar::Status::complete&&arena.empty());
  check(record.action_18==nullptr&&record.action_1c==nullptr);
  check(arena.deletes.size()>=2&&arena.deletes[0]==0&&arena.deletes[1]==0);
  for(std::size_t i=3;i<arena.deletes.size();++i)check(arena.deletes[i]>=arena.deletes[i-1]);
  ++instances;
 }
 // Failed actual factory delivery preserves constructed lists and PyData/count
 // stores. No Quest/actions/reward objects are invented to finish assignment.
 {Arena arena;arena.fail_objective=true;q::Instance instance(reinterpret_cast<std::uintptr_t>(&arena),view,arena.services());scalar::Result result;
  check(instance.construct(0,&result)==scalar::Status::complete);
  check(instance.assign_pydata(view.row(0)->identity,&result)==scalar::Status::service_failed);
  check(result.last_operation==scalar::Operation::assign_objectives&&instance.record().py_data_68==view.row(0));
  check(instance.objectives().count_0>0&&instance.record().action_18==nullptr&&instance.rewards().count_0==0);
  check(instance.destroy(&result)==scalar::Status::complete&&arena.empty());}
 {Arena arena;q::Instance instance(reinterpret_cast<std::uintptr_t>(&arena),view,arena.services());scalar::Result result;
  check(instance.construct(0,&result)==scalar::Status::complete);check(instance.assign_pydata(view.row(0)->identity,&result)==scalar::Status::complete);
  const auto* action=instance.record().action_18;arena.fail_action_delete=true;
  check(instance.destroy(&result)==scalar::Status::service_failed&&instance.record().action_18==action&&instance.record().action_1c);
  arena.fail_action_delete=false;check(instance.destroy(&result)==scalar::Status::complete&&arena.empty());}
 std::printf("{\"validation\":\"PASS\",\"checks\":%u,\"instances\":%u,\"same_record_and_child_stores\":true,\"native_wired\":false,\"factory_scope\":\"selected condition bodies; objective/reward factories and virtual gameplay leaves are observing fixtures\"}\n",checks,instances);return 0;
 }catch(const std::exception& e){std::fprintf(stderr,"%s\n",e.what());return 1;}}
