#include "character_use_ooi_v47.hpp"
#include "character_state.hpp"
#include <stdexcept>
namespace dh2::character {
namespace {
bool require(bool v,const char* what,std::string& e){if(v)return true;e=std::string("Required actual UseOOI V47 ")+what;return false;}
}
CharacterUseOoiV47::CharacterUseOoiV47(CharacterUseOoiFieldsV47 f,CharacterUseOoiServicesV47 s)
 :fields_(std::move(f)),services_(s){
 if(!fields_.receiver_lease||!fields_.character||!fields_.ai3c8||
    !fields_.ooi14a4||!fields_.heading412)
  throw std::invalid_argument("Required SAME Character UseOOI source field lifetimes");
}
bool CharacterUseOoiV47::eligible(bool& out,std::string& e){
 if(!require(bool(services_.state),"CharStateMachine getter",e))return false;
 std::int32_t current{};
 if(!services_.state(services_.context,fields_.character,current,e))return false;
 if(dh2_character_state_is_idle(current,0)){out=true;return true;}
 // Original source reloads SM StateInfo for IsMoving after false IsIdle.
 if(!services_.state(services_.context,fields_.character,current,e))return false;
 out=current==4||current==19;return true;
}
bool CharacterUseOoiV47::set(std::uintptr_t requested,std::string& e){
 const auto result=dh2_character_ai_set_target(fields_.ai3c8,requested,0,&services_.target);
 if(result){e="Required actual UseOOI CharAI.SetTarget service; reached prefix retained ("+std::to_string(result)+")";return false;}
 *fields_.heading412=1;return true;
}
bool CharacterUseOoiV47::force(std::uintptr_t requested,std::string& e){
 if(!requested||fields_.ai3c8->target)return true;
 bool ready{};if(!eligible(ready,e))return false;
 return !ready||set(requested,e);
}
bool CharacterUseOoiV47::use(std::string& e){
 if(!*fields_.ooi14a4||fields_.ai3c8->target)return true;
 bool ready{};if(!eligible(ready,e))return false;
 // Reload after the SM queries, even if a provider changed OOI to NULL.
 return !ready||set(*fields_.ooi14a4,e);
}
bool CharacterUseOoiV47::control(std::uintptr_t requested,std::string& e){
 if(!require(bool(services_.remote34),"Character remote virtual34",e))return false;
 bool remote{};if(!services_.remote34(services_.context,fields_.character,remote,e))return false;
 if(remote)return true;
 if(requested)return force(requested,e);
 return !*fields_.ooi14a4||use(e);
}
ControllerUseOoiV47::ControllerUseOoiV47(ControllerUseOoiFieldsV47 f,ControllerUseOoiServicesV47 s)
 :fields_(std::move(f)),services_(s){
 if(!fields_.receiver_lease||!fields_.controller||!fields_.forced9||!fields_.locked8||
    !fields_.networkA||!fields_.global_blocked||!fields_.actorC||!fields_.controllable4)
  throw std::invalid_argument("Required SAME controller UseOOI source field lifetimes");
}
bool ControllerUseOoiV47::command(std::uintptr_t requested,std::string& e){
 if(!require(*fields_.forced9<=255&&*fields_.locked8<=255&&
    *fields_.networkA<=255&&*fields_.global_blocked<=255,"raw byte projections0..255",e))return false;
 if(!*fields_.forced9&&(*fields_.global_blocked||*fields_.locked8))return true;
 if(!require(bool(services_.online5),"NetworkManager byte5",e))return false;
 bool online{};if(!services_.online5(services_.context,online,e))return false;
 if(online&&*fields_.networkA&&*fields_.actorC){
  if(!require(bool(services_.actor),"network actor source fields",e))return false;
  const auto actor_id=*fields_.actorC;UseOoiNetworkActorV47 actor;
  if(!services_.actor(services_.context,actor_id,actor,e))return false;
  if(!require(actor.identity==actor_id&&actor.receiver_lease&&actor.ooi14a4&&actor.network_id108,"SAME actorC fields",e))return false;
  const auto selected=requested?requested:*actor.ooi14a4;
  if(selected){
   if(!require(bool(services_.is_character24),"selected virtual24",e))return false;
   bool is_character{},dead{};
   if(!services_.is_character24(services_.context,selected,is_character,e))return false;
   if(is_character){
    if(!require(bool(services_.is_dead),"selected Character.IsDead",e)||
       !services_.is_dead(services_.context,selected,dead,e))return false;
   }
   bool skip=dead;
   if(!skip){
    if(!require(bool(services_.interaction90),"selected virtual90",e))return false;
    std::int32_t interaction{};
    if(!services_.interaction90(services_.context,selected,*fields_.actorC,interaction,e))return false;
    skip=interaction==1;
    if(!skip){
     if(!services_.interaction90(services_.context,selected,*fields_.actorC,interaction,e))return false;
     skip=interaction==8;
    }
   }
   if(!skip){
    if(!require(bool(services_.client_get),"Client.Get",e))return false;
    std::uintptr_t client{};std::shared_ptr<void> client_lease;
    if(!services_.client_get(services_.context,client,client_lease,e))return false;
    if(!require(client&&client_lease,"retained Client",e))return false;
    UseOoiNetworkActorV47 target;
    if(!services_.actor(services_.context,selected,target,e))return false;
    if(!require(target.identity==selected&&target.receiver_lease&&target.network_id108,"selected network108",e))return false;
    // Source snapshots IDs before factory allocation. Actor BYTE, target WORD->u16.
    const auto selected_id=static_cast<std::uint16_t>(*target.network_id108);
    const auto source_id=static_cast<std::uint8_t>(*actor.network_id108);
    if(!require(bool(services_.message_create),"CMsgControllerAction factory",e))return false;
    UseOoiMessageV47 message;
    if(!services_.message_create(services_.context,"CMsgControllerAction",true,message,e))return false;
    if(!require(message.identity&&message.receiver_lease&&message.action50&&message.selected52&&message.actor54,"actual message fields50/52/54",e))return false;
    *message.actor54=source_id;*message.action50=5;*message.selected52=selected_id;
    if(!require(bool(services_.message_send),"Client source message send",e)||
       !services_.message_send(services_.context,client,message,e))return false;
   }
  }
 }
 if(!require(*fields_.controllable4&&bool(services_.control50),"controllable4 virtual50",e))return false;
 // Original NULL request survives the network selection; do not pass selected OOI.
 return services_.control50(services_.context,*fields_.controllable4,requested,e);
}
}
