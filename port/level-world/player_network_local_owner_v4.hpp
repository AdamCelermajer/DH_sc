#pragma once
#include "player_manager_owner_v1.hpp"
#include "../game-data/player_savegame_v1.hpp"
#include <array>
namespace dh2::player {
// Exact query-relevant CMatching base/local constructor and Reset storage.
// This owner implements identity selection/IsServer/GetMemberId only; socket,
// room, NetStruct serialization and other matching-mode lifecycles are separate.
class MatchingLocalIdentityOwnerV4 {
 std::uint8_t joined_c_{}; // CMatchingC2 800084 stores0
 std::int32_t member3638_{-1},server363c_{-2}; // Local C1 + Reset
 // CMatching C2 max1c=32,32 MemberInfoNetStruct C1 slots. Their first
 // Int member value150 is explicitly0, not member3638 or a player index.
 std::array<std::int32_t,32> member_values170_v107_{};
 std::int32_t max_members1c_v107_{32};
public:
 std::uintptr_t identity()const noexcept{return reinterpret_cast<std::uintptr_t>(this);}
 std::int32_t member()const noexcept{return member3638_;}
 std::int32_t server_member()const noexcept{return server363c_;}
 bool is_server()const noexcept{return joined_c_&&member3638_>=0&&member3638_==server363c_;}
 bool source_member_id_list_v107(std::vector<std::int32_t>& out,std::string& e)const{
  if(max_members1c_v107_>32){e="Unsupported actual CMatching max1c member storage";return false;}
  out.clear();for(std::int32_t i=0;i<max_members1c_v107_;++i)if(member_values170_v107_[i]>=0)out.push_back(member_values170_v107_[i]);e.clear();return true;
 }
 std::int32_t* source_member_value170_v107(std::uint32_t i)noexcept{return i<member_values170_v107_.size()?&member_values170_v107_[i]:nullptr;}
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
 // Additional query-relevant SAME CNet member values: C1 constructs0;
 // Reset changes owner1a0/room1c8 to-1 and preserves local178=0/state1f0=0.
 // Packet/member-declaration lists are not represented by these scalar lends.
 std::int32_t local178{},room1c8{-1},state1f0{};
 bool query_fields_produced_v67{};
 bool selected_player_info_v67{}; // explicit derived C1 vptr publication
 // Derived PlayerInfo NetStructInt360/value380: C1 assigns0, then source
 // Reset373c64 calls SetCharacterClass370ef0(-1). Produced only for that
 // derived constructor, not bare CNet or observed-owner adoption.
 std::int32_t character_class380_v68{};
 bool character_class_produced_v68{}; // Offline PlayerInfo Reset373bdc projections used by _ManageCharacters.
 // Source CString2d0 resets to actual empty literal; level330 to-1;
 // NetStructBool4c0/500 assignments copy false into values4e5/525.
 std::string character_name2d0_v70;
 std::int32_t character_level330_v70{-1},character_death_timer3a8_v70{-1};
 std::uint8_t character_visible4e5_v70{},character_loading525_v70{};
 // Source ClearLoadingInfo observed writes. Values are not readable as C1
 // projections until that actual operation produces them.
 std::uint8_t loading_member505_v70{},ready_to_roll545_v70{};
 // SAME NetStruct4e8.value1d (PlayerInfo505): SetInCutscene uses this cell.
 // The historical loading_member name does not imply a second loading flag.
 // Metadata is the real NetStructMember C1 successor: timestamp8=0,
 // masks10/14=-1, member18=0, dirty1c=0 (374450..47c/3747ac..7e0).
 struct MemberChangeV99 {
  std::uint64_t timestamp8{};
  std::uint32_t mask10{~std::uint32_t(0)},mask14{~std::uint32_t(0)},member18{};
  std::uint8_t dirty1c{};
 } cutscene_change4e8_v99,death_change388_v99;
 bool cutscene_members_produced_v99{};
 // Additional SAME PlayerInfo members reached by RemoveCharacter371d80.
 // 4a0 is NetStructInt(type32), NOT NetStructByteArray32; its value is4c0.
 std::int32_t remove_scalar4c0_v114{},character_xp448_v114{},character_gold498_v114{};
 // Actual NetStructByteArray36 at3b0: native-width data3d0 +signed size3d4.
 // C1 startsNULL/0; actual PlayerInfo.Reset fills36 bytes ff; tail empties it.
 std::unique_ptr<std::uint8_t[]> character_buffer3d0_v114;
 std::int32_t character_buffer_size3d4_v114{};
 MemberChangeV99 remove_change4a0_v114,class_change360_v114,name_change2b0_v114,
  level_change310_v114,buffer_change3b0_v114,xp_change428_v114,gold_change478_v114,ingame_change4c8_v114;
 bool remove_members_produced_v114{},buffer_empty_pending_v114{};

 bool loading_clear_produced_v70{};
 bool managed_fields_produced_v70{};
 std::shared_ptr<data::PlayerSavegameV1> profile680_v70;
 std::uintptr_t identity()const noexcept{return reinterpret_cast<std::uintptr_t>(this);}
};
class PlayerNetworkLocalOwnerV4 {
 MatchingLocalSelectionOwnerV4& matching_;
 std::map<PlayerInfoFieldsV1*,std::shared_ptr<PlayerInfoNetworkIdentityV4>> records_;
public:
 explicit PlayerNetworkLocalOwnerV4(MatchingLocalSelectionOwnerV4& m):matching_(m){}
 bool source_matching_members_v107(std::shared_ptr<MatchingLocalIdentityOwnerV4>& owner,std::vector<std::int32_t>& out,std::string& e){
  return matching_.get(owner,e)&&owner->source_member_id_list_v107(out,e);
 }
 // Call from original construct_player_info service BEFORE source manager
 // publication. This executes query-relevant source CNet C1/Reset backing;
 // no claim of packet/NetStruct constructor completion is made.
 bool construct(PlayerInfoFieldsV1&,std::shared_ptr<void>,std::string&);
 bool is_local(PlayerInfoFieldsV1&,bool&,std::string&);
 bool publish_player_info_dispatch_v67(PlayerInfoFieldsV1&,std::string&);
 bool source_reset_player_class_v68(PlayerInfoFieldsV1&,std::string&);
 bool source_selected_profile_class_v68(PlayerInfoFieldsV1&,std::int32_t,std::string&);
 // Whole selected NetStruct setters plus SetChanged814f84 over SAME values.
 // Network serialization and other PlayerInfo members remain separate.
 bool source_set_in_cutscene_v99(PlayerInfoFieldsV1&,bool,std::string&);
 bool source_reset_dead_local_v99(PlayerInfoFieldsV1&,std::string&);
 // Actual RemoveCharacter371d80 local destination-member tail only.
 bool source_remove_character_local_tail_v114(PlayerInfoFieldsV1&,std::string&);
 bool is_active_selected_v67(PlayerInfoFieldsV1&,std::uint8_t actual_online5,bool&,std::string&);
 std::shared_ptr<PlayerInfoNetworkIdentityV4> borrow(PlayerInfoFieldsV1&)const;
 void erased(PlayerInfoFieldsV1& info)noexcept{records_.erase(&info);}
};
}


