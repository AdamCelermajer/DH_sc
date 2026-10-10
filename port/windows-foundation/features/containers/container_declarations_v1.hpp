#pragma once

#include "container_class_registry_v1.hpp"
#include "../../actor_definitions.hpp"
#include "../../asset_catalog.hpp"

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "../../../level-loader/game_object_arrays_owner_v81.hpp"

namespace dh::foundation::containers {

// Original container tables (Arrays::OpenableContainers / DestructibleContainers
// and the GameObjectDict visual list), decoded once from the grouped game_objects
// streams in the asset package.
class ContainerTablesV1 {
public:
    bool load(const AssetCatalog& assets, std::string& error);
    bool ready() const noexcept { return ready_; }
    const dh2::loader::GameObjectArraysOwnerV81::Borrow& borrow() const noexcept { return borrow_; }

private:
    dh2::loader::GameObjectArraysOwnerV81::Borrow borrow_;
    bool ready_ = false;
};

struct ContainerInstanceV1 {
    std::uint64_t stableId = 0;
    std::string sourceId, name, gametype, data_desc;
    ContainerFamilyV1 family = ContainerFamilyV1::openable;
    std::int32_t interaction_type = -1;
    std::int32_t row = -1, visual_id = -1, loot_id = -1, sound_id = -1;
    std::string visual_file;   // GameObjectDict ColladaFile, package-relative
    std::string script;        // authored row script (Lua OnOpen), empty when none
    Mat4 transform{};          // authored placement (translation, rotation, scale)
    bool visual_ready = false; // set by ContainerRuntimeV1::adopt
};

struct ContainerLoadReportV1 {
    std::size_t declarations = 0;   // container-family declarations seen
    std::size_t instantiated = 0;
    std::map<std::string, std::size_t> unsupported;  // unregistered gametype -> count
    std::vector<std::string> notices;                // unresolved row/visual, one line each
};

// Builds one ContainerInstanceV1 per authored declaration whose gametype is
// registered and whose data_desc resolves in the original table. Everything is
// read from the authored declarations and the original tables; no map names.
bool load_container_instances_v1(const ContainerTablesV1& tables,
                                 const ContainerClassRegistryV1& registry,
                                 const std::vector<ActorDefinition>& definitions,
                                 std::vector<ContainerInstanceV1>& out,
                                 ContainerLoadReportV1& report,
                                 std::string& error);

} // namespace dh::foundation::containers
