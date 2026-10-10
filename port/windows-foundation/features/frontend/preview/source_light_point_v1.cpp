#include "source_light_point_v1.hpp"

#include <exception>
#include <utility>

namespace dh::foundation::frontend {
namespace {
bool required(std::string& error, const char* name) {
    error = std::string("Required actual class-select LightPoint ") + name;
    return false;
}

bool valid_light_node(const dh2::world::NativeLightNodeV113& node) {
    return node.owner && node.identity && node.light && node.position && node.set_position;
}
}

SourceLightPointOwnerV1::SourceLightPointOwnerV1(SourceLightPointServicesV1 services)
    : services_(std::move(services)) {}

bool SourceLightPointOwnerV1::current(std::string& error) {
    if (!services_.owner || !services_.current)
        return required(error, "same live application/SceneManager/ObjectManager owner");
    if (!services_.current(error)) {
        if (error.empty()) error = "Actual class-select LightPoint source owner is no longer current";
        return false;
    }
    error.clear();
    return true;
}

bool SourceLightPointOwnerV1::fail(std::string error, std::string& out) {
    if (error.empty()) error = "Actual class-select LightPoint source operation failed";
    error_ = std::move(error);
    state_ = SourceLightPointStateV1::failed_prefix;
    out = error_;
    return false;
}

bool SourceLightPointOwnerV1::show(std::uintptr_t scene_root, std::string& error) {
    if (busy_) return fail("LightPoint Show synchronously reentered", error);
    if (state_ != SourceLightPointStateV1::fresh || !scene_root)
        return fail("LightPoint Show cannot replay or use a null scene root", error);
    if (!current(error)) return fail(error, error);

    busy_ = true;
    struct Guard { bool& busy; ~Guard() { busy = false; } } guard{busy_};
    scene_root_ = scene_root;

    auto run = [&](const char* missing, const auto& operation) {
        if (!current(error)) return fail(error, error);
        try {
            if (!operation())
                return fail(error.empty() ? std::string("Actual class-select LightPoint ") + missing : error, error);
        } catch (const std::exception& e) {
            return fail(e.what(), error);
        } catch (...) {
            return fail(std::string("Actual class-select LightPoint ") + missing + " threw", error);
        }
        return true;
    };

    bool found{};
    if (!services_.first_scene_light)
        return fail("actual first authored 'lght' lookup", error);
    if (!run("first authored 'lght' lookup", [&] {
            return services_.first_scene_light(scene_root_, scene_light_, found, error);
        })) return false;
    if (!found) {
        state_ = SourceLightPointStateV1::no_authored_light;
        prefix_ = SourceLightPointPrefixV1::no_authored_light;
        error_.clear();
        error.clear();
        return true;
    }
    if (!valid_light_node(scene_light_))
        return fail("same live authored CLightSceneNode receiver", error);
    prefix_ = SourceLightPointPrefixV1::first_scene_light_found;

    if (!services_.spawn_light_point)
        return fail("ObjectManager::Spawn(" + std::string(SourceLightPointConstructorV1::class_name) + ")", error);
    if (!run("ObjectManager::Spawn(LightPoint)", [&] {
            // The source call is ObjectManager::Spawn("LightPoint",
            // "charselectLight", true, true), then GetObject(0).
            SourceLightPointBorrowV1 reached;
            const bool result = services_.spawn_light_point(
                SourceLightPointConstructorV1::class_name,
                SourceLightPointConstructorV1::source_name,
                SourceLightPointConstructorV1::spawn_flag_a,
                SourceLightPointConstructorV1::spawn_flag_b,
                reached, error);
            light_point_ = std::move(reached); // Preserve a reached failure prefix.
            return result;
        })) return false;
    if (!light_point_.owner || !light_point_.identity)
        return fail("same spawned ObjectManager LightPoint receiver", error);
    if (light_point_.source_type != SourceLightPointConstructorV1::object_type)
        return fail("source type-19 LightPoint receiver", error);
    prefix_ = SourceLightPointPrefixV1::light_point_spawned;

    if (!services_.set_light_node)
        return fail("LightBase::setLightNode", error);
    if (!run("LightBase::setLightNode", [&] {
            return services_.set_light_node(light_point_.identity, scene_light_, values_, error);
        })) return false;
    light_ = scene_light_.light; // Same CLight under the retained source node.
    prefix_ = SourceLightPointPrefixV1::light_node_bound;

    if (!services_.set_light)
        return fail("LightSetManager::SetLight(PlayerLight, 0)", error);
    if (!run("LightSetManager::SetLight(PlayerLight, 0)", [&] {
            return services_.set_light(0, 0, light_point_.identity, light_, error);
        })) return false;
    prefix_ = SourceLightPointPrefixV1::player_light_set;

    if (!services_.player_tweaker_owner || !services_.player_tweaker_identity || !services_.assign_player_tweaker)
        return fail("same Application PlayerLightTweaker::AssignTweaker(0, 0)", error);
    if (!run("PlayerLightTweaker::AssignTweaker(0, 0)", [&] {
            return services_.assign_player_tweaker(services_.player_tweaker_identity, 0,
                light_point_.identity, values_, error);
        })) return false;
    prefix_ = SourceLightPointPrefixV1::player_tweaker_assigned;

    if (!services_.append_scene_automatic_light)
        return fail("same SceneManager automatic-light append", error);
    if (!run("SceneManager automatic-light append", [&] {
            return services_.append_scene_automatic_light(scene_root_, light_point_.identity, error);
        })) return false;
    prefix_ = SourceLightPointPrefixV1::automatic_light_appended;

    // Original Show429270..42928c values, passed to LightBase setters after
    // AssignTweaker and automatic-light append. Linear/quadratic unit
    // conversion is performed by the source LightBase setter (1000/1e6).
    static constexpr std::array<float, 3> attenuation_setter{0.375f, 0.51172f, 1.0469f};
    static constexpr std::array<float, 3> white{1.f, 1.f, 1.f};

    if (!services_.set_attenuation)
        return fail("LightBase::SetAttenuation", error);
    if (!run("LightBase::SetAttenuation", [&] {
            return services_.set_attenuation(light_point_.identity, light_, attenuation_setter, error);
        })) return false;
    values_.attenuation = {attenuation_setter[0], attenuation_setter[1] / 1000.f,
                           attenuation_setter[2] / 1000000.f};
    prefix_ = SourceLightPointPrefixV1::attenuation_set;

    if (!services_.set_ambient)
        return fail("LightBase::SetAmbientColor", error);
    if (!run("LightBase::SetAmbientColor", [&] {
            return services_.set_ambient(light_point_.identity, light_, white, error);
        })) return false;
    values_.ambient = white;
    prefix_ = SourceLightPointPrefixV1::ambient_set;

    if (!services_.set_diffuse)
        return fail("LightBase::SetDiffuseColor", error);
    if (!run("LightBase::SetDiffuseColor", [&] {
            return services_.set_diffuse(light_point_.identity, light_, white, error);
        })) return false;
    values_.diffuse = white;
    prefix_ = SourceLightPointPrefixV1::diffuse_set;

    if (!services_.set_specular)
        return fail("LightBase::SetSpecularColor", error);
    if (!run("LightBase::SetSpecularColor", [&] {
            return services_.set_specular(light_point_.identity, light_, white, error);
        })) return false;
    values_.specular = white;
    prefix_ = SourceLightPointPrefixV1::specular_set;

    state_ = SourceLightPointStateV1::live;
    error_.clear();
    error.clear();
    return true;
}

bool SourceLightPointOwnerV1::after_scene_manager_clear(std::string& error) {
    if (busy_ || (state_ != SourceLightPointStateV1::live &&
                  state_ != SourceLightPointStateV1::no_authored_light &&
                  state_ != SourceLightPointStateV1::failed_prefix))
        return fail("SceneManager.clear must follow the same class-select Show prefix", error);
    state_ = SourceLightPointStateV1::scene_cleared;
    error.clear();
    return true;
}

bool SourceLightPointOwnerV1::after_object_manager_flush(std::string& error) {
    if (busy_ || state_ != SourceLightPointStateV1::scene_cleared)
        return fail("ObjectManager.Flush must follow SceneManager.clear", error);
    // SceneManager.clear removes the CLightSceneNode before ObjectManager.Flush
    // destroys the actual LightPoint. The source receiver leases retire only
    // after both external operations have completed.
    light_point_ = {};
    scene_light_ = {};
    light_.reset();
    scene_root_ = 0;
    state_ = SourceLightPointStateV1::object_manager_flushed;
    error.clear();
    return true;
}

} // namespace dh::foundation::frontend
