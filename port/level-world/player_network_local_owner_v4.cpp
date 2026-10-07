#include "player_network_local_owner_v4.hpp"
#include <exception>
#include <cstring>
namespace dh2::player {
namespace {
// Actual NetStruct::s_changeCounter a33530 is process BSS0, not App dt/frame.
// This is shared by the supported native member setters, never per-player.
std::uint64_t source_change_counter_v99{};
void member_changed_v99(PlayerInfoNetworkIdentityV4::MemberChangeV99& field) noexcept {
 field.dirty1c=1;field.mask14=field.member18;field.mask10=field.member18;
 field.timestamp8=source_change_counter_v99++;
}
}
bool MatchingLocalSelectionOwnerV4::get(std::shared_ptr<MatchingLocalIdentityOwnerV4>& out,std::string& e){
 if(local_){out=local_;return true;}
 // Get800fb0..800fc8 stores1 before invoking Local C1 when static mode0.
 if(!mode_)mode_=1;
 if(mode_!=1){e="Required original nonlocal Matching constructor for source mode "+std::to_string(mode_);return false;}
 auto candidate=std::make_shared<MatchingLocalIdentityOwnerV4>();
 candidate->source_reset_identity(); // Local C1 invokes Reset807e28.
 local_=std::move(candidate);out=local_;return true;
}
bool MatchingLocalSelectionOwnerV4::source_unallocated_mode_store(std::uint32_t mode,std::string& e){
 if(local_){e="Actual Matching replacement/destructor required before changing selected mode";return false;}
 mode_=mode;return true;
}
bool PlayerNetworkLocalOwnerV4::construct(PlayerInfoFieldsV1& record,std::shared_ptr<void> lease,std::string& e){
 if(!lease){e="Required same PlayerInfo parent constructor lease";return false;}
 auto actual=std::make_shared<PlayerInfoNetworkIdentityV4>();actual->receiver=&record;actual->parent_lease=std::move(lease);
 // CNet C1 initializes integer member+180/value1a0 then Reset80f27c
 // writes−1. Publish this narrow backing only after that source field store.
 actual->local178=0;actual->owner1a0=-1;actual->room1c8=-1;actual->state1f0=0;
 actual->query_fields_produced_v67=true;records_[&record]=std::move(actual);return true;
}
std::shared_ptr<PlayerInfoNetworkIdentityV4> PlayerNetworkLocalOwnerV4::borrow(PlayerInfoFieldsV1& record)const{
 auto at=records_.find(&record);return at==records_.end()?nullptr:at->second;
}
bool PlayerNetworkLocalOwnerV4::is_local(PlayerInfoFieldsV1& record,bool& out,std::string& e){
 auto player=borrow(record);if(!player||player->receiver!=&record){e="Required same PlayerInfo CNet owner1a0 constructor successor";return false;}
 std::shared_ptr<MatchingLocalIdentityOwnerV4> matching;
 if(!matching_.get(matching,e))return false;
 // Source80f1ec loads owner AFTER actual matching.IsServer delivery.
 const bool server=matching->is_server();const auto owner=player->owner1a0;
 if(server&&owner<0){out=true;return true;}
 out=owner==matching->member();return true;
}
bool PlayerNetworkLocalOwnerV4::publish_player_info_dispatch_v67(PlayerInfoFieldsV1& record,std::string& e){
 auto actual=borrow(record);
 if(!actual||actual->receiver!=&record||actual->selected_player_info_v67){e="Required fresh SAME CNet base before derived PlayerInfo vptr publication";return false;}
 actual->selected_player_info_v67=true;e.clear();return true;
}
bool PlayerNetworkLocalOwnerV4::is_active_selected_v67(PlayerInfoFieldsV1& record,std::uint8_t online,bool& out,std::string& e){
 auto actual=borrow(record);
 if(!actual||actual->receiver!=&record){e="Required SAME actual PlayerInfo/CNet active receiver";return false;}
 if(!actual->selected_player_info_v67){e="Required actual derived PlayerInfo selected virtual5c producer";return false;}
 // PlayerInfo963930+8+5c selects36d48c, which calls GetOnline then
 // returns literal1 at36d4e4 on zero. Bare CNet80f124 has no such branch.
 if(actual->selected_player_info_v67&&!online){out=true;e.clear();return true;}
 if(!actual->query_fields_produced_v67){e="Required actual CNet178/1a0/1c8/1f0 query field producers";return false;}
 out=actual->local178>=0&&actual->owner1a0>=0&&actual->room1c8>=0&&actual->state1f0==3;
 e.clear();return true;
}
bool PlayerNetworkLocalOwnerV4::source_reset_player_class_v68(PlayerInfoFieldsV1& record,std::string& e){
 auto actual=borrow(record);if(!actual||!actual->selected_player_info_v67||actual->character_class_produced_v68){e="Required fresh derived PlayerInfo Reset class380 producer";return false;}
 // Source PlayerInfoC1 374420 establishes value0; its final Reset373c64
 // calls SetCharacterClass370ef0(-1) on NetStructInt360. This reconstructs
 // that scalar only; dirty packet delivery remains a separate endpoint.
  actual->character_class380_v68=-1;actual->character_class_produced_v68=true;
 actual->character_name2d0_v70.clear();actual->character_level330_v70=-1;actual->character_death_timer3a8_v70=-1;
 actual->character_visible4e5_v70=0;actual->character_loading525_v70=0;
 actual->profile680_v70.reset();actual->managed_fields_produced_v70=true;
 // These metadata stores belong to the derived constructor, before its
 // Reset. Reconstruct no packet list or other unsupported member metadata.
 actual->cutscene_change4e8_v99={};actual->death_change388_v99={};
 actual->loading_member505_v70=0;actual->cutscene_members_produced_v99=true;
 // Same derived C1/Reset backing, not another Net/player owner. Actual
 // NetStructByteArray36 C1 isNULL/0; Reset373c9c fills36 bytes ff.
 actual->remove_scalar4c0_v114=0;actual->character_xp448_v114=0;actual->character_gold498_v114=0;
 actual->remove_change4a0_v114={};actual->class_change360_v114={};actual->name_change2b0_v114={};
 actual->level_change310_v114={};actual->buffer_change3b0_v114={};actual->xp_change428_v114={};
 actual->gold_change478_v114={};actual->ingame_change4c8_v114={};
 try{actual->character_buffer3d0_v114=std::unique_ptr<std::uint8_t[]>(new std::uint8_t[36]);}
 catch(const std::exception& x){e=x.what();return false;}
 actual->character_buffer_size3d4_v114=36;std::memset(actual->character_buffer3d0_v114.get(),0xff,36);
 actual->remove_members_produced_v114=true;
 e.clear();return true;
}
bool PlayerNetworkLocalOwnerV4::source_selected_profile_class_v68(PlayerInfoFieldsV1& record,std::int32_t selected,std::string& e){
 auto actual=borrow(record);if(!actual||!actual->character_class_produced_v68||record.character660||selected<0){e="Required idle SAME produced PlayerInfo class380 and validated selected PCLS";return false;}
 if(actual->character_class380_v68!=-1&&actual->character_class380_v68!=selected){e="Selected profile conflicts with existing PlayerInfo class380";return false;}
 // Explicit native frontend transport of the original PCLS parser result;
 // no preview class/default and no claim of NetStruct network serialization.
 actual->character_class380_v68=selected;e.clear();return true;
}
bool PlayerNetworkLocalOwnerV4::source_set_in_cutscene_v99(PlayerInfoFieldsV1& record,bool value,std::string& e){
 auto actual=borrow(record);
 if(!actual||actual->receiver!=&record||!actual->selected_player_info_v67||!actual->cutscene_members_produced_v99){
  e="Required SAME derived PlayerInfo NetStruct4e8 constructor before SetInCutscene";return false;
 }
 // PlayerInfo459900 selects NetStructBool36da20; equal values are a true
 // source early return, without dirty/counter writes.
 const auto incoming=static_cast<std::uint8_t>(value);
 if(actual->loading_member505_v70!=incoming){actual->loading_member505_v70=incoming;member_changed_v99(actual->cutscene_change4e8_v99);}
 e.clear();return true;
}
bool PlayerNetworkLocalOwnerV4::source_remove_character_local_tail_v114(PlayerInfoFieldsV1& record,std::string& e){
 auto actual=borrow(record);
 if(!actual||actual->receiver!=&record||!actual->parent_lease||!actual->selected_player_info_v67||
    !actual->managed_fields_produced_v70||!actual->remove_members_produced_v114){
  e="Required SAME produced PlayerInfo local RemoveCharacter members";return false;
 }
 // Source371fcc calls actual4a0.virtual1c with integer0 (value4c0).
 // Destination SetValue36d9f0 compares before source SetChanged814f84.
 if(actual->remove_scalar4c0_v114){actual->remove_scalar4c0_v114=0;member_changed_v99(actual->remove_change4a0_v114);}
 bool local{};if(!is_local(record,local,e))return false;
 if(borrow(record)!=actual){e="PlayerInfo identity replaced after actual selected virtual50";return false;}
 if(!local){e.clear();return true;}
 if(actual->character_class380_v68!=-1){actual->character_class380_v68=-1;member_changed_v99(actual->class_change360_v114);}
 if(!actual->character_name2d0_v70.empty()){actual->character_name2d0_v70.clear();member_changed_v99(actual->name_change2b0_v114);}
 if(actual->character_level330_v70!=-1){actual->character_level330_v70=-1;member_changed_v99(actual->level_change310_v114);}
 // NetStructByteArray36.SetBuffer(NULL,0): temporaryNULL/0 is equal to
 // destination size0. Positive length differs, so actual ByteArray frees old
 // storage, stores size0, then performs original new[0], and SetChanged.
 if(actual->character_buffer_size3d4_v114||actual->buffer_empty_pending_v114){
  if(!actual->buffer_empty_pending_v114){actual->character_buffer3d0_v114.reset();actual->character_buffer_size3d4_v114=0;actual->buffer_empty_pending_v114=true;}
  try{actual->character_buffer3d0_v114=std::unique_ptr<std::uint8_t[]>(new std::uint8_t[0]);}
  catch(const std::exception& x){e=x.what();return false;}
  actual->buffer_empty_pending_v114=false;member_changed_v99(actual->buffer_change3b0_v114);
 }
 if(actual->character_xp448_v114){actual->character_xp448_v114=0;member_changed_v99(actual->xp_change428_v114);}
 if(actual->character_gold498_v114){actual->character_gold498_v114=0;member_changed_v99(actual->gold_change478_v114);}
 // Existing historical visible4e5 field IS actual SetIngame's bool value:
 // member4c8 +1d =4e5. Reuse SAME cell rather than shadowing it.
 if(actual->character_visible4e5_v70){actual->character_visible4e5_v70=0;member_changed_v99(actual->ingame_change4c8_v114);}
 e.clear();return true;
}
bool PlayerNetworkLocalOwnerV4::source_reset_dead_local_v99(PlayerInfoFieldsV1& record,std::string& e){
 auto actual=borrow(record);
 if(!actual||actual->receiver!=&record||!actual->selected_player_info_v67||!actual->cutscene_members_produced_v99){
  e="Required SAME derived PlayerInfo NetStruct388 constructor before death reset";return false;
 }
 // EnterCutscene45a5b4..634 assigns -1 through NetStructInt.SetValue36d9f0
 // on receiver388; its value20 is the retained death_timer3a8 cell.
 if(actual->character_death_timer3a8_v70!=-1){actual->character_death_timer3a8_v70=-1;member_changed_v99(actual->death_change388_v99);}
 e.clear();return true;
}
}


