#include "../script_manager_owner_v52.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>

using namespace dh2::loader;
namespace {
std::vector<std::uint8_t> read(const char* path) {
 std::ifstream f(path,std::ios::binary);
 if(!f)throw std::runtime_error(std::string("cannot read ")+path);
 return {std::istreambuf_iterator<char>(f),{}};
}
}
int main(int argc,char** argv) {
 try {
  if(argc!=3)throw std::runtime_error("usage: inventory names.bin scripts.bin");
  const auto names=read(argv[1]),scripts=read(argv[2]);
  auto name_owner=std::make_shared<std::vector<std::uint8_t>>(names);
  auto script_owner=std::make_shared<std::vector<std::uint8_t>>(scripts);
  ScriptReaderV52 nr({name_owner,name_owner->data(),name_owner->size(),true});
  const auto name_count=nr.word();
  std::vector<std::string> decoded_names;
  decoded_names.reserve(name_count);
  for(std::uint32_t i=0;i<name_count;++i){auto text=nr.text(nr.word());decoded_names.emplace_back(text.data());}
  if(!nr.finished()||decoded_names.size()<=16||decoded_names[16]!="Return_Cinematic")
   throw std::runtime_error("actual Witch Cave name table does not map index16 to Return_Cinematic");
  ScriptReaderV52 reader({script_owner,script_owner->data(),script_owner->size(),true});
  const auto count=reader.word();
  if(count!=name_count)throw std::runtime_error("actual Witch Cave script/name table count mismatch");
  struct Command {std::int32_t kind;std::map<std::uint32_t,ScriptScalarV52> scalars;std::map<std::uint32_t,std::vector<char>> strings;};
  std::vector<Command> commands16;
  for(std::uint32_t script=0;script<count;++script){
   const auto commands=reader.word();
   for(std::uint32_t i=0;i<commands;++i){
    const auto kind=static_cast<std::int32_t>(reader.peek_word());
    std::string error;auto data=ScriptCommandDataV52::original_storage_factory(kind,error);
    if(!data)throw std::runtime_error("actual generated-data schema unavailable for Return_Cinematic script family: "+error);
    if(!data->read(reader,error))throw std::runtime_error("actual packaged command data did not decode: "+error);
    if(script==16)commands16.push_back({kind,data->scalar_fields(),data->strings()});
   }
  }
  if(!reader.finished())throw std::runtime_error("unexpected bytes after actual Witch Cave scripts");
  if(commands16.empty())throw std::runtime_error("actual Return_Cinematic has no command rows");
  std::cout<<"actual 002 Witch Cave script index=16 name=Return_Cinematic command_count="<<commands16.size()<<"\n";
  for(std::size_t i=0;i<commands16.size();++i){
   const auto& c=commands16[i];std::cout<<i<<" kind="<<c.kind;
   for(const auto& [offset,value]:c.scalars)std::cout<<" u"<<unsigned(value.width)<<"@"<<offset<<"="<<value.bits;
   for(const auto& [offset,value]:c.strings)std::cout<<" str@"<<offset<<"=\""<<std::string(value.data())<<"\"";
   std::cout<<'\n';
  }
  std::cout<<"actual bytes and generated-data schemas consumed exactly; no synthetic rows or callbacks used\n";
  return 0;
 } catch(const std::exception& e) { std::cerr<<e.what()<<'\n';return 1; }
}
