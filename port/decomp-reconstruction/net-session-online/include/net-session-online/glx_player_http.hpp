// GLXPlayerHttp: the HTTP request buffer of the GLX player services. A request is
// either held in the inline buffer (GET) or in a separately allocated post body
// (POST). It also holds the response and the socket that carries it. Only the
// accessors are reconstructed here. Start, UpdateRequest and the dtor are queued.
//
// Layout (original 32-bit offsets, kept as comments):
//   +0x04 socket (GLXPlayerSocket*)
//   +0x08 inline request buffer (0x400 bytes, GET request text)
//   +0x408 response text (char*)           +0x40c response length (int)
//   +0x410 server address (char*)
//   +0x41c post body (char*)               +0x420 post body length (int)
//   +0x424 send-by-post flag (byte)
#pragma once

#include <cstdint>

#include "net-session-online/glx_player_socket.hpp"

class GLXPlayerHttp {
public:
    // 0x82ce00 _ZN13GLXPlayerHttp12IsSendByPostEv
    bool IsSendByPost() const { return sendByPost_; }
    // 0x82ce2c _ZN13GLXPlayerHttp16GetServerAddressEv
    const char* GetServerAddress() const { return serverAddress_; }
    // 0x82ce34 _ZN13GLXPlayerHttp14GetRequestDataEv: the post body when sending by POST,
    // otherwise the inline buffer.
    const char* GetRequestData() const { return sendByPost_ ? postBody_ : request_; }
    // 0x82cec4 _ZN13GLXPlayerHttp11GetResponseEv
    const char* GetResponse() const { return response_; }
    // 0x82ced4 _ZN13GLXPlayerHttp14SetResponseLenEi
    void SetResponseLen(std::int32_t length) { responseLength_ = length; }
    // 0x82cedc _ZN13GLXPlayerHttp17GetSendPercentageEv
    // The socket's percentage, or 0 when no socket is set. The original tail-calls the
    // socket method, so the value comes back unchanged.
    std::uint32_t GetSendPercentage() const;
    // 0x82d220 _ZN13GLXPlayerHttp16GetRequestLengthEv
    // The post body length when sending by POST, otherwise the length of the inline text.
    std::uint32_t GetRequestLength() const;

    // Not in the original: test seams.
    void SetRequestForTest(const char* text);
    void SetPostForTest(char* body, std::int32_t length, bool byPost) {
        postBody_ = body;
        postLength_ = length;
        sendByPost_ = byPost;
    }
    void SetServerAddressForTest(const char* address) { serverAddress_ = address; }
    void SetResponseForTest(const char* text) { response_ = text; }
    void SetSocketForTest(GLXPlayerSocket* socket) { socket_ = socket; }

private:
    GLXPlayerSocket* socket_ = nullptr;   // +0x04
    char request_[0x400] = {};            // +0x08
    const char* response_ = nullptr;      // +0x408
    std::int32_t responseLength_ = 0;     // +0x40c
    const char* serverAddress_ = nullptr; // +0x410
    char* postBody_ = nullptr;            // +0x41c (owned by the caller in this port)
    std::int32_t postLength_ = 0;         // +0x420
    bool sendByPost_ = false;             // +0x424
};
