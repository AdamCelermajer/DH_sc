#pragma once
#include "lifecycle_v36.hpp"
#include "assigned_root_source_v64.hpp"
#include "filename_root_source_v65.hpp"
#include "stage_loader_v38_root_file.hpp"
#include "stage_loader_v38_init_post.hpp"
#include <canonical_object_manager_v1.hpp>
#include <module_xml_selection_v1.hpp>
namespace dh2::loader {
class RetainedLevelModuleGraphV1;
struct SourceLoadingInputsV43 {
 std::shared_ptr<RetainedLevelModuleGraphV1> preparation;
 // Aliasing shared_ptr is allowed for a manager inside the real candidate owner.
 std::shared_ptr<world::CanonicalObjectManagerV1> manager;
 Stage7GlobalsV38 globals;
 Stage7ServicesV38 procedural;
 AssignedRootServicesV64 assigned_source;
 // Production native filename branch; absent retains historical ZIP inspection.
 std::shared_ptr<FilenameRootRouteV65> native_filename_source;
 std::shared_ptr<void> filename_provider;
 // Original filesystem/mount resolution, supplied by the actual application.
 // Source spelling and the resolved resource identity stay distinct.
 std::function<bool(const std::string&,std::string&,std::string&)> resolve_filename;
 world::ModuleXmlServicesV1 module_xml;
 // Original Stage10 GetInstance/GetSwitch(isTracingLevel_Loading), once
 // before its map1c counter snapshot and resumable InitPost body.
 std::function<bool(std::string&)> stage10_trace;
 CanonicalInitPostServicesV38<world::CanonicalObjectManagerV1> object_services;
 // Real remaining stage bodies, progress and teardown. Leave7/10 and after7 empty.
 LifecycleServicesV36 external;
 std::vector<std::shared_ptr<const void>> resource_pins;
};
// Connects original source stages7/10 to the SAME C1/registry/preparation.
// It never selects a state, publishes GSLevel, creates globals, or certifies
// whole initialization. Other bodies must perform their actual original work.
class SourceLoadingV43 final {
 struct Impl;std::unique_ptr<Impl> impl_;
 explicit SourceLoadingV43(std::unique_ptr<Impl>);
public:
 static bool create(SourceLoadingInputsV43,std::unique_ptr<SourceLoadingV43>&,std::string&);
 ~SourceLoadingV43();
 SourceLoadingV43(const SourceLoadingV43&)=delete;
 SourceLoadingV43& operator=(const SourceLoadingV43&)=delete;
 LifecycleStatusV36 tick();
 void request_cancel() noexcept;
 const LifecycleDiagnosticsV36& diagnostics()const noexcept;
};
}
