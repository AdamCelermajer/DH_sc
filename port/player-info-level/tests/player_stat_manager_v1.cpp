#include "../player_stat_manager_v1.hpp"

#include <cassert>
#include <cstdint>

using namespace dh2::player_stat_manager_v1;

int main() {
    constexpr auto player_id = 0xfffffffdu;
    const auto increment_call = increment(EStatType::defender, player_id);
    assert(increment_call.entry_point == EntryPoint::increment);
    assert(increment_call.stat == EStatType::defender);
    assert(increment_call.player_id_bits == player_id);
    assert(increment_call.amount == 1);
    assert(increment_call.source_address == 0x003790ec);
    assert(!increment_call.state_changed);

    const auto increase_call = increase(EStatType::xp, -17, player_id);
    assert(increase_call.entry_point == EntryPoint::increase);
    assert(increase_call.stat == EStatType::xp);
    assert(increase_call.player_id_bits == player_id);
    assert(increase_call.amount == -17);
    assert(increase_call.source_address == 0x003790e0);
    assert(!increase_call.state_changed);

    // Neither wrapper owns a store: repeated calls return independent receipts
    // and cannot accumulate an invented statistic.
    const auto repeated = increment(EStatType::defender, player_id);
    assert(repeated.amount == increment_call.amount);
    assert(!repeated.state_changed);
}
