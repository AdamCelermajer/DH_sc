#include "design_settings_xp_provider_v1.hpp"

#include <cmath>
#include <cstring>

namespace dh2::design_settings_xp_provider_v1 {
namespace {
std::uint32_t read_u32(const std::uint8_t* bytes) noexcept {
    return std::uint32_t(bytes[0]) | (std::uint32_t(bytes[1]) << 8) |
           (std::uint32_t(bytes[2]) << 16) | (std::uint32_t(bytes[3]) << 24);
}

bool supported(std::uint32_t offset) noexcept {
    switch (offset) {
    case 140: case 144: case 148: case 152: case 156: case 160: case 164:
        return true;
    default:
        return false;
    }
}
} // namespace

std::int32_t read(void* context, std::uint32_t source_member_offset,
                  float* value, std::string& error) {
    if (!context || !value) {
        error = "DesignSettings XP provider received a null owner or output";
        return static_cast<std::int32_t>(Status::invalid_argument);
    }
    if (!supported(source_member_offset)) {
        error = "DesignSettings XP provider received an unsupported member offset";
        return static_cast<std::int32_t>(Status::unsupported_offset);
    }
    const auto& view = *static_cast<const View*>(context);
    if (!view.bytes || view.size < 4 + 172) {
        error = "DesignSettings XP table is truncated";
        return static_cast<std::int32_t>(Status::invalid_table);
    }
    const auto count = read_u32(view.bytes);
    if (!count || count > 4096) {
        error = "DesignSettings XP row count is invalid";
        return static_cast<std::int32_t>(Status::invalid_table);
    }
    const auto cache_offset = source_member_offset - 4; // serialized row omits vptr
    const auto absolute = std::size_t{4} + cache_offset;
    if (absolute > view.size || view.size - absolute < sizeof(float)) {
        error = "DesignSettings XP field exceeds the serialized row";
        return static_cast<std::int32_t>(Status::invalid_table);
    }
    std::uint32_t bits = read_u32(view.bytes + absolute);
    float result = 0.0f;
    std::memcpy(&result, &bits, sizeof(result));
    if (!std::isfinite(result)) {
        error = "DesignSettings XP field is nonfinite";
        return static_cast<std::int32_t>(Status::invalid_table);
    }
    *value = result;
    error.clear();
    return 0;
}

std::int32_t read_combat_music_trigger(void* context,std::int32_t* value,
                                       std::string& error) {
    if(!context||!value){
        error="DesignSettings combat-music provider received a null owner or output";
        return static_cast<std::int32_t>(Status::invalid_argument);
    }
    const auto& view=*static_cast<const View*>(context);
    if(!view.bytes||view.size<4+172){
        error="DesignSettings combat-music table is truncated";
        return static_cast<std::int32_t>(Status::invalid_table);
    }
    const auto count=read_u32(view.bytes);
    if(!count||count>4096||view.size<4+std::size_t(count)*172){
        error="DesignSettings combat-music row count is invalid";
        return static_cast<std::int32_t>(Status::invalid_table);
    }
    // IDA: Structs::DesignSettings::read reads the fifth word as int at
    // runtime member +20; the serialized row omits its four-byte vptr.
    std::int32_t result=0;
    const auto bits=read_u32(view.bytes+4+(20-4));
    std::memcpy(&result,&bits,sizeof(result));
    *value=result;
    error.clear();
    return 0;
}

} // namespace dh2::design_settings_xp_provider_v1
