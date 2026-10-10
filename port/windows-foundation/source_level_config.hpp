#pragma once
#include "../level-world/canonical_level_config_module_v1.hpp"
#include "../level-world/character_design_services.hpp"
#include <functional>
#include <memory>
#include <optional>

namespace dh::foundation {
class AssetCatalog;
struct SourceLevelConfigNetworkBorrow {
    std::shared_ptr<void> applicationOwner;
    const std::uint8_t* onlineByte5 = nullptr;
};
// Recovered AssignObjectNetworkId3431c0 begins by querying actual Online.byte5.
// Offline343274 returns without stores; online requires a different provider.
bool source_level_config_network_leaf(const SourceLevelConfigNetworkBorrow&,
    dh2::world::CanonicalObjectBorrowV1&,std::string& error);
std::function<bool(const char*,bool&,std::string&)> source_level_config_debug_switch(
    std::shared_ptr<dh2::character::DebugSwitches>,std::shared_ptr<void> filesOwner,
    dh2::character::DebugFileServices24,
    std::optional<dh2::character::DebugExistingFileServicesV136> streams = {});
// In-house current-Level config field. Retain as a sibling of construction
// providers; do not put the provider itself inside the Level it pins.
struct SourceLevelConfigSlot { std::shared_ptr<dh2::world::CanonicalLevelConfigV1> config; };
struct SourceLevelConfigProviders {
    std::shared_ptr<void> levelOwner;
    std::shared_ptr<dh2::world::CanonicalPropertyMapV1> properties;
    std::shared_ptr<SourceLevelConfigSlot> current;
    // Must read actual runtime Debug settings. Missing debug fails at InitPost.
    std::function<bool(const char*,bool&,std::string&)> debugSwitch;
    dh2::world::CanonicalClassServicesV1 remaining;
    std::shared_ptr<void> remainingOwner;
};
struct SourceLevelSettings {
    std::array<float,3> ambient{},fogColorRaw{},fogDirectionMask{},clearColorRaw{};
    std::int32_t fogStart=0,fogEnd=0,cameraNear=0,cameraFar=0;
    std::string cameraFile,cameraName,cameraAnimationSet,lightSet,fixedLightSet,skybox;
};
// Recovered class constructor/property/InitPost composition only. Unhandled
// classes delegate to actual remaining services, or fail at their reached call.
class SourceLevelConfig {
public:
    explicit SourceLevelConfig(SourceLevelConfigProviders);
    dh2::world::CanonicalClassServicesV1 services() const noexcept;
    std::shared_ptr<void> lease() const noexcept;
    bool settings(SourceLevelSettings&,std::string& error) const;
private:
    struct Impl;
    std::shared_ptr<Impl> impl_;
};
// Bounded extraction of the single original LevelConfig declaration from the
// selected resource. Executes its actual source factory registration, property
// defaults/overrides and InitPost against caller's SAME manager/current field.
// Does NOT execute earlier/later unrelated declarations, full Level walk,
// Module constructors, room-ID mapping or whole Level.SetLevelConfig effects.
// Runtime Debug/files and property-source callbacks remain explicit in p.
bool load_original_level_config(const AssetCatalog&,const std::string& levelUri,
    SourceLevelConfigProviders p,
    const std::shared_ptr<dh2::world::CanonicalObjectManagerV1>& manager,
    SourceLevelSettings&,std::string& error);
} // namespace dh::foundation
