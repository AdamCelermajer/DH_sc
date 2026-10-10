cmake_minimum_required(VERSION 3.22)
project(dh2_engine_ui LANGUAGES C CXX)
include(gameswf_sources.cmake)
include(gameswf_font_overlay_v1.cmake)
include(gameswf_input_overlay_v1.cmake)
include(gameswf_frame_overlay_v1.cmake)
include(gameswf_player_lifetime_overlay_v1.cmake)
include(gameswf_loader_lifetime_overlay_v1.cmake)
include(gameswf_edit_text_overlay_v1.cmake)
include(gameswf_source_facade_v1.cmake)
include(freetype237-hud.cmake)
# Host clients borrow the engine implemented inside dh2_engine_ui. Keep its
# header contract separate from the static object archive to avoid duplicate
# GameSWF globals when a test also links the world-to-UI adapter.
add_library(dh2_gameswf_headers INTERFACE)
target_include_directories(dh2_gameswf_headers SYSTEM INTERFACE "${DH2_GAMESWF_ROOT}")
target_include_directories(dh2_gameswf_headers SYSTEM BEFORE INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/overlays/edit-text-v1")
target_compile_definitions(dh2_gameswf_headers INTERFACE TU_CONFIG_LINK_TO_JPEGLIB=0 TU_CONFIG_LINK_TO_LIBPNG=0
  TU_CONFIG_LINK_TO_FREETYPE=0 TU_CONFIG_LINK_TO_THREAD=0)
target_link_libraries(dh2_gameswf_headers INTERFACE z ${CMAKE_DL_LIBS})
add_executable(splash_layout_v1_audit tests/splash_layout_v1.cpp)
target_include_directories(splash_layout_v1_audit PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_features(splash_layout_v1_audit PRIVATE cxx_std_17)
add_library(dh2_gameswf_core STATIC ${DH2_GAMESWF_SOURCES})
set_target_properties(dh2_gameswf_core PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_compile_features(dh2_gameswf_core PUBLIC cxx_std_17)
target_include_directories(dh2_gameswf_core SYSTEM PUBLIC "${DH2_GAMESWF_ROOT}")
target_include_directories(dh2_gameswf_core BEFORE PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/overlays/edit-text-v1")
target_include_directories(dh2_gameswf_core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_definitions(dh2_gameswf_core PUBLIC TU_CONFIG_LINK_TO_JPEGLIB=0 TU_CONFIG_LINK_TO_LIBPNG=0
  TU_CONFIG_LINK_TO_FREETYPE=0 TU_CONFIG_LINK_TO_THREAD=0)
# Disabled upstream JPEG declarations have no linked RTTI implementation. Keep
# address/other undefined checks; the frozen core proof records this vptr scope.
target_compile_options(dh2_gameswf_core PRIVATE -w -fno-fast-math -ffp-contract=off -fno-sanitize=vptr)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_compile_options(dh2_gameswf_core PRIVATE -fpermissive)
endif()
target_link_libraries(dh2_gameswf_core PUBLIC z ${CMAKE_DL_LIBS})
add_library(dh2_engine_ui SHARED gfnt.cpp swf_movie.cpp freetype_font.cpp
  freetype_glyph_kernel.cpp swf_freetype_provider.cpp swf_font_geometry.cpp
  swf_font_resolver.cpp viewport.cpp localization.cpp
  freetype_bitmap_alpha.cpp hud_freetype_font.cpp swf_hud_freetype_provider.cpp
  swf_glyph_lookup.cpp hud_player_values.cpp swf_viewport_connection.cpp
  hud_sprite_timeline.cpp hud_sprite_core.cpp hud_advance.cpp hud_advance_owner.cpp
  player_status_hud.cpp enemy_status_hud_v1.cpp enemy_hud_presentation_v2.cpp swf_actionscript_connection.cpp renderfx_text_connection.cpp
  hud_startup_callbacks.cpp game_option_table_v1.cpp owned_hud_settings_v1.cpp
  settings_native_files_v1.cpp settings_language_scene_v1.cpp
  hud_manager.cpp hud_manager_core.cpp text_layout_v1.cpp
  hud_initialization_v1.cpp hud_initialization_core_v1.cpp
  authored_gameplay_hud_v1.cpp
  authored_hud_edge_layout_v6.cpp
  authored_hurt_corners_layout_v7.cpp authored_hurt_pulse_v7.cpp
  authored_status_timeline_v27.cpp
  progression_xp_text_v23.cpp
  flash_anim_manager_v92.cpp
  multi_menu_resource_v93.cpp
  menu_status_messages_v26.cpp menu_status_movie_v26.cpp menu_message_cache_v66.cpp menu_dialog_messages_v97.cpp
  authored_joystick_v1.cpp
  character_menu_as_bridge_v1.cpp
  hud_manager_core_v2.cpp text_display_v2.cpp text_render_owner_v2.cpp
  hud_freetype_font_v2.cpp hud_text_format_v1.cpp hud_text_v1.cpp)
target_sources(dh2_engine_ui PRIVATE item_text_varargs_v5.cpp item_text_owner_v5.cpp)
# The process Debug owner is shared by UI before gameplay exists. Compile its
# sole implementation below world, avoiding a UI -> world dependency cycle.
target_sources(dh2_engine_ui PRIVATE ../level-world/character_design_services.cpp)
target_sources(dh2_engine_ui PRIVATE settings_language_scene_v2.cpp
  item_language_localization_v1.cpp authored_menu_application_fields_v3.cpp
  authored_menu_touchscreen_v3.cpp authored_menu_deadzones_v3.cpp)
target_sources(dh2_engine_ui PRIVATE authored_character_panel_platform_v4.cpp
  authored_menu_character_projection_v4.cpp authored_menu_movie_stack_v4.cpp
  authored_hud_portrait_v4.cpp character_menu_application_v4.cpp
  character_menu_application_native_v4.cpp)
target_sources(dh2_engine_ui PRIVATE character_menu_potions_v4.cpp
  character_menu_potion_text_v4.cpp)
target_sources(dh2_engine_ui PRIVATE character_menu_gold_v4.cpp)
target_sources(dh2_engine_ui PRIVATE
  authored_character_panel_v2.cpp character_menu_movie_v1.cpp
  authored_shared_menu_roster_v27.cpp
  swf_vertex_cache_v36.cpp
  menu_stack_owner_v1.cpp menu_stack_actions_v1.cpp menu_stack_v1.cpp
  authored_character_menu_routes_v1.cpp authored_character_menu_session_v1.cpp
  authored_menu_lifecycle_v1.cpp authored_menu_localization_v1.cpp
  authored_menu_weak_character_v59.cpp
  menu_manager_unload_v58.cpp
  menu_manager_update_v58.cpp
  character_menu_reload_action_v1.cpp character_menu_reload_v1.cpp
  menu_rollover_input_v1.cpp)
target_sources(dh2_engine_ui PRIVATE hud_player_infos.cpp)
target_sources(dh2_engine_ui PRIVATE combat_flash_queue_v1.cpp combat_flash_swf_v1.cpp)
target_sources(dh2_engine_ui PRIVATE
  edit_text_definition_v1.cpp edit_text_event_v1.cpp edit_text_display_v1.cpp
  edit_text_field_v1.cpp edit_text_format_v1.cpp text_filter_v1.cpp
  swf_text_filter_attachment_v1.cpp swf_text_font_platform_v1.cpp swf_edit_text_connection_v1.cpp
  swf_grid_snap_v1.cpp gfnt_text_backend_v1.cpp)
target_sources(dh2_engine_ui PRIVATE
  swf_cursor_input.cpp swf_input_geometry.cpp swf_input_policy.cpp
  swf_controller_storage_v91.cpp
  swf_input_history.cpp swf_input_connection.cpp swf_input_connection_v2.cpp
  swf_event_dispatch.cpp swf_event_core.cpp swf_frame_schedule.cpp
  swf_frame_connection.cpp swf_drag_values.cpp swf_source_movie_v1.cpp
  swf_input_session_v1.cpp swf_input_session_v2.cpp)
# One authoritative facade and one body for each versioned session wrapper.
get_target_property(DH2_UI_SOURCE_FACADE_SOURCES dh2_engine_ui SOURCES)
dh2_select_source_facade_v1(DH2_UI_SOURCE_FACADE_SOURCES)
set_property(TARGET dh2_engine_ui PROPERTY SOURCES "${DH2_UI_SOURCE_FACADE_SOURCES}")
target_compile_features(dh2_engine_ui PUBLIC cxx_std_17)
target_include_directories(dh2_engine_ui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(dh2_engine_ui PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../level-loader")
target_include_directories(dh2_engine_ui PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../game-data")
target_include_directories(dh2_engine_ui BEFORE PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/overlays/edit-text-v1")
target_compile_options(dh2_engine_ui PRIVATE -Wall -Wextra -fno-fast-math -ffp-contract=off)
target_link_libraries(dh2_engine_ui PRIVATE dh2_gameswf_core dh2_freetype237)
target_link_libraries(dh2_engine_ui PRIVATE dh2_game_data)
if(ANDROID)
  target_link_options(dh2_engine_ui PRIVATE -Wl,-z,max-page-size=16384 -Wl,--no-undefined)
else()
  add_executable(swf_assign_save_slot_args_v1_audit tests/swf_assign_save_slot_args_v1.cpp)
  target_link_libraries(swf_assign_save_slot_args_v1_audit PRIVATE dh2_engine_ui dh2_gameswf_headers)
  target_compile_options(swf_assign_save_slot_args_v1_audit PRIVATE -fno-sanitize=vptr)
  add_executable(authored_menu_weak_character_v59_audit tests/authored_menu_weak_character_v59.cpp)
  target_link_libraries(authored_menu_weak_character_v59_audit PRIVATE dh2_engine_ui dh2_gameswf_headers)
  target_compile_options(authored_menu_weak_character_v59_audit PRIVATE -fno-sanitize=vptr)
  add_executable(menu_manager_lifecycle_v58_audit tests/menu_manager_lifecycle_v58.cpp
    menu_manager_unload_v58.cpp menu_manager_update_v58.cpp authored_menu_application_fields_v3.cpp)
  target_include_directories(menu_manager_lifecycle_v58_audit PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
  target_compile_features(menu_manager_lifecycle_v58_audit PRIVATE cxx_std_17)
  add_executable(combat_flash_swf_v1_audit tests/combat_flash_swf_v1.cpp)
  target_compile_options(combat_flash_swf_v1_audit PRIVATE -fno-sanitize=vptr)
  target_link_libraries(combat_flash_swf_v1_audit PRIVATE dh2_engine_ui dh2_level_world dh2_script_runtime dh2_freetype237 dh2_gameswf_headers)
  add_executable(item_text_varargs_v5_audit tests/item_text_varargs_v5.cpp
    tests/item_text_varargs_v5_fixture.cpp)
  target_link_libraries(item_text_varargs_v5_audit PRIVATE dh2_engine_ui)
  add_executable(swf_input_session_v1_audit tests/swf_input_session_v1.cpp)
  target_compile_options(swf_input_session_v1_audit PRIVATE -fno-sanitize=vptr)
  target_link_libraries(swf_input_session_v1_audit PRIVATE dh2_engine_ui dh2_gameswf_headers)
  add_executable(swf_source_session_v2_audit tests/swf_source_session_v2.cpp
    ../android-native/app/src/main/cpp/original_ui_input_session_v1.cpp)
  target_compile_options(swf_source_session_v2_audit PRIVATE -fno-sanitize=vptr)
  target_include_directories(swf_source_session_v2_audit PRIVATE ../android-native/app/src/main/cpp)
  target_link_libraries(swf_source_session_v2_audit PRIVATE dh2_engine_ui dh2_gameswf_headers)
  add_executable(swf_cursor_input_v1_audit tests/swf_cursor_input.cpp)
  target_compile_options(swf_cursor_input_v1_audit PRIVATE -fno-sanitize=vptr)
  target_link_libraries(swf_cursor_input_v1_audit PRIVATE dh2_engine_ui dh2_gameswf_headers)
  add_executable(swf_frame_connection_v1_audit tests/swf_frame_connection.cpp)
  target_compile_options(swf_frame_connection_v1_audit PRIVATE -fno-sanitize=vptr)
  target_link_libraries(swf_frame_connection_v1_audit PRIVATE dh2_engine_ui dh2_gameswf_headers)
  add_executable(swf_frame_gold_v1_audit tests/swf_frame_gold.cpp)
  target_compile_options(swf_frame_gold_v1_audit PRIVATE -fno-sanitize=vptr)
  target_link_libraries(swf_frame_gold_v1_audit PRIVATE dh2_engine_ui dh2_gameswf_headers)
  add_executable(swf_cursor_gold_v1_audit tests/swf_cursor_input_gold.cpp tests/swf_cursor_gold_libm_v1.cpp)
  target_link_options(swf_cursor_gold_v1_audit PRIVATE -Wl,--export-dynamic-symbol=sinf -Wl,--export-dynamic-symbol=cosf -Wl,--export-dynamic-symbol=sincosf)
  target_link_libraries(swf_cursor_gold_v1_audit PRIVATE dh2_engine_ui)
  add_executable(hud_text_format_v1_audit tests/hud_text_format_v1.cpp)
  target_link_libraries(hud_text_format_v1_audit PRIVATE dh2_engine_ui)
  add_executable(hud_skill_text_v1_audit tests/hud_skill_text_v1.cpp)
  target_link_libraries(hud_skill_text_v1_audit PRIVATE dh2_engine_ui dh2_level_world dh2_game_data dh2_script_runtime)
  add_executable(text_display_v2_audit tests/text_display_v2.cpp)
  target_link_libraries(text_display_v2_audit PRIVATE dh2_engine_ui)
  add_executable(text_render_owner_v2_audit tests/text_render_owner_v2.cpp)
  target_link_libraries(text_render_owner_v2_audit PRIVATE dh2_engine_ui dh2_freetype237)
  add_executable(hud_initialization_v1_audit tests/hud_initialization_v1.cpp)
  target_link_libraries(hud_initialization_v1_audit PRIVATE dh2_engine_ui)
  add_executable(hud_initialization_core_v1_audit tests/hud_initialization_core_v1.cpp)
  target_compile_options(hud_initialization_core_v1_audit PRIVATE -fno-sanitize=vptr)
  target_link_libraries(hud_initialization_core_v1_audit PRIVATE dh2_engine_ui dh2_level_world dh2_game_data dh2_gameswf_headers)
  add_executable(text_layout_v1_audit tests/text_layout_v1.cpp)
  target_link_libraries(text_layout_v1_audit PRIVATE dh2_engine_ui)
  add_executable(hud_manager_audit tests/hud_manager.cpp)
  target_link_libraries(hud_manager_audit PRIVATE dh2_engine_ui)
  add_executable(hud_manager_reentry_audit tests/hud_manager_reentry.cpp)
  target_link_libraries(hud_manager_reentry_audit PRIVATE dh2_engine_ui)
  add_executable(hud_manager_backends_audit tests/hud_manager_backends.cpp)
  target_link_libraries(hud_manager_backends_audit PRIVATE dh2_engine_ui dh2_level_world)
  add_executable(hud_manager_core_audit tests/hud_manager_core.cpp)
  target_compile_options(hud_manager_core_audit PRIVATE -fno-sanitize=vptr)
  target_link_libraries(hud_manager_core_audit PRIVATE dh2_engine_ui dh2_level_world dh2_script_runtime dh2_freetype237 dh2_gameswf_headers)
  add_executable(enemy_status_hud_v1_audit tests/enemy_status_hud_v1.cpp)
  target_compile_options(enemy_status_hud_v1_audit PRIVATE -fno-sanitize=vptr)
  target_link_libraries(enemy_status_hud_v1_audit PRIVATE dh2_engine_ui dh2_level_world dh2_script_runtime dh2_freetype237 dh2_gameswf_headers)
  add_executable(hud_startup_audit tests/hud_startup.cpp)
  target_link_libraries(hud_startup_audit PRIVATE dh2_engine_ui)
  add_executable(owned_settings_v1_audit tests/owned_settings_v1.cpp)
  target_link_libraries(owned_settings_v1_audit PRIVATE dh2_engine_ui)
  add_executable(settings_language_scene_v1_audit tests/settings_language_scene_v1.cpp)
  target_link_libraries(settings_language_scene_v1_audit PRIVATE dh2_engine_ui)
  add_executable(authored_menu_options_lifecycle_v1_audit tests/authored_menu_options_lifecycle_v1.cpp)
  target_compile_options(authored_menu_options_lifecycle_v1_audit PRIVATE -fno-sanitize=vptr)
  target_link_libraries(authored_menu_options_lifecycle_v1_audit PRIVATE dh2_engine_ui dh2_gameswf_headers)
  add_executable(swf_actionscript_connection_audit tests/swf_actionscript_connection.cpp)
  target_link_libraries(swf_actionscript_connection_audit PRIVATE dh2_engine_ui dh2_gameswf_headers)
  add_executable(gfnt_audit tests/gfnt.cpp)
  target_link_libraries(gfnt_audit PRIVATE dh2_engine_ui)
  add_executable(swf_movie_audit tests/swf_movie.cpp)
  target_link_libraries(swf_movie_audit PRIVATE dh2_engine_ui dh2_gameswf_headers)
  add_executable(freetype_font_audit tests/freetype_font.cpp)
  target_compile_definitions(freetype_font_audit PRIVATE DH2_FONT_RASTER_ONLY)
  target_link_libraries(freetype_font_audit PRIVATE dh2_engine_ui dh2_freetype237)
  add_executable(freetype_hud_audit tests/freetype_font.cpp)
  target_link_libraries(freetype_hud_audit PRIVATE dh2_engine_ui dh2_freetype237 dh2_gameswf_headers)
  add_executable(swf_font_resolver_audit tests/swf_font_resolver.cpp)
  target_link_libraries(swf_font_resolver_audit PRIVATE dh2_engine_ui)
  add_executable(swf_font_resolver_hud_audit tests/swf_font_resolver_hud.cpp)
  target_link_libraries(swf_font_resolver_hud_audit PRIVATE dh2_engine_ui dh2_freetype237 dh2_gameswf_headers)
  add_executable(ui_viewport_audit tests/viewport.cpp)
  target_link_libraries(ui_viewport_audit PRIVATE dh2_engine_ui)
  add_executable(localization_audit tests/localization.cpp)
  target_link_libraries(localization_audit PRIVATE dh2_engine_ui)
  add_executable(localization_connection_audit tests/localization_connection.cpp)
  target_link_libraries(localization_connection_audit PRIVATE dh2_engine_ui dh2_level_world dh2_script_runtime dh2_gameswf_headers)
  add_executable(hud_freetype_font_audit tests/hud_freetype_font.cpp
    "${DH2_FT237}/src/base/ftbitmap.c")
  # Official bitmap conversion is a test reference, not a production path.
  target_compile_definitions(hud_freetype_font_audit PRIVATE FT2_BUILD_LIBRARY)
  target_link_libraries(hud_freetype_font_audit PRIVATE dh2_engine_ui dh2_freetype237)
  add_executable(hud_freetype_provider_audit tests/hud_freetype_provider.cpp)
  target_link_libraries(hud_freetype_provider_audit PRIVATE dh2_engine_ui dh2_script_runtime)
  add_executable(swf_glyph_lookup_audit tests/swf_glyph_lookup.cpp)
  target_link_libraries(swf_glyph_lookup_audit PRIVATE dh2_engine_ui)
  add_executable(hud_player_values_audit tests/hud_player_values.cpp)
  target_link_libraries(hud_player_values_audit PRIVATE dh2_engine_ui)
  add_executable(player_status_hud_audit tests/player_status_hud.cpp)
  target_link_libraries(player_status_hud_audit PRIVATE dh2_engine_ui dh2_game_data dh2_script_runtime)
  add_executable(swf_viewport_connection_audit tests/overlays/source-facade-v1/swf_viewport_connection.cpp)
  target_link_libraries(swf_viewport_connection_audit PRIVATE dh2_engine_ui dh2_gameswf_headers)
  add_executable(hud_sprite_timeline_audit tests/hud_sprite_timeline.cpp)
  target_link_libraries(hud_sprite_timeline_audit PRIVATE dh2_engine_ui)
  add_executable(hud_advance_audit tests/hud_advance.cpp)
  target_link_libraries(hud_advance_audit PRIVATE dh2_engine_ui)
  add_executable(gameswf_font_overlay_audit tests/gameswf_font_overlay.cpp)
  target_compile_options(gameswf_font_overlay_audit PRIVATE -fno-sanitize=vptr)
  target_link_libraries(gameswf_font_overlay_audit PRIVATE dh2_engine_ui dh2_script_runtime dh2_gameswf_headers)
endif()

target_sources(dh2_engine_ui PRIVATE
  localization_parse_ex_v1.cpp
  swf_menu_parsed_string_v1.cpp
  menu_avatar_preview_v1.cpp
  swf_menu_avatar_preview_v1.cpp
  swf_menu_sound.cpp
  swf_menu_options.cpp
  swf_menu_navigation.cpp
  swf_menu_save_slots.cpp
  save_slot_date_v1.cpp
  menu_save_slot_projection_v1.cpp
  menu_manager_push_v1.cpp
  menu_native_event_v1.cpp
  swf_menu_device_v1.cpp
  loading_menu_v1.cpp
  swf_anim_tooltip_v104.cpp
  swf_loading_menu_v1.cpp
  swf_text_layout_connection.cpp)
