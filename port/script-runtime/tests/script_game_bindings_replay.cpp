#include "script_game_bindings.h"
#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>
namespace {
void check(bool v){if(!v)throw std::runtime_error("original callback replay mismatch");}
template<class T>T read(std::ifstream& in){T v{};in.read(reinterpret_cast<char*>(&v),sizeof(v));check(bool(in));return v;}
using Call=std::array<uint32_t,6>;
struct Fixture {int32_t id=0;std::vector<Call> calls;uintptr_t owner=0xabcdef0123456789ULL;};
int32_t start(void* p,uintptr_t owner,uint32_t duration,int32_t repeat,int32_t event,uintptr_t ref){
  auto& f=*static_cast<Fixture*>(p);check(owner==f.owner&&reinterpret_cast<uintptr_t>(p)>0xffffffffULL);
  f.calls.push_back({0,duration,uint32_t(repeat),uint32_t(event),1,uint32_t(ref)});return f.id;
}
void stop(void* p,uintptr_t owner,uint32_t id){auto& f=*static_cast<Fixture*>(p);check(owner==f.owner);f.calls.push_back({1,id,0,0,1,0});}
}
int main(int argc,char** argv){try{
  check(argc==2);std::ifstream in(argv[1],std::ios::binary);check(read<uint32_t>(in)==0x31424d47);
  auto cases=read<uint32_t>(in);Fixture fixture;dh2_script_game_bindings services{&fixture,fixture.owner,start,stop,0};
  unsigned requests=0;
  for(unsigned i=0;i<cases;++i){
    auto op=read<uint32_t>(in),count=read<uint32_t>(in);fixture.id=read<int32_t>(in);fixture.calls.clear();
    std::vector<dh2_script_value> args(count);
    for(auto& value:args){auto row=read<std::array<uint32_t,4>>(in);value.type=row[0];std::memcpy(&value.number,&row[1],4);value.boolean=row[2];
      if(value.type==4){value.text="0";value.text_bytes=row[3]?1:0;}
      if(value.type==2||value.type==7)value.identity=row[3]?0x123456789abcdef0ULL:0;
    }
    auto expected_count=read<uint32_t>(in),expected_word=read<uint32_t>(in),expected_calls=read<uint32_t>(in);
    std::vector<Call> expected(expected_calls);for(auto& call:expected)call=read<Call>(in);
    std::array<dh2_script_value,16> out{};uint32_t returned=0xffffffffu;char error[256]{};
    const std::array<dh2_script_function,3> functions{dh2_script_game_start_timer,dh2_script_game_stop_timer,dh2_script_game_trace};
    check(op<3&&functions[op](&services,args.data(),count,out.data(),out.size(),&returned,error,sizeof(error))==0);
    uint32_t word=0;if(returned){check(out[0].type==3);std::memcpy(&word,&out[0].number,4);}
    check(returned==expected_count&&word==expected_word&&fixture.calls==expected);requests+=expected_calls;
  }
  check(in.peek()==std::ifstream::traits_type::eof());
  std::cout<<"{\"validation\":\"PASS\",\"original_callback_cases\":"<<cases<<",\"ordered_timer_service_calls\":"<<requests<<",\"borrowed_identities_above_4GiB\":true,\"timer_services\":\"controlled original boundary fixture\",\"mismatches\":0}\n";
  return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<"\n";return 1;}}
