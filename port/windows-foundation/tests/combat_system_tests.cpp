#include "combat_system.hpp"
#include "animation_markers.hpp"
#include <cmath>
#include <iostream>
#include <limits>
#include <map>
#include <stdexcept>

using namespace dh::foundation;
namespace {
void check(bool condition, const char* message) { if (!condition) throw std::runtime_error(message); }
// Numbers and content names here are test fixtures, not recovered game balance.
struct TestWorld : CombatWorld {
    std::map<ActorId, ActorState> actors;
    mutable unsigned resolutions = 0;
    bool provider_available = true;
    ActorState* find_actor(ActorId id) override {
        auto it = actors.find(id); return it == actors.end() ? nullptr : &it->second;
    }
    bool eligible_target(const ActorState& a, const ActorState& b) const override {
        return a.faction_id != b.faction_id;
    }
    float target_radius(const ActorState&) const override { return 0.5f; }
    bool resolve_damage(const std::string& source, const ActorState&, const ActorState&,
                        const std::string&, float& amount, std::string& error) const override {
        ++resolutions;
        if (!provider_available) { error = "fixture provider unavailable"; return false; }
        if (source == "fixture.main") amount = 11;
        else if (source == "fixture.off") amount = 7;
        else { error = "fixture source missing"; return false; }
        return true;
    }
    TestWorld() {
        for (ActorId id : {ActorId(1), ActorId(2)}) {
            ActorState a;
            a.id = id; a.definition_id = "fixture.actor";
            a.faction_id = static_cast<int>(id); a.health = a.max_health = 100;
            a.attack_ids = {"fixture.attack"};
            a.transform.position = {static_cast<float>(id - 1), 0, 0};
            actors.emplace(id, a);
        }
        actors[1].persistent_character_id = "fixture.player";
    }
};
AttackDefinition attack() {
    AttackDefinition a;
    a.id = "fixture.attack"; a.animation_clip_id = "fixture.clip";
    a.maximum_range = 2; a.cooldown_seconds = 0.5;
    a.damage_markers = {{"main_hit", "fixture.main"}, {"off_hit", "fixture.off"}};
    return a;
}
MarkerOccurrence marker(const char* name, std::uint64_t generation, unsigned index = 0) {
    MarkerOccurrence m; m.marker.name = name; m.marker.index = index; m.generation = generation; return m;
}
void authored_markers_shared_path() {
    const std::uint8_t times[] = {50,0,0,0, 100,0,0,0, 125,0,0,0};
    const char* main_names[] = {"main_hit", "camera_event"};
    const char* off_names[] = {"off_hit"};
    const dh2::animation::EventGroup groups[] = {{2,main_names},{1,off_names},{1,main_names}};
    const dh2::animation::EventView view{4,3,times,groups};
    AnimationMarkers track;
    std::string error;
    check(track.load(view,0,150,error), "fixture authored track rejected");
    // Both the persistent player and an NPC use exactly the same combat path.
    for (ActorId attacker : {ActorId(1), ActorId(2)}) {
        TestWorld world; CombatSystem combat(world); MarkerCursor cursor;
        check(AnimationMarkers::restart(cursor,error), "cursor restart failed");
        const ActorId target = attacker == 1 ? 2 : 1;
        check(combat.begin(attacker,target,attack(),cursor.generation,error), "shared actor begin failed");
        check(combat.update(0.25,error), "combat update failed");
        check(world.actors[target].health == 100, "update timer caused damage");
        std::vector<MarkerOccurrence> occurrences;
        check(track.advance(cursor,150,false,occurrences,error), "authored advance failed");
        check(occurrences.size() == 4, "authored occurrence count differs");
        for (const auto& occurrence : occurrences) {
            DamageEvent event;
            check(combat.consume_marker(attacker,occurrence,event,error), "authored consume failed");
        }
        check(world.actors[target].health == 71 && world.resolutions == 3,
              "main/offhand or distinct repeated-name marker damage differs");
        for (const auto& occurrence : occurrences) {
            DamageEvent event;
            check(combat.consume_marker(attacker,occurrence,event,error), "duplicate consume failed");
            check(!event.applied, "duplicate event damaged target");
        }
        check(world.resolutions == 3, "duplicates reran damage policy");
        check(combat.finish(attacker,cursor.generation), "finish failed");
        check(world.actors[attacker].action == CharacterAction::idle &&
              world.actors[attacker].target_id == invalid_actor_id, "finish retained attack state");
    }
}
void lifecycle() {
    TestWorld world; CombatSystem combat(world); std::string error; DamageEvent event;
    auto a = attack();
    check(combat.begin(1,2,a,10,error), "initial begin failed");
    check(combat.consume_marker(1,marker("main_hit",9),event,error) && !event.applied, "stale marker applied");
    auto loop = marker("main_hit",10); loop.cycle = 1;
    check(combat.consume_marker(1,loop,event,error) && !event.applied, "loop replay applied");
    check(!combat.finish(1,9) && combat.attacking(1), "stale finish interrupted attack");
    check(combat.interrupt(1), "interrupt failed");
    check(combat.consume_marker(1,marker("main_hit",10),event,error) && !event.applied, "interrupted marker applied");
    check(!combat.begin(1,2,a,11,error), "cooldown bypassed by interruption");
    check(!combat.update(-1,error) && combat.cooldown_remaining(1) == 0.5, "invalid elapsed changed cooldown");
    check(!combat.update(std::numeric_limits<double>::infinity(),error), "infinite elapsed accepted");
    check(combat.update(0.25,error) && !combat.begin(1,2,a,11,error), "partial cooldown ignored");
    check(combat.update(0.25,error) && combat.begin(1,2,a,11,error), "expired cooldown blocked attack");
    check(combat.consume_marker(1,marker("main_hit",10),event,error) && !event.applied, "old generation damaged new attack");
    world.actors[1].action = CharacterAction::hurt;
    check(combat.update(0,error) && !combat.attacking(1), "injury did not cancel attack");
    check(world.actors[1].action == CharacterAction::hurt && world.actors[1].target_id == 0,
          "injury cleanup changed hurt action or retained target");
    check(combat.consume_marker(1,marker("main_hit",11),event,error) && !event.applied, "injury marker applied");
}
void death_and_misses() {
    TestWorld world; CombatSystem combat(world); std::string error; DamageEvent event;
    auto a = attack(); a.cooldown_seconds = 0;
    check(combat.begin(1,2,a,1,error), "miss begin failed");
    world.actors[2].transform.position = {0,3,0};
    const auto hit = marker("main_hit",1);
    check(combat.consume_marker(1,hit,event,error) && !event.applied, "out of range marker hit");
    world.actors[2].transform.position = {1,0,0};
    check(combat.consume_marker(1,hit,event,error) && !event.applied, "miss replay hit after return to range");
    check(combat.finish(1,1), "miss finish failed");
    world.actors[2].health = 5;
    check(combat.begin(1,2,a,2,error), "lethal begin failed");
    check(combat.consume_marker(1,marker("main_hit",2),event,error) && event.applied &&
          event.target_died && event.health_removed == 5 && event.requested_damage == 11, "lethal event differs");
    check(world.actors[2].action == CharacterAction::dead && world.actors[2].target_id == 0,
          "death state not cleaned");
    check(!combat.attacking(1), "dead target retained active attack");
    check(combat.consume_marker(1,marker("off_hit",2,1),event,error) && !event.applied,
          "dead target received offhand hit");
    check(!combat.begin(1,2,a,3,error), "dead target accepted");
}
void range_boundaries() {
    TestWorld world; auto a = attack(); auto& owner = world.actors[1]; auto& target = world.actors[2];
    target.transform.position = {0,0,2.5f};
    check(!attack_target_in_range(a,owner,target,0.5f), "strict melee upper boundary accepted");
    target.transform.position[2] = std::nextafter(2.5f,0.0f);
    check(attack_target_in_range(a,owner,target,0.5f), "inside melee boundary rejected");
    target.transform.position = {0,3,0};
    check(!attack_target_in_range(a,owner,target,0.5f), "vertical distance ignored");
    a.geometry = AttackGeometry::ranged_band; a.minimum_range = 1; a.maximum_range = 3;
    for (float z : {1.0f,3.0f}) {
        target.transform.position = {0,0,z};
        check(attack_target_in_range(a,owner,target,500), "inclusive ranged boundary rejected");
    }
    for (float z : {std::nextafter(1.0f,0.0f),std::nextafter(3.0f,4.0f)}) {
        target.transform.position = {0,0,z};
        check(!attack_target_in_range(a,owner,target,500), "outside ranged band accepted");
    }
    target.transform.position = {0,std::numeric_limits<float>::quiet_NaN(),0};
    check(!attack_target_in_range(a,owner,target,0), "nonfinite position accepted");
}
void unavailable_damage() {
    TestWorld world; CombatSystem combat(world); std::string error; DamageEvent event;
    check(combat.begin(1,2,attack(),1,error), "provider begin failed");
    world.provider_available = false;
    check(!combat.consume_marker(1,marker("main_hit",1),event,error) && !error.empty(), "missing provider fabricated hit");
    check(world.actors[2].health == 100, "missing provider damaged target");
    world.provider_available = true;
    check(combat.consume_marker(1,marker("main_hit",1),event,error) && event.applied,
          "provider failure consumed marker before data became available");
}
void departure_cooldown() {
    // Source character_state.cpp blur case5 starts timer42 on every departure;
    // character_state_owner.cpp transitions call prior blur before new focus.
    TestWorld world; CombatSystem combat(world); std::string error;
    auto a = attack(); a.cooldown_timing = CooldownTiming::attack_departure;
    check(combat.begin(1,2,a,1,error) && combat.cooldown_remaining(1)==0, "departure cooldown started on entry");
    check(combat.update(2,error) && combat.cooldown_remaining(1)==0, "active time consumed departure delay");
    check(combat.finish(1,1) && combat.cooldown_remaining(1)==.5, "normal departure did not start full delay");
    check(!combat.begin(1,2,a,2,error), "AI ignored departure gate");
    check(combat.update(.5,error) && combat.begin(1,2,a,2,error), "departure gate failed to expire");
    world.actors[1].action = CharacterAction::hurt;
    check(combat.update(.2,error) && std::abs(combat.cooldown_remaining(1)-.3)<1e-12,
          "external interruption interval consumed incorrect delay");
    check(combat.update(.3,error), "interruption delay update failed");
    world.actors[1].action = CharacterAction::idle;
    check(combat.begin(1,2,a,3,error), "interrupt gate did not expire");
    world.actors[1].health=0; world.actors[1].action=CharacterAction::dead;
    check(combat.update(0,error) && !combat.attacking(1) && combat.cooldown_remaining(1)==0,
          "dead focus did not clear actor gate");
    // Source dead focus raises42/44/43 to clear gate bits; the original timer
    // service can still hold its callback, which this simplified gate omits.
    world.actors[1].health=100; world.actors[1].action=CharacterAction::idle;
    check(combat.begin(1,2,a,4,error), "revived actor retained cleared gate");
    world.actors[2].health=0; world.actors[2].action=CharacterAction::dead;
    check(combat.update(.1,error) && std::abs(combat.cooldown_remaining(1)-.4)<1e-12,
          "target invalidation departure failed");
}
}
int main() {
    try {
        authored_markers_shared_path(); lifecycle(); death_and_misses(); range_boundaries(); unavailable_damage(); departure_cooldown();
        std::cout << "combat_system_tests: all checks passed (fixture data only)\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "combat_system_tests: " << e.what() << '\n'; return 1;
    }
}
