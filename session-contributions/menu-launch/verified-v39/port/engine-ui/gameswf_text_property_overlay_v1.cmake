# Temporary property dispatch bridge. Frozen vendor files stay unchanged.
set(DH2_TEXT_STOCK "${DH2_GAMESWF_ROOT}/gameswf/gameswf_text.cpp")
list(FIND DH2_GAMESWF_SOURCES "${DH2_TEXT_STOCK}" DH2_TEXT_INDEX)
if(DH2_TEXT_INDEX LESS 0)
  message(FATAL_ERROR "Text property bridge requires stock text TU")
endif()
list(REMOVE_ITEM DH2_GAMESWF_SOURCES "${DH2_TEXT_STOCK}")
list(APPEND DH2_GAMESWF_SOURCES "${CMAKE_CURRENT_LIST_DIR}/overlays/text-property-v1/gameswf_text.cpp")
