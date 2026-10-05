#pragma once
#include "procedural_layout_v1.hpp"
#include <map>
#include <optional>
namespace dh2::loader {
struct ProceduralModuleSetterV1 {
    std::string name;
    std::optional<std::string> value; // Null original setter input, not an empty override.
};
struct ProceduralModuleV1 {
    std::uint32_t tile{},mvx_document{UINT32_MAX};
    std::string mvx_uri;
    bool mvx_found{};
    std::array<float,3> position{};
    std::vector<ProceduralModuleSetterV1> setter_attempts;
    // Generated non-null setter inputs only. This is not full PropertyMap
    // serialization or a replacement for main-owned registered defaults.
    std::map<std::string,std::string> overrides;
};
struct ProceduralModulePlanV1 {
    ProceduralLayoutResultV1 layout; // Retains full rule/MGX/list graph and visit identity.
    std::vector<XmlDocumentV1::Borrow> mvx_documents;
    std::vector<ProceduralModuleV1> modules;
    std::uint32_t returned_index{};
};
bool project_procedural_module_v1(const ProceduralLayoutTileV1&,std::uint32_t index,
    const ProceduralBlockV1&,const std::string& folder,const std::string& target,
    XmlDocumentV1::Borrow mvx,ProceduralModuleV1& out,std::string& error);
// Original direct MVX lookup and first matching Module/GameObject selection.
// Missing MVX is retained explicitly, matching the original optional Open branch.
// Missing MGP/MVP dependencies are checked by the subsequent source stage.
// Failed candidates leave out unchanged, including its retained source graph.
bool prepare_procedural_modules_v1(const assets::ZipAssetPackV1&,
    const ProceduralLayoutResultV1&,ProceduralModulePlanV1& out,std::string& error);
}
