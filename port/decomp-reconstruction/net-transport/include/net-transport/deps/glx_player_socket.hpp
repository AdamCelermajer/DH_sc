// Stand-in for GLXPlayerSocket, the socket object the lobby connection drives. The class and its
// factory are owned by the net-session-online category. Only the slots the connection code calls
// are declared, named by their byte offsets in the original vtable. The stand-in answers "nothing
// to read, no error", so the connection code runs without a network.
#pragma once

#include <cstdint>
#include <memory>
#include <string>

class GLXPlayerSocket {
public:
    virtual ~GLXPlayerSocket() = default;                        // +0x04 (deleting call)
    virtual int Start() { return 0; }                            // +0x28: begin a connection, nonzero on success
    virtual int Connect(const char* host, int flags) {           // +0x38: connect step for host
        (void)host;
        (void)flags;
        return 0;
    }
    virtual void Close() {}                                      // +0x3c
    virtual int Select(int mode) {                               // +0x40: 0 read ready, 1 write ready; <0 on error
        (void)mode;
        return 0;
    }
    virtual int Send(const void* data, int length) {             // +0x44: bytes sent, <0 on error
        (void)data;
        (void)length;
        return 0;
    }
    virtual int Recv(void* data, int length) {                   // +0x48: bytes received, 0 when closed
        (void)data;
        (void)length;
        return 0;
    }
    virtual bool HasPendingData() const { return false; }        // +0x4c

    // Connect attempts made so far (+0x85c of the original socket object). The connection code
    // resets it when a connect succeeds or gives up.
    int connect_attempts_ = 0;
};

// GLXPlayerSocketFactory::GetSocket(host, port, flags). Stand-in: a socket that never connects.
class GLXPlayerSocketFactory {
public:
    static std::unique_ptr<GLXPlayerSocket> GetSocket(const std::string& host, int port, int flags) {
        (void)host;
        (void)port;
        (void)flags;
        return std::make_unique<GLXPlayerSocket>();
    }
};
