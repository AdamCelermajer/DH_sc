#include "world.hpp"
#include "../world-data/source_layout_adapter.h"

#include <array>
#include <cstdint>
#include <cstring>
#include <string>
#include <utility>
#include <vector>

namespace dh2::world {
namespace {
// RecordKind::mgp is value 1 in the source-world adapter enum. Keep this
// comparison local: including world-data/world.hpp here would redefine the
// unrelated level-world Transform in the shared namespace.
constexpr RecordKind source_mgp_kind = static_cast<RecordKind>(1);
std::uint32_t word(const std::uint8_t* p) {
    return p[0] | (std::uint32_t(p[1]) << 8) |
           (std::uint32_t(p[2]) << 16) | (std::uint32_t(p[3]) << 24);
}
void store_float(std::uint8_t* p, float value) {
    std::uint32_t bits = 0;
    std::memcpy(&bits, &value, sizeof(bits));
    p[0] = static_cast<std::uint8_t>(bits);
    p[1] = static_cast<std::uint8_t>(bits >> 8);
    p[2] = static_cast<std::uint8_t>(bits >> 16);
    p[3] = static_cast<std::uint8_t>(bits >> 24);
}

bool fixed_string(const char* input, std::size_t capacity, std::string& value) {
    const auto* end = static_cast<const char*>(std::memchr(input, 0, capacity));
    if (!end || end == input) return false;
    value.assign(input, static_cast<std::size_t>(end - input));
    return true;
}

}

bool compile_source_layout(const std::uint8_t* mlx, std::size_t mlx_size,
                           const std::uint8_t* spawnpoints, std::size_t spawn_size,
                           std::int32_t entrypoint_id,
                           std::vector<std::uint8_t>& descriptor, std::string& error) {
    descriptor.clear();
    error.clear();
    constexpr std::size_t max_size = 24 + 256 * 128;
    std::vector<std::uint8_t> result(max_size);
    std::size_t result_size = 0;
    char message[160]{};
    if (!dh2_world_compile_static_layout(mlx, mlx_size, "SWAMP",
            "data/scene/001_swamp.mlx", result.data(), result.size(),
            &result_size, message, sizeof(message))) {
        error = message[0] ? message : "Original source MLX import failed";
        return false;
    }
    result.resize(result_size);

    const auto rooms = word(result.data() + 8);
    std::vector<EntryPoint> entries;
    if (!load_entrypoints(spawnpoints, spawn_size, rooms, entries, error)) return false;
    const EntryPoint* selected = nullptr;
    for (const auto& entry : entries) {
        if (entry.id != entrypoint_id) continue;
        if (selected) {
            error = "Duplicate source SpawnPoint entrypoint ID";
            return false;
        }
        selected = &entry;
    }
    if (!selected) {
        error = "Source SpawnPoint entrypoint ID is absent";
        return false;
    }
    for (unsigned axis = 0; axis < 3; ++axis)
        store_float(result.data() + 12 + axis * 4, selected->world.position[axis]);
    descriptor = std::move(result);
    error.clear();
    return true;
}

bool compile_source_spawnpoints(const std::uint8_t* mlx, std::size_t mlx_size,
                               const SourceMgpView* mgps, std::size_t mgp_count,
                               std::vector<std::uint8_t>& spawnpoints, std::string& error) {
    spawnpoints.clear();
    error.clear();
    if (!mgps || !mgp_count || mgp_count > 256) {
        error = "Original source MGP list is empty or exceeds the runtime limit";
        return false;
    }
    std::vector<dh2_world_source_mgp> source_mgps;
    source_mgps.reserve(mgp_count);
    for (std::size_t i = 0; i < mgp_count; ++i) {
        if (!mgps[i].source_path || !mgps[i].data || !mgps[i].size) {
            error = "Original source MGP list contains an incomplete input";
            return false;
        }
        source_mgps.push_back({mgps[i].source_path, mgps[i].data, mgps[i].size});
    }

    constexpr std::size_t max_size = 16 + 3 * 144;
    std::vector<std::uint8_t> result(max_size);
    std::size_t result_size = 0;
    char message[160]{};
    if (!dh2_world_compile_static_spawnpoints(mlx, mlx_size, "SWAMP",
            "data/scene/001_swamp.mlx", source_mgps.data(), source_mgps.size(),
            result.data(), result.size(), &result_size, message, sizeof(message))) {
        error = message[0] ? message : "Original source MGP import failed";
        return false;
    }
    result.resize(result_size);
    spawnpoints = std::move(result);
    return true;
}

bool compile_source_dact(const std::uint8_t* mlx, std::size_t mlx_size,
                         const SourceMgpView* mgps, std::size_t mgp_count,
                         const data::CharacterTable& characters,
                         const data::Dictionary& models,
                         std::vector<std::uint8_t>& dact,std::string& error) {
    dact.clear();
    error.clear();
    if (!mgps || !mgp_count || mgp_count > 256) {
        error = "Original source MGP list is empty or exceeds the runtime limit";
        return false;
    }
    std::vector<dh2_world_source_mgp> source_mgps;
    source_mgps.reserve(mgp_count);
    for (std::size_t i = 0; i < mgp_count; ++i) {
        if (!mgps[i].source_path || !mgps[i].data || !mgps[i].size) {
            error = "Original source MGP list contains an incomplete input";
            return false;
        }
        source_mgps.push_back({mgps[i].source_path, mgps[i].data, mgps[i].size});
    }

    constexpr std::size_t max_size = 16 + 5 * 256;
    std::vector<std::uint8_t> result(max_size);
    std::size_t result_size = 0;
    char message[160]{};
    if (!dh2_world_compile_static_dact(mlx, mlx_size, "SWAMP",
            "data/scene/001_swamp.mlx", source_mgps.data(), source_mgps.size(),
            &characters, &models, result.data(), result.size(), &result_size,
            message, sizeof(message))) {
        error = message[0] ? message : "Original source MGP actor import failed";
        return false;
    }
    result.resize(result_size);
    dact = std::move(result);
    return true;
}

bool compile_source_dact_with_handles(const std::uint8_t* mlx,
                         std::size_t mlx_size, const SourceMgpView* mgps,
                         std::size_t mgp_count,
                         const data::CharacterTable& characters,
                         const data::Dictionary& models,
                         const source_handle_ledger_v1::Ledger& handle_ledger,
                         std::vector<StaticDactActorHandleV1>& actor_sources,
                         std::vector<std::uint8_t>& dact,std::string& error) {
    actor_sources.clear();
    dact.clear();
    error.clear();
    if (!mlx || !mlx_size || !mgps || !mgp_count || mgp_count > 256 ||
        handle_ledger.occurrences.empty()) {
        error = "Source DACT handle mapping inputs are incomplete";
        return false;
    }
    std::vector<dh2_world_source_mgp> source_mgps;
    source_mgps.reserve(mgp_count);
    for (std::size_t i = 0; i < mgp_count; ++i) {
        if (!mgps[i].source_path || !mgps[i].data || !mgps[i].size) {
            error = "Source DACT handle mapping MGP input is incomplete";
            return false;
        }
        source_mgps.push_back({mgps[i].source_path, mgps[i].data, mgps[i].size});
    }

    constexpr std::size_t actor_count = 5;
    constexpr std::size_t dact_size = 16 + actor_count * 256;
    std::vector<std::uint8_t> bytes(dact_size);
    std::array<dh2_world_source_static_character, actor_count> source_rows{};
    std::size_t output_size = 0, source_count = 0;
    char message[160]{};
    if (!dh2_world_compile_static_dact_with_sources(mlx, mlx_size, "SWAMP",
            "data/scene/001_swamp.mlx", source_mgps.data(), source_mgps.size(),
            &characters, &models, bytes.data(), bytes.size(), &output_size,
            source_rows.data(), source_rows.size(), &source_count,
            message, sizeof(message))) {
        error = message[0] ? message : "Original source DACT provenance compile failed";
        return false;
    }
    if (output_size != dact_size || source_count != actor_count) {
        error = "Source DACT rows and retained provenance count differ";
        return false;
    }

    std::vector<StaticDactActorHandleV1> resolved;
    resolved.reserve(actor_count);
    for (std::size_t index = 0; index < source_count; ++index) {
        const auto& row = source_rows[index];
        const source_handle_ledger_v1::Occurrence* selected = nullptr;
        for (const auto& occurrence : handle_ledger.occurrences) {
            const auto& source = occurrence.source;
            if (source.kind != source_mgp_kind ||
                source.module_index != row.module_index ||
                source.record_index != row.source_record) continue;
            if (selected) {
                error = "Source DACT provenance maps to multiple MGP handle occurrences";
                return false;
            }
            selected = &occurrence;
        }
        if (!selected || selected->handle < 0 || selected->duplicate_name ||
            selected->source.name != row.name ||
            selected->source.gametype != "Character" || row.module_index >= mgp_count) {
            error = "Source DACT MGP provenance has no unique retained ObjectManager handle";
            return false;
        }
        StaticDactActorHandleV1 actor{};
        actor.dact_record = static_cast<std::uint32_t>(index);
        actor.module_index = row.module_index;
        actor.source_record = row.source_record;
        actor.source_handle = selected->handle;
        actor.name = row.name;
        actor.character = row.character;
        resolved.push_back(std::move(actor));
    }
    actor_sources = std::move(resolved);
    dact = std::move(bytes);
    error.clear();
    return true;
}

bool compile_source_mvp(const std::uint8_t* mlx, std::size_t mlx_size,
                        const SourceMvpView* mvps, std::size_t mvp_count,
                        std::vector<SourceMvpDecor>& out, std::string& error) {
    out.clear();
    error.clear();
    if (!mvps || !mvp_count || mvp_count > 256) {
        error = "Original source MVP list is empty or exceeds the runtime limit";
        return false;
    }
    std::vector<dh2_world_source_mvp> source_mvps;
    source_mvps.reserve(mvp_count);
    for (std::size_t i = 0; i < mvp_count; ++i) {
        if (!mvps[i].source_path || !mvps[i].data || !mvps[i].size) {
            error = "Original source MVP list contains an incomplete input";
            return false;
        }
        source_mvps.push_back({mvps[i].source_path, mvps[i].data, mvps[i].size});
    }

    constexpr std::size_t max_decors = 10;
    std::array<dh2_world_source_static_decor, max_decors> records{};
    std::size_t record_count = 0;
    char message[160]{};
    if (!dh2_world_compile_static_mvp(mlx, mlx_size, "SWAMP",
            "data/scene/001_swamp.mlx", source_mvps.data(), source_mvps.size(),
            records.data(), records.size(), &record_count, message, sizeof(message))) {
        error = message[0] ? message : "Original source MVP import failed";
        return false;
    }
    if (record_count != max_decors) {
        error = "Original source MVP compiler returned an unexpected Decor count";
        return false;
    }

    std::vector<SourceMvpDecor> result;
    result.reserve(record_count);
    for (std::size_t i = 0; i < record_count; ++i) {
        const auto& source = records[i];
        SourceMvpDecor decor{};
        decor.module_index = source.module_index;
        decor.source_record = source.source_record;
        if (!fixed_string(source.name, sizeof(source.name), decor.name) ||
            !fixed_string(source.xrefobject, sizeof(source.xrefobject), decor.xrefobject) ||
            !fixed_string(source.dae_path, sizeof(source.dae_path), decor.dae_path) ||
            !fixed_string(source.source_path, sizeof(source.source_path), decor.source_path)) {
            error = "Source MVP compiler returned an invalid fixed identity field";
            return false;
        }
        for (unsigned axis = 0; axis < 3; ++axis) {
            decor.local.position[axis] = source.local_transform[axis];
            decor.local.rotation_degrees[axis] = source.local_transform[3 + axis];
            decor.local.scale[axis] = source.local_transform[6 + axis];
            decor.world.position[axis] = source.world_transform[axis];
            decor.world.rotation_degrees[axis] = source.world_transform[3 + axis];
            decor.world.scale[axis] = source.world_transform[6 + axis];
        }
        result.push_back(std::move(decor));
    }
    out = std::move(result);
    return true;
}
}
