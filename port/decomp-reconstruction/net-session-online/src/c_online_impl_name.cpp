#include "net-session-online/c_online_impl.hpp"

// 0x825068 _ZN11COnlineImpl13GetPlayerNameEv
// Partial: the original asks the transport manager (net-transport) for the name when it is initialized,
// through a vtable slot that is not identified. That branch is stubbed here, so the fallback "PLAYER"
// is always returned.
std::string COnlineImpl::GetPlayerName() const {
    return "PLAYER";
}
