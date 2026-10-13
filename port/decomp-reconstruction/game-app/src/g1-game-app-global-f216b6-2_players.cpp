// Batch g1-game-app-global-f216b6-2, companion of g1-game-app-global-f216b6-2.cpp (<global>, part 3 of 22).
// Split off because dh2/PlayerManager.hpp (net-transport IStreamBase) and dh2/Character.hpp (pydata IStreamBase)
// define the same class differently, so one translation unit cannot include both.
// Declarations: dh2/global/IsMultiplayerGame.hpp.
#include "dh2/global/IsMultiplayerGame.hpp"

#include "dh2/PlayerManager.hpp"

// 0x3a42f4 bool IsMultiplayerGame()
// PARTIAL. The original is *(GetOnline() + 5) || PlayerManager::GetNumLocalPlayers(dword_99F76C, 1) > 1. GetOnline is a
// thunk to COnline::GetInstance (0x7fd794). The +0x05 byte is COnline::isOnlineGame_, which is private and has no public
// getter (net-session-online/include/net-session-online/c_online.hpp). Layout request: a public getter for +0x05 on COnline.
// Until it exists the first term is stubbed to false. The player-count term is written as in the original (flag 1 is true).
bool IsMultiplayerGame() {
    const bool onlineGame = false;  // stub for *(GetOnline() + 5)
    return onlineGame || dword_99F76C->GetNumLocalPlayers(true) > 1;
}
