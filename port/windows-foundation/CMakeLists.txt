cmake_minimum_required(VERSION 3.22)
project(dh_game_foundation LANGUAGES C CXX)
add_subdirectory(../physics-backend "${CMAKE_CURRENT_BINARY_DIR}/physics-backend")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(WIN32)
  set(DH2_PLATFORM_SOURCES platform_win32.cpp platform_context_identity.cpp)
  set(DH2_PLATFORM_LIBRARIES opengl32 user32 gdi32)
  set(DH2_FRONTEND_HOST_SOURCE features/frontend/native_host.cpp)
  set(DH2_AUDIO_CONTROL_SOURCES
    features/audio/windows_source_session_control_v1.cpp
    features/audio/winmm_output.cpp)
  set(DH2_AUDIO_LIBRARIES winmm)
else()
  find_package(OpenGL REQUIRED)
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)
  add_compile_definitions(DH_PLATFORM_SDL2)
  set(DH2_PLATFORM_SOURCES platform_linux.cpp platform_context_identity.cpp)
  set(DH2_PLATFORM_LIBRARIES OpenGL::GL PkgConfig::SDL2)
  set(DH2_FRONTEND_HOST_SOURCE features/frontend/native_host_sdl.cpp)
  set(DH2_AUDIO_CONTROL_SOURCES
    features/audio/linux_sdl2_source_session_control_v1.cpp
    features/audio/linux_sdl2_output.cpp)
  set(DH2_AUDIO_LIBRARIES PkgConfig::SDL2)
endif()
add_library(recovered_content STATIC
  ../engine-resources/resources.cpp ../asset-payloads/payloads.cpp
  ../engine-math/math.cpp ../scene-materials/scene.cpp
  ../engine-animation/animation.cpp ../engine-animation/angle_interpreter.cpp
  ../engine-animation/events.cpp ../engine-animation/event_track.cpp
  ../engine-animation/animation_blend.cpp ../level-world/animation_blender.cpp
  ../level-world/visual_timeline.cpp
  ../level-world/visual_motion.cpp ../level-world/physical_controls.cpp
  ../level-world/gameplay_camera_target_v2.cpp
  ../level-world/camera_anchor_owner_v75.cpp
  ../engine-skinning/skinning.cpp ../engine-textures/textures.cpp ../engine-textures/pvrtc.cpp
  ../game-data/data.cpp ../game-data/properties.cpp ../game-data/class_tables.cpp
  ../game-data/animation_tables.cpp ../game-data/design_settings.cpp
  ../game-data/combat.cpp ../game-data/combat_result.cpp
  ../game-data/item_gear_properties_v5.cpp ../game-data/items.cpp
  ../game-data/ai.cpp ../level-world/character_attack_geometry.cpp
  ../level-world/actor_rotation.cpp ../level-world/navigation_heading.cpp
  ../engine-ui/localization.cpp ../engine-ui/localization_parse_ex_v1.cpp ../script-runtime/script_constants.cpp
  ../level-world/gameplay_camera_scene_v3.cpp ../level-world/gameplay_camera_damping_v1.cpp)
add_library(content_xml STATIC
  ../level-loader/vendor/tinyxml/tinyxml.cpp
  ../level-loader/vendor/tinyxml/tinyxmlparser.cpp
  ../level-loader/vendor/tinyxml/tinyxmlerror.cpp
  ../level-loader/vendor/tinyxml/tinystr.cpp)
add_library(foundation_data STATIC character_state.cpp save_store.cpp camera.cpp
  features/skills_animation/skill_animation_program.cpp
  original_pose_blend.cpp
  retained_pose_playback.cpp original_campaign_runtime.cpp
  retained_animation_owner.cpp campaign_camera_adapter.cpp
  retained_sequence_playback.cpp
  original_actor_camera_anchor.cpp
  modular_defaults.cpp
  asset_catalog.cpp world.cpp texture_loader.cpp original_scene.cpp original_character.cpp level_manifest.cpp assembled_level.cpp
  content_paths.cpp collision_scene.cpp actor_definitions.cpp actor_state.cpp actor_movement.cpp gameplay_camera.cpp
  animation_markers.cpp combat_system.cpp targeting.cpp hud_art.cpp hud_geometry.cpp actor_profiles.cpp actor_population.cpp
  original_actor_properties.cpp original_camera_config.cpp equipment_visual.cpp controller_policy.cpp original_combat_properties.cpp target_selection_runtime.cpp actor_combat_runtime.cpp original_melee_bindings.cpp playable_actor_world.cpp original_combat_visual_plan.cpp original_attack_sequence.cpp combat_session.cpp game_save.cpp hud_glyphs.cpp original_actor_labels.cpp
  ../engine-ui/hud_freetype_font.cpp ../engine-ui/freetype_glyph_kernel.cpp ../engine-ui/freetype_bitmap_alpha.cpp)
include(../engine-ui/freetype237-hud.cmake)
target_sources(dh2_freetype237 PRIVATE "${DH2_FT237}/src/base/ftbitmap.c")
target_include_directories(foundation_data PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(foundation_data PUBLIC recovered_content content_xml dh2_freetype237)
add_executable(dh-foundation main.cpp ${DH2_PLATFORM_SOURCES} renderer.cpp render_queue.cpp overlay_renderer.cpp)
target_sources(dh-foundation PRIVATE features/generic_skills/pc_skill_hud_projection_v1.cpp features/generic_skills/pc_gameplay_hud_v1.cpp features/generic_skills/pc_gameplay_hud_source_art_v1.cpp features/generic_skills/pc_gameplay_hud_button_art_v1.cpp)
target_sources(dh-foundation PRIVATE features/frontend/menu_return/menu_return_v1.cpp)
# B004/B029: OOI owner (500 ms candidate refresh) and live Session adapter for the rendered target marker.
target_sources(dh-foundation PRIVATE features/combat/auto_target_marker_v1.cpp features/combat/object_of_interest_owner_v1.cpp features/combat/object_of_interest_world_v1.cpp)
add_library(foundation_runtime_potion STATIC
  features/inventory/runtime_session_potion_use_v1.cpp
  ../level-world/character_skill_application_v6.cpp ../level-world/character_skill_attack_v6.cpp
  ../level-world/character_combat_result_v6.cpp ../level-world/character_hit_v6.cpp
  ../level-world/character_target_providers.cpp ../level-world/character_buffs.cpp ../level-world/character_timers.cpp)
target_link_libraries(foundation_runtime_potion PUBLIC foundation_data)
target_include_directories(foundation_runtime_potion PRIVATE ../level-world ../game-data ../script-runtime)
target_link_libraries(dh-foundation PRIVATE foundation_runtime_potion)
target_sources(foundation_data PRIVATE ../game-data/character_templates_v78.cpp)
target_sources(foundation_data PRIVATE player_profile_properties.cpp)
target_sources(foundation_data PRIVATE embedded_scene_clips.cpp)
target_sources(foundation_data PRIVATE ../game-data/skill_tables.cpp)
add_library(foundation_runtime_audio STATIC
  features/audio/runtime_audio_host_v1.cpp features/audio/runtime_combat_audio_v1.cpp
  features/audio/runtime_session_audio_v1.cpp
  # B040: original LevelConfig music name reader (main.cpp level music hook).
  features/audio/level_music_v1.cpp
  features/audio/runtime_attack_sound_v1.cpp
  ${DH2_AUDIO_CONTROL_SOURCES}
  features/audio/feature_audio.cpp
  ../engine-audio/audio_source_command_v40.cpp ../engine-audio/vox_source_fields_v38.cpp
  ../engine-audio/audio_listener_rows_v38.cpp ../engine-audio/audio_spatial_v34.cpp
  ../engine-audio/integration-v42/audio_native_session_v42.cpp
  ../engine-audio/integration-v40/focus/audio_lifecycle_gate_v40.cpp
  ../engine-audio/audio_gameplay_runtime_v42.cpp ../engine-audio/audio_clock_v40.cpp
  ../engine-audio/audio_source_bindings_v38.cpp ../engine-audio/audio_sample_v34.cpp
  ../engine-audio/audio_catalog_v34.cpp ../engine-audio/audio_bank_v34.cpp
  ../engine-audio/audio_native_envelope_v34.cpp ../engine-audio/audio_mixer_v34.cpp
  ../level-world/vox_play3d_owner_v2.cpp ../level-world/character_combat_sound_v1.cpp
  ../level-world/character_combat_sound_tables_v2.cpp)
target_link_libraries(foundation_runtime_audio PUBLIC foundation_data ${DH2_AUDIO_LIBRARIES})
target_link_libraries(dh-foundation PRIVATE foundation_runtime_audio)
if(WIN32)
  target_sources(foundation_runtime_audio PRIVATE features/audio/frontend_menu_audio_v1.cpp)
endif()
if(NOT WIN32)
  add_executable(linux_sdl2_output_smoke_tests features/audio/linux_sdl2_output_smoke.cpp)
  target_link_libraries(linux_sdl2_output_smoke_tests PRIVATE foundation_runtime_audio)
  add_test(NAME linux_sdl2_output_dummy COMMAND linux_sdl2_output_smoke_tests dummy)
  add_test(NAME linux_sdl2_output_failure COMMAND linux_sdl2_output_smoke_tests failure)
  add_executable(linux_sdl2_source_session_control_smoke_tests
    features/audio/linux_sdl2_source_session_control_v1_smoke.cpp)
  target_link_libraries(linux_sdl2_source_session_control_smoke_tests PRIVATE foundation_runtime_audio)
  add_test(NAME linux_sdl2_source_session_control COMMAND linux_sdl2_source_session_control_smoke_tests)
endif()
add_library(foundation_runtime_skills_menu STATIC
  features/generic_skills/generic_skills_page_v1.cpp
  features/generic_skills/runtime_skills_menu_v1.cpp
  features/generic_skills/runtime_skill_progression_v1.cpp
  features/generic_skills/runtime_skill_session_training_v1.cpp
  ../level-world/player_initial_grants_v2.cpp
  features/generic_skills/runtime_skill_mana_v1.cpp
  features/generic_skills/runtime_skills_text_v1.cpp
  features/character_menu/menu_text_layout_v1.cpp
  features/character_menu/skill_page_text_projection_v1.cpp
  features/skill_ui/skill_ui.cpp features/skill_ui/original_skill_art.cpp
  features/skill_ui/original_skill_presenter.cpp)
target_link_libraries(foundation_runtime_skills_menu PUBLIC foundation_data foundation_frontend)
# Preview 14 Stats +/- training (SWF btn_train_* route; see features/character_menu/stat_training_v1.hpp).
target_sources(foundation_runtime_skills_menu PRIVATE features/character_menu/stat_training_v1.cpp)
target_include_directories(foundation_runtime_skills_menu PRIVATE
  ../scene-materials ../engine-animation ../engine-skinning ../engine-resources)
target_include_directories(dh-foundation PRIVATE
  ../scene-materials ../engine-animation ../engine-skinning ../engine-resources)
target_link_libraries(dh-foundation PRIVATE foundation_runtime_skills_menu)
add_library(foundation_runtime_skill_cast STATIC
  features/generic_skills/runtime_skill_cast_coordinator_v1.cpp
  features/generic_skills/pc_cooldown_frame_v1.cpp
  features/generic_skills/runtime_skill_cast_prepare_v1.cpp
  features/generic_skills/runtime_skill_target_query_v1.cpp
  features/generic_skills/runtime_skill_activation_v1.cpp
  features/generic_skills/runtime_skill_activation_session_v1.cpp
  features/generic_skills/runtime_skill_animation_bank_v1.cpp
  features/faery_menu/hotty_cast_v1.cpp
  features/faery_menu/hotty_cast_session_v1.cpp
  features/faery_menu/hotty_character_cast_v1.cpp
  features/faery_menu/hotty_source_use_v1.cpp
  features/faery_menu/celest_cast_v1.cpp
  features/faery_menu/celest_source_use_v1.cpp
  # Preview 14 FAERY: CharacterState-backed Faery character-menu page (not the legacy Save path).
  features/faery_menu/character_state_page_v1.cpp
  features/faery_menu/original_art.cpp
  features/faery_menu/source_text_v1.cpp
  features/faery_menu/character_state_faery_v1.cpp
  # P15 FAERYSOUND (B050): original Celest/Hotty OnPreSkill_ sound labels.
  features/faery_menu/faery_cast_sound_v1.cpp
  ../game-data/faery_tables.cpp
  ../level-world/character_path_commands.cpp)
target_link_libraries(foundation_runtime_skill_cast PUBLIC foundation_runtime_skills_menu)
target_link_libraries(dh-foundation PRIVATE foundation_runtime_skill_cast)
add_library(foundation_runtime_equipment_menu STATIC
  features/equipment/runtime_equipment_binding_v1.cpp
  features/equipment/runtime_equipment_page_v1.cpp
  features/equipment/runtime_equipment_text_v1.cpp
  features/equipment/runtime_player_locomotion_v1.cpp
  features/equipment/runtime_player_locomotion_program_v1.cpp
  features/equipment/runtime_player_locomotion_library_v1.cpp
  features/combat/runtime_player_combo_chain_v1.cpp
  features/combat/runtime_player_profile_attack_bank_v1.cpp
  ../level-world/player_equipment_queries_v1.cpp ../level-world/character_stance.cpp
  features/equipment/equipment_main_page.cpp features/equipment/equipment_adapter.cpp
  features/equipment/equipment_inventory_actions_v1.cpp
  features/equipment/equipment_menu.cpp features/equipment/original_equipment_art.cpp
  features/equipment/source_equipment_appearance.cpp
  features/equipment/source_equipment_render_bridge.cpp
  features/equipment/source_equipment_material_binding.cpp
  features/equipment/preview/runtime_equipment_preview_v1.cpp
  features/equipment/preview/runtime_equipment_preview_frame_v1.cpp
  features/effects/effects_material_binding.cpp
  features/inventory/inventory_feature.cpp features/inventory/inventory_menu.cpp
  features/inventory/inventory_details.cpp features/inventory/original_inventory_art.cpp
  features/inventory/source_item_descriptors.cpp
  ../engine-ui/item_text_owner_v5.cpp ../engine-ui/item_text_varargs_v5.cpp
  ../engine-ui/character_menu_potion_text_v4.cpp ../game-data/item_presentation_v5.cpp
  ../game-data/player_equipment_v3.cpp ../game-data/item_power_tables_v5.cpp
  ../engine-skinning/visual_skin_owner_v6.cpp
  ../engine-skinning/visual_skin_selection_v6.cpp ../engine-skinning/skin_pose_cache_v32.cpp)
target_link_libraries(foundation_runtime_equipment_menu PUBLIC foundation_data)
target_include_directories(foundation_runtime_equipment_menu PRIVATE
  ../scene-materials ../engine-animation ../engine-skinning ../engine-resources)
target_link_libraries(dh-foundation PRIVATE foundation_runtime_equipment_menu)

add_library(foundation_runtime_effects STATIC
  ../asset-payloads/sha256.cpp
  features/effects/runtime_source_fx_asset_v1.cpp
  features/effects/runtime_swing_fx_observer_v1.cpp
  features/effects/runtime_level_up_presentation_v1.cpp
  features/effects/runtime_effects_factory_v1.cpp
  features/effects/runtime_combat_effects_v1.cpp
  features/effects/runtime_effects_renderer_v1.cpp
  features/effects/celest_target_fx_dispatch_v1.cpp
  features/effects/effects_executor.cpp
  features/effects/effects_render_bridge.cpp
  ../game-data/effects_tables.cpp
  ../level-world/character_mesh_fx_owner_v4.cpp
  ../level-world/character_authored_resource_v32.cpp
  ../level-world/character_authored_fx_forces_v4.cpp
  ../level-world/character_fx_floor_query_v3.cpp
  ../level-world/character_animation_step_fx_v2.cpp
  ../level-world/authored_fx_mesh_graph_v32.cpp
  ../level-world/authored_fx_nonrender_geometry_v32.cpp
  ../level-world/authored_fx_transform_v5.cpp
  ../level-world/character_fx_kernels_v1.cpp
  ../level-world/character_fx_state_v1.cpp
  ../level-world/visual_fx_manager_libraries_v63.cpp
  ../level-world/source_fx_node_matrix_v4.cpp
  ../level-world/fx_texture_animation_v1.cpp
  ../level-world/visual_fx_preload.cpp
  ../scene-materials/particle_scene_v1.cpp
  ../engine-animation/component_applicator.cpp
  ../engine-animation/particle_scalar_animation_v6.cpp
  ../engine-animation/particle_cloud_runtime_v1.cpp
  ../engine-animation/particle_cloud_runtime_v3.cpp
  ../engine-animation/particle_deflector_v1.cpp
  ../engine-animation/particle_force_scene_v2.cpp
  ../engine-animation/particle_bound_forces_v4.cpp
  ../engine-animation/particle_resource_init_v2.cpp
  ../engine-animation/particle_cloud_models_v1.cpp
  ../engine-animation/particle_billboard_v1.cpp
  ../engine-animation/particle_force_scene_v1.cpp
  ../engine-animation/particle_scene_color_v1.cpp
  ../engine-animation/particle_factory.cpp
  ../engine-animation/particle_random_v1.cpp
  ../engine-animation/particle_emission.cpp
  ../engine-animation/particle_box_v2.cpp
  ../engine-animation/material_color.cpp
  ../engine-animation/material_color_v3.cpp
  ../engine-animation/particle_parameter.cpp
  ../engine-animation/particle_resource_init_v32.cpp
  ../engine-animation/particle_billboard_v32.cpp)
target_link_libraries(foundation_runtime_effects PUBLIC foundation_runtime_equipment_menu foundation_runtime_skill_cast)
target_include_directories(foundation_runtime_effects PRIVATE
  ../scene-materials ../engine-animation ../engine-skinning ../engine-resources)
target_link_libraries(dh-foundation PRIVATE foundation_runtime_effects)

target_link_libraries(dh-foundation PRIVATE foundation_data ${DH2_PLATFORM_LIBRARIES})
add_library(foundation_frontend STATIC
  ${DH2_FRONTEND_HOST_SOURCE} features/frontend/frontend_runtime_v1.cpp
  features/frontend/frontend_runtime_contract_v1.cpp features/frontend/frontend_presentation_v1.cpp
  features/frontend/rich_text.cpp
  features/pause_ui/source_pause_ui_v1.cpp
  features/pause_ui/source_pause_ui_render_v1.cpp
  features/pause_ui/source_pause_ui_art_v1.cpp
  features/frontend/art/original_art.cpp features/frontend/art/original_art_data.cpp
  features/frontend/art/runtime_source_data.cpp features/frontend/art/compose.cpp
  features/frontend/flow/menu_flow.cpp features/frontend/flow/presentation.cpp
  features/frontend/input/frontend_input.cpp features/frontend/input/screen_interaction.cpp
  features/frontend/creation/creation_adapter.cpp features/frontend/creation/dynamic_text_bindings.cpp
  features/frontend/creation/runtime_creation_persistence_v1.cpp
  features/frontend/creation/runtime_creation_flow_adapter_v1.cpp
  features/frontend/creation/generic_creation_host_v1.cpp
  features/frontend/creation/runtime_creation_source_loader_v1.cpp
  features/frontend/preview/class_preview_scene.cpp features/frontend/preview/class_light_source.cpp
  features/frontend/preview/creation_preview.cpp features/frontend/preview/scene_materials.cpp
  ../game-data/class_preview_setup.cpp ../game-data/loot_tables_v2.cpp
  ../game-data/fresh_player_profile_v1.cpp
  ../engine-ui/edit_text_definition_v1.cpp)
# P14 schema: per-slot menu metadata stamping + panel projection (engine-ui menu_save_slot_projection_v1).
target_sources(foundation_frontend PRIVATE features/menu_metadata/menu_metadata_v1.cpp ../game-data/level_tables.cpp
  ../engine-ui/menu_save_slot_projection_v1.cpp ../engine-ui/save_slot_date_v1.cpp)
target_link_libraries(foundation_frontend PUBLIC foundation_data ${DH2_PLATFORM_LIBRARIES})
target_include_directories(foundation_frontend PRIVATE
  ../scene-materials ../engine-animation ../engine-skinning ../engine-resources)
target_link_libraries(dh-foundation PRIVATE foundation_frontend)
if(MINGW)
  add_link_options(-static)
  target_link_options(dh-foundation PRIVATE -static)
endif()
enable_testing()
foreach(test save camera asset level_manifest animation_markers combat_system actor_movement gameplay_camera actor_definitions content_paths actor_root_motion original_actor_properties equipment_visual original_melee_bindings original_combat_visual_plan original_attack_sequence)
  add_executable(${test}_tests tests/${test}_tests.cpp)
  target_link_libraries(${test}_tests PRIVATE foundation_data)
  if(test STREQUAL "animation_markers")
    add_test(NAME ${test} COMMAND ${test}_tests "${CMAKE_CURRENT_SOURCE_DIR}/../android-native/app/src/main/assets")
  elseif(test STREQUAL "original_actor_properties")
    add_test(NAME ${test} COMMAND ${test}_tests "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-population-assets")
  else()
    add_test(NAME ${test} COMMAND ${test}_tests "${CMAKE_CURRENT_SOURCE_DIR}/../..")
  endif()
endforeach()
add_executable(character_probe tests/character_probe.cpp)
target_link_libraries(character_probe PRIVATE foundation_data)
add_test(NAME original_character COMMAND character_probe "${CMAKE_CURRENT_SOURCE_DIR}/../android-native/app/src/main/assets")
add_executable(actor_population_template_selection_tests tests/actor_population_template_selection_tests.cpp)
target_link_libraries(actor_population_template_selection_tests PRIVATE foundation_data)
add_test(NAME actor_population_template_selection COMMAND actor_population_template_selection_tests "${CMAKE_CURRENT_SOURCE_DIR}/../..")
add_executable(scene_probe tests/scene_probe.cpp)
target_include_directories(scene_probe PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(scene_probe PRIVATE foundation_data)
if(WIN32)
  add_executable(window_probe tests/window_probe.cpp ${DH2_PLATFORM_SOURCES})
  target_link_libraries(window_probe PRIVATE ${DH2_PLATFORM_LIBRARIES})
  add_test(NAME window_lifecycle COMMAND window_probe)
  add_executable(renderer_visual_quality_tests tests/renderer_visual_quality_tests.cpp ${DH2_PLATFORM_SOURCES} renderer.cpp)
  target_link_libraries(renderer_visual_quality_tests PRIVATE ${DH2_PLATFORM_LIBRARIES})
  add_test(NAME renderer_visual_quality COMMAND renderer_visual_quality_tests)
else()
  add_executable(linux_platform_smoke_tests tests/linux_platform_smoke_tests.cpp ${DH2_PLATFORM_SOURCES} renderer.cpp)
  target_link_libraries(linux_platform_smoke_tests PRIVATE ${DH2_PLATFORM_LIBRARIES})
  add_test(NAME linux_platform_smoke COMMAND linux_platform_smoke_tests)
endif()
add_executable(private_save_transport_win32_tests tests/private_save_transport_win32_tests.cpp
  ../level-world/private_save_file_transport_v45.cpp ../level-world/savegame_jobs_owner_v2.cpp
  ../level-world/savegame_stream_v2.cpp ../game-data/fresh_player_profile_v1.cpp
  ../game-data/player_profile_index_v1.cpp)
target_link_libraries(private_save_transport_win32_tests PRIVATE foundation_data)
add_test(NAME private_save_transport_win32 COMMAND private_save_transport_win32_tests "${CMAKE_CURRENT_SOURCE_DIR}/../..")
add_executable(collision_probe tests/collision_probe.cpp)
target_link_libraries(collision_probe PRIVATE foundation_data)
add_executable(root_pivot_tests tests/root_pivot_tests.cpp)
target_link_libraries(root_pivot_tests PRIVATE foundation_data)
add_test(NAME root_pivot COMMAND root_pivot_tests "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-shared-assets")
add_executable(original_combat_properties_tests tests/original_combat_properties_tests.cpp)
target_link_libraries(original_combat_properties_tests PRIVATE foundation_data)
add_test(NAME original_combat_properties COMMAND original_combat_properties_tests "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-shared-assets")
add_executable(target_selection_runtime_tests tests/target_selection_runtime_tests.cpp)
target_link_libraries(target_selection_runtime_tests PRIVATE foundation_data)
add_test(NAME target_selection_runtime COMMAND target_selection_runtime_tests)
add_executable(actor_combat_runtime_tests tests/actor_combat_runtime_tests.cpp)
target_link_libraries(actor_combat_runtime_tests PRIVATE foundation_data)
add_test(NAME actor_combat_runtime COMMAND actor_combat_runtime_tests)
add_executable(playable_actor_world_tests tests/playable_actor_world_tests.cpp)
target_link_libraries(playable_actor_world_tests PRIVATE foundation_data)
add_test(NAME playable_actor_world COMMAND playable_actor_world_tests "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-shared-assets" "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-shared-assets")
add_executable(combat_session_tests tests/combat_session_tests.cpp)
target_link_libraries(combat_session_tests PRIVATE foundation_data)
add_test(NAME combat_session COMMAND combat_session_tests "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-shared-assets")
add_executable(source_session_program_tests tests/source_session_program_tests.cpp)
target_link_libraries(source_session_program_tests PRIVATE foundation_data)
add_test(NAME source_session_program COMMAND source_session_program_tests "${CMAKE_CURRENT_SOURCE_DIR}/../..")
add_executable(session_skill_binding_tests
  features/skills_animation/session_native_program_tests.cpp
  features/skills_animation/session_skill_animation.cpp
  features/skills_animation/source_skill_animation.cpp
  ../level-world/character_skill_ai_v3.cpp
  ../level-world/character_skill_state_v4.cpp
  ../game-data/skill_tables.cpp)
target_link_libraries(session_skill_binding_tests PRIVATE foundation_data)
add_test(NAME session_skill_binding COMMAND session_skill_binding_tests "${CMAKE_CURRENT_SOURCE_DIR}/../..")
add_executable(game_save_tests tests/game_save_tests.cpp)
target_link_libraries(game_save_tests PRIVATE foundation_data)
add_test(NAME game_save COMMAND game_save_tests "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-shared-assets" "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-shared-assets" "${CMAKE_CURRENT_BINARY_DIR}/live-roundtrip.save")
add_executable(hud_target_tests tests/hud_target_tests.cpp)
target_link_libraries(hud_target_tests PRIVATE foundation_data)
add_test(NAME hud_target COMMAND hud_target_tests)
add_executable(hud_glyphs_tests tests/hud_glyphs_tests.cpp)
target_link_libraries(hud_glyphs_tests PRIVATE foundation_data)
add_test(NAME hud_glyphs COMMAND hud_glyphs_tests "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-shared-assets/data/Fontin SmallCaps.ttf")
add_executable(original_actor_labels_tests tests/original_actor_labels_tests.cpp)
target_link_libraries(original_actor_labels_tests PRIVATE foundation_data)
add_test(NAME original_actor_labels COMMAND original_actor_labels_tests "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-shared-assets")
add_executable(original_pose_blend_tests tests/original_pose_blend_tests.cpp)
target_link_libraries(original_pose_blend_tests PRIVATE foundation_data)
add_test(NAME original_pose_blend COMMAND original_pose_blend_tests "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-shared-assets")
add_executable(skinned_material_binding_tests tests/skinned_material_binding_tests.cpp)
target_link_libraries(skinned_material_binding_tests PRIVATE foundation_data)
add_test(NAME skinned_material_binding COMMAND skinned_material_binding_tests "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-shared-assets" "${CMAKE_CURRENT_BINARY_DIR}/skinned-material-binding-fixture")
add_executable(character_pose_seam_tests tests/character_pose_seam_tests.cpp)
target_link_libraries(character_pose_seam_tests PRIVATE foundation_data)
add_test(NAME character_pose_seam COMMAND character_pose_seam_tests "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-shared-assets")
add_executable(retained_pose_playback_tests tests/retained_pose_playback_tests.cpp)
target_link_libraries(retained_pose_playback_tests PRIVATE foundation_data)
add_test(NAME retained_pose_playback COMMAND retained_pose_playback_tests "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-shared-assets")
add_executable(original_campaign_runtime_tests tests/original_campaign_runtime_tests.cpp)
target_link_libraries(original_campaign_runtime_tests PRIVATE foundation_data)
add_test(NAME original_campaign_runtime COMMAND original_campaign_runtime_tests "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-encounter-source")
add_executable(npc_root_sample_tests tests/npc_root_sample_tests.cpp)
target_link_libraries(npc_root_sample_tests PRIVATE foundation_data)
add_test(NAME npc_root_sample COMMAND npc_root_sample_tests "${CMAKE_CURRENT_SOURCE_DIR}/../..")
add_executable(retained_animation_owner_tests tests/retained_animation_owner_tests.cpp)
target_link_libraries(retained_animation_owner_tests PRIVATE foundation_data)
add_test(NAME retained_animation_owner COMMAND retained_animation_owner_tests "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-shared-assets")
add_executable(campaign_camera_adapter_tests tests/campaign_camera_adapter_tests.cpp)
target_link_libraries(campaign_camera_adapter_tests PRIVATE foundation_data)
add_test(NAME campaign_camera_adapter COMMAND campaign_camera_adapter_tests)
add_executable(retained_sequence_playback_tests tests/retained_sequence_playback_tests.cpp)
target_link_libraries(retained_sequence_playback_tests PRIVATE foundation_data)
add_test(NAME retained_sequence_playback COMMAND retained_sequence_playback_tests "${CMAKE_CURRENT_SOURCE_DIR}/../..")
add_executable(original_camera_transition_tests tests/original_camera_transition_tests.cpp)
target_link_libraries(original_camera_transition_tests PRIVATE foundation_data)
add_test(NAME original_camera_transition COMMAND original_camera_transition_tests "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-shared-assets")
add_executable(original_actor_camera_anchor_tests tests/original_actor_camera_anchor_tests.cpp)
target_link_libraries(original_actor_camera_anchor_tests PRIVATE foundation_data)
add_test(NAME original_actor_camera_anchor COMMAND original_actor_camera_anchor_tests "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-shared-assets")
add_executable(modular_defaults_tests tests/modular_defaults_tests.cpp)
target_link_libraries(modular_defaults_tests PRIVATE foundation_data)
add_test(NAME modular_defaults COMMAND modular_defaults_tests "${CMAKE_CURRENT_SOURCE_DIR}/../..")
target_sources(foundation_data PRIVATE original_actor_lifecycle.cpp)
add_executable(original_actor_lifecycle_tests tests/original_actor_lifecycle_tests.cpp)
target_link_libraries(original_actor_lifecycle_tests PRIVATE foundation_data)
add_test(NAME original_actor_lifecycle COMMAND original_actor_lifecycle_tests)
target_sources(foundation_data PRIVATE source_world_objects.cpp)
add_executable(source_world_objects_tests tests/source_world_objects_tests.cpp)
target_link_libraries(source_world_objects_tests PRIVATE foundation_data)
add_test(NAME source_world_objects COMMAND source_world_objects_tests "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-shared-assets")
add_executable(deferred_population_tests tests/deferred_population_tests.cpp)
target_link_libraries(deferred_population_tests PRIVATE foundation_data)
add_test(NAME deferred_population COMMAND deferred_population_tests "${CMAKE_CURRENT_SOURCE_DIR}/../..")
target_sources(foundation_data PRIVATE original_campaign_world_adapter.cpp)
add_executable(original_campaign_world_adapter_tests tests/original_campaign_world_adapter_tests.cpp)
target_link_libraries(original_campaign_world_adapter_tests PRIVATE foundation_data)
add_test(NAME original_campaign_world_adapter COMMAND original_campaign_world_adapter_tests "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-encounter-source")
# P16 HOST (Preview 16): campaign script-host providers, executor dispatch router and generic trigger zones.
# Additive; the live binding is off unless the EXE gets --campaign-triggers.
target_sources(foundation_data PRIVATE features/campaign_host/campaign_host.cpp features/campaign_host/trigger_zones.cpp features/cinematics/source_campaign_dispatch_v1.cpp)
add_executable(campaign_host_tests features/campaign_host/campaign_host_tests.cpp)
target_link_libraries(campaign_host_tests PRIVATE foundation_data)
add_test(NAME campaign_host COMMAND campaign_host_tests "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-source-clock-v19-preview-15-rc3/assets")
add_executable(campaign_trigger_zone_tests features/campaign_host/trigger_zone_tests.cpp)
target_link_libraries(campaign_trigger_zone_tests PRIVATE foundation_data)
add_test(NAME campaign_trigger_zones COMMAND campaign_trigger_zone_tests "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-source-clock-v19-preview-15-rc3/assets" "${CMAKE_CURRENT_SOURCE_DIR}/assets/original-cache" "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/assets-extra/android")
# P16 CINE (Preview 16): cinematic runner (caption queue, cutscene contract, SKIP hit test, frame description).
target_sources(foundation_data PRIVATE features/cinematic_runner/cinematic_runner.cpp)
add_executable(cinematic_runner_tests features/cinematic_runner/cinematic_runner_tests.cpp)
target_link_libraries(cinematic_runner_tests PRIVATE foundation_data)
add_test(NAME cinematic_runner COMMAND cinematic_runner_tests)
target_sources(foundation_data PRIVATE source_module_trace.cpp)
add_executable(source_module_trace_tests tests/source_module_trace_tests.cpp)
target_link_libraries(source_module_trace_tests PRIVATE foundation_data)
add_test(NAME source_module_trace COMMAND source_module_trace_tests)
add_executable(run_binding_tests tests/run_binding_tests.cpp)
target_link_libraries(run_binding_tests PRIVATE foundation_data)
add_test(NAME run_binding COMMAND run_binding_tests "${CMAKE_CURRENT_SOURCE_DIR}/../..")
include(cmake/OriginalTriggerOwner.cmake)
target_sources(foundation_data PRIVATE original_trigger_contacts.cpp)
target_link_libraries(foundation_data PUBLIC recovered_trigger_contacts)
add_executable(original_trigger_contacts_tests tests/original_trigger_contacts_tests.cpp)
target_link_libraries(original_trigger_contacts_tests PRIVATE foundation_data)
add_test(NAME original_trigger_contacts COMMAND original_trigger_contacts_tests)
target_sources(foundation_data PRIVATE actor_lighting.cpp original_actor_bounds.cpp source_material_pass.cpp
  ../level-world/character_world_npc_bounds_v1.cpp ../level-world/character_scene.cpp
  ../level-world/decor_scene.cpp ../level-world/decor_body_config.cpp
  ../scene-materials/effect_render_pass_v4.cpp ../scene-materials/blood_render_pass_v3.cpp)
add_executable(actor_lighting_tests tests/actor_lighting_tests.cpp)
target_link_libraries(actor_lighting_tests PRIVATE foundation_data)
add_test(NAME actor_lighting COMMAND actor_lighting_tests "${CMAKE_CURRENT_SOURCE_DIR}/assets/original-cache/data/3d/light/swamp.lightset_xml")
add_executable(original_actor_bounds_tests tests/original_actor_bounds_tests.cpp)
target_link_libraries(original_actor_bounds_tests PRIVATE foundation_data)
add_test(NAME original_actor_bounds COMMAND original_actor_bounds_tests "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-shared-assets")
if(WIN32)
  add_executable(source_material_pass_tests tests/source_material_pass_tests.cpp renderer.cpp ${DH2_PLATFORM_SOURCES} render_queue.cpp)
  target_link_libraries(source_material_pass_tests PRIVATE foundation_data ${DH2_PLATFORM_LIBRARIES})
  add_test(NAME source_material_pass COMMAND source_material_pass_tests "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-shared-assets")
endif()

target_sources(foundation_data PRIVATE original_controller_commands.cpp
  ../level-world/character_controller_commands.cpp ../level-world/character_ai_attack.cpp
  ../level-world/character_animation_events.cpp source_actor_camera_anchors.cpp
  ../level-world/gameplay_camera_anchor_v6.cpp)
add_executable(original_controller_commands_tests tests/original_controller_commands_tests.cpp)
target_link_libraries(original_controller_commands_tests PRIVATE foundation_data)
add_test(NAME original_controller_commands COMMAND original_controller_commands_tests)
add_executable(source_actor_camera_anchors_tests tests/source_actor_camera_anchors_tests.cpp)
target_link_libraries(source_actor_camera_anchors_tests PRIVATE foundation_data)
add_test(NAME source_actor_camera_anchors COMMAND source_actor_camera_anchors_tests)

target_sources(foundation_data PRIVATE original_actor_physical.cpp
  ../level-world/character_body_config.cpp ../level-world/character_set_position_v7.cpp
  ../level-world/native_body.cpp ../level-world/physical_world.cpp)
target_link_libraries(foundation_data PUBLIC dh2_box2d_201)
add_executable(original_actor_physical_tests tests/original_actor_physical_tests.cpp)
target_link_libraries(original_actor_physical_tests PRIVATE foundation_data)
add_test(NAME original_actor_physical COMMAND original_actor_physical_tests "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-shared-assets")

target_sources(foundation_data PRIVATE source_level_construction.cpp
  ../level-loader/xml_document_v1.cpp ../level-loader/level_file_walk_v1.cpp
  ../level-loader/object_entry_v1.cpp ../level-loader/canonical_source_adapter_v1.cpp
  ../level-world/canonical_object_factory_v1.cpp ../level-world/canonical_object_manager_v1.cpp
  ../level-world/object_manager_language_registry_v1.cpp ../level-world/canonical_room_lists_v105.cpp
  ../level-world/source_assertion_process_v76.cpp)
target_include_directories(foundation_data PUBLIC ../level-world ../level-loader ../level-loader/vendor/tinyxml)
add_executable(source_level_construction_tests tests/source_level_construction_tests.cpp)
target_link_libraries(source_level_construction_tests PRIVATE foundation_data)
add_test(NAME source_level_construction COMMAND source_level_construction_tests "${CMAKE_CURRENT_SOURCE_DIR}/../..")

target_sources(recovered_content PRIVATE
  ../level-world/navigation_producers.cpp ../level-world/navigation_objects.cpp
  ../level-world/navigation_motion.cpp ../level-world/navigation_world.cpp
  ../level-world/navigation_path.cpp ../level-world/navigation_search.cpp ../level-world/navigation.cpp
  ../level-world/collision.cpp ../level-world/octree.cpp ../level-world/selector.cpp
  ../level-world/floor_source.cpp ../level-world/floors.cpp
  ../level-world/module_floor_append_v2.cpp ../level-world/user_properties_v2.cpp)
target_sources(foundation_data PRIVATE original_actor_navigation.cpp)
add_executable(original_actor_navigation_tests tests/original_actor_navigation_tests.cpp)
target_link_libraries(original_actor_navigation_tests PRIVATE foundation_data)
add_test(NAME original_actor_navigation COMMAND original_actor_navigation_tests "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-shared-assets")

target_sources(foundation_data PRIVATE original_actor_animation_events.cpp ../level-world/character_ai_events.cpp)
add_executable(original_actor_animation_events_tests tests/original_actor_animation_events_tests.cpp)
target_link_libraries(original_actor_animation_events_tests PRIVATE foundation_data)
add_test(NAME original_actor_animation_events COMMAND original_actor_animation_events_tests)

target_sources(foundation_data PRIVATE source_level_config.cpp
  ../level-world/canonical_property_map_v1.cpp ../level-world/canonical_level_config_module_v1.cpp
  ../level-world/character_design_services.cpp)
add_executable(source_level_config_tests tests/source_level_config_tests.cpp)
target_link_libraries(source_level_config_tests PRIVATE foundation_data)
add_test(NAME source_level_config COMMAND source_level_config_tests "${CMAKE_CURRENT_SOURCE_DIR}/../..")

target_sources(foundation_data PRIVATE original_actor_body_plan.cpp playable_actor_bodies.cpp
  original_actor_collision_filter.cpp ../level-world/native_physical_filter_v1.cpp
  source_module_construction.cpp ../level-world/canonical_level_module_bindings_v2.cpp)
add_executable(original_actor_body_plan_tests tests/original_actor_body_plan_tests.cpp)
target_link_libraries(original_actor_body_plan_tests PRIVATE foundation_data)
add_test(NAME original_actor_body_plan COMMAND original_actor_body_plan_tests "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-shared-assets")
add_executable(playable_actor_bodies_tests tests/playable_actor_bodies_tests.cpp)
target_link_libraries(playable_actor_bodies_tests PRIVATE foundation_data)
add_test(NAME playable_actor_bodies COMMAND playable_actor_bodies_tests "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-shared-assets")
add_executable(original_actor_collision_filter_tests tests/original_actor_collision_filter_tests.cpp)
target_link_libraries(original_actor_collision_filter_tests PRIVATE foundation_data)
add_test(NAME original_actor_collision_filter COMMAND original_actor_collision_filter_tests)
add_executable(source_module_construction_tests tests/source_module_construction_tests.cpp)
target_link_libraries(source_module_construction_tests PRIVATE foundation_data)
add_test(NAME source_module_construction COMMAND source_module_construction_tests "${CMAKE_CURRENT_SOURCE_DIR}/../..")
target_sources(foundation_data PRIVATE source_root_scopes.cpp
  ../level-world/application_spawn_random_owner_v4.cpp
  ../game-data/loot_tables_v2.cpp
  ../level-world/canonical_gameobject_base_owner_v1.cpp
  ../level-world/game_object_initialization_owner_v1.cpp
  ../level-world/condition_data_init_v3.cpp
  ../level-world/native_conditions_table_v69.cpp ../level-world/native_conditions_runtime_v69.cpp)
target_include_directories(foundation_data PUBLIC ../game-data)
add_executable(source_root_scopes_tests tests/source_root_scopes_tests.cpp)
target_link_libraries(source_root_scopes_tests PRIVATE foundation_data)
add_test(NAME source_root_scopes COMMAND source_root_scopes_tests "${CMAKE_CURRENT_SOURCE_DIR}/../..")
target_sources(foundation_data PRIVATE source_module_floors.cpp
  ../level-world/module_selected_scene_v2.cpp ../level-world/module_floor_graph_v3.cpp)
add_executable(source_module_floors_tests tests/source_module_floors_tests.cpp)
target_link_libraries(source_module_floors_tests PRIVATE foundation_data)
add_test(NAME source_module_floors COMMAND source_module_floors_tests "${CMAKE_CURRENT_SOURCE_DIR}/../..")
target_sources(foundation_data PRIVATE original_actor_motion_flags.cpp
  ../level-world/navigation_controller.cpp ../level-world/navigation_avoidance.cpp ../level-world/move_state.cpp)
add_executable(source_motion_flags_tests tests/source_motion_flags_tests.cpp)
target_link_libraries(source_motion_flags_tests PRIVATE foundation_data)
add_test(NAME source_motion_flags COMMAND source_motion_flags_tests)
target_sources(foundation_data PRIVATE original_actor_motion_frame.cpp)
add_executable(original_actor_motion_frame_tests tests/original_actor_motion_frame_tests.cpp)
target_link_libraries(original_actor_motion_frame_tests PRIVATE foundation_data)
add_test(NAME original_actor_motion_frame COMMAND original_actor_motion_frame_tests)
target_sources(foundation_data PRIVATE original_actor_target_position.cpp)
add_executable(original_actor_target_position_tests tests/original_actor_target_position_tests.cpp)
target_link_libraries(original_actor_target_position_tests PRIVATE foundation_data)
add_test(NAME original_actor_target_position COMMAND original_actor_target_position_tests "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-shared-assets")
target_sources(foundation_data PRIVATE original_actor_subobjects.cpp ../level-world/subobjects_update.cpp)
add_executable(original_actor_subobjects_tests tests/original_actor_subobjects_tests.cpp)
target_link_libraries(original_actor_subobjects_tests PRIVATE foundation_data)
add_test(NAME original_actor_subobjects COMMAND original_actor_subobjects_tests "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-shared-assets")
target_sources(foundation_data PRIVATE features/character_menu/character_menu.cpp features/character_menu/original_art.cpp features/platform_input/semantic_input.cpp)
target_sources(foundation_data PRIVATE features/character_menu/menu_text.cpp)
target_sources(foundation_data PRIVATE features/character_menu/menu_stats.cpp)
target_sources(foundation_data PRIVATE ../engine-ui/hud_text_v1.cpp ../engine-ui/hud_text_format_v1.cpp)
add_executable(hud_geometry_tests tests/hud_geometry_tests.cpp)
target_link_libraries(hud_geometry_tests PRIVATE foundation_data)
add_test(NAME hud_geometry COMMAND hud_geometry_tests)
# B038: original XP bar (shape 148/150) rendered-fill test; minimal addition beside hud_geometry.
add_executable(hud_xp_bar_tests tests/hud_xp_bar_tests.cpp)
target_link_libraries(hud_xp_bar_tests PRIVATE foundation_data)
add_test(NAME hud_xp_bar COMMAND hud_xp_bar_tests)
add_executable(character_menu_tests features/character_menu/character_menu_tests.cpp)
target_link_libraries(character_menu_tests PRIVATE foundation_data)
add_test(NAME character_menu COMMAND character_menu_tests)
add_executable(semantic_input_tests features/platform_input/semantic_input_tests.cpp)
target_link_libraries(semantic_input_tests PRIVATE foundation_data)
add_test(NAME semantic_input COMMAND semantic_input_tests)
add_executable(menu_text_tests features/character_menu/menu_text_tests.cpp ../engine-ui/text_layout_v1.cpp)
target_link_libraries(menu_text_tests PRIVATE foundation_data)
add_test(NAME menu_text COMMAND menu_text_tests "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-shared-assets")
# Preview 14 FAERY: CharacterState Faery page provider (source-table + English localization inputs).
add_executable(character_state_page_tests features/faery_menu/tests/character_state_page_tests.cpp)
target_link_libraries(character_state_page_tests PRIVATE foundation_runtime_skill_cast foundation_data)
add_test(NAME character_state_page COMMAND character_state_page_tests
  "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/character-skill-session-v2/cache/data/pydata/faeries_pyarray.bin"
  "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/character-skill-session-v2/cache/data/pydata/faeries_pyarraynames.bin"
  "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/character-skill-session-v2/cache/data/pydata/faeries_pystructnames.bin"
  "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-main-frontend-v1/assets")
# P15 FAERYSOUND (B050): Celest/Hotty Pre sound labels per target count.
add_executable(faery_cast_sound_v1_tests features/faery_menu/tests/faery_cast_sound_v1_tests.cpp)
target_link_libraries(faery_cast_sound_v1_tests PRIVATE foundation_runtime_skill_cast)
add_test(NAME faery_cast_sound_v1 COMMAND faery_cast_sound_v1_tests)

target_sources(foundation_data PRIVATE features/combo_chain/source_combo_chain.cpp
  ../level-world/character_attack_animation_v1.cpp ../level-world/character_animation_ai.cpp
  ../game-data/animation_selection.cpp ../game-data/animation_scheduler.cpp)
add_executable(source_combo_chain_tests features/combo_chain/source_combo_chain_tests.cpp)
target_link_libraries(source_combo_chain_tests PRIVATE foundation_data)
add_test(NAME source_combo_chain COMMAND source_combo_chain_tests "${CMAKE_CURRENT_SOURCE_DIR}/../..")
add_executable(combat_session_combo_tests features/combo_chain/combat_session_combo_tests.cpp)
target_link_libraries(combat_session_combo_tests PRIVATE foundation_data)
add_test(NAME combat_session_combo COMMAND combat_session_combo_tests "${CMAKE_CURRENT_SOURCE_DIR}/../..")
# Preview 13 B035 push (knockback) session binder: library source plus focused test.
# The binder is not yet bound by dh-foundation.exe (main.cpp unchanged).
target_sources(foundation_data PRIVATE features/combat/b035_session_push_effect_v1.cpp)
add_executable(b035_session_push_effect_v1_tests features/combat/b035_session_push_effect_v1_tests.cpp)
target_link_libraries(b035_session_push_effect_v1_tests PRIVATE foundation_runtime_equipment_menu foundation_data)
add_test(NAME b035_session_push_effect_v1 COMMAND b035_session_push_effect_v1_tests "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-source-clock-v19-preview-12/assets")

target_sources(foundation_data PRIVATE features/combat_text/combat_text.cpp
  features/combat_text/combat_text_design.cpp features/combat_text/combat_text_live.cpp
  ../level-world/character_combat_text_v1.cpp ../engine-ui/combat_flash_inputs_v1.cpp)
add_executable(combat_text_tests features/combat_text/tests.cpp)
target_link_libraries(combat_text_tests PRIVATE foundation_data)
add_test(NAME combat_text COMMAND combat_text_tests "${CMAKE_CURRENT_SOURCE_DIR}/../..")
add_executable(combat_text_live_tests features/combat_text/live_tests.cpp)
target_link_libraries(combat_text_live_tests PRIVATE foundation_data)
add_test(NAME combat_text_live COMMAND combat_text_live_tests "${CMAKE_CURRENT_SOURCE_DIR}/../..")

# Portable gameplay controller; original tables/rules are data dependencies,
# not a dependency on the recovered engine's Character initialization graph.
add_library(foundation_runtime_enemy STATIC
  features/physics/session_actor_transition_v1.cpp
  features/physics/runtime_session_contact_v1.cpp
  ../level-world/character_script_collision.cpp
  ../level-world/character_cancel_sneaking.cpp
  ../level-world/character_sneaking_tables.cpp
  features/enemy_ai/runtime_enemy_contact_binding_v1.cpp
  features/enemy_ai/runtime_enemy_controller_v1.cpp
  features/enemy_ai/runtime_enemy_navigation_v1.cpp
  features/enemy_ai/monster_decisions.cpp)
target_link_libraries(foundation_runtime_enemy PUBLIC foundation_data)
target_link_libraries(dh-foundation PRIVATE foundation_runtime_enemy)

add_library(foundation_runtime_loot STATIC
  features/loot/runtime_death_rewards_v1.cpp
  features/loot/runtime_session_death_rewards_v1.cpp
  features/loot/runtime_loot_source_owner_v1.cpp
  features/loot/runtime_world_item_adapter_v1.cpp
  features/loot/runtime_world_item_interaction_v1.cpp
  ../level-world/player_progression_v1.cpp
  ../game-data/loot_table_selection_v8.cpp
  ../game-data/loot_item_selection_v8.cpp
  ../game-data/loot_entry_selection_v8.cpp
  ../game-data/loot_power_creation_v7.cpp
  ../game-data/loot_power_resources_v7.cpp
  ../game-data/loot_audiovisual_v8.cpp)
target_link_libraries(foundation_runtime_loot PUBLIC foundation_data foundation_frontend foundation_runtime_equipment_menu)
target_link_libraries(dh-foundation PRIVATE foundation_runtime_loot)

# P14 DROPS: world item rules, source item-drop renderer projection and its
# focused test (rules/pool/scatter/pickup gates over the actual ItemTable).
target_sources(foundation_runtime_loot PRIVATE features/loot/world_drop_rules_v1.cpp)
# P15 B048: item drop/pickup sound ordinals (ItemAudioVisualTable rows) and their focused test.
target_sources(foundation_runtime_loot PRIVATE features/loot/world_item_sound_v1.cpp)
add_executable(world_item_sound_v1_tests features/loot/world_item_sound_v1_tests.cpp)
target_link_libraries(world_item_sound_v1_tests PRIVATE foundation_runtime_loot foundation_data)
add_test(NAME world_item_sound_v1 COMMAND world_item_sound_v1_tests
  "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-source-clock-v19-preview-14-rc1/assets")
target_sources(dh-foundation PRIVATE
  features/interactions/session_world_item_consumer_v1.cpp
  features/interactions/source_world_item_drop_render_v1.cpp
  features/interactions/source_world_item_drop_material_v1.cpp
  features/interactions/world_drop_runtime_v1.cpp
  ../engine-ui/character_menu_font_palette_v1.cpp)
# P15 LEVELUP (I026): level-up FX presentation outcome branches over original EffectsTables.
add_executable(runtime_level_up_presentation_v1_tests features/effects/runtime_level_up_presentation_v1_tests.cpp
  features/effects/runtime_level_up_presentation_v1.cpp ../game-data/effects_tables.cpp ../game-data/data.cpp)
target_link_libraries(runtime_level_up_presentation_v1_tests PRIVATE foundation_data)
add_test(NAME runtime_level_up_presentation_v1 COMMAND runtime_level_up_presentation_v1_tests
  "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-source-clock-v19-preview-14-rc1/assets/data")

add_executable(world_drop_rules_v1_tests features/loot/world_drop_rules_v1_tests.cpp)
target_link_libraries(world_drop_rules_v1_tests PRIVATE foundation_runtime_loot foundation_data)
add_test(NAME world_drop_rules_v1 COMMAND world_drop_rules_v1_tests
  "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-shared-assets")

add_executable(neutral_world_save_tests tests/neutral_world_save_tests.cpp)
target_link_libraries(neutral_world_save_tests PRIVATE foundation_data)
add_test(NAME neutral_world_save COMMAND neutral_world_save_tests
  "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-shared-assets"
  "${CMAKE_CURRENT_BINARY_DIR}/neutral-world-test.save")

add_executable(combat_session_target_cycle_tests tests/combat_session_target_cycle_tests.cpp)
target_link_libraries(combat_session_target_cycle_tests PRIVATE foundation_data)
add_test(NAME combat_session_target_cycle COMMAND combat_session_target_cycle_tests
  "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-shared-assets")

add_executable(player_profile_properties_tests tests/player_profile_properties_tests.cpp)
target_link_libraries(player_profile_properties_tests PRIVATE foundation_runtime_equipment_menu)
add_test(NAME player_profile_properties COMMAND player_profile_properties_tests
  "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-shared-assets"
  "${CMAKE_CURRENT_BINARY_DIR}")

# P14 EQUIP: adapter (equip/auto-equip/class gate/ordering) and main-page (ALL banner/Details commands) tests.
add_executable(equipment_adapter_tests features/equipment/equipment_adapter_tests.cpp)
target_link_libraries(equipment_adapter_tests PRIVATE foundation_runtime_equipment_menu)
add_test(NAME equipment_adapter COMMAND equipment_adapter_tests
  "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-shared-assets")
add_executable(equipment_inventory_actions_tests features/equipment/equipment_inventory_actions_v1_tests.cpp)
target_link_libraries(equipment_inventory_actions_tests PRIVATE foundation_runtime_equipment_menu)
add_test(NAME equipment_inventory_actions COMMAND equipment_inventory_actions_tests)
add_executable(equipment_main_page_tests features/equipment/equipment_main_page_tests.cpp)
target_link_libraries(equipment_main_page_tests PRIVATE foundation_runtime_equipment_menu)
add_test(NAME equipment_main_page COMMAND equipment_main_page_tests
  "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-shared-assets")
# P14 EQUIP (B045): Details list row hit geometry.
add_executable(inventory_details_row_geometry_tests features/inventory/details_row_geometry_tests.cpp)
target_link_libraries(inventory_details_row_geometry_tests PRIVATE foundation_runtime_equipment_menu)
add_test(NAME inventory_details_row_geometry COMMAND inventory_details_row_geometry_tests)

add_executable(combat_session_equipment_multiplicity_tests tests/combat_session_equipment_multiplicity_tests.cpp)
target_link_libraries(combat_session_equipment_multiplicity_tests PRIVATE foundation_data)
add_test(NAME combat_session_equipment_multiplicity COMMAND combat_session_equipment_multiplicity_tests
  "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-shared-assets")

add_executable(combat_session_source_departure_tests tests/combat_session_source_departure_tests.cpp)
target_link_libraries(combat_session_source_departure_tests PRIVATE foundation_data)
add_test(NAME combat_session_source_departure COMMAND combat_session_source_departure_tests
  "${CMAKE_CURRENT_SOURCE_DIR}/../..")

add_executable(runtime_skill_session_training_tests features/generic_skills/runtime_skill_session_training_v1_tests.cpp)
target_link_libraries(runtime_skill_session_training_tests PRIVATE foundation_runtime_skills_menu)
add_test(NAME runtime_skill_session_training COMMAND runtime_skill_session_training_tests
  "${CMAKE_CURRENT_SOURCE_DIR}/../..")
# Preview 14 Stats +/- route: spend, refuse at zero, failed-save rollback, recalc (features/character_menu/stat_training_v1).
add_executable(stat_training_v1_tests features/character_menu/stat_training_v1_tests.cpp)
target_link_libraries(stat_training_v1_tests PRIVATE foundation_runtime_skills_menu)
add_test(NAME stat_training_v1 COMMAND stat_training_v1_tests "${CMAKE_CURRENT_SOURCE_DIR}/../..")

add_executable(combat_session_incoming_receiver_tests tests/combat_session_incoming_receiver_tests.cpp)
target_link_libraries(combat_session_incoming_receiver_tests PRIVATE foundation_runtime_enemy)
add_test(NAME combat_session_incoming_receiver COMMAND combat_session_incoming_receiver_tests
  "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-shared-assets")

add_executable(combat_session_source_attack_bank_tests tests/combat_session_source_attack_bank_tests.cpp)
target_link_libraries(combat_session_source_attack_bank_tests PRIVATE foundation_runtime_equipment_menu)
add_test(NAME combat_session_source_attack_bank COMMAND combat_session_source_attack_bank_tests
  "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-source-clock-v19-preview-9/assets")

add_executable(playable_actor_physics_frame_tests tests/playable_actor_physics_frame_tests.cpp)
target_link_libraries(playable_actor_physics_frame_tests PRIVATE foundation_data)
add_test(NAME playable_actor_physics_frame COMMAND playable_actor_physics_frame_tests
  "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-shared-assets")

add_executable(combat_session_motion_phase_tests tests/combat_session_motion_phase_tests.cpp)
target_link_libraries(combat_session_motion_phase_tests PRIVATE foundation_runtime_equipment_menu)
add_test(NAME combat_session_motion_phase COMMAND combat_session_motion_phase_tests
  "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-source-clock-v19-preview-9/assets")

add_executable(retained_seeded_completion_tests tests/retained_seeded_completion_tests.cpp)
target_link_libraries(retained_seeded_completion_tests PRIVATE foundation_data)
add_test(NAME retained_seeded_completion COMMAND retained_seeded_completion_tests
  "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-source-clock-v19-preview-9/assets")
set_tests_properties(retained_seeded_completion PROPERTIES
  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../..")

add_executable(combat_session_death_end_regression_tests features/combat/death_end_regression_tests.cpp)
target_link_libraries(combat_session_death_end_regression_tests PRIVATE foundation_data)
add_test(NAME combat_session_death_end_regression COMMAND combat_session_death_end_regression_tests
  "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-shared-assets")

add_executable(game_save_physical_presence_tests tests/game_save_physical_presence_tests.cpp)
target_link_libraries(game_save_physical_presence_tests PRIVATE foundation_data)
add_test(NAME game_save_physical_presence COMMAND game_save_physical_presence_tests
  "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-shared-assets"
  "${CMAKE_CURRENT_BINARY_DIR}/game-save-physical-presence-test.save")

add_executable(combat_session_reconstructible_notification_tests tests/combat_session_reconstructible_notification_tests.cpp)
target_link_libraries(combat_session_reconstructible_notification_tests PRIVATE foundation_data)
add_test(NAME combat_session_reconstructible_notification COMMAND combat_session_reconstructible_notification_tests
  "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-shared-assets"
  "${CMAKE_CURRENT_BINARY_DIR}/reconstructible-notification-test.save")

add_executable(combat_session_actor_transition_tests tests/combat_session_actor_transition_tests.cpp)
target_link_libraries(combat_session_actor_transition_tests PRIVATE foundation_runtime_equipment_menu)
add_test(NAME combat_session_actor_transition COMMAND combat_session_actor_transition_tests
  "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-source-clock-v19-preview-9/assets")

add_executable(session_actor_transition_physics_tests features/physics/session_actor_transition_v1_tests.cpp)
target_link_libraries(session_actor_transition_physics_tests PRIVATE foundation_runtime_enemy foundation_runtime_equipment_menu)
add_test(NAME session_actor_transition_physics COMMAND session_actor_transition_physics_tests
  "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-source-clock-v19-preview-9/assets")

add_executable(source_physical_filter_ops_tests tests/source_physical_filter_ops_tests.cpp)
target_link_libraries(source_physical_filter_ops_tests PRIVATE foundation_data)
add_test(NAME source_physical_filter_ops COMMAND source_physical_filter_ops_tests
  "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-shared-assets")

add_executable(playable_actor_body_construction_tests tests/playable_actor_body_construction_tests.cpp)
target_link_libraries(playable_actor_body_construction_tests PRIVATE foundation_data)
add_test(NAME playable_actor_body_construction COMMAND playable_actor_body_construction_tests
  "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-shared-assets")

add_executable(native_world_character_creation_failure_tests ../level-world/tests/native_world_character_creation_failure_v1.cpp)
target_link_libraries(native_world_character_creation_failure_tests PRIVATE foundation_data)
add_test(NAME native_world_character_creation_failure COMMAND native_world_character_creation_failure_tests)

add_executable(combat_session_presentation_state_tests tests/combat_session_presentation_state_tests.cpp)
target_link_libraries(combat_session_presentation_state_tests PRIVATE foundation_data)
add_test(NAME combat_session_presentation_state COMMAND combat_session_presentation_state_tests
  "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-shared-assets")

# P14 schema v4: CharacterState codec (difficulty, menu metadata, visited modules, CQPG v2 counters).
add_executable(schema_v4_tests tests/schema_v4_tests.cpp)
target_link_libraries(schema_v4_tests PRIVATE foundation_data)
add_test(NAME schema_v4 COMMAND schema_v4_tests)
# P14 schema: LevelList row 41 = 001_swamp (The Boglands), stamping rules, slot panel projection.
add_executable(menu_metadata_v1_tests features/menu_metadata/menu_metadata_v1_tests.cpp)
target_link_libraries(menu_metadata_v1_tests PRIVATE foundation_frontend)
add_test(NAME menu_metadata_v1 COMMAND menu_metadata_v1_tests
  "${CMAKE_CURRENT_SOURCE_DIR}/../../.local-inputs/windows-shared-assets"
  "${CMAKE_CURRENT_SOURCE_DIR}/../android-native/app/src/main/assets")
