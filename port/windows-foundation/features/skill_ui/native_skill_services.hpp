#pragma once
#include "native_skill_details.hpp"
#include "skill_ui.hpp"
#include "../../../game-data/fresh_inventory_owned_v4.hpp"
#include "../../../level-world/character_script_session_v3.hpp"
#include <cstdio>

namespace dh2::ui { class CharacterMenuActionsOwnerV1; }
namespace dh::foundation::skill_ui {

// Character::_HasShield (0x003b6fa4) delegates to ItemInventory::HasShield
// (0x00400110) and returns one Lua boolean. This narrow source binding borrows
// the same live inventory that the character menu owns; it never synthesizes
// an equipped item or caches the selected equipment set.
class SourceHasShieldBindingV1 {
 std::function<const dh2::data::FreshInventoryOwnedV4*()> inventory_;
 void* fallback_context_{};
 int(*fallback_)(void*,std::uint32_t,dh2_script_function*,void**){};
 std::string error_;
 static int invoke(void* opaque,const dh2_script_value* args,std::uint32_t count,
                   dh2_script_value* out,std::uint32_t capacity,
                   std::uint32_t* written,char* error,std::size_t error_size){
  if(!opaque||!written||(count&&!args)||(capacity&&!out))return -1;
  auto& self=*static_cast<SourceHasShieldBindingV1*>(opaque);*written=0;self.error_.clear();
  auto fail_source=[&](const char* message){self.error_=message;if(error&&error_size)std::snprintf(error,error_size,"%s",message);return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;};
  if(count!=0)return fail_source("Source HasShield takes no arguments");
  if(capacity<1)return fail_source("Source HasShield boolean result storage unavailable");
  const auto* inventory=self.inventory_?self.inventory_():nullptr;
  if(!inventory)return fail_source("Source HasShield same-player inventory provider unavailable");
  const auto selected=inventory->current_equipment();
  if(selected<0||static_cast<std::size_t>(selected)>=inventory->equipment().size())
   return fail_source("Source HasShield selected equipment set unavailable");
  const auto* slot=inventory->equipment()[selected][2];
  const auto* row=slot&&slot->item?dh2::data::item(inventory->table(),slot->item->id):nullptr;
  if(slot&&slot->item&&!row)return fail_source("Source HasShield live equipment table row unavailable");
  out[0]={};out[0].type=DH2_SCRIPT_BOOLEAN;out[0].boolean=row&&row->record.words[22]==6;
  *written=1;return 0;
 }
public:
 explicit SourceHasShieldBindingV1(
  std::function<const dh2::data::FreshInventoryOwnedV4*()> inventory,
  void* fallback_context=nullptr,
  int(*fallback)(void*,std::uint32_t,dh2_script_function*,void**)=nullptr)
  :inventory_(std::move(inventory)),fallback_context_(fallback_context),fallback_(fallback){}
 SourceHasShieldBindingV1(const SourceHasShieldBindingV1&)=delete;
 SourceHasShieldBindingV1& operator=(const SourceHasShieldBindingV1&)=delete;
 static int binding(void* opaque,std::uint32_t address,dh2_script_function* function,void** context){
  if(!opaque||!function||!context)return -1;
  auto& self=*static_cast<SourceHasShieldBindingV1*>(opaque);
  if(address!=0x3b6fa4u)return self.fallback_?self.fallback_(self.fallback_context_,address,function,context):0;
  *function=&invoke;*context=opaque;return 1;
 }
 const std::string& error()const noexcept{return error_;}
};

// Inputs supplied by the live menu composition. The action owner and query
// owner must both borrow the same selected player graph. The source class
// frame follows the recovered MenuBase::FS_GetPlayerClass2 source branch over
// the retained save class row. An optional native frame callback can serve as
// a consistency check; unsupported source class rows still fail closed.
struct LiveServicesInputV1 {
 int player_index=0;
 std::function<bool(unsigned&,std::string&)> source_class_frame;
 std::function<bool(const std::string&,std::string&,std::string&)> symbol_text;
 std::function<bool(int,const std::string&,character_menu::Frame&,std::string&)> icon;
};

// Bind the presenter to the existing source character-menu action/query
// owners. Saved levels/slots, progression gates, NativeGetSkillDetails text,
// assignment and training all remain owned by that same live player graph.
// Missing or mismatched native providers fail closed when the service is used.
bool bind_live_player_services(dh2::ui::CharacterMenuActionsOwnerV1&,
 dh2::ui::CharacterMenuQueriesOwnerV1&,dh2::data::SkillTables::Borrow,
 const LiveServicesInputV1&,Services&,std::string&);
}
