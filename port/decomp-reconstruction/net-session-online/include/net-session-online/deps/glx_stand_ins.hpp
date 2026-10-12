// Stand-ins for the GLX online layer that GLXPlayerMPLobby depends on. Each
// one names the original symbol it replaces.
#pragma once

#include <cstdint>

// Observer of a GLXPlayerMPBase. Vtable slot 0 is called with the observer as
// this when a request is refused because the player is not in a valid state.
class GLXPlayerMPBaseObserver {
public:
    virtual ~GLXPlayerMPBaseObserver() = default;
    virtual void OnInvalidState() = 0;  // vtable slot 0
};

// Base of the GLX multiplayer player objects. +0x04 is the observer pointer.
class GLXPlayerMPBase {
public:
    explicit GLXPlayerMPBase(GLXPlayerMPBaseObserver* observer) : observer_(observer) {}
    virtual ~GLXPlayerMPBase() = default;

protected:
    GLXPlayerMPBaseObserver* observer_ = nullptr;  // +0x04
};

// The background thread stopped by mpDisconnect. Original vtable slot 1 deletes it.
class ThreadHandle {
public:
    virtual ~ThreadHandle() = default;
};

inline void ThreadStop() {}

// XP_API_GET_TIME(): platform clock in seconds, stored in ConnectionLobby::requestTimeMs.
inline std::uint32_t XP_API_GET_TIME() { return 0; }
