cmake_minimum_required(VERSION 3.22)
project(dh2_native_app LANGUAGES CXX)
option(DH2_NATIVE_HOST_BUILD "Build the real native application with desktop platform adapters" OFF)
# Keep the integrated build within the shared host's RAM while the visible
# emulator runs. Ninja otherwise chooses its own compiler concurrency.
set_property(GLOBAL APPEND PROPERTY JOB_POOLS dh2_compile=2)
set(CMAKE_JOB_POOL_COMPILE dh2_compile)
# Keep source reconstruction symbols and strict floating-point behavior while
# compiling the whole native dependency tree as an optimized playable build.
# Disable explicitly for debugger experiments: -DDH2_OPTIMIZE_NATIVE=OFF.
option(DH2_OPTIMIZE_NATIVE "Optimize native gameplay while retaining debug symbols" ON)
option(DH2_ENABLE_LOAD_DIAGNOSTICS "Compile bounded navigation QA sweeps during world loading" OFF)
option(DH2_REQUEST_LOAD_DIAGNOSTICS_QA "Explicitly run compiled loading QA sweeps" OFF)
if(DH2_REQUEST_LOAD_DIAGNOSTICS_QA AND NOT DH2_ENABLE_LOAD_DIAGNOSTICS)
  message(FATAL_ERROR "Loading QA request requires DH2_ENABLE_LOAD_DIAGNOSTICS")
endif()
if(DH2_OPTIMIZE_NATIVE)
  add_compile_options(-O2 -fno-fast-math -ffp-contract=off)
endif()
if(NOT EXISTS "${DH2_SOURCE_DIR}/port/engine-textures/CMakeLists.txt")
  message(FATAL_ERROR "DH2_SOURCE_DIR must point at the DH_sc source checkout")
endif()
add_subdirectory("${DH2_SOURCE_DIR}/port/engine-textures" "${CMAKE_CURRENT_BINARY_DIR}/engine-textures")
add_subdirectory("${DH2_SOURCE_DIR}/port/scene-materials" "${CMAKE_CURRENT_BINARY_DIR}/scene-materials")
add_subdirectory("${DH2_SOURCE_DIR}/port/engine-animation" "${CMAKE_CURRENT_BINARY_DIR}/engine-animation")
add_subdirectory("${DH2_SOURCE_DIR}/port/engine-skinning" "${CMAKE_CURRENT_BINARY_DIR}/engine-skinning")
add_subdirectory("${DH2_SOURCE_DIR}/port/level-world" "${CMAKE_CURRENT_BINARY_DIR}/level-world")
add_subdirectory("${DH2_SOURCE_DIR}/port/engine-audio" "${CMAKE_CURRENT_BINARY_DIR}/engine-audio")
target_sources(dh2_scene_materials PRIVATE "${DH2_SOURCE_DIR}/port/engine-resources/resource_budget_v37.cpp"
  "${DH2_SOURCE_DIR}/port/engine-resources/retained_bytes_v39.cpp"
  "${DH2_SOURCE_DIR}/port/engine-resources/admitted_cpu_bytes_v40.cpp")

# The retained SWF cache calls the single shared resource ledger directly.
# Link its owning library rather than compiling a second implementation.
target_link_libraries(dh2_engine_ui PRIVATE dh2_scene_materials)
target_sources(dh2_engine_ui PRIVATE "${DH2_SOURCE_DIR}/port/engine-ui/hud_attack_control_v46.cpp")
target_sources(dh2_engine_ui PRIVATE
  "${DH2_SOURCE_DIR}/port/engine-ui/menu_manager_prefix_v62.cpp"
  "${DH2_SOURCE_DIR}/port/engine-ui/menu_postmovie_v62.cpp"
  "${DH2_SOURCE_DIR}/port/engine-ui/authored_menu_drag_v68.cpp")
target_sources(dh2_level_world PRIVATE "${DH2_SOURCE_DIR}/port/level-world/vox_audio_bridge_v34.cpp")
target_sources(dh2_level_world PRIVATE "${DH2_SOURCE_DIR}/port/level-world/character_close_range_v38.cpp")
target_sources(dh2_level_world PRIVATE "${DH2_SOURCE_DIR}/port/level-world/character_world_clear_aggro_v40.cpp")
target_sources(dh2_level_world PRIVATE "${DH2_SOURCE_DIR}/port/level-world/target_frontal_sort_v41.cpp")
target_sources(dh2_level_world PRIVATE "${DH2_SOURCE_DIR}/port/level-world/character_kill_fields_v21.cpp"
  "${DH2_SOURCE_DIR}/port/level-world/retained_character_kill_borrow_v42.cpp")
target_sources(dh2_level_world PRIVATE "${DH2_SOURCE_DIR}/port/level-world/character_skill_target_binding_v46.cpp")
target_link_libraries(dh2_level_world PRIVATE dh2_engine_audio)
add_library(dh2_native SHARED native_app.cpp model_renderer.cpp source_campaign_runtime_v61.cpp source_campaign_script_execution_v96.cpp source_campaign_character_fsm_v101.cpp source_process_trophies_v100.cpp source_process_arrays_v101.cpp source_settings_update_job_v102.cpp authored_shader_program.cpp audio_output_v34.cpp native_resource_budget_v38.cpp present_capabilities_v42.cpp actual_device_android_v54.cpp
  swf_gpu.cpp original_ui_assets.cpp original_ui_session.cpp front_ui_session_v87.cpp front_selected_profile_v50.cpp original_cache_assets_v1.cpp
  gameplay_hud.cpp gameplay_icons.cpp character_panel_session_v1.cpp character_panel_presentation_v1.cpp character_panel_authored_v2.cpp character_panel_reload_v3.cpp character_panel_runtime_v3.cpp source_campaign_character_panel_v95.cpp faery_gameplay_v1.cpp
  faery_cast_owner_v2.cpp faery_cast_state_v2.cpp player_ai_callables_v1.cpp player_timer_helpers_v1.cpp player_recurring_effects_v7.cpp
  )
# Persistent Application audio: one V42 runtime/session owns the native mixer.
target_sources(dh2_native PRIVATE source_campaign_quicksave_v88.cpp source_campaign_camera_release_v88.cpp source_campaign_release_v88.cpp
  "${DH2_SOURCE_DIR}/port/level-loader/level_batching_source_v96.cpp"
  source_campaign_scene_pf_release_v106.cpp
  source_campaign_class_release_v106.cpp
  "${DH2_SOURCE_DIR}/port/level-loader/player_light_tweaker_owner_v90.cpp")
target_sources(dh2_native PRIVATE source_campaign_object_update_actor_v104.cpp
  source_campaign_object_update_bindings_v105.cpp source_campaign_character_unload_v105.cpp
  source_campaign_noncharacter_virtual_v105.cpp source_campaign_ai_queue_v105.cpp
  source_campaign_character_eligibility_v106.cpp)
target_sources(dh2_native PRIVATE source_campaign_character_controller_v107.cpp source_campaign_character_clean_v107.cpp)
target_sources(dh2_native PRIVATE source_campaign_character_zoning_v108.cpp source_campaign_character_start_v108.cpp)
target_sources(dh2_native PRIVATE source_campaign_character_gameobject_v111.cpp
  source_campaign_character_interest_v111.cpp source_campaign_character_frame_v111.cpp
  source_campaign_character_merchant_v114.cpp source_campaign_character_interaction_v114.cpp)
target_sources(dh2_native PRIVATE source_campaign_projectiles_v111.cpp)
target_sources(dh2_native PRIVATE source_campaign_projectile_methods_v112.cpp)
target_sources(dh2_native PRIVATE "${DH2_SOURCE_DIR}/port/level-loader/area_transition_sequence_v114.cpp")
target_sources(dh2_engine_ui PRIVATE "${DH2_SOURCE_DIR}/port/level-loader/area_transition_request_v114.cpp"
  "${DH2_SOURCE_DIR}/port/level-loader/menu_fs_command_v114.cpp"
  "${DH2_SOURCE_DIR}/port/level-loader/menu_fs_trace_v114.cpp"
  "${DH2_SOURCE_DIR}/port/level-loader/menu_end_loading_source_v114.cpp"
  "${DH2_SOURCE_DIR}/port/level-loader/menu_fx_pop_all_v114.cpp"
  "${DH2_SOURCE_DIR}/port/level-loader/menu_main_menu_state_v114.cpp"
  "${DH2_SOURCE_DIR}/port/level-loader/menu_main_menu_load_v114.cpp"
  "${DH2_SOURCE_DIR}/port/level-loader/application_go_to_main_menu_v114.cpp")
target_include_directories(dh2_engine_ui PRIVATE "${DH2_SOURCE_DIR}/port/level-loader")
target_sources(dh2_native PRIVATE source_campaign_transition_restore_v114.cpp)
target_sources(dh2_native PRIVATE source_campaign_combat_v115.cpp)
target_sources(dh2_native PRIVATE source_campaign_player_removal_v114.cpp)
target_sources(dh2_level_world PRIVATE
  "${DH2_SOURCE_DIR}/port/level-world/character_hit_player_v116.cpp"
  "${DH2_SOURCE_DIR}/port/level-world/character_skill_application_v116.cpp")
target_sources(dh2_native PRIVATE source_main_menu_leaves_v114.cpp)
target_sources(dh2_native PRIVATE source_android_notify_trophy_v114.cpp)
target_sources(dh2_level_world PRIVATE "${DH2_SOURCE_DIR}/port/level-world/source_main_menu_process_v114.cpp")
target_sources(dh2_native PRIVATE "${DH2_SOURCE_DIR}/port/level-loader/projectile_methods_v112.cpp"
  "${DH2_SOURCE_DIR}/port/level-loader/projectile_target_methods_v113.cpp"
  "${DH2_SOURCE_DIR}/port/level-loader/laser_projectile_methods_v112.cpp")
target_sources(dh2_native PRIVATE source_campaign_faery_v109.cpp)
target_sources(dh2_native PRIVATE source_campaign_language_refresh_v109.cpp)
target_sources(dh2_native PRIVATE source_campaign_language_scene_v109.cpp)
target_sources(dh2_level_world PRIVATE "${DH2_SOURCE_DIR}/port/level-world/canonical_projectile_physical_v112.cpp"
  "${DH2_SOURCE_DIR}/port/level-world/character_skill_projectile_callbacks_v112.cpp")
target_sources(dh2_level_world PRIVATE "${DH2_SOURCE_DIR}/port/level-world/character_update_pointers_v105.cpp"
  "${DH2_SOURCE_DIR}/port/level-world/canonical_object_update_v102.cpp"
  "${DH2_SOURCE_DIR}/port/level-world/character_loading_queue_v105.cpp")
target_sources(dh2_level_world PRIVATE "${DH2_SOURCE_DIR}/port/level-world/canonical_object_fake_remove_v106.cpp"
  "${DH2_SOURCE_DIR}/port/level-world/character_object_interest_v106.cpp")
target_sources(dh2_level_world PRIVATE "${DH2_SOURCE_DIR}/port/level-world/canonical_object_deferred_v107.cpp")
target_sources(dh2_level_world PRIVATE
  "${DH2_SOURCE_DIR}/port/level-world/gameobject_zoning_v108.cpp")
target_sources(dh2_level_world PRIVATE
  "${DH2_SOURCE_DIR}/port/level-world/character_update_aggro_v108.cpp"
  "${DH2_SOURCE_DIR}/port/level-world/level_can_move_towards_v108.cpp")
target_sources(dh2_level_world PRIVATE "${DH2_SOURCE_DIR}/port/level-world/gameobject_stop_source_v111.cpp")
target_sources(dh2_native PRIVATE
  source_application_pause_save_v1.cpp
  "${DH2_SOURCE_DIR}/port/engine-audio/audio_clock_v40.cpp"
  "${DH2_SOURCE_DIR}/port/engine-audio/audio_source_command_v40.cpp"
  "${DH2_SOURCE_DIR}/port/engine-audio/audio_campaign_bridge_v46.cpp"
  "${DH2_SOURCE_DIR}/port/engine-audio/audio_level_gameplay_v67.cpp"
  "${DH2_SOURCE_DIR}/port/engine-audio/audio_gameplay_runtime_v42.cpp"
  "${DH2_SOURCE_DIR}/port/engine-audio/integration-v42/audio_native_session_v42.cpp"
  "${DH2_SOURCE_DIR}/port/engine-audio/integration-v42/audio_application_manager_v42.cpp"
  "${DH2_SOURCE_DIR}/port/engine-audio/integration-v42/application/native_audio_application_v42.cpp"
  "${DH2_SOURCE_DIR}/port/engine-audio/integration-v40/focus/audio_lifecycle_gate_v40.cpp"
  "${DH2_SOURCE_DIR}/port/engine-audio/integration-v40/focus/audio_lifecycle_jni_v40.cpp"
  "${DH2_SOURCE_DIR}/port/engine-audio/integration-v40/focus/audio_output_v40.cpp"
  "${DH2_SOURCE_DIR}/port/engine-audio/integration-v40/focus/audio_control_v40.cpp")
target_include_directories(dh2_native PRIVATE "${DH2_SOURCE_DIR}"
  "${DH2_SOURCE_DIR}/port/engine-audio/integration-v42")

target_compile_features(dh2_native PRIVATE cxx_std_17)
target_compile_definitions(dh2_native PRIVATE DH2_ENABLE_LOAD_DIAGNOSTICS=$<BOOL:${DH2_ENABLE_LOAD_DIAGNOSTICS}>)
if(DH2_ENABLE_LOAD_DIAGNOSTICS)
  target_compile_definitions(dh2_native PRIVATE DH2_LOAD_DIAGNOSTICS_QA_REQUEST_V39=$<BOOL:${DH2_REQUEST_LOAD_DIAGNOSTICS_QA}>)
endif()
target_compile_options(dh2_native PRIVATE -Wall -Wextra -fno-fast-math -ffp-contract=off)
if(NOT DH2_NATIVE_HOST_BUILD)
 target_compile_options(dh2_native PRIVATE -Werror)
endif()
target_include_directories(dh2_native PRIVATE "${DH2_SOURCE_DIR}/port/engine-ui"
  "${DH2_SOURCE_DIR}/port/engine-math"
  "${DH2_SOURCE_DIR}/port/engine-resources"
  "${DH2_SOURCE_DIR}/port/level-loader"
  "${DH2_SOURCE_DIR}/port/scene-materials")
target_link_options(dh2_native PRIVATE -Wl,-z,max-page-size=16384 -Wl,--no-undefined)
if(NOT DH2_NATIVE_HOST_BUILD)
 target_link_options(dh2_native PRIVATE -Wl,--error-limit=0)
endif()
target_link_libraries(dh2_native PRIVATE dh2_engine_textures dh2_scene_materials dh2_level_world dh2_script_runtime dh2_engine_ui dh2_gameswf_headers dh2_engine_audio android log GLESv2 EGL z dl)
target_sources(dh2_native PRIVATE source_campaign_conditions_v70.cpp source_campaign_quests_v76.cpp source_script_ui_world_v97.cpp renderer_campaign_conditions_v75.cpp
  source_campaign_zones_v83.cpp
  source_campaign_death_rewards_v84.cpp
  source_campaign_items_v88.cpp
  source_campaign_container_targets_v104.cpp
  source_campaign_generated_rooms_v105.cpp
  source_campaign_object_loading_deferred_v106.cpp
  source_campaign_gameplay_inputs_v107.cpp
  source_campaign_spawn_groups_v108.cpp
  source_campaign_save_objects_v86.cpp
  source_campaign_frame_environment_v76.cpp
  source_campaign_fx_v77.cpp
  source_campaign_anchor_v75.cpp
  source_campaign_character_cache_v81.cpp
  source_campaign_script_actor_v96.cpp
  source_campaign_script_world_v117.cpp
  source_campaign_script_audio_v117.cpp
  source_campaign_equipment_services_v118.cpp
  source_campaign_script_ai_v118.cpp
  source_campaign_script_tutorial_v118.cpp
  source_campaign_script_tutorial_control_v118.cpp
  android_intro_movie_v119.cpp
  source_process_zoom_v119.cpp
  native_menu_preview_v121.cpp
  native_menu_preview_player_v122.cpp
  native_menu_preview_lifecycle_v123.cpp
  native_menu_preview_character_services_v122.cpp
  native_menu_preview_services_v122.cpp
  native_menu_preview_fx_initialization_v122.cpp
  native_menu_preview_profile_v122.cpp
  source_campaign_script_environment_v120.cpp
  source_campaign_script_runtime_v99.cpp
  source_campaign_animation_step_v100.cpp
  source_campaign_events_v75.cpp)

target_sources(dh2_level_world PRIVATE "${DH2_SOURCE_DIR}/port/level-world/spawn_group_manager_v108.cpp" "${DH2_SOURCE_DIR}/port/level-world/canonical_spawn_spot_v108.cpp")

target_sources(dh2_native PRIVATE source_campaign_batch_compilation_v111.cpp)
target_sources(dh2_native PRIVATE native_batch_program_v113.cpp)
target_sources(dh2_native PRIVATE source_campaign_scene_lights_v113.cpp)
target_sources(dh2_native PRIVATE source_campaign_light_environment_v113.cpp)
