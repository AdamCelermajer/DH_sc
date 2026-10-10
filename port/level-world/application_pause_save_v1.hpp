#pragma once
#include <cstdint>
#include <functional>
#include <memory>
#include <string>

namespace dh2::application {
// Narrow model of Application::Pause's conditional player-save edge. The
// caller supplies the exact current Level owner and its source +0x130/+0x144
// fields; the callback must preserve that Level identity when delivering the
// source Level::SG_SavePlayer(0, false) operation.
bool application_pause_save_player_v1(
 const std::shared_ptr<void>& current_level,
 std::uint32_t phase130,
 std::uint8_t active144,
 const std::function<bool(const std::shared_ptr<void>&,std::int32_t,bool,std::string&)>& save_player,
 std::string& error);
}
