#pragma once
#include "player_save_load_owner_v1.hpp"
namespace dh2::data {
enum class PlayerSaveWriteOpV1 : std::uint32_t {
 online,local_hosting,hosting_quest_flag,synchronize,setup_sections,
 profile_has_cache,cache_profile,save_all,delete_level_checkpoint,
 delete_player_checkpoint,pack_volatile_quests
};
struct PlayerSaveWriteRequestV1 {
 PlayerSaveWriteOpV1 operation;
 PlayerSaveLoadOwnerV1* authority{};
 PlayerSavegameV1* save{};
 PlayerSaveProfileV1 profile{};
 bool first{},second{};
};
struct PlayerSaveWriteResponseV1 {bool flag{};};
struct PlayerSaveWriteServicesV1 {
 // Genuine reached effects only. save_all must execute the real named writers
 // and retain actual file/job buffers. It cannot be an empty notification.
 std::shared_ptr<void> owner;
 std::function<bool(const PlayerSaveWriteRequestV1&,PlayerSaveWriteResponseV1&,std::string&)> invoke;
};
class PlayerSaveWriteOwnerV1 {
 std::shared_ptr<PlayerSaveLoadOwnerV1> authority_;
 PlayerSaveWriteServicesV1 services_;
 bool running_{};
 std::uint32_t phase_{},calls_{};
 bool send(PlayerSaveWriteOpV1,PlayerSaveWriteResponseV1&,std::string&,bool=false,bool=false);
 bool volatile_quests(std::string&);
public:
 PlayerSaveWriteOwnerV1(std::shared_ptr<PlayerSaveLoadOwnerV1>,PlayerSaveWriteServicesV1={});
 PlayerSaveWriteOwnerV1(const PlayerSaveWriteOwnerV1&)=delete;
 PlayerSaveWriteOwnerV1& operator=(const PlayerSaveWriteOwnerV1&)=delete;
 PlayerSaveLoadOwnerV1& authority()const noexcept{return *authority_;}
 bool save(std::string&);
 std::uint32_t reached_phase()const noexcept{return phase_;}
 std::uint32_t delivered_calls()const noexcept{return calls_;}
};
}
