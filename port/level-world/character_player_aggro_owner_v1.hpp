#pragma once
#include "character_design_services.hpp"
#include <vector>
#include <string>
namespace dh2::character {
// Sole AISPlayer supplemental fields initialized by the real SetScript
// constructor, shared with the selected AIS owner; no alternate CharAI/FSM.
struct PlayerAggroFieldsV1 {
 std::uint32_t field_b8{},field_bc{},field_c0{};
 std::vector<std::uint32_t> pending_c4;
 std::int32_t count_d0{},weight_d4{};
};
struct PlayerAggroLevelBorrowV1 {
 std::uintptr_t identity{};
 // Null LevelConfig is an original assertion branch, not a disabled scene.
 const std::uintptr_t* config{};const std::uint8_t* config_music_enabled{};
 const std::int32_t* music_id{};
};
struct PlayerAggroSoundBorrowV1 {
 std::uintptr_t identity{};std::uint8_t* ambient_31{};const std::uint8_t* level_music_32{};
};
enum PlayerAggroServiceV1:std::uint32_t {
 player_aggro_online_v1=1,player_aggro_local_player_v1,player_aggro_current_level_v1,
 player_aggro_other_weight_v1,player_aggro_sound_v1,player_aggro_threshold_v1,
 player_aggro_set_music_state_v1,player_aggro_play_music_v1,player_aggro_trace_v1,
 player_aggro_config_assert_v1
};
struct PlayerAggroRequestV1 {
 std::uint32_t service{};std::uintptr_t player{},other{};
 const char* name{};std::int32_t count{},weight{},music_id{},loop{},flag{},fade{};
};
struct PlayerAggroResponseV1 {
 std::uint32_t word{};std::int32_t integer{};
 PlayerAggroLevelBorrowV1 level{};PlayerAggroSoundBorrowV1 sound{};
};
struct PlayerAggroServicesV1 {void*context{};int(*invoke)(void*,const PlayerAggroRequestV1*,PlayerAggroResponseV1*){};};
class CharacterPlayerAggroOwnerV1 {
 std::uintptr_t selected_ais_,player_;PlayerAggroFieldsV1& fields_;
 DebugSwitches& debug_;const DebugFileServices24& files_;PlayerAggroServicesV1 services_;
 std::string error_;
 int ask(std::uint32_t,std::uintptr_t,PlayerAggroResponseV1&,const char* = nullptr,
  std::int32_t music=0,std::int32_t loop=0,std::int32_t flag=0,std::int32_t fade=0);
 int tracing(std::uintptr_t,bool before);
public:
 CharacterPlayerAggroOwnerV1(std::uintptr_t actual_selected_ais,std::uintptr_t actual_player,
  PlayerAggroFieldsV1& same_fields,DebugSwitches&,const DebugFileServices24&,PlayerAggroServicesV1);
 int on_aggro(std::uintptr_t actual_other);int on_deaggro(std::uintptr_t actual_other);
 std::uintptr_t selected_ais()const noexcept{return selected_ais_;}
 const std::string& error()const noexcept{return error_;}
};
}
