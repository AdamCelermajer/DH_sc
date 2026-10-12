#include "net-session-online/glx_player_socket.hpp"

#include <cctype>
#include <cstdlib>
#include <cstring>

namespace {

constexpr std::size_t kNpos = std::string::npos;

// The original's std::string::substr throws out_of_range past the end. The port returns an
// empty string instead, and the caller's loop then ends.
std::string SubstrOrEmpty(const std::string& text, std::size_t pos, std::size_t count = kNpos) {
    if (pos > text.size()) {
        return std::string();
    }
    return text.substr(pos, count);
}

// XP_API_TOLOWER over a whole string.
std::string LowerCopy(std::string text) {
    for (char& c : text) {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
    return text;
}

}  // namespace

// 0x8302f0 and 0x83039c _ZN15GLXPlayerSocketC1EPciP23GLXPlayerSocketObserver
// The ARM listing copies the host name with XP_API_STRNEW and stores null when the
// argument is null. The decompiled body omits that copy.
GLXPlayerSocket::GLXPlayerSocket(const char* host, std::int32_t port,
                                 GLXPlayerSocketObserver* observer)
    : port_(port), observer_(observer) {
    if (host != nullptr) {
        const std::size_t length = std::strlen(host);
        host_.reset(new char[length + 1]);
        std::memcpy(host_.get(), host, length + 1);
    }
}

// 0x8301ac _ZN15GLXPlayerSocket5StartEv
void GLXPlayerSocket::Start() {
    receiveLength_ = 0;
    std::memset(receiveBuffer_, 0, sizeof(receiveBuffer_));
    socket_ = -1;
    state_ = 1;
    field0c_ = 0;
}

// 0x83007c _ZN15GLXPlayerSocket6CancelEv
// Only an active socket (state not 0, 6, 7) is cancelled. The vtable hook runs last.
void GLXPlayerSocket::Cancel() {
    const std::int32_t state = state_;
    if (state != 0 && state != 7 && state != 6) {
        state_ = 8;
        field85c_ = 0;
        OnCancelled();
    }
}

// 0x8300bc _ZN15GLXPlayerSocket12IsInProgressEv
// States 0, 7 and 8 are not in progress. State 6 gives 0 and every other state gives 1.
std::int32_t GLXPlayerSocket::IsInProgress() const {
    if (state_ != 0 && state_ != 7 && state_ != 8) {
        return state_ - 6 != 0 ? 1 : 0;
    }
    return 0;
}

// 0x830cb0 _ZN15GLXPlayerSocket19CalculateTotalLengthEv
std::int32_t GLXPlayerSocket::CalculateTotalLength() const {
    constexpr const char* kUpper = "\r\nContent-Length:";
    constexpr const char* kLower = "\r\nContent-length:";

    const std::size_t headerEnd = content_.find("\r\n\r\n", 0, 4);
    if (headerEnd == kNpos) {
        return -1;
    }
    const std::string head = content_.substr(0, headerEnd);

    const std::size_t upper = head.find(kUpper, 0, std::strlen(kUpper));
    if (upper != kNpos) {
        std::string value = SubstrOrEmpty(head, upper + std::strlen(kUpper));
        const std::size_t lineEnd = value.find("\r\n", 0, 2);
        if (lineEnd == kNpos) {
            return -1;
        }
        value = value.substr(0, lineEnd);
        return std::atoi(value.c_str());
    }

    const std::size_t lower = head.find(kLower, 0, std::strlen(kLower));
    if (lower == kNpos) {
        return 0;
    }
    std::string value = SubstrOrEmpty(head, lower + std::strlen(kLower));
    const std::size_t lineEnd = value.find("\r\n");
    if (lineEnd == kNpos) {
        return -1;
    }
    value = value.substr(0, lineEnd);
    return std::atoi(value.c_str());
}

// 0x830448 _ZN15GLXPlayerSocket15ParseHttpHeaderEPc
// The original copies the header block with npos bytes when the CRLF CRLF terminator is
// missing. That is undefined, so the port returns npos there. This is partial.
std::uint32_t GLXPlayerSocket::ParseHttpHeader(const char* name) const {
    const std::string lowerName = LowerCopy(name != nullptr ? name : "");
    const std::size_t end = content_.find("\r\n\r\n", 0, 4);
    if (end == kNpos) {
        return static_cast<std::uint32_t>(kNpos);
    }
    const std::string header = LowerCopy(content_.substr(0, end));
    return static_cast<std::uint32_t>(header.find(lowerName, 0));
}

// 0x8305bc _ZN15GLXPlayerSocket16RemoveHttpHeaderEv
void GLXPlayerSocket::RemoveHttpHeader() {
    const std::size_t end = content_.find("\r\n\r\n", 0, 4);
    if (end != kNpos && end > 0) {
        content_ = SubstrOrEmpty(content_, end + std::strlen("\r\n\r\n"));
    }
}

// 0x83067c _ZN15GLXPlayerSocket18ParseChunkedContentEv
// Walks the chunks in the content, appends each chunk's data to content_ and returns 1.
// The original computes the next chunk offset as (CRLF position + 2 + CRLF position), and
// the size line is cut to the absolute CRLF position. Both are kept as decompiled. This
// is partial: multi-chunk bodies may not decode the way the server meant.
std::int32_t GLXPlayerSocket::ParseChunkedContent() {
    const std::string raw = content_;
    content_.clear();

    std::size_t crlf = raw.find("\r\n", 0, 2);
    if (crlf == kNpos || crlf == 0) {
        return 0;
    }
    std::string line = raw.substr(0, crlf);

    std::size_t space = line.find(" ", 0, 1);
    if (space != kNpos && space > 0) {
        line = line.substr(0, space);
    }
    std::int32_t chunkSize = static_cast<std::int32_t>(std::strtol(line.c_str(), nullptr, 16));
    std::size_t pos = crlf + 2;

    while (chunkSize > 0) {
        content_.append(SubstrOrEmpty(raw, pos, static_cast<std::size_t>(chunkSize)));
        const std::size_t dataEnd = pos + static_cast<std::size_t>(chunkSize);
        const std::size_t next = dataEnd + 2;
        crlf = raw.find("\r\n", next, 2);
        if (crlf == kNpos || crlf == 0) {
            return 0;
        }
        line = SubstrOrEmpty(raw, dataEnd + 2, crlf);
        space = line.find(" ", 0, 1);
        if (space != kNpos && space > 0) {
            line = line.substr(0, space);
        }
        chunkSize = static_cast<std::int32_t>(std::strtol(line.c_str(), nullptr, 16));
        pos = next + 2 + crlf;
    }
    return 1;
}

// 0x83006c _ZN15GLXPlayerSocket17GetSendPercentageEv
std::uint32_t GLXPlayerSocket::GetSendPercentage() const {
    return sendPercentage_ > 100 ? 0 : sendPercentage_;
}

// 0x8302d8 _ZN15GLXPlayerSocket11SetAcceptIPEPc
// The original calls XP_API_STRNEW (operator new[]) on the argument, so the argument must
// not be null. The port copies a null argument as an empty string.
void GLXPlayerSocket::SetAcceptIP(const char* address) {
    const char* text = address != nullptr ? address : "";
    const std::size_t length = std::strlen(text);
    acceptIp_.reset(new char[length + 1]);
    std::memcpy(acceptIp_.get(), text, length + 1);
}

// 0x831528 _ZN22GLXPlayerSocketFactory9GetSocketEPciP23GLXPlayerSocketObserver
GLXPlayerSocket* GLXPlayerSocketFactory::GetSocket(const char* host, std::int32_t port,
                                                   GLXPlayerSocketObserver* observer) {
    return new GLXPlayerSocket(host, port, observer);
}
