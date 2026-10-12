#include "net-session-online/c_msg_attack_result.hpp"

#include <cstdio>

#include "net-session-online/deps/matching.hpp"

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

// Matching stand-in: the message constructors read the local member id from it.
class FakeMatching : public CMatching {
public:
    std::uint32_t MemberBit(int memberId) const override { return 1u << memberId; }
    int LocalMemberId() const override { return 2; }
    int ServerMemberId() const override { return 1; }
    bool IsServer() const override { return false; }
    std::uint32_t GetRoomMask() const override { return 0x7; }
};

static CMessage* MakeAttackResult(bool withSerial) { return new CMsgAttackResult(withSerial); }

int main() {
    FakeMatching matching;
    CMatching::Set(&matching);
    CMessage::InitializeMessageRegistration();
    CMessage::RegisterMessageType("CMsgAttackResult", &MakeAttackResult);

    Character::AttackResult result;
    for (std::uint32_t i = 0; i < 10; ++i) result.words[i] = 100 + i;

    CMessage* created = CMsgAttackResult::Create(7, -3, result, true);
    CHECK(created != nullptr);
    CMsgAttackResult* message = dynamic_cast<CMsgAttackResult*>(created);
    CHECK(message != nullptr);
    if (message != nullptr) {
        CHECK(message->FirstForTest() == 7);
        CHECK(message->SecondForTest() == -3);
        CHECK(message->FlagForTest());
        CHECK(message->ResultForTest().words[0] == 100);
        CHECK(message->ResultForTest().words[9] == 109);
    }
    delete created;

    if (failures == 0) std::printf("c_msg_attack_result_test: all checks passed\n");
    return failures == 0 ? 0 : 1;
}
