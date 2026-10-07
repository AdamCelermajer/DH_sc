#pragma once
#include "player_manager_loot_queries_v8.hpp"
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
// Source internal map and friendly/local vectors. Canonical Character is a
// borrow assigned by the original AddCharacter receiver; ID keys come from
// actual input/controller producers, never World handles or chosen IDs.
class PlayerManagerOwnerV1 {
 std::map<std::int32_t,std::unique_ptr<PlayerInfoFieldsV1>> players_;
 PlayerInfoFieldsV1 dummy_;std::vector<std::int32_t> friendly_,local_,remote_;
 std::int32_t character_count6c4_{},unknown6cc_{-1};
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
 bool get_by_internal(std::int32_t,bool,PlayerInfoFieldsV1*&,std::string&);
 bool get_player(std::int32_t,bool,PlayerInfoFieldsV1*&,std::string&);
 bool get_local_player(std::int32_t,bool,PlayerInfoFieldsV1*&,std::string&);
 bool get_remote_player(std::int32_t,bool,PlayerInfoFieldsV1*&,std::string&);
 bool get_by_character(std::uintptr_t,bool,PlayerInfoFieldsV1*&,std::string&);
 bool num_players(std::int32_t&,std::string&);
 bool num_local_players(bool,std::int32_t&,std::string&);
 bool class_count(std::int32_t base,std::int32_t&,std::string&);
 const std::int32_t* character_count_field()const noexcept{return &character_count6c4_;}
 LootPlayerManagerServicesV8 loot_services()noexcept;
private:
 bool initialized_{};
 bool mapped(std::int32_t,bool,unsigned,PlayerInfoFieldsV1*&,std::string&);
 static bool loot_player(void*,std::int32_t,bool,LootPlayerBorrowV8&,std::string&);
};
}
