cmake_minimum_required(VERSION 3.22)
project(dh2_loader_scene_v3 LANGUAGES CXX)
get_filename_component(loader "${CMAKE_CURRENT_LIST_DIR}/../.." ABSOLUTE)
get_filename_component(port "${loader}/.." ABSOLUTE)
set(snapshot "${loader}/vendor/scene-connected-ef63b7f2573b1c94/port/level-world")
add_library(dh2_loader_scene_v3_owners STATIC
 "${snapshot}/canonical_gameobject_base_owner_v1.cpp"
 "${snapshot}/game_object_visual_asset_owner_v1.cpp"
 "${snapshot}/base_named_animation_controller_v1.cpp"
 "${snapshot}/gameobject_scene_binding_v1.cpp"
 "${snapshot}/retained_gameobject_visual_v1.cpp"
 "${snapshot}/retained_gameobject_visual_asset_connection_v2.cpp"
 "${snapshot}/gameobject_scene_root_registry_v1.cpp"
 "${snapshot}/retained_scene_visual_connection_v3.cpp"
 "${port}/level-world/visual_motion.cpp"
 "${port}/level-world/body_transform.cpp"
 "${port}/level-world/navigation_motion.cpp"
 "${port}/level-world/navigation_world.cpp"
 "${port}/level-world/navigation_search.cpp"
 "${port}/level-world/selector.cpp"
 "${port}/level-world/collision.cpp"
 "${port}/level-world/octree.cpp"
 "${port}/level-world/physical_controls.cpp"
 "${port}/level-world/navigation_objects.cpp"
 "${port}/level-world/decor_body_config.cpp"
 "${port}/level-world/decor_scene.cpp"
 "${port}/level-world/visual_timeline.cpp"
 "${port}/engine-math/math.cpp"
 "${port}/engine-resources/resources.cpp"
 "${port}/asset-payloads/payloads.cpp"
 "${port}/scene-materials/scene.cpp"
 "${port}/engine-animation/animation.cpp"
 "${port}/engine-animation/events.cpp"
 "${port}/engine-animation/event_track.cpp"
 "${port}/engine-animation/animation_blend.cpp"
 "${port}/engine-animation/animation_registration.cpp"
 "${port}/engine-animation/component_applicator.cpp"
 "${port}/engine-animation/angle_interpreter.cpp"
 "${port}/engine-skinning/skinning.cpp")
# The frozen Visual TU uses std::runtime_error without including <stdexcept>.
# Supply that standard header for this exact TU; keep archived bytes untouched.
set_source_files_properties("${snapshot}/retained_gameobject_visual_v1.cpp"
 PROPERTIES COMPILE_OPTIONS "-include;stdexcept")
target_compile_features(dh2_loader_scene_v3_owners PUBLIC cxx_std_17)
target_include_directories(dh2_loader_scene_v3_owners PUBLIC "${snapshot}" "${loader}/vendor/scene-lifecycle-header-85cd574bb8f03bcd" "${port}/level-world")
target_compile_options(dh2_loader_scene_v3_owners PRIVATE -Wall -Wextra -Wno-misleading-indentation -fno-fast-math -ffp-contract=off)
add_executable(dh2_loader_scene_v3_probe "${snapshot}/tests/retained_scene_visual_connection_v3.cpp")
target_compile_options(dh2_loader_scene_v3_probe PRIVATE -Wall -Wextra -Werror -UNDEBUG)
target_link_libraries(dh2_loader_scene_v3_probe PRIVATE dh2_loader_scene_v3_owners)
if(ANDROID)
 target_link_options(dh2_loader_scene_v3_probe PRIVATE -Wl,-z,max-page-size=16384 -Wl,--no-undefined)
endif()
