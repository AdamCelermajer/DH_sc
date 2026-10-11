cmake_minimum_required(VERSION 3.22)
project(dh2_engine_camera LANGUAGES CXX)

add_library(dh2_engine_camera SHARED
    camera_projection.cpp
    frustum.cpp
    frustum_bounds.cpp
    frustum_runtime.cpp
    plane_intersection.cpp
    projection_matrix.cpp)
target_compile_features(dh2_engine_camera PUBLIC cxx_std_17)
target_include_directories(dh2_engine_camera PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_options(dh2_engine_camera PRIVATE
    -Wall -Wextra -Werror -fno-fast-math -ffp-contract=off)
if(ANDROID)
    target_link_options(dh2_engine_camera PRIVATE
        -Wl,-z,max-page-size=16384 -Wl,--no-undefined)
endif()
