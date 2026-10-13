cmake_minimum_required(VERSION 3.16)
project(dh2_decomp_reconstruction CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
enable_testing()

# MinGW: link the C++ runtime statically. Otherwise test executables pick up whichever
# libstdc++-6.dll is first on PATH (Git's msvcrt-based copy on this machine), which gave
# 0xc0000139 and heap errors that were not in the code.
if(MINGW)
  add_link_options(-static-libgcc -static-libstdc++ -static)
endif()

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

# Vendored upstream libraries (vendor-*/upstream, see their README.md and NOTES.md).
# Lua 5.1.4 with the DH2 float lua_Number; linit.c is not built (the sfc wrapper opens libraries).
set(DH2_LUA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor-lua/upstream/lua-5.1.4/src)
file(GLOB lua_srcs CONFIGURE_DEPENDS ${DH2_LUA_DIR}/*.c)
list(FILTER lua_srcs EXCLUDE REGEX "/(lua|luac|print|linit|liolib|loslib|loadlib)\\.c$")
add_library(dh2_vendor_lua STATIC ${lua_srcs})
set_target_properties(dh2_vendor_lua PROPERTIES C_STANDARD 99)
target_include_directories(dh2_vendor_lua SYSTEM PUBLIC ${DH2_LUA_DIR})
# Box2D 2.0.1 with the DH2 b2Settings; b2Collision.cpp is not in the DH2 build.
set(DH2_BOX2D_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor-box2d/upstream/box2d-2.0.1)
file(GLOB_RECURSE box2d_srcs CONFIGURE_DEPENDS ${DH2_BOX2D_DIR}/Source/*.cpp)
list(FILTER box2d_srcs EXCLUDE REGEX "/b2Collision\\.cpp$")
add_library(dh2_vendor_box2d STATIC ${box2d_srcs})
target_include_directories(dh2_vendor_box2d SYSTEM PUBLIC ${DH2_BOX2D_DIR}/Include ${DH2_BOX2D_DIR}/Source)
if(NOT MSVC)
  target_compile_options(dh2_vendor_box2d PRIVATE -w -fno-fast-math -ffp-contract=off -include cstring)
endif()
# gameswf r1714: headers only for now (the library needs the glitch render backend to link).
set(DH2_GAMESWF_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor-gameswf/upstream/gameswf-r1714)

# The include directories a game translation unit sees: the game class headers first,
# then every engine and library category, addressed as <category>/<file>.hpp, then the
# vendored libraries (lua.h, Box2D.h, gameswf/gameswf_value.h).
set(DH2_GAME_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/game/include ${CMAKE_CURRENT_SOURCE_DIR}/shared/include)
foreach(cat IN LISTS DH2_DECOMP_CATEGORIES)
  list(APPEND DH2_GAME_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/${cat}/include)
endforeach()
list(APPEND DH2_GAME_INCLUDES ${DH2_LUA_DIR} ${DH2_BOX2D_DIR}/Include ${DH2_BOX2D_DIR}/Source ${DH2_GAMESWF_DIR})

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
  target_link_libraries(dh2_game PUBLIC dh2_vendor_lua dh2_vendor_box2d)
  # gameswf configuration as the DH2 build uses it; NOMINMAX keeps windows.h macros out of tu-testbed.
  target_compile_definitions(dh2_game PUBLIC NOMINMAX TU_CONFIG_LINK_TO_JPEGLIB=0 TU_CONFIG_LINK_TO_LIBPNG=0
    TU_CONFIG_LINK_TO_FREETYPE=0 TU_CONFIG_LINK_TO_THREAD=0)
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
