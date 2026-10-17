#include "../scene_buffers.hpp"
#include "../../animation-pose/pose.hpp"
#include "../../engine-resources/resources.hpp"
#include "../../skin-payloads/skin.hpp"

#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

namespace {
struct Model {
    const char* path;
    const char* controller;
};
constexpr Model models[] = {
    {"data/3d/characters/infected/infected.bdae", "zombie02-mesh-skin"},
    {"data/3d/characters/infected/burned.bdae", "_mesh_burned-mesh-skin"},
    {"data/3d/characters/infected/inf_blacksmith.bdae", "blacksmith-mesh-skin"},
    {"data/3d/characters/infected/inf_maid.bdae", "_mesh_castle_worker02-mesh-skin"},
    {"data/3d/characters/infected/inf_merchant.bdae", "_mesh_merchant_skinned_batch-mesh-skin"},
    {"data/3d/characters/infected/inf_nun.bdae", "_mesh_nun-mesh-skin"},
};
constexpr const char* clips[] = {
    "data/3d/characters/infected/animations/infected_idle.bdae",
    "data/3d/characters/infected/animations/infected_idle_02.bdae",
    "data/3d/characters/infected/animations/infected_walk.bdae",
};
constexpr const char* actor_diffuse_path =
    "q:/data/iphone/3d/textures/atlas_skinned_characters_animdecor_gameobjects_002.tga";

void require(bool condition, const char* message) {
    if (condition) return;
    std::fprintf(stderr, "Infected actor skin mesh check failed: %s\n", message);
    std::exit(1);
}

std::vector<std::uint8_t> read_file(const std::string& path) {
    std::ifstream input(path, std::ios::binary);
    require(static_cast<bool>(input), path.c_str());
    return {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
}

void check_mesh(const dh2::viewer::SceneMesh& mesh, const char* model,
                const char* clip, std::int32_t time) {
    require(mesh.skin_joints == 20, model);
    require(mesh.draw_commands == 1 && mesh.vertex_count > 0 && mesh.index_count > 0, model);
    std::uint32_t visible_draws = 0, diffuse_samplers = 0;
    for (std::uint32_t i = 0; i < mesh.vertex_count; ++i)
        for (unsigned axis = 0; axis < 5; ++axis)
            require(std::isfinite(mesh.vertices[5U * i + axis]), model);
    for (std::uint32_t i = 0; i < mesh.index_count; ++i)
        require(mesh.indices[i] < mesh.vertex_count, model);
    for (std::uint32_t i = 0; i < mesh.draw_commands; ++i) {
        const auto& draw = mesh.draws[i];
        require(draw.first_vertex + draw.vertex_count <= mesh.vertex_count &&
                draw.first_index + draw.index_count <= mesh.index_count, model);
        if (draw.visible) ++visible_draws;
        require(draw.visible && draw.texture_count == 1 &&
                draw.first_texture < mesh.texture_reference_count, model);
        for (std::uint32_t j = 0; j < draw.texture_count; ++j) {
            const auto& texture = mesh.texture_references[draw.first_texture + j];
            require(texture.image_index == 0 &&
                    std::string(texture.parameter_id) == "diffuse-sampler" &&
                    std::string(texture.source_path) == actor_diffuse_path,
                    "selected skin controller must bind its source diffuse atlas");
            ++diffuse_samplers;
        }
    }
    require(visible_draws > 0, model);
    require(diffuse_samplers == 1, "source diffuse-sampler reference is present");
    (void)clip;
    (void)time;
}
}

int main(int argc, char** argv) {
    require(argc == 2, "usage: infected_actor_skin_mesh_host <cache-root>");
    const std::string cache = argv[1];
    std::uint32_t meshes_checked = 0;
    for (const auto& model_source : models) {
        auto model_bytes = read_file(cache + "/" + model_source.path);
        dh2::resources::BresView model{};
        require(dh2_bres_open(&model, model_bytes.data(), model_bytes.size()) ==
                    dh2::resources::BresError::ok, model_source.path);
        dh2::skin::Skin skin{};
        require(dh2_skin_open(&skin, &model, 0) == dh2::skin::Error::ok &&
                skin.id && std::string(skin.id) == model_source.controller && skin.joints == 20,
                model_source.controller);
        for (const auto* clip_path : clips) {
            auto clip_bytes = read_file(cache + "/" + clip_path);
            dh2::resources::BresView animation{};
            require(dh2_bres_open(&animation, clip_bytes.data(), clip_bytes.size()) ==
                        dh2::resources::BresError::ok, clip_path);
            dh2::pose::Clip clip{};
            require(dh2_pose_clip_open(&clip, &animation, 0) == dh2::pose::Error::ok &&
                    clip.end > clip.start, clip_path);
            const std::int32_t times[] = {
                clip.start,
                static_cast<std::int32_t>(std::int64_t(clip.start) +
                                          (std::int64_t(clip.end) - clip.start) / 2),
                clip.end - 1,
            };
            for (const auto time : times) {
                dh2::viewer::SceneMesh mesh{};
                const auto result = dh2_world_scene_skin_mesh_at(
                    &mesh, &model, &clip, time, model_source.controller);
                require(result == dh2::viewer::SceneMeshError::ok, model_source.path);
                check_mesh(mesh, model_source.path, clip_path, time);
                if (meshes_checked % 9 == 0) {
                    for (std::uint32_t draw_index = 0; draw_index < mesh.draw_commands; ++draw_index) {
                        const auto& draw = mesh.draws[draw_index];
                        std::printf("first_actor_draw material=%s visible=%u textures=%u\n",
                            draw.material_id, draw.visible, draw.texture_count);
                        for (std::uint32_t ref = 0; ref < draw.texture_count; ++ref) {
                            const auto& texture = mesh.texture_references[draw.first_texture + ref];
                            std::printf("  sampler=%s image=%s index=%d path=%s\n",
                                texture.parameter_id, texture.image_id, texture.image_index,
                                texture.source_path);
                        }
                    }
                }
                dh2_viewer_scene_mesh_free(&mesh);
                ++meshes_checked;
            }
        }
    }
    std::printf("pass=true models=%zu clips=%zu pose_samples_per_pair=3 skin_meshes=%u\n",
                sizeof(models) / sizeof(models[0]), sizeof(clips) / sizeof(clips[0]),
                meshes_checked);
    return 0;
}
