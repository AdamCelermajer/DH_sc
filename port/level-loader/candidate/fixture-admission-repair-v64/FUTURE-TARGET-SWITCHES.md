# Future target source switches V64

SOURCE PROPOSAL ONLY. Apply through Main after source review; compilation/execution remains PENDING. These target mappings do not authorize a native job.

- dh2_loader_integrated_loading_v37_probe at tests/cmake-receiver-transport/CMakeLists.txt:404 -> successors/canonical_loading_context_source_probe_v37_admission_v64.cpp
- dh2_loader_integrated_loading_v47_probe at tests/cmake-receiver-transport/CMakeLists.txt:510 -> successors/canonical_loading_context_source_probe_v47_admission_v64.cpp
- dh2_loader_module_graph_source_probe at tests/cmake-receiver-transport/CMakeLists.txt:174 -> successors/canonical_module_graph_source_probe_admission_v64.cpp
- dh2_loader_character_swamp_model_v40_probe at tests/cmake-receiver-transport/CMakeLists.txt:434 -> successors/character_swamp_model_v40_probe_admission_v64.cpp
- dh2_loader_dynamic_module_v44 at tests/cmake-receiver-transport/CMakeLists.txt:465 -> successors/dynamic_module_source_v44_admission_v64.cpp
- generic_actor_draw_submission_v38_probe.cpp: no current CMake target; future source users must choose successors/generic_actor_draw_submission_v38_probe_admission_v64.cpp
- dh2_loader_actor_image_v45_probe at tests/cmake-receiver-transport/CMakeLists.txt:499 -> successors/generic_actor_image_binding_v45_probe_admission_v64.cpp
- dh2_loader_actor_image_lifetime_v48_probe at tests/cmake-receiver-transport/CMakeLists.txt:536 -> successors/generic_actor_image_lifetime_v48_probe_admission_v64.cpp
- generic_actor_visual_input_v37_probe.cpp: no current CMake target; future source users must choose successors/generic_actor_visual_input_v37_probe_admission_v64.cpp
- generic_renderer_input_v36_probe.cpp: no current CMake target; future source users must choose successors/generic_renderer_input_v36_probe_admission_v64.cpp
- gslevel_update_v44_real_c1.cpp: no current CMake target; future source users must choose successors/gslevel_update_v44_real_c1_admission_v64.cpp
- level_update_loading_v46_real_c1.cpp: no current CMake target; future source users must choose successors/level_update_loading_v46_real_c1_admission_v64.cpp
- dh2_loader_source_stage_swamp_v40 at tests/cmake-receiver-transport/CMakeLists.txt:452 -> successors/source_stage_swamp_v40_admission_v64.cpp
- dh2_loader_source_stage_swamp_v47 at tests/cmake-receiver-transport/CMakeLists.txt:505 -> successors/source_stage_swamp_v47_admission_v64.cpp
- dh2_loader_source_visual_helper_v41_probe at tests/cmake-receiver-transport/CMakeLists.txt:425 -> successors/source_visual_helper_v41_probe_admission_v64.cpp
- dh2_loader_same_level_ctor_probe at tests/cmake-receiver-transport/CMakeLists.txt:289 -> successors/canonical_level_constructor_v3_admission_v64.cpp
- dh2_loader_native_ctor_bindings_probe at tests/cmake-receiver-transport/CMakeLists.txt:326 -> successors/level_constructor_bindings_v4_admission_v64.cpp
- dh2_loader_preview_entity_export_v30 at tests/cmake-receiver-transport/CMakeLists.txt:369 -> successors/canonical_preview_entity_export_v30_admission_v64.cpp

Separate parent-covered source: dh2_loader_source_swamp_raw_v58 historical source_stage_swamp_v58.cpp -> parent tests/source_stage_swamp_v63.cpp. Do not also add a V64 replacement for that fixture.
