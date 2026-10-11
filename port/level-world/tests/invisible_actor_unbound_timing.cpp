#include "objects.hpp"

#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
std::vector<std::uint8_t> read(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) throw std::runtime_error("Cannot open " + path);
    return {std::istreambuf_iterator<char>(file), {}};
}
}

int main(int argc, char** argv) {
    if (argc != 2) return 2;
    try {
        const std::string root = argv[1];
        const auto model = read(root + "/actors/invisible_man.bdae");
        const auto clip = read(root + "/actors/prince_idle_shield.bdae");
        dh2::objects::Resource resource;
        std::string error;
        if (!dh2::objects::load_intentional_empty_character_visual(
                model.data(), model.size(), clip.data(), clip.size(), resource,
                error))
            throw std::runtime_error(error);
        if (!resource.intentional_empty_visual || !resource.primitives.empty() ||
            resource.scene.graph.size() != 3)
            throw std::runtime_error("InvisibleMan source scene signature changed");
        if (resource.animation.track_count() != 0 ||
            resource.animation.unbound != 23 || resource.animation.skipped != 0 ||
            resource.animation.start != 0 || resource.animation.end != 1066)
            throw std::runtime_error("InvisibleMan initial clip did not retain source timing");

        // The renderer's actor clip bank loads the same bytes against the
        // retained rest scene, independently from the model's initial clip.
        dh2::animation::Player bank_clip;
        if (!bank_clip.load(clip.data(), clip.size(), resource.rest_scene, error,
                            dh2::animation::MissingTargets::ignore))
            throw std::runtime_error(error);
        if (bank_clip.track_count() != 0 || bank_clip.unbound != 23 ||
            bank_clip.skipped != 0 || bank_clip.segment_count() != 1 ||
            bank_clip.start != 0 || bank_clip.end != 1066)
            throw std::runtime_error("InvisibleMan all-unbound clip bank timing differs");

        const auto particle_bdae = read(root + "/data/3d/interface/marker_poisoned.bdae");
        dh2::scene::Scene particle_scene;
        dh2::animation::Player particle_timeline;
        if (!particle_timeline.load(particle_bdae.data(), particle_bdae.size(),
                                    particle_scene, error,
                                    dh2::animation::MissingTargets::ignore))
            throw std::runtime_error(error);
        if (particle_timeline.track_count() != 0 ||
            particle_timeline.segment_count() != 1 ||
            particle_timeline.start != 0 || particle_timeline.end != 1000)
            throw std::runtime_error("Emitter-only BDAE lost its source timeline interval");

        std::cout << "{\"validation\":\"PASS\",\"model\":\"invisible_man.bdae\","
                     "\"clip\":\"prince_idle_shield.bdae\",\"tracks\":0,"
                     "\"unbound\":23,\"start_ms\":0,\"end_ms\":1066,"
                     "\"emitter_only_timeline\":\"marker_poisoned 0..1000 ms\"}\n";
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << exception.what() << '\n';
        return 1;
    }
}
