#include "source_light_point_v1.hpp"

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <tuple>
#include <vector>

using namespace dh::foundation::frontend;

namespace {
void check(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}
bool near(float left, float right) { return std::fabs(left - right) < 1e-7f; }

struct Fixture {
    std::shared_ptr<int> current_owner = std::make_shared<int>(1);
    std::shared_ptr<int> tweaker_owner = std::make_shared<int>(2);
    std::shared_ptr<int> node_owner = std::make_shared<int>(3);
    std::shared_ptr<int> point_owner = std::make_shared<int>(4);
    std::shared_ptr<int> light_pin = std::make_shared<int>(5);
    std::shared_ptr<dh2::world::NativeLightV113> light;
    SourceLightPointValuesV1 light_values{
        {2.f, .04f, .0005f}, {.1f, .2f, .3f}, {.4f, .5f, .6f}, {.7f, .8f, .9f}};
    dh2::world::NativeLightNodeV113 node;
    std::vector<std::string> calls;
    SourceLightPointServicesV1 services;

    Fixture() {
        // The test records the actual typed receiver identity without
        // constructing the root-owned NativeLight implementation here.
        light = std::shared_ptr<dh2::world::NativeLightV113>(
            light_pin, reinterpret_cast<dh2::world::NativeLightV113*>(light_pin.get()));
        node.owner = node_owner;
        node.identity = 0x100;
        node.light = light;
        node.position = [](auto&, auto&) { return true; };
        node.set_position = [](const auto&, auto&) { return true; };

        services.owner = current_owner;
        services.player_tweaker_owner = tweaker_owner;
        services.player_tweaker_identity = 0x300;
        services.current = [](auto&) { return true; };
        services.first_scene_light = [this](auto root, auto& out, bool& found, auto&) {
            check(root == 0x400, "SceneManager root identity changed");
            calls.emplace_back("first_scene_light");
            out = node;
            found = true;
            return true;
        };
        services.spawn_light_point = [this](const char* class_name, const char* name,
                                             bool flag_a, bool flag_b, auto& out, auto&) {
            check(std::string(class_name) == "LightPoint" && std::string(name) == "charselectLight",
                  "Original LightPoint spawn names changed");
            check(flag_a && flag_b, "Original LightPoint spawn flags changed");
            calls.emplace_back("spawn");
            out = {point_owner, 0x200, 19};
            return true;
        };
        services.set_light_node = [this](auto actor, const auto& actual, auto& copied_values, auto&) {
            check(actor == 0x200 && actual.identity == 0x100 && actual.light == light,
                  "setLightNode did not borrow the same source receivers");
            calls.emplace_back("setLightNode");
            copied_values = light_values; // Source LightBase copies the actual CLight fields.
            return true;
        };
        services.set_light = [this](auto set, auto slot, auto actor, const auto& actual, auto&) {
            check(set == 0 && slot == 0 && actor == 0x200 && actual == light,
                  "AssignTweaker did not reach PlayerLight slot0 with the same CLight");
            calls.emplace_back("set_player_light");
            return true;
        };
        services.assign_player_tweaker = [this](auto tweaker, auto slot, auto actor,
                                                 const auto& values, auto&) {
            check(tweaker == 0x300 && slot == 0 && actor == 0x200,
                  "AssignTweaker app receiver/index differs");
            check(values.attenuation == light_values.attenuation &&
                  values.ambient == std::array<float, 3>{.1f, .2f, .3f} &&
                  values.diffuse == std::array<float, 3>{.4f, .5f, .6f} &&
                  values.specular == std::array<float, 3>{.7f, .8f, .9f},
                  "AssignTweaker did not copy the LightBase fields reached by setLightNode");
            calls.emplace_back("assign_tweaker");
            return true;
        };
        services.append_scene_automatic_light = [this](auto root, auto actor, auto&) {
            check(root == 0x400 && actor == 0x200, "Automatic-light list received a different receiver");
            calls.emplace_back("append_automatic_light");
            return true;
        };
        services.set_attenuation = [this](auto actor, const auto& actual, const auto& value, auto&) {
            check(actor == 0x200 && actual == light &&
                  value == std::array<float, 3>{.375f, .51172f, 1.0469f},
                  "Source attenuation setter inputs differ");
            calls.emplace_back("attenuation");
            light_values.attenuation = {value[0], value[1] / 1000.f, value[2] / 1000000.f};
            return true;
        };
        auto color = [this](const char* label, auto member, auto actor,
                            const auto& actual, const auto& value, auto&) {
            check(actor == 0x200 && actual == light && value == std::array<float, 3>{1, 1, 1},
                  "Source class-light color input/identity differs");
            calls.emplace_back(label);
            light_values.*member = {1, 1, 1};
            return true;
        };
        services.set_ambient = [color](auto actor, const auto& actual, const auto& value, auto& error) mutable {
            return color("ambient", &SourceLightPointValuesV1::ambient, actor, actual, value, error);
        };
        services.set_diffuse = [color](auto actor, const auto& actual, const auto& value, auto& error) mutable {
            return color("diffuse", &SourceLightPointValuesV1::diffuse, actor, actual, value, error);
        };
        services.set_specular = [color](auto actor, const auto& actual, const auto& value, auto& error) mutable {
            return color("specular", &SourceLightPointValuesV1::specular, actor, actual, value, error);
        };
    }

};

void success_sequence() {
    Fixture fixture;
    SourceLightPointOwnerV1 owner(fixture.services);
    std::string error;
    check(owner.show(0x400, error), "Recording-provider sequence fixture failed");
    const std::vector<std::string> expected{
        "first_scene_light", "spawn", "setLightNode", "set_player_light",
        "assign_tweaker", "append_automatic_light", "attenuation", "ambient", "diffuse", "specular"};
    check(fixture.calls == expected, "Menu Show source operation order changed");
    check(owner.state() == SourceLightPointStateV1::live &&
          owner.prefix() == SourceLightPointPrefixV1::specular_set,
          "Successful source prefix/state differs");
    check(owner.light_point().identity == 0x200 && owner.scene_light().identity == 0x100,
          "Same live source receiver identities were not retained");
    check(near(owner.values().attenuation[0], .375f) &&
          near(owner.values().attenuation[1], .51172f / 1000.f) &&
          near(owner.values().attenuation[2], 1.0469f / 1000000.f),
          "LightBase source attenuation conversion differs");
    check(fixture.light_values.ambient == std::array<float, 3>{1, 1, 1} &&
          fixture.light_values.diffuse == std::array<float, 3>{1, 1, 1} &&
          fixture.light_values.specular == std::array<float, 3>{1, 1, 1},
          "Source color setters did not reach the same CLight");
    check(!owner.after_object_manager_flush(error), "ObjectManager.Flush bypassed SceneManager.clear");
    check(owner.after_scene_manager_clear(error) && owner.after_object_manager_flush(error),
          "Source cleanup acknowledgment order failed");
    check(owner.state() == SourceLightPointStateV1::object_manager_flushed &&
          owner.light_point().identity == 0 && owner.scene_light().identity == 0,
          "Source leases survived ObjectManager.Flush");
}

void source_skip() {
    Fixture fixture;
    fixture.services.first_scene_light = [&fixture](auto, auto&, bool& found, auto&) {
        fixture.calls.emplace_back("first_scene_light");
        found = false;
        return true;
    };
    SourceLightPointOwnerV1 owner(fixture.services);
    std::string error;
    check(owner.show(0x400, error) && owner.state() == SourceLightPointStateV1::no_authored_light &&
          owner.prefix() == SourceLightPointPrefixV1::no_authored_light,
          "Source missing-light skip branch changed");
    check(fixture.calls == std::vector<std::string>{"first_scene_light"},
          "Missing authored light reached LightPoint Spawn");
    check(owner.after_scene_manager_clear(error) && owner.after_object_manager_flush(error),
          "No-light Hide acknowledgments failed");
}

void irreversible_failures() {
    const std::vector<std::string> steps{
        "first_scene_light", "spawn", "setLightNode", "set_player_light", "assign_tweaker",
        "append_automatic_light", "attenuation", "ambient", "diffuse", "specular"};
    for (const auto& failed_step : steps) {
        Fixture fixture;
        SourceLightPointServicesV1 services = fixture.services;
        auto fail = [&](const std::string& step) { return [&, step](auto&&... args) {
            fixture.calls.emplace_back(step);
            auto& error = std::get<sizeof...(args) - 1>(std::forward_as_tuple(args...));
            error = "source failure at " + step;
            return false;
        }; };
        // Replace one reached operation while retaining the previous actual
        // provider surface for every other source call.
        if (failed_step == "first_scene_light") services.first_scene_light = [&, f = fail(failed_step)](auto&&... a) mutable { return f(std::forward<decltype(a)>(a)...); };
        if (failed_step == "spawn") services.spawn_light_point = [&, f = fail(failed_step)](auto&&... a) mutable { return f(std::forward<decltype(a)>(a)...); };
        if (failed_step == "setLightNode") services.set_light_node = [&, f = fail(failed_step)](auto&&... a) mutable { return f(std::forward<decltype(a)>(a)...); };
        if (failed_step == "set_player_light") services.set_light = [&, f = fail(failed_step)](auto&&... a) mutable { return f(std::forward<decltype(a)>(a)...); };
        if (failed_step == "assign_tweaker") services.assign_player_tweaker = [&, f = fail(failed_step)](auto&&... a) mutable { return f(std::forward<decltype(a)>(a)...); };
        if (failed_step == "append_automatic_light") services.append_scene_automatic_light = [&, f = fail(failed_step)](auto&&... a) mutable { return f(std::forward<decltype(a)>(a)...); };
        if (failed_step == "attenuation") services.set_attenuation = [&, f = fail(failed_step)](auto&&... a) mutable { return f(std::forward<decltype(a)>(a)...); };
        if (failed_step == "ambient") services.set_ambient = [&, f = fail(failed_step)](auto&&... a) mutable { return f(std::forward<decltype(a)>(a)...); };
        if (failed_step == "diffuse") services.set_diffuse = [&, f = fail(failed_step)](auto&&... a) mutable { return f(std::forward<decltype(a)>(a)...); };
        if (failed_step == "specular") services.set_specular = [&, f = fail(failed_step)](auto&&... a) mutable { return f(std::forward<decltype(a)>(a)...); };

        SourceLightPointOwnerV1 owner(std::move(services));
        std::string error;
        check(!owner.show(0x400, error) && owner.state() == SourceLightPointStateV1::failed_prefix &&
              error == "source failure at " + failed_step,
              "Reached source failure did not retain its error prefix");
        const auto calls_before_replay = fixture.calls.size();
        check(!owner.show(0x400, error) && fixture.calls.size() == calls_before_replay,
              "Failed source prefix was replayed");
        check(owner.after_scene_manager_clear(error) && owner.after_object_manager_flush(error),
              "Reached failure prefix could not acknowledge source Hide order");
    }
}
}

int main() {
    try {
        check(SourceLightPointConstructorV1::address == 0x40bdd8 &&
              SourceLightPointConstructorV1::factory_address == 0x34115c &&
              SourceLightPointConstructorV1::allocation_bytes == 0x1b4 &&
              SourceLightPointConstructorV1::object_type == 19 &&
              SourceLightPointConstructorV1::spawn_flag_a &&
              SourceLightPointConstructorV1::spawn_flag_b,
              "Recovered LightPoint constructor facts changed");
        success_sequence();
        source_skip();
        irreversible_failures();
        std::cout << "PASS source LightPoint constructor facts, Show ordering, setter units, and failure prefixes\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
