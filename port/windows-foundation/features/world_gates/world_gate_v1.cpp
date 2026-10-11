#include "world_gate_v1.hpp"

namespace dh::foundation::world_gates {
namespace {

std::string component_key(const WorldGateV1& gate) {
    return std::string(world_gate_component_v1) + "." + gate.key; // one applied byte per gate key of the object
}

bool read_applied(const WorldObject& object, const WorldGateV1& gate, bool& value) {
    const auto found = object.state_components.find(component_key(gate));
    if (found == object.state_components.end() || found->second.size() != 1 || found->second[0] > 1) return false;
    value = found->second[0] != 0;
    return true;
}

bool write_applied(PlayableActorWorld& world, const WorldGateV1& gate, std::string& error) {
    return world.set_object_component(gate.stable_id, component_key(gate),
                                      std::vector<std::uint8_t>{std::uint8_t(gate.applied ? 1 : 0)}, error);
}

} // namespace

bool bind_world_gates_v1(PlayableActorWorld& world, std::vector<WorldGateV1>& gates,
                         const std::function<bool(const std::string& condition, bool& value)>& current,
                         const WorldGateOwnerV1& owner, std::vector<std::string>& notices, std::string& error) {
    error.clear();
    for (auto& gate : gates) {
        if (gate.stable_id == 0) {
            notices.push_back("world gate without a stable ID is not persisted: " + gate.name);
            continue;
        }
        bool now = false;
        if (!current || !current(gate.condition, now)) {
            notices.push_back("world gate condition unavailable, not bound: " + gate.name + " " + gate.condition);
            continue;
        }
        bool missing_component = false;
        if (const auto* existing = world.find_object(gate.stable_id)) {
            // A restored object (GameSave roster) keeps the value its owner last applied.
            bool stored = false;
            missing_component = !read_applied(*existing, gate, stored);
            gate.applied = missing_component ? now : stored;
        } else {
            WorldObject object;
            object.id = gate.stable_id;
            object.name = gate.name;
            gate.applied = now;
            object.state_components[component_key(gate)] = std::vector<std::uint8_t>{std::uint8_t(now ? 1 : 0)};
            std::string bindError;
            if (!world.bind_object(std::move(object), bindError)) {
                notices.push_back("world gate object bind failed, not persisted: " + gate.name + ": " + bindError);
                continue;
            }
        }
        if (gate.applied != now) {
            // Reconcile: the condition changed while the persisted owner state was not applied.
            owner(gate, now);
            gate.applied = now;
            if (!write_applied(world, gate, error)) return false;
        } else if (missing_component) {
            if (!write_applied(world, gate, error)) return false;
        }
    }
    return true;
}

bool apply_world_gate_flips_v1(PlayableActorWorld& world, std::vector<WorldGateV1>& gates,
                               const std::vector<conditions::ConditionFlipV1>& flips,
                               const WorldGateOwnerV1& owner, std::string& error) {
    error.clear();
    for (const auto& flip : flips) {
        for (auto& gate : gates) {
            if (gate.condition != flip.name || gate.stable_id == 0) continue;
            owner(gate, flip.after);
            gate.applied = flip.after;
            if (!write_applied(world, gate, error)) return false;
        }
    }
    return true;
}

bool source_test_enable_condition_v1(bool has_condition,std::optional<bool> value,
                                     bool& enabled,std::string& error) {
    error.clear();
    if(!has_condition)return true;
    if(!value){error="TestEnableCondition value is unavailable";return false;}
    enabled=*value;
    return true;
}

bool source_test_disable_condition_v1(bool has_condition,std::optional<bool> value,
                                      bool& enabled,std::string& error) {
    error.clear();
    if(!has_condition)return true;
    if(!value){error="TestDisableCondition value is unavailable";return false;}
    if(!*value)enabled=false;
    return true;
}

bool source_condition_lifecycle_step_v1(bool has_enable,std::optional<bool> enable_value,
                                        bool has_disable,std::optional<bool> disable_value,
                                        bool demo_only,SourceConditionLatchV1& latch,
                                        bool& enabled,std::string& error) {
    error.clear();
    const bool enable_idle=latch.enable_tested||!has_enable;
    const bool disable_idle=latch.disable_tested||!has_disable;
    if(enable_idle&&disable_idle)return true; // ObjectManager skips objects with nothing left to test
    if(!enable_idle&&!enable_value){error="TestEnableCondition value is unavailable";return false;}
    if(!disable_idle&&!disable_value){error="TestDisableCondition value is unavailable";return false;}
    const bool activation=enable_idle||*enable_value;
    if(demo_only||!activation)enabled=false;
    else{enabled=true;latch.enable_tested=true;}
    if(!disable_idle&&!*disable_value){enabled=false;latch.disable_tested=true;}
    return true;
}

} // namespace dh::foundation::world_gates
