#pragma once
#include <chrono>
#include "source_campaign_admission_v104.hpp"
#include <array>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>
#include <android/asset_manager.h>
namespace dh2::application {class ApplicationServicesOwnerV5;}
namespace dh2::assets {class ZipAssetPackV1;}
namespace dh2::character {class CharacterGameDesign;struct DebugSwitches;struct DebugFileServices24;struct HostLevel8;}
namespace dh2::character {struct CharacterOidPreloadServicesV81;}
namespace dh2::world {class CanonicalObjectManagerV1;class CanonicalPropertyMapV1;class GameObjectSceneRootRegistryV1;struct CanonicalFactoryEntryV1;struct CanonicalSourceObjectRequestV1;struct CanonicalClassReceiverV1;}
namespace dh2::world {class CanonicalGameObjectGraphV68;class RetainedGameObjectVisualV1;}
namespace dh2::world {class NativeConditionRuntimeV69;class NativeConditionTableV69;}
namespace dh2::world {struct CanonicalZonePhysicalServicesV82;}
namespace dh2::world {struct CanonicalObjectLifecycleV1;class LightQuiescenceLeaseV67;}
namespace dh2::navigation {class CampaignFrameScratchV76;}
namespace dh2::loader {class CanonicalLevelContextV1;class NativeLevelApplicationV25;class NativeLevelConnectionV25;class NativeGSLevelRuntimeV27;class NativeRootLoadingConnectionV50;struct NativeGSLevelGlobalsV27;struct CheckedCommandBorrowV59;}
namespace dh2::camera {class GameplayCameraApplicationV23;struct CameraWorldSessionV23;}
namespace dh2::player {class ApplicationPlayerManagerBootstrapV59;}
namespace dh2::physical {class NativeWorld;struct NativePhysicalFilterBorrowV1;}
namespace dh2::character {class WorldLootGameplayV23;}
namespace dh2::character {class SourceItemResourcesV88;}
namespace dh2::android_ui {struct FrontSelectedProfileV50;}
namespace dh2::data {class EffectsTables;class DesignSettingsOwner;}
namespace dh2::loader {struct ModuleDrawFrameV1;}
namespace dh2::loader {struct LevelGameplayServicesV66;class LevelGameplayUpdateV66;}
namespace dh2::loader {struct LevelPlayerPlacementServicesV68;}
namespace dh2::loader {class GameEventTablesV50;}
namespace dh2::loader {class GameEventRuntimeV75;}
namespace dh2::loader {struct Stage33RestoreServicesV1;struct Stage34ServicesV1;}
namespace dh2::loader {class SourceReleaseJournalV69;class ModuleRoomZoneConnectionV91;}
namespace dh2::loader {struct ProductionNonCharacterOwnersV67;}
namespace dh2::loader {struct SourceLoadingInputsV43;}
namespace model_renderer {
struct CampaignObjectLoadingNativeV95;
struct CampaignGeneratedRoomNativeV92;
class SourceCampaignFxRuntimeV77;
class SourceCampaignSaveObjectsV86;
class SourceCampaignReleaseV88;
class SourceCampaignDeathRewardsV84;
class SourceCampaignItemsV88;
class SourceCampaignContainerTargetsV104;
struct SourceCampaignCandidateBorrowV55;
struct SourceTerminalNativeV97;
struct RendererCampaignNonCharacterInputsV69;
struct SourceConditionDependenciesV70;
struct SourceQuestMarkerDependenciesV76;
struct SourceGameEventDependenciesV75;
struct SourceCampaignAnchorDirectoryV75;
namespace source_campaign_detail_v61 {
using ClassFactory=std::function<bool(const dh2::world::CanonicalFactoryEntryV1&,
 const dh2::world::CanonicalSourceObjectRequestV1&,dh2::world::CanonicalClassReceiverV1&,std::string&)>;
}
// Native boundary receipts only. Every reference/slot points into the actual
// renderer allocation; these types construct no World/manager/actor or state.
struct SourceCanonicalBorrowV61 {
 std::shared_ptr<void> owner;
 std::shared_ptr<dh2::world::CanonicalObjectManagerV1> manager_lease;
 dh2::world::CanonicalObjectManagerV1& manager;
 dh2::world::CanonicalPropertyMapV1& properties;
 std::shared_ptr<dh2::world::GameObjectSceneRootRegistryV1>& scene_roots_v20;
};
struct SourceWorldBorrowV61 {
 std::shared_ptr<SourceCampaignAdmissionV104> admission_v104=std::make_shared<SourceCampaignAdmissionV104>();
 std::shared_ptr<void> owner;
 std::shared_ptr<dh2::application::ApplicationServicesOwnerV5> application;
 std::shared_ptr<dh2::character::CharacterGameDesign> design;
 std::shared_ptr<dh2::data::EffectsTables> effects;
 std::shared_ptr<dh2::data::DesignSettingsOwner> settings;
 std::shared_ptr<dh2::loader::GameEventTablesV50> game_event_tables;
 std::shared_ptr<dh2::loader::GameEventRuntimeV75> game_events_v75;
 std::shared_ptr<SourceCampaignSaveObjectsV86> save_objects_v86;
 std::shared_ptr<SourceCampaignReleaseV88> release_v88;
 std::shared_ptr<SourceGameEventDependenciesV75> game_event_dependencies_v75;
 std::shared_ptr<dh2::character::CharacterOidPreloadServicesV81> character_preload_v81;
 std::shared_ptr<SourceCanonicalBorrowV61> canonical_world;
 std::shared_ptr<dh2::character::DebugSwitches> debug;
 const dh2::character::DebugFileServices24* debug_files{};
 dh2::character::HostLevel8* host_level{};
 std::string files_directory;
 std::shared_ptr<dh2::loader::NativeLevelApplicationV25> level_application;
 std::shared_ptr<void> files_owner;
 std::function<bool(const std::string&,bool&,std::vector<std::uint8_t>&,std::string&)> read;
 std::function<bool(const std::string&,bool&,std::vector<std::uint8_t>&,
  const std::function<bool(std::uint32_t,std::string&)>&,std::string&)> read_admitted_v81;
 std::function<bool(dh2::assets::ZipAssetPackV1&,std::string&)> archive;
 std::shared_ptr<dh2::camera::GameplayCameraApplicationV23> camera_application;
 std::shared_ptr<SourceCampaignAnchorDirectoryV75> camera_anchors_v75;
 std::shared_ptr<dh2::camera::CameraWorldSessionV23>* camera_session{};
 std::shared_ptr<dh2::player::ApplicationPlayerManagerBootstrapV59> player_manager;
 std::shared_ptr<dh2::physical::NativeWorld> physical_world;
 // Genuine remaining nontrivial command Init leaves. The source owner calls
 // these only at their reached original body, never as load readiness probes.
 std::function<bool(const dh2::loader::CheckedCommandBorrowV59&,std::string&)> script_command_init;
 std::function<bool(const std::shared_ptr<dh2::loader::CanonicalLevelContextV1>&,bool,std::string&)> gameplay_update;
 std::function<bool(std::string&)> post_init_characters;
 //Existing Main online/menu/input resource behavior, independent source owner.
 std::shared_ptr<SourceTerminalNativeV97> terminal_loading_v97;
 std::shared_ptr<dh2::loader::LevelGameplayServicesV66> gameplay_services_v66;
 std::shared_ptr<dh2::loader::LevelGameplayUpdateV66> gameplay_owner_v66;
 std::shared_ptr<dh2::loader::LevelPlayerPlacementServicesV68> player_placement_v68;
 std::shared_ptr<dh2::character::WorldLootGameplayV23> prepared_items_v88;
 std::shared_ptr<SourceCampaignItemsV88> item_transport_v88;
 std::shared_ptr<SourceCampaignContainerTargetsV104> container_targets_v104;
 std::shared_ptr<dh2::character::SourceItemResourcesV88> item_resources_v88;
 std::shared_ptr<SourceCampaignDeathRewardsV84> death_rewards_v84;
 std::shared_ptr<dh2::loader::Stage33RestoreServicesV1> stage33_native_v80;
 std::shared_ptr<dh2::loader::Stage34ServicesV1> stage34_native_v80;
 std::shared_ptr<void> source_pm_update_provider_v70;
 std::function<bool(std::string&)> source_pm_update_v70;
 std::function<bool(const SourceCampaignCandidateBorrowV55&,
  RendererCampaignNonCharacterInputsV69&,std::string&)> noncharacter_inputs_v81;
 // Native resource input stays separate from the actual source Stage10/17
 // bridge. Both callbacks borrow this World weakly, never a second scheduler.
 std::function<bool(const SourceCampaignCandidateBorrowV55&,CampaignObjectLoadingNativeV95&,std::string&)> object_loading_native_v95;
 std::function<bool(const SourceCampaignCandidateBorrowV55&,dh2::loader::SourceLoadingInputsV43&,std::string&)> source_object_loading_v95;
 std::shared_ptr<void> noncharacter_producer_v89;
 std::shared_ptr<dh2::loader::ProductionNonCharacterOwnersV67> noncharacter_owners_v105;
 //Actual generated-room constructor/runtime sibling. The provider borrows
 //this World weakly and retains independent native primitive authority.
 std::function<bool(const SourceCampaignCandidateBorrowV55&,CampaignGeneratedRoomNativeV92&,std::string&)> generated_room_native_v92;
 std::shared_ptr<dh2::loader::ModuleRoomZoneConnectionV91> module_room_zones_v91;
 std::shared_ptr<dh2::loader::SourceReleaseJournalV69> noncharacter_release_journal_v89;
 std::function<bool(dh2::world::CanonicalZonePhysicalServicesV82&,std::string&)> physical_services_v90;
 std::function<bool(std::uintptr_t,std::shared_ptr<void>&,dh2::physical::NativePhysicalFilterBorrowV1&,std::string&)> physical_filter_v105;
 std::function<bool(std::uintptr_t,std::string&)> noncharacter_frame_v104;
 std::shared_ptr<dh2::world::CanonicalGameObjectGraphV68> gameobject_graph_v68;
 std::shared_ptr<dh2::navigation::CampaignFrameScratchV76> frame_scratch_v76;
 std::shared_ptr<SourceCampaignFxRuntimeV77> fx_runtime_v77;
 //ONE actual condition arena/table per retained campaign. Its services only
 //weakly borrow this faÃ§ade; publishing these slots creates no World cycle.
 std::shared_ptr<const dh2::world::NativeConditionTableV69> condition_tables_v70;
 std::shared_ptr<dh2::world::NativeConditionRuntimeV69> conditions_v70;
 std::shared_ptr<SourceConditionDependenciesV70> condition_dependencies_v70;
 std::shared_ptr<SourceQuestMarkerDependenciesV76> quest_marker_dependencies_v76;
 bool condition_creation_attempted_v70{};
 std::string condition_creation_failure_v70;
 std::function<bool(std::uint32_t,const char*,std::uint32_t&,std::string&)> fx_debug;
 std::shared_ptr<dh2::loader::NativeLevelConnectionV25>* native_level_c1_v25{};
 std::shared_ptr<dh2::loader::NativeGSLevelRuntimeV27>* native_gslevel_v27{};
 std::unique_ptr<dh2::loader::NativeRootLoadingConnectionV50>* native_loading_v50{};
 std::chrono::steady_clock::time_point* last_frame{};
 std::uint32_t* application_dt{};bool* application_tick{};
};
struct SourceCampaignRendererServicesV61 {
 std::shared_ptr<dh2::loader::NativeGSLevelGlobalsV27> globals;
 // Creates the REAL renderer World and startup dependencies, then lends the
 // SAME canonical references/IO/Scene/PM/physical owner and publication slots.
 std::function<bool(AAssetManager*,const std::shared_ptr<const dh2::android_ui::FrontSelectedProfileV50>&,
   std::shared_ptr<SourceWorldBorrowV61>&,std::string&)> create_world;
};
bool start_source_campaign_runtime_v61(AAssetManager*,
 std::shared_ptr<const dh2::android_ui::FrontSelectedProfileV50>,
 SourceCampaignRendererServicesV61,std::string&);
bool source_campaign_runtime_active_v61();
bool update_source_campaign_noncharacter_v104(const std::shared_ptr<SourceWorldBorrowV61>&,
 std::uintptr_t,std::string&);
bool bind_source_campaign_journal_lifecycle_v104(const std::shared_ptr<SourceWorldBorrowV61>&,
 dh2::world::CanonicalObjectLifecycleV1&,
 std::function<bool(std::shared_ptr<dh2::world::LightQuiescenceLeaseV67>&,std::string&)>,std::string&);
bool borrow_source_campaign_room_camera_v104(const std::shared_ptr<void>&,
 std::array<std::array<float,4>,6>&,std::string&);
bool borrow_source_campaign_physical_services_v90(const std::shared_ptr<SourceWorldBorrowV61>&,
 dh2::world::CanonicalZonePhysicalServicesV82&,std::string&);
bool source_campaign_loading_complete_runtime_v64();
bool capture_source_campaign_modules_runtime_v64(std::vector<dh2::loader::ModuleDrawFrameV1>&,std::string&);
bool bind_source_campaign_object_graph_v68(const SourceCampaignCandidateBorrowV55&,
 std::shared_ptr<dh2::world::CanonicalGameObjectGraphV68>,std::string&);
bool capture_source_campaign_objects_v68(std::vector<std::shared_ptr<dh2::world::RetainedGameObjectVisualV1>>&,std::string&);
bool tick_source_campaign_runtime_v61(std::string&);
bool request_source_campaign_cancel_runtime_v61(std::string&);
bool borrow_source_campaign_candidate_runtime_v61(SourceCampaignCandidateBorrowV55&,std::string&);
bool borrow_source_campaign_condition_world_v70(const SourceCampaignCandidateBorrowV55&,
 std::shared_ptr<SourceWorldBorrowV61>&,std::string&);
bool bind_source_campaign_class_factory_runtime_v61(source_campaign_detail_v61::ClassFactory,std::string&);
bool bind_source_campaign_script_init_runtime_v62(const std::shared_ptr<void>& actual_world,
 const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>&,
 std::function<bool(const dh2::loader::CheckedCommandBorrowV59&,std::string&)>,std::string&);
bool bind_source_campaign_gameplay_runtime_v64(const std::shared_ptr<void>& actual_world,
 const std::shared_ptr<dh2::loader::CanonicalLevelContextV1>&,
 std::function<bool(const std::shared_ptr<dh2::loader::CanonicalLevelContextV1>&,bool,std::string&)>,std::string&);
bool bind_source_campaign_post_init_runtime_v66(const std::shared_ptr<void>& actual_world,
 const std::shared_ptr<dh2::player::ApplicationPlayerManagerBootstrapV59>& actual_pm,
 std::function<bool(std::string&)>,std::string&);
}
