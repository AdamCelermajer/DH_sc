#include "character_faery_motion_v1.hpp"

#include "move_state.hpp"

#include <cstring>

namespace dh2::character_faery_motion_v1 {
namespace {

struct ServiceContext {
    visual::SceneBinding* binding{};
    scene::Scene* scene{};
};

// The Character virtual/camera services of GameObject::UpdateSubObjects, with
// the same answers the Player and monster runtimes give (no gameplay camera or
// auxiliary object; GetSpeed is the GameObject base 4.0 carried by the policy).
std::uint32_t virtual_service(void* raw, std::uint32_t event, float* payload) {
    using namespace subobjects;
    auto* context = static_cast<ServiceContext*>(raw);
    switch (event) {
    case camera_get:
        if (payload) payload[0] = 0;
        return 0;
    case camera_can_move:
        return 1;
    case visual_update:
    case visual_apply_rotation:
    case get_speed:
        return 1;
    case visual_sync_scaling: {
        std::string error;
        return context && context->binding && context->scene &&
                       context->binding->update_world(*context->scene, error)
            ? 1u : ~0u;
    }
    default:
        return ~0u;
    }
}

} // namespace

bool bind(State& state, const scene::Scene& pose_scene, const float position[3],
          const float quaternion[4], const float scale[3], std::string& error) {
    error.clear();
    if (state.bound) return true;
    if (!position || !quaternion || !scale) {
        error = "Faery root-motion binding requires the GameObject pose";
        return false;
    }
    if (!state.binding.bind(pose_scene, error)) return false;
    state.world_scene = pose_scene;
    std::memcpy(state.binding.root.position, position, sizeof(state.binding.root.position));
    std::memcpy(state.binding.root.quaternion, quaternion,
                sizeof(state.binding.root.quaternion));
    std::memcpy(state.binding.root.scale, scale, sizeof(state.binding.root.scale));
    state.binding.root.flags |= 8u;
    state.bound = true;
    return true;
}

bool track_root_motion(State& state, const animation::Player& clip,
                       std::int32_t dictionary, std::int32_t ms, bool replay,
                       bool displacement, std::string& error) {
    error.clear();
    if (!state.bound) {
        error = "Faery root-motion tracker is not bound to a pose scene";
        return false;
    }
    // RootSceneNode::NewAnim -> _ResetDelta on every clip (re)start, including a
    // looping clip's replay (timeline wrap) and a different AnimDict.
    const bool reset = replay || dictionary != state.tracked_dictionary ||
                       ms < state.tracked_ms;
    ++state.sample_serial;
    if (!state.binding.sample(state.world_scene, clip, ms, state.sample_serial,
                              reset, displacement, error))
        return false;
    state.tracked_dictionary = dictionary;
    state.tracked_ms = ms;
    return true;
}

void place(State& state, const float position[3]) noexcept {
    auto& runtime = state.runtime.subobjects;
    std::memcpy(runtime.position, position, sizeof(runtime.position));
    std::memcpy(runtime.destination, position, sizeof(runtime.destination));
    std::memcpy(runtime.previous_position, position, sizeof(runtime.previous_position));
    std::memcpy(state.runtime.previous_position, position,
                sizeof(state.runtime.previous_position));
    // VisualObject::SetPosition: the owner node follows immediately, so a
    // pending scene-phase displacement is applied to the new position.
    std::memcpy(state.binding.root.position, position, sizeof(state.binding.root.position));
    state.binding.root.flags |= 8u;
}

const float* placement_offset(const State& state) noexcept {
    return state.binding.root.helper;
}

const float* owner_quaternion(const State& state) noexcept {
    return state.binding.root.quaternion;
}

Status update(State& state, const Request& request, Result* output,
              std::string& error) {
    error.clear();
    if (!output || !request.controller || !request.geometry || !request.graph ||
        !request.registry || !request.motion_policy || !request.resolved224 ||
        !request.key)
        return Status::invalid_argument;
    *output = {};
    if (!state.bound) return Status::not_bound;
    auto& controller = *request.controller;
    auto& runtime = state.runtime;
    if (!state.runtime_ready) {
        runtime = {};
        place(state, request.position);
        state.runtime_ready = true;
    }

    // Marshal the Faery's canonical owners into the shared GameObject::Update
    // state. Obstacle registrations are keyed by identity, so these by-value
    // copies are written straight back below.
    controller.path.segments = controller.path_storage.data();
    controller.path.capacity = static_cast<std::uint32_t>(controller.path_storage.size());
    std::memcpy(runtime.subobjects.position, request.position,
                sizeof(runtime.subobjects.position));
    std::memcpy(runtime.subobjects.destination, controller.destination,
                sizeof(runtime.subobjects.destination));
    runtime.object = controller.navigation;
    runtime.path = controller.path;
    runtime.rotation = controller.rotation;
    runtime.subobjects.rotation = controller.rotation.rotation[2];
    runtime.controller.heading = controller.heading;
    runtime.controller.path_requested = controller.moving;
    runtime.controller.validate_boundary = 0;

    auto* native = request.body ? request.body->native_body() : nullptr;
    if (native && dh2_native_body_refresh_view(&runtime.body, native) != 0) {
        error = "Faery physical view refresh failed";
        return Status::actor_failed;
    }

    move::Policy decoded{};
    if (dh2_move_policy(&decoded, &request.character_flags) != 0) {
        error = "Faery movement policy decode failed";
        return Status::invalid_argument;
    }
    navigation::ControllerWorkspace workspace{
        controller.path_scratch.data(),
        static_cast<std::uint32_t>(controller.path_scratch.size()), 0,
        nullptr, 0, 0, nullptr, 0, 0};
    const actor::RuntimePolicy policy{{1, 0, 0, decoded.position_from_physics},
                                      0, 0, 0, 0, actor::base_virtual_speed};
    ServiceContext context{&state.binding, &state.world_scene};
    const subobjects::Services services{&context, virtual_service};
    const actor::RuntimeRequest actor_request{
        &runtime, native, &state.binding, &state.world_scene, request.geometry,
        request.graph, request.registry, request.motion_policy, &workspace,
        nullptr, request.resolved224, &policy, &services, nullptr, request.key,
        request.character_flags, request.dt_ms, &controller.moving};
    actor::RuntimeResult result{};
    std::string actor_error;
    if (actor::update_actor(result, actor_request, actor_error) != 0) {
        error = "Faery GameObject::Update failed: " + actor_error;
        return Status::actor_failed;
    }

    controller.navigation = runtime.object;
    controller.path = runtime.path;
    controller.heading = runtime.controller.heading;
    std::memcpy(controller.destination, runtime.subobjects.destination,
                sizeof(controller.destination));
    controller.rotation = runtime.rotation;

    output->actor = result;
    std::memcpy(output->position, runtime.subobjects.position,
                sizeof(output->position));
    std::memcpy(output->previous_position, runtime.previous_position,
                sizeof(output->previous_position));
    output->rotation_z = runtime.rotation.rotation[2];
    output->rotation_changed =
        runtime.previous_rotation[2] != runtime.rotation.rotation[2];
    output->visual_rotation_requested = result.visual_rotation_requested != 0;
    return Status::complete;
}

} // namespace dh2::character_faery_motion_v1
