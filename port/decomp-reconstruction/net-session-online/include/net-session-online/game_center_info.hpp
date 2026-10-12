// GameCenterInfo: the Game Center record of a player list. Only the list at +0x10 is
// modelled, because only the destructor is reconstructed. The other members (+0x00..+0x0f)
// are not identified.
//
// Layout (original 32-bit offsets, kept as comments):
//   +0x10 list of player-ID strings (std::list<std::string>, sentinel at +0x10)
#pragma once

#include <list>
#include <string>

class GameCenterInfo {
public:
    // 0x81ce00 _ZN14GameCenterInfoD1Ev: clears the list. The original returns the object
    // pointer, which a destructor does not expose.
    ~GameCenterInfo();

    // Test seam. Not in the original.
    std::list<std::string>& IdsForTest() { return ids_; }

private:
    std::list<std::string> ids_;  // +0x10
};
