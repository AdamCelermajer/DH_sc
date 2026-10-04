# Call with an explicit UI source list after adding frozen helper/session TUs.
# Select this version once; never compile both a wrapper and its included body.
set(DH2_SOURCE_FACADE_V1_ROOT "${CMAKE_CURRENT_LIST_DIR}")
function(dh2_select_source_facade_v1 DH2_SOURCE_LIST_VARIABLE)
 set(DH2_SOURCE_SELECTED "${${DH2_SOURCE_LIST_VARIABLE}}")
 foreach(DH2_SOURCE_PAIR IN ITEMS
     "swf_movie.cpp|swf_movie.cpp"
     "swf_source_movie_v1.cpp|swf_source_movie.cpp"
     "swf_input_session_v1.cpp|swf_input_session_v1.cpp"
     "swf_input_session_v2.cpp|swf_input_session_v2.cpp")
  string(REPLACE "|" ";" DH2_SOURCE_PAIR_PARTS "${DH2_SOURCE_PAIR}")
  list(GET DH2_SOURCE_PAIR_PARTS 0 DH2_SOURCE_OLD_NAME)
  list(GET DH2_SOURCE_PAIR_PARTS 1 DH2_SOURCE_NEW_NAME)
  set(DH2_SOURCE_MATCHES 0)
  foreach(DH2_SOURCE_ITEM IN LISTS DH2_SOURCE_SELECTED)
   get_filename_component(DH2_SOURCE_ABSOLUTE "${DH2_SOURCE_ITEM}" ABSOLUTE BASE_DIR "${DH2_SOURCE_FACADE_V1_ROOT}")
   if(DH2_SOURCE_ABSOLUTE STREQUAL "${DH2_SOURCE_FACADE_V1_ROOT}/${DH2_SOURCE_OLD_NAME}")
    math(EXPR DH2_SOURCE_MATCHES "${DH2_SOURCE_MATCHES}+1")
    set(DH2_SOURCE_OLD_ITEM "${DH2_SOURCE_ITEM}")
   endif()
  endforeach()
  if(NOT DH2_SOURCE_MATCHES EQUAL 1)
   message(FATAL_ERROR "Source-facade-v1 requires exactly one ${DH2_SOURCE_OLD_NAME}")
  endif()
  list(REMOVE_ITEM DH2_SOURCE_SELECTED "${DH2_SOURCE_OLD_ITEM}")
  list(APPEND DH2_SOURCE_SELECTED "${DH2_SOURCE_FACADE_V1_ROOT}/overlays/source-facade-v1/${DH2_SOURCE_NEW_NAME}")
 endforeach()
 list(APPEND DH2_SOURCE_SELECTED "${DH2_SOURCE_FACADE_V1_ROOT}/swf_source_startup_v1.cpp")
 set(${DH2_SOURCE_LIST_VARIABLE} "${DH2_SOURCE_SELECTED}" PARENT_SCOPE)
endfunction()
