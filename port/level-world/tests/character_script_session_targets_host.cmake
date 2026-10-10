# Host-only completion of the provider sources normally attached by the Android
# app CMake. Include at project() time using
# -DCMAKE_PROJECT_dh2_level_world_INCLUDE=<absolute path to this file>.
# Defer until the standalone World graph has created the owning targets. This
# keeps the shipping source graph untouched and uses real source providers.
function(dh2_complete_character_session_host_providers)
  get_filename_component(dh2_host_port "${CMAKE_CURRENT_SOURCE_DIR}/.." ABSOLUTE)
  target_sources(dh2_scene_materials PRIVATE
    "${dh2_host_port}/engine-resources/resource_budget_v37.cpp"
    "${dh2_host_port}/engine-resources/retained_bytes_v39.cpp"
    "${dh2_host_port}/engine-resources/admitted_cpu_bytes_v40.cpp")
  target_sources(dh2_engine_ui PRIVATE
    "${dh2_host_port}/engine-ui/authored_menu_drag_v68.cpp"
    "${dh2_host_port}/level-loader/menu_fs_command_v114.cpp")
  target_link_libraries(dh2_engine_ui PRIVATE dh2_scene_materials)
  target_sources(dh2_level_world PRIVATE
    "${dh2_host_port}/level-world/character_close_range_v38.cpp"
    "${dh2_host_port}/level-world/target_frontal_sort_v41.cpp"
    "${dh2_host_port}/level-world/character_kill_fields_v21.cpp"
    "${dh2_host_port}/level-world/canonical_spawn_spot_v108.cpp")
  target_link_libraries(character_script_session_targets_audit PRIVATE
    dh2_engine_textures dh2_scene_materials dh2_engine_animation
    dh2_engine_skinning dh2_script_runtime)
endfunction()
cmake_language(DEFER CALL dh2_complete_character_session_host_providers)
