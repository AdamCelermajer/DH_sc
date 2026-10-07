#pragma once
#include <cstdint>
#include <memory>
#include <string>
namespace dh2::player {
// Source-query projection of lazily constructed process singleton receivers.
// GameCenter C1 32bccc writes11=0; OnlineGameState C1 4a007c writes28=0.
// Their other startup/network/platform methods are not represented here.
class PlayerManagerOfflineSelectorsV70 {
 std::uint8_t game_center11_{},online_alias28_{};
public:
 const std::uint8_t& game_center_enabled11()const noexcept{return game_center11_;}
 const std::uint8_t& online_alias_enabled28()const noexcept{return online_alias28_;}
 // Only real later source producer may publish these cells. Enabling either
 // branch requires its actual corresponding alias service; no invented name.
 void source_game_center_enabled_store(std::uint8_t value)noexcept{game_center11_=value;}
 void source_online_alias_enabled_store(std::uint8_t value)noexcept{online_alias28_=value;}
};
std::shared_ptr<PlayerManagerOfflineSelectorsV70> process_player_manager_offline_selectors_v70();
}
