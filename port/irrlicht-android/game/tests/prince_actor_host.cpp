#include "../prince_actor.hpp"
#include "../prince_character_runtime.hpp"

#include "../../../asset-payloads/payloads.hpp"
#include "../../../engine-resources/resources.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

namespace {
void require(bool condition, const char* message) {
    if (condition) return;
    std::fprintf(stderr, "Prince Character host assertion failed: %s\n", message);
    std::exit(1);
}

void require_ok(bool condition, const std::string& error) {
    if (!condition) require(false, error.empty() ? "operation failed without detail" :
                                                  error.c_str());
}

std::vector<std::uint8_t> read(const std::string& path) {
    std::ifstream input(path, std::ios::binary);
    require(input.good(), "staged source asset must be readable");
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

using Pose = std::vector<std::vector<std::array<float, 3>>>;

Pose positions(const dh2::irrlicht_game::PrinceActor& actor) {
    Pose result;
    result.reserve(actor.parts().size());
    for (const auto& part : actor.parts()) {
        std::vector<std::array<float, 3>> vertices;
        vertices.reserve(part.vertices.size());
        for (const auto& vertex : part.vertices) vertices.push_back(vertex.position);
        result.push_back(std::move(vertices));
    }
    return result;
}

double pose_distance(const Pose& a, const Pose& b) {
    require(a.size() == b.size(), "pose primitive count must remain stable");
    double sum = 0.0;
    std::size_t components = 0;
    for (std::size_t part = 0; part < a.size(); ++part) {
        require(a[part].size() == b[part].size(), "pose vertex count must remain stable");
        for (std::size_t vertex = 0; vertex < a[part].size(); ++vertex)
            for (unsigned axis = 0; axis < 3; ++axis) {
                const double difference = static_cast<double>(a[part][vertex][axis]) -
                                          b[part][vertex][axis];
                sum += difference * difference;
                ++components;
            }
    }
    return components ? std::sqrt(sum / components) : 0.0;
}

bool frame(dh2::irrlicht_game::PrinceCharacterRuntime& runtime,
           std::uint32_t time_ms, float x, float y, bool accepted,
           const float position[3], std::string& error) {
    if (!runtime.scene_phase(time_ms, error) ||
        !runtime.update_timers(20, error) ||
        !runtime.set_input(x, y, accepted, error) ||
        !runtime.request_move(error) ||
        !runtime.update_state(20, error) ||
        !runtime.animator_phase(error) ||
        !runtime.update_pose(position, error)) return false;
    return true;
}
} // namespace

int main(int argc, char** argv) {
    require(argc == 2, "staged asset root argument is required");
    const std::string root = argv[1];
    const auto model = read(root + "/models/prince_modular.bdae");
    dh2::resources::BresView source_view{};
    require(dh2_bres_open(&source_view, model.data(), model.size()) ==
                dh2::resources::BresError::ok,
            "Prince model BRES must open");

    dh2::irrlicht_game::PrinceActor rig;
    std::string error;
    require_ok(rig.load(model.data(), model.size(), error), error);
    require(rig.ready() && rig.controller_count() == 4,
            "four source default-warrior controllers must load");
    require(rig.vertex_count() >= 400 && rig.triangle_count() >= 500 &&
                rig.joint_count() >= 20,
            "source rig geometry and joints must not collapse to a marker");

    dh2::irrlicht_game::PrinceCharacterRuntime character;
    require_ok(character.load(rig, read_asset, const_cast<char*>(root.c_str()), error),
               error);
    require(character.ready() && character.registered_resource_count() == 116 &&
                character.registration_occurrence_count() == 158,
            "runtime must load the authored complete bank and registration order");
    require(character.state_id() == 3 &&
                character.sequence_id() == character.idle_sequence_id() &&
                (character.clip_id() == 1040 || character.clip_id() == 1041),
            "Character coordinator must initialize actual source Idle playback");
    const auto initial_idle_clip = character.clip_id();
    require(character.timeline_less_resource_count() > 0,
            "no-payload source registrations must remain explicit bank entries");

    const float origin[3]{0.0f, 0.0f, 0.0f};
    std::uint32_t time_ms = 20;
    for (unsigned i = 0; i < 12; ++i, time_ms += 20)
        require_ok(frame(character, time_ms, 0.0f, 0.0f, false, origin, error), error);
    require(character.state_id() == 3 &&
                character.sequence_id() == character.idle_sequence_id() &&
                character.idle_common_update_calls() > 0,
            "source Idle state and explicit external idle-update boundary must run");
    const auto idle_pose = positions(rig);

    // The same authored source Character event/state/Facts path selects Walk.
    // This input magnitude sits between the source walk/run thresholds.
    for (unsigned i = 0; i < 20; ++i, time_ms += 20)
        require_ok(frame(character, time_ms, .65f, 0.0f, true, origin, error), error);
    require(character.state_id() == 4 &&
                character.sequence_id() == character.walk_sequence_id() &&
                character.clip_id() == 1126,
            "source Move state must select its authored Walk sequence and bank clip");
    const auto walk_clip = character.clip_id();
    const auto walk_pose = positions(rig);
    const double motion = pose_distance(idle_pose, walk_pose);
    require(std::isfinite(motion) && motion > 0.01,
            "source two-slot playback must deform the Prince between Idle and Walk");

    // Releasing input is translated into Facts.heading_active=0. The source
    // Move update raises 0x3f and the recovered state machine chooses Idle.
    for (unsigned i = 0; i < 30; ++i, time_ms += 20)
        require_ok(frame(character, time_ms, 0.0f, 0.0f, false, origin, error), error);
    require(character.state_id() == 3 &&
                character.sequence_id() == character.idle_sequence_id() &&
                (character.clip_id() == 1040 || character.clip_id() == 1041),
            "released input must return to the source Idle sequence via the FSM");
    require(character.external_state_events() >= 2,
            "state-change callbacks must be observed rather than bypassed");

    // With playback time held constant, applying the source owner a second
    // time must translate every already-skinned vertex exactly once.
    require_ok(character.update_pose(origin, error), error);
    const auto stationary = positions(rig);
    const float moved[3]{37.0f, -11.0f, 4.5f};
    require_ok(character.update_pose(moved, error), error);
    const auto translated = positions(rig);
    double translation_error = 0.0;
    std::size_t coordinates = 0;
    for (std::size_t part = 0; part < stationary.size(); ++part)
        for (std::size_t vertex = 0; vertex < stationary[part].size(); ++vertex)
            for (unsigned axis = 0; axis < 3; ++axis) {
                const double delta = translated[part][vertex][axis] -
                                     stationary[part][vertex][axis] - moved[axis];
                translation_error += delta * delta;
                ++coordinates;
            }
    const double translation_rms = coordinates
        ? std::sqrt(translation_error / coordinates) : 1.0;
    require(translation_rms < 1.0e-4,
            "source owner transform must be applied once to the skinned mesh");

    std::printf(
        "Prince source Character host: controllers=%u joints=%u vertices=%u "
        "triangles=%u bank_resources=%u registration_occurrences=%u "
        "timeline_less_resources=%u "
        "state_idle=%d state_walk=%d idle_sequence=%d walk_sequence=%d "
        "initial_idle_clip=%d walk_clip=%d "
        "current_state=%d current_sequence=%d current_clip=%d engine_clip=%d "
        "pose_delta=%.5f owner_translation_rms=%.8f idle_release=pass "
        "source_fsm=pass full_game_ai_physics_combat=not_implemented\n",
        rig.controller_count(), rig.joint_count(), rig.vertex_count(),
        rig.triangle_count(), character.registered_resource_count(),
        character.registration_occurrence_count(),
        character.timeline_less_resource_count(), character.idle_sequence_id(),
        character.walk_sequence_id(), character.idle_sequence_id(),
        character.walk_sequence_id(), initial_idle_clip, walk_clip,
        character.state_id(), character.sequence_id(),
        character.clip_id(), character.engine_clip_id(), motion, translation_rms);
    return 0;
}
