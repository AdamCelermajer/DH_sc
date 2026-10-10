#include "source_owner_contract.hpp"

namespace dh::foundation::companions {
bool project_source_owner(const SourceOwnerBorrow& b,LiveActorOwner& out,std::string& error) {
    if(!b.lease||!b.character_identity||!b.actor||!b.ai_fields||!b.runtime){error="Required SAME retained Character/CharAI/runtime cells and lease";return false;}
    if(b.runtime->object.user!=b.character_identity){error="Source PF receiver identity differs from SAME Character";return false;}
    out={};out.lease=b.lease;out.actor=b.actor;out.ai_fields=b.ai_fields;
    out.path=&b.runtime->path;out.character_identity=b.character_identity;out.script_owner=b.script_owner;
    out.script_session=b.script_session;out.target_bindings=b.target_bindings;
    if(out.script_owner&&out.script_session){error="Companion owner must borrow the one actual Character ScriptOwner generation";return false;}
    if(out.target_bindings&&(!out.target_bindings->state||!out.target_bindings->state->owner||
       out.target_bindings->state->owner->identity!=out.character_identity)){
        error="Companion TargetBindings do not borrow the same Character target receiver";return false;
    }
    return true;
}
bool borrow_companion_scene(const LiveWorldBindings& b,ActorId id,const dh2::scene::Scene*& out,std::string& error) {
    LiveActorOwner owner;if(!borrow_live_owner(b,id,owner,error))return false;
    if(!b.population){error="Required SAME source ActorPopulation visual owner";return false;}
    for(const auto& actor:b.population->actors())if(actor.definition.stableId==id) {
        const auto* scene=actor.visual.retained_scene_borrow();
        if(!scene){error="SAME companion CharacterVisual has no loaded retained Scene";return false;}
        out=scene;return true;
    }
    error="Companion has no actual retained population visual";return false;
}
}
