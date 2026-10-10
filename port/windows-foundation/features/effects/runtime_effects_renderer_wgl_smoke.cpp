#include "runtime_effects_renderer_v1.hpp"
#include "runtime_swing_fx_observer_v1.hpp"
#include "../../../game-data/data.hpp"
#include "../../original_actor_properties.hpp"
#include "../../platform_win32.hpp"
#include "../../renderer.hpp"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <GL/gl.h>

#include <algorithm>
#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::effects;

namespace {
unsigned checks{};
void check(bool value, const std::string& message) {
    ++checks;
    if (!value) throw std::runtime_error("check " + std::to_string(checks) + ": " +
                                         (message.empty() ? "condition failed" : message));
}
std::vector<std::uint8_t> read(const std::filesystem::path& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) throw std::runtime_error("missing source input: " + path.string());
    return {std::istreambuf_iterator<char>(in), {}};
}
dh2::data::Bytes bytes(const std::vector<std::uint8_t>& value) {
    return {value.data(), value.size()};
}
struct SceneRequestFixture {
    unsigned camera_calls{}, driver_calls{};
    static bool camera(void* raw, const dh2::scene::Scene&, float[16], float[3],
                       std::string& error) {
        ++static_cast<SceneRequestFixture*>(raw)->camera_calls;
        error = "Swoosh mesh fixture unexpectedly requested a particle camera";
        return false;
    }
    static bool driver(void* raw, std::uint32_t&, std::string& error) {
        ++static_cast<SceneRequestFixture*>(raw)->driver_calls;
        error = "Swoosh mesh fixture unexpectedly requested particle driver policy";
        return false;
    }
};
struct TextureDiagnostic {
    std::uint32_t width{}, height{}, handle{};
    std::array<std::uint8_t, 3> rgb_min{255,255,255};
    std::array<std::uint8_t, 3> rgb_max{0,0,0};
    bool uploaded{};
};
std::array<std::uint8_t,3> rgb_range_min(const std::vector<std::uint8_t>& rgba) {
    std::array<std::uint8_t,3> result{255,255,255};
    for (std::size_t i=0; i+2<rgba.size(); i+=4)
        for (std::size_t c=0;c<3;++c) result[c]=std::min(result[c],rgba[i+c]);
    return result;
}
std::array<std::uint8_t,3> rgb_range_max(const std::vector<std::uint8_t>& rgba) {
    std::array<std::uint8_t,3> result{0,0,0};
    for (std::size_t i=0; i+2<rgba.size(); i+=4)
        for (std::size_t c=0;c<3;++c) result[c]=std::max(result[c],rgba[i+c]);
    return result;
}
}

int main(int argc, char** argv) {
    try {
        check(argc == 5, "usage: runtime-effects-factory-wgl <workspace-root> <effects-table-dir> <source-effect-uri-root> <framebuffer.ppm>");
        const std::filesystem::path root(argv[1]);
        Window window;
        check(window.open("DH2 actual session FX WGL smoke", 640, 480), window.error());
        Renderer renderer;
        check(renderer.initialize(window.width(), window.height()),
              "Root Renderer did not initialize in the reserved WGL context");
        RuntimeEffectsRendererV1 render_queue(renderer);
        AssetCatalog assets(root / ".local-inputs/windows-shared-assets");
        // The source extraction is content-addressed separately from the shared
        // game-data tree. Its test overlay maps the exact original bytes to the
        // virtual URI consumed by the owner; no resource is regenerated.
        AssetCatalog source_effect_assets(argv[3]);
        std::string error;

        OriginalPropertyDatabase database;
        OriginalMeleeBindings melee;
        check(load_original_property_tables(assets, "original-cache/data/pydata", database, error), error);
        check(melee.load(assets, "original-melee-bindings.xml", error), error);
        ActorCustomization customization;
        customization.skin_id_contains = "_default_warrior-mesh-skin";
        customization.expected_controller_count = 4;
        customization.allow_missing_animation_targets = true;
        OriginalCombatVisualPlan visual_plan;
        check(build_original_combat_visual_plan(assets, melee, "KnightPlayerBase",
              customization, "effects-factory-native", visual_plan, error), error);

        CombatSessionConfig config;
        config.diagnosticRngSeed = 20261009;
        config.playerId = 0x10001;
        config.playerProfileId = "KnightPlayerBase";
        config.tableRoot = "original-cache/data/pydata";
        const auto item_data = assets.read(config.tableRoot + "/loot_table_pyarray.bin");
        const auto item_names = assets.read(config.tableRoot + "/loot_table_pyarraynames.bin");
        const auto item_fields = assets.read(config.tableRoot + "/loot_table_pystructnames.bin");
        dh2::data::ItemTable item_table;
        check(dh2::data::load_items(bytes(item_data), bytes(item_names), bytes(item_fields),
                                    item_table, error), error);
        check(item_table.identifiers.size() > 664, "original player weapon row missing");
        config.mainItemId = item_table.identifiers[664];
        config.equippedItemIds = {config.mainItemId};
        config.playerVisualConfig = visual_plan.config;
        config.playerVisualConfig.motion_node_id = "auto";
        config.playerVisualConfig.consume_root_motion = true;
        CombatSessionProfile profile;
        profile.action = {"AttackStatic", 0, {0, 1}};
        profile.initialIdle = {"Idle", 0, {0}};
        profile.damageMarkerNames = {"attack_mainhand"};
        profile.propertyOptions = {std::nullopt, true};
        profile.customization = customization;
        profile.motionRoot = "auto";
        OriginalAttackSelection selected_attack;
        selected_attack.state = "AttackStatic";
        selected_attack.variant = 0;
        profile.sequenceAction = selected_attack;
        profile.retainedPhaseClock = true;
        config.profiles.emplace(config.playerProfileId, std::move(profile));
        ActorPopulation population;
        CombatSessionProfile enemy_profile;
        enemy_profile.action = {"Attack", 0, {0, 1}};
        enemy_profile.initialIdle = {"Idle", 0, {0}};
        enemy_profile.death = CombatSessionChoice{"Died", 0, {0}};
        enemy_profile.damageMarkerNames = {"attack_mainhand"};
        enemy_profile.propertyOptions = {std::nullopt, true};
        enemy_profile.customization.allow_missing_animation_targets = true;
        config.profiles.emplace("Swamp_LizadMan_Type1", std::move(enemy_profile));
        PopulationActor enemy;
        enemy.profileId = "Swamp_LizadMan_Type1";
        enemy.definition.stableId = 2;
        enemy.definition.sourceId = "actual-effects-factory-target";
        enemy.definition.placement = {1,0,0,0,0,1,0,0,0,0,1,0,0,-100,0,1};
        enemy.transform = {2,0,0,0,0,2,0,0,0,0,2,0,0,-100,0,1};
        population.actors().push_back(std::move(enemy));
        CharacterVisual player;
        CombatSession session;
        check(session.initialize(assets, database, melee, config, player, population,
                                 {0, 0, 0}, customization, error), error);
        const ActorId actor = session.player_id();
        InputActions input;
        input.targetSelect = true;
        check(session.update(0, input, {0,0,0}, 0, error), "select actual session opponent: " + error);
        input.targetSelect = false;
        input.attack = true;
        check(session.update(0, input, {0,0,0}, 0, error), "start actual session attack pose: " + error);
        check(actor == config.playerId && session.retained_actor_pose(actor),
              "actual CombatSession retained actor pose missing");
        const auto* retained_visual = session.retained_actor_visual_borrow(actor);
        check(retained_visual && retained_visual == &player && retained_visual->retained_scene_borrow(),
              "factory must use this session's exact Entry.visual and Scene");

        const auto table_dir = std::filesystem::path(argv[2]);
        const auto table_records = read(table_dir / "effects_pyarray.bin");
        const auto table_names = read(table_dir / "effects_pyarraynames.bin");
        const auto table_schema = read(table_dir / "effects_pystructnames.bin");
        const auto dictionary_names = read(table_dir / "effects_dictionary_pyarraynames.bin");
        const auto dictionary_paths = read(table_dir / "effects_dictionary_pyarray.bin");
        dh2::data::EffectsTables tables;
        check(tables.load(bytes(table_records), bytes(table_names), bytes(table_schema),
                          bytes(dictionary_names), bytes(dictionary_paths), error), error);
        auto table = tables.borrow();
        const auto set = std::find(table.set_names().begin(), table.set_names().end(),
                                   "swoosh_prince_1hand_combo_01");
        check(set != table.set_names().end(), "original authored swoosh effect set absent");
        const auto set_id = static_cast<std::int32_t>(set - table.set_names().begin());
        check(set_id >= 0 && table.sets().at(static_cast<std::size_t>(set_id)).steps.size() > 0,
              "original swoosh set row has no authored steps");

        // Empty PFWorld is a real typed service object in this CPU-only owner
        // test. This mesh-only source set must not request camera/driver policy.
        dh2::navigation::CollisionWorld same_pf_world{};
        SceneRequestFixture cpu;
        RuntimeEffectsFactoryBindingsV1 factory_bindings;
        factory_bindings.assets = &source_effect_assets;
        factory_bindings.tables = table;
        factory_bindings.same_pf_world = &same_pf_world;
        factory_bindings.scene_view = {&cpu, SceneRequestFixture::camera, SceneRequestFixture::driver};
        render_queue.bind_factory_services(factory_bindings);
        TextureDiagnostic texture_diagnostic;
        const auto renderer_texture_upload = factory_bindings.textures.upload;
        factory_bindings.textures.upload = [&](const TextureImage& image,
                                                std::uint32_t& handle,
                                                std::string& upload_error) {
            texture_diagnostic.width = image.width;
            texture_diagnostic.height = image.height;
            texture_diagnostic.rgb_min = rgb_range_min(image.rgba);
            texture_diagnostic.rgb_max = rgb_range_max(image.rgba);
            const bool uploaded = renderer_texture_upload(image, handle, upload_error);
            texture_diagnostic.handle = handle;
            texture_diagnostic.uploaded = uploaded;
            return uploaded;
        };
        const auto submit_frame = factory_bindings.submit;
        auto factory = RuntimeEffectsFactoryV1::create(session, actor, std::move(factory_bindings), error);
        check(factory != nullptr, "actual same-session FX factory creation: " + error);
        check(factory->manager().source_libraries_v63() == nullptr,
              "factory unexpectedly substituted a different App FX-library owner");

        const auto clip_names = read(root / ".local-inputs/windows-shared-assets/data/animations_dictionary_pyarraynames.bin");
        const auto clip_values = read(root / ".local-inputs/windows-shared-assets/data/animations_dictionary_pyarray.bin");
        const auto animation_records = read(root / ".local-inputs/windows-shared-assets/original-cache/data/pydata/animations_pyarray.bin");
        const auto animation_names = read(root / ".local-inputs/windows-shared-assets/original-cache/data/pydata/animations_pyarraynames.bin");
        const auto animation_fields = read(root / ".local-inputs/windows-shared-assets/original-cache/data/pydata/animations_pystructnames.bin");
        dh2::data::Dictionary clips;
        dh2::data::AnimationTables animations;
        check(dh2::data::load_dictionary(bytes(clip_names), bytes(clip_values), clips, error),
              "load original AnimDict for step FX: " + error);
        check(dh2::data::load_animation_tables(bytes(animation_records), bytes(animation_names),
              bytes(animation_fields), clips, animations, error),
              "load original AnimTable for step FX: " + error);
        std::vector<RuntimeSwingFxDiagnosticV1> step_diagnostics;
        RuntimeSwingFxObserverV1 swing(session, animations, item_table, table,
            factory->manager(), [&](const RuntimeSwingFxDiagnosticV1& d) {
                step_diagnostics.push_back(d);
            });
        const auto step_observer = swing.step_entry_observer();
        std::optional<CombatSessionStepEntry> source_step_entry;
        session.set_step_entry_observer([&](const CombatSessionStepEntry& entry) {
            if (!source_step_entry && entry.sequence_id >= 0 &&
                static_cast<std::size_t>(entry.sequence_id) < animations.sequences.size() &&
                entry.step < animations.sequences[static_cast<std::size_t>(entry.sequence_id)].steps.size() &&
                animations.sequences[static_cast<std::size_t>(entry.sequence_id)].steps[entry.step].swoosh)
                source_step_entry = entry;
            step_observer(entry);
        });
        const auto manager_before_step = factory->manager().cold_creations();
        check(manager_before_step == 0, "WGL step proof requires a fresh source FX manager");
        InputActions presentation_input{};
        for (std::uint64_t frame_id = 1; frame_id != 103 && !source_step_entry; ++frame_id) {
            check(session.update(0.064, presentation_input, {0,0,0}, 0, error),
                  "advance actual CombatSession to source Swoosh step: " + error);
            check(factory->runtime().update(frame_id, static_cast<std::int32_t>((frame_id-1)*64), 64, error),
                  "advance same-session FX manager after source step: " + error);
        }
        session.clear_step_entry_observer();
        check(source_step_entry.has_value(), "actual Session never reached source Swoosh step");
        const auto step_fx = std::find_if(step_diagnostics.begin(), step_diagnostics.end(),
            [](const auto& d) { return d.dispatched && !d.source_sets.empty(); });
        check(step_fx != step_diagnostics.end(), "source AnimTable step did not dispatch its original FX set");
        const auto step_proof = *step_fx;
        check(step_proof.actor == actor && step_proof.occurrence == source_step_entry->occurrence &&
              step_proof.sequence_id == source_step_entry->sequence_id &&
              step_proof.step == source_step_entry->step &&
              step_proof.source_sets.front() == set_id,
              "actual step source-set identity differs from original set253 fixture");
        check(factory->manager().cold_creations() > manager_before_step,
              "actual source step did not create an FX instance on the fresh manager");
        const auto created_after_step = factory->manager().cold_creations();
        step_observer(*source_step_entry);
        check(!step_diagnostics.empty() &&
              step_diagnostics.back().detail == "Duplicate exact actor/step occurrence suppressed" &&
              factory->manager().cold_creations() == created_after_step,
              "WGL fixture replayed the same step occurrence into a duplicate FX instance");

        std::shared_ptr<const EffectRenderFrame> frame;
        check(factory->runtime().prepare_render_frame(frame, error),
              "actual step-created source FX packet preparation: " + error);
        check(frame && !frame->packets.empty(), "actual AnimTable step produced no source FX packets");
        for (const auto& packet : frame->packets) {
            check(packet.source.kind == EffectDrawKind::authored_mesh,
                  "mesh-only original swoosh unexpectedly emitted a particle packet");
            check(packet.source.scene != nullptr && packet.source.resource_bytes &&
                  !packet.source.resource_bytes->empty(),
                  "packet lost its original FX-resource Scene or resource bytes");
            check(!packet.mesh.vertices.empty() && !packet.mesh.indices.empty() &&
                  packet.source_retention, "packet missing actual source geometry/lifetime loan");
            check(packet.mesh.ranges.size() == 1 && packet.mesh.ranges[0].material.sourcePass,
                  "actual original material pass not decoded");
        }
        check(cpu.camera_calls == 0 && cpu.driver_calls == 0,
              "source mesh-only test crossed into an invented particle camera/driver branch");
        check(render_queue.texture_uploads() > 0,
              "original material decoder did not upload through RuntimeEffectsRendererV1");

        // Submit through the same immutable frame queue callback used by the
        // root integration, then draw its retained source packet with Renderer.
        const auto packet_count = frame->packets.size();
        check(submit_frame(frame, error), "RuntimeEffectsRendererV1 source-frame submit: " + error);
        check(render_queue.pending_frames() == 1 &&
              render_queue.pending_packets() == frame->packets.size(),
              "RuntimeEffectsRendererV1 did not retain the actual source packet");
        std::weak_ptr<const EffectRenderFrame> queued_frame_weak = frame;
        std::weak_ptr<const void> packet_owner_weak = frame->packets.front().source_retention;
        check(!queued_frame_weak.expired() && !packet_owner_weak.expired(),
              "source frame or packet owner was not retained by the render queue");

        const auto& diagnostic_packet = frame->packets.front();
        const auto& diagnostic_material = diagnostic_packet.mesh.ranges.front().material;
        float u_min=std::numeric_limits<float>::max(), v_min=u_min;
        float u_max=-u_min, v_max=-u_min;
        std::array<float,4> color_min{u_min,u_min,u_min,u_min};
        std::array<float,4> color_max{-u_min,-u_min,-u_min,-u_min};
        for (const auto& vertex : diagnostic_packet.mesh.vertices) {
            u_min=std::min(u_min,vertex.u); v_min=std::min(v_min,vertex.v);
            u_max=std::max(u_max,vertex.u); v_max=std::max(v_max,vertex.v);
            for (std::size_t c=0;c<4;++c) {
                color_min[c]=std::min(color_min[c],vertex.color[c]);
                color_max[c]=std::max(color_max[c],vertex.color[c]);
            }
        }

        Vec3 minimum{std::numeric_limits<float>::max(), std::numeric_limits<float>::max(),
                     std::numeric_limits<float>::max()};
        Vec3 maximum{-minimum.x, -minimum.y, -minimum.z};
        for (const auto& packet : frame->packets) {
            for (const auto& vertex : packet.mesh.vertices) {
                const auto& m = packet.world;
                const auto& p = vertex.position;
                const Vec3 world{m[0]*p.x+m[4]*p.y+m[8]*p.z+m[12],
                                 m[1]*p.x+m[5]*p.y+m[9]*p.z+m[13],
                                 m[2]*p.x+m[6]*p.y+m[10]*p.z+m[14]};
                minimum.x = std::min(minimum.x, world.x);
                minimum.y = std::min(minimum.y, world.y);
                minimum.z = std::min(minimum.z, world.z);
                maximum.x = std::max(maximum.x, world.x);
                maximum.y = std::max(maximum.y, world.y);
                maximum.z = std::max(maximum.z, world.z);
            }
        }
        Camera camera;
        camera.target = {(minimum.x+maximum.x)*0.5f, (minimum.y+maximum.y)*0.5f,
                         (minimum.z+maximum.z)*0.5f};
        const float extent = std::max({maximum.x-minimum.x, maximum.y-minimum.y,
                                      maximum.z-minimum.z, 1.0f});
        camera.eye = {camera.target.x, camera.target.y, camera.target.z + extent*2.8f};
        camera.nearPlane = 0.01f;
        camera.farPlane = extent*20.0f;
        renderer.beginFrame(camera);
        const auto pixel_count = std::size_t(window.width()) * window.height();
        std::vector<std::uint8_t> before(pixel_count*4), after(pixel_count*4);
        glReadBuffer(GL_BACK);
        glReadPixels(0, 0, window.width(), window.height(), GL_RGBA, GL_UNSIGNED_BYTE,
                     before.data());
        const auto clear_rgb_min=rgb_range_min(before);
        const auto clear_rgb_max=rgb_range_max(before);
        check(render_queue.draw_queued(error), "RuntimeEffectsRendererV1 draw queued: " + error);
        renderer.endFrame();
        check(render_queue.finish_and_drain(error),
              "RuntimeEffectsRendererV1 GL-finish/drain: " + error);
        glReadPixels(0, 0, window.width(), window.height(), GL_RGBA, GL_UNSIGNED_BYTE,
                     after.data());
        check(glGetError() == GL_NO_ERROR, "Root Renderer WGL draw/readback produced a GL error");
        std::size_t changed_pixels = 0;
        for (std::size_t i = 0; i < after.size(); i += 4)
            if (before[i] != after[i] || before[i+1] != after[i+1] ||
                before[i+2] != after[i+2]) ++changed_pixels;
        std::ofstream ppm(argv[4], std::ios::binary);
        ppm << "P6\n" << window.width() << ' ' << window.height() << "\n255\n";
        for (int y = window.height() - 1; y >= 0; --y) {
            for (int x = 0; x < window.width(); ++x) {
                const auto offset = (std::size_t(y) * window.width() + x) * 4;
                ppm.write(reinterpret_cast<const char*>(after.data() + offset), 3);
            }
        }
        check(bool(ppm), "WGL source FX framebuffer capture write failed");
        const auto* pass = diagnostic_material.sourcePass ? &*diagnostic_material.sourcePass : nullptr;
        const bool visible = changed_pixels > 10;
        const auto diagnostic_json = [&](std::ostream& out) {
            out << "{\"validation\":\"" << (visible ? "PASS" : "FAIL")
                << "\",\"failure_stage\":\"" << (visible ? "none" : "WGL framebuffer readback")
                << "\",\"changed_pixels\":" << changed_pixels
                << ",\"threshold\":\">10\",\"source_set_id\":"
                << step_proof.source_sets.front() << ",\"step_sequence\":"
                << step_proof.sequence_id << ",\"step_index\":" << step_proof.step
                << ",\"step_occurrence\":" << step_proof.occurrence
                << ",\"source_packets\":" << packet_count
                << ",\"source_material\":{\"texture_handle\":" << diagnostic_material.texture
                << ",\"material_color\":[" << diagnostic_material.color[0] << ','
                << diagnostic_material.color[1] << ',' << diagnostic_material.color[2] << ','
                << diagnostic_material.color[3] << "],\"additive\":"
                << (diagnostic_material.additive ? "true" : "false")
                << ",\"transparent\":" << (diagnostic_material.transparent ? "true" : "false")
                << ",\"vertex_color_min\":[" << color_min[0] << ',' << color_min[1] << ','
                << color_min[2] << ',' << color_min[3] << "],\"vertex_color_max\":["
                << color_max[0] << ',' << color_max[1] << ',' << color_max[2] << ','
                << color_max[3] << "],\"uv_min\":[" << u_min << ',' << v_min
                << "],\"uv_max\":[" << u_max << ',' << v_max << "],\"source_pass\":";
            if (pass) out << "{\"blend\":" << (pass->blend ? "true" : "false")
                << ",\"blend_source\":" << pass->blendSource
                << ",\"blend_destination\":" << pass->blendDestination
                << ",\"depth_test\":" << (pass->depthTest ? "true" : "false")
                << ",\"depth_write\":" << (pass->depthWrite ? "true" : "false")
                << ",\"cull\":" << (pass->cull ? "true" : "false")
                << ",\"alpha_test\":" << (pass->alphaTest ? "true" : "false") << '}';
            else out << "null";
            out << "},\"texture\":{\"uploaded\":" << (texture_diagnostic.uploaded ? "true" : "false")
                << ",\"handle\":" << texture_diagnostic.handle << ",\"width\":"
                << texture_diagnostic.width << ",\"height\":" << texture_diagnostic.height
                << ",\"rgb_min\":[" << unsigned(texture_diagnostic.rgb_min[0]) << ','
                << unsigned(texture_diagnostic.rgb_min[1]) << ',' << unsigned(texture_diagnostic.rgb_min[2])
                << "],\"rgb_max\":[" << unsigned(texture_diagnostic.rgb_max[0]) << ','
                << unsigned(texture_diagnostic.rgb_max[1]) << ',' << unsigned(texture_diagnostic.rgb_max[2])
                << "]},\"actual_texture_handles\":[" << diagnostic_material.texture
                << "],\"renderer_clear_rgb_min\":[" << unsigned(clear_rgb_min[0]) << ','
                << unsigned(clear_rgb_min[1]) << ',' << unsigned(clear_rgb_min[2])
                << "],\"renderer_clear_rgb_max\":[" << unsigned(clear_rgb_max[0]) << ','
                << unsigned(clear_rgb_max[1]) << ',' << unsigned(clear_rgb_max[2])
                << "],\"framebuffer_capture\":\""
                << std::filesystem::path(argv[4]).generic_string() << "\"}";
        };
        {
            std::ofstream status(std::string(argv[4]) + ".status.json", std::ios::binary);
            diagnostic_json(status);
            status << '\n';
            check(bool(status), "WGL diagnostic status sidecar write failed");
        }
        diagnostic_json(std::cout);
        std::cout << '\n' << std::flush;
        check(visible, "Original source swoosh produced no framebuffer changes");
        window.swap();

        // The helper drops its immutable queue only after GL completion. The
        // caller's prepared-frame loan is released separately below.
        check(render_queue.pending_frames() == 0 && render_queue.pending_packets() == 0,
              "RuntimeEffectsRendererV1 queue remained populated after drain");
        frame.reset();
        check(queued_frame_weak.expired(), "source frame survived the queue drain");
        check(packet_owner_weak.expired(), "source packet lease survived the final frame loan");
        factory->runtime().release_actor(actor);
        factory->clear_original_textures();
        check(render_queue.texture_releases() == render_queue.texture_uploads(),
              "actual GL texture handles were not released after frame drain");
        factory.reset();
        check(packet_owner_weak.expired(), "source packet lease survived manager/factory teardown");

        std::cout << "{\"validation\":\"PASS\",\"session_actor\":" << actor
                  << ",\"source_set\":\"" << table.set_names().at(static_cast<std::size_t>(step_proof.source_sets.front()))
                  << "\",\"fresh_manager_before_step\":" << manager_before_step
                  << ",\"step_sequence\":" << step_proof.sequence_id
                  << ",\"step_index\":" << step_proof.step
                  << ",\"step_occurrence\":" << step_proof.occurrence
                  << ",\"step_fx_set\":" << step_proof.source_sets.front()
                  << ",\"source_packets\":" << packet_count
                  << ",\"actual_gl_uploads\":" << render_queue.texture_uploads()
                  << ",\"changed_pixels\":" << changed_pixels
                  << ",\"camera_driver_requests\":0,\"native_wgl_renderer\":true"
                  << ",\"runtime_renderer_helper\":true,\"queue_drained\":true"
                  << ",\"packet_lease_released\":true}\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
