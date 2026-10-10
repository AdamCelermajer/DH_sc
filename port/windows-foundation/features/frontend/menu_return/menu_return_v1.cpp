#include "menu_return_v1.hpp"

namespace dh::foundation::frontend::menu_return {
namespace {
bool fail(std::string& error, const char* message) {
    error = message;
    return false;
}

bool owns_same_lease(const std::weak_ptr<const void>& expected,
                     const std::shared_ptr<const void>& current) {
    return !expected.owner_before(current) && !current.owner_before(expected);
}

bool checkpoint(CombatSession& session,
                generic_skills::RuntimeSkillCastCoordinatorV1& casts,
                std::string& error) {
    if (!session.world() || session.actor_binding_lease().expired())
        return fail(error, "Main-menu return requires the current initialized CombatSession");
    if (!casts.checkpoint_v1(session, error)) return false;
    if (!session.validate_lifecycle_checkpoint(error)) return false;
    error.clear();
    return true;
}
}

bool request_v1(const RequestV1& request, TicketV1& output, std::string& error) {
    output = {};
    if (!request.authored_yes_confirmed)
        return fail(error, "Main-menu return requires the authored confirmation Yes action");
    if (!request.session || !request.skill_casts)
        return fail(error, "Main-menu return requires the current Session and source skill checkpoint owners");
    if (!checkpoint(*request.session, *request.skill_casts, error)) return false;
    const auto lease = request.session->actor_binding_lease().lock();
    if (!lease) return fail(error, "Main-menu return Session lease expired during admission");
    output.session = request.session;
    output.skill_casts = request.skill_casts;
    output.session_lease = lease;
    output.update_serial = request.session->update_serial();
    output.valid = true;
    error.clear();
    return true;
}

bool commit_v1(TicketV1& ticket,
               generic_skills::RuntimeSkillCastCoordinatorV1& casts,
               const std::string& level_uri,
               const std::filesystem::path& live_save_path,
               const std::filesystem::path& profile_path,
               CharacterState& profile,
               CommitV1& output,
               std::string& error) {
    output = {};
    if (!ticket.valid || !ticket.session || ticket.skill_casts != &casts)
        return fail(error, "Main-menu return commit requires its unconsumed admitted request and same skill owner");
    auto& session = *ticket.session;
    const auto current = session.actor_binding_lease().lock();
    if (!current || current != ticket.session_lease.lock() ||
        !owns_same_lease(ticket.session_lease, current) ||
        ticket.update_serial != session.update_serial()) {
        ticket.valid = false;
        return fail(error, "Main-menu return request is stale after Session replacement or update");
    }
    // Consume once the exact request is reached. A provider failure requires a
    // fresh admission; it must not accidentally replay the same click.
    ticket.valid = false;
    if (!checkpoint(session, casts, error)) return false;
    if (profile_path.empty() || profile_path.filename().empty())
        return fail(error, "Main-menu return requires the exact current profile save path");

    auto staged_profile = profile;
    const auto* player = session.actor(session.player_id());
    if (!player || !player->persistent_character_id ||
        *player->persistent_character_id != staged_profile.id)
        return fail(error, "Main-menu return profile identity differs from the live Session player");
    staged_profile.stats.health = player->health;
    staged_profile.stats.max_health = player->max_health;
    staged_profile.stats.resource = player->resource;
    staged_profile.stats.max_resource = player->max_resource;

    if (player->alive()) {
        if (level_uri.empty() || live_save_path.empty() || live_save_path.filename().empty())
            return fail(error, "Living-player menu return requires the current level URI and live-save path");
        GameSave snapshot;
        if (!capture_game_save(level_uri, session.player_id(), staged_profile,
                               *session.world(), snapshot, error)) return false;
        if (!save_game(live_save_path, snapshot, error)) return false;
        staged_profile = snapshot.character;
        output.level_saved = true;
    }
    if (!save_character(profile_path, staged_profile, error)) return false;
    profile = std::move(staged_profile);
    output.profile_saved = true;
    output.may_return_to_frontend = true;
    error.clear();
    return true;
}
} // namespace dh::foundation::frontend::menu_return
