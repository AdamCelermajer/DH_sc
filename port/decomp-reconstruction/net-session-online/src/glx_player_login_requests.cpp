// GLXPlayerLogin request senders and the keep-alive. Each sender formats an
// "f|kind|i|GGI|u|UID|..." request into a 0x1000-byte buffer and hands it to SendByGet.
// The formats are the ones in the binary's .rodata, as far as the pseudocode shows them.
#include <cstdint>
#include <cstdio>
#include <cstring>

#include "net-session-online/deps/glx_stand_ins.hpp"
#include "net-session-online/deps/xp_api.hpp"
#include "net-session-online/glx_player_login.hpp"

namespace {
constexpr std::size_t kRequestBytes = 0x1000;

// A missing user id is written as an empty string, as in GLXPlayerUser's builders.
const char* UidText(const char* uid) { return uid != nullptr ? uid : ""; }
}  // namespace

// 0x83345c _ZN14GLXPlayerLogin32SendGameInvitationGetLobbyServerEPc
void GLXPlayerLogin::SendGameInvitationGetLobbyServer(const char* invitation) {
    char buf[kRequestBytes] = {};
    std::snprintf(buf, sizeof buf, "f|%d|i|%d|u|%s|cn|%s|", 0x6c, static_cast<int>(GetGGI()), UidText(GetUID()),
                  invitation != nullptr ? invitation : "");
    lastKeepAliveMs_ = static_cast<std::int32_t>(XP_API_GET_TIME());
    XP_DEBUG_OUT("SendGameInvitationGetLobbyServer -> buffer = %s\n", buf);
    SendByGet(buf);
}

// 0x833528 _ZN14GLXPlayerLogin18SendGetLobbyServerEv
void GLXPlayerLogin::SendGetLobbyServer() {
    char buf[kRequestBytes] = {};
    std::snprintf(buf, sizeof buf, "f|%d|i|%d|u|%s|", 0x6b, static_cast<int>(GetGGI()), UidText(GetUID()));
    lastKeepAliveMs_ = static_cast<std::int32_t>(XP_API_GET_TIME());
    XP_DEBUG_OUT("SendGetLobbyServer -> buffer = %s\n", buf);
    SendByGet(buf);
}

// 0x8335ec _ZN14GLXPlayerLogin32SendGetLobbyServerWithGameCenterEPcS0_S0_
// Partial: the original appends two fields when their arguments are non-null. Their format
// strings are not identified, so only the base request is built here.
void GLXPlayerLogin::SendGetLobbyServerWithGameCenter(const char* uid, const char* /*gameCenterA*/,
                                                      const char* /*gameCenterB*/) {
    char buf[kRequestBytes] = {};
    std::snprintf(buf, sizeof buf, "f|%d|i|%d|u|%s|", 0x6b, static_cast<int>(GetGGI()), UidText(uid));
    lastKeepAliveMs_ = static_cast<std::int32_t>(XP_API_GET_TIME());
    XP_DEBUG_OUT("SendGetLobbyServer -> buffer = %s\n", buf);
    SendByGet(buf);
}

// 0x83370c _ZN14GLXPlayerLogin18SendGetProductInfoEPc
// Partial: the pseudocode calls strlen on a null list, which the original cannot survive.
// This port sends the request with an empty list in that case.
void GLXPlayerLogin::SendGetProductInfo(const char* list) {
    char buf[kRequestBytes] = {};
    std::snprintf(buf, sizeof buf, "f|%d|i|%d|u|%s|l|%s|", 0x5f, static_cast<int>(GetGGI()), UidText(GetUID()),
                  list != nullptr ? list : "");
    XP_DEBUG_OUT("GLXPlayerLogin::SendGetProductInfo -> buffer = %s\n", buf);
    SendByGet(buf);
}

// 0x833814 _ZN14GLXPlayerLogin15SendGetAPNSInfoEi
void GLXPlayerLogin::SendGetAPNSInfo(std::int32_t type) {
    char buf[kRequestBytes] = {};
    std::snprintf(buf, sizeof buf, "f|%d|i|%d|u|%s|t|%d|", 0x53, static_cast<int>(GetGGI()), UidText(GetUID()),
                  static_cast<int>(type));
    XP_DEBUG_OUT("GLXPlayerLogin::SendGetAPNSInfo -> buffer = %s\n", buf);
    SendByGet(buf);
}

// 0x8338d8 _ZN14GLXPlayerLogin13SendKeepAliveEv
void GLXPlayerLogin::SendKeepAlive() {
    char buf[kRequestBytes] = {};
    std::snprintf(buf, sizeof buf, "f|%d|i|%d|u|%s|", 0x10, static_cast<int>(GetGGI()), UidText(GetUID()));
    lastKeepAliveMs_ = static_cast<std::int32_t>(XP_API_GET_TIME());
    XP_DEBUG_OUT("SendKeepAlive -> buffer = %s\n", buf);
    SendByGet(buf);
}

// 0x83399c _ZN14GLXPlayerLogin9KeepAliveEv
void GLXPlayerLogin::KeepAlive() {
    if (!ShouldSendKeepAlive()) {
        return;
    }
    SendKeepAlive();
}

// 0x8339c0 _ZN14GLXPlayerLogin10SendLogoutEv
void GLXPlayerLogin::SendLogout() {
    keepAliveEnabled_ = false;
    char buf[kRequestBytes] = {};
    std::snprintf(buf, sizeof buf, "f|%d|i|%d|u|%s|", 0x11, static_cast<int>(GetGGI()), UidText(GetUID()));
    XP_DEBUG_OUT("[Logout] before String2Blob -> buffer = %s\n", buf);
    SendByGet(buf);
}

// 0x8343e0 _ZN14GLXPlayerLogin6UpdateEv
void GLXPlayerLogin::Update() {
    KeepAlive();
    GLXPlayerWebComponent::Update();
}

// 0x8343f8 _ZN14GLXPlayerLogin16clearProductInfoEv
void GLXPlayerLogin::clearProductInfo() {
    productGgi_.clear();
    productId_.clear();
    productName_.clear();
    productCount_ = 0;
}

// 0x834928 _ZN14GLXPlayerLoginD1Ev (D2 at 0x834a8c has the same body). The owned strings are
// released by their unique_ptr members; the product lists are cleared here.
GLXPlayerLogin::~GLXPlayerLogin() { clearProductInfo(); }
