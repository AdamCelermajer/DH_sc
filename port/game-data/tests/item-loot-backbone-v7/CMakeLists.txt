cmake_minimum_required(VERSION 3.22)
project(item_loot_backbone_v7_host LANGUAGES C CXX)
get_filename_component(DH2_ITEM_LOOT_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../../.." ABSOLUTE)
add_subdirectory("${DH2_ITEM_LOOT_ROOT}/port/game-data" game-data)
# Stage the candidate through the same selected shared library. When the main
# CMake selects these files, this wrapper detects that selection and adds none.
get_target_property(selected dh2_game_data SOURCES)
foreach(source item_presentation_v5.cpp loot_power_creation_v7.cpp)
  set(found FALSE)
  foreach(existing IN LISTS selected)
    get_filename_component(name "${existing}" NAME)
    if(name STREQUAL source)
      set(found TRUE)
    endif()
  endforeach()
  if(NOT found)
    target_sources(dh2_game_data PRIVATE "${DH2_ITEM_LOOT_ROOT}/port/game-data/${source}")
  endif()
endforeach()
set_source_files_properties("${DH2_ITEM_LOOT_ROOT}/port/game-data/item_presentation_v5.cpp"
  "${DH2_ITEM_LOOT_ROOT}/port/game-data/loot_power_creation_v7.cpp"
  TARGET_DIRECTORY dh2_game_data
  PROPERTIES COMPILE_OPTIONS "-ffp-contract=off;-Wno-misleading-indentation")
add_executable(item_presentation_v5_selected ../item_presentation_v5.cpp
  ../item_presentation_v5_fixture.cpp ../item_power_instance_v5_fixture.cpp)
target_link_libraries(item_presentation_v5_selected PRIVATE dh2_game_data)
add_executable(item_loot_backbone_v7_selected ../item_loot_backbone_v7.cpp
  ../loot_power_creation_v7_fixture.cpp "${DH2_ITEM_LOOT_ROOT}/port/random/random.c")
target_link_libraries(item_loot_backbone_v7_selected PRIVATE dh2_game_data)
target_compile_options(item_loot_backbone_v7_selected PRIVATE -fno-fast-math -ffp-contract=off)
add_executable(loot_entry_selection_v1_selected ../loot_entry_selection_v1_host.cpp)
target_link_libraries(loot_entry_selection_v1_selected PRIVATE dh2_game_data)
add_executable(item_loot_random_table_v1_selected ../item_loot_random_table_v1.cpp
  "${DH2_ITEM_LOOT_ROOT}/port/random/random.c")
target_link_libraries(item_loot_random_table_v1_selected PRIVATE dh2_game_data)
target_compile_options(item_loot_random_table_v1_selected PRIVATE -fno-fast-math -ffp-contract=off)
