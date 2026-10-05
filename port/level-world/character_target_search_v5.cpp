#include "character_target_search_v5.hpp"
#include <cmath>
#include <cstring>
namespace dh2::target_search { namespace {
bool aligned(const void* p,std::uintptr_t a=8) { return p && !(reinterpret_cast<std::uintptr_t>(p)&(a-1)); }
bool overlap(const void* a,std::uintptr_t an,const void* b,std::uintptr_t bn) {
 auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);
 return x<=y?y-x<an:x-y<bn;
}
bool valid_services(const Services16* s) { return aligned(s)&&s->invoke; }
bool valid_list(const List40* l) { return aligned(l)&&aligned(l->heap)&&l->capacity>0&&l->capacity<=65536&&l->count<=l->capacity&&aligned(l->owner)&&(!l->reference_character||aligned(l->reference_character))&&l->sort<=2&&l->reserved==0&&!overlap(l,sizeof(*l),l->heap,l->capacity*sizeof(Target24)); }
bool call(const Services16* s,Service op,const Object48* subject,const Object48* other,Response16& out) {
 out={};Request24 q{op,0,subject?subject->identity:0,other?other->identity:0};return s->invoke(s->context,&q,&out)==0;
}
float mul(float a,float b) { volatile float r=a*b;return r; }
float add(float a,float b) { volatile float r=a+b;return r; }
float sub(float a,float b) { volatile float r=a-b;return r; }
float divide(float a,float b) { volatile float r=a/b;return r; }
float length(const float* v) { return std::sqrt(add(add(mul(v[0],v[0]),mul(v[1],v[1])),mul(v[2],v[2]))); }
float angle(const float* a,const float* b) {
 float dot=add(add(mul(a[0],b[0]),mul(a[1],b[1])),mul(a[2],b[2]));
 float result=std::acos(divide(dot,mul(length(a),length(b))));
 std::uint32_t bits;std::memcpy(&bits,&result,4);bits&=0x7fffffff;std::memcpy(&result,&bits,4);return result;
}
const float* center(Object48* o) { return o->has_target_position?o->target_position:o->position; }
bool less(const Target24& a,const Target24& b,std::uint32_t sort) {
 if(!sort)return false;
 if((a.flags&1)!=(b.flags&1))return (b.flags&1)!=0;
 return sort==1?a.distance>b.distance:a.angle>b.angle;
}
void push(List40* l,Target24 value) {
 auto hole=l->count++;
 while(hole) { auto parent=(hole-1)/2;if(!less(l->heap[parent],value,l->sort))break;l->heap[hole]=l->heap[parent];hole=parent; }
 l->heap[hole]=value;
}
void pop(List40* l) {
 const auto n=--l->count;if(!n)return;auto value=l->heap[n];std::uint32_t hole=0,right=2;
 while(right<n) { auto selected=less(l->heap[right],l->heap[right-1],l->sort)?right-1:right;l->heap[hole]=l->heap[selected];hole=selected;right=(selected+1)*2; }
 if(right==n) { l->heap[hole]=l->heap[right-1];hole=right-1; }
 while(hole) { auto parent=(hole-1)/2;if(!less(l->heap[parent],value,l->sort))break;l->heap[hole]=l->heap[parent];hole=parent; }
 l->heap[hole]=value;
}
// Return -1 on provider error; exact flags1 character path otherwise.
int character_valid(List40* l,Object48* candidate,const Services16* s) {
 auto ref=l->reference_character;if(!ref)return 1;
 if(ref->character_word1314<candidate->character_word1310)return 0;
 Response16 r;
 if(!call(s,is_dead,candidate,nullptr,r))return -1;
 if(r.word)return 0;
 if(!call(s,is_enemy,ref,candidate,r))return -1;
 if(!r.word)return 0;
 if(!call(s,is_player,candidate,nullptr,r))return -1;
 if(!r.word)return 1;
 if(!call(s,is_player,l->reference_character,nullptr,r))return -1;
 return !r.word;
}
} // namespace
extern "C" int dh2_target_list_init_snapshot_v5(List40* l,Target24* heap,std::uint32_t capacity,Object48* owner,std::uint32_t sort,const Services16* s) {
 if(!aligned(l)||!aligned(heap)||!aligned(owner)||!owner->identity||!capacity||capacity>65536||sort>2||!valid_services(s)||overlap(l,sizeof(*l),heap,capacity*sizeof(Target24)))return 1;
 *l={heap,0,capacity,owner,nullptr,sort,0};Response16 r;if(!call(s,is_character,owner,nullptr,r))return 2;if(r.word)l->reference_character=owner;return 0;
}
extern "C" int dh2_target_search_snapshot_v5(List40* l,const Registry8* registry,float radius,float cone,const float captured_origin[3],const Services16* s,const SnapshotResolve16V5* resolver) {
 if(!valid_list(l)||!aligned(registry)||!aligned(registry->rooms)||!valid_services(s)||!aligned(captured_origin,4)||(resolver&&(!aligned(resolver)||!resolver->invoke)))return 1;
 // GameObject Lua wrapper has already captured its owner position. SearchEff
 // captures heading here; provider mutations do not change that stack origin.
 const float rotation=l->owner->rotation;float look[3]={std::sin(rotation),-std::cos(rotation),0};const float* origin=captured_origin;
 while(l->count)pop(l);
 float reference_radius=0;Response16 r;
 if(l->reference_character) { if(!call(s,melee_radius,l->reference_character,nullptr,r))return 2;reference_radius=r.number; }
 auto end=registry->rooms;auto room=end->next;std::uint32_t visits=0;
 if(!aligned(room))return 2;
 // Reset/ValidateCurrent reads each room head once on entering that room.
 if(room!=end&&!aligned(room->objects))return 2;
 Entry16* entry=room==end?nullptr:room->objects->next;
 while(room!=end) {
  if(++visits>65536||!aligned(room)||!aligned(room->objects)||!aligned(entry))return 2;
  if(entry==room->objects) {
   room=room->next;if(!aligned(room))return 2;
   if(room!=end&&!aligned(room->objects))return 2;
   entry=room==end?nullptr:room->objects->next;continue;
  }
  auto object=entry->object;Object48* character=nullptr;
  if(resolver&&resolver->invoke(resolver->context,entry,&object))return 2;
  if(object&&!aligned(object))return 2;
  // GetChar executes even for null/self/invisible Get results.
  if(!call(s,resolve_character,object,nullptr,r))return 2;
  character=reinterpret_cast<Object48*>(r.word);
  if(character&&!aligned(character))return 2;
  if(object&&object!=l->owner&&object->visible) {
   if(!call(s,is_zonable,object,nullptr,r))return 2;
   if(!(r.word&&object->zoned&&!object->in_zone)) {
    if(!call(s,is_interactive,object,l->owner,r))return 2;
    if(r.word) {
     int accepted;
     if(character)accepted=character_valid(l,character,s);
     else { if(!call(s,interaction_type,object,l->owner,r))return 2;accepted=r.word==8; }
     if(accepted<0)return 2;
     if(accepted) {
      if(!call(s,interaction_radius,object,nullptr,r))return 2;
      const float target_radius=r.number;const auto point=center(object);
      float delta[3]={sub(point[0],origin[0]),sub(point[1],origin[1]),sub(point[2],origin[2])};float distance=sub(sub(length(delta),target_radius),reference_radius);
      if(!(distance>radius)) {
       float a=angle(delta,look);
       if(!(cone<3.1415927410125732421875f&&cone<a)) {
        if(l->count>=l->capacity)return 2;
        push(l,{object->identity,distance,a,character?1u:0u,0});
       }
      }
     }
    }
   }
  }
  // Source Next rereads the current intrusive next link after all callbacks.
  entry=entry->next;
 }
 return 0;
}
extern "C" int dh2_target_pop_snapshot_v5(List40* l,Target24* out) {
 if(!valid_list(l)||!aligned(out)||overlap(out,sizeof(*out),l,sizeof(*l))||overlap(out,sizeof(*out),l->heap,l->capacity*sizeof(Target24)))return 1;
 if(!l->count)return 2;
 *out=l->heap[0];pop(l);return 0;
}
}
