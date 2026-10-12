include_guard(GLOBAL)
include("${CMAKE_CURRENT_LIST_DIR}/native_loading_sources_v65.cmake")
# Append V66 to the SAME actual canonical owner, exporting only an interface.
# PROPERTY_MAP/context/TinyXML/stream CPP files must already be coherent there.
function(dh2_loader_attach_procedural_stream_v66 target)
 cmake_parse_arguments(ARG "" "LOADER_DIR;CORE_TARGET;EXPORT_TARGET" "" ${ARGN})
 if(NOT TARGET "${target}" OR NOT ARG_LOADER_DIR)
  message(FATAL_ERROR "V66 needs the existing canonical owner and explicit coherent loader directory")
 endif()
 if(NOT ARG_EXPORT_TARGET)
  set(ARG_EXPORT_TARGET dh2_loader_procedural_stream_v66)
 endif()
 if(TARGET "${ARG_EXPORT_TARGET}")
  get_target_property(previous_owner "${ARG_EXPORT_TARGET}" DH2_V66_CANONICAL_OWNER)
  if(NOT previous_owner STREQUAL target)
   message(FATAL_ERROR "V66 export already belongs to another canonical owner")
  endif()
  return()
 endif()
 if(ARG_CORE_TARGET)
  if(NOT TARGET "${ARG_CORE_TARGET}")
   message(FATAL_ERROR "V66 procedural core must be an existing actual target")
  endif()
  # Private host integration already compiles these on dh2_level_loader.
  # Patch procedural_sources_v1.* there once; never append a second copy.
  if(NOT ARG_CORE_TARGET STREQUAL target)
   target_link_libraries("${target}" PRIVATE "${ARG_CORE_TARGET}")
  endif()
 else()
  # Root native owner currently has no procedural-core source closure.
  _dh2_loader_append_owned_sources_v65("${target}" "${ARG_LOADER_DIR}"
   procedural_random_v1.cpp procedural_file_list_v1.cpp procedural_sources_v1.cpp
   procedural_blocks_v1.cpp procedural_connections_v1.cpp procedural_lists_v1.cpp
   procedural_rules_v1.cpp procedural_instances_v1.cpp procedural_layout_v1.cpp
   procedural_modules_v1.cpp)
 endif()
 _dh2_loader_append_owned_sources_v65("${target}" "${ARG_LOADER_DIR}"
  procedural_xml_emission_v66.cpp procedural_generator_stream_v66.cpp
  procedural_xml_emission_v66.hpp procedural_generator_stream_v66.hpp
  procedural_sources_v1.hpp procedural_distribution_v1.inc)
 target_compile_features("${target}" PUBLIC cxx_std_17)
 # Consumers link this export; the owner MUST NOT link its own export.
 # No second implementation library or static/shared ownership cycle is added.
 add_library("${ARG_EXPORT_TARGET}" INTERFACE)
 set_property(TARGET "${ARG_EXPORT_TARGET}" PROPERTY DH2_V66_CANONICAL_OWNER "${target}")
 target_link_libraries("${ARG_EXPORT_TARGET}" INTERFACE "${target}")
 target_include_directories("${ARG_EXPORT_TARGET}" INTERFACE "${ARG_LOADER_DIR}")
endfunction()