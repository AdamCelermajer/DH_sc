cmake_minimum_required(VERSION 3.22)
project(equipment_multi_gear_v1_host LANGUAGES C CXX)
get_filename_component(DH2_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../../.." ABSOLUTE)
add_subdirectory("${DH2_ROOT}/port/game-data" game-data)
add_executable(equipment_multi_gear_v1 ../equipment_multi_gear_v1.cpp
 "${DH2_ROOT}/port/random/random.c")
target_link_libraries(equipment_multi_gear_v1 PRIVATE dh2_game_data)
target_compile_options(equipment_multi_gear_v1 PRIVATE -fno-fast-math -ffp-contract=off)
