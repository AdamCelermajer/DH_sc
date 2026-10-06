cmake_minimum_required(VERSION 3.22)
project(dh2_native_app LANGUAGES CXX)
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
target_sources(dh2_scene_materials PRIVATE "${DH2_SOURCE_DIR}/port/engine-resources/resource_budget_v37.cpp")
target_sources(dh2_level_world PRIVATE "${DH2_SOURCE_DIR}/port/level-world/vox_audio_bridge_v34.cpp")
target_sources(dh2_level_world PRIVATE "${DH2_SOURCE_DIR}/port/level-world/character_close_range_v38.cpp")
target_link_libraries(dh2_level_world PRIVATE dh2_engine_audio)
add_library(dh2_native SHARED native_app.cpp model_renderer.cpp authored_shader_program.cpp audio_output_v34.cpp native_resource_budget_v38.cpp
  swf_gpu.cpp original_ui_assets.cpp original_ui_session.cpp front_ui_session_v87.cpp original_cache_assets_v1.cpp
  gameplay_hud.cpp gameplay_icons.cpp character_panel_session_v1.cpp character_panel_presentation_v1.cpp character_panel_authored_v2.cpp character_panel_reload_v3.cpp character_panel_runtime_v3.cpp faery_gameplay_v1.cpp
  faery_cast_owner_v2.cpp faery_cast_state_v2.cpp player_ai_callables_v1.cpp player_timer_helpers_v1.cpp player_recurring_effects_v7.cpp
  "${DH2_SOURCE_DIR}/port/asset-payloads/zip_asset_pack_v1.cpp")
target_compile_features(dh2_native PRIVATE cxx_std_17)
target_compile_definitions(dh2_native PRIVATE DH2_ENABLE_LOAD_DIAGNOSTICS=$<BOOL:${DH2_ENABLE_LOAD_DIAGNOSTICS}>)
if(DH2_ENABLE_LOAD_DIAGNOSTICS)
  target_compile_definitions(dh2_native PRIVATE DH2_LOAD_DIAGNOSTICS_QA_REQUEST_V39=$<BOOL:${DH2_REQUEST_LOAD_DIAGNOSTICS_QA}>)
endif()
target_compile_options(dh2_native PRIVATE -Wall -Wextra -Werror -fno-fast-math -ffp-contract=off)
target_include_directories(dh2_native PRIVATE "${DH2_SOURCE_DIR}/port/engine-ui"
  "${DH2_SOURCE_DIR}/port/engine-math"
  "${DH2_SOURCE_DIR}/port/scene-materials")
target_link_options(dh2_native PRIVATE -Wl,-z,max-page-size=16384 -Wl,--no-undefined)
target_link_libraries(dh2_native PRIVATE dh2_engine_textures dh2_scene_materials dh2_level_world dh2_script_runtime dh2_engine_ui dh2_engine_audio android log GLESv2 z dl)
