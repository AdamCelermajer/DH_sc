#pragma once
#include <cstdint>
#include <cstddef>
namespace dh2::ui {
// Source direct fields, borrowed through every synchronous provider callback.
struct HudManagerActor {
 const std::int32_t* resolved; std::uint32_t resolved_count, reserved;
 HudManagerActor* target;
 const std::uintptr_t* skills; std::uint32_t skill_count, reserved1;
 const std::uintptr_t* spells; std::uint32_t spell_count, name_symbol;
 const char* debug_name; std::int32_t network_id; float position[3];
 std::uintptr_t identity;
};
struct HudManagerPlayer {
 HudManagerActor* actor; std::int32_t death_ms,cached_level;
 std::uint8_t ready,reserved[7]; const char* name; std::uintptr_t identity;
};
struct HudManagerState {
 HudManagerActor* cached_target;
 std::uint32_t initialized; std::int32_t slow_ms;
 std::uintptr_t render_fx;
};
enum class HudManagerEntry:std::uint32_t {update=0,initialize=1,one_time=2,fast=3,slow=4};
// Cache index is exactly (original manager offset - 0xc)/0x30, range0..28.
enum class HudManagerOperation:std::uint32_t {
 current_level=1,elapsed,saved_option,root_lookup,cache_initialize,cache_get,
 local_player,player_class,visible,goto_frame,text,property_int,skill_slot,
 skill_usable,spell_usable,cooldown,as_slot_id,online,is_dead,target_character,
 is_character,is_monster,string_symbol,is_boss,level,debug_load,debug_switch,
 hp_fraction,player_count,player_at,player_remote,format_multiplayer,
 project_position,inverse_pixel_x,inverse_pixel_y,position,root_character,
 allies_callback,goto_label,divide_zero,potions,sprite_type,play_state
};
struct HudManagerAllies {
 bool present; std::uint8_t reserved[3]; const char* name;
 std::int32_t level,hp_frame,index,death_seconds;
};
struct HudManagerRequest {
 HudManagerOperation operation; std::uint32_t index;
 std::int32_t value,other;
 std::uintptr_t subject,render_fx;
 const char* text; const void* payload;
 float xyz[3]; std::uint32_t reserved;
};
struct HudManagerResponse {
 std::uintptr_t identity; std::int32_t value; float fraction;
 std::int32_t xy[2]; const char* text;
};
struct HudManagerServices {
 void* context;
 int (*invoke)(void*,HudManagerState*,const HudManagerRequest*,HudManagerResponse*);
};
// Every reached operation requires delivery1; providers may mutate live state.
// Null cache results are accepted only where the original has a null guard.
// Original unchecked dereferences instead reject malformed projections.
// 0 complete/skipped, -1 malformed, -2 required delivery failed. Prefix retained.
const char* hud_manager_cache_path(std::uint32_t cache,std::int32_t style) noexcept;
}
extern "C" int dh2_ui_hud_manager_v1(dh2::ui::HudManagerState*,std::uint32_t entry,const dh2::ui::HudManagerServices*) noexcept;
