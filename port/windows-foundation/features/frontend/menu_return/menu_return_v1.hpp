#pragma once

#include "../../../game_save.hpp"
#include "../../../save_store.hpp"
#include "../../generic_skills/runtime_skill_cast_coordinator_v1.hpp"

namespace dh::foundation::frontend::menu_return {

struct RequestV1 {
    CombatSession* session = nullptr;
    generic_skills::RuntimeSkillCastCoordinatorV1* skill_casts = nullptr;
    bool authored_yes_confirmed = false;
};

// Single-use proof that the confirmed return was admitted at one exact
// Session/update boundary. It is invalidated by the next Session update,
// restore/detach, or a failed commit attempt.
struct TicketV1 {
    CombatSession* session = nullptr;
    generic_skills::RuntimeSkillCastCoordinatorV1* skill_casts = nullptr;
    std::weak_ptr<const void> session_lease;
    std::uint64_t update_serial = 0;
    bool valid = false;
};

struct CommitV1 {
    bool level_saved = false;
    bool profile_saved = false;
    bool may_return_to_frontend = false;
};

// Admission uses the real source-skill and CombatSession checkpoint owners.
// It performs no persistence or navigation side effects.
bool request_v1(const RequestV1&, TicketV1&, std::string& error);

// Commits through the existing GameSave and CharacterState save providers.
// The CharacterState is published back to the caller only after both saves
// succeed. A dead player follows the current host policy: skip GameSave but
// persist the live Session vitals into the profile.
bool commit_v1(TicketV1&, generic_skills::RuntimeSkillCastCoordinatorV1&,
               const std::string& level_uri,
               const std::filesystem::path& live_save_path,
               const std::filesystem::path& profile_path,
               CharacterState&, CommitV1&, std::string& error);

} // namespace dh::foundation::frontend::menu_return
