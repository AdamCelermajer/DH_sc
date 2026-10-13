#include "net-session-online/c_msg_attack_result.hpp"

#include "dh2/Application.hpp"
#include "dh2/Level.hpp"
#include "dh2/global/ctrl_camera.hpp"  // ApplicationGetInstance

// 0x3af330 _ZN16CMsgAttackResult6CreateEiiRKN9Character12AttackResultEb
// The message comes from the registry (CMessage::CreateMessage("CMsgAttackResult", true)). The level tag is the current level's list index (Level
// +0x3c, m_levelListIndex); the original reads it without a null test. The ten result words, the two ids and the flag follow.
CMessage* CMsgAttackResult::Create(int first, int second, const Character::AttackResult& result, bool flag) {
    CMessage* base = CMessage::CreateMessage("CMsgAttackResult", true);
    CMsgAttackResult* message = dynamic_cast<CMsgAttackResult*>(base);
    if (message == nullptr) {
        return base;
    }
    message->levelTag_ = ApplicationGetInstance().GetCurrentLevel()->m_levelListIndex;
    message->result_ = result;
    message->first_ = static_cast<std::int16_t>(first);
    message->second_ = static_cast<std::int16_t>(second);
    message->flag_ = flag;
    return base;
}
