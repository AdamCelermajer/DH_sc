#pragma once
#include <array>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <string>
#include <vector>

namespace dh2::ui {
struct LocalizationBytes {const std::uint8_t* data{};std::size_t size{};};
struct LocalizationServices {
 void* context{};
 // Source getSheetFilename produces text/<filename>. The explicit resource
 // provider resolves that logical URI, retaining a nonzero lease until close.
 bool (*open)(void*,const char*,bool& found,std::vector<std::uint8_t>&,std::uintptr_t& lease,std::string&){};
 bool (*close)(void*,std::uintptr_t,std::string&){};
 // Performs actual DebugSwitches.load then GetSwitch; source ignores the value.
 bool (*debug)(void*,const char* key,std::string&){};
 bool (*constant)(void*,const char* group,const char* key,std::uint32_t&,std::string&){};
 // Each call dynamically queries GetLocalPlayer(0,true)->character. Null is a
 // genuine no-character branch. Source calls it twice when the first is nonnull.
 // Native adapters retain their existing campaign borrower in these calls;
 // the resource/debug callbacks keep their original context unchanged.
 std::function<bool(void*,std::uintptr_t&,std::string&)> player_character{};
 std::function<bool(void*,std::uintptr_t,std::string&,std::string&)> player_name{};
 // Optional front-menu Application substitutions. The title comes from the
 // real MENU_GAME_TITLE ID; its language and the version/operator branch
 // remain explicit Application inputs.
 bool (*application_language)(void*,std::int32_t&,std::string&){};
 bool (*application_version)(void*,std::string&,std::string&){};
};
struct LocalizationResult {std::string text;bool found{};bool sets_menu_string_flag{};};
struct LocalizationArgumentV1;
std::string localization_utf_text_v1(const std::string&,bool add_space);
// Exact bounded source transforms. color mode returns source's changed flag;
// callers retain raw bytes if false. Plain mode rejects varargs directives whose
// required arguments/services are outside this HUD string-only domain.
bool localization_colors(const std::string&,bool add_space,const LocalizationServices&,std::string&,bool& changed,std::string& error);
bool localization_plain(const std::string&,bool add_space,std::string&,std::string& error);
bool localization_player(const std::string&,const std::string& player,bool add_space,std::string&,std::string& error);
class Localization {
 public:
 // Original common_text PyData array, names and schema, in serialized order.
 // Malformed input rejects atomically. Buffers are copied; no borrowed backing.
 bool load(LocalizationBytes records,LocalizationBytes names,LocalizationBytes schema,std::string&);
 bool switch_pack(std::int32_t pack,bool unload_old,std::string&); // -1..8
 bool preload(std::uint32_t pack,std::uint32_t sheet,bool force,const LocalizationServices&,std::string&);
 bool native_string(const std::string& symbol,const LocalizationServices&,LocalizationResult&,std::string&);
 bool parsed_string(const std::string& symbol,const std::vector<LocalizationArgumentV1>&,
     const LocalizationServices&,std::string&,std::string&);
 bool string_id(std::uint32_t,const LocalizationServices&,std::string&,std::string&);
 std::int32_t pack()const{return pack_;}
 const std::vector<std::string>& pack_names()const{return pack_names_;}
 const std::string& sheet_name(std::uint32_t pack,std::uint32_t sheet)const;
 const std::string& sheet_filename(std::uint32_t pack,std::uint32_t sheet)const;
 std::size_t loaded_sheets()const;
 private:
 struct Sheet {std::string filename,name;std::vector<std::string> strings;bool loaded{};};
 std::array<std::array<Sheet,37>,9> sheets_{};std::vector<std::string> pack_names_;
 std::int32_t pack_{-1};bool ready_{},busy_{};
 bool preload_impl(std::uint32_t,std::uint32_t,bool,const LocalizationServices&,std::string&);
 bool index(std::uint32_t sheet,std::uint32_t index,std::int32_t pack,const LocalizationServices&,std::string&,std::string&);
 bool id(std::uint32_t,const LocalizationServices&,std::string&,std::string&);
 bool defaults(const LocalizationServices&,std::string&);
};
} // namespace dh2::ui
