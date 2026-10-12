#include "skill_tables.hpp"
#include <cstring>
#include <stdexcept>
namespace {
using namespace dh2::data;
constexpr unsigned max_bytes=16u*1024u*1024u,max_count=65536,max_text=1048576;
template<class T>bool aligned(const T* p){return p&&reinterpret_cast<std::uintptr_t>(p)%alignof(T)==0;}
bool overlaps(const void* a,std::size_t an,const void* b,std::size_t bn){auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);return an&&bn&&(x<=y?y-x<an:x-y<bn);}
std::uint32_t raw(const std::uint8_t* p){return std::uint32_t(p[0])|(std::uint32_t(p[1])<<8)|(std::uint32_t(p[2])<<16)|(std::uint32_t(p[3])<<24);}
struct Cursor {const std::uint8_t* data;std::uint32_t size,at=0;bool good=true;
 bool take(unsigned n){if(!good||n>size-at){good=false;return false;}return true;}
 unsigned word(){if(!take(4))return 0;auto v=raw(data+at);at+=4;return v;}
 unsigned byte(){if(!take(1))return 0;return data[at++];}
 SkillSpan16 span(unsigned n){if(!take(n))return {};SkillSpan16 s{data+at,n,0};at+=n;return s;}
 SkillSpan16 text(unsigned& n){n=word();if(n>max_text){good=false;return {};}return span(n);}
 SkillSpan16 array(unsigned& n){n=word();if(n>max_count){good=false;return {};}return span(n*4);}
};
bool record(Cursor& c,SkillProjection76& p,SkillSpans48& s){
 p={};auto* w=p.words;w[1]=c.word();w[2]=c.byte();s.display=c.array(w[3]);w[5]=c.word();w[6]=c.byte();w[7]=c.word();w[8]=c.word();s.script=c.text(w[9]);w[11]=c.byte();w[12]=c.word();w[13]=c.word();s.icon=c.text(w[14]);w[16]=c.word();w[17]=c.word();w[18]=c.word();return c.good;
}
}
extern "C" unsigned dh2_skill_decode_record(SkillProjection76* p,SkillSpans48* s,std::uint32_t* used,const std::uint8_t* input,std::uint32_t size){
 if(!aligned(p)||!aligned(s)||!aligned(used)||!input||size>max_bytes||overlaps(p,76,s,48)||overlaps(p,76,used,4)||overlaps(s,48,used,4)||overlaps(p,76,input,size)||overlaps(s,48,input,size)||overlaps(used,4,input,size))return 1;
 Cursor c{input,size};SkillProjection76 next{};SkillSpans48 spans{};if(!record(c,next,spans))return 1;*p=next;*s=spans;*used=c.at;return 0;
}
extern "C" unsigned dh2_skill_decode_list(SkillSpan16* out,std::uint32_t* used,const std::uint8_t* input,std::uint32_t size){
 if(!aligned(out)||!aligned(used)||!input||size>max_bytes||overlaps(out,16,used,4)||overlaps(out,16,input,size)||overlaps(used,4,input,size))return 1;
 Cursor c{input,size};unsigned count=0;auto span=c.array(count);if(!c.good)return 1;*out=span;*used=c.at;return 0;
}
extern "C" unsigned dh2_skill_tables_measure(SkillSummary16* out,const std::uint8_t* input,std::uint32_t size){
 if(!aligned(out)||!input||size>max_bytes||overlaps(out,16,input,size))return 1;Cursor c{input,size};SkillSummary16 next{};next.lists=c.word();if(next.lists>max_count)return 1;
 for(unsigned i=0;i<next.lists&&c.good;++i){unsigned count=0;c.array(count);}next.list_end=c.at;next.skills=c.word();if(next.skills>max_count)return 1;
 for(unsigned i=0;i<next.skills&&c.good;++i){SkillProjection76 p;SkillSpans48 s;record(c,p,s);}if(!c.good)return 1;next.records_end=c.at;*out=next;return 0;
}
#ifndef DH2_SKILL_DECODER_ONLY
namespace {
std::int32_t signed_word(std::uint32_t v){std::int32_t out;std::memcpy(&out,&v,4);return out;}
std::vector<std::int32_t> integers(SkillSpan16 s){std::vector<std::int32_t> out;out.reserve(s.bytes/4);for(unsigned i=0;i<s.bytes;i+=4)out.push_back(signed_word(raw(s.data+i)));return out;}
std::vector<std::string> names(Cursor& c){auto count=c.word();if(!c.good)throw std::runtime_error("Truncated Skill names");if(count>max_count)throw std::runtime_error("Skill names exceed bounds");std::vector<std::string> out;out.reserve(count);for(unsigned i=0;i<count;++i){unsigned n=0;auto s=c.text(n);if(!c.good)throw std::runtime_error("Truncated Skill names");out.emplace_back(reinterpret_cast<const char*>(s.data),s.bytes);}return out;}
std::int32_t find(const std::vector<std::string>& v,const char* key){if(!key)return -1;for(unsigned i=0;i<v.size();++i)if(!std::strcmp(v[i].c_str(),key))return i;return -1;}
const char* const fields[]={"Anim","AnimIsMoving","DisplayProps","ElementalType","FairieDependantText","Flags","Level","Script","SkillAssignable","SkillCurrLevel","SkillDescription","SkillIcon","SkillName","SkillNextLevel","Type"};
}
namespace dh2::data {
struct SkillTables::Snapshot {std::vector<std::vector<std::int32_t>> lists;std::vector<SkillRecord> skills;std::vector<std::string> list_names,skill_names,list_fields,skill_fields;unsigned list_used,records_used,names_used,schema_used;};
bool SkillTables::load(Bytes bytes,Bytes name_bytes,Bytes schema_bytes,std::string& error){
 error.clear();if(snapshot_&&snapshot_.use_count()>1){error="Skill tables borrowed snapshot pinned";return false;}
 try{if(!bytes.data||!name_bytes.data||!schema_bytes.data||bytes.size>max_bytes||name_bytes.size>max_bytes||schema_bytes.size>max_bytes)throw std::runtime_error("Skill input outside bounds");SkillSummary16 summary{};if(dh2_skill_tables_measure(&summary,bytes.data,static_cast<unsigned>(bytes.size)))throw std::runtime_error("Truncated/malformed Skill tables");auto next=std::make_shared<Snapshot>();Cursor n{name_bytes.data,static_cast<unsigned>(name_bytes.size)},s{schema_bytes.data,static_cast<unsigned>(schema_bytes.size)};next->list_names=names(n);next->skill_names=names(n);next->list_fields=names(s);next->skill_fields=names(s);if(next->list_names.size()!=summary.lists||next->skill_names.size()!=summary.skills)throw std::runtime_error("Skill names/table dimensions differ");if(next->list_fields!=std::vector<std::string>{"List"}||next->skill_fields.size()!=15)throw std::runtime_error("Skill source schema differs");for(unsigned i=0;i<15;++i)if(next->skill_fields[i]!=fields[i])throw std::runtime_error("Skill source schema differs");unsigned at=4;next->lists.reserve(summary.lists);
  for(unsigned i=0;i<summary.lists;++i){SkillSpan16 span{};unsigned used=0;if(dh2_skill_decode_list(&span,&used,bytes.data+at,static_cast<unsigned>(bytes.size-at)))throw std::runtime_error("Skill list rejected");next->lists.push_back(integers(span));at+=used;}at+=4;next->skills.reserve(summary.skills);
  for(unsigned i=0;i<summary.skills;++i){SkillRecord r{};SkillSpans48 spans{};unsigned used=0;if(dh2_skill_decode_record(&r.scalar,&spans,&used,bytes.data+at,static_cast<unsigned>(bytes.size-at)))throw std::runtime_error("Skill row rejected");r.display_props=integers(spans.display);r.script.assign(reinterpret_cast<const char*>(spans.script.data),spans.script.bytes);r.icon.assign(reinterpret_cast<const char*>(spans.icon.data),spans.icon.bytes);next->skills.push_back(std::move(r));at+=used;}next->list_used=summary.list_end;next->records_used=at;next->names_used=n.at;next->schema_used=s.at;snapshot_=std::move(next);return true;
 }catch(const std::exception& e){error=e.what();return false;}
}
#define SKILL_VIEW(method,member,type) const type& SkillTables::Borrow::method()const{if(!snapshot_)throw std::logic_error("Missing Skill tables");return snapshot_->member;}
SKILL_VIEW(lists,lists,std::vector<std::vector<std::int32_t>>)
SKILL_VIEW(skills,skills,std::vector<SkillRecord>)
SKILL_VIEW(list_names,list_names,std::vector<std::string>)
SKILL_VIEW(skill_names,skill_names,std::vector<std::string>)
SKILL_VIEW(list_fields,list_fields,std::vector<std::string>)
SKILL_VIEW(skill_fields,skill_fields,std::vector<std::string>)
#undef SKILL_VIEW
std::int32_t SkillTables::Borrow::list_index(const char* k)const{return snapshot_?find(snapshot_->list_names,k):-1;}
std::int32_t SkillTables::Borrow::skill_index(const char* k)const{return snapshot_?find(snapshot_->skill_names,k):-1;}
std::int32_t SkillTables::Borrow::list_field(const char* k)const{return snapshot_?find(snapshot_->list_fields,k):-1;}
std::int32_t SkillTables::Borrow::skill_field(const char* k)const{return snapshot_?find(snapshot_->skill_fields,k):-1;}
std::size_t SkillTables::Borrow::list_table_consumed()const{return snapshot_?snapshot_->list_used:0;}
std::size_t SkillTables::Borrow::records_consumed()const{return snapshot_?snapshot_->records_used:0;}
std::size_t SkillTables::Borrow::names_consumed()const{return snapshot_?snapshot_->names_used:0;}
std::size_t SkillTables::Borrow::schema_consumed()const{return snapshot_?snapshot_->schema_used:0;}
}
#endif
