#pragma once
#include <cstdint>

namespace dh2::ui {
// The authored gameplay SWF's cmdHUDpotion($playerId) calls NativeUsePotion()
// without forwarding its parameter. The offline UI has one local player.
inline bool native_use_potion_player_v1(unsigned nargs,
                                        std::int32_t supplied_player,
                                        std::int32_t& player) noexcept {
 if(nargs==0){player=0;return true;}
 if(nargs!=1)return false;
 player=supplied_player;return true;
}
}
