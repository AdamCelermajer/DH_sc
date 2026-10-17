#include "player_faery_change_v1.hpp"

namespace dh2::data::player_faery_change_v1 {

Status change(PlayerSavegameV1* save, std::uint32_t difficulty,
              std::uint32_t faery_id, const Services& services,
              Result* result, std::string& error) {
    if (!save || !result || !services.update_all_skills || difficulty >= 3 ||
        !save->faeries_initialized()[difficulty]) {
        error = "Faery change requires the initialized canonical Save and Player skill owner";
        return Status::invalid_argument;
    }

    *result = {};
    if (!save->set_current_faery(faery_id, difficulty, error))
        return Status::save_rejected;
    result->save_changed = true;

    if (!services.update_all_skills(services.context, error))
        return Status::skill_update_failed;
    result->skills_updated = true;
    error.clear();
    return Status::complete;
}

} // namespace dh2::data::player_faery_change_v1
