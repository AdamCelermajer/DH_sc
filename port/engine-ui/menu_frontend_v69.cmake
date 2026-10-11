# Reviewed Adam791 verified-v69 frontend overlay. This target borrows the
# existing game-data and inventory-text DSOs; never compile duplicate text TUs.
if(NOT TARGET dh2_inventory_text_v1 OR NOT TARGET dh2_game_data)
 message(FATAL_ERROR "Menu frontend requires canonical game-data and inventory-text targets")
endif()
set(DH2_MENU_UI_DIR "${CMAKE_CURRENT_LIST_DIR}")
include("${DH2_MENU_UI_DIR}/gameswf_sources.cmake")
include("${DH2_MENU_UI_DIR}/gameswf_font_overlay_v1.cmake")
include("${DH2_MENU_UI_DIR}/gameswf_input_overlay_v1.cmake")
include("${DH2_MENU_UI_DIR}/gameswf_frame_overlay_v1.cmake")
include("${DH2_MENU_UI_DIR}/gameswf_text_property_overlay_v1.cmake")
include("${DH2_MENU_UI_DIR}/freetype237-hud.cmake")
add_library(dh2_gameswf_core STATIC ${DH2_GAMESWF_SOURCES})
set_target_properties(dh2_gameswf_core PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_compile_features(dh2_gameswf_core PUBLIC cxx_std_17)
target_include_directories(dh2_gameswf_core SYSTEM PUBLIC "${DH2_GAMESWF_ROOT}")
target_include_directories(dh2_gameswf_core PRIVATE "${DH2_MENU_UI_DIR}")
target_compile_definitions(dh2_gameswf_core PUBLIC TU_CONFIG_LINK_TO_JPEGLIB=0
 TU_CONFIG_LINK_TO_LIBPNG=0 TU_CONFIG_LINK_TO_FREETYPE=0 TU_CONFIG_LINK_TO_THREAD=0)
target_compile_options(dh2_gameswf_core PRIVATE -w -fno-fast-math -ffp-contract=off -fno-sanitize=vptr)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
 target_compile_options(dh2_gameswf_core PRIVATE -fpermissive)
endif()
if(ANDROID)
 target_link_libraries(dh2_gameswf_core PUBLIC z ${CMAKE_DL_LIBS})
else()
 find_package(ZLIB QUIET)
 if(NOT ZLIB_FOUND)
  # Reuse the repository's genuine bundled zlib for host tools that have no
  # development package. Android still uses its platform zlib above.
  set(DH2_MENU_ZLIB_DIR "${DH2_MENU_UI_DIR}/../irrlicht-android/upstream/source/Irrlicht/zlib")
  add_library(dh2_menu_host_zlib STATIC
   "${DH2_MENU_ZLIB_DIR}/adler32.c" "${DH2_MENU_ZLIB_DIR}/crc32.c"
   "${DH2_MENU_ZLIB_DIR}/inflate.c" "${DH2_MENU_ZLIB_DIR}/inffast.c"
   "${DH2_MENU_ZLIB_DIR}/inftrees.c" "${DH2_MENU_ZLIB_DIR}/zutil.c")
  target_include_directories(dh2_menu_host_zlib PUBLIC "${DH2_MENU_ZLIB_DIR}")
  add_library(ZLIB::ZLIB ALIAS dh2_menu_host_zlib)
 endif()
 target_link_libraries(dh2_gameswf_core PUBLIC ZLIB::ZLIB ${CMAKE_DL_LIBS})
endif()
set(DH2_MENU_UI_SOURCES gfnt.cpp swf_movie.cpp freetype_font.cpp
 freetype_glyph_kernel.cpp swf_freetype_provider.cpp swf_font_geometry.cpp
 swf_font_resolver.cpp viewport.cpp freetype_bitmap_alpha.cpp hud_freetype_font.cpp
 swf_hud_freetype_provider.cpp swf_glyph_lookup.cpp hud_player_values.cpp
 swf_viewport_connection.cpp hud_sprite_timeline.cpp hud_sprite_core.cpp
 hud_advance.cpp hud_advance_owner.cpp player_status_hud.cpp
 swf_actionscript_connection.cpp swf_menu_sound.cpp swf_menu_options.cpp
 swf_menu_navigation.cpp swf_menu_save_slots.cpp swf_menu_launch_v1.cpp save_slot_date_v1.cpp
 menu_save_slot_projection_v1.cpp menu_manager_push_v1.cpp menu_native_event_v1.cpp
 renderfx_text_connection.cpp hud_startup_callbacks.cpp game_option_table_v1.cpp
 owned_hud_settings_v1.cpp settings_native_files_v1.cpp settings_language_scene_v1.cpp
 authored_gameplay_hud_v1.cpp authored_hud_edge_layout_v6.cpp authored_joystick_v1.cpp
 hud_manager.cpp hud_manager_core.cpp text_layout_v1.cpp swf_text_layout_connection.cpp
 character_menu_stats_owner_v1.cpp
 character_menu_inventory_order_v1.cpp
 character_menu_reload_v1.cpp scrolling_combat_text_owner_v1.cpp
 scrolling_combat_text_projection_v1.cpp scrolling_combat_text_bridge_v1.cpp
 scrolling_combat_text_position_v1.cpp
 swf_input_history.cpp swf_frame_schedule.cpp swf_frame_connection.cpp
 swf_drag_values.cpp swf_cursor_input.cpp swf_input_geometry.cpp swf_input_policy.cpp
 swf_input_connection.cpp swf_event_dispatch.cpp swf_event_core.cpp)
list(APPEND DH2_MENU_UI_SOURCES world_map_list_v1.cpp level_transition_v1.cpp
 application_level_load_handoff_v1.cpp gslevel_queued_state_handoff_v1.cpp
 gslevel_activation_step_v1.cpp)
list(TRANSFORM DH2_MENU_UI_SOURCES PREPEND "${DH2_MENU_UI_DIR}/")
add_library(dh2_engine_ui SHARED ${DH2_MENU_UI_SOURCES})
target_sources(dh2_engine_ui PRIVATE
 "${DH2_MENU_UI_DIR}/../asset-payloads/sha256.cpp"
 "${DH2_MENU_UI_DIR}/../scene-materials/swf_texture.cpp"
 "${DH2_MENU_UI_DIR}/../scene-materials/shader_sources.cpp"
 "${DH2_MENU_UI_DIR}/../game-data/campaign_profile_files_v1.cpp"
 "${DH2_MENU_UI_DIR}/../game-data/menu_profile_metadata_v1.cpp")
target_compile_features(dh2_engine_ui PUBLIC cxx_std_17)
target_include_directories(dh2_engine_ui PUBLIC "${DH2_MENU_UI_DIR}"
 "${DH2_MENU_UI_DIR}/../asset-payloads" "${DH2_MENU_UI_DIR}/../scene-materials")
target_link_libraries(dh2_engine_ui PUBLIC dh2_inventory_text_v1 dh2_game_data
 PRIVATE dh2_gameswf_core dh2_freetype237)
target_compile_options(dh2_engine_ui PRIVATE -Wall -Wextra -fno-fast-math -ffp-contract=off)
if(MINGW)
 # GameSWF explicitly exports some Windows methods, which disables MinGW's
 # default auto-export. Export the selected facade for actual DSO host tests.
 target_link_options(dh2_engine_ui PRIVATE -Wl,--export-all-symbols)
endif()
if(ANDROID)
 target_link_options(dh2_engine_ui PRIVATE -Wl,-z,max-page-size=16384 -Wl,--no-undefined)
endif()
