// Tests for the r1n-net-session-online-pack2-7d6e5f-0 batch: GLXPlayerSocket (Run, ParseHttpHeader,
// RemoveHttpHeader) with a fake transport, and the field parsers XP_API_PARSE_DATA_NEW and
// LC_API_PARSE_DATA_NEW. Build: bash tools/trackb/check_tu.sh --test <this file> <the sources below>.

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>

#include "net-session-online/glx_player_socket.hpp"
#include "net-session-online/lc_api_net.hpp"
#include "net-session-online/xp_api.hpp"

static int failures = 0;
#define CHECK(c)                                                              \
    do {                                                                      \
        if (!(c)) {                                                           \
            std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c);          \
            ++failures;                                                       \
        }                                                                     \
    } while (0)

namespace {

// Observer that records what Run reports and what it sends.
class RecordingObserver : public GLXPlayerSocketObserver {
public:
    std::string request = "GET / HTTP/1.1\r\n\r\n";
    int receivedCalls = 0;
    int responseCalls = 0;
    std::string response;

    const char* RequestData() const override { return request.c_str(); }
    std::uint32_t RequestLength() const override { return static_cast<std::uint32_t>(request.size()); }
    void OnResponse(const char* data, std::int32_t length) override {
        ++responseCalls;
        response.assign(data != nullptr ? data : "", length > 0 ? static_cast<std::size_t>(length) : 0);
    }
    void OnReceived(std::int32_t) override { ++receivedCalls; }
};

// A transport that connects at once, accepts every send, and returns one reply and then a close.
class FakeSocket : public GLXPlayerSocket {
public:
    FakeSocket(const char* host, std::int32_t port, GLXPlayerSocketObserver* observer)
        : GLXPlayerSocket(host, port, observer) {}

    std::string sent;
    int reads = 0;

    std::int32_t OpenConnection(const char* /*host*/) override { return 7; }
    std::int32_t BeginConnect() override { return 1; }
    std::int32_t IsConnected() const override { return 1; }
    std::int32_t Poll(bool /*wantWrite*/) override { return 1; }
    std::int32_t SendBytes(const char* data, std::int32_t length) override {
        sent.append(data, static_cast<std::size_t>(length));
        return length;
    }
    std::int32_t RecvBytes(char* buffer, std::int32_t length) override {
        ++reads;
        if (reads == 1) {
            const char reply[] = "HTTP/1.1 200 OK\r\nContent-Length: 2\r\n\r\nok";
            const std::int32_t n = static_cast<std::int32_t>(sizeof(reply) - 1);
            const std::int32_t count = n <= length ? n : length;
            std::memcpy(buffer, reply, static_cast<std::size_t>(count));
            return count;
        }
        return 0;  // the peer closed the connection
    }
};

}  // namespace

int main() {
    // ---- GLXPlayerSocket::Run: connect, send, receive, close ----
    {
        RecordingObserver observer;
        FakeSocket socket("h", 80, &observer);
        socket.Start();                       // state 1
        CHECK(socket.IsInProgress() == 1);
        socket.Run();                         // 1 -> 2: open and begin the connect
        socket.Run();                         // 2 -> 3: connected
        socket.Run();                         // 3: the whole request is sent, 3 -> 4
        CHECK(socket.sent == observer.request);
        CHECK(socket.GetSendPercentage() == 100);

        socket.Run();                         // 4: first reply, header removed, body kept
        CHECK(socket.ContentForTest() == "ok");
        CHECK(observer.receivedCalls == 1);
        CHECK(observer.responseCalls == 0);   // the span is not written by the platform path

        socket.Run();                         // 4: close, response reported, 4 -> 6
        CHECK(observer.responseCalls == 1);
        CHECK(socket.ContentForTest().empty());
        CHECK(socket.IsInProgress() == 0);    // state 6 is finished
        socket.Run();                         // state 6 does nothing
        CHECK(observer.responseCalls == 1);
    }

    // ---- GLXPlayerSocket::Run with no transport: the base hooks report failure ----
    // The connect start time (+0x868) is written by the platform open path, which is not ported, so it
    // is 0 here and the connect timeout is already past: the first Run goes to the error state.
    {
        RecordingObserver observer;
        GLXPlayerSocket socket("h", 80, &observer);
        socket.Start();
        socket.Run();
        CHECK(socket.IsErrorOccurred());
        CHECK(socket.IsInProgress() == 0);
    }

    // ---- ParseHttpHeader and RemoveHttpHeader ----
    {
        GLXPlayerSocket socket("h", 80, nullptr);
        socket.SetContentForTest("HTTP/1.1 200 OK\r\nContent-Length: 2\r\n\r\nok");
        CHECK(socket.ParseHttpHeader("Content-Length") == 17);
        CHECK(socket.ParseHttpHeader("X-Missing") == static_cast<std::uint32_t>(std::string::npos));
        socket.RemoveHttpHeader();
        CHECK(socket.ContentForTest() == "ok");

        socket.SetContentForTest("no terminator");
        socket.RemoveHttpHeader();            // no CRLF CRLF: nothing is dropped
        CHECK(socket.ContentForTest() == "no terminator");
    }

    // ---- XP_API_PARSE_DATA_NEW: a field that ends at the NUL keeps all its characters ----
    {
        char text[] = "ab|cd";
        char* out = nullptr;
        const int start = XP_API_PARSE_DATA_NEW(text, &out, 1, '|');
        CHECK(start == 3);
        CHECK(out != nullptr && std::strcmp(out, "cd") == 0);
        delete[] out;
    }

    // ---- LC_API_PARSE_DATA_NEW: a field that ends at a separator is copied whole; the last field loses
    //      its final character (as decompiled) ----
    {
        const char* text = "ab|cd";
        char* out = nullptr;
        CHECK(LC_API_PARSE_DATA_NEW(text, &out, 0, '|') == 0);
        CHECK(out != nullptr && std::strcmp(out, "ab") == 0);
        delete[] out;

        out = nullptr;
        CHECK(LC_API_PARSE_DATA_NEW(text, &out, 1, '|') == 3);
        CHECK(out != nullptr && std::strcmp(out, "c") == 0);
        delete[] out;
    }

    if (failures == 0) {
        std::printf("r1n-net-session-online-pack2-7d6e5f-0 tests: all passed\n");
        return 0;
    }
    std::printf("%d failure(s)\n", failures);
    return 1;
}
