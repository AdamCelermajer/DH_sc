#pragma once

#include "container_class_registry_v1.hpp"
#include "../../actor_definitions.hpp"
#include "../../asset_catalog.hpp"
#include "../../original_character.hpp"

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
    Mat4 transform{};          // authored placement (translation, rotation, scale)
    std::uint8_t state = 0;    // source state byte; 0 = closed (Preview 16 T1)
    bool visual_ready = false;
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

// Decodes each distinct visual once through the embedded-scene path (rigid or
// skinned game objects with their own named clips). Texture binding stays in main,
// shared with the actor materials. Missing files keep their instances hidden.
class ContainerVisualsV1 {
public:
    bool load(const AssetCatalog& assets, std::vector<ContainerInstanceV1>& instances,
              std::vector<std::string>& notices);
    std::map<std::string, std::unique_ptr<CharacterVisual>>& visuals() noexcept { return visuals_; }
    const std::map<std::string, std::unique_ptr<CharacterVisual>>& visuals() const noexcept { return visuals_; }

private:
    std::map<std::string, std::unique_ptr<CharacterVisual>> visuals_;
};

} // namespace dh::foundation::containers
