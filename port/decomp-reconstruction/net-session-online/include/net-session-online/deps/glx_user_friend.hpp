// Stand-in for GLXPlayerUserFriend, the friend list used by the lobby
// friend-search requests. Only the calls the lobby code makes are declared.
#pragma once

#include <cstdint>
#include <string>

class GLXPlayerUserFriend {
public:
    std::int32_t GetFriendsCount() const { return 0; }
    const char* GetName(std::int32_t) const { return nullptr; }
};
