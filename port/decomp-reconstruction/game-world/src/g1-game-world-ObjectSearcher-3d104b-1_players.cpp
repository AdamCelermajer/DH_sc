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
// PARTIAL: the same field as GetChar (PlayerInfo +0x660, now declared as Character* unk_660 in dh2/PlayerInfo.hpp),
// returned as its GameObject base. The conversion needs Character complete, and dh2/Character.hpp cannot share a
// translation unit with dh2/PlayerManager.hpp (CustomFree is redefined in pydata-accessors/deps/stream_reader.hpp; checked
// with check_tu.sh). The body therefore stays a placeholder that returns null.
GameObject* ObjectSearcher::PlayerList::Get() {
    (void)m_players->GetPlayer(m_index, true);
    return nullptr;
}
