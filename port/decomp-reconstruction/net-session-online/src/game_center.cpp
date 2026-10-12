#include "net-session-online/game_center.hpp"

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
