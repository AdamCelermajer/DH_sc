cmake_minimum_required(VERSION 3.24)
project(dh2_decomp_reconstruction CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
enable_testing()

# The original was built with GCC 4.x, which keeps stores to an object's storage before its constructor and in its
# destructor. Newer GCC drops them (-flifetime-dse). The reconstruction and its tests read such state (a destructor that
# leaves a pointer null, bytes compared with a reference object over zeroed storage), so the stores stay.
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-flifetime-dse=0>)
endif()

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
# zlib: libDungeonHunter2.so carries zlib 1.2.3 statically (deflate/inflate/crc32 symbols), but no zlib source
# is under tinyxml-and-c-libs (its zlib rows are name-matched "upstream" without a source file) and the
# WinLibs MinGW has no zlib headers or libz. The repo already carries pristine zlib sources as part of the
# vendored Irrlicht 1.8.5 (zlib 1.2.8, same API and inflate behaviour), so the library is built from those
# files in place, nothing copied. -DDH2_SYSTEM_ZLIB=ON links the toolchain's own libz instead.
option(DH2_SYSTEM_ZLIB "Link the system zlib (-lz) instead of building the vendored sources" OFF)
set(DH2_ZLIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../irrlicht-prototype/upstream/irrlicht-1.8.5/source/Irrlicht/zlib)
if(DH2_SYSTEM_ZLIB)
  add_library(dh2_vendor_zlib INTERFACE)
  target_link_libraries(dh2_vendor_zlib INTERFACE z)
else()
  set(zlib_srcs adler32 compress crc32 deflate gzclose gzlib gzread gzwrite infback inffast inflate inftrees trees uncompr zutil)
  list(TRANSFORM zlib_srcs PREPEND ${DH2_ZLIB_DIR}/)
  list(TRANSFORM zlib_srcs APPEND .c)
  add_library(dh2_vendor_zlib STATIC ${zlib_srcs})
  set_target_properties(dh2_vendor_zlib PROPERTIES C_STANDARD 99)
  target_include_directories(dh2_vendor_zlib SYSTEM PUBLIC ${DH2_ZLIB_DIR})
  if(NOT MSVC)
    target_compile_options(dh2_vendor_zlib PRIVATE -w)
  endif()
endif()
# gameswf r1714: the 68 upstream units of the DH2 build, with the flags its README gives (C++17, -w,
# -fno-fast-math -ffp-contract=off, -fpermissive on GCC). TU_CONFIG_* are global above. Definitions the DH2 build
# replaced sit in "#if 0 /* DH2: ... */" blocks inside these files; Gameloft's own gameswf units
# (vendor-gameswf/src) build into dh2_vendor_gameswf_dh2 in the engine group below.
file(GLOB_RECURSE gameswf_srcs CONFIGURE_DEPENDS ${DH2_GAMESWF_DIR}/*.cpp)
add_library(dh2_vendor_gameswf STATIC ${gameswf_srcs})
dh2_includes(dh2_vendor_gameswf ${CMAKE_CURRENT_SOURCE_DIR}/vendor-gameswf/include)
target_link_libraries(dh2_vendor_gameswf PUBLIC dh2_vendor_zlib)
if(NOT MSVC)
  target_compile_options(dh2_vendor_gameswf PRIVATE -w -fno-fast-math -ffp-contract=off)
  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(dh2_vendor_gameswf PRIVATE -fpermissive)
  endif()
endif()

# Engine, library and vendor-addition categories: one static library each.
set(DH2_LIBS)
foreach(cat IN LISTS DH2_DECOMP_CATEGORIES DH2_VENDOR_CATEGORIES)
  set(dir ${CMAKE_CURRENT_SOURCE_DIR}/${cat})
  file(GLOB_RECURSE srcs CONFIGURE_DEPENDS ${dir}/src/*.cpp ${dir}/src/*.c)
  if(NOT srcs)
    continue()
  endif()
  string(REPLACE "-" "_" lib dh2_${cat})
  if(cat IN_LIST DH2_VENDOR_CATEGORIES)
    set(lib ${lib}_dh2)   # Gameloft's additions; dh2_vendor_<lib> is the upstream library itself
  endif()
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
list(APPEND DH2_LIBS dh2_vendor_lua dh2_vendor_box2d dh2_vendor_gameswf)
if(NOT DH2_SYSTEM_ZLIB)
  list(APPEND DH2_LIBS dh2_vendor_zlib)
endif()

# Everything, linked as one rescanned group (libraries reference each other both ways).
add_library(dh2_all INTERFACE)
target_link_libraries(dh2_all INTERFACE "$<LINK_GROUP:RESCAN,${DH2_LIBS}>")
if(WIN32)
  target_link_libraries(dh2_all INTERFACE ws2_32)
endif()

# Link stubs for the tests (link-1). Symbols that some library object references and no library defines yet (the rows of
# reports/decomp-reconstruction/link-missing.csv that tests reach) are defined here as aborting stubs, one object per
# symbol, in a static library that only the TESTS link, after dh2_all. A real definition in any library therefore wins
# without further changes (the linker takes the first archive that defines a symbol), and nothing here reaches a program
# that links dh2_all alone, which still shows the gaps as undefined symbols. link-stubs/link_stubs.csv lists them:
# symbol,kind(function|data),size,category,address,note. Each stub names itself on stderr and aborts when called.
set(DH2_STUB_DIR ${CMAKE_CURRENT_BINARY_DIR}/link-stubs)
file(MAKE_DIRECTORY ${DH2_STUB_DIR})
set(DH2_STUB_SOURCES)
set(DH2_STUB_CSV ${CMAKE_CURRENT_SOURCE_DIR}/link-stubs/link_stubs.csv)
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${DH2_STUB_CSV})
if(EXISTS ${DH2_STUB_CSV})
  file(STRINGS ${DH2_STUB_CSV} stub_rows)
  list(POP_FRONT stub_rows)   # header
  set(stub_n 0)
  foreach(row IN LISTS stub_rows)
    string(REGEX MATCH "^([^,]*),([^,]*),([^,]*)" ignore "${row}")
    set(stub_sym ${CMAKE_MATCH_1})
    set(stub_kind ${CMAKE_MATCH_2})
    set(stub_size ${CMAKE_MATCH_3})
    if(stub_sym STREQUAL "")
      continue()
    endif()
    math(EXPR stub_n "${stub_n} + 1")
    set(stub_src ${DH2_STUB_DIR}/stub_${stub_n}.cpp)
    if(stub_kind STREQUAL "data")
      set(stub_text "// generated from link-stubs/link_stubs.csv
extern \"C\" { alignas(16) char dh2_stub_data_${stub_n}[${stub_size}] __asm__(\"${stub_sym}\"); }
")
    else()
      set(stub_text "// generated from link-stubs/link_stubs.csv
#include <cstdio>
#include <cstdlib>
extern \"C\" void dh2_stub_fn_${stub_n}() __asm__(\"${stub_sym}\");
extern \"C\" void dh2_stub_fn_${stub_n}() { std::fputs(\"DH2 link stub called (not reconstructed yet): ${stub_sym}\\n\", stderr); std::abort(); }
")
    endif()
    file(GENERATE OUTPUT ${stub_src} CONTENT "${stub_text}")
    list(APPEND DH2_STUB_SOURCES ${stub_src})
  endforeach()
endif()
if(DH2_STUB_SOURCES)
  add_library(dh2_test_link_stubs STATIC ${DH2_STUB_SOURCES})
  set_source_files_properties(${DH2_STUB_SOURCES} PROPERTIES GENERATED TRUE)
  # What the tests link: the same rescanned group as dh2_all, with the stubs as its last archive.
  add_library(dh2_all_tests INTERFACE)
  target_link_libraries(dh2_all_tests INTERFACE "$<LINK_GROUP:RESCAN,${DH2_LIBS},dh2_test_link_stubs>")
  if(WIN32)
    target_link_libraries(dh2_all_tests INTERFACE ws2_32)
  endif()
else()
  add_library(dh2_all_tests INTERFACE)
  target_link_libraries(dh2_all_tests INTERFACE dh2_all)
endif()

# Tests: one executable per file in <category>/tests, linked against everything.
# Many tests define stand-ins of their own for code outside the batch they check (recording stubs, aborting stubs, fake
# globals). Those definitions come first on the link line and must win over the library objects that are pulled in
# for other symbols, as they did when each test was linked with its batch sources only, so the tests link with
# --allow-multiple-definition (the first definition is kept). Duplicate definitions between the libraries themselves are
# checked separately by the link/library-duplicates test below.
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
    target_link_libraries(${exe} PRIVATE dh2_all_tests)
    if(NOT MSVC)
      target_link_options(${exe} PRIVATE -Wl,--allow-multiple-definition)
    endif()
    add_test(NAME ${cat}/${tname} COMMAND ${exe})
  endforeach()
endforeach()

# The libraries must not define a symbol twice between them. One link of every library, whole-archive and with
# unresolved symbols ignored, reports every multiple definition. (The tests above allow multiple definitions on purpose,
# so this is the check for the libraries.) The link as a whole cannot succeed: whole-archive pulls in every row that is not
# reconstructed yet and has no stub (reports/decomp-reconstruction/link-missing.csv), so ld ends with undefined
# references. The test passes when ld reports no multiple definition and no stub in the stub library is stale.
if(NOT MSVC)
  file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/dh2_duplicate_check_main.cpp "int main() { return 0; }
")
  # The stub library is part of the check: a stub for a symbol that a library now defines is a stale stub and fails here.
  set(dup_libs)
  foreach(lib IN LISTS DH2_LIBS)
    list(APPEND dup_libs $<TARGET_FILE:${lib}>)
  endforeach()
  list(APPEND dup_libs $<TARGET_FILE:dh2_test_link_stubs>)
  file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/dh2_duplicate_check.cmake "
string(REPLACE \"|\" \";\" libs \"\${LIBS}\")
execute_process(COMMAND \"\${CXX}\" -static -static-libgcc -static-libstdc++ \"\${MAIN}\" -o \"\${OUT}\"
                        -Wl,--unresolved-symbols=ignore-all -Wl,--whole-archive \${libs} -Wl,--no-whole-archive \${EXTRA}
                ERROR_VARIABLE err OUTPUT_VARIABLE out RESULT_VARIABLE rc)
string(REGEX MATCHALL \"[^\n]*multiple definition[^\n]*\" dups \"\${err}\")
if(dups)
  string(REPLACE \";\" \"\n\" dups \"\${dups}\")
  message(FATAL_ERROR \"multiple definitions between the libraries:\n\${dups}\")
endif()
if(NOT err MATCHES \"undefined reference\" AND NOT rc EQUAL 0)
  message(FATAL_ERROR \"the library link failed for another reason:\n\${err}\")
endif()
message(STATUS \"no multiple definitions between the libraries\")
")
  add_test(NAME link/library-duplicates
           COMMAND ${CMAKE_COMMAND} -DCXX=${CMAKE_CXX_COMPILER}
                   -DMAIN=${CMAKE_CURRENT_BINARY_DIR}/dh2_duplicate_check_main.cpp
                   -DOUT=${CMAKE_CURRENT_BINARY_DIR}/dh2_duplicate_check.exe
                   "-DLIBS=$<JOIN:${dup_libs},|>" "-DEXTRA=$<$<BOOL:${WIN32}>:-lws2_32>"
                   -P ${CMAKE_CURRENT_BINARY_DIR}/dh2_duplicate_check.cmake)
endif()
