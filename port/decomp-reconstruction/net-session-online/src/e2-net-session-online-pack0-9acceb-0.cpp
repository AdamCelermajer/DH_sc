// Track B batch e2-net-session-online-pack0-9acceb-0 (owner CMatching, ported as CMatching).
// The demangled owner in each comment is the original CMatching.
#include "net-session-online/c_matching_base.hpp"

#include <cstdint>

#include "dh2/global/net_singletons.hpp"
#include "net-session-online/c_messaging.hpp"
#include "net-session-online/c_online.hpp"
#include "net-session-online/c_sign_in.hpp"
#include "net-session-online/deps/matching_queue.hpp"
#include "net-transport/connection_manager.hpp"       // CConnectionManager (Update, vtable +0x08)
#include "net-transport/packet_manager.hpp"
#include "net-transport/tb/c_net_player_manager.hpp"
#include "net-transport/tb/cnet_player_info.hpp"  // CNetPlayerManager (Update(int), vtable +0x08)

// 0x801120 _ZN9CMatching13ProcessEventsEv
// Transcribed in the original order. Internal events (CMatching::m_eventQueueInternal) are turned into output events
// (CMatching::m_eventQueue). Calls the original makes on the CNetPlayerManager, CConnectionManager and COnline
// singletons without a check are guarded here: a missing singleton skips the call (the original would fault).
// The original returns the last callee's pointer; the port returns void.
void CMatching::ProcessEvents() {
    CEventQueueBase& internal = MatchingInternalEventQueue();
    CEventQueueBase& output = MatchingOutputEventQueue();
    std::int32_t payload = 0;

    // 0x800000: forward the payload to the output queue as 0x400000 (peek, read, post, consume).
    if (internal.HasEventOccured(0x800000, false)) {
        payload = 0;
        internal.GetEventData(0x800000, &payload, 4);
        output.AddEvent(0x400000, &payload, 4);
        internal.ConsumeEvent(0x800000);
    }

    // 0x800001: initialize the packet manager, the messaging layer and the player manager (slot count this+8).
    if (internal.HasEventOccured(0x800001, true)) {
        initialized_ = true;  // +0x0c
        CPacketManager::Initialize();
        CMessaging::Initialize();
        CNetPlayerManager::Initialize(memberCapacity_);  // *((int *)this + 2)
        output.AddEvent(0x400001, nullptr, 0);
    }

    // 0x800017 or 0x800015 (short-circuit: the second is not read when the first has fired).
    if (internal.HasEventOccured(0x800017, true) || internal.HasEventOccured(0x800015, true)) {
        output.AddEvent(0x400014, nullptr, 0);
    }

    // 0x800014 or 0x800016.
    if (internal.HasEventOccured(0x800014, true) || internal.HasEventOccured(0x800016, true)) {
        output.AddEvent(0x400013, nullptr, 0);
    }

    if (!initialized_) {
        return;
    }

    // 0x800003: the local member joins the player list and the clock is synced.
    if (internal.HasEventOccured(0x800003, true)) {
        if (CNetPlayerManager* players = GetNetPlayerMgr()) {
            players->AddPlayer(LocalMemberId(), 0);  // vtable +0x6c
        }
        if (COnline* online = GetOnline()) {
            online->SyncTime();
        }
    }

    if (internal.HasEventOccured(0x80000c, true)) {
        output.AddEvent(0x400010, nullptr, 0);
    }
    if (internal.HasEventOccured(0x80000d, true)) {
        output.AddEvent(0x400011, nullptr, 0);
    }

    // The three reset events share the reset set: players, connections, messaging queues (resetMaps = true).
    auto resetNetwork = []() {
        if (CNetPlayerManager* players = GetNetPlayerMgr()) {
            players->ResetPlayers();
        }
        if (CConnectionManager* connections = GetConnectionMgr()) {
            connections->DisconnectAll();
        }
        if (CMessaging* messaging = GetMessaging()) {
            messaging->ResetQueues(true);
        }
    };
    // 0x800004 -> 0x400009.
    if (internal.HasEventOccured(0x800004, true)) {
        resetNetwork();
        output.AddEvent(0x400009, nullptr, 0);
    }
    // 0x800005 -> 0x40000a.
    if (internal.HasEventOccured(0x800005, true)) {
        resetNetwork();
        output.AddEvent(0x40000a, nullptr, 0);
    }
    // 0x800009 -> 0x40000b, then COnline::ReportDisconnect(3, 0).
    if (internal.HasEventOccured(0x800009, true)) {
        resetNetwork();
        output.AddEvent(0x40000b, nullptr, 0);
        if (COnline* online = GetOnline()) {
            online->ReportDisconnect(static_cast<COnline::tDisconnectCause>(3), 0);
        }
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

    // The room attributes (vtable +0xa4): when they changed (CRoomAttributes slot +0x14, HasChanged) the output event
    // 0x40000f is posted, and the server, or the member that hosts the first player, resends them (slot +0x18).
    if (CRoomAttributes* attributes = GetRoomAttributes()) {
        if (attributes->HasChanged()) {
            output.AddEvent(0x40000f, nullptr, 0);
            bool resend = IsServer();
            if (!resend) {
                const std::int32_t localMember = CMatching::Get()->LocalMemberId();  // vtable +0x6c
                CNetPlayerManager* players = GetNetPlayerMgr();
                CNetPlayerInfo* host = players != nullptr
                                           ? players->GetPlayer(players->net_struct_.host_member_id_.GetValue(), false)
                                           : nullptr;  // original: no check
                resend = host != nullptr && localMember == host->matching_member_id_.GetValue();  // +0x1a0
            }
            if (resend) {
                GetRoomAttributes()->Resend();
            }
        }
    }

    // The player manager's internal queue (CNetPlayerManager +0x180): 0x300001 posts 0x400008 and syncs the clock.
    if (CNetPlayerManager* players = GetNetPlayerMgr();
        players != nullptr && players->internal_events_.HasEventOccured(0x300001, true)) {
        output.AddEvent(0x400008, nullptr, 0);
        if (COnline* online = GetOnline()) {
            online->SyncTime();
        }
    }

    // 0x800006: forwarded to 0x40000c with its payload (peek, read, consume).
    if (internal.HasEventOccured(0x800006, false)) {
        internal.GetEventData(0x800006, &payload, 4);
        internal.ConsumeEvent(0x800006);
        output.AddEvent(0x40000c, &payload, 4);
    }

    // 0x80000b: every member element gets NetStruct::SetEnabled(IsServer()), then 0x400012 is posted.
    if (internal.HasEventOccured(0x80000b, true)) {
        const bool isServer = IsServer();
        for (std::int32_t i = 0; i < memberCount_; ++i) {  // +0x1c, element stride 432
            members_[i].SetEnabled(isServer);
        }
        output.AddEvent(0x400012, nullptr, 0);
    }

    // 0x800007: a connected member is disconnected (notify = false); any other member id posts 0x40000d.
    if (internal.HasEventOccured(0x800007, false)) {
        internal.GetEventData(0x800007, &payload, 4);
        internal.ConsumeEvent(0x800007);
        if (CConnectionManager* connections = GetConnectionMgr();
            connections != nullptr && connections->IsConnected(payload)) {
            connections->Disconnect(payload, false);
        } else {
            output.AddEvent(0x40000d, &payload, 4);
        }
    }

    // The connection manager's own queue (CConnectionManager +0x08): 0x600004 carries a member id. The local member
    // reports a disconnect with cause 6 after 0x40000a; any other posts 0x40000e.
    if (CConnectionManager* connections = GetConnectionMgr();
        connections != nullptr && connections->event_queue_.HasEventOccured(0x600004, false)) {
        connections->event_queue_.GetEventData(0x600004, &payload, 4);
        connections->event_queue_.ConsumeEvent(0x600004);
        if (LocalMemberId() == payload) {  // vtable +0x6c
            output.AddEvent(0x40000a, &payload, 4);
            if (COnline* online = GetOnline()) {
                online->ReportDisconnect(static_cast<COnline::tDisconnectCause>(6), 0);
            }
        } else {
            output.AddEvent(0x40000e, &payload, 4);
        }
    }
}

// 0x8018fc _ZN9CMatching6UpdateEv
// ProcessEvents; then, when the initialized byte (+0x0c) is set: CMessaging::Get()->Update(); the connection manager's
// Update (vtable +0x08); the player manager's Update(0) (vtable +0x08); the sign-in's Update (CSignIn vtable +0x1c); and this
// object's UpdateMemberIdList (vtable +0x1c). The room mask dirty flag (+0x10) is set and 0 returned.
// Deviation: the original dereferences the connection, player and sign-in singletons unconditionally; each is tested
// here, so a missing one skips only its own call.
int CMatching::Update() {
    ProcessEvents();
    if (initialized_) {
        if (CMessaging* messaging = CMessaging::GetInstance()) {
            messaging->Update();
        }
        if (CConnectionManager* connections = CConnectionManager::GetInstance()) {
            connections->Update();
        }
        if (CNetPlayerManager* players = CNetPlayerManager::GetInstance()) {
            players->Update(0);
        }
        if (CSignIn* signIn = CSignIn::Get()) {
            signIn->Update();
        }
        UpdateMemberIdList();
    }
    roomMaskDirty_ = true;  // +0x10
    return 0;
}
