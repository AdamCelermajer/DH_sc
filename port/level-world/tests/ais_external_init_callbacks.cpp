#include "../ais_external_init_callbacks.hpp"

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>

namespace a=dh2::ais_external_init_callbacks;
namespace {
void require(bool condition){if(!condition)throw std::runtime_error("AIS initialization callback check failed");}
struct Fixture {
    a::State state{0x10001000};
    a::Result result{};
    a::Services services{this,call};
    unsigned calls=0,mode=0;
    std::uintptr_t observed=0;
    a::Callback callback{};
    const char* name=nullptr;
    static std::int32_t call(void* raw,a::State* state,const a::Request* request){
        auto& f=*static_cast<Fixture*>(raw);require(state==&f.state);
        ++f.calls;f.observed=request->ais;f.callback=request->callback;f.name=request->name;
        if(f.mode==1)state->ais=0x10002000;
        if(f.mode==2)return 1;
        if(f.mode==3)throw std::runtime_error("actual provider failure");
        return 0;
    }
};
}
int main(int argc,char** argv){
    if(argc==4){
        Fixture f;const auto callback=static_cast<a::Callback>(std::atoi(argv[1]));
        f.state.ais=std::strtoull(argv[2],nullptr,0);f.mode=std::atoi(argv[3]);
        const auto status=a::invoke(&f.state,callback,&f.services,&f.result);
        std::cout<<"{\"status\":"<<int(status)<<",\"calls\":"<<f.calls<<",\"ais\":"<<f.observed
                 <<",\"name\":\""<<(f.name?f.name:"")<<"\",\"live_ais\":"<<f.state.ais
                 <<",\"default_init\":"<<f.result.default_init_completed<<"}\n";return 0;
    }
    unsigned cases=0;
    for(unsigned callback=0;callback<3;++callback)for(unsigned mode=0;mode<4;++mode){
        Fixture f;f.mode=mode;
        const auto status=a::invoke(&f.state,static_cast<a::Callback>(callback),&f.services,&f.result);
        require(status==(mode>=2?a::Status::service_failed:a::Status::complete));
        require(f.calls==1&&f.result.calls==1&&f.observed==0x10001000&&f.result.ais==0x10001000);
        require(f.callback==static_cast<a::Callback>(callback));
        require(!std::strcmp(f.name,callback==0?"OnInit":callback==1?"OnInitPost":"OnInitFinal"));
        require(f.result.default_init_completed==(callback==0));++cases;
    }
    {Fixture f;f.mode=1;
     require(a::invoke(&f.state,a::Callback::init,&f.services,&f.result)==a::Status::complete);
     require(a::invoke(&f.state,a::Callback::post,&f.services,&f.result)==a::Status::complete);
     require(f.observed==0x10002000&&f.calls==2);++cases;}
    {Fixture f;f.services.call=nullptr;
     require(a::invoke(&f.state,a::Callback::init,&f.services,&f.result)==a::Status::service_unavailable);
     require(f.calls==0&&f.result.calls==0&&f.result.default_init_completed==1);++cases;}
    for(unsigned guard=0;guard<8;++guard){Fixture f;f.result.calls=99;
        a::Status status{};
        switch(guard){
        case 0:status=a::invoke(nullptr,a::Callback::init,&f.services,&f.result);break;
        case 1:status=a::invoke(&f.state,a::Callback::init,nullptr,&f.result);break;
        case 2:status=a::invoke(&f.state,a::Callback::init,&f.services,nullptr);break;
        case 3:f.state.ais=0;status=a::invoke(&f.state,a::Callback::init,&f.services,&f.result);break;
        case 4:status=a::invoke(&f.state,static_cast<a::Callback>(3),&f.services,&f.result);break;
        case 5:status=a::invoke(&f.state,a::Callback::init,reinterpret_cast<a::Services*>(&f.state),&f.result);break;
        case 6:status=a::invoke(&f.state,a::Callback::init,&f.services,reinterpret_cast<a::Result*>(&f.state));break;
        default:status=a::invoke(reinterpret_cast<a::State*>(reinterpret_cast<char*>(&f.state)+1),a::Callback::init,&f.services,&f.result);break;
        }
        require(status==a::Status::invalid_argument&&f.calls==0&&f.result.calls==99);++cases;
    }
    a::default_init();a::default_post();a::default_final();
    std::cout<<"{\"validation\":\"PASS\",\"host_cases\":"<<cases<<"}\n";
}
