#include "../world.hpp"

#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

namespace {
std::vector<std::uint8_t> read(const char* path) {
    std::ifstream file(path, std::ios::binary);
    return {std::istreambuf_iterator<char>(file), {}};
}

bool close(float actual, float expected, float tolerance = 0.02f) {
    return std::isfinite(actual) && std::fabs(actual - expected) <= tolerance;
}

int fail(const char* message, const std::string& detail = {}) {
    std::cerr << message;
    if (!detail.empty()) std::cerr << ": " << detail;
    std::cerr << '\n';
    return 1;
}
}

int main(int argc, char** argv) {
    if (argc != 4) return fail("usage: world_spawnpoints_audit <crypt.bdae> <crypt01.dwld> <crypt01.spwn>");
    const auto bres_bytes = read(argv[1]);
    const auto descriptor = read(argv[2]);
    const auto sidecar = read(argv[3]);
    dh2::resources::BresView bres{};
    if (dh2_bres_open(&bres, bres_bytes.data(), bres_bytes.size()) !=
        dh2::resources::BresError::ok) return fail("Crypt BRES rejected");

    dh2::world::Level level;
    std::string error;
    if (!dh2::world::load(bres, descriptor.data(), descriptor.size(), level, error))
        return fail("DWLD v1 rejected", error);

    std::vector<dh2::world::EntryPoint> entries;
    if (!dh2::world::load_entrypoints(sidecar.data(), sidecar.size(), level.rooms,
                                      entries, error))
        return fail("SPWN sidecar rejected", error);
    if (entries.size() != 2) return fail("unexpected Crypt entrypoint count");

    const auto& from_dw = entries[0];
    const auto& from_gothicus = entries[1];
    if (from_dw.id != 0 || from_dw.room != 0 || from_dw.name != "_prim_EntryPoint_fromDW" ||
        !close(from_dw.local.position[0], -2227.77f) ||
        !close(from_dw.local.position[1], 1220.93f) ||
        !close(from_dw.local.position[2], 812.557f) ||
        from_dw.world.position != from_dw.local.position ||
        from_dw.world.rotation_degrees != from_dw.local.rotation_degrees ||
        from_dw.world.scale != from_dw.local.scale)
        return fail("entrypoint 0 MGP transform differs");
    if (from_gothicus.id != 2 || from_gothicus.room != 7 ||
        from_gothicus.name != "_prim_EntryPoint_fromGothicus2" ||
        !close(from_gothicus.local.position[0], 0.0f) ||
        !close(from_gothicus.local.position[1], 4044.78f) ||
        !close(from_gothicus.local.position[2], 658.711f) ||
        !close(from_gothicus.world.position[0], 0.0f) ||
        !close(from_gothicus.world.position[1], 23244.78f) ||
        !close(from_gothicus.world.position[2], 658.711f))
        return fail("entrypoint 2 MGP/local-to-world transform differs");

    dh2::world::SpawnSelection first;
    if (!dh2::world::select_entrypoint(level, entries, 0, first, error))
        return fail("entrypoint 0 selection failed", error);
    if (first.source.id != 0 || first.rotation_degrees != from_dw.world.rotation_degrees ||
        !first.floor_snapped || first.position != level.spawn ||
        !close(first.position[2], 842.3644f))
        return fail("entrypoint 0 source floor snap differs");

    dh2::world::SpawnSelection second;
    if (!dh2::world::select_entrypoint(level, entries, 2, second, error))
        return fail("entrypoint 2 selection failed", error);
    if (second.source.id != 2 || second.rotation_degrees != from_gothicus.world.rotation_degrees ||
        second.position[0] != 0.0f || !close(second.position[1], 23244.78f) ||
        !second.floor_snapped || !close(second.position[2], 650.571f))
        return fail("entrypoint 2 source transform differs");

    dh2::world::SpawnSelection missing;
    if (dh2::world::select_entrypoint(level, entries, 1, missing, error) || error.empty())
        return fail("unknown entrypoint ID must fail explicitly");

    auto duplicate = sidecar;
    duplicate[16 + 144] = duplicate[16];
    duplicate[16 + 144 + 1] = duplicate[17];
    duplicate[16 + 144 + 2] = duplicate[18];
    duplicate[16 + 144 + 3] = duplicate[19];
    if (dh2::world::load_entrypoints(duplicate.data(), duplicate.size(), level.rooms,
                                     entries, error) || error.empty())
        return fail("duplicate entrypoint IDs must be rejected");

    std::cout << "{\"entries\":2,\"id0_floor_snapped\":true,\"id2_floor_snapped\":"
              << (second.floor_snapped ? "true" : "false")
              << ",\"id2_world_y\":" << second.position[1]
              << ",\"id2_selected_z\":" << second.position[2]
              << ",\"dwld_v1_unchanged\":true}\n";
    return 0;
}
