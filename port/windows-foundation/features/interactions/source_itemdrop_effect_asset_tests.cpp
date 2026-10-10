#include "../../../engine-resources/resources.hpp"
#include <algorithm>
#include <array>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
struct View {
    dh2::resources::BresView bres{};
};
void check(bool value, const std::string& message) {
    if (!value) throw std::runtime_error(message);
}
std::vector<std::uint8_t> read(const char* path) {
    std::ifstream stream(path, std::ios::binary);
    check(bool(stream), std::string("Missing source BRES: ") + path);
    return {std::istreambuf_iterator<char>(stream), {}};
}
std::uint32_t word(const View& view, std::size_t offset) {
    check(offset <= view.bres.size && view.bres.size - offset >= 4,
          "Source material/effect word is outside BRES");
    std::uint32_t value{};
    std::memcpy(&value, view.bres.bytes + offset, sizeof(value));
    return value;
}
std::string text(const View& view, std::uint32_t offset) {
    if (!offset) return {};
    check(offset < view.bres.size, "Source material/effect string is outside BRES");
    const auto* begin = reinterpret_cast<const char*>(view.bres.bytes + offset);
    const auto* end = static_cast<const char*>(std::memchr(begin, 0, view.bres.size - offset));
    check(end != nullptr, "Source material/effect string is unterminated");
    return {begin, end};
}
std::size_t named_row(const View& view, dh2::resources::Library library,
                      const char* expected) {
    std::size_t result = 0;
    unsigned matches = 0;
    for (unsigned i = 0; i < dh2_bres_library_count(&view.bres, library); ++i) {
        const auto* row = dh2_bres_library_item(&view.bres, library,
            static_cast<std::int32_t>(i));
        const auto offset = static_cast<std::size_t>(row - view.bres.bytes);
        if (text(view, word(view, offset)) != expected) continue;
        result = offset;
        ++matches;
    }
    check(matches == 1, std::string("Expected exactly one source library row named ") + expected);
    return result;
}
struct Technique {
    std::string name, vertex, vertex_defines, fragment, fragment_defines;
    std::uint32_t pass_count{};
    std::array<std::uint8_t, 76> render_state{};
};
Technique technique(const View& view, const char* effect_name, const char* wanted) {
    const auto effect = named_row(view, dh2::resources::Library::effect, effect_name);
    const auto count = word(view, effect + 32);
    const auto base = word(view, effect + 36);
    check(count <= 512, "Source effect technique count exceeds bounded audit range");
    Technique result;
    unsigned matches = 0;
    for (std::uint32_t i = 0; i < count; ++i) {
        const auto row = std::size_t(base) + std::size_t(i) * 12;
        if (text(view, word(view, row)) != wanted) continue;
        ++matches;
        result.name = wanted;
        result.pass_count = word(view, row + 4);
        check(result.pass_count == 1,
              std::string("Source effect technique has unsupported pass count: ") + wanted);
        const auto pass = word(view, row + 8);
        (void)word(view, std::size_t(pass) + 112);
        result.vertex = text(view, word(view, std::size_t(pass) + 4));
        result.vertex_defines = text(view, word(view, std::size_t(pass) + 12));
        result.fragment = text(view, word(view, std::size_t(pass) + 16));
        result.fragment_defines = text(view, word(view, std::size_t(pass) + 24));
        std::memcpy(result.render_state.data(), view.bres.bytes + pass + 28,
                    result.render_state.size());
    }
    check(matches == 1, std::string("Source effect technique absent/ambiguous: ") + wanted);
    return result;
}
struct Material {
    std::string id, effect_file, effect_uri, technique;
};
Material authored_material(const View& view, const char* wanted) {
    const auto row = named_row(view, dh2::resources::Library::material, wanted);
    Material result;
    result.id = wanted;
    result.effect_file = text(view, word(view, row + 8));
    result.effect_uri = text(view, word(view, row + 12));
    for (unsigned i = 0; i < word(view, row + 16); ++i) {
        const auto parameter = std::size_t(word(view, row + 20)) + 24 * i;
        const auto name = text(view, word(view, parameter));
        if (name != "Multilight-fx-profile_GLES2/CurrentTechnique") continue;
        check(word(view, parameter + 8) == 20,
              "Source GLES2 CurrentTechnique parameter has an unexpected serialized type");
        result.technique = text(view, word(view, word(view, parameter + 20) + 4));
        break;
    }
    return result;
}
void open(View& view, const std::vector<std::uint8_t>& bytes) {
    check(dh2_bres_open(&view.bres, bytes.data(), bytes.size()) == dh2::resources::BresError::ok,
          "Original itemdrop/effect BRES did not validate");
}
void print_technique(const Technique& t) {
    std::cout << "{\"name\":\"" << t.name << "\",\"passes\":" << t.pass_count
              << ",\"vertex\":\"" << t.vertex << "\",\"vertex_defines\":\""
              << t.vertex_defines << "\",\"fragment\":\"" << t.fragment
              << "\",\"fragment_defines\":\"" << t.fragment_defines << "\"}";
}
}

int main(int argc, char** argv) try {
    check(argc == 4, "supply itemdrops BRES and both source-cache effect BRES copies");
    const auto itemdrop_bytes = read(argv[1]);
    const auto canonical_effect_bytes = read(argv[2]);
    const auto root_effect_bytes = read(argv[3]);
    View itemdrops, canonical_effect, root_effect;
    open(itemdrops, itemdrop_bytes);
    open(canonical_effect, canonical_effect_bytes);
    open(root_effect, root_effect_bytes);

    const auto potion_material = authored_material(itemdrops, "Material__37");
    const auto gold_material = authored_material(itemdrops, "Material__38");
    check(potion_material.effect_file == "GL_Diffuse_L1_VC_iPhone.bdae" &&
          potion_material.effect_uri == "#Multilight-fx" && !potion_material.technique.empty(),
          "Potion Material__37 external effect/selected GLES2 technique changed");
    check(gold_material.effect_file == "GL_Diffuse_L1_VC_iPhone.bdae" &&
          gold_material.effect_uri == "#Multilight-fx" && !gold_material.technique.empty(),
          "Gold Material__38 external effect/selected GLES2 technique changed");

    const auto canonical_potion = technique(canonical_effect, "Multilight-fx",
                                            potion_material.technique.c_str());
    const auto canonical_gold = technique(canonical_effect, "Multilight-fx",
                                          gold_material.technique.c_str());
    const auto root_potion = technique(root_effect, "Multilight-fx",
                                       potion_material.technique.c_str());
    const auto root_gold = technique(root_effect, "Multilight-fx",
                                     gold_material.technique.c_str());
    check(canonical_potion.vertex == "GL_Diffuse_L1_iPhone_VS.glsl" &&
          canonical_potion.fragment == "GL_Diffuse_L1_iPhone_FS.glsl" &&
          canonical_gold.vertex == canonical_potion.vertex &&
          canonical_gold.fragment == canonical_potion.fragment,
          "Canonical effect BRES selected shader paths differ from the source GL_Diffuse family");
    const auto glows = authored_material(itemdrops, "Glows");
    check(glows.effect_file.empty() &&
          glows.effect_uri == "#ProfileCOMMON_Glows-fx1302961734_itemdrops",
          "Potion Glows is not the original local ProfileCOMMON material");
    const auto glows_default = technique(itemdrops,
        "ProfileCOMMON_Glows-fx1302961734_itemdrops", "default");
    check(glows_default.vertex == "ProfileCOMMON_emul_VS.glsl" &&
          glows_default.fragment == "ProfileCOMMON_emul_FS.glsl",
          "Potion Glows default ProfileCOMMON pass shader paths changed");

    std::cout << "{\"validation\":\"PASS\",\"potion_material\":{\"id\":\""
              << potion_material.id << "\",\"effect_file\":\"" << potion_material.effect_file
              << "\",\"effect_uri\":\"" << potion_material.effect_uri
              << "\",\"selected_gles2_technique\":\"" << potion_material.technique
              << "\",\"canonical_effect_pass\":";
    print_technique(canonical_potion);
    std::cout << "},\"gold_material\":{\"id\":\"" << gold_material.id
              << "\",\"effect_file\":\"" << gold_material.effect_file
              << "\",\"effect_uri\":\"" << gold_material.effect_uri
              << "\",\"selected_gles2_technique\":\"" << gold_material.technique
              << "\",\"canonical_effect_pass\":";
    print_technique(canonical_gold);
    std::cout << "},\"root_duplicate_techniques_match\":"
              << ((root_potion.vertex == canonical_potion.vertex &&
                   root_potion.vertex_defines == canonical_potion.vertex_defines &&
                   root_potion.fragment == canonical_potion.fragment &&
                   root_potion.fragment_defines == canonical_potion.fragment_defines &&
                   root_potion.render_state == canonical_potion.render_state &&
                   root_gold.vertex == canonical_gold.vertex &&
                   root_gold.vertex_defines == canonical_gold.vertex_defines &&
                   root_gold.fragment == canonical_gold.fragment &&
                   root_gold.fragment_defines == canonical_gold.fragment_defines &&
                   root_gold.render_state == canonical_gold.render_state) ? "true" : "false")
              << ",\"potion_glows_local_profilecommon\":{\"effect_uri\":\""
              << glows.effect_uri << "\",\"default_pass\":";
    print_technique(glows_default);
    std::cout << "}}\n";
    return 0;
} catch (const std::exception& error) {
    std::cerr << "FAIL: " << error.what() << '\n';
    return 1;
}
