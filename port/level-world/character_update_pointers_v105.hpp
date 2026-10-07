#pragma once
#include "character_ai_pointer_fields_v105.hpp"
#include "character_target_bindings.hpp"
#include <functional>
#include <string>
namespace dh2::character {
using DisabledCharacterBorrowV105=std::function<bool(std::uintptr_t,const std::uint8_t*&,std::string&)>;
// Whole CharAI._UpdatePointers3cb34c followed by Character.UpdateAIPointers
// 3a4344. Nulls pointer values only; keeps ordered-map keys and nodes intact.
bool character_update_pointers_v105(TargetState48&,CharacterAiPointerFieldsV105&,
 std::uintptr_t& ooi14a4,std::uintptr_t& killer144c,
 const DisabledCharacterBorrowV105&,std::string&);
}
