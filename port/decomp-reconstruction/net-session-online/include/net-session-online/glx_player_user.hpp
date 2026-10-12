// GLXPlayerUser: the user record of the GLX player services. It extends the web
// component: the request helpers (send*) build "f|id|i|GGI|u|UID|..." requests and
// the process* methods store the parsed responses in the fields below.
//
// Layout (original 32-bit offsets, kept as comments). The GLXPlayerWebComponent base
// occupies +0x00..+0x3b. Owned strings are unique_ptr<char[]> and arrays of owned
// strings are vectors, instead of raw heap arrays. Member roles that are not
// identified are marked "unknown".
//   +0x40 owned string (unknown)            +0x44 user name (owned)
//   +0x48 password (owned)                  +0x4c email (owned)
//   +0x50 email flag (byte)                 +0x54 country (owned)
//   +0x58 first name (owned)                +0x5c last name (owned)
//   +0x60 sex (byte)                        +0x64 birthday (owned)
//   +0x68 online time                       +0x6c total trophy
//   +0x70 game PID strings (owned, array)   +0x74 game name strings (owned, array)
//   +0x78 game trophy values (array)        +0x7c number of games
//   +0x80 unknown (-1 by clearUserState)    +0x84 current user state (-1 when idle)
//   +0x88 unknown object (freed by clearUserState; not modelled)
//   +0x90 reputation (double)               +0x98 bad reputation count
//   +0x9c good reputation count             +0xa0 trophy
//   +0xa4 best score                        +0xa8 avatar key (owned)
//   +0xb0 avatar last update (owned)        +0xb8 avatar data (unknown object)
//   +0xc0 user data (owned)                 +0xc4 other user name (owned)
//   +0xc8 game icon (owned)                 +0xcc, +0xd0 unknown owned strings
//   +0xd4 trophy-icon mode (-1 by the ctor, 0..2 by processUserTrophyIcon)
//   +0xf4 RSS attachment (owned)            +0x104 promo attachment (owned)
//   +0x108 avatar keys (owned, array)       +0x10c avatar last updates (owned, array)
//   +0x110 avatar status (array)            +0x114 avatar count
//   +0x118 chat warning (owned)             +0x124 unknown owned string
// Not modelled: vector<channel> at +0xe8 and +0xf8, the _LiveFeed array at +0x12c,
// the request client object at +0x20 (see the ctor note), and fields at +0x3c..+0x3f,
// +0x47, +0x49, +0xdc, +0xe4 and +0x128 (their roles are not identified).
#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "net-session-online/glx_player_web_component.hpp"

class GLXPlayerUser : public GLXPlayerWebComponent {
public:
    GLXPlayerUser();                                                 // 0x83f8a4 C2, 0x83f754 C1
    ~GLXPlayerUser() override;                                       // 0x83fb24 D1, 0x83fda0 D2
    GLXPlayerUser(const GLXPlayerUser&) = delete;
    GLXPlayerUser& operator=(const GLXPlayerUser&) = delete;

    // User fields. Strings return nullptr when not set. Index getters return the
    // default when the index is out of range.
    const char* getUserName() const;                                 // 0x83a880
    const char* getPassword() const;                                 // 0x83a888
    const char* getEmail() const;                                    // 0x83a890
    std::uint8_t getEmailFlag() const;                               // 0x83a898
    const char* getCountry() const;                                  // 0x83a8a0
    const char* getFirstName() const;                                 // 0x83a8a8
    const char* getLastName() const;                                 // 0x83a8b0
    std::uint8_t getSex() const;                                     // 0x83a8b8
    const char* getBirthday() const;                                 // 0x83a8c0
    std::int32_t getOnlineTime() const;                              // 0x83a8c8
    std::int32_t getTotalTrophy() const;                             // 0x83a8d0
    std::int32_t getNumberUserGameList() const;                      // 0x83a8d8
    const char* getUserGamePID(std::int32_t index) const;            // 0x83a8e0
    const char* getUserGameName(std::int32_t index) const;           // 0x83a908
    std::int32_t getUserGameTrophy(std::int32_t index) const;        // 0x83a930, default -1
    std::int32_t getCurrentUserState() const;                        // 0x83a954
    double getUserReputation() const;                                // 0x83a95c
    std::int32_t getUserBadReputationCount() const;                  // 0x83a964
    std::int32_t getUserGoodReputationCount() const;                 // 0x83a96c
    std::int32_t getUserTrophy() const;                              // 0x83a974
    std::int32_t getUserBestScore() const;                           // 0x83a97c
    const char* getUserAvatarKey() const;                            // 0x83a984
    const char* getUserAvatarLastUpdate() const;                     // 0x83a98c
    const void* getUserAvatarData() const;                           // 0x83a994
    const char* getUserData() const;                                 // 0x83a99c
    const char* getOtherUserName() const;                            // 0x83a9a4
    const char* getAvatarKey(std::int32_t index) const;              // 0x83a9ac
    const char* getAvatarLastUpdate(std::int32_t index) const;       // 0x83a9dc
    std::int32_t getAvatarStatus(std::int32_t index) const;          // 0x83aa0c

    // 0x83aa38 _ZN13GLXPlayerUser19processUploadAvatarEPc: empty in the original.
    void processUploadAvatar(char* data);

    // Request builders (src/glx_player_user_requests.cpp). Each one sends a request
    // through SendByGet or SendByPost, or reports a rejected argument to the observer.
    void sendGetLiveFeeds(std::int32_t kind);                        // 0x83cdc8
    void sendSetUserMind(char* mind);                                // 0x83cea0 (partial)
    void sendSetUserState(std::int32_t state);                       // 0x83cf88
    void sendGetChatWarning();                                       // 0x83d080
    void sendGetAdvAttachment(std::int32_t id);                      // 0x83d13c
    void sendGetAdvRSS(std::int32_t id);                             // 0x83d200 (POST)
    void sendGetGameTrophyList(std::int32_t page);                   // 0x83d2f8
    void sendGetTrophyIcon(std::int32_t trophy, std::int32_t mode);  // 0x83d3f0
    void sendDelUserStoreData(char* key);                            // 0x83d4d0
    void sendGetUserAvatarList();                                    // 0x83d5c0
    void sendDelUserAvatar(char* avatarKey);                         // 0x83d67c
    void sendUploadUserAvatar(char* path);                           // 0x83d77c (partial)
    void sendDownloadUserAvatar(char* avatarKey);                    // 0x83d84c
    void sendGetUserUID(char* name, bool byUserName);                // 0x83d910
    void sendPointUserReputation(char* name, std::int32_t points, bool byUserName);  // 0x83da24
    void sendSetUserStoreData(char* key, char* value);               // 0x83db50 (POST)
    void sendRetrievalPassword(char* name);                          // 0x83dc4c
    void sendChangeEmail(char* name, char* password, char* newEmail);  // 0x83dd34
    void sendChangePassword(char* name, char* password, char* newPassword, char* repeat);  // 0x83de3c
    void sendChangeUserName(char* name, char* password, char* newName, bool keepOther);    // 0x83df58
    void sendGetUsedApp(char* name);                                 // 0x83e068

    // Clears (free the owned fields and reset the flags they feed).
    void clearUserInformation();                                     // 0x83aa3c
    void clearAvatarList();                                          // 0x83aba4
    void clearUserState();                                           // 0x83ac98
    void clearUserGameList();                                        // 0x83ace0

    // Not in the original: test seams that fill the lists the getters read.
    void SetUserNameForTest(const char* name);
    void SetGameForTest(std::int32_t index, const char* pid, const char* name, std::int32_t trophy);
    void SetAvatarForTest(std::int32_t index, const char* key, const char* lastUpdate, std::int32_t status);
    void SetGameCountForTest(std::int32_t count) { gameCount_ = count; }

private:
    std::unique_ptr<char[]> string40_;                               // +0x40
    std::unique_ptr<char[]> userName_;                               // +0x44
    std::unique_ptr<char[]> password_;                               // +0x48
    std::unique_ptr<char[]> email_;                                  // +0x4c
    std::uint8_t emailFlag_ = 0;                                     // +0x50
    std::unique_ptr<char[]> country_;                                // +0x54
    std::unique_ptr<char[]> firstName_;                              // +0x58
    std::unique_ptr<char[]> lastName_;                               // +0x5c
    std::uint8_t sex_ = 0;                                           // +0x60
    std::unique_ptr<char[]> birthday_;                               // +0x64
    std::int32_t onlineTime_ = 0;                                    // +0x68
    std::int32_t totalTrophy_ = 0;                                   // +0x6c
    std::vector<std::unique_ptr<char[]>> gamePid_;                   // +0x70
    std::vector<std::unique_ptr<char[]>> gameName_;                  // +0x74
    std::vector<std::int32_t> gameTrophy_;                           // +0x78
    std::int32_t gameCount_ = 0;                                     // +0x7c
    std::int32_t field80_ = -1;                                      // +0x80
    std::int32_t currentState_ = -1;                                 // +0x84
    double reputation_ = 0.0;                                        // +0x90
    std::int32_t badReputation_ = 0;                                 // +0x98
    std::int32_t goodReputation_ = 0;                                // +0x9c
    std::int32_t trophy_ = 0;                                        // +0xa0
    std::int32_t bestScore_ = -1;                                    // +0xa4
    std::unique_ptr<char[]> avatarKey_;                              // +0xa8
    std::unique_ptr<char[]> avatarLastUpdate_;                       // +0xb0
    void* avatarData_ = nullptr;                                     // +0xb8 (not owned by this port)
    std::unique_ptr<char[]> userData_;                               // +0xc0
    std::unique_ptr<char[]> otherUserName_;                          // +0xc4
    std::unique_ptr<char[]> gameIcon_;                               // +0xc8
    std::int32_t trophyIconMode_ = -1;                               // +0xd4
    std::unique_ptr<char[]> rssAttachment_;                          // +0xf4
    std::unique_ptr<char[]> promoAttachment_;                        // +0x104
    std::vector<std::unique_ptr<char[]>> avatarKeys_;                // +0x108
    std::vector<std::unique_ptr<char[]>> avatarLastUpdates_;         // +0x10c
    std::vector<std::int32_t> avatarStatus_;                         // +0x110
    std::int32_t avatarCount_ = 0;                                   // +0x114
    std::unique_ptr<char[]> chatWarning_;                            // +0x118
    std::unique_ptr<char[]> string124_;                              // +0x124
};
