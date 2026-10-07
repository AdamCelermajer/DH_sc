#include "swf_grid_snap_v1.hpp"
#include <cstring>
#include <cmath>
extern "C" std::uint32_t dh2_swf_grid_snap_v1(std::uint32_t bits) noexcept {
 float value{};std::memcpy(&value,&bits,4);std::int32_t integer{};
 if(std::isnan(value))integer=0;
 else if(value>=2147483648.f)integer=INT32_MAX;
 else if(value<=-2147483648.f)integer=INT32_MIN;
 else integer=static_cast<std::int32_t>(value);
 const std::uint32_t sum=std::uint32_t(integer)+10u;
 std::int32_t signed_sum{};std::memcpy(&signed_sum,&sum,4);
 const std::uint32_t snapped=std::uint32_t(signed_sum/20)*20u;
 std::int32_t signed_snapped{};std::memcpy(&signed_snapped,&snapped,4);
 value=float(signed_snapped);std::memcpy(&bits,&value,4);return bits;
}
