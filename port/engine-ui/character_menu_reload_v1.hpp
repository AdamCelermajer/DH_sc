#pragma once
#include <cstdint>
#include <cstddef>
namespace dh2::ui {
enum MenuReloadServiceV1:std::uint32_t {
 reload_remove_buffs_v1=0,reload_saved_skills_v1,reload_skill_instances_v1,
 reload_update_skills_v1,reload_recalculate_v1,reload_check_items_v1,
 reload_saved_level_v1,reload_saved_class_v1,reload_menu_fx_v1,reload_spec_prompt_v1
};
struct MenuReloadRequest32V1 {
 std::uint32_t service,argument;std::uintptr_t subject;
 const char* path;const char* callback;
};
struct MenuReloadResponse16V1 {std::uintptr_t identity;std::int32_t value;std::uint32_t reserved;};
struct MenuReloadServices16V1 {
 void* context;
 // Return0 only after genuine synchronous delivery. Component services borrow
 // this same Character's actual properties, Save and skill owner. Menu lookup
 // returns the actual current MultiMenuManager RenderFX, with no cached clone.
 int(*invoke)(void*,const MenuReloadRequest32V1*,MenuReloadResponse16V1*);
};
struct MenuReloadResult16V1 {std::uint32_t phase,calls,specialization,reserved;};
static_assert(sizeof(MenuReloadRequest32V1)==32&&sizeof(MenuReloadResponse16V1)==16&&sizeof(MenuReloadServices16V1)==16&&sizeof(MenuReloadResult16V1)==16);
}
// Complete Character::ReloadSkills3a9db4 coordinator. NativeReloadSkills's AS
// conversion/player selection is separate. Required failures preserve source
// prefixes; no missing skill/Save/UI service becomes a successful no-op.
// 1 complete,-1 malformed atomic,-2 required delivery failed.
extern "C" int dh2_character_menu_reload_v1(dh2::ui::MenuReloadResult16V1*,
 std::uintptr_t character,const dh2::ui::MenuReloadServices16V1*);
