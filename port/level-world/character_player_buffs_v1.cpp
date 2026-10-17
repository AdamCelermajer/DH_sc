#include "character_player_buffs_v1.hpp"
#include "../adam-script-runtime/script_runtime.h"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <deque>
#include <map>
#include <string>
#include <vector>

namespace dh2::character_player_buffs_v1 {
namespace {
struct Range {std::uintptr_t b,e;};
bool range(const void* p,std::size_t n,std::size_t alignment,Range& r){
    const auto at=reinterpret_cast<std::uintptr_t>(p);
    if(!p||at%alignment||at>UINTPTR_MAX-n)return false;
    r={at,at+n};return true;
}
template<class T>bool span(const T* p,Range& r){return range(p,sizeof(T),alignof(T),r);}
bool overlap(Range a,Range b){return a.b<b.e&&b.b<a.e;}
std::int32_t signed_word(std::uint32_t w){std::int32_t s;std::memcpy(&s,&w,4);return s;}
}
struct Owner::Impl {
    struct Declaration;
    struct Instance {data::PropertySheet sheet{};std::uint32_t strength=0;std::int32_t timer=-1;Declaration* declaration=nullptr;};
    struct Declaration {std::int32_t id=-1;std::uintptr_t fx=0;std::string name;std::deque<std::unique_ptr<Instance>> instances;std::vector<const std::int32_t*> sheets;};
    Bindings bindings;std::map<std::int32_t,Declaration> declarations;std::vector<data::PropertyBuffGroup> groups;bool busy=false,retired=false;
    explicit Impl(Bindings b):bindings(b){}
    void publish(){bindings.properties->groups=groups.empty()?nullptr:groups.data();bindings.properties->group_count=static_cast<std::uint32_t>(groups.size());}
    void refresh(){
        // Structural growth reserves group/sheet backing before it happens.
        // Shrinking and publication allocate nothing, including failure paths.
        groups.resize(declarations.size());
        std::size_t index=0;
        for(auto& entry:declarations){auto& d=entry.second;d.sheets.resize(d.instances.size());std::size_t n=0;
            for(auto& instance:d.instances)d.sheets[n++]=instance->sheet.data();
            groups[index++]={d.sheets.data(),static_cast<std::uint32_t>(d.sheets.size())};}
        publish();
    }
    bool call(Result& out,Operation operation,std::int32_t id=0,std::uintptr_t subject=0,
              std::uint32_t duration=0,std::int32_t index=0,std::int32_t* sheet=nullptr,Response* response=nullptr){
        out.last_operation=operation;++out.calls;Response local{};
        const Request q{operation,bindings.character,subject,id,index,0,operation==Operation::timer_start?0x36:0,duration,
                        operation==Operation::fx_enable?1u:0u,sheet};
        try{const auto code=bindings.services.invoke(bindings.services.context,bindings.properties,&q,&local);
            if(code==0&&response)*response=local;
            return code==0;}catch(...){return false;}
    }
    bool recalc(Result& out){refresh();return call(out,Operation::recalculate);}
    Status remove(std::int32_t id,std::uintptr_t identity,Result& out){
        auto found=declarations.find(id);if(found==declarations.end())return Status::complete;auto& d=found->second;
        if(d.instances.size()==1){
            if(!call(out,Operation::timer_stop,d.instances.front()->timer))return Status::provider_failed;
            d.instances.clear();refresh();
            if(!call(out,Operation::fx_release,0,d.fx))return Status::provider_failed;
            d.fx=0;declarations.erase(found);return recalc(out)?Status::complete:Status::provider_failed;
        }
        if(!identity)return Status::complete;
        auto instance=std::find_if(d.instances.begin(),d.instances.end(),[&](const auto& p){return reinterpret_cast<std::uintptr_t>(p.get())==identity;});
        if(instance==d.instances.end())return Status::complete;
        if(!call(out,Operation::timer_stop,(*instance)->timer))return Status::provider_failed;
        d.instances.erase(instance);return recalc(out)?Status::complete:Status::provider_failed;
    }
    Instance* find(std::uintptr_t identity) const {
        for(const auto& entry:declarations)for(const auto& instance:entry.second.instances)
            if(reinterpret_cast<std::uintptr_t>(instance.get())==identity)return instance.get();
        return nullptr;
    }
    bool separate(Range output) const {
        Range r;
        if(!span(this,r)||overlap(output,r)||!span(bindings.properties,r)||overlap(output,r))return false;
        for(const auto* sheet:{bindings.properties->defaults,bindings.properties->types,bindings.properties->base,
                              static_cast<const std::int32_t*>(bindings.properties->saved),bindings.properties->gear,
                              static_cast<const std::int32_t*>(bindings.properties->resolved)})
            if(!range(sheet,896,alignof(std::int32_t),r)||overlap(output,r))return false;
        for(const auto& entry:declarations){
            if(!span(&entry.second,r)||overlap(output,r))return false;
            if(!entry.second.sheets.empty()&&(!range(entry.second.sheets.data(),entry.second.sheets.size()*sizeof(entry.second.sheets[0]),alignof(const std::int32_t*),r)||overlap(output,r)))return false;
            for(const auto& instance:entry.second.instances)if(!span(instance.get(),r)||overlap(output,r))return false;
        }
        if(!groups.empty()&&(!range(groups.data(),groups.size()*sizeof(groups[0]),alignof(data::PropertyBuffGroup),r)||overlap(output,r)))return false;
        return true;
    }
    bool begin(Result* out,const Owner* owner){
        Range r,o,p;
        if(retired||busy||!span(out,r)||!span(owner,o)||!span(bindings.properties,p)||overlap(r,o)||overlap(r,p)||
           !separate(r)||dh2_property_validate(bindings.properties))return false;
        *out={};return true;
    }
    struct Scope {Impl& impl;explicit Scope(Impl& p):impl(p){p.busy=true;}~Scope(){impl.busy=false;}};
};
Owner::Owner(std::unique_ptr<Impl> p):impl_(std::move(p)){}
Owner::~Owner()=default;
std::unique_ptr<Owner> Owner::create(Bindings b){
    Range view;
    if(!b.character||!b.services.invoke||!span(b.properties,view)||dh2_property_validate(b.properties)||
       b.properties->groups||b.properties->group_count||b.class_count>10000||
       (b.fx_count!=UINT32_MAX&&b.fx_count>1000000))return {};
    try{return std::unique_ptr<Owner>(new Owner(std::make_unique<Impl>(b)));}catch(...){return {};}
}
Status Owner::add(std::int32_t id,std::uint32_t duration,std::int32_t capacity,std::uint32_t strength,
                  std::int32_t fx,const char* name,Result* out){
    if(!name||!impl_->begin(out,this))return Status::invalid_argument;
    Impl::Scope scope(*impl_);auto& p=*impl_;
    try{
        if(capacity<=0)capacity=128;
        Impl::Instance* candidate=nullptr;
        auto found=p.declarations.find(id);
        if(found!=p.declarations.end()&&found->second.instances.size()==std::uint32_t(capacity)){
            for(auto& owned:found->second.instances){auto& instance=*owned;
                if(instance.strength<strength){candidate=&instance;instance.strength=strength;continue;}
                if(instance.strength!=strength)continue;
                if(instance.timer==-1||!candidate){candidate=&instance;continue;}
                Response a{},b{};
                if(!p.call(*out,Operation::timer_time_left,candidate->timer,0,0,0,nullptr,&a)||
                   !p.call(*out,Operation::timer_time_left,instance.timer,0,0,0,nullptr,&b))return Status::provider_failed;
                // Source TimeLeft returns elapsed/duration; it compares elapsed.
                if(a.elapsed<b.elapsed)candidate=&instance;
            }
        }
        if(candidate){
            if(!p.call(*out,Operation::timer_stop,candidate->timer))return Status::provider_failed;
            candidate->timer=-1;
        }else{
            if(found==p.declarations.end()){p.groups.reserve(p.declarations.size()+1);p.publish();}
            auto& d=p.declarations[id];d.id=id;p.refresh();d.name=name;
            if(!d.fx&&fx!=-1){Response response{};const bool delivered=p.call(*out,Operation::fx_load,fx,0,0,0,nullptr,&response);
                if(!delivered)return Status::provider_failed;
                d.fx=response.identity;}
            if(d.instances.size()>=std::uint32_t(capacity)){p.refresh();return Status::complete;}
            // Reserve port view backing before owning a new source instance.
            d.sheets.reserve(d.instances.size()+1);
            p.refresh(); // reserve may have moved a currently published array.
            auto instance=std::make_unique<Impl::Instance>();instance->strength=strength;instance->declaration=&d;
            candidate=instance.get();d.instances.push_back(std::move(instance));p.refresh();
        }
        if(duration){Response response{};
            const bool delivered=p.call(*out,Operation::timer_start,0,reinterpret_cast<std::uintptr_t>(candidate),duration,0,nullptr,&response);
            if(!delivered)return Status::provider_failed;
            candidate->timer=response.word;
            if(candidate->timer==-1)return p.remove(id,reinterpret_cast<std::uintptr_t>(candidate),*out);
            if(candidate->timer< -1)return Status::provider_failed;
        }
        auto& d=*candidate->declaration;
        if(d.fx&&d.instances.size()>1){Response response{};
            if(!p.call(*out,Operation::fx_object,0,d.fx,0,0,nullptr,&response)||
               !p.call(*out,Operation::fx_enable,0,response.identity,0,int(d.instances.size()-1)))return Status::provider_failed;}
        std::copy_n(p.bindings.properties->defaults,224,candidate->sheet.begin());
        candidate->sheet[172]=signed_word(strength<<8);p.bindings.properties->resolved[172]=candidate->sheet[172];
        out->instance=reinterpret_cast<std::uintptr_t>(candidate);return Status::complete;
    }catch(...){return Status::provider_failed;}
}
Status Owner::remove(std::int32_t id,std::uintptr_t instance,Result* out){
    if(!impl_->begin(out,this))return Status::invalid_argument;
    Impl::Scope scope(*impl_);try{return impl_->remove(id,instance,*out);}catch(...){return Status::provider_failed;}
}
Status Owner::remove_all(Result* out){
    if(!impl_->begin(out,this))return Status::invalid_argument;
    Impl::Scope scope(*impl_);auto& p=*impl_;
    try{for(auto& entry:p.declarations){auto& d=entry.second;
        for(auto& instance:d.instances)if(!p.call(*out,Operation::timer_stop,instance->timer))return Status::provider_failed;
        d.instances.clear();p.refresh();if(!p.call(*out,Operation::fx_release,0,d.fx))return Status::provider_failed;d.fx=0;}
        p.declarations.clear();return p.recalc(*out)?Status::complete:Status::provider_failed;
    }catch(...){return Status::provider_failed;}
}
Status Owner::expired(const character::Timer32* timer,Result* out){
    Range t,r;if(!span(timer,t)||!span(out,r)||overlap(t,r))return Status::invalid_argument;
    const auto instance=impl_->find(timer->user_ref);if(!instance)return Status::invalid_argument;
    // Source diagnostic-only mismatched timer ID still removes the reference.
    return remove(instance->declaration->id,timer->user_ref,out);
}
Status Owner::apply(std::int32_t id,std::uintptr_t identity,Result* out){
    if(!impl_->begin(out,this))return Status::invalid_argument;
    Impl::Scope scope(*impl_);auto& p=*impl_;auto* instance=p.find(identity);
    if(!instance)return Status::unsupported_domain;
    if(!p.call(*out,Operation::apply_class,id,identity,0,0,instance->sheet.data()))return Status::provider_failed;
    try{return p.recalc(*out)?Status::complete:Status::provider_failed;}catch(...){return Status::provider_failed;}
}
Status Owner::retire(Result* out){
    if(!impl_->begin(out,this))return Status::invalid_argument;
    Impl::Scope scope(*impl_);auto& p=*impl_;
    try{for(auto& entry:p.declarations){auto& d=entry.second;d.instances.clear();p.refresh();
        if(d.fx&&!p.call(*out,Operation::fx_release,0,d.fx))return Status::provider_failed;
        d.fx=0;}
        p.declarations.clear();p.refresh();p.retired=true;return Status::complete;
    }catch(...){return Status::provider_failed;}
}
Status Owner::attach(data::PropertyView* view){
    Range r;if(impl_->busy||impl_->retired||!span(view,r)||dh2_property_validate(view))return Status::invalid_argument;
    const auto* old=impl_->bindings.properties;
    if(view->base!=old->base||view->saved!=old->saved||view->gear!=old->gear||view->resolved!=old->resolved||
       view->defaults!=old->defaults||view->types!=old->types)return Status::invalid_argument;
    impl_->bindings.properties=view;
    try{impl_->refresh();return Status::complete;}catch(...){return Status::provider_failed;}
}
std::uintptr_t Owner::character_identity() const{return impl_->bindings.character;}
std::uint32_t Owner::class_count() const{return impl_->bindings.class_count;}
std::uint32_t Owner::fx_count() const{return impl_->bindings.fx_count;}
std::uint32_t Owner::count() const{std::uint32_t n=0;for(const auto& entry:impl_->declarations)n+=std::uint32_t(entry.second.instances.size());return n;}
std::uint32_t Owner::declarations() const{return std::uint32_t(impl_->declarations.size());}
bool Owner::owned_sheet(std::uintptr_t identity,std::int32_t** out) const{
    Range r,o;if(!span(out,r)||!span(this,o)||overlap(r,o)||!identity||impl_->retired)return false;
    if(!impl_->separate(r))return false;
    auto* instance=impl_->find(identity);if(!instance)return false;
    *out=instance->sheet.data();return true;
}
bool Owner::snapshot(std::uint32_t index,Snapshot* out) const{
    Range r,o;if(!span(out,r)||!span(this,o)||overlap(r,o)||!impl_->separate(r))return false;
    for(const auto& entry:impl_->declarations)for(const auto& instance:entry.second.instances){if(index--==0){
        *out={reinterpret_cast<std::uintptr_t>(instance.get()),entry.first,instance->timer,instance->strength,instance->sheet.data(),entry.second.name.c_str(),entry.second.fx};return true;}}
    return false;
}
namespace {
int fail(char* text,std::size_t n){if(text&&n)std::snprintf(text,n,"required owned Player buff provider/domain unavailable");return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;}
bool number(const dh2_script_value& v,std::int32_t& out){
    if(v.type!=DH2_SCRIPT_NUMBER||!std::isfinite(v.number))return false;
    out=v.number>=2147483648.f?INT32_MAX:v.number< -2147483648.f?INT32_MIN:std::int32_t(v.number);return true;
}
bool unsigned_number(const dh2_script_value& v,std::uint32_t& out){
    if(v.type!=DH2_SCRIPT_NUMBER||!std::isfinite(v.number))return false;
    out=v.number<0?0:v.number>=4294967296.f?UINT32_MAX:std::uint32_t(v.number);return true;
}
bool native_number(const dh2_script_value& v,std::int32_t& out){
    if(v.type==DH2_SCRIPT_NIL){out=0;return true;}
    if(v.type==DH2_SCRIPT_BOOLEAN){out=v.boolean?1:0;return true;}
    return number(v,out);
}
bool controls(CallbackBindings* b,const dh2_script_value* values,std::uint32_t count,std::uint32_t* returned){
    Range br,ar,rr,orr;if(!span(b,br)||!span(b->owner,orr)||!span(returned,rr)||overlap(br,rr)||overlap(orr,rr)||count>65536)return false;
    return !count||(range(values,std::size_t(count)*sizeof(*values),alignof(dh2_script_value),ar)&&!overlap(rr,ar)&&!overlap(br,ar));
}
}
int create_buff(void* raw,const dh2_script_value* a,std::uint32_t n,dh2_script_value* output,
                std::uint32_t capacity,std::uint32_t* returned,char* text,std::size_t size) noexcept{
    try{auto* b=static_cast<CallbackBindings*>(raw);if(!controls(b,a,n,returned))return fail(text,size);
        *returned=0;if(!n||a[0].type!=DH2_SCRIPT_NUMBER)return 0;
        std::uint32_t guard;std::int32_t id;if(!unsigned_number(a[0],guard)||!number(a[0],id))return fail(text,size);
        if(guard>=b->owner->class_count())return 0;
        std::uint32_t duration=0,strength=0;std::int32_t cap=1,fx=-1;const char* name="";
        if(n>1&&a[1].type!=DH2_SCRIPT_NIL&&a[1].type==DH2_SCRIPT_NUMBER&&!unsigned_number(a[1],duration))return fail(text,size);
        if(n>2&&a[2].type!=DH2_SCRIPT_NIL){if(a[2].type==DH2_SCRIPT_BOOLEAN)cap=a[2].boolean?0:1;else if(!native_number(a[2],cap))return fail(text,size);}
        if(n>3&&a[3].type==DH2_SCRIPT_NUMBER&&!unsigned_number(a[3],strength))return fail(text,size);
        if(n>4&&a[4].type!=DH2_SCRIPT_NIL){
            if(a[4].type==DH2_SCRIPT_NUMBER){if(!number(a[4],fx))return fail(text,size);}
            else {std::int32_t converted;if(!native_number(a[4],converted)||b->owner->fx_count()==UINT32_MAX)return fail(text,size);
                if(std::uint32_t(converted)<b->owner->fx_count())fx=converted;}
        }
        if(n>5&&a[5].type==DH2_SCRIPT_STRING){if(!a[5].text)return fail(text,size);name=a[5].text;}
        Range output_range;if(!capacity||!span(output,output_range))return fail(text,size);
        Range input,control,count;if(n){range(a,std::size_t(n)*sizeof(*a),alignof(dh2_script_value),input);if(overlap(input,output_range))return fail(text,size);}
        span(b,control);span(returned,count);if(overlap(control,output_range)||overlap(count,output_range))return fail(text,size);
        Result result{};if(b->owner->add(id,duration,cap,strength,fx,name,&result)!=Status::complete)return fail(text,size);
        if(result.instance){output[0]={};output[0].type=DH2_SCRIPT_IDENTITY;output[0].identity=result.instance;*returned=1;}return 0;
    }catch(...){return fail(text,size);}
}
int remove_buff(void* raw,const dh2_script_value* a,std::uint32_t n,dh2_script_value*,std::uint32_t,
                std::uint32_t* returned,char* text,std::size_t size) noexcept{
    try{auto* b=static_cast<CallbackBindings*>(raw);if(!controls(b,a,n,returned))return fail(text,size);
        *returned=0;if(!n||a[0].type!=DH2_SCRIPT_NUMBER)return 0;
        std::uint32_t id;if(!unsigned_number(a[0],id))return fail(text,size);
        if(id>=b->owner->class_count())return 0;
        if(n>1&&a[1].type!=DH2_SCRIPT_IDENTITY)return 0;
        Result result{};return b->owner->remove(std::int32_t(id),n>1?a[1].identity:0,&result)==Status::complete?0:fail(text,size);
    }catch(...){return fail(text,size);}
}
int apply_buff(void* raw,const dh2_script_value* a,std::uint32_t n,dh2_script_value*,std::uint32_t,
               std::uint32_t* returned,char* text,std::size_t size) noexcept{
    try{auto* b=static_cast<CallbackBindings*>(raw);if(!controls(b,a,n,returned))return fail(text,size);*returned=0;
        if(!n||a[0].type!=DH2_SCRIPT_NUMBER)return 0;
        std::uint32_t guard;if(!unsigned_number(a[0],guard))return fail(text,size);
        if(guard>b->owner->class_count())return 0;
        if(n<2||a[1].type!=DH2_SCRIPT_IDENTITY)return fail(text,size);
        if(!a[1].identity)return 0;
        Result result{};return b->owner->apply(signed_word(guard),a[1].identity,&result)==Status::complete?0:fail(text,size);
    }catch(...){return fail(text,size);}
}
}
