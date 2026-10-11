#pragma once

#include <cstdint>

namespace dh2::ui::cx_player_manager_score_v1 {

// Only the two facts UploadMyScore reads from its +8 login component. These
// must come from a real retained CX client before the request-start branch can
// be reached; the current port has no such client and passes the default.
struct LoginClientV1 {
    bool present{};
    bool upload_enabled{}; // source login component byte +0x40
};

enum class BranchV1 : std::uint8_t {
    wifi_disabled,
    score_ineligible,
    initialization_required,
    login_not_ready,
    request_started
};

struct SnapshotV1 {
    bool internet_access_enabled{}; // CXPlayerManager +5
    bool initialized{};             // CXPlayerManager +4
    std::int32_t state{};           // CXPlayerManager +0x14
    std::int32_t retained_score{};  // CXPlayerManager +0x18
    std::int32_t pending_score{};   // CXPlayerManager +0x1c
};

struct ResultV1 {
    BranchV1 branch{BranchV1::wifi_disabled};
    std::int32_t manager_return{};
    bool clear_property24{};
};

// One process-lifetime projection of the source Singleton<CXPlayerManager>.
// It models UploadMyScore's local branch effects. It owns no Character,
// PropertyView, client, request queue, or remote-completion state.
class OwnerV1 {
    SnapshotV1 state_{};
public:
    ResultV1 send_gl_hi_score(std::int32_t property24_score, bool wifi_alive,
                              LoginClientV1 login = {}) noexcept;
    const SnapshotV1& snapshot() const noexcept { return state_; }
};

// Application-lifetime source singleton used by the selected runtime.
ResultV1 send_gl_hi_score(std::int32_t property24_score, bool wifi_alive,
                          LoginClientV1 login = {}) noexcept;
SnapshotV1 snapshot() noexcept;

} // namespace dh2::ui::cx_player_manager_score_v1
