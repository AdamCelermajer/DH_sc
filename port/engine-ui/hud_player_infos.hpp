#pragma once
#include <cstdint>
namespace dh2::ui {
struct HudInfosActor32 {const std::int32_t* resolved;std::uint32_t count;std::uint8_t active,removed;std::uint16_t reserved0;std::uintptr_t identity;std::uint64_t reserved1;};
struct HudInfosInput24 {std::uintptr_t output_object;std::int32_t player_index;std::uint32_t remote,argument_count,reserved;};
enum class HudInfosOperation:std::uint32_t {player=1,skill_slot,skill_usable,skill_level,skill_info,spell_info,property_int,low_health_constant,spell_usable,potions,saved_dpad,write_member,divide_zero};
enum class HudInfosMember:std::uint32_t {player_active=0,level,hp_pct,hp_lowpct,mp_pct,xp_pct,spell_pct,spell_mp,skill1_pct,skill1_mp,skill2_pct,skill2_mp,skill3_pct,skill3_mp,potions,available_points,touch_to_move};
struct HudInfosRequest32 {HudInfosOperation operation;std::uint32_t index;std::int32_t value;std::uint32_t type;std::uintptr_t actor,output_object;};
struct HudInfosResponse16 {std::uintptr_t identity;std::int32_t value;float fraction;};
struct HudInfosServices16 {void* context;int(*invoke)(void*,const HudInfosRequest32*,HudInfosResponse16*);};
const char* hud_infos_member_name(HudInfosMember);
// Source arguments already coerced by the protected AS adapter. Invalid nargs
// does nothing; valid input captures output object and returned actor. Providers
// must retain coherent actor/property storage through synchronous callbacks.
// Return0 delivered/no-op,-1 malformed,-2 required failure; prefix retained.
}
extern "C" int dh2_ui_hud_player_infos_v1(const dh2::ui::HudInfosInput24*,const dh2::ui::HudInfosServices16*);
static_assert(sizeof(dh2::ui::HudInfosActor32)==32);
static_assert(sizeof(dh2::ui::HudInfosInput24)==24);
static_assert(sizeof(dh2::ui::HudInfosRequest32)==32);
static_assert(sizeof(dh2::ui::HudInfosResponse16)==16);
