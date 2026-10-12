#include "net-session-online/glx_player_http.hpp"
#include "net-session-online/glx_player_socket.hpp"

#include <cstdio>
#include <cstring>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    GLXPlayerSocket sock;
    CHECK(sock.GetAcceptIP() == nullptr);
    CHECK(sock.GetAcceptPort() == 0);
    sock.SetAcceptPort(9001);
    CHECK(sock.GetAcceptPort() == 9001);
    sock.SetSocketType(3);
    CHECK(sock.GetSocketType() == 3);
    sock.SetSocket(42);
    sock.SetAcceptIP("10.0.0.5");
    CHECK(std::strcmp(sock.GetAcceptIP(), "10.0.0.5") == 0);
    sock.SetAcceptIP("10.0.0.6");              // replaces the earlier copy
    CHECK(std::strcmp(sock.GetAcceptIP(), "10.0.0.6") == 0);
    sock.SetAcceptIP(nullptr);
    CHECK(sock.GetAcceptIP()[0] == '\0');
    CHECK(!sock.IsErrorOccurred());
    sock.SetStateForTest(7);
    CHECK(sock.IsErrorOccurred());
    sock.SetSendPercentageForTest(50);
    CHECK(sock.GetSendPercentage() == 50);
    sock.SetSendPercentageForTest(101);
    CHECK(sock.GetSendPercentage() == 0);

    GLXPlayerHttp http;
    CHECK(!http.IsSendByPost());
    CHECK(http.GetServerAddress() == nullptr);
    CHECK(http.GetSendPercentage() == 0);      // no socket set
    http.SetRequestForTest("GET /x?a=1");
    CHECK(std::strcmp(http.GetRequestData(), "GET /x?a=1") == 0);
    CHECK(http.GetRequestLength() == 10);
    http.SetSocketForTest(&sock);
    CHECK(http.GetSendPercentage() == 0);      // the socket value is 101, which reads as 0
    char body[] = "name=value";
    http.SetPostForTest(body, 10, true);
    CHECK(http.IsSendByPost());
    CHECK(http.GetRequestData() == body);
    CHECK(http.GetRequestLength() == 10);
    http.SetResponseLen(4);
    http.SetResponseForTest("ok!");
    CHECK(std::strcmp(http.GetResponse(), "ok!") == 0);

    std::printf("%s\n", failures ? "glx_player_socket: FAILED" : "glx_player_socket: ok");
    return failures != 0;
}
