#pragma once

#include "destructible_container_source_owner_v1.hpp"
#include "destructible_container_table_v1.hpp"

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>

namespace dh2::destructible_container_visual_provider_v1 {

using Facts = destructible_container_source_owner_v1::InitPostFacts;

enum class Status : std::uint8_t {
    complete,
    invalid_argument,
    invalid_source_contract,
    table_row_unavailable,
    visual_path_mismatch,
    bdae_invalid,
    scene_invalid,
    mesh_box_unavailable,
    timeline_state_unresolved,
};

// Builds the bounded facts consumed by DestructibleContainer InitPost from the
// exact source MGP row, its resolved DestructibleContainers table row, and the
// matching GameObjectDict BDAE. `visual_source_path` is the path used to load
// `bdae`; it must resolve to the path selected by the table's Visual ID.
// condition_enabled/deleted_81 are runtime facts supplied by the source
// condition and ObjectManager providers; this helper does not evaluate them.
Status build_init_post_facts(
    const world::SourceObjectCensusV1& source,
    const destructible_container_table_v1::Tables& tables,
    std::string_view visual_source_path,
    const std::uint8_t* bdae, std::size_t bdae_size,
    std::uint8_t condition_enabled, std::uint8_t deleted_81,
    Facts& out, std::string& error) noexcept;

} // namespace dh2::destructible_container_visual_provider_v1
