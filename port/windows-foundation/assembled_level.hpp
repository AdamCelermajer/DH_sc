#pragma once
#include "asset_catalog.hpp"
#include "original_scene.hpp"

namespace dh::foundation {
// Loads visible, unconditional authored module geometry. Campaign predicates
// require a gameplay evaluator; conditional declarations are skipped/reported.
bool load_level(AssetCatalog& assets,const std::filesystem::path& manifestRelative,
                OriginalScene& output,std::string& error);
}
