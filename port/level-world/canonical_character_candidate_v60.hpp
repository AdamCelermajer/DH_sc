#pragma once
#include "canonical_character_family_v4.hpp"
#include "retained_character_family_visual_v6.hpp"
#include "retained_character_position_owner_v7.hpp"
#include "loot_source_fields_v47.hpp"
#include "character_profile_bootstrap_v59.hpp"
#include "character_props_id_owner_v1.hpp"
#include "character_generic_initpost_owner_v61.hpp"
#include "../game-data/player_save_quest_sync_v3.hpp"
#include "character_generic_animation_registration_v62.hpp"
#include "character_init_fx.hpp"
#include "character_world_physical_v62.hpp"
#include "character_model_name_v62.hpp"
#include "character_player_skills_v6.hpp"
#include "../engine-ui/hud_manager.hpp"
#include "character_menu_mutations_v4.hpp"
#include "character_menu_faery_connection_v8.hpp"
#include "character_world_collision_v68.hpp"
#include "../game-data/character_templates_v78.hpp"
#include "player_add_character_owner_v5.hpp"
#include "player_controller_attachment_v70.hpp"
#include "canonical_npc_skills_v84.hpp"
#include "canonical_character_death_v84.hpp"
#include "character_target_marker_v28.hpp"
#include "character_melee_animation_event_v1.hpp"
#include "character_deferred_queue.hpp"
#include <optional>
namespace dh2::character {class CanonicalCharacterSaveV86;class CharacterAiGroupV87;class CanonicalCharacterSpawnSelectV87;}
namespace dh2::world {
struct CanonicalCharacterCandidateRecordV60;
struct CanonicalPlayerScriptInputsV62 {
 character::CharacterScriptSessionInputV3 session;
 data::FaeryTables::Borrow faeries;
 character::skills::PlayerSkillInitServicesV3 initialization;
};
struct CharacterCurrentLevelBorrowV61 {
 std::shared_ptr<void> receiver;
 std::uintptr_t identity{};
 const std::int32_t* mode118{};
};
struct CanonicalCharacterCandidateServicesV60 {
 std::shared_ptr<void> world;
 character::CharacterGameDesign* design{};
 const data::Dictionary* models{};
 character::CharacterModelNameServicesV62 model_name;
 const data::AnimationTables* animation_tables{};
 data::SkillTables::Borrow skills;
 data::FaeryTables::Borrow faeries_v70;
 data::LootTablesV2* loot_tables{};
 data::LootRandom8V2* random{};
 data::LootRandom8V2* alternate_random{};
 std::shared_ptr<void> random_lease;
 std::function<bool(bool&,std::string&)> online_byte5;
 std::function<bool(CanonicalCharacterCandidateRecordV60&,std::string&)> failed_spawn_visible,failed_spawn_mark;
 std::function<bool(CanonicalCharacterCandidateRecordV60&,character::WorldNpcStateServicesV1&,std::string&)> state;
 std::function<bool(float&,float&,std::string&)> state_thresholds_v101;
 std::function<bool(std::uint32_t&,std::string&)> path_policy_v101;
 std::function<bool(CanonicalCharacterCandidateRecordV60&,const character::Request&,std::string&)> state_body_v101;
 std::function<bool(CanonicalCharacterCandidateRecordV60&,const character::StateOwnerRequest48&,std::string&)> state_method_v101;
 std::function<bool(CanonicalCharacterCandidateRecordV60&,const character::NativeFsmRequest32&,std::uint32_t&,std::string&)> state_frame_v101;
 std::function<bool(CanonicalCharacterCandidateRecordV60&,const character::StateOwnerUpdateRequest24&,std::string&)> state_update_v101;
 std::function<bool(CanonicalCharacterCandidateRecordV60&,const character::skills::SkillAIRequest32V3&,character::skills::SkillAIResponse32V3&,std::string&)> skill_ai_v101;
 std::function<bool(CanonicalCharacterCandidateRecordV60&,std::string&)> cancel_sneaking_v101;
 std::function<bool(CanonicalCharacterCandidateRecordV60&,const character::skills::MeleeAnimationRequestV1&,character::skills::MeleeAnimationResponseV1&,std::string&)> melee_animation_v101;
 std::function<bool(CanonicalCharacterCandidateRecordV60&,const character::AnimationAIRequest32&,character::AnimationAIResponse16&,std::string&)> animation_consumer_v101;
 std::function<bool(CanonicalCharacterCandidateRecordV60&,const character::AIEventRequest40&,std::uint32_t&,std::string&)> ai_event_v101;
 std::function<bool(CanonicalCharacterCandidateRecordV60&,const char*,std::string&)> named_sound_v101;
 std::function<bool(CanonicalCharacterCandidateRecordV60&,CanonicalPlayerScriptInputsV62&,std::string&)> player_script;
 std::function<bool(CanonicalCharacterCandidateRecordV60&,character::CharacterScriptSessionInput&,std::string&)> npc_script;
 std::function<bool(const std::shared_ptr<character::ScriptCharacterObject>&,std::string&)> publish_script_object;
 std::function<bool(CanonicalCharacterCandidateRecordV60&,character::CharacterFamilyVisualServicesV6&,std::string&)> visual;
 std::function<bool(CanonicalCharacterCandidateRecordV60&,std::string&)> visual_ready_v77;
 std::function<bool(CanonicalCharacterCandidateRecordV60&,character::RetainedCharacterPositionBackendsV7&,std::string&)> position;
 std::function<bool(const float*,bool&,float&,std::string&)> initial_height;
 std::function<bool(CanonicalCharacterCandidateRecordV60&,GameObjectInitializationServicesV1&,std::string&)> inherited;
 ConditionDataInitServicesV3 conditions;
 std::shared_ptr<physical::NativeWorld> physical_world;
 std::shared_ptr<character::skills::CharacterWorldRuntimeV1> world_targets;
 // Borrow the manager that published this SAME shared Handle. Target-query
 // epochs follow its source frame78; they do not create a second frame clock.
 std::shared_ptr<CanonicalObjectManagerV1> canonical_objects;
 std::function<bool(character::ControllerCommandState32&,CanonicalCharacterCandidateRecordV60&,std::string&)> controller;
 std::function<bool(CanonicalCharacterCandidateRecordV60&,character::CharacterPhysicalCollisionBorrowV62&,character::WorldNpcPhysicalServicesV1&,std::string&)> physical;
 // Builds SAME moved Gear at actual SG_Load4, calls prepare_restore only.
 std::function<bool(CanonicalCharacterCandidateRecordV60&,std::string&)> prepare_player_equipment;
 std::function<bool(CanonicalCharacterCandidateRecordV60&,player::PlayerEquipmentRenderInputsV1&,std::string&)> equipment_inputs;
 std::function<bool(CanonicalCharacterCandidateRecordV60&,std::string&)> release_player_equipment;
 std::function<bool(CanonicalCharacterCandidateRecordV60&,std::uintptr_t,std::string&)> destroy_character_save_v107;
 std::function<bool(CanonicalCharacterCandidateRecordV60&,std::uintptr_t,std::string&)> destroy_character_aux14ec_v107;
 // Native persistence attachment AFTER the whole source InitPost succeeds.
 // Does not grant equipment, reload GEAR or replace Save/VM identities.
 std::function<bool(CanonicalCharacterCandidateRecordV60&,std::string&)> finish_player_profile_v67;
 std::function<bool(CanonicalCharacterCandidateRecordV60&,std::string&)> player_save_final_v122;
 character::CharacterMenuMutationServicesV4 menu_mutations_v68;
 character::CharacterMenuFaeryServicesV8 menu_faery_v68;
 std::shared_ptr<character::WorldNpcCollisionGlobalsV1> collision_globals_v68;
 std::function<bool(std::uint32_t&,std::uint32_t&,std::string&)> collision_clock_v68;
 std::function<bool(CharacterCurrentLevelBorrowV61&,std::string&)> current_level;
 std::shared_ptr<player::PlayerSaveDifficultyGlobalV29> difficulty_global;
 std::function<bool(std::uintptr_t,bool&,std::string&)> is_local_player;
 data::PlayerSaveQuestSyncServicesV3 quest_sync;
 character::DebugSwitches* debug{};
 const character::DebugFileServices24* debug_files{};
 const std::vector<data::CharacterEffects>* character_effects{};
 const fx::PreloadTable16* effect_table{};
 fx::PreloadQueue16* effect_queue{};
 const fx::PreloadServices16* effect_services{};
 std::function<bool(CanonicalCharacterCandidateRecordV60&,std::int32_t,std::uintptr_t&,std::string&)> grab_fx;
 std::function<bool(CanonicalCharacterCandidateRecordV60&,std::string&)> register_character_fx;
 std::function<bool(CanonicalCharacterCandidateRecordV60&,bool&,std::string&)> initialize_target_marker_v70;
 std::function<bool(CanonicalCharacterCandidateRecordV60&,std::string&)> initialize_highlight_v70;
 std::function<bool(CanonicalCharacterCandidateRecordV60&,std::string&)> initialize_sounds_v70;
 std::function<bool(CanonicalCharacterCandidateRecordV60&,const character::NpcInitPostRequestV1&,character::NpcInitPostResponseV1&,std::string&)> remaining;
 character::CharacterPropsIdServicesV1 presets;
 std::shared_ptr<const data::CharacterTemplateTableV78> character_templates_v78;
 std::shared_ptr<const void> selected_profile_lease;
 const std::int32_t* selected_profile_class{};
};
struct CanonicalCharacterCandidateRecordV60:std::enable_shared_from_this<CanonicalCharacterCandidateRecordV60> {
 // Diagnostic provenance for the one retained native main-menu preview.
 // These fields do not participate in save/class/model selection.
 std::int32_t menu_preview_selected_slot_v122{-1};
 bool menu_preview_fresh_v122{};
 std::shared_ptr<character::CharacterAiGroupV87> ai_group_v87;
 std::unique_ptr<character::CanonicalCharacterSpawnSelectV87> spawn_select_v87;
 CanonicalCharacterCandidateServicesV60 services;
 CanonicalSourceObjectRequestV1 source;
 character::CharacterGameDesign::Borrow design;
 std::shared_ptr<character::RetainedCharacterActorV1> actor;
 std::shared_ptr<data::PropertyState> properties;
 std::shared_ptr<data::CombatActorState> life;
 data::PropertyView view{};
 // ONE actual constructor inventory37c for both catalog aliases. Gear takes
 // ownership once; this raw observation remains valid only with that owner.
 std::unique_ptr<data::FreshInventoryOwnedV4> constructor_inventory;
 data::FreshInventoryOwnedV4* inventory37c{};
 player::PlayerEquipmentRenderOwnerV1* prepared_equipment_v60{}; // borrowed SAME moved owner
 std::shared_ptr<void> merchant_inventory_context_v114; //native AddLoot adapter, SAME inventory37c; retains failed source prefixes
 std::unique_ptr<player::PlayerEquipmentRenderOwnerV1> equipment;
 std::shared_ptr<character::LootPlayerFieldAssociationV47> save_fields;
 std::shared_ptr<data::PlayerSavegameV1> save;
 std::shared_ptr<data::PlayerSaveLoadOwnerV1> load;
 std::unique_ptr<data::PlayerSaveQuestSyncOwnerV3> quest_sync_owner;
 std::shared_ptr<character::CharacterProfileBootstrapV59> profile_bootstrap;
 std::shared_ptr<void> preview_profile_v122; //actual process Save/profile reader lifetime
 std::unique_ptr<character::RetainedCharacterFamilyVisualV6> visual;
 std::unique_ptr<character::RetainedCharacterPositionOwnerV7> position;
 character::NpcInitPostBorrowV1 init_fields{};
 std::unique_ptr<character::CharacterNpcInitPostOwnerV1> npc_init;
 std::unique_ptr<character::CharacterGenericInitPostOwnerV61> generic_init;
 std::unique_ptr<GameObjectInitializationBorrowV62> inherited_init;
 std::unique_ptr<character::CharacterWorldPhysicalV62> physical_owner_v62;
 std::unique_ptr<character::skills::CharacterPlayerSkillsV6> player_script_owner_v62;
 std::unique_ptr<character::CanonicalNpcSkillsV84> npc_skills_v84;
 std::unique_ptr<character::CanonicalCharacterSaveV86> save_connection_v86;
 std::unique_ptr<character::CanonicalCharacterDeathV84> death_v84;
 std::unique_ptr<character::CharacterDeferredScript> npc_loaded_init_v70;
 std::shared_ptr<void> host_context_v70; // scoped source PM330/GS host provider, not cached defaults
 std::shared_ptr<void> fsm_context_v101; // SAME FSM/animator dispatch, no second actor
 std::shared_ptr<void> projectile_bindings_v112; // weak World native globals, survives VM finalizers
 std::shared_ptr<void> combat_bindings_v115; //SAME native application callbacks, retained through VM finalizers
 data::AnimationRandom* animation_random_inflight_v101{};
 character::skills::SkillAIOwnerV3 skill_owner_view_v68{}; // projection of SAME Character flags520, refreshed at source calls
 // SAME original player cells, first written by source AddCharacter before
 // InitAll. Their native allocation contents are not C1/default producers.
 std::int32_t player_controller1f88_v70{},player_internal1f8c_v70{};
 std::unique_ptr<CanonicalPlayerFacetV3> player_facet_v70;
 struct ControllableFieldsV70 {std::uintptr_t controller378{};} controllable374_v70;
 std::unique_ptr<player::PlayerControllerAttachmentV70> player_controllers_v70;
 std::shared_ptr<void> target_fx_pin_v70;
 fx::CharacterMeshFxOwnerV4* target_fx_manager_v70{}; // borrowed from the pinned SAME runtime
 std::unique_ptr<character::CharacterTargetMarkerV28> target_marker_v70;
 std::unique_ptr<character::CharacterTimerUtilFieldsV106> timer_util_v107;
 //Stable native loan for entries of the ONE source concurrent-AI map.
 //Not another Character/controller slot; refreshed from the retained receiver.
 character::DeferredQueueOwner16 deferred_receiver_v108{};
 bool clean_attempted_v107{},clean_completed_v107{};
 bool script_load_attempted_v62{},script_init_attempted_v62{};
 // Stable borrowed HUD views. They own no HP/target/skill instances: pointers
 // are refreshed from this receiver's sole actual property/VM owners.
 ui::HudManagerActor hud_actor_v62{};
 std::vector<std::uintptr_t> hud_skills_v62,hud_spells_v62;
 target_search::Object48 target_search_v62{};
 character::skills::SkillTargetCharacterV6 target_character_v62{};
 bool target_registered_v62{};
 std::unique_ptr<character::PlayerFaeryAssociationV8> faery_association_v68;
 character::WorldNpcAISCollisionFieldsV1 collision_fields_v68;
 std::unique_ptr<character::CharacterWorldCollisionV68> collision_owner_v68;
 CharacterCurrentLevelBorrowV61 captured_level_v61; // call-result borrow, no copied Level fields
 bool init_attempted{},init_complete{},failed{},save_attempted{},inventory_transferred{},equipment_released{};
 std::string error;
 bool is_player(bool&,std::string&)const;
 bool initialize(std::string&);
 bool initialize_final_v70(std::string&);
 bool initialize_skill_slots_v70(std::string&);
 bool revive_v70(std::uintptr_t target,std::uint32_t initialize_physical,std::string&);
 // Exact InitializePlayerSavegame3b36b0 C1 -> store14e8 -> SetCharacter.
 // Invoked ONLY by source PlayerManager initialize_save endpoint; no replay.
 bool initialize_player_save(std::string&);
 bool player_add_borrow_v70(player::PlayerAddCharacterBorrowV5&,std::string&);
 bool transfer_inventory(player::PlayerEquipmentRenderInputsV1&,std::string&);
 bool prepare_equipment(std::string&);
 bool take_equipment(std::unique_ptr<player::PlayerEquipmentRenderOwnerV1>&,std::string&);
 bool bind_visual(std::string&);
 bool load_visual_v77(std::string&);
 bool bind_inherited_initialization(std::string&);
 bool initialize_physical(std::string&);
 bool load_script(std::string&);
 bool initialize_loaded_script(std::uint32_t final,std::string&);
 // Frame's actual LoadNInit boundary; preserves a completed unload/reload
 // on SAME Session/skill-vector storage without replaying Character C1.
 int source_load_n_init_v106(std::uint32_t final,std::string&);
 bool refresh_hud_actor_v62(std::string&);
 bool register_world_target_v62(std::string&);
 static int target_borrow_v62(void*,character::skills::WorldTargetActorBorrowV1*);
 bool set_position(const std::array<float,3>&,bool,std::string&);
 static bool init_service(void*,const character::NpcInitPostRequestV1&,character::NpcInitPostResponseV1&,std::string&);
 bool close_after_unpublication(std::string&);
};
class CanonicalCharacterCandidateFactoryV60 {
 CanonicalCharacterCandidateServicesV60 services_;
 std::map<std::uintptr_t,std::shared_ptr<CanonicalCharacterCandidateRecordV60>> records_;
public:
 explicit CanonicalCharacterCandidateFactoryV60(CanonicalCharacterCandidateServicesV60 services):services_(std::move(services)){}
 bool construct(const CanonicalFactoryEntryV1&,const CanonicalSourceObjectRequestV1&,CanonicalClassReceiverV1&,std::string&);
 std::shared_ptr<CanonicalCharacterCandidateRecordV60> find(std::uintptr_t)const;
 std::shared_ptr<CanonicalCharacterCandidateRecordV60> find_hud_actor_v62(std::uintptr_t)const;
 bool physical_peer_v68(void* context,std::uintptr_t&,std::string&)const;
 bool erase_after_unpublication(std::uintptr_t,std::string&);
 bool retire_closed_v111(std::uintptr_t,std::string&);
 const std::shared_ptr<character::skills::CharacterWorldRuntimeV1>& source_targets_v88()const noexcept{return services_.world_targets;}
};
}






