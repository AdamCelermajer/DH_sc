#pragma once
#include <canonical_level_context_v1.hpp>
#include "game_event_manager_v50.hpp"
namespace dh2::loader {
// Same Level194 -> actual GetEventByID4796f0 -> same event.state0.
// No App14 alias, cast of raw194, new table/manager or synthetic state.
inline bool borrow_game_event_state_v74(std::weak_ptr<CanonicalLevelContextV1> weak,
 std::uintptr_t actual194,std::int32_t id,std::shared_ptr<void>& receiver,
 const std::int32_t*& state0,std::string& e){
 auto level=weak.lock();GameEventLevelFieldsV50 fields;
 if(!level||!level->game_event_fields_v50(fields,e))return false;
 if(!actual194||!fields.field194||*fields.field194!=actual194||!fields.owner194||!*fields.owner194||
    reinterpret_cast<std::uintptr_t>(fields.owner194->get())!=actual194){
  e="Required SAME retained native Level194 GameEventManager receiver";return false;
 }
 auto manager=*fields.owner194;auto* event=manager->by_id(id);
 if(!event){receiver.reset();state0=nullptr;e.clear();return true;}
 receiver=std::shared_ptr<void>(manager,event);state0=&event->fields().state0;e.clear();return true;
 // Native NULL event for negative/out-of-range/empty slot is preserved. Main
 // condition caller owns its original assertion/dereference branch policy.
}
}
