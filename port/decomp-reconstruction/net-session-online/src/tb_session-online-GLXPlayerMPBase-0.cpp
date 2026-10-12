// Batch e1-net-session-online-GLXPlayerMPBase-0: GLXPlayerMPBase::setErrMsg(),
// mpSendLogin(char*) and mpSendEstablishConnection(). The observer callbacks are reached
// through the observer's vtable: the vptr points at the first virtual entry, so callback
// slot k is at vptr+0x08+4k (checked against the vtable image at 0x9927e4).
#include <cstdint>
#include <cstring>

#include "net-session-online/deps/connection_transport.hpp"
#include "net-session-online/deps/glx_stand_ins.hpp"
#include "net-session-online/deps/xp_api.hpp"
#include "net-session-online/glx_player_mp_base.hpp"

// 0x84abd8 _ZN15GLXPlayerMPBase9setErrMsgEv
// Partial. Recovered: the old message is freed, as errMsg_.reset(). Not recovered: when
// aux68_ is set, the original copies the message from the aux object, through two
// unnamed virtuals of aux68_ (vtable +0x60 gives the length, +0x64 the data). The aux
// object's type and those slots are not identified, so errMsg_ stays null in that case.
void GLXPlayerMPBase::setErrMsg() {
    errMsg_.reset();
}

// 0x84ac64 _ZN15GLXPlayerMPBase11mpSendLoginEPc
// The Hex-Rays return type is int. The value comes from XP_DEBUG_OUT and from the observer
// callback, which is void, so the port returns void.
// Deviations: the login buffer copy is bounded to the 0x1000 characters that fit (the
// original copies unbounded), and the transport is null-checked where the original
// dereferences it unconditionally.
void GLXPlayerMPBase::mpSendLogin(char* nickname) {
    XP_DEBUG_OUT("GLXPlayerMPbyP2P::mpSendLogin(){\n");
    if (field60_ != 0 && field60_ <= 1) {
        char buffer[0x1001];
        std::memset(buffer, 0, sizeof(buffer));
        std::strncpy(buffer, nickname, sizeof(buffer) - 1);
        XP_DEBUG_OUT("GLXPlayerMPbyP2P::mpLoginNickname %s", buffer);
        field50_ = -1;
        if (transport_ != nullptr) {
            ConnectionSendLoginPackage(transport_, buffer);
        }
        field71_ = 1;
        if (transport_ != nullptr) {
            transport_->timeMs = static_cast<std::uint32_t>(XP_API_GET_TIME());
        }
        XP_DEBUG_OUT("}\n");
    } else {
        field50_ = 0;
        observer_->OnMPLeaveGameSuccess();  // vtable +0x10
    }
}

// 0x84ad78 _ZN15GLXPlayerMPBase25mpSendEstablishConnectionEv
// Partial. Recovered: the observer branch (state 0, OnMPFinishGameSuccess), the transport
// replacement, the state -1, the name, and the flags at +0x70 and +0x6c.
// Not recovered: the decompiled first step of the replacement branch is an unnamed
// virtual call on this object (vtable +0x18). It is left out because the slot is not
// identified. Deviations: the original allocates 0x2050 bytes for the Connection, the
// port uses the ConnectionTransport stand-in; the ctor and setName are stand-ins. The
// Hex-Rays return type is void*; the port returns void.
void GLXPlayerMPBase::mpSendEstablishConnection() {
    if (field60_ != 0) {
        field50_ = 0;
        observer_->OnMPFinishGameSuccess();  // vtable +0x08
        return;
    }
    if (transport_ != nullptr) {
        // NOT RECOVERED: unnamed virtual call on this object (vtable +0x18) goes here.
        delete transport_;  // original: virtual deleting dtor (vtable +0x04 of the transport)
        transport_ = nullptr;
    }
    field50_ = -1;
    transport_ = new ConnectionTransport();
    ConnectionConstruct(transport_, field58_, field5c_);
    ConnectionSetName(transport_, field08_);
    field70_ = 1;
    field6c_ = 1;
}
