#include "player_gear_effects_v5.hpp"
#include <cstring>
namespace dh2::data {
const char* gear_modular_category_v5(std::uint32_t slot) noexcept {
 static constexpr const char* names[]={"MC_Torso","MC_RWeapon","MC_LWeapon","MC_Feet","MC_Hands",nullptr,nullptr,nullptr,"MC_Head"};return slot<9?names[slot]:nullptr;
}
PlayerGearEffectsV5::PlayerGearEffectsV5(FreshInventoryOwnedV4& i,PropertyView& p,const ClassRow* c,std::uint32_t n,ItemPowerTablesV5::Borrow powers):inventory_(&i),properties_(&p),classes_(c),class_count_(n),powers_(std::move(powers)){}
bool PlayerGearEffectsV5::valid(std::string& e)const {
 if(dh2_property_validate(properties_)||!classes_||!class_count_||class_count_>65536||!powers_){e="Gear effects require genuine property/class/power backing";return false;}
 const auto& s=*inventory_->properties();if(properties_->base!=s.base.data()||properties_->saved!=s.saved.data()||properties_->gear!=s.gear.data()||properties_->resolved!=s.resolved.data()){e="Gear PropertyView does not belong to the inventory Character";return false;}return true;
}
bool PlayerGearEffectsV5::update_properties(std::string& e){
 e.clear();if(!valid(e))return false;auto& s=*inventory_->properties();if(dh2_gear_reset_v5(s.gear.data(),properties_->defaults)){e="Invalid gear/default span";return false;}
 for(unsigned slot=0;slot<9;++slot){auto* p=inventory_->equipment()[(slot==1||slot==2)?inventory_->current_equipment():0][slot];if(!p)continue;auto* instance=p->item.get();if(!instance){e="Invalid equipped native item";return false;}auto id=instance->id;auto* metadata=item(inventory_->table(),id);if(!metadata){e="Gear item metadata unavailable";return false;}
  if(dh2_gear_stats_v5(s.gear.data(),properties_->defaults,&metadata->record,slot==2)){e="Invalid source gear stats projection";return false;}
  for(std::size_t k=0;k<instance->powers.size();++k){auto power=instance->powers[k];if(power<0||std::size_t(power)>=powers_.rows().size()){e="Equipped power metadata unavailable";return false;}auto& row=powers_.rows()[power];GearPowerView16V5 view{row.properties.data(),std::uint32_t(row.properties.size()),0};if(dh2_gear_power_v5(s.gear.data(),properties_->defaults,&view,slot==2)){e="Invalid source gear power projection";return false;}}
 }
 if(dh2_class_recalc_base(classes_,class_count_,s.base.data(),properties_)){e="Required class recalculation failed after gear prefix";return false;}return true;
}
bool PlayerGearEffectsV5::validate_hp_mp(std::string& e){e.clear();if(!valid(e))return false;if(dh2_gear_validate_vitals_v5(properties_)){e="Invalid gear vitals projection";return false;}return true;}
bool PlayerGearEffectsV5::update_skin(const std::uintptr_t* visual,const GearSkinServicesV5& svc,std::string& e){
 e.clear();if(!visual||reinterpret_cast<std::uintptr_t>(visual)%alignof(std::uintptr_t)){e="Missing live source VisualObject field";return false;}if(!*visual)return true;
 if(!svc.invoke){e="Skin requires native VisualObject/Debug/resource services";return false;}
 auto call=[&](GearSkinOperationV5 op,const char* name,std::int32_t cat,std::int32_t module,int slot,int mode,std::int32_t& result){GearSkinRequestV5 q{op,op==GearSkinOperationV5::debug_load||op==GearSkinOperationV5::debug_query?0:*visual,name,cat,module,slot,mode};if(!svc.invoke(svc.context,*inventory_,q,result,e)){if(e.empty())e="Required Skin provider failed";return false;}return true;};
 for(unsigned slot=0;slot<9;++slot){auto* selected=inventory_->equipment()[(slot==1||slot==2)?inventory_->current_equipment():0][slot];const char* category=gear_modular_category_v5(slot);if(!category)continue;const bool equipped=selected!=nullptr;std::string name;
  if(equipped){auto* metadata=item(inventory_->table(),selected->item->id);if(!metadata){e="Skin item metadata unavailable";return false;}name=metadata->name;}else name=std::string(category)+"__naked";
  std::int32_t r=0;if(slot==1||slot==2){int mode;if(equipped){if(name.find("Shield")!=std::string::npos)mode=0;else if(name.find("Bow")!=std::string::npos)mode=2;else if(slot==2&&name.find("Claw")!=std::string::npos){auto at=name.find("RWeapon");if(at!=std::string::npos)name[at]='L';mode=2;}else mode=int(slot);}else mode=name.find("Shield")!=std::string::npos?0:int(slot);
   if(!call(GearSkinOperationV5::set_weapon,equipped?name.c_str():nullptr,-1,-1,int(slot),mode,r))return false;
  }else {std::int32_t cat,module;if(!call(GearSkinOperationV5::category_id,category,-1,-1,int(slot),0,cat)||!call(GearSkinOperationV5::module_id,name.c_str(),cat,-1,int(slot),0,module))return false;
   if(equipped&&module==-1){name=std::string(category)+"__placeholder";if(!call(GearSkinOperationV5::module_id,name.c_str(),cat,-1,int(slot),0,module))return false;}
   if(!call(GearSkinOperationV5::debug_load,nullptr,-1,-1,int(slot),0,r)||!call(GearSkinOperationV5::debug_query,"isTracingChar_Modular",-1,-1,int(slot),0,r)||!call(GearSkinOperationV5::set_modular,nullptr,cat,module,int(slot),0,r))return false;
  }
 }
 return true;
}
}

