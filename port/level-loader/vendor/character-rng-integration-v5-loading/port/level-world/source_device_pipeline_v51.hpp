#pragma once
#include <array>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
namespace dh2::world {
struct SourceDevicePipelineBorrowV51 {
 std::shared_ptr<const void> actual_owner;
 std::array<const std::uint8_t*,3> disable_bytes;
 std::function<bool(std::uint32_t&,std::string&)> actual_renderer_mask;
};
// Exact38174c ->3816a8 branch only, no device state/backend is owned here.
inline bool source_device_high_performance_v51(const SourceDevicePipelineBorrowV51& b,bool& out,std::string& error){
 if(!b.actual_owner){error="Required actual Device/programmatic-pipeline field owner";return false;}
 for(const auto* p:b.disable_bytes){if(!p){error="Required actual live Device disable byte";return false;}if(*p){out=false;error.clear();return true;}}
 if(!b.actual_renderer_mask){error="Required actual application renderer programmable-pipeline mask";return false;}
 std::uint32_t mask;if(!b.actual_renderer_mask(mask,error))return false;out=(mask&0x78u)!=0;error.clear();return true;
}
}
