// GLXPlayerUser response handlers (process*). Each one replaces the field it owns with
// the value in the response text. The response is split on '|' into fields; a record
// inside a field is split on '^' (games, avatars) or ',' (reputation). The original reads
// each field with getValue into a fixed buffer. Here each field is copied into a string
// of its exact length, so no buffer can overflow.
//
// A body that starts with "n" carries the other user's name in field 1, which moves every
// later field by two. Those bodies are handled with the offset `base` below.
#include "net-session-online/glx_player_user.hpp"

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <string>
#include <vector>

#include "net-session-online/owned_text.hpp"
#include "net-session-online/xp_api.hpp"

namespace {
constexpr int kBar = '|';    // field separator (delim 124 in the original)
constexpr int kCaret = '^';  // record separator inside a game or avatar field (94)
constexpr int kComma = ',';  // value separator inside the reputation field (44)

// Field index of text (split by delim), copied with getValue. A missing field is "".
std::string Field(const std::string& text, std::int32_t index, int delim = kBar) {
    std::vector<char> src(text.begin(), text.end());
    src.push_back('\0');
    std::vector<char> out(text.size() + 1, '\0');
    getValue(src.data(), out.data(), index, delim);
    return std::string(out.data());
}

std::int32_t IntField(const std::string& text, std::int32_t index, int delim = kBar) {
    return std::atoi(Field(text, index, delim).c_str());
}

// Owned copy of a string, the original's new[] + copy pair.
std::unique_ptr<char[]> OwnedCopy(const std::string& text) {
    auto copy = std::make_unique<char[]>(text.size() + 1);
    std::memcpy(copy.get(), text.c_str(), text.size() + 1);
    return copy;
}

std::unique_ptr<char[]> OwnedField(const std::string& text, std::int32_t index, int delim = kBar) {
    return OwnedCopy(Field(text, index, delim));
}

// Number of '|' separators in text.
std::int32_t CountBars(const std::string& text) {
    std::int32_t count = 0;
    for (const char c : text) {
        if (c == '|') {
            ++count;
        }
    }
    return count;
}
}  // namespace

// 0x83b1ac _ZN13GLXPlayerUser11setPasswordEPc
// Replaces the password with a copy of the text (null stays null, "" stays "").
void GLXPlayerUser::setPassword(char* password) { password_ = OwnedDup(password); }

// 0x83adec _ZN13GLXPlayerUser15processLiveFeedsEPc
// Partial: the live feed list (452 bytes) is parsed into the _LiveFeed array at +0x12c,
// which is not modelled. Nothing is stored here.
void GLXPlayerUser::processLiveFeeds(char* response) { (void)response; }

// 0x83afb8 _ZN13GLXPlayerUser22processUserChatWarningEPc
void GLXPlayerUser::processUserChatWarning(char* text) { chatWarning_ = OwnedNonEmpty(text); }

// 0x83b028 _ZN13GLXPlayerUser22processPromoAttachmentEPc
void GLXPlayerUser::processPromoAttachment(char* text) { promoAttachment_ = OwnedNonEmpty(text); }

// 0x83b074 _ZN13GLXPlayerUser20processRssAttachmentEPc
void GLXPlayerUser::processRssAttachment(char* text) { rssAttachment_ = OwnedNonEmpty(text); }

// 0x83b0c0 _ZN13GLXPlayerUser21processUserTrophyIconEPc
// The trophy-icon mode (+0xd4) picks the field: mode 0 stores the text at +0xcc, mode 1 at
// +0xd0, and mode 2 splits the text into both (field 1 to +0xd0, field 3 to +0xcc).
void GLXPlayerUser::processUserTrophyIcon(char* text) {
    if (text == nullptr || std::strlen(text) == 0) {
        return;
    }
    const std::string body(text);
    switch (trophyIconMode_) {
        case 0:
            iconText0_ = OwnedCopy(body);
            break;
        case 1:
            iconText1_ = OwnedCopy(body);
            break;
        case 2:
            iconText1_ = OwnedField(body, 1);
            iconText0_ = OwnedField(body, 3);
            break;
        default:
            break;
    }
}

// 0x83b1e0 _ZN13GLXPlayerUser19processGetPromo_RSSEPc
// Partial: the response is read as an XML document (slim::XmlDocument), which is not
// reconstructed.
void GLXPlayerUser::processGetPromo_RSS(char* response) { (void)response; }

// 0x83b280 _ZN13GLXPlayerUser17processGetADV_RSSEPc
// Partial: same XML document reader as processGetPromo_RSS.
void GLXPlayerUser::processGetADV_RSS(char* response) { (void)response; }

// 0x83b320 _ZN13GLXPlayerUser19processUserGameIconEPc
// Frees the game icon (+0xc8) and stores field 1. Partial: field 3 (+0xdc) and field 5
// (+0xe4, as an int) are not modelled.
void GLXPlayerUser::processUserGameIcon(char* text) {
    gameIcon_.reset();
    if (text == nullptr || std::strlen(text) == 0) {
        return;
    }
    gameIcon_ = OwnedField(std::string(text), 1);
}

// 0x83b424 _ZN13GLXPlayerUser21processUserAvatarListEPc
// Clears the list, then reads one record per field. A record is key^last-update^status.
// The count is 1 plus the number of '|' separators.
void GLXPlayerUser::processUserAvatarList(char* text) {
    clearAvatarList();
    if (text == nullptr || std::strlen(text) == 0) {
        return;
    }
    const std::string body(text);
    const std::int32_t count = 1 + CountBars(body);
    avatarCount_ = count;
    for (std::int32_t i = 0; i < count; ++i) {
        const std::string record = Field(body, i);
        avatarKeys_.push_back(OwnedField(record, 0, kCaret));
        avatarLastUpdates_.push_back(OwnedField(record, 1, kCaret));
        avatarStatus_.push_back(IntField(record, 2, kCaret));
    }
}

// 0x83b620 _ZN13GLXPlayerUser15processUserDataEPc
// Frees the user data (+0xc0). A body that starts with "n" also stores field 1 as the other
// user's name (+0xc4); the data is then field 2.
void GLXPlayerUser::processUserData(char* text) {
    userData_.reset();
    if (text == nullptr || std::strlen(text) == 0) {
        return;
    }
    const std::string body(text);
    std::int32_t index = 0;
    if (Field(body, 0) == "n") {
        otherUserName_ = OwnedField(body, 1);
        index = 2;
    }
    userData_ = OwnedField(body, index);
}

// 0x83b724 _ZN13GLXPlayerUser21processDownloadAvatarEPc
// Frees the avatar data (+0xb8, opaque here). The copy of the text is not reconstructed.
void GLXPlayerUser::processDownloadAvatar(char* text) {
    avatarData_ = nullptr;
    (void)text;
}

// 0x83b82c _ZN13GLXPlayerUser17processUserAvatarEPc
// Partial: the key (+0xa8) and last-update (+0xb0) fields are stored for the non-"n" form,
// and +0xc4 for the "n" form. The tokens at +0xac and +0xb4 are not modelled, so they are
// not stored.
void GLXPlayerUser::processUserAvatar(char* text) {
    if (text == nullptr || std::strlen(text) == 0) {
        return;
    }
    const std::string body(text);
    if (Field(body, 0) != "n") {
        std::int32_t flagIndex = 1;
        std::int32_t valueIndex = 2;
        if (Field(body, 0) == "d") {
            avatarKey_ = OwnedField(body, 1);
            flagIndex = 2;
            valueIndex = 3;
        }
        if (Field(body, flagIndex) == "t") {
            avatarLastUpdate_ = OwnedField(body, valueIndex);
        }
    } else {
        otherUserName_ = OwnedField(body, 1);
    }
}

// 0x83bae4 _ZN13GLXPlayerUser20processUserBestScoreEPc
// A body that starts with "n" stores field 1 as the other user's name and reads the score
// from field 2. Otherwise the score is field 0.
void GLXPlayerUser::processUserBestScore(char* text) {
    if (text == nullptr || std::strlen(text) == 0) {
        return;
    }
    const std::string body(text);
    if (Field(body, 0) == "n") {
        otherUserName_ = OwnedField(body, 1);
        bestScore_ = IntField(body, 2);
    } else {
        bestScore_ = IntField(body, 0);
    }
}

// 0x83bbf0 _ZN13GLXPlayerUser22processUserInformationEPc
// Clears the user information first. The fields are read from the offset `base`, which is
// 2 for the "n" form (the other user's name is field 1) and 0 otherwise.
void GLXPlayerUser::processUserInformation(char* text) {
    clearUserInformation();
    if (text == nullptr || std::strlen(text) == 0) {
        return;
    }
    const std::string body(text);
    const bool otherForm = Field(body, 0) == "n";
    if (otherForm) {
        otherUserName_ = OwnedField(body, 1);
    }
    const std::int32_t base = otherForm ? 2 : 0;
    string40_ = OwnedField(body, base + 1);
    userName_ = OwnedField(body, base + 3);
    email_ = OwnedField(body, base + 5);
    emailFlag_ = IntField(body, base + 7) != 0 ? 1 : 0;
    country_ = OwnedField(body, base + 9);
    avatarKey_ = OwnedField(body, base + 11);
    avatarLastUpdate_ = OwnedField(body, base + 13);
    reputation_ = static_cast<double>(IntField(body, base + 15));
    firstName_ = OwnedField(body, base + 17);
    lastName_ = OwnedField(body, base + 19);
    sex_ = IntField(body, base + 21) != 0 ? 1 : 0;
    birthday_ = OwnedField(body, base + 23);
    onlineTime_ = IntField(body, base + 25);
    totalTrophy_ = IntField(body, base + 27);
    string124_ = OwnedField(body, base + 29);
}

// 0x83c040 _ZN13GLXPlayerUser17processUserTrophyEPc
// Same shape as processUserBestScore, for the trophy count.
void GLXPlayerUser::processUserTrophy(char* text) {
    if (text == nullptr || std::strlen(text) == 0) {
        return;
    }
    const std::string body(text);
    if (Field(body, 0) == "n") {
        otherUserName_ = OwnedField(body, 1);
        trophy_ = IntField(body, 2);
    } else {
        trophy_ = IntField(body, 0);
    }
}

// 0x83c14c _ZN13GLXPlayerUser16processUserStateEPc
// Clears the state first. Partial: the token at +0x88 (a 16-byte object, not modelled) is
// read past but not stored. The state value and the "a" flag value are kept.
void GLXPlayerUser::processUserState(char* text) {
    clearUserState();
    if (text == nullptr || std::strlen(text) == 0) {
        return;
    }
    const std::string body(text);
    const std::string kind = Field(body, 0);
    if (kind != "n" && kind != "un") {
        if (kind == "t") {
            currentState_ = IntField(body, 1);
            if (Field(body, 4) == "a") {
                field80_ = IntField(body, 5);
            }
        }
        return;
    }
    otherUserName_ = OwnedField(body, 1);
    if (Field(body, 2) != "t") {
        if (Field(body, 5) == "a") {
            field80_ = IntField(body, 6);
        }
    } else {
        currentState_ = IntField(body, 3);
        if (Field(body, 6) == "a") {
            field80_ = IntField(body, 7);
        }
    }
}

// 0x83c484 _ZN13GLXPlayerUser19processUserGameListEPc
// Clears the game list first. Each game is a record pid^name^trophy in fields 0 onward, or
// from field 2 in the "n" form. The count is the number of '|' separators, plus 2 minus the
// number of leading fields (1 or 2), as in the original.
void GLXPlayerUser::processUserGameList(char* text) {
    clearUserGameList();
    if (text == nullptr || std::strlen(text) == 0) {
        return;
    }
    const std::string body(text);
    std::int32_t leading = 1;
    std::int32_t offset = 0;
    if (Field(body, 0) == "n") {
        otherUserName_ = OwnedField(body, 1);
        leading = 2;
        offset = 2;
    }
    const std::int32_t count = CountBars(body) + 2 - leading;
    gameCount_ = count;
    for (std::int32_t i = 0; i < count; ++i) {
        const std::string record = Field(body, i + offset);
        gamePid_.push_back(OwnedField(record, 0, kCaret));
        gameName_.push_back(OwnedField(record, 1, kCaret));
        gameTrophy_.push_back(IntField(record, 2, kCaret));
    }
}

// 0x83c6f8 _ZN13GLXPlayerUser21processUserReputationEPc
// A body that starts with "n" stores field 1 as the other user's name, and the reputation
// is then field 2. The reputation is "value,bad,good": a double, then two ints.
void GLXPlayerUser::processUserReputation(char* text) {
    if (text == nullptr || std::strlen(text) == 0) {
        return;
    }
    const std::string body(text);
    std::int32_t index = 0;
    if (Field(body, 0) == "n") {
        otherUserName_ = OwnedField(body, 1);
        index = 2;
    }
    const std::string reputation = Field(body, index);
    reputation_ = std::atof(Field(reputation, 0, kComma).c_str());
    badReputation_ = IntField(reputation, 1, kComma);
    goodReputation_ = IntField(reputation, 2, kComma);
}

// 0x83c894 _ZN13GLXPlayerUser15OnUpdateSuccessEi
// Routes the response by code to the handler for it, then calls the base, which reports the
// response text to the observer. Partial: code 58 writes +0x3c, which is not modelled, and
// code 98 reads two tokens into +0x11c and +0x120, which are not modelled. Both read or
// skip their fields without storing them.
void GLXPlayerUser::OnUpdateSuccess(std::int32_t code) {
    const std::string body = cursor_ != nullptr ? std::string(cursor_) : std::string();
    std::vector<char> buffer(body.begin(), body.end());
    buffer.push_back('\0');
    char* text = buffer.data();

    switch (code) {
        case 53:
            country_ = OwnedField(body, 1);
            break;
        case 58:
            break;
        case 63:
            otherUserName_.reset();
            processUserGameList(text);
            break;
        case 64:
            otherUserName_.reset();
            processUserState(text);
            break;
        case 65:
            otherUserName_.reset();
            processUserData(text);
            break;
        case 67:
            otherUserName_.reset();
            processUserReputation(text);
            break;
        case 69:
            otherUserName_.reset();
            processUserBestScore(text);
            break;
        case 70:
            otherUserName_.reset();
            processUserAvatar(text);
            break;
        case 71:
            processUploadAvatar(text);
            break;
        case 81:
            otherUserName_.reset();
            processUserInformation(text);
            break;
        case 84:
            processUserTrophyIcon(text);
            break;
        case 88:
            processGetADV_RSS(text);
            break;
        case 89:
            processRssAttachment(text);
            break;
        case 90:
            processUserGameIcon(text);
            break;
        case 98: {
            // The two tokens are read to advance the cursor; their values are not stored.
            std::vector<char> token(body.size() + 256, '\0');
            if (IsNextResponseStringToken("d")) {
                GetNextResponseToken(token.data());
            }
            if (IsNextResponseStringToken("t")) {
                GetNextResponseToken(token.data());
            }
            break;
        }
        case 99:
            processUserChatWarning(text);
            break;
        case 101:
            processDownloadAvatar(text);
            break;
        case 103:
            processUserAvatarList(text);
            break;
        case 111:
            processLiveFeeds(text);
            break;
        case 204:
            processGetPromo_RSS(text);
            break;
        case 205:
            processPromoAttachment(text);
            break;
        default:
            break;
    }
    GLXPlayerWebComponent::OnUpdateSuccess(code);
}
