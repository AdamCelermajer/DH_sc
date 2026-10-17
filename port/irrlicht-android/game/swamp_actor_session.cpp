#include "swamp_actor_session.hpp"

#include <cmath>
#if defined(__MINGW32__)
// The pinned Box2D 2.0.1 headers use the C finite() spelling, which MinGW
// removed from the default global namespace. Keep this compatibility local
// to those upstream headers.
#define finite(value) std::isfinite(value)
#endif
#include "actor_runtime.hpp"
#include "native_body.hpp"
#include "navigation_producers.hpp"
#include "physical_world.hpp"
#include "prince_actor.hpp"
#include "prince_character_runtime.hpp"
#include "swamp_actor_floor_bridge.hpp"
#include "visual_motion.hpp"
#if defined(__MINGW32__)
#undef finite
#endif

#include <algorithm>
#include <cstring>
#include <exception>
#include <limits>
#include <stdexcept>
#include <vector>

namespace dh2::irrlicht_game {
namespace {

constexpr std::uint64_t kSwampPlayerKey = 0x5357414d50000001ull;
constexpr float kSourceDestinationLookahead = 1000.0f;

bool finite(const float* values, std::size_t count) {
    for (std::size_t i = 0; i < count; ++i)
        if (!std::isfinite(values[i])) return false;
    return true;
}

} // namespace

struct SwampActorSession::Impl {
    struct BodyOwner {
        physical::WorldObject services{};
        navigation::PhysicalContact contact{};
        physical::NativeBody* native = nullptr;
        std::uint32_t additions = 0;
        std::uint32_t results = 0;

        BodyOwner() {
            services.context = this;
            services.test = collision_test;
            services.contact = collision_event;
            services.velocity = velocity;
        }

        static unsigned collision_test(void*, void*, const physical::Filter* a,
                                       const physical::Filter* b) {
            if (!a || !b) return 0;
            const navigation::PhysicalContact left{
                1, 0, 1, 1, *a, {}};
            const navigation::PhysicalContact right{
                1, 0, 1, 1, *b, {}};
            return dh2_nav_can_collide(&left, &right) == 1;
        }

        static void collision_event(void* context, physical::ContactEvent event,
                                    void*, const float*, unsigned) {
            auto& self = *static_cast<BodyOwner*>(context);
            if (event == physical::ContactEvent::add) ++self.additions;
            if (event == physical::ContactEvent::result) ++self.results;
        }

        static void velocity(void* context, float* output) {
            if (!output) return;
            const auto* self = static_cast<const BodyOwner*>(context);
            if (self && self->native && self->native->body) {
                const auto value = self->native->body->GetLinearVelocity();
                output[0] = value.x * 100.0f;
                output[1] = value.y * 100.0f;
            } else {
                output[0] = output[1] = 0.0f;
            }
        }

        void set_filter(const physical::CharacterBodyConfig& config) {
            contact = {1, 0, 1, 1,
                {static_cast<std::int16_t>(config.shape.group_index),
                 static_cast<std::uint16_t>(config.shape.category_bits),
                 static_cast<std::uint16_t>(config.shape.mask_bits), 1}, {}};
        }
    };

    PrinceActor* actor = nullptr;
    PrinceCharacterRuntime* character = nullptr;
    const SwampActorFloorBridge* floor = nullptr;
    SwampActorSessionConfig config{};
    physical::NativeWorld world;
    physical::NativeBody body{};
    BodyOwner body_owner;
    physical::CharacterBodyConfig body_config{};
    actor::RuntimeState runtime{};
    actor::RuntimePolicy policy{};
    actor::RuntimeResult actor_result{};
    navigation::MotionPolicy motion_policy{};
    navigation::ObstacleRegistry registry{};
    navigation::ControllerWorkspace workspace{};
    subobjects::Services virtual_services{};
    std::vector<navigation::ObstacleEntry> obstacle_entries;
    std::vector<std::uint32_t> obstacle_floors;
    std::vector<navigation::PathSegment> path_segments;
    std::vector<navigation::PathSegment> scratch_segments;
    std::vector<navigation::AvoidanceActor> scratch_actors;
    std::vector<std::uint32_t> scratch_floors;
    std::uint32_t scene_clock = 0;
    std::uint32_t frame_count = 0;
    std::uint32_t world_steps = 0;
    std::uint32_t body_service_count = 0;
    std::uint32_t body_service_trace_count = 0;
    std::array<std::uint32_t, 16> body_service_order{};
    SwampActorFrameResult latest{};
    bool body_services_bound = false;
    bool initialized = false;

    ~Impl() { shutdown(); }

    static std::uint32_t virtual_service(void*, std::uint32_t event,
                                        float* values) {
        using namespace subobjects;
        switch (event) {
        case camera_get:
            if (values) values[0] = 0.0f; // The source slice has no camera.
            return 0;
        case camera_can_move:
            return 1;
        case visual_update:
        case visual_apply_rotation:
        case visual_sync_scaling:
        case get_speed:
            return 1;
        default:
            return std::numeric_limits<std::uint32_t>::max();
        }
    }

    bool set_body_pin(bool pinned, std::string& error) {
        if (!body.body) {
            error = "Source player body is not live";
            return false;
        }
        const int status = pinned ? dh2_native_body_pin(&body)
                                  : dh2_native_body_unpin(&body);
        if (status || dh2_native_body_refresh_view(&runtime.body, &body)) {
            error = pinned ? "Source player body pin failed"
                           : "Source player body unpin failed";
            return false;
        }
        return true;
    }

    void record_body_service(SwampBodyServiceCall service) {
        ++body_service_count;
        if (body_service_trace_count < body_service_order.size())
            body_service_order[body_service_trace_count++] =
                static_cast<std::uint32_t>(service);
    }

    static bool character_body_pin(void* context, std::string& error) {
        auto& self = *static_cast<Impl*>(context);
        self.record_body_service(SwampBodyServiceCall::pin);
        return self.set_body_pin(true, error);
    }

    static bool character_body_unpin(void* context, std::string& error) {
        auto& self = *static_cast<Impl*>(context);
        self.record_body_service(SwampBodyServiceCall::unpin);
        return self.set_body_pin(false, error);
    }

    static bool character_body_stop(void* context, std::string& error) {
        auto& self = *static_cast<Impl*>(context);
        self.record_body_service(SwampBodyServiceCall::stop);
        return self.stop_source_move(error);
    }

    bool stop_source_move(std::string& error) {
        if (dh2_nav_drop_path(&runtime.path)) {
            error = "Source Move Stop could not drop the actor path";
            return false;
        }
        std::copy(runtime.subobjects.position,
                  runtime.subobjects.position + 3,
                  runtime.subobjects.destination);
        std::copy(runtime.subobjects.position,
                  runtime.subobjects.position + 3, runtime.path.target);
        runtime.controller.path_requested = 0;
        runtime.controller.heading.active = 0;
        std::fill(runtime.controller.heading.direction,
                  runtime.controller.heading.direction + 3, 0.0f);
        return true;
    }

    bool initialize(PrinceActor& prince, PrinceCharacterRuntime& source_character,
                    const SwampActorFloorBridge& source_floor,
                    const SwampActorSessionConfig& input, std::string& error) {
        error.clear();
        if (initialized || !prince.ready() || !source_character.ready() ||
            source_character.state_id() != 3 ||
            source_floor.object_path_mask() != swamp_module_zero_player_path_mask ||
            !source_floor.collision_world() || !source_floor.graph() ||
            !source_floor.graph()->node_count || !source_floor.source_floors().size() ||
            !finite(input.physics_world_bounds.data(), input.physics_world_bounds.size()) ||
            !finite(input.initial_position.data(), input.initial_position.size()) ||
            !finite(input.local_bounds.data(), input.local_bounds.size()) ||
            !finite(input.absolute_bounds.data(), input.absolute_bounds.size())) {
            error = "SWAMP source actor session dependencies or placement are invalid";
            return false;
        }
        if (input.absolute_bounds[0] > input.absolute_bounds[3] ||
            input.absolute_bounds[1] > input.absolute_bounds[4] ||
            input.absolute_bounds[2] > input.absolute_bounds[5]) {
            error = "Source player owner bounds are inverted";
            return false;
        }
        if (source_floor.source_floors().size() > 65535 ||
            source_floor.graph()->node_count > 65535) {
            error = "SWAMP source floor graph exceeds bounded player storage";
            return false;
        }

        actor = &prince;
        character = &source_character;
        floor = &source_floor;
        config = input;
        const auto* geometry = floor->collision_world();
        const auto* graph = floor->graph();
        try {
            world.load(config.physics_world_bounds.data());
        } catch (const std::exception& exception) {
            error = std::string("SWAMP NativeWorld load failed: ") + exception.what();
            shutdown();
            return false;
        }

        physical::CharacterBodyInput body_input{};
        body_input.owner = this;
        body_input.new_physical = &body;
        body_input.character_type = 1;
        body_input.is_player = 1;
        body_input.absolute_bounds[0] = config.absolute_bounds[0];
        body_input.absolute_bounds[1] = config.absolute_bounds[1];
        body_input.absolute_bounds[2] = config.absolute_bounds[3];
        body_input.absolute_bounds[3] = config.absolute_bounds[4];
        body_input.position[0] = config.initial_position[0];
        body_input.position[1] = config.initial_position[1];
        if (dh2_character_body_config(&body_config, &body_input) ||
            !body_config.enabled || !body_config.pinned || body_config.radius <= 0.0f) {
            error = "Recovered Prince player body config rejected source bounds";
            shutdown();
            return false;
        }

        const std::size_t path_capacity = std::size_t(graph->node_count) + 1;
        const std::size_t floor_capacity = floor->source_floors().size() + 1;
        obstacle_entries.assign(256, {});
        obstacle_floors.assign(floor_capacity, 0);
        path_segments.assign(path_capacity, {});
        scratch_segments.assign(path_capacity, {});
        scratch_actors.assign(1, {});
        scratch_floors.assign(floor_capacity, 0);
        registry = {obstacle_entries.data(), 0,
            static_cast<std::uint32_t>(obstacle_entries.size()),
            obstacle_floors.data(), 0,
            static_cast<std::uint32_t>(obstacle_floors.size())};
        workspace = {scratch_segments.data(),
            static_cast<std::uint32_t>(scratch_segments.size()), 0,
            scratch_actors.data(),
            static_cast<std::uint32_t>(scratch_actors.size()), 0,
            scratch_floors.data(),
            static_cast<std::uint32_t>(scratch_floors.size()), 0};
        runtime = {};
        runtime.path.segments = path_segments.data();
        runtime.path.capacity = static_cast<std::uint32_t>(path_segments.size());
        runtime.controller.validate_boundary = 1;
        if (dh2_nav_object_defaults(&runtime.object) ||
            dh2_nav_motion_policy_defaults(&motion_policy)) {
            error = "Source PF object or movement policy initialization failed";
            shutdown();
            return false;
        }
        runtime.object.user = kSwampPlayerKey;
        const navigation::ObjectInitRequest object_init{
            geometry, &runtime.object, kSwampPlayerKey,
            {config.initial_position[0], config.initial_position[1],
             config.initial_position[2]},
            body_config.radius * 100.0f, 0, 0};
        if (dh2_nav_init_object(&object_init)) {
            error = "Source PF player InitObject rejected the SWAMP start position";
            shutdown();
            return false;
        }
        const navigation::ProducerFields producer_fields{
            navigation::ProducerClass::character, 1, body_config.radius, 0,
            {config.absolute_bounds[0], config.absolute_bounds[1]},
            {config.absolute_bounds[3], config.absolute_bounds[4]}};
        const navigation::ProducerRequest producer{
            geometry, &registry, &runtime.object, kSwampPlayerKey,
            &producer_fields};
        if (dh2_nav_update_game_object(&producer)) {
            error = "Source PF player UpdatePFObject rejected the body bounds";
            shutdown();
            return false;
        }

        std::copy(runtime.object.motion.position,
                  runtime.object.motion.position + 3,
                  runtime.subobjects.position);
        std::copy(runtime.object.motion.position,
                  runtime.object.motion.position + 3,
                  runtime.subobjects.destination);
        std::copy(runtime.object.motion.position,
                  runtime.object.motion.position + 3,
                  runtime.path.target);
        std::copy(config.local_bounds.begin(), config.local_bounds.end(),
                  runtime.subobjects.local_bounds);
        std::copy(config.absolute_bounds.begin(), config.absolute_bounds.end(),
                  runtime.subobjects.absolute_bounds);
        std::copy(runtime.object.motion.position,
                  runtime.object.motion.position + 3,
                  runtime.subobjects.previous_position);
        std::copy(runtime.object.motion.position,
                  runtime.object.motion.position + 3,
                  runtime.controller.position);
        std::copy(runtime.object.motion.position,
                  runtime.object.motion.position + 3,
                  runtime.controller.destination);
        std::copy(runtime.object.motion.position,
                  runtime.object.motion.position + 3,
                  runtime.path.position);

        body_owner.set_filter(body_config);
        body_owner.native = &body;
        auto* native = world.create_character(body_config, &body_owner.services);
        if (!native) {
            error = "Source NativeWorld could not create the Prince player body";
            shutdown();
            return false;
        }
        body = {native, body_config.radius, body_config.pinned};
        if (dh2_native_body_refresh_view(&runtime.body, &body)) {
            error = "Source NativeWorld body state could not be initialized";
            shutdown();
            return false;
        }
        const PrinceCharacterBodyServices body_services{
            this, character_body_stop, character_body_pin, character_body_unpin};
        if (!character->bind_body_services(body_services, error)) {
            error = "Prince source body services could not bind to the live NativeWorld body: " + error;
            shutdown();
            return false;
        }
        body_services_bound = true;
        virtual_services = {this, virtual_service};
        if (!character->update_pose(runtime.object.motion.position, error)) {
            shutdown();
            return false;
        }
        initialized = true;
        latest.source_state = 3;
        latest.state_flags = character->state_flags();
        latest.position = {runtime.object.motion.position[0],
                           runtime.object.motion.position[1],
                           runtime.object.motion.position[2]};
        latest.source_floor = runtime.object.motion.floor;
        latest.body_pinned = body.pinned;
        latest.body_present = character->body_present();
        auto native_position = native->GetPosition();
        latest.physics_position = {native_position.x, native_position.y};
        latest.sequence = character->sequence_id();
        latest.clip = character->clip_id();
        return true;
    }

    bool run_frame(const SwampActorFrameInput& input,
                   SwampActorFrameResult& output, std::string& error) {
        error.clear();
        if (!initialized || !actor || !character || !floor ||
            !std::isfinite(input.input_x) || !std::isfinite(input.input_y) ||
            std::fabs(input.input_x) > 1.0f || std::fabs(input.input_y) > 1.0f) {
            error = "SWAMP actor frame input is invalid or the session is not initialized";
            return false;
        }
        if (input.accepted && input.input_x == 0.0f && input.input_y == 0.0f) {
            error = "Accepted SWAMP movement input must have a nonzero source heading";
            return false;
        }
        try {
            const float previous_owner[3]{runtime.subobjects.position[0],
                                          runtime.subobjects.position[1],
                                          runtime.subobjects.position[2]};
            const float previous_root[3]{actor->visual_binding().root.animated[0],
                                         actor->visual_binding().root.animated[1],
                                         actor->visual_binding().root.animated[2]};
            scene_clock += input.dt_ms;
            if (!character->scene_phase(scene_clock, error)) return false;
            world.update(input.dt_ms);
            ++world_steps;
            if (!character->update_timers(input.dt_ms, error)) return false;

            if (!character->set_input(input.input_x, input.input_y,
                                      input.accepted, error)) return false;
            const float heading[3]{input.accepted ? input.input_x : 0.0f,
                                   input.accepted ? input.input_y : 0.0f, 0.0f};
            if (dh2_nav_set_heading(&runtime.controller.heading, heading,
                                    input.accepted ? 1u : 0u)) {
                error = "Source PF movement heading rejected the input";
                return false;
            }
            runtime.rotation.heading_angle = runtime.controller.heading.angle;
            if (runtime.controller.heading.active) {
                runtime.subobjects.destination[0] =
                    runtime.subobjects.position[0] + input.input_x * kSourceDestinationLookahead;
                runtime.subobjects.destination[1] =
                    runtime.subobjects.position[1] + input.input_y * kSourceDestinationLookahead;
                runtime.subobjects.destination[2] = runtime.subobjects.position[2];
                std::copy(runtime.subobjects.destination,
                          runtime.subobjects.destination + 3, runtime.path.target);
            }
            if (!character->request_move(error) ||
                !character->update_state(input.dt_ms, error)) return false;

            const std::int32_t source_state = character->state_id();
            const std::uint32_t source_flags = character->state_flags();
            if ((source_state != 3 && source_state != 4) || !source_flags) {
                error = "SWAMP actor frame left the supported source Idle/Move state slice";
                return false;
            }
            if (source_state != 3 && source_state != 4) {
                error = "SWAMP player session only supports source Idle/Move body states";
                return false;
            }
            if (!character->animator_phase(error)) return false;

            move::Policy decoded{};
            if (dh2_move_policy(&decoded, &source_flags)) {
                error = "Source Character movement flags were rejected";
                return false;
            }
            policy = {{1, 0, 0, decoded.position_from_physics}, 1, 0, 0, 0,
                      actor::base_virtual_speed};
            actor::RuntimeRequest request{
                &runtime, body.body ? &body : nullptr,
                &actor->visual_binding(), &actor->scene(),
                floor->collision_world(), floor->graph(), &registry,
                &motion_policy, &workspace, nullptr,
                config.resolved_character_properties.data(), &policy,
                &virtual_services, nullptr, kSwampPlayerKey, source_flags,
                input.dt_ms};
            if (actor::update_actor(actor_result, request, error)) return false;
            if (!character->update_pose(runtime.subobjects.position, error))
                return false;

            ++frame_count;
            output = {};
            output.frame = frame_count;
            output.world_steps = world_steps;
            output.state_flags = source_flags;
            output.actor_phase = actor_result.phase;
            output.source_floor = runtime.object.motion.floor;
            output.body_pinned = body.pinned;
            output.body_present = character->body_present();
            output.body_service_count = body_service_count;
            std::copy(body_service_order.begin(), body_service_order.end(),
                      output.body_service_order.begin());
            output.source_path_segments = runtime.path.count;
            output.source_path_requested = runtime.controller.path_requested;
            std::copy(runtime.subobjects.position,
                      runtime.subobjects.position + 3, output.position.begin());
            const auto physical_position = body.body->GetPosition();
            output.physics_position = {physical_position.x, physical_position.y};
            output.source_state = source_state;
            output.sequence = character->sequence_id();
            output.clip = character->clip_id();
            output.path_boundary_checked = actor_result.path.boundary_checked;
            output.path_direction_valid = actor_result.path.direction_valid;
            output.desired_heading = {input.accepted ? input.input_x : 0.0f,
                                      input.accepted ? input.input_y : 0.0f};
            output.validated_heading = {runtime.controller.heading.direction[0],
                                        runtime.controller.heading.direction[1]};
            output.desired_rotation = runtime.rotation.heading_angle;
            output.current_rotation = runtime.rotation.rotation[2];
            output.body_radius_physics = body_config.radius;
            output.body_radius_source = body_config.radius * 100.0f;
            for (unsigned axis = 0; axis < 3; ++axis) {
                output.source_owner_delta[axis] =
                    runtime.subobjects.position[axis] - previous_owner[axis];
                output.animation_root_delta[axis] =
                    actor->visual_binding().root.animated[axis] - previous_root[axis];
            }
            latest = output;
            return true;
        } catch (const std::exception& exception) {
            error = std::string("SWAMP actor frame failed: ") + exception.what();
            return false;
        }
    }

    void shutdown() {
        initialized = false;
        if (character && body_services_bound)
            character->unbind_body_services();
        body_services_bound = false;
        if (body.body) {
            try { world.destroy(body.body); }
            catch (...) { body.body = nullptr; }
        }
        body = {};
        body_owner.native = nullptr;
        world.clear();
        actor = nullptr;
        character = nullptr;
        floor = nullptr;
        obstacle_entries.clear();
        obstacle_floors.clear();
        path_segments.clear();
        scratch_segments.clear();
        scratch_actors.clear();
        scratch_floors.clear();
        registry = {};
        workspace = {};
        runtime = {};
        scene_clock = frame_count = world_steps = 0;
        body_service_count = body_service_trace_count = 0;
        body_service_order = {};
        latest = {};
    }
};

SwampActorSession::SwampActorSession() : impl_(std::make_unique<Impl>()) {}
SwampActorSession::~SwampActorSession() = default;
SwampActorSession::SwampActorSession(SwampActorSession&&) noexcept = default;
SwampActorSession& SwampActorSession::operator=(SwampActorSession&&) noexcept = default;

bool SwampActorSession::initialize(PrinceActor& actor,
                                   PrinceCharacterRuntime& character,
                                   const SwampActorFloorBridge& floor,
                                   const SwampActorSessionConfig& config,
                                   std::string& error) {
    if (!impl_) {
        error = "SWAMP source actor session storage is absent";
        return false;
    }
    return impl_->initialize(actor, character, floor, config, error);
}

bool SwampActorSession::frame(const SwampActorFrameInput& input,
                              SwampActorFrameResult& output,
                              std::string& error) {
    if (!impl_) {
        error = "SWAMP source actor session storage is absent";
        return false;
    }
    return impl_->run_frame(input, output, error);
}

void SwampActorSession::shutdown() { if (impl_) impl_->shutdown(); }
bool SwampActorSession::ready() const { return impl_ && impl_->initialized; }
const SwampActorFrameResult& SwampActorSession::last_frame() const {
    static const SwampActorFrameResult empty{};
    return impl_ ? impl_->latest : empty;
}

} // namespace dh2::irrlicht_game
