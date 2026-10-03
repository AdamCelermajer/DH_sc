#include "properties.hpp"
#include <cstring>
namespace {
using namespace dh2::data;
std::int32_t bits(std::uint32_t v){std::int32_t r;std::memcpy(&r,&v,4);return r;}
std::int32_t add(std::int32_t a,std::int32_t b){return bits(std::uint32_t(a)+std::uint32_t(b));}
unsigned type(const PropertyView& v,unsigned p){return v.types[p]==-1?16u:std::uint32_t(v.types[p]);}
bool valid(const PropertyView* v,std::int32_t p){
 if(!v||p<0||p>=224||!v->defaults||!v->types||!v->base||!v->saved||!v->gear||!v->resolved||v->group_count>10000||(v->group_count&&!v->groups))return false;
 unsigned total=0;for(unsigned i=0;i<v->group_count;++i){const auto& g=v->groups[i];if(g.count>10000||total>100000-g.count||(g.count&&!g.sheets))return false;total+=g.count;for(unsigned j=0;j<g.count;++j)if(!g.sheets[j])return false;}return true;
}
std::int32_t resolve(PropertyView& v,unsigned p){
 const auto def=v.defaults[p];const auto t=type(v,p);auto& out=v.resolved[p];
 auto accumulate=[&](std::int32_t x){out=out==def?x:add(out,x);};
 auto group=[&](const PropertyBuffGroup& g){bool found=false;for(unsigned j=0;j<g.count;++j){auto x=g.sheets[j][p];if(x!=def){out=x;found=true;}}return found;};
 if(t&4){
  out=def;for(auto s:std::array<const std::int32_t*,3>{v.base,v.saved,v.gear})if(s[p]!=def)accumulate(s[p]);
  for(unsigned i=0;i<v.group_count;++i)for(unsigned j=0;j<v.groups[i].count;++j){auto x=v.groups[i].sheets[j][p];if(x!=def)accumulate(x);}
 }else if(t&2){
  for(auto s:std::array<const std::int32_t*,3>{v.base,v.saved,v.gear})if(s[p]!=def){out=s[p];return out;}
  for(unsigned i=0;i<v.group_count;++i)if(group(v.groups[i]))return out;
  out=def;
 }else if(t&1){
  for(unsigned i=v.group_count;i>0;--i)if(group(v.groups[i-1]))return out;
  for(auto s:std::array<const std::int32_t*,3>{v.gear,v.saved,v.base})if(s[p]!=def){out=s[p];return out;}
  out=def;
 }else if(t&32){out=v.base[p];accumulate(v.saved[p]);}
 else if(t&16){out=v.base[p];}
 return out;
}
}
extern "C" unsigned dh2_property_resolve(dh2::data::PropertyView* v,std::int32_t p,std::int32_t* result){if(!result||!valid(v,p))return 1;*result=resolve(*v,p);return 0;}
extern "C" unsigned dh2_property_set(dh2::data::PropertyView* v,std::int32_t p,std::int32_t x){if(!valid(v,p))return 1;auto t=type(*v,p);if(t&32){v->saved[p]=x;resolve(*v,p);}else if(t&8)v->resolved[p]=x;return 0;}
extern "C" unsigned dh2_property_add(dh2::data::PropertyView* v,std::int32_t p,std::int32_t x){if(!valid(v,p))return 1;auto t=type(*v,p);if(t&32){v->saved[p]=add(v->saved[p],x);resolve(*v,p);}else if(t&8)v->resolved[p]=add(v->resolved[p],x);return 0;}
extern "C" unsigned dh2_property_set_to_sheet(dh2::data::PropertyView* v,std::int32_t p,std::int32_t x,std::int32_t* sheet){if(!sheet||!valid(v,p))return 1;auto t=type(*v,p);if(t&4){sheet[p]=x;resolve(*v,p);}else if(t&8)v->resolved[p]=x;return 0;}
extern "C" unsigned dh2_property_set_int(dh2::data::PropertyView* v,std::int32_t p,std::int32_t x){return dh2_property_set(v,p,bits(std::uint32_t(x)<<8));}
extern "C" unsigned dh2_property_validate(const dh2::data::PropertyView* v){return valid(v,0)?0:1;}
namespace dh2::data {
bool load_property_rules(const CharacterTable& table,PropertyRules& out,std::string& error){out={};error.clear();if(table.fields.size()!=224||table.rows.size()<2||table.names.size()!=table.rows.size()||table.names[0]!="AAA_DEFAULTS_DONT_DELETE"||table.names[1]!="AAA_TYPES_DONT_DELETE"){error="Missing original property default/type rows";return false;}out.defaults=table.rows[0];out.types=table.rows[1];return true;}
PropertyView property_view(const PropertyRules& r,PropertyState& s){return {r.defaults.data(),r.types.data(),s.base.data(),s.saved.data(),s.gear.data(),s.resolved.data(),nullptr,0};}
void reset_properties(const PropertyRules& r,PropertyState& s,const PropertySheet* base){s.base=base?*base:r.defaults;s.saved=s.gear=s.resolved=r.defaults;}
bool recalc_properties(const PropertyRules& r,PropertyState& s,std::string& error){error.clear();auto candidate=s;auto v=property_view(r,candidate);for(unsigned p=0;p<224;++p)resolve(v,p);s=candidate;return true;}
bool recalc_properties_with_class(const ClassTables& classes,const PropertyRules& r,PropertyState& s,std::string& error){auto candidate=s;if(!apply_class_uncached(classes,candidate.base[26],r,candidate,error)||!recalc_properties(r,candidate,error))return false;s=candidate;return true;}
}
