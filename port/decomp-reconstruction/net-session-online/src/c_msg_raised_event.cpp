#include "net-session-online/c_msg_raised_event.hpp"

#include "net-session-online/deps/matching.hpp"

#include "dh2/QuestEvent.hpp"
#include "dh2/tMsgRaisedEventData.hpp"

// 0x329630 _ZN15CMsgRaisedEventC1Eb
// Partial: the base is the partial CMessage(const char*, bool). The payload words (+0x50..+0x63)
// are zeroed by the member initialisers. The rest of the original body is here: the type name,
// reliable, sendMode 1 (remote) and includeLocalMember. The base sets sendMode 0, so this matters.
CMsgRaisedEvent::CMsgRaisedEvent(bool withSerial) : CMessage("CMsgRaisedEvent", withSerial) {
    typeName_ = "CMsgRaisedEvent";  // +0x14
    reliable_ = true;               // +0x30
    sendMode_ = 1;                  // +0x2c
    includeLocalMember_ = true;     // +0x33
}

// 0x47ac58 _ZN15CMsgRaisedEvent6CreateEP10QuestEvent
// Creates the "CMsgRaisedEvent" message and has the quest event fill the payload at +0x50 (QuestEvent vtable slot +0x0c,
// FillNetworkMsg, with the message's five payload words as a tMsgRaisedEventData). Returns the message.
CMessage* CMsgRaisedEvent::Create(QuestEvent* event) {
    CMessage* message = CMessage::CreateMessage("CMsgRaisedEvent", true);
    static_assert(sizeof(tMsgRaisedEventData) == 20, "the payload is five 32-bit words");
    event->FillNetworkMsg(*reinterpret_cast<tMsgRaisedEventData*>(static_cast<CMsgRaisedEvent*>(message)->GetDataPtr()));
    return message;
}

// 0x49a238 _ZN15CMsgRaisedEvent21RaiseEventFromNetworkEPS_
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
