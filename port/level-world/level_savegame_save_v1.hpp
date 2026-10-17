#pragma once

#include <cstdint>
#include <string>

namespace dh2::level_savegame_save_v1 {

struct State {
    std::uintptr_t savegame{};
    std::uint8_t gate_39{};
    std::uint32_t online{};
    std::uint32_t local_player_is_host{};
    // PlayerManager byte at +0x719, read only for online local-host saves.
    std::uint32_t player_manager_gate_719{};
};

struct Services {
    void* context{};
    // Calls Savegame::saveAll on this same LevelSavegame+4 identity. The
    // canonical registry/section writers and persistence backend stay borrowed.
    std::int32_t (*save_all)(void*, std::uintptr_t savegame,
                             std::string& error){};
};

enum class Status : std::uint32_t {
    saved, skipped, invalid_argument, service_unavailable, service_failed
};

struct Result {
    Status status{Status::invalid_argument};
    std::uint32_t service_calls{};
};

// LevelSavegame::Save at ELF 0x4615ec. The source only dispatches saveAll
// when +4 exists, byte +0x39 is clear, and the save is offline or local-host
// with Application+0x719 clear.
Status run(const State*, const Services*, Result*, std::string& error);

} // namespace dh2::level_savegame_save_v1
