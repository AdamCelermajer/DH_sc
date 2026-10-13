cmake_minimum_required(VERSION 3.24)
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

# Track B builds one program from many folders. Every folder sees every other folder's
# headers (addressed as "<category>/<file>.hpp", "dh2/<Class>.hpp", "glitch-shared/...",
# "lua.h", "Box2D.h", "gameswf/..."), so a class is defined once and used everywhere.
# Tests link every library in one rescanned group, so cross-category calls resolve
# whatever the layering. See TRACK-B.md.
set(DH2_DECOMP_CATEGORIES
  core-platform pydata-accessors net-transport net-session-online vox-audio
  tinyxml-and-c-libs glitch-core-io glitch-video glitch-scene-collada-gui)
set(DH2_GAME_CATEGORIES
  game-anim game-app game-character game-items-save game-ui game-world)
set(DH2_VENDOR_CATEGORIES vendor-lua vendor-box2d vendor-gameswf)

set(DH2_LUA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor-lua/upstream/lua-5.1.4/src)
set(DH2_BOX2D_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor-box2d/upstream/box2d-2.0.1)
set(DH2_GAMESWF_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor-gameswf/upstream/gameswf-r1714)

set(DH2_ALL_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/game/include ${CMAKE_CURRENT_SOURCE_DIR}/shared/include)
foreach(cat IN LISTS DH2_DECOMP_CATEGORIES DH2_VENDOR_CATEGORIES)
  list(APPEND DH2_ALL_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/${cat}/include)
endforeach()
set(DH2_VENDOR_INCLUDES ${DH2_LUA_DIR} ${DH2_BOX2D_DIR}/Include ${DH2_BOX2D_DIR}/Source ${DH2_GAMESWF_DIR})

# gameswf configuration as the DH2 build uses it; NOMINMAX keeps windows.h macros out of tu-testbed.
add_compile_definitions(NOMINMAX TU_CONFIG_LINK_TO_JPEGLIB=0 TU_CONFIG_LINK_TO_LIBPNG=0
  TU_CONFIG_LINK_TO_FREETYPE=0 TU_CONFIG_LINK_TO_THREAD=0)

function(dh2_warnings target)
  if(MSVC)
    target_compile_options(${target} PRIVATE /W3)
  else()
    target_compile_options(${target} PRIVATE -Wall -Wno-unused-variable -Wno-unused-but-set-variable)
  endif()
endfunction()

function(dh2_includes target own)
  target_include_directories(${target} PUBLIC ${own} ${DH2_ALL_INCLUDES})
  target_include_directories(${target} SYSTEM PUBLIC ${DH2_VENDOR_INCLUDES})
endfunction()

# Vendored upstream libraries (vendor-*/upstream, see their README.md and NOTES.md).
# Lua 5.1.4 with the DH2 float lua_Number; linit.c is not built (the sfc wrapper opens libraries).
file(GLOB lua_srcs CONFIGURE_DEPENDS ${DH2_LUA_DIR}/*.c)
list(FILTER lua_srcs EXCLUDE REGEX "/(lua|luac|print|linit|liolib|loslib|loadlib)\\.c$")
add_library(dh2_vendor_lua STATIC ${lua_srcs})
set_target_properties(dh2_vendor_lua PROPERTIES C_STANDARD 99)
target_include_directories(dh2_vendor_lua SYSTEM PUBLIC ${DH2_LUA_DIR})
# Box2D 2.0.1 with the DH2 b2Settings; b2Collision.cpp is not in the DH2 build.
file(GLOB_RECURSE box2d_srcs CONFIGURE_DEPENDS ${DH2_BOX2D_DIR}/Source/*.cpp)
list(FILTER box2d_srcs EXCLUDE REGEX "/b2Collision\\.cpp$")
add_library(dh2_vendor_box2d STATIC ${box2d_srcs})
target_include_directories(dh2_vendor_box2d SYSTEM PUBLIC ${DH2_BOX2D_DIR}/Include ${DH2_BOX2D_DIR}/Source)
if(NOT MSVC)
  target_compile_options(dh2_vendor_box2d PRIVATE -w -fno-fast-math -ffp-contract=off -include cstring)
endif()
# gameswf r1714 is headers-only for now (the library needs the glitch render backend to link).
# Gameloft's gameswf additions (vendor-gameswf/src) build into the engine group below.

# Engine, library and vendor-addition categories: one static library each.
set(DH2_LIBS)
foreach(cat IN LISTS DH2_DECOMP_CATEGORIES DH2_VENDOR_CATEGORIES)
  set(dir ${CMAKE_CURRENT_SOURCE_DIR}/${cat})
  file(GLOB_RECURSE srcs CONFIGURE_DEPENDS ${dir}/src/*.cpp ${dir}/src/*.c)
  if(NOT srcs)
    continue()
  endif()
  string(REPLACE "-" "_" lib dh2_${cat})
  add_library(${lib} STATIC ${srcs})
  dh2_includes(${lib} ${dir}/include)
  dh2_warnings(${lib})
  list(APPEND DH2_LIBS ${lib})
endforeach()

# Gameplay categories build into one library, dh2_game. Every game class has exactly one
# header, game/include/dh2/<Class>.hpp; sources stay in their category's src/.
set(game_srcs)
foreach(cat IN LISTS DH2_GAME_CATEGORIES)
  file(GLOB_RECURSE s CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${cat}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/${cat}/src/*.c)
  list(APPEND game_srcs ${s})
endforeach()
file(GLOB_RECURSE game_shared_srcs CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/game/src/*.cpp)
list(APPEND game_srcs ${game_shared_srcs})
if(game_srcs)
  add_library(dh2_game STATIC ${game_srcs})
  dh2_includes(dh2_game ${CMAKE_CURRENT_SOURCE_DIR}/game/include)
  dh2_warnings(dh2_game)
  list(APPEND DH2_LIBS dh2_game)
endif()
list(APPEND DH2_LIBS dh2_vendor_lua dh2_vendor_box2d)

# Everything, linked as one rescanned group (libraries reference each other both ways).
add_library(dh2_all INTERFACE)
target_link_libraries(dh2_all INTERFACE "$<LINK_GROUP:RESCAN,${DH2_LIBS}>")
if(WIN32)
  target_link_libraries(dh2_all INTERFACE ws2_32)
endif()

# Tests: one executable per file in <category>/tests, linked against everything.
foreach(cat IN LISTS DH2_DECOMP_CATEGORIES DH2_GAME_CATEGORIES DH2_VENDOR_CATEGORIES)
  set(dir ${CMAKE_CURRENT_SOURCE_DIR}/${cat})
  file(GLOB tests CONFIGURE_DEPENDS ${dir}/tests/*.cpp)
  string(REPLACE "-" "_" pfx dh2_${cat})
  foreach(t IN LISTS tests)
    get_filename_component(tname ${t} NAME_WE)
    set(exe ${pfx}_${tname})
    add_executable(${exe} ${t})
    target_include_directories(${exe} PRIVATE ${dir}/include ${DH2_ALL_INCLUDES})
    target_include_directories(${exe} SYSTEM PRIVATE ${DH2_VENDOR_INCLUDES})
    target_link_libraries(${exe} PRIVATE dh2_all)
    add_test(NAME ${cat}/${tname} COMMAND ${exe})
  endforeach()
endforeach()
