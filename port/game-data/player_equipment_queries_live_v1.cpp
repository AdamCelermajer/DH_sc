#include "player_equipment_queries_live_v1.hpp"
namespace dh2::data {namespace {
bool aligned(const void* p,std::size_t n){const auto a=reinterpret_cast<std::uintptr_t>(p);return p&&a%4==0&&a<=UINTPTR_MAX-n;}
bool overlaps(const void* a,std::size_t an,const void* b,std::size_t bn){const auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);return b&&x<y+bn&&y<x+an;}
}
int equipment_weapon_facts_v1(EquipmentWeaponFacts12V1* out,const ItemRecord164* main,const ItemRecord164* off,std::int32_t flag) noexcept {
 if(!aligned(out,sizeof(*out))||(main&&!aligned(main,sizeof(*main)))||(off&&!aligned(off,sizeof(*off)))||overlaps(out,sizeof(*out),main,sizeof(*main))||overlaps(out,sizeof(*out),off,sizeof(*off)))return -1;
 EquipmentWeaponFacts12V1 q;
 if(main){q.main_category=main->words[37];q.flags|=weapon_main;if(q.main_category==4)q.flags|=weapon_bow;if(q.main_category==5)q.flags|=weapon_staff;const auto type=std::uint32_t(main->words[22]);if(type==4||type==5)q.flags|=weapon_ranged;if(main->words[26]==-4){q.flags|=weapon_two_raw;if(type-4<=1||flag==0)q.flags|=weapon_two_effective;}}
 if(off){q.off_category=off->words[37];q.flags|=off->words[22]==6?weapon_shield:weapon_dual;}
 *out=q;return 0;
}
bool PlayerEquipmentQueriesLiveV1::facts(EquipmentWeaponFacts12V1& out,std::string& error)const {
 error.clear();const auto* state=inventory_->properties();
 if(!state||properties_->base!=state->base.data()||properties_->saved!=state->saved.data()||properties_->gear!=state->gear.data()||properties_->resolved!=state->resolved.data()){
  error="Equipment query requires the exact inventory Character property view";return false;
 }
 const auto selected=inventory_->current_equipment();
 if(selected<0||selected>=2){error="Equipment query selected set outside source domain";return false;}
 const auto& set=inventory_->equipment()[std::uint32_t(selected)];const ItemRecord164* records[2]{};
 for(unsigned j=0;j<2;++j)if(set[j+1]){
  const auto* instance=set[j+1]->item.get();const auto* row=instance?item(inventory_->table(),instance->id):nullptr;
  if(!row){error="Equipment query actual weapon metadata absent";return false;}records[j]=&row->record;
 }
 if(equipment_weapon_facts_v1(&out,records[0],records[1],state->resolved[203])){error="Malformed live equipment query boundary";return false;}
 return true;
}
bool PlayerEquipmentQueriesLiveV1::combat_view(CombatantView& out,std::string& error)const {
 EquipmentWeaponFacts12V1 q;if(!facts(q,error))return false;
 if(q.main_category<-1||q.main_category>=141||q.off_category<-1||q.off_category>=141){error="Combat equipment category outside genuine kernel domain";return false;}
 auto value=out;value.properties=inventory_->properties()->resolved.data();value.main_damage_class=q.main_category;value.off_damage_class=q.off_category;
 value.dual_wield=bool(q.flags&weapon_dual);value.shield=bool(q.flags&weapon_shield);value.two_hander=bool(q.flags&weapon_two_raw);out=value;return true;
}
}
