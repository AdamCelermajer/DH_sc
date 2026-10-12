#include "faery_tables.hpp"
#include <cstring>
#include <stdexcept>
namespace {
using namespace dh2::data;
constexpr unsigned max_bytes=16u*1024u*1024u,max_count=65536,max_text=1048576;
template<class T>bool aligned(const T* p){return p&&reinterpret_cast<std::uintptr_t>(p)%alignof(T)==0;}
bool valid(const void* p,std::size_t n){return p&&n<=max_bytes&&reinterpret_cast<std::uintptr_t>(p)<=~std::uintptr_t(0)-n;}
bool overlaps(const void* a,std::size_t an,const void* b,std::size_t bn){auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);return an&&bn&&(x<=y?y-x<an:x-y<bn);}
unsigned raw(const std::uint8_t* p){return unsigned(p[0])|(unsigned(p[1])<<8)|(unsigned(p[2])<<16)|(unsigned(p[3])<<24);}
struct Cursor {const std::uint8_t* data;unsigned size,at=0;bool good=true;
 bool take(unsigned n){if(!good||n>size-at){good=false;return false;}return true;}
 unsigned word(){if(!take(4))return 0;auto n=raw(data+at);at+=4;return n;}
 FaerySpan16 span(unsigned n){if(!take(n))return {};FaerySpan16 s{data+at,n,0};at+=n;return s;}
 FaerySpan16 text(unsigned& n){n=word();if(n>max_text){good=false;return {};}return span(n);}
 FaerySpan16 array(){auto n=word();if(n>max_count){good=false;return {};}return span(n*4);}
};
bool record(Cursor& c,FaeryProjection36& p,FaerySpan16& s){p={};for(unsigned i=1;i<5;++i)p.words[i]=c.word();s=c.text(p.words[5]);p.words[7]=c.word();p.words[8]=c.word();return c.good;}
}
extern "C" unsigned dh2_faery_decode_record(FaeryProjection36* p,FaerySpan16* s,unsigned* used,const std::uint8_t* input,unsigned size){
 if(!aligned(p)||!aligned(s)||!aligned(used)||!valid(input,size)||overlaps(p,36,s,16)||overlaps(p,36,used,4)||overlaps(s,16,used,4)||overlaps(p,36,input,size)||overlaps(s,16,input,size)||overlaps(used,4,input,size))return 1;
 Cursor c{input,size};FaeryProjection36 next{};FaerySpan16 span{};if(!record(c,next,span))return 1;*p=next;*s=span;*used=c.at;return 0;
}
extern "C" unsigned dh2_faery_decode_list(FaerySpan16* out,unsigned* used,const std::uint8_t* input,unsigned size){
 if(!aligned(out)||!aligned(used)||!valid(input,size)||overlaps(out,16,used,4)||overlaps(out,16,input,size)||overlaps(used,4,input,size))return 1;
 Cursor c{input,size};auto s=c.array();if(!c.good)return 1;*out=s;*used=c.at;return 0;
}
extern "C" unsigned dh2_faery_tables_measure(FaerySummary16* out,const std::uint8_t* input,unsigned size){
 if(!aligned(out)||!valid(input,size)||overlaps(out,16,input,size))return 1;Cursor c{input,size};FaerySummary16 next{};next.lists=c.word();if(next.lists>max_count)return 1;
 for(unsigned i=0;i<next.lists&&c.good;++i)c.array();next.list_end=c.at;next.faeries=c.word();if(next.faeries>max_count)return 1;
 for(unsigned i=0;i<next.faeries&&c.good;++i){FaeryProjection36 p{};FaerySpan16 s{};record(c,p,s);}if(!c.good)return 1;next.records_end=c.at;*out=next;return 0;
}
#ifndef DH2_FAERY_DECODER_ONLY
namespace {
std::int32_t signed_word(unsigned n){std::int32_t out;std::memcpy(&out,&n,4);return out;}
std::vector<std::string> names(Cursor& c){auto n=c.word();if(n>max_count)throw std::runtime_error("Faery names exceed bounds");std::vector<std::string> out;out.reserve(n);for(unsigned i=0;i<n;++i){unsigned size=0;auto s=c.text(size);if(!c.good)throw std::runtime_error("Truncated Faery names");out.emplace_back(reinterpret_cast<const char*>(s.data),s.bytes);}if(!c.good)throw std::runtime_error("Truncated Faery names");return out;}
std::int32_t find(const std::vector<std::string>& values,const char* key){if(!key)return -1;for(unsigned i=0;i<values.size();++i)if(!std::strcmp(values[i].c_str(),key))return static_cast<std::int32_t>(i);return -1;}
}
namespace dh2::data {
struct FaeryTables::Snapshot {std::vector<std::vector<std::int32_t>> lists;std::vector<FaeryRecord> faeries;std::vector<std::string> list_names,faery_names,list_fields,faery_fields;unsigned list_used,records_used,names_used,schema_used;};
bool FaeryTables::load(Bytes bytes,Bytes name_bytes,Bytes schema_bytes,std::string& error){
 error.clear();if(snapshot_&&snapshot_.use_count()>1){error="Faery borrowed snapshot pinned";return false;}
 try{if(!valid(bytes.data,bytes.size)||!valid(name_bytes.data,name_bytes.size)||!valid(schema_bytes.data,schema_bytes.size))throw std::runtime_error("Faery input outside bounds");FaerySummary16 summary{};if(dh2_faery_tables_measure(&summary,bytes.data,static_cast<unsigned>(bytes.size)))throw std::runtime_error("Malformed Faery tables");auto next=std::make_shared<Snapshot>();Cursor n{name_bytes.data,static_cast<unsigned>(name_bytes.size)},s{schema_bytes.data,static_cast<unsigned>(schema_bytes.size)};next->list_names=names(n);next->faery_names=names(n);next->faery_fields=names(s);next->list_fields=names(s);if(next->list_names.size()!=summary.lists||next->faery_names.size()!=summary.faeries)throw std::runtime_error("Faery names/table dimensions differ");if(next->faery_fields!=std::vector<std::string>{"Description","Elemental","ModelFile","Name","SpellScript","SpellType","Type"}||next->list_fields!=std::vector<std::string>{"List"})throw std::runtime_error("Faery source schema differs");unsigned at=4;
  for(unsigned i=0;i<summary.lists;++i){FaerySpan16 span{};unsigned used=0;if(dh2_faery_decode_list(&span,&used,bytes.data+at,static_cast<unsigned>(bytes.size-at)))throw std::runtime_error("Faery list rejected");std::vector<std::int32_t> list;for(unsigned j=0;j<span.bytes;j+=4)list.push_back(signed_word(raw(span.data+j)));next->lists.push_back(std::move(list));at+=used;}at+=4;
  for(unsigned i=0;i<summary.faeries;++i){FaeryRecord row{};FaerySpan16 span{};unsigned used=0;if(dh2_faery_decode_record(&row.scalar,&span,&used,bytes.data+at,static_cast<unsigned>(bytes.size-at)))throw std::runtime_error("Faery row rejected");row.script.assign(reinterpret_cast<const char*>(span.data),span.bytes);next->faeries.push_back(std::move(row));at+=used;}next->list_used=summary.list_end;next->records_used=at;next->names_used=n.at;next->schema_used=s.at;snapshot_=std::move(next);return true;
 }catch(const std::exception& e){error=e.what();return false;}
}
#define VIEW(method,member,type) const type& FaeryTables::Borrow::method()const{if(!snapshot_)throw std::logic_error("Missing Faery tables");return snapshot_->member;}
VIEW(lists,lists,std::vector<std::vector<std::int32_t>>)
VIEW(faeries,faeries,std::vector<FaeryRecord>)
VIEW(list_names,list_names,std::vector<std::string>)
VIEW(faery_names,faery_names,std::vector<std::string>)
VIEW(list_fields,list_fields,std::vector<std::string>)
VIEW(faery_fields,faery_fields,std::vector<std::string>)
#undef VIEW
std::int32_t FaeryTables::Borrow::list_index(const char* s)const{return snapshot_?find(snapshot_->list_names,s):-1;}
std::int32_t FaeryTables::Borrow::faery_index(const char* s)const{return snapshot_?find(snapshot_->faery_names,s):-1;}
std::size_t FaeryTables::Borrow::list_table_consumed()const{return snapshot_?snapshot_->list_used:0;}
std::size_t FaeryTables::Borrow::records_consumed()const{return snapshot_?snapshot_->records_used:0;}
std::size_t FaeryTables::Borrow::names_consumed()const{return snapshot_?snapshot_->names_used:0;}
std::size_t FaeryTables::Borrow::schema_consumed()const{return snapshot_?snapshot_->schema_used:0;}
}
#endif
