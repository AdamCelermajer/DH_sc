// Track B batch e1-net-session-online-CMatching-0.
// The original class CMatching is ported as CMatching (c_matching_base.hpp), the same convention
// c_matching_base.cpp uses. Functions whose callees or layout are not in this category are listed
// with no body; the ledger notes name what is missing.

#include "net-session-online/c_matching_base.hpp"
#include "net-session-online/c_messaging.hpp"    // CMessaging::GetInstance, ResetQueues
#include "net-session-online/c_sign_in.hpp"      // CSignIn::Get, Destroy
#include "net-transport/connection_manager.hpp"  // CConnectionManager::Initialize
#include "net-transport/tb/c_net_player_manager.hpp"  // CNetPlayerManager::GetInstance, ResetPlayers

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
// Partial, no body (the port's destructor is the implicit `virtual ~CMatching() = default`). Binary body,
// from the Hex-Rays pseudocode: the vptr is set to the base table, the container at +0x3620 (begin pointer,
// end pointer at +0x3628) is freed through the node allocator when at most 0x80 bytes, else CustomFree, then
// the vptr at +0x18 is set and the 32 member elements are destroyed from element 31 down to 0 through their
// vtable slot 0 (NetStruct destructor). Not ported: the container free (its element type is not identified in
// the port) and the element NetStruct destructors (MemberInfo is a plain struct, so the port destroys its
// vector members implicitly).
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
// Partial. When s_Matching is set: Terminate() on it (vtable +0x2c, its result is discarded by the
// binary), CSignIn::Get()->Destroy() (CSignIn vtable +0x24), then delete s_Matching through its deleting
// destructor (vtable +0x24) and clear the pointer. The binary's Destroy takes this and never uses it, so it
// is non-static here. Partial: the CSignIn pointer is null-checked; the original dereferences it.
void CMatching::Destroy() {
    if (s_Matching == nullptr) return;
    s_Matching->Terminate();
    if (CSignIn* signIn = CSignIn::Get()) signIn->Destroy();
    if (s_Matching != nullptr) {
        delete s_Matching;
        s_Matching = nullptr;
    }
}

// 0x7fed08 _ZN9CMatching25ProcessAcknowledgedPacketEii
// Body in src/c_matching_base.cpp (room attributes call only; partial there). Not repeated here.

// 0x7ff120 _ZN9CMatching8JoinRoomEy
// Partial. Reconstructed: CMessaging::Get() (a thunk to GetInstance) then ResetQueues(true), GetNetPlayerMgr()
// (CNetPlayerManager::GetInstance) then ResetPlayers, the per-member reset (member id -1, team 0, attribute
// buffer cleared, previous id -1), and the virtual JoinRoomInternal (vtable +0x00) with the room id.
// Not ported: the NetStructMember change notifications on the id and team objects, and NetStruct::SetEnabled(
// element, 0) on each element (the port's MemberInfo has no NetStruct base). The Hex-Rays stack value passed to
// the id Set slot is undefined, so only the stored result is reproduced.
std::int32_t CMatching::JoinRoom(std::uint64_t roomId) {
    CMessaging::GetInstance()->ResetQueues(true);
    CNetPlayerManager::GetInstance()->ResetPlayers();
    for (std::int32_t i = 0; i < memberCount_; ++i) {
        MemberInfo& member = members_[i];
        member.memberId = -1;          // element +0x150
        member.teamId = 0;             // element +0x178
        member.roomAttribute.clear();  // element +0x180
        member.previousMemberId = -1;  // element +0x1a8
    }
    return JoinRoomInternal(roomId);  // vtable +0x00
}

// 0x7ff350 _ZN9CMatching14CreateJoinRoomEb15CRoomAttributes
// Partial. Reconstructed: the same messaging and player resets and member reset as JoinRoom, then
// SetAttributeInt(0, -1342128411) on the attributes (0xB000BEE5 as the 32-bit value), then the virtual
// CreateJoinRoomInternal (vtable +0x04). Not ported: the NetStruct::SetEnabled(element, 1) loop that the
// binary runs after the attribute write. The binary's attributes argument is a copy, so the port takes it by
// value.
std::int32_t CMatching::CreateJoinRoom(bool flag, CRoomAttributes attributes) {
    CMessaging::GetInstance()->ResetQueues(true);
    CNetPlayerManager::GetInstance()->ResetPlayers();
    for (std::int32_t i = 0; i < memberCount_; ++i) {
        MemberInfo& member = members_[i];
        member.memberId = -1;          // element +0x150
        member.teamId = 0;             // element +0x178
        member.roomAttribute.clear();  // element +0x180
        member.previousMemberId = -1;  // element +0x1a8
    }
    attributes.SetAttributeInt(0, -1342128411);
    return CreateJoinRoomInternal(flag, attributes);  // vtable +0x04
}

// 0x7ff5a0 _ZN9CMatching9TerminateEv
// Partial, no body. Needs CPacketManager::UnregisterPacketSlot and Terminate, CNetPlayerManager::Terminate,
// CMessaging::Terminate, CConnectionManager::Terminate, CSignIn::Get and its vtable +0x20, the virtual
// slots +0x3c, +0x64 and +0xa4 of CMatching and slot +0x10 of the room attributes (names not identified),
// and the AddEvent code 4194306.

// 0x7ff85c _ZN9CMatching10InitializeEi
// Partial. Reproduced in the original order: when the matching object is already initialized (+0x0c) it posts
// 0x400001 on the output queue (CMatching::m_eventQueue) and returns 0. Otherwise it stores the capacity (+0x08),
// clears the room hidden (+0x0d) and room closed (+0x0e) flags, clears both matching queues, calls
// CSignIn::Get()->Initialize() (the CSignIn vtable +0x08 slot, which sets the sign-in active flag), calls
// CConnectionManager::Initialize() unconditionally, and resets every member element (id -1, team 0, attribute
// buffer empty, previous id -1). The original returns 0 on both paths.
// Not reproduced: CPacketManager::RegisterPacketSlot with the four static forwarders, the room attributes' calls
// (GetRoomAttributes() at vtable +0xa4, then the result's vtable +0x10, not identified) that the binary makes
// between the connection manager init and the member reset, the NetStructMember change notifications of the
// element resets, and NetStruct::SetEnabled(element, false) on every element.
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
    // Unchecked, as in the binary: Get() returns null only for a provider other than 0 or 1.
    CSignIn::Get()->Initialize();  // CSignIn vtable +0x08: sets the active flag
    CConnectionManager::Initialize();  // unconditional
    for (std::int32_t i = 0; i < memberCount_; ++i) {
        MemberInfo& member = members_[i];
        member.memberId = -1;          // element +0x150
        member.teamId = 0;             // element +0x178
        member.roomAttribute.clear();  // element +0x180
        member.previousMemberId = -1;  // element +0x1a8
    }
    return 0;
}

// 0x800f8c _ZN9CMatching3GetEv: the body is in src/glx_320e98.cpp (the one definition of CMatching::Get).
// 0x8010a4 _ZN9CMatching15WritePacketDataEiiR12NetBitStream: the body is in
// src/r1n-net-session-online-CMatching-9acceb-1.cpp (partial: the two serialize steps are stubs).
