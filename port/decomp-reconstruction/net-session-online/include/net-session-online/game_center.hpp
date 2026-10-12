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

    // Test seams. Not in the original.
    bool FeaturesEnabledForTest() const { return featuresEnabled_; }
    const std::list<std::string>& PlayerListForTest() const { return playerList_; }
    void SetUidsForTest(const char* host, const char* user);

private:
    bool featuresEnabled_ = false;               // +0x07
    std::unique_ptr<char[]> hostUid_;            // +0x30
    std::unique_ptr<char[]> userUid_;            // +0x34
    std::list<std::string> playerList_;          // +0x24
    std::int32_t playerCount_ = 0;               // +0x2c
    std::string gcPlayerAlias_;                  // +0x4c
};
