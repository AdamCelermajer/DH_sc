#pragma once
#include "player_manager_owner_v1.hpp"
#include <functional>
namespace dh2::player {
struct PlayerQuickSaveLevelV29 {
 std::shared_ptr<void> owner;std::uintptr_t identity{};
 const std::shared_ptr<void>* save_ec{};const std::uint32_t* phase130{};
};
struct PlayerQuickSaveCharacterV29 {
 std::shared_ptr<void> owner;std::uintptr_t identity{};
 const std::uint32_t* dead{};const float* position160{};float* checkpoint1468{};
};
struct PlayerQuickSaveServicesV29 {
 std::shared_ptr<void> owner;PlayerManagerOwnerV1* players{};
 std::function<bool(std::uintptr_t,PlayerQuickSaveCharacterV29&,std::string&)> character;
 std::function<bool(std::uint8_t&,std::string&)> online_byte5;
 std::function<bool(bool&,std::string&)> local_hosting;
 const std::uint8_t* player_manager719{};
 std::function<bool(const std::shared_ptr<void>&,std::uint8_t*&,std::string&)> save_flag39;
 std::function<bool(const std::shared_ptr<void>&,std::string&)> save_ec;
};
enum class PlayerQuickSavePhaseV29 {query_player,gate,alive,online,checkpoint,flag,save,restore,complete};
struct PlayerQuickSaveReceiptV29 {PlayerQuickSavePhaseV29 phase{};std::uintptr_t player{};bool saved{},complete{};};
// Whole Level::QuickSave3f059c. It borrows the real Level130 and Save_ec;
// source C1 phase0 takes a completed guard return after actual GetLocalPlayer.
class PlayerLevelQuickSaveV29 {
 PlayerQuickSaveServicesV29 services_;PlayerQuickSaveReceiptV29 receipt_;bool busy_{};
public:
 explicit PlayerLevelQuickSaveV29(PlayerQuickSaveServicesV29 s):services_(std::move(s)){}
 bool execute(const PlayerQuickSaveLevelV29&,bool source_disable_flag,std::string&);
 const PlayerQuickSaveReceiptV29& receipt()const noexcept{return receipt_;}
};
}
