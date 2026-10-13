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
// Partial: creates the GLXPlayerLogin singleton on first use. The original then fetches the login observer
// (created on first use as well; that side effect is reproduced) and copies its fields into a temporary
// object. It then registers the login observer through RegisterObserver, which takes a GLXPlayerObserver*
// and cannot take the port's CSignInGLLiveObserver (CSignInGLLiveObserver is not a GLXPlayerObserver), so
// the copy and the registration are not reproduced.
GLXPlayerLogin* CMatchingGLLive::GetGLXPlayerLogin() {
    if (s_GLXPlayerLogin == nullptr) {
        s_GLXPlayerLogin = new GLXPlayerLogin();
        (void)GetGLXPlayerLoginObserver();
    }
    return s_GLXPlayerLogin;
}

// 0x81c37c _ZN15CMatchingGLLive22GetGLXPlayerUserFriendEv
// Partial: creates the GLXPlayerUserFriend singleton on first use and keeps it in the static
// s_GLXPlayerUserFriend, as the decompiled body does. The original then fetches the friend observer (created
// on first use; that side effect is reproduced) and copies its flag byte into a temporary object that is
// destroyed unused (the copy has no visible effect and is not made). It registers the login observer on the
// friend component (RegisterObserver takes a
// GLXPlayerObserver*, see GetGLXPlayerLogin), which is not reproduced. The member at +0x6700 (glxUserFriend_)
// is a different slot and is not written here.
GLXPlayerUserFriend* CMatchingGLLive::GetGLXPlayerUserFriend() {
    if (s_GLXPlayerUserFriend == nullptr) {
        s_GLXPlayerUserFriend = new GLXPlayerUserFriend();
        (void)GetGLXPlayerUserFriendObserver();
    }
    return s_GLXPlayerUserFriend;
}

// 0x81f858 _ZN15CMatchingGLLive9TerminateEv
// Partial. When IsInRoom() (vtable +0x64) is set it calls LeaveRoom (vtable +0x3c). It then clears the CMatching
// output queue (m_eventQueue), the CMatching internal queue (m_eventQueueInternal) and the GLLive queue. When signed
// in it calls the lobby's vtable +0x18 slot, mpDisconnect, and terminates the lobby observer. It terminates the login
// observer, unregisters packet types 2, 3, 9 and 4, calls the CMatching base Terminate, and clears the initialized flag
// (+0x0c), the Game Center flag (+0x67e8), the GLLive started flag and the alpha-server flag (+0x67ed).
// Partial: the original creates the lobby on first use, so a null lobby here skips the disconnect; the lobby observer's
// Terminate calls ClearRoomList, which is not reconstructed.
std::int32_t CMatchingGLLive::Terminate() {
    if (IsInRoom()) {
        LeaveRoom();
    }
    MatchingOutputEventQueue().Clear();
    MatchingInternalEventQueue().Clear();
    s_eventQueueGLLiveInternal.Clear();

    CSignIn* signIn = CSignIn::Get();
    if (signIn != nullptr && signIn->LoggedInForTest()) {
        if (GLXPlayerMPLobby* lobby = GetGLXPlayerMPLobby(); lobby != nullptr) {
            lobby->mpDisconnect();  // GLXPlayerMPLobby vtable +0x18
        }
        GetGLXPlayerMPLobbyObserver()->Terminate();
    }
    GetGLXPlayerLoginObserver()->Terminate();

    CConnectionManager::UnregisterPacketType(2);
    CConnectionManager::UnregisterPacketType(3);
    CConnectionManager::UnregisterPacketType(9);
    CConnectionManager::UnregisterPacketType(4);
    CMatching::Terminate();

    gameCenterActive_ = false;  // +0x67e8
    initialized_ = false;       // +0x0c
    s_IsGameStarting = false;
    useAlphaServer_ = false;    // +0x67ed
    return 0;
}
