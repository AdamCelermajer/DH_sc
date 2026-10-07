include_guard(GLOBAL)
# Include/call from Main's existing owner target; no alternate canonical owner,
# property map, Level, actor runtime, TinyXML or test/fixture targets are created.
function(dh2_loader_attach_catalog_auxiliary_v67 target)
 cmake_parse_arguments(ARG "" "CATALOG_DIR;LOADER_DIR;WORLD_DIR" "" ${ARGN})
 if(NOT TARGET "${target}" OR NOT ARG_CATALOG_DIR OR NOT ARG_LOADER_DIR OR NOT ARG_WORLD_DIR)
  message(FATAL_ERROR "V67 auxiliary needs the existing owner and explicit coherent source directories")
 endif()
 get_target_property(source_dir "${target}" SOURCE_DIR)
 get_target_property(existing "${target}" SOURCES)
 set(existing_absolute)
 foreach(file IN LISTS existing)
  if(NOT file MATCHES "^\\$<")
   get_filename_component(path "${file}" ABSOLUTE BASE_DIR "${source_dir}")
   list(APPEND existing_absolute "${path}")
  endif()
 endforeach()
 set(catalog_sources catalog_auxiliary_v67.cpp catalog_auxiliary_v67.hpp)
 set(loader_sources canonical_auxiliary_families_v16.cpp canonical_auxiliary_families_v16.hpp)
 # Generic V16 also references Destructible and TriggerTrap constructors, even
 # when this part advertises neither; reuse their existing actual owner TUs.
 set(world_sources
  canonical_dummy_owner_v14.cpp canonical_spawn_point_v15.cpp canonical_decor_v15.cpp
  canonical_trigger_zone_v22.cpp canonical_animated_decor_v23.cpp canonical_checkpoint_zone_v26.cpp
  canonical_door_v27.cpp canonical_trigger_object_v28.cpp canonical_exit_zone_v29.cpp
  canonical_quest_move_zone_v31.cpp canonical_sound_emitter_v32.cpp canonical_trigger_trap_v37.cpp
  canonical_destructible_container_v16.cpp game_object_initialization_owner_v1.cpp game_object_set_position_v2.cpp)
 set(added)
 foreach(group IN ITEMS catalog loader world)
  string(TOUPPER "${group}" kind)
  foreach(file IN LISTS ${group}_sources)
   get_filename_component(path "${ARG_${kind}_DIR}/${file}" ABSOLUTE)
   if(NOT EXISTS "${path}")
    message(FATAL_ERROR "V67 actual auxiliary source dependency missing: ${path}")
   endif()
   if(NOT path IN_LIST existing_absolute)
    list(APPEND added "${path}")
    list(APPEND existing_absolute "${path}")
   endif()
  endforeach()
 endforeach()
 if(added)
  target_sources("${target}" PRIVATE ${added})
  set_source_files_properties(${added} PROPERTIES COMPILE_OPTIONS "-Wno-misleading-indentation")
 endif()
 target_compile_features("${target}" PUBLIC cxx_std_17)
 target_include_directories("${target}" BEFORE PRIVATE "${ARG_CATALOG_DIR}" "${ARG_LOADER_DIR}" "${ARG_WORLD_DIR}")
 # Parent assembles common/catalog/environment/container TUs once separately.
 # Do not link this owner's own exported part back to itself or add snapshots.
endfunction()