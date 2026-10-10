#pragma once

#include <cstdint>
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

// Original fades (ms) for the level-music owner. Sources (IDA pseudocode-all.c):
// Level::Update PlayMusic(...,2000) at the first update; PlayerManager::ReviveLocalPlayers
// StopAllMusic(2) then PlayMusic(...,1000); MenuMainMenu::Hide StopMusic(1000);
// VoxSoundManager::PlayMusic same-id branch Resume(emitter, 0.05 s).
inline constexpr int kLevelMusicStartFadeMs = 2000;
inline constexpr int kLevelMusicReviveStopMs = 2;
inline constexpr int kLevelMusicReviveFadeMs = 1000;
inline constexpr int kLevelMusicReturnStopFadeMs = 1000;
inline constexpr int kLevelMusicResumeMs = 50;

// Level::Update start gate. A track starts only when one is configured, the
// output is focused and not minimised (submit needs a focused output), its row
// resolved, and the requested track is not already owned (no restart).
inline bool level_music_start_due_v1(bool hasTrack, bool outputActive,
    std::int32_t requestedOrdinal, std::int32_t ownedOrdinal) noexcept {
    return hasTrack && outputActive && requestedOrdinal >= 0 && requestedOrdinal != ownedOrdinal;
}

// One diagnostic line per level-music transition, so a verifier can confirm
// transitions from the log without listening. kind: start, resume, switch,
// revive-stop, revive-restart, return-stop, output-pause, output-resume.
// B064: the same line for every music owner; scope is "Level" (gameplay) or "Frontend" (title/menu).
inline std::string music_transition_line_v1(const std::string& scope, const std::string& kind,
    const std::string& track, int fadeMs, const std::string& detail) {
    return scope + " music transition: kind=" + kind + " track=" + (track.empty() ? "none" : track) +
           " fadeMs=" + std::to_string(fadeMs) + (detail.empty() ? "" : " " + detail);
}
inline std::string level_music_transition_line_v1(const std::string& kind, const std::string& track,
    int fadeMs, const std::string& detail) {
    return music_transition_line_v1("Level", kind, track, fadeMs, detail);
}

} // namespace dh::foundation::audio
