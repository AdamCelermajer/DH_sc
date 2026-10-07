#pragma once
#include "../scene-materials/scene.hpp"
#include <map>
namespace dh2::world {
//Immutable decoded instance material values from the SAME selected BRES row.
//Shader uniforms select these rows; no fabricated color/shininess defaults.
struct NativeBatchMaterialValueV113 {
 std::uint32_t source_type{};
 std::uint32_t semantic{};
 std::vector<std::uint32_t> element_counts;
 std::vector<std::uint8_t> bytes;
 std::vector<std::string> images;
 std::vector<std::string> light_urls; //actual source17/19 accepted by runtime CLight parameter18
};
struct NativeBatchMaterialValuesV113:std::map<std::string,NativeBatchMaterialValueV113> {
 //Actual selected SPass sampler name→effect parameter name; not a shader
 //name heuristic. Effect defaults remain separate from instance overrides.
 std::map<std::string,std::string> sampler_bindings;
 std::map<std::string,NativeBatchMaterialValueV113> effect_defaults;
};
bool decode_batch_material_values_v113(const resources::BresView&,const std::string&,
 NativeBatchMaterialValuesV113&,std::string&);
bool decode_batch_effect_values_v113(const resources::BresView&,const std::string& uri,
 const std::string& technique,NativeBatchMaterialValuesV113&,std::string&);
}
