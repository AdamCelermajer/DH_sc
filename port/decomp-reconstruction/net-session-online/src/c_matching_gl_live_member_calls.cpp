#include "net-session-online/c_matching_gl_live.hpp"

#include <cstddef>
#include <cstring>
#include <string>
#include <utility>

#include "net-session-online/c_matching_gl_live_observers.hpp"
#include "net-session-online/c_sign_in.hpp"
#include "net-transport/net_bit_stream.hpp"
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
// Partial: the login singleton is fetched first (as in the original, where it is not guarded), then the login
// UID is copied into the friend list object, the friend list is requested (kind 0x3d, first 0, count 0) and the
// byte at +0x67e9 is set. Returns the request's result (the SendByGet result). A null friend list skips the copy and
// the request and returns 0 here (the original would fault).
std::int32_t CMatchingGLLive::InitializeFriend() {
    GLXPlayerLogin* login = GetGLXPlayerLogin();
    std::int32_t result = 0;
    if (glxUserFriend_ != nullptr) {
        glxUserFriend_->SetUID(login->GetUID());
        result = glxUserFriend_->sendGetUserFriends(0, 0);
    }
    glFriendsRequested_ = 1;
    return result;
}

// 0x81c78c _ZN15CMatchingGLLive23SendGLFriendsGameInviteEPc
// Sends the invitation (title "title", text "Lets play Dungeon Hunter 2 !", type 8) to the name through the
// GLXPlayerMessage object (+0x6708), then walks the game friend list (+0x6bd8) for the first entry whose name
// equals the argument and sets its invited flag. The result is the last value computed, as in the original: the
// send result when the list is empty, otherwise the last string comparison (0 once an entry matched).
// Partial: the invitation goes through GLXPlayerMessage::SendOnlineMessage, whose bool argument and result are the
// port's (the original's last argument is 0). A null message object skips the send here (result 0) and a
// null name skips the list walk; the original dereferences both.
std::int32_t CMatchingGLLive::SendGLFriendsGameInvite(char* name) {
    char title[sizeof(kInviteTitle)];
    char text[sizeof(kInviteText)];
    std::memcpy(title, kInviteTitle, sizeof(title));
    std::memcpy(text, kInviteText, sizeof(text));
    std::int32_t result = 0;
    if (glxMessage_ != nullptr) {
        result = glxMessage_->SendOnlineMessage(name, 8, title, text, false);
    }
    if (name == nullptr) {
        return result;
    }
    for (GLFriendInfo& entry : glFriends_) {
        result = std::strcmp(entry.name.c_str(), name);  // LC_API_STRCMP_0
        if (result == 0) {
            entry.invited = true;  // node +0x11
            return result;
        }
    }
    return result;
}

// 0x81ee10 _ZN15CMatchingGLLive19AddFriendInGameListE12GLFriendInfo
// Allocates a list node (0x14 bytes: links, then the three words of the entry) and links it at the tail of the
// list whose head is at +0x6bd8.
void CMatchingGLLive::AddFriendInGameList(GLFriendInfo info) {
    glFriends_.push_back(std::move(info));
}

// 0x81ee6c _ZN15CMatchingGLLive16GetGLFriendsListEv
// Builds a new list (returned through the hidden result pointer) with a copy of every node of the list at +0x6bd8,
// in order. The entries' strings are copied by value here; the original copies the three words (the string
// pointers are shared).
std::list<GLFriendInfo> CMatchingGLLive::GetGLFriendsList() const {
    return glFriends_;
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
// does. The original then creates the lobby observer (on first use; reproduced) and searches the lobby
// member list (tMemberInfo, not in the port) for the signed-in user name, ignoring case. That search is not
// reproduced, so a signed-in lookup returns -1.
std::int32_t CMatchingGLLive::GetGLLiveMemberId() const {
    if (gameCenterActive_) {
        return -1;
    }
    CSignIn* signIn = CSignIn::Get();
    if (signIn == nullptr || !signIn->LoggedIn()) {
        return -1;
    }
    (void)GetGLXPlayerMPLobbyObserver();
    return -1;
}

// 0x81f04c _ZN15CMatchingGLLive14SetPlayerParamESsb
// Partial: stores the text in playerParam_ (+0x67f0) when it is at most 0x80 bytes. With the flag set,
// the text is sent to the lobby as a user parameter, with its length as the key (mpSendSetUserParameter),
// and then a copy of the text goes to the lobby observer's SetPlayerParam. The observer's SetPlayerParam is a
// no-op in the port (its member list is not modelled).
// Partial: a missing lobby skips the send here, because the original creates the lobby on first use
// (GetGLXPlayerMPLobby is partial); the observer call still happens.
void CMatchingGLLive::SetPlayerParam(std::string value, bool sendToLobby) {
    if (value.size() > kMaxPlayerParamBytes) {
        return;
    }
    playerParam_ = value;
    if (!sendToLobby) {
        return;
    }
    if (GLXPlayerMPLobby* lobby = GetGLXPlayerMPLobby(); lobby != nullptr) {
        std::string copy = value;
        lobby->mpSendSetUserParameter(copy.data(), static_cast<std::int32_t>(copy.size()));
    }
    GetGLXPlayerMPLobbyObserver()->SetPlayerParam(value);
}
