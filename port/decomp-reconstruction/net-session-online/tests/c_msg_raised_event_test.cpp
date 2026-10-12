#include "net-session-online/c_msg_raised_event.hpp"

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

static CMessage* MakeRaisedEvent(bool withSerial) { return new CMsgRaisedEvent(withSerial); }

int main() {
    FakeMatching matching;
    CMatching::Set(&matching);
    CMessage::InitializeMessageRegistration();
    CMessage::RegisterMessageType("CMsgRaisedEvent", &MakeRaisedEvent);

    // Create builds a message of the registered type (the payload fill is partial).
    CMessage* created = CMsgRaisedEvent::Create(nullptr);
    CHECK(created != nullptr);
    CHECK(dynamic_cast<CMsgRaisedEvent*>(created) != nullptr);
    delete created;

    // A message from the local member (id 2) is ignored, and so is a null message.
    CMsgRaisedEvent local(true);
    CMsgRaisedEvent::RaiseEventFromNetwork(&local);
    CMsgRaisedEvent::RaiseEventFromNetwork(nullptr);

    if (failures == 0) std::printf("c_msg_raised_event_test: all checks passed\n");
    return failures == 0 ? 0 : 1;
}
