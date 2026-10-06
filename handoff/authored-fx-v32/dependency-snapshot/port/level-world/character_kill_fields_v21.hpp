#pragma once
#include <cstdint>
#include <string>
namespace dh2::character {
// Only missing C1-backed fields. No life/HP/OOI/property/room/Handle copy.
// Embed ONCE on the actual receiver and retain it across renderer restoration.
struct CharacterKillFieldsV21 {
 std::uintptr_t killer144c{},master14d4{};
 std::int16_t template13ca{};std::uint8_t suppress_quest14e4{};
 bool produced{};
 bool construct_fresh(std::string&);
 bool adopt_observed(std::uintptr_t actual_killer,std::uintptr_t actual_master,
  std::int16_t actual_template,std::uint8_t actual_suppress,std::string&);
};
}
