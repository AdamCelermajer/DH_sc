// GLXPlayerMessage request senders. Each formats an "f|kind|i|GGI|u|UID|..." request into a
// 0x1000-byte buffer and hands it to SendByGet. The formats are the ones in the binary's
// .rodata, as far as the pseudocode shows them.
#include <cstdint>
#include <cstdio>

#include "net-session-online/deps/xp_api.hpp"
#include "net-session-online/glx_player_message.hpp"

namespace {
constexpr std::size_t kRequestBytes = 0x1000;

// A missing user id is written as an empty string, as in the other request builders.
const char* UidText(const char* uid) { return uid != nullptr ? uid : ""; }
}  // namespace

// 0x835aac _ZN16GLXPlayerMessage26SendGetMySentMessageHeaderEii
void GLXPlayerMessage::SendGetMySentMessageHeader(std::int32_t first, std::int32_t count) {
    char buf[kRequestBytes] = {};
    std::snprintf(buf, sizeof buf, "f|%d|i|%d|u|%s|a|%d|c|%d|", 0x5c, static_cast<int>(GetGGI()),
                  UidText(GetUID()), first, count);
    XP_DEBUG_OUT("[SendGetMessageHeader] %s\n", buf);
    SendByGet(buf);
}

// 0x835b74 _ZN16GLXPlayerMessage25SendGetMySentMessageCountEv
void GLXPlayerMessage::SendGetMySentMessageCount() {
    char buf[kRequestBytes] = {};
    std::snprintf(buf, sizeof buf, "f|%d|i|%d|u|%s|", 0x5b, static_cast<int>(GetGGI()), UidText(GetUID()));
    XP_DEBUG_OUT("[GLXPlayerMessage::SendGetMySentMessageCount] %s\n", buf);
    SendByGet(buf);
}

// 0x835c30 _ZN16GLXPlayerMessage20SendDelMySentMessageEi
void GLXPlayerMessage::SendDelMySentMessage(std::int32_t messageId) {
    char buf[kRequestBytes] = {};
    std::snprintf(buf, sizeof buf, "f|%d|i|%d|u|%s|m|%d|", 0x5e, static_cast<int>(GetGGI()), UidText(GetUID()),
                  messageId);
    XP_DEBUG_OUT("[SendDelMySentMessage] %s\n", buf);
    SendByGet(buf);
}

// 0x835cf4 _ZN16GLXPlayerMessage17SendDeleteMessageEi
void GLXPlayerMessage::SendDeleteMessage(std::int32_t messageId) {
    char buf[kRequestBytes] = {};
    std::snprintf(buf, sizeof buf, "f|%d|i|%d|u|%s|m|%d|", 0x4e, static_cast<int>(GetGGI()), UidText(GetUID()),
                  messageId);
    XP_DEBUG_OUT("[SendDeleteMessage] %s\n", buf);
    SendByGet(buf);
}

// 0x835fcc _ZN16GLXPlayerMessage21SendReadMySentMessageEi
void GLXPlayerMessage::SendReadMySentMessage(std::int32_t messageId) {
    currentMsgId_ = messageId;
    char buf[kRequestBytes] = {};
    std::snprintf(buf, sizeof buf, "f|%d|i|%d|u|%s|m|%d|", 0x5d, static_cast<int>(GetGGI()), UidText(GetUID()),
                  currentMsgId_);
    XP_DEBUG_OUT("[SendReadMySentMessage] %s\n", buf);
    SendByGet(buf);
}

// 0x836094 _ZN16GLXPlayerMessage15SendReadMessageEi
void GLXPlayerMessage::SendReadMessage(std::int32_t messageId) {
    currentMsgId_ = messageId;
    char buf[kRequestBytes] = {};
    std::snprintf(buf, sizeof buf, "f|%d|i|%d|u|%s|m|%d|", 0x4c, static_cast<int>(GetGGI()), UidText(GetUID()),
                  currentMsgId_);
    XP_DEBUG_OUT("[SendReadMessage] %s\n", buf);
    SendByGet(buf);
}
