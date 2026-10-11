#include "door_collision_v1.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <limits>
#include <unordered_map>

namespace dh::foundation::doors {
namespace {
using BindingMap = std::unordered_map<std::uint64_t, const DoorPhysicalBindingV1*>;

bool index_bindings(const std::vector<DoorPhysicalBindingV1>& values, BindingMap& indexed,
                    const char* label, std::string& error) {
    indexed.clear();
    indexed.reserve(values.size());
    for (const auto& value : values) {
        if (!value.stable_id) {
            error = std::string("Door physical ") + label + " snapshot has a zero stable ID";
            return false;
        }
        if (!indexed.emplace(value.stable_id, &value).second) {
            error = std::string("Door physical ") + label + " snapshot has a duplicate stable ID";
            return false;
        }
    }
    return true;
}

bool requires_body(const DoorPhysicalBindingV1& binding) {
    return binding.collision_enabled && binding.present;
}

struct Vec2 { double x{}, y{}; };
Vec2 operator+(Vec2 a, Vec2 b) { return {a.x + b.x, a.y + b.y}; }
Vec2 operator-(Vec2 a, Vec2 b) { return {a.x - b.x, a.y - b.y}; }
Vec2 operator*(Vec2 a, double n) { return {a.x * n, a.y * n}; }
double dot(Vec2 a, Vec2 b) { return a.x * b.x + a.y * b.y; }
double length_sq(Vec2 a) { return dot(a, a); }

struct Hit { double time{}; Vec2 normal{}; };

bool segment_circle(Vec2 p, Vec2 delta, Vec2 center, double radius, Hit& hit) {
    const Vec2 offset = p - center;
    const double a = length_sq(delta);
    if (a <= 0) return false;
    const double b = 2.0 * dot(offset, delta);
    const double c = length_sq(offset) - radius * radius;
    const double discriminant = b * b - 4.0 * a * c;
    if (discriminant < 0) return false;
    const double t = (-b - std::sqrt(discriminant)) / (2.0 * a);
    if (t < 0 || t > 1) return false;
    const Vec2 contact = p + delta * t;
    Vec2 normal = contact - center;
    const double n2 = length_sq(normal);
    if (n2 == 0) return false;
    normal = normal * (1.0 / std::sqrt(n2));
    if (dot(delta, normal) >= 0) return false;
    hit = {t, normal};
    return true;
}

bool ray_box(Vec2 p, Vec2 delta, const float bounds[4], Hit& hit) {
    double enter = 0, leave = 1;
    Vec2 normal{};
    for (unsigned axis = 0; axis < 2; ++axis) {
        const double start = axis ? p.y : p.x;
        const double move = axis ? delta.y : delta.x;
        const double lo = bounds[axis], hi = bounds[axis + 2];
        if (move == 0) { if (start < lo || start > hi) return false; continue; }
        double t0 = (lo - start) / move, t1 = (hi - start) / move;
        Vec2 n0 = axis ? Vec2{0, -1} : Vec2{-1, 0};
        if (t0 > t1) { std::swap(t0, t1); n0 = axis ? Vec2{0, 1} : Vec2{1, 0}; }
        if (t0 > enter) { enter = t0; normal = n0; }
        leave = std::min(leave, t1);
        if (enter > leave) return false;
    }
    if (enter < 0 || enter > 1 || dot(delta, normal) >= 0) return false;
    hit = {enter, normal};
    return true;
}

bool circle_box(Vec2 p, Vec2 delta, const float bounds[4], double radius, Hit& hit) {
    if (radius == 0) return ray_box(p, delta, bounds, hit);
    const double x = std::clamp(p.x, double(bounds[0]), double(bounds[2]));
    const double y = std::clamp(p.y, double(bounds[1]), double(bounds[3]));
    Vec2 closest{x, y};
    Vec2 separation = p - closest;
    const double distance2 = length_sq(separation);
    if (distance2 <= radius * radius) {
        if (distance2 == 0) {
            const double left = p.x - bounds[0], right = bounds[2] - p.x;
            const double bottom = p.y - bounds[1], top = bounds[3] - p.y;
            const double nearest = std::min({left, right, bottom, top});
            separation = nearest == left ? Vec2{-1, 0} : nearest == right ? Vec2{1, 0} :
                         nearest == bottom ? Vec2{0, -1} : Vec2{0, 1};
        } else separation = separation * (1.0 / std::sqrt(distance2));
        if (dot(delta, separation) < 0) { hit = {0, separation}; return true; }
        return false; // starts at/inside contact and moves away or tangent
    }

    bool found = false;
    Hit best{std::numeric_limits<double>::infinity(), {}};
    const auto consider = [&](double t, Vec2 normal, bool& have, Hit& selected) {
        if (t < 0 || t > 1 || dot(delta, normal) >= 0 || t >= selected.time) return;
        selected = {t, normal}; have = true;
    };
    if (delta.x > 0) {
        const double t = (bounds[0] - radius - p.x) / delta.x;
        const double yy = p.y + t * delta.y;
        if (yy >= bounds[1] && yy <= bounds[3]) consider(t, {-1, 0}, found, best);
    } else if (delta.x < 0) {
        const double t = (bounds[2] + radius - p.x) / delta.x;
        const double yy = p.y + t * delta.y;
        if (yy >= bounds[1] && yy <= bounds[3]) consider(t, {1, 0}, found, best);
    }
    if (delta.y > 0) {
        const double t = (bounds[1] - radius - p.y) / delta.y;
        const double xx = p.x + t * delta.x;
        if (xx >= bounds[0] && xx <= bounds[2]) consider(t, {0, -1}, found, best);
    } else if (delta.y < 0) {
        const double t = (bounds[3] + radius - p.y) / delta.y;
        const double xx = p.x + t * delta.x;
        if (xx >= bounds[0] && xx <= bounds[2]) consider(t, {0, 1}, found, best);
    }

    const std::array<Vec2, 4> corners{{{bounds[0], bounds[1]}, {bounds[0], bounds[3]},
                                       {bounds[2], bounds[1]}, {bounds[2], bounds[3]}}};
    for (const auto corner : corners) {
        Hit corner_hit;
        if (!segment_circle(p, delta, corner, radius, corner_hit)) continue;
        const auto contact = p + delta * corner_hit.time;
        if ((corner.x == bounds[0] && contact.x > bounds[0]) ||
            (corner.x == bounds[2] && contact.x < bounds[2]) ||
            (corner.y == bounds[1] && contact.y > bounds[1]) ||
            (corner.y == bounds[3] && contact.y < bounds[3])) continue;
        consider(corner_hit.time, corner_hit.normal, found, best);
    }
    if (found) hit = best;
    return found;
}

bool filters_collide(const DoorActorCollisionV1& actor, const DoorCollisionVolumeV1& door) {
    if (actor.group_index != 0 && actor.group_index == door.group_index) return actor.group_index > 0;
    return (door.category_bits & actor.mask_bits) != 0 && (actor.category_bits & door.mask_bits) != 0;
}

bool valid_box(const DoorCollisionVolumeV1& door) {
    for (const float value : door.bounds_xy) if (!std::isfinite(value)) return false;
    return door.bounds_xy[0] < door.bounds_xy[2] && door.bounds_xy[1] < door.bounds_xy[3];
}
} // namespace

bool plan_door_physical_reconcile_v1(const std::vector<DoorPhysicalBindingV1>& previous,
                                     const std::vector<DoorPhysicalBindingV1>& current,
                                     std::vector<DoorPhysicalOperationV1>& operations,
                                     std::string& error) {
    error.clear();
    BindingMap old_by_id, new_by_id;
    if (!index_bindings(previous, old_by_id, "previous", error) ||
        !index_bindings(current, new_by_id, "current", error)) return false;

    std::vector<DoorPhysicalOperationV1> planned;
    planned.reserve(previous.size() + current.size());
    for (const auto& next : current) {
        const auto old = old_by_id.find(next.stable_id);
        const bool had_body = old != old_by_id.end() && requires_body(*old->second);
        const bool needs_body = requires_body(next);
        if (!had_body && needs_body) {
            planned.push_back({DoorPhysicalOperationKindV1::create, next.stable_id, next.filter_enabled});
        } else if (had_body && !needs_body) {
            planned.push_back({DoorPhysicalOperationKindV1::retire, next.stable_id, false});
        } else if (had_body && needs_body && old->second->filter_enabled != next.filter_enabled) {
            planned.push_back({DoorPhysicalOperationKindV1::set_filter, next.stable_id, next.filter_enabled});
        }
    }
    for (const auto& old : previous) {
        if (new_by_id.find(old.stable_id) == new_by_id.end() && requires_body(old))
            planned.push_back({DoorPhysicalOperationKindV1::retire, old.stable_id, false});
    }
    operations = std::move(planned);
    return true;
}

bool constrain_door_motion_v1(const std::vector<DoorCollisionVolumeV1>& doors,
                              const DoorActorCollisionV1& actor,
                              const float from_xy[2], const float desired_xy[2],
                              DoorMotionResultV1& result, std::string& error) {
    error.clear();
    if (!from_xy || !desired_xy || !std::isfinite(from_xy[0]) || !std::isfinite(from_xy[1]) ||
        !std::isfinite(desired_xy[0]) || !std::isfinite(desired_xy[1]) ||
        !std::isfinite(actor.radius_game_units) || actor.radius_game_units < 0) {
        error = "Door motion requires finite positions and nonnegative actor radius";
        return false;
    }
    std::unordered_map<std::uint64_t, bool> ids;
    ids.reserve(doors.size());
    for (const auto& door : doors) {
        if (!door.binding.stable_id || !ids.emplace(door.binding.stable_id, true).second) {
            error = "Door motion has a zero or duplicate stable ID";
            return false;
        }
        if (door.binding.collision_enabled && door.binding.present && door.binding.filter_enabled && !valid_box(door)) {
            error = "Colliding Door lacks a finite source ApplyMeshBox AABB";
            return false;
        }
    }

    const Vec2 start{from_xy[0], from_xy[1]}, desired{desired_xy[0], desired_xy[1]};
    Vec2 position = start, remaining = desired - start;
    bool clipped = false;
    std::uint64_t first_blocker = 0;
    if (actor.physical_collision_enabled && actor.move_collision_enabled) {
        constexpr double contact_allowance_game_units = 0.5; // Box2D source slop 0.005m
        const double radius = double(actor.radius_game_units) + contact_allowance_game_units;
        // Each iteration consumes the earliest contact then projects the unconsumed segment
        // onto its tangent. Repeated contacts support corners and adjacent door panels.
        for (unsigned iteration = 0; iteration < 8 && length_sq(remaining) > 1e-12; ++iteration) {
            bool found = false;
            Hit first{std::numeric_limits<double>::infinity(), {}};
            std::uint64_t blocker = 0;
            for (const auto& door : doors) {
                if (!door.binding.collision_enabled || !door.binding.present || !door.binding.filter_enabled ||
                    !filters_collide(actor, door)) continue;
                Hit candidate;
                if (circle_box(position, remaining, door.bounds_xy, radius, candidate) && candidate.time < first.time) {
                    first = candidate; blocker = door.binding.stable_id; found = true;
                }
            }
            if (!found) { position = position + remaining; remaining = {}; break; }
            const double segment_length = std::sqrt(length_sq(remaining));
            const double retreat = segment_length > 0 ? std::min(first.time, contact_allowance_game_units / segment_length) : 0;
            const double safe_time = std::max(0.0, first.time - retreat);
            const Vec2 advance = remaining * safe_time;
            position = position + advance;
            Vec2 rest = remaining * (1.0 - first.time);
            const double into_surface = dot(rest, first.normal);
            if (into_surface < 0) rest = rest - first.normal * into_surface;
            remaining = rest;
            clipped = true;
            if (!first_blocker) first_blocker = blocker;
        }
        if (length_sq(remaining) > 1e-12) {
            // An unresolved chain of more than eight contacts fails closed at the last safe point.
            clipped = true;
            remaining = {};
        }
    } else position = desired;

    result.position_xy[0] = static_cast<float>(position.x);
    result.position_xy[1] = static_cast<float>(position.y);
    result.clipped = clipped;
    result.first_blocking_door = first_blocker;
    return true;
}

} // namespace dh::foundation::doors
