#include "../character_ai_skill_script_check.hpp"
#include <array>
#include <cassert>
#include <cstring>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>
namespace sc=dh2::character_ai_skill_script_check;
struct Value {std::uint32_t type,word;std::uintptr_t identity;const char* text;};
struct Config {
    unsigned kind=0,script=1,set_error=0,set_count=2,check_error=0,count=2,type=3,word=0x3f800000u,identity=17,string_boolean=1,mutation=0;
};
struct Fixture {
    Config c;sc::Character a{101,113},b{202,224};sc::State state{0x1000,&a};
    sc::Services services{this,sizeof(Value),call};sc::Result result{};sc::ValueVector range{};
    std::vector<Value> storage;std::vector<std::array<std::uint64_t,5>> trace;
    std::vector<unsigned> string_trace;
    unsigned fail=99,throwing=99,bad=0,effects=0;bool alive=false,nested=false,nested_ok=false;
    Fixture()=default;
    explicit Fixture(Config q):c(q){a.lua_script=c.script==0?0:c.script==1?113:224;}
    void refresh(){range.begin=reinterpret_cast<std::uintptr_t>(storage.data());range.end=range.begin+storage.size()*sizeof(Value);}
    void values(unsigned n,bool final){
        storage.assign(n,Value{3,0x3f800000u,17,"retained text"});
        if(final&&c.kind<n)storage[c.kind]={c.type,c.word,c.identity,c.string_boolean?(c.word?"0":""):nullptr};
        refresh();
    }
    unsigned boolean(Value& v){
        switch(v.type){
            case 1:case 3:{float n;std::memcpy(&n,&v.word,4);return n!=0.0f;}
            case 2:case 7:return v.identity!=0;
            case 4:{
                string_trace.push_back(0);if(c.mutation&128)v.text=v.text?nullptr:"";
                string_trace.push_back(v.text?1u:0u);string_trace.push_back(1);string_trace.push_back(2);
                return v.text!=nullptr;
            }
            default:return 0;
        }
    }
    static std::int32_t call(void* context,sc::State* state,const sc::Request* q,sc::ReturnValues* r,sc::Response* reply){
        auto& f=*static_cast<Fixture*>(context);const auto op=unsigned(q->operation);assert(state==&f.state&&q->skill==0x1000&&unsigned(q->check)==f.c.kind);
        const auto n=op==2?(q->last-q->first)/sizeof(Value):0;
        f.trace.push_back({op,q->script,q->arguments?q->arguments-q->skill:0,n,op==1?1u:op==3?2u:0u});++f.effects;
        if(op==0){
            assert(!q->resource&&!f.alive);f.alive=true;f.values(0,false);*r={reinterpret_cast<std::uintptr_t>(&f.storage),0,&f.range};
            if(f.c.mutation&1)state->owner=&f.b;
            if(f.bad==1)r->resource=0;
            if(f.bad==2)state->owner=nullptr;
            if(f.bad==3)state->owner=reinterpret_cast<sc::Character*>(&f.result);
            if(f.nested){Fixture inner;inner.c.kind=1;f.nested_ok=inner.run()==sc::Status::complete;}
        }else{
            assert(q->resource==reinterpret_cast<std::uintptr_t>(&f.storage)&&f.alive);
            if(op==1){
                assert(q->script==state->owner->lua_script&&q->arguments==0x100c&&std::string(q->function)=="SetSkill");
                r->error=f.c.set_error;f.values(f.c.set_count,false);
                if(f.c.mutation&2)state->owner=&f.b;
                if(f.c.mutation&4)state->owner->lua_script=state->owner->lua_script==113?224:113;
                if(f.bad==4)r->values=nullptr;
                if(f.bad==5)r->values=reinterpret_cast<sc::ValueVector*>(reinterpret_cast<unsigned char*>(&f.range)+1);
                if(f.bad==6)f.range={9,7};
                if(f.bad==7)r->values=reinterpret_cast<sc::ValueVector*>(&f.result);
                if(f.bad==8)r->resource=7;
                if(f.bad==9)state->owner=nullptr;
                if(f.bad==10)state->owner->lua_script=0;
            }else if(op==2){
                assert(q->first==f.range.begin&&q->last==f.range.end&&n==f.storage.size()&&n!=0);f.values(0,false);
                if(f.c.mutation&8)state->owner=&f.b;
                if(f.c.mutation&16)state->owner->lua_script=state->owner->lua_script==113?224:113;
            }else if(op==3){
                assert(q->script==state->owner->lua_script&&!q->arguments&&std::string(q->function)=="OnSkillCheck"&&f.storage.empty());
                r->error=f.c.check_error;f.values(f.c.count,true);
                if(f.c.mutation&32)state->owner=nullptr;
                if(f.bad==11)r->values=nullptr;
                if(f.bad==12)++f.range.end;
                if(f.bad==13)f.range={0x10000000u,0x10000000u+(1000001ull*sizeof(Value))};
                if(f.bad==16)r->values=reinterpret_cast<sc::ValueVector*>(r);
            }else if(op==4){
                assert(f.c.kind==0&&q->index==0);reply->value=reinterpret_cast<std::uintptr_t>(&f.storage[0]);if(f.bad==14)reply->value=0;
            }else if(op==5){
                const auto selected=reinterpret_cast<std::uintptr_t>(&f.storage[f.c.kind]);assert(q->value==selected);
                reply->boolean=f.boolean(f.storage[f.c.kind]);if(f.bad==15)reply->boolean=2;
            }else{
                assert(op==6);if(f.c.mutation&64)for(auto& v:f.storage)v={0,0,0,0};
                f.storage.clear();f.refresh();f.alive=false;r->resource=0;
            }
        }
        if(f.throwing==op)throw std::runtime_error("provider failure after retained effects");
        return f.fail==op?7:0;
    }
    sc::Status run(){return sc::check(&state,static_cast<sc::Check>(c.kind),&services,&result);}
};
void print(const Fixture& f,sc::Status status){
    const auto& r=f.result;
    std::cout<<"{\"status\":"<<int(status)<<",\"phase\":"<<unsigned(r.phase)<<",\"decision\":"<<unsigned(r.decision)<<",\"calls\":"<<r.service_calls<<",\"set_error\":"<<r.set_skill_error<<",\"check_error\":"<<r.check_error<<",\"count\":"<<r.value_count<<",\"boolean\":"<<r.boolean<<",\"erased\":"<<r.erased<<",\"constructed\":"<<r.constructed<<",\"destroyed\":"<<r.destroyed<<",\"trace\":[";
    for(unsigned i=0;i<f.trace.size();++i){if(i)std::cout<<',';std::cout<<'[';for(unsigned j=0;j<5;++j){if(j)std::cout<<',';std::cout<<f.trace[i][j];}std::cout<<']';}std::cout<<"],\"string_trace\":[";
    for(unsigned i=0;i<f.string_trace.size();++i){if(i)std::cout<<',';std::cout<<f.string_trace[i];}std::cout<<"]}\n";
}
std::vector<Config> cases(){
    std::vector<Config> out;
    for(unsigned kind=0;kind<2;++kind)for(unsigned script=0;script<3;++script)for(unsigned se:std::array<unsigned,2>{0,7})for(unsigned sn:std::array<unsigned,2>{0,2})for(unsigned ce:std::array<unsigned,2>{0,9})for(unsigned count=0;count<3;++count){Config c;c.kind=kind;c.script=script;c.set_error=se;c.set_count=sn;c.check_error=ce;c.count=count;out.push_back(c);}
    const std::array<std::array<unsigned,4>,22> profiles{{
        {0,0,0,1},{1,0,0,1},{1,0x3f800000,0,1},{3,0,0,1},{3,0x80000000,0,1},{3,0x3f800000,0,1},{3,0xbf800000,0,1},{3,0x7f800000,0,1},{3,0xff800000,0,1},{3,0x7fc00000,0,1},{3,1,0,1},{2,0,0,1},{2,0,17,1},{7,0,0,1},{7,0,17,1},{4,0,0,1},{4,1,0,1},{4,0,0,0},{5,0,0,1},{6,0,0,1},{8,0,0,1},{0xffffffff,0,0,1}
    }};
    for(unsigned kind=0;kind<2;++kind)for(auto p:profiles){Config c;c.kind=kind;c.type=p[0];c.word=p[1];c.identity=p[2];c.string_boolean=p[3];out.push_back(c);}
    for(unsigned kind=0;kind<2;++kind)for(unsigned mutation:std::array<unsigned,9>{1,2,4,8,16,31,32,64,128}){Config c;c.kind=kind;c.mutation=mutation;if(mutation==128)c.type=4;out.push_back(c);}
    return out;
}
int main(int argc,char** argv){
    if(argc>1){assert(argc==12);std::array<unsigned,11> p{};for(unsigned i=0;i<11;++i)p[i]=unsigned(std::stoull(argv[i+1]));Config c{p[0],p[1],p[2],p[3],p[4],p[5],p[6],p[7],p[8],p[9],p[10]};Fixture f(c);const auto status=f.run();print(f,status);return 0;}
    unsigned behavior=0,failures=0,boundaries=0,guards=0;
    for(auto c:cases()){
        Fixture f(c);assert(f.run()==sc::Status::complete&&f.result.constructed&&f.result.destroyed&&!f.alive&&f.storage.empty());
        const bool scripted=c.script!=0||(c.mutation&1);
        const auto expected=!scripted?sc::Decision::no_script:c.set_error?sc::Decision::set_skill_error:c.check_error?sc::Decision::check_error:c.count<=c.kind?sc::Decision::insufficient_values:sc::Decision::converted;
        assert(f.result.decision==expected&&f.result.service_calls==f.trace.size());
        if(expected!=sc::Decision::converted)assert(f.result.boolean==0);
        ++behavior;
    }
    for(unsigned kind=0;kind<2;++kind){Fixture f;f.c.kind=kind;f.nested=true;assert(f.run()==sc::Status::complete&&f.nested_ok);++behavior;}
    for(unsigned kind=0;kind<2;++kind)for(unsigned op=0;op<7;++op){if(kind==1&&op==4)continue;for(unsigned throwing=0;throwing<2;++throwing){Fixture f;f.c.kind=kind;if(throwing)f.throwing=op;else f.fail=op;assert(f.run()==sc::Status::service_failed&&f.trace.back()[0]==op&&f.result.destroyed==0&&f.alive==(op!=6));assert(f.result.service_calls==f.trace.size());++failures;}}
    {Fixture f;f.services.invoke=nullptr;assert(f.run()==sc::Status::service_unavailable&&f.result.service_calls==0);++failures;}
    for(unsigned bad=1;bad<=16;++bad){Fixture f;f.bad=bad;assert(f.run()==sc::Status::invalid_source_fact&&f.alive&&f.result.destroyed==0);++boundaries;}
    for(unsigned kind=0;kind<2;++kind)for(unsigned which=0;which<2;++which){Fixture f;f.c.kind=kind;if(which==0){f.c.set_error=7;f.bad=4;}else{f.c.check_error=9;f.bad=11;}assert(f.run()==sc::Status::complete&&f.result.destroyed&&!f.alive);++behavior;}
    Fixture f;f.result.service_calls=0xa5a5a5a5u;
    auto invalid=[&](sc::State* s,sc::Check kind,const sc::Services* svc,sc::Result* r){assert(sc::check(s,kind,svc,r)==sc::Status::invalid_argument&&f.trace.empty()&&f.result.service_calls==0xa5a5a5a5u);++guards;};
    invalid(nullptr,sc::Check::usable,&f.services,&f.result);invalid(&f.state,sc::Check::usable,nullptr,&f.result);invalid(&f.state,sc::Check::usable,&f.services,nullptr);
    invalid(&f.state,static_cast<sc::Check>(2),&f.services,&f.result);
    alignas(16)std::array<unsigned char,128> bytes{};auto* bad=bytes.data()+1;
    invalid(reinterpret_cast<sc::State*>(bad),sc::Check::usable,&f.services,&f.result);invalid(&f.state,sc::Check::usable,reinterpret_cast<const sc::Services*>(bad),&f.result);invalid(&f.state,sc::Check::usable,&f.services,reinterpret_cast<sc::Result*>(bad));
    invalid(&f.state,sc::Check::usable,reinterpret_cast<const sc::Services*>(&f.state),&f.result);invalid(&f.state,sc::Check::usable,&f.services,reinterpret_cast<sc::Result*>(&f.state));invalid(&f.state,sc::Check::usable,&f.services,reinterpret_cast<sc::Result*>(&f.services));
    {sc::State zero{0,&f.a};invalid(&zero,sc::Check::usable,&f.services,&f.result);}
    {sc::State overflow{std::numeric_limits<std::uintptr_t>::max()-11,&f.a};invalid(&overflow,sc::Check::usable,&f.services,&f.result);}
    {auto svc=f.services;svc.native_value_stride=0;invalid(&f.state,sc::Check::usable,&svc,&f.result);svc.native_value_stride=4097;invalid(&f.state,sc::Check::usable,&svc,&f.result);}
    std::cout<<"{\"validation\":\"PASS\",\"host_cases\":"<<behavior<<",\"failure_cases\":"<<failures<<",\"source_boundary_cases\":"<<boundaries<<",\"guard_cases\":"<<guards<<",\"mismatches\":0}\n";
}
