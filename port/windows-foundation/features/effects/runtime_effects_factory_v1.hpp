#pragma once

#include "runtime_combat_effects_v1.hpp"
#include "effects_material_binding.hpp"
#include "../../content_paths.hpp"
#include "../../../level-world/character_authored_fx_forces_v4.hpp"
#include "../../../level-world/character_authored_resource_v32.hpp"
#include "../../../level-world/character_blood_fx_resource_v2.hpp"
#include "../../../level-world/character_fx_floor_query_v3.hpp"
#include "../../../level-world/character_mesh_fx_owner_v4.hpp"
#include <memory>
#include <optional>

namespace dh::foundation::effects {

#if defined(DH_RUNTIME_EFFECTS_FACTORY_TESTING)
bool runtime_effects_factory_test_expired_session_guards(
    class RuntimeEffectsFactoryV1&, std::string& error);
#endif

// Camera owner for the exact retained actor Scene used by CharacterMeshFxOwnerV4.
// The callback receives that borrowed Scene so it cannot silently substitute a
// preview camera or a second scene's view.
enum class RuntimeEffectsParticleColorPolicyV1 { source_white };
struct RuntimeEffectsSceneViewV1 {
    void* context{};
    bool (*camera)(void*, const dh2::scene::Scene&, float view16[16],
                   float position3[3], std::string&){};
    bool (*driver_type)(void*, std::uint32_t&, std::string&){};
    // Explicit port-renderer policy for the authored particle scene-color
    // branch. This selects source-white particle input without claiming an
    // original platform driver's numeric enum. Omission keeps driver_type.
    std::optional<RuntimeEffectsParticleColorPolicyV1> particle_color_policy;
};
// Resolves the value consumed by particle_scene_color_v1. Under source_white,
// the value is only an internal selector for its (driver & 7)==0 white branch;
// it must not be surfaced as an original platform driver ID.
inline bool resolve_runtime_effects_particle_color_branch_v1(
    const RuntimeEffectsSceneViewV1& view, std::uint32_t& branch,
    std::string& error) {
    if (view.particle_color_policy ==
        RuntimeEffectsParticleColorPolicyV1::source_white) {
        branch = 0;
        error.clear();
        return true;
    }
    if (!view.driver_type) {
        error = "Required actual renderer driver type";
        return false;
    }
    return view.driver_type(view.context, branch, error);
}

// Optional source-semantic override for a bound actor. In the current generic
// backend, an actor in the live CombatSession is enabled by default. A future
// source actor owner may report a bound-but-disabled actor explicitly.
struct FxActorPolicyV1 {
    bool disabled{};
};
using FxActorPolicyResolverV1 =
    std::function<bool(ActorId, FxActorPolicyV1&, std::string&)>;

struct RuntimeEffectsFactoryBindingsV1 {
    const AssetCatalog* assets{};
    dh2::data::EffectsTables::Borrow tables;
    dh2::navigation::CollisionWorld* same_pf_world{};
    RuntimeEffectsSceneViewV1 scene_view;
    EffectTextureServices textures;
    std::function<bool(std::shared_ptr<const EffectRenderFrame>, std::string&)> submit;
    FxActorPolicyResolverV1 actor_policy;
};

// Owns the actual source FX manager and the resource/material adapters around
// it. All actor/pose/Scene borrows come from the supplied CombatSession; this
// factory does not create another CharacterVisual or actor pose.
class RuntimeEffectsFactoryV1 final {
public:
    static std::unique_ptr<RuntimeEffectsFactoryV1> create(
        CombatSession&, ActorId scene_actor, RuntimeEffectsFactoryBindingsV1,
        std::string& error);
    ~RuntimeEffectsFactoryV1();
    RuntimeEffectsFactoryV1(const RuntimeEffectsFactoryV1&) = delete;
    RuntimeEffectsFactoryV1& operator=(const RuntimeEffectsFactoryV1&) = delete;

    RuntimeCombatEffectsV1& runtime() noexcept;
    dh2::fx::CharacterMeshFxOwnerV4& manager() noexcept;
    std::size_t cached_original_textures() const noexcept;
    // Verifies the exact live host instance and retained Scene lease, not only
    // coincident ActorIds in separate sessions. No raw saved host is dereferenced.
    bool is_bound_to(const CombatSession&) const noexcept;
    // Call after render-frame loans/queue work have drained and while the GL
    // context used by `textures.upload` is current.
    void clear_original_textures();

private:
#if defined(DH_RUNTIME_EFFECTS_FACTORY_TESTING)
    friend bool runtime_effects_factory_test_expired_session_guards(
        RuntimeEffectsFactoryV1&, std::string& error);
#endif
    struct Impl;
    explicit RuntimeEffectsFactoryV1(std::unique_ptr<Impl>);
    std::unique_ptr<Impl> impl_;
};

} // namespace dh::foundation::effects
