// CMsgRaisedEvent: a quest event raised by one player and replayed on the others. The payload
// starts at +0x50 (the CMessage base ends at +0x50). Only the lookup of the local sender is
// reconstructed; the payload fields (type, network id and three words at +0x50..+0x64) are
// read by the queued parts of RaiseEventFromNetwork and are not modelled here yet.
#pragma once

#include "net-session-online/c_message.hpp"

class QuestEvent;

class CMsgRaisedEvent : public CMessage {
public:
    // Partial: the registry creator is not recovered. This is the plain base constructor call.
    explicit CMsgRaisedEvent(bool withSerial);

    // 0x47ac58 _ZN16CMsgRaisedEvent6CreateEP10QuestEvent. Partial: the original fills the payload
    // at +0x50 by calling a QuestEvent virtual slot (+0x0c) that is not recovered. Here the
    // message is created and the fill is skipped. Returns null when the type is not registered.
    static CMessage* Create(QuestEvent* event);

    // 0x49a238 _ZN16CMsgRaisedEvent17RaiseEventFromNetworkEPS_. Partial: only the guard is
    // reproduced. A message whose sender is the local member is ignored. The quest-event objects
    // (QE_* classes), ObjectManager, PyDataConstants, EventManager and Application are not ported,
    // so other messages are not raised here. The original's return value is not reproduced.
    static void RaiseEventFromNetwork(CMsgRaisedEvent* message);
};
