#pragma once

#include "../../../../level-world/canonical_object_manager_v1.hpp"
#include "../../../../level-world/native_scene_lights_v113.hpp"

#include <array>
#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <string>

namespace dh::foundation::frontend {

// Source LightPoint C1/C2 field projection over the SAME type-19 ObjectBase
// receiver already created and published by ObjectManager. This is not an
// allocator and never manufactures an ObjectBase, SceneManager, or renderer.
// The constructor offsets and writes are recovered from 0x40aba4/0x40bdd8.
struct SourceLightPointNativeFieldsV1 {
    static constexpr std::uintptr_t factory_address = 0x34115c;
    static constexpr std::uintptr_t objectbase_c1_address = 0x33f15c;
    static constexpr std::uintptr_t lightbase_c1_address = 0x40aba4;
    static constexpr std::uintptr_t lightpoint_c1_address = 0x40bdd8;
    static constexpr std::uintptr_t lightbase_set_node_address = 0x40b498;
    static constexpr std::uintptr_t lightpoint_init_post_address = 0x40bcdc;
    static constexpr std::uintptr_t lightpoint_update_address = 0x40ba0c;
    static constexpr std::uintptr_t lightpoint_refresh_address = 0x40bac0;
    static constexpr std::uintptr_t lightpoint_destructor_address = 0x40bd1c;
    static constexpr std::uint32_t allocation_bytes = 0x1b4;
    static constexpr std::uint32_t object_type = 19;

    // LightBase +288 starts null. +304 is deliberately absent: recovered C1
    // does not write it, so this projection does not invent a value for it.
    std::uintptr_t light_node288{};
    std::array<float, 3> attenuation308_{};
    std::array<float, 3> ambient320_{};
    std::array<float, 3> diffuse332_{};
    std::array<float, 3> specular344_{};
    std::uint32_t radius304_bits{};
    bool radius304_produced{};
    // C1 does not write +356. Preserve that fact until the source properties
    // or InitPost producer reaches this LightBase field.
    std::optional<std::uint8_t> add_to_scene356;
    std::string light_base_string360;

    // LightPoint C1 constructs an empty string at +384, the real attached
    // ObjectHandle at +408, and three zero offsets at +420/+424/+428.
    std::string attached_to384;
    dh2::target_providers::Handle16 attached_object408{0, UINT32_MAX, 0};
    std::array<float, 3> attached_offset420{};
    std::uint8_t refresh_state432{};
    std::uint8_t refresh_state433{};
};

// External operations whose source implementations need the real Application,
// SceneManager, ObjectManager, or actor graph. A service is accepted only with
// an owner and identity for that same live application. These hooks continue
// source services; they are not synthetic success defaults.
struct SourceLightPointNativeServicesV1 {
    std::shared_ptr<void> application_owner;
    std::uintptr_t application_identity{};
    std::function<bool(std::string&)> current_application;
    std::function<bool(std::uintptr_t, SourceLightPointNativeFieldsV1&,
                       dh2::world::NativeLightNodeV113&, std::string&)> light_base_init_post;
    std::function<bool(std::uintptr_t, SourceLightPointNativeFieldsV1&,
                       dh2::world::NativeLightNodeV113&, std::string&)> light_base_update;
    std::function<bool(std::uintptr_t, SourceLightPointNativeFieldsV1&,
                       dh2::world::NativeLightNodeV113&, std::string&)> light_point_update_tail;
    std::function<bool(std::uintptr_t, SourceLightPointNativeFieldsV1&,
                       dh2::world::NativeLightNodeV113&, std::string&)> refresh_attachment;
    std::function<bool(std::uintptr_t, std::uintptr_t, std::string&)> source_objectbase_destroy;
};

enum class SourceLightPointNativeStageV1 : std::uint8_t {
    constructed,
    node_bound,
    initialized,
    destroyed,
    failed,
};

// Owns exact LightBase/LightPoint C1 fields and one native reference to the
// same borrowed NativeSceneLightNodeV113. The canonical identity/lease still
// belongs to ObjectManager; this object only adds LightPoint-specific state.
class SourceLightPointNativeV1 final {
    dh2::world::CanonicalObjectBorrowV1 object_;
    std::shared_ptr<void> application_owner_;
    std::uintptr_t application_identity_{};
    SourceLightPointNativeFieldsV1 fields_;
    std::shared_ptr<dh2::world::NativeSceneLightNodeV113> node_owner_;
    dh2::world::NativeLightNodeV113 node_;
    SourceLightPointNativeServicesV1 services_;
    SourceLightPointNativeStageV1 stage_{SourceLightPointNativeStageV1::constructed};
    std::string error_;

    SourceLightPointNativeV1(dh2::world::CanonicalObjectBorrowV1,
                             std::shared_ptr<void>, std::uintptr_t,
                             SourceLightPointNativeServicesV1);
    bool current(std::string&);
    bool fail(std::string, std::string&);

public:
    SourceLightPointNativeV1(const SourceLightPointNativeV1&) = delete;
    SourceLightPointNativeV1& operator=(const SourceLightPointNativeV1&) = delete;
    ~SourceLightPointNativeV1() noexcept;

    // C2 canonical receiver adoption. It requires the actual factory's
    // ObjectManager result (type_f4==19) and the same live application.
    static bool adopt_type19_v1(dh2::world::CanonicalObjectBorrowV1,
                                std::shared_ptr<void> application_owner,
                                std::uintptr_t application_identity,
                                SourceLightPointNativeServicesV1,
                                std::shared_ptr<SourceLightPointNativeV1>&,
                                std::string&);

    // Equivalent of LightBase::setLightNode: retains the same native node and
    // snapshots the same CLight fields into LightBase. A second node cannot be
    // substituted after the source setter is reached.
    bool set_light_node(const dh2::world::NativeLightNodeV113&, std::string&);
    bool init_post(std::string&);
    bool update(std::string&);
    bool refresh_attachment(std::string&);
    bool destroy_after_object_manager_unpublish(std::string&);

    const dh2::world::CanonicalObjectBorrowV1& objectbase() const noexcept { return object_; }
    const SourceLightPointNativeFieldsV1& fields() const noexcept { return fields_; }
    const dh2::world::NativeLightNodeV113& node() const noexcept { return node_; }
    const std::shared_ptr<dh2::world::NativeLightV113>& light() const noexcept { return node_.light; }
    SourceLightPointNativeStageV1 stage() const noexcept { return stage_; }
    const std::string& error() const noexcept { return error_; }
};

} // namespace dh::foundation::frontend
