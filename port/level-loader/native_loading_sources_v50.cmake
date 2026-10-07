# Include from the root's EXISTING dh2_level_world target, after its other
# source additions. This reuses native source kernels and deduplicates exact
# paths; it creates no alternate owner or private/vendor implementation lib.
if(NOT TARGET dh2_level_world)
  message(FATAL_ERROR "V50 requires the existing native dh2_level_world target")
endif()
set(_dh2_v50_loader_sources
  lifecycle_v36.cpp level_source_loading_v43.cpp native_gslevel_frame_v45.cpp
  retained_level_module_graph_v1.cpp canonical_cached_file_v1.cpp
  canonical_module_files_v1.cpp canonical_source_adapter_v1.cpp canonical_receiver_transport_v1.cpp
  cached_level_file_v1.cpp level_file_walk_v1.cpp object_entry_v1.cpp
  object_initialization_v1.cpp resource_paths_v1.cpp user_properties_v1.cpp
  visual_transform_v1.cpp module_draw_frame_v1.cpp script_manager_owner_v52.cpp
  script_manager_execution_v96.cpp script_command_execution_v96.cpp script_execution_control_v96.cpp
  level_root_filename_route_v52.cpp script_command_receivers_v59.cpp level_gameplay_update_v66.cpp)
set(_dh2_v50_world_sources
  canonical_level_module_bindings_v2.cpp canonical_module_graph_v3.cpp
  retained_module_visual_v3.cpp module_selected_scene_v2.cpp
  module_static_scene_bounds_v3.cpp module_visual_mesh_box_v2.cpp
  module_static_scene_v2.cpp module_floor_graph_v3.cpp module_floor_append_v2.cpp
  module_floor_clone_v3.cpp module_pf_room_v3.cpp module_xml_selection_v1.cpp
  module_room_zone_spawn_v3.cpp scene_manager_map_owner_v2.cpp
  condition_data_init_v3.cpp game_object_set_position_v2.cpp game_object_relative_box_v3.cpp
  user_properties_v2.cpp canonical_room_zone_v3.cpp canonical_room_zone_factory_v3.cpp
  application_spawn_random_owner_v4.cpp game_object_spawn_probability_v1.cpp
  level_config_publication_v2.cpp visual_fx_manager_libraries_v63.cpp visual_fx_preload.cpp module_fog_v67.cpp)
get_target_property(_dh2_v50_existing dh2_level_world SOURCES)
set(_dh2_v50_existing_absolute)
foreach(_dh2_v50_path IN LISTS _dh2_v50_existing)
  if(NOT _dh2_v50_path MATCHES "^\\$<")
    get_filename_component(_dh2_v50_absolute "${_dh2_v50_path}" ABSOLUTE BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
    list(APPEND _dh2_v50_existing_absolute "${_dh2_v50_absolute}")
  endif()
endforeach()
set(_dh2_v50_added)
foreach(_dh2_v50_group IN ITEMS loader world)
  foreach(_dh2_v50_file IN LISTS _dh2_v50_${_dh2_v50_group}_sources)
    if(_dh2_v50_group STREQUAL "loader")
      set(_dh2_v50_path "${CMAKE_CURRENT_LIST_DIR}/${_dh2_v50_file}")
    else()
      set(_dh2_v50_path "${CMAKE_CURRENT_LIST_DIR}/../level-world/${_dh2_v50_file}")
    endif()
    get_filename_component(_dh2_v50_path "${_dh2_v50_path}" ABSOLUTE)
    if(NOT EXISTS "${_dh2_v50_path}")
      message(FATAL_ERROR "V50 source dependency missing: ${_dh2_v50_path}")
    endif()
    if(NOT _dh2_v50_path IN_LIST _dh2_v50_existing_absolute)
      target_sources(dh2_level_world PRIVATE "${_dh2_v50_path}")
      list(APPEND _dh2_v50_existing_absolute "${_dh2_v50_path}")
      list(APPEND _dh2_v50_added "${_dh2_v50_path}")
    endif()
  endforeach()
endforeach()
# Existing source style contains compact one-line branches; retain the same
# narrow legacy diagnostic suppression as the other reconstructed kernels.
set_source_files_properties(${_dh2_v50_added} PROPERTIES COMPILE_OPTIONS "-Wno-misleading-indentation")
target_include_directories(dh2_level_world PRIVATE "${CMAKE_CURRENT_LIST_DIR}")
unset(_dh2_v50_loader_sources)
unset(_dh2_v50_world_sources)
unset(_dh2_v50_existing)
unset(_dh2_v50_existing_absolute)
unset(_dh2_v50_added)
unset(_dh2_v50_file)
unset(_dh2_v50_group)
unset(_dh2_v50_path)
unset(_dh2_v50_absolute)
