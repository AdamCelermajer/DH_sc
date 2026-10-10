#include "source_owner_contract.hpp"
#include <iostream>
#include <stdexcept>
using namespace dh::foundation::platform_input;
void check(bool ok,const char* text) {if(!ok)throw std::runtime_error(text);}
int main() {try {
    SemanticInput input;InputOwnerEpoch epoch;std::string error;
    SourceOwnerBorrow owner;owner.receiver_lease=std::make_shared<int>(1);
    owner.epoch=epoch.value();owner.actor_id=7;owner.character=10;owner.controller=20;
    check(epoch.validate(owner,7,error),error.c_str());
    bool enabled{};std::uint8_t actual198{};
    owner.current_level=[&](dh2::ui::MenuLevelBorrowV58& level,std::string&) {level={owner.receiver_lease,123,&actual198};return true;};
    check(source_level_input(owner,enabled,error)&&!enabled,"Rendered level incorrectly implied enabled input");
    actual198=1;check(source_level_input(owner,enabled,error)&&enabled,"Actual Level198 ignored");
    owner.current_level=[](dh2::ui::MenuLevelBorrowV58& level,std::string&) {level.identity=123;return true;};
    check(!source_level_input(owner,enabled,error),"Non-null unleased Level admitted");
    owner.current_level=[](dh2::ui::MenuLevelBorrowV58&,std::string&) {return true;};
    check(source_level_input(owner,enabled,error)&&enabled,"Genuine nullable Level branch changed");
    input.key(0x20,true);input.key('1',true);input.take_frame();
    auto old=epoch.value();bool old_valid_during_release{};
    check(epoch.before_replace(input,[&](const Frame& f,std::string& e) {
        old_valid_during_release=epoch.validate(owner,7,e);
        return f.attack.released&&!f.attack.held&&f.skills[0].released;
    },error),error.c_str());
    check(old_valid_during_release&&epoch.value()!=old,"Old owner invalidated before release delivery");
    check(!epoch.validate(owner,7,error),"Old owner remained valid after replacement publication");
    owner.epoch=epoch.value();check(epoch.validate(owner,7,error),error.c_str());
    input.key(0x20,true);input.take_frame();
    old=epoch.value();check(!epoch.before_replace(input,[](const Frame&,std::string&){return false;},error)&&epoch.value()==old,"Failed release published replacement epoch");
    check(epoch.before_replace(input,[](const Frame& f,std::string&){return f.attack.released;},error),"Retry lost undelivered old-owner release edge");
    std::cout<<"source_owner_contract_tests PASS: genuine Level198, leases, release-before-replace, expired epoch rejection\n";return 0;
}catch(const std::exception& e) {std::cerr<<e.what()<<'\n';return 1;} }
