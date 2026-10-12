#include "net-session-online/glx_player_user.hpp"

#include <cstddef>
#include <cstring>

#include "net-session-online/owned_text.hpp"

namespace {
// Bounds shared by the indexed getters: index is at least 0 and below the size of
// the stored list. The avatar getters also check the avatar count (+0x114).
bool InRange(std::int32_t index, std::size_t size) {
    return index >= 0 && static_cast<std::size_t>(index) < size;
}
}  // namespace

// 0x83f8a4 _ZN13GLXPlayerUserC2Ev, 0x83f754 _ZN13GLXPlayerUserC1Ev
// Partial: the original allocates a 0x428-byte request client (ctor 0x82deac) into
// +0x20 and passes it the strings at +0x10..+0x18. That allocation is not
// reconstructed. The member defaults match the original's zeroing, -1 values included.
GLXPlayerUser::GLXPlayerUser() : GLXPlayerWebComponent() { GLXPlayerWebComponent::LoadConfig(); }

// 0x83fb24 _ZN13GLXPlayerUserD1Ev, 0x83fda0 _ZN13GLXPlayerUserD2Ev
// Partial: the original also destroys the vector<channel> pair at +0xe8 and +0xf8, and the
// field at +0x3c, which are not modelled here. The _LiveFeed array at +0x12c is released by
// its vector member.
GLXPlayerUser::~GLXPlayerUser() = default;

// 0x83a880 _ZN13GLXPlayerUser11getUserNameEv
const char* GLXPlayerUser::getUserName() const { return userName_.get(); }

// 0x83a888 _ZN13GLXPlayerUser11getPasswordEv
const char* GLXPlayerUser::getPassword() const { return password_.get(); }

// 0x83a890 _ZN13GLXPlayerUser8getEmailEv
const char* GLXPlayerUser::getEmail() const { return email_.get(); }

// 0x83a898 _ZN13GLXPlayerUser12getEmailFlagEv
std::uint8_t GLXPlayerUser::getEmailFlag() const { return emailFlag_; }

// 0x83a8a0 _ZN13GLXPlayerUser10getCountryEv
const char* GLXPlayerUser::getCountry() const { return country_.get(); }

// 0x83a8a8 _ZN13GLXPlayerUser12getFirstNameEv
const char* GLXPlayerUser::getFirstName() const { return firstName_.get(); }

// 0x83a8b0 _ZN13GLXPlayerUser11getLastNameEv
const char* GLXPlayerUser::getLastName() const { return lastName_.get(); }

// 0x83a8b8 _ZN13GLXPlayerUser6getSexEv
std::uint8_t GLXPlayerUser::getSex() const { return sex_; }

// 0x83a8c0 _ZN13GLXPlayerUser11getBirthdayEv
const char* GLXPlayerUser::getBirthday() const { return birthday_.get(); }

// 0x83a8c8 _ZN13GLXPlayerUser13getOnlineTimeEv
std::int32_t GLXPlayerUser::getOnlineTime() const { return onlineTime_; }

// 0x83a8d0 _ZN13GLXPlayerUser14getTotalTrophyEv
std::int32_t GLXPlayerUser::getTotalTrophy() const { return totalTrophy_; }

// 0x83a8d8 _ZN13GLXPlayerUser21getNumberUserGameListEv
std::int32_t GLXPlayerUser::getNumberUserGameList() const { return gameCount_; }

// 0x83a8e0 _ZN13GLXPlayerUser14getUserGamePIDEi
// The original returns the stored pointer for any index from 0 up, without a count check.
// The port returns nullptr outside the stored vector.
const char* GLXPlayerUser::getUserGamePID(std::int32_t index) const {
    if (!InRange(index, gamePid_.size())) {
        return nullptr;
    }
    return gamePid_[static_cast<std::size_t>(index)].get();
}

// 0x83a908 _ZN13GLXPlayerUser15getUserGameNameEi
const char* GLXPlayerUser::getUserGameName(std::int32_t index) const {
    if (!InRange(index, gameName_.size())) {
        return nullptr;
    }
    return gameName_[static_cast<std::size_t>(index)].get();
}

// 0x83a930 _ZN13GLXPlayerUser17getUserGameTrophyEi
// Default -1 for a negative index or an empty list.
std::int32_t GLXPlayerUser::getUserGameTrophy(std::int32_t index) const {
    if (!InRange(index, gameTrophy_.size())) {
        return -1;
    }
    return gameTrophy_[static_cast<std::size_t>(index)];
}

// 0x83a954 _ZN13GLXPlayerUser19getCurrentUserStateEv
std::int32_t GLXPlayerUser::getCurrentUserState() const { return currentState_; }

// 0x83a95c _ZN13GLXPlayerUser17getUserReputationEv
double GLXPlayerUser::getUserReputation() const { return reputation_; }

// 0x83a964 _ZN13GLXPlayerUser25getUserBadReputationCountEv
std::int32_t GLXPlayerUser::getUserBadReputationCount() const { return badReputation_; }

// 0x83a96c _ZN13GLXPlayerUser26getUserGoodReputationCountEv
std::int32_t GLXPlayerUser::getUserGoodReputationCount() const { return goodReputation_; }

// 0x83a974 _ZN13GLXPlayerUser13getUserTrophyEv
std::int32_t GLXPlayerUser::getUserTrophy() const { return trophy_; }

// 0x83a97c _ZN13GLXPlayerUser16getUserBestScoreEv
std::int32_t GLXPlayerUser::getUserBestScore() const { return bestScore_; }

// 0x83a984 _ZN13GLXPlayerUser16getUserAvatarKeyEv
const char* GLXPlayerUser::getUserAvatarKey() const { return avatarKey_.get(); }

// 0x83a98c _ZN13GLXPlayerUser23getUserAvatarLastUpdateEv
const char* GLXPlayerUser::getUserAvatarLastUpdate() const { return avatarLastUpdate_.get(); }

// 0x83a994 _ZN13GLXPlayerUser17getUserAvatarDataEv
const void* GLXPlayerUser::getUserAvatarData() const { return avatarData_; }

// 0x83a99c _ZN13GLXPlayerUser11getUserDataEv
const char* GLXPlayerUser::getUserData() const { return userData_.get(); }

// 0x83a9a4 _ZN13GLXPlayerUser16getOtherUserNameEv
const char* GLXPlayerUser::getOtherUserName() const { return otherUserName_.get(); }

// 0x83a9ac _ZN13GLXPlayerUser12getAvatarKeyEi
// Returns the key only when the avatar list is set and the index is below the count.
const char* GLXPlayerUser::getAvatarKey(std::int32_t index) const {
    if (!InRange(index, avatarKeys_.size()) || index >= avatarCount_) {
        return nullptr;
    }
    return avatarKeys_[static_cast<std::size_t>(index)].get();
}

// 0x83a9dc _ZN13GLXPlayerUser19getAvatarLastUpdateEi
const char* GLXPlayerUser::getAvatarLastUpdate(std::int32_t index) const {
    if (!InRange(index, avatarLastUpdates_.size()) || index >= avatarCount_) {
        return nullptr;
    }
    return avatarLastUpdates_[static_cast<std::size_t>(index)].get();
}

// 0x83aa0c _ZN13GLXPlayerUser15getAvatarStatusEi
std::int32_t GLXPlayerUser::getAvatarStatus(std::int32_t index) const {
    if (!InRange(index, avatarStatus_.size()) || index >= avatarCount_) {
        return 0;
    }
    return avatarStatus_[static_cast<std::size_t>(index)];
}

// 0x83aa38 _ZN13GLXPlayerUser19processUploadAvatarEPc
void GLXPlayerUser::processUploadAvatar(char* data) { (void)data; }

// 0x83aa3c _ZN13GLXPlayerUser20clearUserInformationEv
void GLXPlayerUser::clearUserInformation() {
    string40_.reset();
    userName_.reset();
    email_.reset();
    country_.reset();
    firstName_.reset();
    lastName_.reset();
    birthday_.reset();
    otherUserName_.reset();
    avatarKey_.reset();
    avatarLastUpdate_.reset();
    string124_.reset();
    emailFlag_ = 0;
    sex_ = 0;
    onlineTime_ = 0;
    totalTrophy_ = 0;
}

// 0x83aba4 _ZN13GLXPlayerUser15clearAvatarListEv
void GLXPlayerUser::clearAvatarList() {
    avatarKeys_.clear();
    avatarLastUpdates_.clear();
    avatarStatus_.clear();
    avatarCount_ = 0;
}

// 0x83ac98 _ZN13GLXPlayerUser14clearUserStateEv
// Partial: the original also frees the unknown object at +0x88, which is not
// modelled here.
void GLXPlayerUser::clearUserState() {
    otherUserName_.reset();
    field80_ = -1;
    currentState_ = -1;
}

// 0x83ace0 _ZN13GLXPlayerUser17clearUserGameListEv
void GLXPlayerUser::clearUserGameList() {
    gamePid_.clear();
    gameName_.clear();
    gameTrophy_.clear();
    otherUserName_.reset();
    gameCount_ = 0;
}

// Test seams. These are not in the original.
void GLXPlayerUser::SetUserNameForTest(const char* name) { userName_ = OwnedNonEmpty(name); }

void GLXPlayerUser::SetGameForTest(std::int32_t index, const char* pid, const char* name,
                                   std::int32_t trophy) {
    const auto i = static_cast<std::size_t>(index);
    if (gamePid_.size() <= i) gamePid_.resize(i + 1);
    if (gameName_.size() <= i) gameName_.resize(i + 1);
    if (gameTrophy_.size() <= i) gameTrophy_.resize(i + 1, -1);
    gamePid_[i] = OwnedNonEmpty(pid);
    gameName_[i] = OwnedNonEmpty(name);
    gameTrophy_[i] = trophy;
}

void GLXPlayerUser::SetAvatarForTest(std::int32_t index, const char* key, const char* lastUpdate,
                                     std::int32_t status) {
    const auto i = static_cast<std::size_t>(index);
    if (avatarKeys_.size() <= i) avatarKeys_.resize(i + 1);
    if (avatarLastUpdates_.size() <= i) avatarLastUpdates_.resize(i + 1);
    if (avatarStatus_.size() <= i) avatarStatus_.resize(i + 1, 0);
    avatarKeys_[i] = OwnedNonEmpty(key);
    avatarLastUpdates_[i] = OwnedNonEmpty(lastUpdate);
    avatarStatus_[i] = status;
    if (avatarCount_ <= index) avatarCount_ = index + 1;
}
