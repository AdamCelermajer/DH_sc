#pragma once

#include "data.hpp"
#include "player_savegame_v1.hpp"

#include <string>

namespace dh2::data {
struct LevelTables;
struct WorldMapTables;
}

namespace dh2::data::player_level_states_save_writer_v1 {

enum class Status {
    complete,
    invalid_argument,
    source_assertion_boundary,
    failed,
};

struct WriteServicesV1 {
    void* context{};
    // The sink copies synchronously. A failed call keeps any prefix it already
    // accepted, matching the original IStreamBase callback's write order.
    bool (*write)(void*, Bytes, std::string&){};
};

// One LVLS callback writes all three LevelList groups, then all three
// WorldMap groups. Tables, Save arrays, and the sink are borrowed synchronously.
// This writer owns no alternate level/map table or saved-state store.
Status write_lvls_v1(const LevelTables&, const WorldMapTables&,
                     const PlayerSavegameV1&, const WriteServicesV1&,
                     std::string& error);

}  // namespace dh2::data::player_level_states_save_writer_v1
