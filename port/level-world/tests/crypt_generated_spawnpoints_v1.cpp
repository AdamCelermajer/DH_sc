#include "../crypt_generated_spawnpoints_v1.hpp"
#include "../../world-data/world.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

namespace {
using dh2::world::GeneratedMgpView;
using dh2::world::Object;
using dh2::world::SourceLevel;

void require(bool value, const char* message) {
    if (!value) {
        std::cerr << "FAIL: " << message << '\n';
        std::exit(1);
    }
}

std::string read_text(const std::filesystem::path& path) {
    std::ifstream file(path, std::ios::binary);
    require(static_cast<bool>(file), "cannot open source input");
    return std::string(std::istreambuf_iterator<char>(file), {});
}

std::uint32_t read_u32(const std::uint8_t* p) {
    return std::uint32_t(p[0]) | (std::uint32_t(p[1]) << 8) |
        (std::uint32_t(p[2]) << 16) | (std::uint32_t(p[3]) << 24);
}

std::int32_t read_i32(const std::uint8_t* p) {
    const auto bits = read_u32(p);
    std::int32_t value = 0;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

float read_float(const std::uint8_t* p) {
    const auto bits = read_u32(p);
    float value = 0;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

bool close(float a, float b) { return std::abs(a - b) <= 0.0001f; }

std::string filename_from(const char* path) {
    std::string normalized = path ? path : "";
    std::replace(normalized.begin(), normalized.end(), '\\', '/');
    const auto slash = normalized.find_last_of('/');
    return slash == std::string::npos ? normalized : normalized.substr(slash + 1);
}

const char* field(const Object& object, const char* name) {
    return dh2_world_field(&object, name);
}

void import_expected(SourceLevel& expected, const std::vector<GeneratedMgpView>& views) {
    dh2::world::Diagnostic diagnostic{};
    for (std::uint32_t i = 0; i < expected.module_count; ++i) {
        require(dh2_world_import_module_objects(&expected, i, dh2::world::RecordKind::mgp,
            views[i].source_path, views[i].data, views[i].size, &diagnostic) ==
            dh2::world::Error::ok, diagnostic.message);
    }
}

std::string spawn_record_text(const std::string& mgp) {
    const auto type = mgp.find("gametype=\"SpawnPoint\"");
    require(type != std::string::npos, "fixture module has no SpawnPoint");
    const auto begin = mgp.rfind("<GameObject", type);
    const auto end = mgp.find("/>", type);
    require(begin != std::string::npos && end != std::string::npos,
        "cannot locate source SpawnPoint record");
    return mgp.substr(begin, end + 2 - begin);
}

void compare(const std::vector<std::uint8_t>& bytes, const SourceLevel& expected) {
    require(bytes.size() >= 16 && std::memcmp(bytes.data(), "SPWN", 4) == 0,
        "SPWN header missing");
    require(read_u32(bytes.data() + 4) == 1 && read_u32(bytes.data() + 12) == 0,
        "SPWN version/reserved mismatch");
    const auto count = read_u32(bytes.data() + 8);
    require(bytes.size() == 16 + std::size_t(count) * 144,
        "SPWN record length mismatch");

    std::vector<const Object*> expected_rows;
    for (std::uint32_t i = 0; i < expected.entity_count; ++i)
        if (expected.entities[i].gametype &&
            std::strcmp(expected.entities[i].gametype, "SpawnPoint") == 0)
            expected_rows.push_back(&expected.entities[i]);
    require(expected_rows.size() == 2, "Crypt fixture should expose entrypoints 0 and 2");
    require(count == expected_rows.size(), "SPWN count differs from source MGPs");

    for (std::uint32_t i = 0; i < count; ++i) {
        const auto* row = bytes.data() + 16 + std::size_t(i) * 144;
        const auto& source = *expected_rows[i];
        std::int32_t expected_id = 0;
        require(field(source, "entrypointID") != nullptr, "source SpawnPoint ID absent");
        expected_id = std::atoi(field(source, "entrypointID"));
        require(read_i32(row) == expected_id, "entrypoint identity/order differs");
        require(read_u32(row + 4) == source.module_index,
            "module source order differs");
        require(read_i32(row) == (i == 0 ? 0 : 2),
            "Crypt fixture entrypoint IDs differ from its authored 0/2 pair");
        const auto* end = static_cast<const std::uint8_t*>(std::memchr(row + 8, 0, 64));
        require(end && std::string(reinterpret_cast<const char*>(row + 8), end - (row + 8)) ==
            source.name, "SpawnPoint source name differs");

        for (unsigned axis = 0; axis < 3; ++axis) {
            const float module_origin = expected.modules[source.module_index].record.local.position[axis];
            const float expected_values[6]{
                source.local.position[axis],
                source.local.rotation_degrees[axis],
                source.local.scale[axis],
                source.local.position[axis] + module_origin,
                source.local.rotation_degrees[axis],
                source.local.scale[axis],
            };
            for (unsigned group = 0; group < 6; ++group)
                require(close(read_float(row + 72 + (group * 3 + axis) * 4),
                    expected_values[group]), "local/world SpawnPoint transform differs");
        }
    }
    const auto* later = bytes.data() + 16 + 144;
    require(read_u32(later + 4) > 0 &&
        !close(read_float(later + 72 + 28 * 4), read_float(later + 72 + 1 * 4)),
        "non-origin Crypt Module translation was not applied to world position");
}

} // namespace

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "usage: crypt_generated_spawnpoints_v1 <recovered-crypt-files-root>\n";
        return 2;
    }
    const std::filesystem::path files_root = argv[1];
    const auto level_path = files_root / "x07_crypt_backup.mlx";
    const auto level = read_text(level_path);
    constexpr const char* level_source_path = "data/scene/x07_crypt_backup.mlx";

    SourceLevel expected{};
    dh2::world::Diagnostic diagnostic{};
    require(dh2_world_import_level(&expected, "GOTHICUS_CRYPT_01",
        level_source_path, reinterpret_cast<const std::uint8_t*>(level.data()),
        level.size(), &diagnostic) == dh2::world::Error::ok, diagnostic.message);
    require(expected.module_count > 0, "Crypt level contains no Modules");

    std::vector<std::string> mgp_bytes(expected.module_count);
    std::vector<GeneratedMgpView> views(expected.module_count);
    for (std::uint32_t i = 0; i < expected.module_count; ++i) {
        const auto* source_path = field(expected.modules[i].record, "mgp");
        require(source_path != nullptr, "Crypt Module lacks MGP reference");
        mgp_bytes[i] = read_text(files_root / filename_from(source_path));
        views[i] = {source_path,
            reinterpret_cast<const std::uint8_t*>(mgp_bytes[i].data()),
            mgp_bytes[i].size()};
    }
    import_expected(expected, views);

    std::vector<std::uint8_t> output;
    std::string error;
    require(dh2::world::compile_generated_spawnpoints_v1(
        reinterpret_cast<const std::uint8_t*>(level.data()), level.size(),
        "GOTHICUS_CRYPT_01", level_source_path, views.data(), views.size(),
        output, error), error.c_str());
    compare(output, expected);
    std::vector<std::uint8_t> repeated;
    require(dh2::world::compile_generated_spawnpoints_v1(
        reinterpret_cast<const std::uint8_t*>(level.data()), level.size(),
        "GOTHICUS_CRYPT_01", level_source_path, views.data(), views.size(),
        repeated, error) && repeated == output, "SPWN output is not deterministic");

    // The actual Crypt points are unconditional, script-free and unique. Keep
    // behavior outside that evidenced subset explicit: this adapter must not
    // silently erase a condition or collapse a duplicate identity.
    auto conditional = mgp_bytes;
    const auto source_spawn = spawn_record_text(conditional.front());
    const auto source_offset = conditional.front().find(source_spawn);
    const auto marker = source_spawn.find("gametype=\"SpawnPoint\"");
    require(source_offset != std::string::npos && marker != std::string::npos,
        "cannot mutate source SpawnPoint fixture");
    conditional.front().insert(source_offset + marker,
        "activate_cond=\"unresolved_story_flag\" ");
    auto conditional_views = views;
    conditional_views.front().data = reinterpret_cast<const std::uint8_t*>(conditional.front().data());
    conditional_views.front().size = conditional.front().size();
    const bool conditional_accepted = dh2::world::compile_generated_spawnpoints_v1(
        reinterpret_cast<const std::uint8_t*>(level.data()), level.size(),
        "GOTHICUS_CRYPT_01", level_source_path, conditional_views.data(),
        conditional_views.size(), repeated, error);
    require(!conditional_accepted && repeated.empty() &&
        error.find("activation conditions") != std::string::npos,
        "conditional SpawnPoint was treated as unconditional");

    auto duplicate = mgp_bytes;
    auto later_spawn = source_spawn;
    const auto position_field = later_spawn.find("position=\"");
    require(position_field != std::string::npos,
        "cannot find duplicate SpawnPoint position field");
    const auto position_value = position_field + std::strlen("position=\"");
    const auto position_end = later_spawn.find('"', position_value);
    require(position_end != std::string::npos,
        "cannot terminate duplicate SpawnPoint position field");
    later_spawn.replace(position_value, position_end - position_value,
        "123.0,456.0,7.0");
    duplicate.front().insert(duplicate.front().rfind("</Module>"), later_spawn);
    auto duplicate_views = views;
    duplicate_views.front().data = reinterpret_cast<const std::uint8_t*>(duplicate.front().data());
    duplicate_views.front().size = duplicate.front().size();
    require(!dh2::world::compile_generated_spawnpoints_v1(
        reinterpret_cast<const std::uint8_t*>(level.data()), level.size(),
        "GOTHICUS_CRYPT_01", level_source_path, duplicate_views.data(),
        duplicate_views.size(), repeated, error) && repeated.empty() &&
        error.find("duplicate entrypoint IDs") != std::string::npos,
        "duplicate SpawnPoint identity was accepted");

    require(dh2::world::compile_generated_spawnpoint_v1(
        reinterpret_cast<const std::uint8_t*>(level.data()), level.size(),
        "GOTHICUS_CRYPT_01", level_source_path, duplicate_views.data(),
        duplicate_views.size(), 0, repeated, error), error.c_str());
    require(repeated.size() == 16 + 144 && read_i32(repeated.data() + 16) == 0,
        "selected SpawnPoint compiler did not filter to the requested ID");
    const auto selected_x = read_float(repeated.data() + 16 + 72 + 9 * 4);
    require(close(selected_x, 123.0f + expected.modules[0].record.local.position[0]),
        "selected duplicate SpawnPoint did not preserve source last-match order");

    std::cout << "Crypt generated SpawnPoint v1: " << expected.module_count
        << " source Modules, 2 authored entrypoints, order/transforms verified; "
        << "selected duplicate IDs use the last source transform; other conditions fail closed\n";
    dh2_world_free(&expected);
    return 0;
}
