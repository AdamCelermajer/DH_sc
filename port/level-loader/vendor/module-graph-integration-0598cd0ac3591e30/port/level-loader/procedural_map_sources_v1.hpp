#pragma once
#include "procedural_modules_v1.hpp"
#include "fixed_sources_v1.hpp"
namespace dh2::loader {
struct ProceduralMapSourcesV1 {
    std::shared_ptr<const ProceduralModulePlanV1> modules;
    XmlDocumentV1::Borrow inspection_root;
    FixedSourcesV1::Borrow sources;
};
// Explicit inspection adapter: generated module overrides become an owned Level
// document for the existing geometry/declaration stage. Original rule/MGX/MVX
// documents and every original selected MGP/MVP declaration remain retained.
// This derived XML is not original serialized output or the shared runtime ABI.
// It does not supply property defaults or activate/instantiate gameplay objects.
bool prepare_procedural_map_sources_v1(const assets::ZipAssetPackV1&,
    ProceduralModulePlanV1,ProceduralMapSourcesV1& out,std::string& error);
}
