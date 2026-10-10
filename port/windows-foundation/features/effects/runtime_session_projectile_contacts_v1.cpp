#include "runtime_session_projectile_contacts_v1.hpp"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <exception>
#include <stdexcept>
#include <utility>

namespace dh::foundation::effects {
namespace {

bool fail(std::string& error, const char* why) {
    error = why;
    return false;
}

bool source_filters_accept(const dh2::physical::Filter& own,
                           const dh2::physical::Filter& peer) {
    if (own.present != 1 || peer.present != 1) return false;
    if (own.group != 0 && own.group == peer.group) return own.group > 0;
    return (own.mask & peer.category) != 0 &&
           (peer.mask & own.category) != 0;
}

struct ContactFailure final : std::runtime_error {
    explicit ContactFailure(const std::string& what) : std::runtime_error(what) {}
};

} // namespace

bool runtime_session_projectile_body_plan_v1(
    const RuntimeSessionProjectileRenderV1& renderer,
    const RuntimeSessionProjectileVisualV1& visual, bool source_no_collisions,
    RuntimeSessionProjectileBodyPlanV1& output, std::string& error) {
    if (visual.projectile_id == 0 || visual.owner == invalid_actor_id ||
        visual.model_uri.empty() || !std::isfinite(visual.position[0]) ||
        !std::isfinite(visual.position[1]) || !std::isfinite(visual.position[2]))
        return fail(error, "Projectile physical plan requires a live source visual packet");
    RuntimeSessionProjectileBodyPlanV1 plan;
    if (!renderer.source_projectile_bounds_v1(visual.model_uri,
            plan.source_bounds_min, plan.source_bounds_max, error)) return false;
    const float width = plan.source_bounds_max[0] - plan.source_bounds_min[0];
    const float height = plan.source_bounds_max[1] - plan.source_bounds_min[1];
    if (!std::isfinite(width) || !std::isfinite(height) || width < 0 || height < 0)
        return fail(error, "Actual projectile BDAE absolute AABB has invalid XY extents");
    plan.radius = (std::max(width, height) * 0.5f) * 0.01f;
    plan.physics_position = {visual.position[0] * 0.01f,
                             visual.position[1] * 0.01f};
    if (!std::isfinite(plan.radius) || plan.radius <= 0 ||
        !std::isfinite(plan.physics_position[0]) ||
        !std::isfinite(plan.physics_position[1]))
        return fail(error, "Actual projectile BDAE AABB produced invalid native body geometry");
    plan.group_index = source_no_collisions ? static_cast<std::int16_t>(-666) : 0;
    plan.category_bits = 32;
    plan.mask_bits = 0x51f;
    output = plan;
    error.clear();
    return true;
}

struct RuntimeSessionProjectileContactBodiesV1::Entry {
    RuntimeSessionProjectileContactBodiesV1* owner{};
    std::uint64_t projectile_id{};
    ActorId projectile_owner{invalid_actor_id};
    RuntimeSessionProjectileBodyPlanV1 plan{};
    dh2::physical::NativeBody body{};
    dh2::physical::WorldObject transport{};
    std::array<float, 3> source_position{};

    Entry(RuntimeSessionProjectileContactBodiesV1* parent,
          const RuntimeSessionProjectileVisualV1& visual,
          const RuntimeSessionProjectileBodyPlanV1& body_plan)
        : owner(parent), projectile_id(visual.projectile_id),
          projectile_owner(visual.owner), plan(body_plan),
          source_position(visual.position) {
        transport = {this, &RuntimeSessionProjectileContactBodiesV1::test,
            &RuntimeSessionProjectileContactBodiesV1::contact,
            &RuntimeSessionProjectileContactBodiesV1::velocity, {0,0}, 0, 0};
    }
};

RuntimeSessionProjectileContactBodiesV1::RuntimeSessionProjectileContactBodiesV1(
    CombatSession& session, RuntimeSessionProjectileV1& runtime,
    dh2::physical::NativeWorld& world,
    RuntimeSessionProjectileContactServicesV1 services)
    : session_(session), runtime_(runtime), world_(world),
      services_(std::move(services)), lifetime_(session.lifetime_lease()),
      binding_lease_(session.actor_binding_lease()) {}

RuntimeSessionProjectileContactBodiesV1::~RuntimeSessionProjectileContactBodiesV1() {
    std::string error;
    if (!clear(error)) std::terminate();
}

bool RuntimeSessionProjectileContactBodiesV1::same_lease(
    const std::weak_ptr<const void>& a,
    const std::weak_ptr<const void>& b) noexcept {
    return !a.expired() && !b.expired() &&
        !a.owner_before(b) && !b.owner_before(a);
}

bool RuntimeSessionProjectileContactBodiesV1::session_alive() const noexcept {
    const auto lifetime = lifetime_.lock();
    return lifetime && lifetime->alive();
}

bool RuntimeSessionProjectileContactBodiesV1::synchronize(std::string& error) {
    if (!session_alive()) {
        std::string cleanup;
        if (!clear_entries(cleanup)) {
            error = cleanup;
            return false;
        }
        return fail(error, "Projectile contact owner CombatSession was destroyed");
    }
    const auto current_lease = session_.actor_binding_lease();
    if (!same_lease(binding_lease_, current_lease)) {
        if (!clear_entries(error)) return false;
        binding_lease_ = current_lease;
    }
    if (!runtime_.synchronize_session_binding(error)) {
        if (error.empty()) error = "Projectile contact owner runtime binding is stale";
        return false;
    }
    error.clear();
    return true;
}

bool RuntimeSessionProjectileContactBodiesV1::register_projectile(
    std::uint64_t projectile_id,
    const RuntimeSessionProjectileRenderV1& renderer,
    std::uintptr_t& source_identity, std::string& error) {
    source_identity = 0;
    if (!synchronize(error)) return false;
    if (!projectile_id || entries_.count(projectile_id))
        return fail(error, "Projectile contact registration requires a fresh runtime ID");
    if (!world_.backend() || !world_.cleanup_delivery_idle_v106())
        return fail(error, "Projectile body registration requires an idle loaded same NativeWorld");
    if (!services_.resolve_peer_actor || !services_.peer_visible ||
        !services_.source_no_collisions)
        return fail(error, "Projectile body requires source peer/visibility/MP_NoCollisions providers");

    const auto visuals = runtime_.visuals();
    const auto visual = std::find_if(visuals.begin(), visuals.end(),
        [&](const auto& value) { return value.projectile_id == projectile_id; });
    if (visual == visuals.end())
        return fail(error, "Projectile body registration has no current live runtime visual");
    bool no_collisions = false;
    if (!services_.source_no_collisions(no_collisions, error)) {
        if (error.empty()) error = "Original projectile MP_NoCollisions query failed";
        return false;
    }
    RuntimeSessionProjectileBodyPlanV1 plan;
    if (!runtime_session_projectile_body_plan_v1(renderer, *visual,
            no_collisions, plan, error)) return false;

    auto entry = std::make_unique<Entry>(this, *visual, plan);
    const auto identity = reinterpret_cast<std::uintptr_t>(entry.get());
    if (!identity || source_identities_.count(identity))
        return fail(error, "Projectile source identity token is invalid or already registered");

    dh2::physical::CharacterBodyConfig config{};
    config.enabled = 1;
    config.po_character = 0;
    config.body.user_data = entry.get();
    config.body.position[0] = plan.physics_position[0];
    config.body.position[1] = plan.physics_position[1];
    config.body.allow_sleep = 1;
    config.body.is_sleeping = 1;
    config.body.fixed_rotation = 1;
    config.body.bullet = 1;
    config.shape.user_data = entry.get();
    config.shape.kind = 0;
    config.shape.sensor = 1;
    config.shape.friction = 1.0f;
    constexpr std::uint32_t density_bits = 0x4133d70a;
    std::memcpy(&config.shape.density, &density_bits, sizeof(density_bits));
    config.shape.radius = plan.radius;
    config.shape.group_index = plan.group_index;
    config.shape.category_bits = plan.category_bits;
    config.shape.mask_bits = plan.mask_bits;
    config.radius = plan.radius;

    auto* entry_pointer = entry.get();
    entries_.emplace(projectile_id, std::move(entry));
    try {
        entry_pointer->body = {world_.create_character(config,
            &entry_pointer->transport), plan.radius, 0};
    } catch (const std::exception& failure) {
        entries_.erase(projectile_id);
        error = failure.what();
        return false;
    }
    if (!entry_pointer->body.body) {
        entries_.erase(projectile_id);
        return fail(error, "Same NativeWorld refused the source projectile sensor");
    }
    if (!runtime_.attach_native_body(projectile_id, error)) {
        world_.destroy(entry_pointer->body.body);
        entries_.erase(projectile_id);
        return false;
    }
    source_identities_.emplace(identity, projectile_id);
    source_identity = identity;
    error.clear();
    return true;
}

unsigned RuntimeSessionProjectileContactBodiesV1::test(
    void* raw, void* peer, const dh2::physical::Filter* own,
    const dh2::physical::Filter* other_filter) {
    auto& entry = *static_cast<Entry*>(raw);
    auto& owner = *entry.owner;
    if (!owner.session_alive())
        throw ContactFailure("Projectile contact owner CombatSession was destroyed");
    if (!own || !other_filter) throw ContactFailure("Missing source projectile collision filters");
    if (!source_filters_accept(*own, *other_filter) || !peer) return 0;
    ActorId peer_actor = invalid_actor_id;
    bool recognized = false;
    std::string error;
    if (!owner.services_.resolve_peer_actor(peer, peer_actor, recognized, error))
        throw ContactFailure(error.empty() ? "Actual same-Session projectile peer resolution failed" : error);
    if (!recognized) return 0; // Non-Character Activate remains a separate source path.
    if (!peer_actor)
        throw ContactFailure("Recognized projectile Character peer has invalid Session ActorId");
    bool visible = false;
    if (!owner.services_.peer_visible(peer_actor, visible, error))
        throw ContactFailure(error.empty() ? "Actual projectile peer visible80 query failed" : error);
    if (!visible) return 0;
    RuntimeSessionProjectileContactV1 event;
    event.projectile_id = entry.projectile_id;
    event.peer = peer_actor;
    event.point = entry.source_position;
    event.character_peer = true;
    bool accepted = false;
    if (!owner.runtime_.contact(event, accepted, error))
        throw ContactFailure(error.empty() ? "Same-Session projectile contact was rejected" : error);
    return 0; // Original POProjectile test consumes the source event, not response.
}

void RuntimeSessionProjectileContactBodiesV1::contact(
    void* raw, dh2::physical::ContactEvent event, void* peer,
    const float* point, unsigned) {
    // The source projectile class consumes overlap in its collision filter
    // test, which runs during ShouldCollide and rejects physical response.
    // Contact Begin/Persist should therefore be unreachable for this sensor.
    if (event == dh2::physical::ContactEvent::remove ||
        event == dh2::physical::ContactEvent::result) return;
    if (event != dh2::physical::ContactEvent::add &&
        event != dh2::physical::ContactEvent::persist)
        throw ContactFailure("Unknown NativeWorld projectile contact event");
    (void)raw;
    (void)peer;
    (void)point;
}

void RuntimeSessionProjectileContactBodiesV1::velocity(void* raw, float* output) {
    auto& entry = *static_cast<Entry*>(raw);
    dh2::physical::NativeBodyObservation observed{};
    if (!output || dh2_native_body_observe(&observed, &entry.body))
        throw ContactFailure("Required actual projectile NativeBody velocity");
    output[0] = observed.linear_velocity[0] * 100.0f;
    output[1] = observed.linear_velocity[1] * 100.0f;
}

bool RuntimeSessionProjectileContactBodiesV1::after_world_step(
    std::uint64_t source_frame, std::string& error) {
    if (!synchronize(error)) return false;
    if (!source_frame || !world_.backend() ||
        !world_.cleanup_delivery_idle_v106())
        return fail(error, "Projectile post-Step sync requires an idle same NativeWorld");
    if (has_step_frame_ && source_frame <= last_step_frame_)
        return fail(error, "Projectile contact bridge requires an increasing source frame");
    has_step_frame_ = true;
    last_step_frame_ = source_frame;
    for (const auto& pair : entries_) {
        const auto& entry = *pair.second;
        dh2::physical::NativeBodyObservation observed{};
        if (dh2_native_body_observe(&observed, &entry.body))
            return fail(error, "Projectile body observation failed after NativeWorld Step");
        auto position = entry.source_position;
        position[0] = observed.position[0] * 100.0f;
        position[1] = observed.position[1] * 100.0f;
        if (!runtime_.publish_native_body_position(entry.projectile_id,
                position, error)) return false;
    }
    error.clear();
    return true;
}

bool RuntimeSessionProjectileContactBodiesV1::after_runtime_update(
    std::string& error) {
    if (!synchronize(error)) return false;
    if (!world_.backend() || !world_.cleanup_delivery_idle_v106())
        return fail(error, "Projectile velocity publication requires idle same NativeWorld");
    const auto visuals = runtime_.visuals();
    std::map<std::uint64_t, RuntimeSessionProjectileVisualV1> active;
    for (const auto& visual : visuals) active.emplace(visual.projectile_id, visual);
    for (auto it = entries_.begin(); it != entries_.end();) {
        const auto found = active.find(it->first);
        if (found == active.end()) {
            const auto id = it->first;
            ++it;
            if (!remove_entry(id, false, error)) return false;
            continue;
        }
        auto& entry = *it->second;
        std::array<float, 2> velocity_xy{};
        if (!runtime_.native_body_velocity(entry.projectile_id, velocity_xy, error))
            return false;
        const float velocity_value[2]{velocity_xy[0], velocity_xy[1]};
        if (dh2_native_body_set_linear(&entry.body, velocity_value) != 0)
            return fail(error, "Source projectile velocity command failed on its same NativeWorld body");
        entry.source_position = found->second.position;
        ++it;
    }
    error.clear();
    return true;
}

bool RuntimeSessionProjectileContactBodiesV1::resolve_source_identity(
    std::uintptr_t source_identity, std::uint64_t& projectile_id,
    std::string& error) const {
    projectile_id = 0;
    if (!session_alive())
        return fail(error, "Projectile source identity belongs to a destroyed Session");
    if (!same_lease(binding_lease_, session_.actor_binding_lease()))
        return fail(error, "Projectile source identity belongs to a stale Session binding");
    const auto found = source_identities_.find(source_identity);
    if (found == source_identities_.end())
        return fail(error, "Projectile source identity is stale or unregistered");
    projectile_id = found->second;
    error.clear();
    return true;
}

bool RuntimeSessionProjectileContactBodiesV1::remove_entry(
    std::uint64_t projectile_id, bool detach_runtime, std::string& error) {
    const auto found = entries_.find(projectile_id);
    if (found == entries_.end())
        return fail(error, "Projectile body identity is not registered");
    if (!world_.cleanup_delivery_idle_v106())
        return fail(error, "Cannot unregister projectile sensor during NativeWorld delivery");
    if (detach_runtime && session_alive() &&
        same_lease(binding_lease_, session_.actor_binding_lease()) &&
        !runtime_.detach_native_body(projectile_id, error)) return false;
    const auto identity = reinterpret_cast<std::uintptr_t>(found->second.get());
    if (found->second->body.body && world_.backend())
        world_.destroy(found->second->body.body);
    source_identities_.erase(identity);
    entries_.erase(found); // Drops source-generation/contact identity lease.
    error.clear();
    return true;
}

bool RuntimeSessionProjectileContactBodiesV1::unregister_projectile(
    std::uint64_t projectile_id, std::string& error) {
    if (!synchronize(error)) return false;
    return remove_entry(projectile_id, true, error);
}

bool RuntimeSessionProjectileContactBodiesV1::clear_entries(std::string& error) {
    if (world_.backend() && !world_.cleanup_delivery_idle_v106())
        return fail(error, "Cannot clear projectile sensors during NativeWorld delivery");
    for (auto& pair : entries_)
        if (pair.second->body.body && world_.backend())
            world_.destroy(pair.second->body.body);
    entries_.clear();
    source_identities_.clear();
    error.clear();
    return true;
}

bool RuntimeSessionProjectileContactBodiesV1::clear(std::string& error) {
    bool detached = true;
    std::string detach_error;
    if (session_alive() &&
        same_lease(binding_lease_, session_.actor_binding_lease())) {
        for (const auto& pair : entries_) {
            std::string current_error;
            if (!runtime_.detach_native_body(pair.first, current_error)) {
                detached = false;
                if (detach_error.empty()) detach_error = current_error;
            }
        }
    }
    if (!clear_entries(error)) return false;
    if (!detached)
        return fail(error, detach_error.empty() ?
            "Runtime projectile body detach failed during contact owner clear" :
            detach_error.c_str());
    error.clear();
    return true;
}

} // namespace dh::foundation::effects
