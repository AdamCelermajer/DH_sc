#pragma once
#include "character_world_physical_peers_v1.hpp"
#include "character_native_fsm.hpp"
#include "character_design_services.hpp"
namespace dh2::character {
struct WorldPhysicalCharacterBorrowV1 {
 skills::CharacterWorldRuntimeV1& world;std::uintptr_t identity;
 NativeFsm24& machine;DebugSwitches& debug;const DebugFileServices24& files;
 void* context{};
 // SAME Character::RaiseEvent production endpoint, preserving AI then FSM.
 // No separate AI fields or empty successful callback is accepted here.
 int(*raise_event)(void*,std::uintptr_t,std::uint32_t,std::uintptr_t,std::string&){};
};
class CharacterWorldPhysicalCharacterV1 {
 WorldPhysicalCharacterBorrowV1 borrow_;
 bool character(std::uintptr_t&,std::string&);
 bool debug(std::string&);
 bool coherent(std::string&);
public:
 explicit CharacterWorldPhysicalCharacterV1(WorldPhysicalCharacterBorrowV1 b):borrow_(b){}
 bool filter(std::uint16_t,bool&,std::string&);
 bool event(physical::ContactEvent,std::uintptr_t,unsigned,std::string&);
 WorldPhysicalCharacterServicesV1 services()noexcept;
};
}
