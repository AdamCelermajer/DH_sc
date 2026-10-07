#pragma once
#include "game_option_table_v1.hpp"
#include "hud_startup_callbacks.hpp"
#include "localization.hpp"
#include <array>
#include <map>
#include <mutex>
namespace dh2::ui {
struct SettingsParserSpan24V1 {const std::uint8_t* data;std::uint32_t size,cursor,recognized,reserved;};
struct SettingsLookup16V1 {void* context;std::int32_t* (*lookup)(void*,const char*);};
static_assert(sizeof(SettingsParserSpan24V1)==24&&sizeof(SettingsLookup16V1)==16);
class OwnedHudSettingsV1;
struct SettingsFileServicesV1 {
 void* context{};
 // Exact logical dh2_settings.savegame. Found requires a nonzero live lease;
 // close follows the full stream copy, before source parsing/language delivery.
 bool (*open_read)(void*,const char*,bool& found,std::vector<std::uint8_t>&,std::uintptr_t& lease,std::string&){};
 bool (*close_read)(void*,std::uintptr_t,std::string&){};
};
struct SettingsLanguageServicesV1 {
 void* context{};
 // Genuine source object-manager traversal/name invalidation must be delivered.
 // It runs after Language option write, before TextManager switch_pack(lang,1).
 bool (*refresh_scene)(void*,OwnedHudSettingsV1&,std::int32_t language,std::string&){};
 // Required only by source loadSettings(true) non-Korean/non-Japanese branch.
 bool (*platform_language)(void*,std::uint32_t& source_enum,std::string&){};
 Localization* text{};
 // Optional typed successor for the same retained HudText/TextManager owner.
 // Existing Localization API and source ordering remain unchanged when absent.
 void* text_context{};
 bool(*switch_text_pack_v4)(void*,std::int32_t,bool,std::string&){};
};
struct SettingsDeviceFactsV1 {
 std::uint32_t sharp_devices{},htc_devices{},no_igp{},in_multiplayer_mode{},capabilities{},korean_build{},japanese_build{};
};
struct SettingsLoadReceiptV1 {
 bool found{},source_loaded{},source_new_settings{};std::size_t bytes{},consumed{},recognized_records{};
};
// Original private settings owner, not a campaign/character profile save owner.
// Holds actual GameOption descriptor backing and a private option map. File and
// language services are borrowed for each synchronous load; no fixture success.
class OwnedHudSettingsV1 {
 mutable std::recursive_mutex settings_mutex_v102_;
 bool settings_savegame4_v102_{};
 struct Value {std::size_t descriptor{};std::int32_t current{};};
 GameOptionTableV1::Borrow table_;std::map<std::string,Value> options_;
 std::array<std::uint8_t,14> tutorials_{};std::vector<std::uint8_t> file_;
 std::int32_t language_hint_{-1};bool loaded_{},new_settings_{},orientation_{};
 // SAME SavegameManager+0c. C1 46cd84 writes0; NativeSetCurrentDifficulty
 // 43cd9c writes the actual converted signed integer without clamping.
 std::int32_t current_difficulty0c_v67_{};
public:
 //Actual pre-PyData SavegameManager C1: option descriptor array is not yet
 //loaded. loadSettings(true) uses only the empty source map/language fields.
 OwnedHudSettingsV1()=default;
 explicit OwnedHudSettingsV1(GameOptionTableV1::Borrow);
 bool admit_process_option_table_v105(GameOptionTableV1::Borrow,std::string&);
 std::int32_t current_difficulty_v67()const noexcept{return current_difficulty0c_v67_;}
 void source_set_current_difficulty_v67(std::int32_t value)noexcept{current_difficulty0c_v67_=value;}
 // Whole original _initSettings46e47c. Does not load/save files or refresh
 // language, and preserves loaded/new/orientation/file ownership.
 void initialize_defaults(bool language_only=false);
 OwnedHudSettingsV1(const OwnedHudSettingsV1&)=delete;OwnedHudSettingsV1& operator=(const OwnedHudSettingsV1&)=delete;
 bool load(bool language_only,const SettingsFileServicesV1&,const SettingsLanguageServicesV1&,const SettingsDeviceFactsV1&,SettingsLoadReceiptV1&,std::string&);
 bool has_option(const char*)const;
 std::int32_t option(const char*)const; // Source Savegame.getOption miss=-1.
 std::int32_t option_max(const char*)const; // type 2 subtracts one; miss=-1.
 std::int32_t option_string(const char*)const; // value_string + current; miss=-1.
 std::int32_t saved_option(const char*)const; // Application miss=0.
 bool set_option(const char*,std::int32_t); // Unknown key ignored, false=miss.
 std::vector<std::uint8_t> serialized()const; // Source __saveOptions + 14 tutorial bytes.
 std::int32_t language()const; // Source Application.GetDeviceLanguage=-1.
 bool set_language(std::int32_t,const SettingsLanguageServicesV1&,std::string&);
 const GameOptionRow32V1* descriptor(const char*)const;
 const std::array<std::uint8_t,14>& tutorials()const{return tutorials_;}
 std::uint8_t* source_tutorial_cell_v116(std::uint32_t offset)noexcept{return offset>=0x29&&offset<0x29+tutorials_.size()?&tutorials_[offset-0x29]:nullptr;}
 //Observed original tutorial-byte store only; no defaults/load replay.
 bool source_store_tutorial_v88(std::uint32_t offset,std::uint8_t value,std::string& e){
  std::lock_guard<std::recursive_mutex> lock(settings_mutex_v102_);
  if(offset<0x29||offset>=0x29+tutorials_.size()){e="Tutorial source byte outside actual settings receiver";return false;}
  tutorials_[offset-0x29]=value;e.clear();return true;
 }
 const std::vector<std::uint8_t>& file_bytes()const{return file_;}
 bool loaded()const{return loaded_;}bool new_settings()const{return new_settings_;}
 bool orientation()const{return orientation_;}std::int32_t language_hint()const{return language_hint_;}
 std::size_t option_count()const{return options_.size();}
 //saveSettings46cb34 source guards: real settings Savegame+4 and byte37.
 bool source_save_settings_gate_v102()const{
  std::lock_guard<std::recursive_mutex> lock(settings_mutex_v102_);
  return settings_savegame4_v102_&&!new_settings_;
 }
};
// Persistent existing-wrapper adapter. Application/savegame/sound/result must
// match the caller's live HudStartupState48 identities. Only real settings
// operations are handled locally; required audio/result/device backends remain
// the caller's synchronous provider. No AS/global/VM binding is synthesized.
struct SettingsStartupBindingV1 {
 OwnedHudSettingsV1* owner{};const SettingsFileServicesV1* files{};
 const SettingsLanguageServicesV1* language{};const SettingsDeviceFactsV1* device{};
 HudStartupServices16 downstream{};SettingsLoadReceiptV1 last_load{};std::string error;
 std::uintptr_t application_identity{},savegame_identity{};
};
int settings_startup_v1_service(void*,HudStartupState48*,const HudStartupRequest40*,HudStartupResponse16*);
}
// Source __loadOptions count/key/value loop. 0 delivered (including >=128-byte
// key source stop),-1 malformed entry,-2 truncated input. Prior reads/writes are
// retained on later truncation. Lookup returns actual mutable option value or
// null for a genuine miss; it must preserve parser/receiver lifetime.
extern "C" int dh2_settings_v1_read_options(dh2::ui::SettingsParserSpan24V1*,const dh2::ui::SettingsLookup16V1*) noexcept;
