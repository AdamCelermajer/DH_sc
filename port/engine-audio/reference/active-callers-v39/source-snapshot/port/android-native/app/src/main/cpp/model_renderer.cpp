#include "renderer_private_save_read_v39.hpp"
#include "model_renderer.hpp"
#include "../level-loader/native_level_application_v25.hpp"
#include "retained_visual_draw_v27.hpp"
#include "owned_equipment_queries_v4.hpp"
#include "character_panel_runtime_v3.hpp"
#include "character_menu_mutations_v4.hpp"
#include "scene.hpp"
#include "textures.hpp"
#include "animation.hpp"
#include "skinning.hpp"
#include "visual_skin_owner_v6.hpp"
#include "player_equipment_render_owner_v1.hpp"
#include "player_equipment_queries_v1.hpp"
#include "world.hpp"
#include "objects.hpp"
#include "animation_tables.hpp"
#include "animation_scheduler.hpp"
#include "animation_bank.hpp"
#include "class_tables.hpp"
#include "class_preview_setup.hpp"
#include "loot_tables_v2.hpp"
#include "properties.hpp"
#include "vitals.hpp"
#include "combat_events.hpp"
#include "combat_result.hpp"
#include "health.hpp"
#include "combat_application.hpp"
#include "ai.hpp"
#include "aggro.hpp"
#include "navigation_objects.hpp"
#include "navigation_avoidance.hpp"
#include "navigation_producers.hpp"
#include "navigation_heading.hpp"
#include "actor_runtime.hpp"
#include "actor_blended_playback.hpp"
#include "physical_world.hpp"
#include "character_scene.hpp"
#include "character_state.hpp"
#include "character_timers.hpp"
#include "character_ais_death_vm_v2.hpp"
#include "npc_death_owner_v2.hpp"
#include "npc_dead_state_v1.hpp"
#include "character_skills_session.hpp"
#include "character_menu_recalc_owner_v1.hpp"
#include "character_stance.hpp"
#include "character_controller_commands.hpp"
#include "character_path_commands.hpp"
#include "character_game_design.hpp"
#include "design_settings.hpp"
#include "skill_tables.hpp"
#include "character_sneaking_tables.hpp"
#include "actor_initialization.hpp"
#include "character_animation_instance.hpp"
#include "retained_character_actor_v1.hpp"
#include "canonical_existing_actor_publication_v3.hpp"
#include "canonical_property_map_v1.hpp"
#include "canonical_point3d_globals_v1.hpp"
#include "canonical_player_facet_v3.hpp"
#include "player_spawn_metadata_v4.hpp"
#include "player_manager_combat_runtime_v2.hpp"
#include "player_network_local_owner_v4.hpp"
#include "character_world_player_aggro_v2.hpp"
#include "level_config_music_owner_v1.hpp"
#include "visual_fx_tables.hpp"
#include "player_gameplay_binding.hpp"
#include "character_panel_session_v1.hpp"
#include "gameplay_hud.hpp"
#include "character_script_init_vitals.hpp"
#include "character_skill_state_v4.hpp"
#include "character_world_target_pose_v2.hpp"
#include "character_world_target_frame_v2.hpp"
#include "character_path_commands.hpp"
#include "player_target_out_range_v2.hpp"
#include "character_target_events.hpp"
#include "player_target_died_v2.hpp"
#include "character_state_owner_behavior.hpp"
#include "character_world_runtime_v1.hpp"
#include "world_click_fields_owner_v1.hpp"
#include "world_click_application_v1.hpp"
#include "world_touch_projection_v1.hpp"
#include "character_combat_fx_runtime_v4.hpp"
#include "character_target_marker_v28.hpp"
#include "character_authored_fx_forces_v4.hpp"
#include "character_authored_resource_v6.hpp"
#include "authored_fx_geometry_packet_v7.hpp"
#include "character_animation_step_fx_v2.hpp"
#include "character_animation_swoosh_v4.hpp"
#include "audio_animation_swoosh_v38.hpp"
#include "audio_world_producer_v38.hpp"
#include "audio_named_animation_sound_v38.hpp"
#include "audio_target_events_v38.hpp"
#include "canonical_point3d_globals_v1.hpp"
#include "object_manager_language_registry_v1.hpp"
#include "character_language_scene_connection_v1.hpp"
#include "authored_character_application_v1.hpp"
#include "settings_native_files_v1.hpp"
#include "character_combat_results_ai_v1.hpp"
#include "character_blood_fx_resource_v2.hpp"
#include "character_authored_particle_fx_v3.hpp"
#include "character_fx_floor_query_v3.hpp"
#include "character_fx_floor_sync_v29.hpp"
#include "renderer_effect_submission_v34.hpp"
#include "blood_render_pass_v3.hpp"
#include "effect_render_pass_v4.hpp"
#include "shader_program_collection_v4.hpp"
#include "effect_material_directory_v4.hpp"
#include "math.hpp"
#include "transparent_sort_v2.hpp"
#include "texture_owner_v1.hpp"
#include "texture_binding_owner_v1.hpp"
#include "texture_mipmap_v1.hpp"
#include "texture_unbind_v1.hpp"
#include "texture_driver_fields_v1.hpp"
#include "blood_texture_image_v1.hpp"
#include "general_fx_texture_image_v2.hpp"
#include "character_combat_sound_tables_v2.hpp"
#include "vox_play3d_owner_v2.hpp"
#include "shader_sources.hpp"
#include "floors.hpp"
#include "character_world_npc_state_owner_v1.hpp"
#include "character_world_npc_initialization_v1.hpp"
#include "trophy_manager_owner_v1.hpp"
#include "character_world_skill_execution_v6.hpp"
#include "faery_gameplay_v1.hpp"
#include "faery_cast_owner_v2.hpp"
#include "character_menu_faery_connection_v8.hpp"
#include "level_faery_placement_v8.hpp"
#include "player_ai_callables_v1.hpp"
#include "player_timer_helpers_v1.hpp"
#include "player_recurring_effects_v7.hpp"
#include "character_animation_event_owner_v1.hpp"
#include "character_attack_animation_v1.hpp"
#include "character_world_player_attack_owner_v1.hpp"
#include "character_heading_owner_v1.hpp"
#include "character_collision_lifecycle_v1.hpp"
#include "native_physical_filter_v1.hpp"
#include "character_world_attack_geometry_v1.hpp"
#include "npc_inventory_owner_v1.hpp"
#include "character_targetability_owner_v1.hpp"
#include "npc_injury_runtime_v1.hpp"
#include "npc_script_commands_v1.hpp"
#include "canonical_point3d_globals_v1.hpp"
#include "npc_controller_look_v1.hpp"
#include "character_ai_state_changed.hpp"
#include "character_ai_state_changed_vm.hpp"
#include "character_melee_animation_event_v1.hpp"
#include "character_idle_update.hpp"
#include "enemy_status_hud_v1.hpp"
#include "character_combat_follower_v1.hpp"
#include "character_world_npc_bounds_v1.hpp"
#include "combat_flash_inputs_v1.hpp"
#include "player_equipment_queries_v1.hpp"
#include "owned_hud_settings_v1.hpp"
#include "../asset-payloads/sha256.hpp"
#include "character_host_context.hpp"
#include "character_design_services.hpp"
#include "character_script_session.hpp"
#include "character_script_assets_v1.hpp"
#include "original_cache_assets_v1.hpp"
#include "gameplay_camera_application_v23.hpp"
#include "gameplay_camera_anchor_v6.hpp"
#include "authored_camera_basis_v22.hpp"
#include "selected_world_camera_config_v20.hpp"
#include "character_script_objects.hpp"
#include "navigation_producers.hpp"
#include <GLES2/gl2.h>
#include <android/log.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdio>
#include <map>
#include <set>
#include <functional>
#include <stdexcept>
#include <vector>
#include <chrono>
#include <cctype>
#include <cstring>
#include <memory>
#include "frame_perf_v35.hpp"
#include "native_resource_budget_v38.hpp"
#include "world_visibility_v35.hpp"
#include "skin_pose_cache_v32.hpp"
#include <cerrno>
#include "renderer_world_touch_live_v2.inc"

namespace model_renderer {
namespace {
using Matrix=std::array<float,16>;
using Vertex=dh2::objects::Vertex;
struct Draw{GLuint vertices=0,indices=0,diffuse=0,alpha=0;GLsizei count=0;unsigned node=0;dh2::scene::Material material;
 dh2::skinning::Skin skin;std::vector<Vertex> cpu_vertices;std::vector<std::array<float,3>> rest_positions;
 bool environment=false;Matrix placement{};dh2::render::BoundsV35 clip_bounds;
 const void* pose_owner{};std::uint64_t uploaded_pose_revision{};};
std::vector<Draw> draws;std::vector<GLuint> images;GLuint program=0;
Matrix submitted_camera{};int submitted_width{},submitted_height{};
Matrix submitted_view{};std::array<float,3> submitted_eye{};
CombatTextSinkV1 combat_text_sink{};
std::uint32_t current_application_dt{};bool current_application_tick{};
void apply_combat_text_v1(const dh2::data::CombatResult&,std::uintptr_t,std::uintptr_t);
void clear_combat_fx_gpu(bool discard_context);
void clear_retained_loot_gpu_v27(bool discard_context);
void draw_combat_fx_v4();
void begin_gameplay_camera_release_v20();
void finish_gameplay_camera_release_v20();
void initialize_gameplay_camera_v20(const std::string&);
void prepare_gameplay_camera_application_v27();
void flush_gameplay_animation_sets_v27();
Matrix submit_gameplay_camera_v20(int,int);
bool gameplay_camera_ray_v20(std::int32_t,std::int32_t,std::array<float,3>&,std::array<float,3>&,std::string&);
bool fill_combat_text_frame_v1(CombatTextFrameV1&,std::string&);
// Equipment has its own GL lifetime. Retained source parts keep geometry and
// material backing alive through a whole frame and through equipment changes.
std::vector<Draw> equipment_draws;
std::vector<GLuint> equipment_images;
std::vector<dh2::skinning::VisualDrawViewV32> equipment_parts;
std::vector<std::size_t> equipment_draw_part;
struct AggroStorage {
 std::vector<dh2::data::AggroEntry> outgoing,incoming;
 dh2::data::AggroTable outgoing_table{},incoming_table{};
 void rebind(){outgoing_table.entries=outgoing.data();outgoing_table.capacity=unsigned(outgoing.size());incoming_table.entries=incoming.data();incoming_table.capacity=unsigned(incoming.size());}
 AggroStorage()=default;
 AggroStorage(const AggroStorage& other):outgoing(other.outgoing),incoming(other.incoming),outgoing_table(other.outgoing_table),incoming_table(other.incoming_table){rebind();}
 AggroStorage& operator=(const AggroStorage& other){if(this!=&other){outgoing=other.outgoing;incoming=other.incoming;outgoing_table=other.outgoing_table;incoming_table=other.incoming_table;rebind();}return *this;}
 void initialize(unsigned capacity){outgoing.resize(capacity);incoming.resize(capacity);outgoing_table.count=incoming_table.count=0;rebind();}
};
struct MonsterScriptHandle;
struct ObjectActor:dh2::objects::Record {
 dh2::data::AnimationScheduler scheduler;double cursor=0;unsigned completions=0;std::string state="Idle";
 dh2::data::PropertySheet base_class{};int class_id=-1;
 // Gameplay backing survives vector movement and GL resource recreation.
 std::shared_ptr<dh2::data::PropertyState> property_owner=std::make_shared<dh2::data::PropertyState>();
 dh2::data::PropertyState& properties(){return *property_owner;}
 const dh2::data::PropertyState& properties() const{return *property_owner;}
 dh2::animation::EventCursor event_cursor;unsigned animation_events=0;
 std::shared_ptr<dh2::data::CombatActorState> combat_owner=std::make_shared<dh2::data::CombatActorState>();
 dh2::data::CombatActorState& combat_state(){return *combat_owner;}
 const dh2::data::CombatActorState& combat_state() const{return *combat_owner;}
 int combat_target=-1;bool pending_death=false;
 AggroStorage aggro;std::uint64_t identity=0;unsigned target_alive=0,target_sight=0;bool target_seeking=false,ai_attack=false;
 std::shared_ptr<MonsterScriptHandle> script;
  std::vector<std::shared_ptr<dh2::skinning::SkinPoseCacheV32>> pose_caches_v35;
 ObjectActor(const dh2::objects::Record& r):Record(r){}
};
struct ObjectGroup{dh2::objects::Resource resource;std::vector<Draw> draws;std::vector<ObjectActor> instances;std::map<int,dh2::animation::Player> clips;int animation_table=-1;};
std::uint64_t snapshot_checksum(const dh2::data::PropertySheet& sheet){
 std::uint64_t result=14695981039346656037ull;for(auto value:sheet)for(unsigned i=0;i<4;++i){result^=(std::uint32_t(value)>>(8*i))&255;result*=1099511628211ull;}return result;
}
std::vector<ObjectGroup> object_groups;
std::vector<ObjectActor> saved_actors;
struct PlayerCombat {
 std::shared_ptr<dh2::data::PropertyState> property_owner=std::make_shared<dh2::data::PropertyState>();
 std::shared_ptr<dh2::data::CombatActorState> life_owner=std::make_shared<dh2::data::CombatActorState>();
 dh2::data::PropertyState& properties(){return *property_owner;}
 const dh2::data::PropertyState& properties() const{return *property_owner;}
 dh2::data::CombatActorState& life(){return *life_owner;}
 const dh2::data::CombatActorState& life() const{return *life_owner;}
 int animation_table=-1,target=-1;unsigned attempts=0,received=0;
 bool pending_death=false;std::uint64_t death_target=0;
 AggroStorage aggro;
};
PlayerCombat prince_combat;
struct PlayerCanonicalMetadataLiveV4;
struct RendererCanonicalWorldV4;
struct RendererCharacterApplicationV4;
struct RendererPlayerManagerBootV25;
struct WorldScriptContext {
 std::shared_ptr<RendererPlayerManagerBootV25> player_manager_boot_v25;
 std::shared_ptr<dh2::loader::NativeLevelConnectionV25> native_level_c1_v25;
 std::shared_ptr<dh2::loader::NativeGSLevelRuntimeV27> native_gslevel_v27;
 std::shared_ptr<PlayerCanonicalMetadataLiveV4> player_canonical;
 std::shared_ptr<dh2::character::PlayerFaeryAssociationV8> player_faery_v8;
 std::shared_ptr<RendererCanonicalWorldV4> canonical_world;
 std::weak_ptr<RendererCharacterApplicationV4> character_application;
 // Original Level constructor+130. The original _LoadProcess stage producer
 // is not connected yet; world readiness must not manufacture its phase38.
 std::int32_t source_level_load_phase{};
 std::shared_ptr<dh2::character::CharacterGameDesign> design;
 dh2::character::CharacterGameDesign::Borrow tables;
 std::shared_ptr<dh2::data::DesignSettingsOwner> settings_owner;
 dh2::data::DesignSettingsOwner::Borrow settings;
 const std::uint32_t* enemy_spotted_aggro=nullptr;
  std::shared_ptr<dh2::data::SkillTables> skills_owner;
  dh2::ui::GameOptionTableV1 game_options;
  std::unique_ptr<dh2::ui::OwnedHudSettingsV1> saved_options;
 dh2::character::CharacterScriptAssetsV1 script_assets;
 dh2::character::CharacterScriptAssetsV1::Borrow script_files;
 std::unique_ptr<dh2::character::sneaking::SneakingTables> skills;
 dh2::character::ActorInitialization initialization;
 dh2::assets::Sha256Digest initialization_asset_sha{};
 bool initialization_loaded=false;
 std::map<std::string,std::shared_ptr<const dh2::character::CharacterAnimationResources>> monster_resources;
 std::shared_ptr<dh2::data::EffectsTables> effects_owner;
 std::unique_ptr<dh2::fx::PreloadBacking> effects;
 std::vector<std::int32_t> effect_queue_storage;
 dh2::fx::PreloadQueue16 effect_queue{};
 dh2::character::HostPlayer8 host_player{};
 dh2::character::HostLevel8 host_level{-1,0};
 std::vector<dh2::character::LevelRangeRow24> ranges;
 dh2::character::HostContextBindings16 host{{this,host_service}};
 std::string files_directory;
 struct DebugDelete {void operator()(dh2::character::DebugSwitches* value) const{dh2_character_debug_destroy(value);}};
 std::unique_ptr<dh2::character::DebugSwitches,DebugDelete> debug{dh2_character_debug_create()};
 dh2::character::DebugFileServices24 debug_files{this,open_debug,close_debug};
 dh2::character::DebugLevelBinding16 debug_binding{debug.get(),&debug_files};
 dh2::character::LevelServices16 level_services{&debug_binding,dh2_character_debug_level_service};
 struct FxModulesDelete {void operator()(dh2::fx::DebugModules* value)const{dh2_fx_debug_modules_destroy(value);}};
 std::unique_ptr<dh2::fx::DebugModules,FxModulesDelete> effect_modules;
 dh2::fx::PreloadServices16 effect_services{};
 std::unique_ptr<dh2::character::CharacterScriptObjects> objects;
  std::shared_ptr<dh2::character::ScriptCharacterObject> player_object;
  std::shared_ptr<dh2::camera::CameraWorldSessionV23> camera_session_v23;
  std::unique_ptr<dh2::world::SelectedWorldCameraConfigV20> camera_config_v20;
 explicit WorldScriptContext(std::shared_ptr<dh2::character::CharacterGameDesign> owner,
  std::shared_ptr<dh2::data::DesignSettingsOwner> authored,
  std::shared_ptr<dh2::data::SkillTables> skill_owner,
  std::shared_ptr<dh2::data::EffectsTables> effect_owner,std::string directory):
  design(std::move(owner)),tables(design->borrow()),settings_owner(std::move(authored)),
  settings(settings_owner->borrow()),skills_owner(std::move(skill_owner)),
  skills(std::make_unique<dh2::character::sneaking::SneakingTables>(skills_owner->borrow())),
  effects_owner(std::move(effect_owner)),files_directory(std::move(directory)){
  if(!debug||files_directory.empty()||files_directory.front()!='/')throw std::runtime_error("Private game files unavailable");
  std::string error;
  effects=dh2::fx::PreloadBacking::create(effects_owner->borrow(),error);
  if(!effects)throw std::runtime_error("Owned effects preload backing unavailable: "+error);
  effect_queue_storage.resize(effects->table().effect_count);
  effect_queue={effect_queue_storage.data(),0,static_cast<std::uint32_t>(effect_queue_storage.size())};
  effect_modules.reset(dh2_fx_debug_modules_create(debug.get(),&debug_files));
  if(!effect_modules)throw std::runtime_error("Owned effects debug module unavailable");
  effect_services={effect_modules.get(),dh2_fx_debug_preload_service};
  const auto* levels=tables.levels();
  if(!levels||levels->level_names.size()!=levels->levels.size())throw std::runtime_error("Level owner unavailable");
  ranges.resize(levels->levels.size());
  for(unsigned i=0;i<ranges.size();++i){
   std::memcpy(ranges[i].maximum,levels->levels[i].scalar.words+12,12);
   std::memcpy(ranges[i].minimum,levels->levels[i].scalar.words+15,12);
  }
  const auto found=std::find(levels->level_names.begin(),levels->level_names.end(),"GOTHICUS_CRYPT_01");
  if(found==levels->level_names.end())throw std::runtime_error("Original Crypt level row missing");
  host_level.row_index=found-levels->level_names.begin();
  objects=std::make_unique<dh2::character::CharacterScriptObjects>(design->borrow(),debug.get(),&debug_files);
  const auto default_row=settings.row_index("Default");
  if(default_row<0||settings.field_index("EnemySpottedAggro")!=11||
     !(enemy_spotted_aggro=settings.enemy_spotted_aggro_bits(default_row)))
   throw std::runtime_error("Authored enemy detection setting unavailable");
  // The current prototype selects Crypt01/tier0 explicitly. Original menu,
  // savegame and Application selection remain separate required producers.
 }
 static int open_debug(void* pointer,const char* name,std::uintptr_t* handle){
  auto& context=*static_cast<WorldScriptContext*>(pointer);
  if(!name||std::strcmp(name,"DebugSwitches.savegame")||!handle)return 1;
  *handle=0;errno=0;
  const auto path=context.files_directory+"/"+name;
  auto* file=std::fopen(path.c_str(),"rb");
  if(!file){const int reason=errno;__android_log_print(ANDROID_LOG_INFO,"DH2Native","Debug switches file | opened 0 | errno %d | app-private directory",reason);return reason==ENOENT?0:1;}
  *handle=reinterpret_cast<std::uintptr_t>(file);return 0;
 }
 static int close_debug(void*,std::uintptr_t handle){return !handle||std::fclose(reinterpret_cast<std::FILE*>(handle))?1:0;}
 static int host_service(void* pointer,const dh2::character::HostContextRequest16* request,dh2::character::HostContextResponse16* response){
  auto& context=*static_cast<WorldScriptContext*>(pointer);
  switch(request->service){
   case dh2::character::host_get_player:response->data=&context.host_player;return 0;
   case dh2::character::host_get_current_level:response->data=&context.host_level;return 0;
   case dh2::character::host_get_range_rows:response->data=context.ranges.data();response->count=context.ranges.size();return 0;
   default:return 1; // Lua capture owns push results; direct push is unsupported.
  }
 }
};
#include "renderer_player_canonical_metadata_v4.inc"
class RendererNpcCommandsV1;
class RendererNpcDeathV2;
int npc_death_body_v2(MonsterScriptHandle&,dh2::character::State*,const dh2::character::Request&);
int npc_on_died_v2(MonsterScriptHandle&,std::uintptr_t);
void clear_npc_death_owners_v2();
struct MonsterScriptHandle : dh2::character::RetainedCharacterActorV1 {
 std::shared_ptr<WorldScriptContext> context;
 std::shared_ptr<RendererNpcCommandsV1> script_commands;
 std::shared_ptr<RendererNpcDeathV2> death;
 MonsterScriptHandle(std::uintptr_t identity,std::shared_ptr<WorldScriptContext> world,
                     const std::string& catalog)
  : RetainedCharacterActorV1(identity,world,catalog),context(std::move(world)) {
  bodies={this,state_body};
 }
 ~MonsterScriptHandle() override { death.reset();close(); }
   static int native_frame(void*,dh2::character::NativeFsm24*,const dh2::character::NativeFsmRequest32*,std::uint32_t*);
   static int select_animation(void*,dh2::character::State*,int);
   static int idle_update(void*);
   static int animation_service(void*,dh2::character::AIEventState64*,const dh2::character::AIEventRequest40*,std::uint32_t*);
   static void animation_state(void*,dh2::character::AnimationAIState96*,const dh2::character::AnimationAIRequest32*,dh2::character::AnimationAIResponse16*);
   static void observe(void*,dh2::actor::BlendedPlayback&,const dh2::actor::BlendedPlaybackEvent&);
  static void state_body(void*,dh2::character::State*,const dh2::character::Request*);
  static int state_method(void*,dh2::character::StateOwnerMachine40*,const dh2::character::StateOwnerRequest48*,dh2::character::StateOwnerResponse8*);
};
std::shared_ptr<WorldScriptContext> live_script_world;
static void prepare_player_manager_boot_v25(const std::shared_ptr<WorldScriptContext>&,const dh2::player::DevelopmentPlayerLaunchV2&);
static bool player_manager_bound_character_count_v25(const std::shared_ptr<WorldScriptContext>&,std::int32_t&,std::string&);
struct EquipmentPlatform {
 std::shared_ptr<WorldScriptContext> world;
 AAssetManager* manager{};
 dh2::android_ui::OriginalCacheAssetsV1 cache;
 // The current development world is a local single-player session. Original
 // multiplayer/menu campaign selection is a later producer, not a fallback.
 bool online_mode=false,remote_player=false;
 EquipmentPlatform(std::shared_ptr<WorldScriptContext> w,AAssetManager* a):world(std::move(w)),manager(a),cache(a){}
 static dh2::skinning::VisualAssetResultV6 asset(void* p,const char* uri,std::vector<std::uint8_t>& out,std::string& error){
   bool found=false;if(!uri||!static_cast<EquipmentPlatform*>(p)->cache.read(uri,found,out,error))return dh2::skinning::VisualAssetResultV6::failed;
   return found?dh2::skinning::VisualAssetResultV6::found:dh2::skinning::VisualAssetResultV6::missing;
 }
 static bool open_text(void* p,const char* uri,bool& found,std::vector<std::uint8_t>& out,std::uintptr_t& lease,std::string& error){
   lease=0;auto data=std::make_unique<std::vector<std::uint8_t>>();
   if(!uri||!static_cast<EquipmentPlatform*>(p)->cache.read(std::string("data/")+uri,found,*data,error))return false;
   if(!found){out.clear();return true;}out=*data;lease=reinterpret_cast<std::uintptr_t>(data.release());return true;
 }
 static bool close_text(void*,std::uintptr_t lease,std::string& error){
   if(!lease){error="Equipped localization lease missing";return false;}
   delete reinterpret_cast<std::vector<std::uint8_t>*>(lease);return true;
 }
 static bool debug_text(void* p,const char* key,std::string& error){auto& w=*static_cast<EquipmentPlatform*>(p)->world;std::uint32_t value{};
   if(dh2_character_debug_load(w.debug.get(),&w.debug_files)!=1||dh2_character_debug_get(&value,w.debug.get(),key,&w.debug_files)!=1){error="Equipped localization Debug service failed";return false;}return true;
 }
 static bool constant(void* p,const char* group,const char* name,std::uint32_t& value,std::string& error){
   const auto* design=static_cast<EquipmentPlatform*>(p)->world->tables.design();std::int32_t result{};
   if(!design||!design->lookup||design->lookup(design->context,0,group,name,&result)){error="Equipped text constant lookup failed";return false;}
   std::memcpy(&value,&result,4);return true;
 }
 static bool query(void* p,dh2::player::EquipmentWorldQueryV1 operation,std::uintptr_t subject,
  std::uintptr_t& identity,std::int32_t& scalar,std::string& error){
   auto& c=*static_cast<EquipmentPlatform*>(p);identity=0;scalar=0;
   const auto player=c.world->player_object;
    using Q=dh2::player::EquipmentWorldQueryV1;
    if(operation==Q::online){scalar=c.online_mode;return true;}
    // GetPlayersCount is a global Game query. Original skill application calls
    // it with subject0; it does not require a selected Character argument.
     if(operation==Q::player_count)return player_manager_bound_character_count_v25(c.world,scalar,error);
   if(!player||player->identity!=subject){error="Equipped world query requires the selected live player";return false;}
   switch(operation){
     case Q::current_player:identity=player->identity;return true;
     case Q::current_difficulty:scalar=c.world->host_level.difficulty;return true;
     case Q::remotely_updated:scalar=c.remote_player;return true;
     default:error="Original online player record provider is unavailable";return false;
   }
 }
};
dh2::data::LootRandom8V2 equipment_random{0xD22026u,0};
dh2::data::AiTables actor_ai_tables;bool enemy_ai_enabled=true;
std::map<int,dh2::animation::Player> prince_attack_clips;
dh2::data::AnimationBank prince_animation_bank;
dh2::data::PropertyRules actor_property_rules;
dh2::data::CombatRandom combat_random{0xD22026u,0};unsigned combat_hits=0;
dh2::data::AnimationTables actor_animation_tables;dh2::data::Dictionary actor_clip_table;dh2::data::AnimationRandom actor_random;
std::vector<dh2::objects::Record> world_objects;
int inspected_object=-1;
std::chrono::steady_clock::time_point object_epoch;
bool enabled=false;float center[3]{},radius=1,yaw=-1.57f,pitch=.35f,zoom=1;
dh2::scene::Scene current_scene;dh2::animation::Player player;
std::unique_ptr<EquipmentPlatform> equipment_platform;
std::unique_ptr<dh2::player::PlayerEquipmentRenderOwnerV1> player_equipment;
dh2::animation::Player walk_player;dh2::world::Level level;dh2::world::Point actor_position{};
bool world_mode=false,walking=false,resume_world=false;float move_x=0,move_y=0,heading=0;
bool source_hud_heading=false,source_hud_heading_active=false;
unsigned movement_steps=0,blocked_steps=0;
unsigned native_heading_updates=0;
std::chrono::steady_clock::time_point last_frame;
std::chrono::steady_clock::time_point epoch;
// The touch-to-destination and follow-camera producers remain development
// controls. Actor pose/movement, body services and floor validation below use
// the recovered source pipeline and its original scene/Step/actor ordering.
dh2::physical::NativeWorld actor_world;
dh2::actor::RuntimeState prince_runtime{};
// Same Character+120 scale produced by the existing source InitPost scale
// calculation. Retain it for anchored animation FX as well as mesh bounds.
std::array<float,3> prince_source_scale120{};
bool prince_source_scale120_written{};
dh2::physical::NativeBody prince_body{};
dh2::visual::SceneBinding prince_visual;
dh2::actor::BlendedPlayback prince_locomotion;
dh2::character::CharacterStateOwner prince_state_owner{0x100000001ull};
dh2::character::State& prince_state=prince_state_owner.state();
dh2::character::StateOwnerBehaviorPredicate8 prince_state_predicates{};
extern bool native_actor_ready;
struct PlayerSkillsRuntime {
 ~PlayerSkillsRuntime();
 std::unique_ptr<dh2::fx::CharacterAuthoredFxForceFactoryOwnerV4> combat_fx_forces;
 std::unique_ptr<dh2::fx::CharacterAuthoredResourceFactoryV6> combat_fx_particles;
  dh2::character::CharacterCombatSoundTablesV2 combat_sound_tables;
  dh2::audio::AudioSourceBindingsV38 audio_source_bindings_v38;
 std::unique_ptr<dh2::character::skills::CharacterCombatFxRuntimeV4> combat_fx;
 std::unique_ptr<dh2::character::CharacterTargetMarkerV28> target_marker_v28;
 std::map<std::uintptr_t,const dh2::actor::RotationState*> combat_fx_rotations;
 static bool combat_fx_asset(void*,const char*,std::vector<std::uint8_t>&,std::string&);
 static int combat_fx_rotation(void*,std::uintptr_t,float[3]);
 static bool combat_fx_remaining(void*,dh2::fx::MeshFxRequestV1&,std::string&);
 static bool combat_fx_camera(void*,float[16],float[3],std::string&);
 static bool combat_fx_driver(void*,std::uint32_t&,std::string&);
 static void bind_combat_fx(PlayerSkillsRuntime&);
 int combat_fx_application(const dh2::character::skills::SkillApplyRequestV6&,const dh2::data::CombatResult&);
 int combat_fx_frame(std::int32_t,std::int32_t);
 std::shared_ptr<WorldScriptContext> world;
 std::shared_ptr<dh2::data::PlayerSavegameV1> save;
 std::shared_ptr<dh2::data::PropertySheet> temporary=std::make_shared<dh2::data::PropertySheet>();
 dh2::character::NativeFsm24& fsm=prince_state_owner.native_fsm();
 dh2::character::skills::SkillAIOwnerV3 skill_owner{0x100000001ull,0,0};
 // Exact retained CharAI/AISPlayerIPhone source dispatch identities.
 std::array<std::uintptr_t,51> ai_virtuals{},ais_virtuals{};
 dh2::character::AIEventOwner48 event_owner{};
 dh2::character::AIEventState64 events{};
  struct WorldActor {
   PlayerSkillsRuntime* runtime{};
   MonsterScriptHandle* npc{};
   // Character C1 constructor3aa4ec/3aa4f0 stores null at +14a4.
   // This distinct raw target must never copy CharAI's current target.
   dh2::ui::HudManagerActor hud{};
  std::shared_ptr<dh2::character::ScriptCharacterObject> object;
  dh2::target_search::Object48 search{};
  dh2::character::skills::SkillTargetCharacterV6 character{};
  // NPC Handle storage belongs solely to its retained receiver. The player
  // preserves this runtime's existing Handle lifetime on its own branch.
  std::unique_ptr<dh2::target_providers::Handle16> player_handle;
  dh2::target_providers::Handle16* shared_handle{};
   dh2::character::State* machine{};
   // GameObject constructor initializes cached interaction node+2e8 and
   // checked byte+2ec to null/zero; populated only by GetInteractionSpot.
   bool interaction_checked{};
   std::int32_t interaction_node=-1;
   std::uintptr_t interaction_visual{};
   std::uintptr_t target_node{}; // Recovered GameObject constructor null.
   dh2::data::CombatantView combat_facts{};
   std::uintptr_t combat_controller{};
  static int refresh(void* p,dh2::character::skills::WorldTargetActorBorrowV1* out){
   auto& a=*static_cast<WorldActor*>(p);if(!out||!a.object)return -1;
   a.character.identity=a.object->identity;a.character.resolved=a.object->properties->resolved.data();
   a.character.name=a.object->name.c_str();a.character.dead1449=std::uint8_t(a.object->life->dead);
    const dh2::actor::RotationState* actual_rotation=nullptr;
    if(a.object->identity==0x100000001ull)actual_rotation=&prince_runtime.rotation;
    else if(a.npc)actual_rotation=&a.npc->runtime.rotation;
    if(dh2::character::skills::character_world_target_pose_v2(a.search,a.object->identity,a.object->position.data(),actual_rotation))return -1;
    if(dh2::character::skills::character_world_target_cached_fields_v2(a.search,a.object->properties->resolved.data(),a.object->properties->resolved.size()))return -1;
   *out={};out->identity=a.object->identity;out->search=&a.search;out->character=&a.character;
    out->life=a.object->life.get();out->position=a.object->position.data();
    out->scene=&current_scene;
   out->target_node=&a.target_node;
      if(a.object->identity==0x100000001ull){out->heading_angle=&prince_runtime.rotation.heading_angle;out->controller_heading_angle=&prince_runtime.controller.heading.angle;if(native_actor_ready)out->aabb6=prince_runtime.subobjects.absolute_bounds;}
      else if(a.npc&&a.npc->animation){out->scene=&a.npc->animation->scene();out->heading_angle=out->controller_heading_angle=&a.npc->runtime.rotation.heading_angle;if(a.npc->source_bounds_ready)out->aabb6=a.npc->runtime.subobjects.absolute_bounds;}
   // Nonnull target-node/enable/cache producers remain required. Source
   // constructor-null node genuinely selects the existing +160 position.
   return 0;
  }
   static int controller(void*,dh2::character::ControllerCommandState32*);
   static int combat(void*,dh2::character::skills::WorldSkillExecutionActorV6*);
 };
  std::map<std::uintptr_t,WorldActor> world_actors;
  std::uintptr_t hud_reported_target{};int hud_reported_hp=-1;bool hud_reported_dead{};
   std::unique_ptr<dh2::character::skills::CharacterWorldRuntimeV1> targets;
   std::unique_ptr<dh2::world::RendererWorldTouchLiveV2> world_touch;
   static void bind_world_touch(PlayerSkillsRuntime&);
  std::unique_ptr<dh2::player::PlayerNetworkLocalOwnerV4> player_network_local;
  std::unique_ptr<dh2::player::PlayerManagerCombatRuntimeV2> player_manager;
  std::function<void()> player_manager_boot_return_v25;
  std::unique_ptr<dh2::world::LevelConfigMusicOwnerV1> aggro_config;
  std::uintptr_t aggro_config_identity{};
  dh2::character::PlayerAggroFieldsV1 player_aggro_fields;
  dh2::sound::VoxMusicFieldsV1 vox_music_fields;
  std::unique_ptr<dh2::sound::VoxMusicStateOwnerV1> vox_music;
  std::unique_ptr<dh2::character::CharacterWorldPlayerAggroV2> aggro_events;
  std::unique_ptr<dh2::character::skills::CharacterSkillNativeReadOnlyBindingsV6> native;
  std::unique_ptr<dh2::trophies::TrophyManagerOwnerV1> trophies;
  std::unique_ptr<dh2::trophies::TrophyNativeBindingsV1> trophy_bindings;
  dh2::character::CharacterConstructorCombatFieldsV1 combat_fields{};
  dh2::character::DotCombatContext32 combat_context{};
  std::unique_ptr<dh2::character::skills::CharacterWorldSkillExecutionV6> execution;
 std::uint8_t application_mana_bypass{};
 std::uint8_t* source_byte415{}; // borrows sole registered Character +415.
 std::map<std::string,std::uint32_t> application_debug_flags;
 std::unique_ptr<dh2::character::skills::CharacterPlayerSkillsV6> player;
 std::unique_ptr<dh2::data::PlayerSaveLoadOwnerV1> player_save_load;
 std::unique_ptr<dh2::android_ui::CharacterPanelRuntimeV3> character_panel_runtime;
 dh2::character::skills::SkillStateV4 skill_state{&prince_state,0x100000001ull,0,0,0,0,0,0,{0,0}};
 std::map<std::uintptr_t,std::string> debug_tokens;std::uintptr_t debug_serial{};
 std::string error;
 static int cached(void* p,const char* name,dh2::data::Bytes* out,bool* found){
  auto& t=*static_cast<PlayerSkillsRuntime*>(p);const auto* bytes=t.world->script_files.find(name);
  *found=bytes!=nullptr;*out=bytes?dh2::data::Bytes{bytes->data(),bytes->size()}:dh2::data::Bytes{};return 0;
 }
 static int difficulty(void* p,std::int32_t* out){
  auto& t=*static_cast<PlayerSkillsRuntime*>(p);if(!t.world||!out)return -1;
  *out=t.world->host_level.difficulty;return 0;
 }
 static int vitals(void* p,dh2::character::CharacterScriptSessionV3& session,const dh2::character::ScriptLifecycleRequest32& request){
  auto& t=*static_cast<PlayerSkillsRuntime*>(p);
  if(request.service!=dh2::character::script_refresh_vitals||request.subject!=session.timers().owner)return -1;
  const dh2::character::ScriptInitVitals32 input{session.timers().owner,&session.property_view(),t.world->effect_services};
  dh2::character::ScriptInitVitals24 result{};return dh2_character_script_init_vitals(&result,&input)==1?0:-1;
 }
   std::unique_ptr<dh2::android_ui::FaeryCastOwnerV2> cast;
   std::unique_ptr<dh2::character::CharacterAnimationEventOwnerV1> animation_events;
   dh2::character::AnimationFloorBorrowV1 animation_floor{};
   std::uintptr_t animation_visual{};
   std::uint32_t animation_root{};
    std::int32_t animation_lag{};
   dh2::character::AttackState64 attack_fields{};
   dh2::character::ControllerAttackState32 attack_controller{};
   std::unique_ptr<dh2::character::skills::CharacterWorldPlayerAttackOwnerV1> attack_owner;
   std::unique_ptr<dh2::character::skills::CharacterWorldAttackGeometryV1> attack_geometry;
   std::map<std::uintptr_t,std::unique_ptr<dh2::character::NpcInventoryOwnerV1>> attack_npc_inventories;
   static int attack_inventory(void*,std::uintptr_t,dh2::character::skills::WorldAttackInventoryBorrowV1*);
   static int attack_kind(void*,std::uintptr_t,std::int32_t*);
   static int attack_backend(void*,const dh2::character::AttackRequest32*,dh2::character::AttackResponse16*);
    static void bind_attack(PlayerSkillsRuntime&);
    dh2::character::ControllerCommandState32 heading_command{};
    dh2::physical::NativeBody* heading_body{};
    std::uint8_t heading_remote_updated_byte=0;
    std::unique_ptr<dh2::character::CharacterHeadingOwnerV1> heading_owner;
    std::unique_ptr<dh2::character::skills::CharacterWorldTargetFrameV2> target_frame;
    dh2::character::CharacterControlServices16 target_control{};
    std::vector<std::int32_t> target_ai_sounds;
    std::uint32_t source_target_updated88{}; // written at source AI.Update prefix; never used as ctor fact
    std::vector<std::uint32_t> target_route_path_v2; // caller-owned native pathfinder output scratch
    static void bind_target_frame(PlayerSkillsRuntime&);
    int update_target_frame();
    int target_character_event(unsigned,std::uintptr_t);
    int target_ai_event(unsigned,std::uintptr_t);
    static bool heading_position(void*,std::uintptr_t,const float*&,std::string&);
    static bool heading_remote(void*,bool&,std::string&);
    static bool heading_physics(void*,bool&,std::string&);
    static bool heading_event(void*,std::uint32_t,std::string&);
     static void bind_heading(PlayerSkillsRuntime&);
     b2Shape* collision_primary{};
     b2Shape* collision_secondary{};
     std::uint8_t collision_filter_disabled{};
     std::unique_ptr<dh2::character::CharacterCollisionLifecycleV1> collisions;
     static dh2::physical::NativeBody* collision_physical(void*);
     static int collision_method(void*,dh2::character::WorldNpcObjectMethodV1,std::string&);
     static void bind_collisions(PlayerSkillsRuntime&);
     int enable_collisions_source(std::uintptr_t);
   std::uint8_t attack_sticky=1; // CharAI C2 3cec64 writes +4b = 1.
   static int attack_animation_service(void*,dh2::character::AttackState64&,
      const dh2::character::AttackAnimationRequestV1&,std::uint32_t&);
   int attack_animation_step(bool);
  bool timer_failure_logged{};
  static int event_service(void*,dh2::character::AIEventState64*,const dh2::character::AIEventRequest40*,std::uint32_t*);
  static int regen_debug(void*,const dh2::character::skills::SkillAttackNativeRequestV6*,std::uintptr_t*);
  static void bind_cast(PlayerSkillsRuntime&);
  static int skill_service(void*,dh2::character::skills::SkillAIContextV3*,const dh2::character::skills::SkillAIRequest32V3*,dh2::character::skills::SkillAIResponse32V3*);
 static int state_service(void*,dh2::character::skills::SkillStateV4*,const dh2::character::skills::SkillStateRequest32V4*,dh2::character::skills::SkillStateResponse16V4*);
 static int mana_service(void*,const dh2::character::skills::SkillManaRequestV5*,dh2::character::skills::SkillManaResponseV5*);
};
std::unique_ptr<PlayerSkillsRuntime> player_skills_runtime;
bool connect_character_panel_runtime_impl(const dh2::android_ui::CharacterPanelMovieRuntimeV3&,
 dh2::android_ui::CharacterPanelGameplayServicesV2&,std::string&);
std::string source_player_attack_v1(int);
bool source_player_heading_v1(const float*,bool,std::string&);
static int source_named_animation_sound_v38(PlayerSkillsRuntime&,std::uintptr_t,const char*);
extern std::uint32_t controller_global_blocked;
void MonsterScriptHandle::state_body(void* pointer,dh2::character::State* state,const dh2::character::Request* request){
 auto& m=*static_cast<MonsterScriptHandle*>(pointer);
   if(!request||!m.machine||state!=&m.machine->state())throw std::runtime_error("NPC state body must borrow its actual machine");
   const int death_status=npc_death_body_v2(m,state,*request);
   if(death_status<0)throw std::runtime_error("Required original NPC death body service "+std::to_string(request->service));
   if(death_status==1)return;
  if(request->service==dh2::character::idle_common_update){if(idle_update(pointer))throw std::runtime_error("Required source NPC IdleCommonUpdate");return;}
 if(request->service!=dh2::character::set_animation)throw std::runtime_error("Required NPC body service "+std::to_string(request->service));
 if(select_animation(pointer,state,request->argument[0]))throw std::runtime_error("Original NPC animation selection failed");
}
int MonsterScriptHandle::state_method(void* pointer,dh2::character::StateOwnerMachine40* machine,const dh2::character::StateOwnerRequest48* request,dh2::character::StateOwnerResponse8*){
 auto& m=*static_cast<MonsterScriptHandle*>(pointer);
  if(!request||!m.machine||machine!=&m.machine->owner().machine())return -1;
  if(m.injury){const auto handled=m.injury->body(machine,*request);if(handled)return handled==1?0:-1;}
 if(request->operation==dh2::character::state_owner_character_event&&m.state_changed){
  auto* controller=m.controller->command_state(controller_global_blocked);
  if(!controller)return -1;m.ai_owner.locked=controller->locked;m.ai_owner.forced=controller->forced;m.ai_events.global_blocked=controller->global_blocked;
  const int status=m.state_changed->notify(*request);
  if(status)__android_log_print(ANDROID_LOG_ERROR,"DH2Native","NPC state notification failed | %s | %s",m.object->name.c_str(),m.state_changed->error().c_str());
  return status;
 }
 return -1;
}
#include "renderer_npc_animation_v1.inc"
std::vector<std::uint8_t> read(AAssetManager*,const std::string&,const std::string&);
#include "renderer_npc_bounds_v1.inc"
void initialize_player_skills(bool restore);
int prince_owner_service(void*,dh2::character::StateOwnerMachine40*,const dh2::character::StateOwnerRequest48*,dh2::character::StateOwnerResponse8*);
const dh2::character::StateOwnerServices16 prince_owner_services{nullptr,prince_owner_service};
b2FilterData prince_initial_filter;
bool prince_scene_phase=false;
std::uint64_t pending_character_services=0;
dh2::character::Facts* active_prince_facts=nullptr;
void request_prince_death();
int prince_event(unsigned,std::uint64_t);
unsigned prince_event_cause=0;
dh2::character::Facts prince_facts();
void character_service(void*,dh2::character::State*,const dh2::character::Request*);
const dh2::character::Services prince_services{nullptr,character_service};
std::uint32_t prince_flags=0x2380,prince_move_type=0;
// Recovered controller constructor and shared BSS initial values. Original
// script/HUD producers will write these owned native gates as they are bound.
std::uint32_t controller_global_blocked=0,prince_controller_forced=0;
float scene_clock=0;bool native_actor_ready=false;
unsigned native_actor_frames=0,native_physics_steps=0;
std::vector<dh2::navigation::ObstacleEntry> live_obstacle_entries;
std::vector<unsigned> live_obstacle_floors,live_workspace_floors;
std::vector<dh2::navigation::PathSegment> live_path_segments,live_workspace_segments;
std::vector<dh2::navigation::AvoidanceActor> live_workspace_actors;
dh2::navigation::ObstacleRegistry live_registry{};
dh2::navigation::ControllerWorkspace live_workspace{};
dh2::navigation::MotionPolicy live_motion_policy{};
bool borrow_native_peer_contact_v4(void*,dh2::navigation::PhysicalContact&,std::string&);
struct BodyOwner {
 dh2::physical::WorldObject services{};
 dh2::navigation::PhysicalContact contact{};
 dh2::physical::NativeBody* native=nullptr;
 unsigned additions=0,results=0;
 BodyOwner(){services.context=this;services.test=test;services.contact=collision;services.velocity=velocity;}
 static unsigned test(void* a,void* b,const dh2::physical::Filter*,const dh2::physical::Filter*){
   dh2::navigation::PhysicalContact peer{};std::string error;
   if(!borrow_native_peer_contact_v4(b,peer,error))throw std::runtime_error(error);
   const int result=dh2_nav_can_collide(&static_cast<BodyOwner*>(a)->contact,&peer);
   if(result<0)throw std::runtime_error("Required valid same-owner physical contact queries");
   return result==1;
 }
 static void collision(void* a,dh2::physical::ContactEvent event,void*,const float*,unsigned){
  auto& owner=*static_cast<BodyOwner*>(a);owner.additions+=event==dh2::physical::ContactEvent::add;
  owner.results+=event==dh2::physical::ContactEvent::result;
 }
 static void velocity(void* a,float* xy){
  const auto* n=static_cast<BodyOwner*>(a)->native;
  if(n&&n->body){const auto v=n->body->GetLinearVelocity();xy[0]=v.x*100.f;xy[1]=v.y*100.f;}
  else xy[0]=xy[1]=0;
 }
 void set_filter(const dh2::physical::CharacterBodyConfig& c){
  contact={1,0,1,1,{std::int16_t(c.shape.group_index),std::uint16_t(c.shape.category_bits),std::uint16_t(c.shape.mask_bits),1},{}};
 }
};
BodyOwner prince_body_owner;
std::vector<std::unique_ptr<BodyOwner>> decor_body_owners;
std::vector<dh2::physical::NativeBody> decor_bodies;
// Borrowed virtual-query capability; no actor or filter storage is mirrored.
void* item_contact_context_v4{};
bool(*item_contact_borrow_v4)(void*,void*,dh2::navigation::PhysicalContact&,std::string&){};
bool borrow_native_peer_contact_v4(void* peer,dh2::navigation::PhysicalContact& out,std::string& error){
 if(peer==&prince_body_owner){out=prince_body_owner.contact;return true;}
 for(const auto& owner:decor_body_owners)if(peer==owner.get()){out=owner->contact;return true;}
 if(item_contact_borrow_v4&&item_contact_context_v4)
  return item_contact_borrow_v4(item_contact_context_v4,peer,out,error);
 error="Required registered source physical peer context query";return false;
}
bool frozen=false,animation_failed=false,frozen_cursor_logged=false;int sampled_ms=0;
bool character_panel_open=false;
GLint position,texcoord,color,mvp,texture_matrix,material_color,has_alpha,alpha_ref,original_alpha_blue,diffuse_uniform_v35,alpha_uniform_v35;
void check(const char* operation){
  auto code=glGetError();if(code!=GL_NO_ERROR){char b[128];std::snprintf(b,sizeof(b),"%s GL error 0x%04x",operation,code);throw std::runtime_error(b);}
}
GLuint shader(GLenum type,const char* source){
  GLuint s=glCreateShader(type);glShaderSource(s,1,&source,nullptr);glCompileShader(s);GLint ok=0;glGetShaderiv(s,GL_COMPILE_STATUS,&ok);
  if(!ok){char log[2048]{};glGetShaderInfoLog(s,sizeof(log),nullptr,log);glDeleteShader(s);throw std::runtime_error(log);}return s;
}
void create_program(){
  const char* vs=R"(attribute vec3 position;attribute vec2 texcoord;attribute vec4 color;
uniform mat4 mvp;uniform mat4 texture_matrix;varying vec2 uv;varying vec4 tint;
void main(){gl_Position=mvp*vec4(position,1.0);uv=(texture_matrix*vec4(texcoord,0.0,1.0)).xy;tint=color;})";
  const char* fs=R"(precision mediump float;uniform sampler2D diffuse;uniform sampler2D alpha_map;
uniform float has_alpha;uniform float alpha_ref;uniform float original_alpha_blue;uniform vec4 material_color;varying vec2 uv;varying vec4 tint;
void main(){vec4 c=texture2D(diffuse,uv)*tint*material_color;
if(has_alpha>0.5){if(original_alpha_blue>0.5)c.a=texture2D(alpha_map,uv).b;else c.a*=texture2D(alpha_map,uv).r;}if(c.a<=max(alpha_ref,0.0039))discard;gl_FragColor=c;})";
  GLuint v=shader(GL_VERTEX_SHADER,vs),f=0;
  try{f=shader(GL_FRAGMENT_SHADER,fs);}catch(...){glDeleteShader(v);throw;}
  program=glCreateProgram();glAttachShader(program,v);glAttachShader(program,f);glLinkProgram(program);glDeleteShader(v);glDeleteShader(f);
  GLint ok=0;glGetProgramiv(program,GL_LINK_STATUS,&ok);if(!ok){char log[2048]{};glGetProgramInfoLog(program,sizeof(log),nullptr,log);glDeleteProgram(program);program=0;throw std::runtime_error(log);}
  position=glGetAttribLocation(program,"position");texcoord=glGetAttribLocation(program,"texcoord");color=glGetAttribLocation(program,"color");
  mvp=glGetUniformLocation(program,"mvp");texture_matrix=glGetUniformLocation(program,"texture_matrix");material_color=glGetUniformLocation(program,"material_color");
  diffuse_uniform_v35=glGetUniformLocation(program,"diffuse");alpha_uniform_v35=glGetUniformLocation(program,"alpha_map");
  has_alpha=glGetUniformLocation(program,"has_alpha");alpha_ref=glGetUniformLocation(program,"alpha_ref");
  original_alpha_blue=glGetUniformLocation(program,"original_alpha_blue");
}
void release(std::vector<Draw>& batches,std::vector<GLuint>& textures){
  for(auto& b:batches){if(b.vertices)glDeleteBuffers(1,&b.vertices);if(b.indices)glDeleteBuffers(1,&b.indices);}
  for(auto t:textures)glDeleteTextures(1,&t);batches.clear();textures.clear();
}
void release_objects(std::vector<ObjectGroup>& groups){std::vector<GLuint> none;for(auto& group:groups)release(group.draws,none);groups.clear();}
std::vector<std::uint8_t> read(AAssetManager* assets,const std::string& name,const std::string& folder="textures"){
  auto path=name;
  if(folder=="textures")std::transform(path.begin(),path.end(),path.begin(),[](unsigned char c){return char(std::tolower(c));});
  auto* a=AAssetManager_open(assets,(folder+"/"+path).c_str(),AASSET_MODE_BUFFER);
  if(!a)throw std::runtime_error("Bundled asset missing: "+folder+"/"+name);
  const auto n=AAsset_getLength64(a);
  if(n<=0||n>32*1024*1024){AAsset_close(a);throw std::runtime_error("Texture exceeds size limit");}
  std::vector<std::uint8_t> bytes(n);std::size_t done=0;
  while(done<bytes.size()){const auto got=AAsset_read(a,bytes.data()+done,bytes.size()-done);if(got<=0){AAsset_close(a);throw std::runtime_error("Short asset read");}done+=got;}
  AAsset_close(a);return bytes;
}
std::shared_ptr<dh2::character::CharacterGameDesign> load_game_design(AAssetManager* assets){
 std::array<std::array<std::vector<std::uint8_t>,3>,5> raw;
 const char* groups[]={"character_properties","character_classes","ai","ai_factions","levels"};
 const char* suffixes[]={"_pyarray.bin","_pyarraynames.bin","_pystructnames.bin"};
 for(unsigned i=0;i<5;++i)for(unsigned j=0;j<3;++j)raw[i][j]=read(assets,std::string(groups[i])+suffixes[j],"data");
 dh2::character::GameDesignInputs256 inputs{};
 dh2::character::GameDesignTableInput48* views[]={&inputs.characters,&inputs.classes,&inputs.ai,&inputs.factions,&inputs.levels};
 for(unsigned i=0;i<5;++i)*views[i]={{raw[i][0].data(),raw[i][0].size()},{raw[i][1].data(),raw[i][1].size()},{raw[i][2].data(),raw[i][2].size()}};
 const char* constant_files[]={"ai","animations","character_classes","character_properties","common_text","design","dialogs","effects","engine_core","faeries","fonts","game_objects","item_powers","levels","loot_audiovisual","loot_table","multiplayer","projectiles","scripts","skills","specs","trophies","v2conditions","v2eventmanager","v2quests","worldmap"};
 std::vector<std::vector<std::uint8_t>> constants;
 for(const auto* name:constant_files)constants.push_back(read(assets,std::string(name)+"_pycst.bin","data"));
 std::vector<dh2::data::Bytes> constant_views;
 for(const auto& value:constants)constant_views.push_back({value.data(),value.size()});
 inputs.constants=constant_views.data();inputs.constant_count=constant_views.size();
 auto result=std::make_shared<dh2::character::CharacterGameDesign>();std::string error;
 if(!result->initialize(inputs,error))throw std::runtime_error("Persistent design load failed: "+error);
 return result;
}
std::shared_ptr<dh2::data::DesignSettingsOwner> load_design_settings(AAssetManager* assets){
 auto records=read(assets,"design_pyarray.bin","data");
 auto names=read(assets,"design_pyarraynames.bin","data");
 auto schema=read(assets,"design_pystructnames.bin","data");
 auto owner=std::make_shared<dh2::data::DesignSettingsOwner>();std::string error;
 if(!owner->load({records.data(),records.size()},{names.data(),names.size()},
    {schema.data(),schema.size()},error))throw std::runtime_error("Authored design settings load failed: "+error);
 return owner;
}
std::shared_ptr<dh2::data::SkillTables> load_skill_tables(AAssetManager* assets){
 auto records=read(assets,"skills_pyarray.bin","data"),names=read(assets,"skills_pyarraynames.bin","data"),schema=read(assets,"skills_pystructnames.bin","data");
 auto owner=std::make_shared<dh2::data::SkillTables>();std::string error;
 if(!owner->load({records.data(),records.size()},{names.data(),names.size()},{schema.data(),schema.size()},error))
  throw std::runtime_error("Owned skill tables load failed: "+error);
 return owner;
}
std::string digest_text(const dh2::assets::Sha256Digest& digest){
 std::string result;for(auto byte:digest){result+="0123456789abcdef"[byte>>4];result+="0123456789abcdef"[byte&15];}return result;
}
std::shared_ptr<dh2::data::EffectsTables> load_effects_tables(AAssetManager* assets){
 const char* names[]={"effects_pyarray.bin","effects_pyarraynames.bin","effects_pystructnames.bin","effects_dictionary_pyarraynames.bin","effects_dictionary_pyarray.bin"};
 const char* expected[]={"9a743e51e4ba098a63fd274ee890efeaf0dc6f750d51fc53fd9ef2c01695f71b",
  "f91b3ef914e6840c4339c1af32446c5f3d24605aaa4a9403a797cc941edbc043",
  "d4296263d954714d560083e058da3ff4d87d4b35fa7c796c4e88618f4cb2cc2b",
  "8227757b4e18e10f898e0693eeb1b728b024e8b4675162f97c5fbb6647b910f2",
  "e0162462ed31315a637b3f784e6957949054855357cf09431e5249db5c42d294"};
 std::array<std::vector<std::uint8_t>,5> raw;
 for(unsigned i=0;i<5;++i){raw[i]=read(assets,names[i],"data");dh2::assets::Sha256Digest digest{};
  if(!dh2::assets::sha256(raw[i].data(),raw[i].size(),digest)||digest_text(digest)!=expected[i])
   throw std::runtime_error("Bundled effects source digest differs");
 }
 auto owner=std::make_shared<dh2::data::EffectsTables>();std::string error;
 if(!owner->load({raw[0].data(),raw[0].size()},{raw[1].data(),raw[1].size()},
  {raw[2].data(),raw[2].size()},{raw[3].data(),raw[3].size()},{raw[4].data(),raw[4].size()},error))
  throw std::runtime_error("Owned effects tables load failed: "+error);
 return owner;
}
void load_actor_initialization(AAssetManager* assets,WorldScriptContext& context,
 const std::vector<std::uint8_t>& descriptor,const std::vector<dh2::objects::Record>& records){
 auto sidecar=read(assets,"crypt01-actor-initialization.bin","worlds");
 dh2::assets::Sha256Digest descriptor_digest{},sidecar_digest{};
 if(!dh2::assets::sha256(descriptor.data(),descriptor.size(),descriptor_digest)||
    !dh2::assets::sha256(sidecar.data(),sidecar.size(),sidecar_digest))throw std::runtime_error("Actor initialization digest failed");
 std::vector<dh2::character::ActorInitializationKey> keys;
 for(const auto& record:records)if(record.kind==1)keys.push_back({record.room,record.name,record.character});
 if(context.initialization_loaded){
  if(context.initialization.descriptor_sha256!=descriptor_digest||context.initialization_asset_sha!=sidecar_digest)
   throw std::runtime_error("Retained actor initialization assets changed");
 }else{
  std::string error;
  if(!dh2::character::load_actor_initialization(sidecar.data(),sidecar.size(),descriptor_digest,keys,context.initialization,error))
   throw std::runtime_error("Actor initialization sidecar rejected: "+error);
  context.initialization_asset_sha=sidecar_digest;context.initialization_loaded=true;
 }
 __android_log_print(ANDROID_LOG_INFO,"DH2Native","Actor initialization ready | records %zu | sources %zu | DACT SHA %s | sidecar SHA %s | independently hashed bytes",
  context.initialization.records.size(),context.initialization.sources.size(),digest_text(descriptor_digest).c_str(),digest_text(sidecar_digest).c_str());
}
std::shared_ptr<const dh2::character::CharacterAnimationResources> load_monster_resources(
 AAssetManager* assets,WorldScriptContext& context,const ObjectActor& actor){
 auto found=context.monster_resources.find(actor.character);if(found!=context.monster_resources.end())return found->second;
 const std::map<std::string,std::string> tags{{"Crypt_Skeleton","skeleton"},{"CryptSlime","slime"},{"CryptSlime_RE","slime-red"},{"Crypt_Ghost","ghost"}};
 const auto tag=tags.find(actor.character);if(tag==tags.end())throw std::runtime_error("Original monster bank unavailable");
 auto bank=read(assets,"monster-"+tag->second+"-animation-bank.bin","data"),model=read(assets,actor.model,"actors");
 dh2::resources::BresView view{};dh2::scene::Scene factory;std::string error;
 if(dh2_bres_open(&view,model.data(),model.size())!=dh2::resources::BresError::ok||!dh2::scene::load(view,factory,error))
  throw std::runtime_error("Monster factory scene rejected: "+error);
 std::shared_ptr<const dh2::character::CharacterAnimationResources> owner;
 auto reader=[&](const dh2::data::AnimationBankResource& resource,std::vector<std::uint8_t>& output,std::string&){
  const auto split=resource.asset.find_last_of('/');if(split==std::string::npos)return false;
  output=read(assets,resource.asset.substr(split+1),resource.asset.substr(0,split));return true;
 };
 if(!dh2::character::CharacterAnimationResources::load({bank.data(),bank.size()},factory,reader,owner,error)||owner->metadata().character!=actor.character)
  throw std::runtime_error("Monster owned bank rejected: "+error);
 context.monster_resources.emplace(actor.character,owner);return owner;
}
#include "renderer_npc_commands_v1.inc"
int npc_command_position_v1(void* raw,std::uintptr_t identity,const float*& out){
 auto& m=*static_cast<MonsterScriptHandle*>(raw);
 if(!player_skills_runtime||player_skills_runtime->world!=m.context)return -1;
 std::string error;
 const bool result=PlayerSkillsRuntime::heading_position(player_skills_runtime.get(),identity,out,error);
 if(!result)__android_log_print(ANDROID_LOG_ERROR,"DH2Native","Required NPC target position | %s | %s",m.object->name.c_str(),error.c_str());
 return result?0:-1;
}
bool npc_command_raise_v1(void*,MonsterScriptHandle& m,std::uint32_t event,const dh2_script_callback_scope*,std::string& error){
 auto* command=m.controller->command_state(controller_global_blocked);
 if(!command){error="Required same NPC controller for heading RaiseAIEvent";return false;}
 m.ai_owner.forced=command->forced;m.ai_owner.locked=command->locked;m.ai_events.global_blocked=command->global_blocked;
 const dh2::character::AIEventPayload24 payload{};dh2::character::AIEventResult16 result{};
 const dh2::character::AIEventServices24 services{&m,MonsterScriptHandle::animation_service,63,0};
 if(dh2_character_ai_event(&result,&m.ai_events,event,&payload,&services)){
  error="Required same NPC heading RaiseAIEvent "+std::to_string(event)+": "+m.machine->error();return false;
 }
 return true;
}
void suspend_npc_navigation_v1(){
 for(auto& group:object_groups)for(auto& actor:group.instances)if(actor.script&&actor.script->script_commands){
  auto& commands=*actor.script->script_commands;
  if(!commands.suspend_navigation())throw std::runtime_error(commands.owner().error());
 }
}
void initialize_npc_navigation_v1(){
 if(!level.native_floor)throw std::runtime_error("Required actual NPC navigation floor");
 for(auto& group:object_groups)for(auto& actor:group.instances)if(actor.kind==1&&actor.script){
  auto& m=*actor.script;
  if(!m.script_commands||!m.source_bounds_ready)throw std::runtime_error("Required retained NPC commands/owner bounds");
  if(!m.script_commands->rebind_floor(level.native_floor.get()))throw std::runtime_error(m.script_commands->owner().error());
  // This retained NPC graph has no allocated CharacterPhysical. Borrow its
  // actual null body and owner bounds rather than another actor's body.
  if(m.machine->owner().machine().physical)throw std::runtime_error("Required actual NPC CharacterPhysical producer");
  const auto* box=m.runtime.subobjects.absolute_bounds;
  const dh2::navigation::ProducerFields fields{dh2::navigation::ProducerClass::character,0,0,0,{box[0],box[1]},{box[3],box[4]}};
  if(!m.script_commands->initialize_pf(fields))throw std::runtime_error(m.script_commands->owner().error());
 }
}
#include "renderer_retained_character_actor_v1.inc"
// Development-only read probe through the actual Lua object method bridge.
// The transient function observes the retained record and does not set targets,
// move actors or substitute for any original AI callback.
void verify_script_object_position(dh2_script_vm* vm,std::uintptr_t identity,const std::array<float,3>& point){
 const char probe[]="function __dh2_scene_position_probe(o,x,y,z) assert(select('#',o:GetPosition())==3); local a,b,c=o:GetPosition(); assert(a==x and b==y and c==z) end";
 const char cleanup[]="__dh2_scene_position_probe=nil";
 if(dh2_script_vm_load_source_file(vm,probe,sizeof(probe)-1))
  throw std::runtime_error(std::string("Scene position probe load failed: ")+dh2_script_vm_error(vm));
 dh2_script_value values[4]{};values[0].type=DH2_SCRIPT_SOURCE_OBJECT;values[0].identity=identity;
 for(unsigned i=0;i<3;++i){values[i+1].type=DH2_SCRIPT_NUMBER;values[i+1].number=point[i];}
 const auto status=dh2_script_vm_call_discard_source_objects(vm,"__dh2_scene_position_probe",values,4);
 const auto error=status?std::string(dh2_script_vm_error(vm)):std::string();
 const auto cleaned=dh2_script_vm_load_source_file(vm,cleanup,sizeof(cleanup)-1);
 if(status||cleaned)throw std::runtime_error("Scene object position differs: "+error);
}
GLuint upload(AAssetManager* assets,const std::string& name,std::map<std::string,GLuint>& cache,std::vector<GLuint>& owned,
 dh2::android_ui::OriginalCacheAssetsV1* original=nullptr){
  auto found=cache.find(name);if(found!=cache.end())return found->second;
  std::vector<std::uint8_t> rgba;unsigned w=1,h=1;
  if(name.empty())rgba={255,255,255,255};
  else{
    std::vector<std::uint8_t> raw;
    if(original){bool found=false;std::string error;
      if(!original->read("data/3d/textures/"+name,found,raw,error)||!found)
        throw std::runtime_error("Original equipped texture unavailable: "+name+" | "+error);
    }else raw=read(assets,name);
    dh2::textures::View view{};
    if(dh2_texture_open(raw.data(),raw.size(),&view)!=dh2::textures::Error::ok)throw std::runtime_error("Texture header rejected: "+name);
    w=view.width;h=view.height;rgba.resize(std::size_t(w)*h*4);
    if(dh2_texture_decode(&view,rgba.data(),rgba.size())!=dh2::textures::Error::ok)throw std::runtime_error("Texture decode rejected: "+name);
  }
  GLint max=0;glGetIntegerv(GL_MAX_TEXTURE_SIZE,&max);if(w>unsigned(max)||h>unsigned(max))throw std::runtime_error("Texture exceeds GPU limit");
  GLuint t=0;glGenTextures(1,&t);owned.push_back(t);glBindTexture(GL_TEXTURE_2D,t);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  // Original UVs wrap beyond [0,1]; fixture sizes are all powers of two.
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
  glPixelStorei(GL_UNPACK_ALIGNMENT,1);glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,w,h,0,GL_RGBA,GL_UNSIGNED_BYTE,rgba.data());check("Model texture upload");
  cache[name]=t;return t;
}
void sync_equipment_draws(const std::vector<dh2::skinning::VisualDrawViewV32>& parts,
 AAssetManager* assets,dh2::android_ui::OriginalCacheAssetsV1& original){
  bool rebuild=parts.size()!=equipment_parts.size();
  for(std::size_t i=0;i<parts.size();++i){const auto& part=parts[i];
    if(!part.positions||!part.retention||!part.geometry||!part.material_table||!part.materials||
       part.positions->size()!=part.geometry->positions.size()||part.positions->empty())
      throw std::runtime_error("Equipped draw snapshot backing is invalid");
    if(!rebuild){const auto& old=equipment_parts[i];
      rebuild=part.geometry!=old.geometry||part.material_table!=old.material_table||
        part.materials!=old.materials||part.category!=old.category||part.module!=old.module||
        part.weapon_slot!=old.weapon_slot;
    }
  }
  if(rebuild){
    std::vector<Draw> next;std::vector<GLuint> textures;
    std::vector<std::size_t> mapping;std::map<std::string,GLuint> cache;
    try{
      std::size_t total=0;
      for(std::size_t i=0;i<parts.size();++i){const auto& part=parts[i];const auto& geometry=*part.geometry;
        if(geometry.primitives.size()!=part.materials->size()||geometry.positions.size()>65536)
          throw std::runtime_error("Equipped material/topology count is invalid");
        for(std::size_t j=0;j<geometry.primitives.size();++j){const auto& primitive=geometry.primitives[j];
          if(primitive.collada_type||primitive.indices.size()%3||primitive.indices.size()>3000000||
             total>1000000-geometry.positions.size())
            throw std::runtime_error("Equipped triangle/vertex budget exceeded");
          total+=geometry.positions.size();
          const auto material=part.materials->at(j);
          if(material>=part.material_table->size()||part.material_table->at(material).id!=primitive.material_symbol)
            throw std::runtime_error("Equipped primitive material binding differs");
          auto attribute=[&](unsigned slot)->const dh2::skinning::VisualAttributeV6*{
            const auto index=primitive.attributes.at(slot);
            if(index<0)return nullptr;
            if(std::size_t(index)>=geometry.attributes.size())throw std::runtime_error("Equipped attribute index exceeds geometry");
            const auto& value=geometry.attributes[index];
            if(!value.components||value.components>4||value.values.size()!=geometry.positions.size()*value.components)
              throw std::runtime_error("Equipped attribute stream is incomplete");
            return &value;
          };
          const auto* uv=attribute(4);const auto* color_stream=attribute(2);
          if(uv&&uv->components<2)throw std::runtime_error("Equipped UV stream has fewer than two components");
          std::vector<Vertex> vertices(geometry.positions.size());
          for(std::size_t k=0;k<vertices.size();++k){auto& vertex=vertices[k];
            std::copy(part.positions->at(k).begin(),part.positions->at(k).end(),vertex.p);
            if(uv)std::copy_n(uv->values.data()+k*uv->components,2,vertex.uv);
            std::fill(vertex.color,vertex.color+4,1.f);
            if(color_stream)for(unsigned c=0;c<color_stream->components;++c)
              vertex.color[c]=color_stream->values[k*color_stream->components+c]/(color_stream->type==1?255.f:1.f);
          }
          std::vector<std::uint16_t> indices;indices.reserve(primitive.indices.size());
          for(auto index:primitive.indices){if(index>=vertices.size()||index>65535)
              throw std::runtime_error("Equipped index exceeds vertex/GLES2 bounds");
            indices.push_back(static_cast<std::uint16_t>(index));}
          next.emplace_back();auto& draw=next.back();draw.material=part.material_table->at(material);
          draw.placement=part.world;draw.environment=true;draw.count=static_cast<GLsizei>(indices.size());
          draw.cpu_vertices=std::move(vertices);mapping.push_back(i);
          draw.diffuse=upload(assets,draw.material.diffuse,cache,textures,&original);
          draw.alpha=upload(assets,draw.material.alpha_map,cache,textures,&original);
          glGenBuffers(1,&draw.vertices);glBindBuffer(GL_ARRAY_BUFFER,draw.vertices);
          glBufferData(GL_ARRAY_BUFFER,draw.cpu_vertices.size()*sizeof(Vertex),draw.cpu_vertices.data(),GL_DYNAMIC_DRAW);
          glGenBuffers(1,&draw.indices);glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,draw.indices);
          glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size()*sizeof(std::uint16_t),indices.data(),GL_STATIC_DRAW);
          check("Equipped source buffer upload");
        }
      }
    }catch(...){release(next,textures);throw;}
    release(equipment_draws,equipment_images);equipment_draws=std::move(next);
    equipment_images=std::move(textures);equipment_draw_part=std::move(mapping);
    __android_log_print(ANDROID_LOG_INFO,"DH2Native","Equipped GPU graph rebuilt | parts %zu | draws %zu | textures %zu | live source snapshot",
      parts.size(),equipment_draws.size(),equipment_images.size());
  }else{
    for(std::size_t i=0;i<equipment_draws.size();++i){auto& draw=equipment_draws[i];const auto& part=parts.at(equipment_draw_part.at(i));
      if(draw.cpu_vertices.size()!=part.positions->size())throw std::runtime_error("Equipped pose size differs from retained mesh");
      draw.placement=part.world;if(part.pose_revision==equipment_parts.at(equipment_draw_part.at(i)).pose_revision)continue;
      for(std::size_t k=0;k<part.positions->size();++k)std::copy(part.positions->at(k).begin(),part.positions->at(k).end(),draw.cpu_vertices[k].p);
      glBindBuffer(GL_ARRAY_BUFFER,draw.vertices);
      glBufferSubData(GL_ARRAY_BUFFER,0,draw.cpu_vertices.size()*sizeof(Vertex),draw.cpu_vertices.data());
      dh2::perf::state.uploads+=draw.cpu_vertices.size()*sizeof(Vertex);
    }
  }
  equipment_parts.resize(parts.size());
  for(std::size_t i=0;i<parts.size();++i){const auto& source=parts[i];auto& retained=equipment_parts[i];retained.retention=source.retention;retained.geometry=source.geometry;retained.material_table=source.material_table;retained.materials=source.materials;retained.category=source.category;retained.module=source.module;retained.weapon_slot=source.weapon_slot;retained.pose_revision=source.pose_revision;}
}
void bind_player_equipment(AAssetManager* assets,const std::vector<std::uint8_t>& prince,bool restore){
  dh2::skinning::VisualSkinResourcesV6 resource;std::string error;
  if(!resource.load(prince,error))throw std::runtime_error("Equipped player resource failed: "+error);
  if(restore&&player_equipment){
    if(player_equipment->properties()!=prince_combat.property_owner||!equipment_platform||
       equipment_platform->world!=live_script_world)
      throw std::runtime_error("Retained equipment belongs to a different player world");
    equipment_platform->manager=assets;equipment_platform->cache.manager(assets);
    if(!player_equipment->rebind_visual(resource.borrow(),current_scene,error))
      throw std::runtime_error("Retained equipment visual recreation failed: "+error);
  }else{
    clear_npc_death_owners_v2();player_skills_runtime.reset();
    player_equipment.reset();equipment_platform.reset();
    equipment_platform=std::make_unique<EquipmentPlatform>(live_script_world,assets);
     prepare_player_manager_boot_v25(live_script_world,{0,0,0,true,0,nullptr});
    dh2::player::PlayerEquipmentRenderInputsV1 input;
    input.design=live_script_world->design->borrow();input.properties=prince_combat.property_owner;
    equipment_random={0xD22026u,0};input.random=&equipment_random;
    input.character=live_script_world->player_object->identity;
    input.potion_capacity=static_cast<std::int8_t>(std::uint8_t(std::max(0,prince_combat.properties().resolved[194]>>8)));
    input.language_pack=0; // The development scene's selected English pack.
    input.resources=resource.borrow();input.live_scene=&current_scene;
    input.assets={equipment_platform.get(),EquipmentPlatform::asset};
    input.debug=live_script_world->debug.get();input.debug_files=live_script_world->debug_files;
    input.text_environment.localization={equipment_platform.get(),EquipmentPlatform::open_text,
      EquipmentPlatform::close_text,EquipmentPlatform::debug_text,EquipmentPlatform::constant,nullptr,nullptr};
    input.world={equipment_platform.get(),EquipmentPlatform::query};
    player_equipment=std::make_unique<dh2::player::PlayerEquipmentRenderOwnerV1>(std::move(input));
    if(!player_equipment->initialize(error))throw std::runtime_error("Original player equipment initialization failed: "+error);
  }
  const std::vector<dh2::skinning::VisualDrawViewV32>* parts{};
  if(!player_equipment->draw_views(parts,error))throw std::runtime_error("Equipped player draw graph failed: "+error);
  sync_equipment_draws(*parts,assets,equipment_platform->cache);
  const auto* inventory=player_equipment->inventory();
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Native player equipment connected | identity %016llx | items %zu | potions %d | selected %d | retained %u | property backing %016llx | RNG calls %u",
    static_cast<unsigned long long>(inventory->character()),inventory->items().size(),inventory->num_potions(),inventory->current_equipment(),unsigned(restore),
    static_cast<unsigned long long>(reinterpret_cast<std::uintptr_t>(inventory->properties().get())),equipment_random.calls);
  for(std::size_t i=0;i<inventory->items().size();++i){const auto& item=*inventory->items()[i]->item;
    __android_log_print(ANDROID_LOG_INFO,"DH2Native","Native inventory item | index %zu | ID %d | quantity %d | name %s | slots %d %d",
      i,item.id,item.signed_quantity(),item.name.c_str(),int(inventory->items()[i]->slots[0]),int(inventory->items()[i]->slots[1]));
  }
}
std::array<float,3> cross(const std::array<float,3>& a,const std::array<float,3>& b){return {a[1]*b[2]-a[2]*b[1],a[2]*b[0]-a[0]*b[2],a[0]*b[1]-a[1]*b[0]};}
void normalize(std::array<float,3>& a){float n=std::sqrt(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]);for(float& x:a)x/=n;}
float dot(const std::array<float,3>& a,const std::array<float,3>& b){return a[0]*b[0]+a[1]*b[1]+a[2]*b[2];}
Matrix camera(int width,int height){
  const float aspect=float(width)/height,tan=.41421356f;
  const float distance=radius*1.15f*std::sqrt(1+1/(tan*tan*std::min(1.f,aspect)*std::min(1.f,aspect)))*zoom;
  std::array<float,3> eye{center[0]+distance*std::cos(yaw)*std::cos(pitch),center[1]+distance*std::sin(yaw)*std::cos(pitch),center[2]+distance*std::sin(pitch)};
  std::array<float,3> f{center[0]-eye[0],center[1]-eye[1],center[2]-eye[2]};normalize(f);
  auto s=cross(f,{0,0,1});normalize(s);auto u=cross(s,f);
  Matrix view{s[0],u[0],-f[0],0,s[1],u[1],-f[1],0,s[2],u[2],-f[2],0,-dot(s,eye),-dot(u,eye),dot(f,eye),1};
  submitted_view=view;submitted_eye=eye;
  const float near=world_mode?50.f:std::max(.01f,distance-radius*1.5f),far=world_mode?12000.f:distance+radius*3;
  Matrix projection{1/(tan*aspect),0,0,0,0,1/tan,0,0,0,0,-(far+near)/(far-near),-1,0,0,-2*far*near/(far-near),0};
  return dh2::scene::multiply(projection,view);
}
#include "renderer_front_visual_v87.inc"
}
#include "renderer_front_exports_v87.inc"
#include "renderer_character_inventory_preview_v4.inc"
bool character_menu_player_v4(std::int32_t index,bool flag,bool local,std::uintptr_t& identity,std::string& error){
 identity=0;if(!player_skills_runtime||!player_skills_runtime->player_manager){error="Required actual current PlayerManager menu receiver";return false;}
 dh2::player::PlayerInfoFieldsV1* info{};auto& manager=player_skills_runtime->player_manager->manager();
 const bool delivered=local?manager.get_local_player(index,flag,info,error):manager.get_player(index,flag,info,error);
 if(!delivered)return false;identity=info?info->character660:0;return true;
}
#include "renderer_canonical_world_v4.inc"
#include "renderer_native_level_c1_v25.inc"
#include "renderer_loot_gpu_v27.inc"
#include "renderer_character_application_v4.inc"
bool borrow_current_native_level_v27(dh2::loader::CanonicalCurrentLevelBorrowV1& out,std::string& error){
 // Game.GetCurrentLevel reads this BSS global even before GS construction.
 // Its genuine initial NULL is distinct from the demo's unpublised C1 owner.
 return dh2::loader::borrow_current_canonical_level_v1(renderer_gs_globals_v27->borrow(renderer_gs_globals_v27),out,error);
}
void reset_context(){begin_gameplay_camera_release_v20();class_preview_actors.clear();menu_background=class_scene=false;clear_retained_loot_gpu_v27(true);clear_combat_fx_gpu(true);if(player_equipment)player_equipment->detach_visual();equipment_draws.clear();equipment_images.clear();equipment_parts.clear();equipment_draw_part.clear();native_actor_ready=false;actor_world.clear();prince_body={};finish_gameplay_camera_release_v20();resume_world=resume_world||world_mode;if(world_mode){saved_actors.clear();for(const auto& group:object_groups)for(const auto& actor:group.instances)if(actor.kind==1)saved_actors.push_back(actor);}world_mode=false;move_x=move_y=0;draws.clear();images.clear();object_groups.clear();world_objects.clear();prince_locomotion=dh2::actor::BlendedPlayback{};prince_visual={};prince_attack_clips.clear();prince_animation_bank={};scene_clock=0;inspected_object=-1;current_scene={};player=dh2::animation::Player{};walk_player=dh2::animation::Player{};level={};program=0;enabled=false;}
void deactivate(){begin_gameplay_camera_release_v20();release_class_previews();menu_background=class_scene=false;clear_retained_loot_gpu_v27(false);clear_combat_fx_gpu(false);clear_npc_death_owners_v2();native_actor_ready=false;actor_world.clear();prince_body={};finish_gameplay_camera_release_v20();player_skills_runtime.reset();release_canonical_world_v4(live_script_world);if(player_equipment)player_equipment->detach_visual();player_equipment.reset();equipment_platform.reset();release(equipment_draws,equipment_images);equipment_parts.clear();equipment_draw_part.clear();enabled=false;world_mode=false;resume_world=false;move_x=move_y=0;}
bool active(){return enabled;}
void set_character_panel_open(bool value){character_panel_open=value&&world_mode;move_x=move_y=0;source_hud_heading_active=false;last_frame=std::chrono::steady_clock::now();__android_log_print(ANDROID_LOG_INFO,"DH2Native","Character overlay simulation pause | open %d | same live World",character_panel_open);}
void set_enemy_ai(bool value){enemy_ai_enabled=value;__android_log_print(ANDROID_LOG_INFO,"DH2Native","Enemy AI configured | automatic melee %d",value);}
void orbit(float dx,float dy,float factor){yaw+=dx;pitch=std::clamp(pitch+dy,-1.4f,1.4f);zoom=std::clamp(zoom*factor,.35f,4.f);}
void set_time(int milliseconds){
  frozen_cursor_logged=false;
  // World inspection pauses the complete live actor pipeline at its composed
  // pose. Arbitrary millisecond sampling remains a separate model-preview tool.
  if(world_mode){
   frozen=milliseconds>=0;last_frame=std::chrono::steady_clock::now();
   std::uint64_t pose=14695981039346656037ull;
   auto digest=[&](const float* values,unsigned count){for(unsigned i=0;i<count;++i){std::uint32_t word;std::memcpy(&word,values+i,4);for(unsigned j=0;j<4;++j){pose^=(word>>(j*8))&255;pose*=1099511628211ull;}}};
   for(const auto& node:current_scene.graph){digest(node.translation,3);digest(node.quaternion,4);digest(node.scale,3);digest(node.world.data(),16);}
   const auto body=prince_body.body?prince_body.body->GetPosition():b2Vec2(actor_position[0]*.01f,actor_position[1]*.01f);
   __android_log_print(ANDROID_LOG_INFO,"DH2Native","Prince blended inspection | frozen %u | scene %u | Step %u | actor %u | clip %d | ms %d | body %.6g %.6g | pose %016llx",unsigned(frozen),unsigned(scene_clock),native_physics_steps,native_actor_frames,prince_locomotion.current_clip(),prince_locomotion.current_timeline().current_ms,body.x,body.y,static_cast<unsigned long long>(pose));
   return;
  }
  if(milliseconds<0){epoch=std::chrono::steady_clock::now()-std::chrono::milliseconds(sampled_ms-player.start);frozen=false;}
  else{sampled_ms=std::clamp(milliseconds,player.start,player.end);frozen=true;}
}
std::string load(const std::uint8_t* bytes,std::size_t size,AAssetManager* assets){
  release_class_previews();menu_background=class_scene=false;
  if(player_equipment)player_equipment->detach_visual();
  release(equipment_draws,equipment_images);equipment_parts.clear();equipment_draw_part.clear();
  std::vector<Draw> candidate;std::vector<GLuint> textures;
  try{
    if(!assets)throw std::runtime_error("Asset manager unavailable");
    dh2::resources::BresView view{};if(dh2_bres_open(&view,bytes,size)!=dh2::resources::BresError::ok)throw std::runtime_error("BRES header rejected");
    dh2::scene::Scene scene;std::string error;if(!dh2::scene::load(view,scene,error))throw std::runtime_error(error);
    dh2::animation::Player candidate_player;
    auto prince=std::find_if(scene.graph.begin(),scene.graph.end(),[](const dh2::scene::Node& n){return n.id=="prince_modular-node";});
    if(prince!=scene.graph.end()){
      // Preview equipment selection is explicit. The game normally chooses
      // modular controllers dynamically; the serialized scene lists a shadow.
      const auto node=unsigned(prince-scene.graph.begin());scene.instances.clear();
      for(unsigned i=0;i<dh2_bres_library_count(&view,dh2::resources::Library::controller);++i){
        dh2::skinning::Skin skin;if(!dh2::skinning::load(view,i,scene,skin,error))throw std::runtime_error(error);
        if(skin.id.find("_default_warrior-mesh-skin")==std::string::npos)continue;
        dh2::assets::Mesh mesh{};dh2_mesh_open(&mesh,&view,skin.geometry);
        dh2::scene::Instance instance{prince->id,node,skin.geometry,prince->world,{}};instance.controller=i;
        for(unsigned j=0;j<mesh.primitives;++j){dh2::assets::Primitive primitive{};dh2_mesh_primitive(&mesh,j,&primitive);
          auto material=std::find_if(scene.materials.begin(),scene.materials.end(),[&](const dh2::scene::Material& m){return m.id==primitive.material;});
          if(material==scene.materials.end())throw std::runtime_error("Unresolved equipment material");instance.materials.push_back(material-scene.materials.begin());}
        scene.instances.push_back(std::move(instance));
      }
      if(scene.instances.size()!=4)throw std::runtime_error("Incomplete warrior equipment preview");
      auto clip=read(assets,"prince_menu_idle_knight.bdae","animations");
      if(!candidate_player.load(clip.data(),clip.size(),scene,error))throw std::runtime_error("Animation load failed: "+error);
      if(!candidate_player.sample(scene,0,error))throw std::runtime_error(error);
    }else if(!candidate_player.load(bytes,size,scene,error))throw std::runtime_error("Animation load failed: "+error);
    if(!program)create_program();std::map<std::string,GLuint> cache;
    float low[3]{INFINITY,INFINITY,INFINITY},high[3]{-INFINITY,-INFINITY,-INFINITY};unsigned total=0,triangles=0;
    for(const auto& instance:scene.instances){
      dh2::assets::Mesh mesh{};if(dh2_mesh_open(&mesh,&view,instance.geometry)!=dh2::assets::Error::ok)throw std::runtime_error("Geometry rejected");
      if(mesh.primitives!=instance.materials.size())throw std::runtime_error("Material binding count differs from primitive count");
      for(unsigned j=0;j<mesh.primitives;++j){
        dh2::assets::Primitive p{};dh2_mesh_primitive(&mesh,j,&p);
        if(p.collada_type||p.index_count%3||mesh.vertices>65536||p.index_count>3000000)throw std::runtime_error("Unsupported primitive topology or size");
        if(total>1000000-mesh.vertices)throw std::runtime_error("Vertex budget exceeded");total+=mesh.vertices;triangles+=p.index_count/3;
        dh2::assets::Attribute a{},uv{},color_attribute{};
        if(dh2_mesh_attribute(&mesh,p.attributes[0],&a)!=dh2::assets::Error::ok||a.components<3)throw std::runtime_error("Missing position attribute");
        const bool have_uv=dh2_mesh_attribute(&mesh,p.attributes[4],&uv)==dh2::assets::Error::ok&&uv.components>=2;
        const bool have_color=dh2_mesh_attribute(&mesh,p.attributes[2],&color_attribute)==dh2::assets::Error::ok;
        Draw d;d.node=instance.node_index;d.material=scene.materials.at(instance.materials[j]);
        if(instance.controller>=0&&!dh2::skinning::load(view,instance.controller,scene,d.skin,error))throw std::runtime_error(error);
        if(d.material.id!=p.material)throw std::runtime_error("Material binding order/symbol mismatch");
        candidate.push_back(std::move(d));auto& batch=candidate.back();
        batch.diffuse=upload(assets,batch.material.diffuse,cache,textures);batch.alpha=upload(assets,batch.material.alpha_map,cache,textures);
        std::vector<Vertex> vertices(mesh.vertices);
        for(unsigned k=0;k<mesh.vertices;++k){auto& vertex=vertices[k];float raw[4]{};dh2_attribute_read(&a,k,raw);
          for(unsigned row=0;row<3;++row){float x=instance.world[12+row];for(unsigned c=0;c<3;++c)x+=instance.world[c*4+row]*raw[c];
            if(!std::isfinite(x))throw std::runtime_error("Nonfinite vertex position");vertex.p[row]=raw[row];
            if(batch.skin.nodes.empty()){low[row]=std::min(low[row],x);high[row]=std::max(high[row],x);}}
          if(have_uv){dh2_attribute_read(&uv,k,raw);std::copy(raw,raw+2,vertex.uv);}else vertex.uv[0]=vertex.uv[1]=0;
          std::fill(vertex.color,vertex.color+4,1);
          if(have_color){dh2_attribute_read(&color_attribute,k,raw);for(unsigned c=0;c<color_attribute.components;++c)vertex.color[c]=raw[c]/(color_attribute.type==1?255.f:1.f);}
        }
        if(!batch.skin.nodes.empty()){
          batch.rest_positions.resize(vertices.size());for(unsigned k=0;k<vertices.size();++k)std::copy(vertices[k].p,vertices[k].p+3,batch.rest_positions[k].begin());
          std::vector<dh2::skinning::Matrix> matrices;std::vector<std::array<float,3>> deformed;
          if(!dh2::skinning::palette(batch.skin,scene,matrices,error)||!dh2::skinning::positions(batch.skin,matrices,batch.rest_positions,deformed,error))throw std::runtime_error(error);
          for(unsigned k=0;k<vertices.size();++k)for(unsigned row=0;row<3;++row){vertices[k].p[row]=deformed[k][row];low[row]=std::min(low[row],deformed[k][row]);high[row]=std::max(high[row],deformed[k][row]);}
          batch.cpu_vertices=vertices;
        }
        std::vector<std::uint16_t> indices(p.index_count);for(unsigned k=0;k<p.index_count;++k){std::uint32_t x;dh2_index_read(&p,k,&x);indices[k]=x;}
        batch.count=p.index_count;glGenBuffers(1,&batch.vertices);glBindBuffer(GL_ARRAY_BUFFER,batch.vertices);glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(Vertex),vertices.data(),batch.skin.nodes.empty()?GL_STATIC_DRAW:GL_DYNAMIC_DRAW);
        glGenBuffers(1,&batch.indices);glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,batch.indices);glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size()*2,indices.data(),GL_STATIC_DRAW);check("Mesh buffer upload");
      }
    }
    const float extent[3]{high[0]-low[0],high[1]-low[1],high[2]-low[2]};
    float next_radius=std::sqrt(extent[0]*extent[0]+extent[1]*extent[1]+extent[2]*extent[2])*.5f;
    if(!std::isfinite(next_radius)||next_radius<.001f)throw std::runtime_error("Degenerate scene bounds");
    begin_gameplay_camera_release_v20();
    release_objects(object_groups);world_objects.clear();inspected_object=-1;release(draws,images);draws=std::move(candidate);images=std::move(textures);radius=next_radius;
    native_actor_ready=false;actor_world.clear();prince_body={};
    finish_gameplay_camera_release_v20();
    world_mode=false;resume_world=false;move_x=move_y=0;source_hud_heading=false;source_hud_heading_active=false;
    for(unsigned i=0;i<3;++i)center[i]=(low[i]+high[i])*.5f;
    yaw=-1.57f;pitch=.35f;zoom=1;enabled=true;
    current_scene=std::move(scene);player=std::move(candidate_player);epoch=std::chrono::steady_clock::now();sampled_ms=player.start;frozen=false;animation_failed=false;frozen_cursor_logged=false;
    char report[384];std::snprintf(report,sizeof(report),"3D upload OK | %zu draws | %u triangles | %zu textures\n%u animation tracks | %u skipped | Preview lighting. Drag to orbit.",draws.size(),triangles,images.size(),player.track_count(),player.skipped);
    __android_log_print(ANDROID_LOG_INFO,"DH2Native","%s | nodes %u | skipped nongeometry instances %u | skin draws %zu | segments %u",report,current_scene.nodes,current_scene.ignored_instances,std::count_if(draws.begin(),draws.end(),[](const Draw& d){return !d.skin.nodes.empty();}),player.segment_count());return report;
  }catch(const std::exception& e){release(candidate,textures);__android_log_print(ANDROID_LOG_ERROR,"DH2Native","Model load failed: %s",e.what());return std::string("Model load failed: ")+e.what();}
}
void move_axis(float x,float y){
  source_hud_heading=false;source_hud_heading_active=false;
  if(!std::isfinite(x)||!std::isfinite(y))return;
  move_x=std::clamp(x,-1.f,1.f);move_y=std::clamp(y,-1.f,1.f);float length=std::hypot(move_x,move_y);
  if(length>1){move_x/=length;move_y/=length;}
  if(length>.08f)inspected_object=-1;
  if(world_mode&&length<.01f){
    __android_log_print(ANDROID_LOG_INFO,"DH2Native","Player position %.4f %.4f %.4f | moved %u | blocked %u",actor_position[0],actor_position[1],actor_position[2],movement_steps,blocked_steps);
    __android_log_print(ANDROID_LOG_INFO,"DH2Native","Player facing | angle %.9g | native updates %u",heading,native_heading_updates);
  }
}
void focus_object(int index){
  inspected_object=-1;
  if(!world_mode||index<0||unsigned(index)>=world_objects.size())return;
  inspected_object=index;const auto& object=world_objects[index];
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Inspect object %d | %s | %s | room %u | position %.4f %.4f %.4f",index,object.model.c_str(),object.name.c_str(),object.room,object.position[0],object.position[1],object.position[2]);
  for(const auto& group:object_groups)for(const auto& actor:group.instances)if(actor.kind==1&&actor.room==object.room&&actor.name==object.name)__android_log_print(ANDROID_LOG_INFO,"DH2Native","Combat actor state | %s | HP %d | MP %d | dead %u | combo %u | state %s | target %d | hits %u",actor.name.c_str(),actor.properties().resolved[36],actor.properties().resolved[41],actor.combat_state().dead,actor.combat_state().combo_hits,actor.state.c_str(),actor.combat_target,combat_hits);
}
std::string set_object_state(int index,const std::string& state){
 if(!world_mode||index<0||unsigned(index)>=world_objects.size())return "Actor state requires a valid object index";
 const auto& target=world_objects[index];
 for(auto& group:object_groups){for(auto& actor:group.instances){if(actor.room==target.room&&actor.name==target.name){
  if(actor.kind!=1)return "Scenery has no character state";
  if(actor.combat_state().dead&&state!="Died")return "Dead actor cannot enter a live state";
  if(state=="Injured"){
   if(!actor.script||!actor.script->injury)return "Original NPC injury owner unavailable";
   const auto result=actor.script->injury->injure(0x100000001ull);
   __android_log_print(ANDROID_LOG_INFO,"DH2Native","Developer source NPC injury | %s | status %d | state %d | HP %d | gate %.9g",actor.name.c_str(),result,actor.script->machine->state().current,actor.properties().resolved[36],double(actor.script->injury_gate));
   return result==1?"Original NPC injury delivered":"Original NPC injury failed";
  }
  if(state!="Idle"&&state!="Walk"&&state!="Attack"&&state!="Died")return "Actor state is not bundled";
  auto* sequence=dh2::data::animation_state(actor_animation_tables,group.animation_table,state);if(!sequence)return "Original actor state is absent";
  std::string error;if(!actor.scheduler.start(actor_animation_tables,sequence-actor_animation_tables.sequences.data(),actor_random,error))return error;
  actor.cursor=0;actor.completions=0;actor.state=state;actor.event_cursor={};actor.animation_events=0;
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Actor state selected | index %d | %s | %s | state %s | clip %d | layers %zu",index,actor.name.c_str(),actor.model.c_str(),state.c_str(),actor.scheduler.clip().anim,actor.scheduler.frames().size());
  return "Original actor state: "+state;
 }}}
 return "Actor instance is absent";
}
std::string set_combat_target(int index,int target){
 if(!world_mode||index<0||unsigned(index)>=world_objects.size()||target<-2||(target>=0&&unsigned(target)>=world_objects.size())||index==target)return "Combat target indices are invalid";
 const auto& source=world_objects[index];
 if(target==-2&&prince_combat.life().dead)return "Combat target is dead";
 if(target>=0&&world_objects[target].kind!=1)return "Combat target is scenery";
 for(auto& group:object_groups)for(auto& actor:group.instances)if(actor.room==source.room&&actor.name==source.name){
  if(actor.kind!=1||actor.combat_state().dead)return "Combat source is not a living character";
  if(target>=0){const auto& record=world_objects[target];for(const auto& other_group:object_groups)for(const auto& other:other_group.instances)if(other.room==record.room&&other.name==record.name&&other.combat_state().dead)return "Combat target is dead";}
  actor.combat_target=target;
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Combat target selected | index %d | %s | target %d | supplied development target",index,actor.name.c_str(),target);
  return target==-1?"Combat target cleared":"Combat target selected";
 }
 return "Combat source is absent";
}
void add_combat_threat(AggroStorage& owner,AggroStorage& target,std::uint64_t owner_id,std::uint64_t target_id,float amount,unsigned facts){
 auto& outgoing=owner.outgoing_table;auto& incoming=target.incoming_table;
 std::uint32_t bits;std::memcpy(&bits,&amount,4);const dh2::data::AggroRequest request{&outgoing,&incoming,owner_id,target_id,bits,facts};dh2::data::AggroChange result{};
 if(dh2_aggro_apply(&result,&request,dh2::data::aggro_add)){enabled=false;__android_log_print(ANDROID_LOG_ERROR,"DH2Native","Combat aggression application failed");return;}
 __android_log_print(ANDROID_LOG_INFO,"DH2Native","Combat aggression | owner %llu | target %llu | amount bits %08x | delta bits %08x | outgoing %u | incoming %u | requests %u | callback services pending",static_cast<unsigned long long>(owner_id),static_cast<unsigned long long>(target_id),bits,result.returned_bits,outgoing.count,incoming.count,result.requests);
}
dh2::data::AiRangeResult actor_player_range(const ObjectActor& actor){
 const auto* npc=dh2::data::ai_props(actor_ai_tables,actor.properties().resolved[1]);const auto* prince=dh2::data::ai_props(actor_ai_tables,prince_combat.properties().resolved[1]);dh2::data::AiRangeResult result{};if(!npc||!prince)return result;
 const dh2::data::AiRangeRequest request{{actor.position[0],actor.position[1],actor.position[2]},{actor_position[0],actor_position[1],actor_position[2]},npc->melee_radius,prince->melee_radius,npc->view_radius};dh2_ai_range(&result,&request);return result;
}
void update_enemy(ObjectActor& actor,int table){
 if(!enemy_ai_enabled||frozen||character_panel_open||actor.combat_state().dead)return;
 const auto* props=dh2::data::ai_props(actor_ai_tables,actor.properties().resolved[1]);if(!props||props->type!=4||props->script!="monster")return;
 auto range=actor_player_range(actor);
 if(actor.combat_target==-1&&!prince_combat.life().dead&&dh2::data::ai_enemy(actor_ai_tables,actor.properties().resolved[0],prince_combat.properties().resolved[0],false,true)){
  // Current scene has one hostile candidate. The original collision-query
  // backend/ordering is pending; supply that candidate's distance here.
  float distance;std::memcpy(&distance,&range.distance_bits,4);const float view=actor.aggro.outgoing_table.count?props->view_radius:props->view_radius_no_aggro;
  if(view*view>distance){actor.combat_target=-2;actor.target_alive=1;actor.target_sight=range.sight;actor.target_seeking=true;
   __android_log_print(ANDROID_LOG_INFO,"DH2Native","Enemy spotted | %s | target Prince | AI %d | faction %d | distance squared bits %08x | view %.9g | original OnEnemySpotted target assignment",actor.name.c_str(),actor.properties().resolved[1],actor.properties().resolved[0],range.distance_bits,double(view));}
 }
 if(actor.combat_target!=-2)return;
 const unsigned facts=dh2::data::ai_target_present|dh2::data::ai_targetable|dh2::data::ai_callback_clears_dead|dh2::data::ai_callback_clears_sight|(prince_combat.life().dead?0u:dh2::data::ai_target_alive)|(range.sight?dh2::data::ai_target_sight:0u)|(range.melee?dh2::data::ai_target_melee_range:0u);
 const dh2::data::AiTargetRequest request{actor.state=="Attack"?5:3,facts,actor.target_alive,actor.target_sight};dh2::data::AiTargetResult result{};
 if(dh2_ai_target_update(&result,&request)){enabled=false;return;}actor.target_alive=result.alive;actor.target_sight=result.sight;
 auto select=[&](const char* name){auto* sequence=dh2::data::animation_state(actor_animation_tables,table,name);std::string error;if(!sequence||!actor.scheduler.start(actor_animation_tables,sequence-actor_animation_tables.sequences.data(),actor_random,error)){enabled=false;__android_log_print(ANDROID_LOG_ERROR,"DH2Native","Enemy controller animation failed");return;}actor.state=name;actor.cursor=0;actor.completions=0;actor.event_cursor={};actor.animation_events=0;};
 if(!result.target_present){actor.combat_target=-1;if(actor.ai_attack){select("Idle");actor.ai_attack=false;}
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Enemy target cleared | %s | event %u | Prince dead %u | controller stopped | pursuit backend pending",actor.name.c_str(),result.events[result.event_count-1],prince_combat.life().dead);return;}
 if(result.event_count&&result.events[result.event_count-1]==17&&(actor.state!="Attack"||!actor.scheduler.active())){
  select("Attack");actor.ai_attack=true;__android_log_print(ANDROID_LOG_INFO,"DH2Native","Enemy melee selected | %s | target Prince | event 17 | distance squared bits %08x | clip %d | native controller adapter",actor.name.c_str(),range.distance_bits,actor.scheduler.clip().anim);
 }
 // Full pursuit, seeking, attack-delay/FSM and already-attacking target
 // switching remain separate reconstruction work.
}
void bind_player_combat_equipment(dh2::data::CombatantView& view){
 if(!player_equipment||!player_equipment->ready())throw std::runtime_error("Player combat equipment owner unavailable");
 std::string error;
 if(!player_equipment->combat_view(view,error))throw std::runtime_error("Player combat equipment rejected: "+error);
}
void apply_actor_to_player(ObjectActor& attacker,const dh2::data::CombatEventAction& action){
 if(prince_combat.life().dead)return;
 if(attacker.ai_attack&&enemy_ai_enabled&&!actor_player_range(attacker).melee)return;
 // The prototype supplies state 3 when standing and 13 while walking.
 // Original SM_IsIdle(false) accepts both; do not suppress its hit reaction
 // merely because movement input is held. Full original FSM producers remain pending.
 const bool idle=dh2_character_state_is_idle(prince_state.current,0)==1;
  dh2::data::CombatantView av{attacker.properties().resolved.data(),-1,-1,0,0,0,5,attacker.combat_state().combo_hits},dv{prince_combat.properties().resolved.data(),-1,-1,0,0,0,prince_state.current,prince_combat.life().combo_hits};
  bind_player_combat_equipment(dv);
 dh2::data::CombatResult result;dh2::data::MonsterApplication applied;auto ap=dh2::data::property_view(actor_property_rules,attacker.properties()),dp=dh2::data::property_view(actor_property_rules,prince_combat.properties());const dh2::data::MonsterApplicationRequest request{&result,&ap,&dp,&attacker.combat_state(),&prince_combat.life()};
 const unsigned aggro_facts=dh2::data::aggro_owner_player|(attacker.combat_state().dead?dh2::data::aggro_target_dead:0u);
 if(dh2_combat_melee(&result,&av,&dv,&combat_random,action.offhand,0)||dh2_combat_apply_monster_to_player(&applied,&request,idle)){__android_log_print(ANDROID_LOG_ERROR,"DH2Native","Player defender application failed");enabled=false;return;}
  if(applied.hit_called)add_combat_threat(prince_combat.aggro,attacker.aggro,0x100000001ull,attacker.identity,applied.threat,aggro_facts);
  apply_combat_text_v1(result,attacker.identity,0x100000001ull);
 ++combat_hits;++prince_combat.received;
 __android_log_print(ANDROID_LOG_INFO,"DH2Native","Prince damage received | attacker %s | attempt %u | result %d %d %d %d %d %d %u %u %d %d | HP %d %d | dead %u | combo %u | RNG %u %u | statuses %u | low health armed %u | cue %u | checksum %016llx",attacker.name.c_str(),prince_combat.received,result.amount,result.dot_element,result.dot_duration,result.dot_amount,result.hp_leech,result.mp_leech,result.outcomes,result.mask,result.weapon_category,result.element,applied.health.before,applied.health.after,prince_combat.life().dead,attacker.combat_state().combo_hits,combat_random.seed,combat_random.calls,applied.status_requests,prince_combat.life().low_health_armed,applied.health.low_health_cue,static_cast<unsigned long long>(snapshot_checksum(prince_combat.properties().resolved)));
 if(applied.health.low_health_cue)__android_log_print(ANDROID_LOG_INFO,"DH2Native","Player low health request | HP %d | maximum %d | audio pending",applied.health.after,prince_combat.properties().resolved[38]);
 if(applied.status_requests)__android_log_print(ANDROID_LOG_INFO,"DH2Native","Player status services pending | requests %u",applied.status_requests);
 if(applied.health.kill_requested){prince_combat.pending_death=true;prince_combat.death_target=attacker.identity;move_x=move_y=0;if(native_actor_ready)request_prince_death();}
}
void apply_actor_attack(ObjectActor& attacker,const dh2::data::CombatEventAction& action){
 if(action.kind!=dh2::data::CombatEventKind::melee||attacker.combat_state().dead)return;
 if(attacker.combat_target==-2){apply_actor_to_player(attacker,action);return;}
 if(attacker.combat_target<0)return;
 const auto& target_record=world_objects.at(attacker.combat_target);ObjectActor* defender=nullptr;
 for(auto& group:object_groups)for(auto& actor:group.instances)if(actor.room==target_record.room&&actor.name==target_record.name)defender=&actor;
 if(!defender||defender->kind!=1||defender->combat_state().dead)return;
 dh2::data::CombatantView av{attacker.properties().resolved.data(),-1,-1,0,0,0,5,attacker.combat_state().combo_hits},dv{defender->properties().resolved.data(),-1,-1,0,0,0,defender->state=="Attack"?5:-1,defender->combat_state().combo_hits};
 dh2::data::CombatResult result;dh2::data::MonsterApplication applied;
 auto ap=dh2::data::property_view(actor_property_rules,attacker.properties()),dp=dh2::data::property_view(actor_property_rules,defender->properties());
 const dh2::data::MonsterApplicationRequest request{&result,&ap,&dp,&attacker.combat_state(),&defender->combat_state()};
 const unsigned aggro_facts=(defender->combat_state().dead?dh2::data::aggro_owner_dead:0u)|(attacker.combat_state().dead?dh2::data::aggro_target_dead:0u);
 if(dh2_combat_melee(&result,&av,&dv,&combat_random,action.offhand,0)||dh2_combat_apply_monster(&applied,&request)){__android_log_print(ANDROID_LOG_ERROR,"DH2Native","Native combat application failed");enabled=false;return;}
  if(applied.hit_called)add_combat_threat(defender->aggro,attacker.aggro,defender->identity,attacker.identity,applied.threat,aggro_facts);
  apply_combat_text_v1(result,attacker.identity,defender->identity);
 ++combat_hits;
 __android_log_print(ANDROID_LOG_INFO,"DH2Native","Native combat hit | %s | target %s | hit %u | result %d %d %d %d %d %d %u %u %d %d | HP %d %d | dead %u | combo %u | RNG %u %u | statuses %u | threat %.9g",attacker.name.c_str(),defender->name.c_str(),combat_hits,result.amount,result.dot_element,result.dot_duration,result.dot_amount,result.hp_leech,result.mp_leech,result.outcomes,result.mask,result.weapon_category,result.element,applied.health.before,applied.health.after,defender->combat_state().dead,attacker.combat_state().combo_hits,combat_random.seed,combat_random.calls,applied.status_requests,double(applied.threat));
 if(applied.status_requests)__android_log_print(ANDROID_LOG_INFO,"DH2Native","Combat status services pending | %s | requests %u",defender->name.c_str(),applied.status_requests);
 if(applied.health.kill_requested){defender->pending_death=true;__android_log_print(ANDROID_LOG_INFO,"DH2Native","Combat death event queued | %s | event 2 | HP %d | dead %u | lifecycle %d",defender->name.c_str(),defender->properties().resolved[36],defender->combat_state().dead,defender->combat_state().lifecycle);}
}
ObjectActor* player_target(int index){
 if(index<0||unsigned(index)>=world_objects.size())return nullptr;
 const auto& record=world_objects[index];
 for(auto& group:object_groups)for(auto& actor:group.instances)if(actor.kind==1&&actor.room==record.room&&actor.name==record.name&&!actor.combat_state().dead)return &actor;
 return nullptr;
}
std::string player_attack(int supplied_target){return source_player_attack_v1(supplied_target);}
std::array<int,6> player_vitals(){return {prince_combat.properties().resolved[36],prince_combat.properties().resolved[38],prince_combat.properties().resolved[41],prince_combat.properties().resolved[43],int(prince_combat.life().dead),int(prince_combat.life().low_health_armed)};}
std::string player_equipment_action(int operation,int index,int slot){
  if(!world_mode||!native_actor_ready||!player_equipment||!player_equipment->ready()||prince_combat.life().dead)
    return "Equipment action failed: live player unavailable";
  try{
    std::string error;bool accepted=false;std::int32_t result=0;
    if(operation==0&&index>=0)accepted=player_equipment->auto_equip(static_cast<std::uint32_t>(index),result,error);
    else if(operation==1&&index>=0&&slot>=0)accepted=player_equipment->equip(static_cast<std::uint32_t>(slot),static_cast<std::uint32_t>(index),error);
    else if(operation==2&&slot>=0)accepted=player_equipment->unequip(static_cast<std::uint32_t>(slot),error);
    else if(operation==3)accepted=player_equipment->swap(error);
    else error="Unknown equipment action or invalid index/slot";
    if(!accepted)return "Equipment action failed: "+error;
    const std::vector<dh2::skinning::VisualDrawViewV32>* parts{};
    if(!player_equipment->draw_views(parts,error))return "Equipment action failed after source mutation: "+error;
    sync_equipment_draws(*parts,equipment_platform->manager,equipment_platform->cache);
    __android_log_print(ANDROID_LOG_INFO,"DH2Native","Native equipment action | operation %d | index %d | slot %d | result %d | selected %d | parts %zu | property checksum %016llx",
      operation,index,slot,result,player_equipment->inventory()->current_equipment(),equipment_parts.size(),
      static_cast<unsigned long long>(snapshot_checksum(prince_combat.properties().resolved)));
    return "Equipment updated";
  }catch(const std::exception& e){return std::string("Equipment action failed: ")+e.what();}
}
PlayerHudView player_hud_view(){
 const auto& sheet=prince_combat.properties().resolved;
 return {sheet.data(),sheet.size(),reinterpret_cast<std::uintptr_t>(&prince_combat),bool(prince_combat.life().dead)};
}
bool connect_character_panel_runtime(const dh2::android_ui::CharacterPanelMovieRuntimeV3& movie,
 dh2::android_ui::CharacterPanelGameplayServicesV2& out,std::string& error){
 return connect_character_panel_runtime_impl(movie,out,error);
}
PlayerGameplayBinding player_gameplay_binding(){
 PlayerGameplayBinding out{};
 if(!world_mode||!native_actor_ready||!live_script_world||!player_skills_runtime||!player_skills_runtime->player)return out;
 auto& t=*player_skills_runtime;out.design=live_script_world->design->borrow();out.gear=player_equipment.get();
 out.skills=t.player.get();out.save=t.save.get();out.properties=&t.player->session().property_view();
 out.life=&prince_combat.life();out.state=&prince_state;out.character=t.fsm.character;
 out.state_owner=&prince_state_owner;out.world_targets=t.targets.get();
 out.controller={reinterpret_cast<std::uintptr_t>(&prince_runtime.controller),out.character,controller_global_blocked,prince_state.controller_locked,prince_controller_forced,0};
  out.difficulty=live_script_world->host_level.difficulty;out.active=true;
  out.settings=live_script_world->saved_options.get();
  if(t.attack_owner)out.attack_fields=&t.attack_fields;
 out.world_owner=live_script_world;out.skill_tables=live_script_world->skills_owner->borrow();
 out.skill_list_index=prince_combat.properties().resolved.data()+28;out.temporary=t.temporary;
 out.debug=live_script_world->debug.get();out.debug_files=&live_script_world->debug_files;
 out.text_environment.localization={equipment_platform.get(),EquipmentPlatform::open_text,
  EquipmentPlatform::close_text,EquipmentPlatform::debug_text,EquipmentPlatform::constant,nullptr,nullptr};
 const auto* characters=out.design.characters();if(characters){const auto at=std::find(characters->names.begin(),characters->names.end(),"KnightPlayerBase");if(at!=characters->names.end())out.actor_index=at-characters->names.begin();}
 out.potion_capacity_context=out.gear;out.potion_capacity_store=[](void* context,std::int32_t value,std::string& error){
  auto* gear=static_cast<dh2::player::PlayerEquipmentRenderOwnerV1*>(context);
  if(!gear||gear!=player_equipment.get()||!gear->ready()||gear->properties()!=prince_combat.property_owner){error="Potion capacity store requires same live Gear/property owner";return false;}
  gear->project_potion_capacity(static_cast<std::int8_t>(static_cast<std::uint8_t>(value)));return true;
 };
 return out;
}
std::string player_gameplay_action(int operation,int index){
 auto live=player_gameplay_binding();if(!live.active||live.life->dead)return "Player action unavailable";
 if(operation==0){
  if(index<0||index>2)return "Invalid skill slot";const int row=live.save->skill_in_slot(index);
  if(row<0||live.save->skill_level(row)<=0)return "No learned skill assigned";
   std::uint32_t accepted=0;const int status=live.skills->skill_ai(dh2::character::skills::skill_ai_use_v3,row,&accepted);
   __android_log_print(status?ANDROID_LOG_ERROR:ANDROID_LOG_INFO,"DH2Native","Original skill activation | row %d | source status %d | accepted %u | native targets %u | state %d | clip %d | raw MP %d | same retained World/Gear/SkillV6",
    row,status,accepted,player_skills_runtime->native->targets().count,live.state->current,prince_locomotion.current_clip(),live.properties->resolved[41]);
  if(status)return "Skill action requires source provider: "+player_skills_runtime->error+"; "+live.skills->error();
  return accepted?"Skill activated":"Skill unavailable or cooling down";
 }
 if(operation==2){
  if(!live.controller.forced&&(live.controller.global_blocked||live.controller.locked))return "Potion input blocked by player controller";
  return gameplay_use_potion(live);
 }
 if(operation==1){
  if(!player_skills_runtime->cast)return "Original cast owner unavailable";
  if(live.difficulty<0||live.difficulty>2)return "Invalid saved faery difficulty";
  const auto id=live.save->current_faery(unsigned(live.difficulty));
  if(id<0||id>=5||!live.save->faeries_initialized()[unsigned(live.difficulty)]||!live.save->faeries()[unsigned(live.difficulty)][unsigned(id)].state)return "Faery is locked";
  bool allowed=false;auto& cast=*player_skills_runtime->cast;
  if(cast.can_begin_casting(live,&allowed))return "Faery source predicate failed: "+cast.error();
  if(!allowed)return "Faery unavailable or cooling down";
  if(cast.command(live,true,false))return "Faery cast requires source service: "+cast.error();
  return live.state->current==7?"Faery casting":"Faery cast rejected";
 }
 if(operation==3){
  if(!player_skills_runtime->cast)return "Original cast owner unavailable";
  auto& cast=*player_skills_runtime->cast;
  return cast.command(live,false,false)?"Faery end requires source service: "+cast.error():"Faery release delivered";
 }
 return "Unknown player action";
}
std::vector<int> player_gameplay_hud(){return gameplay_hud_snapshot(player_gameplay_binding());}
std::vector<std::string> player_gameplay_hud_icon_names(){return gameplay_hud_icon_names(player_gameplay_binding());}
dh2::ui::EnemyHudWorldBorrowV1 enemy_hud_world_borrow(std::string& error){
 using namespace dh2::ui;EnemyHudWorldBorrowV1 out{};
 if(!player_skills_runtime||!live_script_world||!native_actor_ready){error="Same enemy HUD World unavailable";return out;}
 auto& runtime=*player_skills_runtime;
 for(auto& entry:runtime.world_actors){auto& a=entry.second;if(!a.object||!a.object->properties||!a.object->life){error="Enemy HUD actor authority unavailable";return {};}
  a.hud.resolved=a.object->properties->resolved.data();a.hud.resolved_count=224;
  a.hud.identity=a.object->identity;a.hud.name_symbol=std::uint32_t(a.hud.resolved[18]);
  a.hud.debug_name=a.object->name.c_str();
  std::copy(a.object->position.begin(),a.object->position.end(),a.hud.position);
 }
 auto player=runtime.world_actors.find(runtime.fsm.character);if(player==runtime.world_actors.end()){error="Enemy HUD same player unavailable";return {};}
 out.player=&player->second.hud;out.world=reinterpret_cast<std::uintptr_t>(runtime.targets.get());
 out.services={&runtime,[](void* p,HudManagerState*,const HudManagerRequest* q,HudManagerResponse* r)->int{
  auto& t=*static_cast<PlayerSkillsRuntime*>(p);if(!q||!r)return 0;
  PlayerSkillsRuntime::WorldActor* actor=nullptr;
  for(auto& entry:t.world_actors)if(reinterpret_cast<std::uintptr_t>(&entry.second.hud)==q->subject){actor=&entry.second;break;}
  if(q->operation==HudManagerOperation::debug_load)return dh2_character_debug_load(t.world->debug.get(),&t.world->debug_files)==1;
  if(q->operation==HudManagerOperation::debug_switch){std::uint32_t value;if(!q->text||dh2_character_debug_get(&value,t.world->debug.get(),q->text,&t.world->debug_files)!=1)return 0;
   if(value){t.error="Enemy HUD debug requires actual network ID producer";return 0;}r->value=0;return 1;}
  if(!actor||!actor->object)return 0;
  const auto* props=actor->hud.resolved;const auto* ai=dh2::data::ai_props(*t.world->tables.ai(),props[1]);
  switch(q->operation){
   case HudManagerOperation::is_dead:r->value=actor->object->life->dead;return 1;
   case HudManagerOperation::is_character:r->value=1;return 1;
   case HudManagerOperation::is_monster:if(!ai)return 0;r->value=ai->type==4;return 1;
   case HudManagerOperation::is_boss:if(!ai)return 0;r->value=(ai->flags&4)!=0;return 1; //3a3164 ubfx #2,#1
   case HudManagerOperation::level:{auto view=dh2::data::property_view(*t.world->tables.rules(),*actor->object->properties);std::int32_t level;if(dh2_property_resolve(&view,19,&level))return 0;r->value=level>>8;return 1;}
   case HudManagerOperation::hp_fraction:r->fraction=float(props[36])/float(props[38]);return 1;
   case HudManagerOperation::target_character:{
    const auto identity=actor->object->target.target;if(!identity){r->identity=0;return 1;}
     dh2::target_providers::Handle16 handle{},*shared=nullptr;dh2::target_providers::Registry24* registry=nullptr;std::uintptr_t actual{};
     if(t.targets->handle_borrow(identity,&shared,&registry))return 0;
     const auto queries=t.targets->targets().query_services();
     if(dh2::target_providers::dh2_target_handle_character(&actual,&handle,shared,registry,&queries))return 0;
    auto found=t.world_actors.find(actual);r->identity=found==t.world_actors.end()?0:reinterpret_cast<std::uintptr_t>(&found->second.hud);return 1;
   }
   default:return 0;
  }
  }};
 out.presentation={&runtime,[](void* p,std::uintptr_t id,EnemyHudPresentationV2& result,std::string& error){
  auto& t=*static_cast<PlayerSkillsRuntime*>(p);const auto found=t.world_actors.find(id);
  if(found==t.world_actors.end()||!found->second.object){error="Enemy anchor requires same registered target";return false;}
  const auto& object=*found->second.object;const auto& props=object.properties->resolved;
  result={};result.identity=id;result.raw_hp=props[36];result.raw_max_hp=props[38];result.dead=object.life->dead!=0;
  if(result.dead||result.raw_hp<=0){
   if(t.hud_reported_target!=id||t.hud_reported_hp!=result.raw_hp||t.hud_reported_dead!=result.dead)
    __android_log_print(ANDROID_LOG_INFO,"DH2Native","Enemy HUD presentation | target %016llx | HP %d %d | dead %u | visible 0 | actual life/property owner",static_cast<unsigned long long>(id),result.raw_hp,result.raw_max_hp,unsigned(result.dead));
   t.hud_reported_target=id;t.hud_reported_hp=result.raw_hp;t.hud_reported_dead=result.dead;return true;
  }
  const auto* npc=found->second.npc;
   if(!npc||!npc->rendered_bounds_ready||!npc->rendered_screen_bounds_ready||submitted_width<=0||submitted_height<=0){error="Enemy anchor requires actual submitted mesh/camera";return false;}
  const auto& bounds=npc->rendered_bounds;
  const float point[4]{(bounds[0]+bounds[3])*.5f,(bounds[1]+bounds[4])*.5f,bounds[5],1};float clip[4]{};
  for(unsigned row=0;row<4;++row)for(unsigned column=0;column<4;++column)clip[row]+=submitted_camera[column*4+row]*point[column];
  if(!std::isfinite(clip[3])||clip[3]<=0)return true;
   // Projecting the top centre of a world AABB creates a point that may not
   // belong to the visible mesh. Use the bounds of the submitted vertices in
   // this camera so the bar sits immediately above the actual enemy silhouette.
   const auto& screen=npc->rendered_screen_bounds;
   result.screen_pixels[0]=(screen[0]+screen[1])*.5f;result.screen_pixels[1]=screen[2];
   result.on_screen=result.screen_pixels[0]>=0&&result.screen_pixels[0]<=submitted_width&&result.screen_pixels[1]>=0&&result.screen_pixels[1]<=submitted_height;
  if(t.hud_reported_target!=id||t.hud_reported_hp!=result.raw_hp||t.hud_reported_dead!=result.dead)
   __android_log_print(ANDROID_LOG_INFO,"DH2Native","Enemy HUD presentation | target %016llx | HP %d %d | dead 0 | visible %u | anchor pixels %.9g %.9g | mesh head %.9g %.9g %.9g | Prince %.9g %.9g %.9g | same submitted camera",static_cast<unsigned long long>(id),result.raw_hp,result.raw_max_hp,unsigned(result.on_screen),double(result.screen_pixels[0]),double(result.screen_pixels[1]),double(point[0]),double(point[1]),double(point[2]),double(actor_position[0]),double(actor_position[1]),double(actor_position[2]));
  t.hud_reported_target=id;t.hud_reported_hp=result.raw_hp;t.hud_reported_dead=false;
  return true;
 }};return out;
}
void connect_combat_text(CombatTextSinkV1 sink){combat_text_sink=sink;}
bool combat_text_frame(CombatTextFrameV1& frame,std::string& error){return fill_combat_text_frame_v1(frame,error);}
bool combat_text_project(const float point[3],std::int32_t* x,std::int32_t* y,std::string& error){
 if(!point||!x||!y||submitted_width<=0||submitted_height<=0){error="Combat text requires current submitted camera";return false;}
 float clip[4]{};const float world[4]{point[0],point[1],point[2],1};
 for(unsigned row=0;row<4;++row)for(unsigned column=0;column<4;++column)clip[row]+=submitted_camera[column*4+row]*world[column];
 if(!std::isfinite(clip[3])||clip[3]==0){error="Combat text camera projection has invalid W";return false;}
 const float px=(clip[0]/clip[3]+1)*.5f*submitted_width,py=(1-clip[1]/clip[3])*.5f*submitted_height;
 if(!std::isfinite(px)||!std::isfinite(py)||px<-2147483648.f||px>=2147483648.f||py<-2147483648.f||py>=2147483648.f){error="Combat text projection outside int32 range";return false;}
 *x=std::int32_t(px);*y=std::int32_t(py);return true;
}
void authored_hud_heading(float x,float y,bool active){
  if(!std::isfinite(x)||!std::isfinite(y))throw std::runtime_error("Malformed source HUD heading");
  source_hud_heading=true;source_hud_heading_active=active;move_x=active?x:0;move_y=active?y:0;
}
bool authored_hud_command(const float* direction,bool stop,std::string& error){
 if(!stop&&(!direction||!std::isfinite(direction[0])||!std::isfinite(direction[1])||!std::isfinite(direction[2]))){error="Malformed authored controller direction";return false;}
 if(!source_player_heading_v1(direction,stop,error))return false;
 authored_hud_heading(stop?0:direction[0],stop?0:direction[1],!stop&&(direction[0]!=0||direction[1]!=0||direction[2]!=0));
 return true;
}
bool world_touch(float x,float y,bool released,std::string& error){
 error.clear();
 if(!world_mode||!native_actor_ready||!player_skills_runtime||!player_skills_runtime->world_touch){error="Same live world touch owner unavailable";return false;}
 auto& t=*player_skills_runtime;
 t.targets->begin_frame(native_actor_frames);if(t.targets->refresh()){error=t.targets->error();return false;}
 bool hit=false;const bool ok=t.world_touch->dispatch(x,y,released,hit,error);
 __android_log_print(ok?ANDROID_LOG_INFO:ANDROID_LOG_ERROR,"DH2Native","World touch | DACT development input adapter | phase %s | pixels %.4f %.4f | floor %u | same target %016llx | %s",released?"release":"press",double(x),double(y),unsigned(hit),static_cast<unsigned long long>(t.world->player_object->target.target),error.c_str());
 return ok;
}
void world_touch_cancel(){
 if(!player_skills_runtime||!player_skills_runtime->world_touch)return;
 auto& pending=player_skills_runtime->world_touch->pending;
 pending.pending14c8=0;pending.skill14ca=-1;
}
namespace {
void player_authored_event(const dh2::animation::TriggeredEvent& event,int clip){
 const auto& frames=prince_locomotion.scheduler.frames();
 if(frames.empty())return;
 const dh2::data::CombatEventContext context{prince_state.current,int(frames.front().step),int(frames.back().step),0,-1};
 dh2::data::CombatEventAction action;
 if(dh2_combat_event_route(&action,&context,event.name))throw std::runtime_error("Player combat route failed");
 __android_log_print(ANDROID_LOG_INFO,"DH2Native","Player animation event | clip %d | name %s | sequence %d | attack step %d | kind %d | lag %d | %s | Step %u | position %.4f %.4f %.4f",clip,event.name,action.sequence_step,action.attack_step,int(action.kind),event.lag_ms,prince_scene_phase?"scene before Step":"synchronous actor replay",native_physics_steps,prince_runtime.subobjects.position[0],prince_runtime.subobjects.position[1],prince_runtime.subobjects.position[2]);
 if(action.kind!=dh2::data::CombatEventKind::melee)return;
 if(!player_skills_runtime||!player_skills_runtime->attack_geometry)return;
 auto& runtime=*player_skills_runtime;
 const auto target_id=runtime.world->player_object->target.target;
 ObjectActor* target=nullptr;
 for(auto& group:object_groups)for(auto& candidate:group.instances)
  if(candidate.kind==1&&candidate.identity==target_id)target=&candidate;
 if(!target)return;
 std::int32_t in_range=0;
 if(!runtime.attack_geometry->read(runtime.fsm.character,target_id,
     dh2::character::WorldAIAttackQueryV1::IsInMeleeRange,in_range,runtime.error))
  throw std::runtime_error("Original authored melee range query: "+runtime.error);
 if(!in_range)return;
  dh2::data::CombatantView av{prince_combat.properties().resolved.data(),-1,-1,0,0,0,prince_state.current,prince_combat.life().combo_hits},dv{target->properties().resolved.data(),-1,-1,0,0,0,target->state=="Attack"?5:-1,target->combat_state().combo_hits};
  bind_player_combat_equipment(av);
 auto ap=dh2::data::property_view(actor_property_rules,prince_combat.properties()),dp=dh2::data::property_view(actor_property_rules,target->properties());
 dh2::data::CombatResult result;dh2::data::MonsterApplication applied;
 const dh2::data::MonsterApplicationRequest request{&result,&ap,&dp,&prince_combat.life(),&target->combat_state()};
 const unsigned aggro_facts=(target->combat_state().dead?dh2::data::aggro_owner_dead:0u)|(prince_combat.life().dead?dh2::data::aggro_target_dead:0u);
 if(dh2_combat_melee(&result,&av,&dv,&combat_random,action.offhand,0)||dh2_combat_apply_player_to_monster(&applied,&request))throw std::runtime_error("Player combat application failed");
  if(applied.hit_called)add_combat_threat(target->aggro,prince_combat.aggro,target->identity,0x100000001ull,applied.threat,aggro_facts);
  apply_combat_text_v1(result,runtime.fsm.character,target_id);
 ++combat_hits;++prince_combat.attempts;
 __android_log_print(ANDROID_LOG_INFO,"DH2Native","Prince combat hit | target %s | attempt %u | result %d %d %d %d %d %d %u %u %d %d | HP %d %d | dead %u | combo %u | RNG %u %u | statuses %u",target->name.c_str(),prince_combat.attempts,result.amount,result.dot_element,result.dot_duration,result.dot_amount,result.hp_leech,result.mp_leech,result.outcomes,result.mask,result.weapon_category,result.element,applied.health.before,applied.health.after,target->combat_state().dead,prince_combat.life().combo_hits,combat_random.seed,combat_random.calls,applied.status_requests);
 if(applied.health.kill_requested)target->pending_death=true;
 if(applied.status_requests)__android_log_print(ANDROID_LOG_INFO,"DH2Native","Combat status services pending | %s | requests %u",target->name.c_str(),applied.status_requests);
}
}
namespace {
unsigned actor_virtual_service(void*,unsigned event,float* payload){
 using namespace dh2::subobjects;
 switch(event){
  // The reconstructed scene/body/floor bridge handles physical and position
  // events. The current renderer has no original game camera or auxiliary.
  case camera_get:if(payload)payload[0]=0;return 0;
  case camera_can_move:return 1;
  case visual_update:case visual_apply_rotation:case get_speed:return 1;
  case visual_sync_scaling:{
   std::string error;return prince_visual.update_world(current_scene,error)?1:~0u;
  }
  default:return ~0u;
 }
}
dh2::character::Facts prince_facts(){
 dh2::character::Facts facts{};facts.is_player=1;facts.stance_mask=210;
 dh2::character::StanceFacts16 equipment{dh2::character::stance_is_player,5,{0,0}};
 if(player_equipment&&player_equipment->ready()){
   std::string error;if(!player_equipment->stance_facts(true,5,equipment,error))throw std::runtime_error(error);
 }
 if(dh2_character_anim_stance(&facts.stance,&equipment)!=1)throw std::runtime_error("Character stance producer failed");
 if(player_skills_runtime&&player_skills_runtime->world&&player_skills_runtime->world->player_object)
  facts.target=player_skills_runtime->world->player_object->target.target;
 std::copy(prince_runtime.controller.heading.direction,prince_runtime.controller.heading.direction+3,facts.heading);
 facts.walk_threshold=.45f;facts.run_threshold=.85f;
 const float one=1;dh2::move::Speed movement{};
 if(dh2_move_speed(&movement,prince_combat.properties().resolved.data(),&one)||dh2_character_attack_speed(&facts.attack_speed,prince_combat.properties().resolved.data())!=1)throw std::runtime_error("Character property speed failed");
 facts.walk_speed=movement.walk_multiplier;
 auto sequence=[](const char* name){const auto* value=dh2::data::animation_state(actor_animation_tables,prince_combat.animation_table,name);return value?int(value-actor_animation_tables.sequences.data()):-1;};
 facts.idle=sequence("Idle");facts.walk=sequence("Walk");facts.run=sequence("Run");facts.attack_static=sequence("AttackStatic");facts.attack_moving=sequence("Attack");facts.death=sequence("Died");
 if(const auto* ai=dh2::data::ai_props(actor_ai_tables,prince_combat.properties().resolved[1]))facts.attack_delay=std::uint32_t(ai->attack_delay);
 facts.is_at_destination=dh2_nav_is_at_destination(&prince_runtime.controller,&prince_runtime.path)==1;
 facts.following_path=prince_runtime.path.count!=0;facts.has_ranged_weapon=0;
 return facts;
}
struct CharacterFactsScope {
 dh2::character::Facts* previous;
 explicit CharacterFactsScope(dh2::character::Facts& facts):previous(active_prince_facts){active_prince_facts=&facts;}
 ~CharacterFactsScope(){active_prince_facts=previous;}
};
void refresh_prince_facts(){if(active_prince_facts)*active_prince_facts=prince_facts();}
#include "renderer_target_facing_v38.inc"
int prince_look_service(void*,const dh2::character::CharacterControlRequest32* request,
                       dh2::character::CharacterControlResponse16* response){
 const auto before=prince_runtime.rotation.heading_angle;
 const int result=player_target_facing_service_v38(request,response);
 if(result==1&&before!=prince_runtime.rotation.heading_angle){++native_heading_updates;
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Player canonical target facing | same registered target/heading owner | angle %.9g | updates %u",prince_runtime.rotation.heading_angle,native_heading_updates);
 }
 return result;
}
void character_service(void*,dh2::character::State* state,const dh2::character::Request* request){
 using namespace dh2::character;std::string error;
 switch(request->service){
 case stop:{
  if(player_skills_runtime&&player_skills_runtime->heading_owner){
   auto& t=*player_skills_runtime;t.heading_body=prince_body.body?&prince_body:nullptr;
   if(!t.heading_owner->object_stop(error))throw std::runtime_error(error);
  }else{
   // Initial State focus precedes the retained skill/controller graph. Use
   // the same constructor-owned GameObject fields, in original Stop order.
   if(dh2_nav_drop_path(&prince_runtime.path))throw std::runtime_error("Initial Character Stop path failed");
   std::copy(prince_runtime.subobjects.position,prince_runtime.subobjects.position+3,prince_runtime.subobjects.destination);
   prince_runtime.controller.path_requested=0;prince_runtime.controller.heading.active=0;
   std::fill(prince_runtime.controller.heading.direction,prince_runtime.controller.heading.direction+3,0);
   if(prince_body.body&&(state->flags&2)&&dh2_native_body_stop(&prince_body,prince_runtime.subobjects.position))throw std::runtime_error("Initial Character Stop body failed");
  }
  state->heading_active=prince_runtime.controller.heading.active;refresh_prince_facts();break;
 }
 case pin:if(prince_body.body&&dh2_native_body_pin(&prince_body))throw std::runtime_error("Character pin failed");break;
 case unpin:if(prince_body.body&&dh2_native_body_unpin(&prince_body))throw std::runtime_error("Character unpin failed");break;
 case set_animation:
  state->current_animation=request->argument[0];
  if(!prince_locomotion.start(actor_animation_tables,state->current_animation,actor_random,prince_attack_clips,prince_visual,current_scene,1,error))throw std::runtime_error(error);
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Character animation selected | state %d | flags %x | sequence %d | clip %d | %s",state->current,state->flags,state->current_animation,prince_locomotion.current_clip(),prince_scene_phase?"scene callback":"actor state service");
  break;
 case set_speed:
  if(!prince_locomotion.set_speed(request->scalar,error))throw std::runtime_error(error);
  if(state->current==4)__android_log_print(ANDROID_LOG_INFO,"DH2Native","Locomotion clip: %s | original selected clip %d | timeline speed %.9g | authored restart",state->move_type==2?"Run":"Walk",prince_locomotion.current_clip(),prince_locomotion.current_timeline().scale);
  break;
 case stop_loop:prince_locomotion.stop_loop(false);break;
 case start_timer:{
  if(!player_skills_runtime||!player_skills_runtime->player)throw std::runtime_error("Same player timer store unavailable");
   const auto id=player_skills_runtime->player->session().start_timer(std::uint32_t(request->argument[0]),request->argument[1],request->argument[2],std::uintptr_t(request->identity));
  if(id<0)throw std::runtime_error("Character timer start failed: "+std::to_string(id));
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Character timer started | state %d | slot %d | duration %u | repeat %d | event 0x%x | gate %x",state->current,id,std::uint32_t(request->argument[0]),request->argument[1],request->argument[2],state->attack_gate);break;
 }
 case swap_animation:{
  const auto before_root=prince_locomotion.scheduler.frames().empty()?-1:prince_locomotion.scheduler.frames().front().sequence;
  const auto before_clip=prince_locomotion.current_clip();const auto before_ms=prince_locomotion.current_timeline().current_ms;
  if(!prince_locomotion.swap(actor_animation_tables,request->argument[0],request->argument[1],actor_random,prince_attack_clips,prince_visual,current_scene,state->cached_speed,error))throw std::runtime_error(error);
  if(!prince_locomotion.scheduler.frames().empty())state->current_animation=prince_locomotion.scheduler.frames().front().sequence;
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Character animation swap | state %d | root %d %d | clip %d %d | ms %d %d | desired %d | old %d",state->current,before_root,state->current_animation,before_clip,prince_locomotion.current_clip(),before_ms,prince_locomotion.current_timeline().current_ms,request->argument[0],request->argument[1]);break;
 }
  case look_at:{
   const CharacterControlServices16 services{nullptr,prince_look_service};
   if(!player_skills_runtime||!player_skills_runtime->world||!player_skills_runtime->world->player_object)throw std::runtime_error("Required same-player canonical LookAt owner");
   const auto owner=player_skills_runtime->world->player_object->identity;
   if(request->argument[0]==1){
    const ControllerCommandState32 controller{reinterpret_cast<std::uintptr_t>(&prince_state),owner,controller_global_blocked,state->controller_locked,prince_controller_forced,0};
   if(dh2_character_controller_character(&controller,controller_look_object,request->identity,&services)!=1)throw std::runtime_error("Character controller LookAt failed");
   }else if(dh2_character_control(owner,controller_look_object,request->identity,&services)!=1)throw std::runtime_error("Character LookAt failed");
  break;
 }
 case set_heading:{
  float direction[3];std::memcpy(direction,request->argument,12);
  if(dh2_nav_set_heading(&prince_runtime.controller.heading,direction,unsigned(request->scalar)))throw std::runtime_error("Character heading failed");
  prince_runtime.rotation.heading_angle=prince_runtime.controller.heading.angle;state->heading_active=prince_runtime.controller.heading.active;refresh_prince_facts();break;
 }
 case set_death_filter:case reset_filter:{
  if(!prince_body.body)break;
  auto* shape=prince_body.body->GetShapeList();if(!shape)throw std::runtime_error("Character shape missing");
  auto filter=prince_initial_filter;
  if(request->service==set_death_filter){filter.groupIndex=std::int16_t(request->argument[0]);filter.categoryBits=std::uint16_t(request->argument[1]);filter.maskBits=std::uint16_t(request->argument[2]);}
  shape->SetFilterData(filter);
  prince_body_owner.contact.primary={filter.groupIndex,filter.categoryBits,filter.maskBits,1};
  actor_world.backend()->Refilter(shape);
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Character filter applied | state %d | group %d | category %u | mask %u | primary only",state->current,filter.groupIndex,filter.categoryBits,filter.maskBits);break;
 }
 case remove_body:
  actor_world.destroy(prince_body.body);prince_body.pinned=0;
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Character physical object removed | state %d | source event 22",state->current);break;
 case raise_event:{
  const unsigned event=unsigned(request->argument[0]);
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Character state event | state %d | event %x | prior %d | flags %x",state->current,event,request->argument[1],state->flags);
  if(event==0x1d)__android_log_print(ANDROID_LOG_INFO,"DH2Native","Player source state | previous %d | current %d | event 0x%x | flags %x | root %d | clip %d | position %.4f %.4f %.4f | Step %u",request->argument[1],state->current,prince_event_cause,state->flags,state->current_animation,prince_locomotion.current_clip(),prince_runtime.subobjects.position[0],prince_runtime.subobjects.position[1],prince_runtime.subobjects.position[2],native_physics_steps);
  if(event==0x3f&&prince_event(event,request->identity)<0)throw std::runtime_error("Character state event failed");
  if((event==0x2a||event==0x2b||event==0x2c)&&prince_event(event,request->identity)<0)throw std::runtime_error("Character state gate event failed");
  break;
 }
 default:{
  const auto bit=std::uint64_t(1)<<request->service;
  if(!(pending_character_services&bit)){pending_character_services|=bit;__android_log_print(ANDROID_LOG_INFO,"DH2Native","Character external service pending | service %u | state %d",request->service,state->current);}
  break;
 }
 }
 prince_flags=state->flags;prince_move_type=state->move_type;walking=state->current==4;
}
int prince_event(unsigned event,std::uint64_t payload){
 auto facts=prince_facts();CharacterFactsScope borrow(facts);const auto old=prince_event_cause;prince_event_cause=event;
 const int result=prince_state_owner.event(int(event),payload,prince_owner_services);prince_event_cause=old;return result;
}
void character_playback_event(void*,dh2::actor::BlendedPlayback&,const dh2::actor::BlendedPlaybackEvent& blended){
  const auto& event=blended.event;
  if(player_skills_runtime)player_skills_runtime->animation_lag=event.handoff.lag_ms;
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Blended character event | event 0x%x | clip %d | slot %u | phase %u | lag %d",event.handoff.event_id,event.clip,blended.slot,event.phase,event.handoff.lag_ms);
  if(prince_state.current==6&&event.handoff.event_id==0x28){
   const auto* name=static_cast<const char*>(event.handoff.payload);
   __android_log_print(ANDROID_LOG_INFO,"DH2Native","Source skill authored delivery | clip %d | text %s | lag %d | source FSM %d",event.clip,name?name:"<null>",event.handoff.lag_ms,prince_state.current);
  }
 if(prince_state.current==5&&player_skills_runtime&&player_skills_runtime->attack_owner&&
    (event.handoff.event_id==0x26||event.handoff.event_id==0x27)){
  if(prince_controller_forced||(!controller_global_blocked&&!prince_state.controller_locked)){
   if(player_skills_runtime->attack_animation_step(event.handoff.event_id==0x26))
    throw std::runtime_error("Original attack animator step: "+player_skills_runtime->error);
  }
  if(prince_event(event.handoff.event_id,0)<0)throw std::runtime_error("Original attack animator forwarding failed");return;
 }
 if(prince_state.current==7&&player_skills_runtime&&player_skills_runtime->cast&&
    (event.handoff.event_id==0x26||event.handoff.event_id==0x27)){
  const auto live=player_gameplay_binding();
  if(live.controller.forced||(!live.controller.global_blocked&&!live.controller.locked)){
   if(player_skills_runtime->cast->animation_step(live,event.handoff.event_id==0x26,
       prince_locomotion.step_index(),prince_locomotion.animation_depth()))
    throw std::runtime_error("Original cast animator step: "+player_skills_runtime->cast->error());
  }
  if(prince_event(event.handoff.event_id,0)<0)throw std::runtime_error("Original cast animator state forwarding failed");return;
 }
 // The full native six-event CharAI/AIS service binding remains unfinished.
 // Preserve synchronous authored damage and the existing FSM close boundary.
 if(event.handoff.event_id==0x28){
   if(prince_state.current==7&&player_skills_runtime&&player_skills_runtime->cast){
    if(player_skills_runtime->cast->animation_event(player_gameplay_binding(),event.handoff.payload))throw std::runtime_error("Original cast animation event: "+player_skills_runtime->cast->error());return;
   }
   if(prince_state.current!=5&&player_skills_runtime){
   auto& t=*player_skills_runtime;const dh2::character::skills::SkillStateServices16V4 services{&t,PlayerSkillsRuntime::state_service};
   if(dh2_character_skill_state_v4(&t.skill_state,dh2::character::skills::skill_state_ai_event_v4,0,0,reinterpret_cast<std::uintptr_t>(event.handoff.payload),0,&services))
    throw std::runtime_error("Required source SkillState animation event: "+t.error+"; "+t.player->error());
   return;
  }
  const dh2::animation::TriggeredEvent trigger{event.handoff.lag_ms,event.handoff.payload};player_authored_event(trigger,event.clip);
 }else if(event.handoff.event_id==0x22){
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Character sequence closed | state %d | event 0x22 | clip %d | Step %u | post Step animator | position %.4f %.4f %.4f",prince_state.current,event.clip,native_physics_steps,prince_runtime.subobjects.position[0],prince_runtime.subobjects.position[1],prince_runtime.subobjects.position[2]);
  if(prince_event(0x22,0)<0)throw std::runtime_error("Character sequence close failed");
 }
}
void request_prince_death(){
 if(!prince_combat.pending_death)return;
 const auto facts=prince_facts();prince_state.animation_override=facts.death;
 const int accepted=prince_event(0xc358,prince_combat.death_target);
 if(accepted<0)throw std::runtime_error("Character death state request failed");
 prince_combat.pending_death=false;
 __android_log_print(ANDROID_LOG_INFO,"DH2Native","Player death animation selected | clip %d | dead %u | lifecycle %d | state %d",prince_locomotion.current_clip(),prince_combat.life().dead,prince_combat.life().lifecycle,prince_state.current);
}
void initialize_native_actor(AAssetManager* assets,bool restore){
 std::string error;float bounds[4];
 const bool restore_without_body=restore&&prince_state.current==12&&!prince_state.body_present;
 if(!level.native_floor||dh2_decor_level_world_bounds(bounds))throw std::runtime_error("Native actor world bounds missing");
 actor_world.load(bounds);decor_body_owners.clear();decor_bodies.clear();decor_bodies.reserve(84);
 prince_body={};prince_body_owner.native=&prince_body;
  // The current collision-bounds prototype still uses four default-warrior
  // modules. Live rendered equipment is supplied by the retained inventory
  // owner; equipment-dependent collision bounds remain a separate connection.
 auto prince=read(assets,"prince_modular.bdae","models");dh2::resources::BresView view{};
 dh2::scene::Scene factory;
 if(dh2_bres_open(&view,prince.data(),prince.size())!=dh2::resources::BresError::ok||!dh2::scene::load(view,factory,error))throw std::runtime_error(error);
 const auto node=std::find_if(factory.graph.begin(),factory.graph.end(),[](const auto& n){return n.id=="prince_modular-node";});
 if(node==factory.graph.end())throw std::runtime_error("Native player modular node absent");
 factory.instances.clear();
 for(unsigned i=0;i<dh2_bres_library_count(&view,dh2::resources::Library::controller);++i){
  dh2::skinning::Skin skin;if(!dh2::skinning::load(view,i,factory,skin,error))throw std::runtime_error(error);
  if(skin.id.find("_default_warrior-mesh-skin")==std::string::npos)continue;
  dh2::scene::Instance instance{node->id,unsigned(node-factory.graph.begin()),skin.geometry,node->world,{}};instance.controller=i;
  factory.instances.push_back(std::move(instance));
 }
 if(factory.instances.size()!=4)throw std::runtime_error("Native player equipment bounds incomplete");
 std::vector<dh2::physical::CharacterMeshEntry> entries;
 if(!dh2::physical::character_scene_entries(view,factory,entries,error))throw std::runtime_error(error);
 dh2::physical::CharacterMeshBoxInput mesh_input{};mesh_input.entries=entries.data();mesh_input.count=entries.size();
 std::copy(actor_position.begin(),actor_position.end(),mesh_input.placement.position);
  prince_source_scale120_written=false;
  if(dh2_character_visual_scale(prince_source_scale120.data(),prince_combat.properties().base.data()+12))throw std::runtime_error("Original character visual scale rejected");
  prince_source_scale120_written=true;
  std::copy(prince_source_scale120.begin(),prince_source_scale120.end(),mesh_input.placement.scale);
 dh2::physical::DecorSceneOutput mesh;
 if(dh2_character_mesh_box(&mesh,&mesh_input))throw std::runtime_error("Native player mesh bounds rejected");
 prince_runtime={};std::copy(actor_position.begin(),actor_position.end(),prince_runtime.subobjects.position);
 std::copy(actor_position.begin(),actor_position.end(),prince_runtime.subobjects.destination);
 dh2::physical::CharacterOwnerBoundsInput owner_input{};
 std::copy(mesh.mesh_box,mesh.mesh_box+6,owner_input.mesh_box);std::copy(actor_position.begin(),actor_position.end(),owner_input.position);
 owner_input.collision_scale=prince_combat.properties().resolved[16];
 dh2::physical::CharacterOwnerBounds owner_bounds{};
 if(dh2_character_owner_bounds(&owner_bounds,&owner_input))throw std::runtime_error("Original character owner bounds rejected");
 std::copy(owner_bounds.relative_box,owner_bounds.relative_box+6,prince_runtime.subobjects.local_bounds);
 std::copy(owner_bounds.absolute_box,owner_bounds.absolute_box+6,prince_runtime.subobjects.absolute_bounds);
 prince_runtime.subobjects.rotation=heading;prince_runtime.rotation.rotation[2]=heading;prince_runtime.rotation.heading_angle=heading;
 dh2::physical::CharacterBodyInput input{};input.owner=&prince_runtime;input.new_physical=&prince_body;input.character_type=1;input.is_player=1;
 const auto* box=prince_runtime.subobjects.absolute_bounds;
 input.absolute_bounds[0]=box[0];input.absolute_bounds[1]=box[1];input.absolute_bounds[2]=box[3];input.absolute_bounds[3]=box[4];
 input.position[0]=actor_position[0];input.position[1]=actor_position[1];
 dh2::physical::CharacterBodyConfig config{};
 if(dh2_character_body_config(&config,&input)||!config.enabled)throw std::runtime_error("Native player body definition rejected");
 prince_body_owner.set_filter(config);prince_body_owner.additions=prince_body_owner.results=0;
 prince_body={actor_world.create_character(config,&prince_body_owner.services),config.radius,config.pinned};
  if(!prince_body.body||dh2_native_body_refresh_view(&prince_runtime.body,&prince_body))throw std::runtime_error("Native player body creation failed");
  if(!live_script_world||!live_script_world->player_canonical||
     !live_script_world->player_canonical->publish_physical2dc_v20(reinterpret_cast<std::uintptr_t>(&prince_body),error))
   throw std::runtime_error(error.empty()?"Required same player physical2dc publication":error);
 prince_state_owner.machine().physical=reinterpret_cast<std::uintptr_t>(&prince_body);
 prince_initial_filter=prince_body.body->GetShapeList()->GetFilterData();
 live_obstacle_entries.assign(256,{});live_obstacle_floors.assign(level.native_floor->records.size()+1,0);
 live_registry={live_obstacle_entries.data(),0,unsigned(live_obstacle_entries.size()),live_obstacle_floors.data(),0,unsigned(live_obstacle_floors.size())};
 live_path_segments.resize(level.native_floor->graph.node_count+1);live_workspace_segments.resize(live_path_segments.size());
 live_workspace_floors.resize(live_obstacle_floors.size());live_workspace_actors.resize(1);
 live_workspace={live_workspace_segments.data(),unsigned(live_workspace_segments.size()),0,live_workspace_actors.data(),1,0,live_workspace_floors.data(),unsigned(live_workspace_floors.size()),0};
 prince_runtime.path.segments=live_path_segments.data();prince_runtime.path.capacity=live_path_segments.size();
 std::copy(actor_position.begin(),actor_position.end(),prince_runtime.path.target);
 dh2_nav_object_defaults(&prince_runtime.object);dh2_nav_motion_policy_defaults(&live_motion_policy);
 const dh2::navigation::ObjectInitRequest init{&level.native_floor->collision_world,&prince_runtime.object,0x100000001ull,{actor_position[0],actor_position[1],actor_position[2]},config.radius*100.f,0,0};
 if(dh2_nav_init_object(&init))throw std::runtime_error("Native player floor initialization failed");
 const dh2::navigation::ProducerFields fields{dh2::navigation::ProducerClass::character,1,config.radius,0,{box[0],box[1]},{box[3],box[4]}};
 const dh2::navigation::ProducerRequest producer{&level.native_floor->collision_world,&live_registry,&prince_runtime.object,0x100000001ull,&fields};
 if(dh2_nav_update_game_object(&producer))throw std::runtime_error("Native player obstacle initialization failed");
 std::copy(prince_runtime.object.motion.position,prince_runtime.object.motion.position+3,prince_runtime.subobjects.previous_position);
 std::copy(prince_runtime.object.motion.position,prince_runtime.object.motion.position+3,prince_runtime.subobjects.position);
 std::copy(prince_runtime.object.motion.position,prince_runtime.object.motion.position+3,actor_position.begin());
 // Activity recreation rebuilds a complete CPU playback session and explicitly
 // restarts its saved sequence below. It does not restore interrupted fades.
 prince_visual=dh2::visual::SceneBinding{};
 if(!prince_visual.bind(current_scene,error))throw std::runtime_error(error);
 prince_locomotion=dh2::actor::BlendedPlayback{};scene_clock=0;
 dh2::animation::RegistrationSet registration;
 for(int id:prince_animation_bank.registration_requests){
  const auto resource=prince_attack_clips.find(id);
  if(resource==prince_attack_clips.end()||!registration.append(id,dh2::data::animation_resource_identity(prince_animation_bank,id),&resource->second,error))throw std::runtime_error("Prince registration failed: "+error);
 }
 const int template_id=prince_animation_bank.template_clip_id;
 if(!registration.set_default(dh2::data::animation_resource_identity(prince_animation_bank,template_id),&prince_attack_clips.at(template_id),error))throw std::runtime_error(error);
 registration.refresh_indices();
 if(!prince_locomotion.compile_dynamic(prince_attack_clips,registration,factory,prince_visual,error))throw std::runtime_error("Prince dynamic compilation failed: "+error);
 __android_log_print(ANDROID_LOG_INFO,"DH2Native","Prince blended bank ready | resources %zu | occurrences %zu | targets %zu | template %d | engine %d | game clip %d",prince_attack_clips.size(),registration.occurrences().size(),prince_locomotion.transform_set().targets().size(),template_id,prince_locomotion.current_engine_clip(),prince_locomotion.current_clip());
 std::copy(actor_position.begin(),actor_position.end(),prince_visual.root.position);
 std::copy(mesh.effective_scale,mesh.effective_scale+3,prince_visual.root.scale);
 const float euler[3]{0,0,heading};if(!prince_visual.set_rotation(euler))throw std::runtime_error("Native player rotation rejected");
 prince_locomotion.observer={nullptr,character_playback_event};
 if(!restore){
  prince_state={};pending_character_services=0;prince_event_cause=0;
  prince_state_owner.machine().current_index=-1;prince_state_owner.native_fsm().current_present=0;
 }
 prince_state.body_present=1;
 if(prince_state.current==-1||prince_state.current==3||prince_state.current==4){
  // Development Activity recreation cancels held touch before restoring the
  // world. It supplies Idle through the recovered transition services.
  auto facts=prince_facts();CharacterFactsScope borrow(facts);
  if(prince_state_owner.transition(3,0,0,prince_owner_services)<0)throw std::runtime_error("Character Idle initialization failed");
 }else if(prince_state.current==5){
  const auto facts=prince_facts();
  if(prince_state.current_animation==facts.attack_moving){if(dh2_native_body_unpin(&prince_body))throw std::runtime_error("Attack body restoration failed");}
  else if(dh2_native_body_pin(&prince_body))throw std::runtime_error("Attack body restoration failed");
 }else if(prince_state.current==12){
  const dh2::character::Request filter{dh2::character::set_death_filter,{0,0x51c,3},0,0,0};
  character_service(nullptr,&prince_state,&filter);
  if(restore_without_body){actor_world.destroy(prince_body.body);prince_state.body_present=0;}
 }
 if(restore&&prince_state.current!=3&&prince_state.current!=4&&prince_state.current_animation>=0){
  if(!prince_locomotion.start(actor_animation_tables,prince_state.current_animation,actor_random,prince_attack_clips,prince_visual,current_scene,prince_state.cached_speed,error))throw std::runtime_error("Restored Prince sequence failed: "+error);
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Player blended sequence restarted | state %d | sequence %d | clip %d | engine %d | development recreation",prince_state.current,prince_state.current_animation,prince_locomotion.current_clip(),prince_locomotion.current_engine_clip());
 }
 prince_flags=prince_state.flags;prince_move_type=prince_state.move_type;
 for(const auto& group:object_groups){
  if(group.instances.empty()||group.instances.front().kind!=2)continue;
  auto model=read(assets,group.instances.front().model,"actors");dh2::resources::BresView decor_view{};
  if(dh2_bres_open(&decor_view,model.data(),model.size())!=dh2::resources::BresError::ok)throw std::runtime_error("Decor collision model rejected");
  dh2::physical::DecorSceneMarker marker;
  if(!dh2::physical::decor_scene_marker(decor_view,marker,error))throw std::runtime_error(error);
  if(!marker.found)continue;
  for(const auto& instance:group.instances){
   dh2::physical::DecorSceneInput scene_input{};
   std::copy(instance.position.begin(),instance.position.end(),scene_input.position);
   std::copy(instance.rotation_degrees.begin(),instance.rotation_degrees.end(),scene_input.rotation_degrees);
   std::copy(instance.scale.begin(),instance.scale.end(),scene_input.scale);
   std::copy(marker.bounds,marker.bounds+6,scene_input.marker_bounds);std::copy(marker.parent_scale,marker.parent_scale+3,scene_input.marker_parent_scale);
   dh2::physical::DecorSceneOutput scene_output;
   if(dh2_decor_scene(&scene_output,&scene_input))throw std::runtime_error("Decor collision transform rejected");
   auto owner=std::make_unique<BodyOwner>();decor_bodies.push_back({});auto& body=decor_bodies.back();owner->native=&body;
   dh2::physical::DecorBodyInput decor_input{};decor_input.owner=owner.get();decor_input.new_physical=&body;decor_input.visual_present=decor_input.colbox_found=1;
   std::copy(scene_output.mesh_box,scene_output.mesh_box+6,decor_input.mesh_box);std::copy(instance.position.begin(),instance.position.end(),decor_input.position);
   dh2::physical::DecorBodyConfig definition;
   if(dh2_decor_body_config(&definition,&decor_input))throw std::runtime_error("Decor collision body definition rejected");
   owner->set_filter(definition.physical);body={actor_world.create_character(definition.physical,&owner->services),definition.physical.radius,definition.physical.pinned};
   if(!body.body)throw std::runtime_error("Decor collision body creation failed");
   decor_body_owners.push_back(std::move(owner));
  }
 }
 native_actor_ready=true;native_actor_frames=native_physics_steps=0;
 __android_log_print(ANDROID_LOG_INFO,"DH2Native","Native actor ready | genuine bodies %zu | decor colliders %zu | radius %.9g | source bounds %.6g %.6g %.6g %.6g | flags %x | scene then Step then actor",decor_bodies.size()+unsigned(bool(prince_body.body)),decor_bodies.size(),config.radius*100.f,box[0],box[1],box[3],box[4],prince_flags);
}
#include "renderer_player_cast_v2.inc"
#include "renderer_player_manager_v1.inc"
#include "renderer_gameplay_camera_live_v20.inc"
#include "renderer_player_character_panel_v3.inc"
#include "renderer_character_mutations_v4.inc"
#include "renderer_character_faery_v8.inc"
#include "renderer_character_panel_faery_v29.inc"
bool connect_character_panel_runtime_impl(const dh2::android_ui::CharacterPanelMovieRuntimeV3& movie,
 dh2::android_ui::CharacterPanelGameplayServicesV2& out,std::string& error){
 try{
  if(!player_skills_runtime||!player_skills_runtime->player||!player_skills_runtime->player->ready()){
   error="Character panel requires initialized same player skill owner";return false;
  }
  auto& t=*player_skills_runtime;
  dh2::data::PlayerSaveLoadServicesV1 save_services;save_services.owner=t.world;
  save_services.invoke=[](const auto& q,auto&,std::string& e){
   e="Required canonical Save profile/section producer "+std::to_string(unsigned(q.operation));return false;
  };
  bind_player_character_panel_v3(t,std::move(save_services),movie);
  out.owner=t.world;
  out.reload=[](const auto& p,auto& graph,auto& e){
   if(!player_skills_runtime||player_skills_runtime->world!=p.world_owner||!player_skills_runtime->character_panel_runtime){e="Character panel runtime replaced";return false;}
   return player_skills_runtime->character_panel_runtime->bind(p,graph,e);
  };
  out.queries=[](const auto& p,auto&,auto& graph,auto& e){
   if(!player_skills_runtime||!player_skills_runtime->player_manager||player_skills_runtime->world!=p.world_owner){e="Required same PlayerManager query owner";return false;}
   graph.player=[](std::int32_t index,bool remote,std::uintptr_t& identity,std::string& e){
    if(!player_skills_runtime||!player_skills_runtime->player_manager){e="PlayerManager expired";return false;}
    dh2::player::PlayerInfoFieldsV1* info{};
    if(!player_skills_runtime->player_manager->manager().get_player(index,remote,info,e))return false;
    identity=info?info->character660:0;return true;
   };return true;
   };
   if(!supplement_character_menu_mutations_v4(out,error))return false;
   return supplement_native_character_panel_faery_v29(out,error);
 }catch(const std::exception& failure){error=failure.what();return false;}
}
#include "renderer_authored_effect_scene_v5.inc"
#include "renderer_combat_sound_v2.inc"
#include "renderer_player_aggro_v2.inc"
#include "renderer_npc_death_v2.inc"
int npc_death_body_v2(MonsterScriptHandle& npc,dh2::character::State* state,const dh2::character::Request& request){return npc.death?npc.death->body(state,request):0;}
int npc_on_died_v2(MonsterScriptHandle& npc,std::uintptr_t attacker){return npc.death?npc.death->on_died(attacker,nullptr):-1;}
void clear_npc_death_owners_v2(){for(auto& group:object_groups)for(auto& actor:group.instances)if(actor.script)actor.script->death.reset();}
#include "renderer_combat_fx_runtime_v4.inc"
#include "renderer_animation_sound_v4.inc"
#include "renderer_player_animation_step_fx_v2.inc"
PlayerSkillsRuntime::~PlayerSkillsRuntime(){
 // Marker Drop reaches the SAME FX anchor/floor services. Release before
 // member destruction can invalidate World/target providers or PM ownership.
 target_marker_v28.reset();
 if(player_manager_boot_return_v25)player_manager_boot_return_v25();
 if(prince_locomotion.selection_fx_v2.context==this)prince_locomotion.selection_fx_v2={};
}
#include "renderer_player_collisions_v1.inc"
#include "renderer_player_attack_v1.inc"
#include "renderer_player_heading_v1.inc"
#include "renderer_player_target_frame_v2.inc"
#include "renderer_player_attack_animation_v1.inc"
#include "renderer_combat_text_v1.inc"
int PlayerSkillsRuntime::skill_service(void* p,dh2::character::skills::SkillAIContextV3*,
 const dh2::character::skills::SkillAIRequest32V3* q,dh2::character::skills::SkillAIResponse32V3* out){
 using namespace dh2::character::skills;auto& t=*static_cast<PlayerSkillsRuntime*>(p);
 if(!q||!out||q->character!=t.fsm.character)return -1;
 if(q->operation==skill_ai_set_state_v3){
  t.skill_state.physical=reinterpret_cast<std::uintptr_t>(&prince_body);
  t.skill_state.target=t.world->player_object->target.target;
  const SkillStateServices16V4 services{&t,PlayerSkillsRuntime::state_service};
  const int code=dh2_character_skill_state_v4(&t.skill_state,skill_state_select_v4,q->index,q->value,0,0,&services);
   if(code){t.error="Required source SkillState selection provider: "+t.error+"; "+t.player->session().error()+"; "+t.native->error();return -1;}return 0;
 }
  if(q->operation==skill_ai_trophy_manager_v3||q->operation==skill_ai_trophy_catalog_v3||q->operation==skill_ai_unlock_v3){
   if(!t.trophy_bindings)return -1;
   const int status=t.trophy_bindings->skill_ai(q,out);
   if(status)t.error=t.trophies->error();return status;
  }
  // Network continuations still require their actual owner.
 t.error="Required skill AI provider "+std::to_string(q->operation);return -1;
}
int PlayerSkillsRuntime::mana_service(void* p,const dh2::character::skills::SkillManaRequestV5* request,
 dh2::character::skills::SkillManaResponseV5* out){
 using namespace dh2::character::skills;auto& t=*static_cast<PlayerSkillsRuntime*>(p);if(!request||!out)return -1;
 switch(request->service){
 // This local development Application does not enable the source no-mana
 // bypass. The selected actual player and DebugSwitches remain authoritative.
 case mana_application_v5:out->word=t.application_mana_bypass;return 0;
 case mana_player_v5:out->identity=t.world->player_object->identity;return 0;
 case mana_debug_contains_v5:if(!request->name)return -1;out->word=t.application_debug_flags.find(request->name)!=t.application_debug_flags.end();return 0;
 case mana_debug_load_v5:return dh2_character_debug_load(t.world->debug.get(),&t.world->debug_files)==1?0:-1;
 case mana_debug_construct_v5:if(!request->name)return -1;out->identity=++t.debug_serial;t.debug_tokens[out->identity]=request->name;return 0;
 case mana_debug_get_v5:{const auto at=t.debug_tokens.find(request->subject);if(at==t.debug_tokens.end())return -1;return dh2_character_debug_get(&out->word,t.world->debug.get(),at->second.c_str(),&t.world->debug_files)==1?0:-1;}
 case mana_debug_destroy_v5:return t.debug_tokens.erase(request->subject)==1?0:-1;
 default:return -1;
 }
}
int PlayerSkillsRuntime::WorldActor::controller(void* p,dh2::character::ControllerCommandState32* out){
 auto& a=*static_cast<WorldActor*>(p);if(!out)return -1;
 if(a.machine==&prince_state){*out={reinterpret_cast<std::uintptr_t>(&prince_runtime.controller),a.object->identity,controller_global_blocked,prince_state.controller_locked,prince_controller_forced,0};return 0;}
 if(!a.npc||!a.npc->controller)return -1;const auto* source=a.npc->controller->command_state(controller_global_blocked);if(!source)return -1;*out=*source;return 0;
}
int PlayerSkillsRuntime::WorldActor::combat(void* pointer,dh2::character::skills::WorldSkillExecutionActorV6* out){
 using namespace dh2::character::skills;auto& a=*static_cast<WorldActor*>(pointer);
 if(!out||!a.runtime||!a.object||!a.machine)return -1;
 auto& t=*a.runtime;dh2::data::PropertyView* properties=nullptr;AggroStorage* aggro=nullptr;
 dh2::character::CharacterConstructorCombatFieldsV1* fields=nullptr;
 const dh2::data::FreshInventoryOwnedV4* inventory=nullptr;
 dh2::character::BuffOwner* buffs=nullptr;
 if(a.machine==&prince_state){
  if(!t.player)return -1;properties=&t.player->session().property_view();aggro=&prince_combat.aggro;fields=&t.combat_fields;
  inventory=player_equipment->inventory();buffs=t.player->native_buffs();a.combat_controller=reinterpret_cast<std::uintptr_t>(&prince_runtime.controller);
 }else{
  if(!a.npc||!a.npc->session||!a.npc->controller)return -1;
   properties=&a.npc->session->property_view();fields=&a.npc->machine->combat_fields();a.combat_controller=a.npc->controller->identity();
   const auto owned=t.attack_npc_inventories.find(a.object->identity);
   if(owned==t.attack_npc_inventories.end()||!owned->second)return -1;
    inventory=&owned->second->inventory();
    if(a.npc->death)buffs=a.npc->death->buffs();
  for(auto& group:object_groups)for(auto& actor:group.instances)if(actor.identity==a.object->identity)aggro=&actor.aggro;
 }
 if(!properties||!aggro||!fields)return -1;
 // Existing melee kernels write these exact source scalar projections; import
 // that write before the native halfword/byte bridge and publish its writes
 // back to the compatibility projection after every source execution.
 fields->combo=std::uint16_t(a.object->life->combo_hits);fields->push_death=std::uint8_t(a.object->life->push_death);
 a.combat_facts={properties->resolved,-1,-1,0,0,0,a.machine->current,fields->combo};
  if(inventory){
   const auto& set=inventory->equipment()[inventory->current_equipment()];
   const dh2::data::ItemRecord164* records[2]{};
   for(unsigned hand=0;hand<2;++hand)if(set[hand+1]){
    const auto item=dh2::data::item(inventory->table(),set[hand+1]->item->id);
    if(!item){t.error="Same actor combat weapon metadata absent";return -1;}records[hand]=&item->record;
   }
   dh2::player::EquipmentQueries12V1 queries{};
   if(dh2_equipment_queries_v1(&queries,records[0],records[1],properties->resolved[203]))return -1;
   a.combat_facts.main_damage_class=queries.main_category;a.combat_facts.off_damage_class=queries.off_category;
   a.combat_facts.dual_wield=bool(queries.flags&dh2::player::query_dual);
   a.combat_facts.shield=bool(queries.flags&dh2::player::query_shield);
   a.combat_facts.two_hander=bool(queries.flags&dh2::player::query_two_raw);
  }
 *out={a.object->identity,properties,a.object->life.get(),&a.combat_facts,a.machine,fields,inventory,buffs,nullptr,nullptr,&a.combat_controller,&aggro->outgoing_table,&aggro->incoming_table};return 0;
}
int PlayerSkillsRuntime::state_service(void* p,dh2::character::skills::SkillStateV4* fields,
 const dh2::character::skills::SkillStateRequest32V4* q,dh2::character::skills::SkillStateResponse16V4* out){
 using namespace dh2::character;using namespace dh2::character::skills;
 auto& t=*static_cast<PlayerSkillsRuntime*>(p);if(fields!=&t.skill_state||!q||!out)return -1;
 t.error="SkillState operation "+std::to_string(q->operation)+" value "+std::to_string(q->value)+" index "+std::to_string(q->index);
 const auto body=[&](Service service,int argument=0,float scalar=0){
  const Request request{service,{argument,0,0},scalar,0,0};character_service(nullptr,&prince_state,&request);return 0;};
 switch(q->operation){
 case skill_state_debug_load_v4:return dh2_character_debug_load(t.world->debug.get(),&t.world->debug_files)==1?0:-1;
 case skill_state_debug_construct_v4:{const auto* text=reinterpret_cast<const char*>(q->payload);if(!text)return -1;out->identity=++t.debug_serial;t.debug_tokens[out->identity]=text;return 0;}
 case skill_state_debug_get_v4:{const auto text=t.debug_tokens.find(q->subject);if(text==t.debug_tokens.end())return -1;return dh2_character_debug_get(&out->word,t.world->debug.get(),text->second.c_str(),&t.world->debug_files)==1?0:-1;}
 case skill_state_debug_destroy_v4:return t.debug_tokens.erase(q->subject)==1?0:-1;
 case skill_state_stop_v4:return body(stop);
 case skill_state_raise_v4:{
  const Request request{raise_event,{int(q->value),0,0},0,0,0};character_service(nullptr,&prince_state,&request);
  if(q->value==0x1e||q->value==0x1f){std::uint32_t result=0;return t.player->skill_ai(q->value==0x1e?skill_ai_focus_v3:skill_ai_blur_v3,0,&result);}
  return -1;
 }
 case skill_state_animation_v4:{int sequence;std::memcpy(&sequence,&q->value,4);
  if(sequence==-1){sequence=prince_state.animation_override;prince_state.animation_override=-1;}
  return body(set_animation,sequence);}
 case skill_state_speed_v4:{float speed;std::memcpy(&speed,&q->value,4);return body(set_speed,0,speed);}
 case skill_state_cancel_sneaking_v4:return t.player->native_cancel_sneaking(t.source_byte415);
 case skill_state_pin_v4:return dh2_native_body_pin(&prince_body);
 case skill_state_unpin_v4:return dh2_native_body_unpin(&prince_body);
 case skill_state_timer_v4:return t.player->session().start_timer(q->value,0,int(q->index))<0?-1:0;
 case skill_state_monster_v4:{std::uint32_t player=0;if(t.player->session().source_is_player(player))return -1;out->word=!player;return 0;}
 case skill_state_row_v4:{if(q->index>=t.save->skills().size())return -1;const auto id=t.save->skill_id(q->index);const auto table=t.world->skills_owner->borrow();if(id<0||std::size_t(id)>=table.skills().size())return -1;out->identity=reinterpret_cast<std::uintptr_t>(&table.skills()[id].scalar);return 0;}
 case skill_state_constant_v4:{std::int32_t value;if(t.player->session().constant("AnimStancedAnim","SL__LIST_IPHONE",value))return -1;std::memcpy(&out->word,&value,4);return 0;}
 case skill_state_stance_v4:out->word=prince_facts().stance;return 0;
 case skill_state_state_event_v4:return prince_event(q->value,q->payload)<0?-1:0;
 case skill_state_transition_v4:return prince_state_owner.transition(q->index,q->value,q->payload,prince_owner_services)<0?-1:0;
 case skill_state_step_index_v4:{const auto& frames=prince_locomotion.scheduler.frames();if(frames.empty())return -1;out->word=frames.front().step;return 0;}
 case skill_state_step_count_v4:{const auto& frames=prince_locomotion.scheduler.frames();if(frames.empty())return -1;const auto root=frames.front().sequence;if(root<0||std::size_t(root)>=actor_animation_tables.sequences.size())return -1;out->word=actor_animation_tables.sequences[root].steps.size();return 0;}
 case skill_state_current_v4:out->word=prince_state.current;return 0;
  case skill_state_use_v4:return player_skill_animation_event_v6(t);
  case skill_state_named_prefix_v4:{
   if(q->value==0&&t.animation_events){
    if(t.animation_events->relay(reinterpret_cast<const char*>(q->payload))){
     __android_log_print(ANDROID_LOG_INFO,"DH2Native","Original player animation event delivered | name %s | lag %d | Lua status %u | same Script/World/Scene/PFFloor",reinterpret_cast<const char*>(q->payload),t.animation_lag,t.animation_events->source_lua_error());
     return 0;
    }
    t.error=t.animation_events->error();return -1;
   }
   if(q->value==2&&q->payload&&t.combat_fx){
    // CSAttack strips fx_; CharAI::_OnAnimEvent3d4434 passes the original
    // prefixed name to the same MeshFxManager's ordered set-name lookup.
    const std::string authored="fx_"+std::string(reinterpret_cast<const char*>(q->payload));
    if(t.combat_fx->animation_event(authored.c_str(),t.fsm.character)){
     t.error=t.combat_fx->error();return -1;
    }
     return 0;
    }
    if(q->value==3)return source_named_animation_sound_v38(t,t.fsm.character,reinterpret_cast<const char*>(q->payload));
   t.error="Required source animation prefix "+std::to_string(q->value)+" event "+(q->payload?reinterpret_cast<const char*>(q->payload):"<null>");return -1;
  }
 default:t.error="Required source SkillState provider "+std::to_string(q->operation);return -1;
 }
}
int prince_owner_service(void*,dh2::character::StateOwnerMachine40* machine,
 const dh2::character::StateOwnerRequest48* request,dh2::character::StateOwnerResponse8* out){
 using namespace dh2::character;using namespace dh2::character::skills;
 if(machine!=&prince_state_owner.machine()||!request||!out)return -1;
 auto facts=prince_facts();CharacterFactsScope borrow(facts);
 if(request->operation==state_owner_predicate){
  const StateOwnerPredicateFacts16 predicates{prince_state.flags,prince_state.attack_gate,prince_state_predicates.interaction,prince_state_predicates.can_interrupt,std::uint8_t(prince_state.stop_attack_allowed),{0,0}};
  return dh2_character_state_owner_predicate(out,request->source_function,request->state,&predicates)==1?0:-1;
 }
 if(request->operation==state_owner_blur||request->operation==state_owner_focus||request->operation==state_owner_event){
  if(request->state==7){
   if(!player_skills_runtime||!player_skills_runtime->cast)return -1;
   return player_skills_runtime->cast->state_body(player_gameplay_binding(),request->operation==state_owner_blur?dh2::android_ui::CastBodyV2::blur:request->operation==state_owner_focus?dh2::android_ui::CastBodyV2::focus:dh2::android_ui::CastBodyV2::event);
  }
  if(request->state==6){
   if(!player_skills_runtime||!player_skills_runtime->player->ready())return -1;
   auto& t=*player_skills_runtime;const SkillStateServices16V4 services{&t,PlayerSkillsRuntime::state_service};
   return dh2_character_skill_state_v4(&t.skill_state,request->operation==state_owner_blur?skill_state_blur_v4:request->operation==state_owner_focus?skill_state_focus_v4:skill_state_event_v4,request->event,0,request->payload,0,&services);
  }
  return (request->operation==state_owner_blur?dh2_character_state_blur_body(&prince_state,&facts,&prince_services):request->operation==state_owner_focus?dh2_character_state_focus_body(&prince_state,&facts,request->other,request->payload,&prince_services):dh2_character_state_event_body(&prince_state,&facts,request->event,&prince_services))==1?0:-1;
 }
 if(request->operation==state_owner_character_event){const Request event{raise_event,{int(request->event),request->state,0},0,0,request->payload};character_service(nullptr,&prince_state,&event);return 0;}
 if(request->operation==state_owner_pin)return dh2_native_body_pin(&prince_body);
 // Native profiling records real transitions; no original profiler object is
 // represented by these counters or published as a successful game service.
 if(request->operation==state_owner_profile_begin||request->operation==state_owner_profile_end){
  __android_log_print(ANDROID_LOG_VERBOSE,"DH2Native","Native transition profile | phase %u | state %d | event %x",request->operation,prince_state.current,prince_event_cause);return 0;
 }
 return -1;
}
void PlayerSkillsRuntime::bind_world_touch(PlayerSkillsRuntime& t){
 if(!t.world_touch)t.world_touch=std::make_unique<dh2::world::RendererWorldTouchLiveV2>();
 // Rebinding replaces the pointer's live floor context. Cancel deferred input
 // without replaying release selection or mutating the accepted target.
 t.world_touch->pending.pending14c8=0;t.world_touch->pending.skill14ca=-1;
 auto& b=t.world_touch->same;b.world=t.targets.get();b.floors=level.native_floor.get();
 b.machine=&prince_state;b.target=&t.world->player_object->target;b.target_services=t.world->player_object->binding.services;
 b.submitted_camera=submitted_camera.data();b.width=&submitted_width;b.height=&submitted_height;
 b.source_screen_ray_v20=gameplay_camera_ray_v20;
 b.blocked=&controller_global_blocked;b.forced=&prince_controller_forced;b.settings=t.world->saved_options.get();
 const auto row=t.world->settings.row_index("Default");if(row<0)throw std::runtime_error("Actual Default DesignSettings row missing");
 b.design=&t.world->settings.rows().at(row);
 b.debug=[&t](std::uint32_t site,bool& value,std::string& error){
  const char* key=nullptr;switch(site){case 0x3ae074:case 0x3ae248:case 0x3ae318:key="isTracingChar_CTRL";break;case 0x3ae208:key="UseClickToMove";break;default:error="Unknown source Ctrl_Click Debug callsite";return false;}
  std::uint32_t word{};if(dh2_character_debug_load(t.world->debug.get(),&t.world->debug_files)!=1||dh2_character_debug_get(&word,t.world->debug.get(),key,&t.world->debug_files)!=1){error="Required same source Ctrl_Click Debug owner/files";return false;}value=word!=0;return true;
 };
}
void initialize_player_skills(bool restore){
 if(restore&&player_skills_runtime){
  if(player_skills_runtime->world!=live_script_world||
    player_skills_runtime->player->session().properties()!=prince_combat.property_owner||
    player_skills_runtime->player->session().combat_state()!=prince_combat.life_owner){
     throw std::runtime_error("Retained SkillV6 authority differs from equipped actor");
   }
   // The retained Script/Skill owner survives a visual/world reload, while
   // load_world replaces the floor storage. Refresh this borrow before any
   // source animation event can inspect the player's current cached PFFloor.
    player_skills_runtime->animation_floor={level.native_floor.get(),&prince_runtime.object.motion.floor};
    // initialize_native_actor recreated the physical owner. Borrow its new
    // source primary shape and constructor fields, keeping the Skill owner.
    player_skills_runtime->collision_primary=prince_body.body?prince_body.body->GetShapeList():nullptr;
    player_skills_runtime->collision_secondary=nullptr;
    player_skills_runtime->collision_filter_disabled=0;
     PlayerSkillsRuntime::bind_collisions(*player_skills_runtime);
     PlayerSkillsRuntime::bind_world_touch(*player_skills_runtime);
      PlayerSkillsRuntime::bind_combat_fx(*player_skills_runtime);
    for(auto& group:object_groups)for(auto& actor:group.instances)if(actor.kind==1&&actor.script){
      actor.script->death=std::make_shared<RendererNpcDeathV2>(*actor.script,*player_skills_runtime,actor.script->source_ai_group34);
    }
   player_skills_runtime->player->session().set_position(actor_position);return;
 }
 clear_npc_death_owners_v2();player_skills_runtime.reset();
 auto runtime=std::make_unique<PlayerSkillsRuntime>();runtime->world=live_script_world;
 auto& t=*runtime;const auto id=live_script_world->player_object->identity;
 t.save=std::make_shared<dh2::data::PlayerSavegameV1>();t.save->set_character(id);
 const auto tables=t.world->skills_owner->borrow();const int list=prince_combat.properties().resolved[28];
 if(list<0||std::size_t(list)>=tables.lists().size()||!t.save->initialize_skills(tables.lists()[list],t.error)){
  throw std::runtime_error("Player SkillV6 actual saved-list initialization failed: "+t.error);
 }
  t.save->initialize_faeries();
  if(!t.world->saved_options){
   std::array<std::vector<std::uint8_t>,3> options;
   const char* uris[]{"data/pydata/design_pyarray.bin","data/pydata/design_pyarraynames.bin","data/pydata/design_pystructnames.bin"};
   for(unsigned i=0;i<options.size();++i){bool found=false;
    if(!equipment_platform->cache.read(uris[i],found,options[i],t.error)||!found)throw std::runtime_error("Original GameOptionTable missing: "+t.error);
   }
   if(!t.world->game_options.load_design_cache({options[0].data(),options[0].size()},{options[1].data(),options[1].size()},{options[2].data(),options[2].size()},t.error))throw std::runtime_error("Original GameOptionTable rejected: "+t.error);
   // Same retained Application settings owner. Source constructor has an empty
   // option map; campaign/menu loadSettings is a separate required lifecycle.
   t.world->saved_options=std::make_unique<dh2::ui::OwnedHudSettingsV1>(t.world->game_options.borrow());
  }
  // The original source requests TrophyManager unconditionally on BeginSkill.
  // Load its authored catalog directly from the bundled original cache and
  // retain one manager alongside the same player/Save/World authorities.
  std::array<std::vector<std::uint8_t>,3> trophy_bytes;
  const char* trophy_files[]{"data/pydata/trophies_pyarray.bin","data/pydata/trophies_pyarraynames.bin","data/pydata/trophies_pystructnames.bin"};
  for(unsigned i=0;i<trophy_bytes.size();++i){bool found=false;
   if(!equipment_platform->cache.read(trophy_files[i],found,trophy_bytes[i],t.error)||!found)
    throw std::runtime_error("Original trophy catalog unavailable: "+t.error);
  }
  const auto trophy_catalog=dh2::trophies::TrophyCatalogV1::load(
   {trophy_bytes[0].data(),trophy_bytes[0].size()},{trophy_bytes[1].data(),trophy_bytes[1].size()},
   {trophy_bytes[2].data(),trophy_bytes[2].size()},t.error);
  t.trophies=dh2::trophies::TrophyManagerOwnerV1::create(trophy_catalog,t.world->debug.get(),&t.world->debug_files,{},t.error);
  if(!t.trophies)throw std::runtime_error("Original trophy owner unavailable: "+t.error);
  t.trophy_bindings=std::make_unique<dh2::trophies::TrophyNativeBindingsV1>(*t.trophies);
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Original trophy manager retained | catalog %zu | same player SkillAI owner | unlock notification/save services required",t.trophies->data().size());
 t.targets=std::make_unique<dh2::character::skills::CharacterWorldRuntimeV1>(*t.world->tables.ai());
 auto register_actor=[&](const std::shared_ptr<dh2::character::ScriptCharacterObject>& object,dh2::character::State* machine,MonsterScriptHandle* npc=nullptr){
  auto& a=t.world_actors[object->identity];a.runtime=&t;a.object=object;a.machine=machine;a.npc=npc;
  a.search.identity=object->identity;a.search.visible=1;
  a.character={object->identity,object->properties->resolved.data(),object->name.c_str(),machine?machine->flags:0,std::uint8_t(object->life->dead),0,1,0};
   dh2::character::CharacterTargetabilityOwnerV1 targetability(a.character);
   if(targetability.construct()!=1)throw std::runtime_error("Original nested CharAI targetability construction failed");
  dh2::character::skills::WorldActorRegistrationV1 registration{};
  registration.identity=object->identity;
  registration.context=&a;registration.refresh=PlayerSkillsRuntime::WorldActor::refresh;registration.machine=machine;
  if(npc)a.shared_handle=&npc->shared_handle();
  else {
    if(!t.world->player_canonical||!t.world->player_canonical->bound)
     throw std::runtime_error("Required published same-player canonical Handle");
    a.shared_handle=&t.world->player_canonical->handle;
   }
   registration.handle_key=a.shared_handle->key;
   if(!t.world->canonical_world||!t.world->canonical_world->manager.object(registration.handle_key))
    throw std::runtime_error("Required canonical publication before skill target registration");
  registration.shared_handle=a.shared_handle;registration.current_target=&object->target.target;
  registration.controller=PlayerSkillsRuntime::WorldActor::controller;
  if(t.targets->add(registration))throw std::runtime_error("Actual skill World registration failed: "+t.targets->error());
 };
  register_actor(t.world->player_object,&prince_state);
  t.source_byte415=&t.world_actors.at(id).character.interactive415;
   for(const auto& group:object_groups)for(const auto& actor:group.instances)if(actor.kind==1&&actor.script){
    if(!actor.script->machine)throw std::runtime_error("Actual NPC state owner missing before World registration");
    register_actor(actor.script->object,&actor.script->machine->state(),actor.script.get());
   }
   bind_npc_injuries(t);
   bind_npc_inventories_v1(t);
  dh2::character::CharacterWorldNpcInitializationV1 npc_initialization(*t.targets,t.world->initialization);
  unsigned initialized_npcs=0;
  for(const auto& group:object_groups)for(const auto& actor:group.instances)if(actor.kind==1&&actor.script){
   if(actor.script->machine->state().current!=-1)continue;
   const dh2::character::WorldNpcInitializationBorrowV1 borrowed{actor.identity,actor.room,actor.name.c_str(),actor.script->machine.get(),actor.script->session.get()};
   if(npc_initialization.initialize(borrowed)!=1)throw std::runtime_error("Original NPC level state failed: "+npc_initialization.error());
   ++initialized_npcs;
   __android_log_print(ANDROID_LOG_INFO,"DH2Native","Original NPC level state | %s | current %d | flags %x | clip %d | same World/ScriptSession/FSM/HP",actor.name.c_str(),actor.script->machine->state().current,actor.script->machine->state().flags,actor.script->animation->playback().current_clip());
  }
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Original Level LoadCharStates complete | NPCs %u | authored preset | no synthetic interaction flags",initialized_npcs);
 if(t.targets->refresh())throw std::runtime_error("Actual skill World projection refresh failed");
 t.fsm.character=id;t.skill_owner.character=id;
 t.event_owner={id,reinterpret_cast<std::uintptr_t>(&prince_runtime.controller),reinterpret_cast<std::uintptr_t>(&prince_state),reinterpret_cast<std::uintptr_t>(prince_combat.property_owner.get()),0,0,0,0};
 std::copy_n(dh2::android_ui::player_char_ai_keys_v1(),51,t.ai_virtuals.begin());
  std::copy_n(dh2::android_ui::player_iphone_ai_keys_v1(),51,t.ais_virtuals.begin());
 t.events={live_script_world->player_object->target.identity,&t.event_owner,t.ai_virtuals.data(),0,t.ais_virtuals.data(),0,0,0,0,0,0};
 dh2::character::CharacterScriptSessionInputV3 in{};
 in.identity=id;in.name=live_script_world->player_object->name;in.source_is_character=1;
 in.properties=prince_combat.property_owner;in.combat=prince_combat.life_owner;
 in.temporary=t.temporary;in.position=actor_position;in.savegame=t.save;in.state_machine=&t.fsm;
 in.common=t.world->script_files.common();in.cached_file_context=&t;in.cached_file=PlayerSkillsRuntime::cached;
 in.host=&t.world->host;in.level=&t.world->level_services;in.target=&t.world->player_object->binding;
 in.objects=&t.world->objects->services();
 const dh2::character::skills::SkillManaServicesV5 mana{&t,PlayerSkillsRuntime::mana_service};
 t.native=std::make_unique<dh2::character::skills::CharacterSkillNativeReadOnlyBindingsV6>(*player_equipment->inventory(),t.fsm,mana,t.targets->native_world(id,in.objects));
 bind_development_player_manager_v2(t,{0,0,0,true,id,nullptr});
 const auto aggro_config=read(equipment_platform->manager,"crypt01-level-config-music-v1.bin","worlds");
 bind_player_aggro_v2(t,{aggro_config.data(),aggro_config.size()});
 dh2::character::skills::WorldSkillCombatBackendsV6 combat_services{};
 combat_services.hit={&t,player_manager_hit_v2};
 combat_services.aggro_context=&t;combat_services.aggro_event=player_aggro_callback_v2;
  combat_services.application={&t,[](void* p,const dh2::character::skills::SkillApplyRequestV6* q,dh2::character::skills::SkillApplyResponseV6* out,dh2::data::CombatResult* result){
   using namespace dh2::character::skills;auto& t=*static_cast<PlayerSkillsRuntime*>(p);if(!q||!out)return -1;
   if(q->service==skill_apply_hit_fx_v6){if(!result){t.error="HitFX requires actual CombatResult";return -1;}const auto handled=t.combat_fx_application(*q,*result);return handled==1?0:-1;}
   if(q->service==skill_apply_sound_v6){if(!result)return -1;const auto handled=combat_sound_application_v2(t,*q,*result);return handled==1?0:-1;}
    if(t.player_manager){const auto handled=t.player_manager->application(*q,out);
    if(handled){if(handled<0)t.error=t.player_manager->error();return handled==1?0:-1;}}
   const auto actor=t.world_actors.find(q->subject);
   if(actor!=t.world_actors.end()&&actor->second.npc&&actor->second.npc->injury){
    const auto handled=actor->second.npc->injury->application(*q,out);if(handled)return handled==1?0:-1;
   }
  if(q->service==skill_apply_online_v6||q->service==skill_apply_party_count_v6){std::uintptr_t identity=0;std::int32_t scalar=0;std::string error;
   const auto operation=q->service==skill_apply_online_v6?dh2::player::EquipmentWorldQueryV1::online:dh2::player::EquipmentWorldQueryV1::player_count;
   if(!EquipmentPlatform::query(equipment_platform.get(),operation,q->subject,identity,scalar,error)){t.error=error;return -1;}
   out->word=std::uint32_t(scalar);return 0;
  }
   if(q->service==skill_apply_saved_option_v6){
   if(!q->name||!t.world->saved_options)return -1;const auto& options=*t.world->saved_options;
   const auto* descriptor=options.descriptor(q->name);
   out->word=options.has_option(q->name)&&descriptor&&descriptor->type==0&&options.option(q->name)==descriptor->maximum;
    return 0;
   }
   if(q->service==skill_apply_scrolling_text_v6){
    if(!result)return -1;
    try{apply_combat_text_v1(*result,q->attacker,q->target);return 0;}
    catch(const std::exception& e){t.error=e.what();return -1;}
   }
   if(q->service==skill_apply_ai_combat_v6){
    if(!result||actor==t.world_actors.end()){t.error="AI combat result requires actual registered actor/result";return -1;}
    dh2::character::CombatResultsAiBorrowV1 borrowed{};
    if(actor->second.npc){
     auto* session=actor->second.npc->session.get();
     if(!session){t.error="AI combat result requires retained NPC ScriptSession";return -1;}
     borrowed.owner=&session->owner();
     if(!session->owner().source_combat_results_callback(borrowed.source_character_callback)){
      dh2::character::ScriptSessionView selected{};if(session->owner().active(selected)){t.error="Selected NPC AIS combat callback producer unavailable";return -1;}
     }
    }else{
     if(!t.player){t.error="AI combat result requires retained player ScriptSession";return -1;}
     auto& session=t.player->session();borrowed.owner_v2=&session.owner();
     borrowed.scope=session.current_skill_callback_scope();
     borrowed.source_character_callback=std::uint32_t(t.ais_virtuals[0xb4/4]);
    }
    std::string diagnostic;const auto status=dh2::character::character_combat_results_ai_v1(borrowed,q->attacker,q->target,std::uint8_t(result->outcomes),diagnostic);
    if(status<0){t.error=diagnostic;return -1;}
    if(status)__android_log_print(ANDROID_LOG_ERROR,"DH2Native","Original AIS combat result Lua diagnostic | actor %016llx | %s",static_cast<unsigned long long>(q->subject),diagnostic.c_str());
    return 0;
   }
   t.error="Required original skill application service "+std::to_string(q->service);return -1;
 },nullptr};
 t.execution=std::make_unique<dh2::character::skills::CharacterWorldSkillExecutionV6>(*t.targets,*t.world->tables.ai(),*t.native,*player_equipment->inventory(),t.fsm,t.world->skills_owner->borrow(),t.combat_context,combat_random,*t.world->debug,t.world->debug_files,t.trophy_bindings.get(),combat_services);
 in.gameplay_context=t.execution.get();in.gameplay_binding=dh2::character::skills::CharacterWorldSkillExecutionV6::binding;
  dh2::character::skills::PlayerSkillInitServicesV3 init{&t,PlayerSkillsRuntime::vitals};
  if(!t.world->player_faery_v8)t.world->player_faery_v8=std::make_shared<dh2::character::PlayerFaeryAssociationV8>(id);
  if(t.world->player_faery_v8->character!=id)throw std::runtime_error("Retained source CharAI faery field belongs to another Character");
 init.gameplay.ai=&t.events;init.gameplay.events={&t,PlayerSkillsRuntime::event_service,63,0};init.gameplay.skill_owner=&t.skill_owner;
 init.gameplay.skill_services={&t,PlayerSkillsRuntime::skill_service};
 init.gameplay.difficulty_context=&t;init.gameplay.difficulty=PlayerSkillsRuntime::difficulty;
 t.player=dh2::character::skills::CharacterPlayerSkillsV6::create(t.world->design->borrow(),in,tables,t.world->script_files.faeries(),t.world->effect_services,t.fsm,init,t.error);
 if(!t.player||t.execution->attach(t.player->session()))throw std::runtime_error("Original SkillV6 execution attach failed: "+(t.player?t.execution->error():t.error));
 for(auto& entry:t.world_actors)if(t.execution->add({entry.first,&entry.second,PlayerSkillsRuntime::WorldActor::combat}))throw std::runtime_error("Original SkillV6 actor registration failed: "+t.execution->error());
  if(t.player->initialize(1)!=1||t.native->install_object_binding())throw std::runtime_error("Original equipped SkillV6 initialization failed: "+t.player->error()+"; "+t.native->error());
  t.events.active=t.player->session().owner().lifecycle().active;
  t.animation_floor={level.native_floor.get(),&prince_runtime.object.motion.floor};
  t.animation_visual=reinterpret_cast<std::uintptr_t>(&prince_visual);
  // The original CRootSceneNode owns all serialized scene roots. Its wrapper
  // is represented by UINT32_MAX in the flattened native Scene graph.
  t.animation_root=UINT32_MAX;
  const dh2::character::AnimationEventBorrowV1 animation_actor{
   id,&t.player->session().owner(),&t.events.active,&t.animation_lag,&t.player->session().property_view(),
   t.targets.get(),&t.animation_visual,&current_scene,&t.animation_root,&t.animation_floor,dh2::character::animation_floor_type_v1};
  t.animation_events=std::make_unique<dh2::character::CharacterAnimationEventOwnerV1>(t.events,animation_actor,t.world->effects_owner->borrow(),nullptr);
 if(t.player->session().properties()!=player_equipment->properties()||t.player->session().combat_state()!=prince_combat.life_owner||t.player->native_savegame()!=t.save.get())
  throw std::runtime_error("SkillV6/Save/Gear shared authority verification failed");
 __android_log_print(ANDROID_LOG_INFO,"DH2Native","Retained player SkillV6 ready | character %016llx | skills %u | spells %u | saved rows %zu | buffs %u | same Gear props/life/Save | source level0/empty slots",
  static_cast<unsigned long long>(id),t.player->state().skills.count,t.player->state().spells.count,t.save->skills().size(),t.player->buff_count());
 player_skills_runtime=std::move(runtime);
   PlayerSkillsRuntime::bind_cast(*player_skills_runtime);
   PlayerSkillsRuntime::bind_attack(*player_skills_runtime);
   PlayerSkillsRuntime::bind_heading(*player_skills_runtime);
   PlayerSkillsRuntime::bind_target_frame(*player_skills_runtime);
   PlayerSkillsRuntime::bind_world_touch(*player_skills_runtime);
     PlayerSkillsRuntime::bind_combat_fx(*player_skills_runtime);
   player_skills_runtime->collision_primary=prince_body.body?prince_body.body->GetShapeList():nullptr;
   PlayerSkillsRuntime::bind_collisions(*player_skills_runtime);
 std::string initial_error;
 const auto gameplay=player_gameplay_binding();
 const bool initialized=dh2::android_ui::initialize_player_skill_slots_v1(gameplay,initial_error);
 __android_log_print(initialized?ANDROID_LOG_INFO:ANDROID_LOG_ERROR,"DH2Native",
  "Original initial skill slots | ready %u | row0 level %d | slot0 %d | unspent %d | %s",
  unsigned(initialized),gameplay.save->skill_level(0),gameplay.save->skill_in_slot(0),
  gameplay.properties->resolved[157],initial_error.c_str());
}
void advance_native_actor(unsigned dt_ms){
 std::string error;
 if(frozen||character_panel_open)return; // Preserve the composed live pose, clocks and gameplay state.
 request_prince_death();
  scene_clock+=float(dt_ms);
  prince_scene_phase=true;
  if(!prince_locomotion.scene_phase(std::uint32_t(scene_clock),prince_attack_clips,prince_visual,current_scene,error))throw std::runtime_error(error);
   prince_scene_phase=false;
   npc_scene_phase(dt_ms);
 actor_world.update(dt_ms);++native_physics_steps;
 // Original Character.Update executes CharTimers before AI, state machine,
 // animator and GameObject. ScriptManager blocking remains an explicit zero
 // fact for this development scene, which has no source script manager yet.
 if(player_skills_runtime){auto& t=*player_skills_runtime;
   t.event_owner.forced=prince_controller_forced;t.event_owner.locked=prince_state.controller_locked;t.events.global_blocked=controller_global_blocked;
    const auto timers=dh2::android_ui::player_recurring_effects_v7(*t.player,dt_ms,0);
    if((timers.timer_scan!=1||!timers.owner_ready_after)&&!t.timer_failure_logged){t.timer_failure_logged=true;__android_log_print(ANDROID_LOG_ERROR,"DH2Native","Required sole player recurring delivery | scan %d | ready %d | %s | %s | %s",timers.timer_scan,timers.owner_ready_after,t.error.c_str(),timers.owner_error_after.c_str(),timers.session_error_after.c_str());}
    if(t.update_target_frame())throw std::runtime_error("Source player target frame failed: "+t.error);
  }
 // Authored joystick Update/release already delivered the whole controller
 // command. The hidden development axis uses the same source command owner.
 if(!source_hud_heading&&player_skills_runtime&&player_skills_runtime->heading_owner){
  const bool active=std::hypot(move_x,move_y)>.08f;
  const float input[3]{move_x,move_y,0};
  if((active||prince_runtime.controller.heading.active)&&!source_player_heading_v1(active?input:nullptr,!active,error))throw std::runtime_error(error);
 }
 auto facts=prince_facts();CharacterFactsScope borrow(facts);
 if(prince_state.current==7&&player_skills_runtime&&player_skills_runtime->cast){
   prince_state.elapsed_ms+=dt_ms;
   if(player_skills_runtime->cast->state_body(player_gameplay_binding(),dh2::android_ui::CastBodyV2::update))throw std::runtime_error("Original cast state update failed");
  }else if(prince_state.current==6&&player_skills_runtime){
  prince_state.elapsed_ms+=dt_ms;auto& t=*player_skills_runtime;const dh2::character::skills::SkillStateServices16V4 services{&t,PlayerSkillsRuntime::state_service};
  if(dh2_character_skill_state_v4(&t.skill_state,dh2::character::skills::skill_state_update_v4,0,0,0,0,&services))throw std::runtime_error("Source SkillState update failed");
 }else if(dh2_character_state_update(&prince_state,&facts,dt_ms,&prince_services)<0)throw std::runtime_error("Character state update failed");
 prince_flags=prince_state.flags;prince_move_type=prince_state.move_type;walking=prince_state.current==4;
 const float global_speed=(prince_state.current==4||prince_state.current==5)?prince_state.cached_speed:1.f;
 const bool moving=prince_state.current==4;
 const auto extra=prince_locomotion.completion.extra_ms;
 if(!prince_locomotion.animator_phase(actor_animation_tables,actor_random,prince_attack_clips,prince_visual,current_scene,global_speed,extra,error))throw std::runtime_error(error);
 dh2::move::Policy decoded{};dh2_move_policy(&decoded,&prince_flags);
 // Avoidance policy and the original game camera remain explicit integration
 // boundaries. The floor/path/root/body coordinators execute genuine source.
 const dh2::actor::RuntimePolicy policy{{1,0,0,decoded.position_from_physics},1,0,0,0,dh2::actor::base_virtual_speed};
 const dh2::subobjects::Services services{nullptr,actor_virtual_service};
 const dh2::actor::RuntimeRequest request{&prince_runtime,prince_body.body?&prince_body:nullptr,&prince_visual,&current_scene,&level.native_floor->collision_world,&level.native_floor->graph,&live_registry,&live_motion_policy,&live_workspace,nullptr,prince_combat.properties().resolved.data(),&policy,&services,nullptr,0x100000001ull,prince_flags,dt_ms};
 dh2::actor::RuntimeResult result{};
 if(dh2::actor::update_actor(result,request,error))throw std::runtime_error(error);
 const float dx=prince_runtime.subobjects.position[0]-actor_position[0],dy=prince_runtime.subobjects.position[1]-actor_position[1];
 if(moving){if(dx*dx+dy*dy>0.000001f)++movement_steps;else ++blocked_steps;++native_heading_updates;}
 std::copy(prince_runtime.subobjects.position,prince_runtime.subobjects.position+3,actor_position.begin());heading=prince_runtime.subobjects.rotation;
 if(!live_script_world||!live_script_world->player_object||
    live_script_world->player_object->properties!=prince_combat.property_owner||
    live_script_world->player_object->life!=prince_combat.life_owner)
  throw std::runtime_error("Player scene backing changed during native frame");
 live_script_world->player_object->position=actor_position;
  if(player_skills_runtime){
  player_skills_runtime->player->session().set_position(actor_position);
  player_skills_runtime->targets->begin_frame(native_actor_frames);
   if(player_skills_runtime->targets->refresh())throw std::runtime_error("Skill World current-frame refresh failed");
  }
  npc_animator_phase(dt_ms);
 ++native_actor_frames;
 const auto physical_position=prince_body.body?prince_body.body->GetPosition():b2Vec2(actor_position[0]*.01f,actor_position[1]*.01f);
 if(native_actor_frames==1||native_actor_frames%120==0)__android_log_print(ANDROID_LOG_INFO,"DH2Native","Native actor frame | scene %u | Step %u | actor %u | source phase %u | clip %d | ms %d | replays %u | body %.6g %.6g | contacts %u %u | state %d | body present %d | timeline scale %.9g",prince_locomotion.root_timestamp,native_physics_steps,native_actor_frames,result.phase,prince_locomotion.current_clip(),prince_locomotion.current_timeline().current_ms,prince_locomotion.restarts,physical_position.x,physical_position.y,prince_body_owner.additions,prince_body_owner.results,prince_state.current,int(bool(prince_body.body)),prince_locomotion.current_timeline().scale);
}
}
std::string load_world(const std::uint8_t* descriptor,std::size_t size,AAssetManager* assets,const std::string& files_directory,const std::string& selected_mlx_uri){
  release_class_previews();menu_background=class_scene=false;
  std::vector<Draw> environment;std::vector<GLuint> textures;
  std::vector<ObjectGroup> candidate_groups;
  const bool restore=world_mode||resume_world;const auto previous=actor_position;
  const auto previous_heading=heading;
  const auto previous_random=actor_random;
  if(restore&&!object_groups.empty()){saved_actors.clear();for(const auto& group:object_groups)for(const auto& actor:group.instances)if(actor.kind==1)saved_actors.push_back(actor);}
  try{
    auto raw=read(assets,"crypt.bdae","worlds");dh2::resources::BresView view{};
    if(dh2_bres_open(&view,raw.data(),raw.size())!=dh2::resources::BresError::ok)throw std::runtime_error("World BRES rejected");
    dh2::world::Level candidate;std::string error;if(!dh2::world::load(view,descriptor,size,candidate,error))throw std::runtime_error(error);
    const auto script_context=restore&&live_script_world?live_script_world:std::make_shared<WorldScriptContext>(load_game_design(assets),load_design_settings(assets),load_skill_tables(assets),load_effects_tables(assets),files_directory);
    if(!script_context->script_files){
      dh2::android_ui::OriginalCacheAssetsV1 cache(assets);
      dh2::character::ScriptAssetServicesV1 services;
      services.directory=[&](auto& out,auto& e){return cache.directory(out,e);};
      services.read=[&](const auto& uri,auto& found,auto& out,auto& e){return cache.read(uri,found,out,e);};
      if(!script_context->script_assets.load(services,script_context->skills_owner->borrow(),error))
        throw std::runtime_error("Complete authored script cache unavailable: "+error);
      script_context->script_files=script_context->script_assets.borrow();
    }
    __android_log_print(ANDROID_LOG_INFO,"DH2Native","Owned script cache ready | scripts %zu | bytes %zu | faeries %zu | retained %u | actual APK cache",
      script_context->script_files.files().size(),script_context->script_files.bytes(),
      script_context->script_files.faeries().faeries().size(),unsigned(restore));
    const auto& effects=script_context->effects->source();
    __android_log_print(ANDROID_LOG_INFO,"DH2Native","Effects native backing | sets %zu | character rows %zu | footsteps %zu | dictionary %zu | consumed %zu %zu %zu | snapshot %016llx | modules %016llx | retained %u | source cache owned | FX factory pending",
     effects.sets().size(),effects.characters().size(),effects.footsteps().size(),effects.dictionary().values.size(),effects.set_end(),effects.character_end(),effects.data_consumed(),
     static_cast<unsigned long long>(reinterpret_cast<std::uintptr_t>(script_context->effects.get())),
     static_cast<unsigned long long>(reinterpret_cast<std::uintptr_t>(script_context->effect_modules.get())),unsigned(restore));
    const auto& character_table=*script_context->tables.characters();
    const auto& class_table=*script_context->tables.classes();
    const auto& property_rules=*script_context->tables.rules();
    const auto& ai_tables=*script_context->tables.ai();
    const auto model_names=read(assets,"character_models_dictionary_pyarraynames.bin","data"),model_values=read(assets,"character_models_dictionary_pyarray.bin","data");
    dh2::data::Dictionary model_table;
    if(!dh2::data::load_dictionary({model_names.data(),model_names.size()},{model_values.data(),model_values.size()},model_table,error))throw std::runtime_error(error);
    if(character_table.fields[19]!="Level"||character_table.fields[38]!="Max_HP"||character_table.fields[43]!="Max_MP")throw std::runtime_error("Original class property identifiers differ");
    __android_log_print(ANDROID_LOG_INFO,"DH2Native","Class tables ready | classes %zu | bytes %zu | cached base snapshots",class_table.rows.size(),class_table.data_consumed);
    __android_log_print(ANDROID_LOG_INFO,"DH2Native","AI tables ready | configs %zu | factions %zu | automatic melee %d | pursuit and full FSM pending",ai_tables.rows.size(),ai_tables.factions.size(),enemy_ai_enabled);
    __android_log_print(ANDROID_LOG_INFO,"DH2Native","Property rules ready | defaults row 0 | types row 1 | properties 224 | supplied sheets only");
    // Development probe of the packaged native result calculator. Its copied
    // RNG and supplied unarmed sheets never execute hits or mutate actors.
    const auto defender_row=std::find(character_table.names.begin(),character_table.names.end(),"Crypt_Skeleton");
    if(defender_row==character_table.names.end())throw std::runtime_error("Combat probe defender missing");
    dh2::data::PropertyState probe_defender;dh2::data::reset_properties(property_rules,probe_defender,&character_table.rows.at(defender_row-character_table.names.begin()));
    dh2::data::SpawnVitals probe_vitals;
    if(!dh2::data::recalc_properties_with_class(class_table,property_rules,probe_defender,error)||!dh2::data::initialize_spawn_vitals(property_rules,probe_defender,probe_vitals,error))throw std::runtime_error(error);
    const auto animation_data=read(assets,"animations_pyarray.bin","data"),animation_names=read(assets,"animations_pyarraynames.bin","data"),animation_fields=read(assets,"animations_pystructnames.bin","data"),clip_names=read(assets,"animations_dictionary_pyarraynames.bin","data"),clip_values=read(assets,"animations_dictionary_pyarray.bin","data");
    dh2::data::Dictionary clip_table;dh2::data::AnimationTables animation_tables;dh2::data::AnimationRandom animation_random;
    if(!dh2::data::load_dictionary({clip_names.data(),clip_names.size()},{clip_values.data(),clip_values.size()},clip_table,error)||!dh2::data::load_animation_tables({animation_data.data(),animation_data.size()},{animation_names.data(),animation_names.size()},{animation_fields.data(),animation_fields.size()},clip_table,animation_tables,error))throw std::runtime_error(error);
    __android_log_print(ANDROID_LOG_INFO,"DH2Native","Animation tables ready | sequences %zu | characters %zu | clip paths %zu | bytes %zu",animation_tables.sequences.size(),animation_tables.characters.size(),clip_table.values.size(),animation_tables.data_consumed);
    const auto objects=read(assets,"crypt01.dact","worlds");std::vector<dh2::objects::Record> object_records;
    if(!dh2::objects::load_records(objects.data(),objects.size(),candidate.rooms,character_table,model_table,object_records,error))throw std::runtime_error(error);
    load_actor_initialization(assets,*script_context,objects,object_records);
    auto prince=read(assets,"prince_modular.bdae","models"),idle=read(assets,"prince_idle_shield.bdae","animations"),walk=read(assets,"prince_walk_1hand.bdae","animations");
    dh2::resources::BresView actor_view{};dh2_bres_open(&actor_view,prince.data(),prince.size());dh2::scene::Scene rest;
    if(!dh2::scene::load(actor_view,rest,error))throw std::runtime_error(error);
    dh2::animation::Player candidate_idle,candidate_walk;
    if(!candidate_idle.load(idle.data(),idle.size(),rest,error)||!candidate_walk.load(walk.data(),walk.size(),rest,error))throw std::runtime_error(error);
    const auto knight=std::find(character_table.names.begin(),character_table.names.end(),"KnightPlayerBase");if(knight==character_table.names.end())throw std::runtime_error("Original default player preset missing");
    PlayerCombat fresh_player;dh2::data::reset_properties(property_rules,fresh_player.properties(),&character_table.rows.at(knight-character_table.names.begin()));dh2::data::SpawnVitals player_vitals;
    fresh_player.aggro.initialize(object_records.size()+1);
    if(!dh2::data::recalc_properties_with_class(class_table,property_rules,fresh_player.properties(),error)||!dh2::data::initialize_spawn_vitals(property_rules,fresh_player.properties(),player_vitals,error))throw std::runtime_error(error);
    fresh_player.animation_table=fresh_player.properties().resolved[2];
    const auto bank_bytes=read(assets,"prince-animation-bank.bin","data");
    dh2::data::AnimationBank candidate_bank;
    if(!dh2::data::load_animation_bank({bank_bytes.data(),bank_bytes.size()},candidate_bank,error))throw std::runtime_error("Prince metadata rejected: "+error);
    if(candidate_bank.character!="KnightPlayerBase"||int(candidate_bank.animation_table)!=fresh_player.animation_table||candidate_bank.template_clip_id!=1111)throw std::runtime_error("Prince metadata producer does not match player properties");
    std::map<int,dh2::animation::Player> candidate_player_clips;
    for(const auto& resource:candidate_bank.resources){
     dh2::data::AnimationStep ref;ref.anim=resource.clip_id;
     const auto* authored=dh2::data::animation_clip(ref,clip_table);
     if(!authored||*authored!=resource.authored_path||resource.asset.rfind("animations/",0)!=0)throw std::runtime_error("Prince bank dictionary path mismatch");
     const auto raw_clip=read(assets,resource.asset.substr(11),"animations");
     if(raw_clip.size()!=resource.bytes)throw std::runtime_error("Prince bank resource size mismatch");
     auto& playback=candidate_player_clips[resource.clip_id];
     if(!playback.load(raw_clip.data(),raw_clip.size(),rest,error,dh2::animation::MissingTargets::ignore))throw std::runtime_error("Prince bank resource rejected: "+error);
     __android_log_print(ANDROID_LOG_INFO,"DH2Native",resource.clip_id==1023?"Player death track ready | clip %d | tracks %u | unbound %u | attack_mainhand ms %d":"Player attack track ready | clip %d | tracks %u | unbound %u | attack_mainhand ms %d",resource.clip_id,playback.track_count(),playback.unbound,dh2_events_time(&playback.events.view(),"attack_mainhand"));
    }
    if(!program)create_program();std::map<std::string,GLuint> cache;unsigned triangles=0;
    for(const auto& object:object_records){
      int animation_table=-1;if(object.kind==1){const auto* id=dh2::data::property(character_table,object.character,"AnimTable");if(!id)throw std::runtime_error("Monster animation table property missing");animation_table=*id;}
      auto found=std::find_if(candidate_groups.begin(),candidate_groups.end(),[&](const ObjectGroup& group){return group.instances.front().model==object.model&&group.animation_table==animation_table;});
      auto start_actor=[&](ObjectGroup& group){
        group.instances.emplace_back(object);auto& actor=group.instances.back();
        if(object.kind!=1)return;
        actor.identity=0x100000002ull+(&object-object_records.data());actor.aggro.initialize(object_records.size()+1);
        const auto character=std::find(character_table.names.begin(),character_table.names.end(),object.character);const auto* class_id=dh2::data::property(character_table,object.character,"ClassID");
        if(character==character_table.names.end()||!class_id)throw std::runtime_error("Original monster class link absent");
        actor.class_id=*class_id;actor.base_class=character_table.rows.at(character-character_table.names.begin());
        if(!dh2::data::apply_class(class_table,actor.class_id,actor.base_class,error))throw std::runtime_error(error);
        __android_log_print(ANDROID_LOG_INFO,"DH2Native","Base class snapshot | %s | character %s | class %d | level raw %d | Max_HP raw %d | Max_MP raw %d | checksum %016llx | cached only",actor.name.c_str(),actor.character.c_str(),actor.class_id,actor.base_class[19],actor.base_class[38],actor.base_class[43],static_cast<unsigned long long>(snapshot_checksum(actor.base_class)));
        dh2::data::reset_properties(property_rules,actor.properties(),&character_table.rows.at(character-character_table.names.begin()));
        if(!dh2::data::recalc_properties_with_class(class_table,property_rules,actor.properties(),error))throw std::runtime_error(error);
        const auto& resolved=actor.properties().resolved;
        __android_log_print(ANDROID_LOG_INFO,"DH2Native","Resolved properties | %s | character %s | HP raw %d | Max_HP raw %d | MP raw %d | Max_MP raw %d | checksum %016llx | supplied sheets only",actor.name.c_str(),actor.character.c_str(),resolved[36],resolved[38],resolved[41],resolved[43],static_cast<unsigned long long>(snapshot_checksum(resolved)));
        dh2::data::CombatantView probe_attacker{resolved.data(),-1,-1,0,0,0,5,0},probe_target{probe_defender.resolved.data(),-1,-1,0,0,0,5,0};
        dh2::data::CombatRandom probe_random{0xD22026u+unsigned(character-character_table.names.begin())*4,0};
        dh2::data::CombatResult probe_result;
        if(dh2_combat_melee(&probe_result,&probe_attacker,&probe_target,&probe_random,0,0)!=0)throw std::runtime_error("Combat result probe failed");
        __android_log_print(ANDROID_LOG_INFO,"DH2Native","Combat result probe | %s | character %s | amount %d | dot element %d | dot duration %d | dot amount %d | HP leech %d | MP leech %d | outcomes %u | mask %u | category %d | element %d | seed %u | calls %u | validation only",actor.name.c_str(),actor.character.c_str(),probe_result.amount,probe_result.dot_element,probe_result.dot_duration,probe_result.dot_amount,probe_result.hp_leech,probe_result.mp_leech,probe_result.outcomes,probe_result.mask,probe_result.weapon_category,probe_result.element,probe_random.seed,probe_random.calls);
        // Run nonlethal/lethal health writes on independent copies. Kill and
        // lifecycle requests are recorded; live actor HP/state is untouched.
        for(const unsigned damage:{256u,16384u}){
          auto copy=actor.properties();auto health_view=dh2::data::property_view(property_rules,copy);
          const dh2::data::HealthRequest request{&health_view,damage,dh2::data::health_game_present|dh2::data::health_main_player_present,0,1};dh2::data::HealthChange change;
          if(dh2_health_hit(&change,&request)!=0)throw std::runtime_error("Health probe failed");
          __android_log_print(ANDROID_LOG_INFO,"DH2Native","Health probe | %s | character %s | damage %u | add %d | before %d | after %d | kill %u | armed %u | cue %u | lifecycle %d | dead skip %u | validation only",actor.name.c_str(),actor.character.c_str(),damage,change.raw_add,change.before,change.after,change.kill_requested,change.low_health_armed,change.low_health_cue,change.lifecycle_write,change.skipped_dead);
        }
        const auto* state=dh2::data::animation_state(animation_tables,animation_table,"Idle");if(!state)throw std::runtime_error("Monster original idle state missing");
        const int sequence=state-animation_tables.sequences.data();if(!actor.scheduler.start(animation_tables,sequence,animation_random,error))throw std::runtime_error(error);
        const auto* path=dh2::data::animation_clip(actor.scheduler.clip(),clip_table);if(!path)throw std::runtime_error("Monster original idle clip path missing");
        const auto separator=path->find_last_of("/\\");const auto filename=path->substr(separator==std::string::npos?0:separator+1);
        __android_log_print(ANDROID_LOG_INFO,"DH2Native","Original idle selected | %s | table %d | sequence %d | step %u | clip %d | %s | speed %.4f | actor %s",object.model.c_str(),animation_table,sequence,actor.scheduler.frames().back().step,actor.scheduler.clip().anim,filename.c_str(),actor.scheduler.clip().speed,actor.name.c_str());
      };
      if(found!=candidate_groups.end()){start_actor(*found);continue;}
      candidate_groups.emplace_back();auto& group=candidate_groups.back();group.animation_table=animation_table;start_actor(group);
      auto model=read(assets,object.model,"actors");std::vector<std::uint8_t> clip;
      if(object.kind==1){
        const auto* path=dh2::data::animation_clip(group.instances.front().scheduler.clip(),clip_table);const auto separator=path->find_last_of("/\\");clip=read(assets,path->substr(separator==std::string::npos?0:separator+1),"actors");
      }
      if(!dh2::objects::load_resource(model.data(),model.size(),clip.empty()?nullptr:clip.data(),clip.size(),group.resource,error))throw std::runtime_error(object.model+": "+error);
      if(object.kind==1){
        std::set<int> ids;std::function<void(int,unsigned)> collect=[&](int id,unsigned depth){
          if(depth>=3||id<0||unsigned(id)>=animation_tables.sequences.size())throw std::runtime_error("Actor clip bank redirect outside limit");
          for(const auto& step:animation_tables.sequences[id].steps){if(step.redir==1)collect(step.anim,depth+1);else if(step.anim>=0)ids.insert(step.anim);else throw std::runtime_error("Actor clip bank contains an empty clip");}
        };
        for(const auto* state_name:{"Idle","Walk","Attack","Died"}){const auto* state=dh2::data::animation_state(animation_tables,animation_table,state_name);if(!state)throw std::runtime_error("Actor clip bank state missing");collect(state-animation_tables.sequences.data(),0);}
        for(int id:ids){dh2::data::AnimationStep ref;ref.anim=id;const auto* path=dh2::data::animation_clip(ref,clip_table);if(!path)throw std::runtime_error("Actor clip bank path missing");const auto separator=path->find_last_of("/\\");auto raw_clip=read(assets,path->substr(separator==std::string::npos?0:separator+1),"actors");auto& playback=group.clips[id];
          if(!playback.load(raw_clip.data(),raw_clip.size(),group.resource.rest_scene,error,dh2::animation::MissingTargets::ignore)||playback.end<=playback.start)throw std::runtime_error("Actor clip bank: "+error);
          __android_log_print(ANDROID_LOG_INFO,"DH2Native","Actor event track ready | %s | clip %d | groups %u | attack_mainhand ms %d",object.model.c_str(),id,playback.events.view().count,dh2_events_time(&playback.events.view(),"attack_mainhand"));
        }
        __android_log_print(ANDROID_LOG_INFO,"DH2Native","Actor clip bank ready | %s | clips %zu | independent instance clocks",object.model.c_str(),group.clips.size());
      }
      for(const auto& primitive:group.resource.primitives){Draw d;d.node=primitive.node;d.material=group.resource.scene.materials.at(primitive.material);group.draws.push_back(std::move(d));auto& batch=group.draws.back();
        if(primitive.skin.nodes.empty())for(const auto& vertex:primitive.vertices)batch.clip_bounds.include(vertex.p);
        batch.diffuse=upload(assets,batch.material.diffuse,cache,textures);batch.alpha=upload(assets,batch.material.alpha_map,cache,textures);batch.count=primitive.indices.size();
        glGenBuffers(1,&batch.vertices);glBindBuffer(GL_ARRAY_BUFFER,batch.vertices);glBufferData(GL_ARRAY_BUFFER,primitive.vertices.size()*sizeof(Vertex),primitive.vertices.data(),primitive.skin.nodes.empty()?GL_STATIC_DRAW:GL_DYNAMIC_DRAW);
        glGenBuffers(1,&batch.indices);glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,batch.indices);glBufferData(GL_ELEMENT_ARRAY_BUFFER,primitive.indices.size()*2,primitive.indices.data(),GL_STATIC_DRAW);check("Object buffer upload");
      }
      __android_log_print(ANDROID_LOG_INFO,"DH2Native","Object resource %s | primitives %zu | tracks %u | unbound %u | unsupported %u | removed helpers %u",object.model.c_str(),group.draws.size(),group.resource.animation.track_count(),group.resource.animation.unbound,group.resource.animation.skipped,group.resource.removed_helpers);
    }
    for(const auto& instance:candidate.scene.instances){dh2::assets::Mesh mesh{};
      if(dh2_mesh_open(&mesh,&view,instance.geometry)!=dh2::assets::Error::ok||mesh.primitives!=instance.materials.size())throw std::runtime_error("World mesh/binding rejected");
      for(unsigned j=0;j<mesh.primitives;++j){dh2::assets::Primitive p{};dh2_mesh_primitive(&mesh,j,&p);
        if(p.collada_type||p.index_count%3||mesh.vertices>65536)throw std::runtime_error("World topology rejected");
        Draw batch;batch.environment=true;batch.placement=instance.world;batch.material=candidate.scene.materials.at(instance.materials[j]);
        if(batch.material.id!=p.material)throw std::runtime_error("World visual binding differs");
        environment.push_back(std::move(batch));auto& d=environment.back();d.diffuse=upload(assets,d.material.diffuse,cache,textures);d.alpha=upload(assets,d.material.alpha_map,cache,textures);
        dh2::assets::Attribute position_attribute{},uv{},color_attribute{};
        if(dh2_mesh_attribute(&mesh,p.attributes[0],&position_attribute)!=dh2::assets::Error::ok||position_attribute.components<3)throw std::runtime_error("World position missing");
        const bool have_uv=dh2_mesh_attribute(&mesh,p.attributes[4],&uv)==dh2::assets::Error::ok&&uv.components>=2;
        const bool have_color=dh2_mesh_attribute(&mesh,p.attributes[2],&color_attribute)==dh2::assets::Error::ok;
        std::vector<Vertex> vertices(mesh.vertices);for(unsigned k=0;k<mesh.vertices;++k){float v[4]{};dh2_attribute_read(&position_attribute,k,v);std::copy(v,v+3,vertices[k].p);
          if(have_uv){dh2_attribute_read(&uv,k,v);std::copy(v,v+2,vertices[k].uv);}std::fill(vertices[k].color,vertices[k].color+4,1);
          if(have_color){dh2_attribute_read(&color_attribute,k,v);for(unsigned c=0;c<color_attribute.components;++c)vertices[k].color[c]=v[c]/(color_attribute.type==1?255.f:1.f);}}
        for(const auto& vertex:vertices)d.clip_bounds.include(vertex.p);
        std::vector<std::uint16_t> indices(p.index_count);for(unsigned k=0;k<p.index_count;++k){unsigned index;dh2_index_read(&p,k,&index);indices[k]=index;}
        d.count=p.index_count;triangles+=p.index_count/3;glGenBuffers(1,&d.vertices);glBindBuffer(GL_ARRAY_BUFFER,d.vertices);glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(Vertex),vertices.data(),GL_STATIC_DRAW);
        glGenBuffers(1,&d.indices);glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,d.indices);glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size()*2,indices.data(),GL_STATIC_DRAW);check("World buffer upload");
      }
    }
    const auto actor_report=load(prince.data(),prince.size(),assets);
    if(actor_report.find("Model load failed:")==0)throw std::runtime_error(actor_report);
    environment.insert(environment.end(),std::make_move_iterator(draws.begin()),std::make_move_iterator(draws.end()));draws=std::move(environment);
    images.insert(images.end(),textures.begin(),textures.end());textures.clear();
    object_groups=std::move(candidate_groups);world_objects=std::move(object_records);unsigned monsters=0,decors=0,object_triangles=0,object_draws=0;
    actor_animation_tables=std::move(animation_tables);actor_clip_table=std::move(clip_table);actor_random=restore?previous_random:animation_random;actor_property_rules=property_rules;actor_ai_tables=ai_tables;
    bool combat_resumed=false;
    if(restore){for(auto& group:object_groups)for(auto& actor:group.instances)if(actor.kind==1){
      auto saved=std::find_if(saved_actors.begin(),saved_actors.end(),[&](const ObjectActor& old){return old.room==actor.room&&old.name==actor.name&&old.model==actor.model;});
      if(saved!=saved_actors.end()){
        if(!group.clips.count(saved->scheduler.clip().anim))throw std::runtime_error("Restored actor clip is not bundled");
        actor=*saved;actor.pose_caches_v35.clear();combat_resumed|=actor.combat_target!=-1||actor.combat_state().dead;
        __android_log_print(ANDROID_LOG_INFO,"DH2Native","Combat actor restored | %s | HP %d | dead %u | combo %u | state %s | cursor %.4f | events %u | aggro %u %u | target %d | AI attack %d",actor.name.c_str(),actor.properties().resolved[36],actor.combat_state().dead,actor.combat_state().combo_hits,actor.state.c_str(),actor.cursor,actor.animation_events,actor.aggro.outgoing_table.count,actor.aggro.incoming_table.count,actor.combat_target,actor.ai_attack);
      }
    }}else{combat_random={0xD22026u,0};combat_hits=0;}
    saved_actors.clear();
    if(!restore)prince_combat=std::move(fresh_player);
    if(live_script_world&&live_script_world!=script_context){
     clear_npc_death_owners_v2();player_skills_runtime.reset();
     release_canonical_world_v4(live_script_world);
    }
    live_script_world=script_context;
    if(!script_context->player_canonical){
     script_context->player_canonical=PlayerCanonicalMetadataLiveV4::prepare(0,error);
     if(!script_context->player_canonical)throw std::runtime_error(error);
    }
    // The actual player uses the same retained identity domain as monsters.
    // This is scene lifetime ownership; it does not create original player AI.
    auto player_object=script_context->objects->find(0x100000001ull);
    if(!player_object)player_object=script_context->objects->add(0x100000001ull,
      script_context->player_canonical->name,prince_combat.property_owner,prince_combat.life_owner,
      restore?previous:candidate.spawn);
    if(player_object->properties!=prince_combat.property_owner||
       player_object->life!=prince_combat.life_owner||player_object->name!=script_context->player_canonical->name)
      throw std::runtime_error("Player scene identity backing differs");
    script_context->player_object=player_object;
    player_object->position=restore?previous:candidate.spawn;
    auto host_properties=dh2::data::property_view(property_rules,prince_combat.properties());
    if(dh2_character_host_context_sync_level(&script_context->host_player,&host_properties))throw std::runtime_error("Host cached level sync failed");
    __android_log_print(ANDROID_LOG_INFO,"DH2Native","Persistent game design ready | host cached level %d | level row %d | difficulty %d | authored levels %zu | development Crypt selection",script_context->host_player.cached_level,script_context->host_level.row_index,script_context->host_level.difficulty,script_context->ranges.size());
    __android_log_print(ANDROID_LOG_INFO,"DH2Native","Design settings ready | rows %zu | Default index %d | fields %zu | EnemySpottedAggro bits %08x | records consumed %zu | names consumed %zu | retained %u | backing %016llx | owned cache data",
      script_context->settings.rows().size(),script_context->settings.row_index("Default"),script_context->settings.fields().size(),
      *script_context->enemy_spotted_aggro,script_context->settings.records_consumed(),script_context->settings.names_consumed(),unsigned(restore),
      static_cast<unsigned long long>(reinterpret_cast<std::uintptr_t>(script_context->enemy_spotted_aggro)));
    const auto common_script=script_context->script_files.common();
    const auto* monster_script=script_context->script_files.find("data/scripts/ai/monster.luac");
    if(!common_script.data||!monster_script)throw std::runtime_error("Original monster script files missing from APK cache");
    // Register retained native Character lifetimes before any monster VM starts.
    // These records own the same gameplay backing and survive GL/vector changes.
    for(auto& group:object_groups)for(auto& actor:group.instances)if(actor.kind==1){
      auto object=script_context->objects->find(actor.identity);
      if(!object)object=script_context->objects->add(actor.identity,actor.name,actor.property_owner,actor.combat_owner,actor.position);
      if(object->properties!=actor.property_owner||object->life!=actor.combat_owner||object->name!=actor.name)throw std::runtime_error("Scene script identity backing differs");
      object->position=actor.position;
    }
    unsigned scripts_created=0,scripts_restored=0;
    for(auto& group:object_groups)for(auto& actor:group.instances)if(actor.kind==1){
      const bool retained=bool(actor.script);
      if(retained){
        if(actor.script->context!=script_context||actor.script->session->properties()!=actor.property_owner||actor.script->session->combat_state()!=actor.combat_owner||actor.script->object!=script_context->objects->find(actor.identity))throw std::runtime_error("Restored script backing differs");
        ++scripts_restored;
      }else{
        auto handle=construct_retained_monster_actor_v1(assets,script_context,actor,common_script,*monster_script,candidate.native_floor.get());
        actor.script=std::move(handle);++scripts_created;
        // Character's source refresh follows script initialization; restored
        // characters retain their current HP/MP and never repeat this refill.
        dh2::data::SpawnVitals spawn;
        if(!dh2::data::initialize_spawn_vitals(property_rules,actor.properties(),spawn,error))throw std::runtime_error(error);
        const auto& resolved=actor.properties().resolved;
        __android_log_print(ANDROID_LOG_INFO,"DH2Native","Spawn vitals | %s | character %s | HP raw %d | Max_HP raw %d | MP raw %d | Max_MP raw %d | first HP add raw %d | first MP add raw %d | second HP add raw %d | second MP add raw %d | checksum %016llx | passes 2",actor.name.c_str(),actor.character.c_str(),resolved[36],resolved[38],resolved[41],resolved[43],spawn.first_hp.raw_add,spawn.first_mp.raw_add,spawn.second_hp.raw_add,spawn.second_mp.raw_add,static_cast<unsigned long long>(snapshot_checksum(resolved)));
      }
      auto& session=*actor.script->session;session.set_position(actor.position);
      initialize_npc_visual(actor);
      initialize_npc_owner_bounds_v1(actor,assets);
      const auto* authored=dh2::character::actor_initialization(script_context->initialization,actor.room,actor.name);
      if(!authored||!actor.script->animation||authored->character!=actor.character)throw std::runtime_error("Monster retained native backing missing");
      const auto& native_bank=actor.script->animation->resources().metadata();
      __android_log_print(ANDROID_LOG_INFO,"DH2Native","Monster native backing | %s | character %s | resources %zu | occurrences %zu | template %d | CPU instance %016llx | retained %u | preset %d | authored state %u | CPU playback prepared | live FSM pending",
       actor.name.c_str(),actor.character.c_str(),native_bank.resources.size(),native_bank.registration_requests.size(),native_bank.template_clip_id,
       static_cast<unsigned long long>(reinterpret_cast<std::uintptr_t>(actor.script->animation.get())),unsigned(retained),authored->resolved.preset_state,unsigned(authored->authored[dh2::character::actor_ai_state].present));
      dh2::character::ScriptSessionView view{};
      if(!session.owner().active(view)||view.kind!=dh2::character::script_external||view.loaded_files!=2||session.owner().lifecycle().load_step!=7)throw std::runtime_error("Monster script publication incomplete");
      dh2_script_value script_identity{},has_target{};std::uint32_t returned=0;
      if(dh2_script_vm_call(view.vm,"GetID",nullptr,0,&script_identity,1,&returned)||returned!=1||script_identity.type!=DH2_SCRIPT_IDENTITY||script_identity.identity!=actor.identity)throw std::runtime_error("Live monster GetID differs from scene identity");
      if(dh2_script_vm_call(view.vm,"HasTarget",nullptr,0,&has_target,1,&returned)||returned!=1||has_target.type!=DH2_SCRIPT_BOOLEAN||bool(has_target.boolean)!=bool(actor.script->object->target.target))throw std::runtime_error("Live monster target backing differs");
      __android_log_print(ANDROID_LOG_INFO,"DH2Native","Scene script object | %s | identity %016llx | CharAI %016llx | retained %u | target %016llx | source GetID and HasTarget verified",actor.name.c_str(),static_cast<unsigned long long>(actor.identity),static_cast<unsigned long long>(actor.script->object->target.identity),unsigned(retained),static_cast<unsigned long long>(actor.script->object->target.target));
      dh2_script_value flee{},buff{},saved_x{},saved_y{};
      if(dh2_script_vm_get_global(view.vm,"m_flee_flag",&flee)||dh2_script_vm_get_global(view.vm,"BUFF_ID",&buff)||dh2_script_vm_get_global(view.vm,"saved_X",&saved_x)||dh2_script_vm_get_global(view.vm,"saved_Y",&saved_y)||flee.type!=DH2_SCRIPT_BOOLEAN||(!retained&&!flee.boolean)||buff.type!=DH2_SCRIPT_NUMBER||saved_x.type!=DH2_SCRIPT_NUMBER||saved_y.type!=DH2_SCRIPT_NUMBER)throw std::runtime_error("Original monster Init globals unavailable");
      const auto& lifecycle=session.owner().lifecycle();const auto& timers=session.timers();
      if(lifecycle.timer33<0||lifecycle.timer34<0||std::uint32_t(lifecycle.timer33)>=timers.count||std::uint32_t(lifecycle.timer34)>=timers.count)throw std::runtime_error("Monster owner timers unavailable");
      const auto& ai_tick=timers.slots[lifecycle.timer33];const auto& dot_tick=timers.slots[lifecycle.timer34];
      __android_log_print(ANDROID_LOG_INFO,"DH2Native","Monster script session | %s | character %s | identity %016llx | session %016llx | retained %u | files %zu | step %d | flags %x | level raw %d | HP %d %d | MP %d %d | saved %.9g %.9g | flee %u | buff %.9g | timers %d %u %d %u | registrations %zu | unavailable %zu",actor.name.c_str(),actor.character.c_str(),static_cast<unsigned long long>(actor.identity),static_cast<unsigned long long>(view.identity),unsigned(retained),view.loaded_files,lifecycle.load_step,view.callback_flags,actor.properties().resolved[19],actor.properties().resolved[36],actor.properties().resolved[38],actor.properties().resolved[41],actor.properties().resolved[43],double(saved_x.number),double(saved_y.number),flee.boolean,double(buff.number),ai_tick.event,ai_tick.duration_ms,dot_tick.event,dot_tick.duration_ms,session.registrations().size(),session.missing_bindings().size());
    }
    __android_log_print(ANDROID_LOG_INFO,"DH2Native","Monster scripts ready | initialized %u | retained %u | live original Init | full AI frame pending",scripts_created,scripts_restored);
    // Detach before replacing the explicit immutable bank owner.
    prince_locomotion=dh2::actor::BlendedPlayback{};
    prince_attack_clips=std::move(candidate_player_clips);prince_animation_bank=std::move(candidate_bank);
    __android_log_print(ANDROID_LOG_INFO,"DH2Native","Player properties | KnightPlayerBase | HP %d | MP %d | checksum %016llx | attempts %u | attacking %d",prince_combat.properties().resolved[36],prince_combat.properties().resolved[41],static_cast<unsigned long long>(snapshot_checksum(prince_combat.properties().resolved)),prince_combat.attempts,int(prince_state.current==5));
    for(const auto& group:object_groups)for(const auto& object:group.instances){monsters+=object.kind==1;decors+=object.kind==2;object_triangles+=group.resource.triangles;object_draws+=group.draws.size();}
    suspend_npc_navigation_v1();
    level=std::move(candidate);player=std::move(candidate_idle);walk_player=std::move(candidate_walk);current_scene=std::move(rest);
    actor_position=restore?previous:level.spawn;
    world_mode=true;resume_world=false;walking=false;move_x=move_y=0;source_hud_heading=false;source_hud_heading_active=false;heading=restore?previous_heading:0;movement_steps=blocked_steps=0;
    // A restored floor graph must wait for its own camera submission; do not
    // interpret a new-world tap through the previous GL context's matrix.
    submitted_width=submitted_height=0;
    radius=350;yaw=-1.57f;pitch=.75f;zoom=1;object_epoch=epoch=last_frame=std::chrono::steady_clock::now();sampled_ms=0;frozen=false;
    bind_player_equipment(assets,prince,restore);
     construct_native_level_c1_v25(assets,selected_mlx_uri,script_context);
    initialize_native_actor(assets,restore);
    initialize_npc_navigation_v1();
    publish_canonical_world_v4(script_context);
     initialize_player_skills(restore);
     player_object->position=actor_position;
     initialize_gameplay_camera_v20(selected_mlx_uri);
    __android_log_print(ANDROID_LOG_INFO,"DH2Native","Player script object | identity %016llx | CharAI %016llx | retained %u | properties %016llx | life %016llx | HP %d | dead %u | checksum %016llx | position %.9g %.9g %.9g | shared gameplay backing verified",
      static_cast<unsigned long long>(player_object->identity),static_cast<unsigned long long>(player_object->target.identity),unsigned(restore),
      static_cast<unsigned long long>(reinterpret_cast<std::uintptr_t>(prince_combat.property_owner.get())),
      static_cast<unsigned long long>(reinterpret_cast<std::uintptr_t>(prince_combat.life_owner.get())),
      prince_combat.properties().resolved[36],prince_combat.life().dead,
      static_cast<unsigned long long>(snapshot_checksum(prince_combat.properties().resolved)),
      double(actor_position[0]),double(actor_position[1]),double(actor_position[2]));
    for(auto& group:object_groups)for(auto& actor:group.instances)if(actor.kind==1){
      dh2::character::ScriptSessionView view{};
      if(!actor.script->session->owner().active(view))throw std::runtime_error("Position probe session unavailable");
      const auto& point=actor.script->object->position;
      verify_script_object_position(view.vm,actor.identity,point);
      verify_script_object_position(view.vm,player_object->identity,player_object->position);
      __android_log_print(ANDROID_LOG_INFO,"DH2Native","Scene position methods | %s | identity %016llx | position %.9g %.9g %.9g | player identity %016llx | player position %.9g %.9g %.9g | retained %u | original object GetPosition verified",
        actor.name.c_str(),static_cast<unsigned long long>(actor.identity),double(point[0]),double(point[1]),double(point[2]),
        static_cast<unsigned long long>(player_object->identity),double(player_object->position[0]),double(player_object->position[1]),double(player_object->position[2]),unsigned(restore));
    }
    char report[256];std::snprintf(report,sizeof(report),"Crypt | %u rooms | %u monsters | %u scenery objects\n%u triangles. Drag the movement control to walk.",level.rooms,monsters,decors,triangles+586+object_triangles);
    __android_log_print(ANDROID_LOG_INFO,"DH2Native","Objects ready | monsters %u | decors %u | resources %zu | instance draws %u | triangles %u | character records %zu | model entries %zu | original monster Init",monsters,decors,object_groups.size(),object_draws,object_triangles,character_table.rows.size(),model_table.values.size());
    if(level.native_floor)__android_log_print(ANDROID_LOG_INFO,"DH2Native","Native floors ready | records %zu | graph nodes %u | graph edges %u | selector collision controls height",level.native_floor->records.size(),level.native_floor->graph.node_count,level.native_floor->graph.edge_count);
    if(level.native_floor&&level.native_floor->sewn)__android_log_print(ANDROID_LOG_INFO,"DH2Native","Native floor links ready | neighbour relations %u | validation references %u | graph-node search ready",level.native_floor->sewing.link_count,level.native_floor->graph.validation_count);
#if DH2_ENABLE_LOAD_DIAGNOSTICS
#include "renderer_load_diagnostics_v39.inc"
#endif
    __android_log_print(ANDROID_LOG_INFO,"DH2Native","World ready | rooms %u | visual draws %zu | navigation triangles %zu | idle tracks %u | walk tracks %u | position %.4f %.4f %.4f",level.rooms,draws.size(),level.floor.size(),player.track_count(),walk_player.track_count(),actor_position[0],actor_position[1],actor_position[2]);return std::string(report)+(combat_resumed?"\nNative combat resumed":"");
  }catch(const std::exception& e){native_actor_ready=false;actor_world.clear();prince_body={};enabled=false;world_mode=false;release_objects(candidate_groups);release(environment,textures);__android_log_print(ANDROID_LOG_ERROR,"DH2Native","World load failed: %s",e.what());return std::string("World load failed: ")+e.what();}
}
void draw(int width,int height){
  current_application_dt=0;current_application_tick=false;
  if(!enabled||!program)return;
  float frame_dt=0;
  if(world_mode){const auto now=std::chrono::steady_clock::now();
    const auto now_ms=std::uint32_t(std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count());
    const auto previous_ms=std::uint32_t(std::chrono::duration_cast<std::chrono::milliseconds>(last_frame.time_since_epoch()).count());
    const unsigned dt_ms=now_ms-previous_ms;last_frame=now;
    // The original Application skips updates after a real-time gap >2000ms.
    // Its normal dt is unsigned milliseconds, with one world Step and no cap.
    if(dt_ms<=2000){dh2::perf::Scope perf_actor(dh2::perf::Phase::actor);if(!native_actor_ready)throw std::runtime_error("Native actor runtime is not initialized");advance_native_actor(dt_ms);frame_dt=character_panel_open?0.f:float(dt_ms)*.001f;current_application_dt=dt_ms;current_application_tick=!frozen&&!character_panel_open;}
    const auto& target=inspected_object>=0?world_objects[inspected_object].position:actor_position;
    center[0]=target[0];center[1]=target[1];center[2]=target[2]+90;
  }
  const auto& playback=player;
  if(world_mode)sampled_ms=prince_locomotion.current_timeline().current_ms;
  if(playback.track_count()&&!animation_failed&&!world_mode){
    if(!frozen){const auto elapsed=std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()-epoch).count();sampled_ms=playback.start+elapsed%(playback.end-playback.start);}
    std::string error;if(!playback.sample(current_scene,sampled_ms,error)){animation_failed=true;__android_log_print(ANDROID_LOG_ERROR,"DH2Native","Animation sample failed: %s",error.c_str());}
    if(world_mode&&!prince_visual.update_world(current_scene,error))throw std::runtime_error(error);
  }
  const auto projection=class_scene?class_camera(width,height):menu_background?menu_camera():world_mode?submit_gameplay_camera_v20(width,height):camera(width,height);glUseProgram(program);
  submitted_camera=projection;submitted_width=width;submitted_height=height;
   {dh2::perf::Scope perf_fx(dh2::perf::Phase::fx_prepare);if(world_mode&&player_skills_runtime&&current_application_tick&&player_skills_runtime->combat_fx_frame(std::int32_t(scene_clock),current_application_dt))throw std::runtime_error("Source CombatFX frame failed: "+player_skills_runtime->error);}
  if(world_mode&&player_equipment&&player_equipment->ready()){
    dh2::perf::Scope perf_equipment(dh2::perf::Phase::equipment);
    const std::vector<dh2::skinning::VisualDrawViewV32>* parts{};std::string error;
    if(!player_equipment->draw_views(parts,error))throw std::runtime_error("Live equipment pose failed: "+error);
    sync_equipment_draws(*parts,equipment_platform->manager,equipment_platform->cache);
  }
  glUniform1i(diffuse_uniform_v35,0);glUniform1i(alpha_uniform_v35,1);
  glEnable(GL_DEPTH_TEST);glDepthFunc(GL_LEQUAL);glEnable(GL_BLEND);
  glEnableVertexAttribArray(position);glEnableVertexAttribArray(texcoord);glEnableVertexAttribArray(color);
  const Draw* previous_draw_v35=nullptr;
  glCullFace(GL_BACK);glFrontFace(GL_CCW);
  auto submit=[&](const Draw& b,const Matrix& transform){
    if(dh2::render::outside_clip_v35(b.clip_bounds,transform)){++dh2::perf::state.culled;return;}
    ++dh2::perf::state.draws;
    glUniformMatrix4fv(mvp,1,GL_FALSE,transform.data());
    const auto* old=previous_draw_v35;
    if(!old||old->material.backface!=b.material.backface)b.material.backface?glEnable(GL_CULL_FACE):glDisable(GL_CULL_FACE);
    if(!old||old->material.additive!=b.material.additive){glBlendFunc(GL_SRC_ALPHA,b.material.additive?GL_ONE:GL_ONE_MINUS_SRC_ALPHA);glDepthMask(b.material.additive?GL_FALSE:GL_TRUE);}
    if(!old||std::memcmp(old->material.texture_matrix,b.material.texture_matrix,sizeof(b.material.texture_matrix)))glUniformMatrix4fv(texture_matrix,1,GL_FALSE,b.material.texture_matrix);
    if(!old||std::memcmp(old->material.color,b.material.color,sizeof(b.material.color)))glUniform4fv(material_color,1,b.material.color);
    if(!old||old->material.alpha_map.empty()!=b.material.alpha_map.empty())glUniform1f(has_alpha,b.material.alpha_map.empty()?0:1);
    if(!old||old->material.alpha_ref!=b.material.alpha_ref)glUniform1f(alpha_ref,b.material.alpha_ref);
    const bool blue=b.material.effect_file=="GL_Diffuse_L1_VC_iPhone.bdae"&&
      b.material.gles2_technique=="L1_Vc_Al_----_----_----_----";
    const bool old_blue=old&&old->material.effect_file=="GL_Diffuse_L1_VC_iPhone.bdae"&&old->material.gles2_technique=="L1_Vc_Al_----_----_----_----";
    if(!old||old_blue!=blue)glUniform1f(original_alpha_blue,blue?1:0);
    if(!old||old->diffuse!=b.diffuse){glActiveTexture(GL_TEXTURE0);glBindTexture(GL_TEXTURE_2D,b.diffuse);}
    if(!old||old->alpha!=b.alpha){glActiveTexture(GL_TEXTURE1);glBindTexture(GL_TEXTURE_2D,b.alpha);}
    glBindBuffer(GL_ARRAY_BUFFER,b.vertices);glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,b.indices);
    glVertexAttribPointer(position,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),reinterpret_cast<void*>(offsetof(Vertex,p)));
    glVertexAttribPointer(texcoord,2,GL_FLOAT,GL_FALSE,sizeof(Vertex),reinterpret_cast<void*>(offsetof(Vertex,uv)));
    glVertexAttribPointer(color,4,GL_FLOAT,GL_FALSE,sizeof(Vertex),reinterpret_cast<void*>(offsetof(Vertex,color)));glDrawElements(GL_TRIANGLES,b.count,GL_UNSIGNED_SHORT,nullptr);
    previous_draw_v35=&b;
  };
  {dh2::perf::Scope perf_world(dh2::perf::Phase::world);for(auto& b:draws){
    if(world_mode&&player_equipment&&player_equipment->ready()&&!b.environment)continue;
    if(!b.skin.nodes.empty()){
      std::string error;std::vector<dh2::skinning::Matrix> matrices;std::vector<std::array<float,3>> deformed;
      if(!dh2::skinning::palette(b.skin,current_scene,matrices,error)||!dh2::skinning::positions(b.skin,matrices,b.rest_positions,deformed,error)){
        __android_log_print(ANDROID_LOG_ERROR,"DH2Native","Skin sample failed: %s",error.c_str());enabled=false;return;}
      for(unsigned k=0;k<b.cpu_vertices.size();++k)std::copy(deformed[k].begin(),deformed[k].end(),b.cpu_vertices[k].p);
      glBindBuffer(GL_ARRAY_BUFFER,b.vertices);glBufferSubData(GL_ARRAY_BUFFER,0,b.cpu_vertices.size()*sizeof(Vertex),b.cpu_vertices.data());
    }
    // Native actor joints already include owner * helper * authored graph.
    const auto transform=b.environment?dh2::scene::multiply(projection,b.placement):b.skin.nodes.empty()?dh2::scene::multiply(projection,current_scene.graph[b.node].world):projection;submit(b,transform);
  }}
#include "renderer_front_draw_v87.inc"
  previous_draw_v35=nullptr;
  for(const auto& b:equipment_draws)submit(b,dh2::scene::multiply(projection,b.placement));
  if(world_mode){dh2::perf::Scope perf_actors(dh2::perf::Phase::actors);for(auto& group:object_groups){
    std::string error;
    auto refresh_draw_bounds_v35=[&](){for(unsigned i=0;i<group.draws.size();++i){auto& batch=group.draws[i];const auto& primitive=group.resource.primitives[i];if(primitive.skin.nodes.empty())continue;batch.clip_bounds={};for(const auto& vertex:primitive.vertices)batch.clip_bounds.include(vertex.p);}};
    auto render_actor=[&](const ObjectActor& instance){
     if(instance.script){instance.script->rendered_bounds_ready=false;instance.script->rendered_screen_bounds_ready=false;}
     for(unsigned i=0;i<group.draws.size();++i){const auto& primitive=group.resource.primitives[i];const auto& batch=group.draws[i];
      if(instance.script){const auto pose=primitive.skin.nodes.empty()?dh2::scene::multiply(instance.placement,group.resource.scene.graph[primitive.node].world):instance.placement;
       for(const auto& vertex:primitive.vertices){float point[3]{};for(unsigned row=0;row<3;++row)point[row]=pose[row]*vertex.p[0]+pose[4+row]*vertex.p[1]+pose[8+row]*vertex.p[2]+pose[12+row];npc_include_bound(*instance.script,point);}
      }
      auto transform=dh2::scene::multiply(projection,instance.placement);if(primitive.skin.nodes.empty())transform=dh2::scene::multiply(transform,group.resource.scene.graph[primitive.node].world);
      if(!primitive.skin.nodes.empty()&&!dh2::render::outside_clip_v35(batch.clip_bounds,transform)){group.draws[i].pose_owner=nullptr;glBindBuffer(GL_ARRAY_BUFFER,batch.vertices);glBufferSubData(GL_ARRAY_BUFFER,0,primitive.vertices.size()*sizeof(Vertex),primitive.vertices.data());dh2::perf::state.uploads+=primitive.vertices.size()*sizeof(Vertex);}
      submit(batch,transform);
    }};
    if(group.animation_table<0){
      const auto& clip=group.resource.animation;int ms=clip.start;
      if(clip.track_count()){const auto elapsed=std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()-object_epoch).count();ms=frozen?std::clamp(sampled_ms,clip.start,clip.end):clip.start+elapsed%(clip.end-clip.start);}
      if(!dh2::objects::sample(group.resource,ms,error)){__android_log_print(ANDROID_LOG_ERROR,"DH2Native","Object sample failed: %s",error.c_str());enabled=false;return;}
      refresh_draw_bounds_v35();
      for(const auto& instance:group.instances)render_actor(instance);
    }else for(auto& actor:group.instances){
      if(actor.script){actor.script->object->position=actor.position;actor.script->session->set_position(actor.position);}
      update_enemy(actor,group.animation_table);
      if(actor.pending_death){
        auto* sequence=dh2::data::animation_state(actor_animation_tables,group.animation_table,"Died");
        if(!sequence||!actor.scheduler.start(actor_animation_tables,sequence-actor_animation_tables.sequences.data(),actor_random,error)){__android_log_print(ANDROID_LOG_ERROR,"DH2Native","Combat death animation failed");enabled=false;return;}
        actor.cursor=0;actor.completions=0;actor.state="Died";actor.event_cursor={};actor.animation_events=0;actor.pending_death=false;
       __android_log_print(ANDROID_LOG_INFO,"DH2Native","Combat death animation selected | %s | clip %d | dead %u",actor.name.c_str(),actor.scheduler.clip().anim,actor.combat_state().dead);
      }
      if(actor.script&&!actor.combat_state().dead&&(actor.state=="Idle"||actor.script->machine->state().current==11)){
       const auto& scene=actor.script->animation->scene();
       actor.script->rendered_bounds_ready=false;actor.script->rendered_screen_bounds_ready=false;
       for(unsigned i=0;i<group.draws.size();++i){const auto& primitive=group.resource.primitives[i];auto& batch=group.draws[i];
        if(scene.graph.size()!=group.resource.rest_scene.graph.size()||primitive.node>=scene.graph.size()||scene.graph[primitive.node].id!=group.resource.rest_scene.graph[primitive.node].id)
         throw std::runtime_error("NPC retained CPU graph differs from its immutable render resource");
        if(!primitive.skin.nodes.empty()){
         if(actor.pose_caches_v35.size()!=group.draws.size())actor.pose_caches_v35.resize(group.draws.size());
         auto& cache=actor.pose_caches_v35[i];if(!cache){cache=std::make_shared<dh2::skinning::SkinPoseCacheV32>();cache->bind(primitive.skin,primitive.rest_positions);}
         if(!cache->sample(scene,error))throw std::runtime_error("NPC retained CPU skin failed: "+error);
         const auto& positions=cache->positions();
         batch.clip_bounds={};for(const auto& point:positions)batch.clip_bounds.include(point.data());
         if(batch.cpu_vertices.size()!=primitive.vertices.size())batch.cpu_vertices=primitive.vertices;
         for(unsigned k=0;k<batch.cpu_vertices.size();++k)std::copy(positions[k].begin(),positions[k].end(),batch.cpu_vertices[k].p);
         for(const auto& point:positions)npc_include_bound(*actor.script,point.data());
         if(!dh2::render::outside_clip_v35(batch.clip_bounds,projection)&&(batch.pose_owner!=actor.script.get()||batch.uploaded_pose_revision!=cache->revision())){
          glBindBuffer(GL_ARRAY_BUFFER,batch.vertices);glBufferSubData(GL_ARRAY_BUFFER,0,batch.cpu_vertices.size()*sizeof(Vertex),batch.cpu_vertices.data());
          dh2::perf::state.uploads+=batch.cpu_vertices.size()*sizeof(Vertex);batch.pose_owner=actor.script.get();batch.uploaded_pose_revision=cache->revision();}
        }
        else{const auto& pose=scene.graph[primitive.node].world;for(const auto& vertex:primitive.vertices){float point[3]{};for(unsigned row=0;row<3;++row)point[row]=pose[row]*vertex.p[0]+pose[4+row]*vertex.p[1]+pose[8+row]*vertex.p[2]+pose[12+row];npc_include_bound(*actor.script,point);}}
        // Source joints already contain owner/root/helper transforms. Applying
        // DACT placement again would translate and scale this monster twice.
        submit(batch,primitive.skin.nodes.empty()?dh2::scene::multiply(projection,scene.graph[primitive.node].world):projection);
       }
       continue;
      }
      double remaining=frozen?0:frame_dt*1000;unsigned events=0;
       while(!frozen&&!character_panel_open&&actor.scheduler.active()){
        const auto& clip=group.clips.at(actor.scheduler.clip().anim);const double duration=clip.end-clip.start,speed=actor.scheduler.clip().speed,needed=(duration-actor.cursor)/speed;
        struct EventContext{ObjectActor* actor;int clip,ms;};
        auto dispatch_events=[&](double next){
          const int previous=clip.start+int(std::clamp(actor.cursor,0.,duration)),current=clip.start+int(std::clamp(next,0.,duration));EventContext context{&actor,actor.scheduler.clip().anim,current};
          return dh2_events_update(&clip.events.view(),&actor.event_cursor,previous,current,clip.start,clip.end,[](const dh2::animation::TriggeredEvent* event,void* raw){
            auto& context=*static_cast<EventContext*>(raw);auto& actor=*context.actor;++actor.animation_events;
            __android_log_print(ANDROID_LOG_INFO,"DH2Native","Actor animation event | %s | state %s | clip %d | name %s | lag ms %d | time ms %d | count %u",actor.name.c_str(),actor.state.c_str(),context.clip,event->name,event->lag_ms,context.ms,actor.animation_events);
            // These actors currently have no equipment. CanRangeAttack first
            // checks resolved projectile property 32, then queries inventory.
            // Outer sequence and inner clip steps are distinct original args.
            const auto& frames=actor.scheduler.frames();
            const auto projectile=actor.properties().resolved[32];
            const dh2::data::CombatEventContext combat{actor.state=="Attack"?5:-1,int(frames.front().step),int(frames.back().step),projectile!=-1,projectile};
            dh2::data::CombatEventAction action;
            if(dh2_combat_event_route(&action,&combat,event->name)!=0){__android_log_print(ANDROID_LOG_ERROR,"DH2Native","Actor combat route failed");return;}
            if(action.kind!=dh2::data::CombatEventKind::none)
              __android_log_print(ANDROID_LOG_INFO,"DH2Native","Actor combat action | %s | state %s | clip %d | name %s | kind %d | sequence %d | attack step %d | offhand %d | capability %u | projectile %d | %s",actor.name.c_str(),actor.state.c_str(),context.clip,event->name,int(action.kind),action.sequence_step,action.attack_step,action.offhand,combat.can_range,combat.projectile,actor.combat_target==-1?"execution pending":"native target request");
            apply_actor_attack(actor,action);
          },&context);
        };
        const double next=remaining<needed?actor.cursor+remaining*speed:duration;
        if(!dispatch_events(next)){__android_log_print(ANDROID_LOG_ERROR,"DH2Native","Actor event dispatch failed");enabled=false;return;}
        if(remaining<needed){actor.cursor=next;break;}
        remaining-=std::max(0.,needed);if(++events>64||!actor.scheduler.complete(actor_animation_tables,actor_random,error)){__android_log_print(ANDROID_LOG_ERROR,"DH2Native","Actor completion failed: %s",error.c_str());enabled=false;return;}
        actor.cursor=actor.scheduler.active()?0:duration;actor.event_cursor={};++actor.completions;
        __android_log_print(ANDROID_LOG_INFO,"DH2Native","Actor clip completed | %s | state %s | completions %u | active %d | next clip %d | layers %zu",actor.name.c_str(),actor.state.c_str(),actor.completions,actor.scheduler.active(),actor.scheduler.clip().anim,actor.scheduler.frames().size());
        if(remaining<=0)break;
      }
      const auto& clip=group.clips.at(actor.scheduler.clip().anim);const int ms=frozen?std::clamp(sampled_ms,clip.start,clip.end):clip.start+int(std::clamp(actor.cursor,0.,double(clip.end-clip.start)));
      if(!dh2::objects::sample(group.resource,clip,ms,error)){__android_log_print(ANDROID_LOG_ERROR,"DH2Native","Actor sample failed: %s",error.c_str());enabled=false;return;}
      refresh_draw_bounds_v35();
      render_actor(actor);
    }
  }}
  {dh2::perf::Scope perf_fx(dh2::perf::Phase::fx_draw);if(world_mode)draw_combat_fx_v4();}
  previous_draw_v35=nullptr;
   if(world_mode)for(const auto& record:loot_gpu_records_v27){
    const auto& parts=record.second.geometry.parts();
    for(std::size_t i=0;i<parts.size();++i)if(parts[i].visible)submit(record.second.draws[i],dh2::scene::multiply(projection,record.second.draws[i].placement));
   }
  glDisableVertexAttribArray(position);glDisableVertexAttribArray(texcoord);glDisableVertexAttribArray(color);
  glDepthMask(GL_TRUE);glDisable(GL_BLEND);glDisable(GL_CULL_FACE);glDisable(GL_DEPTH_TEST);glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);glBindBuffer(GL_ARRAY_BUFFER,0);
  const auto e=glGetError();if(e!=GL_NO_ERROR)__android_log_print(ANDROID_LOG_ERROR,"DH2Native","Model draw GL error 0x%04x",e);
  else if(player.track_count()&&frozen&&!animation_failed&&!frozen_cursor_logged){
    __android_log_print(ANDROID_LOG_INFO,"DH2Native","Animation frame rendered at %d ms",sampled_ms);frozen_cursor_logged=true;
  }
}
}
