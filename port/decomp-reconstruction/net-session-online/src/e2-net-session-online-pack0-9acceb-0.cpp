// Track B batch e2-net-session-online-pack0-9acceb-0 (owner CMatching, ported as CMatchingBase).
// Both functions are partial. The demangled owner in each comment is the original CMatching.
#include "net-session-online/c_matching_base.hpp"

#include <cstdint>

#include "net-session-online/deps/matching_queue.hpp"

// 0x801120 _ZN9CMatching13ProcessEventsEv
// Partial. Reproduced, in the original order:
//   - internal 0x800000 is forwarded to the output queue as 0x400000 with its payload;
//   - internal 0x800001 sets initialized_ (+0x0c) and posts 0x400001;
//   - 0x800017 or 0x800015 posts 0x400014, and 0x800014 or 0x800016 posts 0x400013;
//   - in the initialized branch: 0x800003 is consumed, 0x80000c/0x80000d/0x800012/0x80000e/0x80000f
//     post 0x400010/0x400011/0x400007/0x400003/0x400004, 0x800004/0x800005/0x800009 post
//     0x400009/0x40000a/0x40000b, 0x800011 and 0x800010 are consumed, 0x800006 is forwarded as
//     0x40000c with its payload, 0x80000b posts 0x400012, and 0x800007 disconnects a connected member
//     or posts 0x40000d.
// Not reproduced (each branch below is named where it sits in the body):
//   - CPacketManager::Initialize, CMessaging::Initialize and CNetPlayerManager::Initialize (this+8)
//     on 0x800001. The output event 0x400001 is still posted.
//   - The player-manager calls on 0x800003 (AddPlayer and COnline::SyncTime), and on the reset events
//     0x800004, 0x800005 and 0x800009 (CNetPlayerManager::ResetPlayers, CConnectionManager::DisconnectAll,
//     CMessaging::Get()->ResetQueues) and on 0x800009 (COnline::ReportDisconnect with reason 3).
//     Their output events are posted, the calls are not.
//   - The room-attribute predicate (vtable +0x14 on GetRoomAttributes()) with its +0x18 call, and
//     the event 0x40000f it gates, together with the server-or-local-member check.
//   - The CNetPlayerManager event queue at +0x180 (event 0x300001) and its 0x400008 and SyncTime.
//   - The disconnect of 0x800007 goes through the DisconnectMember seam, which CMatchingLocal and the
//     other derived classes do not forward yet, so the member stays connected in those classes.
//   - NetStruct::SetEnabled on each member element for 0x80000b. Only the flag read (IsServer) is done.
//   - The CConnectionManager event queue at +0x08 (event 0x600004) and its member-id branch
//     (posts 0x40000a with COnline::ReportDisconnect(6), or 0x40000e), which is the last block of the body.
// Deviation: the original returns the last callee's pointer. The port returns void.
void CMatchingBase::ProcessEvents() {
    CEventQueueBase& internal = MatchingInternalEventQueue();
    CEventQueueBase& output = MatchingOutputEventQueue();
    std::int32_t payload = 0;

    // 0x800000: forward the payload to the output queue (peek, then read, then consume).
    if (internal.HasEventOccured(0x800000, false)) {
        payload = 0;
        internal.GetEventData(0x800000, &payload, 4);
        output.AddEvent(0x400000, &payload, 4);
        internal.ConsumeEvent(0x800000);
    }

    // 0x800001: initialize. The flag is set before the (not ported) initializers run.
    if (internal.HasEventOccured(0x800001, true)) {
        initialized_ = true;  // +0x0c
        // Not ported: CPacketManager::Initialize, CMessaging::Initialize, CNetPlayerManager::Initialize(this+8).
        output.AddEvent(0x400001, nullptr, 0);
    }

    // 0x800017 or 0x800015 (short-circuit: the second is not read when the first has fired).
    if (internal.HasEventOccured(0x800017, true) || internal.HasEventOccured(0x800015, true)) {
        output.AddEvent(0x400014, nullptr, 0);
    }

    // 0x800014 or 0x800016 (the second is read only when the first has not fired).
    if (internal.HasEventOccured(0x800014, true) || internal.HasEventOccured(0x800016, true)) {
        output.AddEvent(0x400013, nullptr, 0);
    }

    if (initialized_) {
        // 0x800003: AddPlayer(GetNetPlayerMgr(), LocalMemberId(), 0) and SyncTime. Not ported.
        if (internal.HasEventOccured(0x800003, true)) {
        }

        // 0x80000c and 0x80000d are forwarded as they are.
        if (internal.HasEventOccured(0x80000c, true)) {
            output.AddEvent(0x400010, nullptr, 0);
        }
        if (internal.HasEventOccured(0x80000d, true)) {
            output.AddEvent(0x400011, nullptr, 0);
        }

        // 0x800004: ResetPlayers, DisconnectAll and ResetQueues(true) not ported; 0x400009 is posted.
        if (internal.HasEventOccured(0x800004, true)) {
            output.AddEvent(0x400009, nullptr, 0);
        }
        // 0x800005: same reset set not ported; 0x40000a is posted.
        if (internal.HasEventOccured(0x800005, true)) {
            output.AddEvent(0x40000a, nullptr, 0);
        }
        // 0x800009: same reset set not ported; 0x40000b is posted. COnline::ReportDisconnect(3, 0) not ported.
        if (internal.HasEventOccured(0x800009, true)) {
            output.AddEvent(0x40000b, nullptr, 0);
        }

        // 0x800011 and 0x800010: consumed, nothing forwarded.
        if (internal.HasEventOccured(0x800011, true)) {
            internal.ConsumeEvent(0x800011);
        }
        if (internal.HasEventOccured(0x800010, true)) {
            internal.ConsumeEvent(0x800010);
        }

        if (internal.HasEventOccured(0x800012, true)) {
            output.AddEvent(0x400007, nullptr, 0);
        }
        if (internal.HasEventOccured(0x80000e, true)) {
            output.AddEvent(0x400003, nullptr, 0);
        }
        if (internal.HasEventOccured(0x80000f, true)) {
            output.AddEvent(0x400004, nullptr, 0);
        }

        // Not ported: the room-attribute predicate (vtable +0x14 on GetRoomAttributes()), the vtable +0x18
        // call on the same object, and the event 0x40000f with its server-or-local-member check.

        // Not ported: the player-manager queue (CNetPlayerManager+0x180, event 0x300001), which posts 0x400008
        // and calls COnline::SyncTime.

        // 0x800006: forwarded to 0x40000c with its payload (peek, read, consume).
        if (internal.HasEventOccured(0x800006, false)) {
            internal.GetEventData(0x800006, &payload, 4);
            internal.ConsumeEvent(0x800006);
            output.AddEvent(0x40000c, &payload, 4);
        }

        // 0x80000b: the member flags are set from IsServer(). The NetStruct::SetEnabled loop over the
        // member elements is not ported. 0x400012 is posted.
        if (internal.HasEventOccured(0x80000b, true)) {
            const bool isServer = IsServer();
            (void)isServer;  // read by the skipped SetEnabled loop
            output.AddEvent(0x400012, nullptr, 0);
        }

        // 0x800007: a connected member is disconnected; any other member id posts 0x40000d with its payload.
        if (internal.HasEventOccured(0x800007, false)) {
            internal.GetEventData(0x800007, &payload, 4);
            internal.ConsumeEvent(0x800007);
            if (IsConnectedMember(payload)) {
                DisconnectMember(payload);
            } else {
                output.AddEvent(0x40000d, &payload, 4);
            }
        }

        // Not ported: the connection-manager queue (event 0x600004) and its member-id branch, the last
        // block of the body.
    }
}

// 0x8018fc _ZN9CMatching6UpdateEv
// Partial. Reproduced: ProcessEvents runs first, then the room mask dirty flag (+0x10) is set and the
// original returns 0. Not reproduced: the initialized branch, which updates the message layer
// (CMessaging::Get()->Update), the connection manager (vtable +0x08), the net player manager
// (vtable +0x08, then CSignIn::Get()->vtable +0x1c) and the virtual call at vtable +0x1c on this.
int CMatchingBase::Update() {
    ProcessEvents();
    roomMaskDirty_ = true;  // +0x10
    return 0;
}
