#pragma once
#include "player_network_local_owner_v4.hpp"
#include <array>
namespace dh2::player {
// Additive typed backing for the SAME PlayerInfo's NetStructByteArray3/+3d8
// and NetStructByteArray30/+400. No frozen PlayerInfoFieldsV1 layout change.
// Only the constructor's final PlayerInfo::Reset prefix is implemented here;
// serialization/dirty callbacks and _ManageCharacters writes are separate.
class PlayerInfoSkillBuffersV26 {
 PlayerInfoFieldsV1* receiver_{};
 std::shared_ptr<PlayerInfoNetworkIdentityV4> network_;
 std::array<std::int8_t,3> slots_;
 std::array<std::int8_t,30> levels_;
 bool constructed_{};
public:
 bool construct(PlayerInfoFieldsV1&,PlayerNetworkLocalOwnerV4&,std::string&);
 bool get_slots(PlayerInfoFieldsV1&,void* destination,std::size_t capacity,
                std::int32_t& copied,std::string&)const;
 bool get_levels(PlayerInfoFieldsV1&,void* destination,std::size_t capacity,
                 std::int32_t& copied,std::string&)const;
 const PlayerInfoFieldsV1* receiver()const noexcept{return receiver_;}
};
}
