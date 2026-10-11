#pragma once

#include <cstdint>
#include <string>

namespace dh2::ais_default_interact_v1 {

struct Request {
    std::uintptr_t ai_identity{};
    std::uintptr_t owner_character{};
    std::uintptr_t target_game_object{};
};

struct Services {
    void* context{};
    bool (*is_character)(void*, std::uintptr_t, bool&, std::string&){};
    bool (*is_monster)(void*, std::uintptr_t, bool&, std::string&){};
    // Dispatches the target's retained typed Interact provider and returns
    // the target's source +0x2fa retention byte after the call.
    bool (*interact)(void*, std::uintptr_t target, std::uintptr_t owner,
                     std::uint8_t& retain_target, std::string&){};
    bool (*is_merchant)(void*, std::uintptr_t, bool&, std::string&){};
    bool (*clear_target)(void*, std::uintptr_t ai, std::string&){};
    bool (*sync_last_target)(void*, std::uintptr_t ai, std::string&){};
};

struct Result {
    bool interaction_dispatched{};
    bool target_cleared{};
    bool last_target_synchronized{};
};

enum class Status : std::uint8_t {
    complete,
    invalid_argument,
    provider_unavailable,
    provider_failed,
};

// AISDefault::OnInteract (IDA 0x3dc1b0). The source calls the target's
// GameObject::Interact(owner) before reading +0x2fa. It then retains a
// marked target, or a Character merchant; otherwise it calls
// AI_SetTarget(nullptr,false) followed by AI_SyncLastTarget.
Status on_interact(const Request&, const Services&, Result&,
                   std::string& error) noexcept;

} // namespace dh2::ais_default_interact_v1
