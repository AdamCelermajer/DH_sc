#include "character_mana_services_v1.hpp"
#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

namespace dh2::character_mana_services_v1 { namespace {
struct Range { std::uintptr_t begin, end; };
bool range(const void* pointer, std::size_t bytes, std::size_t alignment, Range& out) {
    const auto begin=reinterpret_cast<std::uintptr_t>(pointer);
    if(!pointer || !alignment || begin%alignment || begin>UINTPTR_MAX-bytes)return false;
    out={begin,begin+bytes};return true;
}
bool overlap(Range a,Range b){return a.begin<b.end&&b.begin<a.end;}
bool disjoint(const Range* rows,std::size_t count){for(std::size_t i=0;i<count;++i)for(std::size_t j=0;j<i;++j)if(overlap(rows[i],rows[j]))return false;return true;}

struct Active { std::uintptr_t character; const void* properties; const void* result; };
thread_local std::vector<Active> active;
class ActiveGuard {
public:
    ActiveGuard(const State& state,const Result* result):key_{state.character,state.properties,result} {
        for(const auto& row:active)if(row.character==key_.character||row.properties==key_.properties||row.result==key_.result)return;
        active.push_back(key_);entered_=true;
    }
    ~ActiveGuard(){if(entered_)active.pop_back();}
    bool entered()const{return entered_;}
private:
    Active key_{};bool entered_=false;
};

Status service_status(std::int32_t value){
    if(!value)return Status::complete;
    return Status::service_failed;
}

struct Kernel {
    State state;
    Services services;
    Result& out;
    // UseMana keeps the singleton it loaded in the original sl register for
    // both DebugSwitches pairs. The shared Globals slot may change while a
    // provider runs; that does not retarget this caller's receiver.
    debug_switches::Runtime* debug_owner=nullptr;

    Status call(Operation operation,std::uintptr_t subject,const char* text,Reply& reply){
        if(!services.invoke)return Status::service_unavailable;
        ++out.service_calls;out.last_service=operation;reply={};
        const Request request{operation,subject,text};
        try{return service_status(services.invoke(services.context,&request,&reply));}
        catch(...){return Status::service_failed;}
    }

    Status read_online(bool& remote){
        Reply reply{};auto status=call(Operation::get_online,0,nullptr,reply);
        if(status!=Status::complete)return status;
        ++out.online_queries;out.last_online_identity=reply.identity;out.last_online_byte=reply.word;
        if(!reply.identity||reply.word>255)return Status::invalid_source_result;
        if(reply.word==0){remote=false;return Status::complete;}
        status=call(Operation::is_remotely_updated,state.character,nullptr,reply);
        if(status!=Status::complete)return status;
        ++out.remote_queries;out.last_remote_word=reply.word;remote=reply.word!=0;
        return Status::complete;
    }

    Status has_mana_impl(std::int32_t amount,bool& value){
        ++out.has_mana_calls;
        bool remote=false;auto status=read_online(remote);if(status!=Status::complete)return status;
        if(remote){value=true;return Status::complete;}
        // _GetProperty(41) is a raw read of Character's current resolved-sheet
        // word. Do not recalculate the property or consult an alternate sheet.
        const auto current=state.properties->resolved[41];out.mana_before=current;
        value=current>=amount;return Status::complete;
    }

    Status debug_switch(const Globals& globals,const char* key,bool& value){
        if(!globals.debug_globals||!globals.debug_services)return Status::service_unavailable;
        auto& shared=*globals.debug_globals;
        if(!debug_owner)debug_owner=shared.singleton;
        auto* const selected=debug_owner;
        if(!selected||!selected->identity())return Status::service_unavailable;
        ++out.debug_loads;
        auto status=selected->load(shared,*globals.debug_services);
        out.debug_status=static_cast<std::int32_t>(status);
        if(status!=debug_switches::Status::complete)return status==debug_switches::Status::service_unavailable?Status::service_unavailable:Status::service_failed;

        std::uint8_t raw=0;++out.debug_queries;
        status=selected->get_switch(std::string(key),shared,*globals.debug_services,raw);
        out.debug_status=static_cast<std::int32_t>(status);
        if(status!=debug_switches::Status::complete)return status==debug_switches::Status::service_unavailable?Status::service_unavailable:Status::service_failed;
        value=raw!=0;return Status::complete;
    }
};

bool valid_common(const State* state,const Services* services,bool* result,Result* details,
                 Range* ranges,std::size_t& range_count){
    if(!range(state,sizeof(*state),alignof(State),ranges[range_count++])||
       !range(services,sizeof(*services),alignof(Services),ranges[range_count++])||
       !range(result,sizeof(*result),alignof(bool),ranges[range_count++])||
       !range(details,sizeof(*details),alignof(Result),ranges[range_count++]))return false;
    Range property_range{},mana_exempt_range{};
    if(!state->character||!range(state->properties,sizeof(*state->properties),alignof(data::PropertyView),property_range)||
       !range(state->mana_exempt_14f0,sizeof(*state->mana_exempt_14f0),alignof(std::uint8_t),mana_exempt_range))return false;
    ranges[range_count++]=property_range;ranges[range_count++]=mana_exempt_range;
    if(!disjoint(ranges,range_count)||dh2_property_validate(state->properties)!=0)return false;
    return true;
}

Status enter(const State* state,std::int32_t amount,const Services* services,bool* result,
             Result* details,Range* ranges,std::size_t& count){
    count=0;if(!valid_common(state,services,result,details,ranges,count))return Status::invalid_argument;
    if(amount<0)return Status::unsupported_assertion_domain;
    return Status::complete;
}

int callback_fail(char* error,std::size_t capacity,const char* message){
    if(error&&capacity)std::snprintf(error,capacity,"%s",message);
    return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
}

bool number_to_amount(float number,std::int32_t& amount){
    // The wrapper first converts with __aeabi_f2iz, then the integer caller's
    // assertion checks the converted value. Negative fractions in (-1,0)
    // therefore become the supported integer zero; negative integers do not.
    if(!std::isfinite(number)||number< -2147483648.0f||number>=2147483648.0f)return false;
    amount=static_cast<std::int32_t>(number);return amount>=0;
}

int callback(bool use,void* opaque,const dh2_script_value* arguments,std::uint32_t count,
             dh2_script_value* results,std::uint32_t capacity,std::uint32_t* result_count,
             char* error,std::size_t error_capacity){
    if(result_count)*result_count=0;
    // The original wrappers only test for an empty vector, then read element
    // zero. Additional arguments are ignored. They do not inspect ReturnValues
    // or push a result when the vector is empty or element zero is not numeric.
    if(count==0||!arguments||arguments[0].type!=DH2_SCRIPT_NUMBER)return 0;
    if(!opaque||!results||capacity<1||!result_count)return callback_fail(error,error_capacity,"Mana callback context or result storage is unavailable");
    const auto* context=static_cast<const CallbackContext*>(opaque);
    if(!context->state||!context->services)return callback_fail(error,error_capacity,"Mana callback owner or providers are unavailable");
    if(use&&!context->globals)return callback_fail(error,error_capacity,"UseMana global providers are unavailable");
    std::int32_t amount=0;if(!number_to_amount(arguments[0].number,amount))return callback_fail(error,error_capacity,"Mana amount is outside the supported nonnegative source domain");
    bool value=false;Result details{};
    const auto status=use?use_mana(context->state,context->globals,amount,context->services,&value,&details)
                         :has_mana(context->state,amount,context->services,&value,&details);
    if(status!=Status::complete)return callback_fail(error,error_capacity,"Required Character mana provider failed");
    dh2_script_value output{};output.type=DH2_SCRIPT_BOOLEAN;output.boolean=value?1u:0u;
    results[0]=output;*result_count=1;return 0;
}
} // namespace

Status has_mana(const State* state,std::int32_t amount,const Services* services,bool* result,Result* details){
    Range ranges[6]{};std::size_t count=0;
    auto status=enter(state,amount,services,result,details,ranges,count);if(status!=Status::complete)return status;
    try {
    ActiveGuard active_owner(*state,details);if(!active_owner.entered())return Status::reentrant_owner;
    *details={};details->amount=amount;
    Kernel kernel{*state,*services,*details};bool value=false;
    status=kernel.has_mana_impl(amount,value);
    if(status==Status::complete){details->decision=value?1u:0u;*result=value;}
    return status;
    }catch(...){return Status::service_failed;}
}

Status use_mana(const State* state,const Globals* globals,std::int32_t amount,const Services* services,bool* result,Result* details){
    Range ranges[7]{};std::size_t count=0;
    auto status=enter(state,amount,services,result,details,ranges,count);if(status!=Status::complete)return status;
    Range globals_range{};
    if(globals){if(!range(globals,sizeof(*globals),alignof(Globals),globals_range))return Status::invalid_argument;
        for(std::size_t i=0;i<count;++i)if(overlap(globals_range,ranges[i]))return Status::invalid_argument;
        ranges[count++]=globals_range;}
    const Globals bound_globals=globals?*globals:Globals{};
    const State bound_state=*state;
    const Services bound_services=*services;
    try {
    ActiveGuard active_owner(*state,details);if(!active_owner.entered())return Status::reentrant_owner;
    *details={};details->amount=amount;
    Kernel kernel{bound_state,bound_services,*details};bool value=false,remote=false;
    try {
        status=kernel.read_online(remote);
        if(status==Status::complete&&remote)value=true;
        else if(status==Status::complete) {
            if(!globals||!bound_globals.application_singleton||!*bound_globals.application_singleton||!bound_services.invoke)
                status=Status::service_unavailable;
            else {
                Reply reply{};
                ++details->application_is_saved_option_on_queries;
                status=kernel.call(Operation::application_is_saved_option_on,*bound_globals.application_singleton,"GOD_MANA",reply);
                if(status==Status::complete){details->last_application_is_saved_option_on_word=reply.word;
                    if(reply.word)value=true;
                    else {
                        bool god_mana=false;status=kernel.debug_switch(bound_globals,"GOD_MANA",god_mana);
                        if(status==Status::complete&&god_mana)value=true;
                        else if(status==Status::complete) {
                            const auto exempt=*kernel.state.mana_exempt_14f0;details->mana_exempt_14f0_byte=exempt;
                            if(exempt)value=true;
                            else {
                                bool has=false;status=kernel.has_mana_impl(amount,has);
                                if(status==Status::complete&&!has)value=false;
                                else if(status==Status::complete) {
                                    if(dh2_property_add(kernel.state.properties,41,-amount)!=0)status=Status::invalid_property_view;
                                    else {
                                        details->mana_added=1;
                                        bool ignored=false;status=kernel.debug_switch(bound_globals,"isTracingChar_Stats",ignored);
                                        if(status==Status::complete)value=true;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }catch(...){status=Status::service_failed;}
    if(status==Status::complete){details->decision=value?1u:0u;*result=value;}
    return status;
    }catch(...){return Status::service_failed;}
}

int has_mana_callback(void* context,const dh2_script_value* arguments,std::uint32_t count,
                      dh2_script_value* results,std::uint32_t capacity,std::uint32_t* result_count,
                      char* error,std::size_t error_capacity){
    return callback(false,context,arguments,count,results,capacity,result_count,error,error_capacity);
}
int use_mana_callback(void* context,const dh2_script_value* arguments,std::uint32_t count,
                      dh2_script_value* results,std::uint32_t capacity,std::uint32_t* result_count,
                      char* error,std::size_t error_capacity){
    return callback(true,context,arguments,count,results,capacity,result_count,error,error_capacity);
}

} // namespace dh2::character_mana_services_v1
