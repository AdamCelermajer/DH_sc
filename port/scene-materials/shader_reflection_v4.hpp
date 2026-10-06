#pragma once
#include <cstdint>
#include <string>
#include <vector>
namespace dh2::scene {
struct ShaderUniformReflectionV4 {
 std::string name;std::uint32_t gl_type{},count{};std::int32_t location{};
 std::uint16_t semantic{};std::uint8_t type{},sub_id{};
};
// Whole source name normalization, static semantic directory, sub-ID parsing
// and LinkProgram's raw GL type projection. Input is actual GL reflection.
void shader_uniform_reflect_v4(ShaderUniformReflectionV4&);
// Source5e7b50 stable global/local partition, returning global prefix count.
std::uint32_t shader_uniform_partition_v4(std::vector<ShaderUniformReflectionV4>&);
}
