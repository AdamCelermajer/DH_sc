#include "runtime_session_projectile_render_v1.hpp"
#include "../../content_paths.hpp"
#include "../../source_material_pass.hpp"
#include "../../texture_loader.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::effects;

namespace {
void check(bool condition, const std::string& message) {
    if (!condition) throw std::runtime_error(message);
}
}

int main(int argc, char** argv) {
    try {
        check(argc == 2, "Supply staged source asset root");
        AssetCatalog assets(argv[1]);
        unsigned fake_texture = 100;
        unsigned decoded_textures = 0;
        std::vector<std::string> source_materials;
        std::vector<std::string> source_textures;
        std::optional<SourceMaterialPass> actual_pass;
        std::array<float, 4> actual_color{};
        std::shared_ptr<const RuntimeSessionProjectileRenderFrameV1> submitted;

        RuntimeSessionProjectileRenderServicesV1 services;
        services.material = [&](const RuntimeSessionProjectileMaterialRequestV1& request,
                                Material& output, std::string& error) {
            check(request.resource_uri == "data/3D/projectiles/elemental_bolt_fire.bdae" &&
                  request.authored_material && request.source_material && request.source_image,
                  "Material callback lost actual projectile BRES/material identity");
            check(request.authored_material->id == request.source_material->id,
                  "OriginalScene and BRES material identities differ");
            CommonMaterialPass pass;
            const auto result = resolve_common_material_pass(*request.source_image,
                request.source_material->id, pass, error);
            if (result != CommonMaterialPassResult::applied) {
                if (error.empty()) error = "elemental_bolt_fire material is outside the current COMMON renderer backend";
                return false;
            }

            Material next;
            std::copy_n(request.source_material->color, 4, next.color.begin());
            next.alphaReference = request.source_material->alpha_ref;
            next.additive = request.source_material->additive;
            next.sourcePass = pass.state;
            next.transparent = pass.state.blend;
            next.doubleSided = !pass.state.cull;
            next.lightingEnabled = false;
            source_materials.push_back(request.source_material->id + ":" + pass.technique);
            actual_pass = pass.state;
            actual_color = next.color;

            if (!request.source_material->diffuse.empty()) {
                const auto path = resolve_content_path(assets,
                    request.source_material->diffuse, request.resource_uri);
                TextureImage image;
                if (!load_texture(path, image, error)) return false;
                check(image.width && image.height && image.rgba.size() ==
                    std::size_t(image.width) * image.height * 4,
                    "Original projectile diffuse image decode returned invalid RGBA pixels");
                source_textures.push_back(path.filename().string() + ":" +
                    std::to_string(image.width) + "x" + std::to_string(image.height));
                ++decoded_textures;
                next.texture = ++fake_texture; // CPU packet fixture; no GPU upload.
            }
            output = std::move(next);
            error.clear();
            return true;
        };
        services.submit = [&](std::shared_ptr<const RuntimeSessionProjectileRenderFrameV1> frame,
                              std::string& error) {
            submitted = std::move(frame);
            error.clear();
            return true;
        };

        std::unique_ptr<RuntimeSessionProjectileRenderV1> renderer;
        std::string error;
        check(RuntimeSessionProjectileRenderV1::load(assets, std::move(services),
              renderer, error), error);
        RuntimeSessionProjectileVisualV1 visual;
        visual.projectile_id = 1;
        visual.owner = 1;
        visual.source_row = 20; // Actual Mage Staff01 FireWandProjectile row.
        visual.source_model = 1;
        visual.model_uri = "data/3D/projectiles/elemental_bolt_fire.bdae";
        visual.position = {10.0f, 20.0f, 30.0f};
        visual.forward = {0.0f, 1.0f, 0.0f};
        visual.yaw_radians = 1.57079632679f;

        std::shared_ptr<const RuntimeSessionProjectileRenderFrameV1> frame;
        check(renderer->prepare({visual}, frame, error), error);
        check(frame && frame->renderer_ready && frame->unresolved.empty() &&
              frame->draws.size() == 1,
              "Actual elemental_bolt_fire did not produce a renderer-ready CPU packet");
        const auto source_frame = frame;
        const auto& draw = source_frame->draws.front();
        check(draw.projectile_id == visual.projectile_id && draw.owner == visual.owner &&
              draw.source_row == 20 && draw.source_model == 1 && draw.source_pass_ready &&
              draw.mesh && draw.mesh_lease.get() == draw.mesh && draw.source_retention,
              "Original projectile packet lost source identity, pass, geometry, or lease");
        check(draw.mesh->vertices.size() == 4 && draw.mesh->indices.size() == 6 &&
              draw.mesh->ranges.size() == 1,
              "Actual projectile OriginalScene geometry shape changed");
        const auto authored_bytes = read_content(assets, visual.model_uri);
        OriginalScene authored_scene;
        check(decode_original_scene(authored_bytes, authored_scene, error), error);
        check(draw.mesh->indices == authored_scene.mesh.indices &&
              draw.mesh->vertices.size() == authored_scene.mesh.vertices.size(),
              "Projectile packet topology differs from the independently decoded source OriginalScene");
        float u_min = std::numeric_limits<float>::infinity();
        float u_max = -std::numeric_limits<float>::infinity();
        float v_min = std::numeric_limits<float>::infinity();
        float v_max = -std::numeric_limits<float>::infinity();
        for (std::size_t i = 0; i < draw.mesh->vertices.size(); ++i) {
            const auto& packet_vertex = draw.mesh->vertices[i];
            const auto& source_vertex = authored_scene.mesh.vertices[i];
            check(packet_vertex.position.x == source_vertex.position.x &&
                  packet_vertex.position.y == source_vertex.position.y &&
                  packet_vertex.position.z == source_vertex.position.z &&
                  packet_vertex.u == source_vertex.u && packet_vertex.v == source_vertex.v,
                  "Projectile packet modified decoded source vertex/UV data");
            u_min = std::min(u_min, packet_vertex.u); u_max = std::max(u_max, packet_vertex.u);
            v_min = std::min(v_min, packet_vertex.v); v_max = std::max(v_max, packet_vertex.v);
        }
        check(draw.world[12] == 10.0f && draw.world[13] == 20.0f &&
              draw.world[14] == 30.0f && std::abs(draw.world[0]) < 0.0001f &&
              std::abs(draw.world[1] - 1.0f) < 0.0001f &&
              std::abs(draw.world[4] + 1.0f) < 0.0001f &&
              std::abs(draw.world[5]) < 0.0001f,
              "Source packet placement did not preserve the live value packet position/yaw");
        check(!source_materials.empty() && decoded_textures > 0 &&
              draw.mesh->ranges.front().material.texture != 0,
              "Actual source material/pass and diffuse texture were not CPU-decoded into packet");

        check(renderer->submit({visual}, error), error);
        check(submitted && submitted->draws.size() == 1 &&
              submitted->draws.front().mesh_lease &&
              submitted->draws.front().mesh->vertices.size() == 4,
              "Renderer callback did not receive the retained actual-source packet");

        auto unsupported = visual;
        unsupported.projectile_id = 2;
        unsupported.model_uri = "data/3D/projectiles/unclassified-source.bdae";
        check(renderer->prepare({unsupported}, frame, error), error);
        check(!frame->renderer_ready && frame->draws.empty() &&
              frame->unresolved.size() == 1 && frame->unresolved[0].projectile_id == 2,
              "Unknown source projectile URI received a fabricated model packet");
        check(!renderer->submit({unsupported}, error) && !error.empty(),
              "Unknown projectile source URI crossed renderer submission");

        std::cout << "PASS actual Mage projectile OriginalScene packet; materials=";
        for (const auto& value : source_materials) std::cout << value << ',';
        std::cout << " textures=";
        for (const auto& value : source_textures) std::cout << value << ',';
        std::cout << " vertices=" << draw.mesh->vertices.size()
                  << " indices=" << draw.mesh->indices.size()
                  << " pass_blend=" << (actual_pass && actual_pass->blend)
                  << " pass_src=" << (actual_pass ? actual_pass->blendSource : 0)
                  << " pass_dst=" << (actual_pass ? actual_pass->blendDestination : 0)
                  << " depth=" << (actual_pass && actual_pass->depthTest)
                  << " depth_write=" << (actual_pass && actual_pass->depthWrite)
                  << " cull=" << (actual_pass && actual_pass->cull)
                  << " material_color=" << actual_color[0] << ',' << actual_color[1] << ','
                  << actual_color[2] << ',' << actual_color[3]
                  << " uv=[" << u_min << ',' << u_max << ";" << v_min << ',' << v_max << ']'
                  << " CPU-only texture decode (no GPU claim)\n";
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << exception.what() << '\n';
        return 1;
    }
}
