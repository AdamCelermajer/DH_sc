#pragma once
#include "canonical_openable_graph_v21.hpp"
#include "generic_lua_script_owner_v13.hpp"
#include <cstring>
namespace dh2::world {
// Shared factory continuation. These are functions borrowed from the actual
// candidate's providers, not the standalone Decor receiver or its service bag.
// Preserve an already selected leaf and never create another C1/World owner.
inline bool connect_openable_source_services_v126(CanonicalOpenableGraphServicesV21& out,
 const RetainedGameObjectDecorServicesV1& physical,const OpenableContainerServicesV1& source,
 std::string& e){
#define OPENABLE_PHYSICAL(field) if(!out.decor.field)out.decor.field=physical.field
 OPENABLE_PHYSICAL(debug_switch);OPENABLE_PHYSICAL(update_pf);
 OPENABLE_PHYSICAL(peer_owner);OPENABLE_PHYSICAL(peer_visible80);
#undef OPENABLE_PHYSICAL
#define OPENABLE_SOURCE(field) if(!out.container.field)out.container.field=source.field
 OPENABLE_SOURCE(resolve_item_name);OPENABLE_SOURCE(is_character);
 OPENABLE_SOURCE(play_sound_3d);OPENABLE_SOURCE(script_call);
#undef OPENABLE_SOURCE
 if(!out.decor.debug_switch||!out.decor.update_pf||!out.decor.peer_owner||!out.decor.peer_visible80||
    !out.container.resolve_item_name||!out.container.is_character||!out.container.play_sound_3d||!out.container.script_call){
  e="Required complete SAME Openable factory physical/item/Character/audio/Lua callbacks";return false;
 }
 e.clear();return true;
}
inline bool call_container_script_source_v126(scripts::GenericLuaScriptOwnerV13& script,
 const char* name,std::uintptr_t actor,const char* event,std::string& e){
 dh2_script_value argument{};std::uint32_t count{};
 if(name&&!std::strcmp(name,"OnOpen")){
  if(actor){argument.type=DH2_SCRIPT_SOURCE_OBJECT;argument.identity=actor;count=1;}
 }else if(name&&!std::strcmp(name,"OnAnimEvent")&&event){
  argument.type=DH2_SCRIPT_STRING;argument.text=event;argument.text_bytes=std::strlen(event);count=1;
 }else {e="Required original Container OnOpen/OnAnimEvent Call arguments";return false;}
 //Source discards ordinary protected-call success; required-service failures
 //remain failed deliveries. The actual UserData provider is installed by C1.
 bool source_success{};if(!script.call(name,count?&argument:nullptr,count,source_success,e))return false;
 e.clear();return true;
}
}
