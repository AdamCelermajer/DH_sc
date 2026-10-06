#pragma once
#include <cstdint>
#include <string>
namespace dh2::character {
struct ObjectiveGatheringBorrowV11 {
 std::uintptr_t identity{};
 const std::uint8_t* enabled8{};
 const std::uintptr_t* character10{};
 const std::int32_t* item_id24{};
};
struct ObjectiveGatheringServicesV11 {
 void* context{};
 // Original Objective base Register47ae70/Unregister47adbc BEFORE gathering
 // logic. It may change the borrowed fields, which are read afterwards.
 bool(*base_register)(void*,std::uintptr_t,std::string&){};
 bool(*base_unregister)(void*,std::uintptr_t,std::string&){};
 bool(*inventory_register)(void*,std::uintptr_t,std::int32_t,std::string&){};
 bool(*inventory_unregister)(void*,std::uintptr_t,std::int32_t,std::string&){};
};
bool objective_gathering_register_v11(const ObjectiveGatheringBorrowV11&,
 const ObjectiveGatheringServicesV11&,std::string&);
bool objective_gathering_unregister_v11(const ObjectiveGatheringBorrowV11&,
 const ObjectiveGatheringServicesV11&,std::string&);
}
