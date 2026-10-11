#include "door_collision_geometry_v1.hpp"
#include "../../../level-world/decor_scene.hpp"

#include <cmath>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace dh::foundation;
using namespace dh::foundation::doors;

namespace {
void check(bool value, const std::string& message) {
    if (!value) throw std::runtime_error(message);
}
}

int main(int argc, char** argv) {
    try {
        check(argc == 2, "Supply isolated asset-stage root");
        AssetCatalog assets(argv[1]);
        const char* models[]{
            "data/3d/gameobjects/go_door_fire_red.bdae",
            "data/3d/gameobjects/go_et_root_door_ew.bdae",
            "data/3d/gameobjects/go_et_root_door_ns.bdae",
            "data/3d/gameobjects/go_gate_big.bdae",
            "data/3d/gameobjects/go_gate_small.bdae",
            "data/3d/gameobjects/go_swamp_door_ruins.bdae",
            "data/3d/gameobjects/go_swamp_door_ruins_fire.bdae",
        };
        const float probe_position[]{31.0f, -47.0f, 12.0f};
        const float probe_rotation[]{17.0f, -31.0f, 9.0f};
        std::string error;
        for (const auto* model : models) {
            CharacterVisual visual;
            check(visual.load_embedded_scene(assets, model, error), model + std::string(": ") + error);
            float position[3]{probe_position[0], probe_position[1], probe_position[2]};
            float rotation[3]{probe_rotation[0], probe_rotation[1], probe_rotation[2]};
            float scale[3]{1.25f, 0.75f, 1.1f};
            if (std::strstr(model, "go_swamp_door_ruins.bdae")) {
                position[0] = -588.488f; position[1] = -487.974f; position[2] = 73.5546f;
                rotation[0] = 0.0f; rotation[1] = 0.0f; rotation[2] = 0.0f;
                scale[0] = scale[1] = scale[2] = 1.0f;
            }
            if (std::strstr(model, "go_swamp_door_ruins_fire.bdae")) {
                position[0] = 31.5828f; position[1] = 1401.49f; position[2] = 206.584f;
                rotation[0] = 0.0f; rotation[1] = 0.0f; rotation[2] = -90.0f;
                scale[0] = scale[1] = scale[2] = 1.0f;
            }
            const auto model_bytes = assets.read(model);
            dh2::resources::BresView bres{};
            check(dh2_bres_open(&bres, model_bytes.data(), model_bytes.size()) == dh2::resources::BresError::ok,
                  model + std::string(": BRES open"));
            dh2::physical::DecorSceneMarker marker{};
            check(dh2::physical::decor_scene_marker(bres, *visual.retained_scene_borrow(), marker, error),
                  model + std::string(": ") + error);
            float bounds[6]{};
            check(source_door_apply_mesh_box_v1(assets, visual, position, rotation, scale, bounds, error),
                  model + std::string(": ") + error);
            for (const float value : bounds) check(std::isfinite(value), model + std::string(": nonfinite box"));
            check(bounds[0] < bounds[3] && bounds[1] < bounds[4] && bounds[2] < bounds[5],
                  model + std::string(": degenerate source box"));
            check(std::abs((bounds[0] + bounds[3]) * 0.5f - position[0]) < 0.001f &&
                  std::abs((bounds[1] + bounds[4]) * 0.5f - position[1]) < 0.001f &&
                  std::abs((bounds[2] + bounds[5]) * 0.5f - position[2]) < 0.001f,
                  model + std::string(": ApplyMeshBox lost authored owner position"));
            std::cout << model << " source ApplyMeshBox PASS marker=" << marker.found << " sizeXY="
                      << bounds[3] - bounds[0] << ',' << bounds[4] - bounds[1]
                      << " center=" << (bounds[0] + bounds[3]) * 0.5f << ','
                      << (bounds[1] + bounds[4]) * 0.5f << '\n';
        }
        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
