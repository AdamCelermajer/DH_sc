#include "../application_pause_save_v1.hpp"
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>

namespace {
void check(bool condition,const char* message){if(!condition){std::cerr<<message<<'\n';std::exit(1);}}
}
int main(){
 auto level=std::make_shared<int>(7);int calls=0;std::string error;
 auto save=[&](const std::shared_ptr<void>& actual,std::int32_t player,bool block,std::string&){
  ++calls;check(actual.get()==level.get(),"pause save must preserve the same current Level owner");
  check(player==0,"pause save must target PlayerManager index zero");
  check(!block,"pause save must pass the original false block argument");return true;
 };
 check(dh2::application::application_pause_save_player_v1({},38,1,save,error),"missing Level is a guarded no-op");
 check(dh2::application::application_pause_save_player_v1(level,37,1,save,error),"non-gameplay phase is a guarded no-op");
 check(dh2::application::application_pause_save_player_v1(level,38,0,save,error),"inactive Level byte is a guarded no-op");
 check(calls==0,"guarded paths must not call SG_SavePlayer");
 check(dh2::application::application_pause_save_player_v1(level,38,1,save,error),"active gameplay pause must save");
 check(calls==1,"eligible pause must invoke exactly one player save");
 auto failed=[&](const std::shared_ptr<void>&,std::int32_t,bool,std::string& e){e="save failure";return false;};
 check(!dh2::application::application_pause_save_player_v1(level,38,1,failed,error)&&error=="save failure",
  "save failure must propagate to the lifecycle caller");
 std::cout<<"application_pause_save_v1: 8 checks passed\n";
}
