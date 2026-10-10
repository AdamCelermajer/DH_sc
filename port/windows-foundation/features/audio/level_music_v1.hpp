#pragma once

#include <string>

namespace dh::foundation {
class AssetCatalog;
}

namespace dh::foundation::audio {

// Original LevelConfig music names, as stored by Level::SetLevelConfig from the
// level scene attributes `music` and `safezone_music`. Empty means absent.
// Example: data/scene/001_swamp.mlx -> SwampHubAmbientMusic / SwampMerchantCampMusic.
struct LevelMusicNamesV1 {
    std::string music;
    std::string safezone;
};

// Reads the LevelConfig element (gametype="LevelConfig") from the exact level
// scene through the same XML owner as load_original_level_config. Missing scene
// or malformed XML is an error; a LevelConfig without `music` is not an error.
bool read_level_music_names_v1(const AssetCatalog& assets, const std::string& levelUri,
    LevelMusicNamesV1& names, std::string& error);

} // namespace dh::foundation::audio
