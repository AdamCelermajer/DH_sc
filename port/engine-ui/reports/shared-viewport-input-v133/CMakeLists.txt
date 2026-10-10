cmake_minimum_required(VERSION 3.22)
project(shared_viewport_input LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
add_compile_options(-O1 -g -fsanitize=address,undefined -fno-omit-frame-pointer -fno-fast-math -ffp-contract=off)
add_link_options(-fsanitize=address,undefined)
include("/mnt/c/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/gameswf_sources.cmake")
include("/mnt/c/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/gameswf_input_overlay_v1.cmake")
include("/mnt/c/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/gameswf_frame_overlay_v1.cmake")
add_library(input_core SHARED ${DH2_GAMESWF_SOURCES}
 "/mnt/c/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/swf_cursor_input.cpp"
 "/mnt/c/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/swf_input_geometry.cpp"
 "/mnt/c/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/swf_input_policy.cpp"
 "/mnt/c/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/swf_input_history.cpp"
 "/mnt/c/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/swf_input_connection.cpp"
 "/mnt/c/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/swf_controller_storage_v91.cpp"
 "/mnt/c/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/swf_event_dispatch.cpp"
 "/mnt/c/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/swf_event_core.cpp"
 "/mnt/c/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/swf_viewport_connection.cpp"
 "/mnt/c/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/viewport.cpp"
 "/mnt/c/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/swf_frame_schedule.cpp"
 "/mnt/c/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/swf_frame_connection.cpp"
 "/mnt/c/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/swf_drag_values.cpp"
)
target_include_directories(input_core PUBLIC "/mnt/c/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui" "${DH2_GAMESWF_ROOT}")
target_compile_definitions(input_core PUBLIC TU_CONFIG_LINK_TO_JPEGLIB=0 TU_CONFIG_LINK_TO_LIBPNG=0 TU_CONFIG_LINK_TO_FREETYPE=0 TU_CONFIG_LINK_TO_THREAD=0)
target_compile_options(input_core PRIVATE -w -fpermissive -fno-sanitize=vptr)
target_link_libraries(input_core PUBLIC z ${CMAKE_DL_LIBS})
add_executable(audit "/mnt/c/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/tests/swf_cursor_input.cpp")
target_link_libraries(audit PRIVATE input_core)
target_compile_options(audit PRIVATE -fno-sanitize=vptr)
add_executable(snapshot_repro "/mnt/c/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/tests/swf_cursor_input.cpp")
target_link_libraries(snapshot_repro PRIVATE input_core)
target_compile_options(snapshot_repro PRIVATE -fno-sanitize=vptr)
target_compile_definitions(snapshot_repro PRIVATE DH2_INPUT_VIEWPORT_SNAPSHOT_REPRO=1)
