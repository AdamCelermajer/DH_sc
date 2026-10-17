#pragma once

#include "player_savegame_v1.hpp"

namespace dh2::data::player_faery_change_v1 {

struct Services {
    void* context{};
    bool (*update_all_skills)(void*, std::string&){};
};

struct Result {
    bool save_changed{};
    bool skills_updated{};
};

enum class Status : unsigned char {
    complete,
    invalid_argument,
    save_rejected,
    skill_update_failed,
};

// Character::ChangeFaery's Save then CharAI::UpdateAllSkills prefix. The
// caller retains the one canonical Save and its existing Player skill owner.
Status change(PlayerSavegameV1*, std::uint32_t difficulty,
              std::uint32_t faery_id, const Services&, Result*,
              std::string& error);

} // namespace dh2::data::player_faery_change_v1
