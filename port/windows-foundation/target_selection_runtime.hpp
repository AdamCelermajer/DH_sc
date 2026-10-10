#pragma once
#include "targeting.hpp"
#include "original_actor_properties.hpp"
#include "camera.hpp"
#include "../game-data/ai.hpp"
#include <map>

namespace dh::foundation {
struct ActorSelectionProperties {
    // Supplied by actor construction/visibility/targetability owners.
    bool isPlayer=false, visible=false, targetable=false, inAllowedZone=false;
    std::uint32_t capabilities=0;
    float interactionRadius=0, meleeRadius=0;
    Vec3 targetOffset;
};
enum class SelectionCommand { maintain, clear, direct, cursorRay, nearest, next, previous };
enum class SelectionConeBasis { actorHeading, cameraView };
struct SelectionInput {
    SelectionCommand command=SelectionCommand::maintain;
    ActorId directId=invalid_actor_id;
    Vec3 rayOrigin,rayDirection;
    float rayLength=0;
};
struct SelectionPolicy {
    float range=0, halfConeRadians=3.14159265358979323846f;
    std::uint32_t allowedRelations=static_cast<std::uint32_t>(TargetRelation::hostile);
    std::uint32_t requiredCapabilities=0;
    bool requireLineOfSight=false, nearestOnRayMiss=false;
    SelectionConeBasis coneBasis=SelectionConeBasis::actorHeading;
};
struct SelectionOutput {
    ActorId selectedId=invalid_actor_id;
    bool changed=false, valid=false;
    TargetAim aim;
};
// Faction tables and actors are borrowed. Call bind again after property changes;
// remove/clear bindings before actors are destroyed. No visual or input ownership.
class TargetSelectionRuntime {
public:
    explicit TargetSelectionRuntime(const dh2::data::AiTables& factions):factions_(&factions){}
    bool bind(const ActorState&,const OriginalActorProperties&,
              const ActorSelectionProperties&,std::string& error);
    void remove(ActorId id) { bindings_.erase(id); }
    void clearBindings() { bindings_.clear(); }
    SelectionOutput update(ActorState&,const TargetRegistry&,const SelectionInput&,
                           const SelectionPolicy&,const CollisionScene* =nullptr,
                           const CameraPose* =nullptr) const;
private:
    struct Binding { const ActorState* actor=nullptr; std::int32_t faction=-1; ActorSelectionProperties selection; };
    std::map<ActorId,Binding> bindings_;
    const dh2::data::AiTables* factions_;
    TargetRelation relation(const ActorState&,const ActorState&) const;
    TargetTraits traits(const ActorState&) const;
};
// Cursor coordinates are NDC: (-1,-1) bottom-left, (+1,+1) top-right.
// Uses the actual right-handed view basis, not a walking-plane approximation.
bool selectionCameraRay(const CameraPose&,float ndcX,float ndcY,float aspect,
                        Vec3& origin,Vec3& direction);
}
