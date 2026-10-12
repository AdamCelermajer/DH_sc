#include "net-session-online/c_sign_in_gl_live.hpp"

// 0x824204 _ZN13CSignInGLLive10InitializeEv
// The original sets the active flag at +0x10 when it is clear and returns 0. That is the
// CSignIn::Initialize body, so the base is called.
std::int32_t CSignInGLLive::Initialize() { return CSignIn::Initialize(); }

// 0x82421c _ZN13CSignInGLLive6SignInEv
std::int32_t CSignInGLLive::SignIn() { return 0; }
