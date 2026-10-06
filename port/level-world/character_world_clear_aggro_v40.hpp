#pragma once
#include "character_clear_aggro.hpp"
#include <memory>
#include <string>
namespace dh2::character {
struct WorldClearAggroActorV40 {
 std::uintptr_t identity{};
 data::AggroTable* outgoing{};data::AggroTable* incoming{};
 TargetBindings48* target{};std::uintptr_t controller{};
 std::shared_ptr<void> lifetime; // actual World/actor provider storage lease
};
struct WorldClearAggroServicesV40 {
 void* context{};
 bool(*actor)(void*,std::uintptr_t,WorldClearAggroActorV40&,std::string&){};
 bool(*on_deaggro)(void*,std::uintptr_t receiver_character,std::uintptr_t other_character,
                  const dh2_script_callback_scope*,std::string&){};
 bool(*command_stop)(void*,std::uintptr_t character,std::uintptr_t controller,
                    const dh2_script_callback_scope*,std::string&){};
};
// Composes the original whole AI_ClearAggro3d6d68 body on the SAME two
// maps/TargetBindings/controller. No independent observer or copied target.
class CharacterWorldClearAggroV40 {
 WorldClearAggroServicesV40 services_;std::string error_;
public:
 explicit CharacterWorldClearAggroV40(WorldClearAggroServicesV40 services):services_(services){}
 // NULL other follows original early return, before actor/receiver borrows.
 int clear(std::uintptr_t owner,std::uintptr_t other,const dh2_script_callback_scope* =nullptr);
 const std::string& error()const noexcept{return error_;}
};
}
