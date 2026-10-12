#pragma once
#include "object_enable_condition_v2.hpp"
#include <memory>
namespace dh2::character {
// Pins the SAME retained actor. Cells are native pointers into that owner;
// neither this view nor its V2 conversion constructs/copies a base or save.
struct CharacterEnableConditionFieldsV58 {
 std::shared_ptr<const void> receiver_lease;
 std::uintptr_t identity{};
 std::uint8_t* enabled8a{};
 const std::int32_t* minimum_difficulty_ec{};
 const std::uint8_t* disable_f1{};
 const std::uintptr_t* condition_a8{};
 std::uint8_t* tested_ac{};
 world::ObjectEnableConditionBorrowV2 borrow()const noexcept{
  return {enabled8a,minimum_difficulty_ec,disable_f1,condition_a8,tested_ac};
 }
};
}