// Host audit of the Faery root-motion owner (character_faery_motion_v1).
//
// The shipped Faery translates by VisualObject root motion, not by a physics
// velocity: faeries_celeste_walk.bdae travels its animated root node
// (root_camera) about -1327 units along local Y over its 2000 ms, and
// RootSceneNode::_HandleDisplacement moves the owner node by that delta. This
// audit loads the real celeste model and walk/idle clips and checks the
// scene-phase bookkeeping: displacement along the owner's rotated local Y,
// the per-restart delta reset, the stale-free helper offset, and that an
// MoveGo-less (idle) clip leaves the owner where the GameObject put it.
#include "../character_faery_motion_v1.hpp"
#include "../objects.hpp"

#include <cmath>
#include <cstdio>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
unsigned checks = 0;
void check(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
    ++checks;
}
bool near(float a, float b, float tolerance) { return std::fabs(a - b) <= tolerance; }

std::vector<std::uint8_t> read_file(const std::string& path) {
    std::ifstream stream(path, std::ios::binary);
    if (!stream) throw std::runtime_error("cannot open " + path);
    return {std::istreambuf_iterator<char>(stream), {}};
}
}  // namespace

int main(int argc, char** argv) {
    const std::string assets = argc > 1 ? argv[1] : "port/android-native/app/src/main/assets/actors/";
    using namespace dh2::character_faery_motion_v1;
    const auto model = read_file(assets + "faeries_02_celeste.bdae");
    const auto idle = read_file(assets + "faeries_celeste_idle.bdae");
    const auto walk_bytes = read_file(assets + "faeries_celeste_walk.bdae");
    dh2::objects::Resource resource;
    std::string error;
    check(dh2::objects::load_resource(model.data(), model.size(), idle.data(), idle.size(),
                                      resource, error), "celeste resource loads");
    dh2::animation::Player walk;
    check(walk.load(walk_bytes.data(), walk_bytes.size(), resource.rest_scene, error,
                    dh2::animation::MissingTargets::ignore), "walk clip loads");
    check(walk.end > walk.start, "walk clip has an interval");

    // Owner heading +90 degrees (VisualObject::SetRotation stores quaternion
    // z = -sin(heading/2)): the local -Y travel becomes world +X, i.e. the
    // GameObject forward (sin r, -cos r).
    const float half = std::sqrt(0.5f);
    const float position[3]{1000.f, 0.f, 255.f};
    const float quaternion[4]{0.f, 0.f, -half, half};
    const float scale[3]{1.f, 1.f, 1.f};

    auto step = [&](State& state, std::int32_t ms, bool replay, bool displacement) {
        check(dh2::objects::sample(resource, walk, ms, error), "pose samples");
        check(track_root_motion(state, walk, 7, ms, replay, displacement, error),
              "root motion tracks the sampled pose");
    };

    {
        State state;
        check(track_root_motion(state, walk, 7, 0, true, true, error) == false,
              "tracking requires a bound pose scene");
        check(bind(state, resource.scene, position, quaternion, scale, error), "binds to the pose graph");
        check(state.bound, "state reports bound");
        // First step of a clip: baseline at the clip start, then the sample.
        step(state, 0, true, true);
        check(near(state.binding.root.position[0], 1000.f, 0.5f), "no travel at the clip start");
        step(state, 500, false, true);
        check(near(state.binding.root.position[0], 1000.f + 332.2f, 2.f) &&
              near(state.binding.root.position[1], 0.f, 2.f),
              "500 ms of walk moves the owner +332 along the rotated local -Y");
        check(near(placement_offset(state)[1], 332.2f, 2.f),
              "the helper cancels the animated root translation (model stays at the owner)");
        step(state, 1000, false, true);
        step(state, 1500, false, true);
        check(near(state.binding.root.position[0], 1000.f + 995.9f, 4.f),
              "1500 ms of walk moves the owner about 996 units");
        // Clip replay: the delta restarts from the clip start (no snap back).
        const float before = state.binding.root.position[0];
        step(state, 0, true, true);
        check(near(state.binding.root.position[0], before, 1.f), "a replay restarts the delta without travel");
        step(state, 250, false, true);
        check(near(state.binding.root.position[0], before + 166.f, 2.f), "travel resumes after the restart");
        // A time wrap without an explicit replay also resets (looping timeline).
        step(state, 1900, false, true);
        const float late = state.binding.root.position[0];
        step(state, 100, false, true);
        check(near(state.binding.root.position[0], late + 66.4f, 3.f),
              "a wrapped timeline restarts the delta instead of snapping the owner back");
        // Without displacement (MoveGo clear) the owner stays where the GameObject put it.
        const float parked = state.binding.root.position[0];
        step(state, 400, false, false);
        step(state, 800, false, false);
        check(near(state.binding.root.position[0], parked, 0.01f), "no displacement leaves the owner in place");
        // GameObject::WarpTo/placement moves the owner node immediately.
        const float placed[3]{-50.f, 25.f, 255.f};
        place(state, placed);
        check(near(state.binding.root.position[0], -50.f, 0.001f) &&
              near(state.runtime.subobjects.position[1], 25.f, 0.001f),
              "placement moves the owner node and the update snapshots together");
        check(near(owner_quaternion(state)[2], -half, 1e-6f), "owner quaternion is retained");
    }
    std::printf("character_faery_motion_v1 host audit passed (%u checks)\n", checks);
    return 0;
}
