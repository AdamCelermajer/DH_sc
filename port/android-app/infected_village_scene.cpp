#include "infected_village_scene.hpp"

#include "../scene-payloads/scene.hpp"
#include "../engine-resources/resources.hpp"
#include "../engine-math/math.hpp"
#include "../world-data/world_scene.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace dh2::infectedpreview {
namespace {
constexpr const char* level_path = "data/scene/005_infectedvillage.mlx";
constexpr const char* catalogue_path = "data/3d/modules/infectedvillage/infectedvillage.bdae";
constexpr const char* mgp_paths[module_count] = {
    "data/3d/modules/infectedvillage/mgp/infected01.mgp",
    "data/3d/modules/infectedvillage/mgp/infected02.mgp"};
constexpr const char* mvp_paths[module_count] = {
    "data/3d/modules/infectedvillage/mvp/infected01.mvp",
    "data/3d/modules/infectedvillage/mvp/infected02.mvp"};

Error fail(Diagnostic* diagnostic, Preview* preview, Error error, const char* message) {
    if (preview) dh2::infectedpreview::free(preview);
    if (diagnostic) {
        *diagnostic = {};
        diagnostic->error = error;
        std::snprintf(diagnostic->message, sizeof(diagnostic->message), "%s", message);
    }
    return error;
}

bool path_is(const char* actual, const char* expected) {
    return actual && std::strcmp(actual, expected) == 0;
}

} // namespace

void free(Preview* preview) {
    if (!preview) return;
    for (auto& mesh : preview->modules) dh2_viewer_scene_mesh_free(&mesh);
    dh2_world_free(&preview->level);
    *preview = {};
}

Error load(Preview* output, const SourceFiles* files, Diagnostic* diagnostic) {
    if (!output)
        return fail(diagnostic, nullptr, Error::argument, "Preview output is required");
    // Replace an already loaded Preview safely; first-call callers must pass
    // Preview{} as documented in the public header.
    free(output);
    if (!files || !files->mlx || !files->catalogue ||
        !files->mlx_size || !files->catalogue_size)
        return fail(diagnostic, nullptr, Error::argument, "Missing Infected Village source inputs");
    for (std::uint32_t i = 0; i < module_count; ++i)
        if (!files->mgp[i] || !files->mgp_size[i] || !files->mvp[i] || !files->mvp_size[i])
            return fail(diagnostic, nullptr, Error::argument, "Both module MGP/MVP pairs are required");

    Preview candidate{};
    dh2::world::Diagnostic world_diagnostic{};
    auto result = dh2_world_import_level(&candidate.level, "INFECTED_VILLAGE_01",
        level_path, files->mlx, files->mlx_size, &world_diagnostic);
    if (result != dh2::world::Error::ok)
        return fail(diagnostic, &candidate, Error::level_import,
                    world_diagnostic.message[0] ? world_diagnostic.message : "Infected Village MLX rejected");
    if (candidate.level.module_count != module_count)
        return fail(diagnostic, &candidate, Error::source_paths,
                    "Infected Village source no longer has exactly two modules");

    for (std::uint32_t i = 0; i < module_count; ++i) {
        auto& module = candidate.level.modules[i];
        if (!path_is(module.cache_dae, catalogue_path) ||
            !path_is(module.cache_mgp, mgp_paths[i]) ||
            !path_is(module.cache_mvp, mvp_paths[i]))
            return fail(diagnostic, &candidate, Error::source_paths,
                        "MLX module paths differ from the checked source manifest");
        result = dh2_world_import_module_objects(&candidate.level, i,
            dh2::world::RecordKind::mgp, module.cache_mgp,
            files->mgp[i], files->mgp_size[i], &world_diagnostic);
        if (result != dh2::world::Error::ok)
            return fail(diagnostic, &candidate, Error::module_import,
                        world_diagnostic.message[0] ? world_diagnostic.message : "Module MGP rejected");
        result = dh2_world_import_module_objects(&candidate.level, i,
            dh2::world::RecordKind::mvp, module.cache_mvp,
            files->mvp[i], files->mvp_size[i], &world_diagnostic);
        if (result != dh2::world::Error::ok)
            return fail(diagnostic, &candidate, Error::module_import,
                        world_diagnostic.message[0] ? world_diagnostic.message : "Module MVP rejected");
    }

    dh2::resources::BresView bres{};
    if (dh2_bres_open(&bres, files->catalogue, files->catalogue_size) !=
        dh2::resources::BresError::ok)
        return fail(diagnostic, &candidate, Error::bres_open,
                    "Infected Village catalogue BRES rejected");
    dh2::scene_payload::Scene scene{};
    if (dh2_scene_open(&scene, &bres) != dh2::scene_payload::Error::ok)
        return fail(diagnostic, &candidate, Error::scene_open,
                    "Infected Village catalogue scene rejected");

    std::uint32_t* records = static_cast<std::uint32_t*>(
        std::malloc(65536U * sizeof(std::uint32_t)));
    if (!records)
        return fail(diagnostic, &candidate, Error::allocation,
                    "Could not allocate bounded module subtree records");
    for (std::uint32_t i = 0; i < module_count; ++i) {
        dh2::world::ModuleBinding binding{};
        if (dh2_world_bind_module(&binding, &candidate.level.modules[i], &scene,
                &world_diagnostic) != dh2::world::Error::ok) {
            std::free(records);
            return fail(diagnostic, &candidate, Error::module_bind,
                world_diagnostic.message[0] ? world_diagnostic.message : "Module root bind failed");
        }
        std::uint32_t count = 0;
        if (dh2_world_module_records(records, 65536, &count, &binding, &scene,
                &world_diagnostic) != dh2::world::Error::ok) {
            std::free(records);
            return fail(diagnostic, &candidate, Error::subtree,
                world_diagnostic.message[0] ? world_diagnostic.message : "Module subtree walk failed");
        }
        dh2::math::Matrix4f correction{};
        if (dh2_world_placement_matrix(&correction, &binding, &world_diagnostic) !=
                dh2::world::Error::ok ||
            dh2_world_scene_mesh_nodes(&candidate.modules[i], &bres, records,
                count, &correction) != dh2::viewer::SceneMeshError::ok) {
            std::free(records);
            return fail(diagnostic, &candidate, Error::geometry,
                        "Module source geometry could not be assembled at MLX placement");
        }
        const auto& mesh = candidate.modules[i];
        if (!mesh.draws || !mesh.vertices || !mesh.indices || !mesh.draw_commands ||
            !mesh.vertex_count || !mesh.index_count)
            { std::free(records); return fail(diagnostic, &candidate, Error::geometry,
                        "Module geometry is empty"); }
    }
    std::free(records);
    candidate.ready = true;
    *output = candidate;
    if (diagnostic) *diagnostic = {};
    return Error::ok;
}

} // namespace dh2::infectedpreview
