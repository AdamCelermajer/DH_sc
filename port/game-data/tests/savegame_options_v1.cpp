#include "../savegame_options_v1.hpp"
#include <cassert>
#include <iostream>
#include <string>
namespace k=dh2::data::savegame_options_v1;
int main(int argc,char** argv){
    k::Owner owner;k::Application app{reinterpret_cast<std::uintptr_t>(&app),&owner};
    if(argc==5){
        const auto present=std::stoi(argv[1]);k::Definition row{std::stoi(argv[2]),std::stoi(argv[3])};
        if(present)assert(owner.deliver_record("GOD_MANA",&row,std::stoi(argv[4]))==k::Status::complete);
        bool has=false,toggle=false,saved=false;std::int32_t value=3,saved_value=3;
        assert(owner.has_option("GOD_MANA",&has)==k::Status::complete);
        assert(owner.get_option("GOD_MANA",&value)==k::Status::complete);
        assert(owner.is_option_toggled("GOD_MANA",&toggle)==k::Status::complete);
        assert(k::get_saved_option(&app,"GOD_MANA",&saved_value)==k::Status::complete);
        assert(k::is_saved_option_on(&app,"GOD_MANA",&saved)==k::Status::complete);
        std::cout<<"["<<has<<","<<value<<","<<toggle<<","<<saved_value<<","<<saved<<"]\n";return 0;
    }
    unsigned cases=0;bool value=true;std::int32_t word=8;
    assert(owner.size()==0&&owner.has_option("missing",&value)==k::Status::complete&&!value);++cases;
    assert(owner.get_option("missing",&word)==k::Status::complete&&word==-1);++cases;
    assert(k::get_saved_option(&app,"missing",&word)==k::Status::complete&&word==0);++cases;
    assert(k::is_saved_option_on(&app,"missing",&value)==k::Status::complete&&!value);++cases;
    const k::Definition zero{0,0},nonzero{7,0},other{7,1};
    assert(owner.deliver_record("zero",&zero,0)==k::Status::complete);
    assert(owner.is_option_toggled("zero",&value)==k::Status::complete&&value);++cases;
    assert(owner.deliver_record("GOD_MANA",&nonzero,7)==k::Status::complete);
    assert(k::is_saved_option_on(&app,"GOD_MANA",&value)==k::Status::complete&&value);++cases;
    assert(owner.deliver_record("GOD_MANA",&nonzero,1)==k::Status::complete);
    assert(k::is_saved_option_on(&app,"GOD_MANA",&value)==k::Status::complete&&!value);++cases;
    assert(owner.deliver_record("GOD_MANA",&other,7)==k::Status::complete);
    assert(k::is_saved_option_on(&app,"GOD_MANA",&value)==k::Status::complete&&!value);++cases;
    const char key[]{'z','e','r','o',0,'x',0};
    assert(owner.has_option(key,&value)==k::Status::complete&&value);++cases;
    value=true;assert(owner.has_option(nullptr,&value)==k::Status::invalid_argument&&value);++cases;
    assert(k::get_saved_option(nullptr,"zero",&word)==k::Status::invalid_argument&&word==0);++cases;
    assert(owner.deliver_record("bad",nullptr,1)==k::Status::invalid_argument&&owner.size()==2);++cases;
    std::cout<<"{\"validation\":\"PASS\",\"host_cases\":"<<cases<<",\"empty_source_map\":true}\n";
}
