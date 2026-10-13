// v21.8 ES-API: unit checks for the actor-side TargetList arms (Player filter, named backups, relative search origin)
// and the live HP write, on a synthetic PlayableActorWorld (no asset data required).
#include "runtime_skill_target_query_v1.hpp"

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::generic_skills;

namespace {
void check(bool ok, const std::string& what) {
    if (!ok) throw std::runtime_error(what);
}

constexpr float kPi = 3.1415927410125732421875f;

struct Fixture {
    dh2::data::CombatRandom random{};
    dh2::data::AiTables tables;
    std::unique_ptr<PlayableActorWorld> world;

    Fixture() {
        tables.rows.resize(51);
        tables.factions.resize(11);
        world = std::make_unique<PlayableActorWorld>(tables, random);
    }

    // Caster faces heading 0: look = (0, -1, 0) as in the source query.
    void add(ActorId id, std::array<float, 3> position, bool is_player, bool alive = true) {
        ActorState actor;
        actor.id = id;
        actor.definition_id = "a";
        actor.class_id = "c";
        actor.health = alive ? 10.0f : 0.0f;
        actor.max_health = 10.0f;
        actor.faction_id = is_player ? 1 : 2;
        actor.transform.position = position;
        OriginalCombatProperties props;
        props.sheets.resolved[0] = is_player ? 1 : 2;
        props.sheets.resolved[1] = 50;
        PlayableActorTraits traits;
        traits.targetable = true;
        traits.is_player = is_player;
        std::string error;
        check(world->bind_actor(actor, props, traits, error), "bind_actor failed: " + error);
    }
};

void check_filter_and_backups() {
    check(kSkillCharacterFilterPlayerV1 == 128, "AITargetList_CharacterFilter.Player is 128 (atlas, ai_pycst)");

    SkillTargetListBackupsV1 backups;
    check(backups.get("MaxAngleSearch").empty(), "an absent backup name reads as an empty list");
    backups.backup("MaxAngleSearch", {3, 4});
    check((backups.get("MaxAngleSearch") == std::vector<ActorId>{3, 4}), "backup stores the ordered list");
    backups.backup("MaxAngleSearch", {9});
    check((backups.get("MaxAngleSearch") == std::vector<ActorId>{9}), "a second backup under the same name overwrites");
    backups.backup(SkillTargetListBackupsV1::kDefaultName, {1});
    check((backups.get("default") == std::vector<ActorId>{1}), "the default backup name is \"default\"");
    check(backups.get("MaxAngleSearch") == std::vector<ActorId>{9}, "backups are independent by name");
}

void check_relative_origin() {
    const auto origin = relative_forward_search_origin_v1({1.0f, 2.0f, 3.0f}, {0.0f, -1.0f, 0.0f}, -40.0f);
    check(std::fabs(origin[0] - 1.0f) < 1e-4f && std::fabs(origin[1] - 42.0f) < 1e-4f && std::fabs(origin[2] - 3.0f) < 1e-4f,
          "DeathClaw origin (0,-40,0) moves the centre 40 units behind a caster facing -y");
    const auto same = relative_forward_search_origin_v1({5.0f, 6.0f, 7.0f}, {1.0f, 0.0f, 0.0f}, 0.0f);
    check(same[0] == 5.0f && same[1] == 6.0f && same[2] == 7.0f, "zero forward offset keeps the caster position");
}

void check_player_query() {
    Fixture f;
    f.add(1, {0, 0, 0}, true);               // caster (a player-side character)
    f.add(2, {0, -20, 0}, true);             // allied, straight ahead
    f.add(3, {0, -30, 0}, false);            // enemy in range: excluded by the Player filter
    f.add(4, {1000, 0, 0}, true);            // allied but out of range
    f.add(5, {10, 0, 0}, true, false);       // allied but dead
    f.add(6, {0, 20, 0}, true);              // allied directly behind (angle pi)

    SkillActorTargetQueryV1 out;
    std::string error;
    check(query_player_character_targets_v1(*f.world, 1, {1, 2, 3, 4, 5, 6}, 100.0f, kPi,
                                            SkillTargetSortV1::closest_first, std::nullopt, out, error),
          "player query failed: " + error);
    check(out.status == SkillActorTargetQueryStatusV1::selected_character, "player query selects an ally");
    check((out.ordered_character_targets == std::vector<ActorId>{2, 6}),
          "Player filter: self, enemy, dead and far actors are excluded; the ahead ally comes first (closest)");
    check(out.selected == 2, "closest allied target is selected");

    check(query_player_character_targets_v1(*f.world, 1, {1, 2, 3, 4, 5, 6}, 100.0f, kPi / 2.0f,
                                            SkillTargetSortV1::closest_first, std::nullopt, out, error),
          "player query (narrow cone) failed: " + error);
    check((out.ordered_character_targets == std::vector<ActorId>{2}),
          "a pi/2 cone drops the ally behind the caster");

    check(query_player_character_targets_v1(*f.world, 1, {1, 2, 3}, 5.0f, kPi,
                                            SkillTargetSortV1::closest_first, std::nullopt, out, error),
          "player query (short range) failed: " + error);
    check(out.status == SkillActorTargetQueryStatusV1::no_actor_target && out.ordered_character_targets.empty(),
          "no ally inside range is an empty result, not a fabricated target");

    // Relative origin (DeathClaw-style): 40 units behind the caster, so the ahead ally at y=-20 is 60 away.
    const std::array<float, 3> behind{0.0f, 40.0f, 0.0f};
    check(query_player_character_targets_v1(*f.world, 1, {1, 2, 6}, 30.0f, kPi,
                                            SkillTargetSortV1::closest_first, behind, out, error),
          "player query (origin override) failed: " + error);
    check((out.ordered_character_targets == std::vector<ActorId>{6}),
          "an origin override moves the search centre (the ally behind the origin is nearest)");
}

void check_backup_query() {
    Fixture f;
    f.add(1, {0, 0, 0}, true);
    f.add(2, {0, -20, 0}, true);
    f.add(3, {0, -30, 0}, false);           // enemy, in the backup and in range
    f.add(4, {1000, 0, 0}, false);          // in the backup but out of range
    f.add(8, {0, -15, 0}, false);           // in range but NOT in the backup: never a candidate

    SkillActorTargetQueryV1 out;
    std::string error;
    check(query_backup_character_targets_v1(*f.world, 1, {2, 3, 4, 99}, 100.0f, kPi,
                                            SkillTargetSortV1::closest_first, std::nullopt, out, error),
          "backup query failed: " + error);
    check((out.ordered_character_targets == std::vector<ActorId>{2, 3}),
          "named search: only backup members inside range/cone; unknown ids and non-members are ignored");
}

void check_live_hp() {
    Fixture f;
    f.add(1, {0, 0, 0}, true);
    OriginalCombatProperties props = *f.world->combat_properties(1);
    props.sheets.resolved[36] = 100 * 256;
    props.sheets.resolved[38] = 100 * 256;
    props.sheets.saved[36] = 100 * 256;
    std::string error;
    check(f.world->update_combat_properties(1, props, PlayableActorTraits(*f.world->traits(1)), error), "setup sheet: " + error);
    f.world->find_actor(1)->health = 100.0f;
    f.world->find_actor(1)->max_health = 100.0f;

    check(f.world->set_current_health(1, 50.0f, error), "SetProp(HP,50): " + error);
    float health = 0, maximum = 0;
    check(f.world->current_health(1, health, maximum), "GetPropHP after write");
    check(health == 50.0f && maximum == 100.0f, "GetPropHP returns the written current HP and the maximum");
    check(f.world->find_actor(1)->health == 50.0f, "ActorState vitals mirror the written HP");
    check(f.world->combat_properties(1)->sheets.resolved[36] == 50 * 256, "source cell 36 holds HP x256");
    check(f.world->combat_properties(1)->sheets.saved[36] == 50 * 256,
          "saved current HP moves with the resolved cell (revive bookkeeping)");
    check(f.world->find_actor(1)->alive(), "a plain SetProp does not kill the actor");

    check(!f.world->set_current_health(1, std::numeric_limits<float>::quiet_NaN(), error),
          "a non-finite HP value is refused");
    check(!f.world->set_current_health(99, 10.0f, error), "an unbound actor is refused");
}
// Authored ANGLE vs native half-angle (IDA 0x38fcf0 -> 0x4a2f34): an ANGLE of A admits +-A/2 around the look vector.
void check_native_half_cone() {
    Fixture f;
    f.add(1, {0, 0, 0}, true);                                          // caster facing -y
    const float degrees[] = {40.0f, 50.0f, 70.0f, 179.0f};
    ActorId id = 11;
    for (const float deg : degrees) {
        const float rad = deg * 3.1415927f / 180.0f;
        f.add(id++, {20.0f * std::sin(rad), -20.0f * std::cos(rad), 0.0f}, true);
    }
    std::vector<ActorId> allies{11, 12, 13, 14};
    SkillActorTargetQueryV1 out;
    std::string error;
    // ANGLE 90 -> native 45 degrees: only the 40-degree ally is inside.
    check(query_player_character_targets_v1(*f.world, 1, allies, 100.0f, native_search_max_angle_v1(90.0f),
                                            SkillTargetSortV1::closest_first, std::nullopt, out, error), error);
    check((out.ordered_character_targets == std::vector<ActorId>{11}),
          "ANGLE 90 (native 45 degrees) admits the 40-degree ally only, not the 50-degree one");
    // ANGLE 120 (Charge) -> native 60 degrees: 40 and 50 are inside, 70 is not.
    check(query_player_character_targets_v1(*f.world, 1, allies, 100.0f, native_search_max_angle_v1(120.0f),
                                            SkillTargetSortV1::closest_first, std::nullopt, out, error), error);
    check((out.ordered_character_targets == std::vector<ActorId>{11, 12}),
          "ANGLE 120 (native 60 degrees) admits 40 and 50 degrees, not 70");
    // ANGLE 360 and the no-angle default are the full circle (native pi).
    check(query_player_character_targets_v1(*f.world, 1, allies, 100.0f, native_search_max_angle_v1(360.0f),
                                            SkillTargetSortV1::closest_first, std::nullopt, out, error), error);
    check(out.ordered_character_targets.size() == 4, "ANGLE 360 keeps every in-range ally");
    check(native_search_max_angle_v1(360.0f) >= 3.1415927f - 1e-4f && native_search_max_angle_v1(360.0f) <= 3.1415927f + 1e-4f,
          "ANGLE 360 native half-angle is pi");
}

} // namespace

int main() {
    try {
        check_filter_and_backups();
        check_relative_origin();
        check_player_query();
        check_backup_query();
        check_live_hp();
        check_native_half_cone();
        std::cout << "runtime_skill_actor_api_v1: passed\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "FAIL: " << e.what() << '\n';
        return 1;
    }
}
