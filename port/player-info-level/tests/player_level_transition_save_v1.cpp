#include "../player_level_transition_save_v1.hpp"

#include <cstdlib>
#include <iostream>
#include <vector>

namespace {
using namespace dh2::player_level_transition_save_v1;
void check(bool value,const char* message){if(!value){std::cerr<<"FAIL "<<message<<'\n';std::exit(1);}}
struct Fixture {
    bool active{true},fail_persist{};
    std::vector<std::string> calls;
    static Fixture& self(void* p){return *static_cast<Fixture*>(p);}
    static bool is_active(void* p,std::uintptr_t c,bool& out,std::string&){
        auto& f=self(p);f.calls.push_back("active:"+std::to_string(c));out=f.active;return true;
    }
    static bool level(void* p,std::uintptr_t c,std::int32_t& out,std::string&){
        auto& f=self(p);f.calls.push_back("level:"+std::to_string(c));out=27;return true;
    }
    static bool entry(void* p,std::uintptr_t l,std::int32_t& out,std::string&){
        auto& f=self(p);f.calls.push_back("entry:"+std::to_string(l));out=9;return true;
    }
    static bool difficulty(void* p,std::int32_t& out,std::string&){
        self(p).calls.push_back("difficulty");out=2;return true;
    }
    static bool time(void* p,std::uint32_t& out,std::string&){
        self(p).calls.push_back("time");out=1700000000;return true;
    }
    static bool persist(void* p,dh2::data::PlayerSavegameV1& save,std::string& error){
        auto& f=self(p);f.calls.push_back("persist:"+std::to_string(reinterpret_cast<std::uintptr_t>(&save)));
        if(f.fail_persist){error="writer failed";return false;}return true;
    }
    Services services(){return {this,is_active,level,entry,difficulty,time,persist};}
};
}

int main(){
    using namespace dh2::data;
    std::string error;
    PlayerSavegameV1 save;save.set_character(0x1001);save.set_slot(3);
    save.set_source_save_blocked(true);
    Fixture f;
    Owner owner(save,0x1001,0x2001,f.services());
    check(owner.save_player(true,error),"force-unblocked SG_SavePlayer");
    check(error.empty()&&save.source_save_blocked()&&save.level()==27&&
          save.save_date()==1700000000&&save.level_entry_points()[2]==9&&
          owner.reached_phase()==Phase::complete,"mutations persist through one Save and restore source block");
    check(f.calls==std::vector<std::string>{"active:4097","level:4097","time",
          "entry:8193","difficulty","persist:"+std::to_string(reinterpret_cast<std::uintptr_t>(&save))},
          "source SG_SavePlayer order and canonical Save identity");

    Fixture inactive;inactive.active=false;
    const auto old_level=save.level();
    const auto old_date=save.save_date();
    const auto old_entry=save.level_entry_points()[2];
    Owner inactive_owner(save,0x1001,0x2001,inactive.services());
    check(inactive_owner.save_player(true,error)&&inactive.calls.size()==1&&
          save.source_save_blocked()&&save.level()==old_level&&save.save_date()==old_date&&
          save.level_entry_points()[2]==old_entry,"inactive Character preserves source no-op");

    Fixture failed;failed.fail_persist=true;save.set_source_save_blocked(true);
    Owner failed_owner(save,0x1001,0x2001,failed.services());
    check(!failed_owner.save_player(true,error)&&error=="writer failed"&&
          save.source_save_blocked()&&failed_owner.reached_phase()==Phase::restore_block,
          "writer failure retains mutation prefix but restores Character block state");

    Owner wrong_owner(save,0x9999,0x2001,f.services());
    check(!wrong_owner.save_player(false,error)&&
          error.find("same live Character")!=std::string::npos,
          "foreign Character cannot mutate the canonical Save");
    std::cout<<"PASS player_level_transition_save_v1 checks=4\n";
}
