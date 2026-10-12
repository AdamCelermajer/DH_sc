// CSignInGLLive: the Gameloft Live provider of CSignIn (provider 1). Only the two
// overrides with no dependencies are reconstructed. The rest of the class (login through
// CMatchingGLLive and GLXPlayerLogin, the event queues at +0x2c and +0x34, the mutex at +0x30)
// is queued until CMatchingGLLive's accessors are in the port. The class stays abstract
// because SignIn(CSignInCredentials&), SignOut, CancelLogin, Terminate and Update are not
// overridden here.
//
// Layout (original 32-bit offsets, kept as comments; the base is CSignIn):
//   +0x10 active flag, +0x11 logged-in flag (both in CSignIn)
//   +0x14 event queue (CSignIn::events_)
//   +0x2c second event queue, +0x30 CNetMutex, +0x34 list of CEvent (not modelled)
#pragma once

#include <cstdint>

#include "net-session-online/c_sign_in.hpp"

class CSignInGLLive : public CSignIn {
public:
    // 0x824204 _ZN13CSignInGLLive10InitializeEv: same body as CSignIn::Initialize.
    std::int32_t Initialize() override;
    // 0x82421c _ZN13CSignInGLLive6SignInEv: returns 0.
    std::int32_t SignIn() override;
};
