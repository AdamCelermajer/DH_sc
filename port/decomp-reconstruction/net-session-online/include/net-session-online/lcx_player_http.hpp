// LCXPlayerHttp: the lobby-side HTTP request buffer. It shares the request, response
// and length fields with GLXPlayerHttp (see glx_player_http.hpp). Its socket is an
// abstract interface whose vtable is not reconstructed, so IsInProgress and
// IsErrorOccurred (which call it) are queued.
//
// Layout (original 32-bit offsets, kept as comments):
//   +0x04 socket (LCX socket pointer)      +0x08 inline request buffer (0x400 bytes)
//   +0x408 response text                   +0x40c response length (int)
//   +0x41c post body                       +0x420 post body length (int)
//   +0x424 send-by-post flag (byte)
#pragma once

#include <cstdint>

class LCXPlayerHttp {
public:
    // 0x89d3ac _ZN13LCXPlayerHttp14GetRequestDataEv
    const char* GetRequestData() const { return sendByPost_ ? postBody_ : request_; }
    // 0x89d414 _ZN13LCXPlayerHttp11GetResponseEv
    const char* GetResponse() const { return response_; }
    // 0x89d41c _ZN13LCXPlayerHttp14GetResponseLenEv
    std::int32_t GetResponseLen() const { return responseLength_; }
    // 0x89d424 _ZN13LCXPlayerHttp14SetResponseLenEi
    void SetResponseLen(std::int32_t length) { responseLength_ = length; }
    // 0x89d484 _ZN13LCXPlayerHttp16GetRequestLengthEv
    // The post body length when sending by POST, otherwise the length of the inline text.
    std::uint32_t GetRequestLength() const;

    // Not in the original: test seams.
    void SetRequestForTest(const char* text);
    void SetPostForTest(char* body, std::int32_t length, bool byPost) {
        postBody_ = body;
        postLength_ = length;
        sendByPost_ = byPost;
    }
    void SetResponseForTest(const char* text) { response_ = text; }

private:
    void* socket_ = nullptr;             // +0x04
    char request_[0x400] = {};           // +0x08
    const char* response_ = nullptr;     // +0x408
    std::int32_t responseLength_ = 0;    // +0x40c
    char* postBody_ = nullptr;           // +0x41c
    std::int32_t postLength_ = 0;        // +0x420
    bool sendByPost_ = false;            // +0x424
};
