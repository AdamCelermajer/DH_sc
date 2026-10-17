#include "player_saved_fast_travel_v1.hpp"
#include <cstring>
#include <stdexcept>
namespace dh2::data::player_saved_fast_travel_v1 {namespace {
struct Range{std::uintptr_t begin,end;};
bool overlap(Range a,Range b){return a.begin<b.end&&b.begin<a.end;}
template<class T>bool range(const T* p,Range& r){const auto a=reinterpret_cast<std::uintptr_t>(p);if(!p||a%alignof(T)||a>UINTPTR_MAX-sizeof(T))return false;r={a,a+sizeof(T)};return true;}
struct Call {
 PlayerSavegameV1& save;Bytes bytes;Result& out;std::string& error;
 bool fail(const char* message){if(error.empty())error=message;return false;}
 void stage(Stage value,std::uint32_t caller){out.stage=value;out.source_caller=caller;}
 bool read(std::uint32_t length,const std::uint8_t*& p){++out.read_calls;if(out.consumed>bytes.size||length>bytes.size-out.consumed)return fail("Saved fast travel reached truncated source stream");p=bytes.data+out.consumed;out.consumed+=length;return true;}
 bool text(std::string& value){++out.string_reads;const std::uint8_t* p;if(!read(4,p))return false;const auto length=p[0]|std::uint32_t(p[1])<<8|std::uint32_t(p[2])<<16|std::uint32_t(p[3])<<24;if(!length||length>1048576)return fail("Saved fast travel source string assertion/budget boundary");if(!read(length,p))return false;if(p[length-1])return fail("Saved fast travel source string lacks terminator");value.assign(reinterpret_cast<const char*>(p),length-1);return true;}
 bool load(){
  for(std::uint32_t d=0;d<3;++d){
   out.difficulty=d;std::string value;stage(Stage::text,0x469b90);if(!text(value))return false;
   if(value.size()>64){out.decision=Decision::oversized_stopped;stage(Stage::complete,0x469cc4);return true;}
   std::array<std::uint32_t,2> words{};stage(Stage::decode,0x469c08);
   for(std::uint32_t bit=0;bit<value.size();++bit){const auto character=value[value.size()-1-bit];++out.characters;if(character=='1')words[bit>>5]|=std::uint32_t(1)<<(bit&31);else if(character!='0')return fail("Saved fast travel source invalid-bit throw boundary");}
   auto* const fields=save.source_fast_travel_bits(d);if(!fields)return fail("Actual source saved fast-travel bitset backing required");
   stage(Stage::publish,0x469c64);(*fields)[0]=words[0];++out.word_stores;
   stage(Stage::publish,0x469c6c);(*fields)[1]=words[1];++out.word_stores;++out.completed_difficulties;
  }
  out.decision=Decision::loaded;stage(Stage::complete,0x469c80);return true;
 }
};
}
Runtime::Runtime(PlayerSavegameV1* save):save_(save){Range r;if(!range(save_,r))throw std::invalid_argument("Actual borrowed fast-travel Save required");}
Status Runtime::load(Bytes bytes,Result* out,std::string& error){
 if(busy_)return Status::busy;
 Range s,r,e,t;if(!range(save_,s)||!range(out,r)||!range(&error,e)||!range(this,t)||overlap(r,e)||overlap(r,s)||overlap(e,s)||overlap(r,t)||overlap(e,t))return Status::invalid_argument;
 const auto p=reinterpret_cast<std::uintptr_t>(bytes.data);if((!bytes.data&&bytes.size)||bytes.size>67108864||p>UINTPTR_MAX-bytes.size)return Status::invalid_argument;
 if(bytes.size){const Range input{p,p+bytes.size};if(overlap(input,s)||overlap(input,r)||overlap(input,e)||overlap(input,t))return Status::invalid_argument;}
 *out={};error.clear();busy_=true;struct Guard{bool& busy;~Guard(){busy=false;}}guard{busy_};Call call{*save_,bytes,*out,error};
 try{return call.load()?Status::complete:Status::failed;}catch(...){if(error.empty())error="Saved fast-travel string allocation/provider threw";return Status::failed;}
}
}
