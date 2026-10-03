#include "character_attack_geometry.hpp"
#include <cstring>
namespace {
using namespace dh2::character;
bool aligned(const void* p,std::uintptr_t n){return p&&reinterpret_cast<std::uintptr_t>(p)%n==0;}
bool overlap(const void* a,std::uintptr_t an,const void* b,std::uintptr_t bn){const auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);return an&&bn&&(x<=y?y-x<an:x-y<bn);}
bool main_hand(const CombatInventory16* inv,const CombatItemRecord164* rows,std::uint32_t count,const CombatItemRecord164*& row){
 row=nullptr;
 if(!aligned(inv,alignof(CombatInventory16))||inv->count>65536||inv->current_set<0||static_cast<std::uint32_t>(inv->current_set)>=inv->count||!aligned(inv->sets,alignof(CombatEquipSet8)))return false;
 const auto* ref=inv->sets[inv->current_set].main_hand;if(!ref)return true;
 if(!aligned(ref,alignof(const CombatItemInstance4*))||!aligned(*ref,alignof(CombatItemInstance4)))return false;
 const auto id=(*ref)->item_id;
 if(count>65536||!aligned(rows,alignof(CombatItemRecord164))||id<0||static_cast<std::uint32_t>(id)>=count)return false;
 row=&rows[id];return true;
}
bool output_disjoint(const void* output,std::uintptr_t size,const CombatInventory16* inv,const CombatItemRecord164* rows,std::uint32_t count){
 if(overlap(output,size,inv,sizeof(*inv))||overlap(output,size,inv->sets,std::uintptr_t(inv->count)*sizeof(*inv->sets))||overlap(output,size,rows,std::uintptr_t(count)*sizeof(*rows)))return false;
 const auto* ref=inv->sets[inv->current_set].main_hand;
 return !ref||(!overlap(output,size,ref,sizeof(*ref))&&!overlap(output,size,*ref,sizeof(**ref)));
}
bool ranged(const CombatItemRecord164* row){return row&&(row->words[22]==4||row->words[22]==5);}
std::int32_t asr8(std::int32_t x){return x/256-(x<0&&x%256!=0);}
float integer_square(std::int32_t x){const std::uint32_t raw=static_cast<std::uint32_t>(x);const auto square=raw*raw;std::int32_t signed_square;std::memcpy(&signed_square,&square,4);return static_cast<float>(signed_square);}
float distance(const float* owner,const float* target){volatile float x=owner[0]-target[0],y=owner[1]-target[1],z=owner[2]-target[2];volatile float xx=x*x,yy=y*y,zz=z*z,xy=xx+yy,d=xy+zz;return d;}
}
extern "C" int dh2_attack_equipment_melee_radius(std::int32_t* output,const dh2::character::CombatInventory16* inv,const dh2::character::CombatItemRecord164* rows,std::uint32_t count){
 const CombatItemRecord164* row;
 if(!aligned(output,alignof(std::int32_t))||!main_hand(inv,rows,count,row)||!output_disjoint(output,4,inv,rows,count))return -1;
 if(ranged(row))return 0;
 *output=row?row->words[39]:0;return 1;
}
extern "C" int dh2_attack_range_parameters(std::int32_t* output,const dh2::character::CombatProperties896* props,const dh2::character::CombatInventory16* inv,const dh2::character::CombatItemRecord164* rows,std::uint32_t count){
 if(!aligned(output,alignof(std::int32_t))||!aligned(props,alignof(CombatProperties896))||overlap(output,12,props,sizeof(*props)))return -1;
 if(props->words[32]!=-1){const std::int32_t result[]={asr8(props->words[30]),asr8(props->words[31]),props->words[32]};std::memcpy(output,result,12);return 1;}
 const CombatItemRecord164* row;
 if(!main_hand(inv,rows,count,row)||!output_disjoint(output,12,inv,rows,count))return -1;
 if(!ranged(row))return 0;
 const std::int32_t result[]={row->words[38],row->words[39],row->words[40]};std::memcpy(output,result,12);return 1;
}
extern "C" int dh2_attack_melee_radius(float* output,const dh2::character::CombatProperties896* props,const dh2::character::CombatInventory16* inv,const dh2::character::CombatItemRecord164* rows,std::uint32_t count,const float* ai,std::uint32_t ai_count){
 if(!aligned(output,alignof(float))||!aligned(props,alignof(CombatProperties896))||ai_count>65536||ai_count<=8||!aligned(ai,alignof(float))||overlap(output,4,props,sizeof(*props))||overlap(output,4,ai,std::uintptr_t(ai_count)*4))return -1;
 const CombatItemRecord164* row;if(!main_hand(inv,rows,count,row)||!output_disjoint(output,4,inv,rows,count))return -1;
 auto id=props->words[1];if(id<0||static_cast<std::uint32_t>(id)>=ai_count)id=8;
 const std::int32_t equipment=row&&!ranged(row)?row->words[39]:0;
 volatile float converted=static_cast<float>(equipment),result=converted+ai[id];*output=result;return 0;
}
extern "C" int dh2_attack_melee_distance(const float* owner,const float* target,const float* radii){
 if(!aligned(owner,alignof(float))||!aligned(target,alignof(float))||!aligned(radii,alignof(float)))return -1;
 const float d=distance(owner,target);volatile float reach=radii[0]+radii[1],square=reach*reach;return square>d;
}
extern "C" int dh2_attack_ranged_distance(const float* owner,const float* target,const std::int32_t* limits){
 if(!aligned(owner,alignof(float))||!aligned(target,alignof(float))||!aligned(limits,alignof(std::int32_t)))return -1;
 const float d=distance(owner,target);const float low=integer_square(limits[0]);if(!(low<=d))return 0;const float high=integer_square(limits[1]);return high>=d;
}
