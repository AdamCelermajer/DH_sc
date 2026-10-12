#include "net-session-online/c_msg_raised_event.hpp"

#include "net-session-online/deps/matching.hpp"

// Partial: the registry creator is not recovered (see the header).
CMsgRaisedEvent::CMsgRaisedEvent(bool withSerial) : CMessage("CMsgRaisedEvent", withSerial) {}

// 0x47ac58 _ZN16CMsgRaisedEvent6CreateEP10QuestEvent
// Partial: the payload fill from the QuestEvent is not ported (see the header).
CMessage* CMsgRaisedEvent::Create(QuestEvent* /*event*/) {
    return CMessage::CreateMessage("CMsgRaisedEvent", true);
}

// 0x49a238 _ZN16CMsgRaisedEvent17RaiseEventFromNetworkEPS_
// Partial: see the header. The original returns the EventManager it raised into; not reproduced.
void CMsgRaisedEvent::RaiseEventFromNetwork(CMsgRaisedEvent* message) {
    if (message == nullptr) {
        return;
    }
    CMatching* matching = CMatching::Get();
    if (matching == nullptr) {
        return;
    }
    if (message->IsFrom(matching->LocalMemberId())) {
        return;  // the original's guard: the sender is this member
    }
    // Not ported: building the QE_* quest event for the message type and raising it.
}
