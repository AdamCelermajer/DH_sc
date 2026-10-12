# Append to the SAME canonical production owner; do not introduce another
# property map, base, manager, World, TinyXML or duplicate canonical receiver.
function(dh2_loader_attach_generic_catalog_v70 target)
  cmake_parse_arguments(G "" "CATALOG_DIR;COMMON_CATALOG_DIR;LOADER_DIR;WORLD_DIR" "" ${ARGN})
  if(NOT TARGET ${target} OR NOT G_CATALOG_DIR OR NOT G_COMMON_CATALOG_DIR OR NOT G_LOADER_DIR OR NOT G_WORLD_DIR)
    message(FATAL_ERROR "Generic catalog requires the existing canonical owner and source directories")
  endif()
  get_target_property(_g_sources ${target} SOURCES)
  get_target_property(_g_base ${target} SOURCE_DIR)
  set(_g_seen)
  foreach(_g_source IN LISTS _g_sources)
    if(NOT _g_source MATCHES "^\\$<")
      get_filename_component(_g_absolute "${_g_source}" ABSOLUTE BASE_DIR "${_g_base}")
      cmake_path(NORMAL_PATH _g_absolute)
      list(APPEND _g_seen "${_g_absolute}")
    endif()
  endforeach()
  set(_g_required
    "${G_CATALOG_DIR}/catalog_generic_v70.cpp"
    "${G_WORLD_DIR}/canonical_room_zone_factory_v3.cpp"
    "${G_WORLD_DIR}/canonical_room_zone_v3.cpp"
    "${G_WORLD_DIR}/canonical_trigger_trap_v37.cpp"
    "${G_LOADER_DIR}/canonical_auxiliary_families_v16.cpp"
  )
  foreach(_g_source IN LISTS _g_required)
    get_filename_component(_g_absolute "${_g_source}" ABSOLUTE)
    cmake_path(NORMAL_PATH _g_absolute)
    if(NOT EXISTS "${_g_absolute}")
      message(FATAL_ERROR "Missing real generic catalog source: ${_g_absolute}")
    endif()
    if(NOT _g_absolute IN_LIST _g_seen)
      target_sources(${target} PRIVATE "${_g_absolute}")
      list(APPEND _g_seen "${_g_absolute}")
    endif()
  endforeach()
  # Common V67 core and complete V16 dependency closure are supplied by the
  # existing catalog assembly/auxiliary owner target, never a second archive.
  target_include_directories(${target} PUBLIC "${G_CATALOG_DIR}" "${G_COMMON_CATALOG_DIR}" "${G_LOADER_DIR}" "${G_WORLD_DIR}")
endfunction()



