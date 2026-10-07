#include "canonical_property_map_v1.hpp"
#include <cstdlib>
#include <cstring>
#include <type_traits>
namespace dh2::world {
namespace {
CanonicalPropertyValueV1 zero_value(const CanonicalPropertyValueV1& v){return std::visit([](const auto& x)->CanonicalPropertyValueV1 {using T=std::decay_t<decltype(x)>;return T{};},v);}
bool parse(CanonicalPropertyValueV1& value,const char* text,std::string& error,const CanonicalPropertySourceServicesV1& source){
 if(!text){error="source FromString requires present CString";return false;}
 return std::visit([&](auto& out)->bool {
  using T=std::decay_t<decltype(out)>;
  if constexpr(std::is_same_v<T,std::string>)out=text;
  else if constexpr(std::is_same_v<T,std::uint8_t>)out=std::atoi(text)!=0;
  else if constexpr(std::is_same_v<T,std::int32_t>)out=std::atoi(text);
  else if constexpr(std::is_same_v<T,float>)out=static_cast<float>(std::strtod(text,nullptr));
  else if constexpr(std::is_same_v<T,CanonicalPoint3ListV1>){
   out.clear();if(!source.point3_list_parse_diagnostic){error="Required source vector<Point3D> debug diagnostic30f010";return false;}
   return source.point3_list_parse_diagnostic(source.context,"NEED TO CHECK Venkat Vikram ********** ********** ************",error);
  }else {
   // Actual StrToObj allocates256, strcpy, strtok comma; repeated delimiters
   // collapse, absent components retain the receiver's prior zero prefix.
   if(std::strlen(text)>255){error="source vector strcpy256 domain exceeded";return false;}
   const char* at=text;std::size_t index=0;
   while(*at&&index<out.size()){
    while(*at==',')++at;if(!*at)break;const char* begin=at;while(*at&&*at!=',')++at;std::string token(begin,at);
    if constexpr(std::is_same_v<T,std::array<float,3>>)out[index++]=static_cast<float>(std::strtod(token.c_str(),nullptr));
    else out[index++]=std::atoi(token.c_str());
   }
  }return true;
 },value);
}
}
bool CanonicalPropertyMapV1::write(CanonicalPropertyActorV1& actor,const CanonicalPropertyDescriptorV1& descriptor,const CanonicalPropertyValueV1& value,std::string& e){
 if(descriptor.default_value.index()!=value.index()){e="source descriptor value type mismatch";return false;}
 if(descriptor.source_offset==8){if(!actor.template_name||!std::holds_alternative<std::string>(value)){e="required SAME PropertyMap template_name field";return false;}*actor.template_name=std::get<std::string>(value);return true;}
 auto& f=actor.fields;const auto offset=descriptor.source_offset;
 bool available=true;bool result=std::visit([&](const auto& v)->bool {
  using T=std::decay_t<decltype(v)>;
  if constexpr(std::is_same_v<T,std::uint8_t>){available=f.write_bool!=nullptr;return available&&f.write_bool(f.context,offset,v,e);}
  else if constexpr(std::is_same_v<T,std::int32_t>){available=f.write_int!=nullptr;return available&&f.write_int(f.context,offset,v,e);}
  else if constexpr(std::is_same_v<T,float>){available=f.write_float!=nullptr;return available&&f.write_float(f.context,offset,v,e);}
  else if constexpr(std::is_same_v<T,std::string>){available=f.write_string!=nullptr;return available&&f.write_string(f.context,offset,v,e);}
  else if constexpr(std::is_same_v<T,std::array<float,3>>){available=f.write_vector3!=nullptr;return available&&f.write_vector3(f.context,offset,v,e);}
  else if constexpr(std::is_same_v<T,std::array<std::int32_t,2>>){available=f.write_point2!=nullptr;return available&&f.write_point2(f.context,offset,v,e);}
  else {available=f.write_point3_list!=nullptr;return available&&f.write_point3_list(f.context,offset,v,e);}
 },value);if(!available)e="required SAME actor source-offset typed property writer";return result;
}
CanonicalPropertyMapV1::Descriptors* CanonicalPropertyMapV1::current(CanonicalPropertyActorV1& actor,std::string& e){
 if(!actor.class_name||!actor.template_name){e="required SAME source class/template name fields";return nullptr;}
 auto i=classes_.find(actor.class_name);if(i==classes_.end()){e="required source InitProperties/DeclareProperties";return nullptr;}
 return &i->second[*actor.template_name]; // actual shared map operator[]
}
bool CanonicalPropertyMapV1::init_properties(CanonicalPropertyActorV1& actor,std::string& e){
 if(!actor.class_name||!actor.template_name){e="required SAME source class/template name fields";return false;}
 if(auto failed=incomplete_.find(actor.class_name);failed!=incomplete_.end()){e=failed->second;return false;}
 if(classes_.find(actor.class_name)!=classes_.end())return true;
 const std::string kind=actor.class_name;
  if(kind!="ObjectBase"&&kind!="GameObject"&&kind!="Dummy"&&kind!="Decor"&&kind!="SpawnPoint"&&kind!="SpawnSpot"&&kind!="DestructibleContainer"&&kind!="TriggerZone"&&kind!="TriggerTrap"&&kind!="CheckpointZone"&&kind!="QuestMoveInZone"&&kind!="SoundEmitter"&&kind!="Door"&&kind!="TriggerObject"&&kind!="TriggerZoneExitLevel"&&kind!="Item"&&kind!="Character"&&kind!="Player"&&kind!="OpenableContainer"&&kind!="AnimatedDecor"&&kind!="Module"&&kind!="Block"&&kind!="LevelConfig"&&kind!="RoomZone"&&kind!="LightPoint"&&kind!="Projectile"&&kind!="LaserTypeProjectile"){e="required unrecovered registered class DeclareProperties";return false;}
 auto& base=classes_[kind][""];base.emplace("_templateName",CanonicalPropertyDescriptorV1{8,std::string{}});
 incomplete_[kind]="source DeclareProperties incomplete; candidate context must be released";
 std::uint8_t static_default{};
 if(!actor.fields.read_bool){e="required SAME constructor static84 field reader";return false;}
 if(!actor.fields.read_bool(actor.fields.context,0x84,static_default,e))return false;
 if(kind!="ObjectBase"&&kind!="LevelConfig"&&kind!="RoomZone"&&!source_.position_rotation_default){e="required source Point3D<float>::ZERO default producer";return false;}
 auto add=[&](const char* name,std::uint32_t offset,CanonicalPropertyValueV1 value){base[name]={offset,std::move(value)};};
  // Dummy factory3410a4 installs vptr whose DeclareProperties virtual18 is
  // exactly GameObject38cee8. Keep a distinct SAME class-map entry (and its
  // constructor-produced static default1); alias only this declaration body.
  const std::string declaration_kind=(kind=="Dummy"||kind=="Decor"||kind=="SpawnPoint"||kind=="SpawnSpot"||kind=="DestructibleContainer"||kind=="TriggerZone"||kind=="TriggerTrap"||kind=="CheckpointZone"||kind=="QuestMoveInZone"||kind=="SoundEmitter"||kind=="Door"||kind=="TriggerObject"||kind=="TriggerZoneExitLevel"||kind=="Projectile"||kind=="LaserTypeProjectile")?"GameObject":kind;
  {const auto& kind=declaration_kind;
 #include "canonical_property_declarations_v1.inc"
 #include "canonical_level_config_module_declarations_v1.inc"
  }
 // RoomZone::DeclareProperties396554 is bx lr: only the PropertyMap's
 // own _templateName declaration above exists; do not inherit GameObject.
 #include "canonical_swamp_family_declarations_v15.inc"
 if(kind=="SpawnSpot")add("group",0x374,std::string{});
 #include "canonical_destructible_declarations_v16.inc"
 #include "canonical_trigger_zone_declarations_v22.inc"
  #include "canonical_trigger_trap_declarations_v37.inc"
 if(kind=="CheckpointZone"||kind=="QuestMoveInZone")add("dimensions",0x374,std::array<float,3>{{200.0f,200.0f,200.0f}});
 if(kind=="Door"){add("dimensions",0x374,std::array<float,3>{{200.f,200.f,200.f}});add("data",0x388,std::string{});add("opened",0x3a4,std::uint8_t{1});add("door_collision",0x3a5,std::uint8_t{1});}
 if(kind=="TriggerObject"){
 add("dimensions",0x374,std::array<float,3>{{200.f,200.f,200.f}});
 add("triggercount",0x3a8,std::int32_t{1});add("triggerdelay",0x3ac,std::int32_t{0});add("resetonhubchange",0x3b0,std::uint8_t{0});
 add("data",0x718,std::string{});add("script",0x734,std::string{});add("scriptOff",0x750,std::string{});add("unlock_cond",0x76c,std::string{});
 }
 if(kind=="TriggerZoneExitLevel"){
 add("levelID",0x7d8,std::int32_t{-1});add("levelName",0x7dc,std::string{});
 add("entrypointID",0x7f4,std::int32_t{-1});add("maploc",0x7f8,std::int32_t{-1});add("question",0x7fc,std::int32_t{-1});add("fasttravel",0x800,std::string{});
 }
  if(kind=="SoundEmitter"){
  if(!source_.constant_integer){e="Required actual SoundEmitter PyDataConstants default producer";return false;}
  std::int32_t near{},far{};if(!source_.constant_integer(source_.constant_context,"AudioConstants","CameraReferenceDistance",near,e)||!source_.constant_integer(source_.constant_context,"AudioConstants","CameraMaxDistance",far,e))return false;
  add("loop",0x374,std::uint8_t{1});add("sound",0x378,std::string{});
  add("distMin",0x394,static_cast<float>(near));add("distMax",0x398,static_cast<float>(far));
 }
// Insert within actual CanonicalPropertyMapV1 init_properties declaration body.
// Also add LightPoint to recovered class whitelist; require ZERO producer.
if(kind=="LightPoint") {
 add("position",0x124,*source_.position_rotation_default);
 add("radius",0x130,float{2.0f});
 add("attenuation",0x134,std::array<float,3>{{0,0,0}});
 add("ambientColor",0x140,std::array<float,3>{{0,0,0}});
 add("diffuseColor",0x14c,std::array<float,3>{{0,0,0}});
 add("specularColor",0x158,std::array<float,3>{{0,0,0}});
 add("automatic",0x164,std::uint8_t{0});
 add("dae",0x168,std::string{});
 add("attachedTo",0x180,std::string{});
 add("attachedOffset",0x1a4,*source_.position_rotation_default);
}

incomplete_.erase(kind);
 return true;
}
bool CanonicalPropertyMapV1::set_template(CanonicalPropertyActorV1& actor,const char* name,std::string& e){
 if(!name||!actor.template_name){e="required source SetTemplate CString/field";return false;}
 if(!*name)return true; // source empty-string gate; preserve current template
 *actor.template_name=name;auto* target=current(actor,e);if(!target)return false;
 if(target->empty())*target=classes_[actor.class_name][""]; // source clones each descriptor
 if(!source_.load_template_assertion){e="required original LoadTemplate assertion(9) provider";return false;}
 return source_.load_template_assertion(source_.context,e);
}
bool CanonicalPropertyMapV1::load_defaults(CanonicalPropertyActorV1& actor,std::string& e){
 auto* descriptors=current(actor,e);if(!descriptors)return false;const auto saved=*actor.template_name;
 for(auto& [name,descriptor]:*descriptors)if(!write(actor,descriptor,descriptor.default_value,e))return false;
 *actor.template_name=saved;return true; // original saves/restores template string
}
bool CanonicalPropertyMapV1::set_property(CanonicalPropertyActorV1& actor,const char* name,const char* text,std::string& e){
 if(!name){e="source SetProperty requires descriptor name";return false;}auto* descriptors=current(actor,e);if(!descriptors)return false;
 auto i=descriptors->find(name);if(i==descriptors->end())return true; // actual null descriptor gate
 auto& descriptor=i->second;if(!text)return write(actor,descriptor,descriptor.default_value,e);
 // Explicit modern compatibility correction: the shipped editor-format
 // _templateName attribute selects a named map without a separate "template"
 // attribute. Raw string assignment would leave that map empty and discard
 // subsequent authored fields. Use the existing actual SetTemplate clone and
 // its required LoadTemplate assertion delivery, preserving XML iteration.
 if(descriptor.source_offset==8&&std::holds_alternative<std::string>(descriptor.default_value)&&*text)
  return set_template(actor,text,e);

 if(std::holds_alternative<std::string>(descriptor.default_value))return write(actor,descriptor,std::string{text},e);
 auto value=zero_value(descriptor.default_value);
 // Source FromString clears actual destination before scalar/vector parse.
 if(!write(actor,descriptor,value,e))return false;
 if(!parse(value,text,e,source_))return false;return write(actor,descriptor,value,e);
}
bool CanonicalPropertyMapV1::load_overrides(CanonicalPropertyActorV1& actor,const CanonicalSourceObjectRequestV1& request,std::string& e){
 if(!request.source_lease||!request.attribute){e="required retained XML element source";return false;}auto* descriptors=current(actor,e);if(!descriptors)return false;
 // Source caches iteration tree but resolves each SetProperty against current
 // template again, including after _templateName resets/overrides that field.
 for(auto& [name,descriptor]:*descriptors)if(!set_property(actor,name.c_str(),request.attribute(request.source_context,request.element,name.c_str()),e))return false;
 return true;
}
bool CanonicalPropertyMapV1::set_template_parameter(CanonicalPropertyActorV1& actor,const char* name,const char* text,std::string& e){
 if(!name||!text){e="source template parameter requires descriptor/text";return false;}auto* descriptors=current(actor,e);if(!descriptors)return false;
 auto i=descriptors->find(name);if(i==descriptors->end()){e="source null template descriptor outside safe native domain";return false;}
 i->second.default_value=zero_value(i->second.default_value);return parse(i->second.default_value,text,e,source_);
}
bool CanonicalPropertyMapV1::source_save_descriptors_v66(CanonicalPropertyActorV1& actor,std::vector<std::pair<std::string,CanonicalPropertyDescriptorV1>>& out,std::string& e){
 auto* descriptors=current(actor,e);if(!descriptors)return false;
 std::vector<std::pair<std::string,CanonicalPropertyDescriptorV1>> next;
 next.reserve(descriptors->size());for(const auto& row:*descriptors)next.push_back(row);
 out=std::move(next);return true;
}}

