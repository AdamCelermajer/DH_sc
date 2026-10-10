#include "menu_class_select_light_owner_v1.hpp"
#include "native_scene_lights_v113.hpp"

#include <algorithm>
#include <array>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace dh2::world;

namespace {
void check(bool value, const char* message) {
    if (!value) {
        std::cerr << "FAIL " << message << '\n';
        std::exit(1);
    }
}
}

int main() {
    std::string error;
    std::shared_ptr<NativeSceneLightNodeV113> authored;
    check(NativeSceneLightNodeV113::construct(authored, error),
          "actual native source light-node constructor failed");
    auto native_light = authored->light();
    const auto authored_id = authored->identity();
    const auto light_point_id = std::uintptr_t{0x1919};
    auto integration = std::make_shared<int>(1);
    auto point_lease = std::make_shared<int>(19);
    auto app_tweaker_boundary = std::make_shared<int>(2);
    NativeLightSetV113 light_sets(std::make_shared<LightSetNameOwnerV3>());
    std::vector<std::string> calls;
    std::vector<std::uintptr_t> automatic_lights;

    MenuClassSelectLightServicesV1 services;
    services.owner = integration;
    services.current = [&calls](std::string& out) {
        calls.emplace_back("current"); out.clear(); return true;
    };
    services.first_scene_light = [&authored, &calls](std::uintptr_t root,
        MenuClassSelectSceneLightV1& out, bool& found, std::string& e) {
        calls.emplace_back("first-light");
        if (root != 0x44 || !authored || !authored->live()) { e = "stale test scene"; return false; }
        out.owner = authored; out.identity = authored->identity(); found = true; e.clear(); return true;
    };
    services.spawn_light_point = [&point_lease, &calls, light_point_id](const char* cls,
        const char* name, bool a, bool b, MenuClassSelectLightPointV1& out, std::string& e) {
        calls.emplace_back("spawn");
        if (std::string(cls) != "LightPoint" || std::string(name) != "charselectLight" || !a || !b) {
            e = "source Spawn arguments differ"; return false;
        }
        out = {point_lease, light_point_id, 19}; e.clear(); return true;
    };
    services.set_light_node = [&calls, authored_id, light_point_id](std::uintptr_t point,
        std::uintptr_t node, std::string& e) {
        calls.emplace_back("setLightNode");
        const bool ok = point == light_point_id && node == authored_id;
        e = ok ? std::string{} : "different native scene light"; return ok;
    };
    services.assign_tweaker = [&calls, &light_sets, native_light, app_tweaker_boundary,
        light_point_id](std::uintptr_t point, std::int32_t set, std::int32_t slot, std::string& e) {
        calls.emplace_back("AssignTweaker");
        if (point != light_point_id || set != 0 || slot != 0 || !app_tweaker_boundary) {
            e = "source tweaker target differs"; return false;
        }
        // AssignTweaker's native LightSetManager publication reaches the same CLight.
        // The Application tweaker owner is represented only as an explicitly pinned boundary.
        if (!light_sets.set_light(set, slot, native_light, e)) return false;
        e.clear(); return true;
    };
    services.append_scene_automatic_light = [&calls, &automatic_lights, light_point_id](
        std::uintptr_t point, std::string& e) {
        calls.emplace_back("automatic-light");
        if (point != light_point_id) { e = "different spawned LightPoint"; return false; }
        automatic_lights.push_back(point); e.clear(); return true;
    };
    services.set_attenuation = [&calls, &native_light, light_point_id](std::uintptr_t point,
        const float value[3], std::string& e) {
        calls.emplace_back("attenuation");
        if (point != light_point_id) { e = "different LightPoint"; return false; }
        native_light->attenuation34 = {value[0], value[1] / 1000.f, value[2] / 1000000.f};
        e.clear(); return true;
    };
    auto set_white = [&calls, &native_light, light_point_id](const char* name,
        std::array<float, 4>& target, std::uintptr_t point, const float value[3], std::string& e) {
        calls.emplace_back(name);
        if (point != light_point_id) { e = "different LightPoint"; return false; }
        target = {value[0], value[1], value[2], 1.f}; e.clear(); return true;
    };
    services.set_ambient = [set_white, &native_light](std::uintptr_t p, const float v[3], std::string& e) mutable {
        return set_white("ambient", native_light->ambient4, p, v, e);
    };
    services.set_diffuse = [set_white, &native_light](std::uintptr_t p, const float v[3], std::string& e) mutable {
        return set_white("diffuse", native_light->diffuse14, p, v, e);
    };
    services.set_specular = [set_white, &native_light](std::uintptr_t p, const float v[3], std::string& e) mutable {
        return set_white("specular", native_light->specular24, p, v, e);
    };

    MenuClassSelectLightOwnerV1 menu(services);
    check(menu.show(0x44, error), "source-ordered MenuCharacterSelect::Show light branch failed");
    check(menu.state() == MenuClassSelectLightStateV1::live &&
          menu.prefix() == MenuClassSelectLightPrefixV1::specular_set,
          "successful Show must reach the source specular setter prefix");
    const std::vector<std::string> expected{
        "current", "current", "first-light", "current", "spawn", "current",
        "setLightNode", "current", "AssignTweaker", "current", "automatic-light",
        "current", "attenuation", "current", "ambient", "current", "diffuse",
        "current", "specular"};
    check(calls == expected, "source call order or owner revalidation differs");
    std::shared_ptr<NativeLightV113> published;
    check(light_sets.get_light(0, 0, published, error) && published == native_light,
          "AssignTweaker must publish the same native CLight into LightSetManager slot (0,0)");
    check(automatic_lights == std::vector<std::uintptr_t>{light_point_id},
          "automatic-light list must contain this spawned LightPoint identity");
    check(native_light->attenuation34 == std::array<float,3>{0.375f, 0.00051172f, 0.0000010469f} &&
          native_light->ambient4 == std::array<float,4>{1,1,1,1} &&
          native_light->diffuse14 == std::array<float,4>{1,1,1,1} &&
          native_light->specular24 == std::array<float,4>{1,1,1,1},
          "the original setter values must reach the same actual NativeLightV113 fields");
    check(menu.scene_light().identity == authored_id && menu.light_point().identity == light_point_id,
          "menu owner must retain the exact scene-light and LightPoint identities");

    check(menu.after_scene_manager_clear(error), "source SceneManager.clear acknowledgement failed");
    check(menu.after_object_manager_flush(error), "source ObjectManager.Flush acknowledgement failed");
    check(menu.state() == MenuClassSelectLightStateV1::object_manager_flushed &&
          !menu.scene_light().owner && !menu.light_point().owner,
          "native source leases must retire only after clear then Flush");

    auto absent_light_services = services;
    bool spawn_after_missing_light{};
    absent_light_services.first_scene_light = [](std::uintptr_t,
        MenuClassSelectSceneLightV1& out, bool& found, std::string& e) {
        out = {}; found = false; e.clear(); return true;
    };
    absent_light_services.spawn_light_point = [&spawn_after_missing_light](const char*, const char*,
        bool, bool, MenuClassSelectLightPointV1&, std::string& e) {
        spawn_after_missing_light = true; e = "must not reach spawn"; return false;
    };
    MenuClassSelectLightOwnerV1 absent_menu(std::move(absent_light_services));
    check(absent_menu.show(0x44, error) && !spawn_after_missing_light &&
          absent_menu.state() == MenuClassSelectLightStateV1::no_authored_light &&
          absent_menu.prefix() == MenuClassSelectLightPrefixV1::no_authored_light,
          "a genuine missing authored light must take the original early skip");
    check(absent_menu.after_scene_manager_clear(error) && absent_menu.after_object_manager_flush(error),
          "missing-light Show still follows the source clear-then-Flush lifecycle");

    auto failed_services = services;
    failed_services.append_scene_automatic_light = [](std::uintptr_t, std::string& e) {
        e = "automatic light owner unavailable"; return false;
    };
    const auto count_setters = [&calls] {
        return std::count(calls.begin(), calls.end(), "attenuation") +
               std::count(calls.begin(), calls.end(), "ambient") +
               std::count(calls.begin(), calls.end(), "diffuse") +
               std::count(calls.begin(), calls.end(), "specular");
    };
    const auto setter_calls_before = count_setters();
    MenuClassSelectLightOwnerV1 failed_menu(std::move(failed_services));
    check(!failed_menu.show(0x44, error) &&
          failed_menu.state() == MenuClassSelectLightStateV1::failed_prefix &&
          failed_menu.prefix() == MenuClassSelectLightPrefixV1::player_light_tweaker_assigned &&
          failed_menu.light_point().identity == light_point_id,
          "a reached append failure must retain the exact source prefix and spawned receiver");
    check(count_setters() == setter_calls_before,
          "later color/attenuation setters must not run after failed automatic-light append");
    check(failed_menu.after_scene_manager_clear(error) && failed_menu.after_object_manager_flush(error),
          "failed source prefix leases still retire only after clear then Flush");

    std::cout << "PASS class-select Show source order and same NativeSceneLightNode/CLight/LightSet identity\n";
}
