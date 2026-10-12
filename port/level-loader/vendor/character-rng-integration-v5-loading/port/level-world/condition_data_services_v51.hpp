#pragma once
#include <functional>
#include <memory>
#include <string>
#include <vector>
#include <cstdint>
namespace dh2::world {
struct ConditionDataRowV3 {std::string name;std::uintptr_t argument4{},argument8{};};
struct ConditionDataInitServicesV3 {
 std::shared_ptr<void> owner;
 const std::vector<ConditionDataRowV3>* conditions{};
 // Provider owns actual allocation/ctor/lifetime, and preserves source leaks
 // if Init is called again: this kernel does not clear an old pointer first.
 std::function<bool(std::uintptr_t&,std::string&)> construct_condition;
 std::function<bool(std::uintptr_t,std::uintptr_t,std::uintptr_t,std::string&)> initialize_condition;
 // Whole ConditionList D1 then CustomFree, before compiled pointer is cleared.
 std::function<bool(std::uintptr_t,std::string&)> destroy_condition;
};
}
