#pragma once
#include "player_manager_owner_v1.hpp"
#include <functional>
namespace dh2::player {
// Whole Update378fb4 orchestration. Stage receivers are original manager bodies,
// not readiness callbacks; this owner deliberately does not implement them by
// replaying AddPlayer or by publishing a character count.
enum class PlayerManagerUpdateStageV70 : std::uint32_t {
 check_online_transition=0x37193c, check_local_controllers=0x378c80,
 check_remote_controllers=0x378b94, manage_characters=0x37280c,
 check_local_deaths=0x376100, check_global_deaths=0x375eb4,
 statistics_update=0x3790d8,
 keyboard_update=statistics_update // compatibility spelling; actual PlayerStatManager.Update
};
struct PlayerManagerUpdateServicesV70 {
 std::shared_ptr<void> provider;
 std::function<bool(std::uintptr_t,const std::uint8_t*&,std::shared_ptr<void>&,std::string&)> disabled81;
 std::function<bool(PlayerInfoFieldsV1&,const std::uint8_t*&,std::shared_ptr<void>&,std::string&)> visible4e5;
 std::function<bool(PlayerManagerUpdateStageV70,PlayerManagerOwnerV1&,std::string&)> stage;
};
class PlayerManagerUpdateOwnerV70 {
 PlayerManagerOwnerV1& manager_;
 PlayerManagerUpdateServicesV70 services_;
 bool delivering_{};
public:
 PlayerManagerUpdateOwnerV70(PlayerManagerOwnerV1& m,PlayerManagerUpdateServicesV70 s):manager_(m),services_(std::move(s)){}
 bool update(std::string&);
};
}

