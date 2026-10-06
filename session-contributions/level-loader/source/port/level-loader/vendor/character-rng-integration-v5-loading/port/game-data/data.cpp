#include "data.hpp"
#include <algorithm>
#include <cstring>
#include <set>
#include <stdexcept>
namespace dh2::data {
namespace {
struct Reader{
 Bytes bytes;std::size_t offset=0;
 explicit Reader(Bytes b):bytes(b){if(!b.data||b.size<4||b.size>8*1024*1024)throw std::runtime_error("Data input outside size limit");}
 std::uint32_t word(){if(offset>bytes.size||bytes.size-offset<4)throw std::runtime_error("Truncated data word");const auto* p=bytes.data+offset;offset+=4;return p[0]|(std::uint32_t(p[1])<<8)|(std::uint32_t(p[2])<<16)|(std::uint32_t(p[3])<<24);}
 std::vector<std::string> strings(bool unique){
  const auto count=word();if(!count||count>10000)throw std::runtime_error("String count outside limit");std::vector<std::string> out;out.reserve(count);std::set<std::string> seen;
  for(unsigned i=0;i<count;++i){const auto size=word();if(!size||size>4096||offset>bytes.size||size>bytes.size-offset)throw std::runtime_error("String length outside limit");
   std::string value(reinterpret_cast<const char*>(bytes.data+offset),size);offset+=size;
   if(std::any_of(value.begin(),value.end(),[](unsigned char c){return c<32||c>126;}))throw std::runtime_error("Non-ASCII table identifier");
   if(unique&&!seen.insert(value).second)throw std::runtime_error("Duplicate table identifier");
   out.push_back(std::move(value));
  }return out;
 }
};
}
bool load_characters(Bytes bytes,Bytes names,Bytes fields,CharacterTable& out,std::string& error){
 out={};error.clear();try{
  Reader records(bytes),name_reader(names),field_reader(fields);CharacterTable next;
  next.names=name_reader.strings(true);next.fields=field_reader.strings(true);const auto count=records.word();
  if(count!=next.names.size()||next.fields.size()!=224||std::uint64_t(count)*896>bytes.size-records.offset)throw std::runtime_error("Character table dimensions differ");
  next.rows.resize(count);for(auto& row:next.rows)for(auto& value:row){auto bits=records.word();std::memcpy(&value,&bits,4);}
  next.data_consumed=records.offset;next.names_consumed=name_reader.offset;next.fields_consumed=field_reader.offset;out=std::move(next);return true;
 }catch(const std::exception& e){error=e.what();return false;}
}
bool load_dictionary(Bytes names,Bytes values,Dictionary& out,std::string& error){
 out={};error.clear();try{Reader keys(names),entries(values);Dictionary next;next.names=keys.strings(true);next.values=entries.strings(false);
  if(next.names.size()!=next.values.size()||keys.offset!=names.size||entries.offset!=values.size)throw std::runtime_error("Dictionary dimensions or suffix differ");
  out=std::move(next);return true;
 }catch(const std::exception& e){error=e.what();return false;}
}
const std::int32_t* property(const CharacterTable& table,const std::string& name,const std::string& field){
 const auto row=std::find(table.names.begin(),table.names.end(),name),column=std::find(table.fields.begin(),table.fields.end(),field);
 if(row==table.names.end()||column==table.fields.end()||std::size_t(row-table.names.begin())>=table.rows.size()||column-table.fields.begin()>=224)return nullptr;
 return &table.rows[row-table.names.begin()][column-table.fields.begin()];
}
const std::string* lookup(const Dictionary& table,const std::string& name){auto item=std::find(table.names.begin(),table.names.end(),name);if(item==table.names.end()||std::size_t(item-table.names.begin())>=table.values.size())return nullptr;return &table.values[item-table.names.begin()];}
}
