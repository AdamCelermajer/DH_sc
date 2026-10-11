#pragma once

#include "object_manager_runtime_owner_v1.hpp"
#include "world.hpp"

#include <cstdint>
#include <string>

namespace dh2::destructible_container_source_owner_v1 {

using Manager = object_manager_runtime_owner_v1::Owner;
using GameObject = object_manager_runtime_owner_v1::GameObject;

// The source owner must pass the completed absolute AABB from GameObject's
// InitPost/VisualObject::ApplyMeshBox path. These are XY min/max values in the
// same world units consumed by GameObject::GetInteractionRadius.
struct AbsoluteAabbXY {
    float min_x{};
    float min_y{};
    float max_x{};
    float max_y{};
    std::uint8_t ready{};
};

struct InitPostFacts {
    AbsoluteAabbXY aabb{};
    std::int32_t container_state_394{};
    std::uint8_t deleted_81{};
    std::uint8_t condition_enabled{1};
    std::uint8_t visible_80{1};
    std::uint8_t save_eligibility_28{1};
};

struct Record {
    world::SourceObjectCensusV1 source{};
    GameObject* object{};
    // InitFinal's PODecor body reads the same completed GameObject AABB that
    // InitPost measured from the retained VisualObject. Keep those source
    // bounds with the existing Container owner for its one later phase.
    InitPostFacts init_post_facts{};
    std::uint8_t visible_80{};
    std::uint8_t enabled_8a{1};
    std::uint8_t zoning_enabled_2ee{};
    std::uint8_t deleted_81{};
    std::uint8_t init_final_ready{};
    std::int32_t source_module_id{-1};
    float world_x{};
    float world_y{};
};

enum class Status : std::uint8_t {
    complete,
    invalid_argument,
    invalid_source_contract,
    aabb_unavailable,
    invalid_source_state,
    manager_failed,
};

// Produces one canonical ObjectManager row and appends its identity to the
// canonical room-object list. The source handle, module/record provenance,
// source position, class state, deleted flag, and interaction radius remain
// on that row. A failed publication is rolled back from the same manager.
Status initialize_init_post(Manager&, const world::SourceObjectCensusV1&,
                            const InitPostFacts&, Record&,
                            std::string& error) noexcept;

// Source GetInteractionRadius returns half the smaller completed XY AABB
// extent. No fallback dimension is invented when the AABB is unavailable.
Status interaction_radius(const AbsoluteAabbXY&, float& radius,
                          std::string& error) noexcept;

// Container::InitFinal creates its PODecor body from the same source absolute
// AABB. Returns those four bounds without reconstructing or padding them a
// second time.
Status init_final_absolute_bounds(const Record&, float bounds[4],
                                  std::string& error) noexcept;

} // namespace dh2::destructible_container_source_owner_v1
