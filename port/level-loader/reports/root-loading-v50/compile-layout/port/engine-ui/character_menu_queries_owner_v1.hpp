#pragma once
#include "character_menu_actions_owner_v1.hpp"
#include "hud_initialization_v1.hpp"
#include "hud_text_v1.hpp"
#include "character_menu_font_palette_v1.hpp"
#include "../game-data/item_presentation_v5.hpp"
#include <deque>
namespace dh2::ui {
// Semantic original AS kinds. Object identities are supplied by the actual AS
// host and never interpreted as native pointers by this owner.
struct CharacterMenuValueV1 {
 std::uint32_t kind{};double number{};bool boolean{};std::string text;std::uintptr_t object{};
 static CharacterMenuValueV1 numeric(double);
 static CharacterMenuValueV1 flag(bool);
 static CharacterMenuValueV1 string(std::string);
 static CharacterMenuValueV1 reference(std::uintptr_t);
};
struct CharacterMenuCallV1 {
 std::vector<CharacterMenuValueV1> arguments;
 CharacterMenuValueV1 result;
 // Writes mutate the original supplied AS object. Array appends retain prior
 // entries. The host must validate handles and retain them across reentry.
 std::function<bool(std::uintptr_t,const char*,const CharacterMenuValueV1&,std::string&)> member;
 std::function<bool(std::uintptr_t,const CharacterMenuValueV1&,std::string&)> append;
 std::function<bool(std::uintptr_t,std::string&)> array;
 // Source list callbacks allocate a genuine AS object for each item. The host
 // pins this handle through the invocation and any member-write reentry.
 std::function<bool(std::uintptr_t&,std::string&)> create_object;
 std::function<bool(const CharacterMenuValueV1&,double&,std::string&)> number;
  std::function<bool(const CharacterMenuValueV1&,bool&,std::string&)> boolean;
  // Menu navigation converts names only at the original reached operation.
  // The actual AS host must deliver object/property conversion synchronously.
  std::function<bool(const CharacterMenuValueV1&,std::string&,std::string&)> text;
  // Original navigation uses to_xstring: object/null pointer representation,
  // ordinary string conversion for other tags. Keep it distinct from text.
  std::function<bool(const CharacterMenuValueV1&,std::string&,std::string&)> debug_text;
 // Only valid during this protected native call. Source RenderFX Invoke uses
 // the current movie environment, even during initial load before publication.
 std::function<bool(const char*,const char*,bool,std::string&)> invoke_boolean;
};
class CharacterMenuItemActionsV1;
class CharacterMenuSaveActionsV1;
class CharacterMenuFaeryActionsV1;
struct CharacterMenuQueriesGraphV1 {
 std::shared_ptr<void> owner;
 CharacterMenuActionsOwnerV1* actions{};
 CharacterMenuItemActionsV1* item_actions{};
 CharacterMenuSaveActionsV1* save_actions{};
 CharacterMenuFaeryActionsV1* faery_actions{};
 const data::CharacterTable* characters{};
 HudTextV1* text{};HudTextEnvironmentV1 text_environment;
 // Original global s_temp used by THIS V3 Session's real GetInfo script.
 // This is an explicit shared producer binding, not a new query scratch sheet.
 std::shared_ptr<data::PropertySheet> temporary;
 std::function<bool(const CharacterMenuSkillAuthorityV1&,const std::shared_ptr<data::PropertySheet>&,std::string&)> temporary_binding;
 // Fresh source NativeGetPlayerChar(index,remote), including genuine null.
 std::function<bool(std::int32_t,bool,std::uintptr_t&,std::string&)> player;
 // Source CanIncSkill complete services are separate from IncSkill mutations.
 std::function<bool(std::uintptr_t,std::uint32_t,bool&,std::string&)> can_increment;
 // Source faery getters' global difficulty lookup must remain fresh.
 std::function<bool(std::int32_t&,std::string&)> difficulty;
 std::function<bool(std::uintptr_t,std::uint32_t,std::int32_t&,std::string&)> faery_offset;
 // Actual ItemPresentationOwnerV5 binding. Null is valid only if no powers.
 std::function<bool(const data::ItemInstanceV1&,const std::vector<data::ItemPowerInstanceV5>*&,std::string&)> powers;
 std::function<bool(const data::ItemInstanceV1&,bool&,std::string&)> item_equippable;
 const CharacterMenuFontPaletteV1* font_palette{};
};
// Exact ItemInstance::IsEquippableBy3fa330 offline/remote query. World facts
// are fresh source projections; class IDs resolve genuine authored row names.
bool character_menu_item_equippable_v1(const data::ItemRecord164&,
 const data::PropertySheet&,std::int32_t saved_class,const data::CharacterTable&,
 bool online,bool remote_character,bool&,std::string&);
class CharacterMenuQueriesOwnerV1 {
 CharacterMenuQueriesGraphV1 graph_;
 bool player(std::int32_t,bool,bool,std::uintptr_t&,std::string&)const;
 bool write(CharacterMenuCallV1&,std::uintptr_t,const char*,CharacterMenuValueV1,std::string&)const;
 bool stats(CharacterMenuCallV1&,std::uintptr_t,std::uintptr_t,std::string&);
 bool item_details(CharacterMenuCallV1&,std::uintptr_t,std::uint32_t,std::string&);
 bool item_name(const data::ItemInstanceV1&,std::string&,std::string&)const;
 bool auto_slot(std::uint32_t,std::string&);
 struct SkillFrame;
public:
 explicit CharacterMenuQueriesOwnerV1(CharacterMenuQueriesGraphV1);
 // Returns false only for malformed host projections/required provider misses.
 // Source guard/no-player/no-item paths are successful no-ops, preserving the
 // preexisting result and reached member writes. Call arguments retain order.
 bool dispatch(const char* callback,CharacterMenuCallV1&,std::string&);
};
}
