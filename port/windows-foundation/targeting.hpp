#pragma once
#include "actor_state.hpp"
#include "collision_scene.hpp"
#include <functional>

namespace dh::foundation {
enum class TargetRelation : std::uint32_t { friendly=1, neutral=2, hostile=4 };
struct TargetTraits {
    bool visible=true, targetable=true, inAllowedZone=true;
    std::uint32_t capabilities=0;
    float radius=0;
    // May differ from the feet/world origin, e.g. an authored target marker.
    Vec3 point;
};
struct TargetingConfig {
    float range=0;
    float halfConeRadians=3.14159265358979323846f;
    float sourceRadius=0;
    std::uint32_t allowedRelations=static_cast<std::uint32_t>(TargetRelation::hostile);
    std::uint32_t requiredCapabilities=0;
    bool requireLineOfSight=false;
    bool allowNearestFallback=false;
    // Content provides these rules; different faction IDs do not imply enemies.
    // Callbacks must be deterministic and must not mutate the registry.
    std::function<TargetRelation(const ActorState&,const ActorState&)> relation;
    std::function<TargetTraits(const ActorState&)> traits;
};
struct TargetAim {
    ActorId id=invalid_actor_id;
    Vec3 point, direction;
    float facingRadians=0;
    float edgeDistance=0;
};
// Borrowed registry pointers must remain live for each synchronous call.
using TargetRegistry = std::vector<const ActorState*>;
class TargetingSystem {
public:
    bool eligible(const ActorState& source,const ActorState& target,
                  const TargetingConfig&,const CollisionScene*,TargetAim* =nullptr) const;
    bool select(ActorState& source,ActorId,const TargetRegistry&,
                const TargetingConfig&,const CollisionScene* =nullptr) const;
    bool selectRay(ActorState& source,Vec3 rayOrigin,Vec3 rayDirection,float rayLength,
                   const TargetRegistry&,const TargetingConfig&,const CollisionScene* =nullptr) const;
    bool nearest(ActorState& source,const TargetRegistry&,const TargetingConfig&,
                 const CollisionScene* =nullptr) const;
    bool cycle(ActorState& source,const TargetRegistry&,const TargetingConfig&,
               const CollisionScene* =nullptr,bool backwards=false) const;
    bool refresh(ActorState& source,const TargetRegistry&,const TargetingConfig&,
                 const CollisionScene* =nullptr,TargetAim* =nullptr) const;
    void clear(ActorState& source) const noexcept { source.target_id=invalid_actor_id; }
};
}
