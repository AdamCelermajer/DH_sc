#include "player_heal_aid_v1.hpp"
#include <cstdlib>
#include <iostream>
using namespace dh::foundation::test_aids;
static int checks = 0;
static void check(bool ok, const char* what) { ++checks; if (!ok) { std::cerr << "FAILED: " << what << '\n'; std::exit(1); } }
int main() {
    const auto aid = parse_player_heal_aid_v1("3000-8000:20");
    check(aid.from_frame == 3000 && aid.to_frame == 8000 && aid.period_frames == 20, "parse with period");
    check(parse_player_heal_aid_v1("10-20").period_frames == 15, "default period");
    check(aid.due(3000) && aid.due(3020) && aid.due(7980), "due on the period");
    check(!aid.due(2999) && !aid.due(3010) && !aid.due(8000) && !aid.due(8020), "not due off the period or outside the window (end exclusive)");
    check(!PlayerHealAidV1{}.due(0), "default aid is inactive");
    for (const char* bad : {"", "100", "-5", "5-", "5-5", "9-3", "5-9:0", "5-9:-2", "-1-5"}) {
        bool threw = false;
        try { parse_player_heal_aid_v1(bad); } catch (const std::exception&) { threw = true; }
        check(threw, bad);
    }
    std::cout << "player_heal_aid_v1 tests passed (" << checks << " checks)\n";
    return 0;
}
