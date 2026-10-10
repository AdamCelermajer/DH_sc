#include "auto_target_marker_v1.hpp"

#include <cstdlib>
#include <iostream>

using namespace dh::foundation;

namespace {
void check(bool value, const char* message) {
    if (!value) {
        std::cerr << "FAIL: " << message << '\n';
        std::exit(1);
    }
}
}

int main() {
    constexpr std::uintptr_t player = 10;
    constexpr std::uintptr_t last = 20;
    constexpr std::uintptr_t interest = 30;

    auto result = resolve_auto_target_marker_v1(player, last, interest);
    check(result.identity == last && result.source == AutoTargetMarkerSourceV1::last_target,
          "live last-target must take precedence over OOI");

    result = resolve_auto_target_marker_v1(player, 0, interest);
    check(result.identity == interest && result.source == AutoTargetMarkerSourceV1::object_of_interest,
          "null last-target must fall back to OOI");

    result = resolve_auto_target_marker_v1(player, player, interest);
    check(result.identity == interest && result.source == AutoTargetMarkerSourceV1::object_of_interest,
          "self last-target must fall back to OOI");

    result = resolve_auto_target_marker_v1(player, 0, player);
    check(result.identity == 0 && result.source == AutoTargetMarkerSourceV1::none,
          "null/self candidates must hide the marker");

    result = resolve_auto_target_marker_v1(player, last, 0);
    check(result.identity == last && result.source == AutoTargetMarkerSourceV1::last_target,
          "last-target must remain usable without OOI");

    constexpr std::uintptr_t unresolved = 0xfedcba9876543210ULL;
    result = resolve_auto_target_marker_v1(player, unresolved, interest);
    check(result.identity == unresolved && result.source == AutoTargetMarkerSourceV1::last_target,
          "candidate policy must not invent an alive/registry filter");

    std::cout << "PASS original target-marker candidate precedence, null/self fallback, and unfiltered source identity\n";
    return 0;
}
