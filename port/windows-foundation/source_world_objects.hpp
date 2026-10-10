#pragma once
#include "actor_definitions.hpp"
#include "actor_state.hpp"
#include "camera.hpp"
#include <functional>
#include <optional>

namespace dh::foundation {
// Authored objects plus explicit constructor-derived room bindings. A loader
// occurrence suffix is an identity, never evidence of the original runtime ID.
class SourceWorldObjects {
public:
    bool load(const std::vector<ActorDefinition>&,std::string& error);
    bool bind_module(const std::string& occurrence,int original_module_id,std::string& error);
    bool named_character(const std::string&,int module,ActorId&,bool& found,std::string& error)const;
    bool named_object(const std::string&,int module,ActorId&,bool& found,std::string& error)const;
    bool module_context(ActorId,int& original_module_id,std::string& error)const;
    using ActorAnchor=std::function<bool(ActorId,CameraVec3&,std::string&)>;
    void bind_actor_anchor(ActorAnchor provider){actor_anchor_=std::move(provider);}
    bool anchor(ActorId,CameraVec3&,std::string& error)const;
    const ActorDefinition* definition(ActorId)const noexcept;
private:
    std::map<ActorId,ActorDefinition> objects_;
    std::map<std::string,int> modules_;
    ActorAnchor actor_anchor_;
};
}
