// Populated retained geometry/world input. All type, faction, dead, interaction,
// cast, inventory and damage answers execute recovered native kernels. Controller
// LookAt and AI_AddAggro are deliberately absent, never accepted as empty effects.
struct PopulatedWorldV6 {
 CharacterGameDesign::Borrow design;PlayerEquipmentRenderOwnerV1* gear{};
 sk::CharacterPlayerSkillsV6* player{};NativeDebug* debug{};bool retained_encounter=false;
 std::shared_ptr<data::PropertyState> enemy_properties;data::PropertyView enemy_view;
 target_search::Object48 objects[2]{};
 target_search::Entry16 entry{},entry_sentinel{};target_search::Room16 sentinel{},room{};
 target_search::Registry8 search_registry{};
 target_providers::Handle16 handles[2]{};target_providers::Record16 records[2]{};
 target_providers::Registry24 handles_registry{};
 sk::SkillTargetCharacterV6 characters[2]{};
 std::vector<std::int32_t> types;target_providers::Types16 ai_types{};
 data::CombatantView facts[2]{};sk::SkillAttackActorV6 attacks[2]{};
 sk::SkillApplyActorV6 applies[2]{};
 HitActor32 hit{};HitAttacker24 hit_attacker{};HitServices16 hit_services{this,hit_service};
 data::AggroEntry outgoing_entries[8]{},incoming_entries[8]{};
 data::AggroTable enemy_outgoing{outgoing_entries,0,8},player_incoming{incoming_entries,0,8};
 std::uint32_t required_hit=0;unsigned positive_hp_prefixes=0;
 std::uint16_t combos[2]{};std::uint8_t invulnerable[2]{},push_death[2]{};
 std::int32_t network_ids[2]{-1,-1};unsigned searches{},calculations{},aggro_failures{};
 data::CombatResult latest{};std::uint32_t required_application=0;
 int index(std::uintptr_t id)const {return id==objects[0].identity?0:id==objects[1].identity?1:-1;}
 void refresh(){
  check(player&&gear);characters[0].resolved=player->session().property_view().resolved;
  characters[1].resolved=enemy_view.resolved;
  facts[0].state=3;facts[0].combo_hits=combos[0]; // Selected retained source idle input.
  std::string error;check(gear->combat_view(facts[0],error),error);
  check(facts[0].properties==characters[0].resolved,"Exact Gear/V6 resolved borrow");
  facts[1].properties=enemy_view.resolved;facts[1].state=0;facts[1].combo_hits=combos[1];
  attacks[0]={objects[0].identity,&player->session().property_view(),&facts[0]};
  attacks[1]={objects[1].identity,&enemy_view,&facts[1]};
  for(unsigned i=0;i<2;++i){applies[i].identity=objects[i].identity;applies[i].properties=attacks[i].properties;
   applies[i].combo=&combos[i];applies[i].invulnerable=&invulnerable[i];applies[i].push_death=&push_death[i];applies[i].network_id=&network_ids[i];}
  applies[0].buffs=player->native_buffs();
  hit={objects[1].identity,&enemy_view,objects[1].identity+0x100,0,0};
  hit_attacker={objects[0].identity,&handles[0],&handles_registry};
  applies[0].attacker_handle=&hit_attacker;applies[1].hit=&hit;applies[1].hit_services=&hit_services;
 }
 static int query(void* p,const target_providers::Request24* q,std::uintptr_t* out){
  auto& w=*static_cast<PopulatedWorldV6*>(p);w.refresh();int i=w.index(q->subject),j=w.index(q->other);if(i<0)return -1;
  if(q->service==target_providers::ai_friend)return -1; // Separate friend owner required if reached.
  if(q->service==target_providers::ai_enemy){
   if(j<0)return -1;std::int32_t a,b;check(!sk::dh2_character_skill_target_query_v6(&a,target_providers::is_player,&w.characters[i],nullptr,&w.ai_types,nullptr));check(!sk::dh2_character_skill_target_query_v6(&b,target_providers::is_player,&w.characters[j],nullptr,&w.ai_types,nullptr));
   const bool enemy=data::ai_enemy(*w.design.ai(),w.characters[i].resolved[0],w.characters[j].resolved[0],a,b);
   *out=enemy;return 0;}
  unsigned op=q->service==target_providers::virtual_dead?target_providers::is_dead:q->service==target_providers::virtual_player?target_providers::is_player:q->service==target_providers::virtual_character?target_providers::is_character:0;
  if(!op)return -1;std::int32_t result;auto s=target_providers::Services16{&w,query};
  if(sk::dh2_character_skill_target_query_v6(&result,op,&w.characters[i],j<0?nullptr:&w.characters[j],&w.ai_types,&s))return -1;*out=std::uintptr_t(result);return 0;
 }
 static int search(void* p,const target_search::Request24* q,target_search::Response16* out){
  auto& w=*static_cast<PopulatedWorldV6*>(p);w.refresh();++w.searches;auto i=w.index(q->subject),j=w.index(q->other);if(i<0)return -1;
  if(q->service==target_search::resolve_character){out->word=reinterpret_cast<std::uintptr_t>(&w.objects[i]);return 0;}
  if(q->service==target_search::is_enemy){std::uintptr_t result;target_providers::Request24 request{target_providers::ai_enemy,0,q->subject,q->other};if(query(p,&request,&result))return -1;out->word=result;return 0;}
  if(q->service==target_search::melee_radius||q->service==target_search::interaction_radius){auto* ai=data::ai_props(*w.design.ai(),w.characters[i].resolved[1]);if(!ai)return -1;out->number=q->service==target_search::melee_radius?ai->melee_radius:ai->interact_radius;return 0;}
  const unsigned op=q->service==target_search::is_player?target_providers::is_player:q->service==target_search::is_dead?target_providers::is_dead:q->service==target_search::is_character?target_providers::is_character:q->service==target_search::is_interactive?target_providers::is_interactive:q->service==target_search::interaction_type?target_providers::interaction_type:q->service==target_search::is_zonable?target_providers::is_zonable:0;
  if(!op)return -1;std::int32_t result;auto services=target_providers::Services16{&w,query};if(sk::dh2_character_skill_target_query_v6(&result,op,&w.characters[i],j<0?nullptr:&w.characters[j],&w.ai_types,&services))return -1;out->word=std::uintptr_t(result);return 0;
 }
 static int resolve(void* p,std::uintptr_t id,target_search::Object48** out){auto& w=*static_cast<PopulatedWorldV6*>(p);int i=w.index(id);if(i<0)return -1;*out=&w.objects[i];return 0;}
 static int type(void* p,std::uintptr_t id,const char** out){return static_cast<PopulatedWorldV6*>(p)->index(id)<0?-1:dh2::gameobject_lua::dh2_gameobject_lua_type(out,dh2::gameobject_lua::character);}
 static int methods(void* p,std::uintptr_t id,const dh2_script_object_method** out,std::uint32_t* count){return static_cast<PopulatedWorldV6*>(p)->index(id)<0?-1:dh2::gameobject_lua::dh2_gameobject_lua_methods(out,count,dh2::gameobject_lua::character);}
 static int method(void*,const dh2_script_callback_scope*,std::uintptr_t,std::uint32_t,const dh2_script_value*,std::uint32_t,dh2_script_value*,std::uint32_t,std::uint32_t*,char*,std::size_t){return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;}
 static int handle(void* p,std::uintptr_t id,target_providers::Handle16** out,target_providers::Registry24** registry){auto& w=*static_cast<PopulatedWorldV6*>(p);auto i=w.index(id);if(i<0)return -1;*out=&w.handles[i];*registry=&w.handles_registry;return 0;}
 static int actor(void* p,std::uintptr_t id,sk::SkillAttackActorV6** a,sk::SkillApplyActorV6** b){auto& w=*static_cast<PopulatedWorldV6*>(p);w.refresh();auto i=w.index(id);if(i<0)return -1;*a=&w.attacks[i];*b=&w.applies[i];return 0;}
 static int application(void* p,const sk::SkillApplyRequestV6* q,sk::SkillApplyResponseV6* out,data::CombatResult* attack){
  auto& w=*static_cast<PopulatedWorldV6*>(p);w.latest=*attack;w.required_application=q->service;
  // Explicit selected offline/singleplayer/save option input, matching the
  // same Gear world fixture. Every effect beyond these queries is required.
  if(q->service==sk::skill_apply_online_v6||q->service==sk::skill_apply_saved_option_v6){out->word=0;return 0;}
  if(q->service==sk::skill_apply_party_count_v6){out->word=1;return 0;}
  if(q->service==sk::skill_apply_is_player_v6||q->service==sk::skill_apply_is_dead_v6){target_providers::Request24 request{q->service==sk::skill_apply_is_player_v6?target_providers::virtual_player:target_providers::virtual_dead,0,q->subject,0};std::uintptr_t result;if(query(p,&request,&result))return -1;out->word=result;return 0;}
  if(q->service==sk::skill_apply_aggro_v6){
   if(!w.retained_encounter){++w.aggro_failures;return -1;}
   sk::SkillAggroOwnerV6 owner{w.objects[1].identity,&w.enemy_outgoing};sk::SkillAggroTargetV6 target{w.objects[0].identity,&w.player_incoming};sk::SkillAggroServicesV6 services{&w,aggro};sk::SkillAggroOutputV6 applied{};std::uint32_t amount;std::memcpy(&amount,&q->number,4);
   if(sk::dh2_character_skill_aggro_v6(&applied,&owner,&target,amount,1,&services))return -1;std::memcpy(&out->number,&applied.returned_bits,4);return 0;
  }
  return -1;
 }
 static int aggro(void* p,const sk::SkillAggroRequestV6* q,std::uint32_t* out){
  if(q->service==sk::skill_aggro_target_on_aggro_v6)return -1; // Actual PlayerAIS backend unavailable.
  target_providers::Request24 request{q->service==sk::skill_aggro_owner_player_v6?target_providers::virtual_player:target_providers::virtual_dead,0,q->subject,0};std::uintptr_t answer;if(query(p,&request,&answer))return -1;*out=std::uint32_t(answer);return 0;
 }
 static int hit_service(void* p,HitActor32*,const HitRequest32* q,std::uintptr_t* out){
  auto& w=*static_cast<PopulatedWorldV6*>(p);w.required_hit=q->service;*out=0;
  if(q->service==hit_main_player){*out=w.objects[0].identity;return 0;} // Same selected offline world input.
  if(q->service==hit_online||q->service==hit_application_switch)return 0; // Named offline/application input only.
  if(q->service==hit_debug_load)return dh2_character_debug_load(w.debug->debug,w.debug->files)==1?0:-1;
  if(q->service==hit_debug_query){std::uint32_t answer;if(dh2_character_debug_get(&answer,w.debug->debug,q->name,w.debug->files)!=1)return -1;*out=answer;return 0;}
  if(q->service==hit_is_dead||q->service==hit_is_player||q->service==hit_is_character||q->service==hit_is_monster){
   w.refresh();auto i=w.index(q->subject);if(i<0)return -1;unsigned op=q->service==hit_is_dead?target_providers::is_dead:q->service==hit_is_player?target_providers::is_player:q->service==hit_is_character?target_providers::is_character:target_providers::is_monster;
   std::int32_t answer;target_providers::Services16 services{&w,query};if(sk::dh2_character_skill_target_query_v6(&answer,op,&w.characters[i],nullptr,&w.ai_types,&services))return -1;*out=std::uintptr_t(answer);return 0;
  }
  return -1; // Full Kill, trophy/audio/controller owners stay required.
 }
};
int attack_debug(void* p,const sk::SkillAttackNativeRequestV6* q,std::uintptr_t* out){auto& d=*static_cast<NativeDebug*>(p);sk::SkillManaRequestV5 request{};sk::SkillManaResponseV5 response{};
 request.subject=q->subject;request.name=q->name;
 request.service=q->service==sk::skill_attack_debug_load_v6?sk::mana_debug_load_v5:q->service==sk::skill_attack_string_construct_v6?sk::mana_debug_construct_v5:q->service==sk::skill_attack_debug_get_v6?sk::mana_debug_get_v5:q->service==sk::skill_attack_string_destroy_v6?sk::mana_debug_destroy_v5:0;
 if(!request.service||NativeDebug::mana(p,&request,&response))return -1;*out=q->service==sk::skill_attack_string_construct_v6?response.identity:response.word;return 0;
}
struct SameSaveReloadProofV6 {
 data::PlayerSavegameV1* actual{};sk::CharacterPlayerSkillsV6* player{};data::SkillTables::Borrow tables;
 data::PlayerSaveLoadOwnerV1* load_owner{};
 bool reject_selection=true;unsigned selections=0,loads=0;
 static int selected(void* p,data::PlayerSavegameV1* save,std::uintptr_t identity,const std::vector<std::int32_t>** out){auto& c=*static_cast<SameSaveReloadProofV6*>(p);++c.selections;
  check(save==c.actual&&save==c.player->native_savegame()&&identity==save->character(),"Same Save identity across native deletion");
  check(!save->skills_initialized()&&save->skills().empty()&&save->skills().data()==nullptr,"Actual old allocation released before list selection");
  if(c.reject_selection)return -1;auto id=c.player->session().property_view().resolved[28];check(id>=0&&std::size_t(id)<c.tables.lists().size());*out=&c.tables.lists()[id];return 0;
 }
 static int load(void* p,data::PlayerSavegameV1* save,std::uint32_t mask){auto& c=*static_cast<SameSaveReloadProofV6*>(p);++c.loads;
  check(save==c.actual&&save==c.player->native_savegame()&&mask==8,"Same Save reached exact source Load mask8");
  check(save->skills_initialized()&&save->skill_slots()[0].empty()&&save->skill_slots()[1].empty(),"Source slots cleared after native row reinitialization");
  for(const auto& row:save->skills())check(row.level==0&&row.flag==0&&row.reserved==0,"Exact native InitSkills rows");
  if(c.load_owner){check(&c.load_owner->save()==save,"Load owner retains SAME Save");std::string error;return c.load_owner->load(std::int32_t(mask),error)?0:-1;}
  // This is actual named-section parser input; the outer Save.Load file/backend
  // is unavailable and is still REJECTED after these real parser effects.
  const std::uint8_t empty_named_section[12]{};std::size_t consumed;std::string error;
  check(save->load_skills({empty_named_section,12},c.tables,consumed,error)==0&&consumed==12,error);return -1;
 }
};
