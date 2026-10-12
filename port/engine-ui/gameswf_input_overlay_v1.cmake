# Opt-in setter-history observer. Vendor source bytes stay frozen. Source
# as_object notify occurs after watcher and before map/read-only/store checks.
foreach(DH2_INPUT_TU IN ITEMS gameswf_character gameswf_object)
 set(DH2_INPUT_STOCK "${DH2_GAMESWF_ROOT}/gameswf/${DH2_INPUT_TU}.cpp")
 set(DH2_INPUT_MATCHES 0)
 foreach(DH2_INPUT_SOURCE IN LISTS DH2_GAMESWF_SOURCES)
  if(DH2_INPUT_SOURCE STREQUAL DH2_INPUT_STOCK)
   math(EXPR DH2_INPUT_MATCHES "${DH2_INPUT_MATCHES}+1")
  endif()
 endforeach()
 if(NOT DH2_INPUT_MATCHES EQUAL 1)
  message(FATAL_ERROR "Input-v1 requires exactly one stock ${DH2_INPUT_TU} TU")
 endif()
 list(REMOVE_ITEM DH2_GAMESWF_SOURCES "${DH2_INPUT_STOCK}")
endforeach()
list(APPEND DH2_GAMESWF_SOURCES
 "${CMAKE_CURRENT_LIST_DIR}/overlays/input-v1/gameswf_character.cpp"
 "${CMAKE_CURRENT_LIST_DIR}/overlays/input-v1/gameswf_object.cpp")
