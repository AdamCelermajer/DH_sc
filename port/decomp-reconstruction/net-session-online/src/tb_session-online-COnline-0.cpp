// Track B batch e1-net-session-online-COnline-0: COnline session loop (packets, sync, ping, singleton).
//
// Partial bodies name what is missing. COnline has no declared layout for the initialized flag
// (+0x04), the CEventQueueBase (+0x08), the start time (+0x20, int64), the elapsed/counter/sync
// fields (+0x1c, +0x28, +0x2c) or the time-sync list and mutex (+0x34, +0x38). COnlineImpl declares
// its own copies of the first two, so they are not added here. Layout request in the batch report.
#include "net-session-online/c_online.hpp"

#include "net-session-online/c_online_impl.hpp"
#include "net-session-online/deps/matching.hpp"
#include "net-session-online/c_room_attributes.hpp"  // CRoomAttributes (real)
#include "net-session-online/c_room_search_filter.hpp"  // CRoomSearchFilter (real)
#include "net-session-online/deps/platform_stand_ins.hpp"
#include "net-session-online/deps/stream_bytes.hpp"
#include "net-transport/connection_manager.hpp"  // CConnectionManager (real)

namespace {
// Event types from the immediates in ReportDisconnect.
constexpr std::int32_t kDisconnectCauseEvent = 5242882;   // 0x500002
constexpr std::int32_t kDisconnectDetailEvent = 5242883;  // 0x500003
// Packet type registered for the clock sync (the 1 in Initialize, Terminate, SendSyncTime).
constexpr int kSyncPacketType = 1;
// Size of the sync packet in bytes (12 in every call).
constexpr int kSyncPacketSize = 12;
}  // namespace

COnline* COnline::s_Online = nullptr;

// 0x7fd744 _ZN7COnline11GetInstanceEv
// Original: returns s_Online when set, else allocates a 0x48-byte COnlineImpl and stores it.
// The allocation size is not carried over (the port sizes the object by its own layout).
COnline* COnline::GetInstance() {
    if (s_Online != nullptr) {
        return s_Online;
    }
    s_Online = new COnlineImpl();
    return s_Online;
}

// 0x7fd51c _ZN7COnline16ReportDisconnectENS_15tDisconnectCauseEi
// Partial: the original adds the event to the queue at +0x08 (CEventQueueBase::AddEvent, size 4).
// The queue is not declared on COnline, so only the event choice is kept here.
void COnline::ReportDisconnect(tDisconnectCause cause, std::int32_t detail) {
    const auto causeValue = static_cast<std::int32_t>(cause);
    const bool useDetail = (causeValue == 0) || (detail != 0);
    const std::int32_t eventType = useDetail ? kDisconnectDetailEvent : kDisconnectCauseEvent;
    const std::int32_t payload = useDetail ? detail : causeValue;
    (void)eventType;
    (void)payload;
}

// 0x7fd568 _ZN7COnline7GetPingEv
// Partial: the original returns CConnectionManager::GetServerPing() when the online flag (+0x05) is
// set. GetServerPing is not in the net-session-online stand-in, so the online branch returns 0.
std::int32_t COnline::GetPing() const {
    // Offline: 0, as in the original. Online: the server ping is not available yet, so 0.
    return 0;
}

// 0x7fd634 _ZN7COnline11SendPacketsEv
// Partial: the original calls CPacketManager::IsInitialized and SendPackets (net-transport, not in
// this category) and then CMessaging::ProcessSendQueue (not declared on CMessaging). Neither is called.
void COnline::SendPackets() {}

// 0x7fd66c _ZN7COnline7DestroyEv
// Partial: the original calls CMatching::Destroy(CMatching::Get()) first (not in the stand-in).
// The rest is reconstructed: terminate the singleton, delete it, clear the pointer. The original
// deletes through the virtual deleting destructor (vtable +0x0c); COnline is not polymorphic here,
// so the instance is deleted as the COnlineImpl it was created as.
void COnline::Destroy() {
    if (s_Online == nullptr) {
        return;
    }
    s_Online->Terminate();
    delete static_cast<COnlineImpl*>(s_Online);
    s_Online = nullptr;
}

// 0x7fd6d0 _ZN7COnline9TerminateEv
// Partial: the original clears the initialized flag (+0x04) and clears the event queue at +0x08.
// Neither field is declared on COnline, so only the packet-type unregister is done.
void COnline::Terminate() {
    CConnectionManager::GetInstance()->UnregisterPacketType(kSyncPacketType);
}

// 0x7fd6f4 _ZN7COnline10InitializeEv
// Partial: the original reads the initialized flag (+0x04) and does the body only when it is clear.
// It then stores the vtable slot 0 time (int64) at +0x20 and registers the sync callback with the
// flag value as the protocol. The guard and the +0x20 store are not declared, so the registration
// runs on every call. Map registration is idempotent, so the effect is the same.
std::int32_t COnline::Initialize() {
    CConnectionManager::GetInstance()->RegisterPacketType(kSyncPacketType, 0, &COnline::sPacketReceiverCallback);
    return 0;
}

// 0x7fd914 _ZN7COnline14ReceivePacketsEv
// Partial: the original first records the elapsed time since the start time (+0x20) into +0x1c and
// bumps the counter at +0x28, then processes the messaging receive queue and calls the packet
// manager's ReceivePackets. None of those fields, CPacketManager (net-transport) or the messaging
// receive queue entry point are available here. Returns 0.
std::int32_t COnline::ReceivePackets() {
    return 0;
}

// 0x7fd974 _ZN7COnline12SendSyncTimeEv
// Partial: a client sends its time offset (12 bytes, sync packet type 1) to the server member through
// CConnectionManager::SendTo(type, member, data, size). That point-to-point send is not in the
// stand-in, and the offset is not stored (+0x2c not declared). The server does nothing, as in the original.
void COnline::SendSyncTime() {
    if (IsServer()) {
        return;
    }
}

// 0x7fd9ec _ZN7COnline8SyncTimeEv
// Partial: the client branch clears the time-sync list under the list mutex (+0x34, +0x38), neither of
// which is declared. The flag and the re-send are kept.
void COnline::SyncTime() {
    if (IsServer()) {
        timeSynced_ = true;
        return;
    }
    timeSynced_ = false;
    SendSyncTime();
}

// 0x7fdb58 _ZN7COnline6UpdateEf
// Partial: the original runs the matching and net-log update slots when the initialized flag (+0x04)
// is set, then re-sends the sync request when the clock is not synced and more than 1000 ms have
// passed since the last send (clock vtable slot 0, +0x20 start time, +0x2c sync time). None of those
// are declared, so this body does nothing yet.
void COnline::Update(float /*dt*/) {}

// 0x7fdbe4 _ZN7COnline22PacketReceiverCallbackEiPci
// Partial: the guard is reconstructed (a connected member sending a 12-byte packet). The server branch
// echoes the packet back to the sender with the server-side time filled in (+0x2c, point-to-point send,
// not in the stand-in). The client branch stores the sample in the time-sync list under the list mutex
// (+0x34, +0x38) and, once enough samples arrive, averages them and adjusts the local clock. None of
// that is declared, so only the guard and the server-side check are kept.
void COnline::PacketReceiverCallback(std::int32_t memberId, char* /*data*/, std::int32_t size) {
    CConnectionManager& connections = (*CConnectionManager::GetInstance());
    if (!connections.IsConnected(memberId) || size != kSyncPacketSize) {
        return;
    }
    if (CMatching::Get()->IsServer()) {
        // Server echo: not reconstructed (see above).
        return;
    }
    // Client sample: not reconstructed (see above).
}

// 0x7fdde8 _ZN7COnline23sPacketReceiverCallbackEiPci
// The registered callback. Forwards to the singleton, as in the original.
void COnline::sPacketReceiverCallback(std::int32_t memberId, char* data, std::int32_t size) {
    COnline* instance = GetInstance();
    instance->PacketReceiverCallback(memberId, data, size);
}
