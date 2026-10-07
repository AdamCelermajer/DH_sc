// SAME sourceWorld ScriptContext host projection. The borrowed scalar is
// refreshed from actual PlayerInfo330 at EVERY source request, never Save30
// or resolved property19. The current Level is read from the sole GS slot.
class RendererCanonicalHostContextV70 {
 std::weak_ptr<WorldScriptContext> world_;
 std::weak_ptr<dh2::application::ApplicationServicesOwnerV5> app_;
 dh2::character::HostPlayer8 player_{};dh2::character::HostLevel8 level_{};
 std::shared_ptr<void> player_pin_,level_pin_;
 std::string error_;
 static int invoke(void* raw,const dh2::character::HostContextRequest16* q,dh2::character::HostContextResponse16* out){
  auto& self=*static_cast<RendererCanonicalHostContextV70*>(raw);self.error_.clear();auto world=self.world_.lock();auto app=self.app_.lock();if(!q||!out||!world||!app)return 1;
  switch(q->service){
   case dh2::character::host_get_player:{
    auto pm=app->source_player_manager_v59();auto online=app->get_online_loading_v55();if(!pm||!online)return 1;
    // Whole offline GetHostingPlayer36e0b4/c4 selects internal0,false.
    if(online->byte5()){self.error_="Required whole online GetHostingPlayer host selector";return 1;}
    dh2::player::PlayerInfoFieldsV1* info{};if(!pm->get_by_internal(0,false,info,self.error_)||!info)return 1;
    auto actual=pm->network()->borrow(*info);if(!actual||!actual->managed_fields_produced_v70){self.error_="Required source PlayerInfo330 cached level producer";return 1;}
    self.player_pin_=actual;self.player_.cached_level=actual->character_level330_v70;out->data=&self.player_;return 0;
   }
   case dh2::character::host_get_current_level:{
    dh2::loader::CanonicalCurrentLevelBorrowV1 current;if(!model_renderer::borrow_current_native_level_v27(current,self.error_))return 1;
    self.level_pin_.reset();out->data=nullptr;if(current){self.level_pin_=current.level();const auto& fields=current.level()->constructor_fields_v3();self.level_={fields.row3c,fields.mode118};out->data=&self.level_;}return 0;
   }
   case dh2::character::host_get_range_rows:out->data=world->ranges.data();out->count=std::uint32_t(world->ranges.size());return 0;
   default:return 1; // Actual Lua capture owns source push, as original binder.
  }
 }
public:
 const std::string& error()const noexcept{return error_;}
 dh2::character::HostContextBindings16 bindings{{this,invoke}};
 RendererCanonicalHostContextV70(const std::shared_ptr<WorldScriptContext>& world,const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app):world_(world),app_(app){}
};
