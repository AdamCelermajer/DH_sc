cmake_minimum_required(VERSION 3.16)
project(dh2_decomp_reconstruction CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
enable_testing()

# Engine and library categories: each is an independent static library plus its tests.
# Types that more than one category needs live in shared/include/glitch-shared/ and every
# category includes that directory; the rest are declared locally under
# <category>/include/<category>/deps/ until integration.
set(DH2_DECOMP_CATEGORIES
  core-platform pydata-accessors net-transport net-session-online vox-audio
  tinyxml-and-c-libs glitch-core-io glitch-video glitch-scene-collada-gui)

# Track B gameplay categories. They build into one library, dh2_game, because game
# classes call each other across categories. Every game class has exactly one header,
# game/include/dh2/<Class>.hpp (see TRACK-B.md); sources stay in their category's src/.
set(DH2_GAME_CATEGORIES
  game-anim game-app game-character game-items-save game-ui game-world)

function(dh2_warnings target)
  if(MSVC)
    target_compile_options(${target} PRIVATE /W3)
  else()
    target_compile_options(${target} PRIVATE -Wall -Wno-unused-variable -Wno-unused-but-set-variable)
  endif()
endfunction()

foreach(cat IN LISTS DH2_DECOMP_CATEGORIES)
  set(dir ${CMAKE_CURRENT_SOURCE_DIR}/${cat})
  file(GLOB_RECURSE srcs CONFIGURE_DEPENDS ${dir}/src/*.cpp ${dir}/src/*.c)
  if(NOT srcs)
    continue()
  endif()
  string(REPLACE "-" "_" lib dh2_${cat})
  add_library(${lib} STATIC ${srcs})
  target_include_directories(${lib} PUBLIC ${dir}/include ${CMAKE_CURRENT_SOURCE_DIR}/shared/include)
  dh2_warnings(${lib})
  if(WIN32)
    target_link_libraries(${lib} PUBLIC ws2_32)
  endif()
  file(GLOB tests CONFIGURE_DEPENDS ${dir}/tests/*.cpp)
  foreach(t IN LISTS tests)
    get_filename_component(tname ${t} NAME_WE)
    set(exe ${lib}_${tname})
    add_executable(${exe} ${t})
    target_link_libraries(${exe} PRIVATE ${lib})
    add_test(NAME ${cat}/${tname} COMMAND ${exe})
  endforeach()
endforeach()

# Engine categories call into the lower ones (core-io holds the core value types, the
# quaternion math and os::Printer). Link them in that order so tests resolve.
if(TARGET dh2_glitch_video AND TARGET dh2_glitch_core_io)
  target_link_libraries(dh2_glitch_video PUBLIC dh2_glitch_core_io)
endif()
if(TARGET dh2_glitch_scene_collada_gui)
  foreach(dep dh2_glitch_video dh2_glitch_core_io)
    if(TARGET ${dep})
      target_link_libraries(dh2_glitch_scene_collada_gui PUBLIC ${dep})
    endif()
  endforeach()
endif()

# The include directories a game translation unit sees: the game class headers first,
# then every engine and library category, addressed as <category>/<file>.hpp.
set(DH2_GAME_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/game/include ${CMAKE_CURRENT_SOURCE_DIR}/shared/include)
foreach(cat IN LISTS DH2_DECOMP_CATEGORIES)
  list(APPEND DH2_GAME_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/${cat}/include)
endforeach()

set(game_srcs)
foreach(cat IN LISTS DH2_GAME_CATEGORIES)
  file(GLOB_RECURSE s CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${cat}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/${cat}/src/*.c)
  list(APPEND game_srcs ${s})
endforeach()
file(GLOB_RECURSE game_shared_srcs CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/game/src/*.cpp)
list(APPEND game_srcs ${game_shared_srcs})
if(game_srcs)
  add_library(dh2_game STATIC ${game_srcs})
  target_include_directories(dh2_game PUBLIC ${DH2_GAME_INCLUDES})
  dh2_warnings(dh2_game)
  foreach(cat IN LISTS DH2_GAME_CATEGORIES)
    file(GLOB tests CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${cat}/tests/*.cpp)
    string(REPLACE "-" "_" pfx dh2_${cat})
    foreach(t IN LISTS tests)
      get_filename_component(tname ${t} NAME_WE)
      set(exe ${pfx}_${tname})
      add_executable(${exe} ${t})
      target_link_libraries(${exe} PRIVATE dh2_game)
      add_test(NAME ${cat}/${tname} COMMAND ${exe})
    endforeach()
  endforeach()
endif()
