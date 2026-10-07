#include "character_game_design.hpp"
#include <cstring>
#include <limits>
#include <stdexcept>

namespace dh2::character {
namespace {
struct ConstantsDelete {void operator()(dh2_script_constants* p)const{dh2_script_constants_destroy(p);}};
using Constants=std::unique_ptr<dh2_script_constants,ConstantsDelete>;
using Raw=std::vector<std::uint8_t>;
data::Bytes bytes(const Raw& raw){return {raw.data(),raw.size()};}
Raw copy(data::Bytes input){
 if(!input.data||input.size>8u*1024u*1024u)throw std::runtime_error("Invalid design input stream");
 return Raw(input.data,input.data+input.size);
}
std::vector<std::vector<std::string>> sections(const Raw& raw){
 std::size_t at=0;
 auto word=[&](){if(raw.size()-at<4)throw std::runtime_error("Truncated design names");auto* p=raw.data()+at;at+=4;return std::uint32_t(p[0])|(std::uint32_t(p[1])<<8)|(std::uint32_t(p[2])<<16)|(std::uint32_t(p[3])<<24);};
 std::vector<std::vector<std::string>> result;
 while(at<raw.size()){
  auto count=word();if(count>65536)throw std::runtime_error("Design name count outside limit");
  result.emplace_back();auto& values=result.back();values.reserve(count);
  for(std::uint32_t i=0;i<count;++i){auto n=word();if(n>4096||n>raw.size()-at)throw std::runtime_error("Design name outside limit");values.emplace_back(reinterpret_cast<const char*>(raw.data()+at),n);at+=n;}
 }
 return result;
}
const char* const original_registered_names[]={
#include "reference/character-game-design/registered_names.inc"
};
}
struct CharacterGameDesign::Snapshot {
 std::array<std::array<Raw,3>,5> inputs;
 std::vector<Raw> constant_inputs;
 data::CharacterTable characters;
 data::ClassTables classes;
 data::AiTables ai;
 data::LevelTables levels;
 data::PropertyRules rules;
 std::vector<data::ClassRow> class_rows;
 std::vector<std::string> ai_fields,class_fields;
 std::array<std::vector<const char*>,6> name_pointers;
 std::array<data::DesignNames16,6> names{};
 std::array<GameDesignRegistration32,6> registrations{};
 data::GameDesignTables tables;
 data::DesignRegistry16 registry{};
 Constants constants{dh2_script_constants_create()};
 std::vector<GameDesignConstantLoad24> constant_loads;
 dh2_script_design_bindings design{this,&lookup,0};
 static int lookup(void* opaque,std::uint32_t kind,const char* group,const char* key,std::int32_t* out){
  auto* s=static_cast<Snapshot*>(opaque);
  if(!s||!group||!key||!out)return 1;
  if(kind==0)return dh2_script_constants_get(s->constants.get(),group,key,out);
  if(kind!=1)return 1;
  bool supported=false;
  for(const auto& r:s->registrations)if(!std::strcmp(r.group,group)){supported=true;break;}
  if(!supported)for(const char* name:original_registered_names)if(!std::strcmp(name,group))return 1;
  return dh2_game_design_tables_lookup(&s->registry,kind,group,key,out);
 }
 void finish_names(std::string& error){
  const std::vector<std::string>* lists[]={&ai_fields,&ai.names,&class_fields,&classes.names,&characters.fields,&characters.names};
  const char* groups[]={"AIProps","AITable","ClassFuncList","ClassTable","CharacterProperties","CharacterTable"};
  const std::uint32_t calls[]={0x4be600,0x4be618,0x4be834,0x4be84c,0x4be89c,0x4be8b4};
  const std::uint32_t getters[]={0x4af590,0x4af51c,0x4af1e0,0x4af16c,0x4af110,0x3fa188};
  for(unsigned i=0;i<6;++i){
   auto& pointers=name_pointers[i];for(const auto& name:*lists[i])pointers.push_back(name.c_str());
   names[i]={pointers.data(),static_cast<std::uint32_t>(pointers.size()),0};
   registrations[i]={groups[i],calls[i],getters[i],&names[i],0};
   if(!tables.register_table(groups[i],&names[i],error))throw std::runtime_error(error);
  }
  registry=tables.view();
 }
};
bool CharacterGameDesign::initialize(const GameDesignInputs256& input,std::string& error){
 error.clear();
 if(snapshot_&&snapshot_.use_count()!=1){error="Design snapshot has live borrowers";return false;}
 if(input.reserved||input.constant_count>4096||(input.constant_count&&!input.constants)){error="Invalid design input descriptor";return false;}
 try{
  const GameDesignTableInput48* sources[]={&input.characters,&input.classes,&input.ai,&input.factions,&input.levels};
  std::size_t total=0;
  auto preflight=[&](data::Bytes stream){if(!stream.data||stream.size>8u*1024u*1024u)throw std::runtime_error("Invalid design input stream");total+=stream.size;if(total>64u*1024u*1024u)throw std::runtime_error("Design snapshot size outside limit");};
  for(auto* source:sources)for(auto stream:{source->records,source->names,source->schema})preflight(stream);
  for(std::uint32_t i=0;i<input.constant_count;++i)preflight(input.constants[i]);
  auto next=std::make_shared<Snapshot>();if(!next->constants)throw std::runtime_error("Constant map allocation failed");
  for(unsigned i=0;i<5;++i){const data::Bytes fields[]={sources[i]->records,sources[i]->names,sources[i]->schema};for(unsigned j=0;j<3;++j)next->inputs[i][j]=copy(fields[j]);}
  for(std::uint32_t i=0;i<input.constant_count;++i)next->constant_inputs.push_back(copy(input.constants[i]));
  auto stream=[&](unsigned i,unsigned j){return bytes(next->inputs[i][j]);};
  if(!data::load_characters(stream(0,0),stream(0,1),stream(0,2),next->characters,error)
   ||!data::load_classes(stream(1,0),stream(1,1),stream(1,2),next->classes,error)
   ||!data::load_ai(stream(2,0),stream(2,1),stream(2,2),stream(3,0),stream(3,1),stream(3,2),next->ai,error)
   ||!data::load_levels(stream(4,0),stream(4,1),stream(4,2),next->levels,error)
   ||!data::load_property_rules(next->characters,next->rules,error))return false;
  const auto ai_schema=sections(next->inputs[2][2]),class_schema=sections(next->inputs[1][2]);
  if(ai_schema.size()!=1||class_schema.size()<2)throw std::runtime_error("Missing design schema sections");
  next->ai_fields=ai_schema[0];next->class_fields=class_schema[1];
  for(const auto& row:next->classes.rows)next->class_rows.push_back({row.data(),static_cast<std::uint32_t>(row.size())});
  for(const auto& raw:next->constant_inputs){
   GameDesignConstantLoad24 load{};load.bytes=static_cast<std::uint32_t>(raw.size());
   load.status=dh2_script_constants_load(next->constants.get(),raw.data(),load.bytes,&load.source);
   if(load.status<0)throw std::runtime_error("Constant stream delivery failed");
   next->constant_loads.push_back(load);
  }
  next->finish_names(error);snapshot_=std::move(next);return true;
 }catch(const std::exception& failure){error=failure.what();return false;}
}
#define VIEW(method,field,type) const type* CharacterGameDesign::Borrow::method()const{return snapshot_?&snapshot_->field:nullptr;}
VIEW(characters,characters,data::CharacterTable)
VIEW(classes,classes,data::ClassTables)
VIEW(ai,ai,data::AiTables)
VIEW(levels,levels,data::LevelTables)
VIEW(rules,rules,data::PropertyRules)
VIEW(class_rows,class_rows,std::vector<data::ClassRow>)
VIEW(constant_loads,constant_loads,std::vector<GameDesignConstantLoad24>)
VIEW(design,design,dh2_script_design_bindings)
#undef VIEW
const std::array<GameDesignRegistration32,6>* CharacterGameDesign::Borrow::registrations()const{return snapshot_?&snapshot_->registrations:nullptr;}
bool CharacterGameDesign::Borrow::level_model(LevelModel32& out,data::PropertyView& properties,std::string& error)const{
 error.clear();if(!snapshot_||dh2_property_validate(&properties)){error="Invalid borrowed level properties";return false;}
 out={&properties,snapshot_->class_rows.data(),static_cast<std::uint32_t>(snapshot_->class_rows.size()),0,&snapshot_->design};return true;
}
int CharacterGameDesign::Borrow::bind(dh2_script_vm* vm)const{return snapshot_?dh2_script_design_bind(vm,&snapshot_->design):-1;}
const char* CharacterGameDesign::Borrow::constant_name_v97(const char* group,std::int32_t value)const{
 return snapshot_?dh2_script_constants_name(snapshot_->constants.get(),group,value):nullptr;
}
}
