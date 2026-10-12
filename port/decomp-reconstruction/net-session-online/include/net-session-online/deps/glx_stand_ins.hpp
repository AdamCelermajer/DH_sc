// Stand-ins for the GLX online layer that GLXPlayerMPLobby depends on. Each
// one names the original symbol it replaces.
#pragma once

#include <cstdint>

// The background thread stopped by mpDisconnect. Original vtable slot 1 deletes it.
class ThreadHandle {
public:
    virtual ~ThreadHandle() = default;
};

inline void ThreadStop() {}

// XP_API_GET_TIME(): platform clock in seconds, stored in ConnectionLobby::requestTimeMs.
inline std::uint32_t XP_API_GET_TIME() { return 0; }
