#pragma once

#include "current_level_quest_event_v1.hpp"
#include "object_manager_runtime_owner_v1.hpp"

#include <cstdint>
#include <string>

namespace dh2::level_world::quest_move_in_zone_collision_v1 {

namespace objects = object_manager_runtime_owner_v1;
namespace events = current_level_quest_event_v1;

// Borrowed binding for one already-created QuestMoveInZone GameObject. The
// object remains owned by the active Level's canonical ObjectManager; this
// value carries no second object registry or collision state.
struct BoundZone {
    objects::Owner* object_manager{};
    objects::GameObject* object{};
    // ObjectBase word +25 as read from this exact source object. It is kept
    // separate from the ObjectManager key and from the pointer-width tail.
    std::int32_t network_id{-1};
};

enum class Status : std::uint8_t {
    complete,
    no_event,
    invalid_argument,
    stale_object,
    source_callback_failed,
    constant_unavailable,
    event_raise_failed,
};

struct ActiveLevelView {
    objects::Owner* object_manager{};
};

struct Services {
    void* context{};
    // ObjectBase virtual +0x24. Called first after the nonnull GO boundary.
    bool (*virtual_24)(void*, objects::GameObject& collided,
                       bool& result, std::string& error){};
    // Application::GetCurrentLevel. This callback is reached only after the
    // virtual gate returns true. `present=false` is the source early return.
    bool (*current_level)(void*, ActiveLevelView&, bool& present,
                          std::string& error){};
    // PyDataConstants::getConstant("v2QuestObjectiveType", "MoveInZone").
    // Queried only when the source callback has a current Level.
    bool (*move_in_zone_constant)(void*, std::int32_t& value,
                                 std::string& error){};
    // Calls the current Quest owner’s existing RaiseAsync/Runtime boundary;
    // the producer does not own or expose another event table.
    bool (*raise_current_level_event)(void*, events::Event&,
                                      std::string& error){};
};

struct Result {
    Status status{Status::invalid_argument};
    bool source_gate_passed{};
    bool current_level_present{};
    bool event_raised{};
};

// Zone::InitPost (IDA 0x39771c) scales its default/overridden dimensions,
// writes a centered GameObject AABB, then constructs a PhysicalObject sensor.
// PhysicalObject converts world units to Box2D units at 0.01 (IDA 0x46f2f0).
struct PhysicalSensor {
    float position[2]{};
    float half_extents[2]{};
    std::int16_t group_index{-5};
    std::uint16_t category_bits{0x0800};
    std::uint16_t mask_bits{1310};
    bool sensor{true};
};

// Bind one source QuestMoveInZone instance already present in the shared
// ObjectManager. Call only after its source factory/Add has committed it.
Status bind(objects::Owner&, objects::SourceHandle,
            std::int32_t source_network_id, BoundZone&,
            std::string& error) noexcept;

// Reproduce the selected source Zone's centered XY AABB as the physical
// sensor body. Values use authored world units before the source's 0.01 scale.
Status build_physical_sensor(const float world_position[3],
                             const float authored_scale[3],
                             const float dimensions[3],
                             PhysicalSensor& output,
                             std::string& error) noexcept;

// Build from the canonical GameObject local XY AABB written by Zone::InitPost
// (the four source extents are minX,minY,maxX,maxY). Production callers should
// pass these live source bounds directly; this does not recompute dimensions
// or authored scale.
Status build_physical_sensor_from_aabb(const float world_position[3],
                                       const float local_aabb_xyxy[4],
                                       PhysicalSensor& output,
                                       std::string& error) noexcept;

// Zone::InitPost's default Property44 dimensions are 200 on each axis; its
// centered local AABB is scaled by the exact imported GameObject scale before
// PhysicalObject consumes it. Callers may pass an authored dimensions override
// when the MGP row contains one.
Status build_init_post_local_aabb(const float authored_scale[3],
                                  const float dimensions[3],
                                  float local_aabb_xyxy[4],
                                  std::string& error) noexcept;

// QuestMoveInZone::OnCollisionBegins (IDA 0x396120). The caller must forward
// the actual POZone/physical collision callback's nonnull GameObject; this
// function does not sample proximity or synthesize an enter edge.
Status on_collision_begins(const BoundZone&, objects::GameObject* collided,
                           const Services&, Result&,
                           std::string& error) noexcept;

const char* status_name(Status) noexcept;

}  // namespace dh2::level_world::quest_move_in_zone_collision_v1
