#include "net-session-online/glx_player_web_component.hpp"

#include <cstdlib>
#include <cstring>

#include "net-session-online/deps/glx_stand_ins.hpp"
#include "net-session-online/deps/xp_api.hpp"
#include "net-session-online/glx_player_observer.hpp"

namespace {
// Token buffers in the original are 256-byte stack arrays.
constexpr std::size_t kTokenBytes = 256;
constexpr std::int32_t kGenericFunctionOffset = 500;  // 'g' kind offset
}  // namespace

// 0x8320d0 _ZN21GLXPlayerWebComponentC1Ev, 0x832134 _ZN21GLXPlayerWebComponentC2Ev
GLXPlayerWebComponent::GLXPlayerWebComponent() = default;

// 0x831f14 _ZN21GLXPlayerWebComponentD1Ev, 0x832000 _ZN21GLXPlayerWebComponentD2Ev
// Partial: the original releases requestClient_ through a vtable slot of its
// own type, which is not identified, so the client is not released here.
GLXPlayerWebComponent::~GLXPlayerWebComponent() = default;

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
// Partial: the 520-byte response parser is not reconstructed yet.
void GLXPlayerWebComponent::OnUpdateParse() {}

// 0x831bd4 _ZN21GLXPlayerWebComponent10SendByPostEPc
// Partial: the original sets enabled and encodes the request with
// SSEncDec_String2Blob, then hands it to the request client. The encoder and the
// client are not reconstructed, so only the enabled flag is set.
void GLXPlayerWebComponent::SendByPost(char* request) {
    (void)request;
    enabled_ = true;
}

// 0x831cd4 _ZN21GLXPlayerWebComponent9SendByGetEPc
// Partial: the original sets enabled, records the request id from the request text
// (getValue, not reconstructed), stamps the time and calls the request client. Only
// the enabled flag and the time stamp are set here.
void GLXPlayerWebComponent::SendByGet(char* request) {
    (void)request;
    enabled_ = true;
    requestTime_ = XP_API_GET_TIME();
}

// 0x831c88 _ZN21GLXPlayerWebComponent6CancelEv
// Partial: the request client's abort slot (+0x38 of its vtable) is not identified and
// is not called here.
void GLXPlayerWebComponent::Cancel() {
    enabled_ = false;
    XP_DEBUG_OUT("GLXPlayerWebComponent::Cancel(): m_enable is changed to false!!\n");
    cursor_ = nullptr;
    functionId_ = 0;
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
