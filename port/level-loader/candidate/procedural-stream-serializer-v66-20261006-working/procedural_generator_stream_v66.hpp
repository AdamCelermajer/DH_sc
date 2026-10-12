#pragma once
#include "procedural_xml_emission_v66.hpp"
#include "assigned_loadfile_kernel_v65.hpp"
#include "procedural_sources_v1.hpp"
namespace dh2::loader {
struct ProceduralGenerationServicesV66 {
 std::shared_ptr<void> owner;assets::ZipAssetPackV1 actual_resources;
 ProceduralXmlServicesV66 xml;
};
class ProceduralGeneratorBindingV66:public std::enable_shared_from_this<ProceduralGeneratorBindingV66> {
 struct Impl;std::unique_ptr<Impl> impl_;
 ProceduralGeneratorBindingV66(std::unique_ptr<Impl>);
public:
 ~ProceduralGeneratorBindingV66();
 static bool create(std::weak_ptr<CanonicalLevelContextV1>,ProceduralGenerationServicesV66,
  std::shared_ptr<ProceduralGeneratorBindingV66>&,std::string&);
 // Supplies constructor/generate/destroy/cancel/Assign only. Caller retains
 // actual online/progress/filesystem bodies from the existing source driver.
 Stage7ServicesV38 services();
 AssignedCopyLeavesV65 generated_copy_leaves();
};
}