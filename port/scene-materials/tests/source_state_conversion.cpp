#include "../source_state_conversion.hpp"

#include "../../material-bindings/bindings.hpp"
#include "../render_state_snapshot.hpp"
#include "../technique_selector.hpp"

#include <array>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

namespace {
using dh2::resources::BresView;

std::uint32_t word(const std::uint8_t* p) {
    return std::uint32_t(p[0]) | (std::uint32_t(p[1]) << 8) |
           (std::uint32_t(p[2]) << 16) | (std::uint32_t(p[3]) << 24);
}

void append_hex(std::string& output, const std::uint8_t* bytes, std::size_t size) {
    static constexpr char digits[] = "0123456789abcdef";
    output.clear();
    output.reserve(size * 2);
    for (std::size_t i = 0; i < size; ++i) {
        output.push_back(digits[bytes[i] >> 4]);
        output.push_back(digits[bytes[i] & 15]);
    }
}

void write_json_string(const std::string& value) {
    std::putchar('"');
    for (const auto c : value) {
        if (c == '"' || c == '\\') std::putchar('\\');
        std::putchar(c);
    }
    std::putchar('"');
}

std::array<std::uint8_t, 32> independent_formula(const std::uint8_t* source) {
    const auto state_bits = word(source + 8);
    const auto source_flags = word(source + 12);
    const auto source_flags_10 = word(source + 16);
    const auto put = [](std::uint32_t& to, const std::uint32_t from,
                        unsigned source_bit, unsigned count,
                        unsigned destination_bit) {
        const std::uint32_t mask = (1u << count) - 1u;
        to |= ((from >> source_bit) & mask) << destination_bit;
    };
    std::uint32_t packed = source[0] | (std::uint32_t(source[2]) << 8) |
                           (std::uint32_t(source[3]) << 16);
    put(packed, state_bits, 12, 3, 24);
    put(packed, source_flags, 12, 3, 27);
    put(packed, state_bits, 30, 2, 30);

    std::uint32_t flags = 0;
    put(flags, state_bits, 18, 3, 0);
    put(flags, state_bits, 21, 3, 3);
    put(flags, state_bits, 24, 3, 6);
    put(flags, state_bits, 27, 3, 9);
    put(flags, source_flags, 15, 2, 12);
    put(flags, source_flags, 17, 2, 14);
    put(flags, source_flags, 19, 5, 16);
    put(flags, source_flags, 25, 6, 21);
    put(flags, source_flags_10, 0, 1, 27);

    std::array<std::uint8_t, 32> output{};
    const auto set_word = [&](std::size_t offset, std::uint32_t value) {
        output[offset] = static_cast<std::uint8_t>(value);
        output[offset + 1] = static_cast<std::uint8_t>(value >> 8);
        output[offset + 2] = static_cast<std::uint8_t>(value >> 16);
        output[offset + 3] = static_cast<std::uint8_t>(value >> 24);
    };
    set_word(0, packed);
    set_word(4, flags);
    for (std::size_t i = 0; i < 4; ++i) output[8 + i] = source[0x14 + i];
    set_word(12, word(source + 0x28));
    set_word(16, word(source + 0x2c));
    set_word(20, word(source + 0x30));
    set_word(24, word(source + 0x34));
    set_word(28, word(source + 0x38));
    return output;
}

bool equal(const std::uint8_t* a, const std::uint8_t* b, std::size_t n) {
    for (std::size_t i = 0; i < n; ++i) if (a[i] != b[i]) return false;
    return true;
}

bool locate_technique(const dh2::materials::EffectGroup& group,
                      const char* requested, std::uint8_t& ordinal,
                      const std::uint8_t*& source, std::uint32_t& payload,
                      std::uint32_t& pass_count) {
    std::string error;
    if (dh2::scene_materials::effect_technique_ordinal(
            group, requested, ordinal, error) !=
        dh2::scene_materials::TechniqueSelectorError::ok) return false;
    const auto* record = group.named_records + std::size_t(ordinal) * 12;
    pass_count = word(record + 4);
    payload = word(record + 8);
    if (pass_count != 1 || payload > group.image.size ||
        0x74 > group.image.size - payload ||
        0x1c + dh2::scene_materials::source_state::kVideoStateBytes > 0x74)
        return false;
    source = group.image.bytes + payload + 0x1c;
    return true;
}

bool run(const char* effect_path, const char* scene_path) {
    std::ifstream input(effect_path, std::ios::binary);
    if (!input) return false;
    std::vector<std::uint8_t> bytes((std::istreambuf_iterator<char>(input)),
                                    std::istreambuf_iterator<char>());
    BresView image{};
    if (dh2_bres_open(&image, bytes.data(), bytes.size()) !=
        dh2::resources::BresError::ok) return false;
    dh2::materials::Effect effect{};
    if (dh2_effect_record(&effect, &image, 0) != dh2::materials::Error::ok ||
        std::string(effect.name) != "Multilight") return false;
    dh2::materials::EffectGroup group{};
    if (dh2_effect_group(&group, &effect, 1) != dh2::materials::Error::ok)
        return false;

    // Resolve the source material before looking up the effect technique. This
    // preserves source profile selectors as input provenance without claiming
    // which profile the original runtime ultimately used.
    std::ifstream scene_input(scene_path, std::ios::binary);
    if (!scene_input) return false;
    std::vector<std::uint8_t> scene_bytes(
        (std::istreambuf_iterator<char>(scene_input)),
        std::istreambuf_iterator<char>());
    BresView scene{};
    if (dh2_bres_open(&scene, scene_bytes.data(), scene_bytes.size()) !=
        dh2::resources::BresError::ok) return false;
    const auto material_count = dh2_bres_library_count(
        &scene, dh2::resources::Library::material);
    dh2::materials::Material swamp_material{};
    std::uint32_t matching_materials = 0;
    for (std::uint32_t i = 0; i < material_count; ++i) {
        dh2::materials::Material material{};
        if (dh2_material_record(&material, &scene, static_cast<std::int32_t>(i)) !=
            dh2::materials::Error::ok) return false;
        if (std::string(material.id) == "Material__11611") {
            swamp_material = material;
            ++matching_materials;
        }
    }
    if (matching_materials != 1 ||
        std::string(swamp_material.external_effect_file ?
                        swamp_material.external_effect_file : "") !=
            "GL_Diffuse_L1_VC_iPhone.bdae" ||
        std::string(swamp_material.effect_url ? swamp_material.effect_url : "") !=
            "#Multilight-fx") return false;
    std::vector<dh2::scene_materials::CurrentTechnique> selectors;
    std::string selector_error;
    if (dh2::scene_materials::material_current_techniques(
            swamp_material, selectors, selector_error) !=
            dh2::scene_materials::TechniqueSelectorError::ok ||
        selectors.size() != 2) return false;
    const auto* gles_selector = static_cast<const dh2::scene_materials::CurrentTechnique*>(nullptr);
    const auto* gles2_selector = static_cast<const dh2::scene_materials::CurrentTechnique*>(nullptr);
    for (const auto& selector : selectors) {
        if (selector.profile == "GLES") gles_selector = &selector;
        if (selector.profile == "GLES2") gles2_selector = &selector;
    }
    if (!gles_selector || !gles2_selector ||
        gles_selector->name != "L1_Vc_Al_----_----_----_----" ||
        gles2_selector->name != "L1_Vc_Al_Sp_----_----_----") return false;

    std::uint8_t al_ordinal = 0xff, al_sp_ordinal = 0xff;
    const std::uint8_t* al_source = nullptr;
    const std::uint8_t* al_sp_source = nullptr;
    std::uint32_t al_payload = 0, al_sp_payload = 0;
    std::uint32_t al_pass_count = 0, al_sp_pass_count = 0;
    std::string error;
    std::uint8_t gles_ordinal = 0xff, gles2_ordinal = 0xff;
    if (dh2::scene_materials::effect_technique_ordinal(
            group, gles_selector->name, gles_ordinal, error) !=
            dh2::scene_materials::TechniqueSelectorError::ok ||
        dh2::scene_materials::effect_technique_ordinal(
            group, gles2_selector->name, gles2_ordinal, error) !=
            dh2::scene_materials::TechniqueSelectorError::ok ||
        gles_ordinal != 2 || gles2_ordinal != 4 ||
        !locate_technique(group, gles_selector->name.c_str(),
                          al_ordinal, al_source, al_payload, al_pass_count) ||
        !locate_technique(group, gles2_selector->name.c_str(),
                          al_sp_ordinal, al_sp_source, al_sp_payload,
                          al_sp_pass_count) ||
        al_ordinal != 2 || al_sp_ordinal != 4 ||
        al_payload != 0x3320 || al_sp_payload != 0x3498 ||
        al_pass_count != 1 || al_sp_pass_count != 1 ||
        !equal(al_source, al_sp_source,
               dh2::scene_materials::source_state::kVideoStateBytes))
        return false;

    std::array<std::uint8_t, 32> al_output{}, al_sp_output{};
    using dh2::scene_materials::source_state::convert_video_state;
    using dh2::scene_materials::source_state::Error;
    if (convert_video_state(al_source, 0x4c, al_output.data(), al_output.size()) !=
            Error::ok ||
        convert_video_state(al_sp_source, 0x4c, al_sp_output.data(),
                            al_sp_output.size()) != Error::ok ||
        !equal(al_output.data(), al_sp_output.data(), al_output.size()))
        return false;

    // This reference output is from the original ARM copy constructor on the
    // byte-identical selector-ordinal-2 payload; ordinal 4's source bytes are
    // asserted equal above. The executor differential is reported separately.
    static constexpr std::uint8_t original_output[32] = {
        0x54,0x00,0xff,0x18, 0x07,0x00,0x0b,0x00,
        0xff,0xff,0xff,0xff, 0x00,0x00,0x80,0x3f,
        0x00,0x00,0x80,0x3f, 0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00, 0x00,0x00,0x80,0x3f,
    };
    if (!equal(al_output.data(), original_output, sizeof(original_output)))
        return false;

    dh2::scene_materials::render_state::PassState decoded{};
    if (dh2::scene_materials::render_state::decode_pass_state(
            al_sp_output.data(), al_sp_output.size(), decoded) !=
            dh2::scene_materials::render_state::Error::ok ||
        !decoded.blend_enabled || !decoded.depth_test_enabled ||
        decoded.depth_write_enabled || decoded.blend_source.ordinal != 4 ||
        decoded.blend_source.gl_enum != 0x0302 ||
        decoded.blend_destination.ordinal != 5 ||
        decoded.blend_destination.gl_enum != 0x0303 ||
        decoded.blend_equation.gl_enum != 0x8006 ||
        decoded.depth_function.ordinal != 3 ||
        decoded.depth_function.gl_enum != 0x0203)
        return false;

    // Validate malformed sizes and null inputs; output is never read on error.
    std::array<std::uint8_t, 32> ignored{};
    if (convert_video_state(nullptr, 0x4c, ignored.data(), ignored.size()) != Error::argument ||
        convert_video_state(al_source, 0x4b, ignored.data(), ignored.size()) != Error::input_size ||
        convert_video_state(al_source, 0x4c, ignored.data(), 31) != Error::output_size)
        return false;

    // Reproducible pseudo-random raw bitfield corpus. The parent can feed these
    // input bytes to the original instruction executor for true differential
    // coverage without making this host test depend on private emulator tools.
    std::uint32_t rng = 0x5d7a10c1u;
    constexpr unsigned corpus_size = 36;
    std::array<std::array<std::uint8_t, 0x4c>, corpus_size> corpus{};
    std::array<std::array<std::uint8_t, 0x20>, corpus_size> converted{};
    std::array<std::array<std::uint8_t, 0x20>, corpus_size> reference{};
    for (unsigned row = 0; row < corpus_size; ++row) {
        if (row == 1) {
            corpus[row].fill(0xff);
        } else if (row == 2) {
            for (std::size_t i = 0; i < corpus[row].size(); ++i)
                corpus[row][i] = al_sp_source[i];
        } else if (row == 3) {
            for (std::size_t i = 0; i < corpus[row].size(); ++i)
                corpus[row][i] = static_cast<std::uint8_t>(i * 37u + 11u);
        } else if (row >= 4) {
            for (auto& value : corpus[row]) {
                rng ^= rng << 13; rng ^= rng >> 17; rng ^= rng << 5;
                value = static_cast<std::uint8_t>(rng >> 24);
            }
        }
        if (convert_video_state(corpus[row].data(), corpus[row].size(),
                                converted[row].data(), converted[row].size()) != Error::ok)
            return false;
        reference[row] = independent_formula(corpus[row].data());
        if (!equal(converted[row].data(), reference[row].data(), 0x20))
            return false;
    }

    std::string source_hex, output_hex;
    std::printf("{\"validation\":\"PASS\",\"effect\":\"%s\","
                "\"scene_material\":\"%s\",\"external_effect_file\":\"%s\","
                "\"effect_url\":\"%s\",\"gles_selector\":\"%s\","
                "\"gles2_selector\":\"%s\",\"gles_selector_ordinal\":%u,"
                "\"gles2_selector_ordinal\":%u,"
                "\"group\":1,\"group_name_table_offset\":12412,"
                "\"al_ordinal\":%u,\"al_payload_offset\":%u,"
                "\"al_sp_ordinal\":%u,\"al_sp_payload_offset\":%u,"
                "\"gles2_state_offset\":%u,\"state_bytes\":76,"
                "\"state_equal\":true,\"original_ctor_fixture\":true,"
                "\"original_random_differential\":\"pending_parent_executor\","
                "\"blend_enabled\":true,\"blend_src_gl\":\"GL_SRC_ALPHA\","
                "\"blend_dst_gl\":\"GL_ONE_MINUS_SRC_ALPHA\","
                "\"blend_equation_gl\":\"GL_FUNC_ADD\","
                "\"depth_test\":true,\"depth_compare_gl\":\"GL_LEQUAL\","
                "\"depth_write\":false,"
                "\"random_corpus\":[",
                effect.name, swamp_material.id, swamp_material.external_effect_file,
                swamp_material.effect_url, gles_selector->name.c_str(),
                gles2_selector->name.c_str(), unsigned(gles_ordinal),
                unsigned(gles2_ordinal), unsigned(al_ordinal), al_payload,
                unsigned(al_sp_ordinal), al_sp_payload, al_sp_payload + 0x1c);
    for (unsigned row = 0; row < corpus_size; ++row) {
        append_hex(source_hex, corpus[row].data(), corpus[row].size());
        append_hex(output_hex, converted[row].data(), converted[row].size());
        if (row) std::putchar(',');
        std::printf("{\"input\":"); write_json_string(source_hex);
        std::printf(",\"output\":"); write_json_string(output_hex);
        std::putchar('}');
    }
    std::printf("],\"output_bytes\":");
    append_hex(output_hex, al_sp_output.data(), al_sp_output.size());
    write_json_string(output_hex);
    std::printf("}\n");
    return true;
}

} // namespace

int main(int argc, char** argv) {
    if (argc != 3 || !run(argv[1], argv[2])) {
        std::fputs("FAIL: source-state conversion or checked effect payload assertion\n",
                   stderr);
        return 1;
    }
    return 0;
}
