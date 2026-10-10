#include "source_process_arrays_v101.hpp"
#include "source_process_compiled_members_v121.hpp"
#include <cstring>
#include <iomanip>
#include <sstream>
#include <stdexcept>
namespace dh2::android_ui {
namespace {
std::string stream_offset(std::size_t at){std::ostringstream out;out<<"0x"<<std::hex<<at;return out.str();}
struct Cursor {
 const std::vector<std::uint8_t>& bytes;std::size_t at{};
 void require(std::size_t n){if(at>bytes.size()||n>bytes.size()-at)throw std::runtime_error("Short process PyData array stream at byte offset "+stream_offset(at)+" (need "+std::to_string(n)+" bytes)");}
 std::uint32_t integer(unsigned n=4){require(n);std::uint32_t v{};for(unsigned i=0;i<n;++i)v|=std::uint32_t(bytes[at++])<<(8*i);return v;}
 std::string string(){const auto length_at=at;const auto n=integer();if(n>1000000)throw std::runtime_error("Process PyData string length "+std::to_string(n)+" exceeds native allocation bound at byte offset "+stream_offset(length_at));require(n);std::string out(reinterpret_cast<const char*>(bytes.data()+at),n);at+=n;return out;}
 std::uint32_t count(){const auto count_at=at;auto n=integer();if(n>1000000)throw std::runtime_error("Process PyData count "+std::to_string(n)+" exceeds native allocation bound at byte offset "+stream_offset(count_at));return n;}
 void require_records(std::uint32_t n,const char* shape){
  std::size_t minimum=0;unsigned nested=0;
  for(const char* part=shape;*part;++part){
   if(*part=='['){if(nested++==0)minimum+=4;}
   else if(*part==']'){if(!nested)break;--nested;}
   else if(!nested){switch(*part){case 'b':++minimum;break;case 'h':minimum+=2;break;
    case 'i':case 'f':case 'S':minimum+=4;break;default:throw std::runtime_error("Invalid source record descriptor");}}
  }
  require(0);
  if(minimum&&n>(bytes.size()-at)/minimum)
   throw std::runtime_error("Process PyData record count "+std::to_string(n)+" cannot fit remaining bytes at offset "+stream_offset(at));
 }
 void row(const char*& shape,std::vector<ProcessArrayValueV101>& fields,unsigned depth=0){
  if(depth>32)throw std::runtime_error("Process PyData nested record bound at byte offset "+stream_offset(at));
  while(*shape&&*shape!=']'){
   fields.emplace_back();auto& field=fields.back();const char type=*shape++;
   switch(type){
   case 'i':field.kind=ProcessArrayValueV101::Kind::word;field.bits=integer();break;
   case 'f':field.kind=ProcessArrayValueV101::Kind::float32;field.bits=integer();break;
   case 'b':field.kind=ProcessArrayValueV101::Kind::byte;field.bits=integer(1);break;
   case 'h':field.kind=ProcessArrayValueV101::Kind::half;field.bits=integer(2);break;
   case 'S':field.kind=ProcessArrayValueV101::Kind::string;field.text=string();break;
   case '[':{
    field.kind=ProcessArrayValueV101::Kind::vector;field.bits=count();
    const char* begin=shape;const char* end=shape;unsigned nesting=1;
    while(*end&&nesting){if(*end=='[')++nesting;else if(*end==']')--nesting;if(nesting)++end;}
    if(!*end)throw std::runtime_error("Invalid source nested wire descriptor at byte offset "+stream_offset(at));
    require_records(field.bits,begin);field.elements.resize(field.bits);
    for(auto& element:field.elements){const char* part=begin;row(part,element.fields,depth+1);if(part!=end)throw std::runtime_error("Process PyData nested descriptor mismatch at byte offset "+stream_offset(at));}
    shape=end+1;break;
   }
   default:throw std::runtime_error(std::string("Required original Structs.read wire operation for descriptor '")+type+"' at byte offset "+stream_offset(at));
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
  const ProcessCompiledStructV121* source{};unsigned matches=0;
  for(const auto& actual:process_compiled_structs_v121)if(!std::strcmp(actual.type,entry.type)){source=&actual;++matches;}
  if(matches!=1||!source||!source->source_names||source->count!=entry.count||source->getter!=entry.getter)
   throw std::runtime_error(std::string("Required exact compiled Structs C1 descriptor: ")+entry.type);
  auto& group=groups_.at(entry.group);
  group.members.reserve(source->count);
  for(std::uint32_t i=0;i<source->count;++i){
   if(!source->members[i])throw std::runtime_error(std::string("Required compiled Structs member CString: ")+entry.type);
   group.members.emplace_back(source->members[i]);
  }
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
 std::string active_group="(unmatched registration)";
 try{
  std::vector<std::uint8_t> bytes;if(!read(std::string("data/")+file.uri,bytes,error))throw std::runtime_error(error);
  Cursor stream{bytes};bool registered=false;
  for(const auto& entry:process_array_registration_v101){
   if(std::strcmp(entry.file,file.registration))continue;registered=true;active_group=entry.group;auto& actual=groups_.at(entry.group);
   if(entry.names){
    //Both original readNames and skipNames consume their source names group.
    actual.names_loaded=false;actual.names.clear();actual.name_index.clear();actual.declared_names=stream.count();
    stream.require_records(actual.declared_names,"S");
    actual.names.reserve(actual.declared_names);
    for(std::uint32_t i=0;i<actual.declared_names;++i){actual.names.push_back(stream.string());actual.name_index.emplace(name_key(actual.names.back()),static_cast<std::int32_t>(i));}
    actual.names_loaded=true;
   }else{
    actual.records_loaded=false;actual.rows.clear();actual.declared_rows=stream.count();stream.require_records(actual.declared_rows,entry.shape);actual.rows.resize(actual.declared_rows);
    for(auto& row:actual.rows){const char* shape=entry.shape;stream.row(shape,row.fields);if(*shape)throw std::runtime_error(std::string("Incomplete source record descriptor at byte offset ")+stream_offset(stream.at));}
    actual.records_loaded=true;
   }
  }
  if(!registered)throw std::runtime_error("No registered source reader for this stream at byte offset 0x0");
  if(stream.at!=bytes.size())throw std::runtime_error("Required complete source file registration stream; next byte offset "+stream_offset(stream.at)+", trailing bytes "+std::to_string(bytes.size()-stream.at));
  //Original PyDataArrays.Load reads only the registered record/name streams.
  //Its Structs getters use the already-constructed global CString tables.
  //Bundled *_pystructnames files also contain nested/derived structs, and
  //their ordering is not the addFuncsForFile outer-array registration order.
  //For example, AIFactions has the one compiled member "factions", while
  //its registered [ii] row shape describes that member's nested Id/Value data.
  //Those are independent authorities: cache schema order cannot replace either.
  ++stage38_;error.clear();return true;
 }catch(const std::exception& exception){failed_=true;error_="Process PyData file=data/"+std::string(file.uri)+" group="+active_group+": "+exception.what();error=error_;return false;}
}
}
