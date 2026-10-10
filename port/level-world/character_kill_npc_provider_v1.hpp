#pragma once

#include "character_kill_death_tail_v1.hpp"
#include "character_ai_classification.hpp"
#include "player_kill_continuation_v1.hpp"

#include <cstdint>
#include <string>

namespace dh2::character_kill_npc_provider_v1 {
namespace ctrl = player_kill_continuation_v1;

// Services are projections of existing source owners. `preflight` is pure:
// it verifies the reached Kill branch can call every bound owner, without
// executing a Kill effect. IsPlayer uses the existing source-backed
// character_ai_classification query below rather than a renderer-side guess.
struct Services {
    void* context = nullptr;
    bool (*preflight)(void*, std::uintptr_t character,
                      std::uintptr_t killer, std::uint32_t force,
                      std::string& error) = nullptr;
    // Reuses the already-prepared player continuation for the virtual
    // IsPlayer && !force branch. It starts after the shared dead/HP prefix.
    bool (*player_branch)(void*, std::uintptr_t character,
                          std::uintptr_t killer, std::uint32_t force,
                          std::string& error) = nullptr;
    // Must dispatch the same Character::RaiseEvent(2,killer) path used by
    // Character::Ctrl_Kill, including its active-AIS and state-machine owners.
    bool (*raise_event2)(void*, std::uintptr_t character,
                         std::uintptr_t killer, std::string& error) = nullptr;
    character_kill_death_tail_v1::Services general_tail{};
};

struct Bindings {
    std::uintptr_t character = 0;
    // Canonical Character+0x1449 projection shared with combat and player
    // providers. The uint32 representation is a semantic adapter, not ABI.
    std::uint32_t* dead = nullptr;
    data::PropertyView* properties = nullptr;
    character_ai_classification::State* classification_state = nullptr;
    const character_ai_classification::Services* classification_services = nullptr;
    Services services{};
};

struct Snapshot {
    ctrl::Result ctrl_result{};
    character_kill_death_tail_v1::Result general_tail{};
    std::uint32_t inner_is_dead_reads = 0;
    std::uint32_t prefix_started = 0, dead_written = 0;
    std::uint32_t hp_zero_attempted = 0, hp_zero_completed = 0;
    std::uint32_t is_player_attempted = 0, is_player_completed = 0;
    std::int32_t is_player_result = 0;
    character_ai_classification::Result classification{};
    std::uint32_t player_branch_attempted = 0, player_branch_completed = 0;
    std::uint32_t general_branch_attempted = 0, general_branch_completed = 0;
    std::uint32_t event2_attempted = 0, event2_completed = 0;
};

// One source Kill/Ctrl_Kill episode for one canonical Character. The provider
// borrows dead/property/source owners; it owns only the CtrlCaller latch. Build
// a fresh provider for a real revive. NPC ingress reaches the general tail;
// an actual IsPlayer result still selects the source player branch.
class Runtime final {
    Bindings bindings_{};
    ctrl::CtrlCaller caller_;
    Snapshot snapshot_{};
    bool busy_ = false, kill_consumed_ = false, event2_consumed_ = false;

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

} // namespace dh2::character_kill_npc_provider_v1
