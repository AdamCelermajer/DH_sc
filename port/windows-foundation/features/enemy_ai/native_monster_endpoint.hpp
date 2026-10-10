#pragma once
#include "live_enemy_binding.hpp"
namespace dh::foundation::enemy_ai {
struct NativeMonsterReceiver {
    LiveEnemyBorrow live;
    std::uint8_t* heading_enabled412{};
};
// Actual AISMonster+98 Character pointer is reloaded at original load points.
// Callback services execute whole named source functions, never guessed FSM
// events. This facade does not construct/select an AIS or own any fields.
struct NativeMonsterServices {
    std::function<bool(NativeMonsterReceiver&,std::string&)> receiver;
    std::function<bool(const NativeMonsterReceiver&,ActorId,std::uint32_t mode,std::string&)> set_target;
    std::function<bool(void* captured_controller,std::string&)> controller_stop;
    std::function<bool(ActorId captured_target,std::array<float,3>&,std::string&)> target_position;
    std::function<bool(void* captured_controller,const std::array<float,3>&,std::string&)> controller_move_point;
    std::function<bool(const NativeMonsterReceiver&,std::string&)> clear_all_aggro;
    std::function<bool(std::uint32_t original_function,const NativeMonsterReceiver&,std::string&)> inherited;
};
bool native_monster_target_event(std::uint32_t character_event,ActorId payload,
    const NativeMonsterServices&,std::string&);
}
