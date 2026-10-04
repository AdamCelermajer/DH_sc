#pragma once
#include "game_option_table_v1.hpp"
#include "hud_startup_callbacks.hpp"
#include "localization.hpp"
#include <array>
#include <map>
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
 struct Value {std::size_t descriptor{};std::int32_t current{};};
 GameOptionTableV1::Borrow table_;std::map<std::string,Value> options_;
 std::array<std::uint8_t,14> tutorials_{};std::vector<std::uint8_t> file_;
 std::int32_t language_hint_{-1};bool loaded_{},new_settings_{},orientation_{};
public:
 explicit OwnedHudSettingsV1(GameOptionTableV1::Borrow);
 OwnedHudSettingsV1(const OwnedHudSettingsV1&)=delete;OwnedHudSettingsV1& operator=(const OwnedHudSettingsV1&)=delete;
 bool load(bool language_only,const SettingsFileServicesV1&,const SettingsLanguageServicesV1&,const SettingsDeviceFactsV1&,SettingsLoadReceiptV1&,std::string&);
 bool has_option(const char*)const;
 std::int32_t option(const char*)const; // Source Savegame.getOption miss=-1.
 std::int32_t saved_option(const char*)const; // Application miss=0.
 bool set_option(const char*,std::int32_t); // Unknown key ignored, false=miss.
 std::int32_t language()const; // Source Application.GetDeviceLanguage=-1.
 bool set_language(std::int32_t,const SettingsLanguageServicesV1&,std::string&);
 const GameOptionRow32V1* descriptor(const char*)const;
 const std::array<std::uint8_t,14>& tutorials()const{return tutorials_;}
 const std::vector<std::uint8_t>& file_bytes()const{return file_;}
 bool loaded()const{return loaded_;}bool new_settings()const{return new_settings_;}
 bool orientation()const{return orientation_;}std::int32_t language_hint()const{return language_hint_;}
 std::size_t option_count()const{return options_.size();}
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
