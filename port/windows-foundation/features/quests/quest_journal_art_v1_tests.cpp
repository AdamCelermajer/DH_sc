// B073: Quest Journal source art invariants (retained dqcharmenu SWF export, no renderer).
#include "runtime_quest_menu_art_v1.hpp"

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace dh::foundation;

namespace {
void check(bool ok, const char* message) { if (!ok) throw std::runtime_error(message); }
const HudGeometryBatch* find_role(const std::vector<HudGeometryBatch>& v, const std::string& part) {
    for (const auto& b : v) if (b.role.find(part) != std::string::npos) return &b;
    return nullptr;
}
}

int main() {
    try {
        const auto& art = original_runtime_quest_menu_art_v1();
        // The list mask (556) is solid-filled: a bitmap-only mask used to clip every bitmap under `content`.
        check(find_role(art.page_art, "/Assigned/Header/d1"), "header underline survives the list mask");
        check(find_role(art.page_art, "/Assigned/Header/d4"), "header icon bitmap survives the list mask");
        check(find_role(art.page_art, "/Completed/CompletedTitle/d1"), "completed underline survives the list mask");
        check(find_role(art.page_art, "/Completed/CompletedTitle/d4"), "completed check icon survives the list mask");
        // The orange header icon fill is drawn after (above) its own bitmap, not behind the page background.
        bool orange = false;
        for (const auto& s : art.page_solids)
            if (s.geometry.role.find("/Assigned/Header/d4") != std::string::npos) {
                orange = true;
                check(!s.after_bitmap_role.empty(), "orange header icon fill has a z-order anchor");
                check(find_role(art.page_art, s.after_bitmap_role) != nullptr, "z-order anchor is a drawn bitmap role");
            }
        check(orange, "orange header icon fill exported");
        // QuestPrio frames: 0 = main quest crown (shape 587), 1 = side quest bullet (shape 588); not in the page frame.
        check(!find_role(art.page_art, "/QuestPrio/"), "QuestPrio is chosen at runtime, not baked into the page");
        check(art.prio_art[0].size() == 1 && art.prio_art[0][0].shape_id == 587, "main quest crown frame");
        check(art.prio_art[1].size() == 1 && art.prio_art[1][0].shape_id == 588, "side quest bullet frame");
        check(art.prio_art[0][0].role == art.prio_art[1][0].role, "same placement path for both QuestPrio frames");
        std::cout << "quest_journal_art_v1_tests passed\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "FAIL: " << e.what() << '\n';
        return 1;
    }
}
