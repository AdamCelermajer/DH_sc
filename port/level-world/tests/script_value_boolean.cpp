#include "../script_value_boolean.hpp"
#include <array>
#include <cassert>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>
namespace vb=dh2::script_value_boolean;
const char* text(unsigned mode){switch(mode){case 0:return nullptr;case 1:return "";case 2:return "0";case 3:return "false";default:return "\0hidden";}}
unsigned text_tag(const char* p){if(!p)return 0;if(!*p)return 1;return *p=='0'?2:3;}
struct Fixture {
    vb::Value value{4,0,17,"0"};vb::Services services{this,call};vb::Result out{};
    std::uint32_t raw=17,mutation=0,fail=99,throwing=99;bool alive=false,nested_ok=false;
    unsigned effects=0,alternate_calls=0;std::vector<std::array<std::uint32_t,4>> trace;
    static std::int32_t alternate(void* c,const vb::Value*,const vb::Request*,vb::Response*){++static_cast<Fixture*>(c)->alternate_calls;return 7;}
    static std::int32_t call(void* context,const vb::Value* value,const vb::Request* q,vb::Response* r){
        auto& f=*static_cast<Fixture*>(context);assert(value==&f.value);const auto op=unsigned(q->operation);
        f.trace.push_back({op,q->lua?1u:0u,text_tag(q->text),std::uint32_t(q->index)});++f.effects;
        if(op==0){
            assert(!q->lua&&!f.alive);f.alive=true;r->lua=0x1234;
            if(f.mutation&1)f.value.string=nullptr;
            if(f.mutation&2)f.value.string="";
            if(f.mutation&4)f.value.type=0;
            if(f.mutation&8)f.services.invoke=alternate;
            if(f.mutation&16)f.services.context=nullptr;
            if(f.mutation&256){Fixture inner;f.nested_ok=inner.run()==vb::Status::complete;}
            if(f.mutation&512)r->lua=0;
        }else{
            assert(q->lua==0x1234&&f.alive);
            if(op==1)assert(q->text==f.value.string);
            else if(op==2){assert(q->index==-1);r->raw_boolean=f.raw;if(f.mutation&32)f.value.string=nullptr;}
            else{assert(op==3);f.alive=false;if(f.mutation&64)r->raw_boolean=!f.raw;if(f.mutation&128)f.value={0,0,0,nullptr};}
        }
        if(f.throwing==op)throw std::runtime_error("provider failure after source prefix effects");
        return f.fail==op?7:0;
    }
    vb::Status run(){return vb::execute(&value,&services,&out);}
};
void print(const Fixture& f,vb::Status s){
    std::cout<<"{\"status\":"<<int(s)<<",\"type\":"<<f.out.captured_type<<",\"value\":"<<f.out.value<<",\"raw\":"<<f.out.raw_lua_boolean<<",\"phase\":"<<unsigned(f.out.phase)<<",\"calls\":"<<f.out.service_calls<<",\"closed\":"<<f.out.closed<<",\"trace\":[";
    for(unsigned i=0;i<f.trace.size();++i){if(i)std::cout<<',';std::cout<<'[';for(unsigned j=0;j<4;++j){if(j)std::cout<<',';std::cout<<f.trace[i][j];}std::cout<<']';}std::cout<<"]}\n";
}
int main(int argc,char** argv){
    if(argc>1){assert(argc==7);Fixture f;f.value.type=unsigned(std::stoull(argv[1]));f.value.number_word=unsigned(std::stoull(argv[2]));f.value.object_identity=std::stoull(argv[3]);f.value.string=text(unsigned(std::stoul(argv[4])));f.raw=unsigned(std::stoull(argv[5]));f.mutation=unsigned(std::stoull(argv[6]));auto s=f.run();print(f,s);return 0;}
    unsigned cases=0,failures=0,guards=0;
    for(unsigned tag=0;tag<32;++tag){Fixture f;f.value.type=tag;assert(f.run()==vb::Status::complete&&f.out.captured_type==tag&&f.out.phase==vb::Phase::complete);assert(f.out.value==(tag==2||tag==4||tag==7));++cases;}
    for(auto tag:std::array<unsigned,2>{1,3})for(auto word:std::array<unsigned,14>{0,0x80000000u,1,0x007fffffu,0x00800000u,0x3f800000u,0xbf800000u,0x7f800000u,0xff800000u,0x7fc00000u,0x7f800001u,0xffffffffu,0x80000001u,0x7f7fffffu}){
        vb::Value value{tag,word,0,nullptr};vb::Result r{};assert(vb::execute(&value,nullptr,&r)==vb::Status::complete&&r.value==unsigned((word&0x7fffffffu)!=0)&&r.service_calls==0);++cases;
    }
    for(auto tag:std::array<unsigned,2>{2,7})for(auto id:std::array<std::uintptr_t,4>{0,17,0x100000000ull,std::numeric_limits<std::uintptr_t>::max()}){vb::Value value{tag,0,id,nullptr};vb::Result r{};assert(vb::execute(&value,nullptr,&r)==vb::Status::complete&&r.value==unsigned(id!=0));++cases;}
    for(unsigned mode=0;mode<5;++mode)for(auto raw:std::array<unsigned,4>{0,1,17,0xffffffffu})for(auto mutation:std::array<unsigned,10>{0,1,2,4,8,16,32,64,128,255}){
        Fixture f;f.value.string=text(mode);f.raw=raw;f.mutation=mutation;assert(f.run()==vb::Status::complete&&!f.alive&&f.out.closed==1&&f.out.service_calls==4&&f.out.value==unsigned(raw!=0)&&f.out.captured_type==4&&f.alternate_calls==0);++cases;
    }
    {Fixture f;f.mutation=256;assert(f.run()==vb::Status::complete&&f.nested_ok);++cases;}
    for(unsigned op=0;op<4;++op)for(unsigned throwing=0;throwing<2;++throwing){Fixture f;if(throwing)f.throwing=op;else f.fail=op;assert(f.run()==vb::Status::service_failed&&f.trace.size()==op+1&&f.effects==op+1&&f.out.closed==0&&f.alive==(op!=3));++failures;}
    {Fixture f;f.services.invoke=nullptr;assert(f.run()==vb::Status::service_unavailable&&f.out.service_calls==0);++failures;}
    {Fixture f;assert(vb::execute(&f.value,nullptr,&f.out)==vb::Status::service_unavailable&&f.out.service_calls==0);++failures;}
    {Fixture f;f.mutation=512;assert(f.run()==vb::Status::invalid_source_fact&&f.out.service_calls==1&&f.out.closed==0&&f.alive);++failures;}
    Fixture f;f.out.service_calls=0xa5a5a5a5u;
    auto invalid=[&](const vb::Value* value,const vb::Services* services,vb::Result* r){assert(vb::execute(value,services,r)==vb::Status::invalid_argument&&f.trace.empty()&&f.out.service_calls==0xa5a5a5a5u);++guards;};
    invalid(nullptr,&f.services,&f.out);invalid(&f.value,&f.services,nullptr);
    alignas(16)std::array<unsigned char,128> bytes{};auto* bad=bytes.data()+1;
    invalid(reinterpret_cast<const vb::Value*>(bad),&f.services,&f.out);invalid(&f.value,reinterpret_cast<const vb::Services*>(bad),&f.out);invalid(&f.value,&f.services,reinterpret_cast<vb::Result*>(bad));
    invalid(&f.value,reinterpret_cast<const vb::Services*>(&f.value),&f.out);invalid(&f.value,&f.services,reinterpret_cast<vb::Result*>(&f.value));invalid(&f.value,&f.services,reinterpret_cast<vb::Result*>(&f.services));
    std::cout<<"{\"validation\":\"PASS\",\"host_cases\":"<<cases<<",\"failure_cases\":"<<failures<<",\"guard_cases\":"<<guards<<",\"mismatches\":0}\n";
}
