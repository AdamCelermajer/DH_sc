#pragma once
#include "menu_manager_update_v58.hpp"
#include "authored_menu_application_fields_v3.hpp"
#include "authored_gameplay_hud_v1.hpp"
#include "authored_character_menu_session_v1.hpp"
#include <array>
#include <map>
namespace dh2::ui {
// Exact process-global MenuBase FSCommand registration cells. IDs are source
// endpoint identities, never cast into native pointers/executed as ARM code.
// RegisterFSCommand426980 inserts only an absent name; C2 ignores duplicates.
class MenuBaseFSRegistryV62 {
 std::map<std::string,std::uintptr_t> callbacks_;
 std::uint8_t initialized_source_global_{};
public:
 void source_menu_base_c2_registration();
 std::uintptr_t callback(const std::string&)const noexcept;
};
struct DebugCachedCharacterV62 {
 std::uint8_t byte0{};std::uint32_t word4{};
 std::string path8;
 std::uintptr_t render20{},root24{};
 AuthoredMenuWeakCharacterV59 weak28;
};
class MenuDebugHudOwnerV62;
struct MenuDebugHudServicesV62 {
 std::shared_ptr<void> actual_manager;
 std::function<bool(std::string&)> debug_load;
 std::function<bool(const char*,bool&,std::string&)> debug_switch;
 // Original RegisterMenu42ee94 scans actual RenderFX slots0..3. A found
 // root delivers actual RegisterState; genuine absence leaves render4 NULL.
 std::function<bool(MenuDebugHudOwnerV62&,std::string&)> register_menu;
 // Actual RefreshCache427ca0, using the SAME registered weak root and movie.
 std::function<bool(MenuDebugHudOwnerV62&,DebugCachedCharacterV62&,const char*,std::string&)> refresh_cache;
 // Original LoadFromFile429cd4 reached only with positive render4. The
 // actual FileSystem/g_fileNameDebugParams producer is mandatory here.
 std::function<bool(std::array<std::uint8_t,22>&,bool& written,std::string&)> load_flags;
};
class MenuDebugHudOwnerV62 {
 AuthoredMenuFieldsV1 fields_;
 AuthoredCharacterStateV1 state_;
 MenuStackCharacterV1 projection_{};
 struct EmptyTree {std::uint8_t color{};std::uintptr_t parent{};const void* left{this};const void* right{this};std::uint32_t count{};};
 [[maybe_unused]] EmptyTree tree_c8_;
 std::map<std::string,std::int32_t> counters_c8_v67_;
 std::array<DebugCachedCharacterV62,5> caches_;
 std::array<std::uint8_t,22> flags_{};bool flags_produced_{};
 bool attempted_{},complete_{};std::string failure_;
public:
 MenuDebugHudOwnerV62();
 bool construct(MenuBaseFSRegistryV62&,const MenuDebugHudServicesV62&,std::string&);
 bool source_initialize_v67(const MenuDebugHudServicesV62&,std::string&);
 bool update(std::string&)const; // selected virtual1c=429c18 BX LR
 bool prepare_level_frame_v67(std::string&);
 bool source_counter_store_v67(const std::string&,std::int32_t,std::string&);
 AuthoredMenuFieldsV1& fields()noexcept{return fields_;}
 AuthoredCharacterStateV1& state()noexcept{return state_;}
 MenuStackCharacterV1& projection()noexcept{return projection_;}
 bool flags_produced()const noexcept{return flags_produced_;}
 bool constructed()const noexcept{return complete_;}
 const std::string& failure()const noexcept{return failure_;}
};
// One process lifetime, independent of a World/GL/movie generation. GetInstance
// constructs DebugHUD only when first reached by a nonnull current Level.
class MenuManagerProcessV62 {
 std::uint8_t fill_leaderboard9a5c10_{}; // Source BSS0, not an online guess.
 MenuBaseFSRegistryV62 fs_commands_;
 std::unique_ptr<MenuDebugHudOwnerV62> debug_hud_;
public:
 const std::uint8_t* fill_leaderboard_byte()const noexcept{return &fill_leaderboard9a5c10_;}
 // Only a real recovered producer may write this source global.
 std::uint8_t& source_fill_leaderboard_cell()noexcept{return fill_leaderboard9a5c10_;}
 bool debug_hud(const MenuDebugHudServicesV62&,MenuDebugHudOwnerV62*&,std::string&);
 MenuDebugHudOwnerV62* existing_debug_hud()noexcept{return debug_hud_.get();}
 const MenuBaseFSRegistryV62& fs_commands()const noexcept{return fs_commands_;}
 MenuBaseFSRegistryV62& source_fs_commands()noexcept{return fs_commands_;}
};
struct MenuPrefixCharacterBorrowV62 {
 std::shared_ptr<void> receiver;
 std::uintptr_t character{};
 const std::int32_t* signed_ooi_type14a8{}; // existing native projection of byte
};
struct MenuPrefixHudBorrowV62 {
 std::shared_ptr<void> receiver;
 AuthoredGameplayHudV1* facade{};
 const std::int32_t* cache108{};
};
struct MenuManagerPrefixServicesV62 {
 std::shared_ptr<void> actual_manager;
 std::shared_ptr<MenuManagerProcessV62> process;
 MenuDebugHudServicesV62 debug_hud;
 std::function<bool(std::string&)> fill_leaderboard;
 std::function<bool(MenuLevelBorrowV58&,std::string&)> current_level;
 std::function<bool(std::uintptr_t&,std::string&)> local_player_character;
 std::function<bool(std::uintptr_t,MenuPrefixCharacterBorrowV62&,std::string&)> character;
 // Native cache borrow is separate from source GetHUDRoot. This preserves the
 // original cache-equal branch skipping GetHUDRoot and all AS callbacks.
 std::function<bool(MenuPrefixHudBorrowV62&,std::string&)> action_cache;
 std::function<bool(MenuPrefixHudBorrowV62&,std::string&)> hud_root;
 std::function<bool(bool,std::string&)> set_wire_frame;
 std::function<bool(std::string&)> debug_load;
 std::function<bool(const char*,bool&,std::string&)> debug_switch;
 std::function<bool(std::uint32_t&,std::string&)> application_dt8c;
 std::function<bool(AuthoredMenuApplicationFieldsV3*&,std::string&)> listener_fields;
};
class MenuManagerPrefixV62 {
 MenuManagerPrefixServicesV62 services_;bool busy_{};
public:
 explicit MenuManagerPrefixV62(MenuManagerPrefixServicesV62);
 bool update(bool& early_exit,std::int32_t& dt,std::string&);
};
}
