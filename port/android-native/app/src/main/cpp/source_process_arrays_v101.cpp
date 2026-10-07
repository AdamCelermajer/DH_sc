#include "source_process_arrays_v101.hpp"
#include "source_process_compiled_members_v121.hpp"
#include <cstring>
#include <stdexcept>
namespace dh2::android_ui {
namespace {
struct Cursor {
 const std::vector<std::uint8_t>& bytes;std::size_t at{};
 void require(std::size_t n){if(at>bytes.size()||n>bytes.size()-at)throw std::runtime_error("Short process PyData array stream");}
 std::uint32_t integer(unsigned n=4){require(n);std::uint32_t v{};for(unsigned i=0;i<n;++i)v|=std::uint32_t(bytes[at++])<<(8*i);return v;}
 std::string string(){const auto n=integer();require(n);std::string out(reinterpret_cast<const char*>(bytes.data()+at),n);at+=n;return out;}
 std::uint32_t count(){auto n=integer();if(n>1000000)throw std::runtime_error("Process PyData native allocation bound");return n;}
 void row(const char*& shape,std::vector<ProcessArrayValueV101>& fields,unsigned depth=0){
  if(depth>32)throw std::runtime_error("Process PyData nested record bound");
  while(*shape&&*shape!=']'){
   fields.emplace_back();auto& field=fields.back();const char type=*shape++;
   switch(type){
   case 'i':field.kind=ProcessArrayValueV101::Kind::word;field.bits=integer();break;
   case 'b':field.kind=ProcessArrayValueV101::Kind::byte;field.bits=integer(1);break;
   case 'h':field.kind=ProcessArrayValueV101::Kind::half;field.bits=integer(2);break;
   case 'S':field.kind=ProcessArrayValueV101::Kind::string;field.text=string();break;
   case '[':{
    field.kind=ProcessArrayValueV101::Kind::vector;field.bits=count();field.elements.resize(field.bits);
    const char* begin=shape;const char* end=shape;unsigned nesting=1;
    while(*end&&nesting){if(*end=='[')++nesting;else if(*end==']')--nesting;if(nesting)++end;}
    if(!*end)throw std::runtime_error("Invalid source nested wire descriptor");
    for(auto& element:field.elements){const char* part=begin;row(part,element.fields,depth+1);if(part!=end)throw std::runtime_error("Process PyData nested descriptor mismatch");}
    shape=end+1;break;
   }
   default:throw std::runtime_error("Required original Structs.read wire operation");
   }
  }
 }
};
std::string name_key(const std::string& text){return std::string(text.c_str());}
}
SourceProcessArraysV101::SourceProcessArraysV101(){
 for(const auto& entry:process_array_registration_v101)groups_.try_emplace(entry.group);
 for(const auto& entry:process_array_classes_v101)classes_.emplace(entry.name,entry.group);
 for(const auto& entry:process_array_members_v101){
  member_types_.emplace(entry.type,entry.group); //Original class-map insertion preserves the first duplicate.
  const ProcessCompiledStructV121* source{};
  for(const auto& actual:process_compiled_structs_v121)if(!std::strcmp(actual.type,entry.type)){source=&actual;break;}
  if(!source||source->count!=entry.count||source->getter!=entry.getter)
   throw std::runtime_error(std::string("Required exact compiled Structs C1 descriptor: ")+entry.type);
  auto& group=groups_.at(entry.group);
  group.members.reserve(source->count);
  for(std::uint32_t i=0;i<source->count;++i)group.members.emplace_back(source->members[i]);
  group.members_loaded=true;
 }
}
const ProcessArrayGroupV101* SourceProcessArraysV101::group(const char* name)const noexcept{
 if(!name)return nullptr;const auto found=groups_.find(name);return found==groups_.end()?nullptr:&found->second;
}
bool SourceProcessArraysV101::get_member_id(const char* type,const char* name,std::int32_t& out,std::string& error)const{
 out=-1;if(!type||!name){error="Required actual PyData class/member strings";return false;}
 auto members=member_types_.find(type);
 if(members!=member_types_.end()){
   const auto& actual=groups_.at(members->second);if(!actual.members_loaded){error="Required original compiled member CString constructor";return false;}
  for(std::size_t i=0;i<actual.members.size();++i)if(!std::strcmp(actual.members[i].c_str(),name)){out=static_cast<std::int32_t>(i);break;}
 }else{
  auto klass=classes_.find(type);if(klass!=classes_.end()){
   const auto& actual=groups_.at(klass->second);
   if(!actual.declared_rows){error.clear();return true;} //Original Array.GetMemberID returns-1 for its genuine C1/loaded empty size.
   if(!actual.names_loaded){error="Required actual source array names receiver";return false;}
   const auto found=actual.name_index.find(name);if(found!=actual.name_index.end())out=found->second;
  }
 }
 error.clear();return true;
}
bool SourceProcessArraysV101::load_stage(const Read& read,bool& complete,std::string& error){
 complete=false;if(failed_){error=error_;return false;}
 if(!read){error="Required actual process PyData FileManager reader";return false;}
 if(stage38_==70){complete=true;error.clear();return true;}
 if(stage38_>70){error="Original PyDataArrays.Load phase domain";return false;}
 const auto& file=process_pydata_arrays_v101[stage38_];
 try{
  std::vector<std::uint8_t> bytes;if(!read(std::string("data/")+file.uri,bytes,error))throw std::runtime_error(error);
  Cursor stream{bytes};bool registered=false;
  for(const auto& entry:process_array_registration_v101){
   if(std::strcmp(entry.file,file.registration))continue;registered=true;auto& actual=groups_.at(entry.group);
   if(entry.names){
    //Both original readNames and skipNames consume their source names group.
    actual.names_loaded=false;actual.names.clear();actual.name_index.clear();actual.declared_names=stream.count();
    actual.names.reserve(actual.declared_names);
    for(std::uint32_t i=0;i<actual.declared_names;++i){actual.names.push_back(stream.string());actual.name_index.emplace(name_key(actual.names.back()),static_cast<std::int32_t>(i));}
    actual.names_loaded=true;
   }else{
    actual.records_loaded=false;actual.rows.clear();actual.declared_rows=stream.count();actual.rows.resize(actual.declared_rows);
    for(auto& row:actual.rows){const char* shape=entry.shape;stream.row(shape,row.fields);if(*shape)throw std::runtime_error("Incomplete source record descriptor");}
    actual.records_loaded=true;
   }
  }
  if(!registered||stream.at!=bytes.size())throw std::runtime_error("Required complete source file registration stream");
  //Original PyDataArrays.Load reads only the registered record/name streams.
  //Its Structs getters use the already-constructed global CString tables.
  //Bundled *_pystructnames files also contain nested/derived structs, and
  //their ordering is not the addFuncsForFile outer-array registration order.
  //They are neither a source runtime read nor the authority for those getters.
  ++stage38_;error.clear();return true;
 }catch(const std::exception& exception){failed_=true;error_=exception.what();error=error_;return false;}
}
}
