#include "../menu_avatar_preview_v1.hpp"
#include <fstream>
#include <cstdio>
#include <stdexcept>
using namespace dh2::ui;
static void check(bool v,const char* why){if(!v)throw std::runtime_error(why);}
struct Fixture {
 MenuAvatarPreviewStateV1* state{};std::string events;char reject=0;
 bool record(char event,std::string& error){events+=event+std::to_string(state->slot)+",";if(event==reject){error="rejected";return false;}return true;}
 static bool destroy(void* p,std::string& e){return static_cast<Fixture*>(p)->record('D',e);}
 static bool setup(void* p,int slot,std::string& e){auto& f=*static_cast<Fixture*>(p);check(slot==f.state->slot,"slot unpublished at setup");return f.record('S',e);}
 static bool camera(void* p,std::string& e){return static_cast<Fixture*>(p)->record('C',e);}
};
int main(int argc,char** argv){try{
 check(argc==2,"fixture required");std::ifstream file(argv[1]);MenuAvatarPreviewStateV1 state;Fixture f{&state};
 MenuAvatarPreviewServicesV1 services{&f,Fixture::destroy,Fixture::setup,Fixture::camera};std::string error,expected;int previous,requested,force,final;unsigned count=0;
 while(file>>previous>>requested>>force>>final>>expected){state.slot=previous;f.events.clear();
  check(change_menu_avatar_preview_v1(state,requested,force,services,error),"preview call");
  check(state.slot==final&&(f.events.empty()?"-":f.events)==expected,"original event/publication mismatch");++count;}
 check(count==50,"fixture count");
 for(char reject:{'D','S','C'}){state.slot=2;f.events.clear();f.reject=reject;
  check(!change_menu_avatar_preview_v1(state,3,false,services,error),"failure ignored");
  check(state.slot==(reject=='D'?2:3),"failure slot retention");
  check(f.events==(reject=='D'?"D2,":reject=='S'?"D2,S3,":"D2,S3,C3,"),"failure prefix order");}
 f.reject=0;state.slot=2;f.events.clear();services.setup_character=nullptr;
 check(!change_menu_avatar_preview_v1(state,3,false,services,error)&&state.slot==2&&f.events.empty(),"missing canonical provider mutated state");
 check(change_menu_avatar_preview_v1(state,2,false,services,error)&&f.events.empty(),"same slot needs provider");
 check(!change_menu_avatar_preview_v1(state,4,false,services,error)&&state.slot==2,"unsafe slot accepted");
 std::printf("PASS %u original preview order/publication cases, forced refresh, repeated-slot no-op and failure prefixes\n",count);
}catch(const std::exception& e){std::fprintf(stderr,"FAIL %s\n",e.what());return 1;}}
