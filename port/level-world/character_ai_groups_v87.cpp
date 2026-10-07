#include "character_ai_groups_v87.hpp"
#include "canonical_character_candidate_v60.hpp"
#include "canonical_character_spawn_select_v87.hpp"
#include <algorithm>
#include <map>
namespace dh2::character {
namespace {
using Record=world::CanonicalCharacterCandidateRecordV60;
using Key=std::pair<std::string,std::int32_t>;
std::map<Key,std::shared_ptr<CharacterAiGroupV87>> source_groups;
bool required(std::string& e){e="Required SAME produced CharAI GroupInfo/member/role";return false;}
}
bool source_character_add_to_group_v87(const std::shared_ptr<Record>& r,std::string& e){
 if(!r||!r->actor||!r->actor->object)return required(e);
 auto* name=r->actor->source_string(0x1400);if(!name)return required(e);
 if(name->empty()){e.clear();return true;} //3d380c literal return
 auto* role=r->actor->source_string(0x1418);if(!role)return required(e);
 std::int32_t value;
 if(*role=="Normal")value=0;else if(*role=="Leader")value=1;
 else if(*role=="MasterLeader")value=2;else if(*role=="Group")value=3;
 else {e.clear();return true;} //source unknown-role return, no pointer store
 const auto canonical=r->actor->canonical(r);
 if(!canonical.room64)return required(e);
 auto& group=source_groups[{*name,static_cast<std::int32_t>(*canonical.room64)}];
 if(!group)group=std::make_shared<CharacterAiGroupV87>();
 auto& members=value==2?group->master0_:value==1?group->leader_c_:group->ordinary18_;
 //Original append precedes Character3fc/400 stores. No duplicate suppression.
 members.push_back(r);
 r->ai_group_v87=group;r->actor->source_ai_group_role38=value;
 r->actor->source_ai_group34=reinterpret_cast<std::uintptr_t>(group.get());
 e.clear();return true;
}
bool CharacterAiGroupV87::can_respawn(Record& r,bool& result,std::string& e){
 if(!source_present_||!r.actor||r.ai_group_v87.get()!=this||r.actor->source_ai_group34!=reinterpret_cast<std::uintptr_t>(this))return required(e);
 const auto role=r.actor->source_ai_group_role38;
 if(role==0){result=(byte28_^1u)!=0;e.clear();return true;}
 if(role!=3){result=false;e.clear();return true;}
 if(field24_!=1){result=field24_==2;e.clear();return true;}
 //3d2a80 captures vector length but re-reads each member and real nullable
 //FSM state. An expired native lease is an error, never an invented Limbus.
 const auto count=ordinary18_.size();bool all=true;
 for(std::size_t i=0;i<count;++i){if(i>=ordinary18_.size())return required(e);auto member=ordinary18_[i].lock();
  if(!member||!member->actor||!member->actor->machine)return required(e);
  std::int32_t state{};if(dh2_character_native_fsm_get_integer(&state,&member->actor->machine->native_fsm(),0)!=1)return required(e);
  if(state!=0)all=false;
 }
 field24_=all?2:1;result=all;e.clear();return true;
}
bool CharacterAiGroupV87::remove(Record& r,std::string& e){
 if(!source_present_||!r.actor)return required(e);const auto role=r.actor->source_ai_group_role38;
 if(role<0||role>3){e.clear();return true;}
 auto& members=role==2?master0_:role==1?leader_c_:ordinary18_;
 auto found=std::find_if(members.begin(),members.end(),[&](const auto& weak){auto member=weak.lock();return member.get()==&r;});
 if(found!=members.end())members.erase(found);
 //Original RemoveFromGroup does not clear3fc/400 or erase the map entry.
 e.clear();return true;
}
bool source_character_remove_from_group_v87(Record& r,std::string& e){
 if(!r.actor)return required(e);if(!r.actor->source_ai_group34){e.clear();return true;}
 if(!r.ai_group_v87||r.actor->source_ai_group34!=reinterpret_cast<std::uintptr_t>(r.ai_group_v87.get()))return required(e);
 return r.ai_group_v87->remove(r,e);
}
bool CharacterAiGroupV87::on_died(Record& r,std::uintptr_t attacker,const Kill& kill,std::string& e){
 if(!source_present_||r.ai_group_v87.get()!=this||!r.actor||r.actor->source_ai_group34!=reinterpret_cast<std::uintptr_t>(this))return required(e);
 auto all_dead=[&](auto& list,std::uint8_t& result,bool store_each)->bool{
  const auto count=list.size();result=1;
  for(std::size_t i=0;i<count;++i){
   if(result){if(i>=list.size())return required(e);auto member=list[i].lock();if(!member||!member->life)return required(e);if(!member->life->dead)result=0;}
   if(store_each)byte28_=result;
  }
  return true;
 };
 switch(r.actor->source_ai_group_role38){
 case 2:{
  byte28_=1;
  //Original captures each cohort length separately, and reads current entries
  //on every synchronous Cmd_Kill. Reentrant removal is never snapshot-copied.
  for(auto* list:{&leader_c_,&ordinary18_}){const auto count=list->size();
   for(std::size_t i=0;i<count;++i){if(i>=list->size())return required(e);auto member=(*list)[i].lock();
    if(!member||!member->actor||!member->actor->controller||!kill)return required(e);
    if(!kill(*member,attacker,e))return false;
   }
  }
  break;
 }
 case 1:{if(byte28_)break;byte28_=1;
  if(!leader_c_.empty()){std::uint8_t all{};if(!all_dead(leader_c_,all,true))return false;}
  break;
 }
 case 3:{if(field24_)break;std::uint8_t all{};if(!all_dead(ordinary18_,all,false))return false;field24_=all;break;}
 default:break;
 }
 e.clear();return true;
}
bool source_character_group_on_died_v87(Record& r,std::uintptr_t group,std::uintptr_t attacker,const CharacterAiGroupV87::Kill& kill,std::string& e){
 if(!r.ai_group_v87||group!=reinterpret_cast<std::uintptr_t>(r.ai_group_v87.get()))return required(e);
 return r.ai_group_v87->on_died(r,attacker,kill,e);
}
bool CharacterAiGroupV87::on_enemy_spotted(Record& r,std::string& e){
 if(!source_present_)return required(e);
 if(byte29_){e.clear();return true;}
 if(!r.actor||r.ai_group_v87.get()!=this||r.actor->source_ai_group34!=reinterpret_cast<std::uintptr_t>(this))return required(e);
 auto wake=[&](Record& member)->bool{if(!member.actor||!member.actor->machine)return required(e);
  std::int32_t state{};if(dh2_character_native_fsm_get_integer(&state,&member.actor->machine->native_fsm(),0)!=1)return required(e);
  if(state!=17)return true;return source_character_select_spawn_v87(member,1,0,e);
 };
 auto cohort=[&](auto& list)->bool{const auto count=list.size();for(std::size_t i=0;i<count;++i){
  if(i>=list.size())return required(e);auto member=list[i].lock();if(!member||!wake(*member))return false;
 }return true;};
 switch(r.actor->source_ai_group_role38){
 case 2:if(!cohort(leader_c_))return false;byte29_=1;[[fallthrough]];
 case 1:if(!cohort(ordinary18_)||!wake(r))return false;if(master0_.empty())byte29_=1;break;
 case 3:if(field24_!=-1)break;if(!cohort(ordinary18_))return false;field24_=0;byte29_=1;break;
 case 0:if(!master0_.empty()||!leader_c_.empty())break;if(!cohort(ordinary18_))return false;byte29_=1;break;
 default:break;
 }
 e.clear();return true;
}
bool source_character_group_enemy_spotted_v87(Record& r,std::uintptr_t group,std::string& e){
 if(!r.ai_group_v87||group!=reinterpret_cast<std::uintptr_t>(r.ai_group_v87.get()))return required(e);
 return r.ai_group_v87->on_enemy_spotted(r,e);
}
bool CharacterAiGroupV87::source_limbus_blur_v118(Record& r,std::string& e){
 if(!source_present_||!r.actor||r.ai_group_v87.get()!=this||r.actor->source_ai_group34!=reinterpret_cast<std::uintptr_t>(this))return required(e);
 bool all=true;const auto count=ordinary18_.size();
 for(std::size_t i=0;i<count;++i){if(i>=ordinary18_.size())return required(e);auto member=ordinary18_[i].lock();if(!member||!member->actor||!member->actor->machine)return required(e);if(member.get()==&r)continue;std::int32_t state{};if(dh2_character_native_fsm_get_integer(&state,&member->actor->machine->native_fsm(),0)!=1)return required(e);if(state==0)all=false;}
 field24_=all?0:2;e.clear();return true;
}
bool source_character_can_respawn_v87(Record& r,bool& result,std::string& e){
 if(!r.actor||!r.properties)return required(e);const auto* blocked=r.actor->source_bool_field(0x1481);if(!blocked)return required(e);
 if(*blocked||r.properties->resolved[11]<=0){result=false;e.clear();return true;}
 if(!r.actor->source_ai_group34){result=true;e.clear();return true;}
 if(!r.ai_group_v87||r.actor->source_ai_group34!=reinterpret_cast<std::uintptr_t>(r.ai_group_v87.get()))return required(e);
 return r.ai_group_v87->can_respawn(r,result,e);
}
bool source_character_spawn_predicate_v87(Record& r,std::int32_t state,std::uint32_t& word,std::string& e){
 if(state==0){bool result{};if(!source_character_can_respawn_v87(r,result,e))return false;word=result;return true;}
 if(state!=17){word=1;e.clear();return true;}
 if(!r.actor)return required(e);
 if(r.actor->source_ai_group34){if(!r.ai_group_v87||!r.ai_group_v87->source_present()||r.actor->source_ai_group34!=reinterpret_cast<std::uintptr_t>(r.ai_group_v87.get()))return required(e);word=r.ai_group_v87->can_spawn();e.clear();return true;}
 const auto* spawn=r.actor->source_bool_field(0x1430);if(!spawn)return required(e);word=*spawn;e.clear();return true;
}
void source_character_clear_group_info_v87()noexcept{for(auto& group:source_groups)group.second->source_present_=false;source_groups.clear();}
}
