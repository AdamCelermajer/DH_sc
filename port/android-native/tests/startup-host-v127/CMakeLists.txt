cmake_minimum_required(VERSION 3.22)
project(dh2_startup_data_host_v127 LANGUAGES CXX)
get_filename_component(DH2_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../../.." ABSOLUTE)
find_package(ZLIB REQUIRED)
set(APP "${DH2_ROOT}/port/android-native/app/src/main/cpp")
add_executable(startup_data_host_v127
  startup_data_host_v127.cpp
  "${APP}/source_process_arrays_v101.cpp"
  "${DH2_ROOT}/port/asset-payloads/zip_asset_pack_v1.cpp")
target_include_directories(startup_data_host_v127 PRIVATE "${APP}" "${DH2_ROOT}/port/asset-payloads")
target_compile_features(startup_data_host_v127 PRIVATE cxx_std_17)
target_compile_options(startup_data_host_v127 PRIVATE -O1 -g -Wall -Wextra)
target_link_libraries(startup_data_host_v127 PRIVATE ZLIB::ZLIB)
