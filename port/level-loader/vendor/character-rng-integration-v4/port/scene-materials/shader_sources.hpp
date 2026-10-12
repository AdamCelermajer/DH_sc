#pragma once
#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace dh2::scene {
// Owned copies of the eight source strings passed by 6dfe68 to 6df738.
// A GPU program, resource manager and material binding are separate owners.
struct ShaderSourcePlan {
    std::array<std::string,8> chunks;
    std::string cache_name;
    std::uint32_t gl_type=0;
};
bool shader_code_name(std::string_view first,std::string_view second,
                     std::string_view third,const std::string* additional,
                     std::string& output,std::string& error);
// Original initAdditionalConfig replaces every '^' byte with LF, preserving
// all other bytes. Loading/missing-file/retry and manager publication are services.
bool shader_config_text(std::string_view input,std::string& output,std::string& error);
bool shader_source_plan(std::uint32_t driver_flags,std::uint32_t source_type,
                        std::string_view filename,std::string_view caller,
                        const std::string* additional,std::string_view body,
                        ShaderSourcePlan& output,std::string& error);

struct ShaderPackMember { std::string name;std::vector<std::uint8_t> bytes; };
// Bounded port container reader: actual cache pak is a stored ZIP. Deflate,
// encryption and ZIP64 are rejected. This is not original archive-parser parity.
class ShaderSourcePack {
public:
    bool load(const std::uint8_t* bytes,std::size_t size,std::string& error);
    const std::vector<ShaderPackMember>& members() const noexcept { return members_; }
    const ShaderPackMember* find(std::string_view exact_name) const noexcept;
private:
    std::vector<ShaderPackMember> members_;
};
}
