// Include from renderer_player_manager_v1.inc after all runtime/platform types.
// Root WorldScriptContext member, forward-declared before that struct:
// shared_ptr<RendererPlayerManagerBootV25> player_manager_boot_v25;
// Root PlayerSkillsRuntime member and destructor BODY call BEFORE its owners:
// function<void()> player_manager_boot_return_v25;
// if(player_manager_boot_return_v25)player_manager_boot_return_v25();
// Before Gear.initialize, immediately after actual equipment_platform creation:
// prepare_player_manager_boot_v25(world,{actualInternal,controller,index,local,0,nullptr});
// Existing bind_development_player_manager_v2 delegates transfer below.
static dh2::player::MatchingLocalSelectionOwnerV4 source_matching_local_v4;
struct RendererPlayerManagerBootV25 {
 struct ProviderLifetime {std::weak_ptr<WorldScriptContext> world;};
 std::weak_ptr<WorldScriptContext> world;
 std::shared_ptr<ProviderLifetime> provider_lifetime;
 dh2::player::DevelopmentPlayerLaunchV2 launch;
 std::unique_ptr<dh2::player::PlayerNetworkLocalOwnerV4> network;
 std::unique_ptr<dh2::player::PlayerManagerCombatRuntimeV2> manager;
 dh2::player::PlayerNetworkLocalOwnerV4* actual_network{};
 dh2::player::PlayerManagerCombatRuntimeV2* actual_manager{};
 // AddCharacter's full source continuation is installed once its real actor,
 // Save/AI/visual/controller/light/Level producers are available. Never accepted
 // as an empty completion or a forced count write.
 std::function<bool(const dh2::player::PlayerManagerRequestV1&,
  dh2::player::PlayerManagerResponseV1&,std::string&)> continuation;
 bool prepared{},transferred{};
 static bool service(void* raw,const dh2::player::PlayerManagerRequestV1& q,
  dh2::player::PlayerManagerResponseV1& out,std::string& error){
  auto& self=*static_cast<RendererPlayerManagerBootV25*>(raw);using O=dh2::player::PlayerManagerOperationV1;
  auto current=self.world.lock();
  if(!current){error="Required retained PlayerManager boot World expired";return false;}
  if(q.operation==O::construct_player_info){
   if(!q.player||!self.actual_network){error="Required actual PlayerInfo/CNet constructor owner";return false;}
   *q.player=dh2::player::PlayerInfoFieldsV1{};
   // Independent provider lease pins actual services but only weakly borrows
   // World: no World→boot→networkRecord→World ownership cycle on boot failure.
   return self.actual_network->construct(*q.player,self.provider_lifetime,error);
  }
  if(q.operation==O::online_enabled){
   if(!equipment_platform||equipment_platform->world!=current){error="Required same boot GetOnline owner";return false;}
   out.value=equipment_platform->online_mode;return true;
  }
  if(self.continuation)return self.continuation(q,out,error);
  error="Required original PlayerManager continuation "+std::to_string(std::uint32_t(q.operation));return false;
 }
};
[[maybe_unused]] static void prepare_player_manager_boot_v25(const std::shared_ptr<WorldScriptContext>& world,
 const dh2::player::DevelopmentPlayerLaunchV2& launch){
 if(!world||!equipment_platform||equipment_platform->world!=world)
  throw std::runtime_error("Required actual equipment platform before PlayerManager boot");
 auto& slot=world->player_manager_boot_v25;
 if(slot){
  if(!slot->prepared||!slot->actual_manager||slot->launch.internal!=launch.internal||slot->launch.controller!=launch.controller||
    slot->launch.local_index!=launch.local_index||slot->launch.local!=launch.local)
   throw std::runtime_error("Existing PlayerManager boot source launch differs");
  return; // SAME C1/AddPlayer owner, no replay on GL recreation.
 }
 auto boot=std::make_shared<RendererPlayerManagerBootV25>();boot->world=world;boot->launch=launch;
 boot->provider_lifetime=std::make_shared<RendererPlayerManagerBootV25::ProviderLifetime>();boot->provider_lifetime->world=world;
 boot->network=std::make_unique<dh2::player::PlayerNetworkLocalOwnerV4>(source_matching_local_v4);
 boot->actual_network=boot->network.get();
 boot->manager=std::make_unique<dh2::player::PlayerManagerCombatRuntimeV2>(
  dh2::player::PlayerManagerCombatServicesV2{boot.get(),RendererPlayerManagerBootV25::service});
 boot->actual_manager=boot->manager.get();
 std::string error;
 if(!boot->manager->initialize())throw std::runtime_error("Source PlayerManager C1 failed: "+boot->manager->error());
 if(!boot->manager->manager().add_player(launch.internal,launch.controller,launch.local_index,launch.local,error))
  throw std::runtime_error("Source pre-Gear AddPlayer failed: "+error);
 boot->prepared=true;slot=std::move(boot);
}
[[maybe_unused]] static bool player_manager_bound_character_count_v25(const std::shared_ptr<WorldScriptContext>& world,
 std::int32_t& value,std::string& error){
 if(!world||!world->player_manager_boot_v25||!world->player_manager_boot_v25->prepared||
    !world->player_manager_boot_v25->actual_manager){error="Required actual pre-Gear/same runtime PlayerManager count6c4";return false;}
 value=*world->player_manager_boot_v25->actual_manager->manager().character_count_field();return true;
}
static void transfer_player_manager_boot_v25(PlayerSkillsRuntime& runtime,
 const dh2::player::DevelopmentPlayerLaunchV2& launch){
 auto boot=runtime.world?runtime.world->player_manager_boot_v25:nullptr;
 if(!boot||!boot->prepared||boot->transferred||!boot->manager||!boot->network||
    runtime.player_manager||runtime.player_network_local||!launch.character||
    boot->launch.internal!=launch.internal||boot->launch.controller!=launch.controller||
    boot->launch.local_index!=launch.local_index||boot->launch.local!=launch.local)
  throw std::runtime_error("Required SAME source pre-Gear PlayerManager transfer");
 dh2::player::PlayerInfoFieldsV1* record{};std::string error;
 if(!boot->manager->manager().get_by_internal(launch.internal,false,record,error)||!record||
    record->internal670!=launch.internal||(record->character660&&record->character660!=launch.character))
  throw std::runtime_error("PlayerInfo source adoption differs: "+error);
 // Explicit existing development 3722ec field adoption only. Full original
 // AddCharacter/InitAll/Save/skill/controller/light remains its V5 continuation.
 record->character660=launch.character;record->character_base_id13c8=launch.same_base_id;
 runtime.player_network_local=std::move(boot->network);runtime.player_manager=std::move(boot->manager);
 boot->transferred=true;
 runtime.player_manager_boot_return_v25=[weak=std::weak_ptr<RendererPlayerManagerBootV25>(boot),&runtime](){
  auto actual=weak.lock();if(!actual)return;
  if(runtime.player_manager.get()!=actual->actual_manager||runtime.player_network_local.get()!=actual->actual_network){
   runtime.error="Source PlayerManager transfer ownership changed before return";
   __android_log_print(ANDROID_LOG_ERROR,"DH2Native","%s",runtime.error.c_str());
   actual->actual_manager=nullptr;actual->actual_network=nullptr;actual->transferred=false;
   return; // Future boot/count borrow fails explicitly, never a dangling read.
  }
  actual->manager=std::move(runtime.player_manager);actual->network=std::move(runtime.player_network_local);
  actual->transferred=false;
 };
}
