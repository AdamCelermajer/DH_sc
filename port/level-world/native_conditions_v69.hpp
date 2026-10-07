#pragma once
#include "condition_data_init_v3.hpp"
#include "data.hpp"
#include <optional>
#include <map>
namespace dh2::world {
//Source v2ConditionStub fields4/8/c. Native factory/vptr transport is typed;
//none of the original ARM vtable addresses are executable host pointers.
struct NativeConditionStubV69 {std::int32_t op4{},argument8{},argument_c{};};
struct NativeConditionRowV69 {
 std::string name;
 std::int32_t count4{},type_c{};
 std::unique_ptr<const NativeConditionStubV69[]> stubs8;
};
struct NativeConditionDecodeBudgetV69 {
 //Finite cache-transport policy, distinct from original native format fields.
 std::size_t input_bytes{16u*1024u*1024u},rows{65536},stubs{262144},name_bytes{4u*1024u*1024u};
};
class NativeConditionTableV69 {
 std::vector<NativeConditionRowV69> rows_;
 std::vector<ConditionDataRowV3> condition_data_rows_;
 bool ready_{};
public:
 //Arrays.v2Conditions.read4b8dec/readNames4b15f8, v2CondAnd.read505970 and
 //v2ConditionStub.read50600c: LE count, per-row count/3-word stubs/type.
 bool decode(data::Bytes records,data::Bytes names,data::Bytes schema,
             const NativeConditionDecodeBudgetV69&,std::string&);
 bool ready()const noexcept{return ready_;}
 const auto& rows()const noexcept{return rows_;}
 const auto& condition_data_rows()const noexcept{return condition_data_rows_;}
 const NativeConditionRowV69* argument_receiver(std::uintptr_t stubs,std::uintptr_t count)const noexcept;
};
struct NativeConditionCacheInputsV69 {
 std::shared_ptr<void> actual_cache;
 std::function<bool(const char*,bool&,std::vector<std::uint8_t>&,std::string&)> read;
 NativeConditionDecodeBudgetV69 budget;
};
bool read_native_condition_table_v69(const NativeConditionCacheInputsV69&,
 std::shared_ptr<const NativeConditionTableV69>&,std::string&);
struct NativeConditionPlayerV69 {
 std::shared_ptr<void> receiver;
 std::uintptr_t player_info{};
 const std::uintptr_t* character660{};
};
struct NativeConditionLevelV69 {
 std::shared_ptr<void> receiver;
 std::uintptr_t identity{};
 const std::int32_t* identifier3c{};
 const std::uintptr_t* events194{};
};
struct NativeConditionStateV69 {std::shared_ptr<void> receiver;const std::int32_t* state0{};};
struct NativeConditionDataBorrowV69 {
 std::shared_ptr<void> receiver;
 const std::uintptr_t* compiled1c{};
 const std::uint8_t* tested20{};
};
struct NativeConditionServicesV69 {
 std::shared_ptr<void> transport; //Callbacks must weakly borrow World/App.
 std::function<bool(std::int32_t,bool,NativeConditionPlayerV69&,std::string&)> local_player;
 std::function<bool(NativeConditionLevelV69&,std::string&)> current_level;
 //SG_GetQuestByID3bc2f8(Character, quest, -1), genuine NULL Quest allowed.
 std::function<bool(std::uintptr_t,std::int32_t,std::int32_t,NativeConditionStateV69&,std::string&)> quest_state;
 //GameEvents.GetEventByID4796f0; its positive caller dereferences state0.
 std::function<bool(std::uintptr_t,std::int32_t,NativeConditionStateV69&,std::string&)> event_state;
 std::function<bool(std::int32_t&,std::string&)> assertion_mode;
 std::function<bool(const char*,std::int32_t,const char*,std::string&)> assertion;
};
//Whole ConditionList/7 native Condition implementations. This is a receiver
//arena for ConditionData fields, not a per-object duplicate table/Save/PM.
class NativeConditionRuntimeV69 : public std::enable_shared_from_this<NativeConditionRuntimeV69> {
 struct List;
 std::shared_ptr<const NativeConditionTableV69> tables_;
 NativeConditionServicesV69 services_;
 std::map<std::uintptr_t,std::shared_ptr<List>> lists_;
 std::vector<std::shared_ptr<const void>> source_orphans_;
 bool assign_native(std::uintptr_t,const NativeConditionStubV69*,std::int32_t,
                    std::shared_ptr<const void>,std::string&);
public:
 NativeConditionRuntimeV69()=default;
 NativeConditionRuntimeV69(const NativeConditionRuntimeV69&)=delete;
 NativeConditionRuntimeV69& operator=(const NativeConditionRuntimeV69&)=delete;
 static bool create(std::shared_ptr<const NativeConditionTableV69>,NativeConditionServicesV69,
                    std::shared_ptr<NativeConditionRuntimeV69>&,std::string&);
 ConditionDataInitServicesV3 condition_data_services();
 bool bind_services(NativeConditionServicesV69,std::string&);
 bool construct(std::uintptr_t&,std::string&);
 bool assign_py_data(std::uintptr_t,std::uintptr_t stubs8,std::uintptr_t count4,std::string&);
 //Quest.AssignPyData uses its real v2Quest inline stub array with the SAME
 //ConditionList kernel. This lease pins that immutable decoded source row.
 bool assign_authored_py_data_v76(std::uintptr_t,std::shared_ptr<const void>,
                     const NativeConditionStubV69*,std::int32_t,std::string&);
 bool destroy(std::uintptr_t,std::string&);
 bool evaluate(std::uintptr_t,bool&,std::string&);
 //Whole ConditionData.IsTrue33e5f8 over SAME +1c/+20 source fields.
 bool condition_data_is_true(CanonicalGameObjectBaseOwnerV1&,std::uint32_t offset,bool&,std::string&);
 bool condition_data_is_true(const NativeConditionDataBorrowV69&,bool&,std::string&);
};
}
