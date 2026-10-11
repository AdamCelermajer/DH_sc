cmake_minimum_required(VERSION 3.22)
project(dh2_level_world LANGUAGES C CXX)
if(CMAKE_HOST_WIN32)
 # Permit hashed object paths in deeply nested Android build directories.
 # A smaller limit can make CMake give up hashing and emit the full source path.
 set(CMAKE_OBJECT_PATH_MAX 260)
endif()
if(NOT TARGET dh2_script_runtime)
 add_subdirectory(../adam-script-runtime "${CMAKE_CURRENT_BINARY_DIR}/adam-script-runtime")
endif()
if(NOT TARGET dh2_engine_skinning)
 add_subdirectory(../engine-skinning "${CMAKE_CURRENT_BINARY_DIR}/engine-skinning")
endif()
if(NOT TARGET dh2_game_data)
 add_subdirectory(../game-data "${CMAKE_CURRENT_BINARY_DIR}/game-data")
endif()
if(NOT TARGET dh2_box2d_201)
 add_subdirectory(../physics-backend "${CMAKE_CURRENT_BINARY_DIR}/physics-backend")
endif()
add_library(dh2_level_world SHARED world.cpp objects.cpp navigation.cpp navigation_search.cpp navigation_world.cpp navigation_path.cpp navigation_motion.cpp navigation_objects.cpp navigation_avoidance.cpp navigation_producers.cpp navigation_heading.cpp navigation_controller.cpp physical_controls.cpp body_transform.cpp subobjects_update.cpp controller_physical.cpp character_body_config.cpp collision.cpp octree.cpp selector.cpp floor_source.cpp floors.cpp)
target_sources(dh2_level_world PRIVATE ../floor-types/floor_types.cpp)
target_sources(dh2_level_world PRIVATE source_level_layout.cpp source_handle_ledger_v1.cpp ../world-data/world.cpp ../world-data/source_layout_adapter.cpp)
target_sources(dh2_level_world PRIVATE source_level_owner_v1.cpp level_quick_save_v1.cpp level_savegame_save_v1.cpp level_savegame_owner_v1.cpp character_level_save_serialize_v1.cpp level_savegame_object_manager_v1.cpp level_savegame_player_load_v1.cpp source_level_lifecycle_gate_v1.cpp item_manager_pool_v1.cpp)
if(TARGET dh2_scene_materials)
 # The Android renderer uses the source Module root path to build real
 # per-module Crypt bounds. Keep the portable kernels in the shared world
 # library and reuse the app's already-selected scene/BRES decoder.
 target_sources(dh2_level_world PRIVATE ../world-data/world_scene.cpp ../scene-payloads/scene.cpp
  module_scene_root_bounds.cpp crypt_module_bounds_registry_v1.cpp)
 target_link_libraries(dh2_level_world PRIVATE dh2_scene_materials)
endif()
target_compile_features(dh2_level_world PUBLIC cxx_std_17)
target_sources(dh2_level_world PRIVATE native_visual_fx_manager_v1.cpp)
target_sources(dh2_level_world PRIVATE camera_level_runtime_v1.cpp camera_design_zoom_v1.cpp camera_animset_v1.cpp character_critical_camera_bridge_v1.cpp camera_animset_bank_v1.cpp camera_animset_playback_v1.cpp camera_animset_bank_playback_v1.cpp player_camera_rig_v1.cpp)
target_sources(dh2_level_world PRIVATE character_aggro_character_list.cpp character_aggro_object_manager_list.cpp)
target_sources(dh2_level_world PRIVATE physical_world.cpp physical_lifecycle.cpp native_body.cpp visual_motion.cpp move_state.cpp player_root_motion_policy_v1.cpp decor_body_config.cpp visual_timeline.cpp actor_rotation.cpp actor_runtime.cpp actor_playback.cpp decor_scene.cpp character_scene.cpp)
target_sources(dh2_level_world PRIVATE character_physical_component_v1.cpp)
target_sources(dh2_level_world PRIVATE character_state.cpp animation_blender.cpp)
target_sources(dh2_level_world PRIVATE character_oid_cache_v1.cpp)
target_sources(dh2_level_world PRIVATE character_init_spawned_v1.cpp)
target_sources(dh2_level_world PRIVATE spawn_group_runtime_v1.cpp)
target_sources(dh2_level_world PRIVATE character_cast_state_dispatch_v1.cpp
 character_cast_lifecycle_v1.cpp)
target_sources(dh2_level_world PRIVATE character_coordinator.cpp native_player_character_owner_v1.cpp
 character_ai_skill_machine_projection_v1.cpp)
target_sources(dh2_level_world PRIVATE character_factory.cpp)
target_sources(dh2_level_world PRIVATE crypt_spawn_script_session.cpp crypt_spawn_trigger.cpp
 ../script-runtime/script_runtime.cpp ../pydata-scripts/native/pydata_scripts.cpp
 ../trigger-contact/trigger_contact.cpp ../zone-contact-runtime/zone_geometry.cpp)
target_sources(dh2_level_world PRIVATE character_timers.cpp character_stance.cpp)
target_sources(dh2_level_world PRIVATE character_animation_events.cpp)
target_sources(dh2_level_world PRIVATE character_animation_ai.cpp)
target_sources(dh2_level_world PRIVATE character_enemy_pursuit_v1.cpp)
target_sources(dh2_level_world PRIVATE character_enemy_attack_event_v1.cpp)
target_sources(dh2_level_world PRIVATE character_target_out_of_sight_v1.cpp)
target_sources(dh2_level_world PRIVATE player_attack_step_v1.cpp)
target_sources(dh2_level_world PRIVATE character_script_selection.cpp)
target_sources(dh2_level_world PRIVATE character_script_lifecycle.cpp character_combat_queries.cpp)
target_sources(dh2_level_world PRIVATE character_combat_event_v1.cpp)
target_sources(dh2_level_world PRIVATE character_script_update.cpp character_script_collision.cpp)
target_sources(dh2_level_world PRIVATE actor_blended_playback.cpp)
target_sources(dh2_level_world PRIVATE actor_scene_retention_v1.cpp)
target_sources(dh2_level_world PRIVATE character_controller_commands.cpp character_ai_update.cpp)
target_sources(dh2_level_world PRIVATE character_ai_state.cpp)
target_sources(dh2_level_world PRIVATE character_path_commands.cpp)
target_sources(dh2_level_world PRIVATE character_ai_frame.cpp)

# Source-tested external monster/acquisition kernels. Compiling these into the
# native library establishes the 64-bit build boundary; live actor services,
# candidate ownership and full AIS lifecycle must still be connected by the
# owning game runtime. Reuse this target's existing Adam Lua core exactly once.
# The portable Crypt generation, serialization, DACT and SpawnPoint kernels
# below are selected into this ABI. NativeStartGame now generates row-23 DWLD,
# SPWN and supported direct-Monster DACT; source Module/RoomZone activation
# and the remaining ObjectManager lifecycle are still open.
target_sources(dh2_level_world PRIVATE
 character_aggro_delay.cpp character_ai_turn.cpp
 character_aggro_target_search.cpp character_aggro_candidate_events.cpp
 character_enemy_spotted.cpp monster_external_script_session.cpp
 character_ai_set_target.cpp character_clear_target_v1.cpp character_ai_relations.cpp character_ai_in_combat.cpp
 character_aggro_acquisition_prefix.cpp character_ai_sight.cpp ghost_ai_session.cpp character_monster_retarget.cpp
 source_random_lifecycle_v1.cpp crypt_room_zone_owner_v1.cpp
 object_manager_runtime_owner_v1.cpp object_manager_init_post_v1.cpp
 projectile_lifecycle_runtime_v1.cpp
 projectile_factory_owner_v1.cpp
 projectile_manager_runtime_owner_v1.cpp
 crypt_room_zone_manager_bridge_v1.cpp
 crypt_generated_spawnpoints_v1.cpp crypt_generated_source_dact_v1.cpp
 crypt_generated_mvp_v1.cpp
 ../random-level/native_rule_plan_v1.cpp
 ../random-level/crypt_module_catalog_v1.cpp
 ../random-level/crypt_level_generator_v1.cpp
 ../random-level/crypt_mgx_connectivity_v1.cpp
 ../random-level/crypt_mgx_placement_v1.cpp
 ../random-level/crypt_candidate_selection_v1.cpp
 ../random-level/crypt_rule_distribution_v1.cpp
 ../random-level/crypt_path_runtime_v1.cpp
 ../random-level/crypt_root_rule_runtime_v1.cpp
 ../random-level/crypt_rule_generation_runtime_v1.cpp
 ../random-level/crypt_tile_map_v1.cpp
 ../random-level/crypt_generated_layout_v1.cpp
 ../random-level/crypt_generated_dact_v1.cpp
 ../random/random.c ../lua-numeric/numeric.c)

target_sources(dh2_level_world PRIVATE character_ai_attack.cpp
 character_kill_death_tail_v1.cpp character_kill_source_bridge_v1.cpp
 player_character_kill_provider_v1.cpp character_kill_npc_provider_v1.cpp
 character_kill_field_14e4_v1.cpp)

target_sources(dh2_level_world PRIVATE character_ai_classification.cpp character_zonability.cpp
 character_update_eligibility.cpp lua_script_load_once.cpp)
target_sources(dh2_level_world PRIVATE
 game_object_set_visible.cpp game_object_zoning_visibility.cpp
 object_update_culling.cpp object_update_dispatch.cpp character_culling_runtime.cpp)

# Verified caller/ownership prerequisites. Live Character frame, path/body and
# shared concurrent-map services remain explicit native runtime bindings.
target_sources(dh2_level_world PRIVATE game_object_stop.cpp character_physics_position.cpp
 character_update_script_scheduler.cpp)

target_sources(dh2_level_world PRIVATE character_script_timers.cpp)
target_sources(dh2_level_world PRIVATE character_enemy_retention.cpp character_target_search.cpp
 character_target_list_backup_v1.cpp)
target_sources(dh2_level_world PRIVATE character_ai_master_update.cpp)
target_sources(dh2_level_world PRIVATE character_ai_update_target.cpp)
target_sources(dh2_level_world PRIVATE character_ai_melee_range.cpp)

target_sources(dh2_level_world PRIVATE character_ai_interaction_range.cpp
 character_interaction_spot_owner_v1.cpp ais_external_update.cpp)
target_sources(dh2_level_world PRIVATE character_ai_ranged_range.cpp)
target_sources(dh2_level_world PRIVATE character_range_capability.cpp character_ai_queue.cpp character_ai_queue_owner_v1.cpp)
target_sources(dh2_level_world PRIVATE character_interactive.cpp)
target_sources(dh2_level_world PRIVATE character_ai_initialization.cpp)
target_sources(dh2_level_world PRIVATE ais_faery_update_v1.cpp ais_faery_master_callbacks_v1.cpp)
target_sources(dh2_level_world PRIVATE ais_external_init_vcb.cpp)
target_sources(dh2_level_world PRIVATE ais_player_init_vcb.cpp)
target_sources(dh2_level_world PRIVATE character_skill_cooldown_services.cpp
 player_skill_timer_provider_v1.cpp player_script_timer_dispatch_v1.cpp)
target_sources(dh2_level_world PRIVATE character_player_buffs_v1.cpp)
target_sources(dh2_level_world PRIVATE player_initial_skill_grants_v1.cpp player_skill_progression_v1.cpp)
target_sources(dh2_level_world PRIVATE player_initial_equipment_v1.cpp)
target_sources(dh2_level_world PRIVATE player_ais_lifecycle_v1.cpp)
target_sources(dh2_level_world PRIVATE character_regen_tick_v1.cpp player_ai_timer_events_v1.cpp)

target_sources(dh2_level_world PRIVATE player_skill_cleanup_session_v1.cpp)

target_sources(dh2_level_world PRIVATE player_ai_death_v1.cpp character_aggro_cleanup.cpp)
target_sources(dh2_level_world PRIVATE player_kill_continuation_v1.cpp
 character_kill_quest_tail_v1.cpp)

target_sources(dh2_level_world PRIVATE character_dead_focus_services_v1.cpp)
target_sources(dh2_level_world PRIVATE character_dot_tick.cpp character_dot_attack.cpp)
target_sources(dh2_level_world PRIVATE player_skill_tables_adapter.cpp
 character_player_skills_preparation_v3.cpp character_ai_skill_script_update.cpp
 player_skill_session_v1.cpp player_skill_update_session_v1.cpp
 character_ai_skill_script_check.cpp script_value_boolean.cpp
 script_value_boolean_lua.cpp player_skill_use_session_v1.cpp
 player_skill_property_services_v1.cpp character_ai_skill_dispatch_v1.cpp
 character_ai_skill_commands_v1.cpp)
target_sources(dh2_level_world PRIVATE character_mana_services_v1.cpp)
target_sources(dh2_level_world PRIVATE character_current_spell_v1.cpp)
target_sources(dh2_level_world PRIVATE character_player_scalar_services_v1.cpp
 character_equipped_faery_element_v1.cpp)
target_sources(dh2_level_world PRIVATE character_current_equipped_faery_v1.cpp)
target_sources(dh2_level_world PRIVATE character_skill_fsm_callbacks_v1.cpp
 character_skill_state_dispatch_v1.cpp)
target_include_directories(dh2_level_world PRIVATE ../adam-script-runtime/lua)

target_sources(dh2_level_world PRIVATE ais_external_initialization.cpp ais_native_bindings.cpp room_zone_enrollment.cpp)
target_sources(dh2_level_world PRIVATE character_ai_association.cpp character_native_bindings.cpp)

# Initialization callers verified against the original ARM functions. Their
# live player, level, debug and scene providers are separate owner boundaries.
target_sources(dh2_level_world PRIVATE lua_script_level_queries.cpp
 character_script_set_level.cpp character_regeneration.cpp module_room_zone_bounds.cpp)

target_sources(dh2_level_world PRIVATE debug_switches_runtime.cpp debug_switches_persistence.cpp)
target_sources(dh2_level_world PRIVATE level_construction_fields.cpp)

target_sources(dh2_level_world PRIVATE character_level_runtime.cpp character_init_hp_mp.cpp
 ../pydata-constants/constants.c ../player-info-level/character_level_member.cpp
 ../player-info-level/player_manager_host_level.cpp ../player-info-level/player_locality_v1.cpp
 ../player-info-level/player_local_selection_v1.cpp
 ../player-info-level/input_manager_v1.cpp
 ../player-info-level/netstruct_members_v1.cpp ../player-info-level/cnet_player_info_v1.cpp
  ../player-info-level/player_info_record_v1.cpp
  ../player-info-level/player_info_activity_v1.cpp
  ../player-info-level/player_add_character_v1.cpp
  ../player-info-level/player_offline_registration_v1.cpp
  ../player-info-level/player_offline_registry_v1.cpp
  ../player-info-level/player_manager_friendly_v1.cpp
  ../player-info-level/player_metadata_prepare_v1.cpp
 player_saved_inventory_v1.cpp player_save_inventory_v1.cpp player_gear_save_writer_v1.cpp
 character_saved_class_v1.cpp character_gameplay_save_v1.cpp
 character_init_post_player_v1.cpp character_init_final_v1.cpp character_template_random.cpp
 character_template_factory.cpp character_template_catalog_v1.cpp
 character_template_runtime_v1.cpp
 crypt_template_character_projection_v1.cpp
 object_creation_map_v1.cpp character_constructor_owner_v1.cpp
 character_net_state_owner_v1.cpp
 character_runtime_factory_v1.cpp object_manager_character_factory_walk_v1.cpp
 character_factory_role_query_v1.cpp
 crypt_conditional_character_runtime_v1.cpp
 character_faery_links_v1.cpp character_faery_factory_bridge_v1.cpp
 character_init_post_nonplayer_v1.cpp
 game_object_visual_attachment_v1.cpp)
target_sources(dh2_level_world PRIVATE character_ai_update_all_skills.cpp
 character_ai_update_skills.cpp)
target_sources(dh2_level_world PRIVATE character_ai_set_skills_and_spells.cpp
 character_faery_selection.cpp character_faery_placement_v1.cpp
 character_faery_live_binding_v1.cpp character_faery_level_registry_v1.cpp
 character_faery_selection_owner_v1.cpp
 character_faery_script_session_v1.cpp
 character_faery_controller_v1.cpp
 character_faery_navigation_v1.cpp
 character_faery_record_session_v1.cpp
 character_faery_script_process_v1.cpp
 character_faery_source_physical_owner_v1.cpp
 character_faery_record_physical_v1.cpp
 character_ai_skill_script_constructor.cpp
 character_skill_state_queries.cpp)

target_sources(dh2_level_world PRIVATE ais_state_callbacks.cpp)

target_sources(dh2_level_world PRIVATE ais_external_init_callbacks.cpp)

target_sources(dh2_level_world PRIVATE character_ai_pause_update.cpp)

target_sources(dh2_level_world PRIVATE ais_default_collision_persist.cpp)
target_sources(dh2_level_world PRIVATE character_ai_events.cpp player_enemy_kill_credit_v1.cpp
 character_give_xp_v1.cpp character_level_up_properties_v1.cpp)
target_sources(dh2_level_world PRIVATE character_distribute_xp_v1.cpp
 character_distribute_give_xp_dispatch_v1.cpp
 design_settings_xp_provider_v1.cpp character_distribute_xp_player_owners_v1.cpp
 quest_reward_give_xp_bridge_v1.cpp)
target_sources(dh2_level_world PRIVATE character_attack_geometry.cpp)
target_sources(dh2_level_world PRIVATE loot_pickup_quest_tail_v10.cpp
 loot_pickup_event_bridge_v1.cpp current_level_quest_event_v1.cpp)
target_sources(dh2_level_world PRIVATE quest_move_in_zone_collision_v1.cpp
 quest_move_in_zone_sensor_owner_v1.cpp)
target_sources(dh2_level_world PRIVATE open_game_object_objective_v1.cpp)
target_sources(dh2_level_world PRIVATE game_object_position_owner_v1.cpp
 player_sct_position_provider_v1.cpp)
target_sources(dh2_level_world PRIVATE ais_combat_result_dispatch_v1.cpp)
target_include_directories(dh2_level_world PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_options(dh2_level_world PRIVATE -Wall -Wextra -fno-fast-math -ffp-contract=off)
target_link_libraries(dh2_level_world PUBLIC dh2_engine_skinning dh2_game_data dh2_box2d_201 PRIVATE dh2_script_runtime)
if(ANDROID)
 target_link_options(dh2_level_world PRIVATE -Wl,-z,max-page-size=16384 -Wl,--no-undefined)
else()
 add_executable(source_level_owner_v1_audit tests/source_level_owner_v1.cpp)
 target_link_libraries(source_level_owner_v1_audit PRIVATE dh2_level_world dh2_game_data)
 target_compile_features(source_level_owner_v1_audit PRIVATE cxx_std_17)
 target_compile_options(source_level_owner_v1_audit PRIVATE -Wall -Wextra -Werror)
 if(NOT TARGET dh2_random_level)
  add_subdirectory(../random-level "${CMAKE_CURRENT_BINARY_DIR}/random-level")
 endif()
 add_executable(source_handle_ledger_v1_audit
  tests/source_handle_ledger_v1.cpp source_handle_ledger_v1.cpp
  crypt_generated_source_dact_v1.cpp crypt_generated_mvp_v1.cpp
  ../random-level/crypt_generated_dact_v1.cpp
  ../world-data/world.cpp ../game-data/data.cpp)
 target_link_libraries(source_handle_ledger_v1_audit PRIVATE dh2_random_level dh2_game_data dh2_level_world)
 target_compile_features(source_handle_ledger_v1_audit PRIVATE cxx_std_17)
 target_compile_options(source_handle_ledger_v1_audit PRIVATE -Wall -Wextra -Werror -fno-fast-math -ffp-contract=off)
add_executable(crypt_generated_template_character_source_v1_audit
 tests/crypt_generated_template_character_source_v1.cpp)
 target_link_libraries(crypt_generated_template_character_source_v1_audit
  PRIVATE dh2_level_world dh2_game_data)
 target_compile_features(crypt_generated_template_character_source_v1_audit
  PRIVATE cxx_std_17)
target_compile_options(crypt_generated_template_character_source_v1_audit
 PRIVATE -Wall -Wextra -Werror -fno-fast-math -ffp-contract=off)
add_executable(character_template_runtime_v1_audit
 tests/character_template_runtime_v1.cpp)
target_link_libraries(character_template_runtime_v1_audit
 PRIVATE dh2_level_world dh2_game_data)
target_compile_features(character_template_runtime_v1_audit PRIVATE cxx_std_17)
target_compile_options(character_template_runtime_v1_audit
 PRIVATE -UNDEBUG -Wall -Wextra -Werror -fno-fast-math -ffp-contract=off)
add_executable(object_manager_runtime_owner_v1_audit
  tests/object_manager_runtime_owner_v1.cpp object_manager_runtime_owner_v1.cpp)
 target_compile_features(object_manager_runtime_owner_v1_audit PRIVATE cxx_std_17)
 target_compile_options(object_manager_runtime_owner_v1_audit PRIVATE -UNDEBUG -Wall -Wextra -Werror -fno-fast-math -ffp-contract=off)
 add_executable(projectile_lifecycle_runtime_v1_audit
  tests/projectile_lifecycle_runtime_v1.cpp projectile_lifecycle_runtime_v1.cpp
  projectile_factory_owner_v1.cpp object_creation_map_v1.cpp
  object_manager_runtime_owner_v1.cpp)
 target_link_libraries(projectile_lifecycle_runtime_v1_audit PRIVATE dh2_game_data)
 target_compile_features(projectile_lifecycle_runtime_v1_audit PRIVATE cxx_std_17)
 target_compile_options(projectile_lifecycle_runtime_v1_audit PRIVATE -UNDEBUG -Wall -Wextra -Werror -fno-fast-math -ffp-contract=off)
 add_executable(object_manager_init_post_v1_audit
  tests/object_manager_init_post_v1.cpp object_manager_init_post_v1.cpp
  object_manager_runtime_owner_v1.cpp)
 target_compile_features(object_manager_init_post_v1_audit PRIVATE cxx_std_17)
 target_compile_options(object_manager_init_post_v1_audit PRIVATE -UNDEBUG -Wall -Wextra -Werror -fno-fast-math -ffp-contract=off)
 add_executable(character_constructor_owner_v1_audit
  tests/character_constructor_owner_v1.cpp)
 target_link_libraries(character_constructor_owner_v1_audit PRIVATE dh2_level_world)
 target_compile_features(character_constructor_owner_v1_audit PRIVATE cxx_std_17)
 target_compile_options(character_constructor_owner_v1_audit PRIVATE -Wall -Wextra -Werror)
 add_executable(character_runtime_factory_v1_audit
  tests/character_runtime_factory_v1.cpp
  ../android-native/app/src/main/cpp/native_character_list.cpp)
 target_link_libraries(character_runtime_factory_v1_audit PRIVATE dh2_level_world)
 target_compile_features(character_runtime_factory_v1_audit PRIVATE cxx_std_17)
 target_compile_options(character_runtime_factory_v1_audit PRIVATE -Wall -Wextra -Werror)
 add_executable(object_manager_character_factory_walk_v1_audit
  tests/object_manager_character_factory_walk_v1.cpp)
 target_link_libraries(object_manager_character_factory_walk_v1_audit PRIVATE dh2_level_world)
 target_compile_features(object_manager_character_factory_walk_v1_audit PRIVATE cxx_std_17)
 target_compile_options(object_manager_character_factory_walk_v1_audit PRIVATE -Wall -Wextra -Werror)
 add_executable(character_faery_links_v1_audit
  tests/character_faery_links_v1.cpp)
 target_link_libraries(character_faery_links_v1_audit PRIVATE dh2_level_world)
 target_compile_features(character_faery_links_v1_audit PRIVATE cxx_std_17)
 target_compile_options(character_faery_links_v1_audit PRIVATE -Wall -Wextra -Werror)
 add_executable(character_faery_factory_bridge_v1_audit
  tests/character_faery_factory_bridge_v1.cpp)
 target_link_libraries(character_faery_factory_bridge_v1_audit PRIVATE dh2_level_world)
 target_compile_features(character_faery_factory_bridge_v1_audit PRIVATE cxx_std_17)
 target_compile_options(character_faery_factory_bridge_v1_audit PRIVATE -Wall -Wextra -Werror)
 add_executable(character_faery_live_binding_v1_audit
  tests/character_faery_live_binding_v1.cpp)
 target_link_libraries(character_faery_live_binding_v1_audit PRIVATE dh2_level_world)
 target_compile_features(character_faery_live_binding_v1_audit PRIVATE cxx_std_17)
 target_compile_options(character_faery_live_binding_v1_audit PRIVATE -Wall -Wextra -Werror)
 add_executable(character_faery_level_registry_v1_audit
  tests/character_faery_level_registry_v1.cpp)
 target_link_libraries(character_faery_level_registry_v1_audit PRIVATE dh2_level_world)
 target_compile_features(character_faery_level_registry_v1_audit PRIVATE cxx_std_17)
 target_compile_options(character_faery_level_registry_v1_audit PRIVATE -Wall -Wextra -Werror)
 add_executable(character_faery_script_session_v1_audit
  tests/character_faery_script_session_v1.cpp)
 target_link_libraries(character_faery_script_session_v1_audit PRIVATE dh2_level_world)
 target_compile_features(character_faery_script_session_v1_audit PRIVATE cxx_std_17)
 target_compile_options(character_faery_script_session_v1_audit PRIVATE -Wall -Wextra -Werror)

 add_executable(character_faery_record_session_v1_audit
  tests/character_faery_record_session_v1.cpp)
 target_link_libraries(character_faery_record_session_v1_audit PRIVATE dh2_level_world)
 target_compile_features(character_faery_record_session_v1_audit PRIVATE cxx_std_17)
 target_compile_options(character_faery_record_session_v1_audit PRIVATE -Wall -Wextra -Werror)
 add_executable(character_faery_script_process_v1_audit
  tests/character_faery_script_process_v1.cpp)
 target_link_libraries(character_faery_script_process_v1_audit PRIVATE dh2_level_world)
 target_compile_features(character_faery_script_process_v1_audit PRIVATE cxx_std_17)
 target_compile_options(character_faery_script_process_v1_audit PRIVATE -Wall -Wextra -Werror)

 add_executable(character_faery_record_physical_v1_audit
  tests/character_faery_record_physical_v1.cpp)
 target_link_libraries(character_faery_record_physical_v1_audit PRIVATE dh2_level_world)
 target_compile_features(character_faery_record_physical_v1_audit PRIVATE cxx_std_17)
 target_compile_options(character_faery_record_physical_v1_audit PRIVATE -UNDEBUG -Wall -Wextra -Werror)
 add_executable(character_faery_selection_owner_v1_audit
  tests/character_faery_selection_owner_v1.cpp)
 target_link_libraries(character_faery_selection_owner_v1_audit PRIVATE dh2_level_world)
 target_compile_features(character_faery_selection_owner_v1_audit PRIVATE cxx_std_17)
 target_compile_options(character_faery_selection_owner_v1_audit PRIVATE -UNDEBUG -Wall -Wextra -Werror)
 add_executable(ais_faery_update_v1_audit
  tests/ais_faery_update_v1.cpp)
 target_link_libraries(ais_faery_update_v1_audit PRIVATE dh2_level_world)
 target_compile_features(ais_faery_update_v1_audit PRIVATE cxx_std_17)
 target_compile_options(ais_faery_update_v1_audit PRIVATE -Wall -Wextra -Werror)
 add_executable(ais_faery_master_callbacks_v1_audit
  tests/ais_faery_master_callbacks_v1.cpp)
 target_link_libraries(ais_faery_master_callbacks_v1_audit PRIVATE dh2_level_world)
 target_compile_features(ais_faery_master_callbacks_v1_audit PRIVATE cxx_std_17)
 target_compile_options(ais_faery_master_callbacks_v1_audit PRIVATE -Wall -Wextra -Werror -fno-fast-math -ffp-contract=off)
 add_executable(source_spawnpoints_adapter_audit
  tests/source_spawnpoints_adapter.cpp ../world-data/world.cpp ../world-data/source_layout_adapter.cpp)
 target_link_libraries(source_spawnpoints_adapter_audit PRIVATE dh2_game_data)
 target_compile_features(source_spawnpoints_adapter_audit PRIVATE cxx_std_17)
 target_compile_options(source_spawnpoints_adapter_audit PRIVATE -Wall -Wextra -Werror -fno-fast-math -ffp-contract=off)
 add_executable(source_dact_adapter_audit tests/source_dact_adapter.cpp)
 target_link_libraries(source_dact_adapter_audit PRIVATE dh2_level_world)
 target_compile_features(source_dact_adapter_audit PRIVATE cxx_std_17)
 target_compile_options(source_dact_adapter_audit PRIVATE -Wall -Wextra -Werror -fno-fast-math -ffp-contract=off)
 add_executable(source_mvp_adapter_audit tests/source_mvp_adapter.cpp)
 target_link_libraries(source_mvp_adapter_audit PRIVATE dh2_level_world)
 target_compile_features(source_mvp_adapter_audit PRIVATE cxx_std_17)
 target_compile_options(source_mvp_adapter_audit PRIVATE -Wall -Wextra -Werror -fno-fast-math -ffp-contract=off)
 add_executable(character_init_post_player_v1_audit
  tests/character_init_post_player_v1.cpp character_init_post_player_v1.cpp)
 target_compile_features(character_init_post_player_v1_audit PRIVATE cxx_std_17)
 target_compile_options(character_init_post_player_v1_audit PRIVATE -Wall -Wextra -Werror)
 add_executable(character_init_spawned_v1_audit tests/character_init_spawned_v1.cpp)
 target_link_libraries(character_init_spawned_v1_audit PRIVATE dh2_level_world)
 target_compile_features(character_init_spawned_v1_audit PRIVATE cxx_std_17)
 target_compile_options(character_init_spawned_v1_audit PRIVATE -Wall -Wextra -Werror)
 add_executable(spawn_group_runtime_v1_audit tests/spawn_group_runtime_v1.cpp)
 target_link_libraries(spawn_group_runtime_v1_audit PRIVATE dh2_level_world)
 target_compile_features(spawn_group_runtime_v1_audit PRIVATE cxx_std_17)
 target_compile_options(spawn_group_runtime_v1_audit PRIVATE -Wall -Wextra -Werror)
 add_executable(character_ai_queue_owner_v1_audit tests/character_ai_queue_owner_v1.cpp)
 target_link_libraries(character_ai_queue_owner_v1_audit PRIVATE dh2_level_world)
 target_compile_features(character_ai_queue_owner_v1_audit PRIVATE cxx_std_17)
 target_compile_options(character_ai_queue_owner_v1_audit PRIVATE -Wall -Wextra -Werror -fno-fast-math -ffp-contract=off)
 add_executable(source_random_lifecycle_v1_audit tests/source_random_lifecycle_v1.cpp)
 target_link_libraries(source_random_lifecycle_v1_audit PRIVATE dh2_level_world)
 add_executable(player_gear_save_writer_v1_audit tests/player_gear_save_writer_v1.cpp)
 target_link_libraries(player_gear_save_writer_v1_audit PRIVATE dh2_level_world)
 target_compile_features(player_gear_save_writer_v1_audit PRIVATE cxx_std_17)
 target_compile_options(player_gear_save_writer_v1_audit PRIVATE -Wall -Wextra -Werror -fno-fast-math -ffp-contract=off)
 add_executable(player_save_inventory_v1_audit
  tests/player_save_inventory_v1.cpp)
 target_link_libraries(player_save_inventory_v1_audit PRIVATE dh2_level_world)
 target_compile_features(player_save_inventory_v1_audit PRIVATE cxx_std_17)
 target_compile_options(player_save_inventory_v1_audit PRIVATE
  -Wall -Wextra -Werror -fno-fast-math -ffp-contract=off)
 add_executable(player_skill_property_services_v1_audit tests/player_skill_property_services_v1.cpp)
 target_link_libraries(player_skill_property_services_v1_audit PRIVATE dh2_level_world)
 add_executable(objects_spawn_audit tests/objects_spawn.cpp)
 target_link_libraries(objects_spawn_audit PRIVATE dh2_level_world)
 add_executable(character_coordinator_audit tests/character_coordinator.cpp)
 target_link_libraries(character_coordinator_audit PRIVATE dh2_level_world)
 add_executable(character_cast_coordinator_v1_audit
  tests/character_cast_coordinator_v1.cpp)
 target_link_libraries(character_cast_coordinator_v1_audit PRIVATE dh2_level_world)
 target_compile_features(character_cast_coordinator_v1_audit PRIVATE cxx_std_17)
 target_compile_options(character_cast_coordinator_v1_audit PRIVATE
  -Wall -Wextra -Werror -fno-fast-math -ffp-contract=off)
 add_executable(native_player_character_owner_v1_audit tests/native_player_character_owner_v1.cpp)
 target_link_libraries(native_player_character_owner_v1_audit PRIVATE dh2_level_world)
 add_executable(character_spawn_audit tests/character_spawn.cpp)
 target_link_libraries(character_spawn_audit PRIVATE dh2_level_world)
 add_executable(character_attack_geometry_audit tests/character_attack_geometry.cpp)
 target_link_libraries(character_attack_geometry_audit PRIVATE dh2_level_world)
 add_executable(ais_combat_result_dispatch_v1_audit
  tests/ais_combat_result_dispatch_v1.cpp ais_combat_result_dispatch_v1.cpp)
 target_link_libraries(ais_combat_result_dispatch_v1_audit PRIVATE dh2_game_data)
 target_compile_features(ais_combat_result_dispatch_v1_audit PRIVATE cxx_std_17)
 target_compile_options(ais_combat_result_dispatch_v1_audit PRIVATE -UNDEBUG -Wall -Wextra -Werror -fno-fast-math -ffp-contract=off)
 add_executable(character_ai_events_audit tests/character_ai_events.cpp)
 target_link_libraries(character_ai_events_audit PRIVATE dh2_level_world ${CMAKE_DL_LIBS})
 add_executable(character_ai_event_zero_equivalence_audit
  tests/character_ai_event_zero_equivalence.cpp)
 target_link_libraries(character_ai_event_zero_equivalence_audit PRIVATE dh2_level_world)
 target_compile_features(character_ai_event_zero_equivalence_audit PRIVATE cxx_std_17)
 target_compile_options(character_ai_event_zero_equivalence_audit PRIVATE -Wall -Wextra -Werror)
 add_executable(player_enemy_kill_credit_v1_audit tests/player_enemy_kill_credit_v1.cpp)
 target_link_libraries(player_enemy_kill_credit_v1_audit PRIVATE dh2_level_world)
 add_executable(character_kill_quest_tail_v1_audit tests/character_kill_quest_tail_v1.cpp)
 target_link_libraries(character_kill_quest_tail_v1_audit PRIVATE dh2_level_world)
 add_executable(character_kill_death_tail_v1_audit tests/character_kill_death_tail_v1.cpp)
 target_link_libraries(character_kill_death_tail_v1_audit PRIVATE dh2_level_world)
 add_executable(character_kill_field_14e4_v1_audit
  tests/character_kill_field_14e4_v1.cpp)
 target_link_libraries(character_kill_field_14e4_v1_audit PRIVATE dh2_level_world)
 add_executable(character_kill_ctrl_composition_v1_audit
  tests/character_kill_ctrl_composition_v1.cpp)
 target_link_libraries(character_kill_ctrl_composition_v1_audit PRIVATE dh2_level_world)
 add_executable(character_give_xp_v1_audit tests/character_give_xp_v1.cpp)
 target_link_libraries(character_give_xp_v1_audit PRIVATE dh2_level_world)
 add_executable(quest_reward_give_xp_bridge_v1_audit
  tests/quest_reward_give_xp_bridge_v1.cpp)
 target_link_libraries(quest_reward_give_xp_bridge_v1_audit PRIVATE dh2_level_world)
 add_executable(character_distribute_xp_v1_audit tests/character_distribute_xp_v1.cpp)
 target_link_libraries(character_distribute_xp_v1_audit PRIVATE dh2_level_world)
 add_executable(design_settings_xp_provider_v1_audit tests/design_settings_xp_provider_v1.cpp)
 target_link_libraries(design_settings_xp_provider_v1_audit PRIVATE dh2_level_world)
 add_executable(character_distribute_xp_player_owners_v1_audit
  tests/character_distribute_xp_player_owners_v1.cpp)
 target_link_libraries(character_distribute_xp_player_owners_v1_audit PRIVATE dh2_level_world)
 add_executable(character_script_timers_audit tests/character_script_timers.cpp)
 target_link_libraries(character_script_timers_audit PRIVATE dh2_level_world dh2_script_runtime)
 add_executable(character_controller_commands_audit tests/character_controller_commands.cpp)
 target_link_libraries(character_controller_commands_audit PRIVATE dh2_level_world)
 add_executable(character_ai_update_audit tests/character_ai_update.cpp)
 target_link_libraries(character_ai_update_audit PRIVATE dh2_level_world)
 add_executable(character_ai_update_skills_audit tests/character_ai_update_skills.cpp)
 target_link_libraries(character_ai_update_skills_audit PRIVATE dh2_level_world)
 add_executable(character_ai_state_audit tests/character_ai_state.cpp)
 target_link_libraries(character_ai_state_audit PRIVATE dh2_level_world)
 add_executable(character_ai_attack_audit tests/character_ai_attack.cpp)
 target_link_libraries(character_ai_attack_audit PRIVATE dh2_level_world)
 add_executable(character_path_commands_audit tests/character_path_commands.cpp)
 target_link_libraries(character_path_commands_audit PRIVATE dh2_level_world)
 add_executable(character_ai_frame_audit tests/character_ai_frame.cpp)
 target_link_libraries(character_ai_frame_audit PRIVATE dh2_level_world ${CMAKE_DL_LIBS})
 add_executable(character_animation_ai_audit tests/character_animation_ai.cpp)
 target_link_libraries(character_animation_ai_audit PRIVATE dh2_level_world)
 add_executable(character_enemy_pursuit_v1_audit tests/character_enemy_pursuit_v1.cpp)
 target_link_libraries(character_enemy_pursuit_v1_audit PRIVATE dh2_level_world)
 target_compile_features(character_enemy_pursuit_v1_audit PRIVATE cxx_std_17)
 target_compile_options(character_enemy_pursuit_v1_audit PRIVATE -Wall -Wextra -Werror)
 add_executable(character_enemy_attack_event_v1_audit tests/character_enemy_attack_event_v1.cpp)
 target_link_libraries(character_enemy_attack_event_v1_audit PRIVATE dh2_level_world)
 target_compile_features(character_enemy_attack_event_v1_audit PRIVATE cxx_std_17)
 target_compile_options(character_enemy_attack_event_v1_audit PRIVATE -Wall -Wextra -Werror)
 add_executable(character_target_out_of_sight_v1_audit tests/character_target_out_of_sight_v1.cpp)
 target_link_libraries(character_target_out_of_sight_v1_audit PRIVATE dh2_level_world)
 target_compile_features(character_target_out_of_sight_v1_audit PRIVATE cxx_std_17)
 target_compile_options(character_target_out_of_sight_v1_audit PRIVATE -Wall -Wextra -Werror)
 add_executable(player_attack_step_v1_audit tests/player_attack_step_v1.cpp)
 target_link_libraries(player_attack_step_v1_audit PRIVATE dh2_level_world)
 add_executable(character_script_lifecycle_audit tests/character_script_lifecycle.cpp)
 target_link_libraries(character_script_lifecycle_audit PRIVATE dh2_level_world)
 add_executable(character_script_update_audit tests/character_script_update.cpp)
 target_link_libraries(character_script_update_audit PRIVATE dh2_level_world)
 add_executable(character_script_collision_audit tests/character_script_collision.cpp)
 target_link_libraries(character_script_collision_audit PRIVATE dh2_level_world)
 add_executable(character_combat_queries_audit tests/character_combat_queries.cpp)
 target_link_libraries(character_combat_queries_audit PRIVATE dh2_level_world)
 add_executable(character_combat_event_v1_audit tests/character_combat_event_v1.cpp)
 target_link_libraries(character_combat_event_v1_audit PRIVATE dh2_level_world)
 target_compile_features(character_combat_event_v1_audit PRIVATE cxx_std_17)
 target_compile_options(character_combat_event_v1_audit PRIVATE -Wall -Wextra -Werror)
 add_executable(items_audit ../game-data/tests/items.cpp)
 target_link_libraries(items_audit PRIVATE dh2_game_data dh2_level_world)
 add_executable(character_script_selection_audit tests/character_script_selection.cpp character_script_selection.cpp)
 target_compile_features(character_script_selection_audit PRIVATE cxx_std_17)
 target_compile_options(character_script_selection_audit PRIVATE -Wall -Wextra -fno-builtin -fno-fast-math -ffp-contract=off)
 add_executable(actor_blended_playback_audit tests/actor_blended_playback.cpp)
 target_link_libraries(actor_blended_playback_audit PRIVATE dh2_level_world)
 add_executable(actor_blended_scheduler_audit tests/actor_blended_scheduler.cpp)
 target_link_libraries(actor_blended_scheduler_audit PRIVATE dh2_level_world)
 add_executable(actor_blended_selection_audit tests/actor_blended_selection.cpp)
 target_link_libraries(actor_blended_selection_audit PRIVATE dh2_level_world)
 add_executable(actor_blended_controls_audit tests/actor_blended_controls.cpp)
 target_link_libraries(actor_blended_controls_audit PRIVATE dh2_level_world)
 add_executable(character_animation_events_audit tests/character_animation_events.cpp character_animation_events.cpp)
 target_compile_features(character_animation_events_audit PRIVATE cxx_std_17)
 target_compile_options(character_animation_events_audit PRIVATE -Wall -Wextra -fno-fast-math -ffp-contract=off)
 add_executable(character_timers_audit tests/character_timers.cpp)
 target_link_libraries(character_timers_audit PRIVATE dh2_level_world)
 add_executable(character_stance_audit tests/character_stance.cpp)
 target_link_libraries(character_stance_audit PRIVATE dh2_level_world)
 add_executable(character_state_timers_audit tests/character_state_timers.cpp)
 target_link_libraries(character_state_timers_audit PRIVATE dh2_level_world)
 add_executable(character_state_audit tests/character_state.cpp)
 target_link_libraries(character_state_audit PRIVATE dh2_level_world)
 add_executable(actor_playback_events_audit tests/actor_playback_events.cpp)
 target_link_libraries(actor_playback_events_audit PRIVATE dh2_level_world)
 add_executable(animation_blender_audit tests/animation_blender.cpp)
 target_link_libraries(animation_blender_audit PRIVATE dh2_level_world)
 add_executable(actor_rotation_audit tests/actor_rotation.cpp)
 target_link_libraries(actor_rotation_audit PRIVATE dh2_level_world)
 add_executable(actor_runtime_audit tests/actor_runtime.cpp)
 target_link_libraries(actor_runtime_audit PRIVATE dh2_level_world)
 add_executable(actor_playback_audit tests/actor_playback.cpp)
 target_link_libraries(actor_playback_audit PRIVATE dh2_level_world)
 add_executable(decor_scene_audit tests/decor_scene.cpp)
 target_link_libraries(decor_scene_audit PRIVATE dh2_level_world)
 add_executable(character_scene_audit tests/character_scene.cpp)
 target_link_libraries(character_scene_audit PRIVATE dh2_level_world)
 add_executable(move_state_audit tests/move_state.cpp)
 target_link_libraries(move_state_audit PRIVATE dh2_level_world)
 add_executable(player_root_motion_policy_v1_audit tests/player_root_motion_policy_v1.cpp)
 target_link_libraries(player_root_motion_policy_v1_audit PRIVATE dh2_level_world)
 add_executable(decor_body_config_audit tests/decor_body_config.cpp)
 target_link_libraries(decor_body_config_audit PRIVATE dh2_level_world)
 add_executable(visual_timeline_audit tests/visual_timeline.cpp)
 target_link_libraries(visual_timeline_audit PRIVATE dh2_level_world)
 add_executable(visual_timeline_replay_audit tests/visual_timeline_replay.cpp)
 target_link_libraries(visual_timeline_replay_audit PRIVATE dh2_level_world)
 add_executable(physical_world_audit tests/physical_world.cpp)
 target_link_libraries(physical_world_audit PRIVATE dh2_level_world)
 add_executable(physical_lifecycle_audit tests/physical_lifecycle.cpp)
 target_link_libraries(physical_lifecycle_audit PRIVATE dh2_level_world)
 add_executable(native_body_audit tests/native_body.cpp)
 target_link_libraries(native_body_audit PRIVATE dh2_level_world)
 add_executable(native_body_replay_audit tests/native_body_replay.cpp)
 target_link_libraries(native_body_replay_audit PRIVATE dh2_level_world)
 add_executable(visual_motion_audit tests/visual_motion.cpp)
 target_link_libraries(visual_motion_audit PRIVATE dh2_level_world)
 add_executable(visual_motion_assets_audit tests/visual_motion_assets.cpp)
 target_link_libraries(visual_motion_assets_audit PRIVATE dh2_level_world)
 add_executable(physical_controls_audit tests/physical_controls.cpp)
 target_link_libraries(physical_controls_audit PRIVATE dh2_level_world)
 add_executable(body_transform_audit tests/body_transform.cpp)
 target_link_libraries(body_transform_audit PRIVATE dh2_level_world)
 add_executable(subobjects_update_audit tests/subobjects_update.cpp)
 target_link_libraries(subobjects_update_audit PRIVATE dh2_level_world)
 add_executable(controller_physical_audit tests/controller_physical.cpp)
 target_link_libraries(controller_physical_audit PRIVATE dh2_level_world)
 add_executable(character_body_config_audit tests/character_body_config.cpp)
 target_link_libraries(character_body_config_audit PRIVATE dh2_level_world)
 add_executable(character_physical_component_v1_audit tests/character_physical_component_v1.cpp)
 target_link_libraries(character_physical_component_v1_audit PRIVATE dh2_level_world)
 add_executable(navigation_controller_audit tests/navigation_controller.cpp)
 target_link_libraries(navigation_controller_audit PRIVATE dh2_level_world)
 add_executable(navigation_heading_audit tests/navigation_heading.cpp)
 target_link_libraries(navigation_heading_audit PRIVATE dh2_level_world)
 add_executable(navigation_producers_audit tests/navigation_producers.cpp)
 target_link_libraries(navigation_producers_audit PRIVATE dh2_level_world)
 add_executable(navigation_avoidance_audit tests/navigation_avoidance.cpp)
 target_link_libraries(navigation_avoidance_audit PRIVATE dh2_level_world)
 add_executable(navigation_objects_audit tests/navigation_objects.cpp)
 target_link_libraries(navigation_objects_audit PRIVATE dh2_level_world)
 add_executable(navigation_motion_audit tests/navigation_motion.cpp)
 target_link_libraries(navigation_motion_audit PRIVATE dh2_level_world)
 add_executable(navigation_find_audit tests/navigation_find.cpp)
 target_link_libraries(navigation_find_audit PRIVATE dh2_level_world)
 add_executable(navigation_path_audit tests/navigation_path.cpp)
 target_link_libraries(navigation_path_audit PRIVATE dh2_level_world)
 add_executable(navigation_world_audit tests/navigation_world.cpp)
 target_link_libraries(navigation_world_audit PRIVATE dh2_level_world)
 add_executable(navigation_search_audit tests/navigation_search.cpp)
 target_link_libraries(navigation_search_audit PRIVATE dh2_level_world)
 add_executable(navigation_link_audit tests/navigation_link.cpp)
 target_link_libraries(navigation_link_audit PRIVATE dh2_level_world)
 add_executable(inspect_floors tools/inspect_floors.cpp)
 target_link_libraries(inspect_floors PRIVATE dh2_level_world)
 add_executable(floor_source_audit tests/floor_source.cpp)
 target_link_libraries(floor_source_audit PRIVATE dh2_level_world)
 add_executable(navigation_audit tests/navigation.cpp)
 target_link_libraries(navigation_audit PRIVATE dh2_level_world)
 add_executable(collision_audit tests/collision.cpp)
 target_link_libraries(collision_audit PRIVATE dh2_level_world)
 add_executable(octree_audit tests/octree.cpp)
 target_link_libraries(octree_audit PRIVATE dh2_level_world)
 add_executable(selector_audit tests/selector.cpp)
 target_link_libraries(selector_audit PRIVATE dh2_level_world)
 add_executable(world_audit tests/audit.cpp)
 target_link_libraries(world_audit PRIVATE dh2_level_world)
 add_executable(world_spawnpoints_audit tests/world_spawnpoints.cpp)
 target_link_libraries(world_spawnpoints_audit PRIVATE dh2_level_world)
 add_executable(swamp_static_world_audit tests/swamp_static_world.cpp)
 target_link_libraries(swamp_static_world_audit PRIVATE dh2_level_world)
 add_executable(inspect_world tools/inspect_world.cpp)
 target_link_libraries(inspect_world PRIVATE dh2_level_world)
 add_executable(locomotion_audit tests/locomotion.cpp)
 target_link_libraries(locomotion_audit PRIVATE dh2_engine_skinning)
 add_executable(inspect_animation tools/inspect_animation.cpp)
 target_link_libraries(inspect_animation PRIVATE dh2_engine_animation)
 add_executable(inspect_scene tools/inspect_scene.cpp)
 target_link_libraries(inspect_scene PRIVATE dh2_scene_materials)
 add_executable(objects_audit tests/objects.cpp)
 target_link_libraries(objects_audit PRIVATE dh2_level_world)
 add_executable(object_instance_material_override_host tests/object_instance_material_override.cpp)
 target_link_libraries(object_instance_material_override_host PRIVATE dh2_level_world)
 add_executable(idle_options_audit tests/idle_options.cpp)
 target_link_libraries(idle_options_audit PRIVATE dh2_level_world)
 add_executable(actor_clips_audit tests/actor_clips.cpp)
 target_link_libraries(actor_clips_audit PRIVATE dh2_level_world)
 add_executable(inspect_pose tools/inspect_pose.cpp)
 target_link_libraries(inspect_pose PRIVATE dh2_level_world)
 add_executable(player_combat_audit tests/player_combat.cpp)
 target_link_libraries(player_combat_audit PRIVATE dh2_level_world)
 add_executable(loot_pickup_quest_tail_v10_audit
  tests/loot_pickup_quest_tail_v10.cpp loot_pickup_quest_tail_v10.cpp)
 target_compile_features(loot_pickup_quest_tail_v10_audit PRIVATE cxx_std_17)
 target_compile_options(loot_pickup_quest_tail_v10_audit PRIVATE
  -Wall -Wextra -Werror -fno-fast-math -ffp-contract=off)
 add_executable(current_level_quest_event_v1_audit
  tests/current_level_quest_event_v1.cpp current_level_quest_event_v1.cpp)
 target_link_libraries(current_level_quest_event_v1_audit PRIVATE dh2_game_data)
 target_compile_features(current_level_quest_event_v1_audit PRIVATE cxx_std_17)
 target_compile_options(current_level_quest_event_v1_audit PRIVATE
  -Wall -Wextra -Werror -fno-fast-math -ffp-contract=off)
 add_executable(quest_move_in_zone_collision_v1_audit
  tests/quest_move_in_zone_collision_v1.cpp)
 target_link_libraries(quest_move_in_zone_collision_v1_audit PRIVATE dh2_level_world)
 target_compile_features(quest_move_in_zone_collision_v1_audit PRIVATE cxx_std_17)
 target_compile_options(quest_move_in_zone_collision_v1_audit PRIVATE
  -Wall -Wextra -Werror -fno-fast-math -ffp-contract=off)
 add_executable(quest_move_in_zone_sensor_owner_v1_audit
  tests/quest_move_in_zone_sensor_owner_v1.cpp)
 target_link_libraries(quest_move_in_zone_sensor_owner_v1_audit PRIVATE dh2_level_world)
 target_compile_features(quest_move_in_zone_sensor_owner_v1_audit PRIVATE cxx_std_17)
 target_compile_options(quest_move_in_zone_sensor_owner_v1_audit PRIVATE
  -Wall -Wextra -Werror -fno-fast-math -ffp-contract=off)
 add_executable(loot_pickup_event_bridge_v1_audit
  tests/loot_pickup_event_bridge_v1.cpp)
 target_link_libraries(loot_pickup_event_bridge_v1_audit PRIVATE dh2_level_world)
 target_compile_features(loot_pickup_event_bridge_v1_audit PRIVATE cxx_std_17)
 add_executable(loot_pickup_generated_qest_v1_audit
  tests/loot_pickup_generated_qest_v1.cpp)
 target_link_libraries(loot_pickup_generated_qest_v1_audit PRIVATE dh2_level_world)
 target_compile_features(loot_pickup_generated_qest_v1_audit PRIVATE cxx_std_17)
 target_compile_options(loot_pickup_event_bridge_v1_audit PRIVATE
  -Wall -Wextra -Werror -fno-fast-math -ffp-contract=off)
 add_executable(player_camera_rig_v1_audit tests/player_camera_rig_v1.cpp)
 target_link_libraries(player_camera_rig_v1_audit PRIVATE dh2_level_world)
 target_compile_features(player_camera_rig_v1_audit PRIVATE cxx_std_17)
 target_compile_options(player_camera_rig_v1_audit PRIVATE -Wall -Wextra -Werror -fno-fast-math -ffp-contract=off)
 add_executable(camera_animset_v1_audit tests/camera_animset_v1.cpp)
 target_link_libraries(camera_animset_v1_audit PRIVATE dh2_level_world dh2_game_data)
 target_compile_features(camera_animset_v1_audit PRIVATE cxx_std_17)
 target_compile_options(camera_animset_v1_audit PRIVATE -Wall -Wextra -Werror -fno-fast-math -ffp-contract=off)
 add_executable(camera_design_zoom_v1_audit
  tests/camera_design_zoom_v1_host.cpp camera_design_zoom_v1.cpp)
 target_compile_features(camera_design_zoom_v1_audit PRIVATE cxx_std_17)
 target_compile_options(camera_design_zoom_v1_audit PRIVATE -Wall -Wextra -Werror -fno-fast-math -ffp-contract=off)
 add_executable(character_oid_cache_v1_audit
  tests/character_oid_cache_v1.cpp character_oid_cache_v1.cpp)
 target_compile_features(character_oid_cache_v1_audit PRIVATE cxx_std_17)
 target_compile_options(character_oid_cache_v1_audit PRIVATE
  -Wall -Wextra -Werror -fno-fast-math -ffp-contract=off)
 add_executable(game_object_position_owner_v1_audit
  tests/game_object_position_owner_v1.cpp game_object_position_owner_v1.cpp)
 target_compile_features(game_object_position_owner_v1_audit PRIVATE cxx_std_17)
 target_compile_options(game_object_position_owner_v1_audit PRIVATE
  -Wall -Wextra -Werror -fno-fast-math -ffp-contract=off)
 add_executable(level_savegame_owner_v1_audit
  tests/level_savegame_owner_v1.cpp level_savegame_owner_v1.cpp)
 target_compile_features(level_savegame_owner_v1_audit PRIVATE cxx_std_17)
 target_compile_options(level_savegame_owner_v1_audit PRIVATE -Wall -Wextra -Werror)
 add_executable(character_net_state_owner_v1_audit
  tests/character_net_state_owner_v1.cpp character_net_state_owner_v1.cpp)
 target_compile_features(character_net_state_owner_v1_audit PRIVATE cxx_std_17)
 target_compile_options(character_net_state_owner_v1_audit PRIVATE
  -Wall -Wextra -Werror -fno-fast-math -ffp-contract=off)
 add_executable(game_object_visual_attachment_v1_audit
  tests/game_object_visual_attachment_v1.cpp game_object_visual_attachment_v1.cpp)
 target_compile_features(game_object_visual_attachment_v1_audit PRIVATE cxx_std_17)
 target_compile_options(game_object_visual_attachment_v1_audit PRIVATE
  -Wall -Wextra -Werror -fno-fast-math -ffp-contract=off)
 add_executable(character_interaction_spot_owner_v1_audit
  tests/character_interaction_spot_owner_v1.cpp character_interaction_spot_owner_v1.cpp)
 target_compile_features(character_interaction_spot_owner_v1_audit PRIVATE cxx_std_17)
 target_compile_options(character_interaction_spot_owner_v1_audit PRIVATE
  -Wall -Wextra -Werror -fno-fast-math -ffp-contract=off)
 add_executable(character_level_save_serialize_v1_audit
  tests/character_level_save_serialize_v1.cpp character_level_save_serialize_v1.cpp
  level_savegame_owner_v1.cpp)
 target_compile_features(character_level_save_serialize_v1_audit PRIVATE cxx_std_17)
 target_compile_options(character_level_save_serialize_v1_audit PRIVATE -Wall -Wextra -Werror)
 add_executable(level_savegame_object_manager_v1_audit
  tests/level_savegame_object_manager_v1.cpp)
 target_link_libraries(level_savegame_object_manager_v1_audit PRIVATE dh2_level_world)
 target_compile_features(level_savegame_object_manager_v1_audit PRIVATE cxx_std_17)
 target_compile_options(level_savegame_object_manager_v1_audit PRIVATE -Wall -Wextra -Werror -UNDEBUG)
 add_executable(packet_slot_owner_v1_audit tests/packet_slot_owner_v1.cpp)
 target_compile_features(packet_slot_owner_v1_audit PRIVATE cxx_std_17)
 target_compile_options(packet_slot_owner_v1_audit PRIVATE -Wall -Wextra -Werror -UNDEBUG)
 add_executable(level_savegame_player_load_v1_audit
  tests/level_savegame_player_load_v1.cpp)
 target_link_libraries(level_savegame_player_load_v1_audit PRIVATE dh2_level_world)
 target_compile_features(level_savegame_player_load_v1_audit PRIVATE cxx_std_17)
 target_compile_options(level_savegame_player_load_v1_audit PRIVATE -Wall -Wextra -Werror -UNDEBUG)
endif()
