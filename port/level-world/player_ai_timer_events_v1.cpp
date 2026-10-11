#include "player_ai_timer_events_v1.hpp"
#include <cstring>
#include <stdexcept>

namespace dh2::player_ai_timer_events_v1 { namespace {
namespace c=character;namespace rg=character_regeneration;namespace rt=character_regen_tick_v1;namespace dt=character_dot_tick;
struct Range{std::uintptr_t b,e;};
template<class T>bool range(const T* p,Range& r){const auto a=reinterpret_cast<std::uintptr_t>(p);if(!p||a%alignof(T)||a>UINTPTR_MAX-sizeof(T))return false;r={a,a+sizeof(T)};return true;}
bool overlap(Range a,Range b){return a.b<b.e&&b.b<a.e;}
std::int32_t signed_word(std::uint32_t n){std::int32_t out;std::memcpy(&out,&n,4);return out;}
bool valid(const Bindings& b){return b.ai&&b.ai->identity&&b.ai->owner_04&&b.coordinator&&b.coordinator->bound()&&
    b.ai->owner_04==b.coordinator->owner()&&b.object&&b.object->identity==b.ai->owner_04&&
    b.controller&&b.properties&&b.view&&b.view->resolved&&(bool(b.dead)!=bool(b.dead_byte))&&b.debug_globals&&b.debug_services;}
bool separate(const Bindings& b,Range r){
    const auto clear=[&](const auto* p){Range control;return range(p,control)&&!overlap(control,r);};
    if(!clear(b.ai)||!clear(b.coordinator)||!clear(b.object)||!clear(b.view)||
       (b.dead&&!clear(b.dead))||(b.dead_byte&&!clear(b.dead_byte))||
       !clear(b.debug_globals)||!clear(b.debug_services))return false;
    for(const auto* sheet:{b.view->defaults,b.view->types,b.view->base,static_cast<const std::int32_t*>(b.view->saved),b.view->gear,static_cast<const std::int32_t*>(b.view->resolved)}){
        const auto at=reinterpret_cast<std::uintptr_t>(sheet);if(!sheet||at%4||at>UINTPTR_MAX-896||overlap(r,{at,at+896}))return false;
    }
    return (!b.dot_attack||clear(b.dot_attack))&&(!b.dot_storage||clear(b.dot_storage));
}
}
struct Runtime::Call {
    Runtime& runtime;Bindings& b;Result& out;std::string& error;
    const std::uintptr_t character;const std::int32_t* const resolved;
    debug_switches::Runtime* debug=nullptr;debug_switches::Services debug_services{};
    Call(Runtime& r,Result& result,std::string& why):runtime(r),b(r.bindings_),out(result),error(why),
        character(b.ai->owner_04),resolved(b.view->resolved){}
    int fail(const char* why){if(error.empty())error=why;return 1;}
    bool coherent()const{return b.ai->owner_04==character&&b.coordinator->owner()==character&&b.view->resolved==resolved;}
    int debug_operation(rg::Operation operation,std::uintptr_t subject,std::uintptr_t string,const char* text,rg::Reply* reply){
        switch(operation){
        case rg::Operation::debug_load:
            return debug&&subject==debug->identity()&&debug->load(*b.debug_globals,debug_services)==debug_switches::Status::complete?0:fail("Player tick Debug load failed");
        case rg::Operation::string_construct:{
            auto p=std::make_unique<std::string>(text);const auto id=reinterpret_cast<std::uintptr_t>(p.get());runtime.strings_.emplace(id,std::move(p));reply->identity=id;return 0;
        }
        case rg::Operation::debug_query:{
            const auto found=runtime.strings_.find(string);std::uint8_t value=0;
            if(found==runtime.strings_.end()||!debug||subject!=debug->identity()||debug->get_switch(*found->second,*b.debug_globals,debug_services,value)!=debug_switches::Status::complete)return fail("Player tick Debug query failed");
            reply->word=value;return 0;
        }
        case rg::Operation::string_destroy:
            return runtime.strings_.erase(subject)==1?0:fail("Player tick string destruction failed");
        default:return fail("Player tick Debug operation differs");
        }
    }
    void select_debug(){debug=b.debug_globals->singleton;debug_services=*b.debug_services;}
    static int regeneration(void* raw,const rg::Request* q,rg::Reply* r){
        auto& s=*static_cast<Call*>(raw);if(!q||!r||!s.coherent())return s.fail("Player regeneration backing changed");
        if(q->operation==rg::Operation::read_property){
            if(q->subject!=s.b.properties||q->sheet!=reinterpret_cast<std::uintptr_t>(s.resolved)||q->property>=224)return s.fail("Player cached property request differs");
            r->word=std::uint32_t(s.resolved[q->property]);return 0;
        }
        if(q->operation==rg::Operation::add_property){
            if(q->subject!=s.b.properties||dh2_property_add(s.b.view,q->property,signed_word(q->amount)))return s.fail("Player source property add failed");
            return 0;
        }
        return s.debug_operation(q->operation,q->subject,q->sheet,q->text,r);
    }
    int regenerate(bool mana,std::uint32_t amount){
        select_debug();const rg::State state{character,b.properties,reinterpret_cast<std::uintptr_t>(resolved)};
        const rg::Globals globals{debug?debug->identity():0};const rg::Services services{this,regeneration};
        const auto status=mana?rg::regen_mp(&state,&globals,amount,&services,&out.mp):rg::regen_hp(&state,&globals,amount,&services,&out.hp);
        return status==rg::Status::complete?0:fail("Player source regeneration failed");
    }
    static int regen_tick(void* raw,const rt::Request* q,rt::Reply* r){
        auto& s=*static_cast<Call*>(raw);if(!q||!r||!s.coherent())return s.fail("Player RegenTick backing changed");
        switch(q->operation){
        case rt::Operation::debug_load:return s.debug_operation(rg::Operation::debug_load,q->subject,0,nullptr,r);
        case rt::Operation::string_construct:return s.debug_operation(rg::Operation::string_construct,0,0,q->text,r);
        case rt::Operation::debug_query:return s.debug_operation(rg::Operation::debug_query,q->subject,q->sheet,nullptr,r);
        case rt::Operation::string_destroy:return s.debug_operation(rg::Operation::string_destroy,q->subject,0,nullptr,r);
        case rt::Operation::read_property:{const rg::Request read{rg::Operation::read_property,q->subject,q->sheet,q->property,0,nullptr};return regeneration(raw,&read,r);}
        case rt::Operation::regen_hp:case rt::Operation::regen_mp:
            if(q->subject!=s.character)return s.fail("Player RegenTick actor differs");
            return s.regenerate(q->operation==rt::Operation::regen_mp,q->amount);
        }
        return s.fail("Player RegenTick dependency unavailable");
    }
    int update_regen(){
        object_update_culling::RemoteResult remote{};
        if(object_update_culling::is_remotely_updated(b.object,&remote)!=object_update_culling::Status::complete)return fail("Player remote virtual failed");
        out.remote_word=remote.raw;
        if(remote.raw){out.regen_skipped=1;return 0;}
        // Actual AI_HasAggro/AI_IsAggroed count leaves, then fresh SM state
        // predicates5/6/7 in source short-circuit order. No guessed combat flag.
        out.in_combat=b.ai->tree_7c.count||b.ai->tree_94.count||
            b.coordinator->state.current==5||b.coordinator->state.current==6||b.coordinator->state.current==7;
        select_debug();const rt::State state{character,b.properties,reinterpret_cast<std::uintptr_t>(resolved)};
        const rt::Globals globals{debug?debug->identity():0};const rt::Services services{this,regen_tick};
        return rt::tick(&state,&globals,out.in_combat,&services,&out.regen)==rt::Status::complete?0:fail("Player RegenTick failed");
    }
    static int dot_read(void* raw,const dt::ReadRequest* q,std::uint32_t* value){
        auto& s=*static_cast<Call*>(raw);if(!q||!value||!s.coherent()||q->properties!=s.b.properties||q->sheet!=reinterpret_cast<std::uintptr_t>(s.resolved)||q->property>=224)return s.fail("Player DoT cached backing differs");
        *value=std::uint32_t(s.resolved[q->property]);return 0;
    }
    static int dot_dead(void* raw,std::uintptr_t actor,std::uint32_t* value){
        auto& s=*static_cast<Call*>(raw);if(actor!=s.character||!value||!s.coherent())return s.fail("Player DoT actor differs");
        *value=s.b.dead?*s.b.dead:std::uint32_t(*s.b.dead_byte);return 0;
    }
    static int dot_attack(void* raw,const dt::AttackRequest* q){
        auto& s=*static_cast<Call*>(raw);if(!q||!s.coherent()||q->attacker!=s.character||q->defender!=s.character)return s.fail("Player DoT attack actors differ");
        if(!s.b.dot_attack||!s.b.dot_storage)return s.fail("Player F_DotAttack provider unavailable");
        const character_dot_attack::Arguments args{q->attacker,q->defender,q->amount,q->element};
        return s.b.dot_attack->attack(s.b.dot_storage,&args,q->result,&s.out.attack)==character_dot_attack::Status::complete?0:s.fail("Player F_DotAttack failed");
    }
    static int dot_apply(void* raw,const dt::ApplyRequest* q){
        auto& s=*static_cast<Call*>(raw);if(!q||!s.coherent()||q->attacker!=s.character||q->defender!=s.character)return s.fail("Player DoT application actors differ");
        if(!s.b.apply.invoke)return s.fail("Player F_ApplyResult provider unavailable");
        return s.b.apply.invoke(s.b.apply.context,q,s.error);
    }
    static int dispatch(void* raw,c::AIEventState64*,const c::AIEventRequest40* q,std::uint32_t*){
        auto& s=*static_cast<Call*>(raw);if(!q||q->service!=c::ai_event_helper||!s.coherent())return s.fail("Player tick source dispatch differs");
        if(q->event==0x33&&q->operation==0x3cb77c&&q->subject==s.b.ai->identity)return s.update_regen();
        if(q->event==0x34&&q->operation==0x3df3f0&&q->subject==s.b.properties){
            dt::Owner owner{s.character};const dt::State state{s.b.properties,reinterpret_cast<std::uintptr_t>(s.resolved),&owner};
            const dt::Services services{&s,dot_read,dot_dead,dot_attack,dot_apply};
            return dt::tick(&state,&services,&s.out.dots)==dt::Status::complete?0:s.fail("Player HandleDots failed");
        }
        return s.fail("Player timer source helper unavailable");
    }
};
Runtime::Runtime(Bindings bindings):bindings_(bindings){if(!valid(bindings_))throw std::invalid_argument("Invalid borrowed Player tick owners");}
Status Runtime::deliver(std::int32_t event,const c::Timer32* timer,Result* out,std::string& error){
    if(busy_)return Status::busy;
    Range r,e,t,v;
    if((event!=0x33&&event!=0x34)||!valid(bindings_)||!range(out,r)||!range(&error,e)||!range(this,t)||!range(timer,v)||
       overlap(r,e)||overlap(r,t)||overlap(e,t)||overlap(r,v)||overlap(e,v)||!separate(bindings_,r)||!separate(bindings_,e))return Status::invalid_argument;
    const auto& store=bindings_.coordinator->timers();bool present=false;
    for(std::uint32_t i=0;i<store.count;++i){Range slot;range(store.slots+i,slot);if(overlap(r,slot)||overlap(e,slot))return Status::invalid_argument;present|=timer==store.slots+i;}
    if(!present)return Status::invalid_argument;
    *out={};out->event=std::uint32_t(event);error.clear();busy_=true;struct Scope{bool& busy;~Scope(){busy=false;}} scope{busy_};
    Call call(*this,*out,error);
    c::AIEventOwner48 owner{bindings_.ai->owner_04,bindings_.controller,reinterpret_cast<std::uintptr_t>(&bindings_.coordinator->state),bindings_.properties,0,0,0,0};
    c::AIEventState64 state{bindings_.ai->identity,&owner,nullptr,bindings_.ai->active_ais_1c,nullptr,bindings_.ai->paused_18,0,0,0,0,0};
    const c::AIEventPayload24 payload{reinterpret_cast<std::uintptr_t>(timer),0,0,0};
    const c::AIEventServices24 services{&call,Call::dispatch,1u<<c::ai_event_helper,0};
    try{if(dh2_character_ai_event(&out->dispatch,&state,event,&payload,&services))return Status::failed;return Status::complete;}
    catch(...){if(error.empty())error="Player tick provider exception";return Status::failed;}
}
}
