#include "player_savegame_v1.hpp"
#include <cstring>
#include <limits>
#include <new>
namespace {
using namespace dh2::data;
bool valid(const SavedSkillsView16V1* v){return v&&!v->reserved&&(!v->count||v->rows)&&(!v->rows||reinterpret_cast<std::uintptr_t>(v->rows)%alignof(SavedSkill8V1)==0);}
std::int32_t signed_word(std::uint32_t x){std::int32_t y;std::memcpy(&y,&x,4);return y;}
bool read(PlayerProfileSpan24V1& p,std::uint32_t n,const std::uint8_t*& b,const SavedSkillsLoadServices32V1& s){
 if(n>p.size-p.cursor)return false;b=p.data+p.cursor;p.cursor+=n;if(s.read_observer)s.read_observer(s.context,n);return true;
}
bool word(PlayerProfileSpan24V1& p,std::uint32_t& x,const SavedSkillsLoadServices32V1& s){const std::uint8_t* b;if(!read(p,4,b,s))return false;x=std::uint32_t(b[0])|std::uint32_t(b[1])<<8|std::uint32_t(b[2])<<16|std::uint32_t(b[3])<<24;return true;}
bool string_section(Bytes bytes,std::string& out,std::size_t& used,std::string& error){
 if(!bytes.data||bytes.size<4){error="truncated source string section";return false;}
 auto n=std::uint32_t(bytes.data[0])|std::uint32_t(bytes.data[1])<<8|std::uint32_t(bytes.data[2])<<16|std::uint32_t(bytes.data[3])<<24;
 if(signed_word(n)<=0){out.clear();used=4;error.clear();return true;}
 if(n>bytes.size-4){error="truncated source string payload";return false;}
 if(bytes.data[3+n]!=0){error="unterminated source string payload";return false;}
 out.assign(reinterpret_cast<const char*>(bytes.data+4),n-1);used=4+n;error.clear();return true;
}
}
extern "C" int dh2_player_skills_v1_load(dh2::data::SavedSkillsView16V1* v,dh2::data::PlayerProfileSpan24V1* p,const dh2::data::SavedSkillsLoadServices32V1* s) noexcept {
 if(!valid(v)||!p||!s||!s->skill_id||!s->slot_value||p->reserved0||p->reserved1||p->cursor>p->size||(!p->data&&p->size))return -1;
 try{
 std::uint32_t n;if(!word(*p,n,*s))return -2;
 for(std::int32_t i=0;i<signed_word(n);++i){std::uint32_t len;if(!word(*p,len,*s))return -2;const std::uint8_t* text;
  if(signed_word(len)<=0){text=reinterpret_cast<const std::uint8_t*>("");len=0;}else{if(!read(*p,len,text,*s))return -2;if(text[len-1]!=0)return -1;}
  auto id=s->skill_id(s->context,text,len);SavedSkill8V1* selected=nullptr;
  for(std::uint32_t j=0;j<v->count;++j)if(v->rows[j].id==id){selected=&v->rows[j];break;}
  const std::uint8_t* level;if(!read(*p,2,level,*s))return -2;if(selected)selected->level=std::uint16_t(level[0]|std::uint16_t(level[1])<<8);
 }
 for(std::uint32_t set=0;set<2;++set){if(!word(*p,n,*s))return -2;
  for(std::int32_t i=0;i<signed_word(n);++i){std::uint32_t key,value;if(!word(*p,key,*s))return -2;auto target=s->slot_value(s->context,set,signed_word(key));if(!target)return -3;
   if(!word(*p,value,*s))return -2;*target=value;}
 }
 return 0;
 }catch(...){return -3;}
}
extern "C" int dh2_saved_skill_v1_level(const dh2::data::SavedSkillsView16V1* v,std::uint32_t i) noexcept {return valid(v)&&i<v->count?v->rows[i].level:-1;}
extern "C" int dh2_saved_skill_v1_set_level(dh2::data::SavedSkillsView16V1* v,std::uint32_t i,std::int32_t level) noexcept {if(!valid(v)||i>=v->count)return -1;v->rows[i].level=static_cast<std::uint16_t>(level);return 0;}
extern "C" int dh2_inventory_v1_quantity(std::uint32_t n) noexcept {n&=65535;return n<32768?int(n):int(n)-65536;}
extern "C" int dh2_inventory_v1_current_equipment(std::uint32_t n,std::int32_t requested) noexcept {if(requested>=0&&std::uint32_t(requested)-1>1)return 0;n&=255;return n<128?int(n):int(n)-256;}
extern "C" std::uint32_t dh2_inventory_v1_swap_equipment(std::uint32_t n) noexcept {int x=int(n&255);if(x>=128)x-=256;return static_cast<std::uint32_t>((x+1)%2)&255;}
namespace dh2::data {
bool PlayerSavegameV1::initialize_skills(const std::vector<std::int32_t>& ids,std::string& e){if(skills_initialized_){e.clear();return true;}if(!character_||ids.size()>std::numeric_limits<std::uint32_t>::max()){e="missing actual Character/skill list";return false;}
 try{std::vector<SavedSkill8V1> next;next.reserve(ids.size());for(auto id:ids)next.push_back({id,0,0,0});skills_.swap(next);slots_[0].clear();slots_[1].clear();skills_initialized_=true;e.clear();return true;}catch(...){e="saved skill allocation failed";return false;}}
int PlayerSavegameV1::load_skills(Bytes b,SkillTables::Borrow tables,std::size_t& consumed,std::string& e){if(!character_||!skills_initialized_||!tables||b.size>UINT32_MAX){e="missing Character/initialized skills/SkillTable";return -1;}
 struct Context{PlayerSavegameV1* self;SkillTables::Borrow* tables;}c{this,&tables};SavedSkillsLoadServices32V1 s{&c,
 [](void* p,const std::uint8_t* bytes,std::uint32_t size){std::string name(reinterpret_cast<const char*>(bytes),size);return static_cast<Context*>(p)->tables->skill_index(name.c_str());},
 [](void* p,std::uint32_t set,std::int32_t key)->std::uint32_t*{return &static_cast<Context*>(p)->self->slots_[set][key];},nullptr};
 SavedSkillsView16V1 v{skills_.data(),static_cast<std::uint32_t>(skills_.size()),0};PlayerProfileSpan24V1 span{b.data,static_cast<std::uint32_t>(b.size),0,0,0};int result=dh2_player_skills_v1_load(&v,&span,&s);consumed=span.cursor;e=result?"source skills section failed at byte "+std::to_string(consumed):"";return result;}
bool PlayerSavegameV1::load_name(Bytes b,std::size_t& n,std::string& e){return string_section(b,name_,n,e);}
bool PlayerSavegameV1::load_level(Bytes b,std::size_t& n,std::string& e){if(!b.data||b.size<4){e="truncated source level";return false;}std::uint32_t v=std::uint32_t(b.data[0])|std::uint32_t(b.data[1])<<8|std::uint32_t(b.data[2])<<16|std::uint32_t(b.data[3])<<24;level_=signed_word(v);n=4;e.clear();return true;}
bool PlayerSavegameV1::load_class(Bytes b,const std::vector<std::string>& names,std::size_t& n,std::string& e){std::string key;if(!string_section(b,key,n,e))return false;class_=-1;for(std::size_t i=0;i<names.size();++i)if(std::strcmp(key.c_str(),names[i].c_str())==0){class_=static_cast<std::int32_t>(i);break;}return true;}
bool PlayerSavegameV1::load_location(Bytes b,std::size_t& used,std::string& e){
 used=0;if(b.size>UINT32_MAX||(!b.data&&b.size)){e="invalid source location span";return false;}
 SavedSkillsLoadServices32V1 services{};PlayerProfileSpan24V1 span{b.data,static_cast<std::uint32_t>(b.size),0,0,0};
 auto next=[&](std::uint32_t& value){const bool ok=word(span,value,services);used=span.cursor;if(!ok)e="truncated source location at byte "+std::to_string(used);return ok;};
 std::uint32_t value;if(!next(value))return false;location_.save_date=value;
 for(std::size_t i=0;i<3;++i){
  if(!next(value))return false;location_.levels[i]=signed_word(value);
  if(!next(value))return false;location_.seeds[i]=signed_word(value);
  if(!next(value))return false;location_.current_acts[i]=location_.volatile_acts[i]=signed_word(value);
 }
 e.clear();return true;
}
bool PlayerSavegameV1::load_entry_points(Bytes b,std::size_t& used,std::string& e){
 used=0;if(b.size>UINT32_MAX||(!b.data&&b.size)){e="invalid source entry-point span";return false;}
 SavedSkillsLoadServices32V1 services{};PlayerProfileSpan24V1 span{b.data,static_cast<std::uint32_t>(b.size),0,0,0};
 for(auto& entry:location_.entry_points){std::uint32_t value;if(!word(span,value,services)){used=span.cursor;e="truncated source entry-point section";return false;}entry=signed_word(value);}
 used=span.cursor;e.clear();return true;
}
bool PlayerSavegameV1::load_spawn_points(Bytes b,std::size_t& used,std::string& e){
 used=0;if(b.size>UINT32_MAX||(!b.data&&b.size)){e="invalid source spawn-point span";return false;}
 for(auto& flag:location_.use_spawn_point){if(used>=b.size){e="truncated source spawn-point section";return false;}flag=b.data[used++];}
 e.clear();return true;
}
bool PlayerSavegameV1::set_skill_level(std::uint32_t i,std::int32_t level,std::string& e){SavedSkillsView16V1 v{skills_.data(),static_cast<std::uint32_t>(skills_.size()),0};if(dh2_saved_skill_v1_set_level(&v,i,level)){e="unsafe saved skill index";return false;}e.clear();return true;}
bool PlayerSavegameV1::set_skill_in_slot(std::int32_t key,std::uint32_t row,const SavedSkillUpdateServicesV1& s,std::string& e){if(!skills_initialized_||!character_||key<0||(row!=UINT32_MAX&&row>=skills_.size())){e="invalid source skill assignment";return false;}
 if(row==UINT32_MAX){slots_[0].erase(key);e.clear();return true;}
 if(!s.update_skills){e="required CharAI.UpdateSkills service unavailable";return false;}
 for(auto i=slots_[0].begin();i!=slots_[0].end();)if(i->second==row)i=slots_[0].erase(i);else ++i;slots_[0][key]=row;
 return s.update_skills(s.context,character_,e);}
std::int32_t PlayerSavegameV1::skill_id(std::uint32_t i)const noexcept{return i<skills_.size()?skills_[i].id:-1;}
std::int32_t PlayerSavegameV1::skill_level(std::uint32_t i)const noexcept{return i<skills_.size()?skills_[i].level:-1;}
std::int32_t PlayerSavegameV1::skill_in_slot(std::int32_t key)const noexcept{auto i=slots_[0].find(key);return i==slots_[0].end()?-1:signed_word(i->second);}
std::int32_t PlayerSavegameV1::skill_slot(std::uint32_t row)const noexcept{if(row>=skills_.size())return -1;for(auto& i:slots_[0])if(i.second==row)return i.first;return -1;}
void PlayerSavegameV1::initialize_faeries()noexcept{for(std::size_t i=0;i<3;++i)if(!faeries_initialized_[i]){faeries_[i]={};faeries_initialized_[i]=true;}}
bool PlayerSavegameV1::load_current_faery(Bytes b,std::size_t& used,std::string& e){used=0;for(std::size_t i=0;i<3;++i){if(!faeries_initialized_[i]){e="source faery storage unavailable";return false;}if(!b.data||b.size-used<4){e="truncated current faery";return false;}auto p=b.data+used;auto u=std::uint32_t(p[0])|std::uint32_t(p[1])<<8|std::uint32_t(p[2])<<16|std::uint32_t(p[3])<<24;current_faery_[i]=signed_word(u);used+=4;}e.clear();return true;}
bool PlayerSavegameV1::load_faeries(Bytes b,std::size_t& used,bool& mismatch,std::string& e){used=0;mismatch=false;SavedSkillsLoadServices32V1 s{};PlayerProfileSpan24V1 p{b.data,0,0,0,0};if(b.size>UINT32_MAX||(!b.data&&b.size)){e="invalid faery input";return false;}p.size=static_cast<std::uint32_t>(b.size);
 for(std::size_t i=0;i<3;++i){if(!faeries_initialized_[i]){e="source faery storage unavailable";used=p.cursor;return false;}std::uint32_t id,count;if(!word(p,id,s)){e="truncated current faery";used=p.cursor;return false;}current_faery_[i]=signed_word(id);if(!word(p,count,s)){e="truncated faery count";used=p.cursor;return false;}if(count!=5){mismatch=true;used=p.cursor;e.clear();return true;}
  for(auto& row:faeries_[i]){const std::uint8_t* bytes;if(!read(p,2,bytes,s)){e="truncated faery level";used=p.cursor;return false;}row.level=std::uint16_t(bytes[0]|std::uint16_t(bytes[1])<<8);if(!read(p,1,bytes,s)){e="truncated faery state";used=p.cursor;return false;}row.state=bytes[0];}
 }used=p.cursor;e.clear();return true;}
bool PlayerSavegameV1::load_difficulty(Bytes b,void* ctx,bool (*store)(void*,std::int32_t,std::string&),std::size_t& used,std::string& e){used=0;if(!store||!b.data||b.size<4||b.size>UINT32_MAX){e="required CurrentDifficulty store/input unavailable";return false;}SavedSkillsLoadServices32V1 s{};PlayerProfileSpan24V1 p{b.data,static_cast<std::uint32_t>(b.size),0,0,0};std::uint32_t a;if(!word(p,a,s))return false;used=4;if(!store(ctx,signed_word(a),e))return false;if(!word(p,a,s)){e="truncated unlocked difficulty";return false;}unlocked_difficulty_=signed_word(a);used=8;e.clear();return true;}
bool PlayerSavegameV1::set_faery_level(std::uint32_t id,std::int32_t value,std::uint32_t difficulty,std::string& e){if(difficulty>=3||id>=5||!faeries_initialized_[difficulty]){e="unsafe saved faery index";return false;}faeries_[difficulty][id].level=static_cast<std::uint16_t>(value);e.clear();return true;}
bool PlayerSavegameV1::set_faery_state(std::uint32_t id,std::int32_t value,std::uint32_t difficulty,std::string& e){if(difficulty>=3||id>=5||!faeries_initialized_[difficulty]){e="unsafe saved faery index";return false;}faeries_[difficulty][id].state=static_cast<std::uint8_t>(value);e.clear();return true;}
std::int32_t PlayerSavegameV1::faery_level(std::uint32_t id,std::uint32_t difficulty)const noexcept{return difficulty<3&&id<5&&faeries_initialized_[difficulty]?faeries_[difficulty][id].level:0;}
std::int32_t PlayerSavegameV1::current_faery(std::uint32_t difficulty)const noexcept{return difficulty<3?current_faery_[difficulty]:-1;}
}
