#include "character_kill_contributor_event_v23.hpp"
#include <algorithm>
#include <stdexcept>
namespace dh2::character {
CharacterKillContributorEventV23::CharacterKillContributorEventV23(AIEventState64& ai,ScriptOwner& scripts,KillContributorServicesV23 s):ai_(ai),scripts_(&scripts),services_(std::move(s)){keys();}
CharacterKillContributorEventV23::CharacterKillContributorEventV23(AIEventState64& ai,ScriptOwnerV2& scripts,KillContributorServicesV23 s):ai_(ai),player_scripts_(&scripts),services_(std::move(s)){keys();}
void CharacterKillContributorEventV23::keys(){
 if(!services_.actual_receiver||!ai_.owner||!ai_.ai_virtuals||(ai_.ai_virtuals[0xb0/4]&&ai_.ai_virtuals[0xb0/4]!=0x3d0d80))throw std::invalid_argument("Required SAME CharAI captured+b0 callable metadata");
 previous_=ai_.ai_virtuals;std::copy_n(previous_,keys_.size(),keys_.begin());keys_[0xb0/4]=0x3d0d80;ai_.ai_virtuals=keys_.data();
}
CharacterKillContributorEventV23::~CharacterKillContributorEventV23(){if(ai_.ai_virtuals==keys_.data())ai_.ai_virtuals=previous_;}
int CharacterKillContributorEventV23::service(void* p,AIEventState64* state,const AIEventRequest40* q,std::uint32_t*){
 auto& t=*static_cast<CharacterKillContributorEventV23*>(p);if(!q||state!=&t.ai_||q->event!=4){t.error_="Required actual contributor event4 receiver";return -1;}
 if(q->service==ai_event_state_event){if(q->subject!=state->owner->state_machine||!t.services_.state_event||!t.services_.state_event(4,q->payload,t.scope_,t.error_)){if(t.error_.empty())t.error_="Required SAME source event4 FSM branch";return -1;}return 0;}
 if(q->service!=ai_event_virtual||q->operation!=0xb0||q->callee!=0x3d0d80||q->subject!=state->ai){t.error_="Required original CharAI::OnKill3d0d80";return -1;}
 // Whole CharAI OnKill: a null selected AIS returns without VM delivery.
 const auto active=state->active;if(!active)return 0;
 ScriptSessionView selected{};std::uintptr_t method{};if(!(t.scripts_?t.scripts_->find(active,selected):t.player_scripts_->find(active,selected))||!character_ais_kill_method_v23(selected,method)){t.error_="Required same selected OnKill AIS constructor";return -1;}
 return t.scripts_?character_ais_kill_vm_v23(*t.scripts_,active,method,q->payload,t.scope_,t.error_):character_ais_kill_vm_v23(*t.player_scripts_,active,method,q->payload,t.scope_,t.error_);
}
bool CharacterKillContributorEventV23::raise(std::uintptr_t killed,const dh2_script_callback_scope* scope,std::string& e){
 e.clear();
 if(ai_.active!=(scripts_?scripts_->lifecycle().active:player_scripts_->lifecycle().active)){e="Required SAME live selected AIS before contributor RaiseEvent4";return false;}
 const auto* old=scope_;scope_=scope;struct Restore{const dh2_script_callback_scope*& field;const dh2_script_callback_scope* old;~Restore(){field=old;}}restore{scope_,old};error_.clear();
 AIEventPayload24 payload{killed,0,0,0};AIEventResult16 result{};const AIEventServices24 services{this,service,3,0};
 const auto status=dh2_character_ai_event(&result,&ai_,4,&payload,&services);if(status){e=error_.empty()?"Required source contributor event4 phase "+std::to_string(result.phase):error_;return false;}e=error_;return true;
}
}
