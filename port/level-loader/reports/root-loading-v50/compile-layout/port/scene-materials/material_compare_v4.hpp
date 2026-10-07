#pragma once
#include <array>
#include <cstdint>
#include <string>
#include <vector>
namespace dh2::scene {
// Normalized borrowed runtime directory, not BRES parameter-list order.
struct MaterialParameterCompareV4 {
 std::uint16_t semantic{};std::uint8_t type{};std::uint32_t count{},element_width{};
 const std::uint8_t* bytes{};std::size_t byte_size{};
 // Same retained texture identities; widening never truncates them to ARM32.
 std::vector<std::uintptr_t> textures;
 // Type11 points at actual source Matrix4f68 (16 floats, identity byte/pad).
 std::vector<const std::uint8_t*> matrices;
};
struct MaterialPassCompareV4 {
 std::array<std::uint8_t,32> state{};std::uintptr_t shader_identity{};
 std::uint16_t shader_sort_id{};
 std::vector<std::uint16_t> active_indices;
};
struct MaterialCompareViewV4 {
 std::uint32_t source_hash{};bool hash_ready{};
 std::vector<MaterialPassCompareV4> passes;
 std::vector<MaterialParameterCompareV4> parameters;
 const std::uint8_t* source_identity_matrix68{};
};
// Source3-pass comparison and parameter hash arithmetic over actual reflected
// runtime directories. 0 complete, -2 required/malformed producer. Out and
// hash are unchanged on failure. No shader/material factory is synthesized.
int material_equal_v4(bool&,const MaterialCompareViewV4&,const MaterialCompareViewV4&,std::string&);
int material_less_v4(bool&,const MaterialCompareViewV4&,const MaterialCompareViewV4&,std::string&);
int material_parameter_hash_v4(std::uint32_t&,const MaterialCompareViewV4&,std::string&);
// Constructor shader-ID XOR high byte + render-state kind + current values.
int material_refresh_hash_v4(MaterialCompareViewV4&,std::string&);
}
