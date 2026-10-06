#include "level_tables.hpp"
#include <cstring>
#include <limits>
#include <stdexcept>
namespace {
bool aligned(const void* p,std::size_t alignment){return p&&reinterpret_cast<std::uintptr_t>(p)%alignment==0;}
bool overlap(const void* a,std::size_t an,const void* b,std::size_t bn){
 auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);
 return an&&bn&&(x<=y?y-x<an:x-y<bn);
}
struct Reader {
 const std::uint8_t* data;std::uint32_t size,at=0;
 explicit Reader(dh2::data::Bytes bytes):data(bytes.data){
  if(!data||bytes.size>8u*1024u*1024u)throw std::runtime_error("Level input outside limit");
  size=static_cast<std::uint32_t>(bytes.size);
 }
 void require(std::uint32_t count){if(count>size-at)throw std::runtime_error("Truncated level record");}
 std::uint32_t word(){require(4);auto* p=data+at;at+=4;return std::uint32_t(p[0])|(std::uint32_t(p[1])<<8)|(std::uint32_t(p[2])<<16)|(std::uint32_t(p[3])<<24);}
 std::uint32_t byte(){require(1);return data[at++];}
 dh2::data::LevelTextSpan16 text(){auto n=word();require(n);auto* p=data+at;at+=n;return {p,n,0};}
 std::vector<std::string> strings(){
  auto n=word();if(n>10000)throw std::runtime_error("Level name count outside limit");
  std::vector<std::string> result;result.reserve(n);
  for(std::uint32_t i=0;i<n;++i){auto span=text();result.emplace_back(reinterpret_cast<const char*>(span.data),span.size);}
  return result;
 }
 void end(){if(at!=size)throw std::runtime_error("Unexpected level data suffix");}
};
template<class Projection>bool valid(Projection* out,dh2::data::LevelTextSpan16* text,std::uint32_t spans,
 std::uint32_t* used,const std::uint8_t* input,std::uint32_t size){
 if(!aligned(out,alignof(Projection))||!aligned(text,alignof(dh2::data::LevelTextSpan16))||!aligned(used,alignof(std::uint32_t))||!input||size>8u*1024u*1024u)return false;
 const std::size_t bytes=spans*sizeof(*text);
 return !overlap(out,sizeof(*out),text,bytes)&&!overlap(out,sizeof(*out),used,4)&&!overlap(text,bytes,used,4)
  &&!overlap(out,sizeof(*out),input,size)&&!overlap(text,bytes,input,size)&&!overlap(used,4,input,size);
}
}
extern "C" unsigned dh2_fast_travel_decode_record(dh2::data::FastTravelProjection28* out,
 dh2::data::LevelTextSpan16* text,std::uint32_t* used,const std::uint8_t* input,std::uint32_t size){
 if(!valid(out,text,1,used,input,size))return 1;
 try{Reader r({input,size});dh2::data::FastTravelProjection28 next{};
  next.words[1]=r.word();next.words[2]=r.word();auto span=r.text();next.words[3]=span.size;
  next.words[5]=r.word();next.words[6]=r.word();*out=next;*text=span;*used=r.at;return 0;
 }catch(const std::exception&){return 1;}
}
extern "C" unsigned dh2_level_decode_record(dh2::data::LevelProjection72* out,
 dh2::data::LevelTextSpan16* text,std::uint32_t* used,const std::uint8_t* input,std::uint32_t size){
 if(!valid(out,text,2,used,input,size))return 1;
 try{Reader r({input,size});dh2::data::LevelProjection72 next{};dh2::data::LevelTextSpan16 spans[2];
  next.words[1]=r.byte();spans[0]=r.text();next.words[2]=spans[0].size;
  next.words[4]=r.word();next.words[5]=r.byte();next.words[6]=r.word();
  spans[1]=r.text();next.words[7]=spans[1].size;
  for(unsigned i=9;i<18;++i)next.words[i]=r.word();
  *out=next;text[0]=spans[0];text[1]=spans[1];*used=r.at;return 0;
 }catch(const std::exception&){return 1;}
}
namespace dh2::data {
bool load_levels(Bytes records,Bytes names,Bytes schema,LevelTables& out,std::string& error){
 out={};error.clear();try{
  Reader r(records),n(names),s(schema);LevelTables next;
  next.travel_names=n.strings();next.level_names=n.strings();n.end();
  next.travel_fields=s.strings();next.level_fields=s.strings();s.end();
  const std::vector<std::string> travel_fields={"DescriptionId","EntryPointId","LevelName","LocationType","StringId"};
  const std::vector<std::string> level_fields={"Dbg_IsStable","DynamicBusRouting","Hub","IsRandom","LevelDescription","LevelFile","LevelName","LevelState","MapName","MonsterLvlMax","MonsterLvlMaxHard","MonsterLvlMaxNightmare","MonsterLvlMin","MonsterLvlMinHard","MonsterLvlMinNightmare"};
  if(next.travel_fields!=travel_fields||next.level_fields!=level_fields)throw std::runtime_error("Level schema differs");
  auto count=r.word();if(count!=next.travel_names.size())throw std::runtime_error("Travel dimensions differ");next.travel.reserve(count);
  for(std::uint32_t i=0;i<count;++i){FastTravelRecord row;LevelTextSpan16 span;std::uint32_t used;
   if(dh2_fast_travel_decode_record(&row.scalar,&span,&used,r.data+r.at,r.size-r.at))throw std::runtime_error("Travel row rejected");
   row.level_name.assign(reinterpret_cast<const char*>(span.data),span.size);r.at+=used;next.travel.push_back(std::move(row));
  }
  next.travel_data_consumed=r.at;count=r.word();if(count!=next.level_names.size())throw std::runtime_error("Level dimensions differ");next.levels.reserve(count);
  for(std::uint32_t i=0;i<count;++i){LevelRecord row;LevelTextSpan16 span[2];std::uint32_t used;
   if(dh2_level_decode_record(&row.scalar,span,&used,r.data+r.at,r.size-r.at))throw std::runtime_error("Level row rejected");
   row.description.assign(reinterpret_cast<const char*>(span[0].data),span[0].size);row.file.assign(reinterpret_cast<const char*>(span[1].data),span[1].size);
   r.at+=used;next.levels.push_back(std::move(row));
  }
  r.end();next.data_consumed=r.at;out=std::move(next);return true;
 }catch(const std::exception& failure){error=failure.what();return false;}
}
}
