#include "runtime_effects_factory_v1.hpp"
#include "runtime_source_fx_asset_v1.hpp"

#include <utility>

namespace dh::foundation::effects {
namespace {
bool same_owner(const std::weak_ptr<const void>& a,
                const std::weak_ptr<const void>& b) noexcept {
    return !a.owner_before(b) && !b.owner_before(a);
}
bool fail(std::string& error, const char* message) {
    error = message;
    return false;
}
}

struct RuntimeEffectsFactoryV1::Impl {
    CombatSession& session;
    RuntimeEffectsFactoryBindingsV1 bindings;
    ActorId scene_actor{};
    const dh2::scene::Scene* scene{};
    std::weak_ptr<const void> lease;
    dh2::fx::CharacterAuthoredFxForceFactoryOwnerV4 forces;
    dh2::fx::CharacterAuthoredResourceFactoryV32 resources;
    std::unique_ptr<OriginalEffectMaterialBinding> materials;
    std::unique_ptr<dh2::fx::CharacterMeshFxOwnerV4> manager;
    std::unique_ptr<RuntimeCombatEffectsV1> runtime;

    Impl(CombatSession& s, ActorId owner, RuntimeEffectsFactoryBindingsV1 b,
         const dh2::scene::Scene* retained_scene)
        : session(s), bindings(std::move(b)), scene_actor(owner),
          scene(retained_scene), lease(session.actor_binding_lease()),
          resources(dh2::fx::CharacterBloodFxSceneServicesV2{
              this, camera_adapter, driver_adapter}, forces.factory()) {}

    static bool camera_adapter(void* raw, float view[16], float position[3],
                               std::string& error) {
        auto& self = *static_cast<Impl*>(raw);
        if (self.lease.expired())
            return fail(error, "FX camera Scene lease expired before session access");
        if (!self.scene || !self.bindings.scene_view.camera)
            return fail(error, "Required actual camera owner for the retained FX Scene");
        const auto current = self.session.actor_binding_lease();
        if (current.expired() || !same_owner(current, self.lease))
            return fail(error, "FX camera Scene lease is no longer current");
        return self.bindings.scene_view.camera(self.bindings.scene_view.context,
                                                *self.scene, view, position, error);
    }
    static bool driver_adapter(void* raw, std::uint32_t& type,
                               std::string& error) {
        auto& self = *static_cast<Impl*>(raw);
        return resolve_runtime_effects_particle_color_branch_v1(
            self.bindings.scene_view, type, error);
    }
    static bool read_asset(void* raw, const char* uri,
                           std::vector<std::uint8_t>& bytes, std::string& error) {
        auto& self = *static_cast<Impl*>(raw);
        if (!self.bindings.assets || !uri)
            return fail(error, "Required AssetCatalog and original FX resource URI");
        if (is_runtime_source_fx_uri_v1(uri)) {
            std::string resolved;
            return read_runtime_source_fx_asset_v1(*self.bindings.assets,uri,bytes,resolved,error);
        }
        try {
            bytes = read_content(*self.bindings.assets, uri);
            error.clear();
            return true;
        } catch (const std::exception& e) {
            error = e.what();
            return false;
        }
    }
    bool current_lease(std::string& error) const {
        if (lease.expired())
            return fail(error, "FX manager's saved CombatSession Scene lease has expired");
        const auto current = session.actor_binding_lease();
        return !current.expired() && same_owner(current, lease)
            ? (error.clear(), true)
            : fail(error, "FX manager requires the same retained CombatSession Scene lease");
    }
    bool actor(std::uintptr_t identity, ActorId& id, const ActorState*& state,
               std::string& error) const {
        id = static_cast<ActorId>(identity);
        if (!identity || static_cast<std::uintptr_t>(id) != identity)
            return fail(error, "FX anchor identity is not a CombatSession ActorId");
        if (!current_lease(error)) return false;
        state = session.actor(id);
        if (!state || state->id != id)
            return fail(error, "FX anchor is absent from the same CombatSession");
        return true;
    }
    static bool invoke(void* raw, dh2::fx::MeshFxRequestV1& q,
                       std::string& error) {
        auto& self = *static_cast<Impl*>(raw);
        using Op = dh2::fx::MeshFxOperationV1;
        q.result = 0;
        if (q.live_scene != self.scene)
            return fail(error, "FX request Scene differs from the retained source Scene");
        if (!self.current_lease(error)) return false;
        switch (q.operation) {
        case Op::debug_load:
            // Tracing is diagnostic-only; its omission does not alter FX state,
            // timeline, visibility or the production resource path.
            error.clear(); return true;
        case Op::module_enabled:
            // Construction of this owner is the explicit effects-enabled gate.
            q.result = 1; error.clear(); return true;
        case Op::set_switch:
        case Op::instance_switch:
            error.clear(); return true;
        case Op::anchor_dead:
        case Op::anchor_disabled:
        case Op::anchor_stationary:
        case Op::anchor_position:
        case Op::anchor_rotation:
        case Op::anchor_scale: {
            ActorId id{}; const ActorState* state{};
            if (!self.actor(q.identity, id, state, error)) return false;
            if (q.operation == Op::anchor_dead) {
                q.result = state->alive() ? 0u : 1u;
            } else if (q.operation == Op::anchor_disabled) {
                FxActorPolicyV1 policy{};
                if (self.bindings.actor_policy &&
                    !self.bindings.actor_policy(id, policy, error)) return false;
                q.result = policy.disabled ? 1u : 0u;
            } else if (q.operation == Op::anchor_stationary) {
                // Always resample the retained transform. For a stationary
                // actor this is output-equivalent to the source skip branch.
                q.result = 0;
            } else if (q.operation == Op::anchor_position) {
                std::copy(state->transform.position.begin(),
                          state->transform.position.end(), q.point);
            } else if (q.operation == Op::anchor_rotation) {
                std::copy(state->transform.rotation.begin(),
                          state->transform.rotation.end(), q.point);
            } else {
                std::copy(state->transform.scale.begin(),
                          state->transform.scale.end(), q.point);
            }
            error.clear(); return true;
        }
        case Op::floor_normal: {
            if (!self.bindings.same_pf_world)
                return fail(error, "Required same PFWorld for original FX floor query");
            float normal[3]{};
            return dh2::fx::character_fx_floor_query_v3(
                self.bindings.same_pf_world, q.point, normal, error);
        }
        }
        return fail(error, "Unsupported CharacterMeshFxOwnerV4 service operation");
    }
};

#if defined(DH_RUNTIME_EFFECTS_FACTORY_TESTING)
bool runtime_effects_factory_test_expired_session_guards(
    RuntimeEffectsFactoryV1& factory, std::string& error) {
    if (!factory.impl_) return fail(error, "FX factory test requires the retained Impl");
    std::string current_error, camera_error;
    const bool current = factory.impl_->current_lease(current_error);
    float view[16]{}, position[3]{};
    const bool camera = RuntimeEffectsFactoryV1::Impl::camera_adapter(
        factory.impl_.get(), view, position, camera_error);
    if (current || camera)
        return fail(error, "An expired-session FX API accepted work");
    if (current_error.find("expired") == std::string::npos ||
        camera_error.find("expired") == std::string::npos)
        return fail(error, "Expired-session guards did not report lease expiry");
    error = "current_lease and camera_adapter rejected before touching the destroyed CombatSession";
    return true;
}
#endif

RuntimeEffectsFactoryV1::RuntimeEffectsFactoryV1(std::unique_ptr<Impl> p)
    : impl_(std::move(p)) {}
RuntimeEffectsFactoryV1::~RuntimeEffectsFactoryV1() = default;

std::unique_ptr<RuntimeEffectsFactoryV1> RuntimeEffectsFactoryV1::create(
    CombatSession& session, ActorId scene_actor, RuntimeEffectsFactoryBindingsV1 b,
    std::string& error) {
    error.clear();
    const bool needs_driver_type =
        b.scene_view.particle_color_policy !=
        RuntimeEffectsParticleColorPolicyV1::source_white;
    if (!scene_actor || !b.assets || !b.tables || !b.same_pf_world ||
        !b.scene_view.camera || (needs_driver_type && !b.scene_view.driver_type) ||
        !b.textures.upload ||
        !b.textures.release || !b.submit)
        return fail(error, "FX factory requires source tables, assets, PFWorld, same-scene camera/driver, texture and queue services"), nullptr;
    const auto lease = session.actor_binding_lease();
    if (lease.expired()) return fail(error, "FX factory requires a live CombatSession binding"), nullptr;
    const auto* pose = session.retained_actor_pose(scene_actor);
    const auto* visual = session.retained_actor_visual_borrow(scene_actor);
    const auto* state = session.actor(scene_actor);
    if (!pose || !visual || !state || !visual->loaded())
        return fail(error, "FX factory requires the same-session retained pose, visual and actor"), nullptr;
    const auto* scene = visual->retained_scene_borrow();
    if (!scene) return fail(error, "FX factory requires the actual retained visual Scene"), nullptr;

    try {
        auto impl = std::make_unique<Impl>(session, scene_actor, std::move(b), scene);
        if (!same_owner(session.actor_binding_lease(), impl->lease))
            return fail(error, "CombatSession binding changed while creating the FX owner"), nullptr;
        dh2::fx::MeshFxAssetsV1 assets{impl.get(), Impl::read_asset};
        dh2::fx::MeshFxServicesV1 services{impl.get(), Impl::invoke};
        impl->materials = std::make_unique<OriginalEffectMaterialBinding>(
            *impl->bindings.assets, impl->bindings.textures);
        impl->manager = std::make_unique<dh2::fx::CharacterMeshFxOwnerV4>(
            std::move(impl->bindings.tables), *impl->scene, assets, services,
            impl->resources.factory());
        if (!impl->manager->precache_libraries(error)) return nullptr;
        if (!impl->manager->precached())
            return fail(error, "Original FX library registration did not reach precached state"), nullptr;

        RuntimeCombatEffectsBindingsV1 runtime_bindings;
        runtime_bindings.render.material = [owner = impl.get()](
            const EffectDrawSource& source, const dh2::scene::Material& original,
            Material& output, std::string& why) {
            if (!owner->materials) return fail(why, "Original FX material owner expired");
            return owner->materials->bind(source, original, output, why);
        };
        runtime_bindings.render.submit = impl->bindings.submit;
        impl->runtime = std::make_unique<RuntimeCombatEffectsV1>(
            session, *impl->manager, std::move(runtime_bindings));
        error.clear();
        return std::unique_ptr<RuntimeEffectsFactoryV1>(
            new RuntimeEffectsFactoryV1(std::move(impl)));
    } catch (const std::exception& e) {
        error = e.what();
        return nullptr;
    }
}

RuntimeCombatEffectsV1& RuntimeEffectsFactoryV1::runtime() noexcept {
    return *impl_->runtime;
}
dh2::fx::CharacterMeshFxOwnerV4& RuntimeEffectsFactoryV1::manager() noexcept {
    return *impl_->manager;
}
std::size_t RuntimeEffectsFactoryV1::cached_original_textures() const noexcept {
    return impl_->materials ? impl_->materials->texture_count() : 0;
}
bool RuntimeEffectsFactoryV1::is_bound_to(const CombatSession& candidate) const noexcept {
    if(!impl_||impl_->lease.expired()||&impl_->session!=&candidate)return false;
    const auto current=candidate.actor_binding_lease();
    return !current.expired()&&same_owner(current,impl_->lease)&&
        candidate.actor(impl_->scene_actor)!=nullptr;
}
void RuntimeEffectsFactoryV1::clear_original_textures() {
    if (impl_->materials) impl_->materials->clear();
}

} // namespace dh::foundation::effects
