// GLXPlayerUser response handlers (process*). Each one replaces the field it owns with
// the value in the response text. The pseudocode of most of them ends in a tail call
// that Ghidra could not follow, so the full bodies come from the ARM listing. The
// token parsers the response bodies use (split on '|', key lookup) are not
// reconstructed yet, and those bodies are partial.
#include "net-session-online/glx_player_user.hpp"

#include <cstddef>
#include <cstring>

#include "net-session-online/owned_text.hpp"

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
// The trophy-icon mode (+0xd4) picks the field: mode 0 stores the text at +0xcc and
// mode 1 at +0xd0. Mode 2 splits the text into both fields with the token parser,
// which is not reconstructed, so it is left out here.
void GLXPlayerUser::processUserTrophyIcon(char* text) {
    if (text == nullptr || std::strlen(text) == 0) {
        return;
    }
    if (trophyIconMode_ == 0) {
        iconText0_ = OwnedNonEmpty(text);
    } else if (trophyIconMode_ == 1) {
        iconText1_ = OwnedNonEmpty(text);
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
// Frees the game icon (+0xc8). The token parser that fills it and the two fields after
// it is not reconstructed.
void GLXPlayerUser::processUserGameIcon(char* text) {
    gameIcon_.reset();
    (void)text;
}

// 0x83b424 _ZN13GLXPlayerUser21processUserAvatarListEPc
// Clears the avatar list (clearAvatarList), then sizes the arrays from the number of
// '|' separators. The token parser is not reconstructed.
void GLXPlayerUser::processUserAvatarList(char* text) {
    clearAvatarList();
    (void)text;
}

// 0x83b620 _ZN13GLXPlayerUser15processUserDataEPc
// Frees the user data (+0xc0). The token parser that fills it, and the other field it
// may store at +0xc4, are not reconstructed.
void GLXPlayerUser::processUserData(char* text) {
    userData_.reset();
    (void)text;
}

// 0x83b724 _ZN13GLXPlayerUser21processDownloadAvatarEPc
// Frees the avatar data (+0xb8, opaque here). The copy of the text is not reconstructed.
void GLXPlayerUser::processDownloadAvatar(char* text) {
    avatarData_ = nullptr;
    (void)text;
}

// 0x83b82c _ZN13GLXPlayerUser17processUserAvatarEPc
// Partial: the body is not reconstructed (only the null check is kept).
void GLXPlayerUser::processUserAvatar(char* text) { (void)text; }

// 0x83bae4 _ZN13GLXPlayerUser20processUserBestScoreEPc
// Partial: the parse of the best score is not reconstructed.
void GLXPlayerUser::processUserBestScore(char* text) { (void)text; }

// 0x83bbf0 _ZN13GLXPlayerUser22processUserInformationEPc
// Clears the user information first, as the original does. The parse of the fields is
// not reconstructed.
void GLXPlayerUser::processUserInformation(char* text) {
    clearUserInformation();
    (void)text;
}

// 0x83c040 _ZN13GLXPlayerUser17processUserTrophyEPc
// Partial: the parse of the trophy count is not reconstructed.
void GLXPlayerUser::processUserTrophy(char* text) { (void)text; }

// 0x83c14c _ZN13GLXPlayerUser16processUserStateEPc
// Clears the user state first, as the original does. The parse of the state is not
// reconstructed.
void GLXPlayerUser::processUserState(char* text) {
    clearUserState();
    (void)text;
}

// 0x83c484 _ZN13GLXPlayerUser19processUserGameListEPc
// Clears the game list first, as the original does. The parse of the games is not
// reconstructed.
void GLXPlayerUser::processUserGameList(char* text) {
    clearUserGameList();
    (void)text;
}

// 0x83c6f8 _ZN13GLXPlayerUser21processUserReputationEPc
// Partial: the parse of the reputation is not reconstructed.
void GLXPlayerUser::processUserReputation(char* text) { (void)text; }
