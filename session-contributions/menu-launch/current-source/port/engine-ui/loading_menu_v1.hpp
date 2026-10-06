#pragma once
#include <cstdint>
#include <cstddef>
#include <string>
#include <vector>
namespace dh2::ui {
struct LoadingHintRowV1 { std::uint32_t avatar_id{},string_id{}; };
struct LoadingHintRandomV1 { std::uint32_t seed{},debug_calls{}; };
struct LoadingMenuBytesV1 {const std::uint8_t* data{};std::size_t size{};};
class LoadingHintTableV1 {
public:
 bool load(LoadingMenuBytesV1 records,LoadingMenuBytesV1 names,LoadingMenuBytesV1 fields,std::string&);
 bool next(LoadingHintRandomV1&,std::uint32_t& string_id,std::string&)const;
 const std::vector<LoadingHintRowV1>& rows()const{return rows_;}
private:
 std::vector<LoadingHintRowV1> rows_;
};
// Source kernels consume actual owner state. The front must not manufacture
// a Level, OnlineGameState, or readiness value merely to display progress.
std::int32_t loading_menu_progress_v1(bool level_present,std::uint32_t level_progress,std::uint32_t online_state);
bool loading_menu_end_v1(bool level_present,std::uint32_t& level_state);
// Providers borrow the retained Application/Level/Online owners. Call only on
// their owner thread; identities must refer to the actual current Level.
// A successful current_level read with identity 0 means actual absence.
struct LoadingMenuStateServicesV1 {
 void* context{};
 bool (*current_level)(void*,std::uintptr_t&,std::string&){};
 bool (*level_progress)(void*,std::uintptr_t,std::uint32_t&,std::string&){};
 bool (*online_state)(void*,std::uint32_t&,std::string&){};
 bool (*level_state)(void*,std::uintptr_t,std::uint32_t&,std::string&){};
 // Must verify identity and expected state on the retained owner, then commit
 // the transition there. A detached/cached state word is not a valid provider.
 bool (*advance_level_state)(void*,std::uintptr_t,std::uint32_t expected,std::uint32_t next,std::string&){};
};
bool loading_menu_read_progress_v1(const LoadingMenuStateServicesV1&,std::int32_t&,std::string&);
bool loading_menu_finish_v1(const LoadingMenuStateServicesV1&,bool& advanced,std::string&);

// Borrow actual Online/PlayerManager/Matching owners on their owner thread.
// Unknown player field meanings retain source offsets rather than guessing.
struct LoadingMenuMultiplayerServicesV1 {
 void* context{};
 bool (*enabled)(void*,bool&,std::string&){}; // GetOnline()->byte5
 bool (*all_loading_done)(void*,bool&,std::string&){};
 bool (*all_ready_to_roll)(void*,bool&,std::string&){};
 bool (*online_state)(void*,std::uint32_t&,std::string&){};
 bool (*matching_is_host)(void*,bool&,std::string&){};
 bool (*local_player_hosting)(void*,bool&,std::string&){};
 bool (*hosting_player)(void*,std::uintptr_t&,std::string&){};
 bool (*player_field_4e5)(void*,std::uintptr_t,std::uint8_t&,std::string&){};
 bool (*manager_field_71a)(void*,std::uint8_t&,std::string&){};
 bool (*player_field_505)(void*,std::uintptr_t,std::uint8_t&,std::string&){};
};
bool loading_menu_multiplayer_completed_v1(const LoadingMenuMultiplayerServicesV1&,bool&,std::string&);
bool loading_menu_multiplayer_host_v1(const LoadingMenuMultiplayerServicesV1&,bool&,std::string&);
bool loading_menu_wait_for_host_v1(const LoadingMenuMultiplayerServicesV1&,bool&,std::string&);

// NativeBackToHud/NativeRefreshHudManager borrow actual game/HUD owners.
// Mutations must validate retained identities and affect those owners.
struct LoadingMenuHudServicesV1 {
 void* context{};
 bool (*current_level)(void*,std::uintptr_t&,std::string&){};
 bool (*set_level_field_198)(void*,std::uintptr_t,std::uint8_t,std::string&){};
 bool (*script_manager_field_30)(void*,std::uint8_t&,std::string&){};
 bool (*display_right_hud)(void*,std::string&){}; // actual HUD root DisplayRightHud()
 bool (*action_icon_id)(void*,std::uint32_t&,std::string&){}; // MenuManager+108
 bool (*fill_action_icon)(void*,std::int32_t,std::string&){}; // actual HUD root FillActionIcon(id)
 bool (*reset_finger_map)(void*,std::string&){};
 bool (*reset_update_cursor)(void*,std::string&){};
 bool (*touch_hud_controls)(void*,std::string&){};
 // Each call must query GetLocalPlayer(0,true)->character(+660) afresh.
 bool (*local_character)(void*,std::uintptr_t&,std::string&){};
 bool (*trophy_check_armor_set)(void*,std::uintptr_t,std::string&){};
 bool (*set_info_hud_field_4)(void*,std::uint8_t,std::string&){};
};
bool loading_menu_back_to_hud_v1(const LoadingMenuHudServicesV1&,std::string&);
bool loading_menu_refresh_hud_v1(const LoadingMenuHudServicesV1&,std::string&);
}


