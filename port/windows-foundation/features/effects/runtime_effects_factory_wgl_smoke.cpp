#include "runtime_effects_factory_v1.hpp"
#include "../../../game-data/data.hpp"
#include "../../original_actor_properties.hpp"
#include "../../platform_win32.hpp"
#include "../../renderer.hpp"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <GL/gl.h>

#include <algorithm>
#include <fstream>
#include <iostream>
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
struct RenderCpuFixture {
    Renderer& renderer;
    unsigned uploads{}, releases{}, camera_calls{}, driver_calls{}, submissions{};
    std::shared_ptr<const EffectRenderFrame> frame;
    bool upload(const TextureImage& image, std::uint32_t& id, std::string& error) {
        if (!image.width || !image.height ||
            image.rgba.size() != std::size_t(image.width) * image.height * 4) {
            error = "original decoded texture dimensions/data differ";
            return false;
        }
        id = renderer.createTexture(static_cast<int>(image.width),
                                    static_cast<int>(image.height), image.rgba.data());
        if (!id) {
            error = "Root Renderer rejected the original decoded texture";
            return false;
        }
        ++uploads;
        error.clear();
        return true;
    }
    void release(std::uint32_t id) {
        renderer.destroyTexture(id);
        ++releases;
    }
    static bool camera(void* raw, const dh2::scene::Scene&, float[16], float[3],
                       std::string& error) {
        ++static_cast<RenderCpuFixture*>(raw)->camera_calls;
        error = "Swoosh mesh fixture unexpectedly requested a particle camera";
        return false;
    }
    static bool driver(void* raw, std::uint32_t&, std::string& error) {
        ++static_cast<RenderCpuFixture*>(raw)->driver_calls;
        error = "Swoosh mesh fixture unexpectedly requested particle driver policy";
        return false;
    }
    static bool submit(void* raw, std::shared_ptr<const EffectRenderFrame> frame,
                       std::string& error) {
        auto& self = *static_cast<RenderCpuFixture*>(raw);
        self.frame = std::move(frame);
        ++self.submissions;
        error.clear();
        return true;
    }
};
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
        RenderCpuFixture cpu{renderer};
        RuntimeEffectsFactoryBindingsV1 factory_bindings;
        factory_bindings.assets = &source_effect_assets;
        factory_bindings.tables = table;
        factory_bindings.same_pf_world = &same_pf_world;
        factory_bindings.scene_view = {&cpu, RenderCpuFixture::camera, RenderCpuFixture::driver};
        factory_bindings.textures = {
            [&cpu](const TextureImage& image, std::uint32_t& id, std::string& e) {
                return cpu.upload(image, id, e);
            },
            [&cpu](std::uint32_t id) { cpu.release(id); }};
        factory_bindings.submit = [&cpu](std::shared_ptr<const EffectRenderFrame> f,
                                         std::string& e) {
            return RenderCpuFixture::submit(&cpu, std::move(f), e);
        };
        auto factory = RuntimeEffectsFactoryV1::create(session, actor, std::move(factory_bindings), error);
        check(factory != nullptr, "actual same-session FX factory creation: " + error);
        check(factory->manager().source_libraries_v63() == nullptr,
              "factory unexpectedly substituted a different App FX-library owner");

        // Same original source fx_ route as the EffectsTables name plus the
        // source prefix; no marker position, resource, or set ID is invented.
        RetainedAnimationEvent event;
        event.name = "fx_" + *set;
        event.clip_id = retained_visual->animation_name();
        event.generation = 1;
        event.wall_timestamp_ms = 0;
        const auto dispatched = factory->runtime().event(actor, event, 0, error);
        check(dispatched == DispatchResult::delivered, "original fx_ event dispatch: " + error);
        check(factory->runtime().update(1, 0, 0, error), "initial source FX frame: " + error);
        check(factory->runtime().update(2, 32, 32, error), "advanced source FX frame: " + error);

        std::shared_ptr<const EffectRenderFrame> frame;
        check(factory->runtime().prepare_render_frame(frame, error),
              "actual source FX packet preparation: " + error);
        check(frame && !frame->packets.empty(), "original source swoosh produced no packets");
        for (const auto& packet : frame->packets) {
            check(packet.source.kind == EffectDrawKind::authored_mesh,
                  "mesh-only original swoosh unexpectedly emitted a particle packet");
            check(packet.source.scene == retained_visual->retained_scene_borrow() ||
                  packet.source.resource_bytes,
                  "packet lost original resource or retained source scene");
            check(!packet.mesh.vertices.empty() && !packet.mesh.indices.empty() &&
                  packet.source_retention, "packet missing actual source geometry/lifetime loan");
            check(packet.mesh.ranges.size() == 1 && packet.mesh.ranges[0].material.sourcePass,
                  "actual original material pass not decoded");
        }
        check(cpu.camera_calls == 0 && cpu.driver_calls == 0,
              "source mesh-only test crossed into an invented particle camera/driver branch");
        check(cpu.uploads > 0, "original material decoder did not request CPU fixture upload");
        check(cpu.submissions == 0, "prepare must leave queue submission to the root caller");

        // Submit through the same immutable frame queue callback used by the
        // root integration, then draw its retained source packet with Renderer.
        const auto packet_count = frame->packets.size();
        check(RenderCpuFixture::submit(&cpu, frame, error), error);
        check(cpu.submissions == 1 && cpu.frame && cpu.frame->packets.size() == frame->packets.size(),
              "root queue callback did not retain the same source frame");
        check(cpu.frame.get() == frame.get(), "queue retained a substitute FX frame");
        std::weak_ptr<const EffectRenderFrame> queued_frame_weak = cpu.frame;
        std::weak_ptr<const void> packet_owner_weak = cpu.frame->packets.front().source_retention;
        check(!queued_frame_weak.expired() && !packet_owner_weak.expired(),
              "source frame or packet owner was not retained by the render queue");

        Vec3 minimum{std::numeric_limits<float>::max(), std::numeric_limits<float>::max(),
                     std::numeric_limits<float>::max()};
        Vec3 maximum{-minimum.x, -minimum.y, -minimum.z};
        for (const auto& packet : cpu.frame->packets) {
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
        for (const auto& packet : cpu.frame->packets)
            renderer.drawRange(packet.mesh, packet.mesh.ranges.front(), packet.world);
        renderer.endFrame();
        glFinish();
        glReadPixels(0, 0, window.width(), window.height(), GL_RGBA, GL_UNSIGNED_BYTE,
                     after.data());
        check(glGetError() == GL_NO_ERROR, "Root Renderer WGL draw/readback produced a GL error");
        std::size_t changed_pixels = 0;
        for (std::size_t i = 0; i < after.size(); i += 4)
            if (before[i] != after[i] || before[i+1] != after[i+1] ||
                before[i+2] != after[i+2]) ++changed_pixels;
        check(changed_pixels > 10, "Original source swoosh produced no framebuffer changes");
        std::ofstream ppm(argv[4], std::ios::binary);
        ppm << "P6\n" << window.width() << ' ' << window.height() << "\n255\n";
        for (int y = window.height() - 1; y >= 0; --y) {
            for (int x = 0; x < window.width(); ++x) {
                const auto offset = (std::size_t(y) * window.width() + x) * 4;
                ppm.write(reinterpret_cast<const char*>(after.data() + offset), 3);
            }
        }
        check(bool(ppm), "WGL source FX framebuffer capture write failed");
        window.swap();

        // Release host queue and prepared-frame references only after GL drain.
        cpu.frame.reset();
        frame.reset();
        check(queued_frame_weak.expired(), "source frame survived the queue drain");
        check(packet_owner_weak.expired(),
              "packet source lease survived queue drain and actor release");
        factory->runtime().release_actor(actor);
        factory->clear_original_textures();
        check(cpu.releases == cpu.uploads, "actual GL texture handles were not released after frame drain");
        factory.reset();
        check(packet_owner_weak.expired(), "source packet lease survived manager/factory teardown");

        std::cout << "{\"validation\":\"PASS\",\"session_actor\":" << actor
                  << ",\"source_set\":\"" << *set << "\",\"source_fx_event\":\""
                  << event.name << "\",\"source_packets\":" << packet_count
                  << ",\"actual_gl_uploads\":" << cpu.uploads
                  << ",\"changed_pixels\":" << changed_pixels
                  << ",\"camera_driver_requests\":0,\"native_wgl_renderer\":true"
                  << ",\"queue_drained\":true,\"packet_lease_released\":true}\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
