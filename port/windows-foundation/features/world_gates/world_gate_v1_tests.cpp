// Preview 17 world gate tests: bind, reconcile after load, live flips, persisted applied byte.
#include "world_gate_v1.hpp"
#include "../../asset_catalog.hpp"

#include <cstdio>
#include <map>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::world_gates;

namespace {

void check(bool ok, const char* message) {
    if (!ok) throw std::runtime_error(message);
}

std::uint8_t stored(const PlayableActorWorld& world, ObjectId id, const std::string& key) {
    const auto* object = world.find_object(id);
    check(object != nullptr, "gate object is bound");
    const auto found = object->state_components.find(std::string(world_gate_component_v1) + "." + key);
    check(found != object->state_components.end() && found->second.size() == 1, "gate component is persisted");
    return found->second[0];
}

struct Owner {
    std::vector<std::pair<std::string, bool>> calls;
    WorldGateOwnerV1 fn() {
        return [this](const WorldGateV1& gate, bool value) { calls.emplace_back(gate.name, value); };
    }
};

WorldGateV1 gate(std::uint64_t id, const char* name, const char* key, const char* condition) {
    WorldGateV1 g;
    g.stable_id = id;
    g.gametype = "Door";
    g.name = name;
    g.key = key;
    g.condition = condition;
    return g;
}

void run(const std::string& assets_root) {
    {
        bool enabled=true;std::string error;
        check(source_test_enable_condition_v1(true,true,enabled,error)&&enabled,"true TestEnable enables");
        check(source_test_disable_condition_v1(true,true,enabled,error)&&enabled,"true TestDisable is a no-op");
        check(source_test_disable_condition_v1(true,false,enabled,error)&&!enabled,"false TestDisable disables");
        check(source_test_disable_condition_v1(true,true,enabled,error)&&!enabled,"true TestDisable does not re-enable");
        check(source_test_enable_condition_v1(true,false,enabled,error)&&!enabled,"false TestEnable disables");
        check(source_test_enable_condition_v1(true,true,enabled,error)&&enabled,"later true TestEnable restores the retained object");
        check(source_test_enable_condition_v1(false,std::nullopt,enabled,error)&&enabled,"missing condition list is source no-op");
        const bool before=enabled;
        check(!source_test_disable_condition_v1(true,std::nullopt,enabled,error)&&enabled==before,
              "unknown condition fails without guessing or mutating");
    }
    {   // ObjectManager lifecycle (0x52835 loop, TestEnable 0x33e6d4, TestDisable 0x33e61c, IsTrue 0x33e5f8).
        std::string error;SourceConditionLatchV1 latch;bool enabled=true;
        // NPC_05 shape: no activate list, deactivate_cond=BeforeKillWitch.
        check(source_condition_lifecycle_step_v1(false,std::nullopt,true,true,false,latch,enabled,error)&&enabled&&!latch.disable_tested,
              "deactivate true keeps the object enabled and unlatched");
        check(source_condition_lifecycle_step_v1(false,std::nullopt,true,false,false,latch,enabled,error)&&!enabled&&latch.disable_tested,
              "deactivate false disables and latches");
        check(source_condition_lifecycle_step_v1(false,std::nullopt,true,true,false,latch,enabled,error)&&!enabled,
              "a latched disable is never re-enabled by a later flip");
        SourceConditionLatchV1 none;enabled=false;
        check(source_condition_lifecycle_step_v1(false,std::nullopt,false,std::nullopt,false,none,enabled,error)&&!enabled,
              "no lists: ObjectManager skips the object");
        SourceConditionLatchV1 act;enabled=true;
        check(source_condition_lifecycle_step_v1(true,false,false,std::nullopt,false,act,enabled,error)&&!enabled&&!act.enable_tested,
              "activate false disables without latching");
        check(source_condition_lifecycle_step_v1(true,true,false,std::nullopt,false,act,enabled,error)&&enabled&&act.enable_tested,
              "activate true enables and latches");
        check(source_condition_lifecycle_step_v1(true,false,false,std::nullopt,false,act,enabled,error)&&enabled,
              "a latched activation ignores a later false");
        SourceConditionLatchV1 both;enabled=false;
        check(source_condition_lifecycle_step_v1(true,true,true,false,false,both,enabled,error)&&!enabled&&both.enable_tested&&both.disable_tested,
              "enable runs first and the false disable wins");
        SourceConditionLatchV1 emptyAct;enabled=false;
        check(source_condition_lifecycle_step_v1(false,std::nullopt,true,true,false,emptyAct,enabled,error)&&enabled,
              "empty activate list is true: enables rather than no-op");
        SourceConditionLatchV1 demo;enabled=true;
        check(source_condition_lifecycle_step_v1(true,true,false,std::nullopt,true,demo,enabled,error)&&!enabled&&!demo.enable_tested,
              "demo-only object is disabled");
        SourceConditionLatchV1 unk;enabled=true;
        check(!source_condition_lifecycle_step_v1(true,std::nullopt,false,std::nullopt,false,unk,enabled,error)&&enabled&&!error.empty(),
              "unknown activate value fails without mutating");
        check(!source_condition_lifecycle_step_v1(false,std::nullopt,true,std::nullopt,false,unk,enabled,error)&&enabled,
              "unknown deactivate value fails without mutating");
    }
    AssetCatalog assets(assets_root);
    dh2::data::AiTables tables;
    std::string loadError;
    if (!load_original_ai_tables(assets, "original-cache/data/pydata/", tables, loadError)) throw std::runtime_error(loadError);
    dh2::data::CombatRandom rng{1234, 0};
    PlayableActorWorld world(tables, rng);

    std::map<std::string, bool> values{{"A", true}, {"B", false}};
    auto current = [&values](const std::string& name, bool& value) {
        const auto found = values.find(name);
        if (found == values.end()) return false;
        value = found->second;
        return true;
    };

    // 1. First bind creates objects from the current values; no owner call.
    std::vector<WorldGateV1> gates{gate(101, "door", "unlock_cond", "A"), gate(102, "zone", "activate_cond", "B")};
    std::vector<std::string> notices;
    std::string error;
    Owner owner;
    check(bind_world_gates_v1(world, gates, current, owner.fn(), notices, error), error.c_str());
    check(owner.calls.empty() && notices.empty(), "first bind calls no owner and reports nothing");
    check(gates[0].applied && !gates[1].applied, "applied starts at the current values");
    check(stored(world, 101, "unlock_cond") == 1 && stored(world, 102, "activate_cond") == 0,
          "first bind persists the applied byte per gate key");

    // 2. Live flip: the owner applies once and the byte is persisted.
    std::vector<conditions::ConditionFlipV1> flips{{"B", false, true}};
    values["B"] = true;
    check(apply_world_gate_flips_v1(world, gates, flips, owner.fn(), error), error.c_str());
    check(owner.calls.size() == 1 && owner.calls[0].first == "zone" && owner.calls[0].second,
          "flip reaches the gate reading B with the new value");
    check(stored(world, 102, "activate_cond") == 1, "flip persists the applied value");
    flips = {{"C", false, true}}; // no gate reads C: nothing happens
    check(apply_world_gate_flips_v1(world, gates, flips, owner.fn(), error) && owner.calls.size() == 1,
          "unread condition flips do not call an owner");

    // 3. Load with equal values (restored roster): no replay.
    std::vector<WorldGateV1> loaded{gate(101, "door", "unlock_cond", "A"), gate(102, "zone", "activate_cond", "B")};
    Owner equal;
    check(bind_world_gates_v1(world, loaded, current, equal.fn(), notices, error), error.c_str());
    check(equal.calls.empty() && loaded[1].applied, "restored equal values replay nothing");

    // 4. Reconcile: the condition changed while the owner state was not applied (persisted 0, now true).
    check(world.set_object_component(101, std::string(world_gate_component_v1) + ".unlock_cond",
                                     std::vector<std::uint8_t>{0}, error), error.c_str());
    std::vector<WorldGateV1> reconciled{gate(101, "door", "unlock_cond", "A")};
    Owner reconcile;
    check(bind_world_gates_v1(world, reconciled, current, reconcile.fn(), notices, error), error.c_str());
    check(reconcile.calls.size() == 1 && reconcile.calls[0].second, "a differing restored value is applied once");
    check(reconciled[0].applied && stored(world, 101, "unlock_cond") == 1, "reconcile persists the applied value");

    // 5. Object bound without its gate component (older roster): applied from the current value, component written.
    WorldObject plain;
    plain.id = 104;
    plain.name = "plain";
    check(world.bind_object(std::move(plain), error), error.c_str());
    std::vector<WorldGateV1> older{gate(104, "plain", "activate_cond", "A")};
    Owner older_owner;
    check(bind_world_gates_v1(world, older, current, older_owner.fn(), notices, error), error.c_str());
    check(older_owner.calls.empty() && older[0].applied && stored(world, 104, "activate_cond") == 1,
          "an object without its component is written from the current value");

    // 6. Unavailable condition and missing stable ID are reported, never bound.
    std::vector<WorldGateV1> bad{gate(103, "unknown", "activate_cond", "Missing"), gate(0, "noid", "activate_cond", "A")};
    notices.clear();
    Owner none;
    check(bind_world_gates_v1(world, bad, current, none.fn(), notices, error), error.c_str());
    check(notices.size() == 2 && world.find_object(103) == nullptr, "unavailable and ID-less gates are reported");
    check(none.calls.empty(), "unbound gates do not call the owner");
}

} // namespace

int main(int argc, char** argv) {
    try {
        if (argc != 2) throw std::runtime_error("usage: world_gate_v1_tests <assets root>");
        run(argv[1]);
        std::printf("world_gate_v1: all tests passed\n");
        return 0;
    } catch (const std::exception& e) {
        std::fprintf(stderr, "world_gate_v1 test failed: %s\n", e.what());
        return 1;
    }
}
