#include "player_ais_lifecycle_v1.hpp"
#include "character_script_selection.hpp"
#include "ais_external_init_callbacks.hpp"
#include <cstring>
#include <stdexcept>

namespace dh2::player_ais_lifecycle_v1 { namespace {
namespace c=character;
namespace ext=ais_external_initialization;
struct Range {std::uintptr_t b,e;};
template<class T>bool range(const T* p,Range& r){const auto n=reinterpret_cast<std::uintptr_t>(p);if(!p||n%alignof(T)||n>UINTPTR_MAX-sizeof(T))return false;r={n,n+sizeof(T)};return true;}
bool overlap(Range a,Range b){return a.b<b.e&&b.b<a.e;}
std::int32_t signed_word(std::uint32_t n){std::int32_t r;std::memcpy(&r,&n,4);return r;}
struct Failure {};
struct Call {
    Bindings& b;Result& out;std::string& error;
    c::ScriptLifecycleState64 state{};
    explicit Call(Bindings& binding,Result& result,std::string& message):b(binding),out(result),error(message){pull();}
    void pull(){auto& a=*b.ai;state={a.owner_04,a.active_ais_1c,a.alternate_ais_20,a.script_name_30,
        signed_word(std::uint32_t(a.pointer_28)),signed_word(a.word_10),signed_word(a.word_14),a.byte_24,a.byte_2c,0,0,0};}
    void push(){auto& a=*b.ai;a.owner_04=state.owner;a.active_ais_1c=state.active;a.alternate_ais_20=state.pending;
        a.script_name_30=state.external_name;a.pointer_28=std::uint32_t(state.load_step);a.word_10=std::uint32_t(state.timer33);
        a.word_14=std::uint32_t(state.timer34);a.byte_24=std::uint8_t(state.delayed);a.byte_2c=std::uint8_t(state.scripted);}
    [[noreturn]]void fail(const char* why){if(error.empty())error=why;throw Failure{};}
    player_skill_session_v1::Session& session(){
        auto* s=*b.session_slot;
        if(!s||s->character_identity()!=state.owner||s->ais_identity()!=b.callback_flags->ais||s->ais_identity()!=b.fields->script.identity)
            fail("Player AIS Session identity differs");
        return *s;
    }
    void backend(Operation operation,std::uint32_t allocation=0,std::uint32_t skip=0){
        if(!b.backend.invoke)fail("Player AIS backend unavailable");
        const Request request{operation,state.owner,b.fields->script.identity,allocation,skip};
        if(b.backend.invoke(b.backend.context,&request,error))fail("Player AIS backend failed");
    }
    static std::int32_t construct_service(void* raw,ext::State* fields,const ext::Request* q){
        auto& s=*static_cast<Call*>(raw);
        if(fields!=&s.b.fields->script||!q)return 1;
        if(q->operation==ext::Operation::lua_construct){
            if(q->argument!=1)return 1;
            s.backend(Operation::construct_vm,0xd8,1);
            return s.session().stage()==player_skill_session_v1::Stage::created?0:1;
        }
        if(q->operation==ext::Operation::character_create_bindings){
            if(q->subject!=s.state.owner)return 1;
            return s.session().bind_character_functions(s.error);
        }
        if(q->operation==ext::Operation::path_assign){
            // Session binds the Character source globals, then assigns this
            // actual string in that same binding operation. Verify its result.
            return q->text&&s.session().script_path()==std::string(q->text,q->text_bytes)?0:1;
        }
        return 1;
    }
    void construct(){
        const auto table=*b.tables;
        const ext::Tables base{table.char_ai_script,0};const ext::Services provider{this,construct_service};
        if(ext::construct_char_ai_script(&b.fields->script,true,&base,&provider,&out.constructor)!=ext::Status::complete)
            fail("Player CharAIScript constructor dependency failed");
        // SetScript<AISPlayerIPhone> 3cd06c..3cd098. Empty vector construction
        // invokes its zero-count allocator branch, which allocates nothing.
        b.callback_flags->flags_b8=0;b.fields->script.flags_b8=0;
        b.fields->script.counter_bc=0;b.fields->script.word_c0=0;
        b.fields->script.dispatch_table=table.ais_player;
        b.fields->vector_begin=b.fields->vector_end=b.fields->vector_capacity=0;
        b.fields->skill_d4=b.fields->skill_d0=0;
        b.fields->script.dispatch_table=table.ais_player_iphone;
    }
    static void selected(void* raw,c::ScriptSelectionState16*,std::uint32_t kind){
        auto& s=*static_cast<Call*>(raw);if(kind!=c::script_player_iphone)s.fail("Authored Player AIS kind differs");
        const c::ScriptLifecycleServices16 nested{&s,dispatch};
        if(dh2_character_script_lifecycle(&s.state,c::script_replace_iphone,0,&nested)!=1)s.fail("Player AIS factory failed");
    }
    static void dispatch(void* raw,c::ScriptLifecycleState64* state,const c::ScriptLifecycleRequest32* q,c::ScriptLifecycleResponse16* response){
        auto& s=*static_cast<Call*>(raw);if(state!=&s.state||!q||!response)s.fail("Player AIS lifecycle controls differ");
        // Providers observe each completed source store; their scalar effects
        // are read back at the kernel's subsequent original reload points.
        s.push();++s.out.service_calls;s.out.last_service=q->service;
        try{s.invoke(*q,*response);}catch(...){s.pull();throw;}
        s.pull();
    }
    void invoke(const c::ScriptLifecycleRequest32& q,c::ScriptLifecycleResponse16& response){
        const c::ScriptLifecycleServices16 nested{this,dispatch};
        switch(q.service){
        case c::script_create_step:{
            const auto& row=*b.declaration;
            c::ScriptSelectionState16 selection{state.external_name,state.scripted,0};
            const c::ScriptCreationFacts24 facts{std::uint32_t(row.script.size()),0,row.script.c_str(),b.owner_name};
            const c::ScriptSelectionServices16 services{this,selected};
            if(dh2_character_script_create_step(&selection,&facts,&services)!=1)fail("Player AIS selection failed");
            state.external_name=selection.external_name;state.scripted=selection.scripted;push();break;
        }
        case c::script_construct_iphone:construct();response.identity=b.fields->script.identity;break;
        case c::script_ai_terminate:case c::script_destroy:fail("Player AIS replacement requires its source termination owner");
        case c::script_bind_functions:
            if(q.subject!=b.fields->script.identity||session().bind_ais_functions(error))fail("Player AIS function binding failed");
            break;
        case c::script_set_character:{
            if(q.subject!=b.fields->script.identity||q.payload!=state.owner)fail("Player AIS Character association differs");
            const ext::Services provider{this,construct_service};ext::Result result{};
            if(ext::set_character(&b.fields->script,q.payload,&provider,&result)!=ext::Status::complete)fail("Player AIS SetCharacter failed");
            break;
        }
        case c::script_load_common:{
            player_skill_session_v1::LoadResult result{};
            if(session().load_resolved("data/scripts/ai/_commons.luac",&result,error))fail("Player AIS commons provider failed");
            break;
        }
        case c::script_load_external:fail("Builtin Player unexpectedly reached external script loading");
        case c::script_ai_init:
            if(dh2_character_script_lifecycle(&state,c::script_on_init,0,&nested)!=1)fail("Player CharAI OnInit failed");
            push();break;
        case c::script_pending_init_vcb:{
            if(q.subject!=b.fields->script.identity)fail("Player pending InitVCB identity differs");
            ais_player_init_vcb::Result result{};
            if(session().initialize_vcb(&result,error))fail("Player pending InitVCB failed");
            break;
        }
        case c::script_owner_is_character:response.word=1;break; // This borrowed owner is an actual Character.
        case c::script_owner_is_dead:response.word=*b.dead!=0;break;
        case c::script_query_budget:fail("Player Character reached noncharacter load budget");
        case c::script_timer_stop:
            if(q.subject!=state.owner||b.coordinator->stop_timer(q.argument0)<0)fail("Player AIS timer stop failed");
            break;
        case c::script_design_tick:{
            const char* key=q.argument0==0x33?"AI_Tick":q.argument0==0x34?"DoT_Tick":nullptr;
            dh2_pycst_result result{};
            if(!key||dh2_pycst_get(b.design,"CharacterDesign",15,key,std::strlen(key),&result)||!result.found)fail("Player AIS design tick missing");
            response.word=std::uint32_t(result.value);break;
        }
        case c::script_timer_start:{
            if(q.subject!=state.owner||(q.argument1!=0x33&&q.argument1!=0x34))fail("Player AIS timer request differs");
            const auto id=b.coordinator->start_timer(q.argument0,-1,std::int32_t(q.argument1),0);
            if(id< -1)fail("Player AIS timer storage unavailable");
            response.word=std::uint32_t(id);break;
        }
        case c::script_ais_init:ais_external_init_callbacks::default_init();break;
        case c::script_ais_init_post:ais_external_init_callbacks::default_post();break;
        case c::script_ais_init_final:ais_external_init_callbacks::default_final();break;
        case c::script_refresh_vitals:
            out.init_phase_mask|=1u;
            if(b.vitals->initialize_hp_mp(b.vitals_storage,&out.vitals)!=character_level_runtime::Status::complete)fail("Player initial HP/MP failed");
            break;
        case c::script_configure_skills:{
            out.init_phase_mask|=2u;backend(Operation::configure_skills);
            auto* owner=*b.preparation_slot;
            if(!owner||owner->state().owner!=state.owner||owner->state().ai!=state.active||!(*b.update_slot)||!(*b.use_slot))fail("Player prepared native owners differ");
            break;
        }
        case c::script_update_skills:
            out.init_phase_mask|=4u;
            if(!*b.update_slot||(*b.update_slot)->update(out.update,error))fail("Player initial skill update failed");
            break;
        case c::script_ai_init_post:
            out.init_phase_mask|=8u;
            if(dh2_character_script_lifecycle(&state,c::script_on_init_post,0,&nested)!=1)fail("Player InitPost failed");
            push();break;
        case c::script_ai_init_final:
            out.init_phase_mask|=16u;
            if(dh2_character_script_lifecycle(&state,c::script_on_init_final,0,&nested)!=1)fail("Player InitFinal failed");
            push();break;
        default:fail("Player AIS lifecycle dependency unavailable");
        }
    }
};
bool valid(const Bindings& b){
    if(!b.ai||!b.fields||!b.callback_flags||!b.tables||!b.declaration||!b.owner_name||!b.session_slot||!b.preparation_slot||!b.update_slot||!b.use_slot||!b.savegame||!b.coordinator||!b.dead||!b.design||!b.vitals||!b.vitals_storage)return false;
    return b.ai->identity&&b.ai->owner_04&&b.ai->owner_04==b.coordinator->owner()&&b.coordinator->bound()&&
        b.fields->script.identity&&b.fields->script.binder_identity&&b.fields->script.path_storage_identity&&
        b.callback_flags->ais==b.fields->script.identity&&b.tables->char_ai_script&&b.tables->ais_player&&b.tables->ais_player_iphone&&
        b.declaration->script=="__player__"&&b.declaration->delayed_load<=255&&b.ai->pointer_28<=0x7fffffffu&&
        b.savegame->character()==b.ai->owner_04&&b.vitals_storage->character==b.ai->owner_04;
}
bool separate(const Bindings& b,Range output){
    const auto clear=[&](const auto* p){Range control;return range(p,control)&&!overlap(output,control);};
    if(!clear(b.ai)||!clear(b.fields)||!clear(b.callback_flags)||!clear(b.tables)||!clear(b.declaration)||
       !clear(b.session_slot)||!clear(b.preparation_slot)||!clear(b.update_slot)||!clear(b.use_slot)||
       !clear(b.savegame)||!clear(b.coordinator)||!clear(b.dead)||!clear(b.design)||!clear(b.vitals)||!clear(b.vitals_storage)||
       !clear(b.vitals_storage->view))return false;
    const auto& view=*b.vitals_storage->view;
    for(const auto* sheet:{view.defaults,view.types,view.base,static_cast<const std::int32_t*>(view.saved),view.gear,static_cast<const std::int32_t*>(view.resolved)}){
        const auto begin=reinterpret_cast<std::uintptr_t>(sheet);
        if(!sheet||begin%alignof(std::int32_t)||begin>UINTPTR_MAX-896||overlap(output,{begin,begin+896}))return false;
    }
    return (!*b.session_slot||clear(*b.session_slot))&&(!*b.preparation_slot||clear(*b.preparation_slot))&&
        (!*b.update_slot||clear(*b.update_slot))&&(!*b.use_slot||clear(*b.use_slot));
}
}
Runtime::Runtime(Bindings bindings):bindings_(bindings){if(!valid(bindings_))throw std::invalid_argument("Invalid borrowed Player AIS lifecycle owners");}
Status Runtime::load(Result* out,std::string& error){return execute(character::script_load_process,0,out,error);}
Status Runtime::initialize_process(std::uint32_t final,Result* out,std::string& error){return execute(character::script_init_process,final,out,error);}
Status Runtime::initialize(std::uint32_t final,Result* out,std::string& error){return execute(character::script_load_and_init,final,out,error);}
Status Runtime::execute(character::ScriptLifecycleOperation operation,std::uint32_t final,Result* out,std::string& error){
    if(busy_)return Status::busy;
    Range r,t,e;
    if(final>1||!valid(bindings_)||!range(out,r)||!range(this,t)||!range(&error,e)||
       overlap(r,t)||overlap(e,t)||overlap(e,r)||!separate(bindings_,r)||!separate(bindings_,e))return Status::invalid_argument;
    if(failed_)return Status::failed;
    *out={};error.clear();busy_=true;
    struct Scope{bool& busy;~Scope(){busy=false;}} scope{busy_};
    Call call(bindings_,*out,error);
    // DelayedLoad is the actual immutable AI declaration, applied only before
    // the first source load phase. A retained initialized AIS is never replayed.
    if(operation!=character::script_init_process&&!call.state.active&&!call.state.pending&&call.state.load_step==0){call.state.delayed=bindings_.declaration->delayed_load;call.push();}
    try{
        const character::ScriptLifecycleServices16 services{&call,Call::dispatch};
        out->source_return=dh2_character_script_lifecycle(&call.state,operation,final,&services);
        call.push();
        if(out->source_return<0){failed_=true;error="Player source lifecycle rejected controls";return Status::failed;}
        return Status::complete;
    }catch(...){call.push();failed_=true;if(error.empty())error="Player AIS lifecycle provider exception";return Status::failed;}
}
}
