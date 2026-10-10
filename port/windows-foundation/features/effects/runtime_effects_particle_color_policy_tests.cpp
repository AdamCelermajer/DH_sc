#include "runtime_swing_fx_observer_v1.hpp"
#include "runtime_effects_factory_v1.hpp"

#include <stdexcept>

using namespace dh::foundation::effects;
using namespace dh::foundation;

namespace {
int calls{};
bool legacy_driver(void*, std::uint32_t& value, std::string& error) {
    ++calls;
    value = 0x78u;
    error.clear();
    return true;
}
void require(bool value, const char* why) {
    if (!value) throw std::runtime_error(why);
}
}

int main() {
    std::string error;
    std::uint32_t branch = 0xffffffffu;
    RuntimeEffectsSceneViewV1 view;
    view.particle_color_policy = RuntimeEffectsParticleColorPolicyV1::source_white;
    view.driver_type = legacy_driver;
    require(resolve_runtime_effects_particle_color_branch_v1(view, branch, error),
            "explicit source_white policy failed");
    require(branch == 0 && calls == 0 && error.empty(),
            "source_white must select the white input branch without consulting or impersonating a driver");

    view.particle_color_policy.reset();
    require(resolve_runtime_effects_particle_color_branch_v1(view, branch, error),
            "legacy driver callback path failed");
    require(branch == 0x78u && calls == 1 && error.empty(),
            "omitted policy must preserve the legacy driver enum callback");

    view.driver_type = nullptr;
    branch = 0xffffffffu;
    require(!resolve_runtime_effects_particle_color_branch_v1(view, branch, error) &&
            error == "Required actual renderer driver type" && branch == 0xffffffffu,
            "missing legacy provider must fail without fabricating a branch");

    ActorState actor;
    actor.id = 31;
    actor.transform.position = {10.f, 20.f, 30.f};
    actor.source_target_node180 = std::uintptr_t(0);
    actor.source_target_position184 = std::array<float,3>{90.f, 91.f, 92.f};
    unsigned enabled_queries = 0;
    RuntimeSwingFxEnabledResolverV1 enabled =
        [&](ActorId, std::uint8_t& value, std::string& why) {
            ++enabled_queries;
            value = 1;
            why.clear();
            return true;
        };
    std::array<float,3> position{};
    require(runtime_swing_fx_target_position_v1(actor, actor.id, enabled, position, error) &&
            position == actor.transform.position && enabled_queries == 0,
            "known-null own target node must ignore a stale cached point and use actor position");

    actor.source_target_node180 = std::uintptr_t(42);
    actor.source_target_position184 = std::array<float,3>{1.f, 2.f, 3.f};
    require(runtime_swing_fx_target_position_v1(actor, actor.id, enabled, position, error) &&
            position == *actor.source_target_position184 && enabled_queries == 1,
            "non-null own target node with enabled80 must use the cached point");

    enabled = [&](ActorId, std::uint8_t& value, std::string& why) {
        ++enabled_queries;
        value = 0;
        why.clear();
        return true;
    };
    actor.source_target_position184 = std::array<float,3>{90.f, 91.f, 92.f};
    require(runtime_swing_fx_target_position_v1(actor, actor.id, enabled, position, error) &&
            position == actor.transform.position && enabled_queries == 2,
            "disabled owner must select actor position despite a stale non-null-node cache");

    enabled = [](ActorId, std::uint8_t& value, std::string& why) {
        value = 1;
        why.clear();
        return true;
    };
    actor.source_target_position184.reset();
    position = {7.f, 8.f, 9.f};
    require(!runtime_swing_fx_target_position_v1(actor, actor.id, enabled, position, error) &&
            error.find("cache184") != std::string::npos && position == std::array<float,3>{7.f,8.f,9.f},
            "enabled non-null node without its produced cache must fail without changing caller output");

    actor.source_target_node180.reset();
    require(!runtime_swing_fx_target_position_v1(actor, actor.id, enabled, position, error),
            "unknown own target-node owner must not be treated as a null node");
    return 0;
}
