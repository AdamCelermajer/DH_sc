#include "visual_skin_selection_v6.hpp"
#include <cstring>
#include <string>
namespace {
using namespace dh2::skinning;
bool valid(const VisualSelectionV6* s){
 if(!s||s->count>4096||(s->count&&(!s->categories||!s->cells)))return false;
 for(unsigned i=0;i<s->count;++i){const auto& c=s->categories[i];if(!c.name||!c.default_uri||c.reserved||s->cells[i].reserved||c.count>100000||(c.count&&!c.modules))return false;
  for(unsigned j=0;j<c.count;++j)if(!c.modules[j].uri)return false;
 }
 return true;
}
bool call(VisualSelectionV6& s,const VisualServicesV6* v,VisualOperationV6 op,std::int32_t c=-1,std::int32_t m=-1,std::uintptr_t r=0,std::uintptr_t d=0,const char* t=nullptr,std::int32_t slot=0,std::int32_t mode=0,std::uintptr_t* result=nullptr){
 std::uintptr_t scratch=0;VisualRequestV6 q{op,c,m,slot,mode,r,d,t};return v->invoke(v->context,s,q,result?result:&scratch);
}
}
extern "C" std::int32_t dh2_visual_category_v6(const VisualSelectionV6* s,const char* name){if(!valid(s)||!name)return -1;for(unsigned i=0;i<s->count;++i)if(!std::strcmp(s->categories[i].name,name))return std::int32_t(i);return -1;}
extern "C" std::int32_t dh2_visual_module_uri_v6(const VisualSelectionV6* s,const char* uri){if(!valid(s)||!uri)return -1;for(unsigned i=0;i<s->count;++i)for(unsigned j=0;j<s->categories[i].count;++j)if(!std::strcmp(s->categories[i].modules[j].uri,uri))return std::int32_t(j);return -1;}
extern "C" std::int32_t dh2_visual_module_v6(const VisualSelectionV6* s,std::int32_t,const char* name){if(!name)return -1;try{const std::string uri=std::string("#")+name+"-mesh-skin";return dh2_visual_module_uri_v6(s,uri.c_str());}catch(...){return -1;}}
extern "C" std::int32_t dh2_visual_set_category_v6(VisualSelectionV6* s,std::int32_t category,std::int32_t module,std::uint32_t update,const VisualServicesV6* services){
 if(!valid(s))return -1;
 if(update>1)return -1;
 if(category==-1)return 0;
 if(!services||!services->invoke)return -1;
 if(category<0||std::uint32_t(category)>=s->count||module< -1||(module>=0&&std::uint32_t(module)>=s->categories[category].count))return -1;
 auto& cell=s->cells[category];
 if(cell.id!=module){
  if(cell.resource){auto old=cell.resource;cell.resource=0;if(!call(*s,services,VisualOperationV6::release,category,module,old))return -2;cell.id=-1;}
  if(module!=-1){const auto& descriptor=s->categories[category].modules[module];std::uintptr_t resource=0;
   if(!call(*s,services,VisualOperationV6::construct_module,category,module,0,descriptor.descriptor,descriptor.uri,0,0,&resource))return -2;
   if(resource){if(!call(*s,services,VisualOperationV6::retain,category,module,resource))return -2;auto old=cell.resource;cell.resource=resource;
    if(old&&!call(*s,services,VisualOperationV6::release,category,module,old))return -2;
    cell.id=module;if(!call(*s,services,VisualOperationV6::release,category,module,resource))return -2;
   }
  }
  if(update&&!call(*s,services,VisualOperationV6::update_buffers,category,module,0,0,nullptr,0,std::int32_t((s->buffer_flags^1)&1)))return -2;
 }
 return 0;
}
extern "C" std::int32_t dh2_visual_set_modular_v6(VisualSelectionV6* s,std::int32_t category,std::int32_t module,const VisualServicesV6* services){
 auto status=dh2_visual_set_category_v6(s,category,module,1,services);if(status||category==-1)return status;
 return call(*s,services,VisualOperationV6::visibility,category,module)?0:-2;
}
extern "C" std::int32_t dh2_visual_set_weapon_v6(VisualSelectionV6* s,const char* name,std::int32_t slot,std::int32_t mode,const VisualServicesV6* services){
 if(!valid(s)||!services||!services->invoke||mode<0||mode>2)return -1;
 std::uintptr_t resource=0;
 if(name){try{const auto path=std::string("data/3d/characters/prince/weapons/")+name+".bdae";if(!call(*s,services,VisualOperationV6::construct_weapon,-1,-1,0,0,path.c_str(),slot,mode,&resource))return -2;}catch(...){return -2;}}
 auto& cell=s->weapons[slot==1?0:1];
 if(cell){if(!call(*s,services,VisualOperationV6::detach,-1,-1,cell,0,nullptr,slot,mode))return -2;if(!call(*s,services,VisualOperationV6::release,-1,-1,cell,0,nullptr,slot,mode))return -2;}
 cell=resource;
 const char* anchors[]{"anchor_shield_left_offset","anchor_weapon_right_offset","anchor_weapon_left_offset"};std::uintptr_t parent=0;
 if(!call(*s,services,VisualOperationV6::search,-1,-1,s->root,0,anchors[mode],slot,mode,&parent))return -2;
 if(parent&&!call(*s,services,VisualOperationV6::attach,-1,-1,resource,parent,nullptr,slot,mode))return -2;
 return 0;
}
