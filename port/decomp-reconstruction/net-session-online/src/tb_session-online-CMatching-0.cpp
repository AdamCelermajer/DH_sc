// Track B batch e1-net-session-online-CMatching-0.
// The original class CMatching is ported as CMatching (c_matching_base.hpp), the same convention
// c_matching_base.cpp uses. Functions whose callees or layout are not in this category are listed
// with no body; the ledger notes name what is missing.

#include "net-session-online/c_matching_base.hpp"
#include "net-transport/connection_manager.hpp"  // CConnectionManager::Initialize

#include "net-session-online/deps/matching_queue.hpp"

// 0x7fe4e0 _ZN9CMatching8IsServerEv
// The original is non-virtual; the port keeps the existing pure virtual declaration, so this is its
// out-of-line definition. Reads the initialized flag (+0x0c), then compares the local member id with
// the server member id (vtable +0x6c and +0x74).
bool CMatching::IsServer() const {
    if (!initialized_ || LocalMemberId() < 0) return false;
    const int local = LocalMemberId();
    return local == ServerMemberId();
}

// 0x7fe574 _ZN9CMatching8LockRoomEb
// Partial. The original calls the virtual slot +0x48 when the argument is true; that slot is not
// identified in this category, so the call is not made. The original's return value is that call's
// result (or the object pointer when the argument is false); the port returns 0 in both cases.
int CMatching::LockRoom(bool /*callSlot*/) {
    baseRoomClosed_ = true;  // +0x0e
    return 0;
}

// 0x7fe5e0 _ZN9CMatching9GetTeamIdEi
// Reconstructed. Team value is the unsigned byte at element +0x178 (this+0x198 for member 0).
int CMatching::GetTeamId(int memberId) const {
    const std::int32_t index = GetMemberIndex(memberId);
    if (index < 0 || index >= memberCount_) return 0;
    return static_cast<int>(static_cast<std::uint8_t>(members_[index].teamId));
}

// 0x7fe618 _ZN9CMatching18GetMemberIdByIndexEj
// Partial: the original indexes the member table without a check; the port returns -1 for an index at
// or past the 32-slot table so it never reads outside the array.
std::int32_t CMatching::GetMemberIdByIndex(std::uint32_t index) const {
    if (index >= static_cast<std::uint32_t>(kMaxMembers)) return -1;
    return members_[index].memberId;  // element +0x150
}

// 0x7fff9c _ZN9CMatchingC1Ev and 0x80003c _ZN9CMatchingC2Ev
// Partial. Both bodies are the same. Ported: the room mask dirty flag (+0x10) is set, the member count
// (+0x1c) is set to 32, and the flags are cleared. The member elements are default-constructed. Not
// ported: the vptr of the NetStructArray header (+0x18), the node-allocated container at +0x3620 (which
// the destructor frees), and the NetStruct element constructors.
CMatching::CMatching() {
    memberCapacity_ = 0;       // +0x08
    initialized_ = false;      // +0x0c
    roomHidden_ = false;       // +0x0d
    baseRoomClosed_ = false;   // +0x0e
    unk_0f = 0;                // +0x0f
    roomMaskDirty_ = true;     // +0x10
    memberCount_ = kMaxMembers;  // +0x1c
}

// 0x800a68 _ZN9CMatchingD2Ev, 0x800b00 _ZN9CMatchingD1Ev
// Partial. The port's implicit virtual destructor destroys members_. Not ported: the free of the
// node-allocated container at +0x3620 and the per-element NetStruct destructor call (vtable slot 0).
// 0x800b98 _ZN9CMatchingD0Ev is the deleting destructor and is compiler-generated (see ledger).

// Functions below have no body yet. Each one needs callees or layout that are not in this category
// (see the ledger notes and the layout requests in the batch report).

// 0x7fea14 _ZN9CMatching14ReadPacketDataEiiR12NetBitStream
// Partial, no body. Needs CRoomAttributes::Unserialize with the bits argument (the port's stand-in has
// one argument) and NetStructArray<MemberInfoNetStruct,32>::Load.

// 0x7feac4 _ZN9CMatching16ResendMemberDataEv
// Partial, no body. Needs NetStruct::Resend on each member element (the NetStruct base is not modelled).

// 0x7feb40 _ZN9CMatching10SearchRoomE17CRoomSearchFilterbh
// Partial, no body. Needs CRoomSearchFilter::SetSearchAttributeInt (not in the port) and the virtual
// slot +0x08 of the search object, which is not identified.

// 0x7feb8c _ZN9CMatching7DestroyEv
// Partial, no body. Needs the static instance CMatching::s_Matching, CSignIn::Get and the CSignIn and
// CMatching virtual slots +0x2c, +0x24 (these slot names are not identified).

// 0x7fed08 _ZN9CMatching25ProcessAcknowledgedPacketEii
// Partial, no body. Needs CRoomAttributes::ProcessAcknowledgedPacket (not in the port's CRoomAttributes)
// and NetStruct slot +0x40 on each member element.

// 0x7ff120 _ZN9CMatching8JoinRoomEy
// Partial, no body. Needs CMessaging::Get and ResetQueues, GetNetPlayerMgr and CNetPlayerManager::ResetPlayers,
// NetStructMember::SetChanged, the per-member Set slots, NetStruct::SetEnabled, and the virtual slot 0 that
// receives the join request. The per-member reset is recovered (member id, team and previous id to -1,
// attribute buffer cleared, enabled off) and can be written once NetStruct is in the port.

// 0x7ff350 _ZN9CMatching14CreateJoinRoomEb15CRoomAttributes
// Partial, no body. Same member reset as JoinRoom (decompiled with the object as CMessaging). Needs
// CRoomAttributes::SetAttributeInt(int, int) with the attribute id, NetStruct::SetEnabled and the
// virtual slot +0x04 that receives the request.

// 0x7ff5a0 _ZN9CMatching9TerminateEv
// Partial, no body. Needs CPacketManager::UnregisterPacketSlot and Terminate, CNetPlayerManager::Terminate,
// CMessaging::Terminate, CConnectionManager::Terminate, CSignIn::Get and its vtable +0x20, the virtual
// slots +0x3c, +0x64 and +0xa4 of CMatching and slot +0x10 of the room attributes (names not identified),
// and the AddEvent code 4194306.

// 0x7ff85c _ZN9CMatching10InitializeEi
// Partial. Reproduced: when the matching object is already initialized (+0x0c) it posts 0x400001 on the
// output queue (CMatching::m_eventQueue) and returns. Otherwise it stores the capacity (+0x08), clears the
// room hidden (+0x0d) and room closed (+0x0e) flags, clears both matching queues, and resets every member
// element (id -1, team 0, attribute buffer empty, previous id -1). The original returns 0 on both paths.
// Not reproduced: CPacketManager::RegisterPacketSlot with the four static forwarders, the call through
// CSignIn::Get()'s vtable +0x08 into CConnectionManager::Initialize, the room attributes' vtable +0x10
// slot (not identified), the NetStructMember change notifications of the element resets, and
// NetStruct::SetEnabled(element, false) on every element.
int CMatching::Initialize(int memberCapacity) {
    if (initialized_) {
        MatchingOutputEventQueue().AddEvent(0x400001, nullptr, 0);  // 4194305
        return 0;
    }
    memberCapacity_ = memberCapacity;  // +0x08
    baseRoomClosed_ = false;           // +0x0e
    roomHidden_ = false;               // +0x0d
    MatchingOutputEventQueue().Clear();    // CMatching::m_eventQueue
    MatchingInternalEventQueue().Clear();  // CMatching::m_eventQueueInternal
    for (std::int32_t i = 0; i < memberCount_; ++i) {
        MemberInfo& member = members_[i];
        member.memberId = -1;          // element +0x150
        member.teamId = 0;             // element +0x178
        member.roomAttribute.clear();  // element +0x180
        member.previousMemberId = -1;  // element +0x1a8
    }
    // The binary reaches CConnectionManager::Initialize through CSignIn::Get()'s vtable +0x08; the port calls
    // it directly (CSignIn is not ported).
    if (!CConnectionManager::IsInitialized()) {
        CConnectionManager::Initialize();
    }
    return 0;
}

// 0x800f8c _ZN9CMatching3GetEv
// Partial, no body. The port already has an inline CMatching::Get in deps/matching.hpp that returns the
// current instance. It does not construct CMatchingLocal, CMatchingBluetooth or CMatchingGLLive from
// s_matchingProvider as the original does (sizes 0xa9e8, 0xabb8 and 0x6c08 are recorded in the ledger).

// 0x8010a4 _ZN9CMatching15WritePacketDataEiiR12NetBitStream
// Partial, no body. Needs NetStruct::SetEnabled on the room attributes' NetStruct (+0x08), CRoomAttributes::Serialize
// with the bits argument, and NetStructArray<MemberInfoNetStruct,32>::Serialize.
