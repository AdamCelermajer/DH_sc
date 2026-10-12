#include "net-session-online/c_sign_in_gl_live.hpp"

#include <cstring>

#include "net-session-online/c_matching_gl_live.hpp"
#include "net-session-online/c_matching_gl_live_observers.hpp"
#include "net-session-online/c_sign_in_gl_live_observer.hpp"
#include "net-session-online/deps/matching.hpp"
#include "net-session-online/glx_player_login.hpp"
#include "net-session-online/glx_player_mp_lobby.hpp"

// Seams for the parts of the original that the port does not reach yet. Each one names the
// original accessor it stands for. They are file-local, so the class header keeps the
// layout only. Remove each seam when its accessor is reconstructed.
namespace {

// CMatchingGLLive::GetGLXPlayerLogin (0x81c5e8). Its body is in another slice and is not in the
// port. Null here, so the login requests are not sent.
GLXPlayerLogin* LoginService() { return nullptr; }

// CMatchingGLLive::GetGGI on the matching singleton (0x81c18c). The instance is not reachable
// from here, so the GGI reads as 0.
unsigned long MatchingGgi() { return 0UL; }

// dword_A3387C (0xa3387c): a global char* used as the Game Center user id. It is not recovered
// yet, so it is null here.
const char* GameCenterUserIdGlobal() { return nullptr; }

// CMatching+0x67e8 (the Game Center flag, a byte of the matching object). The port has no getter,
// so it reads as false and the Game Center branches are not taken.
bool GameCenterFlag() { return false; }

// GLXPlayerMPLobby+0x60 (the lobby state). state_ is private and has no getter in the port, so
// it reads as 0 (not ready). Event 6 therefore waits until the getter exists.
std::uint8_t LobbyState(const GLXPlayerMPLobby* /*lobby*/) { return 0; }

}  // namespace

// 0x824204 _ZN13CSignInGLLive10InitializeEv
// The original sets the active flag at +0x10 when it is clear and returns 0. That is the
// CSignIn::Initialize body, so the base is called.
std::int32_t CSignInGLLive::Initialize() { return CSignIn::Initialize(); }

// 0x82421c _ZN13CSignInGLLive6SignInEv
std::int32_t CSignInGLLive::SignIn() { return 0; }

// 0x8246b8 _ZN13CSignInGLLiveC1Ev (C2 at 0x824734 has the same body; skipped).
// The original runs the CSignIn base ctor, sets the vtables, clears the lobby flag, and builds
// the mutex and the empty CEvent list. The member initialisers do all of that here.
CSignInGLLive::CSignInGLLive() {}

// 0x8247ec _ZN13CSignInGLLiveD1Ev (D2 at 0x8248a8 has the same body; skipped).
// Clears the CEvent list, then the mutex and the second queue are destroyed as members, and
// CSignIn::~CSignIn runs last. The original does not destroy the second queue in this body;
// CEventQueueBase's own dtor does it here.
CSignInGLLive::~CSignInGLLive() { pendingEvents_.clear(); }

// 0x8243dc _ZN13CSignInGLLive6SignInER18CSignInCredentials
// Returns 1 when active and 0 otherwise. When active it sends the login (kind 0x0f, with
// "EN" as the language and the context as the device value) through the login seam, keeps the
// credentials, and posts event 1 to both queues.
std::int32_t CSignInGLLive::SignIn(CSignInCredentials& credentials) {
    if (!active_) {
        return 0;
    }
    if (GLXPlayerLogin* login = LoginService(); login != nullptr) {
        login->SendLogin(credentials.Name(), credentials.Password(), nullptr, 1,
                         static_cast<char*>(credentials.Context()), const_cast<char*>("EN"), true);
    }
    credentials_ = CSignInCredentials(credentials.Name(), credentials.Password(), credentials.Context());
    events_.AddEvent(1, nullptr, 0);
    loginEvents_.AddEvent(1, nullptr, 0);
    return 1;
}

// 0x8243a8 _ZN13CSignInGLLive7SignOutEv
// Sends the logout when logged in, then posts event 4 to the CSignIn queue.
void CSignInGLLive::SignOut() {
    if (loggedIn_) {
        if (GLXPlayerLogin* login = LoginService(); login != nullptr) {
            login->SendLogout();
        }
    }
    events_.AddEvent(4, nullptr, 0);
}

// 0x824224 _ZN13CSignInGLLive11CancelLoginEv
// Signs out when logged in, clears the logged-in flag and the lobby flag, and empties both queues.
void CSignInGLLive::CancelLogin() {
    if (loggedIn_) {
        SignOut();
        loggedIn_ = false;
    }
    lobbyPending_ = false;
    events_.Clear();
    loginEvents_.Clear();
}

// 0x82427c _ZN13CSignInGLLive9TerminateEv
// The base Terminate (signs out when active), then clears the lobby flag and both queues.
void CSignInGLLive::Terminate() {
    CSignIn::Terminate();
    lobbyPending_ = false;
    events_.Clear();
    loginEvents_.Clear();
}

// 0x8242b4 _ZN13CSignInGLLive10LoginLobbyEv
// Partial. The original returns the result of the lobby send; the port's sends return nothing.
// Here the Game Center branch is taken only when the Game Center flag is set (see the seam; it
// is not, yet). The other branch needs the login service, so with no login service it sends
// nothing.
void CSignInGLLive::LoginLobby() {
    CMatchingGLLiveLobbyObserver* lobbyObserver = CMatchingGLLive::s_GLXPlayerMPLobbyObserver;
    if (lobbyObserver == nullptr || !lobbyObserver->LoggedInForTest()) {
        return;
    }
    GLXPlayerMPLobby* lobby = CMatchingGLLive::s_GLXPlayerMPLobby;
    if (lobby == nullptr) {
        return;
    }
    if (GameCenterFlag()) {
        // Original: mpSendLobbyLoginWithGameCenter(ggi, id, strlen(id), 1, 1, id, "NON").
        const char* userId = GameCenterUserIdGlobal();
        const auto userIdLength = static_cast<std::int32_t>(userId != nullptr ? std::strlen(userId) : 0);
        lobby->mpSendLobbyLoginWithGameCenter(static_cast<long>(MatchingGgi()), const_cast<char*>(userId),
                                              userIdLength, 1, 1, const_cast<char*>(userId),
                                              const_cast<char*>("NON"));
        return;
    }
    GLXPlayerLogin* login = LoginService();
    if (login == nullptr) {
        return;
    }
    // Original: mpSendLobbyLogin(login GGI, login UID, strlen(UID), 1).
    const unsigned long ggi = login->GetGGI();
    const char* uid = login->GetUID();
    const auto uidLength = static_cast<std::int32_t>(uid != nullptr ? std::strlen(uid) : 0);
    lobby->mpSendLobbyLogin(static_cast<long>(ggi), const_cast<char*>(uid), uidLength, 1);
}

// 0x82447c _ZN13CSignInGLLive6UpdateEv
// Partial. One step of the login state machine per call, in the original's order:
//   1. login observer configured: GLXPlayerLogin::Update (through the login seam).
//   2. lobby pending: the original calls GLXPlayerMPLobby::Update (vtable +0x08). That slot is
//      not declared in the port, so this step is omitted.
//   3. event 1 with the login observer logged in: SendGetLobbyServer, post event 2. The Game
//      Center variant is taken only with the Game Center flag (seam).
//   4. event 2 with the login observer's server byte set: lobby pending, mpSendEstablishConnection,
//      post event 4. The byte is the first byte of the observer's server address (+0x06).
//   5. event 4 with the lobby observer logged in: LoginLobby, post event 6.
//   6. event 6 with lobby state > 1: consume, post CSignIn event 2, logged in, SetGCState(2).
void CSignInGLLive::Update() {
    if (!active_) {
        return;
    }
    CSignInGLLiveObserver* loginObserver = CMatchingGLLive::s_GLXPlayerLoginObserver;
    if (loginObserver != nullptr && loginObserver->ConfiguredForTest()) {
        if (GLXPlayerLogin* login = LoginService(); login != nullptr) {
            login->Update();
        }
    }

    if (loginEvents_.HasEventOccured(1, false)) {
        const bool serverLoggedIn = loginObserver != nullptr && loginObserver->LoggedInForTest();
        if (serverLoggedIn) {
            loginEvents_.ConsumeEvent(1);
            if (GLXPlayerLogin* login = LoginService(); login != nullptr) {
                login->SendGetLobbyServer();
            }
            loginEvents_.AddEvent(2, nullptr, 0);
        } else if (GameCenterFlag()) {
            loginEvents_.ConsumeEvent(1);
            if (GLXPlayerLogin* login = LoginService(); login != nullptr) {
                login->SendGetLobbyServerWithGameCenter(GameCenterUserIdGlobal(), nullptr, nullptr);
            }
            loginEvents_.AddEvent(2, nullptr, 0);
        }
    }

    if (loginEvents_.HasEventOccured(2, false) && loginObserver != nullptr &&
        loginObserver->ServerForTest()[0] != '\0') {
        lobbyPending_ = true;
        loginEvents_.ConsumeEvent(2);
        // The original also reads the login UID here and discards it, so that read is omitted.
        if (GLXPlayerMPLobby* lobby = CMatchingGLLive::s_GLXPlayerMPLobby; lobby != nullptr) {
            lobby->mpSendEstablishConnection();
        }
        loginEvents_.AddEvent(4, nullptr, 0);
    }

    if (loginEvents_.HasEventOccured(4, false)) {
        CMatchingGLLiveLobbyObserver* lobbyObserver = CMatchingGLLive::s_GLXPlayerMPLobbyObserver;
        if (lobbyObserver != nullptr && lobbyObserver->LoggedInForTest()) {
            loginEvents_.ConsumeEvent(4);
            LoginLobby();
            loginEvents_.AddEvent(6, nullptr, 0);
        }
    }

    if (loginEvents_.HasEventOccured(6, false)) {
        if (LobbyState(CMatchingGLLive::s_GLXPlayerMPLobby) > 1) {
            loginEvents_.ConsumeEvent(6);
            events_.AddEvent(2, nullptr, 0);
            loggedIn_ = true;
            // The original calls CMatchingGLLive::SetGCState on the matching singleton. The port's
            // CMatching stand-in forwards the call, and its default is a no-op.
            if (CMatching* matching = CMatching::Get(); matching != nullptr) {
                matching->SetGCState(2);
            }
        }
    }
}
