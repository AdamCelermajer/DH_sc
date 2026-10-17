#pragma once

#include "character_ai_events.hpp"
#include "../game-data/aggro.hpp"
#include "../game-data/properties.hpp"

#include <cstdint>
#include <string>

namespace dh2::player_enemy_kill_credit_v1 {

// Semantic source-slot identities from IDA. These are table tokens consumed by
// the native service adapter; they are not executable ARM addresses.
inline constexpr std::uintptr_t char_ai_on_kill_identity = 0x003d0d80u;
inline constexpr std::uintptr_t ais_player_on_kill_identity = 0x003ddb10u;

struct Bindings {
    std::uintptr_t character = 0;
    std::uintptr_t char_ai = 0;
    std::uintptr_t active_ais = 0;
    std::uintptr_t controller = 0;
    std::uintptr_t state_machine = 0;
    std::uintptr_t property_owner = 0;
    std::uint32_t forced = 0;
    std::uint32_t locked = 0;
    std::uint32_t paused = 0;
    std::uint32_t global_blocked = 0;
    const data::AggroTable* victim_outgoing = nullptr;
    data::PropertyView* properties = nullptr;
    std::uintptr_t* current_target = nullptr;
    void* target_context = nullptr;
    // Optional renderer/UI projection only. The canonical source target is
    // cleared by this kernel with the direct Character::Kill field write.
    int (*clear_matching_target)(void*, std::uintptr_t victim) = nullptr;
    // Supplies the real Coordinator fallback and the active AIS OnKill virtual.
    // For event4 the adapter emits ai_event_ais_virtual/+0xb0 with active_ais
    // as subject; the provider owns the same Player Session/VM.
    character::AIEventServices24 backend{};
};

struct Result {
    character::AIEventResult16 dispatch{};
    std::uint32_t event4_reached = 0;
    std::int32_t event4_status = 0;
    std::uint32_t event4_completed = 0;
    std::uint32_t target_cleared = 0;
    std::uint32_t target_projection_attempted = 0;
    std::int32_t target_projection_status = 0;
    std::int32_t property23_status = 0;
    std::uint32_t property23_added = 0;
    std::int32_t property24_status = 0;
    std::uint32_t property24_added = 0;
};

enum class Status : std::uint32_t {
    complete, ineligible_kill, ineligible_aggro, invalid_argument, busy, consumed, failed
};

// One already-reached Character::Kill recipient episode. Call after the
// initial DropLoot attempt, including a deferred/unsupported loot result.
// The instance is consumed once the supported source recipient starts event4;
// retries must not create another episode. The only modeled recipient is the
// sole Player identity in the victim's outgoing aggro table.
class Runtime {
    Bindings bindings_;
    bool busy_ = false;
    bool consumed_ = false;
public:
    explicit Runtime(Bindings);
    Status after_loot_attempt(std::uintptr_t victim, std::uintptr_t killer,
                              std::uint32_t kill_force, Result*, std::string& error);
};

} // namespace dh2::player_enemy_kill_credit_v1
