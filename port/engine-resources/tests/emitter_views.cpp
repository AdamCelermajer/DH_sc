#include "../resources.hpp"

#include <cassert>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

namespace {
using namespace dh2::resources;

struct Expected {
    const char* suffix;
    std::uint32_t emitter_type;
    std::int32_t max_particles;
    float birth_rate;
    float life;
    float target_size;
    float radius_length;
    float shape_width;
    float shape_height;
    bool width_present;
    bool height_present;
    VisualContentKind visual_kind;
};

bool ends_with(const std::string& value, const char* suffix) {
    const std::string tail(suffix);
    return value.size() >= tail.size()
        && value.compare(value.size() - tail.size(), tail.size(), tail) == 0;
}

void check_asset(const std::string& path, const Expected& expected) {
    std::ifstream input(path, std::ios::binary);
    assert(input.good());
    std::vector<std::uint8_t> bytes((std::istreambuf_iterator<char>(input)),
                                    std::istreambuf_iterator<char>());
    BresView view{};
    assert(dh2_bres_open(&view, bytes.data(), bytes.size()) == BresError::ok);
    assert(dh2_bres_library_count(&view, Library::emitter) == 1);

    Emitter emitter{};
    assert(dh2_bres_emitter(&view, 0, &emitter) == EmitterError::ok);
    assert(emitter.row_index == 0 && emitter.row_offset != 0);
    assert(emitter.emitter_type == expected.emitter_type);
    assert(emitter.max_particles == expected.max_particles);
    assert(std::fabs(emitter.birth_rate - expected.birth_rate) < 0.0001f);
    assert(std::fabs(emitter.life - expected.life) < 0.0001f);
    assert(std::fabs(emitter.target_size - expected.target_size) < 0.0001f);
    assert(std::fabs(emitter.radius_length - expected.radius_length) < 0.0001f);
    assert(emitter.shape_width_present == expected.width_present);
    assert(emitter.shape_height_present == expected.height_present);
    assert(std::fabs(emitter.shape_width - expected.shape_width) < 0.0001f);
    assert(std::fabs(emitter.shape_height - expected.shape_height) < 0.0001f);
    assert(emitter.geometry_mode == 0 && emitter.shape_data_offset != 0);
    assert(emitter.shape_source_tag == 3);
    assert(emitter.direction_type == 1 && emitter.direction_variation_present);
    assert(emitter.spin_axis_type == 0);
    const std::array<float, 3> zero_axis{};
    assert(emitter.spin_axis == zero_axis);
    assert(dh2_bres_emitter(&view, 1, &emitter) == EmitterError::index);
    assert(dh2_bres_emitter(&view, -1, &emitter) == EmitterError::index);

    VisualContent content{};
    assert(dh2_bres_visual_content(&view, &content) == EmitterError::ok);
    assert(content.emitter_count == 1);
    assert(content.kind == expected.visual_kind);
    if (expected.visual_kind == VisualContentKind::particles_without_geometry)
        assert(content.geometry_count == 0 && content.material_count > 0
               && content.effect_count > 0);
}
}

int main(int argc, char** argv) {
    assert(argc == 7);
    const Expected expected[] = {
        {"marker_fear.bdae", 0, 100, 60.0f, 1.0f, 200.0f, 100.0f, 100.0f, 100.0f,
         true, true,
         VisualContentKind::geometry_and_particles},
        {"marker_poisoned.bdae", 1, 100, 7.5f, 1.0f, 325.0f, 25.0f, 0.0f, 0.0f,
         false, true,
         VisualContentKind::particles_without_geometry},
        {"spell_dh2_hurt_earth.bdae", 0, 20, 0.0f, 1.0f, 200.0f, 0.0f, 101.425f, 101.425f,
         true, true,
         VisualContentKind::geometry_and_particles},
        {"spell_dh2_hurt_fire.bdae", 0, 20, 0.0f, 0.833333f, 250.0f, 200.0f, 20.0f, 20.0f,
         true, true,
         VisualContentKind::particles_without_geometry},
        {"spell_dh2_hurt_steam.bdae", 0, 30, 0.0f, 1.33333f, 300.0f, 500.0f, 20.0f, 20.0f,
         true, true,
         VisualContentKind::geometry_and_particles},
        {"bloodsplat.bdae", 1, 30, 0.0f, 0.5f, 200.0f, 78.0911f, 0.0f, 0.0f,
         false, true,
         VisualContentKind::particles_without_geometry}
    };
    for (int i = 0; i < 6; ++i) {
        assert(ends_with(argv[i + 1], expected[i].suffix));
        check_asset(argv[i + 1], expected[i]);
    }
}
