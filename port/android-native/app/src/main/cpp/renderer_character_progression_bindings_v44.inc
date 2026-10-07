// Include after renderer_combat_text_v1.inc, player-manager helpers and current
// PlayerSkillsRuntime. Headers: character_progression_world_v23.hpp,
// player_xp_text_v1.hpp, player_save_write_owner_v1.hpp.
struct RendererProgressionProvidersV44 {
 std::shared_ptr<void> storage;
 const dh2::data::DesignSettingsProjection176* settings{};
 dh2::character::KillLevelProviderV23 level;
 // Reads actual Character+14e8, not merely the Skills VM's retained Save.
 std::function<bool(std::uintptr_t,dh2::data::PlayerSavegameV1*&,std::string&)> save_slot14e8;
 // Actual Application/SavegameManager difficulty getter (not Level118 mode).
 std::function<bool(std::int32_t&,std::string&)> application_difficulty;
 dh2::data::PlayerSaveWriteServicesV1 save;
 std::function<bool(std::string&)> negative_award_assert;
 // Full source player localized/menu/event87/tutorial/campaign/trophy branch.
 std::function<bool(dh2::character::ProgressionActorV1&,std::int32_t,std::string&)> level_presentation;
};
class RendererCharacterProgressionBindingsV44 {
 PlayerSkillsRuntime& t_;RendererProgressionProvidersV44 services_;
 std::unique_ptr<dh2::character::CharacterProgressionWorldV23> owner_;
 bool actor(std::uintptr_t id,dh2::character::ProgressionActorV1& out,std::string& e){
  if(!t_.world||!t_.world->design||!t_.targets||!t_.player){e="Required same live XP World/player design";return false;}
  const auto found=t_.world_actors.find(id);
  if(found==t_.world_actors.end()||!found->second.object||!found->second.object->properties){e="Required registered XP actor";return false;}
  const auto& object=found->second.object;
  auto design=t_.world->design->borrow();auto* classes=design.class_rows();
  if(!classes||classes->empty()||!design.characters()){e="Required actual XP actor/class cache";return false;}
  dh2::character::ProgressionActorV1 a;a.identity=id;a.state=object->properties.get();
  a.actors=design.characters();a.classes=classes->data();a.class_count=unsigned(classes->size());
  a.x=object->position[0];a.y=object->position[1];
  if(id==t_.fsm.character){
   const auto p=model_renderer::player_gameplay_binding();
   if(p.character!=id||p.skills!=t_.player.get()||!p.properties||p.gear!=player_equipment.get()){
    e="Required SAME XP player Gear/Skill/property graph";return false;
   }
   a.properties=p.properties;a.actor_index=p.actor_index;
   std::uint32_t player{};if(t_.player->session().source_is_player(player)){e=t_.player->session().error();return false;}a.player=player!=0;
   if(!services_.save_slot14e8||!services_.save_slot14e8(id,a.save,e))return false;
   if(a.save&&a.save!=t_.player->native_savegame()){e="XP Character14e8 differs from retained SAME Skill Save";return false;}
   if(!player_network_is_local_v4(t_,id,a.local,e)||!PlayerSkillsRuntime::heading_remote(&t_,a.remotely_updated,e))return false;
  }else{
   auto* npc=found->second.npc;if(!npc||!npc->session){e="Required SAME XP NPC PropertyView";return false;}
   a.properties=&npc->session->property_view();
   const auto* remote=npc->source_bool_field(0x118);if(!remote){e="Required NPC source remote118 XP field";return false;}a.remotely_updated=*remote!=0;
   auto q=t_.targets->targets().query_services();std::uintptr_t player{};
   const dh2::target_providers::Request24 query{dh2::target_providers::virtual_player,0,id,0};
   if(!q.invoke||q.invoke(q.context,&query,&player)){e="Required XP actor source IsPlayer";return false;}a.player=player!=0;
   if(a.player){e="Required secondary player Save/locality/selected actor row";return false;}
  }
  dh2::loader::CanonicalCurrentLevelBorrowV1 level;
  if(!services_.level.current||!services_.level.current(level,e))return false;
  if(!level||!level.kill_level()){e="Required actual XP current Level150";return false;}
  a.current_level_policy=int(level.kill_level()->loot_gate150);
  if(!services_.application_difficulty||!services_.application_difficulty(a.current_difficulty,e))return false;
  out=a;return true;
 }
 bool constant(const char* group,const char* name,std::int32_t& out,std::string& e){
  const auto* d=t_.world?t_.world->tables.design():nullptr;
  if(!group||!name||!d||!d->lookup||d->lookup(d->context,0,group,name,&out)){e="Required XP source constant "+std::string(group?group:"")+"."+(name?name:"");return false;}return true;
 }
 bool debug(const char* name,bool& out,std::string& e){std::uint32_t value{};
  if(!t_.world||!name||dh2_character_debug_load(t_.world->debug.get(),&t_.world->debug_files)!=1||
     dh2_character_debug_get(&value,t_.world->debug.get(),name,&t_.world->debug_files)!=1){e="Required XP source debug load/query";return false;}out=value!=0;return true;
 }
 bool save(dh2::character::ProgressionActorV1& a,std::string& e){
  if(!t_.player_save_load||!t_.player||a.identity!=t_.fsm.character||
     a.save!=&t_.player_save_load->save()||a.save!=t_.player->native_savegame()||
     a.properties!=&t_.player->session().property_view()){
   e="Required SAME LevelUp SG_Save Character14e8/profile/property owner";return false;
  }
  auto authority=std::shared_ptr<dh2::data::PlayerSaveLoadOwnerV1>(services_.storage,t_.player_save_load.get());
  dh2::data::PlayerSaveWriteOwnerV1 writer(std::move(authority),services_.save);
  return writer.save(e); // genuine NULL-profile early gate or actual named writers
 }
 bool text(dh2::character::ProgressionActorV1& victim,std::int32_t integer,std::string& e){
  using namespace renderer_combat_text_detail_v1;
  std::int32_t color{};if(!constant("ScrollingCombatText","XPColor",color,e))return false;
  dh2::character::skills::PlayerXPTextServicesV1 s;
  s.common={&t_,follower,renderer_combat_text_detail_v1::position,height,property,dual,renderer_combat_text_detail_v1::player,renderer_combat_text_detail_v1::constant,localized,enqueue};
  s.format=[](void*,const char* localized,std::int32_t value,std::string* out){
   std::string error;return localized&&out&&combat_text_sink.format_integer&&
    combat_text_sink.format_integer(combat_text_sink.context,localized,value,*out,error)?0:-1;
  };
  if(dh2::character::skills::player_xp_text_v1(victim.identity,integer,color,s)!=1){e="Required source XP localized anim_sct_xp follower/format/enqueue";return false;}return true;
 }
public:
 RendererCharacterProgressionBindingsV44(PlayerSkillsRuntime& t,RendererProgressionProvidersV44 s):t_(t),services_(std::move(s)){}
 void release()noexcept{owner_.reset();}
 bool bind(std::string& e){
  if(owner_||!services_.storage||!services_.settings||services_.level.lifetime.expired()||
     !services_.level.current||!services_.save_slot14e8||!services_.application_difficulty||
     !t_.world||!t_.player_manager||!t_.player||!player_equipment){
   e="Required actual XP roster/design/currentLevel/Save14e8/difficulty providers";return false;
  }
  dh2::character::CharacterProgressionServicesV23 s;s.owner=services_.storage;s.players=&t_.player_manager->manager();s.settings=services_.settings;
  s.actor=[this](auto id,auto& out,auto& error){return actor(id,out,error);};
  s.source.constant=[this](const char* name,auto& value,auto& error){return constant("CharacterDesign",name,value,error);};
  s.source.debug=[this](const char* name,auto& value,auto& error){return debug(name,value,error);};
  s.source.negative_award_assert=services_.negative_award_assert;
  s.source.regen_full=[this](auto& a,bool mp,auto& error){
   if(a.identity!=t_.fsm.character||a.properties!=&t_.player->session().property_view()){error="Required same LevelUp Regen Character";return false;}
   const dh2::character::skills::SkillAttackNativeServicesV6 debug{&t_,PlayerSkillsRuntime::regen_debug};
   if(dh2::character::skills::dh2_character_skill_regen_v6(a.properties,mp?1u:0u,-1,&debug)){error="Required genuine LevelUp RegenHP/MP";return false;}return true;
  };
  s.source.save=[this](auto& a,auto& error){return save(a,error);};
  s.source.level_presentation=services_.level_presentation; // missing callback retains exact source prefix
  s.source.statistics_player_lookup=[this](auto& a,auto& error){
   dh2::player::PlayerInfoFieldsV1* actual{};
   if(!t_.player_manager->manager().get_by_character(a.identity,false,actual,error)||!actual){if(error.empty())error="Required XP GetPlayerByCharacter before empty IncreaseStat";return false;}return true;
  };
  s.source.xp_text=[this](auto& victim,auto&,int value,auto& error){return text(victim,value,error);};
  owner_=std::make_unique<dh2::character::CharacterProgressionWorldV23>(std::move(s));return true;
 }
 dh2::character::CharacterProgressionWorldV23* owner()noexcept{return owner_.get();}
};
