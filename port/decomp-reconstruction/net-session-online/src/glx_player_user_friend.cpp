#include "net-session-online/glx_player_user_friend.hpp"

#include <cstddef>

namespace {
// Shared shape of the per-entry getters: a field is read only when its array is
// present and the index is within the count. The original checks index <= count
// (inclusive) for every getter except GetName, which uses index < count. The
// extra size check here stops reads past the end of the vector.
template <typename T>
T EntryOr(const std::vector<T>& field, std::int32_t count, std::int32_t index, bool inclusive,
          T fallback) {
    const bool inRange = index >= 0 && (inclusive ? index <= count : index < count);
    if (field.empty() || !inRange || static_cast<std::size_t>(index) >= field.size()) {
        return fallback;
    }
    return field[static_cast<std::size_t>(index)];
}
}  // namespace

// 0x82b9fc _ZN19GLXPlayerUserFriend13GetUserNumberEi
std::uint32_t GLXPlayerUserFriend::GetUserNumber(std::int32_t index) const {
    return EntryOr<std::uint32_t>(userNumber_, count_, index, true, 0u);
}

// 0x82ba2c _ZN19GLXPlayerUserFriend7GetNameEi
const char* GLXPlayerUserFriend::GetName(std::int32_t index) const {
    const bool inRange = index >= 0 && index < count_;
    if (name_.empty() || !inRange || static_cast<std::size_t>(index) >= name_.size()) {
        return nullptr;
    }
    return name_[static_cast<std::size_t>(index)].c_str();
}

// 0x82ba5c _ZN19GLXPlayerUserFriend10GetCountryEi
std::int32_t GLXPlayerUserFriend::GetCountry(std::int32_t index) const {
    return EntryOr<std::int32_t>(country_, count_, index, true, 0);
}

// 0x82ba8c _ZN19GLXPlayerUserFriend13GetReputationEi
double GLXPlayerUserFriend::GetReputation(std::int32_t index) const {
    return EntryOr<double>(reputation_, count_, index, true, -1.0);
}

// 0x82baf8 _ZN19GLXPlayerUserFriend22GetGoodReputationCountEi
std::int32_t GLXPlayerUserFriend::GetGoodReputationCount(std::int32_t index) const {
    return EntryOr<std::int32_t>(goodRep_, count_, index, true, -1);
}

// 0x82bac8 _ZN19GLXPlayerUserFriend21GetBadReputationCountEi
std::int32_t GLXPlayerUserFriend::GetBadReputationCount(std::int32_t index) const {
    return EntryOr<std::int32_t>(badRep_, count_, index, true, -1);
}

// 0x82bb28 _ZN19GLXPlayerUserFriend8GetStateEi
std::int32_t GLXPlayerUserFriend::GetState(std::int32_t index) const {
    return EntryOr<std::int32_t>(state_, count_, index, true, -1);
}

// 0x82bb58 _ZN19GLXPlayerUserFriend14GetCurrentGameEi
std::int32_t GLXPlayerUserFriend::GetCurrentGame(std::int32_t index) const {
    return EntryOr<std::int32_t>(currentGame_, count_, index, true, 0);
}

// 0x82bb88 _ZN19GLXPlayerUserFriend9GetTrophyEi
std::int32_t GLXPlayerUserFriend::GetTrophy(std::int32_t index) const {
    return EntryOr<std::int32_t>(trophy_, count_, index, true, -1);
}

// 0x82bbb8 _ZN19GLXPlayerUserFriend18GetTotalTrophyScoreEi
std::int32_t GLXPlayerUserFriend::GetTotalTrophyScore(std::int32_t index) const {
    return EntryOr<std::int32_t>(totalTrophy_, count_, index, true, -1);
}

// 0x82bbe8 _ZN19GLXPlayerUserFriend11GetLanguageEi
std::int32_t GLXPlayerUserFriend::GetLanguage(std::int32_t index) const {
    return EntryOr<std::int32_t>(language_, count_, index, true, 0);
}

// 0x82bc18 _ZN19GLXPlayerUserFriend19GetAvatarLastUpdateEi
std::int32_t GLXPlayerUserFriend::GetAvatarLastUpdate(std::int32_t index) const {
    return EntryOr<std::int32_t>(avatarLastUpdate_, count_, index, true, 0);
}

// 0x82bc48 _ZN19GLXPlayerUserFriend12GetAvatarKeyEi
std::int32_t GLXPlayerUserFriend::GetAvatarKey(std::int32_t index) const {
    return EntryOr<std::int32_t>(avatarKey_, count_, index, true, 0);
}

void GLXPlayerUserFriend::ReserveForTest(std::int32_t count) {
    count_ = count;
    const auto n = static_cast<std::size_t>(count);
    userNumber_.assign(n, 0);
    name_.assign(n, std::string());
    country_.assign(n, 0);
    reputation_.assign(n, 0.0);
    badRep_.assign(n, 0);
    goodRep_.assign(n, 0);
    state_.assign(n, 0);
    currentGame_.assign(n, 0);
    trophy_.assign(n, 0);
    language_.assign(n, 0);
    avatarKey_.assign(n, 0);
    avatarLastUpdate_.assign(n, 0);
    totalTrophy_.assign(n, 0);
}

void GLXPlayerUserFriend::SetNameForTest(std::int32_t index, const std::string& name) {
    name_[static_cast<std::size_t>(index)] = name;
}
