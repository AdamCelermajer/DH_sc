#pragma once
#include <cstddef>
#include <cstdint>
namespace dh2::ui {
struct MenuStackRenderV1;
// Borrowed character identity, with source weak-reference liveness projected by caller.
struct MenuStackCharacterV1 {std::uintptr_t identity;std::uint32_t live,visible,type_is_two,focus_enabled;};
struct MenuStackMenuV1 {
 std::uintptr_t identity; MenuStackRenderV1* render; const char* name;
 MenuStackCharacterV1* character; MenuStackCharacterV1* saved_focus;
 std::uint32_t status,visible,valid_menu,reserved;
};
struct MenuStackRenderV1 {
 std::uintptr_t identity; std::uint32_t flags,reserved;
 MenuStackCharacterV1* context; MenuStackCharacterV1* root;
 MenuStackCharacterV1* controller_focus;
 MenuStackMenuV1** states;std::uint32_t count,capacity;
 MenuStackMenuV1** catalog;std::uint32_t catalog_count,catalog_reserved;
};
struct MenuStackGlobalsV1 {
 std::int32_t last_open_menu;std::uint32_t in_game_menu,back_pressed,back_glive,multiplayer,multiplayer_igm,use_native_drm,reserved;
};
struct MenuStackV1 {
 MenuStackRenderV1** renders;std::uint32_t count,capacity;
 MenuStackMenuV1** registry;std::uint32_t registry_count,reserved;
 MenuStackRenderV1* base_render;MenuStackRenderV1* hud_root;
 MenuStackGlobalsV1* globals;
};
enum class MenuStackOperationV1:std::uint32_t {
 debug_message=1,license_check,menu_blur,menu_hide,menu_show,menu_focus,
 invoke_as,play_animation,reset_focus,set_focus,render_reset,
 reset_touch,process_touch,debug_load,debug_switch,register_listener,unregister_listener,
 menu_valid,menu_set_visible
};
struct MenuStackRequestV1 {
 MenuStackOperationV1 operation;std::uint32_t value;
 MenuStackRenderV1* render;MenuStackMenuV1* menu;
 MenuStackCharacterV1* character;const char* text;
 std::uintptr_t result;
};
struct MenuStackServicesV1 {
 void* context;int(*invoke)(void*,MenuStackV1*,MenuStackRequestV1*);
};
// 0 delivered, -1 malformed before mutation, -2 required service failure, -3
// unsupported source-invalid continuation (e.g. expired required weak pointer),
// -4 exhausted caller storage. Services may synchronously mutate valid records;
// objects/string/storage lifetimes must encompass all nested calls.
}
extern "C" {
int dh2_menu_stack_push_v1(dh2::ui::MenuStackV1*,dh2::ui::MenuStackMenuV1*,const dh2::ui::MenuStackServicesV1*);
int dh2_menu_stack_pop_v1(dh2::ui::MenuStackV1*,std::uint32_t all,const dh2::ui::MenuStackServicesV1*);
int dh2_menu_stack_pop_name_v1(dh2::ui::MenuStackV1*,const char*,std::uint32_t above,const dh2::ui::MenuStackServicesV1*);
int dh2_menu_stack_manager_push_v1(dh2::ui::MenuStackV1*,dh2::ui::MenuStackMenuV1*,const dh2::ui::MenuStackServicesV1*);
int dh2_menu_stack_manager_pop_v1(dh2::ui::MenuStackV1*,dh2::ui::MenuStackMenuV1*,const dh2::ui::MenuStackServicesV1*);
int dh2_menu_stack_hide_all_v1(dh2::ui::MenuStackV1*,const dh2::ui::MenuStackServicesV1*);
int dh2_menu_stack_contains_v1(const dh2::ui::MenuStackV1*,const dh2::ui::MenuStackMenuV1*,std::uint32_t*);
int dh2_menu_stack_find_v1(const dh2::ui::MenuStackV1*,const char*,dh2::ui::MenuStackMenuV1**);
// NativePush(0), Pop(1), PopAllAbove(2), PopAllMenus(3). converted_name must
// come from genuine caller as_value::to_xstring; this is not its implementation.
int dh2_menu_stack_native_v1(dh2::ui::MenuStackV1*,std::uint32_t operation,std::uint32_t argc,std::uint32_t first_as_type,const char*converted_name,const dh2::ui::MenuStackServicesV1*);
}
