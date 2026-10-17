#include "player_skill_vm_services.hpp"
#include "../adam-script-runtime/script_function_alias.h"
#include "../lua-numeric/numeric.h"
#include <cstdio>
#include <cstring>
#include <limits>

namespace dh2::player_skill_vm_services {
namespace prep=character_player_skills_preparation_v3;
namespace src=prep::source;
namespace {
struct Busy {bool& flag;explicit Busy(bool& f):flag(f){flag=true;}~Busy(){flag=false;}};
bool valid_path(const std::string& s){return !s.empty() && s.size()<=1048576 && s.find('\0')==std::string::npos;}
bool aligned(const void* p,std::size_t n,std::size_t a){auto v=reinterpret_cast<std::uintptr_t>(p);return p && v%a==0 && v<=UINTPTR_MAX-n;}
void number(dh2_script_value& v,float n){v={};v.type=DH2_SCRIPT_NUMBER;v.number=n;}
int discard(void*,const dh2_script_first_return_v1*,char*,std::size_t){return 0;}
}
struct Session::Impl {
    struct Binding {Impl* self;NativeRequest request;};
    Vm vm;Configuration config;dh2_script_aliases* aliases=nullptr;
    lua_script_load_once::State cache;
    std::vector<std::unique_ptr<Binding>> bindings;
    std::vector<Resource> resources;
    Statistics stats;std::string path,error;bool busy=false;
    explicit Impl(Vm v,Configuration c):vm(std::move(v)),config(std::move(c)),path(config.initial_path){}
    ~Impl(){busy=true;vm.reset();dh2_script_alias_destroy(aliases);}
    static int fail(char* text,std::size_t size,const char* why){if(text && size)std::snprintf(text,size,"%s",why);return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;}
    static int native(void* c,const dh2_script_value* args,std::uint32_t count,
                      dh2_script_value* out,std::uint32_t capacity,std::uint32_t* returned,char* text,std::size_t size) noexcept {
        auto& b=*static_cast<Binding*>(c);auto& self=*b.self;*returned=0;++self.stats.native_calls;
        try {
            if(b.request.domain==Domain::ais) {
                using Fn=ais_native_bindings::Function;auto f=b.request.ais_function;
                if(f==Fn::add_to_vf_table || f==Fn::push_vf_table || f==Fn::pop_vf_table) {
                    int status=f==Fn::add_to_vf_table?dh2_script_alias_add_values(self.aliases,args,count):
                               f==Fn::push_vf_table?dh2_script_alias_push(self.aliases):dh2_script_alias_pop(self.aliases);
                    if(!status)return 0;
                    ++self.stats.required_failures;return fail(text,size,"source VFTable operation failed");
                }
                const int operation=f==Fn::to_fixed?DH2_TO_FIXED:f==Fn::from_fixed?DH2_FROM_FIXED:
                    f==Fn::mul_fixed?DH2_MUL_FIXED:f==Fn::div_fixed?DH2_DIV_FIXED:f==Fn::bit_not?DH2_BIT_NOT:
                    f==Fn::bit_and?DH2_BIT_AND:f==Fn::bit_or?DH2_BIT_OR:f==Fn::bit_xor?DH2_BIT_XOR:-1;
                if(operation>=0) {
                    float input[16];for(std::uint32_t i=0;i<count;++i) {
                        if(args[i].type!=DH2_SCRIPT_NUMBER){++self.stats.required_failures;return fail(text,size,"numeric source coercion unsupported");}
                        input[i]=args[i].number;
                    }
                    dh2_lua_numeric_result value{};
                    if(dh2_lua_numeric(std::uint32_t(operation),input,count,&value) || capacity<value.count) {
                        ++self.stats.required_failures;return fail(text,size,"numeric source input unsupported");
                    }
                    if(value.count)number(out[0],float(value.integer));
                    if(value.count==2)number(out[1],value.number);
                    *returned=value.count;return 0;
                }
            }
            if(self.config.providers.native && !self.config.providers.native(self.config.providers.context,b.request,args,count,out,capacity,returned,text,size))return 0;
        }catch(...){if(text && size)std::snprintf(text,size,"%s","native skill provider exception");}
        ++self.stats.required_failures;
        if(!text || !size || !text[0])return fail(text,size,b.request.name);
        return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
    }
    int bind(const NativeRequest& request) {
        auto b=std::make_unique<Binding>();b->self=this;b->request=request;
        auto* retained=b.get();bindings.push_back(std::move(b));
        return dh2_script_vm_bind_source_values(vm.get(),request.name,native,retained);
    }
    static int open_library(void* p,std::uintptr_t vm_id,ais_native_bindings::Library l) {
        auto& s=*static_cast<Impl*>(p);if(vm_id!=reinterpret_cast<std::uintptr_t>(s.vm.get()))return -1;
        return dh2_script_vm_open_library(s.vm.get(),static_cast<dh2_script_library>(l));
    }
    static int bind_ais(void* p,std::uintptr_t,const ais_native_bindings::Binding* b,std::uintptr_t) {
        auto& s=*static_cast<Impl*>(p);return s.bind({Domain::ais,b->name,b->function,{},s.config.character});
    }
    bool initialize() {
        aliases=dh2_script_alias_create();if(!aliases)return false;
        if(lua_script_load_once::reset(&cache,reinterpret_cast<std::uintptr_t>(vm.get()))!=lua_script_load_once::Status::complete)return false;
        ais_native_bindings::State state{config.ais->ais,reinterpret_cast<std::uintptr_t>(vm.get()),reinterpret_cast<std::uintptr_t>(this)};
        ais_native_bindings::Services services{this,open_library,bind_ais};ais_native_bindings::Result result{};
        if(ais_native_bindings::bind_all(&state,&services,&result)!=ais_native_bindings::Status::complete)return false;
        for(int group=0;group<2;++group) {
            std::size_t count=0;auto* table=group?character_native_bindings::character_own_bindings(&count):character_native_bindings::game_object_bindings(&count);
            for(std::size_t i=0;i<count;++i)if(table[i].kind==character_native_bindings::Kind::function)
                if(bind({Domain::character,table[i].name,{},table[i].function,config.character}))return false;
        }
        return true;
    }
    static std::int32_t load_vm(void* p,std::uintptr_t id,const void* bytes,std::size_t size,const char*) {
        auto& s=*static_cast<Impl*>(p);if(id!=reinterpret_cast<std::uintptr_t>(s.vm.get()))return -1;
        ++s.stats.load_calls;return dh2_script_vm_load_source_file(s.vm.get(),bytes,size);
    }
    int load(const std::string& requested,LoadResult* result) {
        Resource asset;
        if(!config.providers.resolve){error="missing source resource resolver";return -1;}
        ++stats.resolutions;
        if(config.providers.resolve(config.providers.context,requested,asset,error) || !valid_path(asset.resolved_path) ||
           !asset.bytes || asset.bytes->size()>8388608){if(error.empty())error="invalid retained source resource";return -1;}
        // Retain bytes even on error; a resolver's application owners can retire
        // after this call without invalidating borrowed VM/provider resources.
        resources.push_back(std::move(asset));const auto& live=resources.back();
        lua_script_load_once::Source source{reinterpret_cast<std::uintptr_t>(vm.get()),live.resolved_path.c_str(),live.bytes->data(),live.bytes->size()};
        lua_script_load_once::Services services{this,load_vm};auto epoch=dh2_script_vm_required_failure_epoch(vm.get());
        auto status=lua_script_load_once::load_once(&cache,&source,&services,result);
        if(result->cache_hit)++stats.cache_hits;
        error=dh2_script_vm_error(vm.get());
        if(dh2_script_vm_required_failure_epoch(vm.get())!=epoch){if(error.empty())error="required native service failure caught by Lua";return -1;}
        return status==lua_script_load_once::Status::complete || status==lua_script_load_once::Status::load_failed?0:-1;
    }
    int call(const char* name,const dh2_script_value* args,std::uint32_t count,std::uint32_t index,
             dh2_script_return_observer_v1 observer,void* context) {
        const char* alias=dh2_script_alias_resolve(aliases,name);if(!alias)return -1;
        std::string captured(alias);
        int status=dh2_script_vm_call_indexed_source_v3(vm.get(),captured.c_str(),args,count,index,observer,context);
        error=dh2_script_vm_error(vm.get());if(status==-5 && error.empty())error="required native service failure caught by Lua";
        return status;
    }
    std::string script(std::uintptr_t id)const {
        for(const auto& row:config.tables->skills().skills)if(id==reinterpret_cast<std::uintptr_t>(row.script.c_str()))return row.script;
        for(const auto& row:config.tables->faeries().faeries)if(id==reinterpret_cast<std::uintptr_t>(row.spell_script.c_str()))return row.spell_script;
        throw std::invalid_argument("unowned script name identity");
    }
    static int contains(void* p,ais_external_init_vcb::State*,const char* name,bool* out) {
        int member=dh2_script_alias_contains(static_cast<Impl*>(p)->aliases,name);if(member<0)return -1;*out=member!=0;return 0;
    }
    static std::int32_t invoke(void* p,src::State* state,const src::Request* q,const prep::Arguments* args,src::Response* out) {
        auto& s=*static_cast<Impl*>(p);
        if(s.busy || !aligned(state,sizeof(*state),alignof(src::State)) || !aligned(q,sizeof(*q),alignof(src::Request)) ||
           !aligned(out,sizeof(*out),alignof(src::Response)))return -1;
        Busy busy(s.busy);s.error.clear();
        try {
            if(state->owner!=s.config.character || state->ai!=s.config.ais->ais)return -1;
            using Op=src::Operation;
            if(q->operation!=Op::debug_load && q->operation!=Op::debug_get_switch && q->receiver!=state->ai)return -1;
            switch(q->operation) {
            case Op::debug_load:
                if(!s.config.debug || !s.config.debug->singleton)return -1;
                return s.config.debug->singleton->load(*s.config.debug,s.config.debug_services)==debug_switches::Status::complete?0:-1;
            case Op::debug_get_switch: {
                if(!s.config.debug || !s.config.debug->singleton || !q->text)return -1;
                std::uint8_t value=0;
                if(s.config.debug->singleton->get_switch(std::string(q->text,q->text_size),*s.config.debug,s.config.debug_services,value)!=debug_switches::Status::complete)return -1;
                out->word=value;return 0;
            }
            case Op::capture_script_path:out->path=s.path.c_str();out->path_size=s.path.size();return 0;
            case Op::set_script_path: {
                if(!q->saved_path)return -1;std::string path(q->saved_path,q->saved_path_size);
                if(!valid_path(path))return -1;s.path=std::move(path);return 0;
            }
            case Op::load_script: {
                const auto name=q->text?std::string(q->text,q->text_size):s.script(q->script_name);
                if(name.empty() || name.find_first_of("/\\.\0",0,4)!=std::string::npos)return -1;
                LoadResult loaded{};int status=s.load(s.path+name+".luac",&loaded);out->loaded=loaded.source_success;return status;
            }
            case Op::call_script: {
                if(!q->text || std::string(q->text,q->text_size)!="DeclareSkill")return -1;
                dh2_script_value values[2]{};std::uint32_t count=0;
                if(args) {
                    if(!aligned(args,sizeof(*args),alignof(prep::Arguments)) || args->identity!=q->arguments || args->values.size()!=2 ||
                       args->values[0].type!=prep::Value::Type::string || args->values[1].type!=prep::Value::Type::number)return -1;
                    values[0].type=DH2_SCRIPT_STRING;values[0].text=args->values[0].text.c_str();values[0].text_bytes=args->values[0].text.size();
                    float n;std::memcpy(&n,&args->values[1].word,4);number(values[1],n);count=2;
                }else if(q->arguments)return -1;
                int status=s.call("DeclareSkill",values,count,0,discard,nullptr);
                if(!status)++s.stats.declarations;return status? -1:0;
            }
            case Op::init_vcb: {
                ais_external_init_vcb::Services services{&s,contains};ais_external_init_vcb::Result result{};++s.stats.init_vcb_calls;
                return ais_external_init_vcb::initialize_external(s.config.ais,&services,&result)==ais_external_init_vcb::Status::complete?0:-1;
            }
            default:return -1;
            }
        }catch(...){s.error="skill preparation dependency exception";return -1;}
    }
};
Session::Session(std::unique_ptr<Impl> p):impl_(std::move(p)){}
Session::~Session()=default;
std::unique_ptr<Session> Session::adopt(Vm vm,Configuration config,std::string& error) {
    error.clear();
    if(!vm || !config.character || !config.tables || !aligned(config.ais,sizeof(*config.ais),alignof(ais_external_init_vcb::State)) ||
       !config.ais->ais || !valid_path(config.initial_path)){error="invalid VM/actor/AIS/tables/path";return {};}
    try {
        auto p=std::make_unique<Impl>(std::move(vm),std::move(config));
        if(!p->initialize()){error=dh2_script_vm_error(p->vm.get());if(error.empty())error="source binding setup failed";return {};}
        return std::unique_ptr<Session>(new Session(std::move(p)));
    }catch(...){error="source VM ownership allocation failed";return {};}
}
prep::Services Session::preparation_services(){prep::Services s;s.context=impl_.get();s.invoke=Impl::invoke;s.faery=impl_->config.providers.faery;s.lifetime=impl_->config.providers.lifetime;return s;}
int Session::load_resolved(const std::string& requested,LoadResult* output,std::string& error) {
    if(impl_->busy || !valid_path(requested) || !aligned(output,sizeof(*output),alignof(LoadResult)))return -1;
    Busy busy(impl_->busy);impl_->error.clear();
    try {int status=impl_->load(requested,output);error=impl_->error;return status;}
    catch(...){error="source asset dependency exception";return -1;}
}
int Session::call(const char* name,const dh2_script_value* args,std::uint32_t count,std::uint32_t index,
                  dh2_script_return_observer_v1 observer,void* context,std::string& error) {
    if(impl_->busy || !name || !observer)return -1;
    Busy busy(impl_->busy);
    try {int status=impl_->call(name,args,count,index,observer,context);error=impl_->error;return status;}
    catch(...){error="source callback dependency exception";return -1;}
}
dh2_script_vm* Session::vm()const noexcept{return impl_->vm.get();}
const std::string& Session::script_path()const noexcept{return impl_->path;}
std::size_t Session::loaded_path_count()const noexcept{return lua_script_load_once::loaded_path_count(&impl_->cache);}
bool Session::contains_path(const char* path)const noexcept{return lua_script_load_once::contains_path(&impl_->cache,path);}
const Statistics& Session::statistics()const noexcept{return impl_->stats;}
} // namespace dh2::player_skill_vm_services
