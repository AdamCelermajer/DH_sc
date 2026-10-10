#include "target_selection_runtime.hpp"
#include <cmath>

namespace dh::foundation { namespace {
bool finite(Vec3 v){return std::isfinite(v.x)&&std::isfinite(v.y)&&std::isfinite(v.z);}
Vec3 subtract(Vec3 a,Vec3 b){return {a.x-b.x,a.y-b.y,a.z-b.z};}
Vec3 cross(Vec3 a,Vec3 b){return {a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x};}
bool normalize(Vec3& v){const float n=std::sqrt(v.x*v.x+v.y*v.y+v.z*v.z);if(!std::isfinite(n)||n<=0)return false;v={v.x/n,v.y/n,v.z/n};return true;}
Vec3 convert(CameraVec3 v){return {v.x,v.y,v.z};}
}
bool TargetSelectionRuntime::bind(const ActorState& actor,const OriginalActorProperties& properties,
 const ActorSelectionProperties& selection,std::string& error){
    if(actor.id==invalid_actor_id||properties.faction_id!=properties.sheets.resolved[0]||
       actor.faction_id!=properties.faction_id||!finite(selection.targetOffset)||
       !std::isfinite(selection.interactionRadius)||selection.interactionRadius<0||
       !std::isfinite(selection.meleeRadius)||selection.meleeRadius<0){
        error="Selection binding has invalid identity, resolved faction, or geometry";return false;
    }
    bindings_[actor.id]={&actor,properties.faction_id,selection};error.clear();return true;
}
TargetRelation TargetSelectionRuntime::relation(const ActorState& source,const ActorState& target)const{
    const auto s=bindings_.find(source.id),t=bindings_.find(target.id);
    if(s==bindings_.end()||t==bindings_.end()||s->second.actor->id!=source.id||t->second.actor!=&target)
        return static_cast<TargetRelation>(0);
    const auto validFaction=[&](std::int32_t value){return value>=0&&static_cast<std::size_t>(value)<factions_->factions.size()?value:10;};
    const auto owner=validFaction(s->second.faction),other=validFaction(t->second.faction);
    if(static_cast<std::size_t>(owner)>=factions_->factions.size()||static_cast<std::size_t>(other)>=factions_->factions.size())
        return static_cast<TargetRelation>(0);
    for(const auto& entry:factions_->factions[owner])if(entry.id==other){
        if(entry.value<0&&!(s->second.selection.isPlayer&&t->second.selection.isPlayer))return TargetRelation::hostile;
        if(entry.value>0)return TargetRelation::friendly;
        break;
    }
    return TargetRelation::neutral;
}
TargetTraits TargetSelectionRuntime::traits(const ActorState& actor)const{
    TargetTraits result;result.visible=result.targetable=result.inAllowedZone=false;
    const auto found=bindings_.find(actor.id);
    if(found==bindings_.end()||found->second.actor!=&actor||actor.faction_id!=found->second.faction)return result;
    const auto& p=found->second.selection;
    result.visible=p.visible;result.targetable=p.targetable;result.inAllowedZone=p.inAllowedZone;
    result.capabilities=p.capabilities;result.radius=p.interactionRadius;
    result.point={actor.transform.position[0]+p.targetOffset.x,actor.transform.position[1]+p.targetOffset.y,actor.transform.position[2]+p.targetOffset.z};
    return result;
}
SelectionOutput TargetSelectionRuntime::update(ActorState& actor,const TargetRegistry& registry,
 const SelectionInput& input,const SelectionPolicy& policy,const CollisionScene* collision,const CameraPose* camera)const{
    const auto old=actor.target_id;
    SelectionOutput output;const auto found=bindings_.find(actor.id);
    if(found==bindings_.end()||found->second.actor!=&actor||actor.faction_id!=found->second.faction){
        actor.target_id=invalid_actor_id;output.changed=old!=actor.target_id;return output;
    }
    TargetingConfig config;config.range=policy.range;config.halfConeRadians=policy.halfConeRadians;
    config.sourceRadius=found->second.selection.meleeRadius;config.allowedRelations=policy.allowedRelations;
    config.requiredCapabilities=policy.requiredCapabilities;config.requireLineOfSight=policy.requireLineOfSight;
    config.allowNearestFallback=policy.nearestOnRayMiss;
    config.relation=[this](const ActorState& a,const ActorState& b){return relation(a,b);};
    config.traits=[this](const ActorState& a){return traits(a);};
    ActorState source=actor;
    if(policy.coneBasis==SelectionConeBasis::cameraView){
        if(!camera){actor.target_id=invalid_actor_id;output.changed=old!=actor.target_id;return output;}
        const Vec3 view=subtract(convert(camera->target),convert(camera->position));
        if(!finite(view)||(view.x==0&&view.y==0)){actor.target_id=invalid_actor_id;output.changed=old!=actor.target_id;return output;}
        source.transform.rotation[2]=std::atan2(view.x,-view.y);
    }
    TargetingSystem targeting;
    switch(input.command){
    case SelectionCommand::maintain:break;
    case SelectionCommand::clear:targeting.clear(source);break;
    case SelectionCommand::direct:targeting.select(source,input.directId,registry,config,collision);break;
    case SelectionCommand::cursorRay:targeting.selectRay(source,input.rayOrigin,input.rayDirection,input.rayLength,registry,config,collision);break;
    case SelectionCommand::nearest:targeting.nearest(source,registry,config,collision);break;
    case SelectionCommand::next:targeting.cycle(source,registry,config,collision);break;
    case SelectionCommand::previous:targeting.cycle(source,registry,config,collision,true);break;
    default:targeting.clear(source);break;
    }
    output.valid=targeting.refresh(source,registry,config,collision,&output.aim);
    actor.target_id=source.target_id;output.selectedId=actor.target_id;output.changed=old!=actor.target_id;
    return output;
}
bool selectionCameraRay(const CameraPose& camera,float x,float y,float aspect,Vec3& rayOrigin,Vec3& rayDirection){
    if(!std::isfinite(x)||!std::isfinite(y)||!std::isfinite(aspect)||aspect<=0||
       !std::isfinite(camera.verticalFovDegrees)||camera.verticalFovDegrees<=0||camera.verticalFovDegrees>=180)return false;
    auto origin=convert(camera.position),forward=subtract(convert(camera.target),origin),up=convert(camera.up);
    if(!finite(origin)||!finite(forward)||!finite(up)||!normalize(forward))return false;
    auto right=cross(forward,up);if(!normalize(right))return false;up=cross(right,forward);
    const float tangent=std::tan(camera.verticalFovDegrees*0.0087266462599716478846f);
    Vec3 direction{forward.x+right.x*x*tangent*aspect+up.x*y*tangent,
                   forward.y+right.y*x*tangent*aspect+up.y*y*tangent,
                   forward.z+right.z*x*tangent*aspect+up.z*y*tangent};
    if(!normalize(direction))return false;rayOrigin=origin;rayDirection=direction;return true;
}
}
