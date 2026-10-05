#include "character_menu_inventory_order_v1.hpp"
#include <cmath>
#include <cstring>
namespace dh2::ui {
bool character_menu_slot_candidate_v1(const data::ItemInstanceV1& instance,const data::ItemRecord164& row,const data::PropertySheet& props,std::uint32_t slot)noexcept{
 (void)instance;if(slot>=9||row.words[26]==-1)return false;
 auto target=row.words[26];auto type=row.words[22];if(type!=4&&type!=5&&target==1&&props[202])target=-3;
 if(target>=0&&target<9)return std::uint32_t(target)==slot;
 if(target==-3)return slot==1||slot==2;if(target==-2)return slot==5||slot==6;return target==-4&&slot==1;
}
bool character_menu_item_name_less_v1(const data::ItemInstanceV1& a,const data::ItemInstanceV1& b)noexcept{if(a.powers.size()!=b.powers.size())return a.powers.size()>b.powers.size();return std::strcmp(a.name.c_str(),b.name.c_str())<0;}
namespace {
std::int32_t score(const data::ItemInstanceV1& item,const data::ItemRecord164& row,std::int32_t actor)noexcept{
 unsigned word=0;switch(actor){case 263:word=8;break;case 264:word=12;break;case 265:word=11;break;case 290:word=9;break;case 291:word=16;break;case 292:word=15;break;case 325:word=10;break;case 326:word=14;break;case 327:word=13;break;default:return item.value;}
 float multiplier;std::memcpy(&multiplier,&row.words[word],4);volatile float value=float(item.value)*multiplier;
 // ARM __aeabi_f2iz: NaN maps0, finite out-of-domain values saturate.
 if(std::isnan(value))return 0;if(value>=2147483648.f)return INT32_MAX;if(value<=-2147483648.f)return INT32_MIN;return std::int32_t(value);
}
}
bool character_menu_item_value_less_v1(const data::ItemInstanceV1& a,const data::ItemRecord164& ar,const data::ItemInstanceV1& b,const data::ItemRecord164& br,std::int32_t actor)noexcept{auto av=score(a,ar,actor),bv=score(b,br,actor);return av!=bv?av>bv:character_menu_item_name_less_v1(a,b);}
bool character_menu_item_equipment_less_v1(const data::ItemInstanceV1& a,const data::ItemRecord164& ar,const data::ItemInstanceV1& b,const data::ItemRecord164& br,std::int32_t actor,bool available_a,bool available_b,bool equipped_a,bool equipped_b,bool requested_a)noexcept{
 if(equipped_a!=equipped_b)return equipped_a;if(equipped_a)return requested_a;
 if(available_a!=available_b)return available_a;return character_menu_item_value_less_v1(a,ar,b,br,actor);
}
}
