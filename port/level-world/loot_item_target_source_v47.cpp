#include "loot_item_target_source_v47.hpp"
namespace dh2::character {
LootItemTargetSourceV47::LootItemTargetSourceV47(WorldItemLiveOwnerV5& items,
 world::CanonicalObjectManagerV1& manager,TargetServices16 preceding,const data::AiTables& ai,
 data::PropertyView& props,void* context,bool(*position)(void*,std::uintptr_t,const float*&,std::string&))
 :items_(items),manager_(manager),preceding_(preceding),ai_(ai),owner_properties_(props),
 context_(context),owner_position_(position){}
int LootItemTargetSourceV47::invoke(void* raw,TargetState48* state,const TargetRequest24* q,std::uint32_t* out){
 auto& self=*static_cast<LootItemTargetSourceV47*>(raw);
 if(!state||!q||!out||q->reserved)return 1;
 if(q->service!=target_virtual_dead&&q->service!=target_in_sight)
  return self.preceding_.invoke?self.preceding_.invoke(self.preceding_.context,state,q,out):1;
 auto item=self.items_.factory().find(q->subject);
 if(!item)return self.preceding_.invoke?self.preceding_.invoke(self.preceding_.context,state,q,out):1;
 const auto* canonical=self.manager_.object(item->base().shared_handle().key);
 if(!canonical||canonical->identity!=q->subject||!canonical->type_f4||*canonical->type_f4!=3||
    canonical->shared_handle!=&item->base().shared_handle()||!canonical->lease){
  self.error_="Required SAME published pooled Item target Handle/lifetime";return 1;
 }
 if(q->service==target_virtual_dead){
  // Actual Item inherits GameObject.IsDead3400ac, mov r0,0; bx lr.
  *out=0;return 0;
 }
 if(!state->owner||!self.owner_position_||!self.owner_properties_.resolved){
  self.error_="Required SAME owner GetTargetPosition/AI radius";return 1;
 }
 const float* owner{};
 if(!self.owner_position_(self.context_,state->owner->identity,owner,self.error_)||!owner)return 1;
 auto& base=item->base();const auto* node=base.pointer(0x180);
 const auto* visible=base.byte(0x80);const float* selected=base.vector3(0x160);
 if(!node){self.error_="Required Item target cached-node180 source field";return 1;}
 if(*node){
  if(!visible){self.error_="Required positive cached-node Item source visible80";return 1;}
  if(*visible){selected=base.vector3(0x184);if(!selected){self.error_="Required positive Item cached target position184 producer";return 1;}}
 }
 const auto* ai=data::ai_props(self.ai_,self.owner_properties_.resolved[1]);
 if(!selected||!ai){self.error_="Required Item GetTargetPosition/owner AI fallback8";return 1;}
 return dh2_character_target_sight(out,owner,selected,ai->view_radius);
}
}
