#include "procedural_xml_emission_v66.hpp"
#include "procedural_process_scratch_v69.hpp"
#include "tinyxml.h"
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <functional>
#include <exception>
#include <type_traits>
namespace dh2::loader {
namespace {
using Value=world::CanonicalPropertyValueV1;
using Descriptor=world::CanonicalPropertyDescriptorV1;
using Rows=std::vector<std::pair<std::string,Descriptor>>;
bool missing(std::string& e){e="Required produced SAME registered Module/LevelConfig property field";return false;}
bool read_config(world::CanonicalLevelConfigV1& config,world::CanonicalPropertyActorV1& actor,const Descriptor& d,Value& out,std::string& e){
 if(d.source_offset==8){if(!actor.template_name)return missing(e);out=*actor.template_name;return true;}
 return std::visit([&](const auto& expected)->bool{
  using T=std::decay_t<decltype(expected)>;const auto offset=d.source_offset;
  if constexpr(std::is_same_v<T,std::uint8_t>){std::uint8_t value{};if(!actor.fields.read_bool||!actor.fields.read_bool(actor.fields.context,offset,value,e))return false;out=value;}
  else if constexpr(std::is_same_v<T,std::int32_t>){auto p=config.integer(offset);if(!p)return missing(e);out=*p;}
  else if constexpr(std::is_same_v<T,float>){auto p=config.scalar_float_v55(offset);if(!p)return missing(e);out=*p;}
  else if constexpr(std::is_same_v<T,std::string>){auto p=config.string(offset);if(!p)return missing(e);out=*p;}
  else if constexpr(std::is_same_v<T,std::array<float,3>>){auto p=config.vector(offset);if(!p)return missing(e);out=*p;}
  else if constexpr(std::is_same_v<T,world::CanonicalPoint3ListV1>){if(offset!=0x204)return missing(e);out=config.dfog_colors();}
  else return missing(e);return true;
 },d.default_value);
}
bool read_module(world::CanonicalModuleV1& module,world::CanonicalPropertyActorV1& actor,const Descriptor& d,Value& out,std::string& e){
 if(d.source_offset==8){if(!actor.template_name)return missing(e);out=*actor.template_name;return true;}
 return std::visit([&](const auto& expected)->bool{
  using T=std::decay_t<decltype(expected)>;const auto offset=d.source_offset;auto& base=module.base();
  if constexpr(std::is_same_v<T,std::uint8_t>){std::uint8_t value{};if(!actor.fields.read_bool||!actor.fields.read_bool(actor.fields.context,offset,value,e))return false;out=value;}
  else if constexpr(std::is_same_v<T,std::int32_t>){auto p=base.integer(offset);if(!p)return missing(e);out=*p;}
  else if constexpr(std::is_same_v<T,float>){auto p=base.scalar(offset);if(!p)return missing(e);out=*p;}
  else if constexpr(std::is_same_v<T,std::string>){const auto& xml=module.xml();const std::string* p=nullptr;
   switch(offset){case 0x378:p=&xml.mgp378;break;case 0x390:p=&xml.mvp390;break;case 0x3a8:p=&xml.alt_mgp3a8;break;case 0x3c0:p=&xml.alt_mvp3c0;break;case 0x3d8:p=&xml.alt_prob3d8;break;default:p=base.string(offset);}
   if(!p)return missing(e);out=*p;
  }else if constexpr(std::is_same_v<T,std::array<float,3>>){
   if(offset==0x3f0)out=module.fog_color();else{const auto p=base.vector3(offset);if(!p)return missing(e);out=std::array<float,3>{p[0],p[1],p[2]};}
  }else return missing(e);return true;
 },d.default_value);
}
bool property_text(const Value& value,ProceduralXmlServicesV66& services,std::string& out,std::string& e){
 // Strings copy their source CString directly. EVERY other SimpleTypeProperty
 // allocates256 before ObjToStr, including helpers which leak that allocation.
 // Preserve the reached allocation order; engine supplies genuine heap owner.
 if(std::holds_alternative<std::string>(value)){out=std::get<std::string>(value);return true;}
 PropertyScratchV66 scratch;
 // Existing genuine Main memory domain takes precedence. Otherwise the
 // loader owns original CustomAlloc->malloc and its process-lifetime storage.
 if(services.allocate_property_scratch256){if(!services.allocate_property_scratch256(scratch,e))return false;}
 else if(!allocate_process_property_scratch256_v69(scratch,e))return false;
 if(!scratch.owner||!scratch.bytes||scratch.size<256){e="Required retained original property scratch allocation256";return false;}
 return std::visit([&](const auto& actual)->bool{
  using T=std::decay_t<decltype(actual)>;
  if constexpr(std::is_same_v<T,std::string>){return false;}
  else if constexpr(std::is_same_v<T,world::CanonicalPoint3ListV1>){
   // Original vector helper is BX LR: bytes remain exactly as allocated.
   const auto end=std::find(scratch.bytes,scratch.bytes+256,std::uint8_t{0});
   if(end==scratch.bytes+256){e="Original no-op property string lacks NUL within allocation256; unsafe source branch unresolved";return false;}
   out.assign(reinterpret_cast<const char*>(scratch.bytes),std::size_t(end-scratch.bytes));return true;
  }else{
   auto* buffer=reinterpret_cast<char*>(scratch.bytes);int count=0;
   if constexpr(std::is_same_v<T,std::uint8_t>||std::is_same_v<T,std::int32_t>)count=std::snprintf(buffer,256,"%d",int(actual));
   else if constexpr(std::is_same_v<T,float>)count=std::snprintf(buffer,256,"%f",double(actual));
   else if constexpr(std::is_same_v<T,std::array<float,3>>)count=std::snprintf(buffer,256,"%f,%f,%f",double(actual[0]),double(actual[1]),double(actual[2]));
   else if constexpr(std::is_same_v<T,std::array<std::int32_t,2>>)count=std::snprintf(buffer,256,"%d,%d",int(actual[0]),int(actual[1]));
   if(count<0||std::size_t(count)>=256){e="Original property format exceeds allocated256 domain";return false;}
   out.assign(buffer,std::size_t(count));return true;
  }
 },value);
}
template<class Read>
bool save_properties(TiXmlElement& parent,world::CanonicalPropertyActorV1 actor,ProceduralXmlServicesV66& services,Read read,std::string& e){
 Rows descriptors;if(!services.properties->source_save_descriptors_v66(actor,descriptors,e))return false;
 auto element=std::make_unique<TiXmlElement>("GameObject");
 for(const auto& row:descriptors){Value current;std::string text;
  if(!read(actor,row.second,current,e)||!property_text(current,services,text,e))return false;
  // Source5135b4 emits ALL nonNULL registered descriptors in map key order.
  element->SetAttribute(row.first.c_str(),text.c_str());
 }
 parent.LinkEndChild(element.release());return true;
}
}
bool load_original_rule_config_v66(const XmlDocumentV1::Borrow& document,std::uint32_t element,world::CanonicalLevelConfigV1& config,ProceduralXmlServicesV66& services,std::string& e){
 if(!document||element>=document.elements().size()||!services.properties){e="Required actual rules root and registered property map";return false;}
 auto actor=config.properties();Rows descriptors;if(!services.properties->source_save_descriptors_v66(actor,descriptors,e))return false;
 for(const auto& row:descriptors){const auto* attribute=document.elements()[element].attribute(row.first);const char* text=attribute?attribute->c_str():nullptr;
  if(row.first=="_templateName"){
   // Original SetProperty writes CString directly. The shared game's modern
   // _templateName compatibility clone is not invoked by this native producer.
   if(!actor.template_name||!std::holds_alternative<std::string>(row.second.default_value))return missing(e);
   *actor.template_name=text?std::string(text):std::get<std::string>(row.second.default_value);
  }else if(!services.properties->set_property(actor,row.first.c_str(),text,e))return false;
 }
 return true;
}
bool save_modular_level_to_bytes_v66(const ProceduralModulePlanV1& plan,world::CanonicalLevelConfigV1& config,const std::string& filename,ProceduralXmlServicesV66& services,std::vector<std::uint8_t>& out,std::string& e){
 if(!services.actual_runtime_owner||!services.properties||!services.module_globals||!plan.layout.source_owner||plan.modules.size()!=plan.layout.tiles.size()||plan.layout.generated!=!plan.layout.tiles.empty()){e="Required actual generator layout/registered temporary receivers";return false;}
 if(!plan.reference_repairs.empty()){e="Inspection reference repairs cannot become original generated stream";return false;}
 try{
  TiXmlDocument document;
  document.LinkEndChild(new TiXmlDeclaration("1.0","utf-8",""));
  // Original fixed64 copy includes the trailing C terminator in comment value;
  // TiXML visitor prints CString, therefore the exact visible text ends here.
  document.LinkEndChild(new TiXmlComment(" This file was generated by the Dungeon Hunter random generator "));
  document.LinkEndChild(new TiXmlComment(" Software version 0.01 "));
  const auto slash=filename.rfind('/');const auto source=slash==filename.npos?filename:filename.substr(slash+1);
  char comment[128];const auto written=std::snprintf(comment,sizeof comment," Source file: %s ",source.c_str());
  if(written<0||std::size_t(written)>=sizeof comment){e="Original source-file comment format buffer exceeded";return false;}
  document.LinkEndChild(new TiXmlComment(comment));
  auto* level=new TiXmlElement("Level");document.LinkEndChild(level);
  if(!save_properties(*level,config.properties(),services,[&](auto& actor,const auto& d,auto& value,auto& error){return read_config(config,actor,d,value,error);},e))return false;
  std::vector<bool> visited(plan.layout.tiles.size());std::uint32_t next_index=0;
  std::function<bool(std::uint32_t,unsigned)> emit=[&](std::uint32_t tile,unsigned depth){
   if(depth>256||tile>=plan.modules.size()||visited[tile]||plan.modules[tile].tile!=next_index){e="Original tile preorder/index ownership inconsistent";return false;}
   visited[tile]=true;++next_index;
   actor::RuntimeState runtime{};world::CanonicalModuleV1 module(services.actual_runtime_owner,runtime,*services.module_globals,{},{});
   module.base().class_name20()="Module";auto properties=module.properties();
   if(!services.properties->init_properties(properties,e)||!services.properties->load_defaults(properties,e))return false;
   // Sequential original SetProperty calls include meaningful NULL setters.
   for(const auto& setter:plan.modules[tile].setter_attempts)
    if(!services.properties->set_property(properties,setter.name.c_str(),setter.value?setter.value->c_str():nullptr,e))return false;
   if(!save_properties(*level,properties,services,[&](auto& actor,const auto& d,auto& value,auto& error){return read_module(module,actor,d,value,error);},e))return false;
   for(const auto child:plan.layout.tiles[tile].children)if(!emit(child,depth+1))return false;
   return true;
  };
  if(plan.layout.generated&&!emit(0,0))return false;
  if(next_index!=plan.modules.size()){e="Generated tile ownership has disconnected/unserialized nodes";return false;}
  TiXmlPrinter printer;printer.SetLineBreak("\r\n");
  if(!document.Accept(&printer)){e="Actual TinyXML printer delivery failed";return false;}
  const auto* bytes=reinterpret_cast<const std::uint8_t*>(printer.CStr());
  std::vector<std::uint8_t> next(bytes,bytes+printer.Size());out=std::move(next);return true;
 }catch(const std::exception& ex){if(e.empty())e=ex.what();return false;}
}
}
