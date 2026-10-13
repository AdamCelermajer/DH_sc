#include "glitch-scene-collada-gui/recon/v2/wave_07.hpp"

#include <cstdio>
#include <string>
#include <vector>

using namespace glitch::collada;
using namespace glitch::collada::recon_v2_07;

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

static bool near(float a, float b) { return a > b - 1e-4f && a < b + 1e-4f; }

struct SlotRecord {
    void* receiver = nullptr;
    std::uint32_t slot = 0;
    const void* arg = nullptr;
    int calls = 0;
};

static wave_07_slot_call recorder(SlotRecord& rec, int result) {
    return [&rec, result](void* receiver, std::uint32_t slot, const void* arg) {
        rec.receiver = receiver;
        rec.slot = slot;
        rec.arg = arg;
        ++rec.calls;
        return result;
    };
}

int main() {
    // Blend: count 0 gives zero, count 1 copies the key, count 2 is a weighted sum.
    const float values[6] = {1, 2, 3, 4, 5, 6};
    const float weights[2] = {0.5f, 2.0f};
    float out[3] = {9, 9, 9};
    get_blended_0x626c30(nullptr, values, weights, 0, out);
    CHECK(out[0] == 0.0f && out[1] == 0.0f && out[2] == 0.0f);
    get_blended_0x626c30(nullptr, values, weights, 1, out);
    CHECK(out[0] == 1.0f && out[1] == 2.0f && out[2] == 3.0f);
    get_added_0x62a754(nullptr, values, weights, 2, out);
    CHECK(near(out[0], 8.5f) && near(out[1], 11.0f) && near(out[2], 13.5f));

    // Factory config getters write the placeholder record.
    wave_07_buffer_config cfg{0xFFFFFFFFu, 1, 1};
    CColladaDatabase db;
    SGeometry geom{};
    CHECK(get_vertex_buffer_config_0x62ff08(db, &geom, &cfg) == &cfg);
    CHECK(get_index_buffer_config_0x62ff20(db, &geom, &cfg) == &cfg);
    CHECK(cfg.word00 == 4u && cfg.byte04 == 0 && cfg.byte05 == 0);

    // Light lookup strips up to and including '#' and checks the type tag.
    std::vector<std::string> seen;
    wave_07_resource_lookup lookup = [&seen](const char* name, std::uint32_t zero) -> void* {
        seen.emplace_back(name);
        CHECK(zero == 0u);
        static int object = 0;
        return &object;
    };
    wave_07_tag_of good = [](void*) -> std::uint32_t { return kWave07LightTag; };
    wave_07_tag_of bad = [](void*) -> std::uint32_t { return 0u; };
    CHECK(get_external_light_0x62ff80("file.dae#lightA", lookup, good) != nullptr);
    CHECK(seen.back() == "lightA");
    CHECK(get_external_light_0x62ff80("plain", lookup, good) != nullptr);
    CHECK(seen.back() == "plain");
    CHECK(get_external_light_0x62ff80("x#y", lookup, bad) == nullptr);

    // Quantized two-key lerp: keys 0 and 1 of a short track, unit scale, zero offset.
    const std::int16_t keys[6] = {0, 0, 0, 10, 20, 30};
    const float scale[3] = {1, 1, 1};
    const float offset[3] = {0, 0, 0};
    wave_07_quantized<std::int16_t> track{keys, scale, offset};
    float lerp_out[3] = {0, 0, 0};
    const float first = get_key_0x627bb4(track, 0, 1, 0.5f, lerp_out);
    CHECK(near(first, 5.0f) && near(lerp_out[1], 10.0f) && near(lerp_out[2], 15.0f));

    // Float pair lerp reads the vec3 at key a2 and the vec3 after it.
    const float pair[6] = {0, 0, 0, 2, 4, 6};
    float pair_out[3];
    get_key_float_0x6289d4(pair, 0, 0, 0.25f, pair_out);
    CHECK(near(pair_out[0], 0.5f) && near(pair_out[1], 1.0f) && near(pair_out[2], 1.5f));

    // apply_key: interp fills the value, the slot receives it (+0x94 for scale short).
    SlotRecord rec;
    wave_07_key_interp interp = [](int, int, float, float* out3) -> float {
        out3[0] = 1; out3[1] = 2; out3[2] = 3; return 1;
    };
    int receiver_tag = 0;
    CHECK(apply_key_0x627d14(interp, 0, 1, 0.0f, &receiver_tag, recorder(rec, 7)) == 7);
    CHECK(rec.calls == 1 && rec.slot == 0x94u && rec.receiver == &receiver_tag);
    CHECK(rec.arg != nullptr && static_cast<const float*>(rec.arg)[2] == 3.0f);

    // Slot blend: the dead sum is not stored; the tail call goes to +0xA4 with no argument.
    SlotRecord srec;
    CHECK(apply_blended_0x62827c(values, weights, 2, &receiver_tag, recorder(srec, 42)) == 42);
    CHECK(srec.calls == 1 && srec.slot == 0xA4u && srec.arg == nullptr && srec.receiver == &receiver_tag);

    // Material blend: value is blended and passed with the applicator's parameter id.
    animation_track::CMaterialApplicatorInfo info;
    info.parameterId = 0x21;
    std::uint16_t seen_id = 0;
    const float* seen_value = nullptr;
    float seen_v[3] = {0, 0, 0};
    wave_07_param_set set = [&](void*, std::uint16_t id, const void* v) -> int {
        seen_id = id;
        seen_value = static_cast<const float*>(v);
        seen_v[0] = seen_value[0]; seen_v[1] = seen_value[1]; seen_v[2] = seen_value[2];
        return 5;
    };
    CHECK(apply_blended_0x626a38(values, weights, 2, nullptr, &info, set) == 5);
    CHECK(seen_id == 0x21 && near(seen_v[0], 8.5f) && near(seen_v[2], 13.5f));

    // Force bind: the proxy is 16 bytes with vtable, a2, 0, a2; dispatch at +0x58.
    int particle_tag = 0;
    void* base_seen = nullptr;
    wave_07_bind_ports ports;
    ports.base_of = [&](void* self) -> void* { base_seen = self; return &particle_tag; };
    const void* proxy_seen = nullptr;
    std::uint32_t slot_seen = 0;
    ports.call = [&](void* receiver, std::uint32_t slot, const void* arg) -> int {
        CHECK(receiver == &particle_tag);
        slot_seen = slot;
        proxy_seen = arg;
        return 3;
    };
    CHECK(bind_force_0x630018(&receiver_tag, 0x1234u, ports) == 3);
    CHECK(base_seen == &receiver_tag && slot_seen == 0x58u);
    const wave_07_force_proxy* proxy = static_cast<const wave_07_force_proxy*>(proxy_seen);
    CHECK(proxy->vtable == 0x97B378u && proxy->arg_a == 0x1234u && proxy->zero == 0u && proxy->arg_b == 0x1234u);
    delete proxy;

    if (failures == 0) std::printf("recon_v2_wave_07_test: all checks passed\n");
    return failures == 0 ? 0 : 1;
}
