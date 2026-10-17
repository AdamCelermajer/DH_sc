cmake_minimum_required(VERSION 3.22)
project(dh2_desktop_native LANGUAGES C CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

get_filename_component(DH2_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/../.." ABSOLUTE)
set(DH2_SOURCE_DIR "${DH2_SOURCE_DIR}" CACHE PATH "DH_sc source checkout")
if(NOT EXISTS "${DH2_SOURCE_DIR}/port/android-native/app/src/main/cpp/model_renderer.cpp")
  message(FATAL_ERROR "DH2_SOURCE_DIR must point at the DH_sc source checkout")
endif()

set(DH2_ANGLE_ROOT "$ENV{ANGLE_ROOT}" CACHE PATH "Root of a Windows ANGLE package")
set(DH2_SDL2_ROOT "$ENV{SDL2_ROOT}" CACHE PATH "Root of SDL2 development files")

# Put every shared DH2 runtime DLL beside the executable. The selected source
# libraries are shared targets, and Windows does not search their build folders.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")

find_package(SDL2 CONFIG QUIET)
if(TARGET SDL2::SDL2)
  set(DH2_SDL_TARGET SDL2::SDL2)
elseif(TARGET SDL2::SDL2-static)
  set(DH2_SDL_TARGET SDL2::SDL2-static)
else()
  find_path(DH2_SDL2_INCLUDE_DIR SDL.h
    HINTS "${DH2_SDL2_ROOT}/include" "${DH2_SDL2_ROOT}/include/SDL2"
    PATH_SUFFIXES SDL2)
  find_library(DH2_SDL2_LIBRARY NAMES SDL2 SDL2-static
    HINTS "${DH2_SDL2_ROOT}/lib" "${DH2_SDL2_ROOT}/lib/x64")
  if(NOT DH2_SDL2_INCLUDE_DIR OR NOT DH2_SDL2_LIBRARY)
    message(FATAL_ERROR "SDL2 development files not found. Set DH2_SDL2_ROOT or install SDL2 with a CMake package.")
  endif()
  add_library(dh2_sdl2 UNKNOWN IMPORTED)
  set_target_properties(dh2_sdl2 PROPERTIES
    IMPORTED_LOCATION "${DH2_SDL2_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${DH2_SDL2_INCLUDE_DIR}")
  set(DH2_SDL_TARGET dh2_sdl2)
endif()
find_file(DH2_SDL2_DLL NAMES SDL2.dll
  HINTS "${DH2_SDL2_ROOT}/bin" "${DH2_SDL2_ROOT}/lib" "${DH2_SDL2_ROOT}/lib/x64")

find_path(DH2_GLES2_INCLUDE_DIR GLES2/gl2.h
  HINTS "${DH2_ANGLE_ROOT}/include" "${DH2_ANGLE_ROOT}/include/GLES2")
find_library(DH2_GLESV2_LIBRARY NAMES libGLESv2 GLESv2
  HINTS "${DH2_ANGLE_ROOT}/lib" "${DH2_ANGLE_ROOT}/lib/x64" "${DH2_ANGLE_ROOT}/lib64")
find_library(DH2_EGL_LIBRARY NAMES libEGL EGL
  HINTS "${DH2_ANGLE_ROOT}/lib" "${DH2_ANGLE_ROOT}/lib/x64" "${DH2_ANGLE_ROOT}/lib64")
find_file(DH2_GLESV2_DLL NAMES libGLESv2.dll GLESv2.dll
  HINTS "${DH2_ANGLE_ROOT}/bin" "${DH2_ANGLE_ROOT}/lib" "${DH2_ANGLE_ROOT}/lib/x64")
find_file(DH2_EGL_DLL NAMES libEGL.dll EGL.dll
  HINTS "${DH2_ANGLE_ROOT}/bin" "${DH2_ANGLE_ROOT}/lib" "${DH2_ANGLE_ROOT}/lib/x64")
find_file(DH2_D3DCOMPILER_DLL NAMES d3dcompiler_47.dll
  HINTS "${DH2_ANGLE_ROOT}/bin" "${DH2_ANGLE_ROOT}/lib" "${DH2_ANGLE_ROOT}/lib/x64")
if(NOT DH2_GLES2_INCLUDE_DIR OR NOT DH2_GLESV2_LIBRARY OR NOT DH2_EGL_LIBRARY OR NOT DH2_GLESV2_DLL OR NOT DH2_EGL_DLL)
  message(FATAL_ERROR "ANGLE GLES2/EGL headers, import library, and DLLs not found. Set DH2_ANGLE_ROOT to a Windows ANGLE package.")
endif()

add_subdirectory("${DH2_SOURCE_DIR}/port/engine-textures" "${CMAKE_CURRENT_BINARY_DIR}/engine-textures")
add_subdirectory("${DH2_SOURCE_DIR}/port/scene-materials" "${CMAKE_CURRENT_BINARY_DIR}/scene-materials")
add_subdirectory("${DH2_SOURCE_DIR}/port/engine-particles" "${CMAKE_CURRENT_BINARY_DIR}/engine-particles")
add_subdirectory("${DH2_SOURCE_DIR}/port/engine-animation" "${CMAKE_CURRENT_BINARY_DIR}/engine-animation")
add_subdirectory("${DH2_SOURCE_DIR}/port/engine-skinning" "${CMAKE_CURRENT_BINARY_DIR}/engine-skinning")
add_subdirectory("${DH2_SOURCE_DIR}/port/engine-camera" "${CMAKE_CURRENT_BINARY_DIR}/engine-camera")
add_subdirectory("${DH2_SOURCE_DIR}/port/level-world" "${CMAKE_CURRENT_BINARY_DIR}/level-world")
include("${DH2_SOURCE_DIR}/port/engine-ui/inventory_text_v1.cmake")
include("${DH2_SOURCE_DIR}/port/engine-ui/menu_frontend_v69.cmake")

set(DH2_ANDROID_CPP "${DH2_SOURCE_DIR}/port/android-native/app/src/main/cpp")
include("${DH2_ANDROID_CPP}/native_runtime_sources.cmake")
add_executable(dh2_desktop_runtime
  desktop_main.cpp asset_manager.cpp gameplay_audio_sdl.cpp
  "${DH2_ANDROID_CPP}/platform_wifi_alive_windows.cpp"
  ${DH2_NATIVE_RUNTIME_SOURCES})
if(MINGW AND TARGET dh2_gameswf_core)
  # GameSWF's legacy Windows assert macro uses MSVC-only inline asm. The
  # desktop runtime builds its parser and consumers in release mode instead.
  foreach(target IN ITEMS dh2_gameswf_core dh2_engine_ui dh2_desktop_runtime)
    if(TARGET ${target})
      target_compile_definitions(${target} PRIVATE NDEBUG __FUNCDNAME__=__FUNCTION__)
    endif()
  endforeach()
endif()
if(MINGW AND TARGET dh2_box2d_201)
  # Box2D 2.0.1 uses the POSIX finite() spelling on non-MSVC Windows builds.
  target_compile_definitions(dh2_box2d_201 PUBLIC finite=_finite)
endif()
target_compile_features(dh2_desktop_runtime PRIVATE cxx_std_17)
target_compile_definitions(dh2_desktop_runtime PRIVATE SDL_MAIN_HANDLED)
file(TO_CMAKE_PATH "${DH2_SOURCE_DIR}/port/android-native/app/src/main/assets" DH2_DEFAULT_ASSET_DIR)
target_compile_definitions(dh2_desktop_runtime PRIVATE
  DH2_DEFAULT_ASSET_DIR="${DH2_DEFAULT_ASSET_DIR}")
target_include_directories(dh2_desktop_runtime PRIVATE
  "${CMAKE_CURRENT_LIST_DIR}/include"
  "${DH2_ANDROID_CPP}"
  "${DH2_SOURCE_DIR}/port/engine-ui/vendor/gameswf1714"
  "${DH2_GLES2_INCLUDE_DIR}")
target_link_libraries(dh2_desktop_runtime PRIVATE
  ${DH2_SDL_TARGET} "${DH2_GLESV2_LIBRARY}" "${DH2_EGL_LIBRARY}"
  dh2_engine_textures dh2_scene_materials dh2_engine_particles dh2_engine_animation dh2_engine_skinning
  dh2_engine_camera dh2_level_world dh2_script_runtime
  dh2_inventory_text_v1 dh2_engine_ui)
if(WIN32)
  target_link_libraries(dh2_desktop_runtime PRIVATE user32 gdi32 shell32 iphlpapi ws2_32)
  add_custom_command(TARGET dh2_desktop_runtime POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${DH2_GLESV2_DLL}" "$<TARGET_FILE_DIR:dh2_desktop_runtime>"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${DH2_EGL_DLL}" "$<TARGET_FILE_DIR:dh2_desktop_runtime>")
  if(DH2_D3DCOMPILER_DLL)
    add_custom_command(TARGET dh2_desktop_runtime POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different "${DH2_D3DCOMPILER_DLL}" "$<TARGET_FILE_DIR:dh2_desktop_runtime>")
  endif()
  if(DH2_SDL2_DLL)
    add_custom_command(TARGET dh2_desktop_runtime POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different "${DH2_SDL2_DLL}" "$<TARGET_FILE_DIR:dh2_desktop_runtime>")
  endif()
endif()

message(STATUS "DH2 desktop runtime uses the Android app's selected renderer and level/gameplay libraries")
message(STATUS "DH2 asset root at run time: ${DH2_SOURCE_DIR}/port/android-native/app/src/main/assets")
