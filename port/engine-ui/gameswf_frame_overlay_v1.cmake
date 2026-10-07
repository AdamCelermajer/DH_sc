# Source root/sprite scheduling. Combine with input-v1; vendor bytes unchanged.
foreach(DH2_FRAME_TU IN ITEMS gameswf_sprite gameswf_root gameswf_button)
 set(DH2_FRAME_STOCK "${DH2_GAMESWF_ROOT}/gameswf/${DH2_FRAME_TU}.cpp")
 set(DH2_FRAME_MATCHES 0)
 foreach(DH2_FRAME_SOURCE IN LISTS DH2_GAMESWF_SOURCES)
  if(DH2_FRAME_SOURCE STREQUAL DH2_FRAME_STOCK)
   math(EXPR DH2_FRAME_MATCHES "${DH2_FRAME_MATCHES}+1")
  endif()
 endforeach()
 if(NOT DH2_FRAME_MATCHES EQUAL 1)
  message(FATAL_ERROR "Frame-v1 requires exactly one stock ${DH2_FRAME_TU} TU")
 endif()
 list(REMOVE_ITEM DH2_GAMESWF_SOURCES "${DH2_FRAME_STOCK}")
endforeach()
list(APPEND DH2_GAMESWF_SOURCES
 "${CMAKE_CURRENT_LIST_DIR}/overlays/frame-v1/gameswf_sprite.cpp"
 "${CMAKE_CURRENT_LIST_DIR}/overlays/frame-v1/gameswf_root.cpp"
 "${CMAKE_CURRENT_LIST_DIR}/overlays/frame-v1/gameswf_button.cpp")
