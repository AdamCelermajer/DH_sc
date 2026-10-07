#include "../character_player_skills_v6.hpp"
#define CharacterPlayerSkillsV3 CharacterPlayerSkillsV6
#define main existing_player_fixture_main
#include "character_player_skills_v3.cpp"
#undef main
#include "../character_skill_native_v6.hpp"
#include "../character_skill_target_queries_v6.hpp"
#include "../character_skill_aggro_v6.hpp"
#include "../character_skill_save_reload_v6.hpp"
#include "../../game-data/player_save_load_owner_v1.hpp"
#include "../player_equipment_render_owner_v1.hpp"
#include "../gameobject_lua_representation.hpp"
#include "../../script-runtime/script_return_observer_v3.h"
#include <filesystem>
#include <cctype>
using namespace dh2::player;using namespace dh2::ui;
namespace {
struct MenuPlatform {
 std::string root,assets;TextFiles text;DebugSwitches* debug{};DebugFileServices24 files{};
 CharacterGameDesign::Borrow design;std::uintptr_t identity{};unsigned world_calls{},stores{};
 static skinning::VisualAssetResultV6 asset(void* p,const char* path,Raw& out,std::string&){auto& c=*static_cast<MenuPlatform*>(p);std::string uri=path,full;if(uri.find("data/pydata/loot_table_")==0)full=c.root+"/.local-inputs/items-discovery/"+uri.substr(12);else if(uri.find("data/pydata/item_powers_")==0)full=c.root+"/.local-inputs/player-item-effects-v5/power-cache/"+uri.substr(12);else if(uri.find("data/3d/characters/prince/weapons/")==0){auto name=uri.substr(uri.find_last_of('/')+1);for(auto& x:name)x=char(std::tolower(static_cast<unsigned char>(x)));full=c.root+"/.local-inputs/visual-skin-owner-v6/weapons/"+name;}else full=c.assets+"/original-cache/"+uri;if(!std::filesystem::exists(full))return skinning::VisualAssetResultV6::missing;out=file(full);return skinning::VisualAssetResultV6::found;}
 static bool world(void* p,EquipmentWorldQueryV1 q,std::uintptr_t subject,std::uintptr_t& id,std::int32_t& scalar,std::string&){auto& c=*static_cast<MenuPlatform*>(p);check(subject==c.identity);++c.world_calls;id=0;scalar=0;if(q==EquipmentWorldQueryV1::current_player)id=subject;if(q==EquipmentWorldQueryV1::player_count)scalar=1;return true;}
 static bool constant(void* p,const char* group,const char* key,std::uint32_t& out,std::string&){auto& c=*static_cast<MenuPlatform*>(p);if(!std::strcmp(group,"StrID"))return TextFiles::constant(&c.text,group,key,out,c.error);std::int32_t v;auto* bindings=c.design.design();if(!bindings->lookup||bindings->lookup(bindings->context,0,group,key,&v))return false;std::memcpy(&out,&v,4);return true;}
 static bool text_open(void* p,const char* name,bool& found,Raw& out,std::uintptr_t& lease,std::string& e){return TextFiles::open(&static_cast<MenuPlatform*>(p)->text,name,found,out,lease,e);}
 static bool text_close(void* p,std::uintptr_t lease,std::string& e){return TextFiles::close(&static_cast<MenuPlatform*>(p)->text,lease,e);}
 static bool text_debug(void* p,const char* key,std::string&){auto& c=*static_cast<MenuPlatform*>(p);std::uint32_t v;return dh2_character_debug_get(&v,c.debug,key,&c.files)==1;}
 std::string error;
};
struct NativeDebug {
 DebugSwitches* debug;const DebugFileServices24* files;
 std::map<std::uintptr_t,std::string> strings;std::map<std::string,std::uint32_t> application_flags;
 std::uintptr_t serial=1;unsigned queries=0;
 bool fail_stats=false;
 static int mana(void* p,const sk::SkillManaRequestV5* q,sk::SkillManaResponseV5* r){
  auto& self=*static_cast<NativeDebug*>(p);
  switch(q->service){
  case sk::mana_application_v5:r->word=0;return 0; // Explicit application byte5 fixture.
  case sk::mana_player_v5:return -1; // Bypass never reached in this fixture.
  case sk::mana_debug_contains_v5:r->word=self.application_flags.find(q->name)!=self.application_flags.end();return 0;
  case sk::mana_debug_load_v5:return dh2_character_debug_load(self.debug,self.files)==1?0:-1;
  case sk::mana_debug_construct_v5:r->identity=++self.serial;self.strings[r->identity]=q->name;return 0;
  case sk::mana_debug_get_v5:{auto it=self.strings.find(q->subject);if(it==self.strings.end())return -1;++self.queries;
   if(self.fail_stats&&it->second=="isTracingChar_Stats")return -1;
   return dh2_character_debug_get(&r->word,self.debug,it->second.c_str(),self.files)==1?0:-1;}
  case sk::mana_debug_destroy_v5:return self.strings.erase(q->subject)==1?0:-1;
  default:return -1;
  }
 }
 static int target(void* p,TargetState48*,const TargetRequest24* q,std::uint32_t* out){auto& self=*static_cast<NativeDebug*>(p);
  if(q->service==target_debug_load)return dh2_character_debug_load(self.debug,self.files)==1?0:-1;
  if(q->service==target_debug_query)return dh2_character_debug_get(out,self.debug,q->text,self.files)==1?0:-1;
  return -1; // ClearTarget(null) reaches only the actual debug providers.
 }
};
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

}
int main(int argc,char** argv){try{
 check(argc==3,"Usage: skill-populated-v6 workspace original-cache.zip");const std::string root=argv[1],assets=root+"/port/android-native/app/src/main/assets",cache=root+"/.local-inputs/character-skill-session-v2/cache";Inputs raw((root+"/port/level-world/reference/character-game-design/real-cache-inputs.bin").c_str());CharacterGameDesign design;std::string error;check(design.initialize(raw.input,error),error);auto d=design.borrow();data::SkillTables skills;data::FaeryTables faeries;auto load=[&](auto& owner,const std::string& base){auto a=file(base+"_pyarray.bin"),b=file(base+"_pyarraynames.bin"),c=file(base+"_pystructnames.bin");check(owner.load(bytes(a),bytes(b),bytes(c),error),error);};load(skills,assets+"/data/skills");load(faeries,cache+"/data/pydata/faeries");auto sb=skills.borrow();auto cache_owner=script_assets(argv[2],skills.borrow());auto full_cache=cache_owner.borrow();check(full_cache.files().size()==219);
Debug debug_files;DebugFileServices24 files{&debug_files,Debug::open,Debug::close};auto* debug=dh2_character_debug_create();check(debug);std::unique_ptr<DebugSwitches,decltype(&dh2_character_debug_destroy)> debug_owner(debug,dh2_character_debug_destroy);auto* modules=dh2_fx_debug_modules_create(debug,&files);check(modules);std::unique_ptr<fx::DebugModules,decltype(&dh2_fx_debug_modules_destroy)> module_owner(modules,dh2_fx_debug_modules_destroy);fx::PreloadServices16 debug_services{modules,dh2_fx_debug_preload_service};Vitals vitals{&debug_services};sk::PlayerSkillInitServicesV3 init{&vitals,Vitals::invoke};
 skinning::VisualSkinResourcesV6 resources;check(resources.load(file(assets+"/models/prince_modular.bdae"),error),error);unsigned classes=0,checks_completed=0,pre_failures=0,use_failures=0,positive_native_results=0,native_hp_prefixes=0;
std::ostringstream prefixes;
const char* names[]{"KnightPlayerBase","MagePlayerBase","RoguePlayerBase"};
const char* scripts[]{"prince_warrior_bashdown","prince_mage_coldray","prince_rogue_jump_kick"};
for(unsigned which=0;which<3;++which){
 Unit unit;auto identity=std::uintptr_t(0xabcdef0600000001ULL+which*2);
 unit.fsm_state.current=3;unit.fsm={&unit.fsm_state,identity,1,0};
 unit.event_owner={identity,identity+1,reinterpret_cast<std::uintptr_t>(&unit.fsm_state),identity+2,0,0,0,0};unit.ai_keys[0x90/4]=0x3d0ca0;unit.ais_keys[0x90/4]=0x3dcc80;unit.events={identity+3,&unit.event_owner,unit.ai_keys.data(),0,unit.ais_keys.data(),0,0,0,0,0,0};
 const auto at=std::find(d.characters()->names.begin(),d.characters()->names.end(),names[which]);check(at!=d.characters()->names.end());
 CharacterScriptSessionInputV3 in;in.identity=identity;in.name="Player";in.source_is_character=1;in.state_machine=&unit.fsm;
 in.properties=std::make_shared<data::PropertyState>();in.combat=std::make_shared<data::CombatActorState>();in.temporary=std::make_shared<data::PropertySheet>();
 data::reset_properties(*d.rules(),*in.properties,&d.characters()->rows[at-d.characters()->names.begin()]);check(data::recalc_properties_with_class(*d.classes(),*d.rules(),*in.properties,error),error);
 unit.saved=std::make_shared<data::PlayerSavegameV1>();unit.saved->set_character(identity);const auto list=in.properties->resolved[28];check(list>=0&&std::size_t(list)<sb.lists().size());check(unit.saved->initialize_skills(sb.lists()[list],error),error);unit.saved->initialize_faeries();in.savegame=unit.saved;
 unit.scripts=full_cache;in.common=full_cache.common();in.cached_file_context=&unit;in.cached_file=Unit::cached;
 MenuPlatform platform;platform.root=root;platform.assets=assets;platform.identity=identity;platform.debug=debug;platform.files=files;platform.design=design.borrow();platform.text.assets=assets;
 auto constants=file(assets+"/original-cache/data/pydata/common_text_pycst.bin");dh2_script_constants_reload receipt{};check(!dh2_script_constants_load(platform.text.constants,constants.data(),constants.size(),&receipt));
 HudTextEnvironmentV1 env{{&platform,MenuPlatform::text_open,MenuPlatform::text_close,MenuPlatform::text_debug,MenuPlatform::constant,nullptr,nullptr}};
 auto scene=resources.borrow().factory_scene();data::LootRandom8V2 random{1,0};PlayerEquipmentRenderInputsV1 gear_input;gear_input.design=design.borrow();gear_input.properties=in.properties;gear_input.random=&random;gear_input.character=identity;gear_input.potion_capacity=12;gear_input.language_pack=0;gear_input.resources=resources.borrow();gear_input.live_scene=&scene;gear_input.assets={&platform,MenuPlatform::asset};gear_input.debug=debug;gear_input.debug_files=files;gear_input.text_environment=env;gear_input.world={&platform,MenuPlatform::world};
 PlayerEquipmentRenderOwnerV1 gear(std::move(gear_input));check(gear.initialize(error),error);check(gear.inventory()->equipment()[0][1],"Actual initial-grant equipped main hand");
 PopulatedWorldV6 world;world.design=design.borrow();world.gear=&gear;
 world.enemy_properties=std::make_shared<data::PropertyState>();bool enemy_found=false;
 for(std::size_t j=0;j<d.characters()->rows.size();++j){data::reset_properties(*d.rules(),*world.enemy_properties,&d.characters()->rows[j]);if(!data::recalc_properties_with_class(*d.classes(),*d.rules(),*world.enemy_properties,error))continue;
  auto* ai=data::ai_props(*d.ai(),world.enemy_properties->resolved[1]);if(ai&&ai->type==4&&data::ai_enemy(*d.ai(),in.properties->resolved[0],world.enemy_properties->resolved[0],true,false)){enemy_found=true;break;}}
 check(enemy_found,"Actual hostile monster character row");world.enemy_view=data::property_view(*d.rules(),*world.enemy_properties);
 for(const auto& ai:d.ai()->rows)world.types.push_back(ai.type);world.ai_types={world.types.data(),std::uint32_t(world.types.size()),0};
 for(unsigned i=0;i<2;++i){world.objects[i].identity=identity+i;world.objects[i].visible=1;world.characters[i]={identity+i,i?world.enemy_view.resolved:in.properties->resolved.data(),i?"NativeEnemy":"Player",0x2000,0,0,1,1};world.handles[i]={std::int32_t(i+1),0,0};world.records[i]={std::int32_t(i+1),0,identity+i};}
 world.objects[1].position[1]=-100;world.entry={&world.entry_sentinel,&world.objects[1]};world.entry_sentinel={&world.entry,nullptr};world.room={&world.sentinel,&world.entry_sentinel};world.sentinel={&world.room,nullptr};world.search_registry={&world.sentinel};world.handles_registry={world.records,2,2,77,0};
 NativeDebug native_debug{debug,&files};world.debug=&native_debug;sk::SkillManaServicesV5 mana_services{&native_debug,NativeDebug::mana};sk::SkillAttackNativeServicesV6 attack_services{&native_debug,attack_debug};
 TargetOwner16 target_owner{identity,0,0,0};TargetState48 target{identity+4,&target_owner,0,0,0,0,0,0,0,0};TargetBindings48 target_bindings{&target,{&native_debug,NativeDebug::target},nullptr,{0,0}};in.target=&target_bindings;
 dh2_script_object_services objects{&world,PopulatedWorldV6::type,PopulatedWorldV6::methods,PopulatedWorldV6::method};in.objects=&objects;
 sk::SkillNativeWorldV5 live{&world.objects[0],&world.search_registry,{&world,PopulatedWorldV6::search},&world,nullptr,PopulatedWorldV6::resolve,&objects};
 sk::CharacterSkillNativeReadOnlyBindingsV6 readonly(*gear.inventory(),unit.fsm,mana_services,live);
 sk::SkillCombatWorldV6 combat_world{&world,PopulatedWorldV6::handle,{&world,PopulatedWorldV6::query},PopulatedWorldV6::actor,nullptr};
 DotCombatContext32 combat_context{};data::CombatRandom combat_random{1,0};sk::SkillApplyServicesV6 applications{&world,PopulatedWorldV6::application,&attack_services};
 sk::CharacterSkillNativeBindingsV6 native(readonly,*gear.inventory(),unit.fsm,sb,combat_world,combat_context,combat_random,attack_services,applications);
 in.gameplay_context=&native;in.gameplay_binding=sk::CharacterSkillNativeBindingsV6::binding;
 auto player_init=init;player_init.gameplay.ai=&unit.events;unit.skill_owner={identity,0,0};player_init.gameplay.skill_owner=&unit.skill_owner;player_init.gameplay.difficulty_context=&unit;player_init.gameplay.difficulty=Unit::selected;
 unit.player=sk::CharacterPlayerSkillsV6::create(design.borrow(),in,sb,faeries.borrow(),debug_services,unit.fsm,player_init,error);check(bool(unit.player),error);world.player=unit.player.get();
 check(!native.attach(unit.player->session()),native.error());check(unit.player->initialize(1)==1,unit.player->error());check(!readonly.install_object_binding(),readonly.error());world.refresh();
 check(!sk::dh2_character_skill_regen_v6(&world.enemy_view,0,-1,&attack_services),"Native retained enemy spawn HP refresh");check(world.enemy_view.resolved[36]>0,"Actual hostile row maximum HP");
 auto& player=*unit.player;check(player.session().properties()==gear.properties()&&player.native_buffs(),"Same V6/equipment/property/buff owner");
 auto& rows=player.state().skills;auto found=std::find_if(rows.items,rows.items+rows.count,[&](auto* row){return row&&!std::strcmp(row->script,scripts[which]);});check(found!=rows.items+rows.count);auto index=unsigned(found-rows.items);
 check(unit.saved->set_skill_level(index,1,error),error);check(player.update()==1,player.error());check(!dh2_property_set(&player.session().property_view(),41,100000));
 std::uint32_t answer=0;check(!player.callback(index,sk::skill_check_usable_v3,&answer)&&answer==1,player.error());++checks_completed;
 const auto mana_before=in.properties->resolved[41];auto pre=player.callback(index,sk::skill_pre_v3,&answer);check(pre!=0,"Populated Pre must fail at genuine missing Cmd_LookAt");check(readonly.targets().count==1&&readonly.targets().heap[0].identity==world.objects[1].identity,"Retained native populated target prefix: "+std::to_string(readonly.targets().count)+"; "+readonly.error()+"; "+player.error());check(in.properties->resolved[41]==mana_before,"LookAt failure precedes source UseMana");++pre_failures;
 // The source has already cached target / search backup before LookAt failed.
 // Separate actual Use delivery proves native calculation and required effect
 // failure; this is not a claim of successful whole Pre→Use gameplay.
 auto use=player.callback(index,sk::skill_use_v3,&answer);check(use!=0,"Required native target application effect failure");check(world.required_application!=0&&world.latest.mask!=0&&combat_context.attacker==identity,"Source authored Use reached actual V6 calculation/application");check(combat_random.calls>0,"Actual shared combat RNG advanced");++use_failures;
 // Independently calculate the authored current row over the exact same graph.
 const auto source_row=sb.lists()[list][index];auto& skill=sb.skills()[source_row];world.refresh();data::CombatResult calculated{};
 check(!sk::dh2_character_skill_attack_calculate_v6(&calculated,&combat_context,&combat_random,&world.attacks[0],&world.attacks[1],gear.inventory(),skill.scalar.words[7],std::int32_t(skill.scalar.words[6]),&attack_services),"Actual equipped same-owner native calculation");
 check(calculated.amount>0,"Positive authored native result");++positive_native_results;
 // Retained encounter relation is an explicit initial storage fixture, not
 // an accepted acquisition/PlayerAIS OnAggro callback. Subsequent AddAggro and
 // reciprocal relation changes execute the ordered native V6 owner operation.
 const float previous_threat=2.0f;std::uint32_t threat_bits;std::memcpy(&threat_bits,&previous_threat,4);
 world.enemy_outgoing.entries[0]={world.objects[0].identity,threat_bits,0};world.enemy_outgoing.count=1;
 world.player_incoming.entries[0]={world.objects[1].identity,threat_bits,0};world.player_incoming.count=1;world.retained_encounter=true;
 const auto hp_before=world.enemy_view.resolved[36];bool native_hp=false;
 for(unsigned attempt=0;attempt<16&&!native_hp;++attempt){check(player.callback(index,sk::skill_use_v3,&answer)!=0,"Required application tail remains unavailable");native_hp=world.enemy_view.resolved[36]<hp_before;}
 check(native_hp,"Actual authored Use/AddAggro/HitFor HP effect prefix");++native_hp_prefixes;
 check(world.enemy_outgoing.entries[0].threat_bits==world.player_incoming.entries[0].threat_bits,"Same native reciprocal relation value");
 if(which)prefixes<<',';prefixes<<"{\"script\":\""<<scripts[which]<<"\",\"hp_before\":"<<hp_before<<",\"hp_after\":"<<world.enemy_view.resolved[36]<<",\"required_hit_service\":"<<world.required_hit<<",\"required_application_service\":"<<world.required_application<<'}';
 check(player.session().property_view().resolved==gear.property_view()->resolved&&world.attacks[1].properties->resolved==world.enemy_properties->resolved.data(),"No scratch property copies");
 check(native_debug.strings.empty(),"All actual owned debug strings released");
 SameSaveReloadProofV6 save_proof{unit.saved.get(),&player,sb};sk::SkillSaveReloadServicesV6 reload_services{&save_proof,SameSaveReloadProofV6::selected,SameSaveReloadProofV6::load};sk::SkillSaveReloadOutputV6 reload{};
 data::SavedSkillUpdateServicesV1 update_service{&player,[](void* p,std::uintptr_t id,std::string&){auto& p1=*static_cast<sk::CharacterPlayerSkillsV6*>(p);return p1.session().timers().owner==id&&p1.update()==1;}};
 check(unit.saved->set_skill_in_slot(0,index,update_service,error),error);check(unit.saved->has_skill_slots());
 check(sk::dh2_character_skill_save_reload_v6(&reload,unit.saved.get(),&reload_services)==-2&&reload.phase==2&&reload.deleted,"Required selected-list failure retains delete/null prefix");
 check(unit.saved->has_skill_slots(),"Source map clear not reached on list selection failure");
 save_proof.reject_selection=false;
 check(sk::dh2_character_skill_save_reload_v6(&reload,unit.saved.get(),&reload_services)==-2&&reload.phase==6&&!reload.deleted,"Required actual outer Save.Load remains explicit");
 check(save_proof.selections==2&&save_proof.loads==1&&unit.saved->skills().size()==sb.lists()[list].size()&&!unit.saved->has_skill_slots(),"Same Save native reinitialization and source slot clear prefix");
 check(player.native_savegame()==unit.saved.get()&&in.savegame==unit.saved,"Retained original Session/current skill save authority unchanged");
 const auto spell_count=player.state().spells.count;
 save_proof.reject_selection=true;
 check(player.native_reload_skills(&reload_services)!=1,"Whole same-owner reload retains required selection failure");
 check(player.state().skills.count==0&&player.state().spells.count==spell_count,"Actual skill destruction/reset precedes same Save selection; spells retained");
 check(player.native_savegame()==unit.saved.get()&&save_proof.selections==3,"Whole reload retains original Save identity");
 data::PlayerSaveLoadOwnerV1 actual_load(unit.saved);save_proof.load_owner=&actual_load;save_proof.reject_selection=false;
 check(&actual_load.save()==player.native_savegame()&&!actual_load.profile().identity&&unit.saved->slot()==-1,"Genuine same Save constructor-null profile and fresh slot");
 check(player.native_reload_skills(&reload_services)==1,player.error());
 check(actual_load.delivered_calls()==0&&!actual_load.profile().identity&&player.state().skills.count==sb.lists()[list].size(),"Whole native reload succeeds through genuine mask8 null-profile guard and configure/update");
 check(player.state().spells.count==spell_count&&player.native_savegame()==unit.saved.get(),"Whole positive reload retains spells and original Save authority");
 auto retained_profile=std::make_shared<int>(1);check(actual_load.publish_profile({identity+0x100,retained_profile},error),error);
 check(player.native_reload_skills(&reload_services)!=1,"Nonnull retained profile requires actual section backend despite slot -1");
 check(actual_load.profile().identity==identity+0x100&&actual_load.delivered_calls()==1&&player.state().skills.count==0,"Required nonnull section failure preserves real reload prefix before configure");
 check(&actual_load.save()==player.native_savegame()&&unit.saved->skills_initialized(),"Nonnull failure retains SAME Save reinitialization");
 player.session().close();++classes;
}
std::cout<<"{\"validation\":\"PASS\",\"same_owner_classes\":"<<classes<<",\"actual_starter_checks\":"<<checks_completed<<",\"populated_look_at_failure_prefixes\":"<<pre_failures<<",\"actual_use_required_application_failures\":"<<use_failures<<",\"positive_same_owner_native_results\":"<<positive_native_results<<",\"native_hp_effect_prefixes\":"<<native_hp_prefixes<<",\"required_prefixes\":["<<prefixes.str()<<"],\"no_alias_fixture\":true,\"scratch_property_copies\":false,\"explicit_offline_world_geometry_input\":true,\"explicit_retained_encounter_relation_input\":true,\"full_target_application\":false,\"full_campaign\":false,\"checks\":"<<checks<<"}\n";return 0;

}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
#undef CharacterPlayerSkillsV3
