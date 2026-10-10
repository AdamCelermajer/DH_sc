#pragma once
#include "asset_catalog.hpp"
#include "original_scene.hpp"
#include <array>
#include <cstdint>
#include <vector>

namespace dh::foundation {

// One RoomZone source per loaded module (visible, unconditional placement).
// `id` is the placement index among ALL manifest Module declarations, so it stays
// stable when other modules are hidden or conditional. `bounds` is the world AABB
// (minX,minY,minZ,maxX,maxY,maxZ) of the module's `_module_` room box, falling back
// to its visible geometry when the module exports no room box. Ranges index
// OriginalScene::mesh.ranges.
struct LevelModuleZone {
    std::uint32_t id = 0;
    std::string name;
    std::array<float, 6> bounds{};
    std::size_t firstRange = 0, rangeCount = 0;
};
bool load_level_with_module_zones(AssetCatalog& assets,const std::filesystem::path& manifestRelative,
                                  OriginalScene& output,std::vector<LevelModuleZone>& zones,std::string& error);
// Loads visible, unconditional authored module geometry. Campaign predicates
// require a gameplay evaluator; conditional declarations are skipped/reported.
bool load_level(AssetCatalog& assets,const std::filesystem::path& manifestRelative,
                OriginalScene& output,std::string& error);
}
