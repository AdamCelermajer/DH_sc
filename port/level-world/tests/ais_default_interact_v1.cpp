#include "../ais_default_interact_v1.hpp"

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

namespace {
using namespace dh2::ais_default_interact_v1;
struct Fixture {
    std::vector<std::string> calls;
    bool character{};
    bool monster{};
    bool merchant{};
    std::uint8_t retain{};
};
bool is_character(void* raw, std::uintptr_t id, bool& value,
                  std::string& error) {
    auto& f=*static_cast<Fixture*>(raw);
    if(id!=0x300){error="wrong GameObject identity";return false;}
    f.calls.emplace_back("is_character");value=f.character;return true;
}
bool is_monster(void* raw, std::uintptr_t id, bool& value,
                std::string& error) {
    auto& f=*static_cast<Fixture*>(raw);
    if(id!=0x300){error="wrong Character identity";return false;}
    f.calls.emplace_back("is_monster");value=f.monster;return true;
}
bool interact(void* raw, std::uintptr_t target, std::uintptr_t owner,
              std::uint8_t& retain, std::string& error) {
    auto& f=*static_cast<Fixture*>(raw);
    if(target!=0x300||owner!=0x200){error="wrong Interact ABI identities";return false;}
    f.calls.emplace_back("interact");retain=f.retain;return true;
}
bool is_merchant(void* raw, std::uintptr_t id, bool& value,
                 std::string& error) {
    auto& f=*static_cast<Fixture*>(raw);
    if(id!=0x300){error="wrong merchant identity";return false;}
    f.calls.emplace_back("is_merchant");value=f.merchant;return true;
}
bool clear_target(void* raw, std::uintptr_t ai, std::string& error) {
    auto& f=*static_cast<Fixture*>(raw);
    if(ai!=0x100){error="wrong CharAI identity";return false;}
    f.calls.emplace_back("clear");return true;
}
bool sync_last_target(void* raw, std::uintptr_t ai, std::string& error) {
    auto& f=*static_cast<Fixture*>(raw);
    if(ai!=0x100){error="wrong CharAI identity";return false;}
    f.calls.emplace_back("sync");return true;
}
void require(bool value,const char* message) {
    if(!value){std::cerr<<message<<'\n';std::exit(1);}
}
}

int main() {
    const Request request{0x100,0x200,0x300};
    Fixture fixture{};
    const Services services{&fixture,is_character,is_monster,interact,
                            is_merchant,clear_target,sync_last_target};
    Result result{};std::string error;

    fixture.character=true;fixture.monster=true;
    require(on_interact(request,services,result,error)==Status::complete&&
            fixture.calls==std::vector<std::string>{"is_character","is_monster"}&&
            !result.interaction_dispatched,
            "AISDefault did not return before interacting with a Monster target");

    fixture={};fixture.character=true;fixture.retain=1;
    require(on_interact(request,services,result,error)==Status::complete&&
            fixture.calls==std::vector<std::string>{"is_character","is_monster","interact"}&&
            result.interaction_dispatched&&!result.target_cleared,
            "AISDefault did not retain a +0x2fa-marked target");

    fixture={};fixture.character=true;fixture.merchant=true;
    require(on_interact(request,services,result,error)==Status::complete&&
            fixture.calls==std::vector<std::string>{"is_character","is_monster",
              "interact","is_character","is_merchant"}&&
            result.interaction_dispatched&&!result.target_cleared,
            "AISDefault did not retain an unmarked Character merchant");

    fixture={};fixture.character=false;
    require(on_interact(request,services,result,error)==Status::complete&&
            fixture.calls==std::vector<std::string>{"is_character","interact",
              "is_character","clear","sync"}&&
            result.interaction_dispatched&&result.target_cleared&&
            result.last_target_synchronized,
            "AISDefault changed GameObject Interact or clear/sync source order");
    std::cout<<"AISDefault::OnInteract: typed dispatch and clear/sync order verified\n";
}
