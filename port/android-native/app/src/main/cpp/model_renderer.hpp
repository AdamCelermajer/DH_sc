#pragma once
namespace dh2::loader {struct AreaTransitionRequestV114;}
namespace dh2::loader {struct LevelDestroyServicesV1;}
#include "renderer_loot_gpu_v27.hpp"
#include "renderer_native_gslevel_v27.hpp"
#include "actual_device_android_v54.hpp"
#include "front_selected_profile_v50.hpp"
#include "application_services_owner_v5.hpp"
#include "audio_application_manager_v42.hpp"
#include "audio_campaign_bridge_v46.hpp"
#include "audio_level_gameplay_v67.hpp"
#include "campaign_audio_retirement_v102.hpp"
#include <functional>
#include <android/asset_manager.h>
#include <cstddef>
#include <cstdint>
#include <string>
#include <array>
#include <vector>
#include "enemy_status_hud_v1.hpp"
#include "character_combat_text_v1.hpp"
#include "swf_menu_device_v1.hpp"
#include "character_menu_font_palette_v1.hpp"
#include "hud_attack_control_v46.hpp"
namespace dh2::android_ui {struct CharacterPanelMovieRuntimeV3;struct CharacterPanelGameplayServicesV2;struct FrontSelectedProfileV50;}
namespace dh2::ui {class HudTextV1;class AuthoredCharacterApplicationV1;class MenuStatusMessagesV26;class OwnedHudSettingsV1;struct HudTextEnvironmentV1;}
namespace model_renderer {struct PlayerGameplayBinding;struct SourceCampaignCandidateBorrowV55;}
namespace model_renderer {struct SourceDeathRewardsLeavesV84;}
namespace dh2::world {class CanonicalObjectManagerV1;class CanonicalPropertyMapV1;struct CanonicalFactoryEntryV1;struct CanonicalSourceObjectRequestV1;struct CanonicalClassReceiverV1;}
namespace dh2::world {class CanonicalGameObjectGraphV68;class RetainedGameObjectVisualV1;}
namespace dh2::world {class ModulePFRoomsV3;class SceneManagerMapOwnerV2;}
namespace dh2::loader {class CanonicalReceiverTransportV1;class ScriptManagerOwnerV52;}
namespace dh2::floors {struct World;}
namespace dh2::physical {class NativeWorld;}
namespace dh2::world {class GameObjectSceneRootRegistryV1;}
namespace dh2::fx {class VisualFxManagerLibrariesV63;}
namespace dh2::navigation {class CampaignNavigationRegistryV64;}
namespace dh2::camera {class GameplayCameraApplicationV23;}
namespace dh2::loader {struct Stage0ServicesV46;}
namespace dh2::loader {struct CheckedCommandBorrowV59;}
namespace dh2::loader {struct ModuleDrawFrameV1;}
namespace dh2::loader {struct LevelGameplayServicesV66;}
namespace dh2::loader {struct LevelPlayerPlacementServicesV68;}
namespace dh2::character {struct TargetEventState32;struct TargetEventServices40;}
namespace dh2::input {class SourceInputManagerV60;}
namespace dh2::player {struct PlayerManagerPostInitServicesV66;}
namespace dh2::world {class CanonicalOpenableContainerV1;class OpenableContainerTableV1;class CanonicalDestructibleContainerV16;class DestructibleContainerTableV16;}
namespace model_renderer {
bool borrow_actual_gameplay_presentation_v67(const std::shared_ptr<void>& actual_world,
 std::shared_ptr<dh2::ui::OwnedHudSettingsV1>& actual_settings,
 dh2::ui::HudTextEnvironmentV1&,std::shared_ptr<void>& actual_text_owner,std::string&);
bool source_character_table_index_v67(const std::shared_ptr<void>& actual_world,const char*,std::int32_t&,std::string&);
bool source_character_saved_name_v67(const std::shared_ptr<void>& actual_world,std::uintptr_t,std::string&,std::string&);
bool activate_source_campaign_scene_v67(AAssetManager*,std::string&);
bool source_campaign_scene_active_v67();
bool borrow_actual_application_audio_v42(dh2::audio::AudioApplicationBorrowV42&,std::string&);
bool initialize_process_audio_v100(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>&,
 dh2::audio::AudioInitializeServicesV100,std::string&);
bool set_campaign_music_state_v101(const std::shared_ptr<void>&actual_world,const char*,std::string&);
bool play_campaign_music_v101(const std::shared_ptr<void>&actual_world,int,bool,bool,int,std::string&);
bool set_campaign_safe_zone_music_v101(const std::shared_ptr<void>&actual_world,bool,std::string&);
bool bind_campaign_music_aggro_v101(const std::shared_ptr<void>&actual_world,SourceDeathRewardsLeavesV84&,std::string&);
bool stop_application_sounds_v101(const dh2::audio::AudioApplicationBorrowV42&captured,int fade_ms,std::string&);
bool stop_campaign_sound_v106(const std::shared_ptr<void>&actual_world,int sound,int fade_ms,std::string&);
bool stop_campaign_sound_3d_v112(const std::shared_ptr<void>&actual_world,int sound,int fade_ms,const float* center,float radius,std::string&);
 bool campaign_sound_index_v115(const std::shared_ptr<void>&actual_world,const char* name,std::int32_t&,std::string&);
 bool play_campaign_plain_sound_v115(const std::shared_ptr<void>&actual_world,std::int32_t,bool,std::int32_t,std::int32_t,bool,std::string&);
 bool stop_campaign_music_v117(const std::shared_ptr<void>&actual_world,std::int32_t fade_ms,std::string&);
bool take_application_audio_receipt_v101(const dh2::audio::AudioApplicationBorrowV42&,dh2::audio::AudioReceiptV34&,bool&found,std::string&);
bool application_audio_retirement_pending_v101(const dh2::audio::AudioApplicationBorrowV42&,bool&,std::string&);
bool release_campaign_audio_providers_v101(const std::shared_ptr<void>&actual_world,std::string&);
bool borrow_campaign_vox_services_v101(const std::shared_ptr<void>&actual_world,
 dh2::sound::VoxPlay3DServicesV2 existing_prefix,dh2::audio::AudioApplicationBorrowV42&captured,
 dh2::sound::VoxPlay3DServicesV2&out,std::shared_ptr<void>&provider,std::string&);
bool campaign_vox_diagnostic_v101(const std::shared_ptr<void>&exact_borrowed_provider,std::string&);
bool publish_campaign_audio_v46(dh2::audio::AudioCampaignServicesV46,std::string&);
bool compose_campaign_level_audio_v67(dh2::audio::AudioCampaignServicesV46,
  dh2::audio::AudioLevelBindingsV67,dh2::loader::LevelGameplayServicesV66&,
  std::shared_ptr<dh2::audio::AudioLevelGameplayV67>&,std::string&);
bool borrow_campaign_audio_services_v68(const SourceCampaignCandidateBorrowV55&,
 dh2::audio::AudioCampaignServicesV46&,dh2::audio::AudioLevelBindingsV67&,std::string&);
bool borrow_actual_audio_settings_v68(const std::shared_ptr<void>& actual_world,
 std::shared_ptr<dh2::ui::OwnedHudSettingsV1>&,std::string&);
bool submit_campaign_audio_v46(dh2::audio::AudioCategoryV46,const dh2::audio::AudioApplicationBorrowV42&,const std::shared_ptr<void>&actual_world,const dh2::character::CombatSoundPlayV1&,std::string&);
std::function<bool(int,std::string&)> bind_positioned_audio_v46(dh2::audio::AudioCategoryV46,std::shared_ptr<void>actual_world,std::uintptr_t actual_subject,std::function<bool(std::array<float,3>&,std::string&)>actual_position);
std::function<bool(std::string&)> bind_container_play_v46(std::shared_ptr<void>actual_world,std::uintptr_t actual_subject,std::function<bool(int&,std::string&)>actual_get_sound,std::function<bool(std::array<float,3>&,std::string&)>actual_position);
int dispatch_target_audio_v46(dh2::character::TargetEventState32*,unsigned,dh2::character::TargetEventServices40,std::shared_ptr<void>actual_world,std::string&);
bool bind_root_stage0_sound_v44(dh2::loader::Stage0ServicesV46&,std::string&);
std::function<bool(std::string&)> bind_container_precache_v45(std::function<bool(int&,std::string&)>);
std::function<bool(std::string&)> bind_openable_precache_v45(const std::shared_ptr<dh2::world::CanonicalOpenableContainerV1>&,std::shared_ptr<const dh2::world::OpenableContainerTableV1>);
std::function<bool(std::string&)> bind_destructible_precache_v45(const std::shared_ptr<dh2::world::CanonicalDestructibleContainerV16>&,std::shared_ptr<const dh2::world::DestructibleContainerTableV16>);
std::function<bool(std::string&)> bind_openable_precache_slot_v45(std::shared_ptr<std::weak_ptr<dh2::world::CanonicalOpenableContainerV1>>,std::shared_ptr<const dh2::world::OpenableContainerTableV1>);
std::function<bool(std::string&)> bind_destructible_precache_slot_v45(std::shared_ptr<std::weak_ptr<dh2::world::CanonicalDestructibleContainerV16>>,std::shared_ptr<const dh2::world::DestructibleContainerTableV16>);
// Sole retained native Application service receiver; required before camera
// Zoom/Overview attach to Application14. Level has a distinct event base.
bool borrow_actual_application_services_v5(std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>&,std::string&);
bool initialize_source_resource_prefix_v95(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>&,std::string&);
bool prepare_actual_process_player_manager_v119(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>&,std::string&);
bool borrow_source_resource_prefix_v95(std::string&,std::string&);
// Lazy source GetInstance construction; retained across Worlds/GL resets.
bool borrow_actual_input_manager_v60(std::shared_ptr<dh2::input::SourceInputManagerV60>&,std::string&);
// Source campaign adapter. Retains failed original prefixes; no Crypt fallback
// or whole NativeStartGame/Application.LoadLevel/save parity is asserted.
bool start_source_campaign_v55(AAssetManager*,std::shared_ptr<const dh2::android_ui::FrontSelectedProfileV50>,std::string&,
 const dh2::loader::AreaTransitionRequestV114* prepared_transition=nullptr);
bool source_campaign_active_v55();
bool borrow_native_surface_dimensions_v114(std::int32_t&,std::int32_t&,std::string&);
bool bind_native_character_reward_text_v114(const std::shared_ptr<void>&,std::string&);
bool bind_native_campaign_combat_presentation_v115(const std::shared_ptr<void>&,std::string&);
bool bind_native_character_interaction_ui_v115(const std::shared_ptr<void>&,std::string&);
// True only after actual source37->38 and its complete license/fade/onProgress
// tail. It does not activate renderer/player or certify Level.Update gameplay.
bool source_campaign_loading_complete_v64();
bool capture_source_campaign_modules_v64(std::vector<dh2::loader::ModuleDrawFrameV1>&,std::string&);
bool bind_source_campaign_object_graph_v68(const SourceCampaignCandidateBorrowV55&,
 std::shared_ptr<dh2::world::CanonicalGameObjectGraphV68>,std::string&);
bool capture_source_campaign_objects_v68(std::vector<std::shared_ptr<dh2::world::RetainedGameObjectVisualV1>>&,std::string&);
// Native primitive-list transport for the source SceneManager registration
// gate. These mutate real GPU submission storage, never source dirty/counters.
bool clear_source_scene_drawlist_v69(std::string&);
bool clear_source_scene_drawlist_loading_v109(const std::shared_ptr<void>& actual_world,std::string&);
//Modern native backend capabilities from its implemented uint16 index
//streams/GLsizei draw-count domain; not inferred original hardware defaults.
bool native_batch_backend_limits_v111(std::uint32_t& vertices,std::uint32_t& indices,std::string&);
bool retire_source_campaign_batch_object_v113(const std::shared_ptr<void>& actual_world,
 std::uintptr_t object,std::string&);
bool finish_source_campaign_batch_object_retirement_v113(const std::shared_ptr<void>& actual_world,
 std::uintptr_t object,std::string&);
bool rebuild_source_scene_drawlist_v69(std::string&);
bool refresh_source_scene_cached_drawlist_v69(std::string&);
bool advance_source_scene_v69(std::string&);
bool register_source_scene_nodes_v69(bool native_context_force,std::string&);
bool source_scene_drawlist_needed_v69(bool&,std::string&);
// GPU transport for actual retained Modules and canonical character visuals,
// including selected restored Gear. Does not construct a Level/player,
// advance animation/physics clocks or certify complete gameplay activation.
bool prepare_source_campaign_geometry_v64(AAssetManager*,std::string&);
bool draw_source_campaign_geometry_v64(int width,int height,std::string&);
bool retire_source_campaign_geometry_v104(const std::shared_ptr<void>& actual_world,std::string&);
bool retire_source_campaign_object_geometry_v106(const std::shared_ptr<void>& actual_world,
 std::uintptr_t actor,std::uintptr_t actual_visual2d8,std::string&);
bool retire_source_campaign_root_geometry_v106(const std::shared_ptr<void>& actual_world,
 std::uintptr_t actual_root,std::string&);
bool bind_native_level_ui_release_v107(dh2::loader::LevelDestroyServicesV1&,std::string&);
void release_source_campaign_geometry_v64(bool context_lost);
bool tick_source_campaign_v55(std::string&);
// Reachable after source failure; drain actual cleanup before original GS Dtor.
bool request_source_campaign_cancel_v61(std::string&);
struct SourceCampaignCandidateBorrowV55 {
 std::shared_ptr<void> actual_world;
 std::shared_ptr<dh2::application::ApplicationServicesOwnerV5> application;
 std::shared_ptr<dh2::loader::CanonicalLevelContextV1> level;
 std::shared_ptr<dh2::world::CanonicalObjectManagerV1> objects;
 dh2::world::CanonicalPropertyMapV1* properties{};
 std::shared_ptr<const dh2::android_ui::FrontSelectedProfileV50> selected_profile;
 std::shared_ptr<dh2::floors::World> floors;
 std::shared_ptr<dh2::world::GameObjectSceneRootRegistryV1> roots;
 std::shared_ptr<dh2::physical::NativeWorld> physical_world;
 std::shared_ptr<dh2::fx::VisualFxManagerLibrariesV63> fx_libraries;
 std::shared_ptr<dh2::navigation::CampaignNavigationRegistryV64> navigation_registry;
 std::shared_ptr<dh2::camera::GameplayCameraApplicationV23> camera_application;
 // Scoped access to already produced source authorities. Consumers lock only
 // during delivery; retaining the complete candidate would pin its World.
 std::weak_ptr<dh2::world::ModulePFRoomsV3> module_rooms_v69;
 std::weak_ptr<dh2::world::SceneManagerMapOwnerV2> map_owner_v69;
 std::weak_ptr<dh2::loader::CanonicalReceiverTransportV1> receiver_transport_v69;
 std::weak_ptr<dh2::loader::ScriptManagerOwnerV52> script_manager_v69;
};
// Scoped borrow of produced SAME owners; never store this complete receipt
// back in its containing World or claim it establishes Character readiness.
bool borrow_source_campaign_candidate_v55(SourceCampaignCandidateBorrowV55&,std::string&);
using SourceCampaignClassFactoryV60=std::function<bool(const dh2::world::CanonicalFactoryEntryV1&,
 const dh2::world::CanonicalSourceObjectRequestV1&,dh2::world::CanonicalClassReceiverV1&,std::string&)>;
// Bind genuine catalog constructors after actual C1, before root XML factories.
// Captures must borrow the containing World weakly; no second manager/PM/Save.
bool bind_source_campaign_class_factory_v60(SourceCampaignClassFactoryV60,std::string&);
bool bind_source_campaign_script_init_v62(const std::shared_ptr<void>& actual_world,
 const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& actual_application,
 std::function<bool(const dh2::loader::CheckedCommandBorrowV59&,std::string&)>,std::string&);
bool bind_source_campaign_gameplay_v64(const std::shared_ptr<void>& actual_world,
 const std::shared_ptr<dh2::loader::CanonicalLevelContextV1>& actual_level,
 std::function<bool(const std::shared_ptr<dh2::loader::CanonicalLevelContextV1>&,bool force,std::string&)>,std::string&);
bool bind_source_campaign_post_init_v66(const std::shared_ptr<void>& actual_world,
 const std::shared_ptr<dh2::player::ApplicationPlayerManagerBootstrapV59>& actual_pm,
 std::function<bool(std::string&)>,std::string&);
bool bind_source_campaign_post_init_services_v66(const SourceCampaignCandidateBorrowV55&,
 dh2::player::PlayerManagerPostInitServicesV66,std::string&);
bool bind_source_campaign_gameplay_services_v66(const SourceCampaignCandidateBorrowV55&,
 dh2::loader::LevelGameplayServicesV66,std::string&);
bool bind_source_campaign_player_placement_v68(const SourceCampaignCandidateBorrowV55&,
 dh2::loader::LevelPlayerPlacementServicesV68,std::string&);
bool bind_source_player_manager_update_v70(const SourceCampaignCandidateBorrowV55&,
 const std::shared_ptr<dh2::player::ApplicationPlayerManagerBootstrapV59>&,
 std::shared_ptr<void> actual_provider,std::function<bool(std::string&)>,std::string&);
bool enable_source_campaign_fog_v68(const SourceCampaignCandidateBorrowV55&,std::string&);
bool disable_source_campaign_fog_v68(const SourceCampaignCandidateBorrowV55&,std::string&);
bool borrow_source_campaign_gameplay_v64(const SourceCampaignCandidateBorrowV55&,
 std::function<bool(const std::shared_ptr<dh2::loader::CanonicalLevelContextV1>&,bool,std::string&)>&,
 std::string&);
// Actual GL-thread Android surface dimensions. A camera must not use the
// pre-resize sentinel as a source viewport.
bool borrow_actual_camera_viewport_v20(std::int32_t&,std::int32_t&,std::string&);
bool borrow_actual_camera_lg_device_v20(std::uint8_t&,std::string&);
// GL-thread borrow from the sole front session's actual Android JNI inputs.
bool borrow_actual_menu_device_v1(dh2::ui::MenuDeviceFactsV1&,std::string&);
bool borrow_actual_font_palette_v4(const dh2::ui::CharacterMenuFontPaletteV1*&,std::string&);
bool borrow_actual_status_messages_v26(std::shared_ptr<dh2::ui::MenuStatusMessagesV26>&,std::string&);
bool borrow_character_menu_application_v4(dh2::ui::HudTextV1&,std::shared_ptr<void>,std::shared_ptr<dh2::ui::AuthoredCharacterApplicationV1>&,std::string&);
bool draw_character_inventory_preview_v4(int,int,std::string&);
bool character_menu_player_v4(std::int32_t,bool remote_or_include,bool local,std::uintptr_t&,std::string&);
std::string load_menu_background(AAssetManager*);
void draw_menu_background(int width,int height);
bool select_menu_persona(int class_index,AAssetManager*,std::string&);
std::string load_class_scene(AAssetManager*);
bool select_class_scene(int class_index,int dt_ms,std::string&);
bool class_scene_active();
bool class_scene_input_enabled();
void draw_class_scene(int width,int height);
void reset_context();
// Functional loading dependency; inspects actual native ownership and releases
// only unreferenced allocations. Never resets the renderer or physics.
bool clean_native_graphics_v50(std::string&);
void deactivate();
bool active();
std::string load(const std::uint8_t*,std::size_t,AAssetManager*);
std::string load_world(const std::uint8_t*,std::size_t,AAssetManager*,const std::string& files_directory,const std::string& selected_mlx_uri);
void move_axis(float x,float y);
void authored_hud_heading(float x,float y,bool active);
bool authored_hud_command(const float* direction,bool stop,std::string& error);
// Raw viewport pixels; HUD owns pointer gestures and calls only world-owned
// press/release phases. Selection mutates the existing player target binding.
bool world_touch(float x,float y,bool released,std::string& error);
// Modern input ownership cancellation; no target mutation or command replay.
void world_touch_cancel();
// Explicit diagnostic capture only; never advances FX or changes game state.
void debug_cast_probe_v46(bool);
void focus_object(int index);
std::string set_object_state(int index,const std::string& state);
std::string set_combat_target(int index,int target);
std::string player_attack(int target=-1);
bool player_hud_attack_actor_v46(std::int32_t,bool,dh2::ui::HudAttackActorBorrowV46&,std::string&);
bool player_hud_attack_command_v46(std::uintptr_t,std::uintptr_t,std::string&);
std::array<int,6> player_vitals();
// GL-thread action bridge used by the forthcoming character menu. Operations
// 0 auto-equip, 1 equip to slot, 2 unequip slot, 3 swap weapon set.
std::string player_equipment_action(int operation,int index,int slot);
// Synchronous GL-thread borrow of the same resolved sheet used by combat.
// Loading another world revokes this view; UI must not cache the data pointer.
struct PlayerHudView {
    const std::int32_t* resolved{};
    std::size_t count{};
    std::uintptr_t character{};
    bool dead{};
};
PlayerHudView player_hud_view();
std::vector<int> player_gameplay_hud();
std::vector<std::string> player_gameplay_hud_icon_names();
std::string player_gameplay_action(int operation,int index);
dh2::ui::EnemyHudWorldBorrowV1 enemy_hud_world_borrow(std::string& error);
// GL-thread borrowed UI sink. Requests copy event values before the current
// camera is submitted; the retained UI owner projects them after world draw.
struct CombatTextSinkV1 {
    void* context{};
    int(*localized)(void*,std::int32_t,const char**){};
    int(*enqueue)(void*,const dh2::character::skills::CombatTextRequestV1*){};
    bool(*format_integer)(void*,const char*,std::int32_t,std::string&,std::string&){};
};
struct CombatTextFrameV1 {std::uint32_t application_dt{};std::int32_t level_load_phase{};bool debug_disabled{},tick{};};
void connect_combat_text(CombatTextSinkV1);
bool combat_text_project(const float[3],std::int32_t*,std::int32_t*,std::string&);
bool combat_text_frame(CombatTextFrameV1&,std::string&);
// SAME actual Application8c frame delta for native GS, combat and UI. Available
// before Character construction; it does not require a gameplay HUD view.
bool borrow_application_dt_v93(std::uint32_t&,std::string&);
bool borrow_application_time_v68(std::uint32_t&,std::string&);
bool borrow_application_frame_v69(std::uint64_t&,std::string&);
bool borrow_application_frame_count_v68(std::uint32_t&,std::string&);
void orbit(float dx,float dy,float zoom);
void set_time(int milliseconds);
void set_enemy_ai(bool);
// Modern single-player character overlay pauses simulation without changing
// the debug inspection cursor or creating a second player snapshot authority.
void set_character_panel_open(bool);
// Synchronous GL-thread binding to the sole live player/Save/Gear/VM graph.
bool connect_character_panel_runtime(const dh2::android_ui::CharacterPanelMovieRuntimeV3&,
 dh2::android_ui::CharacterPanelGameplayServicesV2&,std::string&);
void draw(int width,int height);
}
