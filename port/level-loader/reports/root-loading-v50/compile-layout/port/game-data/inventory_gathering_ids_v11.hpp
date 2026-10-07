#pragma once
#include <cstdint>
#include <list>
#include <string>
namespace dh2::data {
struct InventoryGatheringAssertServicesV11 {
 void* context{};
 // Whole actual missing-ID assertion policy: source mode0 may return; mode1
 // formats/logs the source assertion; native mode2 must reject, not NULL-write.
 bool(*missing_id)(void*,std::int32_t,std::string&){};
};
// Sole native storage for ItemInventory+30/+34 constructor-empty list.
// Register is inlined in Objective_GatherLoot47eaec, not a new source function.
class InventoryGatheringIdsV11 {
public:
 struct Entry {std::int32_t id{};std::uint8_t references{};};
private:
 std::list<Entry> entries_;
public:
 InventoryGatheringIdsV11()=default;
 InventoryGatheringIdsV11(const InventoryGatheringIdsV11&)=delete;
 bool contains(std::int32_t)const noexcept;
 void register_id(std::int32_t);
 bool unregister_id(std::int32_t,const InventoryGatheringAssertServicesV11&,std::string&);
 const std::list<Entry>& entries()const noexcept{return entries_;}
};
}
