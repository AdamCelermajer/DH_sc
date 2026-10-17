#include "../character_animation_marker_v1.hpp"

#include <cassert>
#include <string>

using namespace dh2::character_animation_marker_v1;

struct Fixture {
    std::string action;
    std::string first;
    std::string second;
    std::uintptr_t identity{};
    std::int32_t step{-1};
    Foot foot{Foot::left};
    unsigned calls{};
};

bool script(void* raw, std::uintptr_t identity, std::string_view name,
            std::int32_t step, std::string&) {
    auto& f=*static_cast<Fixture*>(raw); f.action="script"; f.first=name;
    f.identity=identity; f.step=step; ++f.calls; return true;
}
ProviderResult animation(void* raw, std::uintptr_t identity, std::string_view target,
                         std::string_view name, std::string&) {
    auto& f=*static_cast<Fixture*>(raw); f.action="animation"; f.first=target;
    f.second=name; f.identity=identity; ++f.calls; return ProviderResult::dispatched;
}
ProviderResult fx(void* raw, std::uintptr_t identity, std::string_view name, std::string&) {
    auto& f=*static_cast<Fixture*>(raw); f.action="fx"; f.first=name;
    f.identity=identity; ++f.calls; return ProviderResult::dispatched;
}
ProviderResult sfx(void* raw, std::uintptr_t identity, std::string_view name, std::string&) {
    auto& f=*static_cast<Fixture*>(raw); f.action="sfx"; f.first=name;
    f.identity=identity; ++f.calls; return ProviderResult::dispatched;
}
bool footstep(void* raw, std::uintptr_t identity, Foot foot, std::string&) {
    auto& f=*static_cast<Fixture*>(raw); f.action="footstep";
    f.identity=identity; f.foot=foot; ++f.calls; return true;
}

int main() {
    Fixture f;
    Services services{&f,script,animation,fx,sfx,footstep};
    std::string error;
    auto result=dispatch({0x1234,"ev_step_right",7},services,error);
    assert(result.status==Status::complete && result.kind==Kind::footstep);
    assert(f.calls==2 && f.identity==0x1234 && f.step==7 && f.foot==Foot::left);

    f={};
    result=dispatch({0x1234,"ev_step_left",2},services,error);
    assert(result.status==Status::complete && f.calls==2 && f.foot==Foot::right);

    f={};
    result=dispatch({0x1234,"an_camera/shake",3},services,error);
    assert(result.status==Status::complete && result.kind==Kind::object_animation);
    assert(f.calls==1 && f.first=="camera" && f.second=="shake");
    f={};
    result=dispatch({0x1234,"an_camera/multi/segment",3},services,error);
    assert(result.status==Status::complete && f.first=="camera" &&
           f.second=="multi/segment");
    auto object_no_match=[](void*,std::uintptr_t,std::string_view,
                            std::string_view,std::string&) {
        return ProviderResult::source_no_match;
    };
    result=dispatch({0x1234,"an_unregistered/idle",3},
                    Services{&f,script,object_no_match,fx,sfx,footstep},error);
    assert(result.status==Status::complete && error.empty());
    f={};
    result=dispatch({0x1234,"fx_footprint",4},services,error);
    assert(result.status==Status::complete && result.kind==Kind::effect_set &&
           f.calls==1 && f.first=="footprint");
    f={};
    result=dispatch({0x1234,"sfx_step_stone",5},services,error);
    assert(result.status==Status::complete && result.kind==Kind::sound &&
           f.calls==1 && f.first=="step_stone");
    f={};
    auto missing_name=[](void*,std::uintptr_t,std::string_view,std::string&) {
        return ProviderResult::source_no_match;
    };
    result=dispatch({0x1234,"fx_absent",5},
                    Services{&f,script,animation,missing_name,sfx,footstep},error);
    assert(result.status==Status::complete && result.kind==Kind::effect_set &&
           f.calls==0 && error.empty());
    auto no_launch=[](void*,std::uintptr_t,std::string_view,std::string&) {
        return ProviderResult::source_no_launch;
    };
    result=dispatch({0x1234,"fx_pool_full",5},
                    Services{&f,script,animation,no_launch,sfx,footstep},error);
    assert(result.status==Status::complete && result.kind==Kind::effect_set &&
           f.calls==0 && error.empty());
    auto failed_fx=[](void*,std::uintptr_t,std::string_view,std::string& e) {
        e="mapped FX backend failed";return ProviderResult::failed;
    };
    result=dispatch({0x1234,"fx_present",5},
                    Services{&f,script,animation,failed_fx,sfx,footstep},error);
    assert(result.status==Status::dispatch_failed && error=="mapped FX backend failed");

    f={};
    result=dispatch({0x1234,"ev_step_right",0},
                    Services{&f,script,animation,fx,sfx,nullptr},error);
    assert(result.status==Status::missing_service && f.calls==1 &&
           f.action=="script" && !error.empty());
    result=dispatch({0x1234,"an_missing_slash",0},services,error);
    assert(result.status==Status::complete && result.kind==Kind::ignored);
    result=dispatch({0x1234,"step_right",0},services,error);
    assert(result.status==Status::complete && result.kind==Kind::ignored);
    result=dispatch({0,"ev_go",0},services,error);
    assert(result.status==Status::invalid_request);
}
