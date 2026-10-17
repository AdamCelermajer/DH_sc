#include "../cx_player_manager_score_v1.hpp"

#include <cstdio>

using namespace dh2::ui::cx_player_manager_score_v1;

int main() {
    int failures = 0;
    auto check = [&](bool condition, const char* label) {
        if (!condition) {
            std::fprintf(stderr, "FAIL: %s\n", label);
            ++failures;
        }
    };

    OwnerV1 offline;
    const auto no_wifi = offline.send_gl_hi_score(45, false);
    check(no_wifi.branch == BranchV1::wifi_disabled &&
          !offline.snapshot().internet_access_enabled &&
          offline.snapshot().retained_score == 0 && !no_wifi.clear_property24,
          "Wi-Fi false preserves Property 24 input and only clears source +5");

    OwnerV1 zero_score;
    const auto zero = zero_score.send_gl_hi_score(0, true);
    check(zero.branch == BranchV1::score_ineligible && zero.manager_return == 0 &&
          zero_score.snapshot().state == 16 &&
          zero_score.snapshot().retained_score == 0 && !zero.clear_property24,
          "zero score reaches SwitchState(16), returns zero, and does not clear property");

    OwnerV1 absent_login;
    const auto init = absent_login.send_gl_hi_score(80, true);
    check(init.branch == BranchV1::initialization_required &&
          init.manager_return == 0 && !init.clear_property24 &&
          absent_login.snapshot().retained_score == 80 &&
          !absent_login.snapshot().initialized,
          "eligible score stores +0x18 then stops at missing Init/network provider");

    const auto lower = absent_login.send_gl_hi_score(79, true);
    check(lower.branch == BranchV1::score_ineligible &&
          absent_login.snapshot().state == 16 &&
          absent_login.snapshot().retained_score == 80,
          "score below retained +0x18 is rejected and preserves high score");

    OwnerV1 login;
    const auto not_ready = login.send_gl_hi_score(80, true, {true, false});
    check(not_ready.branch == BranchV1::login_not_ready &&
          not_ready.manager_return == 0 && !not_ready.clear_property24 &&
          login.snapshot().retained_score == 80,
          "present login with +0x40 clear returns zero without clearing property");

    OwnerV1 request;
    const auto started = request.send_gl_hi_score(80, true, {true, true});
    check(started.branch == BranchV1::request_started &&
          started.manager_return == 1 && started.clear_property24 &&
          request.snapshot().state == 10 && request.snapshot().retained_score == 80,
          "ready login enters state 10 and only signals request start, not HTTP completion");

    if (failures) return 1;
    std::puts("PASS: CXPlayerManager score gates, retained score, and async-start semantics");
    return 0;
}
