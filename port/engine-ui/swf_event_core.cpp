#include "swf_event_core.hpp"
#include "gameswf/gameswf_character.h"
#include "gameswf/gameswf_sprite.h"
#include "gameswf/gameswf_action.h"
#include <exception>
namespace dh2::ui {
bool swf_event_method(const SwfViewportLease& lease,gameswf::character* character,
 const char* name,bool& source_invoked,std::string& error){
 if(!lease.owner||!lease.root||!name){error="Required retained SWF event graph/method unavailable";return false;}
 const auto owner=lease.owner;
 source_invoked=false;
 if(!character){error.clear();return true;}
 auto* environment_receiver=character;
 if(!character->is(gameswf::sprite_instance::m_class_id)){
  if(!character->get_parent()){error.clear();return true;}
  if(!character->get_parent()->is(gameswf::sprite_instance::m_class_id)){error.clear();return true;}
  environment_receiver=character->get_parent();
  if(!environment_receiver){error.clear();return true;}
 }
 try{
  // Original keeps input character alive, uses sprite parent's environment
  // when needed, and still passes INPUT character as method this receiver.
  gameswf::gc_ptr<gameswf::character> pin=character;
  auto* environment=environment_receiver->get_environment();
  if(!environment){error="Required SWF method environment unavailable";return false;}
  const auto discarded=gameswf::call_method(environment,character,name,nullptr,0);
  (void)discarded;source_invoked=true;error.clear();return true;
 }catch(const std::exception& e){error=e.what();return false;}
}
}
