#include "ais_external_initialization.hpp"

namespace dh2::ais_external_initialization { namespace {
struct Range{std::uintptr_t first,end;};
bool range(const void* p,std::size_t bytes,std::size_t alignment,Range& out) {
    const auto at=reinterpret_cast<std::uintptr_t>(p);
    if(!p || at%alignment || at>UINTPTR_MAX-bytes)return false;
    out={at,at+bytes};return true;
}
bool overlap(Range a,Range b){return a.first<b.end && b.first<a.end;}
bool valid(State* state,const Services* services,Result* result,Range (&controls)[3]) {
    if(!range(state,sizeof(*state),alignof(State),controls[0]) ||
       !range(services,sizeof(*services),alignof(Services),controls[1]) ||
       !range(result,sizeof(*result),alignof(Result),controls[2]))return false;
    for(unsigned i=0;i<3;++i)for(unsigned j=0;j<i;++j)if(overlap(controls[i],controls[j]))return false;
    return state->identity && state->binder_identity && state->path_storage_identity;
}
Status invoke(State* state,const Services& services,Result* result,const Request& request) {
    if(!services.invoke)return Status::service_unavailable;
    ++result->service_calls;result->last_operation=static_cast<std::uint32_t>(request.operation);
    try {if(services.invoke(services.context,state,&request))return Status::service_failed;}
    catch(...){return Status::service_failed;}
    return Status::complete;
}
Status construct(State* state,bool skip_bind,const Tables* tables,const Services* services,
                 Result* result,bool external) {
    Range controls[3],classes;
    if(!valid(state,services,result,controls) || !range(tables,sizeof(*tables),alignof(Tables),classes))return Status::invalid_argument;
    for(auto control:controls)if(overlap(control,classes))return Status::invalid_argument;
    if(!tables->char_ai_script || (external && !tables->ais_external))return Status::invalid_argument;
    const auto bound=*services;const auto identity=state->identity,binder=state->binder_identity;
    const auto script_table=tables->char_ai_script,external_table=tables->ais_external;
    *result={};
    auto status=invoke(state,bound,result,{Operation::lua_construct,identity,binder,std::uint32_t(skip_bind),nullptr,0});
    if(status!=Status::complete)return status;
    state->owner_98=0;state->dispatch_table=script_table;
    auto& tree=state->state_registry_9c;
    tree.parent=0;tree.color=0;tree.right=reinterpret_cast<std::uintptr_t>(&tree);
    state->current_state_b4=0;tree.left=reinterpret_cast<std::uintptr_t>(&tree);tree.count=0;
    if(!skip_bind) {
        status=invoke(state,bound,result,{Operation::bind_state_functions,identity,binder,0,nullptr,0});
        if(status!=Status::complete)return status;
    }
    if(external) {
        state->word_c0=0;state->flags_b8=0;state->dispatch_table=external_table;state->counter_bc=0;
    }
    result->returned_identity=identity;return Status::complete;
}
}
Status construct_char_ai_script(State* state,bool skip_bind,const Tables* tables,const Services* services,Result* result) {
    return construct(state,skip_bind,tables,services,result,false);
}
Status construct_external(State* state,bool skip_bind,const Tables* tables,const Services* services,Result* result) {
    return construct(state,skip_bind,tables,services,result,true);
}
Status set_character(State* state,std::uintptr_t character,const Services* services,Result* result) {
    Range controls[3];if(!valid(state,services,result,controls) || !character)return Status::invalid_argument;
    const auto bound=*services;const auto binder=state->binder_identity,path=state->path_storage_identity;
    *result={};state->owner_98=character;
    auto status=invoke(state,bound,result,{Operation::character_create_bindings,character,binder,0,nullptr,0});
    if(status!=Status::complete)return status;
    constexpr char text[]="data/scripts/ai/";
    status=invoke(state,bound,result,{Operation::path_assign,path,0,0,text,sizeof(text)-1});
    return status;
}
} // namespace dh2::ais_external_initialization
