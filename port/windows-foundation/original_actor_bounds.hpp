#pragma once
#include "asset_catalog.hpp"
#include "original_pose_blend.hpp"
#include "renderer.hpp"
#include "../level-world/character_scene.hpp"
#include <memory>
namespace dh::foundation {
// Explicit original component binding: the provider group ID is authored data.
// No implicit armor slot/class selection or controller-name heuristics.
struct ActorBoundsComponent { std::string controller_id, group_node_id; };
struct ActorBoundsConfig {
    std::string model_path;
    // Empty uses source visible instances. Nonempty replaces all skinned
    // instances with exactly this selected modular component set.
    std::vector<ActorBoundsComponent> components;
};
struct ActorBoundsPlacement {
    Vec3 position, rotation_degrees;
    std::array<std::int32_t,3> base_scale{}; // source properties12/13/14
    std::int32_t collision_scale=0; // resolved property16, never a radius
    std::uint8_t previous_flat=0;
};
struct OriginalActorBoundsResult {
    std::array<float,6> mesh_box{}, relative_box{}, absolute_box{};
    bool marker=false;
    std::uint32_t flat=0, pf_updates=0;
};
class OriginalActorBounds {
public:
    OriginalActorBounds(); ~OriginalActorBounds();
    OriginalActorBounds(OriginalActorBounds&&) noexcept;
    OriginalActorBounds& operator=(OriginalActorBounds&&) noexcept;
    bool load(const AssetCatalog&,const ActorBoundsConfig&,std::string& error);
    // Null pose means original factory/rest cache. Non-null uses explicitly
    // supplied cached authored local SRT, with outer owner TRS applied once.
    // This is a producer invocation, not a promise source refreshes every frame.
    bool calculate(const ActorBoundsPlacement&,const SkeletalPose* cached_pose,
                   OriginalActorBoundsResult&,std::string& error) const;
    static bool update_absolute(OriginalActorBoundsResult&,Vec3 position,std::string& error);
private: struct Impl; std::unique_ptr<Impl> impl_;
};
}
