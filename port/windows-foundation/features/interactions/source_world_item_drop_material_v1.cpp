#include "source_world_item_drop_material_v1.hpp"
#include "../../asset_catalog.hpp"
#include "../../content_paths.hpp"
#include "../../source_material_pass.hpp"
#include "../../actor_lighting.hpp"
#include "../../../scene-materials/effect_render_pass_v4.hpp"
#include <algorithm>
#include <cstring>
#include <memory>
#include <sstream>
#include <stdexcept>

namespace dh::foundation::interactions {
namespace {
bool fail(std::string& error, const char* reason) { error = reason; return false; }

bool same_material(const OriginalMaterial& projected,
                   const dh2::scene::Material& source) {
    return projected.id == source.id && projected.diffuse == source.diffuse &&
        projected.alphaMap == source.alpha_map && projected.effectFile == source.effect_file &&
        projected.technique == source.gles2_technique &&
        projected.alphaReference == source.alpha_ref && projected.additive == source.additive &&
        std::memcmp(projected.textureMatrix.data(), source.texture_matrix,
                    sizeof(source.texture_matrix)) == 0;
}

bool merge_alpha(const AssetCatalog& assets, const std::string& owner_uri,
                 const std::string& alpha_uri, bool blue, TextureImage& diffuse,
                 std::string& error) {
    TextureImage alpha;
    const auto path = resolve_content_path(assets, alpha_uri, owner_uri);
    if (!load_texture(path, alpha, error)) return false;
    if (!diffuse.width || !diffuse.height || !alpha.width || !alpha.height ||
        diffuse.rgba.size() != std::size_t(diffuse.width) * diffuse.height * 4 ||
        alpha.rgba.size() != std::size_t(alpha.width) * alpha.height * 4)
        return fail(error, "Source itemdrop alpha/diffuse texture decode has invalid dimensions");
    for (std::uint32_t y = 0; y < diffuse.height; ++y) {
        const auto source_y = static_cast<std::uint32_t>(
            std::uint64_t(y) * alpha.height / diffuse.height);
        for (std::uint32_t x = 0; x < diffuse.width; ++x) {
            const auto source_x = static_cast<std::uint32_t>(
                std::uint64_t(x) * alpha.width / diffuse.width);
            const auto destination = (std::size_t(y) * diffuse.width + x) * 4 + 3;
            const auto mask = (std::size_t(source_y) * alpha.width + source_x) * 4 +
                (blue ? 2 : 0);
            const auto value = alpha.rgba[mask];
            diffuse.rgba[destination] = blue ? value : static_cast<std::uint8_t>(
                (unsigned(diffuse.rgba[destination]) * value + 127) / 255);
        }
    }
    error.clear();
    return true;
}

bool alpha_test_define(const std::string& source) {
    std::istringstream lines(source);
    std::string line;
    while (std::getline(lines, line)) {
        std::istringstream words(line);
        std::string directive, name;
        words >> directive >> name;
        if (directive == "#define" && name == "ALPHATEST") return true;
    }
    return false;
}

std::uint32_t bres_word(const dh2::resources::BresView& view, std::size_t offset) {
    if (offset > view.size || view.size - offset < sizeof(std::uint32_t))
        throw std::runtime_error("Source itemdrop effect word is outside BRES");
    std::uint32_t result{};
    std::memcpy(&result, view.bytes + offset, sizeof(result));
    return result;
}

std::string bres_text(const dh2::resources::BresView& view, std::uint32_t offset) {
    if (!offset) return {};
    if (offset >= view.size) throw std::runtime_error("Source itemdrop effect string is outside BRES");
    const auto* begin = reinterpret_cast<const char*>(view.bytes + offset);
    const auto* end = static_cast<const char*>(std::memchr(begin, 0, view.size - offset));
    if (!end) throw std::runtime_error("Source itemdrop effect string is unterminated");
    return {begin, end};
}

bool resolve_external_effect_pass(const dh2::resources::BresView& image,
                                  const dh2::scene::Material& source,
                                  dh2::scene::EffectRenderPassV4& output,
                                  std::string& error) {
    try {
        if (source.effect_file != "GL_Diffuse_L1_VC_iPhone.bdae" ||
            source.effect_uri.empty() || source.effect_uri.front() != '#' ||
            source.gles2_technique.empty())
            throw std::runtime_error("Itemdrop external effect request differs from the authored source declaration");
        std::size_t effect_row = 0;
        unsigned effect_matches = 0;
        const auto effect_name = source.effect_uri.substr(1);
        for (unsigned i = 0; i < dh2_bres_library_count(&image, dh2::resources::Library::effect); ++i) {
            const auto* row = dh2_bres_library_item(&image, dh2::resources::Library::effect,
                static_cast<std::int32_t>(i));
            const auto offset = static_cast<std::size_t>(row - image.bytes);
            if (bres_text(image, bres_word(image, offset)) != effect_name) continue;
            effect_row = offset;
            ++effect_matches;
        }
        if (effect_matches != 1)
            throw std::runtime_error("Source external effect URI did not select exactly one BRES effect");
        const auto count = bres_word(image, effect_row + 32);
        const auto base = bres_word(image, effect_row + 36);
        if (!count || count > 512)
            throw std::runtime_error("Source external effect technique list is empty or outside bounds");
        std::size_t selected_pass = 0;
        unsigned technique_matches = 0;
        for (std::uint32_t i = 0; i < count; ++i) {
            const auto row = std::size_t(base) + std::size_t(i) * 12;
            if (bres_text(image, bres_word(image, row)) != source.gles2_technique) continue;
            ++technique_matches;
            if (bres_word(image, row + 4) != 1)
                throw std::runtime_error("Source external effect selected technique is not single-pass");
            selected_pass = bres_word(image, row + 8);
        }
        if (technique_matches != 1)
            throw std::runtime_error("Source GLES2 technique does not select exactly one external effect range");
        (void)bres_word(image, selected_pass + 112);

        dh2::scene::EffectRenderPassV4 next;
        next.vertex_file = bres_text(image, bres_word(image, selected_pass + 4));
        next.vertex_defines = bres_text(image, bres_word(image, selected_pass + 12));
        next.fragment_file = bres_text(image, bres_word(image, selected_pass + 16));
        next.fragment_defines = bres_text(image, bres_word(image, selected_pass + 24));
        std::memcpy(next.source.data(), image.bytes + selected_pass + 28, next.source.size());
        if (dh2_render_pass_convert_v3(&next.pass, &next.source) != 0)
            throw std::runtime_error("Source external effect render state conversion failed");
        const auto state0 = [&] { std::uint32_t value{}; std::memcpy(&value, next.pass.data(), 4); return value; }();
        const auto state1 = [&] { std::uint32_t value{}; std::memcpy(&value, next.pass.data() + 4, 4); return value; }();
        static const std::uint32_t blend[]{0,1,0x300,0x301,0x302,0x303,0x306,0x307,0x304,0x305,0x8001,0x8002,0x8003,0x8004,0x308};
        static const std::uint32_t equation[]{0x8006,0x800a,0x800b,0x8007,0x8008};
        static const std::uint32_t cull[]{0x405,0x404,0x408};
        const auto src = state0 & 15, dst = (state0 >> 4) & 15;
        const auto eq = (state0 >> 24) & 7, cf = state0 >> 30;
        if (src >= 15 || dst >= 15 || eq >= 5 || cf >= 3)
            throw std::runtime_error("Source external effect render state enum is unsupported");
        next.blend_src = blend[src];
        next.blend_dst = blend[dst];
        next.blend_equation = equation[eq];
        next.depth_function = 0x200 + ((state0 >> 27) & 7);
        next.cull_face = cull[cf];
        next.front_face = (state1 & (1 << 18)) ? 0x900 : 0x901;
        next.blend = state1 & (1 << 16);
        next.cull = state1 & (1 << 17);
        next.depth = state1 & (1 << 19);
        next.depth_write = state1 & (1 << 20);
        next.stencil = state1 & (1 << 27);
        next.sample_coverage = state1 & (1 << 25);
        next.polygon_offset = state1 & ((1 << 21) | (1 << 22) | (1 << 23));
        if (next.stencil || next.sample_coverage || next.polygon_offset)
            throw std::runtime_error("Source external effect uses unsupported auxiliary render state");
        output = std::move(next);
        error.clear();
        return true;
    } catch (const std::exception& exception) {
        error = exception.what();
        return false;
    }
}
}

struct SourceWorldItemDropMaterialBindingsV1::ExternalEffectV1 {
    std::vector<std::uint8_t> bytes;
    dh2::resources::BresView bres{};
};

SourceWorldItemDropMaterialBindingsV1::SourceWorldItemDropMaterialBindingsV1(
    const AssetCatalog& assets, SourceWorldItemDropTextureServicesV1 services,
    const AssetCatalog* source_effect_assets)
    : assets_(assets), source_effect_assets_(source_effect_assets),
      services_(std::move(services)) {}

SourceWorldItemDropMaterialBindingsV1::~SourceWorldItemDropMaterialBindingsV1() {
    clear_textures();
}

void SourceWorldItemDropMaterialBindingsV1::clear_textures() {
    if (services_.release)
        for (const auto& texture : textures_) services_.release(texture.second);
    textures_.clear();
}

bool SourceWorldItemDropMaterialBindingsV1::bind(
    const SourceWorldItemDropMaterialV1& request, Material& output,
    std::string& error) {
    error.clear();
    if (request.resource_uri != "data/3D/GameObjects/itemdrops.bdae" ||
        request.visual_uri.empty() || !request.authored_material ||
        !request.authored_scene_material || !request.source_image ||
        !request.source_image->bytes || !request.source_image->size)
        return fail(error, "Source itemdrop material requires its exact retained BRES/material lease");
    const auto& projected = *request.authored_material;
    const auto& source = *request.authored_scene_material;
    if (!same_material(projected, source))
        return fail(error, "Source itemdrop OriginalMaterial differs from its exact BRES Scene material");

    CommonMaterialPass common;
    const auto common_result = resolve_common_material_pass(
        *request.source_image, source.id, common, error);
    if (common_result == CommonMaterialPassResult::invalid) return false;
    dh2::scene::EffectRenderPassV4 pass;
    bool pass_ready = common_result == CommonMaterialPassResult::applied;
    if (common_result == CommonMaterialPassResult::notCommon && !source.effect_file.empty()) {
        if (!source_effect_assets_)
            return fail(error, "Authored itemdrop external effect requires its source-cache AssetCatalog lease");
        auto found = external_effects_.find(source.effect_file);
        if (found == external_effects_.end()) {
            auto effect = std::make_shared<ExternalEffectV1>();
            effect->bytes = read_content(*source_effect_assets_,
                "data/gfx/effects/" + source.effect_file);
            if (dh2_bres_open(&effect->bres, effect->bytes.data(), effect->bytes.size()) !=
                dh2::resources::BresError::ok)
                return fail(error, "Authored itemdrop external effect BRES did not validate");
            found = external_effects_.emplace(source.effect_file, std::move(effect)).first;
        }
        if (!resolve_external_effect_pass(found->second->bres, source, pass, error)) return false;
        pass_ready = true;
    }
    if (common_result == CommonMaterialPassResult::notCommon && source.effect_file.empty() &&
        !source.gles2_technique.empty()) {
        if (!dh2::scene::effect_render_pass_v4(*request.source_image, source.id.c_str(),
                source.gles2_technique.c_str(), pass, error)) return false;
        pass_ready = true;
    }
    if (pass_ready && common_result == CommonMaterialPassResult::notCommon &&
        pass.blend_equation != 0x8006)
        return fail(error, "Source itemdrop material requires an unsupported blend equation");
    if (pass_ready && common_result == CommonMaterialPassResult::notCommon &&
        (pass.stencil || pass.sample_coverage || pass.polygon_offset))
        return fail(error, "Source itemdrop effect requires unsupported stencil/coverage/offset state");

    Material next;
    std::copy_n(source.color, 4, next.color.begin());
    if (common_result == CommonMaterialPassResult::applied)
        next.sourcePass = common.state;
    else if (pass_ready) next.sourcePass =
        SourceMaterialPass{pass.blend_src, pass.blend_dst, pass.depth_function,
            pass.cull_face, pass.front_face, pass.blend, pass.depth, pass.depth_write,
            pass.cull, alpha_test_define(pass.fragment_defines)};
    next.alphaReference = source.alpha_ref;
    next.doubleSided = common_result == CommonMaterialPassResult::applied ?
        !common.state.cull : pass_ready ? !pass.cull : source.additive;
    next.additive = source.additive;
    next.transparent = common_result == CommonMaterialPassResult::applied ?
        common.state.blend : pass_ready ? pass.blend : source.additive;
    if (pass_ready) {
        const auto& vertex_shader = common_result == CommonMaterialPassResult::applied ?
            common.vertexShader : pass.vertex_file;
        const auto& vertex_defines = common_result == CommonMaterialPassResult::applied ?
            common.vertexDefines : pass.vertex_defines;
        const auto lighting = classifySourceVertexLighting(vertex_shader, vertex_defines);
        if (lighting == SourceVertexLighting::Unsupported) {
            next.sourcePass.reset();
        } else next.lightingEnabled = lighting == SourceVertexLighting::CommonLit ||
            lighting == SourceVertexLighting::DiffuseL1VertexColor;
    }

    if (!source.diffuse.empty() || !source.alpha_map.empty()) {
        if (!services_.upload || !services_.release)
            return fail(error, "Source itemdrop diffuse texture requires current renderer upload/release services");
        try {
            std::string key;
            if (!source.diffuse.empty())
                key = resolve_content_path(assets_, source.diffuse, request.resource_uri).generic_string();
            bool blue_alpha = source.effect_file == "GL_Diffuse_L1_VC_iPhone.bdae" &&
                source.gles2_technique == "L1_Vc_Al_----_----_----_----";
            if (!source.alpha_map.empty() && source.alpha_map != source.diffuse) {
                const auto alpha_path = resolve_content_path(assets_, source.alpha_map,
                                                             request.resource_uri).generic_string();
                key += "|" + alpha_path + (blue_alpha ? "|blue" : "|red");
            }
            const auto cached = textures_.find(key);
            if (cached != textures_.end()) next.texture = cached->second;
            else {
                TextureImage decoded;
                if (!source.diffuse.empty()) {
                    const auto path = resolve_content_path(assets_, source.diffuse, request.resource_uri);
                    if (!load_texture(path, decoded, error)) return false;
                } else {
                    return fail(error, "Source itemdrop alpha-only material lacks an authored diffuse texture");
                }
                if (!source.alpha_map.empty() && source.alpha_map != source.diffuse &&
                    !merge_alpha(assets_, request.resource_uri, source.alpha_map,
                                 blue_alpha, decoded, error)) return false;
                if (!decoded.width || !decoded.height ||
                    decoded.rgba.size() != std::size_t(decoded.width) * decoded.height * 4)
                    return fail(error, "Source itemdrop diffuse/alpha texture decode is incomplete");
                std::uint32_t texture = 0;
                if (!services_.upload(decoded, texture, error) || !texture) {
                    if (texture) services_.release(texture);
                    if (error.empty()) error = "Current renderer rejected source itemdrop texture pixels";
                    return false;
                }
                try { textures_.emplace(std::move(key), texture); }
                catch (...) { services_.release(texture); throw; }
                next.texture = texture;
            }
        } catch (const std::exception& exception) {
            error = exception.what();
            return false;
        }
    }
    output = std::move(next);
    error.clear();
    return true;
}

} // namespace dh::foundation::interactions
