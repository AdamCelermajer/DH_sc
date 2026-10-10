#include "source_owner_contract.hpp"
#include "../../../level-world/character_script_selection.hpp"
#include <cstring>

namespace dh::foundation::companions {
bool advance_source_companion(const LiveWorldBindings& b,ActorId id,const std::string& name,
    const dh2::character::ScriptOwnerServicesV2& services,std::string& error) {
    LiveActorOwner owner;if(!borrow_live_owner(b,id,owner,error))return false;
    const auto* props=b.world->combat_properties(id);
    const auto* row=props?dh2::data::ai_props(b.world->factions(),props->sheets.resolved[1]):nullptr;
    if(!row||(row->script!="rene"&&row->script!="follower"&&row->script!="attacking_follower")){
        error="Required actual authored external companion Script row";return false;
    }
    if(!owner.script_owner||owner.script_owner->lifecycle().owner!=owner.character_identity||!services.invoke){
        error="Required SAME existing Character ScriptOwner/lifecycle/services factory";return false;
    }
    const dh2::character::ScriptCreationFacts24 facts{static_cast<std::uint32_t>(row->script.size()),0,row->script.c_str(),name.c_str()};
    const int status=owner.script_owner->advance(facts,services);
    if(status){error="Source companion initialization failed at retained prefix: "+owner.script_owner->error();return false;}
    dh2::character::ScriptSessionView view{};
    if(owner.script_owner->active(view)) {
        if(view.kind!=dh2::character::script_external||!view.constructor_fields||view.constructor_fields->character!=owner.character_identity){
            error="Companion publication did not retain actual selected AISExternal Character receiver";return false;
        }
    }
    return true;
}
bool advance_source_companion(const LiveWorldBindings& b,ActorId id,
    dh2::character::CharacterScriptSession& session,std::string& error) {
    LiveActorOwner owner;if(!borrow_live_owner(b,id,owner,error))return false;
    const auto* props=b.world->combat_properties(id);
    const auto* row=props?dh2::data::ai_props(b.world->factions(),props->sheets.resolved[1]):nullptr;
    if(!row||(row->script!="rene"&&row->script!="follower"&&row->script!="attacking_follower")||
       session.script_name()!=row->script){error="Required actual selected external companion AI row/session";return false;}
    if(owner.script_session!=&session||owner.script_owner){error="Companion Session is not the one retained by the same Character owner";return false;}
    auto& source_owner=session.owner();
    if(source_owner.lifecycle().owner!=owner.character_identity){error="Companion V1 ScriptOwner identity differs from SAME Character";return false;}
    dh2::character::ScriptSessionView view{};
    if(!source_owner.active(view)) {
        const int status=session.start();
        if(status){error="Original CharacterScriptSession initialization failed at retained prefix: "+session.error();return false;}
        if(!source_owner.active(view)){error="Original companion Session did not publish its source ScriptOwner";return false;}
    }
    if(view.kind!=dh2::character::script_external||!view.constructor_fields||
       view.constructor_fields->character!=owner.character_identity) {
        error="Companion publication is not same-Character AISExternal";return false;
    }
    error.clear();return true;
}
bool bind_source_registration(const std::shared_ptr<LiveWorldBindings>& b,ActorId id,
    const dh2::character::ScriptOwnerRequest& request,NativeBindingOwner& owner,bool& handled,std::string& error) {
    handled=false;
    if(request.service!=dh2::character::owner_register_binding)return true;
    if(!request.binding||!request.session){error="Required actual source binding descriptor/session delivery";return false;}
    if(request.binding->method)return true;
    struct Source {const char* name;std::uint32_t address;};
    static constexpr Source source[]={{"HasMaster",0x3b6f3c},{"HasTarget",0x3b6f50},{"GetTarget",0x3b6c7c},
        {"GetHostPlayer",0x37ca60},{"IsMasterHostPlayer",0x3b6fc4},{"HasPath",0x38e98c},{"GetState",0x3b6d78},
        {"SetMaster",0x3b9084},{"SetTarget",0x3b8f38},{"ClearTarget",0x3b5690},{"MoveTo",0x3bada8},
        {"WarpTo",0x3bb268},{"WarpBehind",0x3b8d98},{"Stop",0x3b56b4},{"Attack",0x3b9f44}};
    for(const auto& entry:source)if(!std::strcmp(entry.name,request.binding->name)) {
        handled=true;
        if(entry.address!=request.binding->original_callback||request.binding->reserved){error="Companion native descriptor address differs from original source callback";return false;}
        return bind_live_native(b,id,request.session->vm,entry.name,owner,handled,error);
    }
    return true;
}
}
