#include "../quest_stream_read_v1.hpp"
#include "../quest_runtime_fields_v1.hpp"
#include <algorithm>
#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <vector>
using namespace dh2::data::quest_stream_read_v1;
namespace {
unsigned checks=0;
void require(bool ok){++checks;if(!ok)throw std::runtime_error("direct stream reader check "+std::to_string(checks));}
struct Fixture {
 StreamRef stream{0x10020000};std::uint32_t word=0xa5a5a5a5;
 std::vector<long long> args;std::vector<std::uint8_t> bytes;
 std::vector<std::array<long long,4>> trace;std::size_t cursor=0;void* live_destination=nullptr;
 std::unique_ptr<Runtime> runtime;bool reenter=false;Status nested=Status::complete;
 explicit Fixture(std::vector<long long> input):args(std::move(input)){
  word=std::uint32_t(args[1]);for(unsigned i=0;i<4;++i)bytes.push_back(std::uint8_t(std::uint32_t(args[2])>>(i*8)));
  bytes.resize(std::min<std::size_t>(args[3],4));runtime=std::make_unique<Runtime>(stream,services());
 }
 int event(unsigned operation,long long a=0,long long b=0,long long c=0){
  trace.push_back({operation,a,b,c});
  if(reenter){reenter=false;Result out;nested=runtime->read_unsigned(&word,&out);}
  if(operation==unsigned(args[9])){const auto value=std::uint32_t(args[10]);std::memcpy(live_destination,&value,4);}
  if(operation==unsigned(args[7])){if(args[8])throw std::runtime_error("declared read/assert/log failure");return 1;}
  return 0;
 }
 Services services(){Services s;s.context=this;
  s.read=[](void* context,StreamRef& stream,void* destination,std::uint64_t requested,std::uint64_t* returned){
   auto& f=*static_cast<Fixture*>(context);if(&stream!=&f.stream||!destination||!returned||requested!=4)return 1;
   f.live_destination=destination;const auto count=std::min<std::size_t>(4,f.bytes.size()-f.cursor);
   if(count)std::memcpy(destination,f.bytes.data()+f.cursor,count);
   f.cursor+=count;
   *returned=f.args[4]==-9?count:(std::uint64_t(std::uint32_t(f.args[5]))<<32)|std::uint32_t(f.args[4]);
   return f.event(0,4,std::uint32_t(*returned),std::uint32_t(*returned>>32));};
  s.assertion_mode=[](void* context,std::int32_t* out){auto& f=*static_cast<Fixture*>(context);*out=std::int32_t(f.args[6]);return f.event(1,f.args[0],*out);};
  s.log_assert=[](void* context,const AssertionRequest& request){auto& f=*static_cast<Fixture*>(context);const auto method=unsigned(f.args[0]);
   const auto source=method==0?0x313b48u:method==1?0x38b758u:0x459090u;
   const char* file=method==2?"..\\..\\project_vs2005\\Game/..\\..\\sources/Utils/StreamReader.h":"..\\..\\project_vs2005\\Game/..\\..\\sources/Utils/IStream.h";
   if(unsigned(request.primitive)!=method||request.source_function!=source||request.source_caller!=source+0x90||request.line!=(method==2?0x50u:0x45u)||
      std::strcmp(request.format,"ASSERT(%s) FAILED: %s:%d\n")||std::strcmp(request.condition,"bytesRead == sizeof(T)")||std::strcmp(request.file,file))return 1;
   return f.event(2,method,source,request.line);};return s;
 }
 Status run(Result* out){if(args[0]==0)return runtime->read_unsigned(&word,out);if(args[0]==1)return runtime->read_signed(reinterpret_cast<std::int32_t*>(&word),out);return runtime->read_quest_signed(reinterpret_cast<std::int32_t*>(&word),out);}
};
void output(const Fixture& f,const Result& r){
 std::cout<<"{\"status\":"<<unsigned(r.status)<<",\"operation\":"<<unsigned(r.last_operation)<<",\"destination\":"<<f.word<<",\"cursor\":"<<f.cursor<<",\"calls\":"<<r.service_calls<<",\"reads\":"<<r.read_calls<<",\"assertions\":"<<r.assertion_reads<<",\"logs\":"<<r.assertion_logs<<",\"source_function\":"<<r.source_function<<",\"source_caller\":"<<r.source_caller<<",\"requested\":"<<r.requested<<",\"returned\":"<<r.returned<<",\"trace\":[";
 bool comma=false;for(const auto& t:f.trace){if(comma)std::cout<<',';comma=true;std::cout<<'['<<t[0]<<','<<t[1]<<','<<t[2]<<','<<t[3]<<']';}std::cout<<"]}";
}
void guards(){const std::vector<long long> full{0,-1515870811,0x12345678,4,-9,0,0,-1,0,-1,55};
 for(unsigned missing=0;missing<3;++missing){auto row=full;row[3]=2;row[6]=1;Fixture f(row);auto s=f.services();if(missing==0)s.read=nullptr;if(missing==1)s.assertion_mode=nullptr;if(missing==2)s.log_assert=nullptr;Runtime r(f.stream,s);Result out;require(r.read_unsigned(&f.word,&out)==Status::service_unavailable);require(f.word==(missing?0xa5a55678u:0xa5a5a5a5u));require(f.cursor==(missing?2u:0u));}
 {Fixture f(full);auto s=f.services();s.assertion_mode=nullptr;s.log_assert=nullptr;Runtime r(f.stream,s);Result out;require(r.read_unsigned(&f.word,&out)==Status::complete&&f.word==0x12345678);}
 {Fixture f(full);f.reenter=true;Result out;require(f.run(&out)==Status::complete&&f.nested==Status::reentrant);}
 {Fixture f(full);f.stream.identity=0;Result out;require(f.run(&out)==Status::projection_changed&&f.trace.empty());}
 {Fixture f(full);require(f.run(nullptr)==Status::invalid_argument);require(f.run(reinterpret_cast<Result*>(&f.word))==Status::invalid_argument);require(f.run(reinterpret_cast<Result*>(&f.stream))==Status::invalid_argument);require(f.run(reinterpret_cast<Result*>(f.runtime.get()))==Status::invalid_argument);require(f.trace.empty()&&f.word==0xa5a5a5a5u);}
 {Fixture f(full);Result out;require(f.runtime->read_unsigned(nullptr,&out)==Status::invalid_argument);require(f.runtime->read_unsigned(reinterpret_cast<std::uint32_t*>(&f.stream),&out)==Status::invalid_argument);require(f.runtime->read_unsigned(reinterpret_cast<std::uint32_t*>(f.runtime.get()),&out)==Status::invalid_argument);require(f.runtime->read_unsigned(reinterpret_cast<std::uint32_t*>(&out),&out)==Status::invalid_argument);alignas(8)std::uint8_t bytes[16]{};require(f.runtime->read_unsigned(reinterpret_cast<std::uint32_t*>(bytes+1),&out)==Status::invalid_argument);require(f.trace.empty());}
 {Fixture f(full);auto s=f.services();s.read=[](void* context,StreamRef& stream,void* destination,std::uint64_t,std::uint64_t* out){auto& f=*static_cast<Fixture*>(context);const std::uint32_t word=99;std::memcpy(destination,&word,4);f.cursor=4;*out=4;stream.identity=44;return 0;};Runtime r(f.stream,s);Result out;require(r.read_unsigned(&f.word,&out)==Status::projection_changed&&f.word==99&&f.cursor==4&&f.stream.identity==44);}
 // Real selected Quest::_loadQuestData writes state0 through this reader. Its
 // following action provider intentionally fails; no empty action succeeds.
 namespace qf=dh2::data::quest_runtime_fields_v1;
 for(unsigned available:{0u,2u,4u}){auto row=full;row[0]=2;row[3]=available;row[6]=2;Fixture f(row);qf::Record quest{0x10010000};quest.state_0=std::int32_t(0xa5a5a5a5u);quest.byte_64=7;std::uintptr_t character=0;qf::ActionRef action{0x10030000,&character};quest.action_18=&action;unsigned action_calls=0;
  struct Context{Fixture* fixture;qf::Record* quest;unsigned* actions;} context{&f,&quest,&action_calls};qf::Services s;s.context=&context;
  s.invoke=[](void* raw,const qf::Request& request,qf::Response*){auto& c=*static_cast<Context*>(raw);if(request.operation==qf::Operation::read_stream_word){if(request.destination!=&c.quest->state_0||request.stream!=&c.fixture->stream)return 1;Result r;return c.fixture->runtime->read_quest_signed(static_cast<std::int32_t*>(request.destination),&r)==Status::complete?0:1;}if(request.operation==qf::Operation::action_virtual){++*c.actions;return 1;}return 1;};
  qf::Runtime loader(quest,s);qf::Result result;require(loader.load_quest_data(f.stream,255,&result)==qf::Status::service_failed);const std::uint32_t expected=available==0?0xa5a5a5a5u:available==2?0xa5a55678u:0x12345678u;require(std::uint32_t(quest.state_0)==expected&&f.cursor==available&&quest.byte_64==7);require(action_calls==(available==4?1u:0u));}
}
}
int main(int argc,char** argv){try{if(argc!=2)return 2;std::ifstream input(argv[1]);if(!input)return 2;std::string line;std::cout<<"{\"results\":[";bool comma=false;while(std::getline(input,line)){std::istringstream stream(line);std::vector<long long> args;long long value;while(stream>>value)args.push_back(value);if(args.size()!=11)throw std::runtime_error("direct reader input width");Fixture f(std::move(args));Result r;f.run(&r);if(comma)std::cout<<',';comma=true;output(f,r);}guards();std::cout<<"],\"native_checks\":"<<checks<<"}\n";return 0;}catch(const std::exception& e){std::cerr<<e.what();return 1;}}
