#include "../fscommand_continue_v1.hpp"
#include <cstdlib>
#include <iostream>
#include <string>

namespace {
void check(bool value,const char* message){if(!value){std::cerr<<"FAIL: "<<message<<'\n';std::exit(1);}}
struct Fixture {int slot=-1;bool provider_ok=true;std::string provider_error="";int queued=-1;int fallback=0;std::string fallback_arg;};
bool resolve(void* raw,std::int32_t& slot,std::string& error){
 auto& f=*static_cast<Fixture*>(raw);if(!f.provider_ok){error=f.provider_error;return false;}slot=f.slot;return true;
}
bool queue(void* raw,std::int32_t slot,std::string&){static_cast<Fixture*>(raw)->queued=slot;return true;}
bool fallback(void* raw,const char* arg,std::string&){auto& f=*static_cast<Fixture*>(raw);++f.fallback;f.fallback_arg=arg;return true;}
}
int main(){
 std::string error;
 Fixture missing;dh2::ui::FsContinueGameServicesV1 services{&missing,resolve,queue,fallback};
 check(dh2::ui::fscommand_continue_game_v1("ContinueGame","main",services,error),"missing last slot uses StartGame fallback");
 check(missing.fallback==1&&missing.fallback_arg=="main"&&missing.queued==-1,"fallback preserves FSCommand argument and queues no resume");
 Fixture valid;valid.slot=7;services.context=&valid;
 check(dh2::ui::fscommand_continue_game_v1("ContinueGame","ignored",services,error),"valid last slot queues resume");
 check(valid.queued==7&&valid.fallback==0,"canonical last slot is queued once");
 Fixture corrupt;corrupt.provider_ok=false;corrupt.provider_error="saved profile data invalid";services.context=&corrupt;
 check(!dh2::ui::fscommand_continue_game_v1("ContinueGame","",services,error),"bad save-data resolution fails");
 check(error=="saved profile data invalid"&&corrupt.queued==-1&&corrupt.fallback==0,"bad save data never queues or falls back");
 Fixture other;services.context=&other;
 check(dh2::ui::fscommand_continue_game_v1("Other","",services,error)&&other.queued==-1&&other.fallback==0,"unrelated FSCommand ignored");
 std::cout<<"PASS: 8 ContinueGame routing checks\n";
}
