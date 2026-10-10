#include "menu_class_select_light_owner_v1.hpp"

#include <exception>
#include <utility>

namespace dh2::world {
namespace {
bool required(std::string& error, const char* name) {
    error = std::string("Required actual class-select light ") + name;
    return false;
}

bool same_owner(const std::shared_ptr<void>& a, const std::shared_ptr<void>& b) noexcept {
    return a && b && a.get() == b.get() && !a.owner_before(b) && !b.owner_before(a);
}
}

MenuClassSelectLightOwnerV1::MenuClassSelectLightOwnerV1(MenuClassSelectLightServicesV1 services)
    : services_(std::move(services)) {}

bool MenuClassSelectLightOwnerV1::current(std::string& error) {
    if (!services_.owner || !services_.current) return required(error, "same live renderer/SceneManager owner");
    if (!services_.current(error)) {
        if (error.empty()) error = "Actual class-select light source owner is no longer current";
        return false;
    }
    error.clear();
    return true;
}

bool MenuClassSelectLightOwnerV1::fail(std::string error, std::string& out) {
    if (error.empty()) error = "Actual class-select light source operation failed";
    error_ = std::move(error);
    state_ = MenuClassSelectLightStateV1::failed_prefix;
    out = error_;
    return false;
}

bool MenuClassSelectLightOwnerV1::show(std::uintptr_t scene_root, std::string& error) {
    if (busy_) return fail("Class-select light lifecycle synchronously reentered", error);
    if (state_ != MenuClassSelectLightStateV1::fresh || !scene_root)
        return fail("Class-select LightPoint Show cannot replay or use a null scene root", error);
    if (!current(error)) return fail(error, error);

    busy_ = true;
    struct Guard { bool& busy; ~Guard() { busy = false; } } guard{busy_};
    scene_root_ = scene_root;
    auto run = [&](const char* name, const auto& operation) {
        if (!current(error)) return fail(error, error);
        try {
            if (!operation())
                return fail(error.empty() ? std::string("Actual class-select light ") + name : error, error);
        } catch (const std::exception& failure) {
            return fail(failure.what(), error);
        } catch (...) {
            return fail(std::string("Actual class-select light ") + name + " threw", error);
        }
        return true;
    };

    bool found{};
    if (!services_.first_scene_light)
        return fail("first authored SceneManager 'lght' lookup", error);
    if (!run("first authored 'lght' lookup", [&] {
            return services_.first_scene_light(scene_root_, scene_light_, found, error);
        })) return false;
    if (!found) {
        scene_light_ = {};
        prefix_ = MenuClassSelectLightPrefixV1::no_authored_light;
        state_ = MenuClassSelectLightStateV1::no_authored_light;
        error_.clear();
        error.clear();
        return true;
    }
    if (!scene_light_.owner || !scene_light_.identity)
        return fail("same retained first authored CLightSceneNode", error);
    prefix_ = MenuClassSelectLightPrefixV1::first_scene_light_found;

    if (!services_.spawn_light_point)
        return fail("ObjectManager::Spawn(LightPoint, charselectLight, true, true)", error);
    if (!run("ObjectManager::Spawn(LightPoint)", [&] {
            // Retain any published ObjectManager prefix even when Spawn reports
            // failure, so Hide can keep it pinned through the real Flush.
            MenuClassSelectLightPointV1 reached;
            const bool ok = services_.spawn_light_point("LightPoint", "charselectLight",
                                                         true, true, reached, error);
            light_point_ = std::move(reached);
            return ok;
        })) return false;
    if (!light_point_.owner || !light_point_.identity)
        return fail("same published ObjectManager LightPoint receiver", error);
    if (light_point_.source_type != 19)
        return fail("original ObjectBase type-19 LightPoint receiver", error);
    prefix_ = MenuClassSelectLightPrefixV1::light_point_spawned;

    if (!services_.set_light_node)
        return fail("LightBase::setLightNode(same first scene light)", error);
    if (!run("LightBase::setLightNode", [&] {
            return services_.set_light_node(light_point_.identity, scene_light_.identity, error);
        })) return false;
    prefix_ = MenuClassSelectLightPrefixV1::light_node_bound;

    if (!services_.assign_tweaker)
        return fail("LightPoint::AssignTweaker(0, 0)", error);
    if (!run("LightPoint::AssignTweaker(0, 0)", [&] {
            return services_.assign_tweaker(light_point_.identity, 0, 0, error);
        })) return false;
    prefix_ = MenuClassSelectLightPrefixV1::player_light_tweaker_assigned;

    if (!services_.append_scene_automatic_light)
        return fail("SceneManager automatic-light append", error);
    if (!run("SceneManager automatic-light append", [&] {
            return services_.append_scene_automatic_light(light_point_.identity, error);
        })) return false;
    prefix_ = MenuClassSelectLightPrefixV1::automatic_light_appended;

    // The original Show stores the same float values after AssignTweaker and
    // the automatic-list append; LightBase setters mutate that same CLight.
    constexpr float attenuation[3]{0.375f, 0.51172f, 1.0469f};
    constexpr float white[3]{1.0f, 1.0f, 1.0f};
    if (!services_.set_attenuation)
        return fail("LightBase::SetAttenuation", error);
    if (!run("LightBase::SetAttenuation", [&] {
            return services_.set_attenuation(light_point_.identity, attenuation, error);
        })) return false;
    prefix_ = MenuClassSelectLightPrefixV1::attenuation_set;

    if (!services_.set_ambient)
        return fail("LightBase::SetAmbientColor", error);
    if (!run("LightBase::SetAmbientColor", [&] {
            return services_.set_ambient(light_point_.identity, white, error);
        })) return false;
    prefix_ = MenuClassSelectLightPrefixV1::ambient_set;

    if (!services_.set_diffuse)
        return fail("LightBase::SetDiffuseColor", error);
    if (!run("LightBase::SetDiffuseColor", [&] {
            return services_.set_diffuse(light_point_.identity, white, error);
        })) return false;
    prefix_ = MenuClassSelectLightPrefixV1::diffuse_set;

    if (!services_.set_specular)
        return fail("LightBase::SetSpecularColor", error);
    if (!run("LightBase::SetSpecularColor", [&] {
            return services_.set_specular(light_point_.identity, white, error);
        })) return false;
    prefix_ = MenuClassSelectLightPrefixV1::specular_set;
    state_ = MenuClassSelectLightStateV1::live;
    error_.clear();
    error.clear();
    return true;
}

bool MenuClassSelectLightOwnerV1::after_scene_manager_clear(std::string& error) {
    if (busy_ || (state_ != MenuClassSelectLightStateV1::live &&
                  state_ != MenuClassSelectLightStateV1::no_authored_light &&
                  state_ != MenuClassSelectLightStateV1::failed_prefix))
        return fail("SceneManager.clear must follow the same class-select Show prefix", error);
    state_ = MenuClassSelectLightStateV1::scene_cleared;
    error.clear();
    return true;
}

bool MenuClassSelectLightOwnerV1::after_object_manager_flush(std::string& error) {
    if (busy_ || state_ != MenuClassSelectLightStateV1::scene_cleared)
        return fail("ObjectManager.Flush must follow SceneManager.clear", error);
    // Source SceneManager.clear removes the native CLightSceneNode first;
    // ObjectManager.Flush then runs the actor's real D0. Only now may these
    // host pins be dropped. No source destruction is performed here.
    light_point_ = {};
    scene_light_ = {};
    scene_root_ = 0;
    state_ = MenuClassSelectLightStateV1::object_manager_flushed;
    error.clear();
    return true;
}

} // namespace dh2::world
