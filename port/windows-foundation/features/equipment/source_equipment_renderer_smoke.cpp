#include "source_equipment_appearance.hpp"
#include "source_equipment_material_binding.hpp"
#include "../../asset_catalog.hpp"
#include "../../content_paths.hpp"
#include "../../platform_win32.hpp"
#include "../../renderer.hpp"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <GL/gl.h>

#include <algorithm>
#include <iostream>
#include <limits>
#include <memory>
#include <optional>
#include <stdexcept>

using namespace dh::foundation;

namespace {
constexpr int width = 640;
constexpr int height = 480;
void check(bool ok, const std::string& error) {
    if (!ok) throw std::runtime_error(error);
}
dh2::skinning::VisualAssetResultV6 weapon_read(void* context, const char* uri,
        std::vector<std::uint8_t>& bytes, std::string& error) {
    try {
        bytes = read_content(*static_cast<const AssetCatalog*>(context), uri);
        return dh2::skinning::VisualAssetResultV6::found;
    } catch (const std::exception& e) {
        error = e.what();
        return dh2::skinning::VisualAssetResultV6::failed;
    }
}
Vec3 world_position(const Mat4& m, const Vec3& p) {
    return {m[0]*p.x+m[4]*p.y+m[8]*p.z+m[12],
            m[1]*p.x+m[5]*p.y+m[9]*p.z+m[13],
            m[2]*p.x+m[6]*p.y+m[10]*p.z+m[14]};
}
}

int main(int argc, char** argv) {
    try {
        check(argc == 2, "Supply workspace root");
        const std::filesystem::path root(argv[1]);
        Window window;
        check(window.open("DH2 source equipment WGL smoke", width, height), window.error());
        Renderer renderer;
        check(renderer.initialize(width, height), "Root Renderer did not initialize in WGL context");

        AssetCatalog body_assets(root / "port/android-native/app/src/main/assets");
        AssetCatalog weapons(root / ".local-inputs/windows-equipment-assets/original-cache");

        CharacterVisual body;
        CharacterVisualConfig config;
        config.model_path = "models/prince_modular.bdae";
        config.template_clip_path = "animations/prince_template_anim.bdae";
        config.clips = {{"idle", "animations/prince_idle_shield.bdae"},
                        {"attack", "animations/prince_1hand_combo_01.bdae"}};
        config.skin_id_contains = "_default_warrior-mesh-skin";
        config.expected_controller_count = 4;
        std::string error;
        check(body.load(body_assets, config, error), error);
        const auto* live_scene = body.retained_scene_borrow();
        check(live_scene != nullptr, "Actual source body Scene missing");

        std::unique_ptr<dh2::skinning::VisualSkinOwnerV6> skin_owner;
        SourceEquipmentImageLeaseV1 body_image;
        check(bind_source_equipment_render_skin(body_assets, body, {&weapons, weapon_read},
                skin_owner, body_image, error), error);
        check(body_image.retention && body_image.image.bytes == body_image.bytes &&
              body_image.image.size == body_image.byte_count &&
              body_image.material_scene != nullptr && body_image.resource_uri == config.model_path,
              "Body image is not a pinned exact source resource lease");

        // Attach one actual original-cache sword through the source skin owner.
        check(skin_owner->set_weapon("mc_rweapon_longsword_01", 1, 1, error), error);
        const std::string weapon_uri = skin_owner->weapon_uri(1);
        check(!weapon_uri.empty(), "Source skin owner did not retain the requested weapon URI");
        const double source_clock = body.animation_elapsed_seconds();

        effects::EffectTextureServices texture_services;
        texture_services.upload = [&](const TextureImage& decoded, std::uint32_t& texture,
                                      std::string& upload_error) {
            if (!decoded.width || !decoded.height ||
                decoded.rgba.size() != std::size_t(decoded.width) * decoded.height * 4) {
                upload_error = "Original texture decode returned invalid RGBA pixels";
                return false;
            }
            texture = renderer.createTexture(static_cast<int>(decoded.width),
                static_cast<int>(decoded.height), decoded.rgba.data());
            if (!texture) upload_error = "Root Renderer rejected an original decoded texture";
            return texture != 0;
        };
        texture_services.release = [&](std::uint32_t texture) { renderer.destroyTexture(texture); };

        std::shared_ptr<const SourceEquipmentRenderFrameV1> test_owned_queue;
        std::weak_ptr<const SourceEquipmentRenderFrameV1> queued_frame_weak;
        std::weak_ptr<const void> packet_owner_weak;
        unsigned submitted = 0;
        SourceEquipmentOriginalBindingsV1 original(weapons, body_assets,
            std::move(texture_services), [&](std::shared_ptr<const SourceEquipmentRenderFrameV1> frame,
                                              std::string& submit_error) {
                if (!frame || frame->packets.empty()) {
                    submit_error = "Equipment bridge submitted an empty immutable frame";
                    return false;
                }
                check(!test_owned_queue, "Test queue received more than one source frame");
                queued_frame_weak = frame;
                packet_owner_weak = frame->packets.front().source_retention;
                test_owned_queue = std::move(frame);
                ++submitted;
                return true;
            });
        auto render_services = original.callbacks();
        auto original_weapon_image = render_services.weapon_image;
        unsigned exact_weapon_leases = 0;
        render_services.weapon_image = [&](const dh2::skinning::VisualDrawViewV32& view,
                const std::string& uri, SourceEquipmentImageLeaseV1& lease, std::string& lease_error) {
            check(view.weapon_slot == 1 && uri == weapon_uri,
                  "Renderer requested a weapon image other than the exact live source URI");
            ++exact_weapon_leases;
            return original_weapon_image(view, uri, lease, lease_error);
        };
        std::optional<SourceEquipmentRenderBridgeV1> bridge;
        bridge.emplace(*skin_owner, body_image, std::move(render_services));
        check(bridge->submit(error), error);
        check(submitted == 1 && test_owned_queue && !test_owned_queue->packets.empty(),
              "Source equipment frame did not reach the test-owned queue");
        check(exact_weapon_leases > 0, "Original weapon BRES lease callback was not exercised");
        check(body.retained_scene_borrow() == live_scene &&
              body.animation_elapsed_seconds() == source_clock,
              "Renderer preparation changed source Scene identity or advanced its clock");

        std::size_t body_packets = 0, weapon_packets = 0;
        Vec3 minimum{std::numeric_limits<float>::max(), std::numeric_limits<float>::max(),
                     std::numeric_limits<float>::max()};
        Vec3 maximum{-minimum.x, -minimum.y, -minimum.z};
        for (const auto& packet : test_owned_queue->packets) {
            check(packet.source_retention && packet.mesh.ranges.size() == 1 &&
                  !packet.mesh.vertices.empty() && !packet.mesh.indices.empty(),
                  "Submitted packet lost its source owner or primitive draw range");
            check(packet.mesh.ranges.front().material.sourcePass.has_value(),
                  "Original material binder omitted source COMMON pass state");
            if (packet.source.weapon_slot == 1) {
                ++weapon_packets;
                check(packet.source_retention && !packet.source.geometry_id.empty(),
                      "Weapon packet lost its exact BRES/geometry retention");
            } else {
                ++body_packets;
                check(packet.source.resource_uri == config.model_path,
                      "Body packet does not refer to the source body resource");
            }
            const auto& range = packet.mesh.ranges.front();
            if (range.material.texture) check(original.texture_count() > 0,
                "Textured source material lacks a retained GL texture lease");
            for (const auto& vertex : packet.mesh.vertices) {
                const auto p = world_position(packet.world, vertex.position);
                minimum.x = std::min(minimum.x, p.x); minimum.y = std::min(minimum.y, p.y);
                minimum.z = std::min(minimum.z, p.z); maximum.x = std::max(maximum.x, p.x);
                maximum.y = std::max(maximum.y, p.y); maximum.z = std::max(maximum.z, p.z);
            }
        }
        check(body_packets > 0 && weapon_packets > 0,
              "Actual source body and original-cache weapon packets are both required");
        check(original.texture_count() > 0,
              "Original source material callback did not decode/upload any source texture");
        const auto uploaded_texture_count = original.texture_count();

        Camera camera;
        camera.target = {(minimum.x+maximum.x)*0.5f, (minimum.y+maximum.y)*0.5f,
                         (minimum.z+maximum.z)*0.5f};
        const float extent = std::max({maximum.x-minimum.x, maximum.y-minimum.y,
                                      maximum.z-minimum.z, 1.0f});
        camera.eye = {camera.target.x, camera.target.y, camera.target.z + extent*2.8f};
        camera.nearPlane = 0.01f;
        camera.farPlane = extent*20.0f;
        renderer.beginFrame(camera);
        std::vector<std::uint8_t> before(std::size_t(width)*height*4), after(before.size());
        glReadBuffer(GL_BACK);
        glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, before.data());
        for (const auto& packet : test_owned_queue->packets)
            renderer.drawRange(packet.mesh, packet.mesh.ranges.front(), packet.world);
        renderer.endFrame();
        glFinish();
        glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, after.data());
        const auto gl_error = glGetError();
        check(gl_error == GL_NO_ERROR, "Root Renderer WGL draw/readback produced a GL error");
        std::size_t changed_pixels = 0;
        for (std::size_t i = 0; i < after.size(); i += 4)
            if (before[i] != after[i] || before[i+1] != after[i+1] || before[i+2] != after[i+2])
                ++changed_pixels;
        check(changed_pixels > 10, "Original source equipment produced no framebuffer changes");
        check(body.retained_scene_borrow() == live_scene &&
              body.animation_elapsed_seconds() == source_clock,
              "WGL rendering advanced or replaced the source pose");

        // The local test queue models a host loan only for this smoke. Retain
        // its immutable packet owners through GL drain and readback, then release
        // the queue before clearing GL textures or destroying either source owner.
        check(!queued_frame_weak.expired() && !packet_owner_weak.expired(),
              "Source frame/packet owner expired before queue drain");
        test_owned_queue.reset();
        check(queued_frame_weak.expired() && packet_owner_weak.expired(),
              "Source frame/packet owner remained queued after drain");
        original.clear_textures();
        check(original.texture_count() == 0, "Original GL texture leases survived explicit clear");
        std::weak_ptr<const void> body_resource_weak = body_image.retention;
        bridge.reset();
        skin_owner.reset();
        body_image.retention.reset();
        check(body_resource_weak.expired(), "Body BRES owner survived after bridge and skin release");
        check(body.retained_scene_borrow() == live_scene,
              "Renderer smoke destroyed or replaced the caller-owned source Scene");

        std::cout << "{\"validation\":\"PASS\",\"body_packets\":" << body_packets
                  << ",\"weapon_packets\":" << weapon_packets
                  << ",\"textures\":" << uploaded_texture_count
                  << ",\"changed_pixels\":" << changed_pixels
                  << ",\"native_wgl\":true,\"source_clock_advanced_during_render\":false"
                  << ",\"production_queue_tested\":false}\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
