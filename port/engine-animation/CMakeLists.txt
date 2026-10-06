cmake_minimum_required(VERSION 3.22)
project(dh2_engine_animation LANGUAGES CXX)
if(NOT TARGET dh2_scene_materials)
  add_subdirectory(../scene-materials "${CMAKE_CURRENT_BINARY_DIR}/scene-materials")
endif()
add_library(dh2_engine_animation SHARED animation.cpp events.cpp event_track.cpp animation_blend.cpp animation_registration.cpp component_applicator.cpp angle_interpreter.cpp)
target_sources(dh2_engine_animation PRIVATE material_color.cpp)
target_sources(dh2_engine_animation PRIVATE particle_box_v2.cpp particle_cloud_runtime_v2.cpp particle_resource_init_v2.cpp material_color_v3.cpp)
target_sources(dh2_engine_animation PRIVATE particle_cloud_runtime_v3.cpp
 particle_deflector_v1.cpp particle_force_scene_v2.cpp particle_bound_forces_v4.cpp)
target_sources(dh2_engine_animation PRIVATE particle_parameter.cpp)
target_sources(dh2_engine_animation PRIVATE particle_scalar_animation_v6.cpp)
target_sources(dh2_engine_animation PRIVATE particle_factory.cpp)
target_sources(dh2_engine_animation PRIVATE particle_random_v1.cpp particle_resource_init_v1.cpp
 particle_force_scene_v1.cpp particle_scene_color_v1.cpp particle_cloud_models_v1.cpp
 particle_cloud_runtime_v1.cpp particle_billboard_v1.cpp)
target_sources(dh2_engine_animation PRIVATE particle_emission.cpp)
target_compile_features(dh2_engine_animation PUBLIC cxx_std_17)
target_include_directories(dh2_engine_animation PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_options(dh2_engine_animation PRIVATE -Wall -Wextra -fno-fast-math -ffp-contract=off)
target_link_libraries(dh2_engine_animation PUBLIC dh2_scene_materials)
if(ANDROID)
  target_link_options(dh2_engine_animation PRIVATE -Wl,-z,max-page-size=16384 -Wl,--no-undefined)
else()
  add_executable(material_color_audit tests/material_color.cpp)
  target_link_libraries(material_color_audit PRIVATE dh2_engine_animation)
  add_executable(particle_parameter_audit tests/particle_parameter.cpp)
  target_link_libraries(particle_parameter_audit PRIVATE dh2_engine_animation)
  add_executable(particle_factory_audit tests/particle_factory.cpp)
  target_link_libraries(particle_factory_audit PRIVATE dh2_engine_animation)
  add_executable(particle_emission_audit tests/particle_emission.cpp)
  target_link_libraries(particle_emission_audit PRIVATE dh2_engine_animation)
  add_executable(animation_registration_audit tests/animation_registration.cpp)
  target_link_libraries(animation_registration_audit PRIVATE dh2_engine_animation)
  add_executable(component_applicator_audit tests/component_applicator_dso.cpp)
  target_link_libraries(component_applicator_audit PRIVATE dh2_engine_animation ${CMAKE_DL_LIBS})
  # Link the audited math code into this executable: --wrap cannot observe
  # libm calls already linked inside the shared scene-materials library.
  add_executable(compiled_transforms_audit tests/compiled_transforms.cpp
    animation.cpp angle_interpreter.cpp events.cpp event_track.cpp
    ../scene-materials/scene.cpp ../engine-resources/resources.cpp
    ../asset-payloads/payloads.cpp ../engine-math/math.cpp)
  target_compile_features(compiled_transforms_audit PRIVATE cxx_std_17)
  target_compile_options(compiled_transforms_audit PRIVATE -Wall -Wextra
    -fno-builtin -fno-fast-math -ffp-contract=off)
  target_link_options(compiled_transforms_audit PRIVATE
    -Wl,--wrap=sinf -Wl,--wrap=acosf -Wl,--wrap=sqrtf)
  add_executable(dynamic_compiled_transforms_audit tests/dynamic_compiled_transforms.cpp
    animation.cpp angle_interpreter.cpp events.cpp event_track.cpp
    ../scene-materials/scene.cpp ../engine-resources/resources.cpp
    ../asset-payloads/payloads.cpp ../engine-math/math.cpp)
  target_compile_features(dynamic_compiled_transforms_audit PRIVATE cxx_std_17)
  target_compile_options(dynamic_compiled_transforms_audit PRIVATE -Wall -Wextra
    -fno-builtin -fno-fast-math -ffp-contract=off)
  target_link_options(dynamic_compiled_transforms_audit PRIVATE
    -Wl,--wrap=sinf -Wl,--wrap=acosf -Wl,--wrap=sqrtf)
  add_executable(component_transforms_audit tests/component_transforms.cpp
    animation.cpp angle_interpreter.cpp events.cpp event_track.cpp
    ../scene-materials/scene.cpp ../engine-resources/resources.cpp
    ../asset-payloads/payloads.cpp ../engine-math/math.cpp)
  target_compile_features(component_transforms_audit PRIVATE cxx_std_17)
  target_compile_options(component_transforms_audit PRIVATE -Wall -Wextra
    -fno-builtin -fno-fast-math -ffp-contract=off)
  target_link_options(component_transforms_audit PRIVATE
    -Wl,--wrap=sinf -Wl,--wrap=cosf -Wl,--wrap=acosf -Wl,--wrap=sqrtf)
  add_executable(animation_audit tests/audit.cpp)
  target_link_libraries(animation_audit PRIVATE dh2_engine_animation)
  add_executable(events_audit tests/events_audit.cpp)
  target_link_libraries(events_audit PRIVATE dh2_engine_animation)
endif()
