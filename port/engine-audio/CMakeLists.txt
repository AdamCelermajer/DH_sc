cmake_minimum_required(VERSION 3.22)
project(dh2_engine_audio LANGUAGES CXX)
add_library(dh2_engine_audio SHARED
  audio_sample_v34.cpp audio_catalog_v34.cpp audio_bank_v34.cpp
  audio_spatial_v34.cpp audio_native_envelope_v34.cpp audio_mixer_v34.cpp
  audio_source_bindings_v38.cpp audio_listener_rows_v38.cpp vox_source_fields_v38.cpp
  audio_target_events_v38.cpp)
target_compile_features(dh2_engine_audio PUBLIC cxx_std_17)
target_include_directories(dh2_engine_audio PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_options(dh2_engine_audio PRIVATE -Wall -Wextra -Werror -fno-fast-math -ffp-contract=off)
if(ANDROID)
  target_link_options(dh2_engine_audio PRIVATE -Wl,-z,max-page-size=16384 -Wl,--no-undefined)
endif()
