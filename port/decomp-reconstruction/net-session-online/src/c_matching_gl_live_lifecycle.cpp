#include "net-session-online/c_matching_gl_live.hpp"

#include "net-session-online/c_lobby_parameter_and_query.hpp"
#include "net-session-online/c_matching_gl_live_observers.hpp"
#include "net-session-online/c_sign_in.hpp"
#include "net-session-online/c_sign_in_gl_live_observer.hpp"
#include "net-session-online/deps/platform_stand_ins.hpp"
#include "net-session-online/deps/stream_bytes.hpp"
#include "net-transport/connection_manager.hpp"  // CConnectionManager (real)
#include "net-session-online/deps/matching_queue.hpp"
#include "net-session-online/glx_player_login.hpp"
#include "net-session-online/glx_player_mp_lobby.hpp"
#include "net-session-online/glx_player_serever_config.hpp"
#include "net-session-online/glx_player_user.hpp"
#include "net-session-online/glx_player_user_friend.hpp"

// Singletons created on first use (the original keeps them as globals of the class).
GLXPlayerUserFriend* CMatchingGLLive::s_GLXPlayerUserFriend = nullptr;
GLXPlayerLogin* CMatchingGLLive::s_GLXPlayerLogin = nullptr;

// 0x81e7f0 _ZN15CMatchingGLLiveC2Eb (the C1 twin at 0x81e9dc has the same body).
// The original runs the CMatching constructor, sets the local member id (+0x3640) to -1, constructs the
// create-room attributes, the server map, the 32 member info elements, the GLX pointers (all null), the room
// attributes map, the search filter, the two mutexes, the Game Center flag from the argument (+0x67e8), the
// flags, the player parameter string, the room list and the other containers, then the Game Center
// attributes and the friend list, loads profile.sav, and sets the base byte +0x0f to 3. The port builds the
// same members through their default member initializers, in declaration order (the base first), so the
// body keeps only the explicit stores. Words the original leaves unset (the peer key counter at +0x363c,
// +0x3630) start at 0. The callee rows keep their own notes: the CMatching constructor and
// MemberInfoNetStruct constructor are partial.
CMatchingGLLive::CMatchingGLLive(bool gameCenterActive) {
    memberId_ = -1;                   // +0x3640
    gameCenterActive_ = gameCenterActive;  // +0x67e8
    LoadGLLiveProfile();
    unk_0f = 3;                       // +0x0f
}

// 0x81d2b4 _ZN15CMatchingGLLiveD1Ev (the D2 twin at 0x81d614 is the same body; the D0 thunk at
// 0x81d5f8 frees the object after this).
// Partial. Released here: the lobby parameter and query (+0x6808), the server config (+0x6710) and the
// GLXPlayerUser (+0x6718); the original does not delete the GLXPlayerUser (it leaks), the port frees it
// because Initialize creates it. The message, friend, observer and leaderboard objects (+0x6700..+0x670c,
// +0x671c) are not created by the port (Update's bootstrap is partial), and the friend list pointer is
// handed in by the tests as a stack object, so none of them is deleted. The original also deletes the
// GLXPlayerLogin, login observer, lobby and lobby observer singletons and resets them to null; the port
// keeps those, because the tests hand in stack objects through the statics. The containers and the base
// are destroyed by the compiler. Only the Game Center started flag is cleared, as in the original.
CMatchingGLLive::~CMatchingGLLive() {
    delete lobbyQuery_;
    lobbyQuery_ = nullptr;
    delete glxServerConfig_;
    glxServerConfig_ = nullptr;
    delete glxUser_;
    glxUser_ = nullptr;
    s_IsGameStarting = false;
}

// CMatching seams for CConnectionManager ((*CConnectionManager::GetInstance()) in the original). Not virtual slots of
// CMatching: CMatching::UpdateMemberIdList and ProcessEvents call the connection manager through them.
// CConnectionManager::GetConnMemberIdList(false).
std::vector<int> CMatchingGLLive::ConnectionMemberIdList() const {
    return CConnectionManager::GetInstance()->GetConnMemberIdList(false);
}
// CConnectionManager::IsConnected(int).
bool CMatchingGLLive::IsConnectedMember(int memberId) const {
    return CConnectionManager::GetInstance()->IsConnected(memberId);
}
// CConnectionManager::Disconnect(int, false).
void CMatchingGLLive::DisconnectMember(int memberId) {
    CConnectionManager::GetInstance()->Disconnect(memberId, false);
}

// 0x81c5e8 _ZN15CMatchingGLLive17GetGLXPlayerLoginEv
// Partial: creates the GLXPlayerLogin singleton on first use. The original also copies the login
// observer's fields into it and registers that observer through RegisterObserver, which takes a
// GLXPlayerObserver* and cannot take the port's CSignInGLLiveObserver. Those steps are not reproduced.
GLXPlayerLogin* CMatchingGLLive::GetGLXPlayerLogin() {
    if (s_GLXPlayerLogin == nullptr) {
        s_GLXPlayerLogin = new GLXPlayerLogin();
    }
    return s_GLXPlayerLogin;
}

// 0x81c37c _ZN15CMatchingGLLive22GetGLXPlayerUserFriendEv
// Partial: creates the GLXPlayerUserFriend singleton on first use and keeps it in the static
// s_GLXPlayerUserFriend, as the decompiled body does. The original also registers the login
// observer on it (RegisterObserver takes a GLXPlayerObserver*, see GetGLXPlayerLogin), which is not
// reproduced. The member at +0x6700 (glxUserFriend_) is a different slot and is not written here.
GLXPlayerUserFriend* CMatchingGLLive::GetGLXPlayerUserFriend() {
    if (s_GLXPlayerUserFriend == nullptr) {
        s_GLXPlayerUserFriend = new GLXPlayerUserFriend();
    }
    return s_GLXPlayerUserFriend;
}

// 0x81f858 _ZN15CMatchingGLLive9TerminateEv
// Partial: clears the matching event queue and the GLLive queue, terminates the login observer and,
// when signed in, the lobby observer, then clears the Game Center flag and the GLLive started flag.
// Not reproduced: the CMatching::m_eventQueue clear, the virtual calls at +0x60 and +0x64 of this
// class (not identified), the vtable call at +0x18 of the lobby object, the four UnregisterPacketType
// calls and the CMatching::Terminate base call.
std::int32_t CMatchingGLLive::Terminate() {
    MatchingInternalEventQueue().Clear();
    s_eventQueueGLLiveInternal.Clear();

    CSignIn* signIn = CSignIn::Get();
    if (signIn != nullptr && signIn->LoggedInForTest()) {
        GetGLXPlayerMPLobbyObserver()->Terminate();
    }
    GetGLXPlayerLoginObserver()->Terminate();

    gameCenterActive_ = false;
    s_IsGameStarting = false;
    return 0;
}
