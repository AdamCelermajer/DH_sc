// GameCenter: the Game Center session state (Apple's service). On the desktop and
// Android targets the feature switch is kept, and the Game Center calls are no-ops.
// Only the parts with checkable logic are reconstructed here. The room and friend
// calls (GCStartMultiplayerGame, GetJoinPlayerList, ...) are queued.
//
// Layout (original 32-bit offsets, kept as comments):
//   +0x07 features enabled (byte)
//   +0x24 player-ID list (std::list<std::string>; the original is an STL list with its
//         sentinel at +0x24)   +0x2c player count (int, set by GetPlayerCount)
//   +0x30 host UID (owned string)    +0x34 local user UID (owned string)
//   +0x4c Game Center player alias (std::string)
#pragma once

#include <cstdint>
#include <list>
#include <memory>
#include <string>

#include "net-session-online/deps/time_of_day.hpp"

class GameCenter {
public:
    // 0x499194 _ZN10GameCenter16EnableGCFeaturesEv
    void EnableGCFeatures() { featuresEnabled_ = true; }
    // 0x4991a0 _ZN10GameCenter17DisableGCFeaturesEv
    void DisableGCFeatures() { featuresEnabled_ = false; }
    // 0x4991ac _ZN10GameCenter15ConnectToServerEi: no-op.
    void ConnectToServer(std::int32_t value);
    // 0x4991b0 _ZN10GameCenter9GCFriendsEi: no-op.
    void GCFriends(std::int32_t value);
    // 0x4991b4 _ZN10GameCenter17ProcessGCAPIErrorEPKcS1_: no-op.
    void ProcessGCAPIError(const char* first, const char* second);
    // 0x4991b8 _ZN10GameCenter21GetGCPlayerAttributesEv: returns 0xffff0000.
    std::int32_t GetGCPlayerAttributes() const;
    // 0x4991c4 _ZN10GameCenter10GetUserUidEv
    const char* GetUserUid() const { return userUid_.get(); }
    // 0x4991cc _ZN10GameCenter10GetHostUidEv
    const char* GetHostUid() const { return hostUid_.get(); }
    // 0x499324 _ZN10GameCenter6IsHostEv: true when the host UID equals the user UID.
    // Partial: the original calls strcmp on both, so a null UID is undefined there. This
    // port treats a null UID as not the host.
    bool IsHost() const;

    // 0x4999b8 _ZN10GameCenter15ResetPlayerListEv: the list is replaced by the local user.
    // Partial in one case: a null user UID is taken as an empty string (the original would
    // construct a string from a null pointer).
    void ResetPlayerList();
    // 0x4991d4 _ZN10GameCenter14GetPlayerCountEv: counts the list, stores the count at +0x2c.
    std::int32_t GetPlayerCount();
    // 0x4995f4 _ZN10GameCenter16GetGCPlayerAliasEv: returns a copy of the alias.
    std::string GetGCPlayerAlias() const { return gcPlayerAlias_; }
    // 0x499618 _ZN10GameCenter16SetGCPlayerAliasEPc: stores the alias. A null alias is
    // taken as an empty string (partial, as for ResetPlayerList).
    void SetGCPlayerAlias(const char* alias);

    // 0x499208 _ZN10GameCenter22DumpPlayerIDStatusListEv: walks the player-ID status list
    // (+0x74). The loop has no body in the binary, so nothing happens.
    void DumpPlayerIDStatusList() const {}
    // 0x499224 _ZN10GameCenter17SaveGCAchievementEPc: appends the pointer to the achievement
    // list (+0x6c). Partial: the original stores the pointer, not a copy, and returns the new
    // list node, which is not exposed here. Allocation failure is not modelled.
    void SaveGCAchievement(const char* name);
    // 0x499264 _ZN10GameCenter17SetPlayerIDStatusEPKcS1_b: replaces the entry for the ID in
    // place, or appends one. Partial: the original returns a register value that is not
    // meaningful, so this returns nothing. A null status is taken as "" (the original would
    // duplicate a null pointer).
    void SetPlayerIDStatus(const char* id, const char* status, bool skip);
    // 0x499564 _ZN10GameCenter13ReportGCScoreEi. Partial: the original reads the gate flag
    // through GCSingleton<GameCenter>::GetInstance(); this port reads its own (+0x05). The
    // gate is not set anywhere in this port yet.
    void ReportGCScore(std::int32_t score);
    // 0x4995a0 _ZN10GameCenter19ReportGCAchievementEPc. Partial, as for ReportGCScore.
    void ReportGCAchievement(const char* name);
    // 0x499efc _ZN10GameCenter15CoinTossForHostEv. Partial: the list is sorted as in the
    // original, but the returned word is read from the STL node storage in the original and
    // is not reproduced. This returns 0.
    std::int32_t CoinTossForHost();
    // 0x49a03c _ZN10GameCenter21GCAddToJoinPlayerListEPKc. Partial: a null ID is taken as ""
    // (the original would construct a string from a null pointer).
    void GCAddToJoinPlayerList(const char* id);
    // 0x499440 _ZN10GameCenter25GCStartGameTimeoutExpiredEv. Partial: the original reads the
    // OnlineSingleton<OnlineGameState> instance, which is not ported. timevaldiff is called on a
    // default OnlineGameState here. That is the same only because timevaldiff reads its two
    // arguments and no member state (inferred from its signature; to be checked in the binary).
    bool GCStartGameTimeoutExpired() const;
    // 0x499f24 _ZN10GameCenter22GCStartMultiplayerGameEv. Partial: the OnlineGameState flag (+0x28
    // of the singleton), OnlineGameState::Init(4), and the MenuManager "GoToVerificationScreen"
    // callback are not ported (missing types). The coin-toss result is not stored: the original
    // writes the int into the host UID pointer slot (+0x30), which is a decompile type clash.
    // The original's return value is not reproduced (void here).
    void GCStartMultiplayerGame();
    // 0x499fc8 _ZN10GameCenter22GCStartMultiplayerGameEb. The bool argument is unused in the
    // original. It clears the start gate (+0x15) and then calls the no-argument overload.
    void GCStartMultiplayerGame(bool /*unused*/);
    // 0x499fe8 _ZN10GameCenter17GetJoinPlayerListEv. The original returns the list by value
    // (hidden result pointer); the copy is the same as the list at +0x24.
    std::list<std::string> GetJoinPlayerList() const { return playerList_; }

    // Test seams. Not in the original.
    bool StartFlagForTest() const { return startFlag_; }
    bool StartGateForTest() const { return startGate_; }
    void SetStartGateForTest(bool open) { startGate_ = open; }
    bool FeaturesEnabledForTest() const { return featuresEnabled_; }
    const std::list<std::string>& PlayerListForTest() const { return playerList_; }
    const std::list<const char*>& AchievementsForTest() const { return achievements_; }
    struct PlayerIdStatus {
        bool skip;
        std::string id;
        std::string status;
    };
    const std::list<PlayerIdStatus>& PlayerIdStatusForTest() const { return playerIdStatus_; }
    bool ScoreFlagForTest() const { return scoreFlag_; }
    bool AchievementFlagForTest() const { return achievementFlag_; }
    std::int32_t PendingScoreForTest() const { return pendingScore_; }
    const std::string& PendingAchievementForTest() const { return pendingAchievement_; }
    void SetGateForTest(bool open) { gateFlag_ = open; }
    void SetUidsForTest(const char* host, const char* user);

private:
    bool featuresEnabled_ = false;               // +0x07
    bool gateFlag_ = false;                      // +0x05 (role not identified)
    bool startFlag_ = false;                     // +0x11 set when a multiplayer start runs
    // +0x15: start gate, cleared by GCStartMultiplayerGame(bool). Its value at construction is
    // not recovered (the ctor is queued), so it defaults to false here.
    bool startGate_ = false;                     // +0x15
    bool achievementFlag_ = false;               // +0x0c
    bool scoreFlag_ = false;                     // +0x0d
    bool resultFlag_ = false;                    // +0x0e (cleared by both reports)
    std::unique_ptr<char[]> hostUid_;            // +0x30
    std::unique_ptr<char[]> userUid_;            // +0x34
    std::string pendingAchievement_;             // +0x38
    std::list<std::string> playerList_;          // +0x24
    std::int32_t playerCount_ = 0;               // +0x2c
    std::int32_t pendingScore_ = -1;             // +0x3c
    std::list<const char*> achievements_;        // +0x6c (list of pointers, not copies)
    std::list<PlayerIdStatus> playerIdStatus_;   // +0x74
    std::string gcPlayerAlias_;                  // +0x4c
    Timeval gameStartTime_{0, 0};                // +0x1c (sec, usec), set by GCStartMultiplayerGame
};
