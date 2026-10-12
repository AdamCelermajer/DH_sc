#include "character_relationships.hpp"
namespace dh2::relationships {namespace {
bool aligned(const void*p,std::uintptr_t a){return p&&!(reinterpret_cast<std::uintptr_t>(p)&(a-1));}
bool overlap(const void*a,std::uintptr_t n,const void*b,std::uintptr_t m){auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);return x<=y?y-x<n:x-y<m;}
bool object(const Object48*p){return aligned(p,8)&&p->character.identity&&(p->object_type||aligned(p->character.properties,4))&&aligned(p->handle,8)&&!p->reserved;}
bool owner(const Object48*p){return object(p)&&aligned(p->character.properties,4);}
std::int32_t faction(const Object48*p,const Factions16*t){auto n=p->character.properties->words[0];return n>=0&&static_cast<std::uint32_t>(n)<t->count?n:10;}
bool call(const Services16*s,Service op,Object48*p,Object48*other,std::uintptr_t&v){v=0;Request24 q{op,0,p->character.identity,other?other->character.identity:0};return s->invoke(s->context,&q,&v)==0;}
bool valid_registry(const target_providers::Registry24*r){if(!aligned(r,8)||r->count>r->capacity||r->capacity>65536||!aligned(r->records,8)||r->reserved)return false;for(std::uint32_t i=0;i<r->count;++i)if(r->records[i].reserved||(i&&r->records[i-1].key>=r->records[i].key))return false;return true;}
bool valid_table(const Factions16*t){if(!aligned(t,8)||!aligned(t->rows,8)||t->count<11||t->count>4096||t->reserved)return false;for(std::uint32_t i=0;i<t->count;++i){auto&r=t->rows[i];if(r.reserved||r.count>4096||(r.count&&!aligned(r.entries,4)))return false;}return true;}
int resolve(Object48*p,target_providers::Registry24*r,Object48*&out){auto&shared=*p->handle;shared.frame=r->frame;auto local=shared;out=nullptr;if(!local.key)return 0;if(!local.cached||local.frame!=r->frame){std::uint32_t lo=0,hi=r->count;while(lo<hi){auto mid=lo+(hi-lo)/2;if(r->records[mid].key<local.key)lo=mid+1;else hi=mid;}if(lo==r->count||r->records[lo].key!=local.key){if(r->count==r->capacity)return 2;for(auto i=r->count;i>lo;--i)r->records[i]=r->records[i-1];r->records[lo]={local.key,0,0};++r->count;}local.cached=r->records[lo].object;}out=reinterpret_cast<Object48*>(local.cached);return out&&!object(out)?2:0;}
}
extern "C" int dh2_character_relationship(std::int32_t*out,std::uint32_t op,State16*ai,Object48*candidate,target_providers::Registry24*r,const Factions16*t,const Services16*s){
 if(!aligned(out,4)||!aligned(ai,8)||!owner(ai->owner)||(candidate&&!object(candidate))||!valid_registry(r)||!valid_table(t)||!aligned(s,8)||!s->invoke||op<1||op>2)return 1;
 if(overlap(out,4,ai,sizeof(*ai))||overlap(out,4,r,sizeof(*r))||overlap(out,4,r->records,r->capacity*sizeof(*r->records))||overlap(out,4,t,sizeof(*t))||overlap(out,4,t->rows,t->count*sizeof(*t->rows))||overlap(out,4,ai->owner,sizeof(*ai->owner))||overlap(out,4,ai->owner->character.properties,896)||(candidate&&(overlap(out,4,candidate,sizeof(*candidate))||overlap(out,4,candidate->character.properties,896)||overlap(out,4,candidate->handle,16))))return 1;
 if(!candidate)candidate=ai->target;
 if(!candidate){*out=0;return 0;}if(!object(candidate))return 2;
 const auto span=r->capacity*sizeof(*r->records);
 if(overlap(out,4,ai->owner->handle,16)||overlap(out,4,candidate,sizeof(*candidate))||overlap(out,4,candidate->handle,16)||overlap(out,4,candidate->character.properties,896)||overlap(out,4,s,sizeof(*s))||overlap(r->records,span,ai,sizeof(*ai))||overlap(r->records,span,r,sizeof(*r))||overlap(r->records,span,candidate,sizeof(*candidate))||overlap(r->records,span,candidate->handle,16)||overlap(r->records,span,ai->owner,sizeof(*ai->owner))||overlap(r->records,span,ai->owner->handle,16)||overlap(r->records,span,s,sizeof(*s)))return 1;
 for(std::uint32_t i=0;i<t->count;++i)if(overlap(out,4,t->rows[i].entries,t->rows[i].count*sizeof(data::AiFactionEntry)))return 1;
 Object48*resolved=nullptr;int status=resolve(candidate,r,resolved);if(status)return status;
 std::uintptr_t v=0;
 if(!resolved||resolved->object_type!=0){
  if(op==2){*out=0;return 0;}
  if(!owner(ai->owner)||!call(s,virtual_interactive,candidate,ai->owner,v))return 2;
  if(!v){*out=0;return 0;}
  if(!owner(ai->owner)||!call(s,virtual_interaction_type,candidate,ai->owner,v))return 2;
  *out=static_cast<std::int32_t>(v)==8;return 0;
 }
 // Source performs its faction range diagnostics before these virtuals, then
 // reloads owner and actual table base for the final first-match traversal.
 if(op==1){
  if(!owner(ai->owner)||!call(s,virtual_player,ai->owner,nullptr,v))return 2;
  if(v){if(!call(s,virtual_player,resolved,nullptr,v))return 2;if(v){*out=0;return 0;}}
 }
 if(!owner(ai->owner)||!object(resolved)||!aligned(resolved->character.properties,4)||!valid_table(t))return 2;
 const auto&row=t->rows[faction(ai->owner,t)];auto target=faction(resolved,t);std::int32_t value=0;
 for(std::uint32_t i=0;i<row.count;++i)if(row.entries[i].id==target){value=row.entries[i].value;break;}
 *out=op==1?value<0:value>0;return 0;
}
}
