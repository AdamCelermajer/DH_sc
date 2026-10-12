#include "game_option_table_v1.hpp"
#include <cstring>
#include <stdexcept>
namespace {
using namespace dh2::ui;
std::uint32_t word(const std::uint8_t* p){return std::uint32_t(p[0])|(std::uint32_t(p[1])<<8)|(std::uint32_t(p[2])<<16)|(std::uint32_t(p[3])<<24);}
bool overlap(const void* a,std::size_t an,const void* b,std::size_t bn){auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);return an&&bn&&(x<=y?y-x<an:x-y<bn);}
struct Reader {
 GameOptionBytesV1 data;std::size_t at{};
 explicit Reader(GameOptionBytesV1 d):data(d){if(!d.data||d.size>16u*1024u*1024u)throw std::runtime_error("GameOption input outside bounds");}
 void require(std::size_t n){if(n>data.size-at)throw std::runtime_error("Truncated GameOption input");}
 std::uint32_t integer(){require(4);auto result=word(data.data+at);at+=4;return result;}
 void fixed_rows(std::size_t stride){auto count=integer();if(count>65536)throw std::runtime_error("GameOption prefix count outside bounds");auto size=std::size_t(count)*stride;require(size);at+=size;}
 std::vector<std::string> names(){auto count=integer();if(count>65536)throw std::runtime_error("GameOption name count outside bounds");std::vector<std::string> result;result.reserve(count);while(count--){auto n=integer();if(n>1048576)throw std::runtime_error("GameOption name outside bounds");require(n);result.emplace_back(reinterpret_cast<const char*>(data.data+at),n);at+=n;}return result;}
};
}
extern "C" unsigned dh2_game_option_v1_decode_record(dh2::ui::GameOptionRow32V1* out,std::uint32_t* used,const std::uint8_t* input,std::uint32_t size) noexcept {
 if(!out||!used||!input||reinterpret_cast<std::uintptr_t>(out)%alignof(GameOptionRow32V1)||reinterpret_cast<std::uintptr_t>(used)%alignof(std::uint32_t)||size<28||size>16u*1024u*1024u||overlap(out,32,used,4)||overlap(out,32,input,size)||overlap(used,4,input,size))return 1;
 std::uint32_t values[8]{};for(unsigned i=0;i<7;++i)values[i+1]=word(input+4*i);std::memcpy(out,values,32);*used=28;return 0;
}
namespace dh2::ui {
struct GameOptionTableV1::Snapshot {
 std::vector<GameOptionRow32V1> rows;std::vector<std::string> names,fields;
 std::size_t record_offset{},name_offset{},record_used{},name_used{};
};
bool GameOptionTableV1::load_design_cache(GameOptionBytesV1 records,GameOptionBytesV1 names,GameOptionBytesV1 schema,std::string& error){
 error.clear();if(snapshot_&&snapshot_.use_count()>1){error="GameOption snapshot borrowed";return false;}
 try{
  Reader r(records),n(names),s(schema);auto next=std::make_shared<Snapshot>();
  // Exact original design prefix: DesignSettings43 words, Difficulty5 words.
  r.fixed_rows(172);r.fixed_rows(20);next->record_offset=r.at;
  n.names();n.names();next->name_offset=n.at;next->names=n.names();next->name_used=n.at;
  auto first=s.names(),second=s.names();if(first.size()!=43||second!=std::vector<std::string>{"DamageInputModifier","DamageOutputModifier","MaxPotionModifier","RespawnTimeModifier","XPRewardModifier"})throw std::runtime_error("GameOption design prefix schema differs");
  next->fields=s.names();const std::vector<std::string> expected{"Default","Label","Max","Min","Step","Type","ValueStr"};if(next->fields!=expected)throw std::runtime_error("GameOption schema differs");
  for(unsigned i=0;i<3;++i)if(s.names()!=expected)throw std::runtime_error("GameOption subtype schema differs");
  auto count=r.integer();if(count>65536||count!=next->names.size())throw std::runtime_error("GameOption records/names differ");next->rows.resize(count);
  for(auto& row:next->rows){r.require(28);std::uint32_t used{};if(dh2_game_option_v1_decode_record(&row,&used,r.data.data+r.at,28))throw std::runtime_error("GameOption record rejected");r.at+=used;}
  next->record_used=r.at;if(r.at!=r.data.size||n.at!=n.data.size||s.at!=s.data.size)throw std::runtime_error("GameOption trailing cache bytes");
  snapshot_=std::move(next);return true;
 }catch(const std::exception& e){error=e.what();return false;}
}
const std::vector<GameOptionRow32V1>& GameOptionTableV1::Borrow::rows()const{if(!snapshot_)throw std::logic_error("No GameOption snapshot");return snapshot_->rows;}
const std::vector<std::string>& GameOptionTableV1::Borrow::names()const{if(!snapshot_)throw std::logic_error("No GameOption snapshot");return snapshot_->names;}
const std::vector<std::string>& GameOptionTableV1::Borrow::fields()const{if(!snapshot_)throw std::logic_error("No GameOption snapshot");return snapshot_->fields;}
std::size_t GameOptionTableV1::Borrow::records_offset()const{return snapshot_?snapshot_->record_offset:0;}
std::size_t GameOptionTableV1::Borrow::names_offset()const{return snapshot_?snapshot_->name_offset:0;}
std::size_t GameOptionTableV1::Borrow::records_consumed()const{return snapshot_?snapshot_->record_used:0;}
std::size_t GameOptionTableV1::Borrow::names_consumed()const{return snapshot_?snapshot_->name_used:0;}
}
