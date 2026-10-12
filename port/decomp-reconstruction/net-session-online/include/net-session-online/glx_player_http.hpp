// GLXPlayerHttp: the HTTP request buffer of the GLX player services. A request is
// either held in the inline buffer (GET) or in a separately allocated post body
// (POST). It also holds the response and the socket that carries it. The request
// builders (sendByGet, sendByPost, sendVideoByPost) are not reconstructed yet.
//
// Layout (original 32-bit offsets, kept as comments):
//   +0x04 socket (GLXPlayerSocket*, owned)
//   +0x08 inline request buffer (0x400 bytes, GET request text)
//   +0x408 response text (owned)           +0x40c response length (int)
//   +0x410 server address (owned copy of ctor argument 1)
//   +0x414 owned copy of ctor argument 2   +0x418 owned copy of ctor argument 3
//   +0x41c post body (owned)               +0x420 post body length (int)
//   +0x424 send-by-post flag (byte)
#pragma once

#include <cstdint>
#include <memory>
#include <utility>

#include "net-session-online/glx_player_socket.hpp"

class GLXPlayerHttp : public GLXPlayerSocketObserver {
public:
    // Not in the original: a default object with no socket, used by the tests.
    GLXPlayerHttp() = default;
    // 0x82deac and 0x82df54 _ZN13GLXPlayerHttpC1EPcS0_S0_. Keeps copies of the three
    // strings and creates the socket on port 0x50 with this object as its observer.
    GLXPlayerHttp(const char* serverAddress, const char* second, const char* third);
    // 0x82d298 (D1) and 0x82d37c (D2). The deleting destructor 0x82d360 is not ported.
    ~GLXPlayerHttp() override;

    // 0x82ce08 _ZN13GLXPlayerHttp5StartEv
    void Start();
    // 0x82ce48 _ZN13GLXPlayerHttp13UpdateRequestEv
    void UpdateRequest();
    // 0x82ce6c _ZN13GLXPlayerHttp12IsInProgressEv
    std::int32_t IsInProgress() const;
    // 0x82ce98 _ZN13GLXPlayerHttp15IsErrorOccurredEv
    std::int32_t IsErrorOccurred() const;
    // 0x82ceec _ZN13GLXPlayerHttp6CancelEv
    void Cancel();

    // 0x82ce00 _ZN13GLXPlayerHttp12IsSendByPostEv
    bool IsSendByPost() const { return sendByPost_; }
    // 0x82ce2c _ZN13GLXPlayerHttp16GetServerAddressEv
    const char* GetServerAddress() const { return serverAddress_.get(); }
    // 0x82ce34 _ZN13GLXPlayerHttp14GetRequestDataEv: the post body when sending by POST,
    // otherwise the inline buffer.
    const char* GetRequestData() const { return sendByPost_ ? postBody_.get() : request_; }
    // 0x82cec4 _ZN13GLXPlayerHttp11GetResponseEv
    const char* GetResponse() const { return response_.get(); }
    // 0x82cecc _ZN13GLXPlayerHttp14GetResponseLenEv
    std::int32_t GetResponseLen() const { return responseLength_; }
    // 0x82ced4 _ZN13GLXPlayerHttp14SetResponseLenEi
    void SetResponseLen(std::int32_t length) { responseLength_ = length; }
    // 0x82d23c _ZN13GLXPlayerHttp15SetResponseDataEPci: replaces the response with a copy
    // of the first length bytes, zero-terminated.
    void SetResponseData(const char* data, std::int32_t length);
    // 0x82cedc _ZN13GLXPlayerHttp17GetSendPercentageEv
    // The socket's percentage, or 0 when no socket is set. The original tail-calls the
    // socket method, so the value comes back unchanged.
    std::uint32_t GetSendPercentage() const;
    // 0x82d220 _ZN13GLXPlayerHttp16GetRequestLengthEv
    // The post body length when sending by POST, otherwise the length of the inline text.
    std::uint32_t GetRequestLength() const;

    // Not in the original: test seams.
    void SetRequestForTest(const char* text);
    void SetPostForTest(const char* body, std::int32_t length, bool byPost);
    void SetServerAddressForTest(const char* address);
    void SetResponseForTest(const char* text);
    void SetSocketForTest(std::unique_ptr<GLXPlayerSocket> socket) { socket_ = std::move(socket); }

private:
    std::unique_ptr<GLXPlayerSocket> socket_;   // +0x04
    char request_[0x400] = {};                  // +0x08
    std::unique_ptr<char[]> response_;          // +0x408
    std::int32_t responseLength_ = 0;           // +0x40c
    std::unique_ptr<char[]> serverAddress_;     // +0x410
    std::unique_ptr<char[]> field414_;          // +0x414 (ctor argument 2)
    std::unique_ptr<char[]> field418_;          // +0x418 (ctor argument 3)
    std::unique_ptr<char[]> postBody_;          // +0x41c
    std::int32_t postLength_ = 0;               // +0x420
    bool sendByPost_ = false;                   // +0x424
};
