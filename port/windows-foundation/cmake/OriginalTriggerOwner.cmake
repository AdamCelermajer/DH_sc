# Compile exact existing recovered trigger/contact bodies without unrelated
# exit/checkpoint/quest code from their broad original translation units.
set(_trigger_world "${CMAKE_CURRENT_SOURCE_DIR}/../level-world")
set(_trigger_generated "${CMAKE_CURRENT_BINARY_DIR}/generated-trigger-owner")
file(MAKE_DIRECTORY "${_trigger_generated}")
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS
    "${_trigger_world}/zone_collision_runtime_v83.cpp"
    "${_trigger_world}/navigation_objects.cpp")
file(READ "${_trigger_world}/zone_collision_runtime_v83.cpp" _zone_source)
string(FIND "${_zone_source}" "bool level_checkpoint_save_v83" _zone_boundary)
if(_zone_boundary LESS 0)
    message(FATAL_ERROR "Recovered Zone contact source boundary changed; review binding")
endif()
string(SUBSTRING "${_zone_source}" 0 ${_zone_boundary} _zone_prefix)
file(WRITE "${_trigger_generated}/zone-contact-source.cpp" "${_zone_prefix}\n}\n")
add_library(recovered_trigger_contacts STATIC
    "${_trigger_world}/canonical_trigger_zone_v22.cpp"
    "${_trigger_world}/canonical_gameobject_base_owner_v1.cpp"
    "${_trigger_world}/game_object_initialization_owner_v1.cpp"
    "${_trigger_generated}/zone-contact-source.cpp")
target_include_directories(recovered_trigger_contacts PRIVATE "${_trigger_world}")
target_link_libraries(recovered_trigger_contacts PUBLIC recovered_content)
