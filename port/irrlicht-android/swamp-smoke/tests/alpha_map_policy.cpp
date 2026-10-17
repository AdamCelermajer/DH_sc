#include "../alpha_map_policy.hpp"
#include "../../../android-app/swamp_render_policy.hpp"

#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

namespace {
using dh2::irrlicht_swamp::AlphaMapMode;
using dh2::scene_materials::CurrentTechnique;

void require(bool condition, const char* message) {
    if (!condition) {
        std::fprintf(stderr, "SWAMP CurrentTechnique alpha policy failed: %s\n", message);
        std::exit(1);
    }
}

void set_reference(dh2::viewer::SceneTextureReference* reference,
                   const char* parameter, const char* path) {
    reference->image_index = 0;
    std::strncpy(reference->parameter_id, parameter,
                 sizeof(reference->parameter_id) - 1);
    std::strncpy(reference->source_path, path,
                 sizeof(reference->source_path) - 1);
}

CurrentTechnique selector(const char* profile, const char* name) {
    CurrentTechnique result{};
    result.profile = profile;
    result.name = name;
    return result;
}
}

int main() {
    const std::vector<CurrentTechnique> al{
        selector("GLES", "L1_Vc_Al_----_----_----_----"),
        selector("GLES2", "L1_Vc_Al_Sp_----_----_----")};
    const auto al_selection = dh2::irrlicht_swamp::resolve_alpha_map_selection(al);
    require(al_selection.mode == AlphaMapMode::source_al_fractional &&
                al_selection.profile_count == 2,
            "consistent GLES/GLES2 AL selector pair must choose fractional alpha");

    const std::vector<CurrentTechnique> at{
        selector("GLES", "L1_Vc_At_----_----_----_----"),
        selector("GLES2", "L1_Vc_At_Sp_----_----_----")};
    require(dh2::irrlicht_swamp::resolve_alpha_map_selection(at).mode ==
                AlphaMapMode::source_at_cutout,
            "consistent GLES/GLES2 AT selector pair must choose cutout mode");

    const std::vector<CurrentTechnique> opaque{
        selector("GLES", "L1_Vc_----_----_----_----_----"),
        selector("GLES2", "L1_Vc_----_Sp_----_----_----")};
    require(dh2::irrlicht_swamp::resolve_alpha_map_selection(opaque).mode ==
                AlphaMapMode::no_alpha_variant,
            "both opaque selectors must remain an explicit no-alpha variant");

    require(dh2::irrlicht_swamp::resolve_alpha_map_selection({}).mode ==
                AlphaMapMode::unsupported,
            "missing selectors must fail closed");
    require(dh2::irrlicht_swamp::resolve_alpha_map_selection({
                selector("GLES", "L1_Vc_Al_----_----_----_----"),
                selector("GLES2", "L1_Vc_At_Sp_----_----_----")}).mode ==
                AlphaMapMode::unsupported,
            "conflicting AL/AT profiles must fail closed");
    require(dh2::irrlicht_swamp::resolve_alpha_map_selection({
                selector("GLES", "L1_Vc_Al_----_----_----_----"),
                selector("GLES2", "L1_Vc_----_Sp_----_----_----")}).mode ==
                AlphaMapMode::unsupported,
            "profile with no alpha variant must conflict with AL profile");
    require(dh2::irrlicht_swamp::resolve_alpha_map_selection({
                selector("Vulkan", "L1_Vc_Al_----_----_----_----")}).mode ==
                AlphaMapMode::unsupported,
            "unknown source profile must fail closed");
    require(dh2::irrlicht_swamp::resolve_alpha_map_selection({
                selector("GLES", "L2_Vc_Al_----_----_----_----")}).mode ==
                AlphaMapMode::unsupported,
            "unsupported selector family must fail closed");
    require(dh2::irrlicht_swamp::resolve_alpha_map_selection({
                selector("GLES", "L1_Vc_Al_----_----_----_----"),
                selector("GLES", "L1_Vc_Al_----_----_----_----")}).mode ==
                AlphaMapMode::unsupported,
            "duplicate source profiles must fail closed");

    dh2::viewer::SceneDrawDescriptor draw{};
    std::strcpy(draw.material_id, "Material__11611");
    std::array<dh2::viewer::SceneTextureReference, 3> refs{};
    set_reference(&refs[0], "Diffuse", "q:/data/iphone/3d/textures/env_swamp.tga");
    set_reference(&refs[1], "AlphaMap",
                  "q:/data/iphone/3d/textures/pvr2_env_swamp_alpha.tga");
    set_reference(&refs[2], "Specular",
                  "q:/data/iphone/3d/textures/env_swamp_spec.tga");
    require(dh2::irrlicht_swamp::swamp_draw_uses_alpha_map_mode(
                draw, refs.data(), refs.size(), al_selection.mode),
            "paired source diffuse/AlphaMap references must accept derived AL mode");
    require(dh2::irrlicht_swamp::swamp_draw_uses_alpha_map_mode(
                draw, refs.data(), refs.size(), AlphaMapMode::source_at_cutout),
            "paired source diffuse/AlphaMap references must accept derived AT mode");
    std::strcpy(draw.material_id, "Material__11610");
    require(dh2::irrlicht_swamp::swamp_draw_uses_alpha_map_mode(
                draw, refs.data(), refs.size(), al_selection.mode),
            "selector policy must not hardcode Material__11611 identity");
    require(!dh2::irrlicht_swamp::swamp_draw_uses_alpha_map_mode(
                draw, refs.data(), 1, al_selection.mode),
            "Diffuse without AlphaMap must not enable alpha-map treatment");
    refs[1].image_index = -1;
    require(!dh2::irrlicht_swamp::swamp_draw_uses_alpha_map_mode(
                draw, refs.data(), refs.size(), al_selection.mode),
            "unresolved AlphaMap image must not be applied");
    require(!dh2::irrlicht_swamp::swamp_draw_uses_alpha_map_mode(
                draw, refs.data(), refs.size(), AlphaMapMode::unsupported),
            "unsupported source selector must not apply alpha-map treatment");
    require(!dh2::viewer::swamp_draw_writes_depth("Material__11598", false),
            "source additive overlay must keep depth writes disabled");

    std::array<std::uint8_t, 8> diffuse{{10, 20, 30, 255, 40, 50, 60, 255}};
    const std::array<std::uint8_t, 8> mask{{1, 2, 3, 0, 4, 5, 6, 127}};
    require(dh2::irrlicht_swamp::apply_swamp_alpha_map(
                diffuse.data(), mask.data(), 2),
            "valid diffuse/AlphaMap composition rejected");
    require(diffuse == std::array<std::uint8_t, 8>{{10, 20, 30, 3,
                                                   40, 50, 60, 6}},
            "composition must preserve diffuse RGB and copy decoded AlphaMap blue");
    require(!dh2::irrlicht_swamp::apply_swamp_alpha_map(nullptr, mask.data(), 1) &&
            !dh2::irrlicht_swamp::apply_swamp_alpha_map(diffuse.data(), nullptr, 1),
            "composition must reject missing pixel buffers");

    std::puts("source CurrentTechnique AL/AT profile resolution, fail-closed selector handling, and blue-to-alpha composition pass");
    return 0;
}
