#include "net-session-online/c_matching_gl_live.hpp"

#include "net-session-online/c_matching_gl_live_observers.hpp"
#include "net-session-online/c_sign_in.hpp"
#include "net-session-online/c_sign_in_gl_live_observer.hpp"
#include "net-session-online/deps/matching_queue.hpp"
#include "net-session-online/glx_player_login.hpp"
#include "net-session-online/glx_player_mp_lobby.hpp"
#include "net-session-online/glx_player_user_friend.hpp"

// Singletons created on first use (the original keeps them as globals of the class).
GLXPlayerUserFriend* CMatchingGLLive::s_GLXPlayerUserFriend = nullptr;
GLXPlayerLogin* CMatchingGLLive::s_GLXPlayerLogin = nullptr;

// 0x81e7f0 _ZN15CMatchingGLLiveC2Eb (the C1 twin at 0x81e9dc has the same body).
// Partial: the CMatching base, the CRoomAttributes, CRoomSearchFilter and CNetMutex members, the
// 32 MemberInfoNetStruct entries (not in the port) and the friend-list head are not built here.
// Sets the local member id to -1 and the Game Center flag from the argument, then loads
// profile.sav as the original does. The byte at +0x0f (set to 3 in the original) is not modelled.
CMatchingGLLive::CMatchingGLLive(bool gameCenterActive) : gameCenterActive_(gameCenterActive) {
    memberId_ = -1;
    LoadGLLiveProfile();
}

// 0x81d2b4 _ZN15CMatchingGLLiveD1Ev (the D2 twin at 0x81d614 is the same body; the D0 thunk at
// 0x81d5f8 frees the object after this).
// Partial: the original deletes the GLX objects it created and the GLLive singletons, then tears
// down the CMatching base and the containers. The port does not release anything yet: the seams
// (SetGLFriendListForTest and the lobby pointer in the tests) hand in objects the class does not
// own, so deleting them here would be wrong. Only the Game Center flag is cleared, as in the original.
CMatchingGLLive::~CMatchingGLLive() {
    s_IsGameStarting = false;
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
