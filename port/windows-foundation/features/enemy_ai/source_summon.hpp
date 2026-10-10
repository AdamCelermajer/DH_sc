#pragma once

#include "source_create_npc.hpp"
#include "../../../script-runtime/script_runtime.h"

#include <array>
#include <functional>
#include <memory>
#include <string>

namespace dh2::enemy_ai {

struct SourceSummonOwnerV1 {
 std::shared_ptr<void> lease;
 std::uintptr_t identity{};
 std::uintptr_t room{}; // Same caller GameObject+2f4, NULL selects no-room path.
 const float* position160{}; // Same caller GameObject world XYZ.
 const float* rotation16c{}; // Same caller GameObject Euler XYZ.
};

struct SourceSummonServicesV1 {
 SourceSummonOwnerV1 owner;
 const data::CharacterTable* characters{};
 std::function<bool(std::int32_t,SourceCreateNpcResultV1&,std::string&)> create_npc;
 std::function<bool(std::uintptr_t,std::array<float,3>&,
  std::array<float,3>&,std::string&)> source_object_pose;
 std::function<bool(std::uintptr_t,const std::array<float,3>&,
  const std::array<float,3>&,const std::array<float,3>&,bool,
  std::array<float,3>&,std::string&)> source_position_from_offsets;
 std::function<bool(world::CanonicalClassReceiverV1&,const std::array<float,3>&,std::string&)> set_initial_position;
 std::function<bool(world::CanonicalClassReceiverV1&,const std::array<float,3>&,bool,std::string&)> set_position;
 std::function<bool(world::CanonicalClassReceiverV1&,const std::array<float,3>&,std::string&)> set_rotation;
 std::function<bool(world::CanonicalClassReceiverV1&,std::string&)> mark_summoned;
 std::function<bool(std::uintptr_t,std::uintptr_t,bool&,std::string&)> room_add_initial_object;
 std::function<bool(world::CanonicalClassReceiverV1&,std::string&)> manager_add_no_room;
 std::function<bool(world::CanonicalClassReceiverV1&,std::string&)> zone_entered;
 std::function<bool(world::CanonicalClassReceiverV1&,bool,bool,std::string&)> set_spawn_state;
 std::function<bool(world::CanonicalClassReceiverV1&,std::int32_t,std::string&)> forward_animation;
 std::function<bool(bool&,std::string&)> online_byte5;
 std::function<bool(world::CanonicalClassReceiverV1&,std::string&)> assign_network_id;
 // Source writes Character network fields and continues the actual online
 // state path after AssignObjectNetworkId; required only when online byte5=1.
 std::function<bool(world::CanonicalClassReceiverV1&,std::uintptr_t,std::string&)> online_post_assignment;
};

// GameObject::_Summon 0x39193c. Values/owners are borrowed for this synchronous
// call. Required services fail at the source prefix; no local actor/FSM or
// population selector is constructed here.
int source_summon_callback_v1(void*,const dh2_script_value*,std::uint32_t,
 dh2_script_value*,std::uint32_t,std::uint32_t*,char*,std::size_t);

} // namespace dh2::enemy_ai
