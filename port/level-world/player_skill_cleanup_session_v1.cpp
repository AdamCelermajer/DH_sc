#include "player_skill_cleanup_session_v1.hpp"
#include <cstring>
#include <deque>
#include <limits>
#include <map>
#include <stdexcept>

namespace dh2::player_skill_cleanup_session_v1 {
namespace prep=character_player_skills_preparation_v3;
namespace {
struct Range {std::uintptr_t first,end;};
bool range(const void* p,std::size_t n,std::size_t alignment,Range& out){
    const auto a=reinterpret_cast<std::uintptr_t>(p);
    if(!p||a%alignment||a>UINTPTR_MAX-n)return false;
    out={a,a+n};return true;
}
bool overlap(Range a,Range b){return a.first<b.end&&b.first<a.end;}
struct Busy {bool& flag;explicit Busy(bool& f):flag(f){flag=true;}~Busy(){flag=false;}};
player_skill_session_v1::Session* const* checked_slot(player_skill_session_v1::Session* const* slot){
    Range r{};if(!range(slot,sizeof(*slot),alignof(player_skill_session_v1::Session*),r))
        throw std::invalid_argument("invalid source cleanup script slot");
    return slot;
}
}
struct Runtime::Impl {
    struct Returns {
        std::vector<dh2_script_value> values;
        std::deque<std::string> strings;
        void erase(){values.clear();strings.clear();}
    };
    player_skill_session_v1::Session* const* slot;
    player_skill_session_v1::Session* canonical;
    prep::Owner& owner;std::uintptr_t character;
    std::map<std::uintptr_t,std::unique_ptr<Returns>> retained;
    bool busy=false;Result* output=nullptr;std::string error;
    Impl(player_skill_session_v1::Session* const* s,prep::Owner& o,std::uintptr_t c)
        :slot(s),canonical(s?*s:nullptr),owner(o),character(c){}
    bool coherent()const {
        return owner.state().owner==character&&owner.state().ai&&
            (!*slot||(*slot==canonical&&canonical->vm()&&
                canonical->character_identity()==character&&canonical->ais_identity()==owner.state().ai));
    }
    void require_owner()const {if(!coherent())throw std::invalid_argument("source cleanup Player ownership changed");}
    bool separate(Range r,const void* p,std::size_t n,std::size_t alignment=1)const {
        Range other{};return range(p,n,alignment,other)&&!overlap(r,other);
    }
    bool separate_owner(Range r)const {
        if(!separate(r,this,sizeof(*this),alignof(Impl))||!separate(r,slot,sizeof(*slot),alignof(decltype(*slot)))||
           !separate(r,&owner,sizeof(owner),alignof(prep::Owner))||
           !separate(r,&owner.state(),sizeof(owner.state()),alignof(prep::source::State))||
           (canonical&&!separate(r,canonical,sizeof(*canonical),alignof(player_skill_session_v1::Session))))return false;
        for(auto list:{List::skill,List::faery}){
            const auto& slots=owner.slots(list);
            if(!separate(r,&slots,sizeof(slots),alignof(decltype(slots))))return false;
            if(!slots.empty()&&!separate(r,slots.data(),slots.size()*sizeof(slots[0]),alignof(std::uintptr_t)))return false;
            for(auto id:slots){
                if(!id)continue;
                const auto* instance=owner.instance(id);const auto* args=owner.instance_arguments(id);
                if(!instance||!args||!separate(r,instance,sizeof(*instance),alignof(prep::constructor::State))||
                   !separate(r,args,sizeof(*args),alignof(prep::Arguments)))return false;
                if(!args->values.empty()&&!separate(r,args->values.data(),args->values.size()*sizeof(prep::Value),alignof(prep::Value)))return false;
                for(const auto& v:args->values)if(!separate(r,v.text.data(),v.text.size()+1))return false;
            }
        }
        return true;
    }
    bool valid_outputs(Result& out,std::string& why,const Runtime* wrapper)const {
        Range r{},e{},w{};
        return range(&out,sizeof(out),alignof(Result),r)&&range(&why,sizeof(why),alignof(std::string),e)&&
            range(wrapper,sizeof(*wrapper),alignof(Runtime),w)&&!overlap(r,e)&&!overlap(r,w)&&!overlap(e,w)&&
            separate_owner(r)&&separate_owner(e);
    }
    static int observe(void* raw,const dh2_script_first_return_v1* input,std::uint32_t count,char*,std::size_t) noexcept {
        auto& r=*static_cast<Returns*>(raw);
        try{
            if(count&&!input)return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
            r.erase();r.values.reserve(count);
            for(std::uint32_t i=0;i<count;++i){
                dh2_script_value v{};v.type=input[i].type;v.number=input[i].number;
                v.boolean=input[i].boolean;v.identity=input[i].identity;
                if(v.type==DH2_SCRIPT_STRING){
                    if(!input[i].text)return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
                    r.strings.emplace_back(input[i].text,input[i].text_bytes);
                    v.text=r.strings.back().c_str();v.text_bytes=r.strings.back().size();
                }
                r.values.push_back(v);
            }
            return 0;
        }catch(...){return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;}
    }
    const prep::constructor::State& instance(std::uintptr_t id)const {
        require_owner();const auto* p=owner.instance(id);
        if(!p||p->identity!=id||p->character!=character)throw std::invalid_argument("source cleanup instance ownership changed");
        return *p;
    }
    int call(std::uintptr_t id,const char* name,bool set,Returns& returns){
        instance(id);auto* session=*slot;
        if(!session)throw std::invalid_argument("missing fresh source cleanup LuaScript");
        std::vector<dh2_script_value> args;
        if(set){
            const auto* original=owner.instance_arguments(id);
            if(!original||original->identity!=id+0xc)throw std::invalid_argument("source cleanup Arguments identity changed");
            for(const auto& v:original->values){
                dh2_script_value a{};
                if(v.type==prep::Value::Type::string){a.type=DH2_SCRIPT_STRING;a.text=v.text.c_str();a.text_bytes=v.text.size();}
                else{a.type=DH2_SCRIPT_NUMBER;if(v.type==prep::Value::Type::number)std::memcpy(&a.number,&v.word,4);
                    else{std::int32_t integer;std::memcpy(&integer,&v.word,4);a.number=static_cast<float>(integer);}}
                args.push_back(a);
            }
            ++output->set_calls;
        }else ++output->cleanup_calls;
        const auto status=session->call_all(name,args.empty()?nullptr:args.data(),std::uint32_t(args.size()),observe,&returns,error);
        output->last_lua_status=status;output->return_count=std::uint32_t(returns.values.size());
        if(status>0)++output->lua_errors;
        return status;
    }
    int one(std::uintptr_t id){
        output->instance=id;output->phase=Phase::construct;
        auto resource=std::make_unique<Returns>();const auto key=reinterpret_cast<std::uintptr_t>(resource.get());
        retained.emplace(key,std::move(resource));++output->constructed;auto& returns=*retained.at(key);
        instance(id);
        if(!*slot)output->decision=Decision::no_script;
        else{
            output->phase=Phase::set_skill;const auto set=call(id,"SetSkill",true,returns);
            if(set<0)return -1;
            if(set>0)output->decision=Decision::set_skill_error;
            else{
                if(!returns.values.empty()){output->phase=Phase::erase;returns.erase();++output->erased;}
                output->phase=Phase::callback;
                if(call(id,"OnSkillCleanUp",false,returns)<0)return -1;
                output->decision=Decision::callback_discard;
            }
        }
        output->phase=Phase::destroy;retained.erase(key);++output->destroyed;++output->completed;return 0;
    }
    int list(List list){
        output->phase=Phase::vector;output->list=list;
        // Each original loop captures end-begin once, then reloads begin at
        // every iteration. Faery count is read only after the skill phase.
        const auto count=owner.slots(list).size();
        if(count>1'000'000)throw std::invalid_argument("source cleanup vector outside bound");
        (list==List::skill?output->skill_slots:output->faery_slots)=std::uint32_t(count);
        for(std::uint32_t index=0;index<count;++index){
            require_owner();output->phase=Phase::vector;output->index=index;
            const auto& current=owner.slots(list);
            if(index>=current.size())throw std::invalid_argument("source cleanup vector backing changed");
            const auto id=current[index];++output->examined;
            if(!id){++output->null_slots;continue;}
            if(one(id))return -1;
        }
        return 0;
    }
};
Runtime::Runtime(player_skill_session_v1::Session* const* slot,prep::Owner& owner,std::uintptr_t character)
    :impl_(std::make_unique<Impl>(checked_slot(slot),owner,character)){
    if(!character||!impl_->coherent())
        throw std::invalid_argument("invalid borrowed Player cleanup owners");
}
Runtime::~Runtime()=default;
std::size_t Runtime::retained_failed_returns()const noexcept{return impl_->retained.size();}
int Runtime::cleanup(List list,std::uint32_t index,Result& out,std::string& error){
    auto& s=*impl_;if(s.busy||(list!=List::skill&&list!=List::faery)||!s.valid_outputs(out,error,this)||!s.coherent())return -1;
    const auto& slots=s.owner.slots(list);if(index>=slots.size()||!slots[index])return -1;
    Busy busy(s.busy);out={};out.list=list;out.index=index;out.examined=1;s.output=&out;s.error.clear();
    try{const auto status=s.one(slots[index]);if(!status)out.phase=Phase::complete;s.output=nullptr;error=s.error;return status;}
    catch(const std::exception& e){s.output=nullptr;error=e.what();return -1;}catch(...){s.output=nullptr;error="source cleanup exception";return -1;}
}
int Runtime::cleanup(List list,Result& out,std::string& error){
    if(list!=List::skill&&list!=List::faery)return -1;
    auto& s=*impl_;if(s.busy||!s.valid_outputs(out,error,this)||!s.coherent())return -1;
    Busy busy(s.busy);out={};s.output=&out;s.error.clear();
    try{const auto status=s.list(list);if(!status)out.phase=Phase::complete;s.output=nullptr;error=s.error;return status;}
    catch(const std::exception& e){s.output=nullptr;error=e.what();return -1;}catch(...){s.output=nullptr;error="source cleanup loop exception";return -1;}
}
int Runtime::cleanup_all(Result& out,std::string& error){
    auto& s=*impl_;if(s.busy||!s.valid_outputs(out,error,this)||!s.coherent())return -1;
    Busy busy(s.busy);out={};s.output=&out;s.error.clear();
    try{int status=s.list(List::skill);if(!status)status=s.list(List::faery);if(!status)out.phase=Phase::complete;s.output=nullptr;error=s.error;return status;}
    catch(const std::exception& e){s.output=nullptr;error=e.what();return -1;}catch(...){s.output=nullptr;error="source cleanup phases exception";return -1;}
}
}
