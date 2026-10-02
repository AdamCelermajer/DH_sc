#include "../pose.hpp"
#include <cassert>
#include <cstdio>
#include <fstream>
#include <iterator>
#include <vector>

static std::uint32_t seed = 20261002;
static std::uint32_t next() {
    seed ^= seed << 13; seed ^= seed >> 17; seed ^= seed << 5; return seed;
}
static std::vector<std::uint8_t> read(const char* path) {
    std::ifstream file(path, std::ios::binary);
    return {(std::istreambuf_iterator<char>(file)), {}};
}
int main(int argc, char** argv) {
    assert(argc == 3);
    const auto original = read(argv[1]), model = read(argv[2]);
    dh2::resources::BresView model_view{};
    assert(dh2_bres_open(&model_view, model.data(), model.size()) == dh2::resources::BresError::ok);
    dh2::skin::Skin skin{};
    assert(dh2_skin_open(&skin, &model_view, 0) == dh2::skin::Error::ok);
    dh2::scene::Scene scene{};
    assert(dh2_scene_open(&scene, &model_view) == dh2::scene::Error::ok);
    dh2::scene::Visual visual{};
    assert(dh2_scene_visual(&scene, 0, &visual) == dh2::scene::Error::ok);
    for (std::uint32_t i = 0; i < 3000; ++i) {
        auto bytes = original;
        if (i % 3 == 0) bytes.resize(next() % bytes.size());
        else if (i % 3 == 1) {
            for (std::uint32_t j = 0, n = 1 + next() % 12; j < n; ++j)
                bytes[next() % bytes.size()] = static_cast<std::uint8_t>(next());
        }
        const auto before = bytes;
        dh2::resources::BresView view{};
        if (dh2_bres_open(&view, bytes.data(), bytes.size()) != dh2::resources::BresError::ok) continue;
        dh2::pose::Clip clip{};
        if (dh2_pose_clip_open(&clip, &view, 0) != dh2::pose::Error::ok) {
            assert(clip.count == 0); continue;
        }
        assert(clip.count <= 128);
        for (std::uint32_t j = 0; j < clip.count; ++j) {
            float out[4]{};
            assert(dh2_pose_sample(&clip, j, -1000, out) == dh2::pose::Error::ok);
            assert(dh2_pose_sample(&clip, j, 1000000, out) == dh2::pose::Error::ok);
        }
        dh2::math::Matrix4f palette[256]{};
        // Corrupted but structurally valid animation may have absent targets
        // or values too large for a finite world; those must fail safely.
        dh2_pose_skin_palette(&clip, clip.end / 2, &skin, &visual, palette, 256);
        assert(dh2_pose_sample(&clip, clip.count, 0, nullptr) != dh2::pose::Error::ok);
        assert(bytes == before);
    }
    std::puts("pose safety: 3000 corruption/truncation cases with sampling and skeleton traversal");
}
