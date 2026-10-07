# Modern last-player teardown/recreation safety; immutable vendor unchanged.
# Restore standard tag registration only when the real registry was cleared.
set(DH2_LOADER_LIFETIME_STOCK "${DH2_GAMESWF_ROOT}/gameswf/gameswf_impl.cpp")
set(DH2_LOADER_LIFETIME_MATCHES 0)
foreach(DH2_LOADER_LIFETIME_SOURCE IN LISTS DH2_GAMESWF_SOURCES)
 if(DH2_LOADER_LIFETIME_SOURCE STREQUAL DH2_LOADER_LIFETIME_STOCK)
  math(EXPR DH2_LOADER_LIFETIME_MATCHES "${DH2_LOADER_LIFETIME_MATCHES}+1")
 endif()
endforeach()
if(NOT DH2_LOADER_LIFETIME_MATCHES EQUAL 1)
 message(FATAL_ERROR "Loader-lifetime-v1 requires exactly one stock impl TU")
endif()
list(REMOVE_ITEM DH2_GAMESWF_SOURCES "${DH2_LOADER_LIFETIME_STOCK}")
list(APPEND DH2_GAMESWF_SOURCES "${CMAKE_CURRENT_LIST_DIR}/overlays/loader-lifetime-v1/gameswf_impl.cpp")
