#pragma once
#include "character_dead_select.hpp"
#include "../game-data/animation_tables.hpp"
#include "../game-data/properties.hpp"
#include <functional>
#include <string>
namespace dh2::character {
struct CharacterDeadSelectionBorrowV56 {
 CharacterStateOwner* machine{};data::PropertyView* properties{};
 const data::AnimationTables* animations{};
 // Borrow existing source FSM+3f (actual SkillApply.push_death target) and
 // separately retained +38. Do not confuse +3e State.dead_alternate with +3f.
 std::uint8_t* pending3f{};std::int32_t* secondary38{};
 StateOwnerServices16 state_services{};
};
struct CharacterDeadSelectionServicesV56 {
 std::function<bool(std::int32_t&,std::string&)> stance_mask,stance;
};
class CharacterDeadSelectionOwnerV56 final {
 CharacterDeadSelectionBorrowV56 b_;CharacterDeadSelectionServicesV56 s_;
 std::vector<DeadAnimationRow16> rows_;std::string error_;
 struct Call;
 static int query(void*,DeadSelect32*,const DeadSelectRequest24*,std::uint32_t*);
 static int state(void*,StateOwnerMachine40*,const StateOwnerRequest48*,StateOwnerResponse8*);
public:
 CharacterDeadSelectionOwnerV56(CharacterDeadSelectionBorrowV56,CharacterDeadSelectionServicesV56);
 int set(bool,std::uintptr_t,bool);
 const std::string& error()const noexcept{return error_;}
};
}
