#include "net-session-online/c_msg_attack_result.hpp"

// Partial: the registry creator is not recovered (see the header).
CMsgAttackResult::CMsgAttackResult(bool withSerial) : CMessage("CMsgAttackResult", withSerial) {}

// 0x3af330 _ZN16CMsgAttackResult6CreateEiiRKN9Character12AttackResultEb
// Partial: see the header. The level tag is 0 here.
CMessage* CMsgAttackResult::Create(int first, int second, const Character::AttackResult& result,
                                   bool flag) {
    CMessage* base = CMessage::CreateMessage("CMsgAttackResult", true);
    CMsgAttackResult* message = dynamic_cast<CMsgAttackResult*>(base);
    if (message == nullptr) {
        return base;
    }
    message->result_ = result;
    message->first_ = static_cast<std::int16_t>(first);
    message->second_ = static_cast<std::int16_t>(second);
    message->flag_ = flag;
    return base;
}
