// Track B batch r1n-net-session-online-CMatching-9acceb-1 (owner CMatching, ported as CMatching).
// CMatching::WritePacketData had no body anywhere in this category. The other functions of the batch
// are completed in place in the files that already hold them (see the ledger rows for their paths).
//
// CMatching::ReadPacketData (0x7fea14) is defined here too, next to its two callers' shared helpers.
#include "net-session-online/c_matching_base.hpp"

// 0x7fea14 _ZN9CMatching14ReadPacketDataEiiR12NetBitStream
// Reconstructed from the 0x7fea14 pseudocode:
//   `v8 = (*vt+0xa4)(this); CRoomAttributes::Unserialize(v8, a2, stream);
//    return NetStructArray<CMatching::MemberInfoNetStruct,32>::Load(this + 24, stream, a2, a3);`
// The `this + 24` is the element array's base (element 0's NetStruct subobject at CMatching +0x20); the
// port passes element 0 and walks the 32 elements itself, because its member table is the plain array
// described in the header rather than net-transport's NetStructArray header. The binary reads only the
// header's live count (memberCount_), so the walk stops there too. The return is Load's result.
// Deviation: GetRoomAttributes() is null-checked, where the binary calls the +0xa4 slot unguarded (only
// derived classes, which return a real room, reach this body).
std::int32_t CMatching::ReadPacketData(std::int32_t a2, std::int32_t a3, NetBitStream& stream) {
    CRoomAttributes* room = GetRoomAttributes();  // vtable +0xa4
    if (room != nullptr) {
        room->Unserialize(a2, stream);  // 0x817fec: NetStruct::ConditionalLoad with the member id
    }
    // NetStructArray<MemberInfoNetStruct,32>::Load (0x7fe964): the presence bit first, then the elements
    // until one reports that it loaded.
    if (stream.ReadBit() == 0) {
        return 0;
    }
    bool loaded = false;
    for (std::int32_t i = 0; i < memberCount_ && !loaded; ++i) {
        loaded = members_[i].NetStruct::Load(stream, a2, static_cast<std::uint32_t>(a3)) != 0;
    }
    return loaded ? 1 : 0;
}

// 0x8010a4 _ZN9CMatching15WritePacketDataEiiR12NetBitStream
// Reconstructed from the 0x8010a4 pseudocode:
//   `v8 = (*vt+0xa4)(this);                      // the room attributes
//    v9 = CMatching::Get(); NetStruct::SetEnabled(room + 8, CMatching::IsServer(v9));
//    v11 = (*vt+0xa4)(this); LOBYTE(v8) = CRoomAttributes::Serialize(v11, a2, a3, stream);
//    return NetStructArray<MemberInfoNetStruct,32>::Serialize(this + 24, stream, a2, a3) | LOBYTE(v8);`
// The enable flag takes IsServer() of the STATIC matching object (CMatching::Get()), not of this.
// The member array's Serialize writes its own presence bit and then the elements' Serialize through
// NetStructArray's rule (stop at the first element that reports data).
// Deviations:
//   - Get() returns null for matching provider 2 (CMatchingBluetooth is not derived from CMatching in this
//     port). The original would then call IsServer() on the Bluetooth object. Here the flag reads false.
//   - the +0xa4 slot is null-checked (as in ReadPacketData above).
std::int32_t CMatching::WritePacketData(std::int32_t a2, std::int32_t a3, NetBitStream& stream) {
    CMatching* matching = Get();  // CMatching::Get() (the static instance)
    const bool isServer = matching != nullptr && matching->IsServer();
    CRoomAttributes* room = GetRoomAttributes();  // vtable +0xa4
    if (room == nullptr) {
        return 0;
    }
    // CRoomAttributes::Serialize(memberId, packetId, stream) is the game-world body (0x817ebc); it
    // forwards to the room's NetStruct::Serialize. The room enable flag is set on the NETSTRUCT of the
    // room (the block at room + 8), which is what the pseudocode's `v8 + 8` names.
    room->Serialize(a2, a3, stream);

    // NetStructArray<MemberInfoNetStruct,32>::Serialize (0x7fe894): the presence bit under
    // HasDataToSend(a2), then the elements until one reports data.
    bool wrote = false;
    if (members_[0].NetStruct::HasDataToSend(a2)) {
        stream.WriteBit(1);
        for (std::int32_t i = 0; i < memberCount_ && !wrote; ++i) {
            wrote = members_[i].NetStruct::Serialize(stream, a2, a3) != 0;
        }
    } else {
        stream.WriteBit(0);
    }
    return wrote ? 1 : 0;
}
