cmake_minimum_required(VERSION 3.16)
project(dh2_loader_connected_source_fixture LANGUAGES CXX)
get_filename_component(loader "${CMAKE_CURRENT_LIST_DIR}/../.." ABSOLUTE)
set(snapshot "${loader}/vendor/connected-owners-1b96f4edb021619d/port/level-world")
# Narrow immutable owner target for source composition verification only.
# Actual game integration instead supplies its existing dh2_level_world target.
add_library(dh2_loader_connected_source_owners STATIC
 "${snapshot}/canonical_object_manager_v1.cpp"
 "${snapshot}/canonical_object_factory_v1.cpp"
 "${snapshot}/canonical_property_map_v1.cpp"
 "${snapshot}/canonical_class_receiver_bindings_v1.cpp")
target_compile_features(dh2_loader_connected_source_owners PUBLIC cxx_std_17)
target_compile_options(dh2_loader_connected_source_owners PRIVATE -Wall -Wextra -Werror -Wno-misleading-indentation -fno-fast-math -ffp-contract=off)
target_include_directories(dh2_loader_connected_source_owners PUBLIC "${snapshot}" "${loader}/../level-world")
set(DH2_LOADER_CANONICAL_OWNER_TARGET dh2_loader_connected_source_owners CACHE STRING "" FORCE)
add_subdirectory("${loader}" "${CMAKE_CURRENT_BINARY_DIR}/loader")
if(TARGET dh2_loader_canonical_owners)
 message(FATAL_ERROR "A second canonical owner target was created")
endif()
add_executable(dh2_loader_connected_source_probe "${loader}/tests/canonical_connected_source_probe.cpp")
target_compile_options(dh2_loader_connected_source_probe PRIVATE -Wall -Wextra -Werror)
target_link_libraries(dh2_loader_connected_source_probe PRIVATE dh2_loader_canonical_adapter)
