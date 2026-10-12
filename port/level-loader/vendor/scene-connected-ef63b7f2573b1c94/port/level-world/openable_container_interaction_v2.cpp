#include "openable_container_interaction_v2.hpp"
namespace dh2::world {
namespace {
template<class F,class... A>bool deliver(const F& f,const char* name,std::string& e,A&&... a){
    if(!f){e=std::string("OpenableContainer::Interact required ")+name;return false;}
    return f(std::forward<A>(a)...,e);
}
}
bool openable_container_interact_v2(OpenableContainerOwnerV1& receiver,
 OpenableContainerFieldsV1& fields,const OpenableContainerInteractionServicesV2& s,
 std::uintptr_t actor,std::string& e){
    bool unlocked=false;
    if(!receiver.try_unlock(actor,unlocked,e))return false;
    if(!unlocked)return true;
    std::uintptr_t level=0;
    if(!deliver(s.current_level,"Application::GetCurrentLevel",e,level))return false;
    if(!level&&!deliver(s.assert_missing_level,"source missing-level Debug assertion",e))return false;
    bool hosting=false;if(!deliver(s.local_player_hosting,"PlayerManager::IsLocalPlayerHosting",e,hosting))return false;
    if(hosting){
        OpenableContainerQuestEventV2 event;event.actor=actor;event.data_id=fields.data374;
        if(!deliver(s.constant,"PyDataConstants",e,"v2QuestObjectiveType","OpenGameObject",event.objective_type))return false;
        if(!deliver(s.room64,"canonical GameObject room64",e,event.room))return false;
        if(!deliver(s.raise_async,"same Level EventManager::RaiseAsync",e,level,event))return false;
    }
    if(!receiver.interact_base(actor,e))return false;
    std::uintptr_t character=0;
    if(!deliver(s.handle_as_character,"GetHandle -> AsChar",e,actor,character))return false;
    if(!character)return true;
    bool player=false;if(!deliver(s.is_player,"Character::IsPlayer",e,character,player))return false;
    if(!player)return true;
    if(!deliver(s.props_add_int,"same CharProperties::AddInt",e,character,218,1))return false;
    std::int32_t count=0;if(!deliver(s.props_get_int,"same CharProperties::GetInt",e,character,218,false,count))return false;
    if(count<=99)return true;
    bool local=false;if(!deliver(s.is_local_player,"PlayerManager::IsLocalPlayer",e,character,local))return false;
    if(!local)return true;
    std::int32_t trophy=-1;if(!deliver(s.trophy_name_index,"Arrays::Trophies name lookup",e,"open_100_chests",trophy))return false;
    return deliver(s.unlock_trophy,"TrophyManager::UnlockTrophy",e,trophy);
}
}
