#pragma once
#include "canonical_gameobject_base_owner_v1.hpp"
#include "condition_data_services_v51.hpp"
#include <functional>
namespace dh2::world {
// Native immutable Arrays::Conditions row. argument8 identifies actual decoded
// v2ConditionStub storage; argument4 preserves the source signed32 count bits.
// ConditionList::AssignPyData478914 consumes them; neither is a fake receiver.
// Whole ConditionData::Init33eb28/Clear33e7c8 control. Input CString and
// compiled/tested fields borrow SAME CanonicalGameObjectBaseOwnerV1 storage.
bool condition_data_init_v3(CanonicalGameObjectBaseOwnerV1&,std::uint32_t condition_offset,
 const ConditionDataInitServicesV3&,std::string&);
bool condition_data_clear_v3(CanonicalGameObjectBaseOwnerV1&,std::uint32_t condition_offset,
 const ConditionDataInitServicesV3&,std::string&);
}
