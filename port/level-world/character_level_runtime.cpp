#include "character_level_runtime.hpp"
#include <cmath>
#include <cstddef>
#include <cstring>

namespace dh2::character_level_runtime {
namespace sl=character_script_set_level;
namespace rg=character_regeneration;
namespace {
struct Range {std::uintptr_t first,end;};
bool range(const void* pointer,std::size_t size,std::size_t alignment,Range& out){const auto at=reinterpret_cast<std::uintptr_t>(pointer);if(!pointer||at%alignment||at>UINTPTR_MAX-size)return false;out={at,at+size};return true;}
bool overlaps(Range a,Range b){return a.first<b.end&&b.first<a.end;}
std::int32_t signed_word(std::uint32_t bits){std::int32_t out;std::memcpy(&out,&bits,4);return out;}
std::int32_t fixed_number(void* context,std::uintptr_t argument,std::uint32_t* out){if(argument!=reinterpret_cast<std::uintptr_t>(context))return 1;std::memcpy(out,context,4);return 0;}
}
struct Runtime::Context {
    Runtime* runtime;Storage storage;NumberServices numbers;Result* result;
    Status error=Status::complete;
    debug_switches::Runtime* captured_debug=nullptr;
    debug_switches::Services captured_debug_services{};
};
std::int32_t Runtime::level_operation(void* opaque,sl::Character* character,const sl::Request* request,std::uint32_t* out){
    auto& context=*static_cast<Context*>(opaque);auto& storage=context.storage;auto& result=*context.result;
    switch(request->operation){
    case sl::Operation::get_number:
        if(!context.numbers.read_number){context.error=Status::number_unavailable;return 1;}
        ++result.number_reads;
        try{if(context.numbers.read_number(context.numbers.context,request->subject,out)){context.error=Status::number_failed;return 1;}}
        catch(...){context.error=Status::number_failed;return 1;}
        return 0;
    case sl::Operation::design_max_level:{
        ++result.design_reads;
        if(storage.design_count>10000||(!storage.designs&&storage.design_count)){context.error=Status::design_missing;return 1;}
        const dh2_pycst_view* selected=nullptr;bool found=false;
        for(std::uint32_t i=0;i<storage.design_count;++i)if(storage.designs[i].manager==request->subject){if(found){context.error=Status::design_missing;return 1;}found=true;selected=storage.designs[i].data;}
        dh2_pycst_result value{};
        if(!selected||dh2_pycst_get(selected,request->category,std::uint32_t(std::strlen(request->category)),request->key,std::uint32_t(std::strlen(request->key)),&value)||!value.found){context.error=Status::design_missing;return 1;}
        *out=std::uint32_t(value.value);return 0;
    }
    case sl::Operation::float_to_signed:{
        float number;std::memcpy(&number,&request->number_bits,4);
        if(!std::isfinite(number)||number< -2147483648.0f||number>=2147483648.0f){context.error=Status::unsupported_numeric;return 1;}
        *out=std::uint32_t(static_cast<std::int32_t>(number));return 0;
    }
    case sl::Operation::recalc_properties:
        // This publishes the original BASE[19]/Character+0x5b8 store before
        // invoking the source non-atomic live class/property kernel.
        storage.base[19]=signed_word(character->base_level_5b8);
        result.class_result=dh2_class_recalc_base(storage.classes,storage.class_count,storage.base,storage.view);
        if(result.class_result){context.error=Status::class_failed;return 1;}
        return 0;
    case sl::Operation::regen_hp:case sl::Operation::regen_mp:{
        const bool mana=request->operation==sl::Operation::regen_mp;
        if(mana)result.mp_attempted=1;else result.hp_attempted=1;
        context.captured_debug=storage.debug_globals?storage.debug_globals->singleton:nullptr;
        context.captured_debug_services=storage.debug_services?*storage.debug_services:debug_switches::Services{};
        rg::State state{storage.character,storage.properties,reinterpret_cast<std::uintptr_t>(storage.view->resolved)};
        rg::Globals globals{context.captured_debug?context.captured_debug->identity():0};rg::Services services{&context,regeneration_operation};
        const auto status=mana?rg::regen_mp(&state,&globals,request->argument,&services,&result.mp):rg::regen_hp(&state,&globals,request->argument,&services,&result.hp);
        if(status!=rg::Status::complete){if(context.error==Status::complete)context.error=Status::debug_failed;return 1;}
        return 0;
    }
    }
    context.error=Status::source_failed;return 1;
}
std::int32_t Runtime::regeneration_operation(void* opaque,const rg::Request* request,rg::Reply* reply){
    auto& context=*static_cast<Context*>(opaque);auto& storage=context.storage;
    switch(request->operation){
    case rg::Operation::read_property:
        if(request->property>=224||request->sheet!=reinterpret_cast<std::uintptr_t>(storage.view->resolved)){context.error=Status::property_failed;return 1;}
        reply->word=std::uint32_t(storage.view->resolved[request->property]);return 0;
    case rg::Operation::add_property:
        if(dh2_property_add(storage.view,std::int32_t(request->property),signed_word(request->amount))){context.error=Status::property_failed;return 1;}return 0;
    case rg::Operation::debug_load:
        if(!context.captured_debug||!storage.debug_globals||request->subject!=context.captured_debug->identity()||context.captured_debug->load(*storage.debug_globals,context.captured_debug_services)!=debug_switches::Status::complete){context.error=Status::debug_failed;return 1;}return 0;
    case rg::Operation::string_construct:
        try{auto string=std::make_unique<std::string>(request->text);const auto id=reinterpret_cast<std::uintptr_t>(string.get());context.runtime->strings_.emplace(id,std::move(string));reply->identity=id;return 0;}
        catch(...){context.error=Status::allocation_failed;return 1;}
    case rg::Operation::debug_query:{
        auto found=context.runtime->strings_.find(request->sheet);std::uint8_t value=0;
        if(found==context.runtime->strings_.end()||!context.captured_debug||!storage.debug_globals||request->subject!=context.captured_debug->identity()||context.captured_debug->get_switch(*found->second,*storage.debug_globals,context.captured_debug_services,value)!=debug_switches::Status::complete){context.error=Status::debug_failed;return 1;}
        reply->word=value;return 0;
    }
    case rg::Operation::string_destroy:
        if(context.runtime->strings_.erase(request->subject)!=1){context.error=Status::source_failed;return 1;}return 0;
    }
    context.error=Status::source_failed;return 1;
}
Status Runtime::set_level(const Storage* storage,const sl::Arguments* arguments,const sl::Globals* globals,const NumberServices* numbers,Result* result){
    Range controls[6];
    if(!range(this,sizeof(*this),alignof(Runtime),controls[0])||!range(storage,sizeof(*storage),alignof(Storage),controls[1])||!range(arguments,sizeof(*arguments),alignof(sl::Arguments),controls[2])||!range(globals,sizeof(*globals),alignof(sl::Globals),controls[3])||!range(numbers,sizeof(*numbers),alignof(NumberServices),controls[4])||!range(result,sizeof(*result),alignof(Result),controls[5]))return Status::invalid_argument;
    for(unsigned i=0;i<6;++i)for(unsigned j=0;j<i;++j)if(overlaps(controls[i],controls[j]))return Status::invalid_argument;
    if(busy_)return Status::busy;
    if(!storage->character||!storage->properties)return Status::invalid_argument;
    if(arguments->count&&arguments->first_type==3){
        Range view;
        if(!range(storage->view,sizeof(*storage->view),alignof(data::PropertyView),view))return Status::invalid_argument;
        for(auto control:controls)if(overlaps(control,view))return Status::invalid_argument;
        if(storage->view->base!=storage->base)return Status::invalid_argument;
        Range sheets[6];const auto& properties=*storage->view;
        const std::int32_t* addresses[6]={storage->base,properties.saved,properties.resolved,properties.defaults,properties.types,properties.gear};
        for(unsigned i=0;i<6;++i){
            if(!range(addresses[i],224*sizeof(std::int32_t),alignof(std::int32_t),sheets[i])||overlaps(view,sheets[i]))return Status::invalid_argument;
            for(auto control:controls)if(overlaps(control,sheets[i]))return Status::invalid_argument;
            for(unsigned j=0;j<i;++j)if((i<3||j<3)&&overlaps(sheets[i],sheets[j]))return Status::invalid_argument;
        }
    }
    *result={};busy_=true;struct BusyGuard{bool& busy;~BusyGuard(){busy=false;}} guard{busy_};
    Context context{this,*storage,*numbers,result};sl::Character character{storage->character,storage->properties,0};sl::Services services{&context,level_operation};
    const auto status=sl::set_level(&character,arguments,globals,&services,&result->level);
    if(status==sl::Status::complete)return Status::complete;
    if(context.error!=Status::complete)return context.error;
    return status==sl::Status::invalid_argument?Status::invalid_argument:Status::source_failed;
}
Status Runtime::set_level_fixed(const Storage* storage,const sl::Globals* globals,float raw,Result* result){
    sl::Arguments arguments{reinterpret_cast<std::uintptr_t>(&raw),1,3};NumberServices numbers{&raw,fixed_number};
    return set_level(storage,&arguments,globals,&numbers,result);
}
Status Runtime::initialize_hp_mp(const Storage* storage,Result* result){
    Range controls[3];
    if(!range(this,sizeof(*this),alignof(Runtime),controls[0])||
       !range(storage,sizeof(*storage),alignof(Storage),controls[1])||
       !range(result,sizeof(*result),alignof(Result),controls[2]))return Status::invalid_argument;
    for(unsigned i=0;i<3;++i)for(unsigned j=0;j<i;++j)
        if(overlaps(controls[i],controls[j]))return Status::invalid_argument;
    if(busy_)return Status::busy;
    if(!storage->character||!storage->properties)return Status::invalid_argument;
    Range view;
    if(!range(storage->view,sizeof(*storage->view),alignof(data::PropertyView),view)||
       storage->view->base!=storage->base)return Status::invalid_argument;
    for(auto control:controls)if(overlaps(control,view))return Status::invalid_argument;
    Range sheets[6];const auto& properties=*storage->view;
    const std::int32_t* addresses[6]={storage->base,properties.saved,properties.resolved,
                                    properties.defaults,properties.types,properties.gear};
    for(unsigned i=0;i<6;++i){
        if(!range(addresses[i],224*sizeof(std::int32_t),alignof(std::int32_t),sheets[i])||
           overlaps(view,sheets[i]))return Status::invalid_argument;
        for(auto control:controls)if(overlaps(control,sheets[i]))return Status::invalid_argument;
        for(unsigned j=0;j<i;++j)if((i<3||j<3)&&overlaps(sheets[i],sheets[j]))return Status::invalid_argument;
    }
    *result={};busy_=true;
    struct BusyGuard{bool& busy;~BusyGuard(){busy=false;}} guard{busy_};
    Context context{this,*storage,{},result};
    namespace init=character_init_hp_mp;
    init::Owner owner{storage->character};
    const init::Services services{&context,
        [](void* raw,const init::Request* request)->std::int32_t {
            auto& captured=*static_cast<Context*>(raw);
            if(request->character!=captured.storage.character)return 1;
            const sl::Request bridge{sl::Operation::regen_hp,request->character,
                                     request->raw_amount,0,nullptr,nullptr};
            std::uint32_t unused=0;return level_operation(raw,nullptr,&bridge,&unused);
        },
        [](void* raw,const init::Request* request)->std::int32_t {
            auto& captured=*static_cast<Context*>(raw);
            if(request->character!=captured.storage.character)return 1;
            const sl::Request bridge{sl::Operation::regen_mp,request->character,
                                     request->raw_amount,0,nullptr,nullptr};
            std::uint32_t unused=0;return level_operation(raw,nullptr,&bridge,&unused);
        }};
    const auto status=init::execute(&owner,&services,&result->init);
    if(status==init::Status::complete)return Status::complete;
    return context.error==Status::complete?Status::source_failed:context.error;
}
} // namespace dh2::character_level_runtime
