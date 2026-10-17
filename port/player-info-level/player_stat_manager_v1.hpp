#pragma once

#include <cstdint>

namespace dh2::player_stat_manager_v1 {

// EStatType values recovered from PlayerStatManager::GetStatStr at 0x3795c8.
enum class EStatType : std::uint32_t {
    killer = 0,
    striker = 1,
    controller = 2,
    defender = 3,
    looter = 4,
    support = 5,
    xp = 6,
};

enum class EntryPoint : std::uint8_t { increment, increase };

// This is a call receipt, not a player-stat store. In the source ELF the
// singleton constructor only installs its vtable; IncreaseStat is BX LR,
// and IncrementStat forwards to it. No counters or lifecycle data are owned
// by PlayerStatManager in this build.
struct Invocation {
    EntryPoint entry_point{};
    EStatType stat{};
    std::uint32_t player_id_bits{};
    std::int32_t amount{};
    std::uint32_t source_address{};
    bool state_changed{};
};

// Source PlayerStatManager::IncrementStat(EStatType, int), 0x3790ec.
Invocation increment(EStatType stat, std::uint32_t player_id_bits) noexcept;

// Source PlayerStatManager::IncreaseStat(EStatType, int, int), 0x3790e0.
Invocation increase(EStatType stat, std::int32_t amount,
                    std::uint32_t player_id_bits) noexcept;

} // namespace dh2::player_stat_manager_v1
