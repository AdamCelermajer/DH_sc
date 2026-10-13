#pragma once
#include <array>
#include <cstdint>
// Plain data shared by player_gameplay_audio.hpp, its generated catalog and the
// Android SFX backend. No behaviour lives here.
namespace dh2::player_gameplay_audio {
// Source: original data/sounds/sounds_he.xml PCM, retained skill and monster
// playback subset. UID is the original sound-pack UID used by StopSound.
struct CatalogEntry {
    const char* label;
    const char* catalog_filename;
    const char* filename;
    std::uint16_t uid;
    // Original SDD Sounds-table ID consumed by VoxSoundManager::Play3D.
    // Most script cues enter by label and do not need this field.
    std::uint16_t source_sound_id=0xffff;
};
// Sounds.xml <sound> attributes the playback owner honours.
struct SfxDefV1 {
    std::uint16_t uid;
    std::uint8_t bank;      // vox priority bank (1 MUSIC .. 7 SKILLS)
    std::uint8_t group;     // Sounds.xml group uid (selects 3D mode and volume slider)
    std::int8_t priority;   // -1, 1, 2, 3; higher wins in the steal behaviours
    bool loop;              // Vox::Play receives this flag, not the caller's loop flag
};
// <bank> maxplaybacks/behaviour (threshold keeps the BankXMLDef default,
// INT_MIN+1, because Sounds.xml sets none).
// behaviour: 0 steal oldest, 1 steal lowest priority, 2 steal low prio or old
// same prio, 3 refuse the new emitter ("do nothing" is not a recognised string).
struct SfxBankV1 {
    std::uint8_t id;
    std::int32_t max_playbacks;
    std::uint8_t behaviour;
};
// <group> pos3d: 0 no, 1 yes, 2 relative; slider: 0 MUSIC, 1 SFX, 2 VFX.
struct SfxGroupV1 {
    std::uint8_t id;
    std::uint8_t pos3d;
    std::uint8_t slider;
};
// <event>: random picks without repeating the last `history` picks, playlist
// cycles in order; `probability` is the percent chance that anything plays.
struct SfxEventV1 {
    std::uint8_t id;
    bool playlist;
    std::uint8_t history;
    std::uint8_t probability;
    std::uint8_t count;
    std::array<std::uint16_t,4> uids;
};
// SDD Sounds rows whose +8 flag selects an event (VoxSoundManager::Play3D).
struct SfxEventRowV1 {
    std::uint16_t row;
    std::uint8_t event;
};
} // namespace dh2::player_gameplay_audio
