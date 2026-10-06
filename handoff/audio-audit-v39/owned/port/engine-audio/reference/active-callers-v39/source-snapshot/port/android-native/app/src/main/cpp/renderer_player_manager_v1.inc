// Include after PlayerSkillsRuntime/EquipmentPlatform declarations. Add member:
// std::unique_ptr<dh2::player::PlayerManagerCombatRuntimeV2> player_manager;
// This is the explicit development launch transport, not NativeStartGame.
// One Application matching selection/identity owner, retained across world/GL
// replacement. Network/room transport and modes2..4 remain required providers.
#include "renderer_player_manager_boot_v25.inc"
static bool player_network_is_local_v4(PlayerSkillsRuntime& t,std::uintptr_t character,
 bool& out,std::string& error){
 if(!character){out=false;return true;} // Original PlayerManager.IsLocalPlayer NULL leaf.
 if(!t.player_manager||!t.player_network_local){error="Required same source PlayerManager/network identity owners";return false;}
 dh2::player::PlayerInfoFieldsV1* record{};
 if(!t.player_manager->manager().get_by_character(character,false,record,error))return false;
 if(!record){error="Required source IsLocal PlayerInfo receiver";return false;}
 return t.player_network_local->is_local(*record,out,error);
}
static void bind_development_player_manager_v2(PlayerSkillsRuntime& t,
 const dh2::player::DevelopmentPlayerLaunchV2& launch){
 transfer_player_manager_boot_v25(t,launch);
}
// Add to genuine WorldSkillCombatBackendsV6.hit. The facade already handles
// isDead/isPlayer/isMonster/debug. Existing trophy wrapper handles trophies.
static int player_manager_hit_v2(void* p,dh2::character::HitActor32*,
 const dh2::character::HitRequest32* q,std::uintptr_t* out){
 auto& t=*static_cast<PlayerSkillsRuntime*>(p);if(!q||!out||!t.player_manager)return -1;
 if(q->service==dh2::character::skills::hit_local_player_v6){
  bool local{};if(!player_network_is_local_v4(t,q->subject,local,t.error))return -1;
  *out=local?1u:0u;return 0;
 }
 const int status=t.player_manager->hit(*q,out);
 if(status){if(status<0)t.error=t.player_manager->error();return status>0?0:-1;}
 if(q->service==dh2::character::hit_online){
  std::uintptr_t identity{};std::int32_t online{};
  if(!EquipmentPlatform::query(equipment_platform.get(),dh2::player::EquipmentWorldQueryV1::online,
      0,identity,online,t.error))return -1;
  *out=std::uintptr_t(online);return 0;
 }
 if(q->service==dh2::character::hit_application_switch){
  if(!q->name||!t.world||!t.world->saved_options){t.error="Required retained Application SavegameManager";return -1;}
  // Application::IsSavedOptionOn320e14 -> hasOption46d4a8 ->
  // isOptionToggled46d418 (type0 and saved value == descriptor maximum).
  const auto& options=*t.world->saved_options;
  const auto* descriptor=options.descriptor(q->name);
  *out=options.has_option(q->name)&&descriptor&&descriptor->type==0&&
       options.option(q->name)==descriptor->maximum;
  return 0;
 }
 t.error="Required original Hit source service "+std::to_string(q->service);return -1;
}
