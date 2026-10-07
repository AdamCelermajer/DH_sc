#include "combat_flash_inputs_v1.hpp"
namespace dh2::ui {
int combat_flash_tick_inputs_v1(CombatFlashTickInputsV1* out,const CombatFlashTickBorrowV1& b){
 if(!out||!b.application_dt||(b.selected_level&&!b.load_phase))return -1;
 const CombatFlashTickInputsV1 value{*b.application_dt,b.selected_level?*b.load_phase:0};*out=value;return 1;
}
int combat_flash_debug_gate_v1(bool* out,character::DebugSwitches* debug,const character::DebugFileServices24* files){
 if(!out||!debug||!files)return -1;
 const auto loaded=dh2_character_debug_load(debug,files);if(loaded<0)return loaded;
 std::uint32_t value{};const auto status=dh2_character_debug_get(&value,debug,"IsDisablingFlashAnimation",files);if(status<0)return status;
 *out=value!=0;return 1;
}
}
