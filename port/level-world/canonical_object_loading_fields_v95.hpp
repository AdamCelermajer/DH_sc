#pragma once
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
namespace dh2::world {
class CanonicalGameObjectBaseOwnerV1;
// Borrowed SAME ObjectBase CString48 (buffer5c) and ConditionData cells.
// The lease pins the receiver throughout each source method; no copy of cells.
struct CanonicalObjectLoadingFieldsV95 {
 std::shared_ptr<void> receiver;
 CanonicalGameObjectBaseOwnerV1* gameobject_base{};
 const std::string* archetype48{};
 std::uint8_t* enabled8a{};
 const std::int32_t* minimum_ec{};
 const std::uint8_t* disabled_f1{};
 std::uintptr_t *condition_a8{},*condition_cc{};
 std::uint8_t *tested_ac{},*tested_d0{};
 // Only ObjectBase-derived non-GameObjects use these actual virtual3c/40 stores.
 std::function<bool(bool,std::string&)> objectbase_enabled_event;
};
}
