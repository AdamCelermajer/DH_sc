#include "npc_menu_services.hpp"
#include <utility>
namespace dh::foundation::interactions {
namespace {
bool valid(const NpcMenuReceiverBorrow& b,std::string& e){
    if(!b.receiver||!b.renderfx||!b.movie){e="Character.Interact requires SAME retained RenderFX/loaded SWF movie";return false;}return true;
}
struct Invocation {const char* path;const char* method;const std::vector<NpcAsValue>* arguments;};
bool apply(void* raw,dh2::ui::SwfAsGraph& graph,std::string& e){
    const auto& invocation=*static_cast<Invocation*>(raw);std::vector<dh2::ui::SwfAsValue> arguments;
    arguments.reserve(invocation.arguments->size());
    for(const auto& value:*invocation.arguments){
        if(value.kind==NpcAsValue::Kind::string)arguments.push_back(dh2::ui::SwfAsValue::text(value.string));
        else arguments.push_back(dh2::ui::SwfAsValue::number(value.number));
    }
    return graph.invoke_renderfx(invocation.path,invocation.method,arguments,e);
}
}
bool invoke_npc_menu_as(const NpcMenuReceiverBorrow& b,const char* path,const char* method,
 const std::vector<NpcAsValue>& values,std::string& e){
    if(!valid(b,e))return false;
    if(!path||!method){e="Character.Interact requires authored AS path/method";return false;}
    Invocation invocation{path,method,&values};
    // Opens the SAME source movie facade Scope. Typed strings/numbers preserve
    // GameSWF tags, object environment and reverse argument-stack convention.
    return b.movie->menu_action_script(&invocation,apply,e);
}
bool bind_npc_menu_services(NpcInteractServices& out,NpcMenuOwnerServices source,std::string& e){
    if(!source.manager){e="Character.Interact requires existing native MenuManager owner";return false;}
    auto roots=[source](bool merchant,std::uintptr_t& id,std::shared_ptr<void>& lease,std::string& error){
        const auto& getter=merchant?source.merchant_root:source.hud_root;
        if(!getter){error=merchant?"Character.Interact requires source MenuManager f4->138 root":"Character.Interact requires source MenuManager.GetHUDRoot";return false;}
        NpcMenuReceiverBorrow borrow;if(!getter(borrow,error)||!valid(borrow,error))return false;
        id=borrow.renderfx;lease=borrow.receiver;return true;
    };
    out.hud_root=[roots](auto& id,auto& lease,std::string& error){return roots(false,id,lease,error);};
    out.merchant_root=[roots](auto& id,auto& lease,std::string& error){return roots(true,id,lease,error);};
    out.player_info_id=[source](std::uintptr_t actor,std::int32_t& id,std::string& error){
        if(!source.player_info){error="Character.Interact requires PlayerManager.GetPlayerByCharacter(false)";return false;}
        NpcPlayerInfoBorrow borrow;if(!source.player_info(actor,borrow,error))return false;
        if(!borrow.receiver||!borrow.identity||!borrow.id678){error="Character.Interact requires SAME produced PlayerInfo678";return false;}
        id=*borrow.id678;return true;
    };
    out.invoke_as=[source](std::uintptr_t renderfx,const char* path,const char* method,const std::vector<NpcAsValue>& values,std::string& error){
        if(!source.renderfx){error="Character.Interact requires existing captured RenderFX movie resolver";return false;}
        NpcMenuReceiverBorrow borrow;if(!source.renderfx(renderfx,borrow,error))return false;
        if(borrow.renderfx!=renderfx){error="Character.Interact captured RenderFX identity mismatch";return false;}
        return invoke_npc_menu_as(borrow,path,method,values,error);
    };return true;
}
}
