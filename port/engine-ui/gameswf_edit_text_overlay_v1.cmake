# Select the source text graph as a unit; never mix stock/overlay class layouts.
foreach(DH2_TEXT_REPLACED_SOURCE
    "${DH2_GAMESWF_ROOT}/gameswf/gameswf_text.cpp"
    "${DH2_GAMESWF_ROOT}/gameswf/gameswf_dlist.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/overlays/loader-lifetime-v1/gameswf_impl.cpp")
  list(FIND DH2_GAMESWF_SOURCES "${DH2_TEXT_REPLACED_SOURCE}" DH2_TEXT_SOURCE_INDEX)
  if(DH2_TEXT_SOURCE_INDEX EQUAL -1)
    message(FATAL_ERROR "Source edit-text overlay requires ${DH2_TEXT_REPLACED_SOURCE}")
  endif()
  list(REMOVE_ITEM DH2_GAMESWF_SOURCES "${DH2_TEXT_REPLACED_SOURCE}")
endforeach()
foreach(DH2_TEXT_SOURCE gameswf_text.cpp gameswf_dlist.cpp gameswf_impl.cpp)
  list(APPEND DH2_GAMESWF_SOURCES "${CMAKE_CURRENT_LIST_DIR}/overlays/edit-text-v1/${DH2_TEXT_SOURCE}")
endforeach()
