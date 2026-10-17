#include "../player_faery_change_v1.hpp"

#include <cassert>
#include <iostream>
#include <string>

namespace change = dh2::data::player_faery_change_v1;

struct SkillOwner {
    dh2::data::PlayerSavegameV1* save{};
    std::uint32_t difficulty{};
    std::int32_t expected_id{};
    unsigned calls{};
    bool succeed=true;
};

static bool update_skills(void* context, std::string& error) {
    auto& owner = *static_cast<SkillOwner*>(context);
    ++owner.calls;
    assert(owner.save->current_faery(owner.difficulty) == owner.expected_id);
    if (!owner.succeed) error = "UpdateAllSkills provider failed";
    return owner.succeed;
}

int main() {
    dh2::data::PlayerSavegameV1 save;
    save.initialize_faeries();
    SkillOwner owner{&save, 1, 3};
    const change::Services services{&owner, update_skills};
    change::Result result{};
    std::string error;

    assert(change::change(&save, 1, 3, services, &result, error) == change::Status::complete);
    assert(result.save_changed && result.skills_updated && owner.calls == 1);
    assert(save.current_faery(1) == 3 && save.current_faery(0) == 0);

    owner.expected_id = 4;
    owner.succeed = false;
    assert(change::change(&save, 1, 4, services, &result, error) == change::Status::skill_update_failed);
    assert(result.save_changed && !result.skills_updated && owner.calls == 2);
    assert(save.current_faery(1) == 4 && error == "UpdateAllSkills provider failed");

    assert(change::change(&save, 1, 5, services, &result, error) == change::Status::save_rejected);
    assert(!result.save_changed && owner.calls == 2 && save.current_faery(1) == 4);
    std::cout << "{\"selection\":true,\"save_before_update\":true,\"failure_order\":true}\n";
}
