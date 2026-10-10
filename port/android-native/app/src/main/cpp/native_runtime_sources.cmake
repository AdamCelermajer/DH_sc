# One implementation selection for the Android JNI host and Windows host.
# Entry points and platform adapters belong to their respective executables.
set(DH2_NATIVE_RUNTIME_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}")
set(DH2_NATIVE_RUNTIME_SOURCES)
foreach(source IN ITEMS
    model_renderer.cpp mod_assets.cpp ghost_ai_owner.cpp native_debug_files.cpp
    native_character_list.cpp native_character_roster_transaction.cpp
    native_ghost_skills.cpp native_ghost_death_v1.cpp native_ghost_script_queries.cpp
    native_character_stop.cpp native_loot_pickup_quest_binding_v1.cpp
    native_player_skills.cpp native_player_profile.cpp native_quest_owner.cpp
    native_quest_cursor.cpp native_menu_runtime.cpp native_trophy_runtime.cpp
    original_ui_session.cpp original_ui_assets.cpp swf_gpu.cpp authored_shader_program.cpp)
  list(APPEND DH2_NATIVE_RUNTIME_SOURCES "${DH2_NATIVE_RUNTIME_SOURCE_DIR}/${source}")
endforeach()
list(APPEND DH2_NATIVE_RUNTIME_SOURCES "${DH2_SOURCE_DIR}/port/pydata-names/names.c")
