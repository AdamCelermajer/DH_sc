#pragma once
#include "procedural_modules_v1.hpp"
#include <canonical_property_map_v1.hpp>
#include <canonical_level_config_module_v1.hpp>
#include <memory>
#include <functional>
namespace dh2::loader {
// Original vector<Point3D> ObjToStr is BX LR. Its preceding CustomAlloc256
// contents are therefore the output source, not an invented empty/color list.
// Owner is the actual allocator/runtime lifetime pin. Original helpers never
// free these allocations; do not zero/free/reuse them at callback return.
struct PropertyScratchV66 {std::shared_ptr<void> owner;std::uint8_t* bytes{};std::size_t size{};};
struct ProceduralXmlServicesV66 {
 std::shared_ptr<void> actual_runtime_owner;
 world::CanonicalPropertyMapV1* properties{};
 world::ModuleRuntimeGlobalsV1* module_globals{};
 std::function<bool(PropertyScratchV66&,std::string&)> allocate_property_scratch256;
};
// Module/LevelConfig field reads and actual registered descriptor traversal are
// loader-owned. Engine supplies genuine globals/property registration and heap.
bool load_original_rule_config_v66(const XmlDocumentV1::Borrow&,std::uint32_t,
 world::CanonicalLevelConfigV1&,ProceduralXmlServicesV66&,std::string&);
bool save_modular_level_to_bytes_v66(const ProceduralModulePlanV1&,
 world::CanonicalLevelConfigV1&,const std::string& actual_rule_filename,
 ProceduralXmlServicesV66&,std::vector<std::uint8_t>&,std::string&);
}