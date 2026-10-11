#include "cx_player_manager_score_v1.hpp"

#include <cstring>

namespace dh2::ui::cx_player_manager_score_v1 {
namespace {
std::int32_t source_add(std::int32_t left, std::int32_t right) noexcept {
    const std::uint32_t bits = static_cast<std::uint32_t>(left) +
                               static_cast<std::uint32_t>(right);
    std::int32_t value{};
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}
OwnerV1& application_owner() noexcept {
    static OwnerV1 owner;
    return owner;
}
}

ResultV1 OwnerV1::send_gl_hi_score(std::int32_t score, bool wifi_alive,
                                   LoginClientV1 login) noexcept {
    ResultV1 result{};
    state_.internet_access_enabled = wifi_alive;
    if (!wifi_alive) {
        result.branch = BranchV1::wifi_disabled;
        return result;
    }

    // CXPlayerManager::UploadMyScore accepts only positive scores at least as
    // large as its retained +0x18 score. This is a valid no-upload result,
    // including score zero; it is not a failed Application load.
    if (score <= 0 || state_.retained_score > score) {
        state_.state = 16;
        result.branch = BranchV1::score_ineligible;
        return result;
    }

    // Source stores +0x18 before checking its login component. With no
    // retained CX client, the source next calls Init and returns zero. The
    // port deliberately reports that missing external service instead of
    // inventing server-config/login completion or clearing Property 24.
    state_.retained_score = score;
    if (!login.present) {
        result.branch = BranchV1::initialization_required;
        return result;
    }
    if (!login.upload_enabled) {
        result.branch = BranchV1::login_not_ready;
        return result;
    }

    if (state_.pending_score > 0) {
        state_.retained_score = source_add(score, state_.pending_score);
        state_.pending_score = 0;
    }
    state_.state = 10;
    result.branch = BranchV1::request_started;
    result.manager_return = 1;
    // Original Application::SendGLHiScore clears property 24 when this
    // return value is nonzero. State 10 only starts the asynchronous request;
    // it does not establish successful remote upload.
    result.clear_property24 = true;
    return result;
}

ResultV1 send_gl_hi_score(std::int32_t score, bool wifi_alive,
                          LoginClientV1 login) noexcept {
    return application_owner().send_gl_hi_score(score, wifi_alive, login);
}

SnapshotV1 snapshot() noexcept {
    return application_owner().snapshot();
}

} // namespace dh2::ui::cx_player_manager_score_v1
