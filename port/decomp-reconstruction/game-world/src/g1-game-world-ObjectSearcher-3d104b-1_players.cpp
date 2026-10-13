// Batch g1-game-world-ObjectSearcher-3d104b-1, companion of g1-game-world-ObjectSearcher-3d104b-1.cpp (part 2 of 3).
// Split off because dh2/PlayerManager.hpp (net-transport IStreamBase) and dh2/Character.hpp (pydata IStreamBase) define
// the same class differently, so one translation unit cannot include both (see the part 0 players file).
//
// PARTIAL, both methods: they read the player's Character field, PlayerInfo +0x660 (1632), from the record that
// PlayerManager::GetPlayer(index, true) returns. dh2/PlayerManager.hpp only forward-declares PlayerInfo, so the field
// cannot be read yet. The lookup is still called. The methods return null until dh2/PlayerInfo.hpp declares the field.

#include "dh2/ObjectSearcher.hpp"
#include "dh2/PlayerManager.hpp"

// 0x38d7bc _ZN14ObjectSearcher10PlayerList7GetCharEv
// PARTIAL: the original returns *(GetPlayer(m_index, 1) + 0x660) as a Character*.
Character* ObjectSearcher::PlayerList::GetChar() {
    (void)m_players->GetPlayer(m_index, true);
    return nullptr;
}

// 0x38d7d4 _ZN14ObjectSearcher10PlayerList3GetEv
// PARTIAL: the same field as GetChar, returned as its GameObject base (the original returns the same word for both).
GameObject* ObjectSearcher::PlayerList::Get() {
    (void)m_players->GetPlayer(m_index, true);
    return nullptr;
}
