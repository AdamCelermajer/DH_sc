#include "runtime_session_projectile_v1.hpp"

#include "../../original_actor_animation_events.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <utility>

namespace dh::foundation::effects {
namespace {
bool fail(std::string& error, const char* why) {
    error = why;
    return false;
}
bool same_owner(const std::weak_ptr<const void>& a,
                const std::weak_ptr<const void>& b) noexcept {
    return !a.owner_before(b) && !b.owner_before(a);
}
bool valid_vec(const std::array<float, 3>& value) noexcept {
    return std::all_of(value.begin(), value.end(),
                       [](float x) { return std::isfinite(x); });
}
float length_squared(const std::array<float, 3>& value) noexcept {
    return value[0] * value[0] + value[1] * value[1] + value[2] * value[2];
}
template<class T>
bool aligned(const T* value) noexcept {
    return value && reinterpret_cast<std::uintptr_t>(value) % alignof(T) == 0;
}
bool source_mainhand_category(
    const dh2::character::CombatInventory16* inventory,
    const dh2::character::CombatItemRecord164* item_rows,
    std::uint32_t item_count, std::int32_t& category,
    std::string& error) {
    using namespace dh2::character;
    category = -1;
    if (!aligned(inventory) || inventory->count > 65536 ||
        inventory->current_set < 0 ||
        static_cast<std::uint32_t>(inventory->current_set) >= inventory->count ||
        !aligned(inventory->sets))
        return fail(error, "Projectile result requires the current source inventory set");
    const auto* hand = inventory->sets[inventory->current_set].main_hand;
    if (!hand) {
        error.clear();
        return true;
    }
    if (!aligned(hand) || !aligned(*hand))
        return fail(error, "Projectile result mainhand item link is malformed");
    const auto item_id = (*hand)->item_id;
    if (item_id < 0) {
        error.clear();
        return true;
    }
    if (item_count > 65536 || !aligned(item_rows) ||
        static_cast<std::uint32_t>(item_id) >= item_count)
        return fail(error, "Projectile result mainhand ItemTable row is unavailable");
    category = item_rows[item_id].words[37];
    if (category < -1 || category >= 141)
        return fail(error, "Projectile result mainhand category is outside source range");
    error.clear();
    return true;
}
}

bool runtime_session_projectile_melee_hit_v1(
    const std::shared_ptr<const dh2::android_ui::SourceProcessArraysV101>& tables,
    const RuntimeSessionProjectileImpactCandidateV1& candidate,
    const dh2::character::CombatInventory16* inventory,
    const dh2::character::CombatItemRecord164* item_rows,
    std::uint32_t item_count, CombatSessionSourceHit& output,
    std::string& error) {
    using Value = dh2::android_ui::ProcessArrayValueV101;
    const auto* projectile_table = tables ? tables->group("ProjectileTable") : nullptr;
    if (!tables || !tables->ready() || !projectile_table ||
        !projectile_table->records_loaded || !projectile_table->names_loaded ||
        projectile_table->declared_rows != projectile_table->rows.size() ||
        candidate.source_row < 0 ||
        static_cast<std::size_t>(candidate.source_row) >= projectile_table->rows.size() ||
        static_cast<std::size_t>(candidate.source_row) >= projectile_table->names.size())
        return fail(error, "Projectile F_MeleeAttack requires the retained actual ProjectileTable row");
    const auto& fields = projectile_table->rows[
        static_cast<std::size_t>(candidate.source_row)].fields;
    // Projectile::SetInfo copies byte row+0x1d to Projectile+0x37c. The
    // recovered native 72-byte projection maps offset29 to field8 (magic).
    if (fields.size() != 20 || fields[8].kind != Value::Kind::byte ||
        fields[8].bits > 255)
        return fail(error, "Projectile F_MeleeAttack magic/critical source byte changed");
    const bool source_critical = fields[8].bits != 0;
    if (candidate.source_critical != source_critical)
        return fail(error, "Projectile critical byte differs from its retained source row");
    if (candidate.owner == invalid_actor_id || candidate.target == invalid_actor_id ||
        candidate.owner == candidate.target || candidate.source_marker.empty())
        return fail(error, "Projectile F_MeleeAttack occurrence identity is incomplete");
    std::int32_t category = -1;
    if (!source_mainhand_category(inventory, item_rows, item_count, category, error))
        return false;
    CombatSessionSourceHit hit;
    hit.attacker = candidate.owner;
    hit.target = candidate.target;
    hit.source_id = "source-projectile-f-melee-attack";
    hit.marker_name = candidate.source_marker;
    hit.mask = source_critical ? 0x0005554au : 0x0022aab5u;
    hit.category = category;
    hit.element = -1;
    hit.direct_amount = 0;
    output = std::move(hit);
    error.clear();
    return true;
}

RuntimeSessionProjectileV1::RuntimeSessionProjectileV1(
    CombatSession& session, RuntimeSessionProjectileServicesV1 services)
    : session_(session), services_(std::move(services)),
      session_lifetime_(session.lifetime_lease()) {}

bool RuntimeSessionProjectileV1::LaunchKey::operator<(
    const LaunchKey& other) const noexcept {
    if (actor != other.actor) return actor < other.actor;
    if (generation != other.generation) return generation < other.generation;
    return occurrence < other.occurrence;
}

void RuntimeSessionProjectileV1::clear() noexcept {
    projectiles_.clear();
    launched_.clear();
    impacts_.clear();
    results_.clear();
    has_source_frame_ = false;
    last_source_frame_ = 0;
    binding_lease_.reset();
}

bool RuntimeSessionProjectileV1::session_alive() const noexcept {
    const auto lifetime = session_lifetime_.lock();
    return lifetime && lifetime->alive();
}

bool RuntimeSessionProjectileV1::synchronize(std::string& error) {
    // session_ is a borrowed reference. Never even request its actor lease
    // until the independently captured lifetime witness confirms it exists.
    if (!session_alive()) {
        clear();
        return fail(error, "Projectile store owner CombatSession was destroyed");
    }
    const auto current = session_.actor_binding_lease();
    if (current.expired()) {
        clear();
        return fail(error, "Projectile store requires a live CombatSession binding");
    }
    if (!same_owner(binding_lease_, current)) {
        projectiles_.clear();
        launched_.clear();
        impacts_.clear();
        results_.clear();
        has_source_frame_ = false;
        last_source_frame_ = 0;
        binding_lease_ = current;
    }
    error.clear();
    return true;
}

bool RuntimeSessionProjectileV1::synchronize_session_binding(
    std::string& error) {
    return synchronize(error);
}

bool RuntimeSessionProjectileV1::attach_native_body(
    std::uint64_t id, std::string& error) {
    if (!synchronize(error)) return false;
    const auto found = projectiles_.find(id);
    if (found == projectiles_.end() || !found->second.live)
        return fail(error, "Native projectile body requires a live runtime record");
    if (found->second.native_body)
        return fail(error, "Runtime projectile already has a NativeWorld body");
    found->second.native_body = true;
    error.clear();
    return true;
}

bool RuntimeSessionProjectileV1::detach_native_body(
    std::uint64_t id, std::string& error) {
    if (!synchronize(error)) return false;
    const auto found = projectiles_.find(id);
    if (found == projectiles_.end() || !found->second.live ||
        !found->second.native_body)
        return fail(error, "Runtime projectile has no current native body to detach");
    found->second.native_body = false;
    error.clear();
    return true;
}

bool RuntimeSessionProjectileV1::publish_native_body_position(
    std::uint64_t id, const std::array<float, 3>& position,
    std::string& error) {
    if (!synchronize(error)) return false;
    const auto found = projectiles_.find(id);
    if (found == projectiles_.end() || !found->second.live ||
        !found->second.native_body)
        return fail(error, "Native projectile position has no current registered body");
    if (!valid_vec(position))
        return fail(error, "Native projectile position is nonfinite");
    found->second.position = position;
    error.clear();
    return true;
}

bool RuntimeSessionProjectileV1::native_body_velocity(
    std::uint64_t id, std::array<float, 2>& velocity,
    std::string& error) const {
    if (!session_alive())
        return fail(error, "Projectile store owner CombatSession was destroyed");
    const auto found = projectiles_.find(id);
    if (found == projectiles_.end() || !found->second.live ||
        !found->second.native_body)
        return fail(error, "Native projectile velocity has no current live body");
    velocity = {found->second.physics_velocity[0],
                found->second.physics_velocity[1]};
    error.clear();
    return true;
}

bool RuntimeSessionProjectileV1::current_actor(ActorId id,
                                                std::string& error) const {
    if (!session_alive())
        return fail(error, "Projectile store owner CombatSession was destroyed");
    if (id == invalid_actor_id || !session_.actor(id))
        return fail(error, "Projectile owner is not a current Session actor");
    const auto current = session_.actor_binding_lease();
    if (current.expired() || !same_owner(binding_lease_, current))
        return fail(error, "Projectile actor binding lease changed");
    error.clear();
    return true;
}

bool RuntimeSessionProjectileV1::launch_state5(
    const std::shared_ptr<const dh2::android_ui::SourceProcessArraysV101>& tables,
    const RuntimeSessionProjectileLaunchV1& request, bool& accepted,
    std::uint64_t& projectile_id, std::string& error) {
    accepted = false;
    projectile_id = 0;
    if (!synchronize(error) || !current_actor(request.owner, error)) return false;
    if (request.occurrence == 0 || request.event.generation == 0 ||
        request.event.clip_id.empty() || request.event.name.empty())
        return fail(error, "Projectile launch requires the retained marker occurrence");
    if (request.pose.owner != request.owner || !valid_vec(request.pose.origin) ||
        !valid_vec(request.pose.forward))
        return fail(error, "Projectile launch pose is not a finite same-actor loan");

    const LaunchKey key{request.owner, request.event.generation,
                        request.occurrence};
    const auto previous = launched_.find(key);
    if (previous != launched_.end()) {
        projectile_id = previous->second;
        error.clear();
        return true;
    }

    // The original caller has already selected its current properties and
    // equipment; query those exact loans here rather than accepting a cached
    // projectile ID or a generic "ranged" profile bit.
    if (!request.current_properties)
        return fail(error, "Reached Mage projectile event lacks current Character properties");
    bool can_range = false;
    RuntimeSourceRangeSelectionV1 selection;
    if (!runtime_source_range_selection_v1(
            tables, *request.current_properties, request.current_inventory,
            request.item_rows, request.item_count, can_range, selection, error))
        return false;
    if (!can_range) {
        error.clear();
        return true;
    }
    if (request.source_event.state != 5 ||
        classify_original_named_animation_event(request.event.name, 5, true) !=
            OriginalNamedAnimationKind::projectile) {
        error.clear();
        return true;
    }
    auto context = request.source_event;
    context.can_range = 1;
    if (context.projectile != selection.projectile.row)
        return fail(error, "State-5 projectile row differs from the live CanRangeAttack result");
    bool handled = false;
    RuntimeSourceProjectilePlanV1 source;
    if (!runtime_source_ranged_event_v1(tables, context, request.event.name.c_str(),
                                        handled, source, error))
        return false;
    if (!handled) {
        error.clear();
        return true;
    }
    // The selected Mage Staff01 source uses a positive fixed timer. Other
    // source rows whose timer is supplied by an animated model need that actual
    // duration provider before this runtime can admit them.
    if (source.requires_model_duration() || source.timer_ms < 0 ||
        !std::isfinite(source.velocity) || source.velocity <= 0 ||
        !std::isfinite(source.velocity_damping) || source.velocity_damping < 0 ||
        !std::isfinite(source.max_distance))
        return fail(error, "Projectile row needs an unsupported source duration or motion value");

    const float norm2 = length_squared(request.pose.forward);
    if (!std::isfinite(norm2) || norm2 <= 1.0e-12f)
        return fail(error, "Projectile owner look vector is not normalizable");
    const float inverse = 1.0f / std::sqrt(norm2);
    Record record;
    record.id = next_projectile_id_++;
    if (!record.id || next_projectile_id_ == 0)
        return fail(error, "Projectile identity space exhausted");
    record.source_generation = request.event.generation;
    record.source_occurrence = request.occurrence;
    record.owner = request.owner;
    record.marker = request.event.name;
    record.source = std::move(source);
    record.origin = request.pose.origin;
    record.position = request.pose.origin;
    for (unsigned i = 0; i < 3; ++i)
        record.forward[i] = request.pose.forward[i] * inverse;
    record.speed = record.source.velocity;
    record.remaining_ms = record.source.timer_ms;
    projectile_id = record.id;
    projectiles_.emplace(record.id, std::move(record));
    launched_.emplace(key, projectile_id);
    accepted = true;
    error.clear();
    return true;
}

bool RuntimeSessionProjectileV1::contact(
    const RuntimeSessionProjectileContactV1& contact_event, bool& accepted,
    std::string& error) {
    accepted = false;
    if (!synchronize(error)) return false;
    const auto found = projectiles_.find(contact_event.projectile_id);
    if (found == projectiles_.end() || !found->second.live) {
        error.clear();
        return true;
    }
    auto& projectile = found->second;
    if (!current_actor(projectile.owner, error) ||
        !current_actor(contact_event.peer, error)) return false;
    if (!contact_event.character_peer)
        return fail(error, "Non-character projectile interaction needs its source object activator");
    if (!valid_vec(contact_event.point))
        return fail(error, "Projectile contact point must be finite");
    if (contact_event.peer == projectile.owner) {
        error.clear();
        return true;
    }
    if (projectile.source.dedicated &&
        (projectile.explicit_target == invalid_actor_id ||
         projectile.explicit_target != contact_event.peer)) {
        error.clear();
        return true;
    }
    if (!projectile.source.friendly_fire) {
        if (!services_.source_is_enemy)
            return fail(error, "Required actual source AI_IsEnemy contact query");
        bool owner_has_ai = false, enemy = false;
        if (!services_.source_is_enemy(projectile.owner, contact_event.peer,
                                       owner_has_ai, enemy, error))
            return fail(error, "Source projectile enemy query failed");
        if (owner_has_ai && !enemy) {
            error.clear();
            return true;
        }
    }
    if (projectile.pending_hit) {
        error.clear();
        return true;
    }
    RuntimeSessionProjectileImpactCandidateV1 candidate;
    candidate.projectile_id = projectile.id;
    candidate.source_generation = projectile.source_generation;
    candidate.source_occurrence = projectile.source_occurrence;
    candidate.owner = projectile.owner;
    candidate.target = contact_event.peer;
    candidate.source_row = projectile.source.row;
    candidate.contact_point = contact_event.point;
    candidate.source_marker = projectile.marker;
    candidate.source_critical = projectile.source.magic;
    projectile.contact = contact_event;
    projectile.pending_hit = std::move(candidate);
    accepted = true;
    error.clear();
    return true;
}

bool RuntimeSessionProjectileV1::process_pending(Record& projectile,
                                                  std::string& error) {
    if (!session_alive())
        return fail(error, "Projectile store owner CombatSession was destroyed");
    if (!projectile.pending_hit) {
        error.clear();
        return true;
    }
    if (!projectile.owner_projectile_hit_delivered) {
        if (!services_.source_owner_on_projectile_hit)
            return fail(error,
                "Required source owner CharAI::OnProjectileHit peer provider");
        if (!services_.source_owner_on_projectile_hit(*projectile.pending_hit,
                                                       error))
            return fail(error, "Source owner OnProjectileHit provider failed");
        projectile.owner_projectile_hit_delivered = true;
    }
    if (!services_.source_hit_semantics)
        return fail(error, "Required reached source F_MeleeAttack result inputs");
    if (next_hit_occurrence_ > std::numeric_limits<std::uint32_t>::max())
        return fail(error, "Projectile hit occurrence stream exhausted");
    CombatSessionSourceHit hit;
    if (!services_.source_hit_semantics(*projectile.pending_hit, hit, error))
        return fail(error, "Source projectile hit-semantics provider failed");
    hit.attacker = projectile.owner;
    hit.target = projectile.pending_hit->target;
    hit.binding_lease = binding_lease_;
    hit.generation = projectile.source_generation;
    hit.event_index = static_cast<std::uint32_t>(next_hit_occurrence_++);
    if (hit.source_id.empty() || hit.marker_name.empty())
        return fail(error, "Source projectile result lacks its exact source/marker identity");
    // Session's result ledger is source-scoped. Each live projectile gets its
    // own source occurrence namespace so a slower older projectile is not
    // rejected merely because a later attack sequence launched another one.
    hit.source_id += "/projectile-" + std::to_string(projectile.id);
    DamageEvent receipt;
    // The provider is feature-owned and may reenter application code. Recheck
    // the lease after it returns before dereferencing the borrowed Session.
    if (!session_alive())
        return fail(error, "Projectile store owner CombatSession was destroyed");
    if (!session_.apply_source_result(hit, receipt, error)) return false;
    if (!receipt.applied || !receipt.source_outcomes)
        return fail(error, "Session did not return an applied source projectile result");
    const bool continues = ((*receipt.source_outcomes & 3u) != 0);
    const auto* result_world = session_.world();
    if (!result_world)
        return fail(error, "Required same-Session F_ApplyResult calculation owner");
    const auto& resolutions = result_world->pending_resolutions();
    const auto calculated = std::find_if(resolutions.rbegin(), resolutions.rend(),
        [&](const PlayableCombatResolution& value) {
            return value.attacker == projectile.owner &&
                value.victim == projectile.pending_hit->target &&
                value.source_id == hit.source_id &&
                value.marker_name == projectile.pending_hit->source_marker;
        });
    if (calculated == resolutions.rend())
        return fail(error,
            "Required exact same-Session F_MeleeAttack result for projectile receipt");
    RuntimeSessionProjectileImpactV1 impact{projectile.id, projectile.owner,
        projectile.pending_hit->target, projectile.source.row,
        continues ? 0 : 4, projectile.pending_hit->contact_point,
        projectile.source.impact_fx, projectile.source.impact_sound, receipt};
    impact.source_calculation = calculated->melee;
    impact.source_apply_result_flag = false;
    impacts_.push_back(std::move(impact));
    results_.push_back(receipt);
    projectile.pending_hit.reset();
    projectile.contact.reset();
    projectile.owner_projectile_hit_delivered = false;
    if (!continues) projectile.live = false;
    error.clear();
    return true;
}

bool RuntimeSessionProjectileV1::update(std::uint64_t frame,
    std::int32_t app_dt_ms, bool paused, std::string& error) {
    if (!synchronize(error)) return false;
    if (app_dt_ms < 0)
        return fail(error, "Source projectile App dt must be nonnegative");
    if (has_source_frame_ && frame <= last_source_frame_)
        return fail(error, "Projectile store requires one increasing source frame token");
    has_source_frame_ = true;
    last_source_frame_ = frame;
    // Level::Update returns before PhysicalWorld::update and ObjectManager when
    // globally paused. No queued projectile callback or virtual velocity write
    // is reached during that frame.
    if (paused) {
        error.clear();
        return true;
    }
    const auto dt = app_dt_ms;
    for (auto it = projectiles_.begin(); it != projectiles_.end();) {
        auto& projectile = it->second;
        if (!current_actor(projectile.owner, error)) return false;
        if (!projectile.live) {
            it = projectiles_.erase(it);
            continue;
        }
        if (!process_pending(projectile, error)) return false;
        if (!projectile.live) {
            it = projectiles_.erase(it);
            continue;
        }
        if (dt > 0 && !projectile.native_body) {
            // Level::Update steps Box2D before ObjectManager/Projectile::Update.
            // The velocity written by the prior GameObject::Update therefore
            // moves this frame. PhysicalObject stores velocity in Box2D units,
            // while GetPosition publishes game XY multiplied by 100.
            const float seconds = static_cast<float>(dt) * 0.001f;
            constexpr float physics_to_game = 100.0f;
            projectile.position[0] += projectile.physics_velocity[0] *
                                      physics_to_game * seconds;
            projectile.position[1] += projectile.physics_velocity[1] *
                                      physics_to_game * seconds;
        }
        // GameObject::UpdateSubObjects calls virtual GetSpeed and writes an XY
        // velocity after the World step. Projectile::Update damps its scalar
        // only after that write, so the body consumes this value next frame.
        projectile.physics_velocity = {
            projectile.speed * projectile.forward[0],
            projectile.speed * projectile.forward[1], 0.0f};
        if (dt > 0) {
            const float seconds = static_cast<float>(dt) * 0.001f;
            projectile.speed -= projectile.source.velocity_damping * seconds;
            projectile.remaining_ms -= dt;
        }
        bool expired = projectile.remaining_ms <= 0 || projectile.speed <= 0;
        if (!expired && projectile.source.max_distance >= 0) {
            std::array<float, 3> delta{};
            for (unsigned axis = 0; axis < 3; ++axis)
                delta[axis] = projectile.position[axis] - projectile.origin[axis];
            const float max_sq = projectile.source.max_distance *
                                 projectile.source.max_distance;
            expired = length_squared(delta) >= max_sq;
        }
        if (expired) {
            impacts_.push_back({projectile.id, projectile.owner,
                invalid_actor_id, projectile.source.row, 1, projectile.position,
                projectile.source.expire_fx, projectile.source.expire_sound,
                std::nullopt});
            it = projectiles_.erase(it);
            continue;
        }
        ++it;
    }
    error.clear();
    return true;
}

std::vector<RuntimeSessionProjectileVisualV1>
RuntimeSessionProjectileV1::visuals() const {
    std::vector<RuntimeSessionProjectileVisualV1> result;
    if (!session_alive()) return result;
    result.reserve(projectiles_.size());
    for (const auto& pair : projectiles_) {
        const auto& p = pair.second;
        if (!p.live) continue;
        result.push_back({p.id, p.owner, p.source.row, p.source.model,
            p.source.model_uri, p.position, p.forward,
            std::atan2(p.forward[1], p.forward[0])});
    }
    return result;
}

std::vector<RuntimeSessionProjectileImpactV1>
RuntimeSessionProjectileV1::take_impacts() {
    std::vector<RuntimeSessionProjectileImpactV1> result;
    if (!session_alive()) {
        clear();
        return result;
    }
    result.swap(impacts_);
    return result;
}

std::vector<DamageEvent> RuntimeSessionProjectileV1::take_results() {
    std::vector<DamageEvent> result;
    if (!session_alive()) {
        clear();
        return result;
    }
    result.swap(results_);
    return result;
}

std::size_t RuntimeSessionProjectileV1::active_count() const noexcept {
    return session_alive() ? projectiles_.size() : 0;
}

} // namespace dh::foundation::effects
