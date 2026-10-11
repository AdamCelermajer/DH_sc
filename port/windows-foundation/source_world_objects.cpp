#include "source_world_objects.hpp"
#include <cmath>
#include <set>
namespace dh::foundation {
bool SourceWorldObjects::load(const std::vector<ActorDefinition>& definitions,std::string& error){
    std::map<ActorId,ActorDefinition> staged;
    for(const auto& d:definitions){
        if(d.stableId==invalid_actor_id||!staged.emplace(d.stableId,d).second){error="Invalid or duplicate source world object identity";return false;}
        for(float f:d.placement)if(!std::isfinite(f)){error="Nonfinite source world object placement";return false;}
    }
    objects_=std::move(staged);modules_.clear();error.clear();return true;
}
bool SourceWorldObjects::bind_module(const std::string& occurrence,int id,std::string& error){
    if(occurrence.empty()||id<0){error="Source module binding requires an occurrence and original nonnegative ID";return false;}
    bool exists=false;for(const auto& item:objects_)exists|=item.second.moduleName==occurrence;
    if(!exists){error="Unknown source module occurrence";return false;}
    const auto previous=modules_.find(occurrence);
    if(previous!=modules_.end()&&previous->second!=id){error="Source module occurrence already has a different runtime ID";return false;}
    for(const auto& item:modules_)if(item.first!=occurrence&&item.second==id){error="Original runtime module ID already belongs to another occurrence";return false;}
    modules_[occurrence]=id;error.clear();return true;
}
bool SourceWorldObjects::named_object(const std::string& name,int module,ActorId& out,bool& found,std::string& error)const{
    if(name.empty()||module< -1){error="Invalid original object name or module context";return false;}
    ActorId candidate=invalid_actor_id;bool unresolved=false;
    for(const auto& item:objects_){
        const auto& object=item.second;if(object.name!=name)continue;
        if(module>=0){
            // Original level context publishes room-1 outside module loads.
            if(object.moduleName.empty())continue;
            const auto binding=modules_.find(object.moduleName);
            if(binding==modules_.end()){unresolved=true;continue;}
            if(binding->second!=module)continue;
        }
        if(candidate!=invalid_actor_id){error="Ambiguous source object name; original object-list ordering provider is required";return false;}
        candidate=item.first;
    }
    if(unresolved){error="Named source object requires its constructor-derived module binding";return false;}
    out=candidate;found=candidate!=invalid_actor_id;error.clear();return true;
}
bool SourceWorldObjects::named_character(const std::string& name,int module,ActorId& out,bool& found,std::string& error)const{
    ActorId candidate;bool exists;if(!named_object(name,module,candidate,exists,error))return false;
    out=exists&&objects_.at(candidate).gametype=="Character"?candidate:invalid_actor_id;
    found=out!=invalid_actor_id;return true;
}
const ActorDefinition* SourceWorldObjects::definition(ActorId id)const noexcept{
    const auto i=objects_.find(id);return i==objects_.end()?nullptr:&i->second;
}
bool SourceWorldObjects::module_context(ActorId id,int& out,std::string& error)const{
    const auto* object=definition(id);
    if(!object){error="Source context requires a known authored object";return false;}
    if(object->moduleName.empty()){out=-1;error.clear();return true;}
    const auto binding=modules_.find(object->moduleName);
    if(binding==modules_.end()){error="Source context requires its actual Module constructor binding";return false;}
    out=binding->second;error.clear();return true;
}
bool SourceWorldObjects::anchor(ActorId id,CameraVec3& out,std::string& error)const{
    const auto* object=definition(id);if(!object){error="Unknown source camera object";return false;}
    if(object->gametype=="Character"){
        if(!actor_anchor_){error="Live Character camera anchor provider is required";return false;}
        CameraVec3 value;if(!actor_anchor_(id,value,error))return false;
        if(!std::isfinite(value.x)||!std::isfinite(value.y)||!std::isfinite(value.z)){error="Nonfinite live Character anchor";return false;}
        out=value;error.clear();return true;
    }
    // P16 CINE: IDA GameObject::GetCameraAnchorPosition (0x003943b8) returns the object's own position
    // (this+352) unless an auxiliary anchor node is attached (this+184; not decoded here). Any authored
    // non-Character object (Dummy, OpenableContainer, Waypoint...) therefore uses its placement translation.
    out={object->placement[12],object->placement[13],object->placement[14]};error.clear();return true;
}
}
