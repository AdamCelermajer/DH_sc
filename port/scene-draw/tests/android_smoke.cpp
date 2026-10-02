// Source-only Android command-line check; BRES bytes are supplied at runtime.
#include "../draw.hpp"

#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>

namespace {
struct Observed {
    std::uint32_t commands;
    std::uint64_t triangles;
    bool valid;
};

bool visit(const dh2::draw::Command* draw, void* user) {
    auto& observed = *static_cast<Observed*>(user);
    if (!draw || !draw->node_id || !draw->geometry_id || !draw->material_id ||
        draw->geometry_index < 0 || draw->primitive_index < 0 ||
        !draw->vertex_count || !draw->index_count || draw->index_count % 3 ||
        (draw->index_width != 2 && draw->index_width != 4)) {
        observed.valid = false;
        return false;
    }
    for (float component : draw->world.m)
        if (!std::isfinite(component)) {
            observed.valid = false;
            return false;
        }
    if (draw->world.m[3] != 0 || draw->world.m[7] != 0 ||
        draw->world.m[11] != 0 || draw->world.m[15] != 1) {
        observed.valid = false;
        return false;
    }
    ++observed.commands;
    observed.triangles += draw->index_count / 3;
    return true;
}
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::fprintf(stderr, "usage: %s private_scene.bdae\n", argv[0]);
        return 2;
    }
    std::FILE* file = std::fopen(argv[1], "rb");
    if (!file) return 3;
    if (std::fseek(file, 0, SEEK_END) != 0) { std::fclose(file); return 4; }
    const long count = std::ftell(file);
    if (count <= 0 || count > 32L * 1024 * 1024 ||
        std::fseek(file, 0, SEEK_SET) != 0) { std::fclose(file); return 4; }
    auto* bytes = static_cast<std::uint8_t*>(std::malloc(static_cast<std::size_t>(count)));
    if (!bytes) { std::fclose(file); return 5; }
    const auto read = std::fread(bytes, 1, static_cast<std::size_t>(count), file);
    std::fclose(file);
    if (read != static_cast<std::size_t>(count)) { std::free(bytes); return 6; }
    dh2::resources::BresView image{};
    if (dh2_bres_open(&image, bytes, static_cast<std::size_t>(count))
        != dh2::resources::BresError::ok) { std::free(bytes); return 7; }
    dh2::draw::Stats stats{};
    Observed observed{0, 0, true};
    const auto result = dh2_static_scene_draws(&stats, &image, visit, &observed,
                                                100000, 100000);
    const bool okay = result == dh2::draw::Error::ok && observed.valid &&
        stats.nodes > 0 && stats.draw_commands > 0 &&
        observed.commands == stats.draw_commands &&
        observed.triangles == stats.triangles;
    if (okay)
        std::printf("scene draw: nodes=%u commands=%u triangles=%llu unresolved_geometry=%u unresolved_materials=%u\n",
                    stats.nodes, stats.draw_commands,
                    static_cast<unsigned long long>(stats.triangles),
                    stats.skipped_unresolved_geometry, stats.unresolved_materials);
    else
        std::fprintf(stderr, "scene draw failed: error=%u nodes=%u commands=%u\n",
                     static_cast<unsigned>(result), stats.nodes, stats.draw_commands);
    std::free(bytes);
    return okay ? 0 : 8;
}
