// GLXPlayerLeaderboard: the leaderboard and rank lookup of the GLX player services.
// The web component fills parallel arrays, one entry per player, and the current
// player's own row. Only the getters are reconstructed here. The request senders,
// the response parsers and getLeaderboardEntry (its out-parameter types are not
// settled) are queued.
//
// Layout (original 32-bit offsets, kept as comments; the GLXPlayerWebComponent base
// occupies +0x00..+0x3b):
//   +0x3c entry count (int)                  +0x40 player names (char* array)
//   +0x44 player countries (int array)       +0x48 player positions (int array)
//   +0x4c player scores (int array)          +0x50 player score data (int array)
//   +0x54 score data count (int)
//   +0x58 current player position            +0x5c current player score
//   +0x60 current player score data          +0x64 current player country
#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "net-session-online/glx_player_web_component.hpp"

class GLXPlayerLeaderboard : public GLXPlayerWebComponent {
public:
    // Not a method in the original: the owner writes +0x54 directly (CXPlayerManager::OnRequestSuccess).
    void SetScoreDataCount(std::int32_t count) { scoreDataCount_ = count; }
    // 0x832198 _ZN20GLXPlayerLeaderboard35getCurrentPlayerLeaderboardPositionEv
    std::int32_t getCurrentPlayerLeaderboardPosition() const { return currentPosition_; }
    // 0x8321a0 _ZN20GLXPlayerLeaderboard34getCurrentPlayerLeaderboardCountryEv
    std::int32_t getCurrentPlayerLeaderboardCountry() const { return currentCountry_; }
    // 0x8321a8 _ZN20GLXPlayerLeaderboard32getCurrentPlayerLeaderboardScoreEv
    std::int32_t getCurrentPlayerLeaderboardScore() const { return currentScore_; }
    // 0x8321b0 _ZN20GLXPlayerLeaderboard36getCurrentPlayerLeaderboardScoreDataEv
    std::int32_t getCurrentPlayerLeaderboardScoreData() const { return currentScoreData_; }
    // 0x8321b8 _ZN20GLXPlayerLeaderboard18getLeaderboardSizeEv: -1 when no names are held.
    std::int32_t getLeaderboardSize() const;
    // 0x83223c _ZN20GLXPlayerLeaderboard29getLeaderboardEntryPlayerNameEi: default null.
    const char* getLeaderboardEntryPlayerName(std::int32_t index) const;
    // 0x832264 _ZN20GLXPlayerLeaderboard32getLeaderboardEntryPlayerCountryEi: default 0.
    std::int32_t getLeaderboardEntryPlayerCountry(std::int32_t index) const;
    // 0x83228c _ZN20GLXPlayerLeaderboard33getLeaderboardEntryPlayerPositionEi: default -1.
    std::int32_t getLeaderboardEntryPlayerPosition(std::int32_t index) const;
    // 0x8322b4 _ZN20GLXPlayerLeaderboard30getLeaderboardEntryPlayerScoreEi: default 0xfff5d3d6.
    std::int32_t getLeaderboardEntryPlayerScore(std::int32_t index) const;
    // 0x8322e0 _ZN20GLXPlayerLeaderboard34getLeaderboardEntryPlayerScoreDataEi: default 0.
    std::int32_t getLeaderboardEntryPlayerScoreData(std::int32_t index) const;

    // Request senders: "f|kind|i|GGI|u|UID|..." requests handed to SendByGet.
    // 0x832cc4 _ZN20GLXPlayerLeaderboard23sendRankGetAroundPlayerEiii (kind 0x70). Partial: the
    // optional field taken when the first argument is not negative is not identified.
    void sendRankGetAroundPlayer(std::int32_t first, std::int32_t page, std::int32_t time);
    // 0x832dc4 _ZN20GLXPlayerLeaderboard11sendRankGetEiiiib (kind 0x6f). Partial: the optional
    // field taken when the first argument is not negative is not identified.
    void sendRankGet(std::int32_t first, std::int32_t x, std::int32_t page, std::int32_t time, bool withScores);
    // 0x832ee4 _ZN20GLXPlayerLeaderboard13sendHighScoreEPc (kind 0x6e). Partial: the appended
    // field (the argument) uses a format that is not identified, so only the base request is sent.
    void sendHighScore(const char* score);
    // 0x832fb8 _ZN20GLXPlayerLeaderboard13sendHighScoreEiii (kind 0x6e). s is the score, l the
    // level (sent only when not negative) and t the time.
    void sendHighScore(std::int32_t s, std::int32_t l, std::int32_t t);

    // Not in the original: test seams that fill the parsed lists.
    void SetEntryForTest(std::int32_t index, const char* name, std::int32_t country,
                         std::int32_t position, std::int32_t score, std::int32_t scoreData);
    void SetCountForTest(std::int32_t count) { count_ = count; }
    void SetCurrentForTest(std::int32_t position, std::int32_t country, std::int32_t score,
                           std::int32_t scoreData);

private:
    std::int32_t count_ = 0;                    // +0x3c
    std::vector<std::string> names_;            // +0x40
    std::vector<std::int32_t> countries_;       // +0x44
    std::vector<std::int32_t> positions_;       // +0x48
    std::vector<std::int32_t> scores_;          // +0x4c
    std::vector<std::int32_t> scoreData_;       // +0x50
    std::int32_t scoreDataCount_ = 0;           // +0x54
    std::int32_t currentPosition_ = 0;          // +0x58
    std::int32_t currentScore_ = 0;             // +0x5c
    std::int32_t currentScoreData_ = 0;         // +0x60
    std::int32_t currentCountry_ = 0;           // +0x64
    bool withScores_ = false;                   // +0x68 (set by sendRankGet; name not identified)
};
