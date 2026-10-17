#include "../player_camera_rig_v1.hpp"
#include "../../game-data/animation_tables.hpp"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
std::vector<std::uint8_t> read(const char* path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) throw std::runtime_error(std::string("Cannot open ") + path);
    return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
}
std::uint32_t read_u32(const std::uint8_t* bytes) {
    return std::uint32_t(bytes[0]) | (std::uint32_t(bytes[1]) << 8) |
           (std::uint32_t(bytes[2]) << 16) | (std::uint32_t(bytes[3]) << 24);
}
std::string path_join(const std::string& root, const char* path) {
    return root + "/" + path;
}
std::string lower(std::string value) {
    for (auto& character : value) {
        const auto byte = static_cast<unsigned char>(character);
        character = static_cast<char>(std::tolower(byte));
    }
    return value;
}
float design_field(const std::vector<std::uint8_t>& bytes, std::size_t field) {
    if (bytes.size() < 4 + (field + 1) * 4) throw std::runtime_error("DesignSettings row truncated");
    const auto bits = read_u32(bytes.data() + 4 + field * 4);
    float value{};
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}
void require(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}
bool near(float a, float b, float epsilon = 0.02f) {
    return std::abs(a - b) <= epsilon;
}
}

int main(int argc, char** argv) {
    try {
        if (argc != 4) {
            std::cerr << "usage: swamp_camera_tests_rig_v1_audit cameratests.bdae camera_idle.bdae assets-root\n";
            return 2;
        }
        const auto camera = read(argv[1]);
        const auto idle = read(argv[2]);
        const std::string assets_root = argv[3];
        dh2::player_camera_rig_v1::Rig rig;
        std::string error;
        require(rig.load({camera.data(), camera.size(), idle.data(), idle.size()}, error), error.c_str());

        const auto& projection = rig.projection();
        require(near(projection.source_fov_value, 45.0f), "CameraTests BRES FOV changed");
        require(near(projection.aspect_ratio, 1.5f), "CameraTests BRES aspect changed");
        require(near(projection.near_clip, 600.0f), "CameraTests BRES near clip changed");
        require(near(projection.far_clip, 3800.0f), "CameraTests BRES far clip changed");
        require(rig.animation_start() == 0 && rig.animation_end() == 33,
                "SWAMP camera idle animation range changed");
        require(rig.track_count() == 6 && !rig.skipped_tracks() && !rig.unbound_tracks(),
                "SWAMP camera idle tracks are not fully supported and bound");

        dh2::player_camera_rig_v1::Pose first{}, middle{};
        require(rig.sample(rig.animation_start(), &first, error), error.c_str());
        require(rig.sample(rig.animation_start() +
                    (rig.animation_end() - rig.animation_start()) / 2, &middle, error), error.c_str());
        for (const auto* matrix : {&first.camera, &first.target, &first.up_vector,
                                   &middle.camera, &middle.target, &middle.up_vector}) {
            for (float value : *matrix) require(std::isfinite(value), "CameraTests pose is nonfinite");
        }

        const float eye[3]{first.camera[12] - first.target[12],
                           first.camera[13] - first.target[13],
                           first.camera[14] - first.target[14]};
        require(near(eye[0], 1380.0f) && near(eye[1], -1380.0f) && near(eye[2], 2450.0f),
                "CameraTests initial eye offset differs from its authored scene nodes");
        float forward[3]{first.target[12] - first.camera[12],
                         first.target[13] - first.camera[13],
                         first.target[14] - first.camera[14]};
        float up[3]{first.up_vector[12] - first.camera[12],
                    first.up_vector[13] - first.camera[13],
                    first.up_vector[14] - first.camera[14]};
        float dot = 0.0f, forward_sq = 0.0f, up_sq = 0.0f;
        for (unsigned i = 0; i < 3; ++i) {
            dot += forward[i] * up[i];
            forward_sq += forward[i] * forward[i];
            up_sq += up[i] * up[i];
        }
        require(forward_sq > 0.0f && up_sq > 0.0f &&
                    std::abs(dot / std::sqrt(forward_sq * up_sq)) < 0.01f,
                "CameraTests authored up-vector is not orthogonal to its view direction");

        const auto dictionary_names = read(path_join(assets_root,
                "data/animations_dictionary_pyarraynames.bin").c_str());
        const auto dictionary_values = read(path_join(assets_root,
                "data/animations_dictionary_pyarray.bin").c_str());
        const auto animation_records = read(path_join(assets_root,
                "data/animations_pyarray.bin").c_str());
        const auto animation_names = read(path_join(assets_root,
                "data/animations_pyarraynames.bin").c_str());
        const auto animation_fields = read(path_join(assets_root,
                "data/animations_pystructnames.bin").c_str());
        dh2::data::Dictionary clips;
        dh2::data::AnimationTables tables;
        require(dh2::data::load_dictionary(
                    {dictionary_names.data(), dictionary_names.size()},
                    {dictionary_values.data(), dictionary_values.size()}, clips, error), error.c_str());
        require(dh2::data::load_animation_tables(
                    {animation_records.data(), animation_records.size()},
                    {animation_names.data(), animation_names.size()},
                    {animation_fields.data(), animation_fields.size()}, clips, tables, error), error.c_str());
        const auto selected = std::find(tables.camera_names.begin(), tables.camera_names.end(), "Default");
        require(selected == tables.camera_names.begin() && !tables.cameras.empty(),
                "SWAMP camera_animset Default no longer selects CamAnimSet row 0");
        require(std::find(tables.camera_names.begin(), tables.camera_names.end(),
                          "PlayerCamera_Default") == tables.camera_names.end(),
                "Camera node name was incorrectly treated as a CamAnimSet name");
        const auto idle_id = tables.cameras.front().idle;
        require(idle_id == 55 && static_cast<std::size_t>(idle_id) < clips.values.size() &&
                    lower(clips.values[static_cast<std::size_t>(idle_id)]) ==
                        "data/3d/camera/animations/common/camera_idle.bdae",
                "CamAnimSet Default idle no longer resolves to the tested camera_idle asset");

        const auto design = read(path_join(assets_root,
                "original-cache/data/pydata/design_pyarray.bin").c_str());
        require(design.size() >= 4 + 43 * 4 && read_u32(design.data()) == 1,
                "SWAMP DesignSettings row is missing or changed");
        const float mini_map_max = design_field(design, 17);
        const float mini_map_min = design_field(design, 18);
        const float zoom_max = design_field(design, 41);
        const float zoom_min = design_field(design, 42);
        require(near(mini_map_max, 0.5f) && near(mini_map_min, -1.5f) &&
                    near(zoom_max, 0.35f) && near(zoom_min, 0.0f),
                "SWAMP CameraLevel DesignSettings zoom bounds changed");

        std::cout << "PASS: SWAMP CameraTests BDAE/idle rig | tracks=" << rig.track_count()
                  << " | clip=" << rig.animation_start() << ".." << rig.animation_end()
                  << " | eye-offset=" << eye[0] << ',' << eye[1] << ',' << eye[2]
                  << " | CamAnimSet Default row=0 idle=" << idle_id
                  << " | zoom=" << zoom_min << ".." << zoom_max
                  << " alternate=" << mini_map_min << ".." << mini_map_max << '\n';
        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
