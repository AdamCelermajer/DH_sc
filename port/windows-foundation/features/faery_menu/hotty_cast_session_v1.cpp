#include "hotty_cast_v1.hpp"
#include "../../combat_session.hpp"

namespace dh::foundation::faery_menu {
namespace {
bool same_owner(const std::weak_ptr<const void>& a,
                const std::weak_ptr<const void>& b) noexcept {
    return !a.owner_before(b) && !b.owner_before(a);
}
bool fail(std::string& error, const char* message) {
    error = message;
    return false;
}
}

bool advance_hotty_cooldown_clock_v1(const CombatSession& session,
                                     double update_dt_seconds,
                                     HottyCooldownClockV1& clock,
                                     std::string& error) {
    const auto lease = session.actor_binding_lease();
    if (!session.world() || lease.expired())
        return fail(error, "Hotty cooldown clock requires a live updated CombatSession");
    if (clock.has_binding_lease) {
        if (clock.binding_lease.expired() || !same_owner(clock.binding_lease, lease))
            return fail(error, "Hotty cooldown state belongs to another or expired CombatSession binding");
    } else if (clock.session_update_serial != 0) {
        return fail(error, "Hotty cooldown clock was advanced without binding to its source CombatSession");
    }
    if (!advance_hotty_cooldown_clock_v1(session.update_serial(), update_dt_seconds,
                                         clock, error)) return false;
    clock.binding_lease = lease;
    clock.has_binding_lease = true;
    error.clear();
    return true;
}

bool prepare_hotty_spell_v1(
    CombatSession& session, CharacterState& state, std::int32_t difficulty,
    const dh2::data::FaeryTables::Borrow& tables,
    const dh2::data::ClassTables& classes, const dh2::data::PropertyRules& rules,
    const HottySourcePolicyV1& policy, HottyCooldownClockV1& clock,
    HottyPreparedCastV1& output, std::string& error,
    const HottySourceTargetListV1* exact_source_targets) {
    const auto lease = session.actor_binding_lease();
    if (lease.expired() || !clock.has_binding_lease || clock.binding_lease.expired() ||
        !same_owner(lease, clock.binding_lease))
        return fail(error, "Hotty source preparation requires cooldowns owned by this live CombatSession");
    auto* world = session.world();
    const auto caster = session.player_id();
    if (!world || caster == invalid_actor_id)
        return fail(error, "Hotty source adapter requires the same live CombatSession player and world");
    if (!exact_source_targets)
        return fail(error, "Hotty CombatSession preparation requires the complete source target list before UseMana/cooldown");
    return prepare_hotty_spell_v1(*world, caster, session.update_serial(), state,
                                 difficulty, tables, classes, rules, policy,
                                 clock, output, error, exact_source_targets);
}
} // namespace dh::foundation::faery_menu
