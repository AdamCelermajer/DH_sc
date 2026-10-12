// GLXPlayerUser request builders (send*). Each one checks its arguments, formats a
// "f|kind|i|GGI|u|UID|..." request and hands it to SendByGet or SendByPost. A rejected
// argument reports (kind, -100) to the observer (slot 3, OnResponseError) and sends
// nothing. The format strings are the ones in the binary's .rodata.
#include <cstdint>
#include <cstdio>
#include <cstring>

#include "net-session-online/deps/xp_api.hpp"
#include "net-session-online/glx_player_observer.hpp"
#include "net-session-online/glx_player_user.hpp"

namespace {
constexpr std::int32_t kInvalidArgument = -100;  // 0xffffff9c in the original
constexpr std::size_t kRequestBytes = 0x1000;

// The request header reads the GGI (+0x08) and the user id (+0x0c) of the web component.
// A missing user id is written as an empty string here.
const char* UidText(const char* uid) { return uid != nullptr ? uid : ""; }
}  // namespace

// 0x83cdc8 _ZN13GLXPlayerUser16sendGetLiveFeedsEi
void GLXPlayerUser::sendGetLiveFeeds(std::int32_t kind) {
    if (kind - 1 >= 0 && kind - 1 < 0x14) {
        char buf[kRequestBytes] = {};
        std::snprintf(buf, sizeof buf, "f|%d|i|%d|u|%s|c|%d|", 0x6f, static_cast<int>(GetGGI()),
                      UidText(GetUID()), kind);
        XP_DEBUG_OUT("GLXPlayerUser::sendGetLiveFeeds -> buffer = %s\n", buf);
        SendByGet(buf);
    }
}

// 0x83cea0 _ZN13GLXPlayerUser15sendSetUserMindEPc
// Partial: the original accepts a mind text of 1 to 255 characters (strlen - 1 <= 0xfe),
// allocates a buffer and formats the request with the "mind" key (binary format string).
// Only the length check is kept here; no request is built.
void GLXPlayerUser::sendSetUserMind(char* mind) {
    if (mind == nullptr) {
        return;
    }
    const std::size_t length = std::strlen(mind);
    (void)length;  // 1..255 is accepted by the original; the request is not built here
}

// 0x83cf88 _ZN13GLXPlayerUser16sendSetUserStateEi
void GLXPlayerUser::sendSetUserState(std::int32_t state) {
    if (state == 2 || state == 3) {
        char buf[kRequestBytes] = {};
        std::snprintf(buf, sizeof buf, "f|%d|i|%d|u|%s|a|%d|", 0x61, static_cast<int>(GetGGI()),
                      UidText(GetUID()), state);
        XP_DEBUG_OUT("GLXPlayerUser::sendSetUserState -> buffer = %s\n", buf);
        SendByGet(buf);
    } else if (observer_ != nullptr) {
        observer_->OnResponseError(0x61, kInvalidArgument);
    }
}

// 0x83d080 _ZN13GLXPlayerUser18sendGetChatWarningEv
void GLXPlayerUser::sendGetChatWarning() {
    char buf[kRequestBytes] = {};
    std::snprintf(buf, sizeof buf, "f|%d|i|%d|u|%s|", 99, static_cast<int>(GetGGI()), UidText(GetUID()));
    XP_DEBUG_OUT("[GLXPlayerChat::SendGetChatWarning] -> buffer = %s\n", buf);
    SendByGet(buf);
}

// 0x83d13c _ZN13GLXPlayerUser20sendGetAdvAttachmentEi
void GLXPlayerUser::sendGetAdvAttachment(std::int32_t id) {
    char buf[kRequestBytes] = {};
    std::snprintf(buf, sizeof buf, "f|%d|i|%d|u|%s|a|%d|", 0x59, static_cast<int>(GetGGI()),
                  UidText(GetUID()), id);
    XP_DEBUG_OUT("GLXPlayerUser::sendGetAdvAttachment before String2Blob -> buffer = %s\n", buf);
    SendByGet(buf);
}

// 0x83d200 _ZN13GLXPlayerUser13sendGetAdvRSSEi
// Sent by POST. An id of -1 leaves out the "p" key.
void GLXPlayerUser::sendGetAdvRSS(std::int32_t id) {
    char buf[kRequestBytes] = {};
    if (id == -1) {
        std::snprintf(buf, sizeof buf, "f|%d|i|%d|u|%s|", 0x58, static_cast<int>(GetGGI()), UidText(GetUID()));
    } else {
        std::snprintf(buf, sizeof buf, "f|%d|i|%d|u|%s|p|%d|", 0x58, static_cast<int>(GetGGI()),
                      UidText(GetUID()), id);
    }
    XP_DEBUG_OUT("GLXPlayerUser::sendGetAdvRSS before String2Blob -> buffer = %s\n", buf);
    SendByPost(buf);
}

// 0x83d2f8 _ZN13GLXPlayerUser21sendGetGameTrophyListEi
void GLXPlayerUser::sendGetGameTrophyList(std::int32_t page) {
    char buf[kRequestBytes] = {};
    if (page == -1) {
        std::snprintf(buf, sizeof buf, "f|%d|i|%d|u|%s|", 0x56, static_cast<int>(GetGGI()), UidText(GetUID()));
    } else {
        std::snprintf(buf, sizeof buf, "f|%d|i|%d|u|%s|p|%d|", 0x56, static_cast<int>(GetGGI()),
                      UidText(GetUID()), page);
    }
    XP_DEBUG_OUT("GLXPlayerUser::sendGetGameTrophyList before String2Blob -> buffer = %s\n", buf);
    SendByGet(buf);
}

// 0x83d3f0 _ZN13GLXPlayerUser17sendGetTrophyIconEii
// A mode (rt) of 0..2 is stored at +0xd4 and sent. Other modes send nothing.
void GLXPlayerUser::sendGetTrophyIcon(std::int32_t trophy, std::int32_t mode) {
    if (static_cast<std::uint32_t>(mode) < 3u) {
        trophyIconMode_ = mode;
        char buf[kRequestBytes] = {};
        std::snprintf(buf, sizeof buf, "f|%d|i|%d|u|%s|t|%d|rt|%d|", 0x54, static_cast<int>(GetGGI()),
                      UidText(GetUID()), trophy, mode);
        XP_DEBUG_OUT("GLXPlayerUser::sendGetTrophyIcon before String2Blob -> buffer = %s\n", buf);
        SendByGet(buf);
    }
}

// 0x83d4d0 _ZN13GLXPlayerUser20sendDelUserStoreDataEPc
void GLXPlayerUser::sendDelUserStoreData(char* key) {
    if (key == nullptr) {
        if (observer_ != nullptr) observer_->OnResponseError(0x48, kInvalidArgument);
        return;
    }
    char buf[kRequestBytes] = {};
    std::snprintf(buf, sizeof buf, "f|%d|i|%d|u|%s|k|%s|", 0x48, static_cast<int>(GetGGI()),
                  UidText(GetUID()), key);
    XP_DEBUG_OUT("GLXPlayerUser::sendDelUserStoreData before String2Blob -> buffer = %s\n", buf);
    SendByGet(buf);
}

// 0x83d5c0 _ZN13GLXPlayerUser21sendGetUserAvatarListEv
void GLXPlayerUser::sendGetUserAvatarList() {
    char buf[kRequestBytes] = {};
    std::snprintf(buf, sizeof buf, "f|%d|i|%d|u|%s|", 0x67, static_cast<int>(GetGGI()), UidText(GetUID()));
    XP_DEBUG_OUT("GLXPlayerUser::sendGetUserAvatarList-> buffer = %s\n", buf);
    SendByGet(buf);
}

// 0x83d67c _ZN13GLXPlayerUser17sendDelUserAvatarEPc
void GLXPlayerUser::sendDelUserAvatar(char* avatarKey) {
    if (avatarKey == nullptr) {
        if (observer_ != nullptr) observer_->OnResponseError(0x66, kInvalidArgument);
        return;
    }
    char buf[kRequestBytes] = {};
    std::snprintf(buf, sizeof buf, "f|%d|i|%d|u|%s|d|%s|", 0x66, static_cast<int>(GetGGI()),
                  UidText(GetUID()), avatarKey);
    XP_DEBUG_OUT("GLXPlayerUser::sendDelUserAvatar-> buffer = %s\n", buf);
    SendByGet(buf);
}

// 0x83d77c _ZN13GLXPlayerUser20sendUploadUserAvatarEPc
// Partial: the original formats the request into a heap buffer of strlen + 0x80 bytes
// and sends it by POST. The format string is not identified, so only the null case
// (observer slot 3 with 0x47) is reproduced.
void GLXPlayerUser::sendUploadUserAvatar(char* path) {
    if (path == nullptr && observer_ != nullptr) {
        observer_->OnResponseError(0x47, kInvalidArgument);
    }
}

// 0x83d84c _ZN13GLXPlayerUser22sendDownloadUserAvatarEPc
void GLXPlayerUser::sendDownloadUserAvatar(char* avatarKey) {
    char buf[kRequestBytes] = {};
    std::snprintf(buf, sizeof buf, "f|%d|i|%d|u|%s|d|%s|", 0x65, static_cast<int>(GetGGI()),
                  UidText(GetUID()), avatarKey);
    XP_DEBUG_OUT("GLXPlayerUser::sendGetUserAvatar before String2Blob -> buffer = %s\n", buf);
    SendByGet(buf);
}

// 0x83d910 _ZN13GLXPlayerUser14sendGetUserUIDEPcb
// The bool selects the "un" key (true) or the "n" key (false).
void GLXPlayerUser::sendGetUserUID(char* name, bool byUserName) {
    if (name == nullptr) {
        if (observer_ != nullptr) observer_->OnResponseError(0x3a, kInvalidArgument);
        return;
    }
    char buf[kRequestBytes] = {};
    std::snprintf(buf, sizeof buf, byUserName ? "f|%d|i|%d|u|%s|un|%s|" : "f|%d|i|%d|u|%s|n|%s|", 0x3a,
                  static_cast<int>(GetGGI()), UidText(GetUID()), name);
    XP_DEBUG_OUT("GLXPlayerUser::sendGetUserUID before String2Blob -> buffer = %s\n", buf);
    SendByGet(buf);
}

// 0x83da24 _ZN13GLXPlayerUser23sendPointUserReputationEPcib
// The points must be 1 or 5. The bool selects the "un" key (true) or "n" (false).
void GLXPlayerUser::sendPointUserReputation(char* name, std::int32_t points, bool byUserName) {
    if (name == nullptr || (points != 5 && points != 1)) {
        if (observer_ != nullptr) observer_->OnResponseError(0x44, kInvalidArgument);
        return;
    }
    char buf[kRequestBytes] = {};
    std::snprintf(buf, sizeof buf, byUserName ? "f|%d|i|%d|u|%s|un|%s|r|%d|" : "f|%d|i|%d|u|%s|n|%s|r|%d|",
                  0x44, static_cast<int>(GetGGI()), UidText(GetUID()), name, points);
    XP_DEBUG_OUT("GLXPlayerUser::sendUpdateReputation before String2Blob -> buffer = %s\n", buf);
    SendByGet(buf);
}

// 0x83db50 _ZN13GLXPlayerUser20sendSetUserStoreDataEPcS0_
// Sent by POST. Without a key the observer gets (0x42, -100) and nothing is sent.
void GLXPlayerUser::sendSetUserStoreData(char* key, char* value) {
    if (key == nullptr) {
        if (observer_ != nullptr) observer_->OnResponseError(0x42, kInvalidArgument);
        return;
    }
    char buf[kRequestBytes] = {};
    std::snprintf(buf, sizeof buf, "f|%d|i|%d|u|%s|k|%s|d|%s|", 0x42, static_cast<int>(GetGGI()),
                  UidText(GetUID()), key, value);
    XP_DEBUG_OUT("GLXPlayerUser::sendSetUserStoreData -> buffer length = %d\n",
                 static_cast<int>(std::strlen(buf)));
    SendByPost(buf);
}

// 0x83dc4c _ZN13GLXPlayerUser21sendRetrievalPasswordEPc
void GLXPlayerUser::sendRetrievalPassword(char* name) {
    if (name == nullptr) {
        if (observer_ != nullptr) observer_->OnResponseError(0x34, kInvalidArgument);
        return;
    }
    char buf[kRequestBytes] = {};
    std::snprintf(buf, sizeof buf, "f|%d|i|%d|u|%s|", 0x34, static_cast<int>(GetGGI()), name);
    XP_DEBUG_OUT("GLXPlayerUser::sendRetrievalPassword before String2Blob -> buffer = %s\n", buf);
    SendByGet(buf);
}

// 0x83dd34 _ZN13GLXPlayerUser15sendChangeEmailEPcS0_S0_
void GLXPlayerUser::sendChangeEmail(char* name, char* password, char* newEmail) {
    if (name == nullptr || password == nullptr || newEmail == nullptr) {
        if (observer_ != nullptr) observer_->OnResponseError(0x33, kInvalidArgument);
        return;
    }
    char buf[kRequestBytes] = {};
    std::snprintf(buf, sizeof buf, "f|%d|i|%d|u|%s|p|%s|ne|%s|", 0x33, static_cast<int>(GetGGI()), name,
                  password, newEmail);
    XP_DEBUG_OUT("GLXPlayerUser::sendChangeEmail before String2Blob -> buffer = %s\n", buf);
    SendByGet(buf);
}

// 0x83de3c _ZN13GLXPlayerUser18sendChangePasswordEPcS0_S0_S0_
// The new password must match its repeat (strcmp of the two).
void GLXPlayerUser::sendChangePassword(char* name, char* password, char* newPassword, char* repeat) {
    if (name == nullptr || password == nullptr || newPassword == nullptr || repeat == nullptr ||
        std::strcmp(newPassword, repeat) != 0) {
        if (observer_ != nullptr) observer_->OnResponseError(0x32, kInvalidArgument);
        return;
    }
    char buf[kRequestBytes] = {};
    std::snprintf(buf, sizeof buf, "f|%d|i|%d|u|%s|p|%s|np|%s|vp|%s|", 0x32, static_cast<int>(GetGGI()),
                  name, password, newPassword, repeat);
    XP_DEBUG_OUT("GLXPlayerUser::sendChangePassword before String2Blob -> buffer = %s\n", buf);
    SendByGet(buf);
}

// 0x83df58 _ZN13GLXPlayerUser18sendChangeUserNameEPcS0_S0_b
void GLXPlayerUser::sendChangeUserName(char* name, char* password, char* newName, bool keepOther) {
    if (name == nullptr || password == nullptr || newName == nullptr) {
        if (observer_ != nullptr) observer_->OnResponseError(0x0e, kInvalidArgument);
        return;
    }
    char buf[kRequestBytes] = {};
    std::snprintf(buf, sizeof buf, "f|%d|i|%d|u|%s|p|%s|nu|%s|o|%d|", 0x0e, static_cast<int>(GetGGI()), name,
                  password, newName, keepOther ? 1 : 0);
    XP_DEBUG_OUT("GLXPlayerUser::sendChangeUserName before String2Blob -> buffer = %s\n", buf);
    SendByGet(buf);
}

// 0x83e068 _ZN13GLXPlayerUser14sendGetUsedAppEPc
// Without a name the request uses the user id.
void GLXPlayerUser::sendGetUsedApp(char* name) {
    char buf[kRequestBytes] = {};
    std::snprintf(buf, sizeof buf, "f|%d|i|%d|u|%s|", 0x39, static_cast<int>(GetGGI()),
                  name == nullptr ? UidText(GetUID()) : name);
    XP_DEBUG_OUT("GLXPlayerUser::sendGetUsedApp before String2Blob -> buffer = %s\n", buf);
    SendByGet(buf);
}
