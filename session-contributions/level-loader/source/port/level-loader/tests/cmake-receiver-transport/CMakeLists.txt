cmake_minimum_required(VERSION 3.16)
project(dh2_loader_receiver_transport LANGUAGES C CXX)
get_filename_component(loader "${CMAKE_CURRENT_LIST_DIR}/../.." ABSOLUTE)
set(DH2_LOADER_GRAPH_SNAPSHOT "${loader}/vendor/character-rng-integration-v5-loading" CACHE PATH "Coherent canonical graph source tree")
set(snapshot "${DH2_LOADER_GRAPH_SNAPSHOT}/port/level-world")
add_library(dh2_loader_transport_source_owners STATIC
 "${snapshot}/canonical_object_manager_v1.cpp"
 "${snapshot}/canonical_object_factory_v1.cpp"
 "${snapshot}/canonical_property_map_v1.cpp")
target_compile_features(dh2_loader_transport_source_owners PUBLIC cxx_std_17)
target_include_directories(dh2_loader_transport_source_owners PUBLIC "${snapshot}" "${loader}/../level-world")
target_compile_options(dh2_loader_transport_source_owners PRIVATE -Wall -Wextra -Werror -Wno-misleading-indentation -fno-fast-math -ffp-contract=off)
set(DH2_LOADER_CANONICAL_OWNER_TARGET dh2_loader_transport_source_owners CACHE STRING "" FORCE)
target_sources(dh2_loader_transport_source_owners PRIVATE "${snapshot}/module_xml_selection_v1.cpp")
set(DH2_LOADER_MODULE_OWNER_TARGET dh2_loader_transport_source_owners CACHE STRING "" FORCE)
add_subdirectory("${loader}" "${CMAKE_CURRENT_BINARY_DIR}/loader")
if(TARGET dh2_loader_canonical_owners)
 message(FATAL_ERROR "Second canonical manager/factory owner target created")
endif()

set(graph_snapshot "${DH2_LOADER_GRAPH_SNAPSHOT}")
set(config_snapshot "${graph_snapshot}/port/level-world")
add_library(dh2_loader_config_source_contract STATIC
 "${config_snapshot}/canonical_level_config_module_v1.cpp"
 "${config_snapshot}/level_config_publication_v2.cpp")
target_compile_features(dh2_loader_config_source_contract PUBLIC cxx_std_17)
target_include_directories(dh2_loader_config_source_contract PUBLIC "${config_snapshot}" "${graph_snapshot}/port/level-loader")
target_link_libraries(dh2_loader_config_source_contract PUBLIC dh2_loader_canonical_receiver_transport)
# Only the actual LevelConfig methods are reached by this probe. Separate
# function sections let the linker discard unrelated incomplete Module paths;
# no replacement functions or unresolved-symbol suppression are supplied.
target_compile_options(dh2_loader_config_source_contract PRIVATE -Wall -Wextra -Werror -Wno-misleading-indentation -ffunction-sections -fdata-sections -fno-fast-math -ffp-contract=off)
target_include_directories(dh2_loader_canonical_level_context BEFORE PUBLIC "${graph_snapshot}/port/level-world" "${graph_snapshot}/port/level-loader")
target_include_directories(dh2_loader_canonical_module_files BEFORE PUBLIC "${graph_snapshot}/port/level-world" "${graph_snapshot}/port/level-loader")
# Compile context and its relay from ONE coherent physical header tree.
set(context_sources "${graph_snapshot}/port/level-loader/canonical_level_context_v1.cpp")
if(EXISTS "${graph_snapshot}/port/level-loader/canonical_level_prepared_source_v25.cpp")
 list(APPEND context_sources "${graph_snapshot}/port/level-loader/canonical_level_prepared_source_v25.cpp")
endif()
set_property(TARGET dh2_loader_canonical_level_context PROPERTY SOURCES ${context_sources})
set_property(TARGET dh2_loader_canonical_module_files PROPERTY SOURCES "${graph_snapshot}/port/level-loader/canonical_module_files_v1.cpp")
add_library(dh2_loader_level_constructor_source STATIC "${graph_snapshot}/port/level-loader/level_constructor_v3.cpp" "${graph_snapshot}/port/level-world/event_manager_owner_v12.cpp")
target_compile_features(dh2_loader_level_constructor_source PUBLIC cxx_std_17)
target_include_directories(dh2_loader_level_constructor_source PUBLIC "${graph_snapshot}/port/level-loader" "${graph_snapshot}/port/level-world")
target_compile_options(dh2_loader_level_constructor_source PRIVATE -Wall -Wextra -Werror -Wno-misleading-indentation -ffunction-sections -fdata-sections)
target_link_libraries(dh2_loader_canonical_level_context PUBLIC dh2_loader_level_constructor_source)
add_library(dh2_loader_coherent_level_context INTERFACE)
target_compile_features(dh2_loader_coherent_level_context INTERFACE cxx_std_17)
target_include_directories(dh2_loader_coherent_level_context INTERFACE "${graph_snapshot}/port/level-loader" "${config_snapshot}")
target_link_libraries(dh2_loader_coherent_level_context INTERFACE dh2_loader_canonical_level_context)
add_executable(dh2_loader_config_source_probe "${loader}/tests/canonical_config_source_probe.cpp")
target_compile_options(dh2_loader_config_source_probe PRIVATE -Wall -Wextra -Werror -Wno-misleading-indentation)
target_link_libraries(dh2_loader_config_source_probe PRIVATE dh2_loader_config_source_contract dh2_loader_coherent_level_context)
target_link_options(dh2_loader_config_source_probe PRIVATE -Wl,--gc-sections -Wl,--no-undefined)

# One authoritative kernel implementation per target/world; reuse target names
# while selecting incoming coherent source paths for this standalone build.
set_property(TARGET dh2_loader_floors PROPERTY SOURCES
 "${config_snapshot}/floors.cpp"
 "${config_snapshot}/floor_source.cpp"
 "${config_snapshot}/selector.cpp"
 "${config_snapshot}/collision.cpp"
 "${config_snapshot}/octree.cpp"
 "${config_snapshot}/navigation.cpp"
 "${config_snapshot}/navigation_search.cpp"
 "${config_snapshot}/navigation_world.cpp"
 "${config_snapshot}/navigation_path.cpp"
 "${config_snapshot}/navigation_motion.cpp"
)
target_include_directories(dh2_loader_floors BEFORE PUBLIC "${config_snapshot}")
set_property(TARGET dh2_loader_scene PROPERTY SOURCES
 "${graph_snapshot}/port/scene-materials/scene.cpp"
 "${graph_snapshot}/port/engine-resources/resources.cpp"
 "${graph_snapshot}/port/asset-payloads/payloads.cpp"
 "${graph_snapshot}/port/engine-math/math.cpp"
)
target_include_directories(dh2_loader_scene BEFORE PUBLIC "${config_snapshot}")
add_library(dh2_loader_module_graph_owners STATIC
 "${config_snapshot}/canonical_gameobject_base_owner_v1.cpp"
 "${config_snapshot}/canonical_level_module_bindings_v2.cpp"
 "${config_snapshot}/canonical_module_graph_v3.cpp"
 "${config_snapshot}/canonical_room_zone_factory_v3.cpp"
 "${config_snapshot}/canonical_room_zone_v3.cpp"
 "${config_snapshot}/canonical_spawn_owner_v1.cpp"
 "${config_snapshot}/condition_data_init_v3.cpp"
 "${config_snapshot}/decor_body_config.cpp"
 "${config_snapshot}/decor_scene.cpp"
 "${config_snapshot}/game_object_initialization_owner_v1.cpp"
 "${config_snapshot}/game_object_relative_box_v3.cpp"
 "${config_snapshot}/game_object_set_position_v2.cpp"
 "${config_snapshot}/game_object_visual_asset_owner_v1.cpp"
 "${config_snapshot}/gameobject_scene_root_registry_v1.cpp"
 "${config_snapshot}/level_config_publication_v2.cpp"
 "${config_snapshot}/light_set_name_owner_v3.cpp"
 "${config_snapshot}/module_floor_append_v2.cpp"
 "${config_snapshot}/module_floor_clone_v3.cpp"
 "${config_snapshot}/module_floor_graph_v3.cpp"
 "${config_snapshot}/module_pf_actor_connection_v3.cpp"
 "${config_snapshot}/module_pf_room_v3.cpp"
 "${config_snapshot}/module_room_zone_spawn_v3.cpp"
 "${config_snapshot}/module_selected_scene_v2.cpp"
 "${config_snapshot}/module_static_scene_bounds_v3.cpp"
 "${config_snapshot}/module_static_scene_v2.cpp"
 "${config_snapshot}/module_visual_mesh_box_v2.cpp"
 "${config_snapshot}/navigation_objects.cpp"
 "${config_snapshot}/navigation_producers.cpp"
 "${config_snapshot}/object_enable_condition_v2.cpp"
 "${config_snapshot}/retained_module_visual_v3.cpp"
 "${config_snapshot}/visual_anim_controller_owner_v4.cpp"
 "${config_snapshot}/scene_manager_map_owner_v2.cpp"
 "${config_snapshot}/user_properties_v2.cpp"
)
target_compile_features(dh2_loader_module_graph_owners PUBLIC cxx_std_17)
target_include_directories(dh2_loader_module_graph_owners PUBLIC "${config_snapshot}" "${graph_snapshot}/port/level-loader")
target_compile_options(dh2_loader_module_graph_owners PRIVATE -Wall -Wextra -Werror -Wno-misleading-indentation -ffunction-sections -fdata-sections -fno-fast-math -ffp-contract=off)
target_link_libraries(dh2_loader_module_graph_owners PUBLIC dh2_loader_config_source_contract dh2_level_loader)
add_subdirectory("${graph_snapshot}/port/physics-backend" "${CMAKE_CURRENT_BINARY_DIR}/openable-box2d")
add_library(dh2_loader_openable_graph_owners STATIC
 "${graph_snapshot}/port/engine-animation/angle_interpreter.cpp"
 "${graph_snapshot}/port/engine-animation/animation.cpp"
 "${graph_snapshot}/port/engine-animation/events.cpp"
 "${graph_snapshot}/port/engine-animation/event_track.cpp"
 "${graph_snapshot}/port/engine-skinning/skinning.cpp"
 "${graph_snapshot}/port/game-data/animation_tables.cpp"
 "${graph_snapshot}/port/game-data/data.cpp"
 "${graph_snapshot}/port/level-world/actor_rotation.cpp"
 "${graph_snapshot}/port/level-world/actor_runtime.cpp"
 "${graph_snapshot}/port/level-world/authored_scene_subtree_v2.cpp"
 "${graph_snapshot}/port/level-world/base_named_animation_controller_v1.cpp"
 "${graph_snapshot}/port/level-world/canonical_class_receiver_bindings_v1.cpp"
 "${graph_snapshot}/port/level-world/canonical_openable_container_v1.cpp"
 "${graph_snapshot}/port/level-world/canonical_openable_graph_v4.cpp"
 "${graph_snapshot}/port/level-world/character_body_config.cpp"
 "${graph_snapshot}/port/level-world/character_combat_queries.cpp"
 "${graph_snapshot}/port/level-world/character_target_providers.cpp"
 "${graph_snapshot}/port/level-world/character_world_npc_object_v1.cpp"
 "${graph_snapshot}/port/level-world/gameobject_scene_binding_v1.cpp"
 "${graph_snapshot}/port/level-world/move_state.cpp"
 "${graph_snapshot}/port/level-world/native_body.cpp"
 "${graph_snapshot}/port/level-world/navigation_avoidance.cpp"
 "${graph_snapshot}/port/level-world/navigation_controller.cpp"
 "${graph_snapshot}/port/level-world/navigation_heading.cpp"
 "${graph_snapshot}/port/level-world/openable_container_owner_v1.cpp"
 "${graph_snapshot}/port/level-world/openable_container_property_connection_v1.cpp"
 "${graph_snapshot}/port/level-world/physical_controls.cpp"
 "${graph_snapshot}/port/level-world/physical_world.cpp"
 "${graph_snapshot}/port/level-world/retained_gameobject_decor_v1.cpp"
 "${graph_snapshot}/port/level-world/retained_gameobject_visual_asset_connection_v1.cpp"
 "${graph_snapshot}/port/level-world/retained_gameobject_visual_asset_connection_v2.cpp"
 "${graph_snapshot}/port/level-world/retained_gameobject_visual_v1.cpp"
 "${graph_snapshot}/port/level-world/retained_scene_visual_connection_v3.cpp"
 "${graph_snapshot}/port/level-world/subobjects_update.cpp"
 "${graph_snapshot}/port/level-world/visual_aabb_dispatch_scope_v3.cpp"
 "${graph_snapshot}/port/level-world/visual_mesh_box_fallback_v2.cpp"
 "${graph_snapshot}/port/level-world/visual_motion.cpp"
 "${graph_snapshot}/port/level-world/visual_timeline.cpp"
)
target_compile_features(dh2_loader_openable_graph_owners PUBLIC cxx_std_17)
target_include_directories(dh2_loader_openable_graph_owners PUBLIC "${config_snapshot}")
target_compile_options(dh2_loader_openable_graph_owners PRIVATE -Wall -Wextra -Werror -Wno-misleading-indentation -ffunction-sections -fdata-sections -fno-fast-math -ffp-contract=off)
target_link_libraries(dh2_loader_openable_graph_owners PUBLIC dh2_loader_module_graph_owners dh2_box2d_201)

# Render bridge consumes this SAME authoritative graph owner target.
add_library(dh2_loader_module_draw_frame STATIC "${loader}/module_draw_frame_v1.cpp")
target_compile_features(dh2_loader_module_draw_frame PUBLIC cxx_std_17)
target_include_directories(dh2_loader_module_draw_frame PRIVATE "${loader}")
target_compile_options(dh2_loader_module_draw_frame PRIVATE -Wall -Wextra -Werror)
target_link_libraries(dh2_loader_module_draw_frame PUBLIC dh2_loader_module_graph_owners)
add_library(dh2_loader_retained_module_graph STATIC "${loader}/retained_level_module_graph_v1.cpp")
target_compile_features(dh2_loader_retained_module_graph PUBLIC cxx_std_17)
target_compile_options(dh2_loader_retained_module_graph PRIVATE -Wall -Wextra -Werror -Wno-misleading-indentation)
target_include_directories(dh2_loader_retained_module_graph PUBLIC "${graph_snapshot}/port/level-loader" "${loader}")
target_link_libraries(dh2_loader_retained_module_graph PUBLIC dh2_loader_module_draw_frame dh2_loader_coherent_level_context dh2_loader_canonical_module_files)
add_executable(dh2_loader_module_graph_source_probe "${loader}/tests/canonical_module_graph_source_probe.cpp")
target_compile_options(dh2_loader_module_graph_source_probe PRIVATE -Wall -Wextra -Werror -Wno-misleading-indentation -UNDEBUG)
target_link_libraries(dh2_loader_module_graph_source_probe PRIVATE dh2_loader_openable_graph_owners dh2_loader_retained_module_graph)
target_link_options(dh2_loader_module_graph_source_probe PRIVATE -Wl,--gc-sections -Wl,--no-undefined)

add_executable(dh2_loader_config_across_regression "${loader}/vendor/config-across-a3df90efd191542c/port/level-world/tests/canonical_level_config_v1.cpp")
target_compile_features(dh2_loader_config_across_regression PRIVATE cxx_std_17)
target_compile_options(dh2_loader_config_across_regression PRIVATE -Wall -Wextra -Werror -Wno-misleading-indentation -UNDEBUG)
target_link_libraries(dh2_loader_config_across_regression PRIVATE dh2_loader_config_source_contract dh2_loader_module_graph_owners)
target_link_options(dh2_loader_config_across_regression PRIVATE -Wl,--gc-sections -Wl,--no-undefined)

add_executable(dh2_loader_room_zone_across_regression "${loader}/vendor/room-zone-leaf-v4/room_zone_byte87_safety_v4.cpp")
target_compile_features(dh2_loader_room_zone_across_regression PRIVATE cxx_std_17)
target_compile_options(dh2_loader_room_zone_across_regression PRIVATE -Wall -Wextra -Werror -Wno-misleading-indentation -UNDEBUG)
target_link_libraries(dh2_loader_room_zone_across_regression PRIVATE dh2_loader_module_graph_owners)
target_link_options(dh2_loader_room_zone_across_regression PRIVATE -Wl,--gc-sections -Wl,--no-undefined)
add_library(dh2_loader_character_rng_owners STATIC
 "${graph_snapshot}/port/level-world/canonical_character_family_v4.cpp"
 "${graph_snapshot}/port/level-world/retained_character_actor_v1.cpp"
 "${graph_snapshot}/port/level-world/character_npc_initpost_owner_v1.cpp"
 "${graph_snapshot}/port/level-world/application_spawn_random_owner_v4.cpp"
 "${graph_snapshot}/port/game-data/loot_tables_v2.cpp"
 "${graph_snapshot}/port/level-world/character_game_design.cpp"
 "${graph_snapshot}/port/game-data/properties.cpp"
 "${graph_snapshot}/port/game-data/class_tables.cpp"
 "${graph_snapshot}/port/game-data/items.cpp"
 "${graph_snapshot}/port/game-data/game_design_tables.cpp"
 "${graph_snapshot}/port/game-data/level_tables.cpp"
 "${graph_snapshot}/port/game-data/ai.cpp"
 "${graph_snapshot}/port/game-data/fresh_inventory_owned_v4.cpp"
 "${graph_snapshot}/port/level-world/character_script_objects.cpp"
)
target_sources(dh2_loader_character_rng_owners PRIVATE
 "${graph_snapshot}/port/level-world/character_world_npc_state_owner_v1.cpp"
 "${graph_snapshot}/port/level-world/character_world_npc_initialization_v1.cpp"
 "${graph_snapshot}/port/level-world/character_idle_events.cpp"
 "${graph_snapshot}/port/level-world/character_script_session.cpp"
 "${graph_snapshot}/port/level-world/character_script_owner.cpp"
 "${graph_snapshot}/port/level-world/character_scene.cpp"
 "${graph_snapshot}/port/level-world/character_state_owner.cpp"
 "${graph_snapshot}/port/level-world/character_state_owner_extensions.cpp"
 "${graph_snapshot}/port/level-world/character_state_owner_behavior.cpp"
 "${graph_snapshot}/port/level-world/character_native_fsm.cpp"
 "${graph_snapshot}/port/level-world/character_design_services.cpp"
 "${graph_snapshot}/port/level-world/character_ai_state_changed.cpp"
 "${graph_snapshot}/port/level-world/character_state.cpp"
 "${graph_snapshot}/port/level-world/character_state_empty.cpp"
 "${graph_snapshot}/port/level-world/character_pre_spawn.cpp"
)
target_sources(dh2_loader_character_rng_owners PRIVATE
 "${graph_snapshot}/port/game-data/game_object_dictionary_v11.cpp"
 "${graph_snapshot}/port/level-world/openable_container_data_connection_v11.cpp")
target_sources(dh2_loader_character_rng_owners PRIVATE
 "${graph_snapshot}/port/level-world/character_set_position_v7.cpp"
 "${graph_snapshot}/port/level-world/retained_character_position_owner_v7.cpp")
target_sources(dh2_loader_character_rng_owners PRIVATE "${graph_snapshot}/port/level-world/canonical_dummy_owner_v14.cpp")
target_sources(dh2_loader_character_rng_owners PRIVATE
 "${graph_snapshot}/port/level-world/canonical_spawn_point_v15.cpp"
 "${graph_snapshot}/port/level-world/canonical_decor_v15.cpp"
 "${graph_snapshot}/port/level-world/canonical_destructible_container_v16.cpp"
 "${graph_snapshot}/port/level-world/destructible_container_data_v16.cpp")
target_compile_features(dh2_loader_character_rng_owners PUBLIC cxx_std_17)
target_include_directories(dh2_loader_character_rng_owners PUBLIC "${config_snapshot}" "${graph_snapshot}/port/game-data")
target_compile_options(dh2_loader_character_rng_owners PRIVATE -Wall -Wextra -Werror -Wno-misleading-indentation -ffunction-sections -fdata-sections -fno-fast-math -ffp-contract=off)
target_link_libraries(dh2_loader_character_rng_owners PUBLIC dh2_loader_openable_graph_owners)
add_executable(dh2_loader_character_family_prefix_probe "${graph_snapshot}/port/level-world/tests/canonical_character_family_v4.cpp")
target_compile_options(dh2_loader_character_family_prefix_probe PRIVATE -Wall -Wextra -Werror -Wno-misleading-indentation -UNDEBUG)
target_link_libraries(dh2_loader_character_family_prefix_probe PRIVATE dh2_loader_character_rng_owners)
target_link_options(dh2_loader_character_family_prefix_probe PRIVATE -Wl,--gc-sections -Wl,--no-undefined)

add_library(dh2_loader_level_class_dispatch STATIC "${loader}/canonical_level_class_dispatch_v1.cpp")
target_compile_features(dh2_loader_level_class_dispatch PUBLIC cxx_std_17)
target_compile_options(dh2_loader_level_class_dispatch PRIVATE -Wall -Wextra -Werror -Wno-misleading-indentation)
target_include_directories(dh2_loader_level_class_dispatch PUBLIC "${graph_snapshot}/port/level-world" "${loader}")
target_link_libraries(dh2_loader_level_class_dispatch PUBLIC dh2_loader_character_rng_owners)
target_link_libraries(dh2_loader_module_graph_source_probe PRIVATE dh2_loader_level_class_dispatch)
add_executable(dh2_loader_application_rng_probe "${graph_snapshot}/port/level-world/tests/application_spawn_random_owner_v4.cpp")
target_compile_options(dh2_loader_application_rng_probe PRIVATE -Wall -Wextra -Werror -Wno-misleading-indentation -UNDEBUG)
target_link_libraries(dh2_loader_application_rng_probe PRIVATE dh2_loader_character_rng_owners)
target_link_options(dh2_loader_application_rng_probe PRIVATE -Wl,--gc-sections -Wl,--no-undefined)

add_executable(dh2_loader_chest_data_probe "${loader}/vendor/openable-container-data-v11-7575323594a91cf7/port/level-world/tests/openable_container_data_connection_v11.cpp")
target_compile_options(dh2_loader_chest_data_probe PRIVATE -Wall -Wextra -Werror -Wno-misleading-indentation -UNDEBUG)
target_link_libraries(dh2_loader_chest_data_probe PRIVATE dh2_loader_character_rng_owners)
target_link_options(dh2_loader_chest_data_probe PRIVATE -Wl,--gc-sections -Wl,--no-undefined)

add_executable(dh2_loader_character_position_kernel_probe "${graph_snapshot}/port/level-world/tests/character_set_position_v7.cpp")
target_compile_options(dh2_loader_character_position_kernel_probe PRIVATE -Wall -Wextra -Werror -Wno-misleading-indentation -UNDEBUG)
target_include_directories(dh2_loader_character_position_kernel_probe BEFORE PRIVATE "${graph_snapshot}/port/level-world")
target_link_libraries(dh2_loader_character_position_kernel_probe PRIVATE dh2_loader_character_rng_owners)
target_link_options(dh2_loader_character_position_kernel_probe PRIVATE -Wl,--gc-sections -Wl,--no-undefined)

add_executable(dh2_loader_character_position_owner_probe "${graph_snapshot}/port/level-world/tests/retained_character_position_owner_v7.cpp")
target_compile_options(dh2_loader_character_position_owner_probe PRIVATE -Wall -Wextra -Werror -Wno-misleading-indentation -UNDEBUG)
target_include_directories(dh2_loader_character_position_owner_probe BEFORE PRIVATE "${graph_snapshot}/port/level-world")
target_link_libraries(dh2_loader_character_position_owner_probe PRIVATE dh2_loader_character_rng_owners)
target_link_options(dh2_loader_character_position_owner_probe PRIVATE -Wl,--gc-sections -Wl,--no-undefined)

add_executable(dh2_loader_character_position_prefix_probe "${graph_snapshot}/port/level-world/tests/canonical_character_position_prefix_v7.cpp")
target_compile_options(dh2_loader_character_position_prefix_probe PRIVATE -Wall -Wextra -Werror -Wno-misleading-indentation -UNDEBUG)
target_include_directories(dh2_loader_character_position_prefix_probe BEFORE PRIVATE "${graph_snapshot}/port/level-world")
target_link_libraries(dh2_loader_character_position_prefix_probe PRIVATE dh2_loader_character_rng_owners)
target_link_options(dh2_loader_character_position_prefix_probe PRIVATE -Wl,--gc-sections -Wl,--no-undefined)

add_library(dh2_loader_device_pipeline_borrow STATIC "${loader}/device_pipeline_borrow_v1.cpp" "${graph_snapshot}/port/engine-ui/hud_startup_callbacks.cpp")
target_compile_features(dh2_loader_device_pipeline_borrow PUBLIC cxx_std_17)
target_include_directories(dh2_loader_device_pipeline_borrow PUBLIC "${graph_snapshot}/port/engine-ui" "${loader}")
target_compile_options(dh2_loader_device_pipeline_borrow PRIVATE -Wall -Wextra -Werror -Wno-misleading-indentation -ffunction-sections -fdata-sections)
target_link_libraries(dh2_loader_module_graph_source_probe PRIVATE dh2_loader_device_pipeline_borrow)

add_executable(dh2_loader_dummy_source_probe "${graph_snapshot}/port/level-world/tests/canonical_dummy_v14.cpp")
target_compile_options(dh2_loader_dummy_source_probe PRIVATE -Wall -Wextra -Werror -Wno-misleading-indentation -UNDEBUG)
target_link_libraries(dh2_loader_dummy_source_probe PRIVATE dh2_loader_character_rng_owners)
target_link_options(dh2_loader_dummy_source_probe PRIVATE -Wl,--gc-sections -Wl,--no-undefined)

add_executable(dh2_loader_same_level_ctor_probe "${graph_snapshot}/port/level-loader/tests/canonical_level_constructor_v3.cpp")
target_compile_options(dh2_loader_same_level_ctor_probe PRIVATE -Wall -Wextra -Werror -Wno-misleading-indentation -UNDEBUG)
target_link_libraries(dh2_loader_same_level_ctor_probe PRIVATE dh2_loader_coherent_level_context)
target_link_options(dh2_loader_same_level_ctor_probe PRIVATE -Wl,--gc-sections -Wl,--no-undefined)

add_executable(dh2_loader_swamp_families_source_probe "${graph_snapshot}/port/level-world/tests/canonical_swamp_family_v15.cpp")
target_compile_options(dh2_loader_swamp_families_source_probe PRIVATE -Wall -Wextra -Werror -Wno-misleading-indentation -UNDEBUG)
target_link_libraries(dh2_loader_swamp_families_source_probe PRIVATE dh2_loader_character_rng_owners)
target_link_options(dh2_loader_swamp_families_source_probe PRIVATE -Wl,--gc-sections -Wl,--no-undefined)

add_executable(dh2_loader_destructible_source_probe "${graph_snapshot}/port/level-world/tests/canonical_destructible_container_v16.cpp")
target_compile_options(dh2_loader_destructible_source_probe PRIVATE -Wall -Wextra -Werror -Wno-misleading-indentation -UNDEBUG)
target_link_libraries(dh2_loader_destructible_source_probe PRIVATE dh2_loader_character_rng_owners)
target_link_options(dh2_loader_destructible_source_probe PRIVATE -Wl,--gc-sections -Wl,--no-undefined)

add_library(dh2_loader_auxiliary_families STATIC "${loader}/canonical_auxiliary_families_v16.cpp")
target_compile_features(dh2_loader_auxiliary_families PUBLIC cxx_std_17)
target_include_directories(dh2_loader_auxiliary_families BEFORE PUBLIC "${graph_snapshot}/port/level-world" "${loader}")
target_compile_options(dh2_loader_auxiliary_families PRIVATE -Wall -Wextra -Werror -Wno-misleading-indentation -ffunction-sections -fdata-sections)
target_link_libraries(dh2_loader_auxiliary_families PUBLIC dh2_loader_character_rng_owners)
target_link_libraries(dh2_loader_module_graph_source_probe PRIVATE dh2_loader_auxiliary_families)

# The V3 TU textually includes V1/runtime. They are never standalone TUs.
set(DH2_SCRIPT_V24_ROOT "${graph_snapshot}")
include("${graph_snapshot}/port/script-runtime/reports/native-production-closure-v24/script-runtime-production-v24.cmake")
target_link_libraries(dh2_loader_character_rng_owners PUBLIC dh2_script_runtime)
target_sources(dh2_loader_character_rng_owners PRIVATE "${graph_snapshot}/port/game-data/player_profile_index_v1.cpp")
add_library(dh2_loader_native_ctor_bindings STATIC
 "${graph_snapshot}/port/level-loader/level_constructor_bindings_v4.cpp"
 "${graph_snapshot}/port/level-world/generic_lua_script_owner_v13.cpp"
 "${graph_snapshot}/port/level-world/level_savegame_owner_v1.cpp"
 "${graph_snapshot}/port/level-world/level_savegame_cache_v1.cpp"
 "${graph_snapshot}/port/level-world/level_savegame_runtime_v1.cpp")
target_compile_features(dh2_loader_native_ctor_bindings PUBLIC cxx_std_17)
target_include_directories(dh2_loader_native_ctor_bindings PUBLIC "${graph_snapshot}/port/level-loader" "${graph_snapshot}/port/level-world")
target_compile_options(dh2_loader_native_ctor_bindings PRIVATE -Wall -Wextra -Werror -Wno-misleading-indentation -ffunction-sections -fdata-sections)
target_link_libraries(dh2_loader_native_ctor_bindings PUBLIC dh2_loader_coherent_level_context dh2_loader_character_rng_owners dh2_script_runtime)
add_executable(dh2_loader_native_ctor_bindings_probe "${graph_snapshot}/port/level-loader/tests/level_constructor_bindings_v4.cpp")
target_compile_options(dh2_loader_native_ctor_bindings_probe PRIVATE -Wall -Wextra -Werror -Wno-misleading-indentation -UNDEBUG)
target_link_libraries(dh2_loader_native_ctor_bindings_probe PRIVATE dh2_loader_native_ctor_bindings)
target_link_options(dh2_loader_native_ctor_bindings_probe PRIVATE -Wl,--gc-sections -Wl,--no-undefined)

target_link_libraries(dh2_loader_module_graph_source_probe PRIVATE dh2_loader_native_ctor_bindings)

# GCC requires the C++ fixed-width names explicitly. Keep the frozen source
# and public headers byte-identical; include cstdint before this original TU.
set_source_files_properties("${graph_snapshot}/port/level-world/generic_lua_script_owner_v13.cpp"
 PROPERTIES COMPILE_OPTIONS "-include;cstdint")

target_sources(dh2_loader_openable_graph_owners PRIVATE
 "${graph_snapshot}/port/level-world/generic_animation_callbacks_v21.cpp"
 "${graph_snapshot}/port/level-world/base_index_animation_controller_v21.cpp"
 "${graph_snapshot}/port/level-world/retained_generic_animator_v21.cpp"
 "${graph_snapshot}/port/level-world/container_animation_connection_v21.cpp"
 "${graph_snapshot}/port/level-world/canonical_openable_graph_v21.cpp")
add_executable(dh2_loader_container_callbacks_v21_probe "${graph_snapshot}/port/level-world/tests/canonical_openable_graph_v21.cpp")
target_compile_options(dh2_loader_container_callbacks_v21_probe PRIVATE -Wall -Wextra -Werror -Wno-misleading-indentation -UNDEBUG)
target_link_libraries(dh2_loader_container_callbacks_v21_probe PRIVATE dh2_loader_character_rng_owners)
target_link_options(dh2_loader_container_callbacks_v21_probe PRIVATE -Wl,--gc-sections -Wl,--no-undefined)

target_sources(dh2_loader_character_rng_owners PRIVATE "${graph_snapshot}/port/level-world/canonical_trigger_zone_v22.cpp")
add_executable(dh2_loader_trigger_zone_v22_probe "${graph_snapshot}/port/level-world/tests/canonical_trigger_zone_v22.cpp")
target_compile_options(dh2_loader_trigger_zone_v22_probe PRIVATE -Wall -Wextra -Werror -Wno-misleading-indentation -UNDEBUG)
target_link_libraries(dh2_loader_trigger_zone_v22_probe PRIVATE dh2_loader_character_rng_owners)
target_link_options(dh2_loader_trigger_zone_v22_probe PRIVATE -Wl,--gc-sections -Wl,--no-undefined)

# Keep the frozen fixture unchanged while making its std::copy_n dependency
# explicit for GCC, which does not inherit algorithm from unrelated headers.
target_compile_options(dh2_loader_trigger_zone_v22_probe PRIVATE -include algorithm)

target_sources(dh2_loader_character_rng_owners PRIVATE "${graph_snapshot}/port/level-world/canonical_animated_decor_v23.cpp")

target_sources(dh2_loader_character_rng_owners PRIVATE "${graph_snapshot}/port/level-world/canonical_checkpoint_zone_v26.cpp")

target_sources(dh2_loader_character_rng_owners PRIVATE "${graph_snapshot}/port/level-world/canonical_door_v27.cpp")

target_sources(dh2_loader_character_rng_owners PRIVATE "${graph_snapshot}/port/level-world/canonical_trigger_object_v28.cpp")

target_sources(dh2_loader_character_rng_owners PRIVATE "${graph_snapshot}/port/level-world/canonical_exit_zone_v29.cpp")

add_executable(dh2_loader_preview_entity_export_v30 ../canonical_preview_entity_export_v30.cpp)
target_link_libraries(dh2_loader_preview_entity_export_v30 PRIVATE dh2_loader_openable_graph_owners dh2_loader_retained_module_graph)

target_include_directories(dh2_loader_preview_entity_export_v30 BEFORE PRIVATE "$<TARGET_PROPERTY:dh2_loader_module_graph_source_probe,INCLUDE_DIRECTORIES>")
target_compile_features(dh2_loader_preview_entity_export_v30 PRIVATE cxx_std_17)

target_link_libraries(dh2_loader_preview_entity_export_v30 PRIVATE "$<TARGET_PROPERTY:dh2_loader_module_graph_source_probe,LINK_LIBRARIES>")

target_link_options(dh2_loader_preview_entity_export_v30 PRIVATE "$<TARGET_PROPERTY:dh2_loader_module_graph_source_probe,LINK_OPTIONS>")

target_sources(dh2_loader_character_rng_owners PRIVATE "${graph_snapshot}/port/level-world/canonical_quest_move_zone_v31.cpp")

target_sources(dh2_loader_character_rng_owners PRIVATE "${graph_snapshot}/port/level-world/canonical_sound_emitter_v32.cpp")

add_executable(dh2_loader_character_template_assets_v35_probe ../character_templates_v35.cpp ../../character_template_assets_v35.cpp)
target_include_directories(dh2_loader_character_template_assets_v35_probe PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../.." $<TARGET_PROPERTY:dh2_loader_module_graph_source_probe,INCLUDE_DIRECTORIES>)
target_link_libraries(dh2_loader_character_template_assets_v35_probe PRIVATE $<TARGET_PROPERTY:dh2_loader_module_graph_source_probe,LINK_LIBRARIES>)
target_link_options(dh2_loader_character_template_assets_v35_probe PRIVATE $<TARGET_PROPERTY:dh2_loader_module_graph_source_probe,LINK_OPTIONS>)

target_sources(dh2_loader_preview_entity_export_v30 PRIVATE ../../character_template_assets_v35.cpp)

# Selected successor has one actual C1 state/counter storage and prepared-source TU.
# V4 remains an immutable standalone proof checkpoint; it cannot bind these words.
if(EXISTS "${graph_snapshot}/port/level-loader/canonical_level_prepared_source_v25.cpp")
 target_sources(dh2_loader_auxiliary_families PRIVATE "${graph_snapshot}/port/level-world/canonical_trigger_trap_v37.cpp")
 add_library(dh2_loader_lifecycle_v36 STATIC "${loader}/lifecycle_v36.cpp")
 target_compile_features(dh2_loader_lifecycle_v36 PUBLIC cxx_std_17)
 target_include_directories(dh2_loader_lifecycle_v36 PUBLIC "${loader}")
 target_compile_options(dh2_loader_lifecycle_v36 PRIVATE -Wall -Wextra -Werror -Wno-misleading-indentation)
 get_target_property(source_probe_links dh2_loader_module_graph_source_probe LINK_LIBRARIES)
 add_library(dh2_loader_generic_renderer_input_v37 STATIC "${loader}/generic_renderer_input_v36.cpp" "${loader}/generic_actor_visual_input_v37.cpp")
 target_compile_features(dh2_loader_generic_renderer_input_v37 PUBLIC cxx_std_17)
 target_include_directories(dh2_loader_generic_renderer_input_v37 BEFORE PUBLIC "${graph_snapshot}/port/level-loader" "${graph_snapshot}/port/level-world" "${graph_snapshot}/port/scene-materials" "${loader}")
 target_compile_options(dh2_loader_generic_renderer_input_v37 PRIVATE -Wall -Wextra -Werror -Wno-misleading-indentation)
 target_link_libraries(dh2_loader_generic_renderer_input_v37 PUBLIC ${source_probe_links})
 add_executable(dh2_loader_integrated_loading_v37_probe "${loader}/tests/canonical_loading_context_source_probe_v37.cpp")
 target_compile_features(dh2_loader_integrated_loading_v37_probe PRIVATE cxx_std_17)
 target_compile_options(dh2_loader_integrated_loading_v37_probe PRIVATE -Wall -Wextra -Werror -Wno-misleading-indentation -UNDEBUG)
 target_link_libraries(dh2_loader_integrated_loading_v37_probe PRIVATE ${source_probe_links} dh2_loader_lifecycle_v36 dh2_loader_generic_renderer_input_v37)
 target_link_options(dh2_loader_integrated_loading_v37_probe PRIVATE -Wl,--gc-sections -Wl,--no-undefined)
endif()

if(EXISTS "${graph_snapshot}/port/level-world/canonical_trigger_trap_v37.cpp")
 add_executable(dh2_loader_selected_trigger_trap_v37_probe "${loader}/tests/trigger-trap-v37/canonical_trigger_trap_selected_v37_test.cpp")
 target_compile_features(dh2_loader_selected_trigger_trap_v37_probe PRIVATE cxx_std_17)
 target_compile_options(dh2_loader_selected_trigger_trap_v37_probe PRIVATE -Wall -Wextra -Werror -Wno-misleading-indentation -UNDEBUG)
 target_link_libraries(dh2_loader_selected_trigger_trap_v37_probe PRIVATE ${source_probe_links})
 target_link_options(dh2_loader_selected_trigger_trap_v37_probe PRIVATE -Wl,--gc-sections -Wl,--no-undefined)
endif()

if(EXISTS "${graph_snapshot}/port/level-world/character_model_name_v38.cpp")
 target_sources(dh2_loader_character_rng_owners PRIVATE "${graph_snapshot}/port/level-world/character_model_name_v38.cpp")
 target_sources(dh2_loader_generic_renderer_input_v37 PRIVATE
  "${loader}/generic_actor_draw_submission_v38.cpp"
  "${loader}/scene_material_slot_v39.cpp"
  "${loader}/render_eligibility_v40.cpp")
 add_executable(dh2_loader_source_visual_helper_v41_probe "${loader}/tests/source_visual_helper_v41_probe.cpp")
 target_compile_features(dh2_loader_source_visual_helper_v41_probe PRIVATE cxx_std_17)
 target_compile_options(dh2_loader_source_visual_helper_v41_probe PRIVATE -Wall -Wextra -Werror -Wno-misleading-indentation -UNDEBUG)
 target_link_libraries(dh2_loader_source_visual_helper_v41_probe PRIVATE ${source_probe_links} dh2_loader_generic_renderer_input_v37)
 target_link_options(dh2_loader_source_visual_helper_v41_probe PRIVATE -Wl,--gc-sections -Wl,--no-undefined)
endif()

if(EXISTS "${graph_snapshot}/port/level-world/character_model_name_v38.cpp")
# Parent adds after verifying/rebuilding coherent V5 selected owners.
add_executable(dh2_loader_character_swamp_model_v40_probe "${loader}/tests/character_swamp_model_v40_probe.cpp")
target_compile_features(dh2_loader_character_swamp_model_v40_probe PRIVATE cxx_std_17)
target_compile_options(dh2_loader_character_swamp_model_v40_probe PRIVATE -Wall -Wextra -Werror -Wno-misleading-indentation -UNDEBUG)
target_link_libraries(dh2_loader_character_swamp_model_v40_probe PRIVATE ${source_probe_links} dh2_loader_lifecycle_v36 dh2_loader_generic_renderer_input_v37)
target_link_options(dh2_loader_character_swamp_model_v40_probe PRIVATE -Wl,--gc-sections -Wl,--no-undefined)
# character_model_name_v38.cpp must be in coherent Character owner target;
# existing source_probe_links supplies character_template_assets_v35.cpp.

 target_sources(dh2_loader_character_swamp_model_v40_probe PRIVATE "${loader}/character_template_assets_v35.cpp")
endif()

if(EXISTS "${graph_snapshot}/port/level-loader/canonical_level_prepared_source_v25.cpp")
 add_library(dh2_loader_source_loading_v43 STATIC "${loader}/level_source_loading_v43.cpp")
 target_compile_features(dh2_loader_source_loading_v43 PUBLIC cxx_std_17)
 target_include_directories(dh2_loader_source_loading_v43 BEFORE PUBLIC "${graph_snapshot}/port/level-loader" "${graph_snapshot}/port/level-world" "${loader}")
 target_compile_options(dh2_loader_source_loading_v43 PRIVATE -Wall -Wextra -Werror -Wno-misleading-indentation)
 target_link_libraries(dh2_loader_source_loading_v43 PUBLIC ${source_probe_links} dh2_loader_lifecycle_v36)
 if(EXISTS "${loader}/tests/source_stage_swamp_v40.cpp")
  add_executable(dh2_loader_source_stage_swamp_v40 "${loader}/tests/source_stage_swamp_v40.cpp")
  target_compile_features(dh2_loader_source_stage_swamp_v40 PRIVATE cxx_std_17)
  target_compile_options(dh2_loader_source_stage_swamp_v40 PRIVATE -Wall -Wextra -Werror -Wno-misleading-indentation -UNDEBUG)
  target_link_libraries(dh2_loader_source_stage_swamp_v40 PRIVATE dh2_loader_source_loading_v43)
  target_link_options(dh2_loader_source_stage_swamp_v40 PRIVATE -Wl,--gc-sections -Wl,--no-undefined)
 endif()
endif()

add_executable(dh2_loader_cancel_reentry_v43 "${loader}/tests/lifecycle_cancel_reentry_v43.cpp")
target_compile_features(dh2_loader_cancel_reentry_v43 PRIVATE cxx_std_17)
target_compile_options(dh2_loader_cancel_reentry_v43 PRIVATE -Wall -Wextra -Werror -UNDEBUG)
target_link_libraries(dh2_loader_cancel_reentry_v43 PRIVATE dh2_loader_lifecycle_v36)
