#pragma once
#include <cstdint>
#include <string>
namespace dh2::world {
// Borrows actual ConditionData+1c/+20 and ObjectBase fields, never copies them.
struct ObjectEnableConditionBorrowV2 {
 std::uint8_t* enabled8a{};
 const std::int32_t* minimum_difficulty_ec{};
 const std::uint8_t* disable_f1{};
 const std::uintptr_t* condition_a8{};
 std::uint8_t* tested_ac{};
};
struct ObjectEnableConditionServicesV2 {
 void* context{};
 // Actual GetLocalPlayer(0,true)->Character+14e8 SAVE presence and that
 // PlayerSavegame's quest-sync byte14. This is NOT Save.profile(+8).
 // Field name local_profile is retained only for existing adapter ABI.
 bool(*local_profile)(void*,bool& character,bool& save,std::uint8_t& byte14,std::string&){};
 bool(*current_level)(void*,bool& present,std::int32_t& difficulty118,std::string&){};
 bool(*condition_is_true)(void*,std::uintptr_t,bool&,std::string&){};
 // Actual selected virtual+44/+48; called after enabled8a mutation.
 bool(*enabled_event)(void*,bool,std::string&){};
};
bool object_set_enable_v2(ObjectEnableConditionBorrowV2,ObjectEnableConditionServicesV2,bool,std::string&);
bool object_test_enable_condition_v2(ObjectEnableConditionBorrowV2,ObjectEnableConditionServicesV2,bool mark_tested,bool& enabled,std::string&);
//Whole TestDisableCondition33e61c. condition_a8/tested_ac lend SAME cc/d0
//for this call; minimum/disable-f1 are not read by the original disable body.
bool object_test_disable_condition_v103(ObjectEnableConditionBorrowV2,ObjectEnableConditionServicesV2,bool mark_tested,bool& enabled,std::string&);
}
