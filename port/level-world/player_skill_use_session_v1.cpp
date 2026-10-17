#include "player_skill_use_session_v1.hpp"
#include <cstring>
#include <deque>
#include <map>
#include <stdexcept>
#include <limits>

namespace dh2::player_skill_use_session_v1 {
namespace prep=character_player_skills_preparation_v3;
namespace check_source=character_ai_skill_script_check;
namespace boolean=script_value_boolean;
namespace {
struct Busy {bool& value;explicit Busy(bool& v):value(v){value=true;}~Busy(){value=false;}};
bool overlaps(const void* p,std::size_t n,const void* q,std::size_t m){
    auto a=reinterpret_cast<std::uintptr_t>(p),b=reinterpret_cast<std::uintptr_t>(q);return a<=b?b-a<n:a-b<m;
}
}
struct Runtime::Impl {
    struct Returns {
        check_source::ValueVector vector{};
        std::vector<boolean::Value> values;
        std::deque<std::string> strings;
        void sync(){const auto b=values.empty()?0:reinterpret_cast<std::uintptr_t>(values.data());vector={b,b?b+values.size()*sizeof(boolean::Value):0};}
        void erase(){values.clear();strings.clear();sync();}
    };
    player_skill_session_v1::Session& session;prep::Owner& owner;std::uintptr_t character;
    script_value_boolean_lua::TemporaryLua bool_lua;
    std::map<std::uintptr_t,std::unique_ptr<Returns>> retained;
    bool busy=false;Result* output=nullptr;std::string error;
    Impl(player_skill_session_v1::Session& s,prep::Owner& o,std::uintptr_t c):session(s),owner(o),character(c){}
    static int observe(void* raw,const dh2_script_first_return_v1* values,std::uint32_t count,char*,std::size_t) noexcept {
        auto& r=*static_cast<Returns*>(raw);
        try {
            if(count && !values)return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
            r.erase();r.values.reserve(count);
            for(std::uint32_t i=0;i<count;++i){
                const auto& input=values[i];boolean::Value value{input.type,0,input.identity,nullptr};
                std::memcpy(&value.number_word,&input.number,4);
                if(input.type==DH2_SCRIPT_STRING){
                    if(!input.text)return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
                    r.strings.emplace_back(input.text,input.text_bytes);value.string=r.strings.back().c_str();
                }
                r.values.push_back(value);
            }
            r.sync();return 0;
        }catch(...){return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;}
    }
    const prep::constructor::State* instance(std::uintptr_t id)const {
        auto* p=owner.instance(id);
        if(!p || p->identity!=id || p->character!=character || owner.state().owner!=character)
            throw std::invalid_argument("source skill/Character ownership changed");
        return p;
    }
    std::uintptr_t slot(List list,std::uint32_t index)const {
        if(list!=List::skill && list!=List::faery)throw std::invalid_argument("invalid skill list");
        const auto& slots=owner.slots(list);if(index>=slots.size() || !slots[index])throw std::invalid_argument("missing source skill slot");
        instance(slots[index]);return slots[index];
    }
    int call(std::uintptr_t id,const char* function,bool set,Returns& values,std::uint32_t& source_error){
        instance(id);std::vector<dh2_script_value> args;
        if(set){
            const auto* original=owner.instance_arguments(id);
            if(!original || original->identity!=id+0xc)throw std::invalid_argument("source Arguments identity changed");
            for(const auto& v:original->values){
                dh2_script_value value{};
                if(v.type==prep::Value::Type::string){value.type=DH2_SCRIPT_STRING;value.text=v.text.c_str();value.text_bytes=v.text.size();}
                else {value.type=DH2_SCRIPT_NUMBER;
                    if(v.type==prep::Value::Type::number)std::memcpy(&value.number,&v.word,4);
                    else {std::int32_t n;std::memcpy(&n,&v.word,4);value.number=static_cast<float>(n);}}
                args.push_back(value);
            }
        }
        ++output->call_count;
        const int status=session.call_all(function,args.empty()?nullptr:args.data(),std::uint32_t(args.size()),observe,&values,error);
        output->last_lua_status=status;output->return_count=std::uint32_t(values.values.size());
        if(status<0)return -1;
        source_error=std::uint32_t(status);return 0;
    }
    std::uintptr_t construct(){auto r=std::make_unique<Returns>();const auto id=reinterpret_cast<std::uintptr_t>(r.get());retained.emplace(id,std::move(r));output->constructed=1;return id;}
    Returns& returns(std::uintptr_t id){auto it=retained.find(id);if(it==retained.end())throw std::invalid_argument("unknown ReturnValues resource");return *it->second;}
    bool truth(const boolean::Value* value){
        auto services=bool_lua.services();boolean::Result result{};
        if(boolean::execute(value,&services,&result)!=boolean::Status::complete)throw std::runtime_error("source Value::getBool failed");
        return result.value!=0;
    }
    static std::int32_t check_invoke(void* raw,check_source::State* state,const check_source::Request* q,
                                     check_source::ReturnValues* projection,check_source::Response* response){
        auto& s=*static_cast<Impl*>(raw);
        if(!s.output || !q || !state || !projection || !response || state->identity!=q->skill)return -1;
        try {
            using Op=check_source::Operation;
            if(q->operation==Op::construct_values){
                s.output->phase=Phase::construct;projection->resource=s.construct();projection->error=0;projection->values=&s.returns(projection->resource).vector;return 0;
            }
            auto& r=s.returns(q->resource);
            if(projection->resource!=q->resource || projection->values!=&r.vector)return -1;
            switch(q->operation){
            case Op::call_set_skill:
                s.output->phase=Phase::set_skill;
                if(q->script!=reinterpret_cast<std::uintptr_t>(s.session.vm()) || q->arguments!=q->skill+0xc)return -1;
                return s.call(q->skill,q->function,true,r,projection->error);
            case Op::erase_values:
                s.output->phase=Phase::erase;
                if(q->first!=r.vector.begin || q->last!=r.vector.end)return -1;
                r.erase();s.output->erased=1;return 0;
            case Op::call_skill_check:
                s.output->phase=Phase::callback;
                if(q->script!=reinterpret_cast<std::uintptr_t>(s.session.vm()))return -1;
                return s.call(q->skill,q->function,false,r,projection->error);
            case Op::operator_index:
                if(q->index || r.values.empty())return -1;
                response->value=reinterpret_cast<std::uintptr_t>(r.values.data());return 0;
            case Op::get_bool: {
                s.output->phase=Phase::boolean;
                const auto at=q->value,first=r.vector.begin,last=r.vector.end;
                if(!first || at<first || at>=last || (at-first)%sizeof(boolean::Value))return -1;
                response->boolean=s.truth(reinterpret_cast<const boolean::Value*>(at));return 0;
            }
            case Op::destroy_values:s.output->phase=Phase::destroy;s.retained.erase(q->resource);s.output->destroyed=1;return 0;
            default:return -1;
            }
        }catch(const std::exception& e){s.error=e.what();return -1;}catch(...){s.error="skill-check provider exception";return -1;}
    }
    int invoke(std::uintptr_t skill,Callback callback){
        // Original callers construct before the source owner's script gate.
        output->phase=Phase::construct;const auto id=construct();auto& r=returns(id);std::uint32_t source_error=0;
        instance(skill);
        if(!session.vm()){output->decision=Decision::no_script;}
        else {
            output->phase=Phase::set_skill;if(call(skill,"SetSkill",true,r,source_error))return -1;
            if(source_error)output->decision=Decision::set_skill_error;
            else {
                if(r.vector.begin!=r.vector.end){output->phase=Phase::erase;r.erase();output->erased=1;}
                // Original reloads instance->owner and Character's LuaScript.
                // This adapter's borrowed session must remain the same VM.
                instance(skill);if(!session.vm()){error="missing fresh source LuaScript";return -1;}
                const char* name=callback==Callback::pre?"OnPreSkill":callback==Callback::use?"OnSkill":"OnPostSkill";
                output->phase=Phase::callback;if(call(skill,name,false,r,source_error))return -1;
                if(callback==Callback::post)output->decision=Decision::post_discard;
                else if(source_error)output->decision=Decision::callback_error;
                else if(r.values.empty()){output->value=1;output->decision=Decision::empty_success;}
                else {output->phase=Phase::boolean;output->value=truth(r.values.data());output->decision=Decision::converted;}
            }
        }
        output->phase=Phase::destroy;retained.erase(id);output->destroyed=1;output->phase=Phase::complete;return 0;
    }
    bool valid_output(const Result& out,const Runtime* wrapper)const {
        const auto p=reinterpret_cast<std::uintptr_t>(&out);
        return p%alignof(Result)==0 && p<=UINTPTR_MAX-sizeof(out) &&
            !overlaps(&out,sizeof(out),this,sizeof(*this)) && !overlaps(&out,sizeof(out),wrapper,sizeof(*wrapper)) &&
            !overlaps(&out,sizeof(out),&session,sizeof(session)) && !overlaps(&out,sizeof(out),&owner,sizeof(owner));
    }
};
Runtime::Runtime(player_skill_session_v1::Session& s,prep::Owner& o,std::uintptr_t c):impl_(std::make_unique<Impl>(s,o,c)){
    if(!c || o.state().owner!=c || s.character_identity()!=c || !s.vm())throw std::invalid_argument("invalid borrowed Player skill owners");
}
Runtime::~Runtime()=default;
std::size_t Runtime::retained_failed_returns()const noexcept{return impl_->retained.size();}
int Runtime::check(List list,std::uint32_t slot,Check kind,Result& out,std::string& error){
    auto& s=*impl_;if(s.busy || !s.valid_output(out,this) || (kind!=Check::usable && kind!=Check::active))return -1;
    Busy busy(s.busy);
    try {
        const auto skill=s.slot(list,slot);out={};s.output=&out;s.error.clear();
        check_source::Character character{s.character,reinterpret_cast<std::uintptr_t>(s.session.vm())};check_source::State state{skill,&character};
        const check_source::Services services{&s,sizeof(boolean::Value),Impl::check_invoke};
        const auto status=check_source::check(&state,kind,&services,&out.source_check);
        out.value=out.source_check.boolean;if(status==check_source::Status::complete)out.phase=Phase::complete;
        s.output=nullptr;error=s.error;return status==check_source::Status::complete?0:-1;
    }catch(const std::exception& e){s.output=nullptr;error=e.what();return -1;}catch(...){s.output=nullptr;error="source skill check exception";return -1;}
}
int Runtime::invoke(List list,std::uint32_t slot,Callback callback,Result& out,std::string& error){
    auto& s=*impl_;if(s.busy || !s.valid_output(out,this) || (callback!=Callback::pre && callback!=Callback::use && callback!=Callback::post))return -1;
    Busy busy(s.busy);
    try {
        const auto skill=s.slot(list,slot);out={};s.output=&out;s.error.clear();int status=s.invoke(skill,callback);s.output=nullptr;error=s.error;return status;
    }catch(const std::exception& e){s.output=nullptr;error=e.what();return -1;}catch(...){s.output=nullptr;error="source skill callback exception";return -1;}
}
}
