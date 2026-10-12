#include "net-session-online/cxplayer_manager.hpp"

#include <cstdio>
#include <cstring>

namespace {

// Size of the user name and password fields (+0x20 and +0x4e in the original).
constexpr std::size_t kCredentialFieldBytes = 46;

// Copies a credential like the original strcpy, but never past its 46-byte field.
void CopyCredential(char* field, const char* text) {
    std::strncpy(field, text, kCredentialFieldBytes - 1);
    field[kCredentialFieldBytes - 1] = '\0';
}

}  // namespace

// 0x52df54 _ZN15CXPlayerManagerC1Ev (C2 at 0x52df10 has the same body)
CXPlayerManager::CXPlayerManager() = default;

// 0x52e034 _ZN15CXPlayerManagerD1Ev (D2 at 0x52df98): deletes the three owned objects
// through their virtual destructors, in the original order (+0x10, +0x08, +0x0c).
CXPlayerManager::~CXPlayerManager() {
    delete third_;
    third_ = nullptr;
    delete login_;
    login_ = nullptr;
    delete leaderboard_;
    leaderboard_ = nullptr;
}

// 0x52e0d0 _ZN15CXPlayerManager11SwitchStateE20CXPlayerManagerState
void CXPlayerManager::SwitchState(CXPlayerManagerState state) { state_ = state; }

// 0x52e0d8 _ZN15CXPlayerManager7IsReadyEv: true in the state 0x10.
bool CXPlayerManager::IsReady() const { return state_ == kReadyState; }

// 0x52e0ec _ZN15CXPlayerManager10IsLoggedInEv
// The original checks: state not 0, a login object whose keep-alive byte is set, and the
// state not 8, 9 or 10. It then returns 1 unless the state is 0x0b.
int CXPlayerManager::IsLoggedIn() const {
    if (state_ != 0 && login_ != nullptr && login_->KeepAliveEnabled() && state_ != 8 && state_ != 9 &&
        state_ != 10) {
        return state_ != 0xb ? 1 : 0;
    }
    return 0;
}

// 0x52e2d4 _ZN15CXPlayerManager14OnNetworkErrorEv
void CXPlayerManager::OnNetworkError() {
    std::puts("XPlayerManager: ERROR! NETWORK ERROR!");
    lastError_ = 3;
    SwitchState(0x11);
}

// 0x52e15c _ZN15CXPlayerManager16OnRequestTimeoutEi
// In state 0x0b the score moves from +0x18 to the pending score. In any other state the
// original prints a line whose argument is missing from the pseudocode, so it is printed
// here without the value.
void CXPlayerManager::OnRequestTimeout(std::int32_t /*requestId*/) {
    if (state_ == 0xb) {
        const std::int32_t moved = score_;
        score_ = 0;
        pendingScore_ += moved;
    } else {
        std::puts("XPlayerManager: ERROR! REQUEST TIMEOUT");
    }
    lastError_ = 3;
    SwitchState(0x11);
}

// 0x52e1b4 _ZN15CXPlayerManager16OnRequestFailureEii
// Partial: the first debug line of the original has no arguments, so it is not reproduced.
// Outside state 4 the error is 3, and state 0x0b first moves the score to the pending value.
// In state 4 the error code decides: 0x2e and 0x43 give 1, 0x2f and 0x31 give 2, and the
// rest give 3. A function id other than 0x0f is reported as a state mismatch.
void CXPlayerManager::OnRequestFailure(std::int32_t functionId, std::int32_t errorCode) {
    std::printf("XPlayerManager: ERROR! REQUEST FAILURE func_id = %d, error_code = %d\n", functionId, errorCode);
    if (state_ != 4) {
        if (state_ == 0xb) {
            pendingScore_ += score_;
            score_ = 0;
        }
        lastError_ = 3;
        SwitchState(0x11);
        return;
    }
    if (functionId != 0xf) {
        std::puts("XPlayerManager: ERROR! LOGIN STATES MISSMATCH");
    }
    switch (errorCode) {
        case 0x2e:
        case 0x43:
            lastError_ = 1;
            break;
        case 0x2f:
        case 0x31:
            lastError_ = 2;
            break;
        default:
            lastError_ = 3;
            break;
    }
    SwitchState(0x11);
}

// 0x52e304 _ZN15CXPlayerManager6LogoutEv
void CXPlayerManager::Logout() {
    if (login_ != nullptr) {
        std::puts("XPlayerManager: sending logout");
        SwitchState(5);
        lastError_ = 0;
        score_ = 0;
        return;
    }
    lastError_ = 3;
    SwitchState(0x11);
}

// 0x52e358 _ZN15CXPlayerManager5LoginEPcS0_
// With no login object the manager goes to the error state. With a logged-in service it
// logs out, stores the credentials and waits in state 2. Otherwise it stores them and waits
// in state 3.
void CXPlayerManager::Login(const char* user, const char* password) {
    if (login_ == nullptr) {
        lastError_ = 3;
        SwitchState(0x11);
        return;
    }
    if (login_->KeepAliveEnabled()) {
        Logout();
        CopyCredential(userName_, user);
        CopyCredential(password_, password);
        SwitchState(2);
        return;
    }
    std::printf("XPlayerManager: sending login data username = %s password = %s\n", user, password);
    CopyCredential(userName_, user);
    CopyCredential(password_, password);
    SwitchState(3);
}

// 0x52e3f8 _ZN15CXPlayerManager16OnRequestSuccessEiPci
// State 0 creates the login and leaderboard services and waits in state 4. State 2 logs in
// with the stored credentials. States 4, 6, 0x0b, 0x0d and 0x0f are handled as in the
// original. The rest print the not-implemented line.
void CXPlayerManager::OnRequestSuccess(std::int32_t /*functionId*/, const char* data, std::int32_t dataLength) {
    std::printf("XPlayerManager: REQUESTSUCCESS data = %s, data_len = %d\n", data, dataLength);
    switch (state_) {
        case 0:
            login_ = new GLXPlayerLogin();
            login_->RegisterObserver(this);
            leaderboard_ = new GLXPlayerLeaderboard();
            leaderboard_->RegisterObserver(this);
            leaderboard_->SetScoreDataCount(1);
            state_ = 4;
            return;
        case 2:
            Login(userName_, password_);
            return;
        case 4:
            return;
        case 6:
            SwitchState(0);
            return;
        case 0xb:
            score_ = 0;
            SwitchState(0x10);
            return;
        case 0xd:
        case 0xf:
            SwitchState(0x10);
            return;
        default:
            break;
    }
    std::puts("XPlayerManager: Request Success action not implemented!");
}

// 0x52e524 _ZN15CXPlayerManager4InitEv (partial)
// The original returns when the flag at +0x04 is set. Otherwise it loads the debug switches
// (another category) and allocates a 20-byte string whose use is not recovered. Only the
// flag test is reproduced.
void CXPlayerManager::Init() {
    if (initialized_) {
        return;
    }
}

// 0x52e6cc _ZN15CXPlayerManager13UploadMyScoreEi
// A score below the stored one, or below 1, is refused and the state becomes 0x10. An
// accepted score is stored. Without a login object the manager initializes and returns
// false. With a logged-in service the pending score is added and the state becomes 10.
bool CXPlayerManager::UploadMyScore(std::int32_t score) {
    if (score < score_ || score < 1) {
        std::printf("XPlayerManager: NOT uploading my score = %d because its NOT higher than %d\n", score, score_);
        SwitchState(0x10);
        return false;
    }
    std::printf("XPlayerManager: Uploading my score = %d because its higher or equal than %d\n", score, score_);
    score_ = score;
    if (login_ == nullptr) {
        Init();
        return false;
    }
    if (!login_->KeepAliveEnabled()) {
        return false;
    }
    if (pendingScore_ > 0) {
        score_ = score + pendingScore_;
        pendingScore_ = 0;
    }
    SwitchState(10);
    return true;
}

// Observer slot 0 is OnNetworkError in the original vtable.
void CXPlayerManager::OnDownloadError(std::int32_t /*code*/) { OnNetworkError(); }

// Observer slot 3 forwards to OnRequestFailure (argument order inferred).
void CXPlayerManager::OnResponseError(std::int32_t code, std::int32_t detail) { OnRequestFailure(code, detail); }

// Observer slot 2 forwards to OnRequestSuccess.
void CXPlayerManager::OnResponseText(std::int32_t functionId, const char* text, std::int32_t length) {
    OnRequestSuccess(functionId, text, length);
}
