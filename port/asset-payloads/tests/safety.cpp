#include "../payloads.hpp"
#include <cassert>
#include <cstdio>
#include <fstream>
#include <iterator>
#include <vector>

using namespace dh2::assets;
using namespace dh2::resources;
static void inspect(std::vector<std::uint8_t>& bytes, std::size_t size) {
    BresView image{};
    if (dh2_bres_open(&image, bytes.data(), size) != BresError::ok) return;
    const auto n = dh2_bres_library_count(&image, Library::geometry);
    for (std::uint32_t i = 0; i < n; ++i) {
        Mesh mesh{};
        if (dh2_mesh_open(&mesh, &image, i) != Error::ok) {
            Type1Geometry type1{};
            if (dh2_type1_geometry_open(&type1, &image, i) != Error::ok) continue;
            mesh = type1.embedded_mesh;
        }
        for (std::uint32_t j = 0; j < mesh.attributes; ++j) {
            Attribute a{}; assert(dh2_mesh_attribute(&mesh, j, &a) == Error::ok);
            float f[16]; if (a.vertices) assert(dh2_attribute_read(&a, a.vertices - 1, f));
            assert(!dh2_attribute_read(&a, a.vertices, f));
        }
    }
    const auto animations = dh2_bres_library_count(&image, Library::animation);
    const auto segments = dh2_animation_segments(&image);
    for (std::uint32_t i = 0; i < animations; ++i) for (std::uint32_t j = 0; j < segments; ++j) {
        Animation a{};
        if (dh2_animation_open(&a, &image, i, j) != Error::ok) continue;
        for (std::uint32_t s = 0; s < dh2_animation_samplers(&a); ++s) {
            Vector t{}, v{};
            assert(dh2_animation_vector(&a, s, false, &t));
            assert(dh2_animation_vector(&a, s, true, &v));
            float f[16];
            if (v.count) assert(dh2_vector_read(&v, v.count - 1, f));
            assert(!dh2_vector_read(&v, v.count, f));
            std::int32_t key = -1; float fraction = -1;
            dh2_animation_find(&a, s, 100, &key, &fraction);
            dh2_animation_key_time(&a, s, std::int32_t(t.count) - 1);
        }
    }
}
int main(int argc, char** argv) {
    assert(argc == 2);
    std::ifstream stream(argv[1], std::ios::binary);
    std::vector<std::uint8_t> input((std::istreambuf_iterator<char>(stream)), {});
    assert(input.size() > 256);
    inspect(input, input.size());
    std::uint32_t state = 0x0d220026;
    auto next = [&]() { state ^= state << 13; state ^= state >> 17; state ^= state << 5; return state; };
    for (std::uint32_t i = 0; i < 5000; ++i) {
        auto probe = input;
        if (i % 4 == 0) inspect(probe, next() % probe.size());
        else {
            // Deep mutations after the outer header reach nested schema checks.
            const auto p = 60 + next() % (probe.size() - 60);
            for (std::size_t k = 0; k < 4 && p + k < probe.size(); ++k) probe[p + k] = next();
            inspect(probe, probe.size());
        }
    }
    std::puts("5,000 nested corruption/truncation probes passed (ASan/UBSan)");
}
