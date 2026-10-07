#pragma once
#include "player_manager_loot_queries_v8.hpp"
#include "savegame_stream_v2.hpp"
#include <map>
#include <memory>
#include <vector>
namespace dh2::player {
// PlayerInfo::Reset373bdc scalar fields; one retained projection of the live
// record, never a Save/inventory copy. Embedded network properties and their
// constructor/reset are required separate borrowed producers.
struct PlayerInfoFieldsV1 {
 std::uintptr_t character660{};const std::int16_t* character_base_id13c8{};
 std::int32_t save_slot664{-1},controller668{-1};std::uint8_t local66c{1};
 std::int32_t internal670{-1},controller_local674{-1},friendly678{-1},local_remote67c{-1};
 std::int32_t unknown680{},unknown684{};
};
enum class PlayerManagerOperationV1 : std::uint32_t {
 online_enabled=0x7fd794,network_enabled=0x320e98,
 session_ready=0x800f8c,session_active=0x8100e0,
 construct_player_info=0x37418c,reset_player_info=0x373bdc,
 remove_character=0x371d80,character_initialization=0x372220,
 network_get_info=0x36dec4,network_add_player=0x378af4,
 network_remove_player=0x3726bc
};
struct PlayerManagerRequestV1 {
 PlayerManagerOperationV1 operation;std::int32_t id{},argument{},secondary{};bool flag{};
 PlayerInfoFieldsV1* player{};
 // Whole AddCharacter/RemoveCharacter write this SAME source field at their
 // recovered point, including failure prefixes after that store.
 std::int32_t* character_count6c4{};
};
struct PlayerManagerResponseV1 {std::int32_t value{};PlayerInfoFieldsV1* player{};};
struct PlayerManagerServicesV1 {
 void* context{};
 bool(*invoke)(void*,const PlayerManagerRequestV1&,PlayerManagerResponseV1&,std::string&){};
};
// Same PM C1 scalar storage374b98..374bd0. These are frame/body fields,
// distinct from PlayerInfo network members and Character initialization.
struct PlayerManagerSourceFrameFieldsV68 {
 std::uint8_t byte6c9{},byte6ca{},byte6cb{},byte6d0{};
 std::uintptr_t field6d4{},field6d8{};
 std::uint8_t byte710{},byte711{};
 std::uintptr_t field714{};
 std::uint8_t byte718{},byte719{},byte71a{},byte71b{};
};
// Source internal map and friendly/local vectors. Canonical Character is a
// borrow assigned by the original AddCharacter receiver; ID keys come from
// actual input/controller producers, never World handles or chosen IDs.
class PlayerManagerOwnerV1 {
 std::map<std::int32_t,std::unique_ptr<PlayerInfoFieldsV1>> players_;
 PlayerInfoFieldsV1 dummy_;std::vector<std::int32_t> friendly_,local_,remote_;
 std::int32_t character_count6c4_{};
 bool remove_characters_active_v114_{};std::int32_t remove_characters_index_v114_{};
 PlayerInfoFieldsV1* remove_characters_pending_v114_{};std::int32_t remove_characters_internal_v114_{};
 [[maybe_unused]] std::int32_t unknown6cc_{-1}; // Preserve recovered C1 field.
 PlayerManagerSourceFrameFieldsV68 frame_v68_;
 level::SavegameStreamV2 quest_sync6e0_v70_;
 bool running_{};PlayerManagerServicesV1 services_;
 bool call(PlayerManagerRequestV1,PlayerManagerResponseV1&,std::string&);
 bool network(bool&,std::string&);
 bool numbers(std::string&);
public:
 explicit PlayerManagerOwnerV1(PlayerManagerServicesV1 services):services_(services){}
 PlayerManagerOwnerV1(const PlayerManagerOwnerV1&)=delete;
 PlayerManagerOwnerV1& operator=(const PlayerManagerOwnerV1&)=delete;
 // Source dummy PlayerInfo constructor must complete before publishing.
 bool initialize(std::string&);
 bool add_player(std::int32_t internal,std::int32_t source_controller,
                 std::int32_t source_local_index,bool local,std::string&);
 bool add_character(std::int32_t internal,std::string&);
 bool remove_player(std::int32_t internal,std::string&);
 // RemoveAllPlayers3727c0 captures GetNumPlayers once, then removes current
 // friendly player0 each time and finally stores the same character count0.
 bool remove_all_players_v114(std::string&);
 // Original RemoveAllCharacters3721b8: live count/index walk, SAME existing
 // RemoveCharacter receiver, final source count store. No actor deletion clone.
 bool remove_all_characters_v114(std::string&);
 bool get_by_internal(std::int32_t,bool,PlayerInfoFieldsV1*&,std::string&);
 bool get_player(std::int32_t,bool,PlayerInfoFieldsV1*&,std::string&);
 bool get_local_player(std::int32_t,bool,PlayerInfoFieldsV1*&,std::string&);
 bool get_remote_player(std::int32_t,bool,PlayerInfoFieldsV1*&,std::string&);
 bool get_by_character(std::uintptr_t,bool,PlayerInfoFieldsV1*&,std::string&);
 bool num_players(std::int32_t&,std::string&);
 bool num_local_players(bool,std::int32_t&,std::string&);
 bool class_count(std::int32_t base,std::int32_t&,std::string&);
 const std::int32_t* character_count_field()const noexcept{return &character_count6c4_;}
 PlayerManagerSourceFrameFieldsV68* source_frame_fields_v68()noexcept{return initialized_?&frame_v68_:nullptr;}
 const PlayerManagerSourceFrameFieldsV68* source_frame_fields_v68()const noexcept{return initialized_?&frame_v68_:nullptr;}
 std::int32_t* source_loading6cc_v70()noexcept{return initialized_?&unknown6cc_:nullptr;}
 level::SavegameStreamV2* source_quest_sync6e0_v70()noexcept{return initialized_?&quest_sync6e0_v70_:nullptr;}
 // Native transport admission only; exposes existing constructor/reentry
 // guards without adding a source count, readiness byte or manager owner.
 bool source_initialized_v59()const noexcept{return initialized_;}
 bool source_running_v59()const noexcept{return running_;}
 // Original IsPlayerInLocalMap36d280 is only this existing-map lookup;
 // it neither queries online transport nor allocates/changes PlayerInfo.
 bool is_player_in_local_map_v59(std::int32_t id)const noexcept{return players_.find(id)!=players_.end();}
 LootPlayerManagerServicesV8 loot_services()noexcept;
private:
 bool initialized_{};
 bool mapped(std::int32_t,bool,unsigned,PlayerInfoFieldsV1*&,std::string&);
 static bool loot_player(void*,std::int32_t,bool,LootPlayerBorrowV8&,std::string&);
};
}
