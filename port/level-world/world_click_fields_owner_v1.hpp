#pragma once
#include "world_click_target_v1.hpp"
namespace dh2::world {
// Retain exactly once inside the canonical Character transport. These source
// fields were not projected by TargetState48; no target/HP/FSM duplication.
struct WorldClickFieldsOwnerV1 {
 std::array<float,3> destination14b0{},origin14bc{};
 std::uint8_t pending14c8{0};std::int16_t skill14ca{-1};
 std::uint8_t click_target413{1}; // CharAIC2+4b source3cec64, NOT zero.
 WorldClickFieldsOwnerV1()=default;
 WorldClickFieldsOwnerV1(const WorldClickFieldsOwnerV1&)=delete;
 WorldClickFieldsV1 borrow(std::uintptr_t identity,character::TargetState48& same_target,character::TargetServices16 same_services)noexcept{
  return {identity,&pending14c8,&skill14ca,destination14b0.data(),origin14bc.data(),&click_target413,&same_target,same_services};
 }
};
}
