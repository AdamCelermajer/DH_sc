#include "script_value_boolean.hpp"
#include <limits>
namespace dh2::script_value_boolean {namespace {
struct Range {std::uintptr_t a,b;};
template<class T>bool range(const T* p,Range& r){
    const auto a=reinterpret_cast<std::uintptr_t>(p);
    if(!p||a%alignof(T)||a>std::numeric_limits<std::uintptr_t>::max()-sizeof(T))return false;
    r={a,a+sizeof(T)};return true;
}
bool overlap(Range a,Range b){return a.a<b.b&&b.a<a.b;}
Status invoke(const Value* value,const Services& services,Result& out,Operation op,
              Response& reply,const char* text=nullptr){
    out.phase=static_cast<Phase>(static_cast<unsigned>(op)+1);
    if(!services.invoke)return Status::service_unavailable;
    const Request request{op,out.lua,text,op==Operation::to_boolean?-1:0};
    reply={};++out.service_calls;
    try{if(services.invoke(services.context,value,&request,&reply))return Status::service_failed;}
    catch(...){return Status::service_failed;}
    return Status::complete;
}
}
Status execute(const Value* value,const Services* services,Result* out){
    Range input,result,binding;
    if(!range(value,input)||!range(out,result)||overlap(input,result))return Status::invalid_argument;
    if(services&&(!range(services,binding)||overlap(binding,input)||overlap(binding,result)))return Status::invalid_argument;
    const Services bound=services?*services:Services{};
    const auto type=value->type;
    *out={};out->captured_type=type;
    if(type==1||type==3){
        // __aeabi_fcmpeq(binary32,+0) is nonzero only for the two zero words.
        // Direct bit classification reproduces that helper's exact result,
        // including every NaN payload/subnormal, without host FP traps/flags.
        out->value=(value->number_word&0x7fffffffu)!=0;
    }else if(type==2||type==7)out->value=value->object_identity!=0;
    else if(type==4){
        Response reply{};
        auto status=invoke(value,bound,*out,Operation::new_state,reply);
        if(status!=Status::complete)return status;
        out->lua=reply.lua;if(!out->lua)return Status::invalid_source_fact;
        // Original reads Value+20 after luaL_newstate returns, using captured
        // original Value identity even if the callback changes its raw type.
        const auto text=value->string;
        status=invoke(value,bound,*out,Operation::push_string,reply,text);
        if(status!=Status::complete)return status;
        status=invoke(value,bound,*out,Operation::to_boolean,reply);
        if(status!=Status::complete)return status;
        out->raw_lua_boolean=reply.raw_boolean;
        out->value=reply.raw_boolean!=0;
        status=invoke(value,bound,*out,Operation::close_state,reply);
        if(status!=Status::complete)return status;
        out->closed=1;
    }
    out->phase=Phase::complete;return Status::complete;
}
} // namespace dh2::script_value_boolean
