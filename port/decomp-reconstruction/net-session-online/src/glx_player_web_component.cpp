#include "net-session-online/glx_player_web_component.hpp"

#include <cstdlib>
#include <cstring>

#include "net-session-online/deps/glx_stand_ins.hpp"
#include "net-session-online/deps/xp_api.hpp"
#include "net-session-online/glx_player_observer.hpp"
#include "net-session-online/glx_text.hpp"

namespace {
// Token buffers in the original are 256-byte stack arrays.
constexpr std::size_t kTokenBytes = 256;
constexpr std::int32_t kGenericFunctionOffset = 500;  // 'g' kind offset
}  // namespace

// 0x8320d0 _ZN21GLXPlayerWebComponentC1Ev, 0x832134 _ZN21GLXPlayerWebComponentC2Ev
GLXPlayerWebComponent::GLXPlayerWebComponent() = default;

// 0x831f14 _ZN21GLXPlayerWebComponentD1Ev, 0x832000 _ZN21GLXPlayerWebComponentD2Ev
// Partial: the original releases requestClient_ through vtable slot +0x14 of its type, which
// is not identified. The port deletes the client through its unique_ptr. The owned strings
// are released here, as in the original.
GLXPlayerWebComponent::~GLXPlayerWebComponent() {
    cursor_ = nullptr;
    string10_.reset();
    string14_.reset();
    string18_.reset();
    url_.reset();
    requestClient_.reset();
    requestTime_ = 0;
    requestId_ = -1;
}

// 0x83155c _ZN21GLXPlayerWebComponent19StartResponseParserEPci
void GLXPlayerWebComponent::StartResponseParser(const char* response, std::int32_t functionId) {
    functionId_ = functionId;
    cursor_ = response;
}

// 0x831568 _ZN21GLXPlayerWebComponent13GetFunctionIDEci
std::int32_t GLXPlayerWebComponent::GetFunctionID(char kind, std::int32_t id) {
    if (kind == 'f') {
        return id;
    }
    if (kind == 'g') {
        return id + kGenericFunctionOffset;
    }
    return -1;
}

// 0x831588 _ZN21GLXPlayerWebComponent15OnUpdateSuccessEi
// The observer slot 2 call is in the ARM listing (blx through [observer+8]), not in
// the pseudocode, which ends in a tail call. Text is "" when no response is set.
void GLXPlayerWebComponent::OnUpdateSuccess(std::int32_t arg) {
    const char* text = cursor_ != nullptr ? cursor_ : "";
    if (observer_ != nullptr) {
        observer_->OnResponseText(arg, text, static_cast<std::int32_t>(std::strlen(text)));
    }
}

// 0x8315c4 _ZN21GLXPlayerWebComponent10LoadConfigEv
// Partial: the original copies the service URL from GLXPlayerSereverConfig::GetXPlayerURL
// into url_ and then parses it. Neither step is reconstructed yet.
void GLXPlayerWebComponent::LoadConfig() {}

// 0x8319b0 _ZN21GLXPlayerWebComponent13OnUpdateParseEv
// Partial: the token dispatch is reconstructed. The observer slot +0x00 call on the failure
// paths (OnDownloadError) passes an argument that the pseudocode does not show, so the port
// passes 0 there. The original's StartResponseParser call passes the response length as the
// function id, which this keeps.
void GLXPlayerWebComponent::OnUpdateParse() {
    const char* response = requestClient_ ? requestClient_->GetResponse() : nullptr;
    const std::int32_t length = requestClient_ ? requestClient_->GetResponseLen() : 0;
    StartResponseParser(response, length);

    const auto notifyDownloadError = [this]() {
        if (observer_ != nullptr) {
            observer_->OnDownloadError(0);
        }
    };
    if (cursor_ == nullptr || functionId_ == 0) {
        notifyDownloadError();
        return;
    }

    char token[0x1000] = {};
    GetNextResponseToken(token);
    char kind = 0;
    if (std::strcmp(token, "f") == 0) {
        kind = 'f';
    } else if (std::strcmp(token, "g") == 0) {
        kind = 'g';
    } else {
        notifyDownloadError();
        return;
    }
    std::memset(token, 0, sizeof(token));
    GetNextResponseToken(token);
    const std::int32_t functionId = GetFunctionID(kind, std::atoi(token));
    if (!IsNextResponseStringToken("r")) {
        notifyDownloadError();
        return;
    }
    std::memset(token, 0, sizeof(token));
    GetNextResponseToken(token);
    if (std::strcmp(token, "s") == 0) {
        OnUpdateSuccess(functionId);
    } else if (std::strcmp(token, "e") == 0) {
        OnUpdateFailure(functionId);
    }
}

// 0x831bd4 _ZN21GLXPlayerWebComponent10SendByPostEPc
// Partial: the original sets enabled and encodes the request with SSEncDec_String2Blob
// (not identified), builds "b=<blob>" and hands it to the request client's POST slot. Only
// the enabled flag is set here.
void GLXPlayerWebComponent::SendByPost(char* request) {
    (void)request;
    enabled_ = true;
}

// 0x831cd4 _ZN21GLXPlayerWebComponent9SendByGetEPc
// Partial: the request id is the second '|' field of the request, read from the raw text,
// and the time is stamped. The encoded "b=<blob>" text (SSEncDec_String2Blob, not identified)
// and the client's GET send are not reconstructed.
void GLXPlayerWebComponent::SendByGet(char* request) {
    enabled_ = true;
    requestId_ = std::atoi(GLXField(request, 1, '|').c_str());
    requestTime_ = XP_API_GET_TIME();
}

// 0x831c88 _ZN21GLXPlayerWebComponent6CancelEv
// The original returns the request client's cancel result (an int in the pseudocode). This
// port returns nothing, since no caller uses it. The cancel call is the client's Cancel.
void GLXPlayerWebComponent::Cancel() {
    enabled_ = false;
    XP_DEBUG_OUT("GLXPlayerWebComponent::Cancel(): m_enable is changed to false!!\n");
    cursor_ = nullptr;
    functionId_ = 0;
    if (requestClient_) {
        requestClient_->Cancel();
    }
    requestId_ = -1;
    requestTime_ = 0;
}

// 0x831dfc _ZN21GLXPlayerWebComponent6UpdateEv
// Partial: the poll of the request client and the 18000 ms timeout branch are not
// reconstructed; they need the client's slots.
void GLXPlayerWebComponent::Update() {}

// 0x83197c _ZN21GLXPlayerWebComponent15OnUpdateFailureEi
bool GLXPlayerWebComponent::OnUpdateFailure(std::int32_t code) {
    const std::int32_t detail = GetNextResponseIntToken();
    if (observer_ != nullptr) {
        observer_->OnResponseError(code, detail);
    }
    return true;
}

// 0x831744 _ZN21GLXPlayerWebComponent20GetNextResponseTokenEPc
char* GLXPlayerWebComponent::GetNextResponseToken(char* out) {
    const char* start = cursor_;
    if (start == nullptr || *start == '\0') {
        out[0] = '\0';
        return out;
    }
    const char* p = start;
    std::size_t length = 0;
    if (*start == '|') {
        // Empty token: skip the separator.
        cursor_ = start + 1;
    } else {
        for (;;) {
            ++p;
            cursor_ = p;
            if (*p == '\0') {
                break;  // end of text: the cursor stays on the terminator
            }
            if (*p == '|') {
                cursor_ = p + 1;
                break;
            }
        }
        length = static_cast<std::size_t>(p - start);
    }
    if (length != 0) {
        std::memcpy(out, start, length);
        out[length] = '\0';
        return out;
    }
    out[0] = '\0';
    return out;
}

// 0x8317dc _ZN21GLXPlayerWebComponent22IsNextResponseIntTokenEi
// The pseudocode ends in a call that looks like a stack check. The ARM listing shows
// the function returns (atoi(token) == expected).
bool GLXPlayerWebComponent::IsNextResponseIntToken(std::int32_t expected) {
    char token[kTokenBytes] = {};
    GetNextResponseToken(token);
    return std::atoi(token) == expected;
}

// 0x83186c _ZN21GLXPlayerWebComponent25IsNextResponseStringTokenEPKc
// Returns strcmp(expected, token) == 0. The ARM listing confirms the 0/1 result.
bool GLXPlayerWebComponent::IsNextResponseStringToken(const char* expected) {
    char token[kTokenBytes] = {};
    GetNextResponseToken(token);
    return std::strcmp(expected, token) == 0;
}

// 0x8318fc _ZN21GLXPlayerWebComponent23GetNextResponseIntTokenEv
std::int32_t GLXPlayerWebComponent::GetNextResponseIntToken() {
    char token[kTokenBytes] = {};
    GetNextResponseToken(token);
    return std::atoi(token);
}
