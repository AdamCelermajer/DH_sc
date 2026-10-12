// CMsgAttackResult: the result of an attack, sent to the other players. The payload starts at
// +0x50. Layout (original 32-bit offsets, kept as comments):
//   +0x50 level tag (int)   +0x54..+0x7b Character::AttackResult (40 bytes, stand-in)
//   +0x7c first short   +0x7e second short   +0x80 flag (bool)
#pragma once

#include <cstdint>

#include "net-session-online/c_message.hpp"
#include "net-session-online/deps/character_attack_result.hpp"

class CMsgAttackResult : public CMessage {
public:
    // Partial: the registry creator is not recovered. This is the plain base constructor call.
    explicit CMsgAttackResult(bool withSerial);

    // 0x3af330 _ZN15CMsgAttackResult6CreateEiiRKN9Character12AttackResultEb
    // Partial: the original stores the current level's field at +0x3c (level +60), which is not
    // ported (Application and Level are missing), so the tag is stored as 0. The result copy, the
    // two shorts and the flag are reproduced. Returns null when the type is not registered.
    static CMessage* Create(int first, int second, const Character::AttackResult& result, bool flag);

    // Test seams. Not in the original.
    const Character::AttackResult& ResultForTest() const { return result_; }
    std::int16_t FirstForTest() const { return first_; }
    std::int16_t SecondForTest() const { return second_; }
    bool FlagForTest() const { return flag_; }

private:
    std::int32_t levelTag_ = 0;             // +0x50
    Character::AttackResult result_{};      // +0x54
    std::int16_t first_ = 0;                // +0x7c
    std::int16_t second_ = 0;               // +0x7e
    bool flag_ = false;                     // +0x80
};
