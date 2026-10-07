#include "player_light_tweaker_owner_v90.hpp"
#include <cmath>
#include <limits>
#include <exception>
namespace dh2::application {
namespace {
bool utf16_xml(const std::vector<std::uint8_t>& input,std::vector<std::uint8_t>& out,std::string& e){
 if(input.size()<2||input[0]!=0xff||input[1]!=0xfe||(input.size()&1)){e="Required original UTF16LE tweaker XML transport";return false;}
 auto emit=[&](std::uint32_t c){if(c<0x80)out.push_back(static_cast<std::uint8_t>(c));else if(c<0x800){out.push_back(0xc0|(c>>6));out.push_back(0x80|(c&63));}else if(c<0x10000){out.push_back(0xe0|(c>>12));out.push_back(0x80|((c>>6)&63));out.push_back(0x80|(c&63));}else{out.push_back(0xf0|(c>>18));out.push_back(0x80|((c>>12)&63));out.push_back(0x80|((c>>6)&63));out.push_back(0x80|(c&63));}};
 for(std::size_t i=2;i<input.size();i+=2){std::uint32_t c=input[i]|(std::uint32_t(input[i+1])<<8);
  if(!c){e="Original tweaker XML contains embedded NUL outside read domain";return false;}
  if(c>=0xd800&&c<=0xdbff){if(i+3>=input.size()){e="Incomplete tweaker UTF16 surrogate";return false;}const auto low=input[i+2]|(std::uint32_t(input[i+3])<<8);if(low<0xdc00||low>0xdfff){e="Invalid tweaker UTF16 surrogate";return false;}c=0x10000+((c-0xd800)<<10)+(low-0xdc00);i+=2;}
  else if(c>=0xdc00&&c<=0xdfff){e="Unpaired tweaker UTF16 surrogate";return false;}emit(c);
 }return true;
}
bool source_f2iz(float value,const PlayerLightTweakServicesV90& s,std::int32_t& out,std::string& e){
 // Ordinary finite domain is exact truncation. Do not use a C++ undefined
 // cast for the source libgcc NaN/overflow branch.
 if(std::isfinite(value)&&double(value)>=double(INT32_MIN)&&double(value)<=double(INT32_MAX)){out=static_cast<std::int32_t>(value);return true;}
 if(s.exceptional_float_to_int)return s.exceptional_float_to_int(value,out,e);
 e="Required actual __aeabi_f2iz for exceptional tweaker distance";return false;
}
}
PlayerLightTweakerOwnerV90::~PlayerLightTweakerOwnerV90(){std::string ignored;close(ignored);}
bool PlayerLightTweakerOwnerV90::register_cell(const std::string& name,std::uint32_t type,std::uint32_t offset,void* cell,std::string&){
 // registerVariableName32bb98 appends only before a new map registration.
 if(mappings4_.find(name)==mappings4_.end())group1c_.variables.push_back(name);
 mappings4_[name]={type,offset,cell};return true;
}
bool PlayerLightTweakerOwnerV90::construct(const Read& read,std::string& e){
 if(attempted_||closed_||busy_){e="Tweaker C1 cannot replay";return false;}
 if(!read){e="Required genuine tweaker source file transport";return false;}
 attempted_=true;busy_=true;struct Guard{bool& b;~Guard(){b=false;}}guard{busy_};
 try{
  for(std::size_t i=0;i<5;++i){const auto index="["+std::to_string(i)+"]";
   for(std::size_t j=0;j<3;++j)if(!register_cell("m_attenuation"+index+"."+std::string(1,"XYZ"[j]),1,0x7c+12*i+4*j,&attenuation7c_[i][j],e))return false;
   if(!register_cell("m_ambientColor"+index,5,0xb8+4*i,ambientb8_[i].data(),e)||!register_cell("m_diffuseColor"+index,5,0xcc+4*i,diffusecc_[i].data(),e)||!register_cell("m_specularColor"+index,5,0xe0+4*i,speculare0_[i].data(),e))return false;
  }
  if(!register_cell("m_fogColor",5,0xf4,fogf4_.data(),e)||!register_cell("m_fogStart",1,0x110,&start110_,e)||!register_cell("m_fogEnd",1,0x114,&end114_,e))return false;
  for(std::size_t j=0;j<3;++j)if(!register_cell("m_fogDirectionMask."+std::string(1,"XYZ"[j]),1,0x11c+4*j,&direction11c_[j],e))return false;
  if(!register_cell("m_lightType",2,0xf8,&light_typef8_,e))return false;
  const std::string uri="data/tweaker/player_light.tweaker_xml";bool found{};std::vector<std::uint8_t> bytes;
  if(!read(uri,found,bytes,e))return false;
  if(found){
   std::vector<std::uint8_t> utf8;if(!utf16_xml(bytes,utf8,e))return false;
   std::shared_ptr<loader::XmlDocumentV1::NativeBackendV65> backend;
   if(!loader::XmlDocumentV1::allocate_native_backend_v65(backend,e))return false;
   struct Release{std::shared_ptr<loader::XmlDocumentV1::NativeBackendV65> b;~Release(){b->release_native();}}release{backend};
   loader::XmlDocumentV1 document;if(!document.capture_retained_level_buffer_v65(uri,std::move(utf8),backend,e))return false;
   auto xml=document.borrow();if(!xml.parsed()){e="Malformed genuine tweaker XML; no authored defaults applied";return false;}
   const auto& elements=xml.elements();
   if(xml.roots().empty()||elements[xml.roots().front()].tag!="attributes"){e="Required original CXMLAttributes root";return false;}
   const auto root=xml.roots().front();
   for(auto child:elements[root].children){const auto& node=elements[child];const auto* name=node.attribute("name");if(node.tag=="string"&&name&&*name=="Tweakable"){const auto* value=node.attribute("value");if(value&&!value->empty())name54_=*value;break;}}
   // local32b650 visits source attribute groups recursively. Its body never
   // calls setValue and never mutates this Tweaker registration group.
   std::function<void(std::uint32_t)> visit=[&](std::uint32_t at){for(auto child:elements[at].children)if(elements[child].tag=="group")visit(child);};visit(root);
  }
  constructed_=true;e.clear();return true;
 }catch(const std::exception& ex){e=ex.what();return false;}
}
bool PlayerLightTweakerOwnerV90::assign_light_type_after_publication(const std::string& value,std::string& e){
 if(!constructed()){e="Required published complete PlayerLightTweaker C1";return false;}if(published_label_){e.clear();return true;}try{light_typef8_=value;published_label_=true;e.clear();return true;}catch(const std::exception& ex){e=ex.what();return false;}
}
bool PlayerLightTweakerOwnerV90::source_assign_light_v113(std::uint32_t index,std::uintptr_t actor,const std::array<float,3>& attenuation,const std::array<float,3>& ambient,const std::array<float,3>& diffuse,const std::array<float,3>& specular,std::string& e){
 if(!constructed()){e="Actual AssignTweaker requires SAME constructed App58..64 owner";return false;}
 if(index>4){e.clear();return true;} //40b844 original unsigned range returns before stores.
 lights128_[index]=actor;attenuation7c_[index]=attenuation;
 const auto byte=[](float v){const float scaled=v*255.f;std::int32_t integer;if(std::isnan(scaled))integer=0;else if(scaled>=2147483648.f)integer=INT32_MAX;else if(scaled<=-2147483648.f)integer=INT32_MIN;else integer=static_cast<std::int32_t>(scaled);return static_cast<std::uint8_t>(integer);};
 for(auto pair:{std::pair<std::array<std::uint8_t,4>*,const std::array<float,3>*>{&ambientb8_[index],&ambient},{&diffusecc_[index],&diffuse},{&speculare0_[index],&specular}}){for(unsigned i=0;i<3;++i)(*pair.first)[i]=byte((*pair.second)[i]);(*pair.first)[3]=255;}
 e.clear();return true;
}
bool PlayerLightTweakerOwnerV90::set_value(const TweakAttributesV90& attrs,std::int32_t index,const PlayerLightTweakServicesV90& s,std::string& e){
 if(!constructed()||busy_||!attrs.owner||!attrs.name){e="Required SAME live tweaker/native attributes; recursive setter rejected";return false;}
 busy_=true;struct Guard{bool& b;~Guard(){b=false;}}guard{busy_};
 try{std::string name;if(!attrs.name(index,name,e))return false;const auto at=mappings4_.find(name);if(at==mappings4_.end()){e.clear();return true;}const auto map=at->second;
  if(map.type==1){float value;if(!attrs.float_value||!attrs.float_value(index,value,e))return false;*static_cast<float*>(map.cell)=value;if(map.offset==0x110)start_produced_=true;if(map.offset==0x114)end_produced_=true;}
  else if(map.type==2){std::string value;if(!attrs.string_value||!attrs.string_value(index,value,e))return false;*static_cast<std::string*>(map.cell)=std::move(value);}
  else if(map.type==5){std::uint32_t value;if(!attrs.color_value||!attrs.color_value(index,value,e))return false;auto* dest=static_cast<std::uint8_t*>(map.cell);for(unsigned i=0;i<4;++i)dest[i]=static_cast<std::uint8_t>(value>>(8*i));if(map.offset==0xf4)color_produced_=true;}
  else{e="Unreconstructed mapping enum outside this PlayerLightTweaker C1";return false;}
  return on_set_value(name,s,e); // original registered store precedes side effects
 }catch(const std::exception& ex){e=ex.what();return false;}
}
bool PlayerLightTweakerOwnerV90::on_set_value(const std::string& name,const PlayerLightTweakServicesV90& s,std::string& e){
 auto prefix=[&](const char* p){return name.compare(0,std::char_traits<char>::length(p),p)==0;};
 int family=-1;std::size_t digit{};
 if(prefix("m_attenuation")){family=0;digit=14;}else if(prefix("m_ambientColor")){family=1;digit=15;}else if(prefix("m_diffuseColor")){family=2;digit=15;}else if(prefix("m_specularColor")){family=3;digit=16;}
 if(family>=0){const auto i=std::size_t(name[digit]-'0');if(i>=5){e="Unsafe source light index outside actual registered names";return false;}const auto id=lights128_[i];if(!id){e.clear();return true;}TweakLightV90 light;if(!s.provider||!s.light||!s.light(id,light,e)||!light.owner||light.identity!=id){if(e.empty())e="Required actual SAME published LightBase128 receiver";return false;}
  if(family==0){if(!light.attenuation){e="Required native LightBase.SetAttenuation";return false;}return light.attenuation(attenuation7c_[i],e);}
  const auto& color=family==1?ambientb8_[i]:family==2?diffusecc_[i]:speculare0_[i];const float g=float(color[1])/255.0f,b=float(color[2])/255.0f,r=float(color[0])/255.0f;const std::array<float,3> rgb{r,g,b};auto set=family==1?light.ambient:family==2?light.diffuse:light.specular;if(!set){e="Required original native LightBase color setter";return false;}return set(rgb,e);
 }
 if(prefix("m_fogColor")){
  if(!s.provider||!color_produced_||!s.driver_fog_color){e="Required produced actual fogf4 and native driver color parameter";return false;}if(!s.driver_fog_color(fogf4_,e))return false;
  std::shared_ptr<loader::CanonicalLevelContextV1> level;if(!s.current_level||!s.current_level(level,e))return false;if(!level){e.clear();return true;}
  if(!s.config_color){e="Required fresh SAME LevelConfig color stores";return false;}const float r=float(fogf4_[0]),g=float(fogf4_[1]),b=float(fogf4_[2]);
  if(!s.config_color(level,0x1e0,{r,g,b},e)||!s.config_color(level,0x1ec,{r,g,b},e))return false;
 }else if(prefix("m_fogStart")||prefix("m_fogEnd")){
  if(!start_produced_||!end_produced_){e="Original onSet reads unproduced companion fog distance; unsafe native edge remains open";return false;}
  if(!s.provider||!s.driver_fog_distances){e="Required actual native driver fog distance parameter";return false;}if(!s.driver_fog_distances(start110_,end114_,e))return false;
  std::shared_ptr<loader::CanonicalLevelContextV1> level;if(!s.current_level||!s.current_level(level,e))return false;if(!level){e.clear();return true;}
  std::int32_t start,end;if(!source_f2iz(start110_,s,start,e)||!source_f2iz(end114_,s,end,e))return false;
  if(!s.config_int||!s.config_int(level,0x1d8,start,e)||!s.config_int(level,0x1dc,end,e))return false;
  if(end114_>0.0f&&flag118_==0.0f){flag118_=1.0f;if(!s.level_enable_fog){e="Required original current Level.EnableFog(NULL)";return false;}return s.level_enable_fog(level,e);}
 }else if(prefix("m_fogDirectionMask")){
  std::shared_ptr<loader::CanonicalLevelContextV1> level;if(!s.current_level||!s.current_level(level,e))return false;
  if(!s.provider||!s.scene_direction||!s.scene_direction(direction11c_,e))return false;
  if(level){if(!s.level_enable_fog){e="Required original Level.EnableFog(NULL)";return false;}return s.level_enable_fog(level,e);}
 }
 e.clear();return true;
}
bool PlayerLightTweakerOwnerV90::close(std::string& e){
 if(busy_){e="Cannot D1 actual tweaker during source callback";return false;}if(closed_){e.clear();return true;}
 // Native D1 order: f8 then CTweakable D2 vector70, string54, SGroup1c,
 // finally map4. The borrowed device and NULL/source light refs are not D0'd.
 std::string().swap(light_typef8_);std::string().swap(name54_);
 std::vector<Group>().swap(group1c_.children);std::vector<std::string>().swap(group1c_.variables);std::string().swap(group1c_.name);mappings4_.clear();closed_=true;e.clear();return true;
}
}
