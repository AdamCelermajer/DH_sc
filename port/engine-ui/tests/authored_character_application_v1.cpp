#include "../authored_character_application_v1.hpp"
#include <iostream>
#include <stdexcept>
using namespace dh2::ui;
void check(bool yes,const char* why){if(!yes)throw std::runtime_error(why);}
struct Fixture {std::vector<HudStartupOperation> trace;bool reject{},performance{};
 static int service(void* p,HudStartupState48*,const HudStartupRequest40* q,HudStartupResponse16* r){auto& f=*static_cast<Fixture*>(p);f.trace.push_back(q->operation);if(f.reject)return -1;if(q->operation==HudStartupOperation::get_language)r->value=0;if(q->operation==HudStartupOperation::is_high_performance)r->value=f.performance;return 0;}
};
int main(){try{
 auto fixture=std::make_shared<Fixture>();HudStartupState48 state{1,2,0,99,0,0,0,0};
 AuthoredCharacterApplicationV1 application(fixture,state,{fixture.get(),Fixture::service});CharacterMenuCallV1 call;std::string error;
 check(application.dispatch("NativeLoadSettings",call,error),"source load wrapper");
 check(fixture->trace==std::vector<HudStartupOperation>{HudStartupOperation::load_settings,HudStartupOperation::update_saved_values,HudStartupOperation::get_language,HudStartupOperation::set_language},"whole original no-sound branch order");
 check(state.result==99&&call.result.kind==0,"result restoration/load result untouched");
 fixture->trace.clear();fixture->performance=true;check(application.dispatch("NativeIsMultiplayerEnabled",call,error)&&call.result.kind==1&&call.result.boolean,"actual source device result to AS transport");
 check(fixture->trace==std::vector<HudStartupOperation>{HudStartupOperation::is_high_performance}&&state.result==99,"result service owned directly and restored");
 state.sharp_devices=1;fixture->trace.clear();check(application.dispatch("NativeIsMultiplayerEnabled",call,error)&&fixture->trace.empty()&&call.result.boolean,"source Sharp branch skips performance query");
 state.sharp_devices=0;fixture->reject=true;call.result=CharacterMenuValueV1::numeric(17);check(!application.dispatch("NativeIsMultiplayerEnabled",call,error)&&call.result.number==17&&state.result==99,"required failure retains result");
 std::cout<<"PASS source startup wrappers/actual AS result transport; settings/device endpoints declared fixtures\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
