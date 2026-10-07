#pragma once
#include "data.hpp"
namespace dh2::data {
// Source ARM32 scalar projection, not a native object/vtable layout. Pointer
// slots are zero. String lengths and every other raw word remain exact.
struct FastTravelProjection28 {std::uint32_t words[7];};
struct LevelProjection72 {std::uint32_t words[18];};
struct LevelTextSpan16 {const std::uint8_t* data;std::uint32_t size,reserved;};
static_assert(sizeof(FastTravelProjection28)==28&&sizeof(LevelProjection72)==72&&sizeof(LevelTextSpan16)==16);
struct FastTravelRecord {FastTravelProjection28 scalar{};std::string level_name;};
struct LevelRecord {LevelProjection72 scalar{};std::string description,file;
 const std::string& dynamic_bus_routing_v94()const noexcept{return description;}
};
struct LevelTables {
 std::vector<std::string> travel_names,level_names,travel_fields,level_fields;
 std::vector<FastTravelRecord> travel;
 std::vector<LevelRecord> levels;
 std::size_t travel_data_consumed=0,data_consumed=0;
};
// Both original ordered sections: 33 travel destinations then 51 level rows
// in this cache. Dimensions are caller data, not hardcoded application state.
bool load_levels(Bytes records,Bytes names,Bytes schema,LevelTables&,std::string&);
}
// Bounded readers:0 success,1 malformed/truncated. Outputs change only on
// success. Text spans borrow caller bytes; source bool bytes are raw uint8,
// including noncanonical values. No native bool or pointer-width conversion.
extern "C" unsigned dh2_fast_travel_decode_record(dh2::data::FastTravelProjection28*,
 dh2::data::LevelTextSpan16*,std::uint32_t*,const std::uint8_t*,std::uint32_t);
extern "C" unsigned dh2_level_decode_record(dh2::data::LevelProjection72*,
 dh2::data::LevelTextSpan16*,std::uint32_t*,const std::uint8_t*,std::uint32_t);
