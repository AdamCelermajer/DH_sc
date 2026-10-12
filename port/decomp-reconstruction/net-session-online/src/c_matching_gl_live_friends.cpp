#include "net-session-online/c_matching_gl_live.hpp"

#include "net-session-online/glx_player_user_friend.hpp"

// 0x81bc54 _ZN15CMatchingGLLive16GetTransportTypeE14tPROTOCOL_TYPE
// The table is CSWTCH.901 in the binary (DCD 1, 2, 1, 0; see ida-asm/asm_chunk_018.asm).
std::int32_t CMatchingGLLive::GetTransportType(std::uint32_t protocolType) {
    static const std::int32_t kTransportTypes[3] = {1, 2, 1};
    if (protocolType > 2) {
        return 0;
    }
    return kTransportTypes[protocolType];
}

// 0x81c098 _ZN15CMatchingGLLive15GetGLFriendsCountEv
// Deviation: the original calls GetFriendsCount on the friend pointer without a null check
// (the decompiled result is the pointer itself, which is 0 when the list is null). This port
// returns 0 in that case.
std::int32_t CMatchingGLLive::GetGLFriendsCount() const {
    if (glxUserFriend_ == nullptr) {
        return 0;
    }
    return glxUserFriend_->GetFriendsCount();
}

// 0x81c0ac _ZN15CMatchingGLLive15GetGLFriendStateEi
// Deviation: a null list gives -1 (the original would call through a null pointer).
std::int32_t CMatchingGLLive::GetGLFriendState(std::int32_t index) const {
    if (index >= 0 && index < GetGLFriendsCount()) {
        return glxUserFriend_->GetState(index);
    }
    return -1;
}

// 0x81c0e8 _ZN15CMatchingGLLive17SendGLFriendInviteEPc
// Partial: sendAddUserFriend returns void in glx_player_user_friend.hpp, so the original's
// return value is not produced. A null list sends nothing.
std::int32_t CMatchingGLLive::SendGLFriendInvite(const char* name) const {
    if (glxUserFriend_ != nullptr) {
        glxUserFriend_->sendAddUserFriend(name, nullptr, false);
    }
    return 0;
}

// 0x81c0fc _ZN15CMatchingGLLive15GetGLFriendsNameEi
// Deviation: a null list gives nullptr (the original would call through a null pointer).
const char* CMatchingGLLive::GetGLFriendsName(std::int32_t index) const {
    if (index >= 0 && index < GetGLFriendsCount()) {
        return glxUserFriend_->GetName(index);
    }
    return nullptr;
}
