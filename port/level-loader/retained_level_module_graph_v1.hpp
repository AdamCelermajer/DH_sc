#pragma once
#include <canonical_module_files_v1.hpp>
#include "canonical_module_graph_v3.hpp"
#include "module_draw_frame_v1.hpp"

namespace dh2::loader {
enum class RetainedModulePreparationStageV1 {
    idle, root_file, root_complete, module_init_post, modules_initialized,
    floor_post_load, floors_prepared, module_init_final, geometry_prepared,
    module_files, object_sources_complete, failed
};
struct RetainedModulePreparationStatusV1 {
    RetainedModulePreparationStageV1 stage{RetainedModulePreparationStageV1::idle};
    RetainedModulePreparationStageV1 failed_at{RetainedModulePreparationStageV1::idle};
    std::size_t declarations{}, modules{}, initialized{}, finalized{}, loaded{};
    bool root_complete{}, floors_prepared{}, geometry_prepared{}, object_sources_complete{};
    std::string error;
};
struct RetainedLevelModuleGraphInputsV1 {
    assets::ZipAssetPackV1 archive;
    std::shared_ptr<CanonicalLevelContextV1> level;
    // Pins the actual manager, class dispatch and all borrowed providers.
    // Keep it and this preparation as siblings; it must not own preparation.
    std::shared_ptr<void> candidate_owner;
    world::CanonicalObjectManagerV1* manager{};
    world::CanonicalClassServicesV1 classes;
    CanonicalFileSourceServicesV1 source;
    std::shared_ptr<world::CanonicalModuleGraphV3> graph;
    std::shared_ptr<floors::World> floors;
    std::shared_ptr<world::ModulePFRoomsV3> rooms;
};
// Retained SOURCE preparation, usable by the application and renderer.
// Borrows the existing Level/manager/dispatch and actual map/floor graph. No
// Level/manager/current-global construction, publication or substitute engine
// providers. Operations are explicit preparation stages, NOT a reconstruction
// of the complete Level::Init call order. Unsupported declarations stop the
// original unfiltered walk; failure retains every completed source prefix.
class RetainedLevelModuleGraphV1 final {
    RetainedLevelModuleGraphInputsV1 input_;
    std::unique_ptr<CanonicalCachedFileV1> root_;
    std::shared_ptr<CanonicalModuleFilesV1> files_;
    std::vector<std::shared_ptr<world::CanonicalModuleRecordV2>> modules_;
    RetainedModulePreparationStatusV1 status_;
    bool busy_{},floor_attempted_{},active_{true};
    std::vector<bool> init_attempted_,final_attempted_;
    explicit RetainedLevelModuleGraphV1(RetainedLevelModuleGraphInputsV1);
    bool fail(RetainedModulePreparationStageV1,const std::string&,std::string&);
    bool enter(RetainedModulePreparationStageV1,std::string&);
    bool collect_modules(std::string&);
public:
    static bool create(RetainedLevelModuleGraphInputsV1,
        std::shared_ptr<RetainedLevelModuleGraphV1>& out,std::string&);
    RetainedLevelModuleGraphV1(const RetainedLevelModuleGraphV1&)=delete;
    RetainedLevelModuleGraphV1& operator=(const RetainedLevelModuleGraphV1&)=delete;
    LevelFileWalkStepV1 load_root_step();
    bool initialize_next_module(std::string&);
    bool prepare_floors(std::string&);
    bool finalize_next_module(std::string&);
    bool load_next_module_sources(const world::ModuleXmlServicesV1&,std::string&);
    // A per-frame immutable resource pin, never a second scene/world. Missing
    // visuals fail; no demo geometry, transform recomputation or actor skipping.
    bool capture_draw_frames(std::vector<ModuleDrawFrameV1>& out,std::string&)const;
    const RetainedModulePreparationStatusV1& status()const noexcept{return status_;}
    const auto& level()const noexcept{return input_.level;}
    world::CanonicalObjectManagerV1& manager()const noexcept{return *input_.manager;}
    const auto& floor_world()const noexcept{return input_.floors;}
    const auto& modules()const noexcept{return modules_;}
    const CanonicalCachedFileV1& root_file()const noexcept{return *root_;}
    const auto& module_files()const noexcept{return files_;}
    // Actual Level::Unload/manager unpublication/visual release runs first in
    // the caller. This releases only journals, with retry of unfinished tails.
    bool discard_after_owner_release(const std::function<bool(std::string&)>&,std::string&);
};
}
