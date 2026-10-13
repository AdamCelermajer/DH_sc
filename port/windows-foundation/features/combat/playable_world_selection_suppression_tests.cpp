// B155: Character::SetIsTargetable(false) (0x3b7a64, Character+1045) is read by IsInteractive, i.e. by target selection; it
// never gates a damage roll. PlayableActorWorld::eligible_target (selection) honours the flag, damage_eligible does not.
#include "../../playable_actor_world.hpp"
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
namespace {
void check(bool ok, const char* what) { if (!ok) throw std::runtime_error(what); }
}
int main() {
    try {
        std::string error;
        dh2::data::CombatRandom random{};
        dh2::data::AiTables tables;
        tables.rows.resize(51);
        tables.factions.resize(11);
        tables.factions[1] = {{2, -1}}; // faction 1 is hostile to faction 2
        PlayableActorWorld world(tables, random);
        ActorState attacker; attacker.id = 1; attacker.definition_id = "a"; attacker.class_id = "c"; attacker.health = attacker.max_health = 10; attacker.faction_id = 1;
        ActorState victim = attacker; victim.id = 2; victim.faction_id = 2;
        OriginalCombatProperties props; props.sheets.resolved[0] = 1; props.sheets.resolved[1] = 50;
        OriginalCombatProperties props2 = props; props2.sheets.resolved[0] = 2;
        PlayableActorTraits traits; traits.targetable = true;
        check(world.bind_actor(attacker, props, traits, error), error.c_str());
        check(world.bind_actor(victim, props2, traits, error), error.c_str());
        const auto* a = world.find_actor(1);
        const auto* v = world.find_actor(2);
        check(world.eligible_target(*a, *v) && world.damage_eligible(*a, *v), "an ordinary hostile target is eligible for selection and damage");
        world.set_selection_suppressed(2, true);
        check(world.selection_suppressed(2), "flag stored");
        check(!world.eligible_target(*a, *v), "SetIsTargetable(false): not selectable");
        check(world.damage_eligible(*a, *v), "SetIsTargetable(false): still damageable");
        world.set_selection_suppressed(2, false);
        check(world.eligible_target(*a, *v), "SetIsTargetable(true): selectable again");
        // A truly untargetable binding (lifecycle/hidden permission) stays undamageable.
        PlayableActorTraits hidden; hidden.targetable = false;
        ActorState ghost = attacker; ghost.id = 3; ghost.faction_id = 2;
        check(world.bind_actor(ghost, props2, hidden, error), error.c_str());
        check(!world.damage_eligible(*a, *world.find_actor(3)) && !world.eligible_target(*a, *world.find_actor(3)), "a permission-disabled actor is neither selectable nor damageable");
        std::cout << "playable_world_selection_suppression: passed\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "FAIL: " << e.what() << '\n';
        return 1;
    }
}
