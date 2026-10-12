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
