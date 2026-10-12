#include "character_template_assets_v35.hpp"
#include <cstring>
#include <stdexcept>
namespace dh2::loader {
namespace {
struct Reader {
 data::Bytes bytes;std::size_t at{};
 explicit Reader(data::Bytes b):bytes(b){if(!b.data||b.size>8u*1024u*1024u)throw std::runtime_error("Invalid Character template stream");}
 std::uint32_t word(){if(at>bytes.size||bytes.size-at<4)throw std::runtime_error("Short Character template word");const auto*p=bytes.data+at;at+=4;return p[0]|(std::uint32_t(p[1])<<8)|(std::uint32_t(p[2])<<16)|(std::uint32_t(p[3])<<24);}
 std::string text(){auto n=word();if(n>4096||n>bytes.size-at)throw std::runtime_error("Invalid Character template name span");std::string out(reinterpret_cast<const char*>(bytes.data+at),n);at+=n;return out;}
 void finished(){if(at!=bytes.size)throw std::runtime_error("Unexpected Character template stream suffix");}
};
std::int16_t half(std::uint32_t word){const auto low=std::uint16_t(word);std::int16_t result;std::memcpy(&result,&low,2);return result;}
}
struct CharacterTemplateAssetsV35::Snapshot {std::vector<std::string> names;std::vector<std::vector<std::int32_t>> rows;};
const std::vector<std::string>& CharacterTemplateAssetsV35::Borrow::names()const{if(!snapshot_)throw std::logic_error("Character template snapshot absent");return snapshot_->names;}
const std::vector<std::vector<std::int32_t>>& CharacterTemplateAssetsV35::Borrow::rows()const{if(!snapshot_)throw std::logic_error("Character template snapshot absent");return snapshot_->rows;}
bool CharacterTemplateAssetsV35::load(data::Bytes records,data::Bytes names,data::Bytes schema,std::string&e){
 e.clear();if(snapshot_&&snapshot_.use_count()!=1){e="Character template snapshot has live borrowers";return false;}
 try{Reader fields(schema);if(fields.word()!=1||fields.text()!="Name"||fields.word()!=1||fields.text()!="CharInfo")throw std::runtime_error("Character template source schema differs");fields.finished();
  Reader data(records),labels(names);auto count=data.word();if(count>65536||labels.word()!=count)throw std::runtime_error("Character template count/name count differs");auto next=std::make_shared<Snapshot>();next->rows.reserve(count);next->names.reserve(count);
  for(std::uint32_t i=0;i<count;++i){auto n=data.word();if(n>65536||n>(data.bytes.size-data.at)/4)throw std::runtime_error("Character template entry span malformed");next->rows.emplace_back();auto&row=next->rows.back();row.reserve(n);for(std::uint32_t j=0;j<n;++j){auto bits=data.word();std::int32_t value;std::memcpy(&value,&bits,4);row.push_back(value);}next->names.push_back(labels.text());}
  data.finished();labels.finished();snapshot_=std::move(next);return true;
 }catch(const std::exception& ex){e=ex.what();return false;}
}
bool CharacterTemplateAssetsV35::Borrow::select(const std::string&name,std::int16_t&properties,std::int16_t&template_id,data::LootRandom8V2&random,std::string&e)const{
 e.clear();if(properties!=-1)return true; // Exact source cache fast path.
 if(!snapshot_||name.empty()){e="Required loaded Character templates and nonempty authored char_template";return false;}
 template_id=-1;for(std::size_t i=0;i<snapshot_->names.size();++i)if(!std::strcmp(name.c_str(),snapshot_->names[i].c_str())){template_id=half(static_cast<std::uint32_t>(i));break;}
 if(template_id<0)return true;const auto&row=snapshot_->rows.at(static_cast<std::size_t>(template_id));if(row.empty())return true;
 std::int32_t index{};if(dh2_loot_v2_random(&random,static_cast<std::int32_t>(row.size()),&index)){e="Required original application Random channel0";return false;}
 if(index<0||static_cast<std::size_t>(index)>=row.size()){e="Original Character template random index outside source row";return false;}
 properties=half(static_cast<std::uint32_t>(row[static_cast<std::size_t>(index)]));return true;
}
}
