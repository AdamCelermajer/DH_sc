# Scoped original StringManager/Item text dependency closure. Include only
# after the existing dh2_game_data target; no GameSWF/menu/renderer import.
if(NOT TARGET dh2_game_data)
 message(FATAL_ERROR "Inventory text requires the existing dh2_game_data target")
endif()
add_library(dh2_inventory_text_v1 SHARED
 "${CMAKE_CURRENT_LIST_DIR}/localization.cpp"
 "${CMAKE_CURRENT_LIST_DIR}/hud_text_v1.cpp"
 "${CMAKE_CURRENT_LIST_DIR}/hud_text_format_v1.cpp"
 "${CMAKE_CURRENT_LIST_DIR}/item_text_varargs_v5.cpp"
 "${CMAKE_CURRENT_LIST_DIR}/item_text_owner_v5.cpp")
target_compile_features(dh2_inventory_text_v1 PUBLIC cxx_std_17)
target_compile_options(dh2_inventory_text_v1 PRIVATE -Wall -Wextra -fno-fast-math -ffp-contract=off)
target_include_directories(dh2_inventory_text_v1 PUBLIC "${CMAKE_CURRENT_LIST_DIR}")
target_link_libraries(dh2_inventory_text_v1 PUBLIC dh2_game_data)
if(ANDROID)
 target_link_options(dh2_inventory_text_v1 PRIVATE -Wl,-z,max-page-size=16384 -Wl,--no-undefined)
endif()
