# V67 actual source dependency closure

Refreshed after Root imported the auxiliary receiver/factory batch. Source inspection only; no builds, tests, syntax checks, hashes, freezes or fixtures. Existing Root files win over all private/snapshot duplicates and remain untouched. In particular the current SoundEmitter PropertyMap design lookup, base/context/map/engine owners and imported receiver body/header versions are preserved.

## Missing-only adoption files

None remain in this bounded V67 catalog source/header closure after Root imports. Do not copy the duplicate private loader TriggerTrap body/header: Root already has the actual world receiver.

## Required actual link TUs

These existing Root-relative CPPs must occur once on SAME dh2_level_world, either directly or through existing production-targets.cmake/native_noncharacter_sources_v67.cmake attachment. This list is a dependency/adoption instruction, not proof of a configured or linked build.
- `port/level-loader/canonical_auxiliary_families_v16.cpp`
- `port/level-loader/canonical_light_point_factory_v53.cpp`
- `port/level-loader/canonical_light_point_v53.cpp`
- `port/level-loader/catalog_auxiliary_v67.cpp`
- `port/level-loader/catalog_containers_v67.cpp`
- `port/level-loader/catalog_environment_v67.cpp`
- `port/level-loader/production_noncharacter_catalog_v67.cpp`
- `port/level-loader/production_noncharacter_composition_v67.cpp`
- `port/level-world/canonical_animated_decor_v23.cpp`
- `port/level-world/canonical_checkpoint_zone_v26.cpp`
- `port/level-world/canonical_decor_v15.cpp`
- `port/level-world/canonical_destructible_container_v16.cpp`
- `port/level-world/canonical_door_v27.cpp`
- `port/level-world/canonical_dummy_owner_v14.cpp`
- `port/level-world/canonical_exit_zone_v29.cpp`
- `port/level-world/canonical_openable_graph_v21.cpp`
- `port/level-world/canonical_quest_move_zone_v31.cpp`
- `port/level-world/canonical_sound_emitter_v32.cpp`
- `port/level-world/canonical_spawn_point_v15.cpp`
- `port/level-world/canonical_trigger_object_v28.cpp`
- `port/level-world/canonical_trigger_trap_v37.cpp`
- `port/level-world/canonical_trigger_zone_v22.cpp`
- `port/level-world/game_object_initialization_owner_v1.cpp`
- `port/level-world/game_object_set_position_v2.cpp`
- `port/level-world/light_set_name_owner_v3.cpp`

`required-additional-link-tus.txt` contains only genuinely absent CPPs; it is empty when all required sources have been imported. `required-existing-link-tus.txt` lists the retained required owner TUs above. Do not compile headers/inc separately or add another manager, base, actor runtime, context, parser, stream or class owner.

## Existing-preserve paths

- `port/level-loader/canonical_auxiliary_families_v16.cpp` (required catalog source/header)
- `port/level-loader/canonical_auxiliary_families_v16.hpp` (required catalog source/header)
- `port/level-loader/canonical_level_context_v1.hpp` (included by port/level-loader/production_noncharacter_catalog_v67.hpp)
- `port/level-loader/canonical_light_point_factory_v53.cpp` (required catalog source/header)
- `port/level-loader/canonical_light_point_factory_v53.hpp` (required catalog source/header)
- `port/level-loader/canonical_light_point_v53.cpp` (required catalog source/header)
- `port/level-loader/canonical_light_point_v53.hpp` (required catalog source/header)
- `port/level-loader/catalog_auxiliary_v67.cpp` (required catalog source/header)
- `port/level-loader/catalog_auxiliary_v67.hpp` (required catalog source/header)
- `port/level-loader/catalog_containers_v67.cpp` (required catalog source/header)
- `port/level-loader/catalog_containers_v67.hpp` (required catalog source/header)
- `port/level-loader/catalog_environment_v67.cpp` (required catalog source/header)
- `port/level-loader/catalog_environment_v67.hpp` (required catalog source/header)
- `port/level-loader/production_noncharacter_catalog_v67.cpp` (required catalog source/header)
- `port/level-loader/production_noncharacter_catalog_v67.hpp` (required catalog source/header)
- `port/level-loader/production_noncharacter_composition_v67.cpp` (required catalog source/header)
- `port/level-loader/production_noncharacter_composition_v67.hpp` (required catalog source/header)
- `port/level-world/canonical_animated_decor_v23.cpp` (required catalog source/header)
- `port/level-world/canonical_animated_decor_v23.hpp` (required catalog source/header)
- `port/level-world/canonical_checkpoint_zone_v26.cpp` (required catalog source/header)
- `port/level-world/canonical_checkpoint_zone_v26.hpp` (required catalog source/header)
- `port/level-world/canonical_class_receiver_bindings_v1.hpp` (included by port/level-loader/canonical_light_point_factory_v53.hpp)
- `port/level-world/canonical_decor_v15.cpp` (required catalog source/header)
- `port/level-world/canonical_decor_v15.hpp` (required catalog source/header)
- `port/level-world/canonical_destructible_container_v16.cpp` (required catalog source/header)
- `port/level-world/canonical_destructible_container_v16.hpp` (required catalog source/header)
- `port/level-world/canonical_destructible_declarations_v16.inc` (included by port/level-world/canonical_property_map_v1.cpp)
- `port/level-world/canonical_door_v27.cpp` (required catalog source/header)
- `port/level-world/canonical_door_v27.hpp` (required catalog source/header)
- `port/level-world/canonical_dummy_owner_v14.cpp` (required catalog source/header)
- `port/level-world/canonical_dummy_owner_v14.hpp` (required catalog source/header)
- `port/level-world/canonical_exit_zone_v29.cpp` (required catalog source/header)
- `port/level-world/canonical_exit_zone_v29.hpp` (required catalog source/header)
- `port/level-world/canonical_family_fields_v15.hpp` (included by port/level-world/canonical_spawn_point_v15.hpp)
- `port/level-world/canonical_gameobject_base_owner_v1.hpp` (included by port/level-world/game_object_initialization_owner_v1.hpp)
- `port/level-world/canonical_item_candidate_transport_v57.hpp` (included by port/level-loader/catalog_containers_v67.hpp)
- `port/level-world/canonical_level_config_module_declarations_v1.inc` (included by port/level-world/canonical_property_map_v1.cpp)
- `port/level-world/canonical_object_factory_v1.hpp` (included by port/level-loader/production_noncharacter_catalog_v67.hpp)
- `port/level-world/canonical_openable_container_v1.hpp` (included by port/level-world/canonical_destructible_container_v16.hpp)
- `port/level-world/canonical_openable_graph_v21.cpp` (required catalog source/header)
- `port/level-world/canonical_openable_graph_v21.hpp` (required catalog source/header)
- `port/level-world/canonical_property_declarations_v1.inc` (included by port/level-world/canonical_property_map_v1.cpp)
- `port/level-world/canonical_property_map_v1.cpp` (required catalog source/header)
- `port/level-world/canonical_property_map_v1.hpp` (included by port/level-loader/canonical_light_point_v53.hpp)
- `port/level-world/canonical_quest_move_zone_v31.cpp` (required catalog source/header)
- `port/level-world/canonical_quest_move_zone_v31.hpp` (required catalog source/header)
- `port/level-world/canonical_sound_emitter_v32.cpp` (required catalog source/header)
- `port/level-world/canonical_sound_emitter_v32.hpp` (required catalog source/header)
- `port/level-world/canonical_spawn_point_v15.cpp` (required catalog source/header)
- `port/level-world/canonical_spawn_point_v15.hpp` (required catalog source/header)
- `port/level-world/canonical_swamp_family_declarations_v15.inc` (included by port/level-world/canonical_property_map_v1.cpp)
- `port/level-world/canonical_trigger_object_v28.cpp` (required catalog source/header)
- `port/level-world/canonical_trigger_object_v28.hpp` (required catalog source/header)
- `port/level-world/canonical_trigger_trap_declarations_v37.inc` (included by port/level-world/canonical_property_map_v1.cpp)
- `port/level-world/canonical_trigger_trap_v37.cpp` (required catalog source/header)
- `port/level-world/canonical_trigger_trap_v37.hpp` (required catalog source/header)
- `port/level-world/canonical_trigger_zone_declarations_v22.inc` (included by port/level-world/canonical_property_map_v1.cpp)
- `port/level-world/canonical_trigger_zone_v22.cpp` (required catalog source/header)
- `port/level-world/canonical_trigger_zone_v22.hpp` (required catalog source/header)
- `port/level-world/destructible_container_data_v16.hpp` (included by port/level-world/canonical_destructible_container_v16.hpp)
- `port/level-world/game_object_initialization_owner_v1.cpp` (required catalog source/header)
- `port/level-world/game_object_initialization_owner_v1.hpp` (required catalog source/header)
- `port/level-world/game_object_set_position_v2.cpp` (required catalog source/header)
- `port/level-world/game_object_set_position_v2.hpp` (required catalog source/header)
- `port/level-world/light_set_name_owner_v3.cpp` (required catalog source/header)
- `port/level-world/light_set_name_owner_v3.hpp` (required catalog source/header)
- `port/level-world/retained_gameobject_decor_v1.hpp` (included by port/level-world/canonical_openable_graph_v21.hpp)
- `port/level-world/retained_generic_animator_v21.hpp` (included by port/level-world/canonical_openable_graph_v21.hpp)
- `port/level-world/retained_scene_visual_connection_v3.hpp` (included by port/level-world/canonical_openable_graph_v21.hpp)

## Unresolved

No missing quoted/project seed paths in this bounded closure. Existing transitive engine/base dependencies form preservation boundaries; this is not a whole engine audit.

The recursive source adoption follows missing quoted/project headers and inc files, including project angle headers. Actual imports are inspected as seeds; existing unrelated engine/base paths are preserved rather than copied from a snapshot. Main still supplies real class Init/InitPost/InitFinal/resource/destruction and release-journal leaves. Source closure does not establish activation, visibility or native link/runtime success.
