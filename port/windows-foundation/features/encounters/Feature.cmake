# Root may include this optional feature registration in the core build.
# The feature itself does not edit the root CMakeLists.txt.
target_sources(foundation_data PRIVATE
  features/encounters/encounter_services.cpp
  features/encounters/trigger_constructors.cpp
  features/encounters/npc_profile_binding.cpp
  features/encounters/activation_bindings.cpp
  ../level-world/application_spawn_random_owner_v4.cpp
  ../game-data/character_templates_v78.cpp
  ../level-world/character_props_id_owner_v1.cpp
  ../level-world/object_enable_condition_v2.cpp
  ../level-world/gameobject_online_update_v5.cpp)
add_executable(feature_encounters_connected_tests features/encounters/connected_tests.cpp)
target_link_libraries(feature_encounters_connected_tests PRIVATE foundation_data)
add_test(NAME feature_encounters_connected COMMAND feature_encounters_connected_tests "${CMAKE_CURRENT_SOURCE_DIR}/../..")
add_executable(feature_encounters_npc_profile_tests features/encounters/npc_profile_tests.cpp)
target_link_libraries(feature_encounters_npc_profile_tests PRIVATE foundation_data)
add_test(NAME feature_encounters_npc_profile COMMAND feature_encounters_npc_profile_tests "${CMAKE_CURRENT_SOURCE_DIR}/../..")
