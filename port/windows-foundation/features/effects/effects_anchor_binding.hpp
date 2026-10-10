#pragma once
#include "effects_executor.hpp"
#include <functional>
#include <map>

namespace dh::foundation::effects {
struct EffectAnchorSample {
    bool dead{}, disabled{}, stationary{};
    std::array<float,3> source_position{}, source_rotation{}, source_scale{};
};
struct EffectAnchorBorrow {
    std::shared_ptr<void> lifetime;
    // Reads the SAME retained source actor/subobject fields each request.
    // Source rotation is radians, not decomposed rendered Euler angles.
    std::function<bool(EffectAnchorSample&,std::string&)> read;
};
// Identity registry only, with no actor fields, pose, movement state or clock.
// Root source actor/socket producers register their actual identity borrowers.
class EffectsAnchorBinding {
public:
    explicit EffectsAnchorBinding(std::function<void(std::uintptr_t)> source_detach={})
        : detach_(std::move(source_detach)) {}
    void bind_manager(dh2::fx::CharacterMeshFxOwnerV4& manager) {
        detach_=[&manager](std::uintptr_t identity) { manager.detach_anchor_v117(identity); };
    }
    bool bind(std::uintptr_t identity,EffectAnchorBorrow,std::string& error);
    bool invoke(dh2::fx::MeshFxRequestV1&,bool& handled,std::string& error) const;
    void release(std::uintptr_t identity) noexcept;
    void clear() noexcept;
private:
    std::function<void(std::uintptr_t)> detach_;
    std::map<std::uintptr_t,EffectAnchorBorrow> anchors_;
};
}
