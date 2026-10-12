// CMatching member and room layer (base of CMatchingLocal, CMatchingGLLive, CMatchingBluetooth).
// The class is ported as CMatchingBase (see c_matching_base.hpp); the demangled owner in each
// comment is the original CMatching.
#include "net-session-online/c_matching_base.hpp"

#include <cstring>

#include "net-session-online/deps/matching_queue.hpp"

// 0x7fed84 _ZN9CMatching17ProcessLostPacketEii
// Partial: the room attributes receive the call. The original then calls the NetStruct slot +0x44 of
// every member element (0..memberCount-1) with the same two arguments; that is not ported.
void CMatchingBase::ProcessLostPacket(int a2, int a3) {
    if (CRoomAttributes* room = GetRoomAttributes()) room->ProcessLostPacket(a2, a3);
}

// 0x7ff008 _ZN9CMatching21SetRoomMemberAttibuteEiPKvi
// Partial: the bytes are copied (NetStructByteArray<64>::SetBuffer copies into a ByteArray and then
// calls the member's Set slot, which is not ported). A negative size is not verified and is treated
// as empty.
void CMatchingBase::SetRoomMemberAttibute(int memberId, const void* data, int size) {
    const std::int32_t index = GetMemberIndex(memberId);
    if (index < 0 || index >= memberCount_) return;
    auto& buffer = members_[index].roomAttribute;  // element +0x180
    if (data == nullptr || size <= 0) {
        buffer.clear();
        return;
    }
    const auto* bytes = static_cast<const std::uint8_t*>(data);
    buffer.assign(bytes, bytes + size);
}

// 0x7ff054 _ZN9CMatching21GetRoomMemberAttibuteEiPvi
// The fourth argument is not read by the body. The stored size is copied and returned.
int CMatchingBase::GetRoomMemberAttibute(int memberId, void* out, int /*outSize*/) {
    const std::int32_t index = GetMemberIndex(memberId);
    if (index < 0 || index >= memberCount_) return 0;
    const auto& buffer = members_[index].roomAttribute;  // element +0x180
    if (buffer.empty()) return 0;
    std::memcpy(out, buffer.data(), buffer.size());
    return static_cast<int>(buffer.size());
}

// 0x8000dc _ZN9CMatching11GetRoomMaskEv
// The cache is a function-local static in the original, so it is shared by every instance. Kept so.
std::uint32_t CMatchingBase::GetRoomMask() {
    static std::uint32_t maskCache = 0;  // CMatching::GetRoomMask()::maskCache
    if (!roomMaskDirty_) return maskCache;

    std::uint32_t mask = MemberBit(ServerMemberId());
    if (IsServer()) {
        for (int memberId : ConnectedMemberIds()) mask |= MemberBit(memberId);
    } else {
        mask |= MemberBit(LocalMemberId());
    }
    maskCache = mask;
    roomMaskDirty_ = false;
    return mask;
}

// 0x8001d4 _ZN9CMatching12TestRoomMaskEj
bool CMatchingBase::TestRoomMask(std::uint32_t bits) {
    const std::uint32_t mask = GetRoomMask();
    return (mask & bits) == mask;
}

// 0x8002bc _ZN9CMatching18UpdateMemberIdListEv
// Partial: the stored values and the queued events follow the body. Not ported: the NetStruct change
// notifications on each member field (NetStructMember::SetChanged, and the Set slots that the body
// calls with the value). The original also compares the new id with a stack slot that is not
// identified; the port always stores the new value.
void CMatchingBase::UpdateMemberIdList() {
    if (!UnknownGate()) return;

    if (IsServer() && UnknownGate()) {
        // Server: connected members that have no slot get one (id, team 0, empty attribute buffer).
        std::vector<int> candidates = ConnectionMemberIdList();
        candidates.push_back(LocalMemberId());
        for (int memberId : candidates) {
            const std::int32_t index = GetMemberIndex(memberId);
            if (index < 0 || index >= memberCount_) continue;
            MemberInfo& member = members_[index];
            if (member.memberId >= 0) continue;  // element +0x150 already holds an id
            if (IsMemberServer(memberId) || IsConnectedMember(memberId)) {
                member.memberId = memberId;     // member id object +0x130
                member.teamId = 0;              // team object +0x158
                member.roomAttribute.clear();   // attribute buffer +0x180
            }
        }

        // Server: members that are no longer connected lose their id, team and attribute buffer.
        for (std::int32_t i = 0; i < memberCount_; ++i) {
            MemberInfo& member = members_[i];
            if (member.memberId < 0) continue;
            if (IsMemberServer(member.memberId) || IsConnectedMember(member.memberId)) continue;
            member.memberId = -1;
            member.teamId = 0;
            member.roomAttribute.clear();
        }
    }

    // Every member: queue the change of the id against the previous id, then remember the new id.
    // Event 0x800007 carries the previous id (queued when the id is now empty).
    // Event 0x800006 carries the current id (queued when the id is set, or when the previous was empty).
    // The names of these event codes are not identified.
    for (std::int32_t i = 0; i < memberCount_; ++i) {
        MemberInfo& member = members_[i];
        if (member.previousMemberId == member.memberId) continue;

        if (member.memberId >= 0) {
            if (member.previousMemberId < 0) {
                int id = member.memberId;
                MatchingInternalEventQueue().AddEvent(0x800006, &id, 4);
            }
        } else {
            int previous = member.previousMemberId;
            MatchingInternalEventQueue().AddEvent(0x800007, &previous, 4);
            if (member.previousMemberId < 0) {
                int id = member.memberId;
                MatchingInternalEventQueue().AddEvent(0x800006, &id, 4);
            }
        }
        member.previousMemberId = member.memberId;
    }
}

// 0x800808 _ZN9CMatching15GetMemberIdListEv
// Returns the ids (element +0x150) of the members that hold one, in slot order.
std::vector<int> CMatchingBase::GetMemberIdList() const {
    std::vector<int> ids;
    for (std::int32_t i = 0; i < memberCount_; ++i) {
        const int memberId = members_[i].memberId;
        if (memberId >= 0) ids.push_back(memberId);
    }
    return ids;
}

// 0x8008ac _ZN9CMatching9SetTeamIdEih
// Partial: the team value is stored (team object +0x158, value +0x178). The NetStruct change
// notification is not ported.
void CMatchingBase::SetTeamId(int memberId, std::uint8_t teamId) {
    const std::int32_t index = GetMemberIndex(memberId);
    if (index < 0 || index >= memberCount_) return;
    members_[index].teamId = static_cast<std::int32_t>(teamId);
}
