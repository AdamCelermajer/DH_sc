#pragma once
#include <cstdint>
namespace dh2::ui {
// A borrowed complete SkillTable record. Source pointer words are represented
// by genuine owned vectors/text, never by serialized ARM32 addresses.
struct HudInitSkill96 {
 const std::int32_t* display_properties;
 std::uint32_t display_count,reserved;
 std::int32_t required_level,current_text,description_text,name_text,next_text;
 std::uint8_t faery_dependent,assignable;
 std::uint16_t reserved1;
 const char* icon;
 std::uintptr_t identity;
 std::uint64_t reserved2[5];
};
enum class HudInitOperation:std::uint32_t {
 argument_type=1,argument_number,argument_boolean,cast_object,cast_array,
 player,skill_slot,skill_id,character_skill,character_level,skill_level,
 unlocked_difficulty,can_increment,current_faery,character_faery_offset,
 faery_level,constant,string_symbol,arguments_create,arguments_append,
 skill_info,property,parse_text,write_member,array_push,result_boolean,
 result_number,result_object,argument_is_number,argument_string,
 option_current,option_maximum,option_string_id,language_override,
 publish_language,platform_music_support
};
enum class HudInitMember:std::uint32_t {
 skill_name,skill_description,skill_current,skill_next,skill_assignable,
 skill_icon,skill_level,skill_slot,skill_unlocked,faery_id,faery_upgraded,
 option_maximum,option_current,option_string
};
struct HudInitInput16 {std::uintptr_t call;std::uint32_t argument_count,available_arguments;};
struct HudInitRequest64 {
 HudInitOperation operation;std::uint32_t index;
 std::int32_t value,other;std::uint32_t type,reserved;
 std::uintptr_t subject,object;
 const char* text;const char* name;
 double number;
};
struct HudInitResponse32 {
 std::uintptr_t identity;const char* text;double number;
 std::int32_t value;float fraction;
};
struct HudInitServices16 {void* context;int(*invoke)(void*,const HudInitRequest64*,HudInitResponse32*);};
const char* hud_init_member_name(HudInitMember) noexcept;
// entry0=NativeSkillGetEquipedSkillsIDs4425a0,1=NativeGetSkillDetails445a10,
// 2=NativeHUDGetActiveFaery44a820,3=NativeGetOptionParameters44a298,
// 4=NativeUseIpodPlayer43a974. Source guards and ordered effects execute.
// Providers return exactly1 for delivery; getter misses are separate values.
// Text/vector/record responses must be pinned through the entire invocation,
// including synchronous reentry. arguments_create returns a distinct owned
// per-invocation VarArgs handle; parse_text must execute required formatting.
// Returns0 delivered/no-op,-1 malformed,-2 required failure. Failed prefixes
// remain visible. Details unconditionally reads arguments0..2 in source; this
// native ABI rejects a projection with fewer than3 available argument slots.
}
extern "C" int dh2_ui_hud_initialization_v1(const dh2::ui::HudInitInput16*,std::uint32_t entry,const dh2::ui::HudInitServices16*);
static_assert(sizeof(dh2::ui::HudInitSkill96)==96);
static_assert(sizeof(dh2::ui::HudInitInput16)==16);
static_assert(sizeof(dh2::ui::HudInitRequest64)==64);
static_assert(sizeof(dh2::ui::HudInitResponse32)==32);
