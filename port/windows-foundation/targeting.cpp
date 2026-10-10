#include "targeting.hpp"
#include <algorithm>
#include <cmath>
#include <limits>

namespace dh::foundation { namespace {
Vec3 difference(Vec3 a,Vec3 b){return {a.x-b.x,a.y-b.y,a.z-b.z};}
float dot(Vec3 a,Vec3 b){return a.x*b.x+a.y*b.y+a.z*b.z;}
bool finite(Vec3 p){return std::isfinite(p.x)&&std::isfinite(p.y)&&std::isfinite(p.z);}
Vec3 origin(const ActorState& actor){return {actor.transform.position[0],actor.transform.position[1],actor.transform.position[2]};}
const ActorState* find(ActorId id,const TargetRegistry& registry){
    for(const auto* actor:registry)if(actor&&actor->id==id)return actor;
    return nullptr;
}
struct Candidate {const ActorState* actor;TargetAim aim;};
std::vector<Candidate> candidates(const TargetingSystem& system,const ActorState& source,
 const TargetRegistry& registry,const TargetingConfig& config,const CollisionScene* collision){
    std::vector<Candidate> result;
    for(const auto* actor:registry){TargetAim aim;
        if(actor&&system.eligible(source,*actor,config,collision,&aim)&&
           std::none_of(result.begin(),result.end(),[&](const Candidate& c){return c.actor->id==actor->id;}))
            result.push_back({actor,aim});
    }
    std::sort(result.begin(),result.end(),[](const Candidate& a,const Candidate& b){
        return a.aim.edgeDistance==b.aim.edgeDistance?a.actor->id<b.actor->id:a.aim.edgeDistance<b.aim.edgeDistance;
    });
    return result;
}
}
bool TargetingSystem::eligible(const ActorState& source,const ActorState& target,
 const TargetingConfig& config,const CollisionScene* collision,TargetAim* out)const{
    if(!source.alive()||source.action==CharacterAction::dead||!target.alive()||target.action==CharacterAction::dead||
       source.id==invalid_actor_id||target.id==invalid_actor_id||source.id==target.id||
       !config.relation||!config.traits||!std::isfinite(config.range)||config.range<0||
       !std::isfinite(config.sourceRadius)||config.sourceRadius<0||
       !std::isfinite(config.halfConeRadians)||config.halfConeRadians<0)return false;
    const auto traits=config.traits(target);
    if(!traits.visible||!traits.targetable||!traits.inAllowedZone||
       !finite(traits.point)||!std::isfinite(traits.radius)||traits.radius<0||
       (traits.capabilities&config.requiredCapabilities)!=config.requiredCapabilities||
       !(config.allowedRelations&static_cast<std::uint32_t>(config.relation(source,target))))return false;
    const auto from=origin(source);if(!finite(from))return false;
    const auto delta=difference(traits.point,from);
    const float distance=std::sqrt(dot(delta,delta));
    if(!std::isfinite(distance))return false;
    const float edge=distance-traits.radius-config.sourceRadius;
    if(edge>config.range)return false;
    const float heading=source.transform.rotation[2];if(!std::isfinite(heading))return false;
    // Recovered source heading convention in the XY world plane.
    const Vec3 forward{std::sin(heading),-std::cos(heading),0};
    if(distance>0&&config.halfConeRadians<3.14159265358979323846f&&
       std::acos(std::clamp(dot(delta,forward)/distance,-1.0f,1.0f))>config.halfConeRadians)return false;
    if(config.requireLineOfSight){
        if(!collision)return false;
        FloorHit hit;
        if(collision->raycast(from,traits.point,hit)&&
           dot(difference(hit.point,from),difference(hit.point,from))+0.0001f<distance*distance)return false;
    }
    if(out){out->id=target.id;out->point=traits.point;out->edgeDistance=edge;
        out->direction=distance>0?Vec3{delta.x/distance,delta.y/distance,delta.z/distance}:forward;
        out->facingRadians=(delta.x!=0||delta.y!=0)?std::atan2(delta.x,-delta.y):heading;}
    return true;
}
bool TargetingSystem::select(ActorState& source,ActorId id,const TargetRegistry& registry,
 const TargetingConfig& config,const CollisionScene* collision)const{
    const auto* actor=find(id,registry);
    if(actor&&eligible(source,*actor,config,collision)){source.target_id=id;return true;}
    clear(source);return false;
}
bool TargetingSystem::nearest(ActorState& source,const TargetRegistry& registry,
 const TargetingConfig& config,const CollisionScene* collision)const{
    const auto available=candidates(*this,source,registry,config,collision);
    source.target_id=available.empty()?invalid_actor_id:available.front().actor->id;
    return !available.empty();
}
bool TargetingSystem::cycle(ActorState& source,const TargetRegistry& registry,
 const TargetingConfig& config,const CollisionScene* collision,bool backwards)const{
    const auto available=candidates(*this,source,registry,config,collision);
    if(available.empty()){clear(source);return false;}
    auto found=std::find_if(available.begin(),available.end(),[&](const Candidate& c){return c.actor->id==source.target_id;});
    std::size_t index=backwards?available.size()-1:0;
    if(found!=available.end()){
        const auto current=static_cast<std::size_t>(found-available.begin());
        index=backwards?(current+available.size()-1)%available.size():(current+1)%available.size();
    }
    source.target_id=available[index].actor->id;return true;
}
bool TargetingSystem::refresh(ActorState& source,const TargetRegistry& registry,
 const TargetingConfig& config,const CollisionScene* collision,TargetAim* aim)const{
    if(aim)*aim={};
    const auto* target=find(source.target_id,registry);
    if(target&&eligible(source,*target,config,collision,aim))return true;
    clear(source);return false;
}
bool TargetingSystem::selectRay(ActorState& source,Vec3 from,Vec3 direction,float rayLength,
 const TargetRegistry& registry,const TargetingConfig& config,const CollisionScene* collision)const{
    const float magnitude=std::sqrt(dot(direction,direction));
    if(!finite(from)||!finite(direction)||!std::isfinite(rayLength)||rayLength<=0||!std::isfinite(magnitude)||magnitude<=0){clear(source);return false;}
    direction={direction.x/magnitude,direction.y/magnitude,direction.z/magnitude};
    float nearestHit=rayLength;bool blocked=false;
    if(collision){FloorHit hit;Vec3 end{from.x+direction.x*rayLength,from.y+direction.y*rayLength,from.z+direction.z*rayLength};
        if(collision->raycast(from,end,hit)){nearestHit=std::sqrt(dot(difference(hit.point,from),difference(hit.point,from)));blocked=true;}}
    ActorId selected=invalid_actor_id;
    for(const auto& candidate:candidates(*this,source,registry,config,collision)){
        const auto traits=config.traits(*candidate.actor);
        const auto offset=difference(from,candidate.aim.point);
        const float b=dot(offset,direction),c=dot(offset,offset)-traits.radius*traits.radius;
        const float discriminant=b*b-c;if(discriminant<0)continue;
        const float entry=c<=0?0:-b-std::sqrt(discriminant);
        if(entry<0||entry>nearestHit||(blocked&&entry>=nearestHit)||
           (entry==nearestHit&&selected!=invalid_actor_id&&candidate.actor->id>selected))continue;
        nearestHit=entry;selected=candidate.actor->id;blocked=false;
    }
    source.target_id=selected;
    if(selected!=invalid_actor_id)return true;
    return config.allowNearestFallback?nearest(source,registry,config,collision):false;
}
}
