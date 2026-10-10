#include "session_faery_page_v1.hpp"

#include "../../playable_actor_world.hpp"

namespace dh::foundation::faery_menu {
namespace {
bool fail(std::string& error, const char* message) {
    error = message;
    return false;
}

bool validate_live_session(const SessionFaeryActivationV1& activation,
                           const std::weak_ptr<const CombatSessionLifetime>& session_lifetime,
                           const std::weak_ptr<const void>& actor_lease,
                           std::string& error) {
    if (!activation.owner || !activation.character || !activation.session ||
        !activation.tables || !activation.animation_bank ||
        activation.difficulty < 0 || activation.difficulty >= 3 ||
        activation.profile_faery_list_id < 0 ||
        !activation.validate_same_session || !activation.change_current ||
        !activation.update_all_skills || !activation.place_selected_visual)
        return fail(error, "Session Faery activation requires same-Session source, UpdateAllSkills, and visual placement services");
    // CombatSession is a borrowed raw pointer. Its lifetime witness must be
    // acquired at bind time and locked before any later dereference.
    const auto lifetime = session_lifetime.lock();
    if (!lifetime || !lifetime->alive())
        return fail(error, "Session Faery activation lost its live Session lifetime");
    const auto retained_actor = actor_lease.lock();
    const auto current_actor = activation.session->actor_binding_lease().lock();
    if (!retained_actor || current_actor.get() != retained_actor.get())
        return fail(error, "Session Faery activation lost its live player binding lease");
    auto* world = activation.session->world();
    const auto player = activation.session->player_id();
    const auto* actor = world && player != invalid_actor_id ? world->find_actor(player) : nullptr;
    const auto* traits = world && player != invalid_actor_id ? world->traits(player) : nullptr;
    if (!actor || !traits || !traits->is_player ||
        !actor->persistent_character_id ||
        *actor->persistent_character_id != activation.character->id)
        return fail(error, "Session Faery activation does not match the controlled player's CharacterState");
    if (activation.animation_bank->character_state_id != activation.character->id ||
        activation.animation_bank->class_id != activation.character->class_id ||
        !activation.animation_bank->source_animation_table_id)
        return fail(error, "Session Faery activation requires the same profile's preloaded Cast bank");
    const auto list_id = activation.character->source_faery_list_id;
    if (!activation.character->source_faery_state_known ||
        list_id != activation.profile_faery_list_id || list_id < 0 ||
        static_cast<std::size_t>(list_id) >= activation.tables.lists().size() ||
        activation.tables.lists()[static_cast<std::size_t>(list_id)].size() != 5)
        return fail(error, "Session Faery activation requires the current source five-slot FaeryList");
    const auto current = activation.character->faery_by_difficulty[
        static_cast<std::size_t>(activation.difficulty)].current_faery;
    if (current < 0 || current >= 5)
        return fail(error, "Session Faery activation has an invalid current source slot");
    if (!activation.validate_same_session(*activation.session,
                                         *activation.character, error)) {
        if (error.empty()) error = "Same-Session Faery validation rejected the current profile";
        return false;
    }
    error.clear();
    return true;
}

bool preflight_cast_bank(const SessionFaeryActivationV1& activation,
                         std::uint32_t slot, std::string& error) {
    if (slot >= 5) return fail(error, "Faery source menu slot must be in 0..4");
    const auto& list = activation.tables.lists()[static_cast<std::size_t>(
        activation.character->source_faery_list_id)];
    const auto record_id = list[slot];
    if (record_id < 0 || static_cast<std::size_t>(record_id) >=
            activation.tables.faeries().size())
        return fail(error, "Current source FaeryList slot has no actual FaeryTable row");
    CharacterState staged = *activation.character;
    staged.faery_by_difficulty[static_cast<std::size_t>(activation.difficulty)]
        .current_faery = static_cast<std::int32_t>(slot);
    generic_skills::RuntimeSkillFaeryAnimationSlotV1 resolved;
    return generic_skills::resolve_runtime_faery_animation_slot_v1(
        staged, activation.difficulty, *activation.animation_bank, resolved, error);
}
}

bool bind_session_faery_page_provider_v1(
    CharacterStateFaeryBindingsV1 page,
    SessionFaeryActivationV1 activation,
    dh::foundation::character_menu::SourcePageProviderV1& output,
    std::string& error) {
    if (!page.owner || !page.character || !page.tables || !page.localize ||
        page.owner.get() != activation.owner.get() ||
        page.character != activation.character ||
        &page.tables.lists() != &activation.tables.lists() ||
        &page.tables.faeries() != &activation.tables.faeries() ||
        page.difficulty != activation.difficulty ||
        activation.profile_faery_list_id != page.character->source_faery_list_id)
        return fail(error, "Session Faery page and activation must borrow the same owner, profile, tables, and difficulty");
    if (!activation.validate_same_session || !activation.change_current ||
        !activation.update_all_skills || !activation.place_selected_visual)
        return fail(error, "Session Faery page rejects missing ChangeFaery, UpdateAllSkills, or visual placement continuation");
    const auto session_lifetime = activation.session
        ? activation.session->lifetime_lease()
        : std::weak_ptr<const CombatSessionLifetime>{};
    const auto actor_lease = activation.session
        ? activation.session->actor_binding_lease() : std::weak_ptr<const void>{};
    if (!validate_live_session(activation, session_lifetime, actor_lease, error) ||
        !preflight_cast_bank(activation, static_cast<std::uint32_t>(
            activation.character->faery_by_difficulty[static_cast<std::size_t>(
                activation.difficulty)].current_faery), error)) return false;

    page.activate_slot = [activation = std::move(activation), session_lifetime, actor_lease](
        std::uint32_t slot, std::string& message) mutable {
        if (!validate_live_session(activation, session_lifetime, actor_lease, message) ||
            !preflight_cast_bank(activation, slot, message)) return false;
        if (!activation.change_current(*activation.session,
                                       *activation.character, slot, message)) {
            if (message.empty()) message = "Same-Session source ChangeFaery rejected the selected slot";
            return false;
        }
        const auto selected = activation.character->faery_by_difficulty[
            static_cast<std::size_t>(activation.difficulty)].current_faery;
        if (selected != static_cast<std::int32_t>(slot))
            return fail(message, "Same-Session ChangeFaery did not publish the selected source Save slot");
        if (!activation.update_all_skills(*activation.session,
                                          *activation.character, message)) {
            if (message.empty()) message = "Same-Session source UpdateAllSkills continuation failed";
            return false;
        }
        if (!activation.place_selected_visual(*activation.session,
                                               *activation.character, message)) {
            if (message.empty()) message = "Same-Session Faery visual placement continuation failed";
            return false;
        }
        message.clear();
        return true;
    };
    return bind_character_state_faery_provider_v1(std::move(page), output, error);
}

} // namespace dh::foundation::faery_menu
