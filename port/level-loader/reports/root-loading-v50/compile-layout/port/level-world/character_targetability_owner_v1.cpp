#include "character_targetability_owner_v1.hpp"
namespace dh2::character {
int CharacterTargetabilityOwnerV1::construct(){
 if(!valid())return -1;
 // Character C1+3c8 calls CharAI C2 at3aa210. C2 mov r0,#1 at3cec18,
 // strb r0,[r4,#4d] at3cec6c. C1 duplicate stores at3cedcc.
 actor_->interactive415=1;return 1;
}
int CharacterTargetabilityOwnerV1::set_is_targetable(std::uint32_t count,std::uint32_t type,bool value){
 if(!valid())return -1;
 // Source3b7a94 count!=0,3b7aa8 first Value.type==1 (Lua boolean).
 // Empty/nonboolean calls return without mutation, including numeric1.
 if(count&&type==1)actor_->interactive415=value?1:0;
 return 1;
}
int CharacterTargetabilityOwnerV1::cancel_sneaking(skills::CharacterPlayerSkillsV6& player){
 if(!valid()||player.session().timers().owner!=actor_->identity||
    player.session().property_view().resolved!=actor_->resolved)return -1;
 return player.native_cancel_sneaking(source_byte415());
}
}
