#include "net-session-online/cxplayer_manager.hpp"

#include <cstdio>

// 0x52e0d0 _ZN15CXPlayerManager11SwitchStateE20CXPlayerManagerState
void CXPlayerManager::SwitchState(CXPlayerManagerState state) { state_ = state; }

// 0x52e0d8 _ZN15CXPlayerManager7IsReadyEv: true in the state 0x10.
bool CXPlayerManager::IsReady() const { return state_ == kReadyState; }

// 0x52e2d4 _ZN15CXPlayerManager14OnNetworkErrorEv
void CXPlayerManager::OnNetworkError() {
    std::puts("XPlayerManager: ERROR! NETWORK ERROR!");
    lastError_ = 3;
    SwitchState(0x11);
}
