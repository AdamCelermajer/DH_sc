#pragma once

#include <cstdint>

namespace dh2::ais_faery_update_v1 {

struct Character {
    std::uintptr_t identity{};
    std::uintptr_t master{};
    std::uintptr_t visual{};
};

struct State {
    std::uintptr_t identity{};
    Character* character{};
    std::int32_t last_faery_id{-1};
};

enum class Operation : std::uint32_t {
    default_on_update,
    current_faery_id,
    set_modular_skin,
};

struct Request {
    Operation operation{};
    std::uintptr_t subject{};
    std::int32_t argument0{};
    std::int32_t argument1{};
};

struct Services {
    void* context{};
    // Routes to the existing AISDefault, Save/current-faery, and VisualObject
    // owners. A zero return is success; no timer, Save, or visual is created.
    int (*invoke)(void*, const Request&, std::int32_t*){};
};

struct Result {
    std::uint32_t callbacks{};
    std::int32_t current_faery_id{-1};
    bool visual_changed{};
};

enum class Status { complete, invalid_argument, service_unavailable, failed };

// AISFaery::OnUpdate @ 0x3de544. Calls AISDefault::OnUpdate first, then reads
// the Character's master and visual; for a changed SG_GetCurrentFaerieId(-1),
// rereads the source value, stores it in AISFaery+0xc4, and calls
// VisualObject::SetModularSkin(0, id). The owner and services are borrowed.
Status update(State*, const Services*, Result*);

} // namespace dh2::ais_faery_update_v1
