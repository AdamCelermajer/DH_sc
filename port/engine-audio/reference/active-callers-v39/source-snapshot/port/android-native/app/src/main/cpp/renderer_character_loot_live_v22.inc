// Include after the Item graph and KillV21 adapters. Preparation ONLY: root
// publishes at real ItemManager initialization and hooks Kill only when its
// current Level/XP/quests/audio source continuation is available.
struct RendererCharacterLootServicesV22 {
 std::shared_ptr<void> provider_lease;
 dh2::data::LootPowerResourcesV7::Borrow powers;
 dh2::character::LootDropAwardServicesV8 drop;
 dh2::character::WorldItemLootCreationServicesV10 creation;
 std::function<bool(std::uintptr_t,dh2::character::CharacterLootLiveBorrowV22&,std::string&)> actor;
 std::function<bool(std::uintptr_t,bool&,std::string&)> is_player;
};
class RendererCharacterLootLiveV22 {
 PlayerSkillsRuntime& t_;RendererCharacterLootServicesV22 services_;
 std::unique_ptr<dh2::character::CharacterLootLiveV22> owner_;
 static bool actor(void* p,std::uintptr_t id,dh2::character::CharacterLootLiveBorrowV22& out,std::string& e){
  auto& self=*static_cast<RendererCharacterLootLiveV22*>(p);
  if(!self.services_.actor){e="Required canonical NPC/Player loot actor projection";return false;}
  return self.services_.actor(id,out,e);
 }
 static bool player(void* p,std::uintptr_t id,bool& out,std::string& e){
  auto& self=*static_cast<RendererCharacterLootLiveV22*>(p);
  if(!self.services_.is_player){e="Required actual Character loot IsPlayer";return false;}
  return self.services_.is_player(id,out,e);
 }
public:
 RendererCharacterLootLiveV22(PlayerSkillsRuntime& t,RendererCharacterLootServicesV22 s):t_(t),services_(std::move(s)){}
 bool bind(std::string& e){
  if(owner_||!t_.targets||!t_.player_manager||!player_equipment||!services_.provider_lease||!services_.actor||!services_.is_player){e="Required SAME World/Gear/PlayerManager loot provider lifetime";return false;}
  dh2::data::LootTablesV2::Borrow tables;dh2::data::ItemPowerTablesV5::Borrow definitions;
  dh2::data::ItemTextServicesV5 text;dh2::data::LootRandom8V2* rng{};
  if(!player_equipment->loot_sources_v8(tables,definitions,text,rng,e)||!rng){if(e.empty())e="Required SAME Gear Application RNG and loot cache";return false;}
  // Use existing Gear's real name/stat/requirements presentation provider.
  // Debug, powers, notifications and positive current-Level remain supplied.
  auto creation=services_.creation;creation.source.text=text;
  dh2::character::CharacterLootLiveServicesV22 services;
  services.provider_lease=services_.provider_lease;services.context=this;
  services.actor=actor;services.is_player=player;services.drop=services_.drop;services.creation=creation;
  owner_=std::make_unique<dh2::character::CharacterLootLiveV22>(*t_.targets,t_.player_manager->manager(),tables,
   services_.powers,definitions,*rng,std::move(services));return true;
 }
 bool precache(std::string& e){if(!owner_){e="Required bound canonical ItemManager before PreCache";return false;}return owner_->precache(e);}
 bool bind_pool(dh2::character::WorldLootItemRuntimeV1& actual,std::string& e){if(!owner_){e="Required loot source owner before SAME pool publication";return false;}return owner_->bind_pool(actual,e);}
 dh2::character::LootDropAwardServicesV8 drop_services(){return owner_?owner_->drop_services():dh2::character::LootDropAwardServicesV8{};}
 bool route(dh2::character::KillActor56& a,const dh2::character::KillRequest56& q,
  dh2::character::KillResponse16& out,bool& handled,std::string& e){
  handled=q.service==dh2::character::kill_drop_loot;if(!handled)return true;
  if(!owner_){e="Required retained live DropLoot graph";return false;}return owner_->route(a,q,out,handled,e);
 }
 dh2::character::CharacterLootLiveV22* owner()noexcept{return owner_.get();}
};
