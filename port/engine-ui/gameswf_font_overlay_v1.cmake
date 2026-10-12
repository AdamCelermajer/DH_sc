# Include after gameswf_sources.cmake and before constructing dh2_gameswf_core.
# This is an opt-in source replacement; never compile both font TUs.
set(DH2_GAMESWF_FONT_STOCK "${DH2_GAMESWF_ROOT}/gameswf/gameswf_font.cpp")
set(DH2_GAMESWF_FONT_MATCHES 0)
foreach(DH2_GAMESWF_SOURCE IN LISTS DH2_GAMESWF_SOURCES)
  if(DH2_GAMESWF_SOURCE STREQUAL DH2_GAMESWF_FONT_STOCK)
    math(EXPR DH2_GAMESWF_FONT_MATCHES "${DH2_GAMESWF_FONT_MATCHES}+1")
  endif()
endforeach()
if(NOT DH2_GAMESWF_FONT_MATCHES EQUAL 1)
  message(FATAL_ERROR "DH2 font-v1 overlay requires exactly one stock font TU")
endif()
list(REMOVE_ITEM DH2_GAMESWF_SOURCES "${DH2_GAMESWF_FONT_STOCK}")
list(APPEND DH2_GAMESWF_SOURCES "${CMAKE_CURRENT_LIST_DIR}/overlays/font-v1/gameswf_font.cpp")
