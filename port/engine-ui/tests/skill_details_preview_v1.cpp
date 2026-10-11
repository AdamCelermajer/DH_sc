#include "../skill_details_preview_v1.hpp"

#include <iostream>
#include <stdexcept>

static void check(bool ok, const char* message) {
    if (!ok) throw std::runtime_error(message);
}

int main() {
    using namespace dh2::ui;
    const auto locked = skill_detail_preview_v1(4, 5, 0, 4, true);
    check(!locked.unlocked &&
              locked.current == SkillDetailCurrentTextV1::unlock_level &&
              locked.next == SkillDetailNextTextV1::empty,
          "locked skill shows its unlock level and no next rank");
    const auto untrained = skill_detail_preview_v1(5, 5, 0, 4, true);
    check(untrained.unlocked &&
              untrained.current == SkillDetailCurrentTextV1::needs_skill_points &&
              untrained.next == SkillDetailNextTextV1::next_rank,
          "untrained skill shows points warning and next rank preview");
    const auto capped = skill_detail_preview_v1(5, 5, 4, 4, true);
    check(capped.current == SkillDetailCurrentTextV1::current_rank &&
              capped.next == SkillDetailNextTextV1::maximum_level,
          "capped skill shows the maximum level message");
    const auto blocked = skill_detail_preview_v1(5, 5, 1, 4, false);
    check(blocked.current == SkillDetailCurrentTextV1::current_rank &&
              blocked.next == SkillDetailNextTextV1::training_blocked,
          "prerequisite-blocked skill shows its training requirements message");
    const auto trainable = skill_detail_preview_v1(5, 5, 1, 4, true);
    check(trainable.current == SkillDetailCurrentTextV1::current_rank &&
              trainable.next == SkillDetailNextTextV1::next_rank,
          "trainable skill shows current and next rank previews");
    std::cout << "{\"validation\":\"PASS\",\"checks\":5}\n";
}
