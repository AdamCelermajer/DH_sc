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
// Asm/Hex-Rays: when the argument is true the binary calls the CMatching virtual at byte offset +0x48 and
// passes its result back; the +0x0e base room-closed byte is set unconditionally afterwards; the return is
// the slot result when the argument was true, else the object pointer.
// Reproduced: the unconditional +0x0e store and, this pass, the else-branch return value (`this`; the binary
// returns that same address in r0, so the port's `this` is the identical value).
// Still partial, concrete missing item: the +0x48 call. That slot is CloseRoom in every derived vtable
// (CMatchingLocal 0x806e4c, CMatchingBluetooth the same, CMatchingGLLive 0x81bc1c), and CloseRoom (0x806e4c)
// is NOT reachable there: in this port it is a non-virtual CMatchingLocal method and the base's virtual
// declarations are in a different order from the binary's vtable (the port's slot +0x48 is a different
// function). So the local close -- roomHidden_ = IsRoomHidden(), the room-closed flag, the hidden flag and
// internal event 0x80000a -- does not run. Fixing that needs the CMatching vtable order corrected, which is a
// category-level change, not a local one.
int CMatching::LockRoom(bool callSlot) {
    baseRoomClosed_ = true;  // +0x0e
    if (callSlot) {
        return 0;  // the +0x48 CloseRoom call is not ported (see above); its reconstituted result is 0
    }
    return static_cast<int>(reinterpret_cast<std::uintptr_t>(this));
}

// 0x7fe5e0 _ZN9CMatching9GetTeamIdEi
// Reconstructed. The team value is the byte held by the element's team member (value at element
// +0x178; a NetStructUInt<8>, so it is an unsigned byte). 0 when the index is outside
// 0..memberCount_.
int CMatching::GetTeamId(int memberId) const {
    const std::int32_t index = GetMemberIndex(memberId);
    if (index < 0 || index >= memberCount_) return 0;
    return members_[index].team();  // element +0x178
}

// 0x7fe618 _ZN9CMatching18GetMemberIdByIndexEj
// Asm/Hex-Rays: `return *((_DWORD*)this + 108*a2 + 92)` -- the member id of element a2 with NO range check
// (108 dwords = 432 bytes = the element stride; 92 dwords = 0x170 = element base 0x20 + the id value at
// +0x150). Callers pass the member count as the bound, so an index of 32 reads 432 bytes past element 31,
// i.e. past the whole 32-slot table. That read has no defensible reconstructed value: the function's own
// contract is "the id of element index", and there is no element 32.
// Host-necessitated deviation, named here because the test pins it: an index at or past the 32 slots
// returns -1, which is exactly the id an empty element holds (JoinRoom writes -1 into every element's id
// member, and GetMemberIdList skips ids < 0). So the answer for an out-of-range index is the same as the
// answer for an in-range empty slot -- the value the binary would read for an untouched slot -- and it can
// never collide with a real member id (0 would alias the id of a real member 0). No production caller
// reaches this: CMatchingGLLive::GetMemberIdList walks 0..count-1 and all other call sites test the range.
std::int32_t CMatching::GetMemberIdByIndex(std::uint32_t index) const {
    if (index >= static_cast<std::uint32_t>(kMaxMembers)) return -1;  // out of the table: the empty-slot id
    return members_[index].id();  // element +0x150
}

// 0x7fff9c _ZN9CMatchingC1Ev and 0x80003c _ZN9CMatchingC2Ev
// Partial. Both bodies are the same. Reproduced after the re-derivation against the 0x80003c pseudocode:
// roomMaskDirty_ = 1 (+0x10), the NetStructArray header's vptr word (+0x18; the port has no slot for it),
// memberCount_ = 32 (+0x1c = the array's count), memberCapacity_ = 0 (+0x08), the four flags at
// +0x0c..+0x0f cleared, the 32 member elements constructed in place (element base +0x20, stride 432;
// the real CMatching::MemberInfoNetStruct constructor runs for each), and the three words at
// +0x3620..+0x3628 cleared. NOT ported: the NetStructArray vptr at +0x18 (the port keeps the count as a
// plain int, see the header) and the node-allocated container at +0x3620 (its element type is not
// identified; the binary's destructor frees it). The old note said "element NetStruct constructors" were
// not ported -- they are now: the element type is a real NetStruct record.
CMatching::CMatching() {
    memberCapacity_ = 0;       // +0x08
    initialized_ = false;      // +0x0c
    roomHidden_ = false;       // +0x0d
    baseRoomClosed_ = false;   // +0x0e
    unk_0f = 0;                // +0x0f
    roomMaskDirty_ = true;     // +0x10
    memberCount_ = kMaxMembers;  // +0x1c (the NetStructArray count)
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

// 0x7fea14 _ZN9CMatching14ReadPacketDataEiiR12NetBitStream: the body is in
// src/r1n-net-session-online-CMatching-9acceb-1.cpp (reconstructed against 0x7fea14).

// 0x7feac4 _ZN9CMatching16ResendMemberDataEv: the body is in src/c_matching_base.cpp (reconstructed).

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
// Reconstructed from the 0x7ff120 pseudocode, including the parts the earlier note listed as missing.
// Documented order:
//   1. CMessaging::Get()->ResetQueues(true); GetNetPlayerMgr()->ResetPlayers().
//   2. while (memberCount_ > 0), for every element 0..memberCount_-1 (v7 walks with stride 432 while v6
//      counts, so the element base is this + 432*i + 0x20):
//        - a stack NetStructInt<32> temporary is built (sizeBits 32, stamp 0, ackMask/sentMask -1,
//          changeMask 0, changed false) and SetChanged is called on it when its value word is not -1.
//          That word is never written by the original, so this only advances NetStruct::s_changeCounter
//          -- the case this category has already settled (NOTES.md: SetCharacterXP/SetCharacterNumPotions).
//        - the element's id member (+0x130) gets SetValue(-1) through vtable +0x1c (the stack temporary
//          is the argument). The port's idMember_.SetValue(-1) does exactly that, and it fires the real
//          change notification when the value differs.
//        - the same for the team member (+0x158) with the 8-bit temporary and value 0.
//        - the attribute member (+0x180) gets SetBuffer(nullptr, 0).
//        - the cached id (element +0x1a8) is set to -1 without a NetStruct call.
//   3. Then, for every element, NetStruct::SetEnabled(element, 0) (the +0x20 address is the element
//      base: 432*i + 32).
//   4. Tail call to vtable +0x00 (JoinRoomInternal). Still the base default here: CMatchingLocal::
//      JoinRoomInternal (0x808774) is declared without `override`, so it holds no slot. That is
//      consequence 2 of the KNOWN DEFECT block in c_matching_base.hpp (scheduled, not this batch).
std::int32_t CMatching::JoinRoom(std::uint64_t roomId) {
    CMessaging::GetInstance()->ResetQueues(true);
    CNetPlayerManager::GetInstance()->ResetPlayers();
    for (std::int32_t i = 0; i < memberCount_; ++i) {
        MemberInfoNetStruct& element = members_[i];
        element.setId(-1);                        // id member SetValue, element +0x150
        element.setTeam(0);                       // team member SetValue, element +0x178
        element.setRoomAttribute(nullptr, 0);     // attribute member SetBuffer, element +0x1a0
        element.setPreviousMemberId(-1);          // element +0x1a8
    }
    for (std::int32_t i = 0; i < memberCount_; ++i) {
        members_[i].SetEnabled(false);            // NetStruct::SetEnabled(element, 0)
    }
    return JoinRoomInternal(roomId);  // vtable +0x00
}

// 0x7ff350 _ZN9CMatching14CreateJoinRoomEb15CRoomAttributes
// Reconstructed from the 0x7ff350 pseudocode. Same two resets and the same per-element reset as
// JoinRoom (id member SetValue(-1), team member SetValue(0), attribute member SetBuffer(nullptr, 0),
// cached id -1). Then CRoomAttributes::SetAttributeInt(0, -1342128411) on the by-value attributes
// (0xB000BEE5), then NetStruct::SetEnabled(element, 1) on every element (the +0x20 element base again),
// then the tail call to CreateJoinRoomInternal (vtable +0x04), which CMatchingLocal and CMatchingGLLive
// do carry with the binary's signature. The attributes argument is a copy, as the binary's is.
std::int32_t CMatching::CreateJoinRoom(bool flag, CRoomAttributes attributes) {
    CMessaging::GetInstance()->ResetQueues(true);
    CNetPlayerManager::GetInstance()->ResetPlayers();
    for (std::int32_t i = 0; i < memberCount_; ++i) {
        MemberInfoNetStruct& element = members_[i];
        element.setId(-1);                        // id member SetValue, element +0x150
        element.setTeam(0);                       // team member SetValue, element +0x178
        element.setRoomAttribute(nullptr, 0);     // attribute member SetBuffer, element +0x1a0
        element.setPreviousMemberId(-1);          // element +0x1a8
    }
    attributes.SetAttributeInt(ROOM_INT_ATTRIBUTE_0, -1342128411);
    for (std::int32_t i = 0; i < memberCount_; ++i) {
        members_[i].SetEnabled(true);             // NetStruct::SetEnabled(element, 1)
    }
    return CreateJoinRoomInternal(flag, attributes);  // vtable +0x04
}

// 0x7ff5a0 _ZN9CMatching9TerminateEv
// Partial, no body. Needs CPacketManager::UnregisterPacketSlot and Terminate, CNetPlayerManager::Terminate,
// CMessaging::Terminate, CConnectionManager::Terminate, CSignIn::Get and its vtable +0x20, the virtual
// slots +0x3c, +0x64 and +0xa4 of CMatching and slot +0x10 of the room attributes (names not identified),
// and the AddEvent code 4194306.

// 0x7ff85c _ZN9CMatching10InitializeEi
// Partial. Reproduced in the original order: when the matching object is already initialized (+0x0c) it posts
// 0x400001 on the output queue (CMatching::m_eventQueue) and returns 0. Otherwise it stores the capacity
// (+0x08), clears the room hidden (+0x0d) and room closed (+0x0e) flags, clears both matching queues, calls
// CSignIn::Get()->Initialize() (the CSignIn vtable +0x08 slot, which sets the sign-in active flag), calls
// CConnectionManager::Initialize() unconditionally, then resets every member element and calls
// NetStruct::SetEnabled(element, false) on each. The original returns 0 on both paths.
// Not reproduced: CPacketManager::RegisterPacketSlot with the four static forwarders, and the room
// attributes' calls (GetRoomAttributes() at vtable +0xa4, then the result's vtable +0x10, not identified)
// that the binary makes between the connection manager init and the member reset.
// Correction of the earlier note: it said the reset loop was a DO-WHILE whose index was the +0x0c byte it
// had just read; the assembly at 0x7ff908 shows `LDR R3,[R7,#0x1C]; CMP R3,#0; BLE` and a `v2 = 0` index,
// so the loop count is memberCount_ (+0x1c = the NetStructArray count, 32 from the constructor) and the
// loop is a plain for. What is still missing is the capacity store's effect on the count: the binary's
// `*((_DWORD *)this + 2) = a2` writes memberCapacity_ and the net-transport array constructor has already
// set count 32, so the loop walks 32 elements regardless of the argument -- the port does the same.
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
        MemberInfoNetStruct& element = members_[i];
        element.setId(-1);                        // id member SetValue, element +0x150
        element.setTeam(0);                       // team member SetValue, element +0x178
        element.setRoomAttribute(nullptr, 0);     // attribute member SetBuffer, element +0x1a0
        element.setPreviousMemberId(-1);          // element +0x1a8
    }
    for (std::int32_t i = 0; i < memberCount_; ++i) {
        members_[i].SetEnabled(false);            // NetStruct::SetEnabled(element, 0)
    }
    return 0;
}

// 0x800f8c _ZN9CMatching3GetEv: the body is in src/glx_320e98.cpp (the one definition of CMatching::Get).
// 0x8010a4 _ZN9CMatching15WritePacketDataEiiR12NetBitStream: the body is in
// src/r1n-net-session-online-CMatching-9acceb-1.cpp (partial: the two serialize steps are stubs).
