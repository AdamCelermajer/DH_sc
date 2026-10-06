#include "character_world_ranged_attack_v41.hpp"
#include <cstring>
namespace dh2::character::skills {
CharacterWorldRangedAttackV41::CharacterWorldRangedAttackV41(CharacterWorldRuntimeV1& w,CharacterWorldAttackGeometryV1& g,DebugSwitches& d,const DebugFileServices24& f,WorldRangedAttackBorrowV41 b,WorldRangedAttackBackendsV41 s,std::uint32_t n):world_(w),geometry_(g),debug_(d),files_(f),b_(std::move(b)),s_(s){if(n&&n<=65536){heap_.resize(n);ordered_.reserve(n);}}
bool CharacterWorldRangedAttackV41::refresh(){
 if(!b_.lifetime||!b_.fields||!b_.target||!b_.target->owner||!b_.machine||!b_.heading_active1b5||*b_.heading_active1b5>255||b_.fields->owner!=b_.target->owner->identity||world_.state_borrow(b_.fields->owner)!=b_.machine){error_="Required SAME registered ranged Character/AI/FSM/heading lifetime";return false;}
 b_.fields->target=b_.target->target;b_.fields->last_target=b_.target->last_target;b_.fields->owner_flags528=b_.machine->attack_gate;b_.fields->heading_active=*b_.heading_active1b5;return true;
}
bool CharacterWorldRangedAttackV41::parameters(std::int32_t out[3],bool& enabled){
 WorldTargetActorBorrowV1 a{};if(world_.actor(b_.fields->owner,&a)||!a.character||!a.character->resolved){error_="Required same ranged cached Character properties";return false;}
 CombatProperties896 properties{};std::memcpy(properties.words,a.character->resolved,sizeof properties.words);
 if(properties.words[32]!=-1){const int r=dh2_attack_range_parameters(out,&properties,nullptr,nullptr,0);enabled=r>0;return r>=0;}
 WorldAttackInventoryBorrowV1 inventory{};
 if(!s_.inventory||s_.inventory(s_.context,b_.fields->owner,&inventory)){error_="Required SAME actual ranged inventory";return false;}
 if(inventory.owned){
  const auto& inv=*inventory.owned;if(inv.character()!=b_.fields->owner||!inv.properties()||inv.properties()->resolved.data()!=a.character->resolved){error_="Ranged inventory/property authority mismatch";return false;}
  const int selected=inv.current_equipment();if(selected<0||std::size_t(selected)>=inv.equipment().size()){error_="Invalid source ranged selected equipment";return false;}
  const auto* slot=inv.equipment()[selected][1];if(!slot){enabled=false;return true;}if(!slot->item){error_="Required actual main-hand Item backing";return false;}
  const auto* row=data::item(inv.table(),slot->item->id);if(!row){error_="Required actual ranged ItemTable row";return false;}
  // Same ItemInventory.CanRangeAttack3ffebc leaves outputs unchanged unless
  // real type4/5. Direct typed row borrow avoids remapping item IDs to scratch.
  enabled=row->record.words[22]==4||row->record.words[22]==5;
  if(enabled)std::memcpy(out,row->record.words+38,12);return true;
 }
 if(!inventory.projection||inventory.resolved!=a.character->resolved){error_="Required source-owned ranged inventory projection";return false;}
 const int r=dh2_attack_range_parameters(out,&properties,inventory.projection,inventory.rows,inventory.row_count);if(r<0){error_="Malformed actual ranged inventory parameters";return false;}enabled=r>0;return true;
}
int CharacterWorldRangedAttackV41::search(void* raw,const target_search::Request24* q,target_search::Response16* out){auto& r=*static_cast<CharacterWorldRangedAttackV41*>(raw);if(!q||!out)return -1;if(q->service==target_search::melee_radius)return r.geometry_.melee_radius(q->subject,out->number,r.error_)?0:-1;auto s=r.world_.targets().search_services();return s.invoke(s.context,q,out);}
int CharacterWorldRangedAttackV41::invoke(void* raw,AttackState64& fields,const RangedAttackRequestV41& q,RangedAttackResponseV41& out,std::string& error){auto& r=*static_cast<CharacterWorldRangedAttackV41*>(raw);if(&fields!=r.b_.fields||!r.refresh()){error=r.error_;return -1;}const int result=r.call(q,out);if(!r.refresh())return -1;if(result)error=r.error_.empty()?"Required world ranged source operation "+std::to_string(unsigned(q.operation)):r.error_;return result;}
int CharacterWorldRangedAttackV41::call(const RangedAttackRequestV41& q,RangedAttackResponseV41& out){
 using O=RangedAttackOperationV41;const auto owner=b_.fields->owner;
 if(q.operation==O::range_parameters){bool value{};if(!parameters(out.parameters,value))return -1;out.word=value;return 0;}
 if(q.operation==O::owner_dead||q.operation==O::target_dead||q.operation==O::owner_player){auto s=world_.targets().search_services();target_search::Response16 response{};const target_search::Request24 request{q.operation==O::owner_player?target_search::is_player:target_search::is_dead,0,q.operation==O::target_dead?q.other:owner,0};if(s.invoke(s.context,&request,&response)){error_=world_.targets().error();return -1;}out.word=std::uint32_t(response.word);return 0;}
 if(q.operation==O::is_attacking){out.word=b_.machine->current==5;return 0;}
 if(q.operation==O::debug){if(!q.name||dh2_character_debug_load(&debug_,&files_)!=1||dh2_character_debug_get(&out.word,&debug_,q.name,&files_)!=1){error_="Required ranged DebugSwitches load/query";return -1;}return 0;}
 if(q.operation==O::can_attack_current){bool allowed{};if(!world_ai_can_attack_v1(owner,0,b_.target->target,s_.queries,allowed,error_))return -1;out.word=allowed;return 0;}
 if(q.operation==O::frontal_angle){std::int32_t v;if(!s_.frontal_angle||s_.frontal_angle(s_.context,&v,error_))return -1;std::memcpy(&out.word,&v,4);return 0;}
 if(q.operation==O::look_at){
  // Source3d0974/3d0900 calls GameObject.LookAt directly, not Cmd_LookAt.
  // Reuse registered target-node/heading producers without controller gates.
  if(s_.look_at)return s_.look_at(s_.context,owner,q.other,error_);
  auto controls=world_.targets().control_services_v38();if(dh2_character_control(owner,controller_look_object,q.other,&controls)!=1){error_=world_.targets().error();return -1;}return 0;
 }
 if(q.operation==O::set_target){if(!b_.target_services||dh2_character_ai_set_target(b_.target,q.other,0,b_.target_services)){error_="Required SAME ranged source SetTarget";return -1;}return 0;}
 if(q.operation==O::set_attack_state)return s_.set_attack_state?s_.set_attack_state(s_.context,owner,q.other,false,error_):-1;
 if(q.operation==O::melee_fallback)return s_.melee_fallback?s_.melee_fallback(s_.context,owner,q.other,q.speculative,error_):-1;
 target_search::Services16 services{this,search};
 if(q.operation==O::list_create){WorldTargetActorBorrowV1 a{};if(heap_.empty()||world_.refresh()||world_.actor(owner,&a)||!a.search||target_search::dh2_target_list_init(&list_,heap_.data(),std::uint32_t(heap_.size()),a.search,1,&services)){error_="Required same ranged TargetList constructor";return -1;}ordered_.clear();view_={reinterpret_cast<std::uintptr_t>(this),nullptr,0,0};out.list=&view_;return 0;}
 if(q.other!=reinterpret_cast<std::uintptr_t>(&view_)){error_="Ranged target-list lifetime mismatch";return -1;}
 if(q.operation==O::list_frontal_sort){ordered_.clear();view_.entries=nullptr;view_.count=view_.cursor=0;return target_frontal_sort_v41(list_,error_);}
 if(q.operation==O::list_search){if(world_.refresh()||target_search::dh2_target_search(&list_,&world_.registry(),q.radius,q.cone,&services)){error_="Required source ranged world Search";return -1;}ordered_.clear();target_search::Target24 picked{};while(list_.count){if(target_search::dh2_target_pop(&list_,&picked))return -1;ordered_.push_back(picked.identity);}view_.entries=ordered_.data();view_.count=std::uint32_t(ordered_.size());view_.cursor=0;return 0;}
 if(q.operation==O::list_destroy){ordered_.clear();view_={};list_.count=0;return 0;}
 error_="Unknown ranged source operation";return -1;
}
int CharacterWorldRangedAttackV41::attack(std::uintptr_t requested,std::uint32_t speculative){error_.clear();if(!refresh())return -1;const RangedAttackServicesV41 services{this,invoke};return character_ranged_attack_v41(*b_.fields,requested,speculative,services,error_);}
}
