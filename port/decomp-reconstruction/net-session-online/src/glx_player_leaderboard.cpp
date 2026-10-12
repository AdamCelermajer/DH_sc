#include "net-session-online/glx_player_leaderboard.hpp"

#include <cstdio>

namespace {
// The original's default score for a missing entry (0xfff5d3d6 as an unsigned word).
constexpr std::int32_t kMissingScore = static_cast<std::int32_t>(0xfff5d3d6u);

// The original reads entry index only when index < count and the array is allocated.
template <typename T>
bool EntryPresent(const std::vector<T>& list, std::int32_t index, std::int32_t count) {
    return index >= 0 && index < count && static_cast<std::size_t>(index) < list.size();
}
}  // namespace

// 0x8321b8 _ZN20GLXPlayerLeaderboard18getLeaderboardSizeEv
std::int32_t GLXPlayerLeaderboard::getLeaderboardSize() const {
    if (names_.empty()) {
        return -1;
    }
    return count_;
}

// 0x83223c _ZN20GLXPlayerLeaderboard29getLeaderboardEntryPlayerNameEi
const char* GLXPlayerLeaderboard::getLeaderboardEntryPlayerName(std::int32_t index) const {
    if (EntryPresent(names_, index, count_)) {
        return names_[static_cast<std::size_t>(index)].c_str();
    }
    return nullptr;
}

// 0x832264 _ZN20GLXPlayerLeaderboard32getLeaderboardEntryPlayerCountryEi
std::int32_t GLXPlayerLeaderboard::getLeaderboardEntryPlayerCountry(std::int32_t index) const {
    if (EntryPresent(countries_, index, count_)) {
        return countries_[static_cast<std::size_t>(index)];
    }
    return 0;
}

// 0x83228c _ZN20GLXPlayerLeaderboard33getLeaderboardEntryPlayerPositionEi
std::int32_t GLXPlayerLeaderboard::getLeaderboardEntryPlayerPosition(std::int32_t index) const {
    if (EntryPresent(positions_, index, count_)) {
        return positions_[static_cast<std::size_t>(index)];
    }
    return -1;
}

// 0x8322b4 _ZN20GLXPlayerLeaderboard30getLeaderboardEntryPlayerScoreEi
std::int32_t GLXPlayerLeaderboard::getLeaderboardEntryPlayerScore(std::int32_t index) const {
    if (EntryPresent(scores_, index, count_)) {
        return scores_[static_cast<std::size_t>(index)];
    }
    return kMissingScore;
}

// 0x8322e0 _ZN20GLXPlayerLeaderboard34getLeaderboardEntryPlayerScoreDataEi
std::int32_t GLXPlayerLeaderboard::getLeaderboardEntryPlayerScoreData(std::int32_t index) const {
    if (EntryPresent(scoreData_, index, count_)) {
        return scoreData_[static_cast<std::size_t>(index)];
    }
    return 0;
}

void GLXPlayerLeaderboard::SetEntryForTest(std::int32_t index, const char* name, std::int32_t country,
                                           std::int32_t position, std::int32_t score,
                                           std::int32_t scoreData) {
    const auto i = static_cast<std::size_t>(index);
    if (names_.size() <= i) names_.resize(i + 1);
    if (countries_.size() <= i) countries_.resize(i + 1);
    if (positions_.size() <= i) positions_.resize(i + 1);
    if (scores_.size() <= i) scores_.resize(i + 1);
    if (scoreData_.size() <= i) scoreData_.resize(i + 1);
    names_[i] = name != nullptr ? name : "";
    countries_[i] = country;
    positions_[i] = position;
    scores_[i] = score;
    scoreData_[i] = scoreData;
}

void GLXPlayerLeaderboard::SetCurrentForTest(std::int32_t position, std::int32_t country,
                                             std::int32_t score, std::int32_t scoreData) {
    currentPosition_ = position;
    currentCountry_ = country;
    currentScore_ = score;
    currentScoreData_ = scoreData;
}

// 0x832c2c _ZN20GLXPlayerLeaderboard13addScoreEntryEPciiiPii
std::int32_t GLXPlayerLeaderboard::addScoreEntry(std::string& buffer, std::int32_t score,
                                                 std::int32_t level, std::int32_t time,
                                                 const std::int32_t* scores,
                                                 std::int32_t count) const {
    char segment[128];
    std::int32_t written = std::snprintf(segment, sizeof(segment), "l|%d|t|%d|sl|0|s|%d|",
                                         level, time, score);
    buffer += segment;
    if (count > 0) {
        for (std::int32_t i = 0; i != count;) {
            const std::int32_t value = scores[i++];
            written = std::snprintf(segment, sizeof(segment), "l|%d|t|%d|sl|%d|s|%d|",
                                    level, time, i, value);
            buffer += segment;
        }
    }
    return written;
}
