#include "../prince_actor.hpp"
#include "../prince_character_runtime.hpp"
#include "../swamp_actor_floor_bridge.hpp"
#include "../swamp_actor_session.hpp"

#include "../../../asset-payloads/payloads.hpp"
#include "../../../engine-resources/resources.hpp"
#include "../../../level-world/character_scene.hpp"
#include "../../../level-world/actor_runtime.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iterator>
#include <limits>
#include <string>
#include <vector>

namespace {
struct SnapshotHeader {
    char magic[8];
    std::uint32_t version;
    std::uint32_t surface_count;
    std::uint32_t triangle_count;
};
static_assert(sizeof(SnapshotHeader) == 20);

void require(bool condition, const char* message) {
    if (condition) return;
    std::fprintf(stderr, "SWAMP actor session assertion failed: %s\n", message);
    std::exit(1);
}

void print_trace(const char* label,
                 const dh2::irrlicht_game::SwampActorFrameResult& value) {
    std::fprintf(stderr,
        "TRACE %s pos=(%.3f,%.3f) desired=(%.3f,%.3f) validated=(%.3f,%.3f) "
        "owner_delta=(%.3f,%.3f) animation_root_delta=(%.3f,%.3f) "
        "rotation=(%.3f,%.3f) body_radius=%.3f source_floor=%u boundary=%u direction=%u\n",
        label, value.position[0], value.position[1],
        value.desired_heading[0], value.desired_heading[1],
        value.validated_heading[0], value.validated_heading[1],
        value.source_owner_delta[0], value.source_owner_delta[1],
        value.animation_root_delta[0], value.animation_root_delta[1],
        value.desired_rotation, value.current_rotation,
        value.body_radius_source, value.source_floor,
        value.path_boundary_checked, value.path_direction_valid);
}

void read_exact(std::ifstream& input, void* value, std::size_t size) {
    input.read(static_cast<char*>(value), static_cast<std::streamsize>(size));
    require(bool(input), "source floor snapshot must be complete");
}

std::vector<std::uint8_t> read(const std::string& path) {
    std::ifstream input(path, std::ios::binary);
    require(input.good(), "staged model or source animation asset must be readable");
    return {std::istreambuf_iterator<char>(input),
            std::istreambuf_iterator<char>()};
}

bool read_asset(void* context, const char* path,
                std::vector<std::uint8_t>* output, std::string* error) {
    if (!context || !path || !output || !error) return false;
    try {
        *output = read(std::string(static_cast<const char*>(context)) + "/" + path);
        return true;
    } catch (...) {
        *error = std::string("missing staged test asset: ") + path;
        return false;
    }
}

bool unused_body_service(void*, std::string&) { return true; }

void load_floor_bridge(const std::string& snapshot_path,
                       dh2::irrlicht_game::SwampActorFloorBridge& bridge) {
    std::ifstream input(snapshot_path, std::ios::binary);
    require(input.good(), "source Navigation snapshot must be readable");
    SnapshotHeader header{};
    read_exact(input, &header, sizeof(header));
    require(std::memcmp(header.magic, "DH2SWF01", 8) == 0 && header.version == 1 &&
                header.surface_count > 0 && header.surface_count <= 256 &&
                header.triangle_count > 0 && header.triangle_count <= 100000,
            "source Navigation snapshot header must be valid");
    std::vector<dh2::irrlicht_game::SwampSourceSurfaceView> surfaces(header.surface_count);
    std::vector<dh2::irrlicht_game::SwampSourceTriangleView> triangles(header.triangle_count);
    read_exact(input, surfaces.data(), surfaces.size() * sizeof(surfaces.front()));
    read_exact(input, triangles.data(), triangles.size() * sizeof(triangles.front()));
    require(input.peek() == std::char_traits<char>::eof(),
            "source Navigation snapshot must have no trailing bytes");
    const dh2::irrlicht_game::SwampSourceNavigationView view{
        surfaces.data(), header.surface_count, header.surface_count,
        triangles.data(), header.triangle_count, header.triangle_count};
    std::string error;
    require(bridge.build_module_zero(view, 0x2, error), error.c_str());
}

dh2::irrlicht_game::SwampActorSessionConfig make_config(
    dh2::irrlicht_game::PrinceActor& actor,
    dh2::irrlicht_game::PrinceCharacterRuntime& character) {
    using namespace dh2;
    using namespace dh2::irrlicht_game;
    const float initial[3]{1090.75f, -212.202f, 258.0f};
    const float origin[3]{0, 0, 0};
    std::string pose_error;
    require(character.update_pose(origin, pose_error),
            "source Prince default pose must deform before sizing the body");
    std::array<float, 6> mesh_box{
        std::numeric_limits<float>::infinity(),
        std::numeric_limits<float>::infinity(),
        std::numeric_limits<float>::infinity(),
        -std::numeric_limits<float>::infinity(),
        -std::numeric_limits<float>::infinity(),
        -std::numeric_limits<float>::infinity()};
    for (const auto& part : actor.parts()) {
        for (const auto& vertex : part.vertices) {
            for (unsigned axis = 0; axis < 3; ++axis) {
                mesh_box[axis] = std::min(mesh_box[axis], vertex.position[axis]);
                mesh_box[axis + 3] = std::max(mesh_box[axis + 3], vertex.position[axis]);
            }
        }
    }
    for (const auto value : mesh_box)
        require(std::isfinite(value), "four-mesh source Prince bounds must be finite");
    std::array<std::int32_t, 224> resolved{};
    require(character.copy_resolved_properties(resolved),
            "source Prince's resolved 224-property sheet must be available");

    physical::CharacterOwnerBoundsInput owner_input{};
    std::copy(mesh_box.begin(), mesh_box.end(), owner_input.mesh_box);
    std::copy(initial, initial + 3, owner_input.position);
    owner_input.collision_scale = resolved[16];
    physical::CharacterOwnerBounds owner_bounds{};
    require(dh2_character_owner_bounds(&owner_bounds, &owner_input) == 0,
            "source Character owner bounds producer must accept the fixture");

    SwampActorSessionConfig config{};
    // Physics space is source XY * .01. These bounds encompass the selected
    // SWAMP room and leave the real source body well inside the world.
    config.physics_world_bounds = {-100.0f, -100.0f, 100.0f, 100.0f};
    std::copy(initial, initial + 3, config.initial_position.begin());
    std::copy(owner_bounds.relative_box, owner_bounds.relative_box + 6,
              config.local_bounds.begin());
    std::copy(owner_bounds.absolute_box, owner_bounds.absolute_box + 6,
              config.absolute_bounds.begin());
    config.resolved_character_properties = resolved;
    return config;
}
} // namespace

int main(int argc, char** argv) {
    require(argc == 3, "staged asset directory and source Navigation snapshot are required");
    const std::string root = argv[1];
    auto model = read(root + "/models/prince_modular.bdae");
    dh2::irrlicht_game::PrinceActor actor;
    std::string error;
    require(actor.load(model.data(), model.size(), error), error.c_str());
    require(actor.ready() && actor.controller_count() == 4,
            "source Prince must retain its four warrior skin controllers");

    dh2::irrlicht_game::PrinceCharacterRuntime character;
    require(character.load(actor, read_asset, const_cast<char*>(root.c_str()), error),
            error.c_str());
    require(character.ready() && character.state_id() == 3 &&
                character.registered_resource_count() == 116 &&
                character.registration_occurrence_count() == 158,
            "session must consume the source Idle Character and authored bank");

    dh2::irrlicht_game::SwampActorFloorBridge floor;
    load_floor_bridge(argv[2], floor);
    auto config = make_config(actor, character);
    dh2::irrlicht_game::SwampActorSession session;
    require(session.initialize(actor, character, floor, config, error), error.c_str());
    require(session.ready() && session.last_frame().source_state == 3 &&
                session.last_frame().state_flags == 0x2380 &&
                session.last_frame().body_pinned == 1 &&
                session.last_frame().body_present == 1 &&
                character.body_present() && character.body_services_bound() &&
                session.last_frame().source_floor == 0,
            "fresh player session must attach a pinned body to the boardwalk floor");
    const dh2::irrlicht_game::PrinceCharacterBodyServices duplicate_binding{
        &character, unused_body_service, unused_body_service, unused_body_service};
    require(!character.bind_body_services(duplicate_binding, error) &&
                character.body_present() && character.body_services_bound(),
            "a second caller must not replace the session's live borrowed body callbacks");
    require(!character.load(actor, read_asset, const_cast<char*>(root.c_str()), error) &&
                character.body_present() && character.body_services_bound(),
            "source Character reload must be rejected while its NativeWorld body is attached");

    dh2::irrlicht_game::SwampActorFrameResult output{};
    const auto before_invalid = session.last_frame();
    dh2::irrlicht_game::SwampActorFrameInput invalid{};
    invalid.dt_ms = 16;
    invalid.input_x = 1.1f;
    invalid.accepted = true;
    require(!session.frame(invalid, output, error) && !error.empty(),
            "out-of-range touch input must be rejected");
    require(session.last_frame().frame == before_invalid.frame &&
                session.last_frame().world_steps == before_invalid.world_steps,
            "rejected input must not step physics or advance session state");

    dh2::irrlicht_game::SwampActorFrameInput tick{};
    tick.dt_ms = 16;
    for (unsigned i = 0; i < 2; ++i)
        require(session.frame(tick, output, error), error.c_str());
    require(output.frame == 2 && output.world_steps == 2 &&
                output.source_state == 3 && output.state_flags == 0x2380 &&
                output.body_pinned == 1 && output.actor_phase == dh2::actor::completed,
            "Idle frame ordering must run one world step and complete actor subobjects");

    tick.input_x = 0.65f;
    tick.accepted = true;
    float start_x = output.position[0];
    unsigned moving_frames = 0;
    for (; moving_frames < 60; ++moving_frames) {
        require(session.frame(tick, output, error), error.c_str());
        if (output.source_state == 4 && moving_frames > 3) break;
    }
    if (!(output.source_state == 4 && output.state_flags == 0x23c1 &&
          output.clip == 1126 && output.body_pinned == 0 &&
          output.source_path_requested == 1 && output.source_path_segments > 0 &&
          output.path_boundary_checked && output.path_direction_valid)) {
        std::fprintf(stderr,
            "Move values state=%d flags=0x%X clip=%d pinned=%u path_requested=%u path_segments=%u boundary=%u direction=%u frames=%u world_steps=%u\n",
            output.source_state, output.state_flags, output.clip,
            output.body_pinned, output.source_path_requested,
            output.source_path_segments, output.path_boundary_checked,
            output.path_direction_valid, output.frame, output.world_steps);
    }
    require(output.source_state == 4 && output.state_flags == 0x23c1 &&
                output.clip == 1126 && output.body_pinned == 0 &&
                output.body_present == 1 && output.body_service_count == 1 &&
                output.body_service_order[0] == static_cast<std::uint32_t>(
                    dh2::irrlicht_game::SwampBodyServiceCall::unpin) &&
                output.source_path_requested == 1,
            "held input must select source Move/Walk and unpin the native body");
    for (unsigned i = 0; i < 8; ++i) {
        require(session.frame(tick, output, error), error.c_str());
    }
    require(output.source_state == 4 && output.body_pinned == 0 &&
                std::fabs(output.position[0] - start_x) > 0.1f &&
                output.source_floor == 0 && output.source_path_requested == 1 &&
                output.path_boundary_checked && output.path_direction_valid &&
                output.desired_heading[0] > 0.6f &&
                std::fabs(output.validated_heading[0]) > 0.7f &&
                output.source_owner_delta[0] > 0.1f,
            "source movement must advance with a checked direction on the real boardwalk floor");

    tick.input_x = tick.input_y = 0.0f;
    tick.accepted = false;
    for (unsigned i = 0; i < 80 && output.source_state != 3; ++i)
        require(session.frame(tick, output, error), error.c_str());
    require(output.source_state == 3 && output.state_flags == 0x2380 &&
                output.body_pinned == 1 && session.last_frame().world_steps ==
                    session.last_frame().frame && output.source_path_segments == 0 &&
                output.source_path_requested == 0 && output.body_present == 1 &&
                output.body_service_count == 3 &&
                output.body_service_order[0] == static_cast<std::uint32_t>(
                    dh2::irrlicht_game::SwampBodyServiceCall::unpin) &&
                output.body_service_order[1] == static_cast<std::uint32_t>(
                    dh2::irrlicht_game::SwampBodyServiceCall::stop) &&
                output.body_service_order[2] == static_cast<std::uint32_t>(
                    dh2::irrlicht_game::SwampBodyServiceCall::pin),
            "released input must return to Idle, clear its source path and repin the body");

    const auto after_x_stop = output;
    unsigned free_y_frames = 0;
    unsigned redirected_y_frames = 0;
    unsigned redirected_x_motion_frames = 0;
    dh2::irrlicht_game::SwampActorFrameResult free_y_sample{};
    dh2::irrlicht_game::SwampActorFrameResult redirected_y_sample{};
    dh2::irrlicht_game::SwampActorFrameResult redirected_x_sample{};
    tick.input_x = 0.0f;
    tick.input_y = 0.65f;
    tick.accepted = true;
    for (unsigned i = 0; i < 18; ++i) {
        require(session.frame(tick, output, error), error.c_str());
        const bool validated_y = std::fabs(output.validated_heading[1]) > 0.7f &&
                                 std::fabs(output.validated_heading[0]) < 0.3f;
        const bool moved_y = output.source_owner_delta[1] > 0.1f &&
                             std::fabs(output.source_owner_delta[0]) < 2.5f;
        const bool redirected_x = output.validated_heading[0] > 0.7f &&
                                  std::fabs(output.validated_heading[1]) < 0.3f;
        if (output.source_state == 4 && validated_y && moved_y) {
            ++free_y_frames;
            if (free_y_frames == 1) free_y_sample = output;
        }
        if (output.source_state == 4 && redirected_x) {
            ++redirected_y_frames;
            if (redirected_y_frames == 1) redirected_y_sample = output;
            if (output.source_owner_delta[0] > 0.1f) {
                ++redirected_x_motion_frames;
                if (redirected_x_motion_frames == 1) redirected_x_sample = output;
            }
        }
    }
    print_trace("X_STOP", after_x_stop);
    print_trace("Y_FREE", free_y_sample);
    print_trace("Y_EDGE_REDIRECT", redirected_y_sample);
    print_trace("Y_BOUNDARY_SLIDE", redirected_x_sample);
    require(std::isfinite(output.body_radius_source) &&
                output.body_radius_source > 0.0f &&
                std::isfinite(output.body_radius_physics) &&
                output.body_radius_physics > 0.0f,
            "axis sequence must use the source-derived nonzero player collision radius");
    require(free_y_frames > 0 && redirected_y_frames > 0 &&
                redirected_x_motion_frames > 0 &&
                output.body_present == 1 && output.body_pinned == 0 &&
                output.body_service_count == 4 &&
                output.body_service_order[3] == static_cast<std::uint32_t>(
                    dh2::irrlicht_game::SwampBodyServiceCall::unpin) &&
                free_y_sample.desired_heading[1] > 0.6f &&
                free_y_sample.validated_heading[1] > 0.7f &&
                redirected_y_sample.path_boundary_checked == 1 &&
                redirected_y_sample.desired_heading[1] > 0.6f &&
                redirected_y_sample.validated_heading[0] > 0.7f &&
                redirected_x_sample.source_owner_delta[0] > 0.1f,
            "real SWAMP floor must permit free +Y motion then redirect the actor along the boardwalk edge");

    const auto frames = output.frame;
    const auto steps = output.world_steps;
    session.shutdown();
    session.shutdown();
    require(!session.ready() && !character.body_present() &&
                !character.body_services_bound(),
            "session teardown must detach its body callbacks and clear body_present before freeing the body");
    require(character.set_input(0.0f, 0.0f, false, error) &&
                character.update_state(16, error) && character.state_id() == 3,
            "Character updates after session teardown must not call the released body owner");
    std::printf(
        "{\"validation\":\"PASS\",\"frames\":%u,\"world_steps\":%u,"
        "\"moving_frames\":%u,\"source_state_idle\":3,\"source_state_move\":4,"
        "\"idle_flags\":9088,\"move_flags\":9153,\"idle_body_pinned\":true,"
        "\"move_body_unpinned\":true,\"move_direction_checked_while_moving\":true,"
        "\"source_body_present_bound\":true,\"move_focus_unpins_body\":true,"
        "\"move_blur_call_order\":[\"stop\",\"pin\"],"
        "\"body_service_calls_before_shutdown\":4,"
        "\"duplicate_body_binding_rejected\":true,\"live_character_reload_rejected\":true,"
        "\"body_services_detached_before_world_teardown\":true,"
        "\"post_teardown_character_update_safe\":true,"
        "\"released_input_returns_idle_and_pins_body\":true,"
        "\"source_path_segments_after_release\":0,\"source_path_requested_after_release\":0,"
        "\"actual_source_properties\":true,\"body_radius_source\":%.3f,"
        "\"resolved_collision_scale_property\":%d,"
        "\"source_x_stop_y_free_frames\":%u,\"source_y_boundary_redirect_frames\":%u,"
        "\"source_y_redirected_x_motion_frames\":%u,"
        "\"source_y_free_axis_then_boundary_slide\":true,"
        "\"floor\":0,\"runtime\":\"authored Character Coordinator + 116/158 source bank + BlendedPlayback + actor_runtime + one NativeWorld Step per frame\","
        "\"scope\":\"host-only module-zero SWAMP session; no APK or Android runtime claim\"}\n",
        frames, steps, moving_frames, output.body_radius_source,
        config.resolved_character_properties[16],
        free_y_frames, redirected_y_frames, redirected_x_motion_frames);
    return 0;
}
