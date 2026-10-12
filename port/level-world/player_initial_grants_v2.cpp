#include "player_initial_grants_v2.hpp"
namespace {
bool valid(std::uintptr_t owner,const dh2::player::InitialGrantServices16V2* s){auto p=reinterpret_cast<std::uintptr_t>(s);return owner&&s&&p%alignof(dh2::player::InitialGrantServices16V2)==0&&p<=UINTPTR_MAX-sizeof(*s)&&s->invoke;}
int ask(const dh2::player::InitialGrantServices16V2* s,std::uintptr_t owner,std::uint32_t op,std::int32_t& value,std::int32_t a=0,std::int32_t b=0,std::int32_t c=0,std::int32_t d=0,std::int32_t e=0){dh2::player::InitialGrantRequest32V2 r{owner,op,{a,b,c,d,e}};dh2::player::InitialGrantResponse8V2 out{};if(s->invoke(s->context,&r,&out)||out.reserved)return -2;value=out.value;return 0;}
}
extern "C" int dh2_player_initial_equipment_v2(std::uintptr_t owner,const dh2::player::InitialGrantServices16V2* s) noexcept {using namespace dh2::player;
 if(!valid(owner,s))return -1;std::int32_t v=0;
 if(ask(s,owner,online,v))return -2;if(v){if(ask(s,owner,online_player_record,v,0))return -2;if(v!=1)return 0;}
 if(ask(s,owner,num_items,v))return -2;if(v)return ask(s,owner,update_skin,v);
 if(ask(s,owner,read_gold,v))return -2;if(v)return ask(s,owner,update_skin,v);
 if(ask(s,owner,loot_property,v,9))return -2;auto loot=v;if(ask(s,owner,add_loot,v,loot,0,0,-1,0))return -2;
 if(ask(s,owner,num_items,v))return -2;if(!v)return 0;if(v<0||v>65536)return -2;auto count=v;
 for(std::int32_t i=0;i<count;++i){if(ask(s,owner,is_equippable,v,i))return -2;if(v&&ask(s,owner,auto_equip,v,i))return -2;}return 0;
}
extern "C" int dh2_player_initial_skill_slots_v2(std::uintptr_t owner,const dh2::player::InitialGrantServices16V2* s) noexcept {using namespace dh2::player;
 if(!valid(owner,s))return -1;std::int32_t v=0;if(ask(s,owner,has_skill_slots,v))return -2;if(v)return 0;
 if(ask(s,owner,set_skill_slot,v,0,0)||ask(s,owner,swap_equipment,v)||ask(s,owner,set_skill_slot,v,0,0)||ask(s,owner,swap_equipment,v)||ask(s,owner,skill_level,v,0))return -2;
 if(!v&&ask(s,owner,increment_skill,v,0,0))return -2;return 0;
}
extern "C" int dh2_player_increment_skill_v2(std::int32_t* out,std::uintptr_t owner,std::int32_t row,std::uint32_t test,const dh2::player::InitialGrantServices16V2* s) noexcept {using namespace dh2::player;
 auto p=reinterpret_cast<std::uintptr_t>(out),sp=reinterpret_cast<std::uintptr_t>(s);if(!out||p%4||p>UINTPTR_MAX-4||!valid(owner,s)||row<0||(p<sp+sizeof(*s)&&sp<p+4))return -1;
 auto finish=[&](int value){*out=value;return 0;};std::int32_t v=0;
 if(ask(s,owner,has_savegame,v)||!v||ask(s,owner,has_saved_rows,v)||!v)return -2;
 auto trace=[&](std::int32_t load,std::int32_t query){return ask(s,owner,debug_load,v,load)||ask(s,owner,debug_query,v,query);};
 if(ask(s,owner,property_integer,v,157,0))return -2;if(v<=0){if(trace(0x3bcdb8,0x3bcde0))return -2;return finish(0);}
 if(ask(s,owner,skill_available,v,row))return -2;if(!v){if(trace(0x3bcce0,0x3bcd08))return -2;return finish(0);}
 if(ask(s,owner,skill_limit,v,0))return -2;auto cap=v;if(ask(s,owner,difficulty_unlocked,v))return -2;
 if(v==1){if(ask(s,owner,skill_limit,cap,1))return -2;}else{if(ask(s,owner,difficulty_unlocked,v))return -2;if(v==2&&ask(s,owner,skill_limit,cap,2))return -2;}
 if(ask(s,owner,saved_level_read,v,row))return -2;if(cap<=std::int32_t(std::uint16_t(v)))return finish(0);
 if(ask(s,owner,can_increment,v,row))return -2;if(!v)return finish(0);if(test)return finish(1);
 if(ask(s,owner,property_add,v,157,-1)||ask(s,owner,saved_level_increment,v,row)||ask(s,owner,update_all_skills,v)||ask(s,owner,properties_recalculate,v,1)||ask(s,owner,property_integer,v,194,0))return -2;
 auto capacity=v<0?0:v;if(ask(s,owner,potion_capacity_store,v,std::int32_t(std::uint8_t(capacity)))||trace(0x3bcef0,0x3bcf18))return -2;return finish(1);
}
