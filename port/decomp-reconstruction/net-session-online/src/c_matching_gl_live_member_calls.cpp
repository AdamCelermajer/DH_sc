#include "net-session-online/c_matching_gl_live.hpp"

#include <cstddef>
#include <cstring>
#include <string>

#include "net-session-online/c_matching_gl_live_observers.hpp"
#include "net-session-online/c_sign_in.hpp"
#include "net-session-online/deps/net_bit_stream.hpp"
#include "net-session-online/glx_player_component.hpp"
#include "net-session-online/glx_player_login.hpp"
#include "net-session-online/glx_player_message.hpp"
#include "net-session-online/glx_player_mp_lobby.hpp"
#include "net-session-online/glx_player_user.hpp"
#include "net-session-online/glx_player_user_friend.hpp"

namespace {

// Text of the game invitation sent by SendGLFriendsGameInvite (decompiled literal).
constexpr char kInviteTitle[] = "title";
constexpr char kInviteText[] = "Lets play Dungeon Hunter 2 !";
// Largest user parameter accepted by SetPlayerParam (0x80 bytes, decompiled bound).
constexpr std::size_t kMaxPlayerParamBytes = 0x80;

}  // namespace

// 0x81c138 _ZN15CMatchingGLLive18GetGLFriendsNumberEi
// Returns the user number of friend `index` when 0 <= index < the friend count, else 0. A null
// friend list returns 0 here; the original would dereference it.
std::int32_t CMatchingGLLive::GetGLFriendsNumber(std::int32_t index) {
    if (glxUserFriend_ == nullptr || index < 0 || index >= glxUserFriend_->GetFriendsCount()) {
        return 0;
    }
    return static_cast<std::int32_t>(glxUserFriend_->GetUserNumber(index));
}

// 0x81c174 _ZN15CMatchingGLLive23SendRequestLostPasswordEPc
// Forwards the name to the GLXPlayerUser object (+0x6718) and returns 1. A null user object skips
// the request here; the original would call through it.
std::int32_t CMatchingGLLive::SendRequestLostPassword(char* name) {
    if (glxUser_ != nullptr) {
        glxUser_->sendRetrievalPassword(name);
    }
    return 1;
}

// 0x81c748 _ZN15CMatchingGLLive16InitializeFriendEv
// Partial: copies the login UID into the friend list object, requests the friend list (kind 0x3d,
// first 0, count 0) and sets the byte at +0x67e9. The original returns the request's result; the
// port's sendGetUserFriends returns void, so this returns 0. A null friend list skips the copy and the
// request here.
std::int32_t CMatchingGLLive::InitializeFriend() {
    if (glxUserFriend_ != nullptr) {
        glxUserFriend_->SetUID(GetGLXPlayerLogin()->GetUID());
        glxUserFriend_->sendGetUserFriends(0, 0);
    }
    glFriendsRequested_ = 1;
    return 0;
}

// 0x81c78c _ZN15CMatchingGLLive23SendGLFriendsGameInviteEPc
// Partial: sends the invitation (title "title", text "Lets play Dungeon Hunter 2 !", type 8) to the
// name through the GLXPlayerMessage object (+0x6708) and returns its result. The original then sets
// the invited flag in the matching GL friend list entry; that list is GLFriendInfo, which the port does
// not have, so the loop is not reproduced. A null message object returns 0.
std::int32_t CMatchingGLLive::SendGLFriendsGameInvite(char* name) {
    if (glxMessage_ == nullptr) {
        return 0;
    }
    char title[sizeof(kInviteTitle)];
    char text[sizeof(kInviteText)];
    std::memcpy(title, kInviteTitle, sizeof(title));
    std::memcpy(text, kInviteText, sizeof(text));
    return glxMessage_->SendOnlineMessage(name, 8, title, text, false);
}

// 0x81dea0 _ZNK15CMatchingGLLive11GetMemberIdEv
// The local member id while the game runs; otherwise the id found in the GLLive member list.
std::int32_t CMatchingGLLive::GetMemberId() const {
    if (s_IsGameStarting) {
        return memberId_;  // +0x3640
    }
    return GetGLLiveMemberId();
}

// 0x81dd04 _ZNK15CMatchingGLLive17GetGLLiveMemberIdEv
// Partial: returns -1 when the Game Center flag is set or the user is not signed in, as the original
// does. The original then searches the lobby member list (tMemberInfo, not in the port) for the signed-in
// user name, ignoring case. That search is not reproduced, so a signed-in lookup returns -1.
std::int32_t CMatchingGLLive::GetGLLiveMemberId() const {
    if (gameCenterActive_) {
        return -1;
    }
    CSignIn* signIn = CSignIn::Get();
    if (signIn == nullptr || !signIn->LoggedInForTest()) {
        return -1;
    }
    return -1;
}

// 0x81f04c _ZN15CMatchingGLLive14SetPlayerParamESsb
// Partial: stores the text in playerParam_ (+0x67f0) when it is at most 0x80 bytes. With the flag set,
// the text is sent to the lobby as a user parameter, with its length as the key, which is how the
// original calls mpSendSetUserParameter. The original also calls the lobby observer's SetPlayerParam,
// which is not declared in the port, so that call is skipped. A missing lobby skips the send here.
void CMatchingGLLive::SetPlayerParam(std::string value, bool sendToLobby) {
    if (value.size() > kMaxPlayerParamBytes) {
        return;
    }
    playerParam_ = value;
    if (!sendToLobby) {
        return;
    }
    GLXPlayerMPLobby* lobby = GetGLXPlayerMPLobby();
    if (lobby == nullptr) {
        return;
    }
    std::string copy = value;
    lobby->mpSendSetUserParameter(copy.data(), static_cast<std::int32_t>(copy.size()));
}
