#include "character_player_scalar_services_v1.hpp"
#include "../adam-script-runtime/script_function_alias.h"
#include <cmath>
#include <cstdio>
#include <cstring>

namespace dh2::character_player_scalar_services_v1 {namespace {
struct Range {std::uintptr_t b,e;};
bool range(const void* p,std::size_t n,std::size_t a,Range& r){auto v=reinterpret_cast<std::uintptr_t>(p);if(!p||v%a||v>UINTPTR_MAX-n)return false;r={v,v+n};return true;}
template<class T>bool span(const T* p,Range& r){return range(p,sizeof(T),alignof(T),r);}
bool overlap(Range a,Range b){return a.b<b.e&&b.b<a.e;}
bool valid(const State* s,Result* out){Range a,b,c;return span(s,a)&&span(out,b)&&!overlap(a,b)&&s->ais&&span(s->integers,c)&&!overlap(b,c)&&!overlap(a,c);}
bool name_valid(const char* p){Range r;if(!range(p,4096,1,r))return false;for(std::size_t i=0;i<4096;++i)if(!p[i])return true;return false;}
bool name_separate(const char* p,const State* s,const Result* out){
    Range key,sr,mr,rr;if(!name_valid(p)||!span(s,sr)||!span(s->integers,mr)||!span(out,rr))return false;
    range(p,std::strlen(p)+1,1,key);return !overlap(key,sr)&&!overlap(key,mr)&&!overlap(key,rr);
}
bool args_valid(const Arguments* a,Range& r){
    if(!span(a,r)||a->count>65536)return false;
    if(!a->count)return true;
    Range values;return range(a->values,sizeof(dh2_script_value)*std::size_t(a->count),alignof(dh2_script_value),values);
}
bool controls(const State* s,const Arguments* a,const Services* service,Result* out){
    Range ar,rr,sr,mr,vr;
    if(!valid(s,out)||!args_valid(a,ar)||!span(out,rr)||!span(s,sr)||!span(s->integers,mr)||overlap(ar,rr)||overlap(ar,sr)||overlap(ar,mr))return false;
    if(a->count){range(a->values,sizeof(dh2_script_value)*std::size_t(a->count),alignof(dh2_script_value),vr);if(overlap(vr,rr)||overlap(vr,sr)||overlap(vr,mr)||overlap(vr,ar))return false;}
    if(service){Range pr;if(!span(service,pr)||overlap(pr,rr)||overlap(pr,ar)||overlap(pr,sr)||overlap(pr,mr)||(a->count&&overlap(pr,vr)))return false;}
    return true;
}
Status string_value(const Services& services,const dh2_script_value* value,const char*& key,Result& out){
    out.phase=Phase::string;++out.conversions;
    if(value->type==DH2_SCRIPT_STRING){key=value->text;return name_valid(key)?Status::complete:Status::unsupported_domain;}
    if(!services.get_string)return Status::provider_failed;
    try{if(services.get_string(services.context,value,&key))return Status::provider_failed;}catch(...){return Status::provider_failed;}
    return name_valid(key)?Status::complete:Status::unsupported_domain;
}
Status numeric_value(const Services& services,const dh2_script_value* value,std::int32_t& word,Result& out){
    out.phase=Phase::number;++out.conversions;float n=0;
    if(value->type==DH2_SCRIPT_NUMBER)n=value->number;
    else {
        if(!services.get_number)return Status::provider_failed;
        try{if(services.get_number(services.context,value,&n))return Status::provider_failed;}catch(...){return Status::provider_failed;}
    }
    if(!std::isfinite(n)||n<-2147483648.f||n>=2147483648.f)return Status::unsupported_domain;
    word=static_cast<std::int32_t>(n);return Status::complete;
}
Status write(const State& s,const char* key,std::int32_t value,Result& out){
    ++out.hash_reads;const auto hash=dh2_script_alias_hash(key);
    try{
        const auto found=s.integers->find(hash);
        if(found==s.integers->end()){s.integers->emplace(hash,value);++out.inserted;}
        else found->second=value;
        out.value=value;return Status::complete;
    }catch(...){return Status::provider_failed;}
}
Status read(const State& s,const char* key,Result& out){
    out.phase=Phase::dictionary;++out.hash_reads;const auto hash=dh2_script_alias_hash(key);
    const auto found=s.integers->find(hash);
    if(found!=s.integers->end()){out.value=found->second;return Status::complete;}
    // Original GetInt calls SetInt with captured key and zero; that second
    // caller hashes the original string afresh before inserting its entry.
    return write(s,key,0,out);
}
int fail(char* text,std::size_t n){if(text&&n)std::snprintf(text,n,"required LuaScript private-integer provider/domain unavailable");return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;}
}
Status get(const State* s,const char* key,std::int32_t* value,Result* out){
    Range v,r,a,m;
    if(!valid(s,out)||!span(value,v)||!span(out,r)||!span(s,a)||!span(s->integers,m)||overlap(v,r)||overlap(v,a)||overlap(v,m)||!name_separate(key,s,out))return Status::invalid_argument;
    Range kr;range(key,std::strlen(key)+1,1,kr);if(overlap(v,kr))return Status::invalid_argument;
    const State captured=*s;*out={};out->ais=captured.ais;
    auto status=read(captured,key,*out);if(status==Status::complete){*value=out->value;out->returned=1;out->phase=Phase::complete;}return status;
}
Status set(const State* s,const char* key,std::int32_t value,Result* out){
    if(!valid(s,out)||!name_separate(key,s,out))return Status::invalid_argument;
    const State captured=*s;*out={};out->ais=captured.ais;out->phase=Phase::dictionary;
    auto status=write(captured,key,value,*out);if(status==Status::complete)out->phase=Phase::complete;return status;
}
Status get_callback(const State* state,const Arguments* a,const Services* services,Result* out){
    if(!controls(state,a,services,out))return Status::invalid_argument;
    const State captured=*state;const Services bound=services?*services:Services{};*out={};out->ais=captured.ais;
    if(!a->count){out->phase=Phase::complete;return Status::complete;}
    const char* key=nullptr;auto status=string_value(bound,a->values,key,*out);
    if(status!=Status::complete)return status;
    if(!name_separate(key,state,out))return Status::unsupported_domain;
    status=read(captured,key,*out);if(status==Status::complete){out->returned=1;out->phase=Phase::complete;}return status;
}
Status set_callback(const State* state,const Arguments* a,const Services* services,Result* out){
    if(!controls(state,a,services,out))return Status::invalid_argument;
    const State captured=*state;const Services bound=services?*services:Services{};*out={};out->ais=captured.ais;
    if(a->count<2){out->phase=Phase::complete;return Status::complete;}
    const char* key=nullptr;auto status=string_value(bound,a->values,key,*out);
    if(status!=Status::complete)return status;
    if(!controls(state,a,services,out)||a->count<2||!name_separate(key,state,out))return Status::unsupported_domain;
    std::int32_t value=0;status=numeric_value(bound,a->values+1,value,*out);
    if(status!=Status::complete)return status;
    out->phase=Phase::dictionary;status=write(captured,key,value,*out);if(status==Status::complete)out->phase=Phase::complete;return status;
}
namespace {
int callback(bool is_set,void* opaque,const dh2_script_value* values,std::uint32_t count,
             dh2_script_value* output,std::uint32_t capacity,std::uint32_t* returned,char* text,std::size_t n){
    auto* b=static_cast<Bindings*>(opaque);Range br,rr,vr;
    if(!span(b,br)||!span(returned,rr)||overlap(br,rr)||count>65536||(count&&!values))return fail(text,n);
    if(!is_set&&count&&(!capacity||!span(output,vr)||overlap(vr,br)||overlap(vr,rr)))return fail(text,n);
    Range sr,mr,ar,pr;
    if(!span(b->state,sr)||!span(b->state->integers,mr)||overlap(rr,sr)||overlap(rr,mr)||
       (!is_set&&count&&(overlap(vr,sr)||overlap(vr,mr))))return fail(text,n);
    if(b->services&&(!span(b->services,pr)||overlap(rr,pr)||(!is_set&&count&&overlap(vr,pr))))return fail(text,n);
    if(count&&(!range(values,sizeof(dh2_script_value)*std::size_t(count),alignof(dh2_script_value),ar)||overlap(rr,ar)||
       (!is_set&&(overlap(vr,ar)))))return fail(text,n);
    *returned=0;Arguments args{values,count};Result result{};
    const auto status=is_set?set_callback(b->state,&args,b->services,&result):get_callback(b->state,&args,b->services,&result);
    if(status!=Status::complete)return fail(text,n);
    if(result.returned){output[0]={};output[0].type=DH2_SCRIPT_NUMBER;output[0].number=static_cast<float>(result.value);*returned=1;}return 0;
}
}
int get_int(void* b,const dh2_script_value* a,std::uint32_t c,dh2_script_value* o,std::uint32_t cap,std::uint32_t* r,char* e,std::size_t n) noexcept {try{return callback(false,b,a,c,o,cap,r,e,n);}catch(...){return fail(e,n);}}
int set_int(void* b,const dh2_script_value* a,std::uint32_t c,dh2_script_value* o,std::uint32_t cap,std::uint32_t* r,char* e,std::size_t n) noexcept {try{return callback(true,b,a,c,o,cap,r,e,n);}catch(...){return fail(e,n);}}
}
