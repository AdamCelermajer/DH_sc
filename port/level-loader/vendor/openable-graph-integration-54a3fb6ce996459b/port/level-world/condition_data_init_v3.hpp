#pragma once
#include "canonical_gameobject_base_owner_v1.hpp"
#include <functional>
namespace dh2::world {
// Native immutable Arrays::Conditions row. argument8/argument4 are actual
// decoded native receivers passed to CCondition::Init, never ARM addresses.
struct ConditionDataRowV3 {std::string name;std::uintptr_t argument4{},argument8{};};
struct ConditionDataInitServicesV3 {
 std::shared_ptr<void> owner;
 const std::vector<ConditionDataRowV3>* conditions{};
 // Provider owns actual allocation/ctor/lifetime, and preserves source leaks
 // if Init is called again: this kernel does not clear an old pointer first.
 std::function<bool(std::uintptr_t&,std::string&)> construct_condition;
 std::function<bool(std::uintptr_t,std::uintptr_t,std::uintptr_t,std::string&)> initialize_condition;
 // Whole CCondition destructor then free, before compiled pointer is cleared.
 std::function<bool(std::uintptr_t,std::string&)> destroy_condition;
};
// Whole ConditionData::Init33eb28/Clear33e7c8 control. Input CString and
// compiled/tested fields borrow SAME CanonicalGameObjectBaseOwnerV1 storage.
bool condition_data_init_v3(CanonicalGameObjectBaseOwnerV1&,std::uint32_t condition_offset,
 const ConditionDataInitServicesV3&,std::string&);
bool condition_data_clear_v3(CanonicalGameObjectBaseOwnerV1&,std::uint32_t condition_offset,
 const ConditionDataInitServicesV3&,std::string&);
}
