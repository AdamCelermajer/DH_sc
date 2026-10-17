#include "../quest_objective_factory_v1.hpp"
#include <algorithm>
#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace dh2::data::quest_objective_factory_v1;
namespace {
using Input=std::array<std::int64_t,5>;
struct Sink {
 std::int64_t fail_call=0,accepted_prefix=0;
 std::vector<std::uint8_t> bytes;
 std::vector<std::size_t> calls;
};
bool write(void* raw,dh2::data::Bytes data,std::string& error){
 auto& sink=*static_cast<Sink*>(raw);
 const auto call=std::int64_t(sink.calls.size()+1);sink.calls.push_back(data.size);
 const auto accepted=call==sink.fail_call
  ? std::min(data.size,std::size_t(std::max<std::int64_t>(0,sink.accepted_prefix)))
  : data.size;
 sink.bytes.insert(sink.bytes.end(),data.data,data.data+accepted);
 if(call==sink.fail_call){error="injected sink failure";return false;}
 return true;
}
template<class T>void values(const T& items){std::cout<<'[';bool comma=false;for(auto n:items){if(comma)std::cout<<',';comma=true;std::cout<<+n;}std::cout<<']';}
void run(const Input& in){
 Record q{0x10001000};q.dispatch_0=Dispatch(std::uint32_t(in[0]));
 q.fields.character_10=0x10001000;q.done_14=std::uint8_t(in[1]);
 q.quantity_20=std::uint32_t(in[2]);
 Sink sink{in[3],in[4]};dh2::data::player_save_section_writers_v1::WriteServicesV1 stream{&sink,write};
 Runtime runtime({});Result result{};std::string error;
 const auto status=runtime.save_data(q,stream,&result,error);
 std::cout<<"{\"status\":"<<unsigned(status)<<",\"calls\":";values(sink.calls);
 std::cout<<",\"bytes\":";values(sink.bytes);
 std::cout<<",\"service_calls\":"<<result.service_calls<<",\"error\":"<<(!error.empty()?1:0)<<"}";
}
}
int main(int argc,char** argv){try{
 if(argc!=2)throw std::runtime_error("expected case file");
 std::ifstream input(argv[1]);if(!input)throw std::runtime_error("cannot read cases");
 Input row{};std::cout<<"[";bool comma=false;
 while(input>>row[0]>>row[1]>>row[2]>>row[3]>>row[4]){if(comma)std::cout<<',';comma=true;run(row);}
 std::cout<<"]\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}return 0;
}
