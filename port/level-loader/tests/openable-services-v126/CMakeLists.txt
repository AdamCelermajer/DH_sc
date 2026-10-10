cmake_minimum_required(VERSION 3.22)
project(openable_services_v126 LANGUAGES C CXX)
get_filename_component(DH2_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../.." ABSOLUTE)
set(DH2_OPTIMIZE_NATIVE OFF CACHE BOOL "" FORCE)
set(DH2_BUILD_NATIVE_AUDITS OFF CACHE BOOL "" FORCE)
add_compile_options(-O1 -g -ffunction-sections -fdata-sections -Wno-error=misleading-indentation)
add_subdirectory("${DH2_SOURCE_DIR}/port/engine-textures" textures)
#Existing world sources already compiled with their explicit relative texture
#headers. This harness needs the real linked library, not new include search paths.
set_property(TARGET dh2_engine_textures PROPERTY INTERFACE_INCLUDE_DIRECTORIES "")
add_subdirectory("${DH2_SOURCE_DIR}/port/level-world" world)
target_sources(dh2_scene_materials PRIVATE
 "${DH2_SOURCE_DIR}/port/engine-resources/resource_budget_v37.cpp"
 "${DH2_SOURCE_DIR}/port/engine-resources/retained_bytes_v39.cpp"
 "${DH2_SOURCE_DIR}/port/engine-resources/admitted_cpu_bytes_v40.cpp")
target_link_libraries(dh2_engine_ui PRIVATE "$<LINK_ONLY:dh2_scene_materials>")
#The shipping host harness supplies <algorithm> through its target PCH.
#Keep this focused standalone harness equivalent without editing Item scope.
set_source_files_properties("${DH2_SOURCE_DIR}/port/level-world/canonical_item_factory_v2.cpp"
 "${DH2_SOURCE_DIR}/port/level-world/canonical_container_target_v104.cpp"
 TARGET_DIRECTORY dh2_level_world PROPERTIES COMPILE_OPTIONS "-include;algorithm")
add_executable(openable_factory_services_v126
 ../openable_factory_services_v126.cpp
 ../../catalog_containers_v67.cpp
 ../../canonical_level_context_v1.cpp
 ../../canonical_receiver_transport_v1.cpp)
target_include_directories(openable_factory_services_v126 PRIVATE
 "${DH2_SOURCE_DIR}/port/level-loader" "${DH2_SOURCE_DIR}/port/level-world"
 "${DH2_SOURCE_DIR}/port/engine-audio")
target_compile_features(openable_factory_services_v126 PRIVATE cxx_std_17)
target_link_options(openable_factory_services_v126 PRIVATE -Wl,--gc-sections)
target_link_libraries(openable_factory_services_v126 PRIVATE dh2_level_world dh2_script_runtime dh2_engine_ui)
foreach(name openable_container_owner_v1 canonical_openable_graph_v4 canonical_openable_graph_v21)
 add_executable(${name} "${DH2_SOURCE_DIR}/port/level-world/tests/${name}.cpp")
 target_compile_features(${name} PRIVATE cxx_std_17)
 target_link_libraries(${name} PRIVATE dh2_level_world)
endforeach()
add_executable(stage17_virtual58_direct_factories_v1
 ../stage17_virtual58_direct_factories_v1.cpp
 ../../canonical_receiver_transport_v1.cpp
 ../../canonical_floor_v72.cpp
 ../../canonical_colbox_v71.cpp)
target_include_directories(stage17_virtual58_direct_factories_v1 PRIVATE
 "${DH2_SOURCE_DIR}/port/level-loader" "${DH2_SOURCE_DIR}/port/level-world")
target_compile_features(stage17_virtual58_direct_factories_v1 PRIVATE cxx_std_17)
target_link_options(stage17_virtual58_direct_factories_v1 PRIVATE -Wl,--gc-sections)
target_link_libraries(stage17_virtual58_direct_factories_v1 PRIVATE dh2_level_world)
enable_testing()
add_test(NAME factory-composed COMMAND openable_factory_services_v126 "${DH2_SOURCE_DIR}/reference/openable-container-v1/cache")
add_test(NAME source-owner COMMAND openable_container_owner_v1)
add_test(NAME graph-v4 COMMAND canonical_openable_graph_v4 "${DH2_SOURCE_DIR}/reference/openable-container-v1/cache")
add_test(NAME graph-v21 COMMAND canonical_openable_graph_v21 "${DH2_SOURCE_DIR}/reference/openable-container-v1/cache")
add_test(NAME stage17-virtual58-direct-factories COMMAND stage17_virtual58_direct_factories_v1)
