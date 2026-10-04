#include "character_script_init_vitals.hpp"
#include <array>
#include <cstring>
namespace {
using namespace dh2::character;
namespace data=dh2::data;
bool aligned(const void* p,std::size_t n){return p&&reinterpret_cast<std::uintptr_t>(p)%n==0;}
bool overlap(const void* a,std::size_t n,const void* b,std::size_t m){auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);return x<=y?y-x<n:x-y<m;}
std::int32_t signed_word(std::uint32_t n){std::int32_t v;std::memcpy(&v,&n,4);return v;}
bool valid(const data::PropertyView* v){
 if(!aligned(v,alignof(data::PropertyView)))return false;
 for(auto p:std::array<const std::int32_t*,6>{v->defaults,v->types,v->base,v->saved,v->gear,v->resolved})if(!aligned(p,4))return false;
 if(v->group_count&&!aligned(v->groups,alignof(data::PropertyBuffGroup)))return false;
 if(dh2_property_validate(v))return false;
 for(unsigned i=0;i<v->group_count;++i){const auto& g=v->groups[i];if(g.count&&!aligned(g.sheets,8))return false;for(unsigned j=0;j<g.count;++j)if(!aligned(g.sheets[j],4))return false;}
 return true;
}
int regen(data::VitalsChange& out,data::PropertyView* view,unsigned current_id,unsigned maximum_id,const dh2::fx::PreloadServices16& debug){
 if(!valid(view))return -2;
 const auto current=view->resolved[current_id],maximum=view->resolved[maximum_id];
 auto delta=maximum;
 if(signed_word(std::uint32_t(current)+std::uint32_t(delta))>maximum)delta=signed_word(std::uint32_t(maximum)-std::uint32_t(current));
 out={0,current,current};
 if(delta<=0)return 1;
 unsigned ignored=0;
 if(debug.call(debug.context,dh2::fx::debug_load,nullptr,&ignored)||debug.call(debug.context,dh2::fx::debug_switch,"isTracingChar_Stats",&ignored))return -2;
 if(!valid(view)||dh2_property_add(view,current_id,delta))return -2;
 out={delta,current,view->resolved[current_id]};return 1;
}
}
extern "C" int dh2_character_script_init_vitals(dh2::character::ScriptInitVitals24* out,const dh2::character::ScriptInitVitals32* binding){
 using namespace dh2::character;
 if(!aligned(out,4)||!aligned(binding,8)||!binding->owner||!binding->debug.call||!valid(binding->properties)||overlap(out,24,binding,32)||overlap(out,24,binding->properties,sizeof(data::PropertyView)))return -1;
 for(auto p:std::array<const std::int32_t*,6>{binding->properties->defaults,binding->properties->types,binding->properties->base,binding->properties->saved,binding->properties->gear,binding->properties->resolved})if(overlap(out,24,p,896))return -1;
 const auto* view_input=binding->properties;
 if(view_input->group_count&&overlap(out,24,view_input->groups,std::size_t(view_input->group_count)*sizeof(data::PropertyBuffGroup)))return -1;
 for(unsigned i=0;i<view_input->group_count;++i){const auto& g=view_input->groups[i];if(g.count&&overlap(out,24,g.sheets,std::size_t(g.count)*8))return -1;for(unsigned j=0;j<g.count;++j)if(overlap(out,24,g.sheets[j],896))return -1;}
 // Source _InitHpMp captures Character receiver; its sheets and debug binding
 // are stable projection backing, while callback mutations of contents survive.
 auto* view=binding->properties;const auto debug=binding->debug;*out={};
 auto result=regen(out->hp,view,36,38,debug);if(result<0)return result;
 return regen(out->mp,view,41,43,debug);
}
