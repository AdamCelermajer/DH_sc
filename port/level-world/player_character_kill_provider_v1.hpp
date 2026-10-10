#pragma once

#include "player_kill_continuation_v1.hpp"

#include <cstdint>
#include <string>

namespace dh2::player_character_kill_provider_v1 {
namespace ctrl = player_kill_continuation_v1;

// Character::RaiseEvent(2, killer) first dispatches the active AIS OnDied,
// then forwards event 2 to the same Character state-machine owner.
struct EventServices {
    void* context = nullptr;
    int (*on_died)(void*, std::uintptr_t character, std::uintptr_t killer,
                   std::string& error) = nullptr;
    int (*state_event)(void*, std::uintptr_t character, std::uint32_t event,
                       std::uintptr_t payload, std::string& error) = nullptr;
};

struct Bindings {
    std::uintptr_t character = 0;
    // Semantic native projection of the source Character+0x1449 byte. This
    // must be the same dead state read by the combat/player owners.
    std::uint32_t* dead = nullptr;
    data::PropertyView* properties = nullptr;
    ctrl::Backend continuation{};
    EventServices events{};
};

struct Snapshot {
    ctrl::Result ctrl_result{};
    ctrl::Result continuation_result{};
    std::uint32_t outer_is_dead_reads = 0;
    std::uint32_t inner_is_dead_reads = 0;
    std::uint32_t kill_prefix_started = 0;
    std::uint32_t kill_prefix_completed = 0;
    std::uint32_t on_died_started = 0;
    std::uint32_t on_died_completed = 0;
    std::uint32_t state_event_started = 0;
    std::uint32_t state_event_completed = 0;
};

// One Character::Ctrl_Kill / Character::Kill episode. The caller binds the
// returned CtrlCaller into the existing shared ingress bridge. The inner
// provider performs the exact IsDead -> dead byte -> PROPS_Set(36,0) prefix,
// delegates the player/generic tail to the selected Kill runtime, and lets
// CtrlCaller issue event 2 only after successful Kill return.
//
// Keep this owner alive for one death episode. A later revive needs a fresh
// player-kill continuation and provider bound to the same canonical owners.
class Runtime final {
    Bindings bindings_{};
    ctrl::Runtime continuation_;
    ctrl::CtrlCaller caller_;
    Snapshot snapshot_{};
    bool busy_ = false;
    bool kill_consumed_ = false;
    bool event_consumed_ = false;

    static int invoke(void*, const ctrl::Request*, ctrl::Reply*,
                      std::string& error);
    int dispatch(const ctrl::Request&, ctrl::Reply&, std::string& error);
    int inner_kill(std::uintptr_t killer, std::uint32_t force,
                   std::string& error);
    int raise_event2(std::uintptr_t killer, std::string& error);

public:
    explicit Runtime(Bindings);
    Runtime(const Runtime&) = delete;
    Runtime& operator=(const Runtime&) = delete;

    ctrl::CtrlCaller& caller() noexcept { return caller_; }
    const Snapshot& snapshot() const noexcept { return snapshot_; }
};

} // namespace dh2::player_character_kill_provider_v1
