#include "player_skill_session_v1.hpp"
#include "script_object_methods_v1.hpp"
#include "../adam-script-runtime/script_function_alias.h"
#include "../lua-numeric/numeric.h"
#include <cstdio>
#include <cstring>
#include <set>
#include <stdexcept>
#include <string>

namespace dh2::player_skill_session_v1 {
namespace prep=character_player_skills_preparation_v3;
namespace src=prep::source;
namespace {
struct Busy {bool& flag;explicit Busy(bool& f):flag(f){flag=true;}~Busy(){flag=false;}};
bool valid_path(const std::string& s){return !s.empty() && s.size()<4096 && s.find('\0')==std::string::npos;}
bool span(const void* p,std::size_t n,std::size_t a){auto v=reinterpret_cast<std::uintptr_t>(p);return p && v%a==0 && v<=UINTPTR_MAX-n;}
bool overlap(const void* p,std::size_t n,const void* q,std::size_t m){auto a=reinterpret_cast<std::uintptr_t>(p),b=reinterpret_cast<std::uintptr_t>(q);return a<=b?b-a<n:a-b<m;}
void number(dh2_script_value& v,float n){v={};v.type=DH2_SCRIPT_NUMBER;v.number=n;}
int discard(void*,const dh2_script_first_return_v1*,char*,std::size_t){return 0;}
// Both transports hand the callback the same method-bearing GameObject tables
// (`__dh2_player_object`, installed by bind_character) that GetSelf and
// GetTargetListTop return, so `attacker:GetID()` resolves in a stolen
// OnTargetHit handler exactly like `target:PlayFX(...)` in a spell script.
constexpr char combat_identity_transport[] = R"lua(
function __dh2_player_combat(callback_name, attacker, defender)
    return _G[callback_name](__dh2_player_object(attacker), __dh2_player_object(defender))
end
)lua";
constexpr char projectile_identity_transport[] = R"lua(
function __dh2_player_projectile_delayed(callback_name, target, skill_script)
    return _G[callback_name](__dh2_player_object(target), skill_script)
end
)lua";
// Player-VM tail of the shared GameObject table convention. The functions
// below are the Player-visible natives that return a GameObject UserData in the
// original (GameObject::_GetSelf pushUserData(this); GameObject::_GetTargetListTop
// pushUserData(top) followed by distance, angle, IsCharacter and the extra
// word, or pushPointer(nullptr) for an empty list; GameObject::_Summon
// pushUserData(the new Character), or nothing when the request is rejected).
constexpr char player_object_functions[] = R"lua(
__dh2_player_object = __dh2_object
local __dh2_native_GetSelf = GetSelf
function GetSelf()
    local identity = __dh2_native_GetSelf()
    if identity == nil then return nil end
    return __dh2_object(identity)
end
local __dh2_native_GetTargetListTop = GetTargetListTop
function GetTargetListTop(...)
    local identity, distance, angle, is_character, extra = __dh2_native_GetTargetListTop(...)
    -- An empty list returns the null Pointer alone.
    if distance == nil then return identity end
    return __dh2_object(identity), distance, angle, is_character, extra
end
local __dh2_native_Summon = Summon
function Summon(...)
    local identity = __dh2_native_Summon(...)
    if identity == nil then return nil end
    return __dh2_object(identity)
end
)lua";
int discard_returns(void*,const dh2_script_first_return_v1*,std::uint32_t,
                    char*,std::size_t){return 0;}
}
void VmDeleter::operator()(dh2_script_vm* p)const noexcept{dh2_script_vm_destroy(p);}
struct Session::Impl {
    struct Binding {Impl* self;NativeRequest request;};
    Vm vm;Configuration config;dh2_script_aliases* aliases=nullptr;
    lua_script_load_once::State cache;
    std::vector<std::unique_ptr<Binding>> bindings;
    std::vector<Resource> resources;
    Statistics stats;std::string path,error;Stage stage=Stage::created;bool busy=false;
    bool combat_identity_transport_loaded=false;
    bool projectile_identity_transport_loaded=false;
    Impl(Vm v,Configuration c):vm(std::move(v)),config(std::move(c)),path(config.initial_path){}
    ~Impl(){busy=true;vm.reset();config.providers.lifetime.reset();dh2_script_alias_destroy(aliases);}
    int failure(char* text,std::size_t size,const char* why){
        ++stats.required_failures;if(text && size)std::snprintf(text,size,"%s",why);
        return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
    }
    static int native(void* context,const dh2_script_value* args,std::uint32_t count,
                      dh2_script_value* out,std::uint32_t capacity,std::uint32_t* returned,char* text,std::size_t size) noexcept {
        auto& b=*static_cast<Binding*>(context);auto& s=*b.self;++s.stats.native_calls;
        if(!returned || (count && !args))return s.failure(text,size,"invalid native callback controls");
        *returned=0;
        try {
            if(b.request.domain==Domain::ais) {
                using F=ais_native_bindings::Function;auto f=b.request.ais_function;
                if(f==F::add_to_vf_table || f==F::push_vf_table || f==F::pop_vf_table) {
                    int status=f==F::add_to_vf_table?dh2_script_alias_add_values(s.aliases,args,count):
                        f==F::push_vf_table?dh2_script_alias_push(s.aliases):dh2_script_alias_pop(s.aliases);
                    return status?s.failure(text,size,"source VFTable operation failed"):0;
                }
                int operation=f==F::to_fixed?DH2_TO_FIXED:f==F::from_fixed?DH2_FROM_FIXED:
                    f==F::mul_fixed?DH2_MUL_FIXED:f==F::div_fixed?DH2_DIV_FIXED:f==F::bit_not?DH2_BIT_NOT:
                    f==F::bit_and?DH2_BIT_AND:f==F::bit_or?DH2_BIT_OR:f==F::bit_xor?DH2_BIT_XOR:-1;
                bool numeric=operation>=0 && count<=256;
                for(std::uint32_t i=0;numeric && i<count;++i)numeric=args[i].type==DH2_SCRIPT_NUMBER;
                if(numeric) {
                    float input[256];for(std::uint32_t i=0;i<count;++i)input[i]=args[i].number;
                    dh2_lua_numeric_result value{};
                    if(dh2_lua_numeric(std::uint32_t(operation),input,count,&value) || capacity<value.count || (value.count && !out))
                        return s.failure(text,size,"unsupported source numeric domain");
                    if(value.count)number(out[0],float(value.integer));
                    if(value.count==2)number(out[1],value.number);
                    *returned=value.count;return 0;
                }
            }
            if(!s.config.providers.native)return s.failure(text,size,b.request.name);
            int status=s.config.providers.native(s.config.providers.context,b.request,args,count,out,capacity,returned,text,size);
            if(status==DH2_SCRIPT_REQUIRED_SERVICE_FAILURE){++s.stats.required_failures;return status;}
            if(status<0) {
                // Keep a reason the provider already wrote; fall back to the native's name.
                if(text && size && text[0]){++s.stats.required_failures;return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;}
                return s.failure(text,size,b.request.name);
            }
            return status;
        }catch(...){return s.failure(text,size,"native player provider exception");}
    }
    int bind(NativeRequest request,const char* global_name=nullptr) {
        auto b=std::make_unique<Binding>();b->self=this;b->request=request;
        auto* live=b.get();bindings.push_back(std::move(b));
        return dh2_script_vm_bind_source_values(vm.get(),global_name?global_name:request.name,native,live);
    }
    static int open_library(void* p,std::uintptr_t id,ais_native_bindings::Library l) {
        auto& s=*static_cast<Impl*>(p);if(id!=reinterpret_cast<std::uintptr_t>(s.vm.get()))return -1;
        return dh2_script_vm_open_library(s.vm.get(),static_cast<dh2_script_library>(l));
    }
    static int bind_ais(void* p,std::uintptr_t binder,const ais_native_bindings::Binding* b,std::uintptr_t userdata) {
        auto& s=*static_cast<Impl*>(p);if(binder!=reinterpret_cast<std::uintptr_t>(&s))return -1;
        int status=s.bind({Domain::ais,b->name,b->function,{},s.config.character,userdata});
        if(!status)++s.stats.ais_bindings;return status;
    }
    int bind_ais() {
        ais_native_bindings::State state{config.ais->ais,reinterpret_cast<std::uintptr_t>(vm.get()),reinterpret_cast<std::uintptr_t>(this)};
        ais_native_bindings::Services services{this,open_library,bind_ais};ais_native_bindings::Result result{};
        return ais_native_bindings::bind_all(&state,&services,&result)==ais_native_bindings::Status::complete?0:-1;
    }
    int bind_character() {
        // Source Kind::method rows. `GetID` keeps the shared prelude's
        // `self._this` (the method is the receiver's own Pointer); every other
        // row becomes a hidden native installed into the object method table.
        std::set<std::string> methods;
        for(int group=0;group<2;++group) {
            std::size_t count=0;auto* table=group?character_native_bindings::character_own_bindings(&count):character_native_bindings::game_object_bindings(&count);
            for(std::size_t i=0;i<count;++i)if(table[i].kind==character_native_bindings::Kind::function) {
                auto userdata=table[i].context==character_native_bindings::Context::character?config.character:0;
                if(bind({Domain::character,table[i].name,{},table[i].function,config.character,userdata}))return -1;
                ++stats.character_function_bindings;
            } else if(std::strcmp(table[i].name,"GetID")!=0 && methods.insert(table[i].name).second) {
                const std::string global=std::string("__dh2_method_")+table[i].name;
                if(bind({Domain::character,table[i].name,{},table[i].function,config.character,0,true},global.c_str()))return -1;
                ++stats.method_bindings;
            }
        }
        std::string chunk(DH2_SCRIPT_OBJECT_METHODS_PRELUDE_V1);
        for(const auto& name:methods)chunk+="__dh2_object_methods."+name+" = __dh2_method_"+name+"\n";
        chunk+=player_object_functions;
        if(dh2_script_vm_load(vm.get(),chunk.data(),chunk.size(),"player-gameobject-method-table")){
            error=dh2_script_vm_error(vm.get());
            if(error.empty())error="Player GameObject method table load failed";
            return -1;
        }
        return 0;
    }
    static std::int32_t load_vm(void* p,std::uintptr_t id,const void* bytes,std::size_t n,const char*) {
        auto& s=*static_cast<Impl*>(p);if(id!=reinterpret_cast<std::uintptr_t>(s.vm.get()))return -1;
        ++s.stats.load_calls;return dh2_script_vm_load_source_file(s.vm.get(),bytes,n);
    }
    int load(const std::string& requested,LoadResult* result) {
        Resource asset;
        if(!config.providers.resolve){error="missing source resource resolver";return -1;}
        ++stats.resolutions;
        if(config.providers.resolve(config.providers.context,requested,asset,error) || !valid_path(asset.resolved_path) ||
           !asset.bytes || asset.bytes->empty() || asset.bytes->size()>8388608){if(error.empty())error="invalid retained source resource";return -1;}
        resources.push_back(std::move(asset));const auto& live=resources.back();
        lua_script_load_once::Source source{reinterpret_cast<std::uintptr_t>(vm.get()),live.resolved_path.c_str(),live.bytes->data(),live.bytes->size()};
        lua_script_load_once::Services services{this,load_vm};auto epoch=dh2_script_vm_required_failure_epoch(vm.get());
        auto status=lua_script_load_once::load_once(&cache,&source,&services,result);
        if(result->cache_hit)++stats.cache_hits;
        error=dh2_script_vm_error(vm.get());
        if(dh2_script_vm_required_failure_epoch(vm.get())!=epoch){if(error.empty())error="required native service failure caught by Lua";return -5;}
        if(status==lua_script_load_once::Status::complete)return 0;
        if(status==lua_script_load_once::Status::load_failed)return result->vm_status>0?0:result->vm_status;
        if(error.empty())error="source load/cache provider failed";
        return -1;
    }
    int call(const char* name,const dh2_script_value* args,std::uint32_t count,std::uint32_t index,
             dh2_script_return_observer_v1 observer,void* context) {
        const char* alias=dh2_script_alias_resolve(aliases,name);if(!alias){error="invalid source callback alias";return -1;}
        std::string captured(alias);
        int status=dh2_script_vm_call_indexed_source_v3(vm.get(),captured.c_str(),args,count,index,observer,context);
        error=dh2_script_vm_error(vm.get());if(status==-5 && error.empty())error="required native service failure caught by Lua";
        return status;
    }
    int call_all(const char* name,const dh2_script_value* args,std::uint32_t count,
                 dh2_script_returns_observer_v1 observer,void* context) {
        const char* alias=dh2_script_alias_resolve(aliases,name);if(!alias){error="invalid source callback alias";return -1;}
        std::string captured(alias);
        int status=dh2_script_vm_call_all_source_v1(vm.get(),captured.c_str(),args,count,observer,context);
        error=dh2_script_vm_error(vm.get());if(status==-5 && error.empty())error="required native service failure caught by Lua";
        return status;
    }
    static int contains(void* p,ais_player_init_vcb::State*,const char* name,bool* out) {
        int member=dh2_script_alias_contains(static_cast<Impl*>(p)->aliases,name);if(member<0)return -1;*out=member!=0;return 0;
    }
    int vcb(ais_player_init_vcb::Result* out) {
        ais_player_init_vcb::Services services{this,contains};++stats.init_vcb_calls;
        auto status=ais_player_init_vcb::initialize(config.ais,&services,out);
        if(status!=ais_player_init_vcb::Status::complete){error="source Player InitVCB failed";return -1;}
        return 0;
    }
    std::string script(std::uintptr_t id)const {
        for(const auto& row:config.tables->skills().skills)if(id==reinterpret_cast<std::uintptr_t>(row.script.c_str()))return row.script;
        for(const auto& row:config.tables->faeries().faeries)if(id==reinterpret_cast<std::uintptr_t>(row.spell_script.c_str()))return row.spell_script;
        throw std::invalid_argument("unowned script name identity");
    }
    static std::int32_t invoke(void* p,src::State* state,const src::Request* q,const prep::Arguments* args,src::Response* out) {
        auto& s=*static_cast<Impl*>(p);
        if(s.busy || s.stage!=Stage::character_bound || !span(state,sizeof(*state),alignof(src::State)) ||
           !span(q,sizeof(*q),alignof(src::Request)) || !s.output(out,sizeof(*out),alignof(src::Response)) ||
           overlap(out,sizeof(*out),state,sizeof(*state)) || overlap(out,sizeof(*out),q,sizeof(*q)) ||
           overlap(out,sizeof(*out),s.config.ais,sizeof(*s.config.ais)))return -1;
        Busy busy(s.busy);s.error.clear();
        try {
            if(state->owner!=s.config.character || state->ai!=s.config.ais->ais){s.error="Character/Player AIS receiver changed";return -1;}
            using Op=src::Operation;
            if(q->operation!=Op::debug_load && q->operation!=Op::debug_get_switch && q->receiver!=state->ai)return -1;
            switch(q->operation) {
            case Op::debug_load:
                if(!s.config.debug || !s.config.debug->singleton){s.error="missing Debug runtime";return -1;}
                {auto status=s.config.debug->singleton->load(*s.config.debug,s.config.debug_services);if(status!=debug_switches::Status::complete){s.error="Debug load status "+std::to_string(int(status));return -1;}return 0;}
            case Op::debug_get_switch: {
                if(!s.config.debug || !s.config.debug->singleton || !q->text || q->text_size>4096)return -1;
                std::uint8_t value=0;
                auto status=s.config.debug->singleton->get_switch(std::string(q->text,q->text_size),*s.config.debug,s.config.debug_services,value);
                if(status!=debug_switches::Status::complete){s.error="Debug query status "+std::to_string(int(status));return -1;}
                out->word=value;return 0;
            }
            case Op::capture_script_path:out->path=s.path.c_str();out->path_size=s.path.size();return 0;
            case Op::set_script_path: {
                const auto* value=q->text?q->text:q->saved_path;
                const auto size=q->text?q->text_size:q->saved_path_size;
                if(!value || size>=4096)return -1;
                std::string next(value,size);if(!valid_path(next))return -1;s.path=std::move(next);return 0;
            }
            case Op::load_script: {
                if(q->text && q->text_size>=4096)return -1;
                const auto name=q->text?std::string(q->text,q->text_size):s.script(q->script_name);
                if(name.empty() || name.find_first_of("/\\.\0",0,4)!=std::string::npos)return -1;
                LoadResult loaded{};int status=s.load(s.path+name+".luac",&loaded);out->loaded=loaded.source_success;return status;
            }
            case Op::call_script: {
                if(!q->text || std::string(q->text,q->text_size)!="DeclareSkill")return -1;
                dh2_script_value values[2]{};std::uint32_t count=0;
                if(args) {
                    if(!span(args,sizeof(*args),alignof(prep::Arguments)) || args->identity!=q->arguments || args->values.size()!=2 ||
                       args->values[0].type!=prep::Value::Type::string || args->values[1].type!=prep::Value::Type::number)return -1;
                    values[0].type=DH2_SCRIPT_STRING;values[0].text=args->values[0].text.c_str();values[0].text_bytes=args->values[0].text.size();
                    float n;std::memcpy(&n,&args->values[1].word,4);number(values[1],n);count=2;
                }else if(q->arguments)return -1;
                int status=s.call("DeclareSkill",values,count,0,discard,nullptr);
                if(!status)++s.stats.declarations;return status? -1:0;
            }
            case Op::init_vcb: {ais_player_init_vcb::Result result{};return s.vcb(&result);}
            default:return -1;
            }
        }catch(...){s.error="player preparation dependency exception";return -1;}
    }
    bool output(const void* p,std::size_t n,std::size_t alignment)const {
        if(!span(p,n,alignment) || overlap(p,n,this,sizeof(*this)) || overlap(p,n,config.ais,sizeof(*config.ais)))return false;
        if(config.debug && (overlap(p,n,config.debug,sizeof(*config.debug)) ||
           (config.debug->singleton && overlap(p,n,config.debug->singleton,sizeof(*config.debug->singleton)))))return false;
        for(const auto& asset:resources)if(overlap(p,n,asset.bytes->data(),asset.bytes->size()))return false;
        return true;
    }
};
Session::Session(std::unique_ptr<Impl> p):impl_(std::move(p)){}
Session::~Session()=default;
std::unique_ptr<Session> Session::adopt(Vm vm,Configuration config,std::string& error) {
    if(!vm || !config.character || !config.tables || !span(config.ais,sizeof(*config.ais),alignof(ais_player_init_vcb::State)) ||
       !config.ais->ais || !valid_path(config.initial_path)){vm.reset();error="invalid VM/Character/Player AIS/tables/path";return {};}
    try {
        auto p=std::make_unique<Impl>(std::move(vm),std::move(config));
        p->aliases=dh2_script_alias_create();
        if(!p->aliases || lua_script_load_once::reset(&p->cache,reinterpret_cast<std::uintptr_t>(p->vm.get()))!=lua_script_load_once::Status::complete){error="source VM cache/alias allocation failed";return {};}
        auto session=std::unique_ptr<Session>(new Session(std::move(p)));error.clear();return session;
    }catch(...){error="source VM ownership allocation failed";return {};}
}
int Session::bind_ais_functions(std::string& error) {
    if(impl_->busy || impl_->stage!=Stage::created)return -1;
    Busy busy(impl_->busy);impl_->error.clear();int status=-1;
    try{status=impl_->bind_ais();}catch(...){impl_->error="AIS registration exception";}
    impl_->stage=status?Stage::faulted:Stage::ais_bound;
    if(status && impl_->error.empty())impl_->error=dh2_script_vm_error(impl_->vm.get());
    error=impl_->error;return status;
}
int Session::bind_character_functions(std::string& error) {
    if(impl_->busy || impl_->stage!=Stage::ais_bound)return -1;
    Busy busy(impl_->busy);impl_->error.clear();int status=-1;
    try{status=impl_->bind_character();}catch(...){impl_->error="Character registration exception";}
    impl_->stage=status?Stage::faulted:Stage::character_bound;error=impl_->error;return status;
}
prep::Services Session::preparation_services(){prep::Services s;s.context=impl_.get();s.invoke=Impl::invoke;s.faery=impl_->config.providers.faery;s.lifetime=impl_->config.providers.lifetime;return s;}
int Session::load_resolved(const std::string& requested,LoadResult* out,std::string& error) {
    if(impl_->busy || impl_->stage!=Stage::character_bound || !valid_path(requested) || !impl_->output(out,sizeof(*out),alignof(LoadResult)) || overlap(out,sizeof(*out),this,sizeof(*this)))return -1;
    Busy busy(impl_->busy);impl_->error.clear();
    try{const std::string captured=requested;int status=impl_->load(captured,out);error=impl_->error;return status;}
    catch(...){error="source resource dependency exception";return -1;}
}
int Session::call(const char* name,const dh2_script_value* args,std::uint32_t count,std::uint32_t index,
                  dh2_script_return_observer_v1 observer,void* context,std::string& error) {
    if(impl_->busy || impl_->stage!=Stage::character_bound || !name || !observer)return -1;
    Busy busy(impl_->busy);impl_->error.clear();
    try{int status=impl_->call(name,args,count,index,observer,context);error=impl_->error;return status;}
    catch(...){error="source callback dependency exception";return -1;}
}
int Session::initialize_vcb(ais_player_init_vcb::Result* out,std::string& error) {
    if(impl_->busy || impl_->stage!=Stage::character_bound || !impl_->output(out,sizeof(*out),alignof(ais_player_init_vcb::Result)) || overlap(out,sizeof(*out),this,sizeof(*this)))return -1;
    Busy busy(impl_->busy);impl_->error.clear();int status=impl_->vcb(out);error=impl_->error;return status;
}
int Session::dispatch_combat_result(std::uintptr_t ais,
    ais_combat_result_dispatch_v1::Callback callback,
    std::uintptr_t attacker,std::uintptr_t defender,std::string& error) {
    if(impl_->busy || impl_->stage!=Stage::character_bound || !ais ||
       ais!=impl_->config.ais->ais || !attacker || !defender || attacker==defender ||
       (attacker!=impl_->config.character && defender!=impl_->config.character))return -1;
    Busy busy(impl_->busy);impl_->error.clear();
    try {
        const char* requested=callback==ais_combat_result_dispatch_v1::Callback::target_hit?
            "OnTargetHit":callback==ais_combat_result_dispatch_v1::Callback::target_missed?
            "OnTargetMissed":nullptr;
        if(!requested){impl_->error="unsupported Player combat callback";error=impl_->error;return -1;}
        const char* resolved=dh2_script_alias_resolve(impl_->aliases,requested);
        if(!resolved){impl_->error="Player combat callback alias is absent";error=impl_->error;return -1;}
        const std::string captured(resolved);
        if(!impl_->combat_identity_transport_loaded) {
            const int loaded=dh2_script_vm_load(impl_->vm.get(),combat_identity_transport,
                sizeof(combat_identity_transport)-1,"player-neutral-combat-identity-transport");
            if(loaded){impl_->error=dh2_script_vm_error(impl_->vm.get());
                if(impl_->error.empty())impl_->error="Player combat identity transport load failed";
                ++impl_->stats.combat_callback_failures;error=impl_->error;return loaded;}
            impl_->combat_identity_transport_loaded=true;
        }
        dh2_script_value arguments[3]{};
        arguments[0].type=DH2_SCRIPT_STRING;
        arguments[0].text=captured.c_str();
        arguments[0].text_bytes=captured.size();
        arguments[1].type=DH2_SCRIPT_IDENTITY;arguments[1].identity=attacker;
        arguments[2].type=DH2_SCRIPT_IDENTITY;arguments[2].identity=defender;
        ++impl_->stats.combat_callback_calls;
        const int status=dh2_script_vm_call_discard_source(impl_->vm.get(),
            "__dh2_player_combat",arguments,3);
        impl_->error=dh2_script_vm_error(impl_->vm.get());
        if(status) {
            ++impl_->stats.combat_callback_failures;
            if(status==-5&&impl_->error.empty())impl_->error="required Player combat service failure caught by Lua";
        }
        error=impl_->error;return status;
    }catch(...){
        ++impl_->stats.combat_callback_failures;error="Player combat callback dependency exception";return -1;
    }
}
int Session::call_all(const char* name,const dh2_script_value* args,std::uint32_t count,
                     dh2_script_returns_observer_v1 observer,void* context,std::string& error) {
    if(impl_->busy || impl_->stage!=Stage::character_bound || !name || !observer)return -1;
    Busy busy(impl_->busy);impl_->error.clear();
    try{int status=impl_->call_all(name,args,count,observer,context);error=impl_->error;return status;}
    catch(...){error="source callback dependency exception";return -1;}
}
int Session::call_projectile_skill(const char* callback_name,
    const dh2_script_value* set_skill_arguments,std::uint32_t count,
    std::uintptr_t target_identity,std::uintptr_t projectile_identity,
    dh2_script_returns_observer_v1 observer,void* context,std::string& error) {
    auto& s=*impl_;
    if(s.busy || s.stage!=Stage::character_bound || !set_skill_arguments ||
       count!=2 || !target_identity || !projectile_identity || !callback_name || !observer ||
       (std::strcmp(callback_name,"OnDelayedSkill")!=0 &&
        std::strcmp(callback_name,"OnDelayedSkillCheck")!=0) ||
       set_skill_arguments[0].type!=DH2_SCRIPT_STRING ||
       !set_skill_arguments[0].text ||
       set_skill_arguments[1].type!=DH2_SCRIPT_NUMBER) {
        error="invalid same-VM projectile skill callback request";return -1;
    }
    const char* set_skill_alias=dh2_script_alias_resolve(s.aliases,"SetSkill");
    if(!set_skill_alias) {
        error="projectile SetSkill alias is absent";return -1;
    }
    Busy busy(s.busy);s.error.clear();
    try {
        const std::string set_skill_name(set_skill_alias);
        const auto discard=dh2_script_vm_call_all_source_v1(
            s.vm.get(),set_skill_name.c_str(),set_skill_arguments,count,
            discard_returns,nullptr);
        s.error=dh2_script_vm_error(s.vm.get());
        // CharAISkillScript returns zero immediately when SetSkill errors. Its
        // ReturnValues are cleared before the delayed callback and never gate it.
        if(discard!=0) {
            if(discard==-5 && s.error.empty())
                s.error="required Player SetSkill service failure caught by Lua";
            error=s.error;return discard;
        }
        // Resolve the second source name only after SetSkill has run, so any
        // alias-table mutation it performs is visible to the delayed call.
        const char* delayed_alias=dh2_script_alias_resolve(s.aliases,callback_name);
        if(!delayed_alias) {
            error="delayed projectile callback alias is absent";return -1;
        }
        const std::string delayed_name(delayed_alias);
        if(!s.projectile_identity_transport_loaded) {
            const int loaded=dh2_script_vm_load(s.vm.get(),projectile_identity_transport,
                sizeof(projectile_identity_transport)-1,
                "player-projectile-identity-transport");
            if(loaded) {
                s.error=dh2_script_vm_error(s.vm.get());
                if(s.error.empty())s.error="Player projectile identity transport load failed";
                error=s.error;return loaded;
            }
            s.projectile_identity_transport_loaded=true;
        }
        dh2_script_value arguments[3]{};
        arguments[0].type=DH2_SCRIPT_STRING;
        arguments[0].text=delayed_name.c_str();
        arguments[0].text_bytes=delayed_name.size();
        arguments[1].type=DH2_SCRIPT_IDENTITY;arguments[1].identity=target_identity;
        arguments[2].type=DH2_SCRIPT_IDENTITY;arguments[2].identity=projectile_identity;
        const auto delayed=dh2_script_vm_call_all_source_v1(s.vm.get(),
            "__dh2_player_projectile_delayed",arguments,3,
            observer,context);
        s.error=dh2_script_vm_error(s.vm.get());
        if(delayed==-5 && s.error.empty())
            s.error="required Player delayed projectile service failure caught by Lua";
        error=s.error;return delayed;
    } catch(...) {
        error="Player projectile callback dependency exception";return -1;
    }
}
dh2_script_vm* Session::vm()const noexcept{return impl_->vm.get();}
bool Session::has_source_lua_script()const noexcept{return impl_&&impl_->vm.get()!=nullptr;}
std::uintptr_t Session::character_identity()const noexcept{return impl_->config.character;}
std::uintptr_t Session::ais_identity()const noexcept{return impl_->config.ais->ais;}
Stage Session::stage()const noexcept{return impl_->stage;}
const std::string& Session::script_path()const noexcept{return impl_->path;}
const std::string& Session::last_error()const noexcept{return impl_->error;}
const Statistics& Session::statistics()const noexcept{return impl_->stats;}
std::size_t Session::loaded_path_count()const noexcept{return lua_script_load_once::loaded_path_count(&impl_->cache);}
bool Session::contains_path(const char* path)const noexcept{return lua_script_load_once::contains_path(&impl_->cache,path);}
bool Session::contains_alias(const char* name)const noexcept{return dh2_script_alias_contains(impl_->aliases,name)>0;}
} // namespace dh2::player_skill_session_v1
