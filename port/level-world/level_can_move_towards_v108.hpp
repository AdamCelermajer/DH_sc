#pragma once
#include <array>
#include <functional>
#include <memory>
#include <string>
namespace dh2::world {
struct LevelCanMovePartyV108 {std::uintptr_t character{};bool dead{};const float* position160{};std::shared_ptr<void> receiver;};
struct LevelCanMoveServicesV108 {
 std::shared_ptr<void> owner;
 std::function<bool(std::int32_t&,std::string&)> count6c4;
 std::function<bool(std::uint8_t&,std::string&)> online5;
 std::function<bool(bool&,std::array<float,3>&,std::string&)> camera_parent4;
 std::function<bool(std::array<float,3>&,std::string&)> limits; //sides,top,bottom source row
 std::function<bool(std::uint32_t,const char*,std::string&)> assertion;
 std::function<bool(unsigned,LevelCanMovePartyV108&,std::string&)> player;
 std::function<bool(const float*,std::array<float,2>&,std::string&)> screen;
};
bool level_can_move_towards_v108(const float* position160,const float* heading1b8,
 const LevelCanMoveServicesV108&,bool&,std::string&);
}
