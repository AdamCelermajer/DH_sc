#include "net-session-online/glx_player_message.hpp"

#include <cstdio>
#include <cstring>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    GLXPlayerMessage msg;
    CHECK(msg.getMsgListCount() == 0);
    CHECK(msg.getMsgID(0) == -1);
    CHECK(msg.getMsgTitle(0) == nullptr);
    CHECK(msg.getMsgCount(1, 0) == 0);          // cleared box
    CHECK(msg.getMsgCount(3, 0) == -1);          // unknown type
    CHECK(msg.getMsgCount(1, 3) == -1);          // box out of range
    CHECK(msg.getMySentMsgCount() == 0);

    msg.SetCountForTest(3);
    msg.SetMessageForTest(0, 100, 1, 7, false, "first");
    msg.SetMessageForTest(1, 101, 2, 8, true, "second");
    msg.SetMessageForTest(2, 102, 1, 9, false, nullptr);
    CHECK(msg.getMsgID(1) == 101);
    CHECK(msg.getMsgType(1) == 2);
    CHECK(msg.getMsgSender(2) == 9);
    CHECK(std::strcmp(msg.getMsgTitle(0), "first") == 0);
    CHECK(msg.getMsgTitle(3) == nullptr);
    CHECK(msg.getMsgReadState(1));
    CHECK(!msg.getMsgReadState(0));
    CHECK(msg.getMsgSenderNumber(0) == 0);       // not set by the seam: default
    CHECK(msg.getMsgCreatedTime(0) == 0);

    msg.SetBoxCountForTest(2, 1, 5);
    CHECK(msg.getMsgCount(2, 1) == 5);
    CHECK(msg.getMsgCount(2, 0) == 0);
    msg.SetBoxCountForTest(0x20, 2, 4);
    CHECK(msg.getMsgCount(0x20, 2) == 4);
    msg.clearMsgCount();
    CHECK(msg.getMsgCount(2, 1) == 0);
    CHECK(msg.getMsgCount(0x20, 2) == 0);

    CHECK(msg.getCurrentMsgPosition() == -1);    // no current id yet
    CHECK(msg.getCurrentMsgID() == 0);

    std::printf("%s\n", failures ? "glx_player_message: FAILED" : "glx_player_message: ok");
    return failures != 0;
}
