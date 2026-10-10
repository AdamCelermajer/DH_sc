#pragma once

#include "../../../../level-world/native_scene_lights_v113.hpp"

#include <array>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>

namespace dh::foundation::frontend {

// Values recovered from libDungeonHunter2.so, not inferred from the preview
// renderer. LightPoint::C1 is 0x40bdd8 and allocates 0x1b4 bytes through
// GetNewInstance<LightPoint>() at 0x34115c. Its LightBase subobject is created
// with ObjectBase type 19. Native allocation and manager publication remain
// the responsibility of the source ObjectManager provider.
struct SourceLightPointConstructorV1 {
    static constexpr std::uintptr_t address = 0x40bdd8;
    static constexpr std::uintptr_t factory_address = 0x34115c;
    static constexpr std::uint32_t allocation_bytes = 0x1b4;
    static constexpr std::uint32_t object_type = 19;
    static constexpr const char* class_name = "LightPoint";
    static constexpr const char* source_name = "charselectLight";
    static constexpr bool spawn_flag_a = true;
    static constexpr bool spawn_flag_b = true;
};

struct SourceLightPointValuesV1 {
    std::array<float, 3> attenuation{};
    std::array<float, 3> ambient{};
    std::array<float, 3> diffuse{};
    std::array<float, 3> specular{};
};

struct SourceLightPointBorrowV1 {
    // Lease and identity come from the same real ObjectManager LightPoint.
    std::shared_ptr<void> owner;
    std::uintptr_t identity{};
    std::uint32_t source_type{};
};

enum class SourceLightPointPrefixV1 {
    fresh,
    first_scene_light_found,
    light_point_spawned,
    light_node_bound,
    player_light_set,
    player_tweaker_assigned,
    automatic_light_appended,
    attenuation_set,
    ambient_set,
    diffuse_set,
    specular_set,
    no_authored_light,
};

enum class SourceLightPointStateV1 {
    fresh,
    no_authored_light,
    live,
    failed_prefix,
    scene_cleared,
    object_manager_flushed,
};

struct SourceLightPointServicesV1 {
    // Pins one current application/SceneManager/ObjectManager integration.
    // These are caller-owned source providers, not constructors for managers.
    std::shared_ptr<void> owner;
    // Root binds this identity to the current real PlayerLightTweakerOwnerV90.
    std::shared_ptr<void> player_tweaker_owner;
    std::uintptr_t player_tweaker_identity{};
    std::function<bool(std::string&)> current;

    std::function<bool(std::uintptr_t, dh2::world::NativeLightNodeV113&, bool&, std::string&)> first_scene_light;
    std::function<bool(const char*, const char*, bool, bool, SourceLightPointBorrowV1&, std::string&)> spawn_light_point;
    std::function<bool(std::uintptr_t, const dh2::world::NativeLightNodeV113&,
                       SourceLightPointValuesV1&, std::string&)> set_light_node;
    std::function<bool(std::int32_t, std::int32_t, std::uintptr_t,
                       const std::shared_ptr<dh2::world::NativeLightV113>&, std::string&)> set_light;
    std::function<bool(std::uintptr_t, std::uint32_t, std::uintptr_t,
                       const SourceLightPointValuesV1&, std::string&)> assign_player_tweaker;
    std::function<bool(std::uintptr_t, std::uintptr_t, std::string&)> append_scene_automatic_light;
    std::function<bool(std::uintptr_t, const std::shared_ptr<dh2::world::NativeLightV113>&,
                       const std::array<float, 3>&, std::string&)> set_attenuation;
    std::function<bool(std::uintptr_t, const std::shared_ptr<dh2::world::NativeLightV113>&,
                       const std::array<float, 3>&, std::string&)> set_ambient;
    std::function<bool(std::uintptr_t, const std::shared_ptr<dh2::world::NativeLightV113>&,
                       const std::array<float, 3>&, std::string&)> set_diffuse;
    std::function<bool(std::uintptr_t, const std::shared_ptr<dh2::world::NativeLightV113>&,
                       const std::array<float, 3>&, std::string&)> set_specular;
};

// Source-ordered coordinator for MenuCharacterSelect::Show's LightPoint
// branch. It never creates managers, a LightPoint, a CLight, or a scene node.
// Every callback must reach the corresponding operation on the same live
// source objects. It retains reached leases through SceneManager.clear and
// ObjectManager.Flush and records irreversible failure prefixes.
class SourceLightPointOwnerV1 final {
    SourceLightPointServicesV1 services_;
    dh2::world::NativeLightNodeV113 scene_light_;
    SourceLightPointBorrowV1 light_point_;
    std::shared_ptr<dh2::world::NativeLightV113> light_;
    SourceLightPointValuesV1 values_;
    std::uintptr_t scene_root_{};
    SourceLightPointStateV1 state_{SourceLightPointStateV1::fresh};
    SourceLightPointPrefixV1 prefix_{SourceLightPointPrefixV1::fresh};
    bool busy_{};
    std::string error_;

    bool current(std::string&);
    bool fail(std::string, std::string&);

public:
    explicit SourceLightPointOwnerV1(SourceLightPointServicesV1);
    SourceLightPointOwnerV1(const SourceLightPointOwnerV1&) = delete;
    SourceLightPointOwnerV1& operator=(const SourceLightPointOwnerV1&) = delete;
    ~SourceLightPointOwnerV1() noexcept = default;

    // Mirrors the original Show branch. A real absence of a scene 'lght' node
    // is the source's skip path; after Spawn the exact reached prefix is kept.
    bool show(std::uintptr_t scene_root, std::string&);

    // Acknowledge actual outer Hide calls; these methods perform no cleanup.
    bool after_scene_manager_clear(std::string&);
    bool after_object_manager_flush(std::string&);

    SourceLightPointStateV1 state() const noexcept { return state_; }
    SourceLightPointPrefixV1 prefix() const noexcept { return prefix_; }
    const SourceLightPointBorrowV1& light_point() const noexcept { return light_point_; }
    const dh2::world::NativeLightNodeV113& scene_light() const noexcept { return scene_light_; }
    const SourceLightPointValuesV1& values() const noexcept { return values_; }
    const std::string& error() const noexcept { return error_; }
};

} // namespace dh::foundation::frontend
