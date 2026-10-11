#include "../player_manager_remove_character_v1.hpp"

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

namespace u = dh2::player_manager_update_v1;
namespace r = dh2::player_manager_remove_character_v1;
namespace p = dh2::player_info_record_v1;

struct Fixture {
    std::vector<int> trace;
    int fail_stage{};
    u::State* manager{};
    bool count_decrement_visible_before_first_effect{};
    static Fixture& get(void* raw) { return *static_cast<Fixture*>(raw); }
    static int stage(void* raw, int id, std::string& error) {
        auto& f=get(raw); f.trace.push_back(id);
        if(id==1&&f.manager)
            f.count_decrement_visible_before_first_effect=
                f.manager->local_death_count_6c4==0&&
                f.manager->counted_player_character==0;
        if(f.fail_stage==id){error="injected provider failure";return 1;}
        return 0;
    }
    static int position(void* raw,std::uintptr_t,std::string& e){return stage(raw,1,e);}
    static int disable(void* raw,std::uintptr_t,std::string& e){return stage(raw,2,e);}
    static int camera(void* raw,std::uintptr_t,std::string& e){return stage(raw,3,e);}
    static int ai(void* raw,std::uintptr_t,std::string& e){return stage(raw,4,e);}
    static int rename(void* raw,std::uintptr_t,const std::string& old_name,
                      const std::string& new_name,std::string& e){
        if(old_name!="PlayerCharacter_0"||new_name!="PlayerCharacter_0_Deleted_0")
            return 1;
        return stage(raw,5,e);
    }
    static int level(void* raw,std::int32_t remaining,bool local,
                     std::int32_t cls,std::string& e){
        if(remaining!=0||!local||cls==-1)return 1;
        return stage(raw,6,e);
    }
    static int tail(void* raw,p::Record& record,std::uintptr_t identity,
                    std::uint8_t flag,std::string& e){
        if(record.character_660!=identity||flag!=0)return 1;
        auto& f=get(raw);f.trace.push_back(7);record.character_660=0;
        return f.fail_stage==7?(e="injected provider failure",1):0;
    }
    static int hud(void* raw,std::string& e){return stage(raw,8,e);}
    r::Services services(){return {this,position,disable,camera,ai,rename,level,tail,hud};}
};

[[noreturn]] void bad(const char* message){std::cerr<<message<<'\n';std::exit(1);}
void check(bool condition,const char* message){if(!condition)bad(message);}

int main(){
    constexpr std::uintptr_t manager_id=0x1000,character_id=0x2000;
    {
        u::State manager{};p::Record player{};Fixture f;f.manager=&manager;
        check(u::manager_constructed(manager,manager_id),"manager receipt");
        check(u::player_character_added(manager,manager_id,character_id),"add receipt");
        manager.busy=true;player.constructed=true;player.character_660=character_id;
        r::Result result{};std::string error;
        const auto status=r::remove_character(manager,player,f.services(),manager_id,
            character_id,"PlayerCharacter_0",true,true,3,&result,error);
        if(status!=r::Status::complete||result.reached!=r::Stage::complete)
            std::cerr<<"status="<<unsigned(status)<<" stage="<<unsigned(result.reached)
                     <<" err="<<error<<" callbacks="<<result.callbacks<<'\n';
        check(status==r::Status::complete&&result.reached==r::Stage::complete,
              "complete status and final stage");
        check((f.trace==std::vector<int>{1,2,3,4,5,6,7,8}),"source stage order");
        check(f.count_decrement_visible_before_first_effect,
              "source count decrement precedes SetPosition");
        check(player.character_660==0&&manager.local_death_count_6c4==0&&
              manager.counted_player_character==0,"same-owner tails published");
        check(manager.deleted_character_suffix_counter==1,"suffix counter advanced once");
        player.constructed=false;
    }
    {
        u::State manager{};p::Record player{};Fixture f;f.manager=&manager;
        check(u::manager_constructed(manager,manager_id),"failure manager receipt");
        check(u::player_character_added(manager,manager_id,character_id),"failure add receipt");
        manager.busy=true;player.constructed=true;player.character_660=character_id;
        f.fail_stage=4;r::Result result{};std::string error;
        const auto status=r::remove_character(manager,player,f.services(),manager_id,
            character_id,"PlayerCharacter_0",true,true,3,&result,error);
        check(status==r::Status::service_failed&&result.reached==r::Stage::clear_ai_ooi_aggro,
              "provider failure reports exact prefix");
        check((f.trace==std::vector<int>{1,2,3,4}),"failure stops at exact source stage");
        check(player.character_660==character_id&&manager.local_death_count_6c4==0&&
              manager.counted_player_character==0&&
              manager.deleted_character_suffix_counter==0,
              "failure preserves source prefix count decrement and untouched later tail");
        check(f.count_decrement_visible_before_first_effect,
              "failed removal still decrements count before SetPosition");
        player.constructed=false;
    }
    {
        u::State manager{};p::Record player{};Fixture f;
        check(u::manager_constructed(manager,manager_id),"identity manager receipt");
        check(u::player_character_added(manager,manager_id,character_id),"identity add receipt");
        manager.busy=true;player.constructed=true;player.character_660=0x3000;
        r::Result result{};std::string error;
        const auto status=r::remove_character(manager,player,f.services(),manager_id,
            character_id,"PlayerCharacter_0",true,true,3,&result,error);
        check(status==r::Status::wrong_character&&result.callbacks==0&&f.trace.empty(),
              "mismatched PlayerInfo Character identity fails before side effects");
        check(player.character_660==0x3000&&manager.local_death_count_6c4==1&&
              manager.counted_player_character==character_id&&
              manager.deleted_character_suffix_counter==0,
              "wrong identity preserves PlayerInfo, count, and deletion counter");
        player.constructed=false;
    }
    std::cout<<"player_manager_remove_character_v1_host: 3 cases passed\n";
}
