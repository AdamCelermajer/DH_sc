#include "character_cancel_sneaking.hpp"
namespace {
using namespace dh2::character::sneaking;
template<class T>bool aligned(const T* p){return p&&reinterpret_cast<std::uintptr_t>(p)%alignof(T)==0;}
bool character(const Character48* p){if(!aligned(p))return false;for(auto x:p->reserved)if(x)return false;return true;}
bool sheet(const Character48* p,unsigned need){return character(p)&&!p->resolved.reserved&&p->resolved.count<=224&&p->resolved.count>need&&aligned(p->resolved.words);}
unsigned call(const Services16* s,Operation op,std::uintptr_t receiver,unsigned index,unsigned arg,std::uint32_t& result){
 if(!aligned(s)||!s->invoke)return 1;Request24 r{op,index,receiver,arg,0};return s->invoke(s->context,&r,&result)?2:0;
}
unsigned list(const Character48* c,const List16*& result,const Tables32*& table){
 if(!sheet(c,28)||!aligned(c->tables))return 1;table=c->tables;
 if(table->reserved0||table->reserved1||table->list_count>65536||!aligned(table->lists))return 1;
 auto id=c->resolved.words[28];if(id<0||static_cast<std::uint32_t>(id)>=table->list_count)id=3;
 if(static_cast<std::uint32_t>(id)>=table->list_count)return 1;
 result=table->lists+id;if(result->reserved||result->count>65536||(result->count&&!aligned(result->ids)))return 1;return 0;
}
unsigned skill(const Tables32* t,std::int32_t id,const Skill76*& result){
 if(t->skill_count>65536||id<0||static_cast<std::uint32_t>(id)>=t->skill_count||!aligned(t->skills))return 1;
 result=t->skills+id;return 0;
}
bool vector(const AI24* a){return aligned(a)&&!a->reserved&&a->count<=65536&&(!a->count||aligned(a->scripts));}
}
extern "C" unsigned dh2_character_cancel_skill(AI24* ai,std::uint32_t index,const Services16* services){
 if(!vector(ai)||index>=ai->count)return 1;auto script=ai->scripts[index];if(!script)return 0;
 const List16* selected=nullptr;const Tables32* tables=nullptr;
 if(list(ai->owner,selected,tables)||index>=selected->count)return 1;
 const Skill76* row=nullptr;if(skill(tables,selected->ids[index],row))return 1;
 if(row->words[18]!=1)return 0;
 // Source reloads the script vector after the authored-row query and again
 // after Active, whose synchronous callback may replace the selected script.
 if(!vector(ai)||index>=ai->count)return 1;script=ai->scripts[index];std::uint32_t active=0;
 auto status=call(services,skill_check_active,script,index,0,active);if(status||!active)return status;
 if(!vector(ai)||index>=ai->count)return 1;script=ai->scripts[index];std::uint32_t ignored=0;
 return call(services,skill_pre,script,index,0,ignored);
}
extern "C" unsigned dh2_character_cancel_sneaking(Character48* c,const Services16* services){
 if(!character(c))return 1;std::uint32_t player=0;auto status=call(services,is_player,c->identity,0,0,player);if(status)return status;
 if(player){std::uint32_t ignored=0;status=call(services,delete_buff,c->identity,0,0x92,ignored);if(status)return status;c->changed415=1;}
 // IsSneaking3bc690 reads the resolved property through GetProperty3dedb4
 // then uses signed >0. No bit normalization/fixed-point scaling occurs.
 if(!sheet(c,198))return 1;if(c->resolved.words[198]<=0)return 0;
 const List16* selected=nullptr;const Tables32* tables=nullptr;if(list(c,selected,tables))return 1;
 for(std::uint32_t i=0;i<selected->count;++i){const Skill76* row=nullptr;if(skill(tables,selected->ids[i],row))return 1;
  if(row->words[7]&0x02000000u)return dh2_character_cancel_skill(c->ai,i,services);
 }return 0;
}
