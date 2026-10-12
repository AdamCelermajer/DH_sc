// GLXPlayerMessage: the player message box of the GLX player services (inbox and
// sent box). The web component parses the header lists into the arrays below, one
// entry per message. Only the accessors are reconstructed here. The send and
// response parsers are queued.
//
// Layout (original 32-bit offsets, kept as comments; the GLXPlayerWebComponent base
// occupies +0x00..+0x3b):
//   +0x3c message ids (int array)          +0x40 titles (char* array)
//   +0x44 sender numbers (int array)       +0x48 sender ids (int array)
//   +0x4c receiver numbers (int array)     +0x50 receiver ids (int array)
//   +0x54 created times (int array)        +0x58 read states (byte array)
//   +0x5c message types (int array)        +0x60 message count (int)
//   +0x64 current message id (int)         +0x68 body (char*)
//   +0x6c..+0xb3 six boxes of three ints (12 bytes each); see BoxIndex()
//   +0xb4 sent message count (int)
#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "net-session-online/glx_player_web_component.hpp"

class GLXPlayerMessage : public GLXPlayerWebComponent {
public:
    // 0x834d2c _ZN16GLXPlayerMessage15getMsgListCountEv
    std::int32_t getMsgListCount() const { return messageCount_; }
    // 0x834d34 _ZN16GLXPlayerMessage11getMsgCountEii
    // type 1, 2, 4, 8, 0x10, 0x20 selects a box group; box must be 0..2. Otherwise -1.
    std::int32_t getMsgCount(std::int32_t type, std::int32_t box) const;
    // 0x834dc0 _ZN16GLXPlayerMessage17getMySentMsgCountEv
    std::int32_t getMySentMsgCount() const { return mySentCount_; }
    // 0x834dc8 _ZN16GLXPlayerMessage8getMsgIDEi: default -1.
    std::int32_t getMsgID(std::int32_t index) const;
    // 0x834df8 _ZN16GLXPlayerMessage11getMsgTitleEi: default null.
    const char* getMsgTitle(std::int32_t index) const;
    // 0x834e28 _ZN16GLXPlayerMessage18getMsgSenderNumberEi: default 0.
    std::int32_t getMsgSenderNumber(std::int32_t index) const;
    // 0x834e58 _ZN16GLXPlayerMessage12getMsgSenderEi: default 0.
    std::int32_t getMsgSender(std::int32_t index) const;
    // 0x834e88 _ZN16GLXPlayerMessage23getMsgDesUserNumberListEi: default 0.
    std::int32_t getMsgDesUserNumberList(std::int32_t index) const;
    // 0x834eb8 _ZN16GLXPlayerMessage21getMsgDesUserNameListEi: default 0.
    std::int32_t getMsgDesUserNameList(std::int32_t index) const;
    // 0x834ee8 _ZN16GLXPlayerMessage17getMsgCreatedTimeEi: default 0.
    std::int32_t getMsgCreatedTime(std::int32_t index) const;
    // 0x834f18 _ZN16GLXPlayerMessage15getMsgReadStateEi: default 0.
    bool getMsgReadState(std::int32_t index) const;
    // 0x834f44 _ZN16GLXPlayerMessage10getMsgBodyEv
    const char* getMsgBody() const { return body_.c_str(); }
    // 0x834f4c _ZN16GLXPlayerMessage15getCurrentMsgIDEv
    std::int32_t getCurrentMsgID() const { return currentMsgId_; }
    // 0x834f54 _ZN16GLXPlayerMessage21getCurrentMsgPositionEv: index of the current id, or -1.
    std::int32_t getCurrentMsgPosition() const;
    // 0x834fa4 _ZN16GLXPlayerMessage10getMsgTypeEi: default -1.
    std::int32_t getMsgType(std::int32_t index) const;
    // 0x834fd4 _ZN16GLXPlayerMessage13clearMsgCountEv: zeroes the six box groups.
    void clearMsgCount();

    // Request senders: "f|kind|i|GGI|u|UID|..." requests handed to SendByGet.
    // 0x835aac _ZN16GLXPlayerMessage26SendGetMySentMessageHeaderEii (kind 0x5c, "a|" and "c|" fields)
    void SendGetMySentMessageHeader(std::int32_t first, std::int32_t count);
    // 0x835b74 _ZN16GLXPlayerMessage25SendGetMySentMessageCountEv (kind 0x5b)
    void SendGetMySentMessageCount();
    // 0x835c30 _ZN16GLXPlayerMessage20SendDelMySentMessageEi (kind 0x5e, "m|" field)
    void SendDelMySentMessage(std::int32_t messageId);
    // 0x835cf4 _ZN16GLXPlayerMessage17SendDeleteMessageEi (kind 0x4e, "m|" field)
    void SendDeleteMessage(std::int32_t messageId);
    // 0x835fcc _ZN16GLXPlayerMessage21SendReadMySentMessageEi (kind 0x5d). Stores the id at +0x64.
    void SendReadMySentMessage(std::int32_t messageId);
    // 0x836094 _ZN16GLXPlayerMessage15SendReadMessageEi (kind 0x4c). Stores the id at +0x64.
    void SendReadMessage(std::int32_t messageId);

    // Not in the original: test seams that fill the parsed lists.
    void SetMessageForTest(std::int32_t index, std::int32_t id, std::int32_t type,
                           std::int32_t sender, bool read, const char* title);
    void SetCountForTest(std::int32_t count) { messageCount_ = count; }
    void SetBoxCountForTest(std::int32_t type, std::int32_t box, std::int32_t value);

private:
    // Returns the group index for a box type (0..5), or -1 for an unknown type.
    static std::int32_t BoxIndex(std::int32_t type);

    std::vector<std::int32_t> ids_;             // +0x3c
    std::vector<std::string> titles_;           // +0x40
    std::vector<std::int32_t> senderNumbers_;   // +0x44
    std::vector<std::int32_t> senders_;         // +0x48
    std::vector<std::int32_t> desUserNumbers_;  // +0x4c
    std::vector<std::int32_t> desUserNames_;    // +0x50
    std::vector<std::int32_t> createdTimes_;    // +0x54
    std::vector<std::uint8_t> readStates_;      // +0x58
    std::vector<std::int32_t> types_;           // +0x5c
    std::int32_t messageCount_ = 0;             // +0x60
    std::int32_t currentMsgId_ = 0;             // +0x64
    std::string body_;                          // +0x68
    std::int32_t boxCounts_[6][3] = {};         // +0x6c..+0xb3
    std::int32_t mySentCount_ = 0;              // +0xb4
};
