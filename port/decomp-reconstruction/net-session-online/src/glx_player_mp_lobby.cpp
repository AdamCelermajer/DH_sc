#include "net-session-online/glx_player_mp_lobby.hpp"

#include <cstdlib>
#include <cstring>
#include <map>
#include <string>
#include <vector>

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
constexpr std::int32_t kKindCreateLobby = 5;

// Packs the friend names the way the original builds them: friends are visited from the last
// to the first, and each name is written as a 2-byte big-endian length followed by its bytes.
// The count is capped at 30 and returned through count. An empty list gives an empty buffer.
std::vector<char> PackFriendNames(GLXPlayerUserFriend* friends, std::int32_t& count) {
    count = friends->GetFriendsCount();
    if (count >= kFriendListCap) {
        count = kFriendListCap;
    }
    std::vector<char> packed;
    for (std::int32_t i = count - 1; i >= 0; --i) {
        const char* name = friends->GetName(i);
        if (name == nullptr) {
            continue;
        }
        const std::size_t length = std::strlen(name);
        packed.push_back(static_cast<char>((length >> 8) & 0xff));
        packed.push_back(static_cast<char>(length & 0xff));
        packed.insert(packed.end(), name, name + length);
    }
    return packed;
}
}  // namespace

// 0x83a64c _ZN16GLXPlayerMPLobbyC1EP23GLXPlayerMPBaseObserverPc (C2 0x83a6c8, same body)
// Base ctor, then the config text is copied into an owned string at +0x7c (new[] copy; a null
// config leaves it null), then Init.
GLXPlayerMPLobby::GLXPlayerMPLobby(GLXPlayerMPBaseObserver* observer, const char* config)
    : GLXPlayerMPBase(observer) {
    if (config != nullptr) {
        const std::size_t length = std::strlen(config);
        config_.reset(new char[length + 1]);
        std::memcpy(config_.get(), config, length + 1);
    }
    Init();
}

// 0x83a4f0 _ZN16GLXPlayerMPLobbyD1Ev, 0x83a5ac _ZN16GLXPlayerMPLobbyD2Ev (same body)
// Frees the owned address text at +0x58 (std::free, as the original's LC_API_FREE_0; the port
// never allocates it, LoadConfig is partial), then mpDisconnect and the connection release. The
// owned config text at +0x7c is released by its unique_ptr member after this body.
GLXPlayerMPLobby::~GLXPlayerMPLobby() {
    if (field58_ != nullptr) {
        std::free(field58_);
        field58_ = nullptr;
    }
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

// Refused requests call observer vtable slot 0, which is OnMPFinishGameSuccess. Checked against the
// binary: the refusal in mpSendJoinPredefinedLobby (0x838528) calls the observer's slot 0 with no
// argument, and sets the result to 0x32 first.
void GLXPlayerMPLobby::RefuseRequest() {
    lastResult_ = kRefused;
    observer_->OnMPFinishGameSuccess();
}

void GLXPlayerMPLobby::StampRequest(std::int32_t kind) {
    connection_->SetRequestTime(XP_API_GET_TIME());
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
// Packs the friend names and forwards them to the overload above. This path does not
// stamp the request: the overload does.
void GLXPlayerMPLobby::mpSendGetLobbyForFriendWithGameParam(std::int32_t a, std::uint8_t b,
                                                            GLXPlayerUserFriend* friends,
                                                            CLobbyParameterAndQuery* query) {
    XP_DEBUG_OUT("GLXPlayerMPLobby::mpSendGetLobbyForFriendWithGameParam(){\n");
    if (state_ <= 1) {
        RefuseRequest();
        return;
    }
    lastResult_ = kPending;
    std::int32_t count = 0;
    std::vector<char> packed = PackFriendNames(friends, count);
    mpSendGetLobbyForFriendWithGameParam(a, b, count, packed.empty() ? nullptr : packed.data(),
                                         static_cast<std::int32_t>(packed.size()), query);
    XP_DEBUG_OUT("}\n");
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

// 0x837a88 _ZN16GLXPlayerMPLobby23mpSendGetLobbyForFriendEihP19GLXPlayerUserFriend
// Packs the friend names and sends them with the count, then stamps the request.
void GLXPlayerMPLobby::mpSendGetLobbyForFriend(std::int32_t a, std::uint8_t b, GLXPlayerUserFriend* friends) {
    XP_DEBUG_OUT("GLXPlayerMPLobby::mpSendGetLobbyForName(){\n");
    if (state_ <= 1) {
        RefuseRequest();
        return;
    }
    lastResult_ = kPending;
    std::int32_t count = 0;
    std::vector<char> packed = PackFriendNames(friends, count);
    connection_->sendGetLobbyForFriend(a, b, static_cast<std::uint8_t>(count),
                                       packed.empty() ? nullptr : packed.data(),
                                       static_cast<std::int32_t>(packed.size()));
    StampRequest(kKindGetLobbyForFriend);
    XP_DEBUG_OUT("}\n");
}

// 0x8387a4 _ZN16GLXPlayerMPLobby17mpSendCreateLobbyEPchhiS0_iS0_iP23CLobbyParameterAndQuery
void GLXPlayerMPLobby::mpSendCreateLobby(char* a, std::uint8_t b, std::uint8_t c, std::int32_t d, char* e,
                                         std::int32_t f, char* g, std::int32_t h, CLobbyParameterAndQuery* q) {
    XP_DEBUG_OUT("GLXPlayerMPLobby::mpSendCreateLobby(){\n");
    if (state_ <= 1) {
        RefuseRequest();
        return;
    }
    lastResult_ = kPending;
    connection_->sendCreateLobbyPackage(requestArg_, a, b, c, d, e, f, g, h, q);
    StampRequest(kKindCreateLobby);
    XP_DEBUG_OUT("}\n");
}

// 0x83a744 _ZN16GLXPlayerMPLobby31mpSendCreateLobbyWithGameCenterEPchhiS0_iS0_iP23CLobbyParameterAndQueryRSt4listISsSaISsEE
// The name list is copied node by node, as the original does, and the copy is released
// after the send.
void GLXPlayerMPLobby::mpSendCreateLobbyWithGameCenter(char* a, std::uint8_t b, std::uint8_t c,
                                                       std::int32_t d, char* e, std::int32_t f, char* g,
                                                       std::int32_t h, CLobbyParameterAndQuery* q,
                                                       std::list<std::string>& names) {
    XP_DEBUG_OUT("GLXPlayerMPLobby::mpSendCreateLobbyWithGameCenter(){\n");
    if (state_ <= 1) {
        RefuseRequest();
        return;
    }
    lastResult_ = kPending;
    const std::list<std::string> copy(names);
    connection_->sendCreateLobbyPackageWithGameCenter(requestArg_, a, b, c, d, e, f, g, h, q, copy);
    StampRequest(kKindCreateLobby);
    XP_DEBUG_OUT("}\n");
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
// The map is walked in key order (the original's in-order tree walk). For each entry the query
// block gets AddQueryCondition(key, 2, text) (vtable +0x0c) and the parameter block gets
// SetParameterValue(key, text) (vtable +0x08). The connection then sends the two blocks, the
// request time is stamped, and the pending kind is 14. The state check and the refusal
// (result 0x32, observer slot 0) are as the original. The map value is read as the string's
// c_str(); the original reads the same character data from the node.
void GLXPlayerMPLobby::mpSendJoinPredefinedLobby(char* a, std::int32_t b, char* c, std::int32_t d,
                                                 std::int32_t e,
                                                 std::map<std::int32_t, std::string>& names) {
    XP_DEBUG_OUT("GLXPlayerMPLobby::mpSendJoinPredefinedLobby(){\n");
    if (state_ <= 1) {
        RefuseRequest();
        return;
    }
    // queryCondition is v12 in the original (sent as a7) and parameterValues is v11 (sent as a8).
    CLobbyParameterAndQuery queryCondition;
    CLobbyParameterAndQuery parameterValues;
    for (const auto& entry : names) {
        queryCondition.AddQueryCondition(entry.first, 2, entry.second.c_str());
        parameterValues.SetParameterValue(entry.first, entry.second.c_str());
    }
    connection_->sendJoinPredefinedLobbyPackage(a, b, c, d, e, &queryCondition, &parameterValues);
    lastResult_ = kPending;
    StampRequest(kKindJoinLobby);
    XP_DEBUG_OUT("}\n");
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
// Connected: result -1, the connection's finish package (Connection vtable +0x14,
// sendFinishConnectionPackage), request time, pending kind 16. Not connected: mpDisconnect (this
// object's vtable +0x18), then result 0.
// Partial: the original then calls observer slot 3 (OnMPSetParamSuccess, an int argument). The
// pseudocode does not show that argument (r1 is not set before the call), so the call is not made.
// The original returns the observer call's result; the port's void return drops it.
void GLXPlayerMPLobby::mpSendDisconnect() {
    if (!connection_) {
        return;
    }
    if (connection_->isConnected()) {
        lastResult_ = kPending;
        connection_->sendFinishConnectionPackage();
        connection_->SetRequestTime(XP_API_GET_TIME());
        pendingKind_ = kKindDisconnect;
        pendingActive_ = true;
        return;
    }
    mpDisconnect();
    lastResult_ = 0;
}

// 0x838bf0 _ZN16GLXPlayerMPLobby12mpDisconnectEv
void GLXPlayerMPLobby::mpDisconnect() {
    unknown88_ = -1;
    if (worker_) {
        ThreadStop();
        worker_.reset();
    }
    connection_->disconnect();
    aux68_.reset();
    state_ = 0;
}

// 0x839e7c _ZN16GLXPlayerMPLobby25mpSendEstablishConnectionEv
// The state byte (+0x60) selects the branch. Non-zero: result 0 and observer slot 2
// (OnMPLeaveGameSuccess, no argument). Zero: an existing connection is torn down through mpDisconnect
// (this object's vtable +0x18) and freed, the result becomes -1, a new ConnectionLobby is built on
// the config host (+0x58) and port (+0x5c), it is named from the +0x08 name text, and the flag at
// +0x6c is set. The original's 0x2058-byte allocation is the ConnectionLobby size; the port uses
// the object's own size. The original returns the observer call's or the trace's result; the port's
// void return drops it.
void GLXPlayerMPLobby::mpSendEstablishConnection() {
    XP_DEBUG_OUT("GLXPlayerMPLobby::mpSendEstablishConnection(){\n");
    if (state_ != 0) {
        lastResult_ = 0;
        observer_->OnMPLeaveGameSuccess();
        return;
    }
    if (connection_) {
        mpDisconnect();
        connection_.reset();
    }
    lastResult_ = kPending;
    connection_ = std::make_unique<ConnectionLobby>(field58_, field5c_);
    connection_->setName(field08_);
    field6c_ = 1;
    XP_DEBUG_OUT("}\n");
}

// 0x839f4c _ZN16GLXPlayerMPLobby10LoadConfigEv
// Partial: not recovered. The original opens oconf.bar (XP_API_FILE_OPEN, the whole file read,
// then one key:value line per entry). GGI is stored at +0x48 with atoi. XPlayMPURL is split into
// the address (owned, +0x58) and the port (atoi, +0x5c), and is read only when the config text at
// +0x7c is null. A second parse of the config text at +0x7c runs when the file is missing (not
// recovered). The port prints only the not-found trace, so +0x58 and +0x5c stay zero.
void GLXPlayerMPLobby::LoadConfig() {
    XP_DEBUG_OUT("[GLXPlayerMPBase] load config - %s not found \n", "oconf.bar");
}
