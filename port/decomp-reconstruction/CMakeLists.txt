cmake_minimum_required(VERSION 3.16)
project(dh2_decomp_reconstruction CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
enable_testing()

# Each category is an independent static library plus its tests. Categories do
# not link to each other yet. Types that more than one category needs live in
# shared/include/glitch-shared/ and every category includes that directory; the rest
# are declared locally under <category>/include/<category>/deps/ until integration.
set(DH2_DECOMP_CATEGORIES
  core-platform pydata-accessors net-transport net-session-online vox-audio
  tinyxml-and-c-libs glitch-core-io glitch-video glitch-scene-collada-gui)

foreach(cat IN LISTS DH2_DECOMP_CATEGORIES)
  set(dir ${CMAKE_CURRENT_SOURCE_DIR}/${cat})
  file(GLOB_RECURSE srcs CONFIGURE_DEPENDS ${dir}/src/*.cpp ${dir}/src/*.c)
  if(NOT srcs)
    continue()
  endif()
  string(REPLACE "-" "_" lib dh2_${cat})
  add_library(${lib} STATIC ${srcs})
  target_include_directories(${lib} PUBLIC ${dir}/include ${CMAKE_CURRENT_SOURCE_DIR}/shared/include)
  if(MSVC)
    target_compile_options(${lib} PRIVATE /W3)
  else()
    target_compile_options(${lib} PRIVATE -Wall -Wno-unused-variable -Wno-unused-but-set-variable)
  endif()
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
