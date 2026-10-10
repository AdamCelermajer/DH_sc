#include "../../../engine-animation/animation.hpp"
#include "../../../engine-resources/resources.hpp"

#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
void check(bool ok, const std::string& message) {
    if (!ok) throw std::runtime_error(message);
}

std::vector<std::uint8_t> read_bytes(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    check(static_cast<bool>(in), "missing source animation: " + path);
    return {std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
}

void validate(const std::string& path) {
    const auto bytes = read_bytes(path);
    dh2::scene::Scene empty_scene;
    dh2::animation::Player player;
    std::string error;
    check(player.load(bytes.data(), bytes.size(), empty_scene, error,
                      dh2::animation::MissingTargets::ignore),
          path + ": native BDAE/animation decode failed: " + error);
    const auto& events = player.events.view();
    check(events.type == 1 || events.type == 2,
          path + ": unexpected decoded event-track type");
    check(events.count > 0, path + ": source animation has no decoded event entries");
    check(player.unbound > 0 || player.skipped > 0 || player.track_count() > 0,
          path + ": native decoder accepted no animation transform records");
    std::cout << path << ": parsed tracks=" << player.track_count()
              << " unbound=" << player.unbound
              << " skipped=" << player.skipped
              << " event_entries=" << events.count << '\n';
}
} // namespace

int main(int argc, char** argv) {
    try {
        check(argc == 2, "usage: audio_source_asset_provenance_v1_tests <source-cache-root>");
        const std::string animation_root = std::string(argv[1]) +
            "/data/3d/characters/prince/animations/";
        validate(animation_root + "skill_dh2_prince_rogue_quickness.bdae");
        validate(animation_root + "skill_dh2_prince_rogue_roundhouse.bdae");
        std::cout << "source animation provenance decode passed\n";
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << '\n';
        return 1;
    }
}
