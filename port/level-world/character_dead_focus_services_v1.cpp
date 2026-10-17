#include "character_dead_focus_services_v1.hpp"
#include <limits>

namespace dh2::character_dead_focus_services_v1 { namespace {
Status fail(Result& out,Phase phase,std::string& error,const char* why){out.phase=phase;if(error.empty())error=why;return Status::provider_failed;}
struct Scope {bool& busy;explicit Scope(bool& value):busy(value){busy=true;}~Scope(){busy=false;}};
bool overlaps(const void* a,std::size_t an,const void* b,std::size_t bn)noexcept{
    if(!a||!b||!an||!bn)return false;
    const auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);
    return x<=y?y-x<an:x-y<bn;
}
}
bool Adapter::valid()const noexcept {
    const auto& b=bindings_;
    return b.character&&b.properties&&b.buffs&&b.buffs->character_identity()==b.character&&
        b.byte415&&b.self_fx&&b.state_fx&&b.highlight&&b.self_fx!=b.state_fx&&
        b.self_fx!=b.highlight&&b.state_fx!=b.highlight&&dh2_property_validate(b.properties)==0;
}
bool Adapter::output_aliases(const Result* out,const std::string& error)const noexcept{
    const auto& b=bindings_;
    auto hit=[&](const void* p,std::size_t size){return overlaps(out,sizeof(*out),p,size);};
    if(hit(this,sizeof(*this))||hit(&error,sizeof(error))||hit(b.properties,sizeof(*b.properties))||
       hit(b.buffs,sizeof(*b.buffs))||hit(b.byte415,1)||hit(b.self_fx,sizeof(*b.self_fx))||
       hit(b.state_fx,sizeof(*b.state_fx))||hit(b.highlight,sizeof(*b.highlight))||
       hit(b.debug_globals,sizeof(debug_switches::Globals))||
       hit(b.debug_services,sizeof(debug_switches::Services))||
       hit(b.preparation,sizeof(character_player_skills_preparation_v3::Owner))||
       hit(b.skill_calls,sizeof(player_skill_use_session_v1::Runtime)))return true;
    const auto& v=*b.properties;
    const std::int32_t* sheets[]={v.defaults,v.types,v.base,v.saved,v.gear,v.resolved};
    for(const auto* sheet:sheets)
        if(hit(sheet,224*sizeof(std::int32_t)))return true;
    if(hit(v.groups,std::size_t(v.group_count)*sizeof(data::PropertyBuffGroup)))return true;
    for(std::uint32_t n=0;n<v.group_count;++n){
        const auto& g=v.groups[n];
        if(hit(g.sheets,std::size_t(g.count)*sizeof(*g.sheets)))return true;
        for(std::uint32_t j=0;j<g.count;++j)if(hit(g.sheets[j],224*sizeof(std::int32_t)))return true;
    }
    return false;
}
bool Adapter::handles(character::Service service)noexcept {
    using namespace character;
    return service==dead_focus_prelude||service==character::cancel_sneaking||service==remove_highlight||
        service==disable_state_fx||service==disable_self_fx||service==remove_buffs;
}
Status Adapter::focus_prelude(Result* output,std::string& error){
    return deliver(character::dead_focus_prelude,output,error);
}
Status Adapter::drop(std::uintptr_t* field,bool highlight,Result& out,std::string& error){
    if(!*field)return Status::complete; // Exact source null-handle branch.
    out.phase=Phase::drop_fx;
    if(!bindings_.services.drop_fx)return fail(out,Phase::drop_fx,error,"Nonnull Character FX requires its actual manager");
    ++out.calls;
    if(bindings_.services.drop_fx(bindings_.services.context,*field,error))return fail(out,Phase::drop_fx,error,"Character FX drop failed");
    if(highlight)*field=0; // Additional source RemoveMultiplayerHighlight write.
    return Status::complete;
}
Status Adapter::cancel_sneaking(Result& out,std::string& error){
    auto& b=bindings_;out.phase=Phase::is_player;
    if(!b.services.is_player)return fail(out,Phase::is_player,error,"Character IsPlayer provider missing");
    ++out.calls;
    if(b.services.is_player(b.services.context,b.character,&out.player,error))return fail(out,Phase::is_player,error,"Character IsPlayer failed");
    if(out.player){
        out.phase=Phase::delete_sneak_buff;++out.calls;
        if(b.buffs->remove(146,0,&out.buff)!=character_player_buffs_v1::Status::complete)
            return fail(out,Phase::delete_sneak_buff,error,"Source DelBuff(146,NULL) failed");
        out.phase=Phase::mark_byte415;*b.byte415=1;
    }
    out.phase=Phase::read_sneak;out.sneak=b.properties->resolved[198];
    if(out.sneak<=0)return Status::complete;
    out.phase=Phase::list;
    if(!b.skills)return fail(out,Phase::list,error,"Positive Sneak requires actual SkillTables");
    const auto selector=b.properties->resolved[28];
    const std::size_t selected=selector<0||std::uint32_t(selector)>=b.skills->skill_lists.size()?3u:std::size_t(selector);
    if(selected>=b.skills->skill_lists.size())return fail(out,Phase::list,error,"Source SkillList fallback3 unavailable");
    const auto* const list=&b.skills->skill_lists[selected];
    if(list->members.size()>std::uint32_t(std::numeric_limits<std::int32_t>::max()))return fail(out,Phase::list,error,"SkillList native size unsupported");
    for(std::size_t slot=0;slot<list->members.size();++slot){
        out.phase=Phase::skill_row;const auto id=list->members[slot];
        if(id<0||std::uint32_t(id)>=b.skills->skills.size())return fail(out,Phase::skill_row,error,"SkillList row unavailable");
        if(!(std::uint32_t(b.skills->skills[id].flags)&0x02000000u))continue;
        out.selected_slot=std::int32_t(slot);out.phase=Phase::script;
        if(!b.preparation)return fail(out,Phase::script,error,"Sneak script vector unavailable");
        using List=character_ai_set_skills_and_spells::List;
        if(b.preparation->state().owner!=b.character)return fail(out,Phase::script,error,"Sneak preparation Character differs");
        const auto& scripts=b.preparation->slots(List::skill);
        if(slot>=scripts.size())return fail(out,Phase::script,error,"Sneak script slot unavailable");
        if(!scripts[slot])return Status::complete;
        // AI_CancelSkill re-fetches the live owner SkillList after script test.
        const auto again=b.properties->resolved[28];
        const std::size_t which=again<0||std::uint32_t(again)>=b.skills->skill_lists.size()?3u:std::size_t(again);
        if(which>=b.skills->skill_lists.size()||slot>=b.skills->skill_lists[which].members.size())return fail(out,Phase::skill_row,error,"Reloaded Sneak SkillList unavailable");
        const auto row=b.skills->skill_lists[which].members[slot];
        if(row<0||std::uint32_t(row)>=b.skills->skills.size())return fail(out,Phase::skill_row,error,"Reloaded Sneak skill unavailable");
        if(b.skills->skills[row].type!=1)return Status::complete;
        if(!b.skill_calls)return fail(out,Phase::active,error,"Sneak Active/Pre VM provider missing");
        player_skill_use_session_v1::Result called{};out.phase=Phase::active;++out.calls;
        if(b.skill_calls->check(List::skill,std::uint32_t(slot),character_ai_skill_script_check::Check::active,called,error))return fail(out,Phase::active,error,"Sneak Active failed");
        out.active=called.value;
        if(!out.active)return Status::complete;
        out.phase=Phase::pre;++out.calls;
        if(b.skill_calls->invoke(List::skill,std::uint32_t(slot),player_skill_use_session_v1::Callback::pre,called,error))return fail(out,Phase::pre,error,"Sneak Pre failed");
        out.pre_delivered=1;return Status::complete;
    }
    return Status::complete;
}
Status Adapter::deliver(character::Service service,Result* output,std::string& error){
    if(busy_)return Status::busy;
    if(!output||!handles(service)||!valid()||output_aliases(output,error))return Status::invalid_argument;
    *output={};error.clear();Scope scope(busy_);auto& out=*output;auto& b=bindings_;
    try{
        Status status=Status::complete;
        using namespace character;
        switch(service){
        case dead_focus_prelude:{
            if(!b.debug_globals||!b.debug_services||!b.debug_globals->singleton)return fail(out,Phase::debug_load,error,"Dead focus Debug owner missing");
            // Source captures Debug singleton once before both explicit loads.
            auto* const debug=b.debug_globals->singleton;
            for(const char* key:{"isTracingCharState","isTracingCSDead"}){
                out.phase=Phase::debug_load;++out.calls;
                if(debug->load(*b.debug_globals,*b.debug_services)!=debug_switches::Status::complete)return fail(out,Phase::debug_load,error,"Dead focus Debug load failed");
                out.phase=Phase::debug_query;++out.calls;std::uint8_t value=0;
                if(debug->get_switch(key,*b.debug_globals,*b.debug_services,value)!=debug_switches::Status::complete)return fail(out,Phase::debug_query,error,"Dead focus Debug query failed");
            }break;
        }
        case character::cancel_sneaking:status=cancel_sneaking(out,error);break;
        case remove_highlight:status=drop(b.highlight,true,out,error);break;
        case disable_state_fx:status=drop(b.state_fx,false,out,error);break;
        case disable_self_fx:status=drop(b.self_fx,false,out,error);break;
        case remove_buffs:
            out.phase=Phase::remove_buffs;++out.calls;
            if(b.buffs->remove_all(&out.buff)!=character_player_buffs_v1::Status::complete)
                status=fail(out,Phase::remove_buffs,error,"Source RemoveAllBuffs failed");
            break;
        default:return Status::invalid_argument;
        }
        if(status==Status::complete)out.phase=Phase::complete;
        return status;
    }catch(...){if(error.empty())error="Dead focus provider exception; source effects retained";return Status::provider_failed;}
}
}
