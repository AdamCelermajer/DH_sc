#include "../quest_runtime_fields_v1.hpp"
#include "../player_saved_quests_v1.hpp"
#include <array>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace dh2::data::quest_runtime_fields_v1;
namespace {
constexpr std::uintptr_t QUEST=0x10000100,ROW1=0x10003000,ROW2=0x10004000,ACTION1=0x10006000,ACTION2=0x10006100;
using Input=std::array<std::int64_t,8>;
using Event=std::array<std::int64_t,7>;
unsigned checks=0;void check(bool value){if(!value)throw std::runtime_error("quest field guard "+std::to_string(checks+1));++checks;}
struct Fixture {
 Record q{QUEST};PyDataRef rows[2]{{ROW1},{ROW2}};
 std::uintptr_t owners[2]{0xcccccccc,0xcccccccc};ActionRef actions[2]{{ACTION1,&owners[0]},{ACTION2,&owners[1]}};
 std::array<std::array<std::uint32_t,71>,2> data{};
 std::array<std::uint32_t,15> list_words{};
 dh2::data::player_saved_quests_v1::StreamRef stream{0x10008000};
 std::uint32_t cursor=0,volatile_result=0;
 std::vector<Event> trace;Input input{};Runtime* runtime=nullptr;Status nested=Status::complete;
 void setup(const Input& in){
  input=in;list_words.fill(0xcccccccc);q.state_0=in[0]==4?-858993460:std::int32_t(in[1]);q.word_4=-858993460;q.fields.row_8=-858993460;q.word_c=-858993460;
  q.difficulty_10=std::int32_t(in[2]);q.fields.definition_name_14=0xcccccccc;q.fields.character_60=std::uintptr_t(in[4]);
  q.action_18=(in[3]&1)?&actions[0]:nullptr;q.action_1c=(in[3]&2)?&actions[1]:nullptr;
  q.byte_5c=q.byte_5d=q.byte_64=0xcc;q.py_data_68=&rows[0];
  for(unsigned n=0;n<2;++n){auto& d=data[n];for(unsigned i=0;i<d.size();++i)d[i]=0x70000000u+n*0x1000+i;
   for(unsigned i:{0x14u,0x1cu,0x24u,0x2cu,0x34u})d[i/4]=n?std::uint32_t(-2):3;
   d[0x9c/4]=n?UINT32_MAX:6;d[0x118/4]=n?UINT32_MAX:41;
  }
 }
 Services services(){return {this,[](void* raw,const Request& r,Response* out)->std::int32_t{
  auto& f=*static_cast<Fixture*>(raw);
  f.trace.push_back({std::int64_t(r.operation),std::int64_t(r.target),std::int64_t(r.value),r.count,std::int64_t(r.row?r.row->identity:0),r.offset,std::int64_t(r.stream?r.stream->identity:0)});
  if(f.input[5]==9&&r.operation==Operation::read_stream_word){f.q.state_0=std::int32_t((std::uint32_t(f.q.state_0)&0xffff0000u)|(std::uint32_t(f.input[1])&0xffffu));f.cursor+=2;}
  if(f.input[6]>0&&f.trace.size()==std::size_t(f.input[6])){if(f.input[7])throw std::runtime_error("provider");return 1;}
  switch(r.operation){
   case Operation::construct_conditions:for(unsigned i=0;i<3;++i)f.list_words[i]=0;break;
   case Operation::construct_objectives:for(unsigned i=3;i<6;++i)f.list_words[i]=0;break;
   case Operation::construct_rewards:f.list_words[6]=f.list_words[7]=f.list_words[14]=0;f.list_words[8]&=0xffffff00u;f.list_words[12]=f.list_words[13]=QUEST+0x40;break;
   case Operation::read_py_word:{const auto i=r.row==&f.rows[0]?0:1;out->value=f.data[i][r.offset/4];break;}
   case Operation::create_objective:out->action=r.offset==0x3c?&f.actions[0]:&f.actions[1];break;
   case Operation::read_stream_word:check(r.destination==&f.q.state_0&&r.stream==&f.stream);*static_cast<std::int32_t*>(r.destination)=std::int32_t(f.input[1]);f.cursor+=4;break;
   case Operation::action_virtual:if(r.offset==0x28){check(r.stream==&f.stream);f.cursor+=3;}break;
   case Operation::load_objectives:check(r.stream==&f.stream);f.cursor+=2;break;
   default:break;
  }
  if(f.input[5]==1&&r.operation==Operation::objective_owners)f.q.fields.character_60=std::uint32_t(f.q.fields.character_60+7);
  if(f.input[5]==2&&r.operation==Operation::assign_conditions)f.q.py_data_68=&f.rows[1];
  if(f.input[5]==3&&r.operation==Operation::assign_objectives)f.q.difficulty_10=2;
  if(f.input[5]==4&&r.operation==Operation::create_objective&&r.offset==0x3c)f.q.py_data_68=&f.rows[1];
  if(f.input[5]==5&&(r.operation==Operation::action_virtual||r.operation==Operation::remove_markers||r.operation==Operation::unregister_objectives))f.q.py_data_68=&f.rows[1];
  if(f.input[5]==6&&r.operation==Operation::construct_objectives)f.q.fields.character_60=std::uint32_t(f.q.fields.character_60+9);
  if(f.input[5]==7&&r.operation==Operation::objective_owners){Result nested;f.nested=f.runtime->owner_children(&nested);}
  if(f.input[5]==8)f.q.ref.identity=QUEST+0x100;
  if(f.input[5]==10&&r.operation==Operation::load_objectives)f.q.state_0=3;
  if(f.input[5]==11&&r.operation==Operation::action_virtual&&r.offset==4)f.q.action_1c=nullptr;
  if(f.input[5]==12&&r.operation==Operation::action_virtual&&r.offset==4)f.q.action_1c=&f.actions[0];
  return 0;
 }};}
 Status run(Result& result){Runtime r(q,services());runtime=&r;switch(input[0]){
  case 0:return r.construct(std::int32_t(input[2]),&result);
  case 1:return r.owner_children(&result);
  case 2:return r.assign_pydata(rows[0],&result);
  case 4:return r.load_quest_data(stream,std::uint32_t(input[2]),&result);
  case 5:volatile_result=is_volatile_state(std::int32_t(input[1]));return Status::complete;
  case 6:case 7:return r.destroy(&result);
  default:return r.reinit(&result);
 }}
 std::vector<std::int64_t> projection()const {
  std::vector<std::int64_t> result{q.state_0,q.word_4,q.fields.row_8,q.word_c,q.difficulty_10,std::int64_t(q.fields.definition_name_14),
   std::int64_t(q.action_18?q.action_18->identity:0),std::int64_t(q.action_1c?q.action_1c->identity:0)};
  for(auto word:list_words)result.push_back(std::int64_t(word));
  for(auto value:{std::int64_t(q.byte_5c),std::int64_t(q.byte_5d),std::int64_t(q.fields.character_60),std::int64_t(q.byte_64),std::int64_t(q.py_data_68?q.py_data_68->identity:0),std::int64_t(owners[0]),std::int64_t(owners[1])})result.push_back(value);
  result.push_back(cursor);result.push_back(volatile_result);
  return result;
 }
};
template<class Values>void json_array(const Values& values){std::cout<<'[';bool first=true;for(const auto& value:values){if(!first)std::cout<<',';first=false;std::cout<<value;}std::cout<<']';}
void guards(){
 const Input in{1,2,1,3,123,0,0,0};
 {Fixture f;f.setup(in);Runtime r(f.q,{});Result out;check(r.owner_children(&out)==Status::service_unavailable);check(f.q.fields.character_60==123&&f.owners[0]==0xcccccccc);}
 {Fixture f;f.setup(in);Runtime r(f.q,f.services());check(r.owner_children(nullptr)==Status::invalid_argument);check(f.trace.empty());}
 {Fixture f;f.setup(in);Runtime r(f.q,f.services());auto* bad=reinterpret_cast<Result*>(&f.q);check(r.owner_children(bad)==Status::invalid_argument);check(f.trace.empty());}
 {Fixture f;f.setup(in);Runtime r(f.q,f.services());Result out;f.q.ref.fields=nullptr;check(r.owner_children(&out)==Status::projection_changed);check(f.trace.empty());}
 {Fixture f;auto row=in;row[5]=7;f.setup(row);Result out;check(f.run(out)==Status::complete&&f.nested==Status::reentrant);check(f.trace.size()==2);}
 {Fixture f;auto row=in;row[5]=8;f.setup(row);Result out;check(f.run(out)==Status::projection_changed&&f.trace.size()==1);check(f.owners[0]==0xcccccccc);}
 {Fixture f;f.setup(in);f.actions[0].character_10=nullptr;Result out;check(f.run(out)==Status::source_fault&&f.trace.size()==2);check(f.owners[1]==0xcccccccc);}
 {Fixture f;f.setup(in);f.actions[1].character_10=reinterpret_cast<std::uintptr_t*>(&f.q);Result out;check(f.run(out)==Status::source_fault);check(f.owners[0]==123&&f.q.state_0==2);}
 {Fixture f;auto row=in;row[0]=3;f.setup(row);f.q.action_18=nullptr;Result out;check(f.run(out)==Status::source_fault&&f.trace.empty());check(f.q.state_0==2);}
 {Fixture f;auto row=in;row[0]=3;row[1]=0;f.setup(row);f.q.py_data_68=nullptr;Result out;check(f.run(out)==Status::source_fault&&f.trace.empty());}
 {Fixture f;f.setup(in);Runtime r(f.q,f.services());Result out;PyDataRef bad{};check(r.assign_pydata(bad,&out)==Status::invalid_argument);check(f.q.py_data_68==&f.rows[0]&&f.trace.empty());}
 {Fixture f;f.setup(in);Runtime r(f.q,f.services());auto* bad=reinterpret_cast<Result*>(f.actions[0].character_10);check(r.owner_children(bad)==Status::invalid_argument);check(f.trace.empty());}
 {Fixture f;f.setup(in);f.q.fields.character_60=UINT64_C(0x12345678000000a1);Result out;check(f.run(out)==Status::complete);check(f.owners[0]==f.q.fields.character_60&&f.owners[1]==f.q.fields.character_60);}
 {Fixture f;auto row=in;row[0]=4;f.setup(row);f.q.action_18=nullptr;Result out;check(f.run(out)==Status::source_fault);check(f.q.state_0==2&&f.cursor==4&&f.trace.size()==1);}
 {Fixture f;auto row=in;row[0]=4;f.setup(row);f.q.action_1c=nullptr;Result out;check(f.run(out)==Status::source_fault);check(f.q.state_0==2&&f.cursor==7&&f.trace.size()==2);}
 {Fixture f;auto row=in;row[0]=4;f.setup(row);f.stream.identity=0;Result out;check(f.run(out)==Status::invalid_argument&&f.trace.empty()&&f.cursor==0);}
}
}
int main(int argc,char** argv){try{
 check(argc==2);std::ifstream input(argv[1]);check(bool(input));std::cout<<"{\"results\":[";bool first=true;Input row;
 while(input>>row[0]){for(unsigned i=1;i<row.size();++i)check(bool(input>>row[i]));Fixture f;f.setup(row);Result result;const auto status=f.run(result);if(!first)std::cout<<',';first=false;
  std::cout<<"{\"status\":"<<std::uint32_t(status)<<",\"projection\":";json_array(f.projection());std::cout<<",\"trace\":[";
  bool event_first=true;for(const auto& event:f.trace){if(!event_first)std::cout<<',';event_first=false;json_array(event);}std::cout<<"]}";
 }
 const auto prior=checks;guards();std::cout<<"],\"native_guard_checks\":"<<checks-prior<<"}\n";return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
