#include "player_stat_manager_v1.hpp"

namespace dh2::player_stat_manager_v1 {

Invocation increment(EStatType stat, std::uint32_t player_id_bits) noexcept {
    // The original wrapper supplies a delta of one and branches directly to
    // IncreaseStat. Its body is BX LR, so no persistent value is changed.
    return {EntryPoint::increment, stat, player_id_bits, 1, 0x003790ec, false};
}

Invocation increase(EStatType stat, std::int32_t amount,
                    std::uint32_t player_id_bits) noexcept {
    // Exact body at 0x3790e0 is BX LR. Preserve raw caller arguments in the
    // receipt without manufacturing a counter update or clamping behavior.
    return {EntryPoint::increase, stat, player_id_bits, amount, 0x003790e0, false};
}

} // namespace dh2::player_stat_manager_v1
