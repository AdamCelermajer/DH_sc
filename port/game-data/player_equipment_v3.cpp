#include "player_equipment_v3.hpp"
#include <cstddef>
#include <cstring>
#include <initializer_list>
using namespace dh2::data;
namespace {
bool span(const void* p,std::size_t n,std::size_t align){auto a=reinterpret_cast<std::uintptr_t>(p);return p&&a%align==0&&a+n>=a;}
bool slot_valid(const EquipmentSlot16V3* p){if(!span(p,sizeof(*p),8))return false;for(auto v:p->reserved)if(v)return false;return !p->item||(span(p->item,sizeof(*p->item),8)&&!p->item->reserved);}
bool valid(const EquipmentState72V3* s){if(!(span(s,sizeof(*s),alignof(EquipmentState72V3))&&s->owner&&!s->reserved&&!s->reserved2&&s->count<=65536&&s->slots&&s->slots<=9&&s->selected>=0&&s->selected<=1&&(!s->count||span(s->items,sizeof(void*)*s->count,8))&&span(s->equipment[0],8*s->slots,8)&&span(s->equipment[1],8*s->slots,8)&&s->table_count<=65536&&(!s->table_count||span(s->table,12*s->table_count,4))))return false;
 for(std::uint32_t i=0;i<s->count;++i)if(s->items[i]&&!slot_valid(s->items[i]))return false;
 for(unsigned set=0;set<2;++set)for(std::uint32_t i=0;i<s->slots;++i)if(s->equipment[set][i]&&!slot_valid(s->equipment[set][i]))return false;
 return true;}
bool service(const EquipmentServices16V3* p){return span(p,sizeof(*p),8)&&p->invoke;}
int set(const EquipmentState72V3* s,std::int32_t slot){return slot<0||slot==1||slot==2?s->selected:0;}
const EquipmentRow12V3* row(const EquipmentState72V3* s,const EquipmentItem16V3* i){if(!span(i,sizeof(*i),8)||i->reserved||i->id<0||std::uint32_t(i->id)>=s->table_count)return nullptr;return s->table+i->id;}
int invoke(EquipmentState72V3* s,const EquipmentServices16V3* p,std::uint32_t op,std::uint32_t caller,EquipmentItem16V3* item,std::int32_t a=0,std::int32_t b=0,std::int32_t c=0,EquipmentResponse16V3* response=nullptr){if(!service(p))return -2;EquipmentRequest40V3 q{s->owner,item,op,caller,{a,b,c,0}};EquipmentResponse16V3 r{};if(p->invoke(p->context,s,&q,&r)!=0)return -2;if(response)*response=r;return 0;}
int two(const EquipmentState72V3* s,bool ignore,std::int32_t& out){out=0;if(s->slots<=1)return -3;auto* p=s->equipment[set(s,1)][1];if(!p)return 0;if(!slot_valid(p))return -1;auto* r=row(s,p->item);if(!r)return -3;if(std::uint32_t(r->type-std::uint32_t(4))<=1||ignore){out=r->slotting==-4;return 0;}out=r->slotting==-4&&s->flag1324==0;return 0;}
int unequip(EquipmentState72V3* s,std::uint32_t slot,std::int32_t selected,const EquipmentServices16V3* p){if(slot>=s->slots)return -3;auto active=selected==-1?set(s,slot):selected;if(active<0||active>1)return -3;auto* old=s->equipment[active][slot];s->equipment[active][slot]=nullptr;if(!old)return 0;if(!slot_valid(old))return -1;old->slots[active]=-1;std::int16_t packed;std::memcpy(&packed,old->slots,2);if(packed!=-1)return 0;auto* info=row(s,old->item);if(!info)return -3;if(!std::uint8_t(info->stackable))return 0;
 EquipmentResponse16V3 found{};auto rc=invoke(s,p,has_like,0x400464,old->item,0,0,0,&found);if(rc)return rc;if(!found.value)return 0;EquipmentResponse16V3 equipped{};rc=invoke(s,p,is_equipped,0x400478,nullptr,std::int32_t(found.index),0,0,&equipped);if(rc)return rc;if(equipped.value)return 0;if(found.index>=s->count||!slot_valid(s->items[found.index]))return -3;auto* target=s->items[found.index]->item;if(!target)return -3;auto* source=old->item;if(!row(s,source))return -3;rc=invoke(s,p,add_quantity,0x400498,target,source->quantity);if(rc)return rc;return invoke(s,p,delete_item,0x4004a4,old->item);
}
int equip(EquipmentState72V3* s,std::uint32_t requested,std::uint32_t index,bool forced,const EquipmentServices16V3* p){if(requested>=s->slots||index>=s->count)return -3;auto* itemslot=s->items[index];if(!itemslot)return 0;if(!slot_valid(itemslot))return -1;if(!itemslot->item)return 0;auto active=set(s,requested);auto* info=row(s,itemslot->item);if(!info)return -3;auto slotting=info->slotting;info=row(s,itemslot->item);if(info->type!=5&&info->type!=4&&slotting==-4&&s->flag1324)slotting=1;info=row(s,itemslot->item);if(info->slotting==-1)return 0;
 auto oldslot=itemslot->slots[active];if(oldslot==std::int32_t(requested)&&s->equipment[active][requested]==itemslot)return 0;auto rc=unequip(s,requested,-1,p);if(rc)return rc;oldslot=itemslot->slots[active];if(oldslot!=-1){if(oldslot<0||std::uint32_t(oldslot)>=s->slots)return -3;if(s->equipment[active][std::uint32_t(oldslot)]==itemslot){rc=unequip(s,std::uint32_t(oldslot),-1,p);if(rc)return rc;}}
 auto target=requested;
 if(slotting==-4){if(!forced){rc=unequip(s,2,-1,p);if(rc)return rc;}rc=unequip(s,1,-1,p);if(rc)return rc;target=1;}
 else if(requested==2){std::int32_t has;rc=two(s,false,has);if(rc)return rc;if(has&&!forced){rc=unequip(s,1,-1,p);if(rc)return rc;target=2;}}
 if(target>=s->slots)return -3;auto* item=itemslot->item;if(!row(s,item))return -3;EquipmentItem16V3* remainder=nullptr;if(item->quantity!=1){EquipmentResponse16V3 split_result{};rc=invoke(s,p,split,0x400808,item,std::int32_t(item->quantity)-1,0,0,&split_result);if(rc)return rc;remainder=split_result.item;if(!remainder)return -3;}
 s->equipment[active][target]=itemslot;auto* live=s->equipment[active][target];if(!slot_valid(live))return -1;live->slots[active]=std::int8_t(target);
 if(remainder)return invoke(s,p,force_add,0x400854,remainder,1,1);return 0;
}
int automatic(std::int32_t& result,EquipmentState72V3* s,std::uint32_t index,const EquipmentServices16V3* p){result=0;if(index>=s->count)return -3;auto* itemslot=s->items[index];if(!slot_valid(itemslot))return -1;auto* info=row(s,itemslot->item);if(!info)return -3;if(info->slotting==-1)return 0;auto slotting=info->slotting;info=row(s,itemslot->item);auto type=info->type;
 if(type!=5&&type!=4){if(slotting==1&&s->flag1320)slotting=-3;else if(slotting==-4&&s->flag1324)slotting=1;}
 int rc;
 if(slotting>=0&&std::uint32_t(slotting)<s->slots){if(slotting==2){std::int32_t has;rc=two(s,false,has);if(rc)return rc;if(has){rc=unequip(s,1,-1,p);if(rc)return rc;}}rc=equip(s,std::uint32_t(slotting),index,false,p);}
 else if(slotting==-3||slotting==-2){auto first=slotting==-3?1u:5u,second=first+1;if(second>=s->slots)return -3;auto active=set(s,first);std::uint32_t chosen=first;if(s->equipment[active][first]){active=set(s,second);if(s->equipment[active][second])return 0;chosen=second;}rc=equip(s,chosen,index,false,p);}
 else if(slotting==-4){rc=unequip(s,2,-1,p);if(rc)return rc;rc=equip(s,1,index,false,p);}
 else return 0;
 if(rc)return rc;result=1;return 0;
}
bool output(std::int32_t* p,const EquipmentState72V3* s,const EquipmentServices16V3* svc=nullptr){if(!span(p,4,4))return false;auto a=reinterpret_cast<std::uintptr_t>(p);auto disjoint=[a](const void* q,std::size_t n){auto b=reinterpret_cast<std::uintptr_t>(q);return a+4<=b||a>=b+n;};if(!disjoint(s,sizeof(*s))||!disjoint(s->items,8*s->count)||!disjoint(s->table,12*s->table_count)||(svc&&!disjoint(svc,sizeof(*svc))))return false;for(unsigned set=0;set<2;++set)if(!disjoint(s->equipment[set],8*s->slots))return false;for(std::uint32_t i=0;i<s->count;++i){auto* q=s->items[i];if(q&&(!disjoint(q,sizeof(*q))||(q->item&&!disjoint(q->item,sizeof(*q->item)))))return false;}for(unsigned set=0;set<2;++set)for(std::uint32_t i=0;i<s->slots;++i){auto* q=s->equipment[set][i];if(q&&(!disjoint(q,sizeof(*q))||(q->item&&!disjoint(q->item,sizeof(*q->item)))))return false;}return true;}
}
extern "C" int dh2_equipment_auto_v3(std::int32_t* out,EquipmentState72V3* s,std::uint32_t i,const EquipmentServices16V3* p) noexcept{if(!valid(s)||!output(out,s,p)||!service(p))return -1;std::int32_t result;auto rc=automatic(result,s,i,p);if(!rc)*out=result;return rc;}
extern "C" int dh2_equipment_character_auto_v3(std::int32_t* out,EquipmentState72V3* s,std::uint32_t i,const EquipmentServices16V3* p) noexcept{if(!valid(s)||!output(out,s,p)||!service(p))return -1;std::int32_t result;auto rc=automatic(result,s,i,p);if(rc)return rc;for(auto op:{update_gear_properties,skin,validate_hp_mp}){rc=invoke(s,p,op,op==update_gear_properties?0x3a9fc0:op==skin?0x3a9fc8:0x3a9fd0,nullptr);if(rc)return rc;}*out=result;return 0;}
extern "C" int dh2_equipment_to_slot_v3(EquipmentState72V3* s,std::uint32_t slot,std::uint32_t i,std::uint32_t force,const EquipmentServices16V3* p) noexcept{if(!valid(s)||!service(p)||force>1)return -1;return equip(s,slot,i,force!=0,p);}
extern "C" int dh2_equipment_from_slot_v3(EquipmentState72V3* s,std::uint32_t slot,std::int32_t selected,const EquipmentServices16V3* p) noexcept{if(!valid(s)||!service(p))return -1;return unequip(s,slot,selected,p);}
extern "C" int dh2_equipment_has_two_hander_v3(std::int32_t* out,const EquipmentState72V3* s,std::uint32_t ignore) noexcept{if(!valid(s)||!output(out,s)||ignore>1)return -1;std::int32_t result;auto rc=two(s,ignore!=0,result);if(!rc)*out=result;return rc;}
extern "C" int dh2_equipment_slot_taken_v3(std::int32_t* out,const EquipmentState72V3* s,std::uint32_t slot) noexcept{if(!valid(s)||!output(out,s))return -1;if(slot>=s->slots)return -3;*out=s->equipment[set(s,slot)][slot]!=nullptr;return 0;}
