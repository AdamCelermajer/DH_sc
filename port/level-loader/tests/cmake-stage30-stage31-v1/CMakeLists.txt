cmake_minimum_required(VERSION 3.16)
project(dh2_stage30_stage31_owner_regressions LANGUAGES CXX)
get_filename_component(root "${CMAKE_CURRENT_LIST_DIR}/../../../.." ABSOLUTE)
set(DH2_FX_LIBRARY_CPP "${root}/port/level-world/visual_fx_manager_libraries_v63.cpp" CACHE FILEPATH "Actual FX library owner source")
set(DH2_GAME_EVENT_CPP "${root}/port/level-loader/game_event_runtime_v75.cpp" CACHE FILEPATH "Actual event runtime owner source")
option(DH2_REGRESSION_SANITIZERS "Enable address/undefined behavior checks" OFF)
add_executable(stage30_fx_library_v63
 "${root}/port/level-world/tests/visual_fx_manager_libraries_v63_v1.cpp"
 "${DH2_FX_LIBRARY_CPP}" "${root}/port/game-data/effects_tables.cpp" "${root}/port/game-data/data.cpp"
 "${root}/port/level-world/visual_fx_preload.cpp" "${root}/port/level-loader/lifecycle_v36.cpp")
add_executable(stage31_game_event_reinit_v75
 "${root}/port/level-loader/tests/game_event_runtime_v75_reinit_v1.cpp"
 "${DH2_GAME_EVENT_CPP}" "${root}/port/level-loader/game_event_manager_v50.cpp"
 "${root}/port/level-world/event_manager_owner_v12.cpp")
foreach(target stage30_fx_library_v63 stage31_game_event_reinit_v75)
 target_compile_features(${target} PRIVATE cxx_std_17)
 target_compile_options(${target} PRIVATE -Wall -Wextra -Werror -Wno-misleading-indentation -ffunction-sections -fdata-sections)
 target_link_options(${target} PRIVATE -Wl,--gc-sections)
 target_include_directories(${target} PRIVATE
  "${root}/port/level-world" "${root}/port/level-loader" "${root}/port/game-data"
  "${root}/port/scene-materials" "${root}/port/engine-math" "${root}/port/engine-animation"
  "${root}/port/engine-resources" "${root}/port/engine-skinning" "${root}/port/engine-ui")
 if(DH2_REGRESSION_SANITIZERS)
  target_compile_options(${target} PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
  target_link_options(${target} PRIVATE -fsanitize=address,undefined)
 endif()
endforeach()
enable_testing()
add_test(NAME stage30_fx_library_v63 COMMAND stage30_fx_library_v63)
add_test(NAME stage31_game_event_reinit_v75 COMMAND stage31_game_event_reinit_v75)
