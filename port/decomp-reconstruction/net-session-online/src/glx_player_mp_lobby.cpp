#include "net-session-online/glx_player_mp_lobby.hpp"

#include "net-session-online/c_lobby_parameter_and_query.hpp"
#include "net-session-online/deps/xp_api.hpp"

// Result codes stored at +0x50.
namespace {
constexpr std::int32_t kPending = -1;
constexpr std::int32_t kRefused = 0x32;

// Request kinds stored at +0x84. Values come from the mpSend* bodies.
constexpr std::int32_t kKindLeaveLobby = 3;
constexpr std::int32_t kKindGetPlayerCounter = 2;
constexpr std::int32_t kKindSetPlayerStatus = 4;
constexpr std::int32_t kKindKickOut = 6;
constexpr std::int32_t kKindSetUserParameter = 7;
constexpr std::int32_t kKindSetGameParameter = 8;
constexpr std::int32_t kKindLaunchGame = 9;
constexpr std::int32_t kKindGetLobbyForName = 10;
constexpr std::int32_t kKindGetLobbyList = 11;
constexpr std::int32_t kKindGetLobbyForFriend = 12;
constexpr std::int32_t kKindGetLobbyInfo = 13;
constexpr std::int32_t kKindJoinLobby = 14;
constexpr std::int32_t kKindRejoinLobby = 15;
constexpr std::int32_t kKindDisconnect = 16;
constexpr std::int32_t kKindCancelAutoMatch = 17;
constexpr std::int32_t kKindGetLobbyForNameWithGameParam = 18;
constexpr std::int32_t kKindGetLobbyListWithGameParam = 19;
constexpr std::int32_t kKindGetLobbyForFriendWithGameParam = 20;
constexpr std::int32_t kKindLogin = 1;
constexpr std::int32_t kKindInit = 21;
constexpr std::int32_t kFriendListCap = 30;
}  // namespace

// 0x83a4a4 _ZN16GLXPlayerMPLobby4InitEv
GLXPlayerMPLobby::GLXPlayerMPLobby(GLXPlayerMPBaseObserver* observer, const char* config)
    : GLXPlayerMPBase(observer) {
    // Original: config != 0 goes to XP_API_STRLEN, a non-returning tail call. Not recovered.
    (void)config;
    Init();
}

// 0x83a4f0 _ZN16GLXPlayerMPLobbyD1Ev, 0x83a5ac _ZN16GLXPlayerMPLobbyD2Ev
// Partial: the original also frees the owned objects at +0x58 and +0x7c, whose
// types are not recovered, so they are not freed here.
GLXPlayerMPLobby::~GLXPlayerMPLobby() {
    if (connection_) {
        mpDisconnect();
        connection_.reset();
    }
}

// 0x83a4a4 _ZN16GLXPlayerMPLobby4InitEv
void GLXPlayerMPLobby::Init() {
    unknown61_ = 10;
    pendingKind_ = kKindInit;
    pendingActive_ = false;
    connection_.reset();
    unknown63_ = 0xff;
    unknown64_ = 0xff;
    state_ = 0;
    unknown62_ = 0;
    unknown4c_ = 0;
    autoMatch_ = false;
    LoadConfig();
    unknown88_ = -1;
}

// 0x837630 _ZN16GLXPlayerMPLobby13mpIsInSessionEv
bool GLXPlayerMPLobby::mpIsInSession() const { return state_ > 2; }

// 0x837644 _ZN16GLXPlayerMPLobby10mpIsInRoomEv
bool GLXPlayerMPLobby::mpIsInRoom() const { return state_ > 2; }

// 0x837658 _ZN16GLXPlayerMPLobby10mpIsInGameEv
bool GLXPlayerMPLobby::mpIsInGame() const { return state_ > 4; }

// 0x83766c _ZN16GLXPlayerMPLobby10mpIsMasterEv
bool GLXPlayerMPLobby::mpIsMaster() const { return isMaster_; }

void GLXPlayerMPLobby::RefuseRequest() {
    lastResult_ = kRefused;
    observer_->OnInvalidState();
}

void GLXPlayerMPLobby::StampRequest(std::int32_t kind) {
    connection_->requestTimeMs = XP_API_GET_TIME();
    pendingKind_ = kind;
    pendingActive_ = true;
}

// 0x837674 _ZN16GLXPlayerMPLobby22mpSendGetPlayerCounterEv
void GLXPlayerMPLobby::mpSendGetPlayerCounter() {
    XP_DEBUG_OUT("GLXPlayerMPLobby::mpSendGetPlayerCounter {\n");
    if (state_ > 1) {
        lastResult_ = kPending;
        connection_->sendGetPlayerCounter();
        StampRequest(kKindGetPlayerCounter);
        return;
    }
    RefuseRequest();
}

// 0x8376fc _ZN16GLXPlayerMPLobby36mpSendGetLobbyForFriendWithGameParamEihiPciP23CLobbyParameterAndQuery
void GLXPlayerMPLobby::mpSendGetLobbyForFriendWithGameParam(std::int32_t a, std::uint8_t b,
                                                            std::int32_t c, char* d, std::int32_t e,
                                                            CLobbyParameterAndQuery* f) {
    XP_DEBUG_OUT("GLXPlayerMPLobby::mpSendGetLobbyForFriendWithGameParam(), FriendList{\n");
    if (state_ > 1) {
        lastResult_ = kPending;
        connection_->sendGetLobbyForFriendWithGameParam(a, b, static_cast<std::uint8_t>(c), d, e, f);
        StampRequest(kKindGetLobbyForFriendWithGameParam);
        return;
    }
    RefuseRequest();
}

// 0x83787c _ZN16GLXPlayerMPLobby36mpSendGetLobbyForFriendWithGameParamEihP19GLXPlayerUserFriendP23CLobbyParameterAndQuery
// Partial: the original copies each friend name into a 128-byte stack buffer, then
// makes a tail call that is not identified. Those copies are never read here, so
// they are omitted. The count is capped at 30.
void GLXPlayerMPLobby::mpSendGetLobbyForFriendWithGameParam(std::int32_t a, std::uint8_t b,
                                                            GLXPlayerUserFriend* friends,
                                                            CLobbyParameterAndQuery* query) {
    XP_DEBUG_OUT("GLXPlayerMPLobby::mpSendGetLobbyForFriendWithGameParam(){\n");
    if (state_ < 2) {
        lastResult_ = kRefused;
        observer_->OnInvalidState();
    } else {
        lastResult_ = kPending;
        std::int32_t count = friends->GetFriendsCount();
        if (count > kFriendListCap - 1) {
            count = kFriendListCap;
        }
        mpSendGetLobbyForFriendWithGameParam(a, b, count, nullptr, 0, query);
    }
}

// 0x8377c0 _ZN16GLXPlayerMPLobby23mpSendGetLobbyForFriendEihiPci
void GLXPlayerMPLobby::mpSendGetLobbyForFriend(std::int32_t a, std::uint8_t b, std::int32_t c,
                                               char* d, std::int32_t e) {
    XP_DEBUG_OUT("GLXPlayerMPLobby::mpSendGetLobbyForName(), FriendList{\n");
    if (state_ > 1) {
        lastResult_ = kPending;
        connection_->sendGetLobbyForFriend(a, b, static_cast<std::uint8_t>(c), d, e);
        StampRequest(kKindGetLobbyForFriend);
        return;
    }
    RefuseRequest();
}

// 0x837cc8 _ZN16GLXPlayerMPLobby34mpSendGetLobbyForNameWithGameParamEPc
void GLXPlayerMPLobby::mpSendGetLobbyForNameWithGameParam(char* name) {
    XP_DEBUG_OUT("GLXPlayerMPLobby::mpSendGetLobbyForName(){\n");
    if (state_ > 1) {
        lastResult_ = kPending;
        connection_->sendGetLobbyForNameWithGameParam(name);
        StampRequest(kKindGetLobbyForNameWithGameParam);
        return;
    }
    RefuseRequest();
}

// 0x837d58 _ZN16GLXPlayerMPLobby31mpSendGetLobbyListWithGameParamEiihP23CLobbyParameterAndQueryh
void GLXPlayerMPLobby::mpSendGetLobbyListWithGameParam(std::int32_t a, std::int32_t b, std::uint8_t c,
                                                       CLobbyParameterAndQuery* query, std::uint8_t d) {
    XP_DEBUG_OUT("GLXPlayerMPLobby::mpSendGetLobbyListWithGameParam(){\n");
    if (state_ > 1) {
        lastResult_ = kPending;
        if (c == 0) {
            c = 1;
        }
        connection_->sendGetLobbyListPackageWithGameParam(requestArg_, a, b & ~(b >> 31),
                                                          c, query, d);
        StampRequest(kKindGetLobbyListWithGameParam);
        return;
    }
    RefuseRequest();
}

// 0x838468 _ZN16GLXPlayerMPLobby18mpSendGetLobbyListEiihP23CLobbyParameterAndQueryh
void GLXPlayerMPLobby::mpSendGetLobbyList(std::int32_t a, std::int32_t b, std::uint8_t c,
                                          CLobbyParameterAndQuery* query, std::uint8_t d) {
    XP_DEBUG_OUT("GLXPlayerMPLobby::mpSendGetLobbyList(){\n");
    if (state_ > 1) {
        lastResult_ = kPending;
        if (c == 0) {
            c = 1;
        }
        connection_->sendGetLobbyListPackage(requestArg_, a, b & ~(b >> 31), c, query, d);
        StampRequest(kKindGetLobbyList);
        return;
    }
    RefuseRequest();
}

// 0x837e18 _ZN16GLXPlayerMPLobby13mpSendKickOutEPc
void GLXPlayerMPLobby::mpSendKickOut(char* name) {
    XP_DEBUG_OUT("GLXPlayerMPLobby::mpSendKickOut(){\n");
    if (state_ > 1) {
        lastResult_ = kPending;
        connection_->sendKickOutPackage(name);
        StampRequest(kKindKickOut);
        return;
    }
    RefuseRequest();
}

// 0x837ea8 _ZN16GLXPlayerMPLobby28mpSendCancelAutoMatchRequestEv
void GLXPlayerMPLobby::mpSendCancelAutoMatchRequest() {
    XP_DEBUG_OUT("GLXPlayerMPLobby::mpSendCancelAutoMatchRequest(){\n");
    if (state_ > 1) {
        lastResult_ = kPending;
        autoMatch_ = false;
        state_ = 2;
        connection_->sendCancelAutoMatchPackage();
        StampRequest(kKindCancelAutoMatch);
        return;
    }
    RefuseRequest();
}

// 0x837f40 _ZN16GLXPlayerMPLobby22mpSendAutoMatchRequestEPciS0_iss
void GLXPlayerMPLobby::mpSendAutoMatchRequest(char* a, std::int32_t b, char* c, std::int32_t d,
                                              std::int16_t e, std::int16_t f) {
    XP_DEBUG_OUT("GLXPlayerMPLobby::mpSendAutoMatchRequest(){\n");
    if (state_ > 1) {
        lastResult_ = kPending;
        autoMatch_ = true;
        connection_->sendAutoMatchPackage(a, b, c, d, e, f);
        pendingActive_ = false;
        return;
    }
    RefuseRequest();
}

// 0x837ff0 _ZN16GLXPlayerMPLobby21mpSendGetLobbyForNameEPc
void GLXPlayerMPLobby::mpSendGetLobbyForName(char* name) {
    XP_DEBUG_OUT("GLXPlayerMPLobby::mpSendGetLobbyForName(){\n");
    if (state_ > 1) {
        lastResult_ = kPending;
        connection_->sendGetLobbyForName(name);
        StampRequest(kKindGetLobbyForName);
        return;
    }
    RefuseRequest();
}

// 0x838080 _ZN16GLXPlayerMPLobby18mpSendGetLobbyInfoEi
void GLXPlayerMPLobby::mpSendGetLobbyInfo(std::int32_t lobbyId) {
    XP_DEBUG_OUT("GLXPlayerMPLobby::mpSendGetLobbyInfo(){\n");
    if (state_ > 1) {
        lastResult_ = kPending;
        connection_->sendGetLobbyInfo(lobbyId);
        StampRequest(kKindGetLobbyInfo);
        return;
    }
    RefuseRequest();
}

// 0x838110 _ZN16GLXPlayerMPLobby22mpSendSetUserParameterEPci
void GLXPlayerMPLobby::mpSendSetUserParameter(char* value, std::int32_t key) {
    XP_DEBUG_OUT("GLXPlayerMPLobby::mpSendSetUserParameter(){\n");
    if (state_ > 1) {
        lastResult_ = kPending;
        connection_->sendSetUserParameter(value, static_cast<std::int16_t>(key));
        StampRequest(kKindSetUserParameter);
        return;
    }
    RefuseRequest();
}

// 0x838248 _ZN16GLXPlayerMPLobby17mpSendRejoinLobbyEi
// The original does not print a closing trace on this path.
void GLXPlayerMPLobby::mpSendRejoinLobby(std::int32_t lobbyId) {
    XP_DEBUG_OUT("GLXPlayerMPLobby::mpSendRejoinLobby()\n");
    if (state_ > 1) {
        lastResult_ = kPending;
        connection_->sendRejoinLobbyPackage(lobbyId);
        StampRequest(kKindRejoinLobby);
        return;
    }
    RefuseRequest();
}

// 0x8382c8 _ZN16GLXPlayerMPLobby21mpSendSetPlayerStatusEh
void GLXPlayerMPLobby::mpSendSetPlayerStatus(std::uint8_t status) {
    XP_DEBUG_OUT("GLXPlayerMPLobby::mpSendSetPlayerStatus(){\n");
    if (state_ > 1) {
        lastResult_ = kPending;
        connection_->sendSetPlayerStatusPackage(status);
        StampRequest(kKindSetPlayerStatus);
        return;
    }
    RefuseRequest();
}

// 0x838358 _ZN16GLXPlayerMPLobby21mpSendLaunchLobbyGameEv
void GLXPlayerMPLobby::mpSendLaunchLobbyGame() {
    XP_DEBUG_OUT("GLXPlayerMPLobby::mpSendLaunchLobbyGame(){\n");
    if (state_ > 1) {
        lastResult_ = kPending;
        connection_->sendLunchLobbyGamePackage();
        StampRequest(kKindLaunchGame);
        return;
    }
    RefuseRequest();
}

// 0x8383e0 _ZN16GLXPlayerMPLobby16mpSendLeaveLobbyEv
void GLXPlayerMPLobby::mpSendLeaveLobby() {
    XP_DEBUG_OUT("GLXPlayerMPLobby::mpSendLeaveLobby(){\n");
    if (state_ > 1) {
        lastResult_ = kPending;
        connection_->sendLeaveLobbyPackage();
        StampRequest(kKindLeaveLobby);
        return;
    }
    RefuseRequest();
}

// 0x8381a8 _ZN16GLXPlayerMPLobby22mpSendSetGameParameterEPciP23CLobbyParameterAndQuery
void GLXPlayerMPLobby::mpSendSetGameParameter(char* name, std::int32_t value,
                                              CLobbyParameterAndQuery* query) {
    XP_DEBUG_OUT("GLXPlayerMPLobby::mpSendSetGameParameter(){\n");
    if (state_ > 1) {
        lastResult_ = kPending;
        connection_->sendSetGameParameter(name, static_cast<std::int16_t>(value), query);
        StampRequest(kKindSetGameParameter);
        return;
    }
    RefuseRequest();
}

// 0x8386f0 _ZN16GLXPlayerMPLobby15mpSendJoinLobbyEihPci
// The original sends first and sets the pending result after the send.
void GLXPlayerMPLobby::mpSendJoinLobby(std::int32_t lobbyId, std::uint8_t b, char* c, std::int32_t d) {
    XP_DEBUG_OUT("GLXPlayerMPLobby::mpSendJoinLobby(){\n");
    if (state_ > 1) {
        connection_->sendJoinLobbyPackage(lobbyId, b, c, d);
        lastResult_ = kPending;
        StampRequest(kKindJoinLobby);
        return;
    }
    RefuseRequest();
}

// 0x838528 _ZN16GLXPlayerMPLobby25mpSendJoinPredefinedLobbyEPciS0_iiRSt3mapIiSsSt4lessIiESaISt4pairIKiSsEEE
// Partial. When the state is above 1 the original makes a call that the
// decompiler marks as non-returning (an operator new of 0x40 bytes). That call is
// not identified, so only the refusal branch is kept.
void GLXPlayerMPLobby::mpSendJoinPredefinedLobby() {
    if (state_ <= 1) {
        RefuseRequest();
    }
}

// 0x838890 _ZN16GLXPlayerMPLobby30mpSendLobbyLoginWithGameCenterElPcihhS0_S0_
// No state check on this path.
void GLXPlayerMPLobby::mpSendLobbyLoginWithGameCenter(long a, char* b, std::int32_t c, std::uint8_t d,
                                                      std::uint8_t e, char* f, char* g) {
    XP_DEBUG_OUT("GLXPlayerMPLobby::mpSendLobbyLoginWithGameCenter\n");
    lastResult_ = kPending;
    connection_->sendLobbyLoginPackageWithGameCenter(a, b, static_cast<std::int16_t>(c), d, e, f, g);
    StampRequest(kKindLogin);
}

// 0x838928 _ZN16GLXPlayerMPLobby16mpSendLobbyLoginElPcih
// No state check on this path.
void GLXPlayerMPLobby::mpSendLobbyLogin(long a, char* b, std::int32_t c, std::uint8_t d) {
    XP_DEBUG_OUT("GLXPlayerMPLobby::mpSendLobbyLogin\n");
    lastResult_ = kPending;
    connection_->sendLobbyLoginPackage(a, b, static_cast<std::int16_t>(c), d);
    StampRequest(kKindLogin);
}

// 0x8389a8 _ZN16GLXPlayerMPLobby16mpSendDisconnectEv
// Partial. The virtual calls on this object (vtable slot 6) and on the observer
// (slot 3), and the ConnectionLobby virtual slot 0x14, are not identified, so
// those steps are omitted.
void GLXPlayerMPLobby::mpSendDisconnect() {
    if (!connection_) {
        return;
    }
    if (!Connection::isConnected()) {
        lastResult_ = 0;
        return;
    }
    lastResult_ = kPending;
    connection_->requestTimeMs = XP_API_GET_TIME();
    pendingKind_ = kKindDisconnect;
    pendingActive_ = true;
}

// 0x838bf0 _ZN16GLXPlayerMPLobby12mpDisconnectEv
void GLXPlayerMPLobby::mpDisconnect() {
    unknown88_ = -1;
    if (worker_) {
        ThreadStop();
        worker_.reset();
    }
    Connection::disconnect(connection_.get());
    aux68_.reset();
    state_ = 0;
}

// 0x839e7c _ZN16GLXPlayerMPLobby25mpSendEstablishConnectionEv
// Partial. When flag18_ is clear the original tears down the connection, sets the
// result to -1 and then creates a 0x2058-byte ConnectionLobby. The decompiler
// shows the allocation's result unused and marks the call non-returning, so the
// allocation is not reproduced.
void GLXPlayerMPLobby::mpSendEstablishConnection() {
    XP_DEBUG_OUT("GLXPlayerMPLobby::mpSendEstablishConnection(){\n");
    if (flag18_ == 0) {
        connection_.reset();
        lastResult_ = kPending;
        return;
    }
    lastResult_ = 0;
}

// 0x839f4c _ZN16GLXPlayerMPLobby10LoadConfigEv
// Partial. The original opens oconf.bar and parses it. The file loader is not a
// stand-in yet, so only the trace is printed.
void GLXPlayerMPLobby::LoadConfig() {
    XP_DEBUG_OUT("[GLXPlayerMPBase] load config - %s not found \n", "oconf.bar");
}
