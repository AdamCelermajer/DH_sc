#pragma once
#include "loot_tables_v2.hpp"
namespace dh2::data {
enum class LootEntryOperationV8:std::uint32_t {
 debug_load=0x337888,debug_query=0x337a88,
 mage_count=0x36eac8,rogue_count=0x36eac0,warrior_count=0x36eab8,
 assertion=0x30e004
};
struct LootEntryRequestV8 {LootEntryOperationV8 operation;std::uint32_t caller;const char* key{};};
struct LootEntryServicesV8 {
 void* context{};
 bool(*invoke)(void*,const LootEntryRequestV8&,std::int32_t&,std::string&){};
};
// Complete original entry classification/effective-weight/percentage-roll and
// both weighted-index overload bodies. Borrow the actual shared source RNG;
// Debug and PlayerManager counters remain fresh synchronous required queries.
class LootEntrySelectionV8 {
 LootRandom8V2& random_;LootEntryServicesV8 services_;
 bool request(LootEntryOperationV8,std::uint32_t,const char*,std::int32_t&,std::string&);
 bool debug(std::uint32_t,const char*,std::int32_t&,std::string&);
public:
 LootEntrySelectionV8(LootRandom8V2& r,LootEntryServicesV8 s):random_(r),services_(s){}
 bool uses_percent(const LootEntry32V2&,bool&,std::string&);
 bool effective_weight(const LootEntry32V2&,std::int32_t&,std::string&);
 bool percent_roll(const LootEntry32V2&,bool&,std::string&);
 bool weighted_index(const std::vector<const LootEntry32V2*>&,std::uint32_t&,bool vector_overload,std::string&);
 bool trace(std::uint32_t caller,std::string&);
};
}
