#pragma once
#include "actor_runtime.hpp"
#include <functional>
#include <memory>
namespace dh2::character {
// Sole inherited pointer-field storage for Character. Embed once in its actual
// retained receiver; C1 calls construct exactly once before property defaults.
struct CharacterPositionFieldsV7 {
 std::uintptr_t physical2dc{},attached2e0{};
 bool constructed{};
 bool construct(actor::RuntimeState&,std::string&);
 // Adoption borrows actual source pointer values only. Runtime has already
 // passed initialization: bounds/destination/PF/path must not be reset.
 bool adopt(std::uintptr_t actual_physical2dc,std::uintptr_t actual_attached2e0,std::string&);
};
struct CharacterPositionBorrowV7 {
 std::shared_ptr<void> receiver;
 std::uintptr_t identity{};
 float* position160{};float* relative144{};float* absolute12c{};
 float* destination1a8{};
 const std::uintptr_t* attached2e0{};const std::uintptr_t* physical2dc{};const std::uintptr_t* visual2d8{};
 // Internal semantic-view publication only, never a Lua/PF/backend callback.
 // Source Position160 remains the sole authority; runtime observations mirror.
 std::function<void()> publish_position;
};
struct CharacterPositionServicesV7 {
 std::shared_ptr<void> world;
 std::function<bool(std::uintptr_t,float*&,std::string&)> attached_position;
 std::function<bool(std::uintptr_t,float,float,std::string&)> physical_position;
 std::function<bool(std::uintptr_t,std::string&)> visual_sync_position;
};
enum class CharacterPositionPhaseV7 {not_started,attached,current,bounds,physical,visual,destination,complete};
struct CharacterPositionResultV7 {CharacterPositionPhaseV7 phase{CharacterPositionPhaseV7::not_started};};
// Whole393db4/38aac8/393600 over genuine field borrows. Preserves argument
// aliases and reentrant source pointer rereads. No path/floor/heading/velocity
// reset: none occurs in those original bodies. Failure retains source prefix.
bool character_set_position_v7(CharacterPositionResultV7&,const CharacterPositionBorrowV7&,
 const float* source_position,bool destination,const CharacterPositionServicesV7&,std::string&);
}
