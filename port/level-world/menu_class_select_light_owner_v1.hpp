#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <string>

namespace dh2::world {

// The borrowed first authored light returned by the source
// SceneManager::getSceneNodeFromType('lght') call in
// MenuCharacterSelect::Show. The pin is a host lifetime lease; identity is the
// actual menu-scene light-node receiver, never a copied light value.
struct MenuClassSelectSceneLightV1 {
    std::shared_ptr<void> owner;
    std::uintptr_t identity{};
};

// The actual LightPoint object created by ObjectManager::Spawn. The manager
// remains the source owner; this lease only keeps the same receiver usable
// while the menu sequence runs.
struct MenuClassSelectLightPointV1 {
    std::shared_ptr<void> owner;
    std::uintptr_t identity{};
    std::uint32_t source_type{};
};

struct MenuClassSelectLightServicesV1 {
    std::shared_ptr<void> owner;
    std::function<bool(std::string&)> current;
    std::function<bool(std::uintptr_t, MenuClassSelectSceneLightV1&, bool&, std::string&)> first_scene_light;
    std::function<bool(const char*, const char*, bool, bool, MenuClassSelectLightPointV1&, std::string&)> spawn_light_point;
    std::function<bool(std::uintptr_t, std::uintptr_t, std::string&)> set_light_node;
    // Same LightPoint::AssignTweaker call publishes LightBase to LightSetManager
    // and the Application PlayerLightTweaker cells.
    std::function<bool(std::uintptr_t, std::int32_t, std::int32_t, std::string&)> assign_tweaker;
    std::function<bool(std::uintptr_t, std::string&)> append_scene_automatic_light;
    std::function<bool(std::uintptr_t, const float[3], std::string&)> set_attenuation;
    std::function<bool(std::uintptr_t, const float[3], std::string&)> set_ambient;
    std::function<bool(std::uintptr_t, const float[3], std::string&)> set_diffuse;
    std::function<bool(std::uintptr_t, const float[3], std::string&)> set_specular;
};

enum class MenuClassSelectLightStateV1 {
    fresh,
    no_authored_light,
    live,
    failed_prefix,
    scene_cleared,
    object_manager_flushed,
};

enum class MenuClassSelectLightPrefixV1 {
    fresh,
    no_authored_light,
    first_scene_light_found,
    light_point_spawned,
    light_node_bound,
    player_light_tweaker_assigned,
    automatic_light_appended,
    attenuation_set,
    ambient_set,
    diffuse_set,
    specular_set,
};

// Owns only the menu route around the Omni01 LightPoint. It does not create a
// SceneManager, RenderFX, ObjectManager or native LightPoint. The caller must
// bind every callback to the same live source owners. ObjectManager remains
// responsible for LightPoint::Update each frame; this owner does not issue a
// second Update. Its destructor is passive: source SceneManager.clear must
// precede ObjectManager.Flush, and both must finish before the leases retire.
class MenuClassSelectLightOwnerV1 final {
    MenuClassSelectLightServicesV1 services_;
    MenuClassSelectSceneLightV1 scene_light_;
    MenuClassSelectLightPointV1 light_point_;
    std::uintptr_t scene_root_{};
    MenuClassSelectLightStateV1 state_{MenuClassSelectLightStateV1::fresh};
    MenuClassSelectLightPrefixV1 prefix_{MenuClassSelectLightPrefixV1::fresh};
    bool busy_{};
    std::string error_;

    bool current(std::string&);
    bool fail(std::string, std::string&);

public:
    explicit MenuClassSelectLightOwnerV1(MenuClassSelectLightServicesV1);
    MenuClassSelectLightOwnerV1(const MenuClassSelectLightOwnerV1&) = delete;
    MenuClassSelectLightOwnerV1& operator=(const MenuClassSelectLightOwnerV1&) = delete;
    ~MenuClassSelectLightOwnerV1() noexcept = default;

    // Mirrors the Show light branch. A genuine missing 'lght' node is the
    // source's skip result. Once a LightPoint spawn or later call is reached,
    // failures retain the exact partial receiver and forbid replay.
    bool show(std::uintptr_t scene_root, std::string&);

    // These acknowledge completion of the actual surrounding Hide calls.
    // They perform no cleanup themselves and reject the reversed source order.
    bool after_scene_manager_clear(std::string&);
    bool after_object_manager_flush(std::string&);

    MenuClassSelectLightStateV1 state() const noexcept { return state_; }
    MenuClassSelectLightPrefixV1 prefix() const noexcept { return prefix_; }
    const MenuClassSelectLightPointV1& light_point() const noexcept { return light_point_; }
    const MenuClassSelectSceneLightV1& scene_light() const noexcept { return scene_light_; }
    const std::string& error() const noexcept { return error_; }
};

} // namespace dh2::world
