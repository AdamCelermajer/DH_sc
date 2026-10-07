// Include after renderer_character_loot_gameplay_v23.inc and V31 actor adapters.
// Header world_loot_canonical_bindings_v44.hpp must be included at TU scope.
// Construction creates no pool. Only the actual published Level state29 hook
// may invoke precache_source_stage29; this adapter never calls initialize_final.
class RendererCanonicalLootConnectionV44 {
 PlayerSkillsRuntime& t_;
 RendererLootGameplayProvidersV23 source_;
 void* upstream_context_{};void* upstream_physical_context_{};
 decltype(dh2::character::WorldLootGameplayServicesV23::frame_services) upstream_frame_{};
 decltype(dh2::character::WorldLootGameplayServicesV23::tooltip_ooi) upstream_tooltip_{};
 decltype(RendererLootGameplayProvidersV23::physical_services) upstream_physical_{};
 void* upstream_item_context_{};
 decltype(dh2::character::WorldItemLiveServicesV5::outer_item) upstream_outer_{};
 dh2::character::WorldLootCanonicalServicesV44 canonical_source_;
 std::unique_ptr<dh2::character::WorldLootCanonicalBindingsV44> canonical_;
 std::unique_ptr<RendererCharacterLootGameplayV23> gameplay_;
 static dh2::character::WorldItemLiveOwnerV5* items(void* raw){
  auto& self=*static_cast<RendererCanonicalLootConnectionV44*>(raw);
  auto* owner=self.gameplay_?self.gameplay_->owner():nullptr;
  return owner?owner->items():nullptr;
 }
 static bool save(void* raw,std::uintptr_t id,dh2::character::LootCharacterSaveBorrowV44& out,std::string& e){
  auto& self=*static_cast<RendererCanonicalLootConnectionV44*>(raw);auto& source=self.canonical_source_;
  if(!source.character_save){e="Required original Character+14e8 Save source borrow";return false;}
  return source.character_save(source.context,id,out,e);
 }
 static bool condition(void* raw,std::uintptr_t id,bool& out,std::string& e){
  auto& s=static_cast<RendererCanonicalLootConnectionV44*>(raw)->canonical_source_;
  if(!s.condition_is_true){e="Required actual compiled Condition IsTrue";return false;}
  return s.condition_is_true(s.context,id,out,e);
 }
 static bool assertion(void* raw,std::string& e){
  auto& s=static_cast<RendererCanonicalLootConnectionV44*>(raw)->canonical_source_;
  if(!s.null_handle_assertion){e="Required reached ObjectHandle NULL Debug assertion";return false;}
  return s.null_handle_assertion(s.context,e);
 }
 static bool unknown(void* raw,const char* name,std::string& e){
  auto& s=static_cast<RendererCanonicalLootConnectionV44*>(raw)->canonical_source_;
  if(!s.unknown_type_debug){e="Required reached unknown source class Debug";return false;}
  return s.unknown_type_debug(s.context,name,e);
 }
 static bool peer(void* raw,void* address,dh2::character::LootPhysicalPeerBorrowV44& out,std::string& e){
  auto& s=static_cast<RendererCanonicalLootConnectionV44*>(raw)->canonical_source_;
  if(!s.physical_peer){e="Required exact actual physical callback-context identity projection";return false;}
  return s.physical_peer(s.context,address,out,e);
 }
 static bool fields(void* raw,std::uintptr_t id,dh2::character::LootPhysicalPeerBorrowV44& out,std::string& e){
  auto& s=static_cast<RendererCanonicalLootConnectionV44*>(raw)->canonical_source_;
  if(!s.character_fields){e="Required actual published peer visible80/Character OOI fields";return false;}
  return s.character_fields(s.context,id,out,e);
 }
 static bool destroy(void* raw,std::uintptr_t id,std::string& e){
  auto& s=static_cast<RendererCanonicalLootConnectionV44*>(raw)->canonical_source_;
  if(!s.destroy_previous_physical){e="Required reached previous PhysicalObject destructor";return false;}
  return s.destroy_previous_physical(s.context,id,e);
 }
 static bool state(void* raw,std::uintptr_t id,const std::int32_t*& out,std::shared_ptr<void>& lease,std::string& e){
  auto& s=static_cast<RendererCanonicalLootConnectionV44*>(raw)->canonical_source_;
  if(!s.character_state){e="Required actual Character SM.state20 source producer";return false;}
  return s.character_state(s.context,id,out,lease,e);
 }
 static bool outer(void* raw,const dh2::character::WorldItemRequestV1& q,std::int32_t& out,std::string& e){
  auto& self=*static_cast<RendererCanonicalLootConnectionV44*>(raw);bool handled{};
  if(!self.canonical_->outer_item(q,out,handled,e))return false;if(handled)return true;
  if(!self.upstream_outer_){e="Required reached source Item outer/tooltip operation "+std::to_string(unsigned(q.operation));return false;}
  return self.upstream_outer_(self.upstream_item_context_,q,out,e);
 }
 static bool current(void* raw,dh2::character::LootCurrentLevelV23& out,std::string& e){
  auto& self=*static_cast<RendererCanonicalLootConnectionV44*>(raw);
  return self.canonical_->current_level(out,e);
 }
 static bool constant(void* raw,const char* group,const char* key,std::int32_t& out,std::string& e){
  auto& self=*static_cast<RendererCanonicalLootConnectionV44*>(raw);
  if(!self.t_.world){e="Required current SAME loot World design snapshot";return false;}
  return dh2::character::world_loot_design_constant_v44(self.t_.world->tables.design(),group,key,out,e);
 }
 static bool frame(void* raw,dh2::character::WorldItemGraphV3& graph,
   dh2::character::WorldItemFrameServicesV5& out,std::string& e){
  auto& self=*static_cast<RendererCanonicalLootConnectionV44*>(raw);
  if(!self.upstream_frame_){e="Required shared floor/PF/runtime/camera Item frame provider";return false;}
  return self.upstream_frame_(self.upstream_context_,graph,out,e);
 }
 static bool tooltip(void* raw,std::uintptr_t id,std::uintptr_t& out,std::string& e){
  auto& self=*static_cast<RendererCanonicalLootConnectionV44*>(raw);
  if(!self.upstream_tooltip_){e="Required reached actual tooltip Character OOI";return false;}
  return self.upstream_tooltip_(self.upstream_context_,id,out,e);
 }
 static bool physical(void* raw,dh2::character::RetainedWorldItemObjectV1& item,
   dh2::character::WorldItemPhysicalServicesV2& out,std::string& e){
  auto& self=*static_cast<RendererCanonicalLootConnectionV44*>(raw);
  if(self.upstream_physical_&&!self.upstream_physical_(
     self.upstream_physical_context_,item,out,e))return false;
  return self.canonical_->physical_services(item,out,e);
 }
public:
 RendererCanonicalLootConnectionV44(PlayerSkillsRuntime& t,RendererLootGameplayProvidersV23 source,
   dh2::loader::CanonicalGSLevelGlobalSlotV1 actual_gs,
   dh2::character::WorldLootCanonicalServicesV44 canonical_source)
  :t_(t),source_(std::move(source)),canonical_source_(std::move(canonical_source)){
  upstream_context_=source_.source.context;upstream_frame_=source_.source.frame_services;
  upstream_tooltip_=source_.source.tooltip_ooi;
  upstream_physical_context_=source_.physical_context;upstream_physical_=source_.physical_services;
  upstream_item_context_=source_.source.items.context;upstream_outer_=source_.source.items.outer_item;
  if(!t_.world||!t_.world->canonical_world||!t_.player_manager)
   throw std::invalid_argument("Required SAME canonical World and PM before loot composition");
  auto s=canonical_source_;s.context=this;s.items=items;s.character_save=save;
  s.condition_is_true=condition;s.null_handle_assertion=assertion;s.unknown_type_debug=unknown;
  s.physical_peer=peer;s.character_fields=fields;s.character_state=state;s.destroy_previous_physical=destroy;
  canonical_=std::make_unique<dh2::character::WorldLootCanonicalBindingsV44>(
   t_.world->canonical_world->manager,t_.player_manager->manager(),std::move(actual_gs),std::move(s));
 }
 bool precache_source_stage29(std::string& e){
  dh2::character::LootCurrentLevelV23 level;
  if(!canonical_->current_level(level,e))return false;
  if(!level.identity||!level.phase130||*level.phase130!=29){
   e="Item145 requires actual published GS Level stage29; no demo-C1 fallback";return false;
  }
  if(gameplay_){e="Item stage29 retains its reached prefix; retry unavailable";return false;}
  auto p=std::move(source_);p.source.items.factory=canonical_->factory_services(p.source.items.factory);
  p.source.context=this;p.source.current_level=current;p.source.frame_services=frame;p.source.tooltip_ooi=tooltip;
  p.source.items.context=this;p.source.items.outer_item=outer;
  p.source.loot.drop.context=this;p.source.loot.drop.constant=constant;
  p.physical_context=this;p.physical_services=physical;
  gameplay_=std::make_unique<RendererCharacterLootGameplayV23>(t_,std::move(p));
  return gameplay_->bind(e);
 }
 bool route(dh2::character::KillActor56& actor,const dh2::character::KillRequest56& q,
   dh2::character::KillResponse16& out,bool& handled,std::string& e){
  handled=q.service==dh2::character::kill_drop_loot;if(!handled)return true;
  if(!gameplay_){e="Actual stage29 Item145 producer has not run";return false;}
  return gameplay_->route(actor,q,out,handled,e);
 }
 RendererCharacterLootGameplayV23* gameplay()noexcept{return gameplay_.get();}
 dh2::character::WorldLootCanonicalBindingsV44& canonical()noexcept{return *canonical_;}
};
