#pragma once
#include "../game-data/loot_tables_v2.hpp"
#include <functional>
#include <memory>
namespace dh2::world {
struct GameObjectSpawnProbabilityBorrowV1 {
 std::shared_ptr<void> owner;
 std::int32_t* cached_roll270{};
 const std::int32_t* probability274{};
 const std::int32_t* network_id108{};
 const std::int32_t* online_owner_fc{};
 std::uint8_t* byte82{};
 // Same original Random channel0/channel1 owners used by other World callers.
 data::LootRandom8V2* random0{};data::LootRandom8V2* random1{};
 std::function<bool(bool&,std::string&)> handle_as_player_character,online_byte5;
 std::function<bool(std::string&)> set_visible_false,object_base_delete,mark_for_deletion;
};
bool game_object_check_spawn_probability_v1(GameObjectSpawnProbabilityBorrowV1&,
 std::int32_t& roll,std::int32_t& probability,std::string&);
// Original38ab60 is mov r0,#1;bx lr (independent of XML condition spelling).
bool game_object_meet_condition_v1(bool&,std::string&);
}
