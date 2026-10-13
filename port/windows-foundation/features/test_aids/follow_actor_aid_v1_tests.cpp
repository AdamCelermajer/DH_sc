#include "follow_actor_aid_v1.hpp"
#include <iostream>
using namespace dh::foundation::test_aids;
static int checks = 0;
static void check(bool ok, const char* what) { ++checks; if (!ok) { std::cerr << "FAILED: " << what << '\n'; std::exit(1); } }
int main() {
    const auto aid = parse_follow_actor_aid_v1("SwampKing@2900-8000:350");
    check(aid.profile == "SwampKing" && aid.from_frame == 2900 && aid.to_frame == 8000 && aid.distance == 350.0f, "parse with distance");
    check(parse_follow_actor_aid_v1("SwampKing@10-20").distance == 300.0f, "default distance");
    for (const char* bad : {"", "SwampKing", "@1-2", "A@1", "A@1-", "A@5-5", "A@9-3", "A@1-2:0", "A@1-2:-4"}) {
        bool threw = false;
        try { parse_follow_actor_aid_v1(bad); } catch (const std::exception&) { threw = true; }
        check(threw, bad);
    }
    const float hero[3]{0, 0, -114}, far_boss[3]{1000, 0, 0}, near_boss[3]{300, 0, 0};
    const auto placed = follow_actor_placement_v1(aid, 2900, true, hero, far_boss);
    check(placed && std::abs(placed->x - 650.0f) < 0.01f && placed->y == 0 && placed->z == -114.0f, "placed 350 from the boss on the hero side, hero height kept");
    check(!follow_actor_placement_v1(aid, 2899, true, hero, far_boss), "before the window");
    check(!follow_actor_placement_v1(aid, 8000, true, hero, far_boss), "window end is exclusive");
    check(!follow_actor_placement_v1(aid, 2910, true, hero, far_boss), "only on the period");
    check(follow_actor_placement_v1(aid, 2930, true, hero, far_boss).has_value(), "next period");
    check(!follow_actor_placement_v1(aid, 2900, false, hero, far_boss), "dead target is not followed");
    check(!follow_actor_placement_v1(aid, 2900, true, hero, near_boss), "already near");
    const float on_top[3]{5000, 0, 0};
    const float same[3]{5000, 0, 0};
    check(follow_actor_placement_v1(aid, 2900, true, on_top, same) == std::nullopt, "target on the hero: no placement");
    std::cout << "follow_actor_aid_v1 tests passed (" << checks << " checks)\n";
    return 0;
}
