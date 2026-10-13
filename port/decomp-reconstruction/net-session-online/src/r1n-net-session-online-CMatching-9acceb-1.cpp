// Track B batch r1n-net-session-online-CMatching-9acceb-1 (owner CMatching, ported as CMatching).
// CMatching::WritePacketData had no body anywhere in this category. The other functions of the batch
// are completed in place in the files that already hold them (see the ledger rows for their paths).
#include "net-session-online/c_matching_base.hpp"

// 0x8010a4 _ZN9CMatching15WritePacketDataEiiR12NetBitStream
// Partial. Reproduced, in the original order:
//   - the room attributes are fetched through the virtual GetRoomAttributes() (vtable +0xa4);
//   - the static matching object is fetched with CMatching::Get() and its IsServer() flag is written to the
//     room attributes' NetStruct enable flag (NetStruct::SetEnabled(room + 8, IsServer), SetNetEnabled here).
// Not ported (their results are taken as 0, so the function returns 0):
//   - CRoomAttributes::Serialize(int, int, NetBitStream&) (0x817ebc). The port declares only the
//     Serialize(NetBitStream&, unsigned) overload and it has no body. The original's second
//     GetRoomAttributes() call is for this step, so it is not repeated here.
//   - NetStructArray<MemberInfoNetStruct,32>::Serialize on the member array at +0x18. The port's MemberInfo
//     is a plain struct, not a NetStruct element.
// The original returns the OR of the two results. The port returns 0 for both stubs.
// Deviations:
//   - Get() returns null for matching provider 2 (CMatchingBluetooth is not derived from CMatching in this
//     port). The original would then call IsServer() on the Bluetooth object. Here the flag reads false.
//   - The base GetRoomAttributes() returns null, so the enable flag is set only when a room exists. The
//     binary calls the slot unguarded, but only derived classes, which return a room, reach this body.
std::int32_t CMatching::WritePacketData(std::int32_t a2, std::int32_t a3, NetBitStream& stream) {
    (void)a2;
    (void)a3;
    (void)stream;
    CMatching* matching = Get();                                // CMatching::Get() (static instance)
    const bool isServer = matching != nullptr && matching->IsServer();
    if (CRoomAttributes* room = GetRoomAttributes()) {           // vtable +0xa4
        room->SetNetEnabled(isServer);                           // NetStruct::SetEnabled(room + 8, IsServer)
    }
    // Not ported: CRoomAttributes::Serialize(a2, a3, stream) and the member array's Serialize (see above).
    return 0;
}
