# Additive last-player teardown/recreation safety; vendor bytes unchanged.
# No claim of original ARM32 destructor parity. Null only the deleted map.
set(DH2_PLAYER_LIFETIME_STOCK "${DH2_GAMESWF_ROOT}/gameswf/gameswf_player.cpp")
set(DH2_PLAYER_LIFETIME_MATCHES 0)
foreach(DH2_PLAYER_LIFETIME_SOURCE IN LISTS DH2_GAMESWF_SOURCES)
 if(DH2_PLAYER_LIFETIME_SOURCE STREQUAL DH2_PLAYER_LIFETIME_STOCK)
  math(EXPR DH2_PLAYER_LIFETIME_MATCHES "${DH2_PLAYER_LIFETIME_MATCHES}+1")
 endif()
endforeach()
if(NOT DH2_PLAYER_LIFETIME_MATCHES EQUAL 1)
 message(FATAL_ERROR "Player-lifetime-v1 requires exactly one stock player TU")
endif()
list(REMOVE_ITEM DH2_GAMESWF_SOURCES "${DH2_PLAYER_LIFETIME_STOCK}")
list(APPEND DH2_GAMESWF_SOURCES "${CMAKE_CURRENT_LIST_DIR}/overlays/player-lifetime-v1/gameswf_player.cpp")
