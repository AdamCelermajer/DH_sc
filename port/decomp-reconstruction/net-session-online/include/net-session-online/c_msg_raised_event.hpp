// CMsgRaisedEvent: a quest event raised by one player and replayed on the others. The payload
// starts at +0x50 (the CMessage base ends at +0x50): five 32-bit words, +0x50..+0x63. The names
// come from the reads in RaiseEventFromNetwork. The QE_* quest-event classes are not ported.
#pragma once

#include <cstdint>

#include "net-session-online/c_message.hpp"

class QuestEvent;

class CMsgRaisedEvent : public CMessage {
public:
    // 0x329630 _ZN15CMsgRaisedEventC1Eb. Partial: the body is complete (payload zeroed, type name,
    // reliable, sendMode 1, includeLocalMember), but it runs the CMessage(const char*, bool) base,
    // which is partial (the byte at +0x3c is not modelled).
    explicit CMsgRaisedEvent(bool withSerial);

    // Payload (original 32-bit offsets). Zeroed by the constructor.
    std::int32_t eventKind_ = 0;         // +0x50: selector of the switch in RaiseEventFromNetwork
    std::int32_t objectNetworkId_ = 0;   // +0x54: network id of the object the event names
    std::int32_t unk_58 = 0;             // +0x58: read and copied into the quest event, role not identified
    std::int32_t unk_5c = 0;             // +0x5c: read and copied into the quest event, role not identified
    std::int32_t targetNetworkId_ = 0;   // +0x60: network id of the second object (case 4 resolves it)

    // 0x47ac58 _ZN15CMsgRaisedEvent6CreateEP10QuestEvent. Partial: the original fills the payload
    // at +0x50 by calling a QuestEvent virtual slot (+0x0c) that is not recovered. Here the
    // message is created and the fill is skipped. Returns null when the type is not registered.
    static CMessage* Create(QuestEvent* event);

    // 0x49a238 _ZN15CMsgRaisedEvent21RaiseEventFromNetworkEPS_. Partial: only the guard is
    // reproduced. A message whose sender is the local member is ignored. The quest-event objects
    // (QE_* classes), ObjectManager, PyDataConstants, EventManager and Application are not ported,
    // so other messages are not raised here. The original's return value is not reproduced.
    static void RaiseEventFromNetwork(CMsgRaisedEvent* message);
};
