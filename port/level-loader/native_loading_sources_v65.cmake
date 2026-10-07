include_guard(GLOBAL)
# Production closure only. Reuses an EXISTING canonical native owner target;
# no test executables, alternate Level/property manager or vendor owner library.
set(_DH2_NATIVE_LOADING_CLOSURE_DIR_V65 "${CMAKE_CURRENT_LIST_DIR}")

function(_dh2_loader_append_owned_sources_v65 target root)
 get_target_property(owner_source_dir "${target}" SOURCE_DIR)
 get_target_property(existing "${target}" SOURCES)
 if(NOT existing)
  set(existing)
 endif()
 set(existing_absolute)
 foreach(path IN LISTS existing)
  if(NOT path MATCHES "^\\$<")
   get_filename_component(absolute "${path}" ABSOLUTE BASE_DIR "${owner_source_dir}")
   list(APPEND existing_absolute "${absolute}")
  endif()
 endforeach()
 set(added)
 foreach(file IN LISTS ARGN)
  get_filename_component(path "${root}/${file}" ABSOLUTE)
  if(NOT EXISTS "${path}")
   message(FATAL_ERROR "Native loader production source missing: ${path}")
  endif()
  if(NOT path IN_LIST existing_absolute)
   list(APPEND added "${path}")
   list(APPEND existing_absolute "${path}")
  endif()
 endforeach()
 if(added)
  target_sources("${target}" PRIVATE ${added})
  set_source_files_properties(${added} PROPERTIES COMPILE_OPTIONS "-Wno-misleading-indentation")
 endif()
endfunction()

function(dh2_loader_attach_native_sources_v65 target)
 cmake_parse_arguments(ARG "" "LOADER_DIR;WORLD_DIR;XML_TARGET" "" ${ARGN})
 if(NOT TARGET "${target}")
  message(FATAL_ERROR "Native loader requires its existing canonical owner target: ${target}")
 endif()
 if(NOT ARG_LOADER_DIR)
  set(ARG_LOADER_DIR "${_DH2_NATIVE_LOADING_CLOSURE_DIR_V65}")
 endif()
 if(NOT ARG_WORLD_DIR)
  set(ARG_WORLD_DIR "${ARG_LOADER_DIR}/../level-world")
 endif()
 if(NOT ARG_XML_TARGET OR NOT TARGET "${ARG_XML_TARGET}")
  message(FATAL_ERROR "Native loader requires the ONE existing TinyXML owner target")
 endif()
 # Canonical Level/C1/property-map/base/registry/stream implementations remain
 # on their actual owner target. Do not append private snapshot implementations.
 _dh2_loader_append_owned_sources_v65("${target}" "${ARG_LOADER_DIR}"
  lifecycle_v36.cpp level_source_loading_v43.cpp native_gslevel_frame_v45.cpp
  retained_level_module_graph_v1.cpp canonical_cached_file_v1.cpp
  canonical_module_files_v1.cpp canonical_source_adapter_v1.cpp canonical_receiver_transport_v1.cpp
  cached_level_file_v1.cpp level_file_walk_v1.cpp object_entry_v1.cpp
  object_initialization_v1.cpp resource_paths_v1.cpp user_properties_v1.cpp
  visual_transform_v1.cpp module_draw_frame_v1.cpp script_manager_owner_v52.cpp
  level_root_filename_route_v52.cpp level_root_cfs_filename_v52.cpp
  original_filesystem_fields_v50.cpp script_command_receivers_v59.cpp
  assigned_loadfile_kernel_v65.cpp xml_document_v1.cpp level_lightset_stage9_v53.cpp)
 _dh2_loader_append_owned_sources_v65("${target}" "${ARG_WORLD_DIR}"
  canonical_level_module_bindings_v2.cpp canonical_module_graph_v3.cpp
  retained_module_visual_v3.cpp module_selected_scene_v2.cpp
  native_batch_resources_v110.cpp
  module_static_scene_bounds_v3.cpp module_visual_mesh_box_v2.cpp
  module_static_scene_v2.cpp module_floor_graph_v3.cpp module_floor_append_v2.cpp
  module_floor_clone_v3.cpp module_pf_room_v3.cpp module_xml_selection_v1.cpp
  module_room_zone_spawn_v3.cpp scene_manager_map_owner_v2.cpp
  condition_data_init_v3.cpp game_object_set_position_v2.cpp game_object_relative_box_v3.cpp
  user_properties_v2.cpp canonical_room_zone_v3.cpp canonical_room_zone_factory_v3.cpp
  application_spawn_random_owner_v4.cpp game_object_spawn_probability_v1.cpp
  level_config_publication_v2.cpp visual_fx_manager_libraries_v63.cpp visual_fx_preload.cpp)
 target_compile_features("${target}" PUBLIC cxx_std_17)
 target_include_directories("${target}" BEFORE PRIVATE "${ARG_LOADER_DIR}" "${ARG_WORLD_DIR}")
 target_link_libraries("${target}" PRIVATE "${ARG_XML_TARGET}")
 # Track selected API headers with the owning loader TUs. No extra CPP owners.
 _dh2_loader_append_owned_sources_v65("${target}" "${ARG_LOADER_DIR}"
  canonical_module_files_v1.hpp canonical_cached_file_v1.hpp cached_level_file_v1.hpp
  xml_document_v1.hpp retained_level_module_graph_v1.hpp level_source_loading_v43.hpp
  level_root_filename_route_v52.hpp level_root_cfs_filename_v52.hpp original_filesystem_fields_v50.hpp
  assigned_root_source_v64.hpp assigned_loadfile_kernel_v65.hpp filename_root_source_v65.hpp
  native_root_source_binding_v65.hpp stage7_cancel_cleanup.hpp
  stage_loader_v38_root_file.hpp level_source_loading_update_v46.hpp
  native_gslevel_loading_connection_v49.hpp script_command_receivers_v59.hpp
  command_c1_descriptors_v59.inc script_manager_owner_v52.hpp script_data_schemas_v52.inc
  script_command_init_v62.hpp script_manager_bind_v63.hpp script_manager_complete_bind.hpp)
endfunction()
