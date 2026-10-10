#pragma once
#include "renderer.hpp"
#include "../engine-resources/resources.hpp"
#include <string>
namespace dh::foundation {
enum class CommonMaterialPassResult { notCommon, applied, invalid };
struct CommonMaterialPass {
    SourceMaterialPass state;
    std::string technique, vertexShader, fragmentShader, vertexDefines, fragmentDefines;
};
// Only a local, selected ProfileCOMMON pass is admitted. Other shader families
// retain their existing backend; this does not select a lighting technique.
CommonMaterialPassResult resolve_common_material_pass(const dh2::resources::BresView&,
    const std::string& materialId, CommonMaterialPass&, std::string& error);
}
