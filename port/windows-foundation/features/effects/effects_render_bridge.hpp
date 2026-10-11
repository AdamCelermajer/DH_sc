#pragma once
#include "effects_executor.hpp"
#include "../../renderer.hpp"
#include "../../retained_animation_owner.hpp"
#include "../../../level-world/character_particle_fx_resource_v2.hpp"
#include <functional>
#include <map>

namespace dh::foundation::effects {
enum class EffectDrawKind { authored_mesh, authored_particle };
struct EffectDrawSource {
    EffectDrawKind kind{};
    std::uintptr_t fx{}, node{}, particle{};
    std::uint32_t material{}, primitive{}, rendering_layer{}, camera_offset_word{};
    std::string resource_uri;
    std::shared_ptr<const std::vector<std::uint8_t>> resource_bytes;
    dh2::resources::BresView image{};
    const dh2::scene::Scene* scene{};
    const dh2::math::Matrix4f* source_texture_matrix{};
    std::optional<std::uintptr_t> source_owner;
    bool source_color_missing{}, source_normal_missing{};
};
struct EffectRenderPacket {
    Mesh mesh;
    Mat4 world{};
    EffectDrawSource source;
    // Pins the SAME source graph/material/skin snapshots until queue flush.
    std::shared_ptr<const void> source_retention;
};
struct EffectRenderFrame { std::vector<EffectRenderPacket> packets; };
struct EffectRenderServices {
    // Receives the actual animated material and BRES. Must resolve its selected
    // original shader/pass and decode/upload the original texture. No generic
    // additive/alpha heuristic is admitted in place of that actual pass.
    std::function<bool(const EffectDrawSource&, const dh2::scene::Material&,
                       Material&, std::string&)> material;
    // Root composes SAME SceneManager layer/priority/distance ordering with all
    // other scene nodes. Keep this frame alive until Renderer/RenderQueue flush.
    std::function<bool(std::shared_ptr<const EffectRenderFrame>, std::string&)> submit;
};
// Converts already baked/skinned original streams to the root Renderer format.
// It never builds billboards, ticks a cloud, changes alpha or reapplies UVs.
// P17 FIREWALL: an authored FX resource owned by a scene visual (door, decor, object) rather than by the
// CharacterMeshFxOwnerV4 set manager. Its particle and mesh sources join the same frame and the same
// material/texture services. uri is the model path the resource came from (texture resolution base).
struct EmbeddedFxSourceV1 {
    std::shared_ptr<dh2::fx::CharacterParticleFxResourceV2> resource;
    std::string uri;
    bool include_meshes = true;
};

class EffectsRenderBridge {
public:
    EffectsRenderBridge(dh2::fx::CharacterMeshFxOwnerV4& manager, EffectRenderServices services)
        : manager_(manager), services_(std::move(services)) {}
    bool prepare(std::shared_ptr<const EffectRenderFrame>&, std::string& error) const;
    bool prepare(std::shared_ptr<const EffectRenderFrame>&, const std::vector<EmbeddedFxSourceV1>& embedded,
                 std::string& error) const;
    bool submit(std::string& error) const;
    // Also usable for an embedded actor's SAME retained particle/mesh receivers.
    static bool convert(const dh2::skinning::VisualDrawPartV6&, std::uint32_t primitive,
                        std::uint32_t material, EffectDrawSource,
                        const EffectRenderServices&, EffectRenderPacket&, std::string&);
private:
    dh2::fx::CharacterMeshFxOwnerV4& manager_;
    EffectRenderServices services_;
    // P17 FIREWALL: bound material per (scene FX resource, material id). The pass, texture and alpha state are
    // constant; only the colour follows the authored track each frame, so it is refreshed on every hit.
    mutable std::map<std::string, Material> embedded_material_cache_;
};

struct RetainedEffectBindings {
    // Resolve actual source actor/subobject, target position and rotation from
    // the retained scene/body. This must not make another actor/pose receiver.
    std::function<bool(std::uintptr_t actor, ActorAttachment&, std::string&)> attachment;
};
class RetainedEffectsAdapter {
public:
    RetainedEffectsAdapter(dh2::fx::CharacterMeshFxOwnerV4& manager, RetainedEffectBindings bindings)
        : manager_(manager), executor_(manager), bindings_(std::move(bindings)) {}
    // Host first executes actor event0x28, then its original named/root forwarding.
    // The ordinal is the occurrence in that SAME detached audiovisual batch.
    DispatchResult event(std::uintptr_t actor, const RetainedAnimationEvent&,
                         std::uint64_t occurrence_ordinal, std::string& error);
    EffectsExecutor& executor() noexcept { return executor_; }
    // Interruption preserves source effects. Death/release detaches original
    // anchors; unload requires a quiescent manager and drained render-frame loans.
    void release_actor(std::uintptr_t actor, std::uintptr_t nullable_socket=0);
private:
    using Key=std::tuple<std::uintptr_t,std::string,std::uint32_t,std::uint64_t,
                         std::uint32_t,std::int32_t,std::uint64_t>;
    dh2::fx::CharacterMeshFxOwnerV4& manager_;
    EffectsExecutor executor_;
    RetainedEffectBindings bindings_;
    std::set<Key> delivered_;
};
}
