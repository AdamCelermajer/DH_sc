#include "../player_root_motion_policy_v1.hpp"

#include <cstdint>
#include <cstdio>

namespace policy = dh2::actor::player_root_motion_policy_v1;

int main() {
    policy::Projection projection{};

    if (policy::project(&projection, 0x2380u) != policy::Status::complete ||
        projection.displacement) {
        std::fputs("Idle flags 0x2380 must disable visual root displacement\n", stderr);
        return 1;
    }

    if (policy::project(&projection, 0x23c1u) != policy::Status::complete ||
        !projection.displacement) {
        std::fputs("Move flags 0x23c1 must enable visual root displacement\n", stderr);
        return 2;
    }

    if (policy::project(nullptr, 0x23c1u) != policy::Status::invalid_argument) {
        std::fputs("Null output must be rejected\n", stderr);
        return 3;
    }

    std::puts("player_root_motion_policy_v1: Idle and Move source flags passed");
    return 0;
}
