#pragma once
#include "player_manager_owner_v1.hpp"
namespace dh2::player {
// Exact query-relevant CMatching base/local constructor and Reset storage.
// This owner implements identity selection/IsServer/GetMemberId only; socket,
// room, NetStruct serialization and other matching-mode lifecycles are separate.
class MatchingLocalIdentityOwnerV4 {
 std::uint8_t joined_c_{}; // CMatchingC2 800084 stores0
 std::int32_t member3638_{-1},server363c_{-2}; // Local C1 + Reset
public:
 std::uintptr_t identity()const noexcept{return reinterpret_cast<std::uintptr_t>(this);}
 std::int32_t member()const noexcept{return member3638_;}
 std::int32_t server_member()const noexcept{return server363c_;}
 bool is_server()const noexcept{return joined_c_&&member3638_>=0&&member3638_==server363c_;}
 void source_reset_identity()noexcept{member3638_=-1;server363c_=-2;}
 // Explicit source field writes by future actual matching producers.
 void source_identity_store(std::uint8_t joined,std::int32_t member,std::int32_t server)noexcept{joined_c_=joined;member3638_=member;server363c_=server;}
};
class MatchingLocalSelectionOwnerV4 {
 std::uint32_t mode_{1}; // Actual .data99e234 initial1; singleton.bss is0.
 std::shared_ptr<MatchingLocalIdentityOwnerV4> local_;
public:
 bool get(std::shared_ptr<MatchingLocalIdentityOwnerV4>&,std::string&);
 std::uint32_t source_mode()const noexcept{return mode_;}
 // Actual mode producer may write only before the first source Get allocation.
 bool source_unallocated_mode_store(std::uint32_t,std::string&);
};
// Missing network-owner field of the SAME PlayerInfo, held as a typed ctor
// successor rather than appended to frozen PlayerInfoFieldsV1 binary layout.
struct PlayerInfoNetworkIdentityV4 {
 PlayerInfoFieldsV1* receiver{};std::shared_ptr<void> parent_lease;
 std::int32_t owner1a0{-1}; // CNetPlayerInfo.Reset80f290/94, AFTER C1
 std::uintptr_t identity()const noexcept{return reinterpret_cast<std::uintptr_t>(this);}
};
class PlayerNetworkLocalOwnerV4 {
 MatchingLocalSelectionOwnerV4& matching_;
 std::map<PlayerInfoFieldsV1*,std::shared_ptr<PlayerInfoNetworkIdentityV4>> records_;
public:
 explicit PlayerNetworkLocalOwnerV4(MatchingLocalSelectionOwnerV4& m):matching_(m){}
 // Call from original construct_player_info service BEFORE source manager
 // publication. This executes query-relevant source CNet C1/Reset backing;
 // no claim of packet/NetStruct constructor completion is made.
 bool construct(PlayerInfoFieldsV1&,std::shared_ptr<void>,std::string&);
 bool is_local(PlayerInfoFieldsV1&,bool&,std::string&);
 std::shared_ptr<PlayerInfoNetworkIdentityV4> borrow(PlayerInfoFieldsV1&)const;
 void erased(PlayerInfoFieldsV1& info)noexcept{records_.erase(&info);}
};
}
