#include "net-session-online/lcx_player_http.hpp"
#include "net-session-online/lcx_player_socket.hpp"

#include <cstdio>
#include <cstring>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    LCXPlayerSocket sock;
    CHECK(sock.GetAcceptIP() == nullptr);
    sock.SetAcceptPort(7000);
    CHECK(sock.GetAcceptPort() == 7000);
    sock.SetSocketType(2);
    CHECK(sock.GetSocketType() == 2);
    sock.SetSocket(9);
    sock.SetAcceptIP("lobby.local");
    CHECK(std::strcmp(sock.GetAcceptIP(), "lobby.local") == 0);
    CHECK(!sock.IsErrorOccurred());
    sock.SetStateForTest(7);
    CHECK(sock.IsErrorOccurred());

    LCXPlayerHttp http;
    http.SetRequestForTest("GET /lobby");
    CHECK(std::strcmp(http.GetRequestData(), "GET /lobby") == 0);
    CHECK(http.GetRequestLength() == 10);
    char body[] = "a=b";
    http.SetPostForTest(body, 3, true);
    CHECK(http.GetRequestData() == body);
    CHECK(http.GetRequestLength() == 3);
    http.SetResponseLen(12);
    CHECK(http.GetResponseLen() == 12);
    http.SetResponseForTest("reply");
    CHECK(std::strcmp(http.GetResponse(), "reply") == 0);

    std::printf("%s\n", failures ? "lcx_player_socket: FAILED" : "lcx_player_socket: ok");
    return failures != 0;
}
