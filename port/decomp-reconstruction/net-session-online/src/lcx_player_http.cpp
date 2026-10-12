#include "net-session-online/lcx_player_http.hpp"

#include <algorithm>
#include <cstring>
#include <string>

namespace {

std::unique_ptr<char[]> CopyText(const char* text) {
    if (text == nullptr) {
        return nullptr;
    }
    const std::size_t length = std::strlen(text);
    std::unique_ptr<char[]> copy(new char[length + 1]);
    std::memcpy(copy.get(), text, length + 1);
    return copy;
}

std::unique_ptr<char[]> CopyBytes(const char* data, std::size_t length) {
    std::unique_ptr<char[]> copy(new char[length + 1]());
    if (data != nullptr && length > 0) {
        std::memcpy(copy.get(), data, length);
    }
    return copy;
}

// The owned text as a std::string; an absent text is empty (the original strcats a null,
// which is undefined, so this is a deviation only on that path).
std::string Text(const std::unique_ptr<char[]>& text) {
    return text ? std::string(text.get()) : std::string();
}

// Stand-in for the original's host extraction (LC_API_PARSE_DATA on the URL, not recovered).
// The host is not extracted, so the Host header is empty. This is why the send functions
// are partial.
std::string HostOf(const char* url) {
    (void)url;
    return std::string();
}

// Stand-in for ALicenseCheck_GetUserAgent (the ALicenseCheck functions are not identified).
// The User-Agent is empty.
std::string UserAgent() {
    return std::string();
}

// The inline request buffer is 0x400 bytes (the original's m_abBufferIn is 1024). The
// original writes without a bound; the port keeps the first 0x3ff bytes.
void StoreRequest(char (&buffer)[0x400], const std::string& text) {
    std::memset(buffer, 0, sizeof(buffer));
    const std::size_t length = std::min(text.size(), sizeof(buffer) - 1);
    std::memcpy(buffer, text.data(), length);
}

}  // namespace

// 0x89d950 and 0x89d9f8 _ZN13LCXPlayerHttpC1EPcS0_S0_
// Partial: the socket comes from LCXPlayerSocketFactory (partial). The observer is this.
LCXPlayerHttp::LCXPlayerHttp(const char* serverAddress, const char* phpScriptVersion,
                             const char* phpAddress)
    : socket_(LCXPlayerSocketFactory::GetSocket(serverAddress, 80, this)),
      serverAddress_(CopyText(serverAddress)),
      phpScriptVersion_(CopyText(phpScriptVersion)),
      phpAddress_(CopyText(phpAddress)) {}

// 0x89d4fc _ZN13LCXPlayerHttpD1Ev
// Partial: the original calls LCXPlayerHttp::Cancel (0x89d42c) first, which is not ported
// here. The owned text and buffers are released by their unique_ptr members.
LCXPlayerHttp::~LCXPlayerHttp() {
    delete socket_;
}

// 0x89d4a0 _ZN13LCXPlayerHttp15SetResponseDataEPci
void LCXPlayerHttp::SetResponseData(const char* data, std::int32_t size) {
    const std::size_t length = static_cast<std::size_t>(size);
    response_.reset(new char[length + 1]());
    if (data != nullptr) {
        std::memcpy(response_.get(), data, length);
    }
    responseLength_ = size;
}

// 0x89dd70 _ZN13LCXPlayerHttp12downloadFileEPcS0_S0_S0_ll
// The Range header carries the start, and the end when start < end. Keep-alive, Accept and
// User-Agent lines are as the original has them.
void LCXPlayerHttp::downloadFile(const char* server, const char* object, const char* cookie,
                                 const char* referer, long from, long to) {
    std::string request = std::string("GET ") + (object != nullptr ? object : "") + " HTTP/1.1"
                          "\r\n" "Host:" + (server != nullptr ? server : "") + "\r\n";
    if (referer != nullptr) {
        request += std::string("Referer:") + referer + "\r\n";
    }
    request += "Accept:*/*\r\n"
               "User-Agent:Mozilla/4.0 (compatible; MSIE 5.00; Windows 98)\r\n"
               "Connection:Keep-Alive\r\n";
    if (cookie != nullptr) {
        request += std::string("Set Cookie:0") + cookie + "\r\n";
    }
    if (from > 0) {
        request += "Range: bytes=" + std::to_string(from) + "-";
        if (from < to) {
            request += std::to_string(to);
        }
        request += "\r\n";
    }
    request += "\r\n";

    StoreRequest(request_, request);
    sendByPost_ = false;
    response_.reset();
    responseLength_ = 0;
    socket_->Start();
    response_.reset();
    responseLength_ = 0;
}

// 0x89d6a8 _ZN13LCXPlayerHttp18sendByGetWithNoVerEPcS0_
// Partial: the Host value and the User-Agent are stand-ins (see the helpers above).
// The target is the PHP address when one was given, otherwise the URL.
void LCXPlayerHttp::sendByGetWithNoVer(const char* url, const char* query) {
    if (url == nullptr || query == nullptr) {
        return;
    }
    const std::string target = phpAddress_ ? std::string(phpAddress_.get()) : std::string(url);
    const std::string request = std::string("GET ") + target + "?" + query + " HTTP/1.1\r\n"
                                "Host: " + HostOf(url) + "\r\nUser-Agent: " + UserAgent() +
                                "\r\n\r\n";

    StoreRequest(request_, request);
    response_.reset();
    sendByPost_ = false;
    socket_->Start();
    response_.reset();
    responseLength_ = 0;
}

// 0x89e2e8 _ZN13LCXPlayerHttp9sendByGetEPcS0_
// Partial: the Host value is a stand-in. The version parameter is appended after the query.
void LCXPlayerHttp::sendByGet(const char* url, const char* query) {
    if (url == nullptr || query == nullptr) {
        return;
    }
    const std::string request = std::string("GET ") + url + "?" + query + "&v=" +
                                Text(phpScriptVersion_) + " HTTP/1.1\r\n" + "Host: " + HostOf(url) +
                                "\r\n\r\n";

    response_.reset();
    StoreRequest(request_, request);
    sendByPost_ = false;
    socket_->Start();
    response_.reset();
    responseLength_ = 0;
}

// 0x89e048 _ZN13LCXPlayerHttp10sendByPostEPcS0_
// Partial: the Host value is a stand-in. The post body is the request head followed by the
// query (with "&v=<version>" appended); the original appends to the caller's query in place,
// the port works on a copy.
void LCXPlayerHttp::sendByPost(const char* url, const char* query) {
    if (url == nullptr || query == nullptr) {
        return;
    }
    const std::string fullQuery = std::string(query) + "&v=" + Text(phpScriptVersion_);
    const std::string head = std::string("POST ") + url + " HTTP/1.1\r\n" + "Host: " + HostOf(url) +
                             "\r\n" + "Content-type: application/x-www-form-urlencoded\r\n" +
                             "Content-length: " + std::to_string(fullQuery.size()) + "\r\n\r\n";
    const std::string body = head + fullQuery;

    response_.reset();
    StoreRequest(request_, head);
    postBody_ = CopyBytes(body.data(), body.size());
    postLength_ = static_cast<std::int32_t>(body.size());
    sendByPost_ = true;
    socket_->Start();
    response_.reset();
    responseLength_ = 0;
}

// 0x89daa0 _ZN13LCXPlayerHttp15sendVideoByPostEPcS0_S0_RiS0_
// Partial: the Host value is a stand-in. The multipart part is built as the pseudocode has
// it: the video bytes between a part header and a closing boundary. The post buffer is the
// HTTP head followed by that part. size is replaced by the total post length.
void LCXPlayerHttp::sendVideoByPost(const char* url, const char* query, const char* data,
                                    std::int32_t& size, const char* videoId) {
    if (url == nullptr || query == nullptr) {
        return;
    }
    const std::string fullQuery = std::string(query) + "&v=" + Text(phpScriptVersion_);

    std::string multipart = std::string("------GL_HTTP_POST_AVI\r\n") +
                            "Content-Disposition: form-data; name=\"file\"; filename=\"" +
                            (videoId != nullptr ? videoId : "") + "\"\r\n" +
                            "Content-Type: video/avi\r\n\r\n";
    multipart.append(data != nullptr ? data : "", static_cast<std::size_t>(size));
    multipart += "\r\n------GL_HTTP_POST_AVI--\r\n";

    const std::string head = std::string("POST ") + url + "?" + fullQuery + " HTTP/1.1\r\n" +
                             "Host: " + HostOf(url) + "\r\n" +
                             "Accept: */*\r\n" +
                             "Content-Type: multipart/form-data; boundary=----GL_HTTP_POST_AVI\r\n" +
                             "Content-length: " + std::to_string(multipart.size()) + "\r\n" +
                             "Proxy-Connection: Keep-Alive\r\n" +
                             "Pragma: no-cache\r\n\r\n";
    const std::string body = head + multipart;

    response_.reset();
    postBody_ = CopyBytes(body.data(), body.size());
    size = static_cast<std::int32_t>(body.size());
    postLength_ = size;
    sendByPost_ = true;
    socket_->Start();
    response_.reset();
    responseLength_ = 0;
}

// 0x89d484 _ZN13LCXPlayerHttp16GetRequestLengthEv
// The GET path is LC_API_STRLEN (a strlen wrapper) on the inline buffer.
std::uint32_t LCXPlayerHttp::GetRequestLength() const {
    if (sendByPost_) {
        return static_cast<std::uint32_t>(postLength_);
    }
    return static_cast<std::uint32_t>(std::strlen(request_));
}

// Not in the original: test seams.
void LCXPlayerHttp::SetPostForTest(const char* body, std::int32_t length, bool byPost) {
    postBody_ = CopyBytes(body, static_cast<std::size_t>(length));
    postLength_ = length;
    sendByPost_ = byPost;
}

void LCXPlayerHttp::SetResponseForTest(const char* text) {
    response_ = CopyText(text);
}

void LCXPlayerHttp::SetRequestForTest(const char* text) {
    std::strncpy(request_, text, sizeof(request_) - 1);
    request_[sizeof(request_) - 1] = '\0';
}
