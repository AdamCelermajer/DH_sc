// GLXPlayerUserFriend: the friend list. Each per-entry field is a parallel array
// indexed by the friend index; the original stores them as raw heap arrays at
// the offsets listed below and checks the index against the count at +0x3c.
//
// Layout (original 32-bit offsets, kept as comments):
//   +0x3c count
//   +0x40 user number (uint)     +0x44 name (char*)          +0x48 country
//   +0x4c reputation (double)    +0x50 bad reputation count  +0x54 good reputation count
//   +0x58 state                  +0x5c current game          +0x60 trophy
//   +0x64 language               +0x68 avatar key            +0x6c avatar last update
//   +0x70 total trophy score
//
// Getters do not return an error for an out-of-range index: they return the
// default shown in each comment. The network calls (send*, process*) are not
// reconstructed yet.
#pragma once

#include <cstdint>
#include <string>
#include <vector>

class GLXPlayerUserFriend {
public:
    // 0x82b9f4 _ZN19GLXPlayerUserFriend15GetFriendsCountEv
    std::int32_t GetFriendsCount() const { return count_; }

    // 0x82b9fc GetUserNumber(int): default 0.
    std::uint32_t GetUserNumber(std::int32_t index) const;
    // 0x82ba2c GetName(int): default null. Index is strictly less than the count here.
    const char* GetName(std::int32_t index) const;
    // 0x82ba5c GetCountry(int): default 0.
    std::int32_t GetCountry(std::int32_t index) const;
    // 0x82ba8c GetReputation(int): default -1.0.
    double GetReputation(std::int32_t index) const;
    // 0x82baf8 GetGoodReputationCount(int): default -1.
    std::int32_t GetGoodReputationCount(std::int32_t index) const;
    // 0x82bac8 GetBadReputationCount(int): default -1.
    std::int32_t GetBadReputationCount(std::int32_t index) const;
    // 0x82bb28 GetState(int): default -1.
    std::int32_t GetState(std::int32_t index) const;
    // 0x82bb58 GetCurrentGame(int): default 0.
    std::int32_t GetCurrentGame(std::int32_t index) const;
    // 0x82bb88 GetTrophy(int): default -1.
    std::int32_t GetTrophy(std::int32_t index) const;
    // 0x82bbb8 GetTotalTrophyScore(int): default -1.
    std::int32_t GetTotalTrophyScore(std::int32_t index) const;
    // 0x82bbe8 GetLanguage(int): default 0.
    std::int32_t GetLanguage(std::int32_t index) const;
    // 0x82bc18 GetAvatarLastUpdate(int): default 0.
    std::int32_t GetAvatarLastUpdate(std::int32_t index) const;
    // 0x82bc48 GetAvatarKey(int): default 0.
    std::int32_t GetAvatarKey(std::int32_t index) const;

    // Not in the original: test seam. Sizes every per-entry array to count entries.
    void ReserveForTest(std::int32_t count);
    void SetNameForTest(std::int32_t index, const std::string& name);

private:
    std::int32_t count_ = 0;                  // +0x3c
    std::vector<std::uint32_t> userNumber_;   // +0x40
    std::vector<std::string> name_;           // +0x44
    std::vector<std::int32_t> country_;       // +0x48
    std::vector<double> reputation_;          // +0x4c
    std::vector<std::int32_t> badRep_;        // +0x50
    std::vector<std::int32_t> goodRep_;       // +0x54
    std::vector<std::int32_t> state_;         // +0x58
    std::vector<std::int32_t> currentGame_;   // +0x5c
    std::vector<std::int32_t> trophy_;        // +0x60
    std::vector<std::int32_t> language_;      // +0x64
    std::vector<std::int32_t> avatarKey_;     // +0x68
    std::vector<std::int32_t> avatarLastUpdate_;  // +0x6c
    std::vector<std::int32_t> totalTrophy_;   // +0x70
};
