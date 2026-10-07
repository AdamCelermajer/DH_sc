#include "character_templates_v78.hpp"
#include <cstring>
#include <stdexcept>
namespace dh2::data {namespace {
class Reader {
 Bytes bytes_;std::size_t at_{};
public:
 explicit Reader(Bytes bytes):bytes_(bytes){if((bytes.size&&!bytes.data)||bytes.size>8u*1024u*1024u)throw std::runtime_error("Invalid source CharTemplate stream");}
 std::uint32_t word(){if(bytes_.size-at_<4)throw std::runtime_error("Truncated source CharTemplate word");const auto* p=bytes_.data+at_;at_+=4;return std::uint32_t(p[0])|(std::uint32_t(p[1])<<8)|(std::uint32_t(p[2])<<16)|(std::uint32_t(p[3])<<24);}
 std::string text(){const auto n=word();if(n>65536u||n>bytes_.size-at_)throw std::runtime_error("Truncated source CharTemplate CString");std::string out(reinterpret_cast<const char*>(bytes_.data+at_),n);at_+=n;return out;}
 bool done()const noexcept{return at_==bytes_.size;}
};
std::int16_t narrow(std::int32_t value){const auto bits=static_cast<std::uint16_t>(value);std::int16_t out;std::memcpy(&out,&bits,2);return out;}
}
bool CharacterTemplateTableV78::load(Bytes records,Bytes names,Bytes schema,std::string& e){
 if(ready_){e="Actual initialized CharacterTemplates table cannot be replaced under live actors";return false;}
 try{
  Reader r(records),n(names),s(schema);
  if(s.word()!=1||s.text()!="Name"||s.word()!=1||s.text()!="CharInfo"||!s.done())throw std::runtime_error("Original CharTemplate schema/order differs");
  const auto count=r.word();if(count>65536u||n.word()!=count)throw std::runtime_error("CharTemplate member/name counts differ");
  std::vector<CharacterTemplateRowV78> rows;std::vector<std::string> texts;rows.reserve(count);texts.reserve(count);
  std::uint32_t total{};
  for(std::uint32_t i=0;i<count;++i){
   CharacterTemplateRowV78 row;const auto members=r.word();if(members>65536u||total>1048576u-members)throw std::runtime_error("CharTemplate members outside native admission bound");
   total+=members;row.char_info.reserve(members);row.selected_ids.reserve(members);
   for(std::uint32_t j=0;j<members;++j){const auto word=r.word();std::int32_t value;std::memcpy(&value,&word,4);row.char_info.push_back(value);row.selected_ids.push_back(narrow(value));}
   rows.push_back(std::move(row));texts.push_back(n.text());
  }
  if(!r.done()||!n.done())throw std::runtime_error("Unexpected source CharTemplate stream tail");
  rows_=std::move(rows);names_=std::move(texts);ready_=true;e.clear();return true;
 }catch(const std::exception& error){e=error.what();return false;}
}
std::int32_t CharacterTemplateTableV78::find(const char* name)const noexcept{
 if(!ready_||!name)return -1;for(std::size_t i=0;i<names_.size();++i)if(!std::strcmp(name,names_[i].c_str()))return static_cast<std::int32_t>(i);return -1;
}
bool CharacterTemplateTableV78::preset(const std::string& name,const std::int16_t*& out,std::uint32_t& count,std::string& e)const{
 if(!ready_){e="Required real initialized CharacterTemplates Arrays owner";return false;}
 const auto id=find(name.c_str());out=nullptr;count=0;
 if(id>=0){const auto& row=rows_[static_cast<std::size_t>(id)];out=row.selected_ids.data();count=static_cast<std::uint32_t>(row.selected_ids.size());}
 e.clear();return true;
}
bool CharacterTemplateTableV78::safe_template_id(const std::string& name,std::int16_t& cache,std::int32_t& out,std::string& e)const{
 if(!ready_){e="Required actual CharacterTemplates member/name initialization";return false;}
 if(!name.empty())cache=narrow(find(name.c_str()));out=cache;e.clear();return true;
}
}
