#include "character_dot_attack.hpp"
#include <cstring>

namespace dh2::character_dot_attack { namespace {
struct Range {std::uintptr_t begin,end;};
bool range(const void* p,std::size_t bytes,std::size_t alignment,Range& out) {
    const auto at=reinterpret_cast<std::uintptr_t>(p);
    if(!p||at%alignment||at>UINTPTR_MAX-bytes)return false;
    out={at,at+bytes};return true;
}
bool overlaps(Range a,Range b){return a.begin<b.end&&b.begin<a.end;}
std::int32_t signed_word(std::uint32_t value){std::int32_t result;std::memcpy(&result,&value,4);return result;}
Status invoke(const Services& services,Result* result,const Request& request,Reply& reply) {
    if(!services.invoke)return Status::service_unavailable;
    ++result->calls;result->last_operation=std::uint32_t(request.operation);reply={};
    try {if(services.invoke(services.context,&request,&reply))return Status::service_failed;}
    catch(...){return Status::service_failed;}
    return Status::complete;
}
Status debug_phase(std::uintptr_t selected,const Services& services,Result* result) {
    if(!selected)return Status::invalid_argument;
    Reply reply{};
    auto status=invoke(services,result,{Operation::debug_load,selected,0,nullptr,nullptr,{},0,0},reply);
    if(status!=Status::complete)return status;
    ++result->loads;
    status=invoke(services,result,{Operation::string_construct,0,0,"isTracingChar_Attack",nullptr,{},0,0},reply);
    if(status!=Status::complete)return status;
    const auto string=reply.identity;if(!string)return Status::service_failed;
    ++result->constructed;
    status=invoke(services,result,{Operation::debug_query,selected,string,nullptr,nullptr,{},0,0},reply);
    if(status!=Status::complete)return status;
    ++result->queries;
    status=invoke(services,result,{Operation::string_destroy,string,0,nullptr,nullptr,{},0,0},reply);
    if(status==Status::complete)++result->destroyed;
    return status;
}
}
Status execute(const Arguments* arguments,const Globals* globals,const Services* services,data::CombatResult* output,Result* result) {
    Range controls[5];
    if(!range(arguments,sizeof(*arguments),alignof(Arguments),controls[0])||
       !range(globals,sizeof(*globals),alignof(Globals),controls[1])||
       !range(services,sizeof(*services),alignof(Services),controls[2])||
       !range(output,sizeof(*output),alignof(data::CombatResult),controls[3])||
       !range(result,sizeof(*result),alignof(Result),controls[4]))return Status::invalid_argument;
    for(unsigned i=0;i<5;++i)for(unsigned j=0;j<i;++j)if(overlaps(controls[i],controls[j]))return Status::invalid_argument;
    const auto args=*arguments;const auto bound=*services;
    if(!args.attacker||!args.defender)return Status::invalid_argument;
    const auto selected=globals->debug_switches;*result={};
    auto status=debug_phase(selected,bound,result);if(status!=Status::complete)return status;
    Reply reply{};
    status=invoke(bound,result,{Operation::calculate_result,0,0,nullptr,output,args,0x20080000u,-1},reply);
    if(status==Status::complete)++result->calculated;
    return status;
}
struct Runtime::Context {
    Runtime* runtime;Storage storage;Result* result;
    Status error=Status::complete;
    debug_switches::Runtime* captured_debug;
    Services services;
};
std::int32_t Runtime::operation(void* opaque,const Request* request,Reply* reply) {
    auto& context=*static_cast<Context*>(opaque);auto& storage=context.storage;auto& owner=*context.runtime;
    switch(request->operation) {
    case Operation::debug_load:
        if(!context.captured_debug||request->subject!=context.captured_debug->identity()||
           context.captured_debug->load(*storage.debug_globals,*storage.debug_services)!=debug_switches::Status::complete) {
            context.error=Status::debug_failed;return 1;
        }return 0;
    case Operation::string_construct:
        try {auto string=std::make_unique<std::string>(request->text);const auto id=reinterpret_cast<std::uintptr_t>(string.get());
             owner.strings_.emplace(id,std::move(string));reply->identity=id;return 0;}
        catch(...){context.error=Status::service_failed;return 1;}
    case Operation::debug_query: {
        const auto found=owner.strings_.find(request->string);std::uint8_t value=0;
        if(found==owner.strings_.end()||!context.captured_debug||request->subject!=context.captured_debug->identity()||
           context.captured_debug->get_switch(*found->second,*storage.debug_globals,*storage.debug_services,value)!=debug_switches::Status::complete) {
            context.error=Status::debug_failed;return 1;
        }reply->word=value;return 0;
    }
    case Operation::string_destroy:
        if(owner.strings_.erase(request->subject)!=1){context.error=Status::service_failed;return 1;}return 0;
    case Operation::calculate_result: {
        if(request->mask!=0x20080000u||request->weapon_category!=-1){context.error=Status::calculation_failed;return 1;}
        // _F_ResetResult72B and the three argument stores precede context
        // initialization and the second mandatory Debug load/query.
        *request->output=data::CombatResult{};request->output->mask=request->mask;
        request->output->weapon_category=-1;request->output->element=request->arguments.element;
        auto& combat=*storage.combat_context;
        combat.attacker=request->arguments.attacker;combat.defender=request->arguments.defender;
        const Actor* attacker=nullptr;const Actor* defender=nullptr;
        for(std::uint32_t i=0;i<storage.actor_count;++i) {
            const auto& actor=storage.actors[i];
            if(actor.identity==combat.attacker){if(attacker){context.error=Status::actor_unavailable;return 1;}attacker=&actor;}
            if(actor.identity==combat.defender){if(defender){context.error=Status::actor_unavailable;return 1;}defender=&actor;}
        }
        Range sheet{};
        if(!attacker||!defender||!range(attacker->resolved,224*sizeof(std::int32_t),alignof(std::int32_t),sheet)||
           !range(defender->resolved,224*sizeof(std::int32_t),alignof(std::int32_t),sheet)) {
            context.error=Status::actor_unavailable;return 1;
        }
        const auto first=std::uint32_t(attacker->resolved[19]);++context.result->level_reads;
        const auto second=std::uint32_t(defender->resolved[19]);++context.result->level_reads;
        // Exact CF_SetCombatants scalar store order after its two pure cached
        // getters. Native pointer/field layout is intentionally separate.
        combat.critical=0;combat.reverse_level_delta=second-first;
        combat.element=request->arguments.element;combat.offhand=0;combat.magic=0;
        combat.level_delta=first-second;combat.blocked=0;
        context.captured_debug=storage.debug_globals->singleton;
        const auto status=debug_phase(context.captured_debug?context.captured_debug->identity():0,context.services,context.result);
        if(status!=Status::complete){if(context.error==Status::complete)context.error=status;return 1;}
        // Original direct branch freshly rereads both shared context identities
        // after Debug. With either null it writes amount0. No target lookup or
        // object dereference occurs in the CF__CalcDamage type3 subpath.
        if(!combat.attacker||!combat.defender){request->output->amount=0;return 0;}
        data::CombatantView a{attacker->resolved},d{defender->resolved};
        const data::DamageRequest calculation{&a,&d,storage.random,signed_word(request->arguments.amount),3,combat.element,0};
        data::Damage damage;
        if(dh2_combat_damage(&damage,&calculation)){context.error=Status::calculation_failed;return 1;}
        request->output->amount=damage.amount;return 0;
    }
    }
    context.error=Status::service_failed;return 1;
}
Status Runtime::attack(const Storage* storage,const Arguments* arguments,data::CombatResult* output,Result* result) {
    Range controls[8];
    if(!range(this,sizeof(*this),alignof(Runtime),controls[0])||
       !range(storage,sizeof(*storage),alignof(Storage),controls[1])||
       !range(arguments,sizeof(*arguments),alignof(Arguments),controls[2])||
       !range(output,sizeof(*output),alignof(data::CombatResult),controls[3])||
       !range(result,sizeof(*result),alignof(Result),controls[4]))return Status::invalid_argument;
    for(unsigned i=0;i<5;++i)for(unsigned j=0;j<i;++j)if(overlaps(controls[i],controls[j]))return Status::invalid_argument;
    if(busy_)return Status::busy;
    const auto captured=*storage;
    if(!captured.actor_count||captured.actor_count>65536||
       !range(captured.combat_context,sizeof(*captured.combat_context),alignof(CombatContext),controls[5])||
       !range(captured.random,sizeof(*captured.random),alignof(data::CombatRandom),controls[6])||
       !range(captured.debug_globals,sizeof(*captured.debug_globals),alignof(debug_switches::Globals),controls[7]))return Status::invalid_argument;
    for(unsigned i=5;i<8;++i)for(unsigned j=0;j<i;++j)if(overlaps(controls[i],controls[j]))return Status::invalid_argument;
    Range actors{},debug_services{};
    if(!range(captured.actors,captured.actor_count*sizeof(Actor),alignof(Actor),actors)||
       !range(captured.debug_services,sizeof(*captured.debug_services),alignof(debug_switches::Services),debug_services)||
       overlaps(actors,debug_services))return Status::invalid_argument;
    for(auto control:controls)if(overlaps(control,actors)||overlaps(control,debug_services))return Status::invalid_argument;
    // Resolved storage may be shared by self attacks. It cannot alias controls,
    // output/context/random or actor descriptors that the source writes/reads.
    for(std::uint32_t i=0;i<captured.actor_count;++i) {
        Range sheet{};
        if(!captured.actors[i].identity||!range(captured.actors[i].resolved,224*sizeof(std::int32_t),alignof(std::int32_t),sheet)||
           overlaps(sheet,actors)||overlaps(sheet,debug_services))return Status::invalid_argument;
        for(auto control:controls)if(overlaps(sheet,control))return Status::invalid_argument;
    }
    const auto bound_debug_services=*captured.debug_services;
    Context context{this,captured,result,Status::complete,captured.debug_globals->singleton,{}};
    context.storage.debug_services=&bound_debug_services;context.services={&context,operation};
    Globals globals{context.captured_debug?context.captured_debug->identity():0};
    busy_=true;struct Guard{bool& busy;~Guard(){busy=false;}}guard{busy_};
    const auto status=execute(arguments,&globals,&context.services,output,result);
    return context.error!=Status::complete?context.error:status;
}
} // namespace dh2::character_dot_attack
