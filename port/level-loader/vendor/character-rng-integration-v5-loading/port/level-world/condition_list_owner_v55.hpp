#pragma once
#include "condition_data_services_v51.hpp"
#include <array>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>
namespace dh2::world {
// Decoded actual source transport, not a live native C++ vtable. Original
// CondStub reader emits this source-vtable provenance word plus op/args.
struct ConditionStubV55 {std::uint32_t source_vtable;std::int32_t op,argument1,argument2;};
static_assert(sizeof(ConditionStubV55)==16);
struct ConditionTableRowV55 {std::string name;std::int32_t list_op;std::vector<ConditionStubV55> stubs;};
class ConditionTablesV55 {
 std::vector<ConditionTableRowV55> storage_;
 std::vector<ConditionDataRowV3> rows_;
 ConditionTablesV55()=default;
public:
 static bool load(const std::uint8_t*,std::size_t,const std::uint8_t*,std::size_t,std::shared_ptr<const ConditionTablesV55>&,std::string&);
 const auto& rows()const noexcept{return rows_;}
 const auto& storage()const noexcept{return storage_;}
 bool contains(const ConditionStubV55*,std::int32_t)const noexcept;
};
struct ActualConditionBorrowV55 {
 std::uintptr_t receiver{};
 std::shared_ptr<const void> receiver_alias; // alias.get()==actual receiver
 std::uintptr_t* live_stub4{}; // main's checked SAME actual Condition field
};
struct ActualConditionFactoriesV55 {
 std::shared_ptr<void> owner;
 // Source table index0..6. Main owns each genuine constructor and evaluator.
 std::array<std::function<bool(ActualConditionBorrowV55&,std::string&)>,7> create;
 std::function<bool(const ActualConditionBorrowV55&,std::string&)> deleting_destructor;
};
// Three source cells of the real ConditionList. Native pointers use native
// width; original ARM allocation is12 bytes. This is NOT an ARM-image memcpy.
struct ConditionListFieldsV55 {std::int32_t count{};std::uintptr_t* instances4{};const ConditionStubV55* stubs8{};};
class ConditionListOwnerV55:public std::enable_shared_from_this<ConditionListOwnerV55> {
 struct Array;struct List;
 std::shared_ptr<const ConditionTablesV55> table_;
 ActualConditionFactoriesV55 factories_;
 std::vector<std::unique_ptr<List>> lists_;
 std::vector<std::unique_ptr<Array>> arrays_;
 List* find(std::uintptr_t)const noexcept;
 bool construct(std::uintptr_t&,std::string&);
 bool assign(std::uintptr_t,const ConditionStubV55*,std::int32_t,std::string&);
 bool destroy(std::uintptr_t,std::string&);
public:
 ConditionListOwnerV55(std::shared_ptr<const ConditionTablesV55>,ActualConditionFactoriesV55);
 ~ConditionListOwnerV55();
 ConditionDataInitServicesV3 services();
 const ConditionListFieldsV55* fields(std::uintptr_t)const noexcept;
 std::size_t retained_list_allocations()const noexcept;
 // Source repeated Init/Assign leaks are retained until owning-world teardown;
 // no eager Clear, evaluator, save/quest state, or second global is introduced.
};
}
