#include "net-session-online/tb/online_singleton.hpp"

#include <cstdio>
#include <cstdlib>
#include <new>

#include "net-session-online/online_game_state.hpp"

// Engine assert level, defined in core-platform (src/memory.cpp). 0 is the default.
extern int gAssertLevel;

// The class template's static instance pointer. The original keeps it in one global, and
// the explicit instantiation below provides it for OnlineGameState.
template <class T>
T* OnlineSingleton<T>::m_instance = nullptr;

// 0x320e98 _ZN15OnlineSingletonI15OnlineGameStateE11GetInstanceEv
//
// Partial. The original's control flow, in order:
//   1. Return m_instance when it is set.
//   2. Otherwise allocate 0x5c bytes with operator new(size, hint 0), construct the object in
//      place with OnlineGameState::OnlineGameState(), and store the pointer in m_instance.
//   3. When the allocation returned null, check gAssertLevel:
//      2: store 0 to address 0 to force a crash;
//      1: print "ASSERT(m_instance != 0) FAILED" and return m_instance (null);
//      other: return the (null) block.
//   4. Return the new block.
//
// Deviations and open points:
//   - The allocation is `new (std::nothrow) T()`. The hint argument (0) is not modelled, since
//     this category has no hinted operator new. The block is sizeof(T) on the host, not 0x5c.
//   - On a null block the original still calls the constructor (on null, which is undefined).
//     The port skips it.
//   - Level 2 calls std::abort() where the original writes to address 0. Same effect (a crash),
//     but not the same instruction.
//   - The fprintf stream is `__sF + 0xa8` in the ARM listing. Which FILE this is has not been
//     identified, so the port uses stderr. The file-name string is the one in the pseudocode
//     (the string says MultiplayerStateMachine.h, which the original copied into this function).
template <class T>
T* OnlineSingleton<T>::GetInstance() {
    T* v0 = m_instance;
    if (m_instance == nullptr) {
        v0 = new (std::nothrow) T();
        m_instance = v0;
        if (v0 == nullptr) {
            if (gAssertLevel == 2) {
                std::abort();
            } else if (gAssertLevel == 1) {
                std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", "m_instance != 0",
                             "..\\..\\project_vs2005\\Game/..\\..\\sources/Game/Multiplayer/MultiplayerStateMachine.h",
                             152);
                return m_instance;
            }
        }
    }
    return v0;
}

template class OnlineSingleton<OnlineGameState>;
