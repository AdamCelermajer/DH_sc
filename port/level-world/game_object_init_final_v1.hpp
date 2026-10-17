#pragma once

#include "character_runtime_factory_v1.hpp"

#include <cstdint>
#include <string>
#include <string_view>

namespace dh2::game_object_init_final_v1 {

enum class Operation : std::uint8_t {
    check_spawn_probability,
    set_enabled_true,
    init_pf_object,
    sync_visual,
    find_target_node,
    update_pf_object,
};

struct Request {
    Operation operation{};
    const char* name{};
};

struct Reply {
    std::int32_t word{};
    std::uintptr_t identity{};
};

struct Services {
    void* context{};
    // Providers must operate on this exact factory Record and its existing
    // level/visual/physical owners. In particular, init_pf_object is the
    // source PFWorld::InitObject path, not the Faery Box2D body constructor.
    int (*invoke)(void*, character_runtime_factory_v1::Record&,
                  const Request&, Reply&, std::string&){};
};

enum class Status : std::uint8_t { complete, invalid_argument, busy, failed };
enum class Decision : std::uint8_t {
    not_started,
    probability_rejected,
    deleted,
    initialized,
};

struct Result {
    Decision decision{Decision::not_started};
    std::uint32_t entered_calls{};
    std::uint32_t last_operation{};
    std::uint32_t stores{};
    std::int32_t spawn_probability_result{};
    std::uintptr_t character{};
};

// Exact LightSetManager::GetLightSetIdFromName lookup order from its source
// constructor. Unknown strings return row zero, as the native loop does.
std::int32_t light_set_id_from_name(std::string_view name) noexcept;

// Source GameObject::InitFinal body at 0x38cd48. Calls remain explicit so the
// caller can bind PFWorld, VisualObject, scene-node, and PFObject services to
// the current Level and this Character Record. Reached provider failures leave
// prior source-order writes intact. The existing Faery CharacterVisual is the
// canonical visual facade for this factory path; no second visual owner is
// created here.
Status initialize(character_runtime_factory_v1::Record&,
                  Services, Result*, std::string& error);

} // namespace dh2::game_object_init_final_v1
