#include "player_skill_update_session_v1.hpp"
#include "character_skill_state_queries.hpp"
#include <cstring>
#include <cstdio>
#include <map>
#include <stdexcept>

namespace dh2::player_skill_update_session_v1 {
namespace prep=character_player_skills_preparation_v3;
namespace all=character_ai_update_all_skills;
namespace selected=character_ai_update_skills;
namespace single=character_ai_skill_script_update;
namespace {
struct Busy {bool& flag;explicit Busy(bool& f):flag(f){flag=true;}~Busy(){flag=false;}};
all::ScriptVector range(const std::vector<std::uintptr_t>& v){
    const auto* b=v.empty()?nullptr:v.data();return {b,b?b+v.size():nullptr};
}
}
struct Runtime::Impl {
    struct Returns {
        single::ValueVector projection{};
        std::vector<dh2_script_value> values;
        std::vector<std::string> texts;
        void sync(){
            const auto b=values.empty()?0:reinterpret_cast<std::uintptr_t>(values.data());
            projection={b,b?b+values.size()*sizeof(dh2_script_value):0};
        }
    };
    player_skill_session_v1::Session& session;prep::Owner& owner;
    std::uintptr_t ai,character;const std::int32_t& machine;
    std::map<std::uintptr_t,std::unique_ptr<Returns>> returns;
    bool busy=false;
    std::uint32_t* callbacks=nullptr;
    std::uint32_t* lua_errors=nullptr;
    int* last_lua_status=nullptr;
    std::string error;
    Impl(player_skill_session_v1::Session& s,prep::Owner& p,std::uintptr_t a,
         std::uintptr_t c,const std::int32_t& m):session(s),owner(p),ai(a),character(c),machine(m){}
    static int observe(void* raw,const dh2_script_first_return_v1* v,std::uint32_t count,char* why,std::size_t n){
        auto& r=*static_cast<Returns*>(raw);
        if(!v&&count)return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
        try{
            r.values.clear();r.texts.clear();r.texts.resize(count);r.values.reserve(count);
            for(std::uint32_t i=0;i<count;++i){
                dh2_script_value value{};value.type=v[i].type;value.number=v[i].number;
                value.boolean=v[i].boolean;value.identity=v[i].identity;
                if(v[i].type==DH2_SCRIPT_STRING){
                    if(!v[i].text)throw std::runtime_error("source skill string unavailable");
                    r.texts[i].assign(v[i].text,v[i].text_bytes);
                    value.text=r.texts[i].c_str();value.text_bytes=r.texts[i].size();
                }
                r.values.push_back(value);
            }
            r.sync();return 0;
        }catch(...){
            r.sync();if(why&&n)std::snprintf(why,n,"source skill ReturnValues storage failed");
            return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
        }
    }
    int call(const char* fn,const prep::Arguments* arguments,Returns& r,single::ReturnValues& projection){
        std::vector<dh2_script_value> a;
        if(arguments)for(const auto& v:arguments->values){
            dh2_script_value value{};
            if(v.type==prep::Value::Type::string){value.type=DH2_SCRIPT_STRING;value.text=v.text.c_str();value.text_bytes=v.text.size();}
            else {value.type=DH2_SCRIPT_NUMBER;
                if(v.type==prep::Value::Type::number)std::memcpy(&value.number,&v.word,4);
                else {std::int32_t integer;std::memcpy(&integer,&v.word,4);value.number=static_cast<float>(integer);}}
            a.push_back(value);
        }
        const int status=session.call_all(fn,a.empty()?nullptr:a.data(),std::uint32_t(a.size()),observe,&r,error);
        if(!last_lua_status||!lua_errors)return -1;
        *last_lua_status=status;
        if(status<0)return -1;
        projection.error=static_cast<std::uint32_t>(status);
        if(status){++*lua_errors;r.values.clear();r.texts.clear();r.sync();}
        return 0;
    }
    static std::int32_t invoke_one(void* raw,single::State* state,const single::Request* q,single::ReturnValues* value){
        auto& s=*static_cast<Impl*>(raw);
        if(!state||!q||!value||!s.callbacks||state->identity!=q->skill)return -1;
        if(q->operation==single::Operation::construct_values){
            auto r=std::make_unique<Returns>();const auto id=reinterpret_cast<std::uintptr_t>(r.get());
            value->resource=id;value->error=0;value->values=&r->projection;
            s.returns.emplace(id,std::move(r));return 0;
        }
        auto found=s.returns.find(q->resource);
        if(found==s.returns.end()||value->resource!=q->resource||value->values!=&found->second->projection)return -1;
        auto& r=*found->second;
        switch(q->operation){
        case single::Operation::call_set_skill: {
            const auto* args=s.owner.instance_arguments(q->skill);
            if(!args||args->identity!=q->arguments||q->script!=reinterpret_cast<std::uintptr_t>(s.session.vm()))return -1;
            return s.call(q->function,args,r,*value);
        }
        case single::Operation::erase_values:
            if(q->first!=r.projection.begin||q->last!=r.projection.end)return -1;
            r.values.clear();r.texts.clear();r.sync();return 0;
        case single::Operation::call_on_skill_update:
            if(q->script!=reinterpret_cast<std::uintptr_t>(s.session.vm()))return -1;
            return s.call(q->function,nullptr,r,*value);
        case single::Operation::destroy_values:s.returns.erase(found);return 0;
        default:return -1;
        }
    }
    static std::int32_t invoke_all(void* raw,all::State* state,const all::Request* q,all::Response* value){
        auto& s=*static_cast<Impl*>(raw);
        if(!state||!q||!value||!s.callbacks||state->ai!=s.ai||state->owner!=s.character)return -1;
        if(q->operation!=all::Operation::on_skill_update){
            if(q->subject!=s.character)return -1;
            const character_skill_state_queries::Machine m{&s.machine};character_skill_state_queries::Result result{};
            const auto query=q->operation==all::Operation::is_using_skill?
                character_skill_state_queries::Query::using_skill:character_skill_state_queries::Query::casting;
            if(character_skill_state_queries::query(query,&m,&result)!=character_skill_state_queries::Status::complete)return -1;
            value->word=result.value;return 0;
        }
        const auto* instance=s.owner.instance(q->subject);
        if(!instance||instance->character!=s.character||instance->identity!=q->subject)return -1;
        single::Character character{s.character,reinterpret_cast<std::uintptr_t>(s.session.vm())};
        single::State projection{instance->identity,&character};single::Result result{};
        const single::Services services{&s,invoke_one};
        const auto status=single::update(&projection,&services,&result);
        if(status!=single::Status::complete){if(s.error.empty())s.error="source skill update failed";return -1;}
        ++*s.callbacks;return 0;
    }
    static std::int32_t invoke_selected_vector(void* raw,selected::State* state,
                                               selected::List list,
                                               selected::ScriptVector* output){
        auto& s=*static_cast<Impl*>(raw);
        if(!state||!output||!s.callbacks||state->ai!=s.ai||state->owner!=s.character)return -1;
        const auto which=list==selected::List::skill?prep::source::List::skill:
            list==selected::List::faery?prep::source::List::faery:prep::source::List::skill;
        if(list!=selected::List::skill&&list!=selected::List::faery)return -1;
        const auto& scripts=s.owner.slots(which);
        const auto* begin=scripts.empty()?nullptr:scripts.data();
        *output={begin,begin?begin+scripts.size():nullptr};
        return 0;
    }
    static std::int32_t invoke_selected_script(void* raw,selected::State* state,
                                               selected::List list,std::uint32_t index,
                                               std::uintptr_t script){
        auto& s=*static_cast<Impl*>(raw);
        if(!state||!s.callbacks||state->ai!=s.ai||state->owner!=s.character||!script)return -1;
        const auto which=list==selected::List::skill?prep::source::List::skill:
            list==selected::List::faery?prep::source::List::faery:prep::source::List::skill;
        if(list!=selected::List::skill&&list!=selected::List::faery)return -1;
        const auto& scripts=s.owner.slots(which);
        if(index>=scripts.size()||scripts[index]!=script)return -1;
        const auto* instance=s.owner.instance(script);
        if(!instance||instance->identity!=script||instance->character!=s.character)return -1;
        single::Character character{s.character,reinterpret_cast<std::uintptr_t>(s.session.vm())};
        single::State projection{script,&character};single::Result result{};
        const single::Services services{&s,invoke_one};
        const auto status=single::update(&projection,&services,&result);
        if(status!=single::Status::complete){if(s.error.empty())s.error="source selected-faery update failed";return -1;}
        ++*s.callbacks;return 0;
    }
};
Runtime::Runtime(player_skill_session_v1::Session& s,prep::Owner& p,std::uintptr_t ai,
                 std::uintptr_t character,const std::int32_t& machine):impl_(std::make_unique<Impl>(s,p,ai,character,machine)){
    if(!ai||!character||p.state().owner!=character||s.character_identity()!=character||!s.vm())throw std::invalid_argument("invalid player skill update owners");
}
Runtime::~Runtime()=default;
std::size_t Runtime::retained_failed_returns()const noexcept{return impl_->returns.size();}
int Runtime::update(Result& output,std::string& error){
    auto& s=*impl_;if(s.busy){error="player skill update reentry";return -1;}
    Busy busy(s.busy);output={};s.callbacks=&output.callbacks;s.lua_errors=&output.lua_errors;s.last_lua_status=&output.last_lua_status;s.error.clear();
    try{
        if(s.owner.state().owner!=s.character)throw std::runtime_error("player skill owner changed");
        all::State state{s.ai,s.character,range(s.owner.slots(prep::source::List::skill)),range(s.owner.slots(prep::source::List::faery))};
        const all::Services services{&s,Impl::invoke_all};
        output.status=all::update(&state,&services,&output.source);s.callbacks=nullptr;s.lua_errors=nullptr;s.last_lua_status=nullptr;error=s.error;
        return output.status==all::Status::complete?0:-1;
    }catch(const std::exception& e){s.callbacks=nullptr;s.lua_errors=nullptr;s.last_lua_status=nullptr;error=e.what();return -1;}
    catch(...){s.callbacks=nullptr;s.lua_errors=nullptr;s.last_lua_status=nullptr;error="player skill update exception";return -1;}
}
int Runtime::update_current_faery(const data::PlayerSavegameV1& savegame,
                                  const std::int32_t& difficulty,
                                  SelectedFaeryResult& output,std::string& error){
    auto& s=*impl_;if(s.busy){error="player selected-faery update reentry";return -1;}
    if(savegame.character()!=s.character||s.owner.state().owner!=s.character||
       s.session.character_identity()!=s.character||!s.session.vm()){
        error="selected-faery Save, Character, preparation owner and VM identities differ";return -1;
    }
    Busy busy(s.busy);output={};s.callbacks=&output.callbacks;s.lua_errors=&output.lua_errors;s.last_lua_status=&output.last_lua_status;s.error.clear();
    try{
        selected::State state{s.ai,s.character,&s.machine,&savegame,&difficulty};
        const selected::Services services{&s,Impl::invoke_selected_vector,Impl::invoke_selected_script};
        output.status=selected::update(&state,&services,&output.source);
        s.callbacks=nullptr;s.lua_errors=nullptr;s.last_lua_status=nullptr;error=s.error;
        return output.status==selected::Status::complete?0:-1;
    }catch(const std::exception& e){s.callbacks=nullptr;s.lua_errors=nullptr;s.last_lua_status=nullptr;error=e.what();return -1;}
    catch(...){s.callbacks=nullptr;s.lua_errors=nullptr;s.last_lua_status=nullptr;error="player selected-faery update exception";return -1;}
}
}
