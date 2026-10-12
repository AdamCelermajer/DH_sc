#include "net-session-online/game_center.hpp"
#include "net-session-online/online_game_state.hpp"

#include <cstring>

// 0x4991ac _ZN10GameCenter15ConnectToServerEi (bx lr)
void GameCenter::ConnectToServer(std::int32_t /*value*/) {}

// 0x4991b0 _ZN10GameCenter9GCFriendsEi (bx lr)
void GameCenter::GCFriends(std::int32_t /*value*/) {}

// 0x4991b4 _ZN10GameCenter17ProcessGCAPIErrorEPKcS1_ (bx lr)
void GameCenter::ProcessGCAPIError(const char* /*first*/, const char* /*second*/) {}

// 0x4991b8 _ZN10GameCenter21GetGCPlayerAttributesEv
std::int32_t GameCenter::GetGCPlayerAttributes() const {
    return static_cast<std::int32_t>(0xffff0000u);
}

// 0x499324 _ZN10GameCenter6IsHostEv
bool GameCenter::IsHost() const {
    if (hostUid_ == nullptr || userUid_ == nullptr) {
        return false;
    }
    return std::strcmp(hostUid_.get(), userUid_.get()) == 0;
}

void GameCenter::SetUidsForTest(const char* host, const char* user) {
    auto copy = [](const char* text) -> std::unique_ptr<char[]> {
        if (text == nullptr) {
            return nullptr;
        }
        const std::size_t length = std::strlen(text) + 1;
        std::unique_ptr<char[]> out(new char[length]);
        std::memcpy(out.get(), text, length);
        return out;
    };
    hostUid_ = copy(host);
    userUid_ = copy(user);
}

// 0x4999b8 _ZN10GameCenter15ResetPlayerListEv
void GameCenter::ResetPlayerList() {
    playerList_.clear();
    playerList_.emplace_back(userUid_ != nullptr ? userUid_.get() : "");
}

// 0x4991d4 _ZN10GameCenter14GetPlayerCountEv
std::int32_t GameCenter::GetPlayerCount() {
    playerCount_ = static_cast<std::int32_t>(playerList_.size());
    return playerCount_;
}

// 0x499618 _ZN10GameCenter16SetGCPlayerAliasEPc
void GameCenter::SetGCPlayerAlias(const char* alias) {
    gcPlayerAlias_ = alias != nullptr ? alias : "";
}

// 0x499224 _ZN10GameCenter17SaveGCAchievementEPc
void GameCenter::SaveGCAchievement(const char* name) { achievements_.push_back(name); }

// 0x499264 _ZN10GameCenter17SetPlayerIDStatusEPKcS1_b
void GameCenter::SetPlayerIDStatus(const char* id, const char* status, bool skip) {
    if (id == nullptr) {
        return;
    }
    PlayerIdStatus entry{skip, id, status != nullptr ? status : ""};
    for (auto it = playerIdStatus_.begin(); it != playerIdStatus_.end(); ++it) {
        if (std::strcmp(id, it->id.c_str()) == 0) {
            *it = entry;  // the original links the new node in the old node's place
            return;
        }
    }
    playerIdStatus_.push_back(entry);
}

// 0x499564 _ZN10GameCenter13ReportGCScoreEi
void GameCenter::ReportGCScore(std::int32_t score) {
    if (!gateFlag_) {
        return;
    }
    pendingScore_ = score;
    scoreFlag_ = true;
    resultFlag_ = false;
}

// 0x4995a0 _ZN10GameCenter19ReportGCAchievementEPc
void GameCenter::ReportGCAchievement(const char* name) {
    if (!gateFlag_) {
        return;
    }
    pendingAchievement_ = name != nullptr ? name : "";
    achievementFlag_ = true;
    resultFlag_ = false;
}

// 0x499efc _ZN10GameCenter15CoinTossForHostEv
std::int32_t GameCenter::CoinTossForHost() {
    playerList_.sort();
    return 0;  // partial: the returned word is not reproduced (see game_center.hpp)
}

// 0x49a03c _ZN10GameCenter21GCAddToJoinPlayerListEPKc
void GameCenter::GCAddToJoinPlayerList(const char* id) {
    playerList_.emplace_back(id != nullptr ? id : "");
}

// 0x499440 _ZN10GameCenter25GCStartGameTimeoutExpiredEv
// Partial: see game_center.hpp. The elapsed time is compared with 150000 ms.
bool GameCenter::GCStartGameTimeoutExpired() const {
    Timeval now{0, 0};
    GetTimeOfDay(&now);
    const OnlineGameState stateForTimeDiff{};
    return stateForTimeDiff.timevaldiff(&gameStartTime_, &now) > 150000;
}

// 0x499f24 _ZN10GameCenter22GCStartMultiplayerGameEv
// Partial: see game_center.hpp. Only the GameCenter-owned steps run here.
void GameCenter::GCStartMultiplayerGame() {
    if (startGate_) {
        return;
    }
    startFlag_ = true;
    GetTimeOfDay(&gameStartTime_);
    CoinTossForHost();  // partial: the returned word is not stored (see game_center.hpp)
    DumpPlayerIDStatusList();
    // Not ported: OnlineGameState flag (+0x28 of the singleton), the GoToVerificationScreen
    // callback, and OnlineGameState::Init(4).
}

// 0x499fc8 _ZN10GameCenter22GCStartMultiplayerGameEb
void GameCenter::GCStartMultiplayerGame(bool /*unused*/) {
    startGate_ = false;
    GCStartMultiplayerGame();
}
