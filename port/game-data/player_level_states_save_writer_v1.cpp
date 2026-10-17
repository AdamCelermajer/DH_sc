#include "player_level_states_save_writer_v1.hpp"

#include "level_tables.hpp"
#include "world_map_tables.hpp"

#include <cstdint>
#include <cstring>
#include <limits>

namespace dh2::data::player_level_states_save_writer_v1 {
namespace {

constexpr std::uint32_t kMaxRows = 65536;
constexpr std::uint32_t kMaxNameBytes = 1024 * 1024;

bool emit(const WriteServicesV1& stream, const std::uint8_t* bytes,
          std::size_t size, std::string& error) {
    if (!stream.write(stream.context, {bytes, size}, error)) {
        if (error.empty()) error = "source LVLS stream write failed";
        return false;
    }
    return true;
}

bool word(const WriteServicesV1& stream, std::int32_t value,
          std::string& error) {
    const auto bits = static_cast<std::uint32_t>(value);
    const std::uint8_t bytes[]{std::uint8_t(bits), std::uint8_t(bits >> 8),
                              std::uint8_t(bits >> 16),
                              std::uint8_t(bits >> 24)};
    return emit(stream, bytes, sizeof(bytes), error);
}

bool source_string(const WriteServicesV1& stream, const std::string& value,
                   std::string& error) {
    // PlayerSavegame::__SaveLevelStates uses writeAs(std::string): an int32
    // byte length including the C terminator, followed by one data write.
    const auto length = std::strlen(value.c_str());
    if (length >= kMaxNameBytes ||
        length >= std::size_t(std::numeric_limits<std::int32_t>::max())) {
        error = "source LVLS table name exceeds native bound";
        return false;
    }
    const auto size_with_terminator = static_cast<std::int32_t>(length + 1);
    if (!word(stream, size_with_terminator, error)) return false;
    return emit(stream, reinterpret_cast<const std::uint8_t*>(value.c_str()),
                length + 1, error);
}

bool write_level_groups(const LevelTables& levels,
                        const PlayerSavegameV1& save,
                        const WriteServicesV1& stream,
                        std::string& error, bool& source_boundary) {
    for (std::uint32_t difficulty = 0; difficulty < 3; ++difficulty) {
        // IDA 0x46a838 reads LevelList::size once for each difficulty group.
        const auto count = levels.levels.size();
        if (count > kMaxRows || count > std::size_t(INT32_MAX)) {
            error = "source LevelList count exceeds native bound";
            return false;
        }
        if (!word(stream, static_cast<std::int32_t>(count), error)) return false;

        for (std::uint32_t row = 0; row < count; ++row) {
            // The source copies the C name into a temporary before writing it.
            if (row >= levels.levels.size()) {
                error = "source LevelList row disappeared during LVLS write";
                return false;
            }
            const auto name = levels.levels[row].name;
            if (!source_string(stream, name, error)) return false;

            // IDA 0x467040 dereferences the saved array after writing the name.
            // Missing or short native backing fails at that same reached prefix.
            const auto* states = save.source_level_states(difficulty);
            if (!states || !states->words || row >= states->count) {
                error = "source LevelList saved-state array unavailable at reached row";
                source_boundary = true;
                return false;
            }
            if (!word(stream, states->words[row], error)) return false;
        }
    }
    return true;
}

bool write_world_map_groups(const WorldMapTables& world_map,
                            const PlayerSavegameV1& save,
                            const WriteServicesV1& stream,
                            std::string& error, bool& source_boundary) {
    for (std::uint32_t difficulty = 0; difficulty < 3; ++difficulty) {
        // There is no separate source WorldMap save callback: the same
        // __SaveLevelStates function reads this count after its level groups.
        const auto count = world_map.locations.size();
        if (count > kMaxRows || count > std::size_t(INT32_MAX)) {
            error = "source WorldMap count exceeds native bound";
            return false;
        }
        if (!word(stream, static_cast<std::int32_t>(count), error)) return false;

        for (std::uint32_t row = 0; row < count; ++row) {
            if (row >= world_map.locations.size()) {
                error = "source WorldMap row disappeared during LVLS write";
                return false;
            }
            const auto name = world_map.locations[row].name;
            if (!source_string(stream, name, error)) return false;

            const auto* states = save.source_world_map_states(difficulty);
            if (!states || !states->words || row >= states->count) {
                error = "source WorldMap saved-state array unavailable at reached row";
                source_boundary = true;
                return false;
            }
            if (!word(stream, states->words[row], error)) return false;
        }
    }
    return true;
}

}  // namespace

Status write_lvls_v1(const LevelTables& levels, const WorldMapTables& world_map,
                     const PlayerSavegameV1& save,
                     const WriteServicesV1& stream, std::string& error) {
    error.clear();
    if (!stream.write) {
        error = "source LVLS stream writer required";
        return Status::invalid_argument;
    }

    try {
        bool source_boundary = false;
        if (!write_level_groups(levels, save, stream, error, source_boundary) ||
            !write_world_map_groups(world_map, save, stream, error,
                                    source_boundary))
            return source_boundary ? Status::source_assertion_boundary
                                   : Status::failed;
        return Status::complete;
    } catch (...) {
        if (error.empty()) error = "source LVLS callback provider threw";
        return Status::failed;
    }
}

}  // namespace dh2::data::player_level_states_save_writer_v1
