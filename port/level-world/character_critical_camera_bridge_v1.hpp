#pragma once

#include "../game-data/combat_result.hpp"
#include "camera_animset_v1.hpp"

#include <cstdint>

namespace dh2::character_critical_camera_bridge_v1 {

enum class Query : std::uint32_t {
    requester_is_player,
    local_player_count,
    requester_is_local
};

struct QueryRequest {
    Query operation;
    std::uintptr_t character;
};

using Invoke = std::int32_t (*)(void*, const QueryRequest&, std::int32_t*);

struct Request {
    const data::CombatResult* result;
    std::uintptr_t attacker;
    void* context;
    Invoke invoke;
};

struct Report {
    std::uint32_t queries_attempted = 0;
    std::uint32_t queries_completed = 0;
    std::uint32_t combat_result_player_gate = 0;
    std::uint32_t camera_can_play = 0;
    std::uint32_t play_requested = 0;
};

enum class Status : std::uint32_t {
    complete,
    invalid_argument,
    provider_failed,
    request_failed
};

// Character::F_ApplyResult's critical-camera branch. It first checks the
// mutable AttackResult critical outcome and attacker IsPlayer, then calls
// CameraLevel::CanPlayShakeAnim, which performs a fresh IsPlayer query and,
// for Players only, requires exactly one local player and IsLocalPlayer.
// The existing PlaybackOwner produces the source Crit/PlayAnim(id,0,true)
// request; its existing bank/rig playback owner executes that request.
Status resolve(const Request*, camera_animset_v1::PlaybackOwner&,
               camera_animset_v1::PlayRequest*, Report*);

} // namespace dh2::character_critical_camera_bridge_v1
