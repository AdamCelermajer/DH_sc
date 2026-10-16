// Track B batch r1n-net-session-online-CMatching-9acceb-1 (owner CMatching, ported as CMatching).
// CMatching::WritePacketData had no body anywhere in this category. The other functions of the batch
// are completed in place in the files that already hold them (see the ledger rows for their paths).
//
// CMatching::ReadPacketData (0x7fea14) is defined here too, next to its two callers' shared helpers.
#include "net-session-online/c_matching_base.hpp"

// NetStructArray<CMatching::MemberInfoNetStruct,32>::Load (0x7fe964): the presence bit first, then the elements
// until one reports that it loaded.
bool CMatching::LoadMemberArray(NetBitStream& stream, std::int32_t a2, std::int32_t a3) {
    if (stream.ReadBit() == 0) {
        return false;
    }
    bool loaded = false;
    for (std::int32_t i = 0; i < memberCount_ && !loaded; ++i) {
        loaded = members_[i].NetStruct::Load(stream, a2, static_cast<std::uint32_t>(a3)) != 0;
    }
    return loaded;
}

// NetStructArray<CMatching::MemberInfoNetStruct,32>::Serialize (0x7fe894): the presence bit under the ARRAY's
// HasDataToSend(a2) (vtable slot 0, 0x7fe7e8: true when ANY element's HasDataToSend (slot +0x28) is true), then
// the elements until one reports data. (An earlier version gated on element 0 only; corrected in the VF2 audit.)
bool CMatching::SerializeMemberArray(NetBitStream& stream, std::int32_t a2, std::int32_t a3) {
    bool any = false;
    for (std::int32_t i = 0; i < memberCount_ && !any; ++i) {
        any = members_[i].NetStruct::HasDataToSend(a2) != 0;
    }
    bool wrote = false;
    if (any) {
        stream.WriteBit(1);
        for (std::int32_t i = 0; i < memberCount_ && !wrote; ++i) {
            wrote = members_[i].NetStruct::Serialize(stream, a2, a3) != 0;
        }
    } else {
        stream.WriteBit(0);
    }
    return wrote;
}

// 0x7fea14 _ZN9CMatching14ReadPacketDataEiiR12NetBitStream
// Reconstructed from the 0x7fea14 pseudocode:
//   `v8 = (*vt+0xa4)(this); CRoomAttributes::Unserialize(v8, a2, stream);
//    return NetStructArray<CMatching::MemberInfoNetStruct,32>::Load(this + 24, stream, a2, a3);`
// Deviation: GetRoomAttributes() is null-checked, where the binary calls the +0xa4 slot unguarded (only
// derived classes, which return a real room, reach this body).
std::int32_t CMatching::ReadPacketData(std::int32_t a2, std::int32_t a3, NetBitStream& stream) {
    CRoomAttributes* room = GetRoomAttributes();  // vtable +0xa4
    if (room != nullptr) {
        room->Unserialize(a2, stream);  // 0x817fec: NetStruct::ConditionalLoad with the member id
    }
    return LoadMemberArray(stream, a2, a3) ? 1 : 0;
}

// 0x8010a4 _ZN9CMatching15WritePacketDataEiiR12NetBitStream
// Reconstructed from the 0x8010a4 pseudocode and re-verified against it this pass:
//   `v8 = (*vt+0xa4)(this);                      // the room attributes
//    v9 = CMatching::Get(); IsServer = CMatching::IsServer(v9);
//    NetStruct::SetEnabled(v8 + 8, IsServer);    // the room's NetRoomAttributes block (CRoomAttributes+0x08)
//    v11 = (*vt+0xa4)(this); LOBYTE(v8) = CRoomAttributes::Serialize(v11, a2, a3, stream);
//    return NetStructArray<MemberInfoNetStruct,32>::Serialize(this + 24, stream, a2, a3) | LOBYTE(v8);`
// The enable flag takes IsServer() of the STATIC matching object (CMatching::Get()), not of this, and it is
// written BEFORE the room serializes -- NetStruct::SerializeInternal reads enabled_ on its first line, so
// the order is observable and is the binary's.
// The member array's Serialize writes its own presence bit and then the elements' Serialize through
// NetStructArray's rule (stop at the first element that reports data). Both sub-serialize results are OR-ed
// into the return value, as the binary's `| LOBYTE(v8)` does.
// Deviations:
//   - Get() returns null for matching provider 2 (CMatchingBluetooth is not derived from CMatching in this
//     port). The original would then call IsServer() on the Bluetooth object. Here the flag reads false.
//   - the +0xa4 slot is null-checked (as in ReadPacketData above); the binary calls it unguarded, which
//     only derived classes that return a real room ever reach.
// Mutation observed by tests/r1n-net-session-online-CMatching-9acceb-1_test.cpp: with the static instance a
// server, the room's NetStruct ends up enabled; with a client, disabled.
std::int32_t CMatching::WritePacketData(std::int32_t a2, std::int32_t a3, NetBitStream& stream) {
    CMatching* matching = Get();  // CMatching::Get() (the static instance)
    const bool isServer = matching != nullptr && matching->IsServer();
    CRoomAttributes* room = GetRoomAttributes();  // vtable +0xa4
    if (room == nullptr) {
        return 0;
    }
    // NetStruct::SetEnabled on the room's replicated block (CRoomAttributes+0x08 / NetRoomAttributes), with
    // the static instance's IsServer() as the argument.
    room->m_netRoomAttributes.SetEnabled(isServer);
    // CRoomAttributes::Serialize(memberId, packetId, stream) is the game-world body (0x817ebc); it
    // forwards to the room's NetStruct::Serialize and returns `NetStruct::Serialize(...) != 0`.
    const bool roomWrote = room->Serialize(a2, a3, stream);

    const bool wrote = SerializeMemberArray(stream, a2, a3);  // NetStructArray<...>::Serialize (0x7fe894)
    return (roomWrote ? 1 : 0) | (wrote ? 1 : 0);
}
