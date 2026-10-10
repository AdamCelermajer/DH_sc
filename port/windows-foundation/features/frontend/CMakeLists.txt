cmake_minimum_required(VERSION 3.22)
project(dh_original_frontend_diagnostic LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(NOT WIN32)
  message(FATAL_ERROR "Native frontend diagnostic requires Windows/WGL")
endif()
set(FOUNDATION_BUILD "${CMAKE_CURRENT_SOURCE_DIR}/../../../../.local-inputs/windows-foundation-build" CACHE PATH "Existing verified foundation libraries")
set(FOUNDATION_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/../..")
add_library(frontend_foundation INTERFACE)
target_include_directories(frontend_foundation INTERFACE
  "${FOUNDATION_SOURCE}"
  "${FOUNDATION_SOURCE}/../level-world"
  "${FOUNDATION_SOURCE}/../level-loader"
  "${FOUNDATION_SOURCE}/../game-data"
  "${FOUNDATION_SOURCE}/../engine-animation"
  "${FOUNDATION_SOURCE}/../engine-skinning"
  "${FOUNDATION_SOURCE}/../scene-materials"
  "${FOUNDATION_SOURCE}/../physics-backend/box2d-2.0.1/Include")
target_link_libraries(frontend_foundation INTERFACE
  "${FOUNDATION_BUILD}/libfoundation_data.a"
  "${FOUNDATION_BUILD}/librecovered_content.a"
  "${FOUNDATION_BUILD}/libcontent_xml.a"
  "${FOUNDATION_BUILD}/libdh2_freetype237.a"
  "${FOUNDATION_BUILD}/librecovered_trigger_contacts.a"
  "${FOUNDATION_BUILD}/physics-backend/libdh2_box2d_201.a")
file(GLOB FRONTEND_COMPONENTS CONFIGURE_DEPENDS art/*.cpp flow/*.cpp creation/*.cpp preview/*.cpp input/*.cpp)
list(FILTER FRONTEND_COMPONENTS EXCLUDE REGEX "(_tests|/tests|_probe)\\.cpp$")
add_library(frontend_components STATIC ${FRONTEND_COMPONENTS}
  frontend_runtime_contract_v1.cpp
  "${FOUNDATION_SOURCE}/features/platform_input/source_owner_contract.cpp")
target_sources(frontend_components PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/../../../game-data/class_preview_setup.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/../../../game-data/loot_tables_v2.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/../../../game-data/skill_tables.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/../../../game-data/fresh_player_profile_v1.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/../../../engine-ui/edit_text_definition_v1.cpp")
target_link_libraries(frontend_components PUBLIC frontend_foundation)
enable_testing()
foreach(component art flow creation preview input)
  file(GLOB COMPONENT_TESTS "${component}/*_tests.cpp")
  if(component STREQUAL "creation")
    # These source-owner integration tests have dedicated Windows runners
    # that compile their real transport implementation set, not the diagnostic
    # foundation archive set used by the broad component suite.
    list(FILTER COMPONENT_TESTS EXCLUDE REGEX "(selected_profile_binding|source_indexed_slot_service)_v1_tests\\.cpp$")
  endif()
  foreach(source ${COMPONENT_TESTS})
    get_filename_component(test ${source} NAME_WE)
    add_executable(${test} ${source})
    target_link_libraries(${test} PRIVATE frontend_components)
    if(test STREQUAL "runtime_creation_persistence_v1_tests" OR
       test STREQUAL "runtime_creation_flow_adapter_v1_tests")
      # This proof reads the already-extracted original-cache tree, not the
      # packaged Android data/ directory.
      add_test(NAME ${test} COMMAND ${test} "${FOUNDATION_BUILD}/../windows-shared-assets")
    else()
      add_test(NAME ${test} COMMAND ${test} "${CMAKE_CURRENT_SOURCE_DIR}/../../../android-native/app/src/main/assets")
    endif()
    if(MINGW)
      target_link_options(${test} PRIVATE -static)
    endif()
  endforeach()
endforeach()
add_executable(frontend_art_tests art/tests.cpp)
target_link_libraries(frontend_art_tests PRIVATE frontend_components)
add_test(NAME frontend_art_tests COMMAND frontend_art_tests)
add_executable(frontend_creation_boundary_checks verification/creation_boundary_checks.cpp)
target_link_libraries(frontend_creation_boundary_checks PRIVATE frontend_components)
add_test(NAME frontend_creation_boundary_checks COMMAND frontend_creation_boundary_checks)
add_executable(frontend_runtime_v1_tests frontend_runtime_v1_tests.cpp)
target_link_libraries(frontend_runtime_v1_tests PRIVATE frontend_components)
add_test(NAME frontend_runtime_v1_tests COMMAND frontend_runtime_v1_tests)
add_executable(frontend_cleanup_v1_tests frontend_cleanup_v1_tests.cpp)
target_link_libraries(frontend_cleanup_v1_tests PRIVATE frontend_components)
add_test(NAME frontend_cleanup_v1_tests COMMAND frontend_cleanup_v1_tests)
if(MINGW)
  target_link_options(frontend_art_tests PRIVATE -static)
  target_link_options(frontend_creation_boundary_checks PRIVATE -static)
  target_link_options(frontend_runtime_v1_tests PRIVATE -static)
  target_link_options(frontend_cleanup_v1_tests PRIVATE -static)
endif()
add_library(frontend_runtime_v1 STATIC
  native_host.cpp frontend_runtime_v1.cpp frontend_presentation_v1.cpp rich_text.cpp
  "${FOUNDATION_SOURCE}/overlay_renderer.cpp")
target_link_libraries(frontend_runtime_v1 PUBLIC
  frontend_components frontend_foundation opengl32 user32 gdi32)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/interactive.cpp")
  add_executable(dh-frontend-diagnostic interactive.cpp
    "${FOUNDATION_SOURCE}/renderer.cpp"
    "${FOUNDATION_SOURCE}/platform_win32.cpp")
  target_link_libraries(dh-frontend-diagnostic PRIVATE frontend_runtime_v1 opengl32 user32 gdi32)
  add_test(NAME frontend_native_event_smoke COMMAND dh-frontend-diagnostic
    --assets "${CMAKE_CURRENT_SOURCE_DIR}/../../../android-native/app/src/main/assets"
    --verify-native-input --frames 500 --fixed-step 0.016)
  set_tests_properties(frontend_native_event_smoke PROPERTIES TIMEOUT 60)
  if(MINGW)
    target_link_options(dh-frontend-diagnostic PRIVATE -static)
  endif()
endif()
