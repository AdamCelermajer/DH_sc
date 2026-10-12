#include "module_xml_selection_v1.hpp"
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <vector>
namespace dh2::world {
namespace {
std::vector<std::string> split(const std::string& text){std::stringstream input(text);std::vector<std::string> result;std::string word;while(std::getline(input,word,','))result.push_back(word);return result;}
std::int32_t signed32(std::uint32_t bits){std::int32_t value;std::memcpy(&value,&bits,4);return value;}
}
bool module_choose_xmls_v1(const ModuleXmlFieldsV1& f,const ModuleXmlServicesV1& s,std::string& mgp,std::string& mvp,std::string& e){
 // Reached prefix always publishes the primary strings before alternate gates.
 mgp=f.mgp378;mvp=f.mvp390;if(f.alt_mgp3a8.empty()||f.alt_mvp3c0.empty())return true;
 auto a=split(f.alt_mgp3a8),b=split(f.alt_mvp3c0);a.push_back(f.mgp378);b.push_back(f.mvp390);
 std::vector<std::int32_t> weights;
 if(f.alt_prob3d8.empty())weights.assign(a.size(),static_cast<std::int32_t>(100/a.size()));
 else{std::uint32_t sum=0;for(auto& text:split(f.alt_prob3d8)){auto n=std::atoi(text.c_str());weights.push_back(n);sum+=static_cast<std::uint32_t>(n);}weights.push_back(signed32(100u-sum));}
 // Original source asserts count mismatch then may read beyond vector. Native
 // bounded successor preserves primary output and refuses that unsafe domain.
 if(a.size()!=b.size()||a.size()!=weights.size()){e="Original Module::_ChooseXmls count assertion domain exceeded";return false;}
 if(!s.owner||!s.random){e="Required SAME Module source Random388c58";return false;}
 std::int32_t roll=0;if(!s.random(100,roll,e))return false;
 std::uint32_t cumulative=0;
 for(std::size_t i=0;i<weights.size();++i){cumulative+=static_cast<std::uint32_t>(weights[i]);if(roll<signed32(cumulative)){mgp=a[i];mvp=b[i];break;}}
 // Uniform truncation remainder leaves the already-written primary strings.
 return true;
}
bool module_load_v1(const ModuleXmlFieldsV1& f,std::int32_t id,const float* position,const ModuleXmlServicesV1& random,const ModuleLevelLoadBorrowV1& level,std::string& e){
 if(!level.owner||!level.object_module_id18c||!level.module_offset160||!position){e="Required SAME Level Module load fields/current Level";return false;}
 *level.object_module_id18c=id;for(unsigned i=0;i<3;++i)level.module_offset160[i]=position[i];
 std::string mgp,mvp;if(!module_choose_xmls_v1(f,random,mgp,mvp,e))return false;
 for(auto* name:{&mgp,&mvp})if(!name->empty()){
  if(!level.load_file){e="Required original Level::LoadFile3f3b40";return false;}
  bool loaded=false;do{if(!level.load_file(*name,"Module",loaded,e))return false;}while(!loaded);
 }
 for(unsigned i=0;i<3;++i)level.module_offset160[i]=0.f;*level.object_module_id18c=-1;return true;
}
}
